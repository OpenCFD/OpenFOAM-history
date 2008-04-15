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
 *	GDB
 *
 *	$Log: trillium.c,v $
 *	Revision 6.3  2002/10/09 20:56:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:37  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:45:24  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:32:16  gdburns
 * Ohio Release
 * 
 *	Function:	- internal client access to local servers
 */

#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <typical.h>
#include <terror.h>

/*
 * external functions
 */
extern char		*flforget();
extern int		*pdoom();

/*
 *	rforget
 *
 *	Function:	- translates tag to address
 *	Accepts:	- server node ID (not used, always local)
 *			- flag tag
 *	Returns:	- address or 0
 */
char
*rforget(nodeid, tag)

int4			nodeid;
int4			tag;

{
	return(flforget(tag));
}

/*
 *	rpdoom
 *
 *	Function:	- delivers a signal to kenyad process(es)
 *	Accepts:	- server node ID (not used, always local)
 *			- process selection method
 *			- selection value
 *			- signal number
 *	Returns:	- 0 or ERROR
 */
int
rpdoom(nodeid, flags, value, sig)

int4			nodeid;
int4			flags;
int4			value;
int4			sig;

{
	struct preq	request;	/* kenya request */
	struct preply	reply;		/* kenya reply */

	request.pq_sel_method = flags;
	request.pq_sel_value = value;
	request.pq_signal = sig;

	pdoom(&request, &reply);

	if (reply.pr_reply) {
		errno = ttol(reply.pr_reply);
		return(LAMERROR);
	} else {
		return(0);
	}
}
