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
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: rpwait.c,v 6.5 2002/10/09 21:00:48 brbarret Exp $
 * 
 *	Function:	- waits for kenya process to die
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 *
 *	Accepts:	- ptr to node ID
 *			- ptr to process ID
 *			- ptr to child process status
 *
 *	Returns:	- 0 or ERROR with errno set accordingly
 */

#include <lam_config.h>

#include <unistd.h>

#include <etc_misc.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <typical.h>

int
rpwait(nodeid, pid, status)

int4			*nodeid;
int			*pid;
int			*status;

{
	struct nmsg	nhead;		/* network message descriptor */
	struct preply	reply;		/* kenyad reply */

	nhead.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
	nhead.nh_type = 1;
	nhead.nh_flags = 0;
	nhead.nh_length = sizeof(struct preply);
	nhead.nh_msg = (char *) &reply;

	if (nrecv(&nhead)) {
		return(ERROR);
	}

	*nodeid = ttol(reply.pr_nodeid);
	*pid = (int) ttol(reply.pr_pid);
	*status = (int) ttol(reply.pr_reply);

	return(0);
}
