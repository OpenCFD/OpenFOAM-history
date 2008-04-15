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
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: rtrget.c,v 6.5 2002/10/09 21:00:48 brbarret Exp $
 *
 *	Function:	- unloads trace data from remote trace daemon
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <trreq.h>
#include <typical.h>

/*
 * local functions
 */
static int4		rtr();

/*
 *	lam_rtrget
 *
 *	Function:	- reads trace data from remote node
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- buffer ptr ptr
 *	Returns:	- total length of received trace data
 */
int4
lam_rtrget(nodeid, lnum, pid, buf)

int4			nodeid;
int4			lnum;
int4			pid;
char			**buf;

{
	return(rtr(nodeid, lnum, pid, buf, TRQGET));
}

/*
 *	lam_rtrforget
 *
 *	Function:	- destructively reads trace data from remote node
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- buffer ptr ptr
 *	Returns:	- total length of received trace data
 */
int4
lam_rtrforget(nodeid, lnum, pid, buf)

int4			nodeid;
int4			lnum;
int4			pid;
char			**buf;

{
	return(rtr(nodeid, lnum, pid, buf, TRQFORGET));
}

/*
 *	rtr
 *
 *	Function:	- unloads trace data from remote node
 *			- writes trace data into malloc'd buffer
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- trace buffer ptr ptr (value returned)
 *			- traced request code
 *	Returns:	- total length of received trace data
 */
static int4
rtr(nodeid, lnum, pid, buf, req)

int4			nodeid;
int4			lnum;
int4			pid;
int4			req;
char			**buf;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */
	struct trreply	*reply;		/* traced reply */
	int4		trtotal;	/* total matching traces */
	int4		trleft;		/* traces left to receive */
	int4		mask;		/* signal mask */

	request = (struct trreq *) nhead.nh_data;
	reply = (struct trreply *) nhead.nh_data;

	request->trq_src_node = (nodeid == LOCAL) ? nodeid : getnodeid();
	request->trq_src_event = -lam_getpid();
	request->trq_req = req;
	request->trq_lnum = lnum;
	request->trq_pid = pid;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}
/*
 * Receive total trace length.
 */
	nhead.nh_event = -lam_getpid();
	nhead.nh_type = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	trtotal = reply->trr_length;
	trleft = reply->trr_length;

	*buf = malloc((unsigned) trtotal);

	if (*buf == 0) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_msg = *buf;

	while ((trleft > 0) && (reply->trr_errno == 0)) {
		nhead.nh_type = 0;
		nhead.nh_length = MAXNMSGLEN;

		if (nrecv(&nhead)) {
			free(*buf);
			ksigsetmask(mask);
			return(ERROR);
		}

		trleft -= nhead.nh_length;
		nhead.nh_msg += nhead.nh_length;
	}

	ksigsetmask(mask);

	if (reply->trr_errno) {
		errno = reply->trr_errno;
		free(*buf);
		return(ERROR);
	} else {
		return(trtotal);
	}
}
