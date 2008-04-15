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
 *	$Id: bforward.c,v 1.4 2002/10/09 20:56:29 brbarret Exp $
 * 
 *	Function:	- bforward, henchman of bufferd
 *			- forwards remote messages for bufferd
 */

#include <lam_config.h>

#include <string.h>

#include <bfreq.h>
#include <events.h>
#include <net.h>
#include <rreq.h>
#include <terror.h>

/*
 * local variables
 */
static char		buf[MAXNMSGLEN];
static struct nmsg	nhead;			/* network message */

/*
 * local functions
 */
static void		bwrecv();

/*
 * global functions
 */
void			(*(bw_init()))();
void			(*(bforward()))();

/*
 *	bw_init
 *
 *	Function:	- bforward initialization
 */
void (*(
bw_init()))()

{
/*
 * Receive first request.
 */
	LAM_ZERO_ME(nhead);
	memset((void*) buf, -1, MAXNMSGLEN);

	bwrecv();
	return((void (*)()) bforward);
}

/*
 *	bforward
 *
 *	Function:	- server loop
 *			- forwards message & receives next message
 */
void
(*(bforward()))()

{
/*
 * Don't forward the KTRY, DDLI or KTRACE flags.
 */
	nhead.nh_flags &= ~(KTRY | DDLI | KTRACE);

	if (getroute(&nhead)) lampanic("bforward (getroute)");

	if (dsend(&nhead)) lampanic("bforward (dsend)");
/*
 * Receive next message.
 */
	bwrecv();
	return((void (*)()) bforward);
}

/*
 *	bwrecv
 *
 *	Function:	- receives a message from bufferd
 */
static void
bwrecv()

{
	nhead.nh_event = EVBFORWARD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = MAXNMSGLEN;
	nhead.nh_msg = buf;

	if (bfrecv(&nhead)) lampanic("bforward (bfrecv)");
}
