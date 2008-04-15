/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: iod.c,v 6.10 2003/10/11 19:15:34 brbarret Exp $
 *
 *	Function:	- IO server daemon (iod)
 *
 *			- Iod opens and listens on a IO server socket.
 *
 * 			- Clients requiring the forwarding of a output stream,
 *			  connect to iod via this socket, write forwarding 
 *			  information (remote node and LAM file descriptor) 
 *			  to iod via the socket and then redirect the stream
 *			  to the socket. This is all done via the library
 *			  function liosattach().
 *
 *			- Iod after accepting a client connection, reads the
 *			  forwarding information, adds it to a table and
 *			  re-enters the server loop.
 *
 *			- The server loop continually reads data from connected
 *			  clients and forwards it.  Client disconnects are
 *			  detected as an EOF on the connection, the forwarding 
 *			  information is removed from the table, and a close is
 *			  done on the destination LAM file.
 */

#include "lam_config.h"
#include "sfh.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include "events.h"
#include "freq.h"
#include "laminternal.h"
#include "net.h"
#include "terror.h"
#include "typical.h"
#include "t_types.h"

/*
 * public functions
 */
void			(*(io_init()))();
void			io_forward();
void			io_new();

/*
 * external functions
 */
extern void		(*(iod()))();
extern void		io_accept();
extern void		io_register();
extern void		io_deregister();
extern int		lpattach();
extern int		mread();
extern int		sfh_sock_open_srv_unix_stm();
extern int		sfh_sock_accept_tmout();

/*
 * private functions
 */
static void		io_rfclose();

/*
 * private variables
 */
struct fdentry {
	int4		fe_node;		/* remote node */
	int4		fe_tfd;			/* LAM file descriptor */
};

static struct nmsg	reply;			/* reply from filed */
static struct fdentry	fdmap[FD_SETSIZE];	/* map file desc. to LAM IO */
static char		iobuf[MAXNMSGLEN];	/* IO buffer */

/*
 *	io_init
 *
 *	Function:	- iod initialization
 */
void (*(
io_init()))()

{
	int		i;
	int		fd;			/* IO server file desc. */
	char		*iosname;		/* IO server socket name */
/*
 * Attach to kernel.
 */
	if (lpattach("iod")) lampanic("iod (lpattach)");
/*
 * Set up server socket.
 */
	iosname = lam_get_iosockname();
	if (iosname == 0) lampanic("iod (killname)");

	if ((fd = sfh_sock_open_srv_unix_stm(iosname)) < 0) {
		lampanic("iod (sfh_sock_open_srv_unix_stm)");
	}
	
	free(iosname);

	if (fcntl(fd, F_SETFD, 1) == -1) {
	    lampanic("iod (fcntl)");
	}
/*
 * Initialize forwarding table.
 */
	for (i = 0; i < FD_SETSIZE; i++) {
		fdmap[i].fe_node = NOTNODEID;
	}
/*
 * Accept client connections.
 */
	io_accept(fd);

	return(iod());
}

/*
 *	io_new
 *
 *	Function:	- make connection with new client
 *	Accepts:	- IO server file desc. 
 */
void
io_new(fd_io)

int			fd_io;

{
	int4		finfo[3];		/* forwarding info */
	int		newfd;			/* new connection file desc. */
	int		pfd;			/* passed filed desc. */
	int		nread;
/*
 * Make connection with client.
 */
	if ((newfd = sfh_sock_accept_tmout(fd_io, -1)) < 0) {
		return;
	}
	
	if (fcntl(newfd, F_SETFD, 1) == -1) {
	    lampanic("iod (fcntl)");
	}
/*	
 * Read forwarding information from the client.
 */
	nread = mread(newfd, (char *) finfo, sizeof(finfo));
	if (nread != sizeof(finfo)) {
		close(newfd);
		return;
	}

	if (finfo[2]) {
		pfd = sfh_recv_fd(newfd);
		close(newfd);
		if (pfd < 0)
			return;
		newfd = pfd;
	}
/*
 * Add forwarding information to table.
 */
	fdmap[newfd].fe_node = finfo[0];
	fdmap[newfd].fe_tfd = finfo[1];
/*
 * Accept input from client.
 */
	io_register(newfd);
}

/*
 *	io_forward
 *
 *	Function:	- read and forward client print data
 *	Accepts:	- client file descriptor
 */
void
io_forward(fd)

int			fd;

{
	int		nread;

	while (((nread = read(fd, iobuf, MAXNMSGLEN)) < 0) && (errno == EINTR));

	if (nread > 0) {
/*
 * Forward data if the mapping is valid.  Invalid mappings should never occur
 * and are ignored.  
 */
		if (fdmap[fd].fe_node != NOTNODEID) {
			_lam_rfwrite(fdmap[fd].fe_node, fdmap[fd].fe_tfd,
					LAM_O_1WAY, iobuf, (int4) nread);
		}
	} else {
/*
 * Error or EOF on socket, close LAM file descriptor and close connection to
 * client.  
 */
		io_rfclose(fd);
		io_deregister(fd);
		fdmap[fd].fe_node = NOTNODEID;
		close(fd);
	}
}

/*
 *	io_rfclose
 *
 *	Function:	- close LAM file descriptor
 *			- errors are ignored
 *	Accepts:	- file descriptor
 */
static void
io_rfclose(fd)

int			fd;

{
	struct nmsg	nhead;			/* network message desc. */
	struct freq	*request;		/* filed request */
/*
 * error check
 */
	if (fdmap[fd].fe_node == NOTNODEID) {
		return;
	}
/*
 * Don't close standard descriptors.
 */
	if (fdmap[fd].fe_tfd <= 2) {
		return;
	}
/*
 * Setup and make request to filed.
 */
	LAM_ZERO_ME(nhead);
	request = (struct freq *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = EVIOD;
	request->fq_req = FQCLOSE;
	request->fq_tfd = fdmap[fd].fe_tfd;

	nhead.nh_node = fdmap[fd].fe_node;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nsend(&nhead)) {
		return;
	}
/*
 * Receive filed reply.  It is not acted upon.
 */
	reply.nh_event = EVIOD;
	reply.nh_type = 0;
	reply.nh_flags = 0;
	reply.nh_length = 0;
	reply.nh_msg = 0;

	nrecv(&reply);
}
