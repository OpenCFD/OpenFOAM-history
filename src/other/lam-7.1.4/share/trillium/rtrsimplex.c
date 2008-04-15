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
 *	$Id: rtrsimplex.c,v 6.4 2002/10/09 21:00:48 brbarret Exp $
 *
 *	Function:	- simple traced requests
 */

#include <events.h>
#include <net.h>
#include <trreq.h>

/*
 *	lam_rtrsweep
 *
 *	Function:	- sweeps out traced's stored trace data
 *			- no reply from traced
 *	Accepts:	- target node ID
 *			- list number
 *	Returns:	- return value of nsend()
 */
int
lam_rtrsweep(nodeid, lnum)

int4			nodeid;
int4			lnum;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */

	request = (struct trreq *) nhead.nh_data;
	request->trq_req = TRQSWEEP;
	request->trq_lnum = lnum;
 	request->trq_pid = TRANYPID;
	
	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(nsend(&nhead));
}

/*
 *	lam_rtrudie
 *
 *	Function:	- removes pid related information
 *			- no reply from traced
 *	Accepts:	- target node ID
 *			- pid
 *	Returns:	- return value of nsend()
 */
int
lam_rtrudie(nodeid, pid)

int4			nodeid;
int4			pid;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */

	request = (struct trreq *) nhead.nh_data;
	request->trq_req = TRQUDIE;
	request->trq_pid = pid;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(nsend(&nhead));
}

/*
 *	lam_rtrwipe
 *
 *	Function:	- resets traced
 *			- no reply from traced
 *	Accepts:	- target node ID
 *	Returns:	- return value of nsend()
 */
int
lam_rtrwipe(nodeid)

int4			nodeid;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */

	request = (struct trreq *) nhead.nh_data;
	request->trq_req = TRQWIPE;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(nsend(&nhead));
}

/*
 *	lam_rtrcleanobjs
 *
 *	Function:	- makes traced clean up registered objects
 *			- no reply from traced
 *	Accepts:	- target node ID
 *	Returns:	- return value of nsend()
 */
int
lam_rtrcleanobjs(nodeid)

int4			nodeid;

{
	struct nmsg	nhead;		/* network message desc. */
	struct trreq  	*request;	/* traced request */

	request = (struct trreq *) nhead.nh_data;
	request->trq_req = TRQCLEANOBJS;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(nsend(&nhead));
}
