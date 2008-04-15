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
 *      $Id: rreq.h,v 6.6 2002/10/09 20:57:26 brbarret Exp $
 *
 *	Function:	- constants and templates associated with the router
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#ifndef _RREQ
#define _RREQ

#include <lam_config.h>
#include <net.h>
#include <portable.h>

/*
 * misc.
 */
#define RNRENT		10		/* # new route enties */

/*
 * router requests
 */
#define RQMAX		7		/* # router requests */
#define RQGETROUTE	0		/* translate node to event */
#define RQSETSMART	1		/* set smartness */
#define RQSETROUTES	2		/* adds route entries */
#define RQGETNODES	3		/* get matching node ID list */
#define RQREMROUTE	4		/* remove one specific node */
#define RQGETSMART	5		/* wait for a smart local router */
#define RQGETNTYPE	6		/* count matching nodes */

/*
 * router request types
 */
#define RDUMB		1		/* dumb router without table */
#define RSMART		2		/* smart router with table */

/*
 * router request template
 */
struct rreq {
	int4		rq_src_event;	/* client's event */
	int4		rq_request;	/* specific router request */
	int4		rq_nodeid;	/* node ID */
	int4		rq_nrents;	/* # route entries */
	int4		rq_event;	/* forwarding event */
	int4		rq_pid;		/* process ID */
	int4		rq_nodetype;	/* node type */
	int4		rq_typemask;	/* type mask */
	int4		rq_ncpus;	/* number of CPUs */
#define rq_smartness	rq_typemask	/* smartness true/false */
};

/*
 * router reply template
 */
struct rreply {
	int4		rr_reply;	/* return code */
	int4		rr_nodetype;	/* node type */
	int4		rr_ncpus;	/* number of CPUs */
	int4		rr_event;	/* forwarding event */
	int4		rr_event2;	/* min-route event */
	int4		rr_link2;	/* min-route link # */
	int4		rr_link;	/* forwarding link # */
	int4		rr_ntype;	/* # of nodes of given type */
};

/*
 * route table entry
 */
struct route {
	int4		r_nodeid;	/* destination node */
	int4		r_event;	/* forward to this event */
	int4		r_link;		/* forward to this link # */
	int4		r_event2;	/* min-route event */
	int4		r_link2;	/* min-route link # */
	int4		r_nodetype;	/* NT bits defined in net.h */

  /* This is very LAM specific, because it assumes that every other
     host will have a link entry.

     This info doesn't really belong here, but it is not worth
     creating a whole new lamd pseudo daemon and index table to get
     the number of CPUs on each host */

	int4		r_ncpus;	/* number of CPUs at the end
					   of this link */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		getnodes __ARGS((int4 *pnodes, int4 nnodes,
					int4 nodetype, int4 typemask));
extern int4		getntype __ARGS((int4 nodetype, int4 typemask));
extern int		getrent __ARGS((struct route *rent));
extern int		getrentc __ARGS((struct route *rent));
extern int		getroute __ARGS((struct nmsg *nhead));
extern int4		getrtype __ARGS((int4 node));
extern int		getsmart __ARGS((void));
extern int		rrsetsmart __ARGS((int4 node, int smartness));
extern int		rremrent __ARGS((int4 node, int4 destnode));
extern int		rrsetrents __ARGS((int4 node, struct route *prents,
					int4 nrents));

#ifdef __cplusplus
}
#endif

#endif
