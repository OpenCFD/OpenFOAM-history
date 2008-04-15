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
 *	$Id: lioattach.c,v 1.6 2003/02/16 17:06:15 brbarret Exp $
 *
 *	Function:	- attach stream to io daemon
 *	Accepts:	- stream
 *			- remote IO node
 *			- remote LAM file descriptor
 *			- buffering mode (one of _IOFBF, _IOLBF or _IONBF)
 *	Returns:	- 0 or LAMERROR
 */

#include <lam_config.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sfh.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <laminternal.h>
#include <sfh.h>


int
liosattach(FILE *stream, int4 node, int4 tfd, int buffering)
{
	int		iofd;			/* socket to IO daemon */
	char		*iosock;		/* IO daemon server sock name */
	int4		data[3];		/* forwarding info */
/*
 * Create a socket connected to IO daemon.
 */
	iosock = lam_get_iosockname();
	if (iosock == 0) return(LAMERROR);

	iofd = sfh_sock_open_clt_unix_stm(iosock);
	free(iosock);
	if (iofd < 0) return(LAMERROR);
/*
 * Pass forwarding information to IO daemon.
 */
	data[0] = node;
	data[1] = tfd;
	data[2] = 0;

	if (mwrite(iofd, (char *) data, sizeof(data)) != sizeof(data)) {
		close(iofd);
		return(LAMERROR);
	}
/*
 * Redirect the stream to the socket to IO daemon and set the 
 * line buffering discipline.
 */
	if (freopen("/dev/null", "a", stream) == NULL) {
		close(iofd);
		return(LAMERROR);
	}

	if (setvbuf(stream, NULL, buffering, BUFSIZ)) {
		return(LAMERROR);
	}

	if (dup2(iofd, fileno(stream)) < 0) {
		close(iofd);
		return(LAMERROR);
	}

	return(0);
}

#if LAM_HAVE_FD_PASSING

int
liofdattach(int fd, int4 node, int4 tfd)
{
	int		iofd;		/* socket to IO daemon */
	char		*iosock;	/* IO daemon server sock name */
	int4		data[3];	/* forwarding info */
/*
 * Create a socket connected to IO daemon.
 */
	iosock = lam_get_iosockname();
	if (iosock == 0) 
	  return(LAMERROR);

	iofd = sfh_sock_open_clt_unix_stm(iosock);
	free(iosock);
	if (iofd < 0)
	  return(LAMERROR);
/*
 * Pass forwarding information to IO daemon.
 */
	data[0] = node;
	data[1] = tfd;
	data[2] = 1;

	if (mwrite(iofd, (char *) data, sizeof(data)) != sizeof(data)) {
		close(iofd);
		return(LAMERROR);
	}

	if (sfh_send_fd(iofd, fd)) {
		close(iofd);
		return(LAMERROR);
	}

	close(iofd);
	return(0);
}

#else

int
liofdattach(int fd, int4 node, int4 tfd)
{
    errno = ENOPFDSUPPORT;
    return(LAMERROR);
}

#endif
