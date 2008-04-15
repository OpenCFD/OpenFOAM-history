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
 *	$Id: ssi_rpi_ib_recv_queue.c,v 1.1 2003/11/11 18:36:55 vsahay Exp $
 *
 *	Function:	- maintain pending receive and ack queues
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_reqlist.h>
#include <rpi_ib_proc.h>

/*
 * Queue up a request to be received from a specific MPI process.
 * This queue is monitored/advanced [indirectly] in lam_ssi_rpi_ib_advance().
 */
int
lam_ssi_rpi_ib_add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "add_read: expecting read from process %d (proc %p, req %p)",
                  ps->cp_proc->p_gps.gps_grank, &(req->rq_rpi), req));

  if (lam_ssi_rpi_ib_reqlist_append(&(ps->cp_pending_recvs), req) != 0)
    return LAMERROR;

  req->rq_rpi->is_advancing = 1;

  return 0;
}


/*
 * Queue up a request to be received from any MPI process in the
 * comunicator.  This queue is monitored/advanced [indirectly] in
 * lam_ssi_rpi_ib_advance().
 */
int
lam_ssi_rpi_ib_add_read_any_src(MPI_Request req)
{
  int i;
  MPI_Comm comm = req->rq_comm;
  MPI_Group group;
  struct _proc **procs;

  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "add_read_any: expecting read from any process, "
                  "req %p", req));

  /* If an intercommunicator, use the remote group.  Otherwise, use
     the local group */

  if (comm->c_flags & LAM_CINTER)
    group = comm->c_rgroup;
  else
    group = comm->c_group;

  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "add_read_any_src: got %d procs in this group",
                  group->g_nprocs));
  for (i = 0, procs = group->g_procs; i < group->g_nprocs; ++i) {
    if (lam_ssi_rpi_ib_reqlist_append(&(procs[i]->p_rpi->cp_pending_recvs), 
				  req) != 0)
      return LAMERROR;
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "add_read_any: added for proc %p\n", 
                    procs[i]->p_rpi));
  }

  req->rq_rpi->is_advancing = 1;

  return 0;
}


/*
 * Remove a MPI_ANY_SOURCE request from the pending receive queue of
 * all processes in the communicator, with the exception of the one
 * proc (this is for the case where a long message starts to be
 * received and we want to remove the request from all queues *except*
 * the one where it is actually being received from).
 */
int
lam_ssi_rpi_ib_rm_read_any_src(MPI_Request req, struct _proc *exception)
{
  int i;
  MPI_Comm comm = req->rq_comm;
  MPI_Group group;
  struct _proc **procs;

  lam_debug_cond((lam_ssi_rpi_ib_did, "rm_read_any_src, req %p", req));

  /* If an intercommunicator, use the remote group.  Otherwise, use
     the local group */

  if (comm->c_flags & LAM_CINTER)
    group = comm->c_rgroup;
  else
    group = comm->c_group;

  for (i = 0, procs = group->g_procs; i < group->g_nprocs; ++i) {
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "rm_read_any_src: removing from proc %d %p",
                    i, procs[i]));
    if (procs[i] != exception &&
	lam_ssi_rpi_ib_reqlist_rm_req(&(procs[i]->p_rpi->cp_pending_recvs), 
				      req) != 1)
      return LAMERROR;
  }

  lam_debug_cond((lam_ssi_rpi_ib_did, "rm_read_any_src successful"));
  return 0;
}
