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
 *	$Log: bfselect.c,v $
 *	Revision 6.3  2002/10/09 20:59:49  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/26 18:22:02  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 23:56:34  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:57:53  gdburns
 * Ohio Release
 * 
 *	Function:	- blocking probe on multiple SQL sync points
 *	Accepts:	- synchronization array
 *			- array size
 *			- additional flags
 *	Returns:	- index of matched sync pt or ERROR
 *			- updates bfk_type with probed sync point
 *			- does not return probed length
 */

#include <bfreq.h>
#include <events.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
bfselect(pbfk, nbfk, flags, sync_index)

struct bfsync		*pbfk;
int			nbfk;
int4			flags;
int			*sync_index;

{
	struct bfreq	*req;		/* bufferd request */
	struct nmsg	nhreq;		/* bufferd req msg */
	struct nmsg	nhprobe;	/* probed message */
	int		i;

	if ((nbfk < 0) || (nbfk > (MAXNMSGLEN / sizeof(struct bfsync)))) {
		errno = EINVAL;
		return(LAMERROR);
	}
/*
 * Send a query message to the buffer.
 */
	req = (struct bfreq *) nhreq.nh_data;
	req->bfq_req = BFQRECV;
	req->bfq_event = pbfk->bfk_event;
	req->bfq_type = pbfk->bfk_type;
	req->bfq_flags = flags | KSYNCSQL;
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

	nhprobe.nh_flags = KPROBE | KSYNCSQL | (flags & KTRACE);
	nhprobe.nh_msg = 0;
/*
 * Receive probes using the full SQL wildcard until a probe
 * matches one of the sync points.
 */
	for (;;) {
		nhprobe.nh_event =  req->bfq_event | KEVENTHI;
		nhprobe.nh_type = KTYPEHI | KTYPELO;
		nhprobe.nh_length = MAXNMSGLEN;

		if (dsfr(&nhreq, &nhprobe)) return(LAMERROR);

		for (i = 0; i < nbfk; ++i) {

			if (bfk_nh_cmp(pbfk + i, &nhprobe)) {
				pbfk[i].bfk_type = nhprobe.nh_type;
				*sync_index = i;
				return(0);
			}
		}
	}
}
