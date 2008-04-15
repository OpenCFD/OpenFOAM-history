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
 *	$Id: datasyncack.c,v 1.5 2003/05/05 21:45:46 ssankara Exp $
 *
 *	Function:	- reply to an ACK from the impid (if we 
 *                        haven't already)
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <impi-defs.h> 
#include <impi.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

/*
 * local variables
 */
static int fl_debug = 0;


/*
 * local routines
 */
static int lam_impi_datasync_done_handler(MPI_Request req);

/*
 *	lam_impi_ping_ack_handler
 *
 *	Function:	- send back a ping to the impid
 *                      - called from within the RPI when a ping request 
 *                        completes
 *      Accepts:        - request of ping that completed
 *      Returns:        - MPI_SUCCESS on success, err otherwise
 */
int
lam_impi_datasync_handler(MPI_Request req)
{
  int err;
  IMPI_Uint8 *buf;
  MPI_Request ack = MPI_REQUEST_NULL;

  DBUG("LAM rank in lam_impi_ping_ack_handler: req %p\n", req);

  if ((req->rq_flags & LAM_RQFACKDONE) != 0 ||
      (req->rq_flags & LAM_RQFCANCEL) != 0)
    return MPI_SUCCESS;

  /* Since we're basing this off a request that we can't assume will
     die after this new request, we have to malloc/copy the drqid that
     was received.  */

  buf = (IMPI_Uint8*) malloc(sizeof(IMPI_Uint8));
  lam_memcpy(buf, req->rq_buf, sizeof(IMPI_Uint8));
  err = _mpi_req_build(buf, 1, IMPI_TYPE_UINT8, 
		       impid_comm->c_group->g_nprocs - 1,
		       IMPI_SYNCACK_TAG, impid_comm, LAM_RQISEND, &ack);
  if (err != MPI_SUCCESS) 
    return(err);

  ack->rq_marks |= (LAM_RQFMAND | LAM_RQFDYNBUF);

  err = _mpi_req_start(ack);
  if (err != MPI_SUCCESS) 
    return(err);

  DBUG("LAM rank queued up an ACK to go back to the IMPID: %lld\n", *buf);
  DBUG("LAM ACK req: %p\n", ack);
  
  if (lam_rq_flblock == 1) {
    DBUG("LAM rank queued ACK to be blocking\n");
    _mpi_req_blkset_m(ack);
  }

  ack->rq_flags |= LAM_RQFORPHAN;
  ack->rq_hdlr = lam_impi_datasync_done_handler;
  ack->rq_extra = req;
  _mpi_req_add(ack);

  req->rq_flags |= LAM_RQFACKDONE;
  req->rq_extra = (void*) ack;

  return MPI_SUCCESS;
}


/*
 *	lam_impi_ping_ack_done_handler
 *
 *	Function:	- reap a ping ack that was previously orphaned
 *                      - called from within the RPI when a ping request 
 *                        completes
 *      Accepts:        - request of ping that completed
 *      Returns:        - MPI_SUCCESS on success, err otherwise
 */
static int
lam_impi_datasync_done_handler(MPI_Request req)
{
  DBUG("LAM rank In lam_impi_ping_ack_done_handler: %p\n", req);
  if (req->rq_extra != 0) 
    ((MPI_Request) req->rq_extra)->rq_extra = 0;

  return MPI_SUCCESS;
}

#endif /* LAM_WANT_IMPI */


