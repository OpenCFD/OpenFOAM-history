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
 *
 * $Id: rtrstore.c,v 6.4 2002/12/14 04:04:56 jsquyres Exp $
 * 
 *	Function:	- stores trace data with traced
 *			- generates 1 to N store requests, each less
 *			  than MAXNMSGLEN
 *
 *	Accepts:	- target node ID (should be LOCAL)
 *			- list number
 *			- process ID
 *			- ptr to trace data buffer
 *			- buffer length
 *
 *	Returns:	- return value of nsend()
 */

#include <events.h>
#include <net.h>
#include <portable.h>
#include <trreq.h>
#include <typical.h>


int
lam_rtrstore(int4 nodeid, int4 lnum, int4 pid, const char *buffer, int4 length)
{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq	*request;	/* traced request */

	LAM_ZERO_ME(nhead);
	request = (struct trreq *) nhead.nh_data;

	request->trq_req = TRQSTORE;
	request->trq_pid = pid;
	request->trq_lnum = lnum;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_msg = (char *) buffer;

	while (length > 0) {
		nhead.nh_length = (length > MAXNMSGLEN) ? MAXNMSGLEN : length;
		request->trq_length = nhead.nh_length;

		if (nsend(&nhead)) return(LAMERROR);

		length -= nhead.nh_length;
		buffer += nhead.nh_length;
	}

	return(0);
}
