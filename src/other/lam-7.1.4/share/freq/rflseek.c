/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	GDB
 *
 *	$Id: rflseek.c,v 6.6 2002/10/09 20:57:14 brbarret Exp $
 *
 *	Function:	- seeks on a file using remote filed
 *	Accepts:	- runtime file descriptor
 *			- seek offset
 *			- type of seek operation
 *	Returns:	- 0 or ERROR
 */

#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <typical.h>
#include <etc_misc.h>

#include <errno.h>
#include <unistd.h>

/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */

/*
 * external functions
 */
extern int		_fdvalid();	/* check validity of file desc */

int4
lam_rflseek(fd, offset, whence)

int			fd;
int4			offset;
int			whence;

{
	struct nmsg	nhead;		/* network message desc. */
	struct freq  	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd)) return(LAMERROR);

	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQLSEEK;
	request->fq_tfd = _ufd[fd].fu_tfd;
	request->fq_offset = offset;
	request->fq_whence = (int4) whence;

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	nhead.nh_node = _ufd[fd].fu_node;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	nhead.nh_event = -lam_getpid();

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	if (reply->fr_errno != 0) {
		errno = reply->fr_errno;
		ksigsetmask(mask);
		return(LAMERROR);
	}

	ksigsetmask(mask);
	return(reply->fr_ret);
}

/*
 * backwards compatibility
 */
int4 rflseek(fd, offset, whence) int fd, whence; int4 offset;
	{ return(lam_rflseek(fd, offset, whence)); }
