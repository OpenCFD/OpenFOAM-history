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
 *	GDB/RBD
 *
 *	Function:	- receives a network message
 *			- The KTRY functionality is not air-tight.
 *	Accepts:	- network message descriptor
 */

#include <string.h>

#include <bfreq.h>
#include <events.h>
#include <kio.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;

int
nrecv(nhead)

struct nmsg		*nhead;		/* network message header */

{
	struct nmsg	incoming;	/* incoming packet */
	int4		remlen;		/* remaining length */
	char		*save_msg;	/* nh_msg temporary */
/*
 * Probe first for the message, if KTRY is requested.
 * This is not an air-tight solution.
 */
	if ((nhead->nh_flags & KTRY) && !(nhead->nh_flags & NOBUF)) {
		incoming.nh_event = nhead->nh_event;
		incoming.nh_type = nhead->nh_type;
		incoming.nh_flags = nhead->nh_flags;

		switch (nprobe(&incoming)) {

			case LAMERROR:
			return(LAMERROR);

			case 1:
			break;

			default:
			errno = EWOULDBLOCK;
			return(LAMERROR);
		}
	}
/*
 * Receive the network message using frecv() or bfrecv().
 */
	remlen = nhead->nh_length;

	incoming.nh_event = nhead->nh_event;
	incoming.nh_type = nhead->nh_type;
	incoming.nh_flags = nhead->nh_flags | NMORE;
	incoming.nh_length = MAXNMSGLEN;
	incoming.nh_msg = nhead->nh_msg;

	while ((remlen > MAXNMSGLEN) && (incoming.nh_flags & NMORE)) {
		save_msg = incoming.nh_msg;

		if (nhead->nh_flags & NOBUF) {
			if (frecv(&incoming)) return(LAMERROR);
		} else {
			if (bfrecv(&incoming)) return(LAMERROR);
		}

		incoming.nh_msg = save_msg;
		remlen -= incoming.nh_length;
		incoming.nh_msg += incoming.nh_length;
	}

	if (incoming.nh_flags & NMORE) {
/*
 * The next packet should be the last.
 */
		incoming.nh_length = remlen;

		if (nhead->nh_flags & NOBUF) {
			if (frecv(&incoming)) return(LAMERROR);
		} else {
			if (bfrecv(&incoming)) return(LAMERROR);
		}

		remlen -= LAM_min(remlen, incoming.nh_length);
/*
 * Suck excess packets into the vacuum of space.
 */
		while (incoming.nh_flags & NMORE) {
			incoming.nh_length = 0;

			if (nhead->nh_flags & NOBUF) {
				if (frecv(&incoming)) return(LAMERROR);
			} else {
				if (bfrecv(&incoming)) return(LAMERROR);
			}
		}
	}
/*
 *	else if (remlen > MAXNMSGLEN)
 *
 * We just ran out of packets.
 */
	nhead->nh_length = nhead->nh_length - remlen;
	nhead->nh_type = incoming.nh_type;
	memcpy((char *) nhead->nh_data, (char *) incoming.nh_data,
			NHDSIZE * sizeof(int4));

	return(0);
}

/*
 *	ntry_recv
 *
 *	Function:	- non-blocking nrecv()
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
ntry_recv(nhead)

struct nmsg		*nhead;

{
	int		ret;

	nhead->nh_flags |= KTRY;
	ret = nrecv(nhead);
	nhead->nh_flags &= ~KTRY;
	return(ret);
}
