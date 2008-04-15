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
 *
 * $Id: rpstate.c,v 6.6 2002/12/12 22:17:13 jsquyres Exp $
 *
 *	Function:	- retrieves status of remote processes
 *	Accepts:	- remote node ID
 *			- process select flags
 *			- process select value
 *			- process state table
 *			- process state table # entries
 *	Returns:	- # retrieved process states or ERROR
 */

#include <lam_config.h>

#include <unistd.h>

#include <events.h>
#include <etc_misc.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <typical.h>


int4
rpstate(int4 nodeid, int4 sflags, int4 svalue, struct pstate pstable[], 
	int4 maxproc)
{
	struct nmsg	nhead;		/* network nhead */
	struct preq	request;	/* kenya request */
	int4		nproc;		/* # replied states */
	int4		mask;		/* signal mask */
	int4		ret;		/* reply code */

	if (maxproc <= 0) {
		return(0);
	}

	nproc = 0;

	LAM_ZERO_ME(nhead);
	LAM_ZERO_ME(request);
	request.pq_sel_method = ltot(sflags);
	request.pq_sel_value = ltot(svalue);
	request.pq_req = ltot(PQSTATE);
	request.pq_maxproc = ltot(maxproc);
	request.pq_src_node = (nodeid == LOCAL) ?
			ltot(nodeid) : ltot(getnodeid());
	request.pq_src_event = ltot((int4) -lam_getpid());

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
	nhead.nh_length = sizeof(struct pstate);

	do {
		nhead.nh_msg = (char *) pstable;

		if (nrecv(&nhead)) {
			ksigsetmask(mask);
			return(ERROR);
		}
/*
 * Convert byte ordering (sigh).
 */
		pstable->ps_reply = ttol(pstable->ps_reply);
		pstable->ps_index = ttol(pstable->ps_index);
		pstable->ps_pid = ttol(pstable->ps_pid);
		pstable->ps_rtf = ttol(pstable->ps_rtf);
		pstable->ps_event = ttol(pstable->ps_event);
		pstable->ps_nodeid = ttol(pstable->ps_nodeid);
		pstable->ps_kernel.ks_state =
				ttol(pstable->ps_kernel.ks_state);
		pstable->ps_kernel.ks_priority =
				ttol(pstable->ps_kernel.ks_priority);
		pstable->ps_kernel.ks_event =
				ttol(pstable->ps_kernel.ks_event);
		pstable->ps_kernel.ks_type =
				ttol(pstable->ps_kernel.ks_type);
		pstable->ps_kernel.ks_signal =
				ttol(pstable->ps_kernel.ks_signal);

		ret = pstable->ps_reply;
		++nproc;
		++pstable;
	} while ((nproc < maxproc) && !ret);

	ksigsetmask(mask);
	if (ret) nproc--;
	return(nproc);
}
