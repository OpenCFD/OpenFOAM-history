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
 *	$Id: rload.c,v 6.6 2002/10/09 21:00:47 brbarret Exp $
 *
 *	Function:	- loads file onto remote node
 *	Accepts:	- source node of file 
 *			- destination node 
 *			- flat tag
 *			- filename
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <etc_misc.h>
#include <events.h>
#include <ksignal.h>
#include <lreq.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>

int
rload(int4 src_node, int4 dest_node, int4 tag, CONST char *filename)
{
	struct lreply	*reply;			/* loadd reply */
	struct lreq	*req;			/* loadd request */
	struct nmsg	nhead;			/* network header */
	char		*fullpath = 0;
	int		mask;
	int		bmalloc;		/* malloc'd filename */
/*
 * If the source node is the local node, get a full path for the
 * file in the client, not loadd.
 */
	if ((src_node == LOCAL) || (src_node == getnodeid())) {
		fullpath = sfh_path_env_find((char*) filename, R_OK | X_OK);
		if (fullpath == 0) return(LAMERROR);
		bmalloc = TRUE;
		nhead.nh_length = strlen(fullpath) + 1;
	} else {
		bmalloc = FALSE;
		nhead.nh_length = strlen(filename) + 1;
	}
/*
 * Check name length.
 */
	if (nhead.nh_length > FNAMELEN) {

		if (bmalloc) {
			free(fullpath);
		}

		errno = ENAMETOOLONG;
		return(LAMERROR);
	}

	req = (struct lreq *) nhead.nh_data;
	reply = (struct lreply *) nhead.nh_data;
/*
 * Complete request information structure.
 */
	req->lq_req = LQLOAD;
	req->lq_src_node = (src_node == LOCAL) ? src_node : getnodeid();
	req->lq_src_event = -lam_getpid();
	req->lq_dest_node = dest_node;
	req->lq_tag = tag;

	nhead.nh_node = src_node;
	nhead.nh_event = EVLOADD;
	nhead.nh_type = 0;	  
	nhead.nh_flags = 0;
	nhead.nh_msg = bmalloc ? fullpath : (char *) filename;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
/*
 * Send loadd request.
 */
	if (nsend(&nhead)) {

		if (bmalloc) {
			free(fullpath);
		}

		ksigsetmask(mask);
		return(LAMERROR);
	}

	if (bmalloc) {
		free(fullpath);
	}

	nhead.nh_event = -lam_getpid();
/*
 * Receive reply.
 */
	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	ksigsetmask(mask);
/*
 * Set local errno with returned errno value.
 */
	if (reply->lr_reply) {
		errno = reply->lr_reply;
		return(LAMERROR);
	} else {
		return(0);
	}
}
