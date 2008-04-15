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
 *	RBD
 *
 *	Function:	- receives a message at the datalink sublayer
 *			- do not overwrite the header, only change the
 *			  nh_length and nh_type fields and the data pouch
 */

#include <string.h>

#include <net.h>
#include <typical.h>

int
drecv(nhead)

struct nmsg		*nhead;

{
	struct nmsg	incoming;	/* received network message header */
/*
 * Receive the message in a separate header.
 */
	memcpy((char *) &incoming, (char *) nhead, sizeof(struct nmsg));

	if (frecv(&incoming)) return(LAMERROR);
/*
 * Copy the received nh_type and nh_length fields and the data pouch.
 */
	nhead->nh_type = incoming.nh_type;
	nhead->nh_length = LAM_min(incoming.nh_length, nhead->nh_length);
	memcpy((char *) nhead->nh_data, (char *) incoming.nh_data,
			NHDSIZE * sizeof(int4));

	return(0);
}

/*
 *	dtry_recv
 *
 *	Function:	- non-blocking drecv()
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
dtry_recv(nhead)

struct nmsg		*nhead;

{
	int		ret;

	nhead->nh_flags |= KTRY;
	ret = drecv(nhead);
	nhead->nh_flags &= ~KTRY;
	return(ret);
}
