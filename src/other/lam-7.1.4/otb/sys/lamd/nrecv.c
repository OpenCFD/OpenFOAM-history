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
 *	Copyright 1994 The Ohio State University
 *	GDB
 *
 *	$Id: nrecv.c,v 6.4 2002/10/09 20:56:47 brbarret Exp $
 * 
 *	Function:	- receives a buffered network message
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <bfreq.h>
#include <events.h>
#include <net.h>

int
nrecv(nhead)

struct nmsg		*nhead;

{
	struct bfreq	*req;		/* bufferd request */
	struct nmsg	bfhead;		/* bufferd message */

	if (!(nhead->nh_flags & NOBUF)) {
		LAM_ZERO_ME(bfhead);
		req = (struct bfreq *) bfhead.nh_data;
		req->bfq_req = BFQRECV;
		req->bfq_event = nhead->nh_event;
		req->bfq_type = nhead->nh_type;
		bfhead.nh_dl_event = EVBUFFERD;
		bfhead.nh_node = LOCAL;
		bfhead.nh_event = EVBUFFERD;
		bfhead.nh_type = DLCTL;
		bfhead.nh_flags = 0;
		bfhead.nh_length = 0;
		bfhead.nh_msg = 0;

		return(dsfr(&bfhead, nhead));
	} else {
		return(frecv(nhead));
	}
}
