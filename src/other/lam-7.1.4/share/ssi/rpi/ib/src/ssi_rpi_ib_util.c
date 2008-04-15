/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_rpi_ib_util.c,v 1.1 2003/11/11 18:36:56 vsahay Exp $
 *
 *	Function:	- everything to do with tiny messages
 */

#include <lam_config.h>

#include <stdio.h>

#include <etc_misc.h>
#include <lamdebug.h>
#include <mpisys.h>
#include <rpisys.h>

#include <rpi_ib.h>
#include <rpi_ib_req.h>
#include <rpi_ib_util.h>
#include <rpi_ib_interval.h>
#include <rpi_ib_dreg.h>


/*
 *	lam_ssi_rpi_ib_fill_wildcards
 *
 *	Function:	- replace wildcards in request with matched values
 *			  and fill in the sequence number
 *	Accepts:	- request
 *			- matched envelope
 */
void
lam_ssi_rpi_ib_fill_wildcards(MPI_Request req, struct lam_ssi_rpi_envl *env)
{
  struct _group	*g;
		
  req->rq_seq = env->ce_seq;
    
  if (req->rq_rpi->cq_envbuf->ge_env.ce_tag == MPI_ANY_TAG)
    req->rq_rpi->cq_envbuf->ge_env.ce_tag = env->ce_tag;

  if (req->rq_rpi->cq_peer == MPI_ANY_SOURCE) {
    req->rq_rpi->cq_peer = env->ce_rank;
    req->rq_rpi->cq_envbuf->ge_env.ce_rank = env->ce_rank;
	
    g = (LAM_IS_INTER(req->rq_comm)) ?
      req->rq_comm->c_rgroup : req->rq_comm->c_group;

    req->rq_proc = g->g_procs[req->rq_rpi->cq_peer];
  }
}
