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
 *	$Id: ilsend.c,v 1.14 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- in the case of the msg being for an IMPI host send 
 *                        to the impid the meta-info in an IMPI_Lamgiappe msg
 *                        and then forward the msg on to the impid for routing 
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <blktype.h>
#include <lamdebug.h>


/*
 * private variables
 */
static int fl_debug = 0;


/*
 * IMPI_Send_lamgiappe
 *
 * Function:    - Send an IMPI_Lamgiappe meta information message to the 
 *                impid
 *
 * Accepts:     - request type
 *              - count of the message
 *              - datatype of the message
 *              - destination of the message
 *              - tag of the message
 *              - communicator of the message
 *              - ptr to flag to fill if this is an IMPI request
 * Returns:     - MPI_SUCCESS on success, err otherwise
 *
 */
int
IMPI_Send_lamgiappe(int reqtype, int count, MPI_Datatype dtype, 
		    int dest, int tag, MPI_Comm comm, int *flag)
{
  *flag = 0;
  DBUG("IMPI_Send_lamgiappe\n");
  if (LAM_IS_IMPI(comm) &&
      lam_impid_proc != 0 &&
      LAM_GPSCMP(&(comm->c_group->g_procs[dest]->p_gps),
		 &(lam_impid_proc->p_gps)) == 0) {
    int err;
    MPI_Request req = MPI_REQUEST_NULL;
    int impid_rank = (impid_comm->c_group->g_nprocs) - 1;

    IMPI_Lamgiappe *lamgiappe = malloc(sizeof(IMPI_Lamgiappe));
    IMPI_Fill_lamgiappe(reqtype, count, dtype, dest, tag, comm, lamgiappe);

    /* Send the lamgiappe to the impid giving the impid all the meta
     * info to recv the msg */

    *flag = 1;
    DBUG("IMPI_Send_lamgiappe: building request\n"); 
    switch (reqtype) {
    case LAM_RQIRSEND:
      /* This type is ok */
      err = _mpi_req_build(lamgiappe, 1, IMPI_LAMGIAPPE, impid_rank, 
			   IMPI_LAMGIAPPE_TAG, impid_comm, reqtype, &req);
      break;

    case LAM_RQISEND:
    case LAM_RQISSEND:
      err = _mpi_req_build(lamgiappe, 1, IMPI_LAMGIAPPE, impid_rank,
			   IMPI_LAMGIAPPE_TAG, impid_comm, LAM_RQISEND, &req);
      break;

    case LAM_RQIBSEND:
    case LAM_RQIFAKE:
    case LAM_RQIRECV:
    case LAM_RQIPROBE:
      /* These should never get called here */
      err = (LAMERROR);
      break;
    }
    if (err != MPI_SUCCESS) 
      return(err);
    DBUG("IMPI_Send_lamgiappe: request built\n");

    req->rq_marks |= (LAM_RQFDYNBUF | LAM_RQFMAND);

    /* Send the request, and then ditch it.  We're blocking, so this
       is ok to do */

    DBUG("IMPI_Send_lamgiappe: starting request\n");
    err = _mpi_req_start(req);
    if (err != MPI_SUCCESS) 
      return(err);
    DBUG("IMPI_Send_lamgiappe: adding request\n");
    _mpi_req_add(req);
    DBUG("IMPI_Send_lamgiappe: clearing blocking\n");
    _mpi_req_blkclr();
    _mpi_req_blkset_m(req);
    err = _mpi_req_advance();
    if (err != MPI_SUCCESS) 
      return(err);
    _mpi_req_rem_m(req);
    err = _mpi_req_destroy(&req);
    if (err != MPI_SUCCESS) 
      return(err);
    
    /* This will never be an issend -- only blocking calls
       (e.g,. MPI_Send, MPI_Ssend, etc.) are made to this function.
       And MPI_Ssend will take care of its own ping.  Hence, we don't
       post and wait for an additional ping/ACK here. */

    DBUG("IMPI_Send_lamgiappe: done\n");
  }

  return (MPI_SUCCESS);
}

#endif
