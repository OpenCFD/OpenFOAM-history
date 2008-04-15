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
 * $Id: rfclose.c,v 6.7 2003/04/10 17:50:47 jsquyres Exp $
 *
 *	Function:	- closes a file using remote filed
 *	Accepts:	- runtime file descriptor
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
extern int		_fdvalid(int fd); /* check validity of file desc */


int
lam_rfclose(int fd)
{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd)) 
          return(LAMERROR);

        LAM_ZERO_ME(nhead);
	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQCLOSE;
	request->fq_tfd = _ufd[fd].fu_tfd;

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

	ksigsetmask(mask);

	if (reply->fr_errno != 0) {
		errno = reply->fr_errno;
		return(LAMERROR);
	} else {
		_ufd[fd].fu_tflags = 0;
		return(0);
	}
}


/*
 * backwards compatibility
 */
int rfclose(int fd)
{ 
  return(lam_rfclose(fd)); 
}
