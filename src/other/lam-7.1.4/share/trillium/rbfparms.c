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
 *	Function:	- controls buffer parameters
 */

#include <lam_config.h>

#include <unistd.h>

#include <bfreq.h>
#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
rbfparms(node, parms)

int4			node;
struct bfparms		*parms;

{
	struct nmsg	nhreq;		/* request message */
	struct nmsg	nhreply;	/* reply message */
	struct bfreq  	*request;	/* bufferd request */
	struct bfreply  *reply;		/* bufferd reply */
	int4		mask;		/* signal mask */

	request = (struct bfreq *) nhreq.nh_data;
	request->bfq_req = BFQSPACE;
	request->bfq_src_node = (node == LOCAL) ? node : getnodeid();
	request->bfq_src_event = -lam_getpid();
	request->bfq_maxspace = parms->bfp_maxspace;

	nhreq.nh_node = node;
	nhreq.nh_event = EVBUFFERD;
	nhreq.nh_type = DLCTL;
	nhreq.nh_flags = NOBUF;
	nhreq.nh_length = 0;
	nhreq.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhreq)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	nhreply.nh_event = -lam_getpid();
	nhreply.nh_type = 0;
	nhreply.nh_flags = 0;
	nhreply.nh_length = 0;
	nhreply.nh_msg = 0;

	if (nrecv(&nhreply)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	ksigsetmask(mask);

	reply = (struct bfreply *) nhreply.nh_data;

	if (reply->bfr_reply) {
		errno = reply->bfr_reply;
		return(LAMERROR);
	}

	return(0);
}
