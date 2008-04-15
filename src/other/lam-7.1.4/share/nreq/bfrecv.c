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
 *	$Id: bfrecv.c,v 6.4 2002/10/09 20:59:49 brbarret Exp $
 * 
 *	Function:	- buffer receive (atomic/front/back)
 */

#include <string.h>

#include <bfreq.h>
#include <events.h>
#include <ktrace.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 *	bfrecv
 *
 *	Function:	- receives a packet from a buffer
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
bfrecv(nhead)

struct nmsg		*nhead;

{
	struct bfreq	*req;			/* bufferd request */
	struct nmsg	bfhead;			/* bufferd message */

	LAM_ZERO_ME(bfhead);
	nhead->nh_flags &= ~KTRY;
/*
 * Send a query message to the buffer.
 */
	req = (struct bfreq *) bfhead.nh_data;
	req->bfq_req = BFQRECV;
	req->bfq_event = nhead->nh_event;
	req->bfq_type = nhead->nh_type;
	req->bfq_flags = nhead->nh_flags;

	bfhead.nh_dl_event = EVBUFFERD;
	bfhead.nh_node = LOCAL;
	bfhead.nh_event = EVBUFFERD;
	bfhead.nh_type = DLCTL;
	bfhead.nh_flags = nhead->nh_flags & KTRACE;
	bfhead.nh_length = 0;
	bfhead.nh_msg = 0;

	return(dsfr(&bfhead, nhead));
}

/*
 *	bfrecvfront
 *
 *	Function:	- front-end to receive a packet from a buffer
 *	Accepts:	- network message descriptor ptr
 *	Returns:	- receive file desc. or ERROR
 */
int
bfrecvfront(nhead)

struct nmsg		*nhead;

{
	struct nmsg	bfhead;		/* bufferd message */
	struct bfreq	*req;		/* bufferd request */

	nhead->nh_flags &= ~KTRY;
/*
 * Send a query message to the buffer.
 */
	req = (struct bfreq *) bfhead.nh_data;
	req->bfq_req = BFQRECV;
	req->bfq_event = nhead->nh_event;
	req->bfq_type = nhead->nh_type;
	req->bfq_flags = nhead->nh_flags;
	bfhead.nh_dl_event = EVBUFFERD;
	bfhead.nh_node = LOCAL;
	bfhead.nh_event = EVBUFFERD;
	bfhead.nh_type = DLCTL;
	bfhead.nh_flags = nhead->nh_flags & KTRACE;
	bfhead.nh_length = 0;
	bfhead.nh_msg = 0;

	return(dsfrfront(&bfhead, nhead));
}

/*
 *	bfrecvback
 *
 *	Function:	- finishes receiving a packet from bufferd
 *	Accepts:	- network message descriptor ptr
 *	Returns:	- 0 or ERROR
 */
int
bfrecvback(nhead)

struct nmsg		*nhead;

{
	return(dsfrback(nhead));
}
