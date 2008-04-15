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
 *	$Id: rbflook.c,v 6.5 2002/10/09 21:00:47 brbarret Exp $
 * 
 *	Function:	- receives a copy of a buffered network packet
 *	Accepts:	- server node
 *			- network message descriptor
 *			- sequence #
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <bfreq.h>
#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
rbflook(node, pnh_look, seq)

int4			node;
struct nmsg		*pnh_look;
int4			seq;

{
	struct bfreq	*pbfq;		/* bufferd request */
	struct bfreply	*pbfr;		/* bufferd reply */
	struct nmsg	nhbf;		/* bufferd message */
	int4		mask;		/* signal mask */
/*
 * Send a query message to the buffer.
 */
	pbfq = (struct bfreq *) nhbf.nh_data;

	pbfq->bfq_req = BFQLOOK;
	pbfq->bfq_src_node = (node == LOCAL) ? node : getnodeid();
	pbfq->bfq_src_event = -lam_getpid();
	pbfq->bfq_seq = seq;

	nhbf.nh_dl_event = EVBUFFERD;
	nhbf.nh_node = node;
	nhbf.nh_event = EVBUFFERD;
	nhbf.nh_type = DLCTL;
	nhbf.nh_flags = 0;
	nhbf.nh_length = 0;
	nhbf.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhbf)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhbf.nh_event = -lam_getpid();
	nhbf.nh_type = pnh_look->nh_type;
	nhbf.nh_length = 0;
	nhbf.nh_msg = 0;

	if (nrecv(&nhbf)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	pbfr = (struct bfreply *) nhbf.nh_data;

	if (pbfr->bfr_reply) {
		errno = pbfr->bfr_reply;
		ksigsetmask(mask);
		return(ERROR);
	}

	pnh_look->nh_type = pbfr->bfr_type;

	nhbf.nh_type = pnh_look->nh_type;
	nhbf.nh_length = pnh_look->nh_length;
	nhbf.nh_msg = pnh_look->nh_msg;

	if (nrecv(&nhbf)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	ksigsetmask(mask);

	pnh_look->nh_length = nhbf.nh_length;
	memcpy((char *) pnh_look->nh_data, (char *) nhbf.nh_data,
			NHDSIZE * sizeof(int4));
	return(0);
}
