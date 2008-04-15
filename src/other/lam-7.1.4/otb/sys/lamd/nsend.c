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
 *	$Id: nsend.c,v 6.4 2002/10/09 20:56:47 brbarret Exp $
 * 
 *	Function:	- sends a network message
 */

#include <events.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>

int
nsend(nhead)

struct nmsg		*nhead;		/* network message header */

{
/*
 * Get the routing information.
 */
	if ((nhead->nh_flags & NOBUF) ||
			((nhead->nh_node != LOCAL) &&
			(nhead->nh_node != getnodeid()))) {

		if (getroute(nhead)) {
			return(ERROR);
		}
	} else {
		nhead->nh_dl_event = EVBUFFERD;
	}
/*
 * If this send is for a multireel, nh_data[6] contains the packet number.
 */
	if (nhead->nh_flags & NREEL) {
		nhead->nh_data[6] = 0;
	}

	return(dsend(nhead));
}
