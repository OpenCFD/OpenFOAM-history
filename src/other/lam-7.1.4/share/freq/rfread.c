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
 *	GDB
 *
 *	$Id: rfread.c,v 6.7 2003/11/02 19:30:51 brbarret Exp $
 *
 *	Function:	- reads from a file using remote filed
 *	Accepts:	- runtime file descriptor
 *			- buffer ptr
 *			- buffer length
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
extern int		_fdvalid(int);	/* check validity of file desc */

int4
lam_rfread(fd, buf, len)

int			fd;
char			*buf;
int4			len;

{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
	int4		tot_len = 0;	/* total length written */
	int4		pkt_len;	/* one packet length */
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd)) return(LAMERROR);

	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQREAD;
	request->fq_tfd = _ufd[fd].fu_tfd;
	request->fq_length = len;

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
/*
 * Receive input buffer packets.
 */
	nhead.nh_msg = buf;
	nhead.nh_event = -lam_getpid();

	do {
		nhead.nh_type = 0;
		nhead.nh_length = (len > MAXNMSGLEN) ? MAXNMSGLEN : len;
		pkt_len = nhead.nh_length;

		if (nrecv(&nhead)) {
			ksigsetmask(mask);
			return(LAMERROR);
		}

		if (reply->fr_errno != 0) {
			errno = reply->fr_errno;
			ksigsetmask(mask);
			return(LAMERROR);
		}

		else if (reply->fr_ret != pkt_len) {
			len = 0;
		}

		else {
			len -= pkt_len;
			nhead.nh_msg += pkt_len;
		}

		tot_len += reply->fr_ret;
	} while (len > 0);

	ksigsetmask(mask);
	return(tot_len);
}

/*
 * backwards compatibility
 */
int4
rfread(int fd, char *buf, int4 len)
{
  return(lam_rfread(fd, buf, len));
}
