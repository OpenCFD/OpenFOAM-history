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
 *	$Id: dsend.c,v 6.4 2002/10/09 20:59:49 brbarret Exp $
 *
 *	Function:	- sends a message at the datalink sublayer
 */

#include <net.h>

int
dsend(nhead)

struct nmsg		*nhead;

{
	struct kmsg	khead;		/* kernel message descriptor */	
	int4		mynodeid;
/*
 * Set up kernel message descriptor to send the network frame header.
 */
	khead.k_event = nhead->nh_dl_event;
	khead.k_length = nhead->nh_length;
/*
 * Categorize the message.
 * We must allow LOCAL to fall through before calling the router.
 */
	mynodeid = (nhead->nh_node == LOCAL) ? LOCAL : getnodeid();

	if ((nhead->nh_dl_event == nhead->nh_event) &&
			(nhead->nh_node == mynodeid)) {
		khead.k_type = nhead->nh_type;
		khead.k_flags = KNMSG | nhead->nh_flags;
	} else {
		khead.k_type = (nhead->nh_event >= 0) ? DLUSR : DLSYS;
		khead.k_flags = KNMSG | (nhead->nh_flags & (KTRY | KTRACE));

		if (nhead->nh_flags & DDLI) {
			khead.k_type |= DLI;
			nhead->nh_flags &= ~DDLI;
		}
	}

	khead.k_msg = (char *) nhead;

	return(ksend(&khead));
}

/*
 *	dtry_send
 *
 *	Function:	- non-blocking dsend()
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
dtry_send(nhead)

struct nmsg		*nhead;

{
	int		ret;

	nhead->nh_flags |= KTRY;
	ret = dsend(nhead);
	nhead->nh_flags &= ~KTRY;
	return(ret);
}
