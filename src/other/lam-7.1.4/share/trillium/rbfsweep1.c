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
 *	Copyright 1994 The Ohio State University
 *	GDB
 *
 *	$Log: rbfsweep1.c,v $
 *	Revision 6.3  2002/10/09 21:00:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/26 21:50:00  kmeyer1
 *	
 *	added copyright
 *	
 *	Revision 6.1  1996/11/24 00:25:54  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  14:04:58  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:13:28  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:58:05  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:46:14  gdburns
 * Ohio Release
 * 
 * Revision 2.2  94/02/15  15:24:44  gdburns
 * pre-release to 2.3
 * 
 *	Function:	- removes buffered messages with a particular event
 *	Accepts:	- node ID
 *			- event
 */

#include <net.h>
#include <events.h>
#include <bfreq.h>

int
rbfsweep1(node, event)

int4			node;
int4			event;

{
	struct nmsg	nhead;		/* network header */
	struct bfreq  	*request;	/* bufferd request */

	request = (struct bfreq *) nhead.nh_data;
	request->bfq_req = BFQSWEEP1;
	request->bfq_event = event;

	nhead.nh_node = node;
	nhead.nh_event = EVBUFFERD;
	nhead.nh_type = DLCTL;
	nhead.nh_flags = NOBUF;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(nsend(&nhead));
}
