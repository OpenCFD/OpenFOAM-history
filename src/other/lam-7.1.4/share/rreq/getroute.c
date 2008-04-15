/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 *
 * $Id: getroute.c,v 6.1 2003/03/25 00:26:13 jsquyres Exp $
 *
 *	Function:	- obtains routing information
 */

#include <lam_config.h>

#include <all_hash.h>
#include <etc_misc.h>
#include <events.h>
#include <kio.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>


/*
 *	getroute
 *
 *	Function:	- fills routing info in network message header
 *			- destination nodeid/event must be set
 *			- fills forwarding event and link info
 *	Accepts:	- ptr network message header
 *	Returns:	- 0 or ERROR
 */
int
getroute(struct nmsg *nhead)
{
	struct route	rent;		/* route table entry */
/*
 * Local node, forward on the given event.
 */
	if (nhead->nh_node == LOCAL) {
		nhead->nh_dl_event = nhead->nh_event;
	}
/*
 * Get a route table entry.
 */
	else {
		rent.r_nodeid = nhead->nh_node;

		if (getrentc(&rent)) return(LAMERROR);
/*
 * Is this my own node.  Forward on the given event.
 */
		if (rent.r_event == NOTEVENT) {
			nhead->nh_dl_event = nhead->nh_event;
		}
/*
 * Set the forwarding event and link from the route table entry.
 */
		else {
			nhead->nh_dl_event = rent.r_event;
			nhead->nh_dl_link = rent.r_link;
		}
	}

	return(0);
}


/*
 *	getrtype
 *
 *	Function:	- gets the type of a given node
 *	Accepts:	- node identifier
 *	Returns:	- nodetype or NOTNODETYPE
 */
int4
getrtype(int4 nodeid)
{
	struct route	rent;		/* route table entry */

	rent.r_nodeid = nodeid;

	if (getrentc(&rent)) {
		return(NOTNODETYPE);
	} else {
/*
 * Note: Even if getrentc() is successful, the nodetype returned 
 * in rent might still be NOTNODETYPE when a dumb route is returned.
 */
		errno = 0;
		return(rent.r_nodetype);
	}
}
