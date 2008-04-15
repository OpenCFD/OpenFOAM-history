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
 *	Function:	- nonblocking probe on multiple SQL sync points
 *	Accepts:	- synchronization array
 *			- array size
 *			- additional flags
 *			- index of matched sync pt (out)
 *	Returns:	- 0 or ERROR
 *			- updates bfk_type with probed sync point
 *			- does not return probed length
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
bfiselect(pbfk, nbfk, flags, sync_index)

struct bfsync		*pbfk;
int			nbfk;
int4			flags;
int			*sync_index;

{
	struct bfreq	*req;		/* bufferd request */
	struct bfreply	*reply;		/* bufferd reply */
	struct nmsg	nhreq;		/* bufferd req msg */
	struct nmsg	nhreply;	/* bufferd reply msg */

	if ((nbfk < 0) || (nbfk > (MAXNMSGLEN / sizeof(struct bfsync)))) {
		errno = EINVAL;
		return(LAMERROR);
	}
/*
 * Send a query message to the buffer.
 */
	req = (struct bfreq *) nhreq.nh_data;
	req->bfq_req = BFQPROBE;
	req->bfq_src_node = LOCAL;
	req->bfq_src_event = -lam_getpid();
	req->bfq_event = pbfk->bfk_event;
	req->bfq_type = pbfk->bfk_type;
	req->bfq_flags = flags;
/*
 * Set up bufferd request message.
 */
	nhreq.nh_dl_event = EVBUFFERD;
	nhreq.nh_node = LOCAL;
	nhreq.nh_event = EVBUFFERD;
	nhreq.nh_type = DLCTL;
	nhreq.nh_flags = flags & KTRACE;
	nhreq.nh_length = (nbfk == 1) ? 0 : sizeof(struct bfsync) * nbfk;
	nhreq.nh_msg = (char *) pbfk;

	nhreply.nh_event = -lam_getpid();
	nhreply.nh_type = 0;
	nhreply.nh_flags = 0;
	nhreply.nh_length = 0;
	nhreply.nh_msg = 0;

	if (dsfr(&nhreq, &nhreply)) return(LAMERROR);

	reply = (struct bfreply *) nhreply.nh_data;

	if (reply->bfr_reply) {
		errno = reply->bfr_reply;
		return(LAMERROR);
	}

	*sync_index = reply->bfr_index;

	if (reply->bfr_index >= 0) {
		pbfk[reply->bfr_index].bfk_type = reply->bfr_type;
	}

	return(0);
}
