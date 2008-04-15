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
 *	Copyright 1994 The Ohio State University
 *	GDB
 *
 *	$Id: rf77.c,v 6.7 2003/11/02 19:30:51 brbarret Exp $
 *
 *	Function:	- functions to support F77 I/O
 */

#include <errno.h>
#include <unistd.h>

#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */

/*
 * external functions
 */
extern int		_fdvalid(int);	/* check validity of file desc */

/*
 *	rf77back
 *
 *	Function:	- move file position ptr backwards one record
 *			- assumes an F77 sequential file
 *	Accepts:	- file descriptor handle
 *			- formatted or unformatted indicator
 *	Returns:	- 0 or ERROR
 */
int
lam_rf77back(int fd, char fmt)
{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd) == ERROR) {
		return((int4) ERROR);
	}

	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQF77BACK;
	request->fq_tfd = _ufd[fd].fu_tfd;
	request->fq_fmt = fmt;

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	nhead.nh_node = _ufd[fd].fu_node;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nsend(&nhead)) {
		return((int4) ERROR);
	}

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
	nhead.nh_event = -lam_getpid();

	if (nrecv(&nhead)) {
		return((int4) ERROR);
	}

	if (reply->fr_errno != 0) {
		errno = reply->fr_errno;
		return((int4) ERROR);
	}

	ksigsetmask(mask);

	return((int) reply->fr_ret);
}

/*
 *	rf77read
 *
 *	Function:	- read a record from a file
 *			- assumes an F77 sequential file
 *	Accepts:	- file descriptor handle
 *			- ptr to input buffer
 *			- length of input buffer
 *			- formatted or unformatted indicator
 *	Returns:	- actual length of record in file
 */
int4
lam_rf77read(int fd, char *buf, int4 len, char fmt)
{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
	int4		tot_len = 0;	/* total length read */
/*
 * Do simple error checking.
 */
	if (_fdvalid(fd) == ERROR) {
		return((int4) ERROR);
	}

	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQF77READ;
	request->fq_tfd = _ufd[fd].fu_tfd;
	request->fq_length = len;
	request->fq_fmt = fmt;

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	nhead.nh_node = _ufd[fd].fu_node;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nsend(&nhead)) {
		return((int4) ERROR);
	}

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
/*
 * Receive input buffer packets.
 * Each packet contains the length of the total input data.
 */
	nhead.nh_msg = buf;
	nhead.nh_event = -lam_getpid();

	do {
		nhead.nh_type = 0;
		nhead.nh_length = MAXNMSGLEN;

		if (nrecv(&nhead)) {
			return((int4) ERROR);
		}

		if (reply->fr_errno != 0) {
			errno = reply->fr_errno;
			return((int4) ERROR);
		}

		tot_len += nhead.nh_length;
	} while (tot_len < reply->fr_ret);

	ksigsetmask(mask);

	return(tot_len);
}
