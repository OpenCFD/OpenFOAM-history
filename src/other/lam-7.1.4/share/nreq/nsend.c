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
 *	$Log: nsend.c,v $
 *	Revision 6.3  2002/10/09 20:59:50  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/26 18:22:04  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 23:56:43  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:58:10  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.2  96/01/15  21:03:14  gdburns
 * Send the overall length of a multi-packet message in
 * nh_data[0] of the first packet.
 * 
 * Revision 5.2.1.1  94/12/22  17:15:38  raja
 * Remove low-level tracing.
 * 
 * Revision 5.2  94/08/22  14:07:14  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:52:56  gdburns
 * Ohio Release
 * 
 * Revision 2.3.1.1  94/05/18  10:17:58  gdburns
 * Support KTRY.
 * 
 * Revision 2.3  94/04/22  12:44:10  gdburns
 * Ohio Release
 * 
 *	Function:	- sends a network message
 *			- packetizes long messages
 *			- for NREEL, nh_data[6] is the packet #
 *			- for NMORE, nh_data[0] is the total length
 *	Accepts:	- network message descriptor
 */

#include <portable.h>
#include <events.h>
#include <kio.h>
#include <net.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;

int
nsend(nhead)

struct nmsg		*nhead;		/* network message header */

{
	int		f_nreel;	/* NREEL status flag */
	int4		length;		/* remaining length */
	int4		save_flags;	/* saved flags */
	int4		save_length;	/* saved length */
	int4		save_data0;	/* saved data[0] */
	char		*save_msg;	/* saved message ptr */
/*
 * Get the routing information.
 */
	if ((nhead->nh_flags & NOBUF) ||
			((nhead->nh_node != LOCAL) &&
			(nhead->nh_node != getnodeid()))) {

		if (getroute(nhead)) return(LAMERROR);
	} else {
		nhead->nh_dl_event = EVBUFFERD;
	}
/*
 * If this send is for a multireel, nh_data[6] contains the packet number.
 */
	f_nreel = FALSE;

	if (nhead->nh_flags & NREEL) {
		f_nreel = TRUE;
		nhead->nh_data[6] = 0;
	}
/*
 * multi-packet message
 */
	if (nhead->nh_length > MAXNMSGLEN) {
		save_flags = nhead->nh_flags;
		save_msg = nhead->nh_msg;
		save_length = nhead->nh_length;
		save_data0 = nhead->nh_data[0];
		length = save_length;
		nhead->nh_length = MAXNMSGLEN;
		nhead->nh_flags |= NMORE;
		nhead->nh_data[0] = save_length;
/*
* Send first packet.
*/
		if (dsend(nhead)) {
			nhead->nh_length = save_length;
			nhead->nh_flags = save_flags;
			return(LAMERROR);
		}

		nhead->nh_flags &= ~KTRY;
		nhead->nh_flags |= N2ND;
		nhead->nh_data[0] = save_data0;
		length -= MAXNMSGLEN;
		nhead->nh_msg += MAXNMSGLEN;

		if (f_nreel) {
			nhead->nh_data[6]++;
		}
/*
* Send the middle packets.
*/
		while (length > MAXNMSGLEN) {		

			if (dsend(nhead)) {
				nhead->nh_flags = save_flags;
				nhead->nh_length = save_length;
				nhead->nh_msg = save_msg;
				return(LAMERROR);
			}

			length -= MAXNMSGLEN;
			nhead->nh_msg += MAXNMSGLEN;

			if (f_nreel) {
				nhead->nh_data[6]++;
			}
		}

		nhead->nh_length = length;
		nhead->nh_flags &= ~NMORE;
/*
* Send the last packet.
*/
		if (dsend(nhead)) {
			nhead->nh_flags = save_flags;
			nhead->nh_length = save_length;
			nhead->nh_msg = save_msg;
			return(LAMERROR);
		}

		nhead->nh_msg = save_msg;
		nhead->nh_length = save_length;
		nhead->nh_flags = save_flags;
	}

	else {
		if (dsend(nhead)) return(LAMERROR);
	}

	return(0);
}

/*
 *	ntry_send
 *
 *	Function:	- non-blocking nsend()
 *	Accepts:	- ptr to network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
ntry_send(nhead)

struct nmsg		*nhead;

{
	int		ret;

	nhead->nh_flags |= KTRY;
	ret = nsend(nhead);
	nhead->nh_flags &= ~KTRY;
	return(ret);
}
