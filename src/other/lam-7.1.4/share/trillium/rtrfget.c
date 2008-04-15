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
 *	Copyright 1994 The Ohio State University
 *	GDB
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
 *	lam_rtrfget
 *
 *	Function:	- reads trace data from remote node
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- file descriptor
 *	Returns:	- total length of received trace data
 */
int4
lam_rtrfget(nodeid, lnum, pid, fd)

int4			nodeid;
int4			lnum;
int4			pid;
int			fd;

{
	return(rtr(nodeid, lnum, pid, fd, TRQGET));
}

/*
 *	lam_rtrfforget
 *
 *	Function:	- destructively reads trace data from remote node
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- file descriptor
 *	Returns:	- total length of received trace data
 */
int4
lam_rtrfforget(nodeid, lnum, pid, fd)

int4			nodeid;
int4			lnum;
int4			pid;
int			fd;

{
	return(rtr(nodeid, lnum, pid, fd, TRQFORGET));
}

/*
 *	rtr
 *
 *	Function:	- unloads trace data from remote node
 *			- writes trace data into file
 *	Accepts:	- target node ID
 *			- list number
 *			- process ID
 *			- file descriptor
 *			- traced request code
 *	Returns:	- total length of received trace data
 */
static int4
rtr(nodeid, lnum, pid, fd, req)

int4			nodeid;
int4			lnum;
int4			pid;
int4			req;
int			fd;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */
	struct trreply	*reply;		/* traced reply */
	int4		trtotal;	/* total matching traces */
	int4		trleft;		/* traces left to receive */
	int4		mask;		/* favourite index */

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
		return(LAMERROR);
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
		return(LAMERROR);
	}

	trtotal = reply->trr_length;
	trleft = reply->trr_length;

	if (trtotal > 0) {
		nhead.nh_msg = malloc((unsigned) MAXNMSGLEN);

		if (nhead.nh_msg == 0) {
			ksigsetmask(mask);
			return(LAMERROR);
		}
	}

	while ((trleft > 0) && (reply->trr_errno == 0)) {
		nhead.nh_type = 0;
		nhead.nh_length = MAXNMSGLEN;

		if (nrecv(&nhead)) {
			free(nhead.nh_msg);
			ksigsetmask(mask);
			return(LAMERROR);
		}

		if (write(fd, nhead.nh_msg, nhead.nh_length) < 0) {
			free(nhead.nh_msg);
			ksigsetmask(mask);
			return(LAMERROR);
		}

		trleft -= nhead.nh_length;
	}

	ksigsetmask(mask);

	if (trtotal > 0) {
		free(nhead.nh_msg);
	}

	if (reply->trr_errno) {
		errno = reply->trr_errno;
		return(LAMERROR);
	} else {
		return(trtotal);
	}
}
