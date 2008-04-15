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
 *	$Id: recho.c,v 6.5 2002/10/09 21:00:47 brbarret Exp $
 *
 *	Function:	- echoes a message to/from a remote node
 *	Accepts:	- remote node
 *			- outgoing message pointer
 *			- imcoming message pointer
 *			- outgoing message length
 *	Returns:	- incoming message length
 */

#include <lam_config.h>

#include <etc_misc.h>
#include <ereq.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>

#include <unistd.h>

int
recho(node, src_msg, dest_msg, length)

int4			node;			/* destination node */
CONST char		*src_msg;		/* ptr to source message */
char			*dest_msg;		/* echo message here */
int4			length;			/* length of echo message */

{
	struct nmsg	nhead;			/* message descriptor */
	struct ereq	*request;		/* echo request */
	int		mask;
/*
 * Check sanity on the length field.
 */
	if ((length < 0) || (length > MAXNMSGLEN)) {
		errno = EMSGLEN;
		return(ERROR);
	}
/*
 * Fill in echod request.
 */
	request = (struct ereq *) nhead.nh_data;
	request->eq_event = -lam_getpid();
	request->eq_node = ((node == LOCAL) || (getrtype(node) & NT_CAST)) ?
			node : getnodeid();
	
	nhead.nh_node = node;
	nhead.nh_length = length;
	nhead.nh_event = EVECHOD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_msg = (char *) src_msg;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
/*
 * Send request to the echo daemon.
 */
	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_msg = dest_msg;
	nhead.nh_length = length;
/*
 * Received echoed message.
 */
	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	ksigsetmask(mask);
	return(nhead.nh_length);
}
