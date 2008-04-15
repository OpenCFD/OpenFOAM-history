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
 *	Ohio Trollius
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	Function:	- sends, then receives a message at the
 *			  datalink sublayer (front/back)
 */

#include <net.h>

/*
 * local functions
 */
static void		dsfrsetup(struct nmsg *nhsend, struct nmsg *nhrecv, 
                                  struct kmsg *khsend, struct kmsg *khrecv);

/*
 *	dsfr
 *
 *	Function:	- atomic datalink send and forward recv
 *			- full operation
 *	Accepts:	- ptr send pkt
 *			- ptr recv pkt
 *	Returns:	- 0 or ERROR
 */
int
dsfr(nhsend, nhrecv)

struct nmsg		*nhsend;
struct nmsg		*nhrecv;

{
	struct kmsg	khsend;		/* kernel send message */	
	struct kmsg	khrecv;		/* kernel receive message */	
	int		r;

	LAM_ZERO_ME(khsend);
	LAM_ZERO_ME(khrecv);

	dsfrsetup(nhsend, nhrecv, &khsend, &khrecv);
	khrecv.k_msg = (char *) nhrecv;
	r = ksr(&khsend, &khrecv);
/*
 * In KPROBE, no data is transferred, not even the nmsg header, so
 * fill in the key fields from the kernel message descriptor.
 */
	if ((r == 0) && (khrecv.k_flags & KPROBE)) {
		nhrecv->nh_event = khrecv.k_event;
		nhrecv->nh_type = khrecv.k_type;
		nhrecv->nh_length = khrecv.k_length;
	}

	return(r);
}

/*
 *	dsfrfront
 *
 *	Function:	- sends message and initiates receive
 *	Accepts:	- ptr send packet
 *			- ptr receive packet
 *	Returns:	- receive file desc. or ERROR
 */
int
dsfrfront(nhsend, nhrecv)

struct nmsg		*nhsend;
struct nmsg		*nhrecv;

{
	struct kmsg	khsend;		/* send kernel msg */
	struct kmsg	khrecv;		/* recv kernel msg */

	dsfrsetup(nhsend, nhrecv, &khsend, &khrecv);
	return(ksrfront(&khsend, &khrecv));
}

/*
 *	dsfrback
 *
 *	Function:	- finishes receiving network pkt message
 *	Accepts:	- ptr receive packet
 *	Returns:	- 0 or ERROR
 */
int
dsfrback(nhrecv)

struct nmsg		*nhrecv;

{
	struct kmsg	khrecv;		/* kernel receive message */	
	int		r;

	khrecv.k_flags = KNMSG | nhrecv->nh_flags;
	khrecv.k_msg = (char *) nhrecv;

	r = ksrback(&khrecv);
/*
 * In KPROBE, no data is transferred, not even the nmsg header, so
 * fill in the key fields from the kernel message descriptor.
 */
	if ((r == 0) && (khrecv.k_flags & KPROBE)) {
		nhrecv->nh_event = khrecv.k_event;
		nhrecv->nh_type = khrecv.k_type;
		nhrecv->nh_length = khrecv.k_length;
	}

	return(r);
}

/*
 *	dsfrsetup
 *
 *	Function:	- setup prior to ksrfront() or ksr()
 *	Accepts:	- ptr send packet
 *			- ptr receive packet
 *			- ptr send kmsg
 *			- ptr receive kmsg
 */
static void
dsfrsetup(struct nmsg *nhsend, struct nmsg *nhrecv, 
          struct kmsg *khsend, struct kmsg *khrecv)
{
/*
 * Set up the kernel message to send the network frame.
 */
	khsend->k_event = nhsend->nh_dl_event;
	khsend->k_length = nhsend->nh_length;
/*
 * Currently, dsfr() is never used to forward messages, since
 * the underlying ksr() only supports a local client/server interaction.
 * Thus we always code type and flags for final delivery.
 */
	khsend->k_type = nhsend->nh_type;
	khsend->k_flags = KNMSG | nhsend->nh_flags;
	khsend->k_msg = (char *) nhsend;
/*
 * Set up kernel message to receive the network frame.
 */     
	khrecv->k_event = nhrecv->nh_event;
	khrecv->k_type = nhrecv->nh_type;
	khrecv->k_length = nhrecv->nh_length;
	khrecv->k_flags = KNMSG | nhrecv->nh_flags;
}
