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
 *	$Id: rfwrite.c,v 6.6 2003/01/16 15:46:29 brbarret Exp $
 *
 *      Function:       - writes to a file using remote filed
 *      Accepts:        - runtime file descriptor
 *                      - buffer ptr
 *                      - buffer length
 *      Returns:        - 0 or ERROR
 */

#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * global functions
 */
int4			lam_rfwrite();
int4			_lam_rfwrite();

/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */

/*
 * external functions
 */
extern int		_fdvalid();	/* check validity of file desc */


int4
lam_rfwrite(fd, buf, len)

int			fd;
CONST char		*buf;
int4			len;

{
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd)) return((int4) LAMERROR);

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	return(_lam_rfwrite(_ufd[fd].fu_node, _ufd[fd].fu_tfd,
		_ufd[fd].fu_tflags, buf, len));
}

int4
_lam_rfwrite(node, tfd, tflags, buf, len)

int4			node;
int4			tfd;
int4			tflags;
CONST char		*buf;
int4			len;

{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
	int4		tot_len = 0;	/* total length written */
	int4		pkt_len;	/* one packet length */

	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	nhead.nh_node = node;
	nhead.nh_flags = tflags & LAM_O_DMSG;

	nhead.nh_msg = (char *) buf;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
/*
 * Packetize output buffer.
 */
	do {
		request->fq_src_node = getnodeid();
		request->fq_src_event = -lam_getpid();
		request->fq_req = FQWRITE;
		request->fq_tfd = tfd;

		nhead.nh_event = EVFILED;
		nhead.nh_type = 0;
		nhead.nh_length = (len > MAXNMSGLEN) ? MAXNMSGLEN : len;
		pkt_len = nhead.nh_length;
		request->fq_length = pkt_len;

		if (nsend(&nhead)) {
			ksigsetmask(mask);
			return(LAMERROR);
		}

		if (! (tflags & LAM_O_1WAY)) {
			nhead.nh_event = -lam_getpid();
			nhead.nh_type = 0;
			nhead.nh_length = 0;

			if (nrecv(&nhead)) {
				ksigsetmask(mask);
				return(LAMERROR);
			}

			if (reply->fr_errno != 0) {
				ksigsetmask(mask);
				errno = reply->fr_errno;
				return(LAMERROR);
			}

			else if (reply->fr_ret != pkt_len) {
				len = 0;
			}

			else {
				len -= pkt_len;
				nhead.nh_msg += pkt_len;
			}
		} else {
			reply->fr_ret = pkt_len;
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
int4 rfwrite(fd, buf, len) int fd; CONST char *buf; int4 len;
	{ return(lam_rfwrite(fd, buf, len)); }
