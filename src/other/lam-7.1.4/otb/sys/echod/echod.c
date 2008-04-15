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
 *	$Id: echod.c,v 1.5 2003/09/27 05:16:15 brbarret Exp $
 * 
 *	Function:	- echo server
 *			- receives a message and echos it back to the sender
 *			- primarily a diagnostic tool
 *			- message size limited to MAXNMSGLEN
 */

#include <lam_config.h>

#include <string.h>

#include <events.h>
#include <ereq.h>
#include <net.h>
#include <preq.h>
#include <terror.h>

/*
 * local variables
 */
static char		msg[MAXNMSGLEN];	/* echo message */
static struct nmsg	nhead;			/* echo network message */

/*
 * global functions
 */
void			(*(e_init()))();
void			(*(echod()))();

/*
 *	e_init
 *
 *	Function:	- echod initialization
 */
void (*(
e_init()))()

{
/*
 * Attach to kernel.
 */
	if (lpattach("echod")) {
		lampanic("echod (lpattach)");
	}
/*
 * Receive first request.
 */
	LAM_ZERO_ME(nhead);
	memset((void*) msg, -1, MAXNMSGLEN);
	nhead.nh_event = EVECHOD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = MAXNMSGLEN;
	nhead.nh_msg = msg;

	if (nrecv(&nhead)) {
		lampanic("echod (nrecv)");
	}

	return((void (*)()) echod);
}

/*
 *	echod
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(echod()))()

{
	struct ereq	*request;		/* request from client */

	request = (struct ereq *) nhead.nh_data;
/*
 * Reply to the request.
 */
	nhead.nh_node = request->eq_node;
	nhead.nh_event = request->eq_event;
	nhead.nh_flags = NREEL;
	nhead.nh_msg = msg;

	if (nsend(&nhead)) {
		lampanic("echod (nsend)");
	}
/*
 * Receive a request.
 */
	nhead.nh_event = EVECHOD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = MAXNMSGLEN;
	nhead.nh_msg = msg;

	if (nrecv(&nhead)) {
		lampanic("echod (nrecv)");
	}

	return((void (*)()) echod);
}
