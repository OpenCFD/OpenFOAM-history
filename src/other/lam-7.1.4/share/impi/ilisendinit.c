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
 *	$Id: ilisendinit.c,v 1.7 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- in the case of the msg being for an IMPI host send 
 *                        to the impid the meta-info in an IMPI_Lamgiappe msg
 *                        and then forward the msg on to the impid for routing 
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <blktype.h>


/*
 * Local variables
 */
static int fl_debug = 0;


/*
 * IMPI_Isend_lamgiappe
 *
 * Function:    - Send an IMPI_Lamgiappe meta information message to the 
 *                impid, persistent
 *              - fills MPI_Request with MPI_REQUEST_NULL if not a proxy
 *                send to the impid
 *
 * Accepts:     - the type of the request
 *              - count of the message
 *              - datatype of the message
 *              - destination of the message
 *              - tag of the message
 *              - communicator of the message
 *              - ptr to a MPI_Request to fill
 * Returns:     - 0 on success, LAMERROR on failure
 */
int
IMPI_Isend_lamgiappe_init(int reqtype, int count, MPI_Datatype dtype,
			  int dest, int tag, MPI_Comm comm, MPI_Request *req)
{
  int err;

  *req = MPI_REQUEST_NULL;

  if (LAM_IS_IMPI(comm) &&
      lam_impid_proc != 0 &&
      LAM_GPSCMP(&(comm->c_group->g_procs[dest]->p_gps),
		 &(lam_impid_proc->p_gps)) == 0) {
    int impid_rank = (impid_comm->c_group->g_nprocs) - 1;
    IMPI_Lamgiappe *lamgiappe = malloc(sizeof(IMPI_Lamgiappe));
    IMPI_Fill_lamgiappe(reqtype, count, dtype, dest, tag, comm, lamgiappe);

    DBUG("IMPI_Isend_Lamgiappe_init starting\n");

    /* Send the lamgiappe to the impid giving the impid all the meta
     * info to recv the msg */

    err = _mpi_req_build(lamgiappe, 1, IMPI_LAMGIAPPE, impid_rank, 
			 IMPI_LAMGIAPPE_TAG, impid_comm, LAM_RQISEND, req);
    if (err != MPI_SUCCESS) 
      return(err);

    (*req)->rq_marks |= (LAM_RQFDYNBUF | LAM_RQFPERSIST | LAM_RQFMAND);

    /* If this is an ISSEND, we need to post *another* receive (as a
       shadow to this one) for the sync ACK that we'll get back from
       the impid.  Set the RQFMAND mark so that MPI_Wait*() /
       MPI_Test*() will wait for the shadow to complete before saying
       that the whole request has finished. */

    if (reqtype == LAM_RQISSEND) {
      DBUG("IMPI_Isend_Lamgiappe_init registering for ping\n");
      err = IMPI_Register_syncack_init(dest, tag, comm, &((*req)->rq_shadow));
      if (err != MPI_SUCCESS)
	return err;
    }
    DBUG("IMPI_Isend_Lamgiappe_init done\n");
  }

  return MPI_SUCCESS;
}

#endif
