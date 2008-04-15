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
 *	$Id: frecv.c,v 6.4 2002/10/09 20:59:49 brbarret Exp $
 * 
 *	Function:	- forwarding datalink recv
 */

#include <net.h>

/*
 *	frecv
 *
 *	Function:	- receives a forwarded message at the datalink sublayer
 *			- the header is overwritten by the received message
 *	Accepts:	- network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
frecv(nhead)

struct nmsg		*nhead;

{
	struct kmsg	khead;		/* kernel message descriptor */
/*
 * Set up kernel message descriptor to receive network frame header.
 */     
	LAM_ZERO_ME(khead);
	khead.k_event = nhead->nh_event;
	khead.k_type = nhead->nh_type;
	khead.k_length = nhead->nh_length;
	khead.k_flags = KNMSG | nhead->nh_flags;
	khead.k_msg = (char *) nhead;
/*
 * Receive network frame header.
 */
	return(krecv(&khead));
}

/*
 *	frecvfront
 *
 *	Function:	- front half of frecv()
 *	Accepts:	- network message descriptor
 *	Returns:	- client-kernel socket or ERROR
 */
int
frecvfront(nhead)

struct nmsg		*nhead;

{
	struct kmsg	khead;		/* kernel message descriptor */
/*
 * Set up kernel message descriptor to receive network frame header.
 */     
	khead.k_event = nhead->nh_event;
	khead.k_type = nhead->nh_type;
	khead.k_length = nhead->nh_length;
	khead.k_flags = KNMSG | nhead->nh_flags;
	khead.k_msg = (char *) nhead;
/*
 * Receive network frame header.
 */
	return(krecvfront(&khead));
}

/*
 *	frecvback
 *
 *	Function:	- back half of frecv()
 *	Accepts:	- network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
frecvback(nhead)

struct nmsg		*nhead;

{
	struct kmsg	khead;		/* kernel message descriptor */

	khead.k_flags = KNMSG | nhead->nh_flags;
	khead.k_msg = (char *) nhead;
	return(krecvback(&khead));
}
