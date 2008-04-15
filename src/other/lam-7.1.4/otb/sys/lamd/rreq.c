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
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	$Log: rreq.c,v $
 *	Revision 6.3  2002/10/09 20:56:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:37  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:45:21  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:32:15  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/26  14:04:19  gdburns
 * Remove getsmart().
 * 
 * Revision 5.2  94/08/22  13:51:17  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:42:49  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:28:20  gdburns
 * Ohio Release
 * 
 *	Function:	- router functions for internal processes
 */

#include <memory.h>

#include <events.h>
#include <net.h>
#include <portable.h>
#include <rreq.h>
#include <typical.h>
#include <terror.h>

/*
 * external functions
 */
extern struct nmsg	*rqgetnodes();
extern struct nmsg	*rqgetntype();
extern struct nmsg	*rqgetrent();

/*
 *	getroute
 *
 *	Function:	- fills route information into message desc.
 *	Accepts:	- message desc. ptr
 *	Returns:	- 0 or ERROR
 */
int
getroute(nhead)

struct nmsg		*nhead;		/* network message header */

{
	struct rreq	rq;		/* router request */
	struct rreply	*rr;		/* router reply */
	struct nmsg	*rrhead;	/* router reply message */
/*
 * Local node, forward on the given event.
 */
	if (nhead->nh_node == LOCAL) {
		nhead->nh_dl_event = nhead->nh_event;
	} else {
/*
 * Get a route table entry.
 * Set up the router request.
 */
		rq.rq_request = RQGETROUTE;
		rq.rq_nodeid = nhead->nh_node;

		rrhead = rqgetrent(&rq);
		rr = (struct rreply *) rrhead->nh_data;

		if (rr->rr_reply) {
			errno = rr->rr_reply;
			return(LAMERROR);
		}
/*
 * This must be my own node.  Forward on the given event.
 */
		else if (rr->rr_event == NOTEVENT) {
			nhead->nh_dl_event = nhead->nh_event;
		}
/*
 * Set the forwarding event and link from the route table entry.
 */
		else {
			nhead->nh_dl_event = rr->rr_event;
			nhead->nh_dl_link = rr->rr_link;
		}
	}

	return(0);
}

/*
 *	getntype
 *
 *	Function:	- counts type matching node IDs
 *	Accepts:	- type requirement mask
 *			- type interest mask
 *	Returns:	- # matching node IDs or ERROR
 */
int4
getntype(nodetype, typemask)

int4			nodetype;
int4			typemask;

{
	struct rreq	rq;		/* router request */
	struct rreply	*rr;		/* router reply */
	struct nmsg	*rrhead;	/* router reply message */
/*
 * Set up the router request.
 */
	rq.rq_request = RQGETNTYPE;
	rq.rq_nodetype = nodetype;
	rq.rq_typemask = typemask;

	rrhead = rqgetntype(&rq);
	rr = (struct rreply *) rrhead->nh_data;

	if (rr->rr_reply) {
		errno = rr->rr_reply;
		return(LAMERROR);
	} else {
		return(rr->rr_ntype);
	}
}

/*
 *	getnodes
 *
 *	Function:	- obtains type matching node IDs
 *	Accepts:	- node ID array
 *			- node ID array # elements
 *			- type requirement mask
 *			- type interest mask
 *	Returns:	- 0 or ERROR
 */
int
getnodes(pnodes, nnodes, nodetype, typemask)

int4			*pnodes;
int4			nnodes;
int4			nodetype;
int4			typemask;

{
	struct rreq	rq;		/* router request */
	struct rreply	*rr;		/* router reply */
	struct nmsg	*rrhead;	/* router reply message */
/*
 * Set up the router request.
 */
	rq.rq_request = RQGETNODES;
	rq.rq_nodetype = nodetype;
	rq.rq_typemask = typemask;

	rrhead = rqgetnodes(&rq);
	rr = (struct rreply *) rrhead->nh_data;

	if (rr->rr_reply) {
		errno = rr->rr_reply;
		return(LAMERROR);
	} else {
		nnodes *= sizeof(int4);
		memcpy((char *) pnodes, rrhead->nh_msg,
				nnodes > rrhead->nh_length ?
				rrhead->nh_length : nnodes);
		return(0);
	}
}

/*
 *	getrtype
 *
 *	Accepts:	- node ID
 *	Returns:	- node type or ERROR
 */
int4
getrtype(nodeid)

int4			nodeid;

{
	struct rreq	rq;		/* router request */
	struct rreply	*rr;		/* router reply */
	struct nmsg	*rrhead;	/* router reply message */
/*
 * Get a route table entry.
 * Set up the router request.
 */
	rq.rq_request = RQGETROUTE;
	rq.rq_nodeid = nodeid;

	rrhead = rqgetrent(&rq);
	rr = (struct rreply *) rrhead->nh_data;

	if (rr->rr_reply) {
		errno = rr->rr_reply;
		return(ERROR);
	} else {
		return(rr->rr_nodetype);
	}
}
