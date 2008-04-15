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
 *	$Id: rflclean.c,v 6.5 2002/10/09 21:00:47 brbarret Exp $
 * 
 *	Function:	- removes tag and associated storage
 *	Accepts:	- remote node ID
 *			- tag value
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <etc_misc.h>
#include <events.h>
#include <flatreq.h>
#include <ksignal.h>
#include <net.h>
#include <typical.h>

#include <errno.h>
#include <unistd.h>


int
rflclean(nodeid, tag)

int4			nodeid;
int4			tag;

{
	struct flreq	*request;	/* flatd request */
	struct flreply	*reply;		/* flatd reply */
	struct nmsg	nhead;		/* network message */
	int		mask;		/* signal mask */

	request = (struct flreq *) nhead.nh_data;
	reply = (struct flreply *) nhead.nh_data;

	request->flq_src_node = (nodeid == LOCAL) ? nodeid : getnodeid();
	request->flq_src_event = -lam_getpid();
	request->flq_req = FLQCLEAN;
	request->flq_tag = tag;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVFLATD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_msg = 0;
	nhead.nh_length = 0;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_event = request->flq_src_event;
	
	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	ksigsetmask(mask);

	if (reply->flr_reply) {
		errno = reply->flr_reply;
		return(ERROR);
	}

	return(0);
}
