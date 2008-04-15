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
 *	Copyright 1993 The Ohio State University
 *	GDB
 *
 *	$Log: getsmart.c,v $
 *	Revision 6.3  2002/10/09 21:00:08  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/26 18:29:02  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/24 00:22:56  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  14:01:05  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:10:01  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:57:24  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:45:51  gdburns
 * Ohio Release
 * 
 * Revision 2.2.1.3  93/10/13  17:09:51  gdburns
 * Remove remote replies.
 * Use KSUPER.
 * 
 * Revision 2.2.1.2  93/02/18  17:55:51  gdburns
 * minor stds upgrade
 * 
 * Revision 2.2.1.1  92/08/12  16:39:38  gdburns
 * Set nh_event and nh_node for dsend().
 * 
 * Revision 2.2  92/04/30  14:49:44  trillium
 * Ohio Release
 * 
 * Revision 2.1  91/03/20  11:37:56  gdburns
 * Ohio Release
 * 
 *	Function:	- waits for a smart router
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <events.h>
#include <net.h>
#include <rreq.h>

int
getsmart()

{
	struct rreq	*request;		/* router request */
	struct nmsg	nhead;			/* network header */
/*
 * Set up the router request.
 */
	request = (struct rreq *) nhead.nh_data;
	request->rq_request = RQGETSMART;

	nhead.nh_dl_event = EVROUTER;
	nhead.nh_node = LOCAL;
	nhead.nh_event = EVROUTER;
	nhead.nh_type = RSMART;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	return(dsend(&nhead));
}
