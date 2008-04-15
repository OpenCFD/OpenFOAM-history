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
 *	Function:	- gathers message status from a remote bufferd
 *	Accepts:	- target node ID
 *			- buffer status table
 *			- # of table entries
 *			- buffer parameter structure ptr
 *	Returns:	- number of replied descriptors or ERROR
 */

#include <lam_config.h>

#include <unistd.h>

#include <bfreq.h>
#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
rbfstate(node, tbl, tblsize, parms)

int4			node;		/* target node ID */
int			tblsize;	/* buffer status table size */
struct bfstatus		tbl[];		/* buffer status table */
struct bfparms		*parms;		/* buffer parameters */

{
	struct nmsg	nhead;		/* network message desc. */
	struct bfreq	*request;	/* bufferd request */
	struct bfreply	*reply;		/* bufferd reply */
	int4		minsize;	/* # expected elements */
	int4		mask;		/* signal mask */
	int4		totsize;	/* total # replied descs */

	request = (struct bfreq *) nhead.nh_data;
	reply = (struct bfreply *) nhead.nh_data;

	request->bfq_src_node = (node == LOCAL) ? node : getnodeid();
	request->bfq_src_event = -lam_getpid();
	request->bfq_req = BFQSTATE;
	request->bfq_maxtbl = tblsize;

	nhead.nh_node = node;
	nhead.nh_event = EVBUFFERD;
	nhead.nh_type = DLCTL;
	nhead.nh_flags = NOBUF;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_type = 0;
	nhead.nh_flags = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	if (reply->bfr_reply) {
		errno = reply->bfr_reply;
		ksigsetmask(mask);
		return(LAMERROR);
	}

	parms->bfp_maxspace = reply->bfr_maxspace;
	parms->bfp_nspace = reply->bfr_nspace;
	totsize = reply->bfr_nbuf;
	minsize = LAM_min(totsize, tblsize);
	nhead.nh_length = MAXNMSGLEN;
	nhead.nh_msg = (char *) tbl;

	while (minsize > 0) {
	
		if (nrecv(&nhead)) {
			ksigsetmask(mask);
			return(LAMERROR);
		}
			
		minsize -= nhead.nh_length / sizeof(struct bfstatus);
		nhead.nh_msg += nhead.nh_length;
	}

	ksigsetmask(mask);
	return(totsize);
}
