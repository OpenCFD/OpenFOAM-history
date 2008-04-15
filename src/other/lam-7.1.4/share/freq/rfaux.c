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
 * $Id: rfaux.c,v 6.9 2003/05/21 19:17:02 jsquyres Exp $
 *
 *	Function:	- useful auxiliary functions for the file runtime
 *			  library
 */

#include <ctype.h>
#include <errno.h>
#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <typical.h>
#include <etc_misc.h>

#include <stdlib.h>
#include <unistd.h>


/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */


/*
 * global functions
 */
int _makefreq(int4 node, int req, int4 param, char *ibuf, int4 ilen, 
              char *obuf, int4 olen);
char * _fnparse(char *xname, int4 *node);
int _fdvalid(int fd);


/*
 *	makefreq
 *
 *	Function:	- simple request to filed
 *
 *	Accepts:	- node ID of filed
 *			- request code
 *			- generic parameter
 *			- ptr to input buffer
 *			- length of input buffer
 *			- ptr to output buffer
 *			- length of output buffer
 *
 *	Returns:	- reply code from filed
 */
int
_makefreq(int4 node, int req, int4 param, char *ibuf, int4 ilen, char *obuf,
	  int4 olen)
{
	struct nmsg	nhead;		/* network message desc. */
	struct freq  	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */


	LAM_ZERO_ME(nhead);
	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;
/*
 * Set up the filed request.
 */
	request->fq_src_node = (node == LOCAL) ? LOCAL : getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = (int4) req;
	request->fq_param0 = param;
/*
 * Set up the request message descriptor.
 */
	nhead.nh_node = node;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = ilen;
	nhead.nh_msg = ibuf;

	if (nsend(&nhead)) {
		return(ERROR);
	}

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
/*
 * Set up the reply message descriptor.
 */
	nhead.nh_event = -lam_getpid();
	nhead.nh_length = olen;
	nhead.nh_msg = obuf;

	if (nrecv(&nhead)) {
		return(ERROR);
	}

	(void) ksigsetmask(mask);

	if (reply->fr_errno != 0) {
		errno = reply->fr_errno;
	}

	return(reply->fr_ret);
}


/*
 *	fnparse
 *
 *	Function:	- parse extended file name for server node
 *			  and local file name
 *
 *	Accepts:	- extened file name
 *			- ptr to server node ID (value returned)
 *
 *	Returns:	- ptr to local file name
 */
char *
_fnparse(char *xname, int4 *node)
{
	char		*cp;		/* search char ptr */

	for (cp = xname; 
	     (*cp != '\0') && isdigit((int) *cp); ++cp) {
	}

	if ((*cp == ':') && (cp != xname)) {
		*node = atoi4(xname);
		xname = cp + 1;
	}

	else {
		*node = getorigin();
	}

	return(xname);
}


/*
 *	fdvalid
 *
 *	Function:	- check validity of user file descriptor handle
 *
 *	Accepts:	- handle
 *
 *	Returns:	- 0 or ERROR
 */
int
_fdvalid(int fd)
{
	if ((fd < 0) || (fd > FUMAX) || (_ufd[fd].fu_tflags == 0)) {
		errno = EBADF;
		return(ERROR);
	} else {
		return(0);
	}
}
