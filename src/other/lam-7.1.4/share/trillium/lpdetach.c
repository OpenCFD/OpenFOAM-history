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
 *	$Id: lpdetach.c,v 6.8 2002/10/09 21:00:46 brbarret Exp $
 *
 *	Function:	- detaches caller from the local kenya server
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 *	Accepts:	- target process ID
 *			- exit status
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <etc_misc.h>
#include <events.h>
#include <kio.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <typical.h>
#include <terror.h>

#include <unistd.h>

/*
 * external variables
 */
extern struct kio_t	_kio;		/* Kernel Input/Output */

int
lpdetach(pid, status)

int			pid;
int			status;

{
	struct nmsg	nhead;		/* network header */
	struct preq  	request;	/* kenya request */
	struct preply	reply;		/* kenya reply */
	int		mask;

	LAM_ZERO_ME(nhead);
	LAM_ZERO_ME(request);
	LAM_ZERO_ME(reply);

	request.pq_src_node = ltot(LOCAL);
	request.pq_src_event = ltot((int4) -lam_getpid());
	request.pq_pid = ltot(pid ? (int4) pid : (int4) lam_getpid());
	request.pq_status = ltot((int4) status);
	request.pq_req = ltot(PQDETACH);

	nhead.nh_node = LOCAL;
	nhead.nh_event = EVKENYAD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = sizeof(request);
	nhead.nh_msg = (char *) &request;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_flags = 0;
	nhead.nh_length = sizeof(reply);
	nhead.nh_msg = (char *) &reply;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	ksigsetmask(mask);

	if (reply.pr_reply) {
		errno = ttol(reply.pr_reply);
		return(ERROR);
	} else {
		_kio.ki_rtf &= ~RTF_KENYA_ATTACH;
		return(0);
	}
}

/*
 *	rpdetach
 *
 *	Function:	- now just a wrapper for lpdetach()
 *			- obsolete; don't use this, use lpdetach()
 *	Accepts:	- destination node
 *			- target process ID
 *			- exit status
 *	Returns:	- 0 or ERROR
 */
int
rpdetach(node, pid, status)

int4			node;
int			pid;
int			status;

{
	if ((node != LOCAL) && (node != getnodeid())) {
		errno = EBADNODE;
		return(ERROR);
	}

	return(lpdetach(pid, status));
}
