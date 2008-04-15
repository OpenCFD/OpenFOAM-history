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
 *	GDB/RBD
 *
 *	$Id: router.c,v 1.4 2002/10/09 20:56:51 brbarret Exp $
 * 
 *	Function:	- route daemon
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <all_hash.h>
#include <events.h>
#include <net.h>
#include <priority.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>

/*
 * external functions
 */
extern int4		rtdinit();
extern int4		next_prime();

/*
 * global functions
 */
void			(*(r_init()))();
void			(*(router()))();
struct nmsg		*rqgetnodes();
struct nmsg		*rqgetntype();
struct nmsg		*rqgetrent();
struct nmsg		*rqgetsmart();

/*
 * local functions
 */
static int4		count_ntype();
static struct nmsg	*rqremrent();
static struct nmsg	*rqsetrents();
static struct nmsg	*rqsetsmart();

/*
 * local variables
 */
static struct nmsg	from;		/* request message */
static struct nmsg	to;		/* reply message */
static int4		*pnodes = 0;	/* node ID array */
static int4		smartness;	/* router req filter */

static struct nmsg	*(*rqfunc[RQMAX])() = {
				rqgetrent,
				rqsetsmart,
				rqsetrents,
				rqgetnodes,
				rqremrent,
				rqgetsmart,
				rqgetntype,
			};

static HASH		*rtd;		/* route table hash descriptor */

/*
 * global variables
 */
struct route		rentnew[RNRENT];/* input route table entries */

/*
 *	r_init
 *
 *	Function:	- router initialization
 */
void (*(
r_init()))()

{
/*
 * Initialize route table.
 */
	smartness = rtdinit(&rtd);
/*
 * Receive first request.
 */
	LAM_ZERO_ME(from);
	memset((void*) rentnew, -1, sizeof(struct route) * RNRENT);
	from.nh_event = EVROUTER;
	from.nh_type = smartness;
	from.nh_flags = NOBUF;
	from.nh_length = sizeof(struct route) * RNRENT;
	from.nh_msg = (char *) rentnew;

	if (nrecv(&from)) lampanic("router (nrecv)");

	return((void (*)()) router);
}

/*
 *	router
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(router()))()

{
	struct rreq	*prq;		/* router request */
	struct nmsg	*pto = 0;	/* reply message */

	prq = (struct rreq *) from.nh_data;

	if ((prq->rq_request >= RQMAX) || (prq->rq_request < 0)) {
		errno = EBADREQUEST;
		lampanic("router");
	} else {
		pto = (*(rqfunc[prq->rq_request]))(prq);
	}

	if (pto) {
		pto->nh_dl_event = prq->rq_src_event;
		pto->nh_node = LOCAL;
		pto->nh_event = prq->rq_src_event;
		pto->nh_type = 0;
		pto->nh_flags = DINT4MSG;

		if (dtry_send(pto)) {

			if (errno != EWOULDBLOCK) {
				lampanic("router (dsend)");
			}
		}
	}
/*
 * Receive next request.
 */
	from.nh_event = EVROUTER;
	from.nh_type = smartness;
	from.nh_flags = NOBUF;
	from.nh_length = sizeof(struct route) * RNRENT;
	from.nh_msg = (char *) rentnew;

	if (nrecv(&from)) lampanic("router (nrecv)");

	return((void (*)()) router);
}

/*
 *	rqgetrent
 *
 *	Function:	- obtains route entry for given node ID
 */
struct nmsg *
rqgetrent(prq)

struct rreq		*prq;

{
	struct route	*p;		/* favourite pointer */
	struct rreply	*rr;		/* router reply */
/*
 * Get the node entry in the route table.
 */
	p = (struct route *) ah_find(rtd, prq->rq_nodeid);
	rr = (struct rreply *) to.nh_data;

	if (p == 0) {
		rr->rr_reply = EBADNODE;
	} else {
		rr->rr_reply = 0;
		rr->rr_nodetype = p->r_nodetype;
		rr->rr_ncpus = p->r_ncpus;
		rr->rr_event = p->r_event;
		rr->rr_link = p->r_link;
		rr->rr_event2 = p->r_event2;
		rr->rr_link2 = p->r_link2;
	}

	to.nh_length = 0;
	return(&to);
}

/*
 *	rqsetsmart
 *
 *	Function:	- sets the smartness filter
 */
static struct nmsg *
rqsetsmart(prq)

struct rreq		*prq;

{
	smartness = prq->rq_smartness;
	return(0);
}

/*
 *	rqsetrents
 *
 *	Function:	- adds or modifies route table entries
 */
static struct nmsg *
rqsetrents(prq)

struct rreq		*prq;

{
	struct route	*prent;		/* existing route */
	int4		i;

	for (i = 0; i < prq->rq_nrents; ++i) {

	    if (rentnew[i].r_nodeid == NOTNODEID) {
		continue;
	    }

	    prent = (struct route *) ah_find(rtd, rentnew[i].r_nodeid);
/*
 * If node entry is not defined; insert it.
 */
	    if (prent == 0) {

		if (ah_insert(rtd, (void *) &rentnew[i])) {

		    if (errno == EFULL) {

                        if (ah_expand(rtd, 1 + next_prime(ah_count(rtd))))
                            lampanic("router (ah_expand)");

                        if (ah_insert(rtd, (void *) &rentnew[i]))
                            lampanic("router (ah_insert)");
		    }
		    else {
			lampanic("router (ah_insert)");
		    }
		}
	    }
/*
 * If node entry is defined; modify it.
 */
	    else {
		prent->r_nodetype = rentnew[i].r_nodetype;
		prent->r_ncpus = rentnew[i].r_ncpus;
		prent->r_event = rentnew[i].r_event;
		prent->r_link = rentnew[i].r_link;
		prent->r_event2 = rentnew[i].r_event2;
		prent->r_link2 = rentnew[i].r_link2;
	    }
	}

	return(0);
}

/*
 *	rqgetnodes
 *
 *	Function:	- delivers an array of matching node IDs
 */
struct nmsg *
rqgetnodes(prq)

struct rreq		*prq;

{
	int4		*p;
	int4		nnodes;		/* length of node list */
	int4		nodetype;	/* requested node type */
	int4		typemask;	/* nodetype bit mask */
	struct route	*prent;		/* ptr to route entry */
	struct rreply	*rr;		/* router reply */
/*
 * Allocate the array of nodeids needed.
 */
	rr = (struct rreply *) to.nh_data;
	rr->rr_reply = 0;
	typemask = prq->rq_typemask;
	nodetype = prq->rq_nodetype;
	nnodes = count_ntype(nodetype, typemask) * sizeof(int4);

	if (nnodes == 0) {
		to.nh_length = 0;
		return(&to);
	}

	if (pnodes) {
		free((char *) pnodes);
	}

	p = pnodes = (int4 *) malloc((unsigned) nnodes);
	if (p == 0) lampanic("router (malloc)");

	nodetype &= typemask;
/*
 * Loop through the route table looking for the matching nodeids.
 */
	prent = (struct route *) ah_top(rtd);

	while (prent) {

		if ((prent->r_nodetype & typemask) == nodetype) {
			*p++ = prent->r_nodeid;
		}

		prent = (struct route *) ah_next(rtd, (void *) prent);
	}

	to.nh_length = nnodes;
	to.nh_msg = (char *) pnodes;
	return(&to);
}

/*
 *	rqremrent
 *
 *	Function:	- removes a single route table entry
 */
static struct nmsg *
rqremrent(prq)

struct rreq		*prq;

{
	ah_delete(rtd, prq->rq_nodeid);
	return(0);
}

/*
 *	rqgetsmart
 *
 *	Function:	- This request is filtered with a smart bit
 *			  in the type field, so I must be smart.
 */
struct nmsg *
rqgetsmart()

{
	return(0);
}

/*
 *	rqgetntype
 *
 *	Function:	- obtains # of nodes matching given type
 */
struct nmsg *
rqgetntype(prq)

struct rreq		*prq;

{
	struct rreply	*rr;		/* router reply */

	rr = (struct rreply *) to.nh_data;
	rr->rr_reply = 0;
	rr->rr_ntype = count_ntype(prq->rq_nodetype, prq->rq_typemask);
	to.nh_length = 0;
	return(&to);
}

/*
 *	count_ntype
 *
 *	Function:	- counts type matching nodes
 *	Accepts:	- nodetype bit pattern
 *			- valid mask pattern for the nodetype
 *	Returns:	- # matching nodes or 0 if no route table
 */
static int4
count_ntype(nodetype, typemask)

int4			nodetype;
int4			typemask;

{
	int4		numnodes;		/* # nodes that match */
	struct route	*p;			/* favourite pointer */
/*
 * Loop through the route table counting the matching nodeids.
 */
	numnodes = 0;
	nodetype &= typemask;
	p = (struct route *) ah_top(rtd);

	while (p) {

		if ((p->r_nodetype & typemask) == nodetype) {
			numnodes++;
		}

		p = (struct route *) ah_next(rtd, (void *) p);
	}

	return(numnodes);
}
