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
 * $Id: ssi_coll_lam_basic.c,v 1.13 2003/10/23 23:11:12 adharurk Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpisys.h>

static const lam_ssi_coll_actions_1_1_0_t lam_basic_inter_functions = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_lam_basic_init,
  lam_ssi_coll_lam_basic_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  lam_ssi_coll_lam_basic_allgather_inter,
  NULL,
  lam_ssi_coll_lam_basic_allreduce_inter,
  lam_ssi_coll_lam_basic_alltoall_inter,
  lam_ssi_coll_lam_basic_alltoallv_inter,
  lam_ssi_coll_lam_basic_alltoallw_inter,
  lam_ssi_coll_lam_basic_barrier_inter,
  1,
  lam_ssi_coll_lam_basic_bcast_inter,
  NULL, 
  lam_ssi_coll_lam_basic_gather_inter,
  lam_ssi_coll_lam_basic_gatherv_inter,
  1,
  lam_ssi_coll_lam_basic_reduce_inter,
  lam_ssi_coll_lam_basic_reduce_scatter_inter,
  NULL,
  lam_ssi_coll_lam_basic_scatter_inter,
  lam_ssi_coll_lam_basic_scatterv_inter,
};

/*
 * Linear set of collective algorithms for the other-than-the-lamd RPI
 * (ugh)
 */
static const lam_ssi_coll_actions_1_1_0_t linear = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_lam_basic_init,
  lam_ssi_coll_lam_basic_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  lam_ssi_coll_lam_basic_allgather,
  lam_ssi_coll_lam_basic_allgatherv,
  lam_ssi_coll_lam_basic_allreduce,
  lam_ssi_coll_lam_basic_alltoall,
  lam_ssi_coll_lam_basic_alltoallv,
  lam_ssi_coll_lam_basic_alltoallw,
  lam_ssi_coll_lam_basic_barrier_lin,
  1,
  lam_ssi_coll_lam_basic_bcast_lin,
  lam_ssi_coll_lam_basic_exscan,
  lam_ssi_coll_lam_basic_gather,
  lam_ssi_coll_lam_basic_gatherv,
  1,
  lam_ssi_coll_lam_basic_reduce_lin,
  lam_ssi_coll_lam_basic_reduce_scatter,
  lam_ssi_coll_lam_basic_scan,
  lam_ssi_coll_lam_basic_scatter,
  lam_ssi_coll_lam_basic_scatterv,
};


/*
 * Linear set of collective algorithms for the lamd RPI (ugh)
 */
static const lam_ssi_coll_actions_1_1_0_t linear_lamd = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_lam_basic_init,
  lam_ssi_coll_lam_basic_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  lam_ssi_coll_lam_basic_allgather,
  lam_ssi_coll_lam_basic_allgatherv,
  lam_ssi_coll_lam_basic_allreduce,
  lam_ssi_coll_lam_basic_alltoall_lamd,
  lam_ssi_coll_lam_basic_alltoallv_lamd,
  lam_ssi_coll_lam_basic_alltoallw_lamd,
  lam_ssi_coll_lam_basic_barrier_lin,
  1,
  lam_ssi_coll_lam_basic_bcast_lin_lamd,
  lam_ssi_coll_lam_basic_exscan, 
  lam_ssi_coll_lam_basic_gather,
  lam_ssi_coll_lam_basic_gatherv,
  1,
  lam_ssi_coll_lam_basic_reduce_lin,
  lam_ssi_coll_lam_basic_reduce_scatter,
  lam_ssi_coll_lam_basic_scan,
  lam_ssi_coll_lam_basic_scatter,
  lam_ssi_coll_lam_basic_scatterv,
};


/*
 * Lograthmic set of collective algorithms.  Note that not all
 * collectives have lograthmic algorithms.  For example, scan will use
 * the same algorithm as in the linear set.
 */
static const lam_ssi_coll_actions_1_1_0_t log = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_lam_basic_init,
  lam_ssi_coll_lam_basic_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  lam_ssi_coll_lam_basic_allgather,
  lam_ssi_coll_lam_basic_allgatherv,
  lam_ssi_coll_lam_basic_allreduce,
  lam_ssi_coll_lam_basic_alltoall,
  lam_ssi_coll_lam_basic_alltoallv,
  lam_ssi_coll_lam_basic_alltoallw,
  lam_ssi_coll_lam_basic_barrier_log,
  1,
  lam_ssi_coll_lam_basic_bcast_log,
  lam_ssi_coll_lam_basic_exscan,
  lam_ssi_coll_lam_basic_gather,
  lam_ssi_coll_lam_basic_gatherv,
  1,
  lam_ssi_coll_lam_basic_reduce_log,
  lam_ssi_coll_lam_basic_reduce_scatter,
  lam_ssi_coll_lam_basic_scan,
  lam_ssi_coll_lam_basic_scatter,
  lam_ssi_coll_lam_basic_scatterv,
};


/*
 * Lograthmic set of collective algorithms for the lamd RPI (ugh!)
 * (same disclaimers as above)
 */
static const lam_ssi_coll_actions_1_1_0_t log_lamd = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_lam_basic_init,
  lam_ssi_coll_lam_basic_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  lam_ssi_coll_lam_basic_allgather,
  lam_ssi_coll_lam_basic_allgatherv,
  lam_ssi_coll_lam_basic_allreduce,
  lam_ssi_coll_lam_basic_alltoall_lamd,
  lam_ssi_coll_lam_basic_alltoallv_lamd,
  lam_ssi_coll_lam_basic_alltoallw_lamd, 
  lam_ssi_coll_lam_basic_barrier_log,
  1,
  lam_ssi_coll_lam_basic_bcast_log_lamd,
  lam_ssi_coll_lam_basic_exscan,
  lam_ssi_coll_lam_basic_gather,
  lam_ssi_coll_lam_basic_gatherv,
  1,
  lam_ssi_coll_lam_basic_reduce_log,
  lam_ssi_coll_lam_basic_reduce_scatter,
  lam_ssi_coll_lam_basic_scan,
  lam_ssi_coll_lam_basic_scatter,
  lam_ssi_coll_lam_basic_scatterv,
};

/*
 * Initial query function that is invoked during MPI_INIT, allowing
 * this module to indicate what level of thread support it provides.
 */
int 
lam_ssi_coll_lam_basic_thread_query(int *thread_min, int *thread_max)
{
  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_MULTIPLE;
  
  return 0;
}


/*
 * Invoked when there's a new communicator that has been created.
 * Look at the communicator and decide which set of functions and
 * priority we want to return.
 */
const lam_ssi_coll_actions_1_1_0_t *
lam_ssi_coll_lam_basic_query(MPI_Comm comm, int *priority)
{
  int size;
  
  *priority = 0;

  /* for MPI_COMM_SELF, ensure that lam_basic is selected. We need to fix this.
   * The list for local_comm should be created at the module open time so that
   * the action is not tied to any other activity
   */
  if (comm == MPI_COMM_SELF)
    *priority = 100;

  /* Make lam_basic_data pointer NULL.  This pointer will be NON-NULL only
     if the comm is INTERCOMM and any of the lam_basic functions are going to 
     be used */
  comm->c_ssi_coll_lam_basic_data = NULL;

  /* This is horrible, and a break in the abstraction, but we have to
     know whether we're using the lamd RPI or not.  :-( We also get to
     choose whether to use linear or log-based algorithms. */

  /* From query return the intracommunicator or intercommunicator pointers
     depending upon the whether the communicator is inter or intar */

  if (LAM_IS_INTRA(comm)) {
    /* intracommunicator */
    MPI_Comm_size(comm, &size);
    if (size <= lam_ssi_coll_base_crossover) {
      if (RPI_IS_LAMD() == 1)
	return &linear_lamd;
      else
	return &linear;
    } else {
      if (RPI_IS_LAMD() == 1)
	return &log_lamd;
      else
	return &log;
    }
  } else {
    /* intercommunicator */
    /* We need to setup the local_comm for passed intercomm. This must be done
       during query function so that the lam_basic functions can use this 
       local_comm even if some other module is selected 
       First make c_ssi_coll point to lam_basic *intracommunicator* functions
       so that intercomm_merge function can use them. Since intercomm merge
       is changed so that it does not use any collective, this step is not at 
       all necessary. But I am still assigning some set of pointers so comm 
       instead of keeping them hanging.
    */
    if (size <= lam_ssi_coll_base_crossover) {
      if (RPI_IS_LAMD() == 1)
	comm->c_ssi_coll = linear_lamd;
      else
	comm->c_ssi_coll = linear;
    } else {
      if (RPI_IS_LAMD() == 1)
	comm->c_ssi_coll = log_lamd;
      else
	comm->c_ssi_coll = log;
    }

    /* Now set up the local_comm for intercomm */
    setup_local_comm_for_intercomm(comm);

    /* set-up over, return *inter* comm functions */
    return &lam_basic_inter_functions;
  }
}

int
lam_ssi_coll_lam_basic_end_query(MPI_Comm comm) {
  return 0;
}

int
lam_ssi_coll_lam_basic_init(MPI_Comm comm, 
			    const lam_ssi_coll_actions_1_1_0_t **new_actions)
{
  /* For implementing intercomm design, we have two pointers on each comm. 
     First pointer stores the local data structure which a module wants and 
     is generally created during module init.  In case of intracomm 
     or intercomm, we know that lam_basic does not want any special data so
     this pointer is NULL. 
     In case of intercomm, the second pointer maintains the local_comm 
     associated with the intercomm.  This pointer won't be NULL in cases when
     any of the lam_basic intercomm functions are going to be used.  
  */

  comm->c_ssi_coll_data = NULL;

  if (LAM_IS_INTER(comm)) {

    /* Reassign new pointers */
    *new_actions = &lam_basic_inter_functions;
  }

  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "lam_basic: init communicator %s",
	      comm->c_name);
  return 0;
}


int
lam_ssi_coll_lam_basic_finalize(MPI_Comm comm)
{
  /* Nothing to finalize on the communicator */
  lam_ssi_coll_lam_basic_data_t* lblcd;
  if (LAM_IS_INTER(comm)) {
    /* We are going to free the lcd structure associated with intercomm only if
     we are sure that lam_basic will not be used for any of the intercomm
     algorithms.  Even if user module *foo* is going to use a single method 
     from lam_basic, we need to keep this structure and it will be destroyed
     during intercomm finalise */

    lblcd = comm->c_ssi_coll_lam_basic_data; 
    if (lblcd != NULL) {
      if (MPI_Comm_free(&(lblcd->local_comm)) == LAMERROR)
	return LAMERROR;
      free(lblcd);
    }
  }
  
  if (lam_ssi_coll_lam_basic_end_query(comm) == LAMERROR)
    return LAMERROR;
  
  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "lam_basic: finalize communicator %s",
	      comm->c_name);
  return 0;
}
