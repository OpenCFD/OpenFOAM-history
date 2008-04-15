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
 *	$Id: rget.c,v 6.6 2002/10/09 21:00:47 brbarret Exp $
 * 
 *	Function:	- runtime access to the flat server
 *			- inquires about a given tag
 */

#include <lam_config.h>

#include <etc_misc.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <events.h>
#include <flatreq.h>
#include <ksignal.h>
#include <net.h>
#include <typical.h>


/*
 * local functions
 */
static char		*do_get();

/*
 *	rget
 *
 *	Function:	- translates tag to address
 *	Accepts:	- server node ID
 *			- tag value
 *	Returns:	- corresponding tag address or ERROR
 */
char
*rget(nodeid, tag)

int4			nodeid;
int4			tag;

{
	return(do_get(nodeid, tag, (int4) FLQGET));
}

/*
 *	rforget
 *
 *	Function:	- translates tag to address
 *			- removes tag from server
 *	Accepts:	- server node ID
 *			- tag value
 *	Returns:	- corresponding tag address or ERROR
 */
char
*rforget(nodeid, tag)

int4			nodeid;
int4			tag;

{
	return(do_get(nodeid, tag, (int4) FLQFORGET));
}

/*
 *	do_get
 *
 *	Function:	- translates tag to address
 *	Accepts:	- server node ID
 *			- tag value
 *			- flatd request
 *	Returns:	- corresponding tag address or ERROR
 */
static char
*do_get(nodeid, tag, req)

int4			nodeid;
int4			tag;
int4			req;

{
	struct flreq	*request;	/* flatd request */
	struct flreply	*reply;		/* flatd reply */
	struct nmsg	nhead;		/* network message */
	int		mask;		/* signal mask */
	char		*p;		/* favourite ptr */
	char		fname[FLPATHLEN];
					/* tagged file name (OTB) */

	request = (struct flreq *) nhead.nh_data;
	reply = (struct flreply *) nhead.nh_data;

	request->flq_src_node = (nodeid == LOCAL) ? nodeid : getnodeid();
	request->flq_src_event = -lam_getpid();
	request->flq_req = req;
	request->flq_tag = tag;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVFLATD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_msg = 0;
	nhead.nh_length = 0;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(0);
	}

	nhead.nh_event = request->flq_src_event;
	nhead.nh_msg = fname;
	nhead.nh_length = FLPATHLEN;
	
	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(0);
	}

	ksigsetmask(mask);

	if (reply->flr_reply) {
		errno = reply->flr_reply;
		return(0);
	}
/*
 * The message body holds the name of a file from the remote daemon.
 * Allocate space for the filename, copy it and return a ptr to it.
 */
	p = malloc((unsigned) FLPATHLEN);
	if (p == 0) return(0);

	strcpy(p, nhead.nh_msg);
	return(p);
}
