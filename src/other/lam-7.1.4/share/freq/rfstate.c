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
 *	$Id: rfstate.c,v 6.7 2002/10/09 20:57:14 brbarret Exp $
 * 
 *	Function:	- gathers state from a remote filed
 *			- receives file descriptors from
 *			  filed until reply code is EEOF
 *
 *	Accepts:	- target node ID
 *			- ptr to file descriptor table
 *			- # of entries in table
 *
 *	Returns:	- number of replied descriptors or ERROR
 */

#include <string.h>
#include <unistd.h>

#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <etc_misc.h>

/*
 * local variables
 */
static char		hole[MAXNMSGLEN];


int
lam_rfstate(int nodeid, struct fstate ftbl[], int maxcount)
{
	struct nmsg	nhead;		/* network message desc. */
	struct freq  	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	struct fstate	*fhole;		/* ptr incoming file descs */
	int		count;		/* # descs in this msg */
	int		totcount;	/* total # replied descs */
	int		i;		/* favourite index */
	int		mask;		/* signal mask */

	totcount = 0;
	fhole = (struct fstate *) hole;
	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = (nodeid == LOCAL) ? nodeid : getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQSTATE;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = (char *) 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_msg = hole;

	do {
		nhead.nh_length = MAXNMSGLEN;

		if (nrecv(&nhead)) {
			ksigsetmask(mask);
			return(LAMERROR);
		}

		count = nhead.nh_length / sizeof(struct fstate);
		totcount += count;

		if (maxcount > 0) {
/*
 * Convert byte ordering (sigh).
 */
			for (i = 0; i < count; ++i) {
				fhole[i].fs_tfd = ttol(fhole[i].fs_tfd);
				fhole[i].fs_tflags = ttol(fhole[i].fs_tflags);
				fhole[i].fs_flow = ttol(fhole[i].fs_flow);
				fhole[i].fs_count = ttol(fhole[i].fs_count);
				fhole[i].fs_src_node =
					ttol(fhole[i].fs_src_node);
				fhole[i].fs_src_event =
					ttol(fhole[i].fs_src_event);
			}

			if (count > maxcount) count = maxcount;

			memcpy((char *) ftbl, hole,
				count * sizeof(struct fstate));
			maxcount -= count;
			ftbl += count;
		}

	} while (! reply->fr_errno);

	ksigsetmask(mask);

	if (reply->fr_errno == EEOF) {
		return(totcount);
	} else {
		errno = reply->fr_errno;
		return(LAMERROR);
	}
}

#if 0
/* Down with backwards compatability! */
/*
 * backwards compatibility
 */
int rfstate(nodeid, ftbl, maxcount) int nodeid, maxcount; struct fstate ftbl[];
	{ return(lam_rfstate(nodeid, ftbl, maxcount)); }
#endif
