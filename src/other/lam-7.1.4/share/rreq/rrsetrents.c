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
 *	GDB/RBD
 *
 *      $Id: rrsetrents.c,v 6.5 2002/10/09 21:00:08 brbarret Exp $
 *
 *	Function:	- changes route information in router
 */

#include <lam_config.h>

#include <events.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>

/*
 *	rrsetrents
 *
 *	Function:	- adds route information to router
 *	Accepts:	- node
 *			- route entry array
 *			- array size
 */
int
rrsetrents(node, prents, nrents)

int4			node;
struct route		*prents;
int4			nrents;

{
	struct rreq	*prq;		/* router request */
	struct nmsg	nhq;		/* router message */
/*
 * Set up the router request.
 */
	LAM_ZERO_ME(nhq);
	prq = (struct rreq *) nhq.nh_data;
	prq->rq_request = RQSETROUTES;

	nhq.nh_node = node;
	nhq.nh_event = EVROUTER;
	nhq.nh_type = 0;
	nhq.nh_flags = NOBUF;

	while (nrents > 0) {
		prq->rq_nrents = LAM_min(RNRENT, nrents);
		nhq.nh_length = prq->rq_nrents * sizeof(struct route);
		nhq.nh_msg = (char *) prents;

		if (nsend(&nhq)) return(ERROR);

		nrents -= prq->rq_nrents;
		prents += prq->rq_nrents;
	}

	return(0);
}

/*
 *	rremrent
 *
 *	Function:	- removes route information on one node
 *	Accepts:	- router node
 *			- removed node
 */
int
rremrent(node, remnode)

int4			node;
int4			remnode;

{
	struct rreq	*prq;		/* router request */
	struct nmsg	nhq;		/* router message */
/*
 * Set up the router request.
 */
	prq = (struct rreq *) nhq.nh_data;
	prq->rq_request = RQREMROUTE;
	prq->rq_nodeid = remnode;

	nhq.nh_node = node;
	nhq.nh_event = EVROUTER;
	nhq.nh_type = 0;
	nhq.nh_flags = NOBUF;
	nhq.nh_length = 0;
	nhq.nh_msg = 0;

	return(nsend(&nhq));
}
