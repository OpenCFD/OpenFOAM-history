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
 *	$Id: lamgiappe.c,v 1.6 2002/10/09 20:57:17 brbarret Exp $
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
#include <lamdebug.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <blktype.h>

/*
 * private varaibles
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
 * Accepts:     - whether req is LAM_RQISSEND or not
 *              - count of the message
 *              - datatype of the message
 *              - destination of the message
 *              - tag of the message
 *              - communicator of the message
 *              - ptr to a MPI_Request to fill
 * Returns:     - 0 on success, LAMERROR on failure
 */
int
IMPI_Fill_lamgiappe(int reqtype, int count, MPI_Datatype dtype,
		    int dest, int tag, MPI_Comm comm, 
		    IMPI_Lamgiappe *lamgiappe)
{
  LAM_ZERO_ME(*lamgiappe);
  if (LAM_IS_IMPI(comm) &&
      lam_impid_proc != 0 &&
      LAM_GPSCMP(&(comm->c_group->g_procs[dest]->p_gps),
		 &(lam_impid_proc->p_gps)) == 0) {
    int src = comm->c_group->g_myrank;

    DBUG("Proxy send to the impid\n");

    if (comm == MPI_COMM_WORLD) {
      lamgiappe->world_src_rank  = src;
      lamgiappe->world_dest_rank = dest;
    } else {
      lamgiappe->world_src_rank  = 
	comm->c_group->g_procs[src]->p_gps.gps_grank;
      lamgiappe->world_dest_rank =
	comm->c_group->g_procs[dest]->p_gps.gps_grank;
    }
    
    /* We could calculate this in the impid, but doing it here prevents
       some looping */
    
    lamgiappe->local_src_rank = lamgiappe->world_src_rank - 
      impi_server_info.start_rank;
    DBUG("Constructing lamgiappe: local_src_rank = %d, start_rank = %d\n",
	 lamgiappe->local_src_rank, impi_server_info.start_rank);
    DBUG("Constructing lamgiappe: world_src_rank = %d\n",
	 lamgiappe->world_src_rank);
    DBUG("Constructing lamgiappe: world_dest_rank = %d\n",
	 lamgiappe->world_dest_rank);
    
    lamgiappe->src_rank = src;
    lamgiappe->cid = comm->c_contextid;
    lamgiappe->dest_rank = dest;
    lamgiappe->tag = tag;
    lamgiappe->length = count * dtype->dt_size;
    lamgiappe->is_ssend = (reqtype == LAM_RQISSEND);
  } else
    lamgiappe->cid = -1;

  return 0;
}

#endif
