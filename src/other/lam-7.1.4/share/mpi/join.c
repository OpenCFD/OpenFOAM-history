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
 *
 * $Id: join.c,v 1.16 2003/10/11 19:15:35 brbarret Exp $
 *
 *	Function:	- join MPI applications
 *	Accepts:	- socket file descriptor
 *			- intercomm between applications (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/time.h>				/* LINUX FD_SET etc. */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <app_mgmt.h>
#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <portable.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <typical.h>


/*
 * private functions
 */
static int exchange_with_peer(int fd, int cid, int *peercid,
                              struct _gps *peergps);
static int readwrite(int fd, int nbytes, char *inbuf, char *outbuf);


/*@
   MPI_Comm_join - Connect two MPI processed joined by a socket

Input Parameter:
. fd - socket file descriptor

Output Parameter:
. newcomm - intercommunicator with client as remote group

Description:

This function only works between two LAM/MPI processes that are
connected by a socket.  They either must have the same endian
orientation, or not have used the homogeneous flag to 'mpirun'(1)
(-O).  Both processes must be in a single LAM universe -- they must
share LAM daemons that are already connected to each other.  That is,
they were either initially lamboot(1)ed together, or a lamgrow(1)
command was given to grow an initial LAM universe such that the
resulting set includes the two hosts in question.

The socket may not be used by the calling application until
'MPI_Comm_join' has completed.  When 'MPI_Comm_join' completes, 'fd'
may be used by the calling application.  Per MPI-2, the socket is only
used for initial handshaking -- it will not be used as the
communications channel between the newly connected processes.

Sample:

The following code shows an abbreviated example using 'MPI_Comm_join'
since no example is provided in the MPI-2 standard.  Note that only
one rank in each MPI application should call this function since each
socket can only have two endpoints (i.e., an endpoint in each
process).  Upon successful completion, the intercommunicator that is
returned will have two members -- the local process and the remote
process.

.vb
  MPI_Comm
  do_join(char* server_name, int port) {
    int sfd, fd = -1;
    unsigned char addr[4];
    MPI_Comm intercomm;

    if (server_name != NULL) {
      if (getinetaddr(server_name, &addr) == 0)
        fd = open_socket_to_server(addr, port);
    } else {
      if ((sfd = open_listening_socket(&port)) >= 0) {
        fd = accept_client_socket(sfd, -1);
        close(sfd);
      }
    }
    if (fd < 0)
      MPI_Abort(MPI_COMM_WORLD, 0);

    MPI_Comm_join(fd, &intercomm);
    return intercomm;
  }
.ve

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INTERN
.N MPI_ERR_OTHER

.seealso lamboot(1), lamgrow(1), mpirun(1), MPI_Finalize(3)

.N WEB
@*/
int
MPI_Comm_join(int fd, MPI_Comm *newcomm)
{
	struct _proc	*p;			/* peer process */
	struct _gps	peergps;		/* peer GPS */
	MPI_Group	peergrp;		/* peer group */
	int		peercid;		/* peer ID */
	int		cid;			/* new context ID */

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMJOIN);
/*
 * Check the arguments.
 */
	if (fd < 0) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_ARG, EINVAL,
			"illegal file descriptor"));
	}

	if (newcomm == 0) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_ARG, EINVAL,
			"null pointer"));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMJOIN));

	cid = lam_getcid();

	if (cid < 0) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_INTERN, EFULL, ""));
	}
/*
 * Exchange cid and GPS with peer.
 */
	if (exchange_with_peer(fd, cid, &peercid, &peergps)) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_OTHER, errno, ""));
	}
/*
 * The intercomm context ID is the maximium of the local and peer CIDs.
 */
	cid = LAM_max(cid, peercid);
/*
 * Create the remote (client) group.
 */
	peergrp = (MPI_Group) 
		malloc(sizeof(struct _group) + sizeof(struct _proc *));
	if (peergrp == 0) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_OTHER, errno, ""));
	}
	peergrp->g_nprocs = 1;
	peergrp->g_myrank = MPI_UNDEFINED;
	peergrp->g_refcount = 1;
	peergrp->g_f77handle = -1;
	peergrp->g_procs = (struct _proc **)
				((char *) peergrp + sizeof(struct _group));

	p = *peergrp->g_procs = lam_procadd(&peergps);
	if (p == 0) {
		free((char *) peergrp);
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_OTHER, errno, ""));
	}
	if (!(p->p_mode & LAM_PRPIINIT)) {
		p->p_mode |= LAM_PCLIENT;
	}
	p->p_refcount++;
/*
 * Create the server-client intercommunicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, MPI_COMM_SELF->c_group,
				peergrp, LAM_CINTER, newcomm)) {

		free((char *) peergrp);
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_OTHER, errno, ""));
	}

	MPI_COMM_SELF->c_group->g_refcount++;
	(*newcomm)->c_errhdl = MPI_COMM_SELF->c_errhdl;
	MPI_COMM_SELF->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_INTERN, errno, ""));
	}

	if (lam_tr_comm(*newcomm)) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_INTERN, errno, ""));
	}

	lam_setcid(cid);
/*
 * Setup the new processes.
 */
	if (RPI_ADDPROCS(peergrp->g_procs, peergrp->g_nprocs)) {
		return(lam_err_comm(MPI_COMM_SELF, MPI_ERR_OTHER, errno, ""));
	}

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(MPI_COMM_SELF, BLKMPICOMMJOIN,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMJOIN, 0, 0, 0, 0));

        lam_resetfunc_m(BLKMPICOMMJOIN);
	return(MPI_SUCCESS);
}


/*
 *	exchange_with_peer
 *
 *	Function:	- exchange information with peer over a socket
 *	Accepts:	- socket file descriptor
 *			- local cid
 *			- peer cid (out)
 *			- peer GPS (out)
 *	Returns:	- 0 or LAMERROR
 */
static int
exchange_with_peer(int fd, int cid, int *peercid, struct _gps *peergps)
{
	struct _gps	mygps;
	int4		cid4;
	int4		peercid4;

	mygps = lam_myproc->p_gps;
	mltoti4(&mygps, sizeof(struct _gps) / sizeof(int4));
	cid4 = (int4) cid;
	mltoti4(&cid4, 1);

	if (readwrite(fd, sizeof(struct _gps),
			(char *) peergps, (char *) &mygps)) {
		return(LAMERROR);
	}

	if (readwrite(fd, sizeof(int4),
			(char *) &peercid4, (char *) &cid4)) {
		return(LAMERROR);
	}

	mttoli4(peergps, sizeof(struct _gps) / sizeof(int4));
	mttoli4(&peercid4, 1);
	*peercid = (int) peercid4;
	return(0);
}


/*
 *	readwrite
 *
 *	Function:	- exchange bytes over a socket
 *	Accepts:	- socket file descriptor
 *			- number of bytes to exchange
 *			- incoming data buffer
 *			- outgoing data buffer (filled)
 *	Returns:	- 0 or LAMERROR
 */
static int
readwrite(int fd, int nbytes, char *inbuf, char *outbuf)
{
	fd_set		rfd;
	fd_set		wfd;
	int		ntowrite;		/* # bytes left to write */
	int		ntoread;		/* # bytes left to read */
	int		nready;			/* # ready file descriptors */
	int		r;

	FD_ZERO(&rfd);
	FD_ZERO(&wfd);
	ntoread = nbytes;
	ntowrite = nbytes;
/*
 * Loop writing and reading until all is done.  Write first to avoid
 * deadlock.
 */
	while (ntowrite > 0 || ntoread > 0) {

		if (ntowrite > 0) {
			FD_SET(fd, &wfd);
		} else {
			FD_CLR(fd, &wfd);
		}

		if (ntoread > 0) {
			FD_SET(fd, &rfd);
		} else {
			FD_CLR(fd, &rfd);
		}

		do {
			nready = select(fd + 1, &rfd, &wfd,
					(fd_set *) 0, (struct timeval *) 0);

			if (nready < 0 && errno != EINTR) {
				return(LAMERROR);
			}
		} while (nready < 0);

		if (FD_ISSET(fd, &wfd)) {
			r = write(fd, outbuf, ntowrite);

			if (r < 0) {
				if (errno != EINTR) {
					return(LAMERROR);
				}	
			} else if (r == 0) {
				errno = EEOF;
			} else {
				ntowrite -= r;
				outbuf += r;
			}
		}

		if (FD_ISSET(fd, &rfd)) {
			r = read(fd, inbuf, ntoread);

			if (r < 0) {
				if (errno != EINTR) {
					return(LAMERROR);
				}	
			} else if (r == 0) {
				errno = EEOF;
			} else {
				ntoread -= r;
				inbuf += r;
			}
		}
	}

	return(0);
}
