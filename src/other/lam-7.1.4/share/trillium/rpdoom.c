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
 * $Id: rpdoom.c,v 6.6 2003/08/21 17:55:20 jsquyres Exp $
 * 
 *	Function:	- delivers a signal to a remote process
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 *	Accepts:	- remote node
 *			- remote process selection method
 *			- remote process selection value
 *			- signal number
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <errno.h>
#include <unistd.h>

#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <rreq.h>
#include <t_types.h>
#include <typical.h>


int
rpdoom(int4 nodeid, int4 sflags, int4 svalue, int4 signal)
{
	struct nmsg	nhead;		/* network nhead */
	struct preq	request;	/* kenya request */
	struct preply	reply;		/* kenya reply */
	int		mask;

        LAM_ZERO_ME(nhead);
        LAM_ZERO_ME(request);
	request.pq_sel_method = ltot(sflags);
	request.pq_sel_value = ltot(svalue);
	request.pq_src_node =
			((nodeid == LOCAL) || tiscast(getrtype(nodeid))) ?
			ltot(nodeid) : ltot(getnodeid());
	request.pq_src_event = ltot(- ((int4) lam_getpid()));
	request.pq_signal = ltot(signal);
	request.pq_req = ltot(PQDOOM);

	nhead.nh_node = nodeid;
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
		return(0);
	}
}
