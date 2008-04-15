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
 *	$Id: ssi_rpi_util.c,v 1.4 2003/02/10 23:07:33 jsquyres Exp $
 *
 *	Function:	- GM client-to-client interface
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lam-ssi-rpi.h>
#include <lamdebug.h>


/*
 * public variables
 */

int lam_ssi_rpi_haveadv = 0;
int lam_ssi_rpi_flblock = 0;


/*
 *	lam_ssi_rpi_fill_mpi_status
 *
 *	Function:	- fill in the MPI status object
 *	Accepts:	- request
 *			- rank
 *			- tag
 *			- message length
 */
void
lam_ssi_rpi_fill_mpi_status(MPI_Request req, int rank, int tag, int length)
{
  req->rq_status.MPI_SOURCE = rank;
  req->rq_status.MPI_TAG = tag;
  req->rq_status.st_length = length;
}


/*
 *	lam_ssi_rpi_envl_cmp
 *
 *	Function:	- check if envelopes match
 *			- second envelope may contain wildcards and first
 *			  may not
 *	Accepts:	- ptr to envelope
 *			- ptr to request envelope
 *	Returns:	- 0 if match, 1 if not
 */
int
lam_ssi_rpi_envl_cmp(struct lam_ssi_rpi_envl *p1, struct lam_ssi_rpi_envl *p2)
{
#if LAM_WANT_DEBUG && 0
  extern int lam_rpi_gm_did;
  lam_debug_cond((lam_rpi_gm_did, 
		  "c2c_envl_cmp %p: p1: %d %d %d 0x%x\n",
		  p1, p1->ce_cid, p1->ce_rank, p1->ce_tag, 
		  p1->ce_flags));
  lam_debug_cond((lam_rpi_gm_did, 
		  "c2c_envl_cmp %p: p2: %d %d %d 0x%x\n",
		  p2, p2->ce_cid, p2->ce_rank, p2->ce_tag, 
		  p2->ce_flags));
#endif

  if ((p1->ce_cid == p2->ce_cid) &&
      ((p1->ce_rank == p2->ce_rank) || (p2->ce_rank == MPI_ANY_SOURCE)) &&
      ((p1->ce_tag == p2->ce_tag) || (p2->ce_tag == MPI_ANY_TAG)) &&
      ((p1->ce_flags & C2CACK) == (p2->ce_flags & C2CACK)) &&
      ((p1->ce_flags & C2C2ND) == (p2->ce_flags & C2C2ND)))
    return(0);

  return(1);
}


/*
 *      lam_ssi_rpi_comm_dead
 *
 *      Function:       - sets dead communicator error for request
 *      Accepts:        - request
 *      Returns:        - 1
 */
int
lam_ssi_rpi_comm_dead(MPI_Request req)
{
  if (req->rq_state != LAM_RQSDONE && req->rq_state != LAM_RQSINIT) {
    lam_rq_nactv--;
  }

  req->rq_state = LAM_RQSDONE;
  lam_ssi_rpi_haveadv = 1;

  if (req->rq_comm->c_flags & LAM_CLDEAD) {
    req->rq_status.MPI_ERROR = lam_mkerr(MPI_ERR_LOCALDEAD, 0);
  } else {
    req->rq_status.MPI_ERROR = lam_mkerr(MPI_ERR_REMOTEDEAD, 0);
  }

  return(1);
}
