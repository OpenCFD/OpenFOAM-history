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
 *	Function:	- polling probe on one sync point in bufferd
 *	Accepts:	- network msg
 *	Returns:	- 0 if no message, 1 if message, or ERROR
 *			- if message found, updates nh_event, nh_type,
 *			  nh_length and nh_data[0, 1] with matching message
 */

#include <unistd.h>

#include <lam_config.h>
#include <bfreq.h>
#include <etc_misc.h>
#include <events.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
bfiprobe(pnh)

struct nmsg		*pnh;

{
	struct bfreq	*req;		/* bufferd request */
	struct bfreply	*reply;		/* bufferd reply */
	struct nmsg	nhreq;		/* bufferd req msg */
	struct nmsg	nhreply;	/* bufferd reply msg */
/*
 * Set up bufferd request.
 */
	req = (struct bfreq *) nhreq.nh_data;
	req->bfq_req = BFQPROBE;
	req->bfq_src_node = LOCAL;
	req->bfq_src_event = -lam_getpid();
	req->bfq_flags = pnh->nh_flags;
	req->bfq_event = pnh->nh_event;
	req->bfq_type = pnh->nh_type;
/*
 * Set up bufferd request message.
 */
	nhreq.nh_dl_event = EVBUFFERD;
	nhreq.nh_node = LOCAL;
	nhreq.nh_event = EVBUFFERD;
	nhreq.nh_type = DLCTL;
	nhreq.nh_flags = pnh->nh_flags & KTRACE;
	nhreq.nh_length = 0;
	nhreq.nh_msg = 0;
/*
 * Set up bufferd reply message.
 */
	nhreply.nh_event = -lam_getpid();
	nhreply.nh_type = 0;
	nhreply.nh_flags = 0;
	nhreply.nh_length = 0;
	nhreply.nh_msg = 0;

	if (dsfr(&nhreq, &nhreply)) return(LAMERROR);
/*
 * Consider bufferd reply.
 */
	reply = (struct bfreply *) nhreply.nh_data;

	if (reply->bfr_reply) {
		errno = reply->bfr_reply;
		return(LAMERROR);
	}
/*
 * Pass the probe information back to the caller.
 */
	if (reply->bfr_index == 0) {
		pnh->nh_type = reply->bfr_type;
		pnh->nh_flags = reply->bfr_flags;
		pnh->nh_length = reply->bfr_length;
		pnh->nh_data[0] = reply->bfr_data0;
		pnh->nh_data[1] = reply->bfr_data1;
		return(1);
	}
/*
 * There is no matching message.
 */
	else {
		return(0);
	}
}
