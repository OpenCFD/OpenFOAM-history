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
 *	$Id: rrsetsmart.c,v 6.4 2002/10/09 21:00:08 brbarret Exp $
 * 
 *	Function:	- sets smartness mode in router
 */

#include <lam_config.h>

#include <events.h>
#include <net.h>
#include <rreq.h>

int
rrsetsmart(node, smartness)

int4			node;
int4			smartness;

{
	struct rreq	*prq;		/* router request */
	struct nmsg	nhq;		/* router message */
/*
 * Set up the router request.
 */
	LAM_ZERO_ME(nhq);
	prq = (struct rreq *) nhq.nh_data;
	prq->rq_request = RQSETSMART;
	prq->rq_smartness = smartness;

	nhq.nh_node = node;
	nhq.nh_event = EVROUTER;
	nhq.nh_type = 0;
	nhq.nh_flags = NOBUF;
	nhq.nh_length = 0;
	nhq.nh_msg = 0;

	return(nsend(&nhq));
}
