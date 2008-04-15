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
 * $Id: getnodes.c,v 6.9 2002/12/12 22:55:38 jsquyres Exp $
 * 
 *	Function:	- multiple node route matching
 */

#include <lam_config.h>
#include <etc_misc.h>
#include <events.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>

#include <errno.h>
#include <unistd.h>


/*
 *	getnodes
 *
 *	Function:	- gets matching node IDs from local router
 *	Accepts: 	- node array
 *			- # array elements
 *			- node type
 *			- node type mask
 *	Returns:	- 0 or ERROR
 */
int
getnodes(int4 *pnodes, int4 nnodes, int4 nodetype, int4 typemask)
{
	struct rreq	*request;	/* router request */
	struct rreply	*reply;		/* router reply */
	struct nmsg	nhreq;		/* request message */
	struct nmsg	nhreply;	/* reply message */

	LAM_ZERO_ME(nhreq);
	LAM_ZERO_ME(nhreply);
/*
 * Set up the router request.
 */
	request = (struct rreq *) nhreq.nh_data;
	request->rq_src_event = -lam_getpid();
	request->rq_request = RQGETNODES;
	request->rq_pid = lam_getpid();		/* for sa_rtr */
	request->rq_nodetype = nodetype;
	request->rq_typemask = typemask;

	nhreq.nh_dl_event = EVROUTER;
	nhreq.nh_node = LOCAL;
	nhreq.nh_event = EVROUTER;
	nhreq.nh_type = RSMART;
	nhreq.nh_flags = 0;
	nhreq.nh_length = 0;
	nhreq.nh_msg = 0;

	nhreply.nh_event = -lam_getpid();
	nhreply.nh_type = 0;
	nhreply.nh_flags = 0;
	nhreply.nh_length = nnodes * sizeof(int);
	nhreply.nh_msg = (char *) pnodes;

	if (dsfr(&nhreq, &nhreply)) {
		return(ERROR);
	}

	reply = (struct rreply *) nhreply.nh_data;

	if (reply->rr_reply) {
		errno = reply->rr_reply;
		return(ERROR);
	} else {
		return(0);
	}
}


/*
 *	getntype
 *
 *	Returns:	- # of nodes with types satisfying the
 *			  given type in all bits that are set
 *			  in the given mask
 *	Accepts:	- nodetype bitfield value
 *			- nodetype bitfield mask
 */
int4
getntype(int4 node_type, int4 type_mask)
{
	struct rreq	*request;	/* router request */
	struct rreply	*reply;		/* router reply */
	struct nmsg	nhreq;		/* request message */
	struct nmsg	nhreply;	/* reply message */

	LAM_ZERO_ME(nhreq);
	LAM_ZERO_ME(nhreply);
/*
 * Set up the router request.
 */
	request = (struct rreq *) nhreq.nh_data;
	request->rq_src_event = -lam_getpid();
	request->rq_request = RQGETNTYPE;
	request->rq_nodetype = node_type;
	request->rq_typemask = type_mask;

	nhreq.nh_dl_event = EVROUTER;
	nhreq.nh_node = LOCAL;
	nhreq.nh_event = EVROUTER;
	nhreq.nh_type = RSMART;
	nhreq.nh_flags = 0;
	nhreq.nh_length = 0;
	nhreq.nh_msg = 0;

	nhreply.nh_event = -lam_getpid();
	nhreply.nh_type = 0;
	nhreply.nh_flags = 0;
	nhreply.nh_length = 0;
	nhreply.nh_msg = 0;

	if (dsfr(&nhreq, &nhreply)) {
		return(ERROR);
	}

	reply = (struct rreply *) nhreply.nh_data;

	if (reply->rr_reply) {
		errno = reply->rr_reply;
		return(ERROR);
	} else {
		return(reply->rr_ntype);
	}
}
