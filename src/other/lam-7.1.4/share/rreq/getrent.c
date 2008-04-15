/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: getrent.c,v 6.9 2003/03/25 00:26:13 jsquyres Exp $
 *
 *	Function:	- obtains routing information
 */

#include <lam_config.h>

#include <all_hash.h>
#include <etc_misc.h>
#include <events.h>
#include <kio.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>


/*
 * external variables
 */
extern struct kio_t	_kio;


/*
 *	getrent
 *
 *	Functions:	- obtains one route entry from the router
 *			- nodeid must be set in route entry
 *	Accepts:	- route entry ptr
 *	Returns:	- 0 or ERROR
 */
int
getrent(struct route *rent)
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
	request->rq_request = RQGETROUTE;
	request->rq_nodeid = rent->r_nodeid;

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

	if (dsfr(&nhreq, &nhreply)) return(LAMERROR);

	reply = (struct rreply *) nhreply.nh_data;

	if (reply->rr_reply) {
		errno = reply->rr_reply;
		return(LAMERROR);
	} else {
		rent->r_nodetype = reply->rr_nodetype;
		rent->r_ncpus = reply->rr_ncpus;
		rent->r_event = reply->rr_event;
		rent->r_link = reply->rr_link;
		rent->r_event2 = reply->rr_event2;
		rent->r_link2 = reply->rr_link2;
		return(0);
	}
}


/*
 *	getrentc
 *
 *	Function:	- obtains and caches a route table entry
 *			- nodeid must be set in route entry
 *			- uses getrent() to consult the router
 *	Accepts:	- route entry ptr
 *	Returns:	- 0 or ERROR
 */
int
getrentc(struct route *rent)
{
	struct route	*phit;
	SHASH		ahsd;
/*
 * Initialize route cache.
 */
	if (ahs_init((int4) MAXRCACHE, (int4) sizeof(struct route),
			(int4) NOTNODEID, AHNOINIT,
			(void *) _kio.ki_route, (int4 *) 0, &ahsd) == 0) {
		return(LAMERROR);
	}
/*
 * Check the route cache.
 */
	phit = (struct route *) ahs_find(&ahsd, rent->r_nodeid);

	if (phit) {
		memcpy((char *) rent, (char *) phit, sizeof(struct route));
	}
/*
 * Get the route table entry from the router.
 */
	else {
		if (getrent(rent)) return(LAMERROR);

		if (rent->r_nodetype != NOTNODETYPE) {
/*
 * It is not the dumb route so insert the entry in the route cache.
 */
			if (ahs_kick(&ahsd, (void *) rent)) return(LAMERROR);
		}
	}

	return(0);
}
