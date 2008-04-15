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
 * $Id: lam-ssi-coll.h,v 1.9 2004/03/09 16:49:39 adharurk Exp $
 *
 *	Function:	- SSI collective interface
 */

#ifndef LAM_SSI_COLL_H
#define LAM_SSI_COLL_H

#include <lam_config.h>

#include <mpi.h>
#include <all_list.h>
#include <lam-ssi.h>


/*
 * Types for each API function
 */

typedef int (*lam_ssi_coll_thread_query_fn_t)(int *thread_min,
                                              int *thread_max);
typedef const struct lam_ssi_coll_actions_1_0_0 *
  (*lam_ssi_coll_query_1_0_0_fn_t)(MPI_Comm comm, int *priority);
typedef const struct lam_ssi_coll_actions_1_1_0 *
  (*lam_ssi_coll_query_1_1_0_fn_t)(MPI_Comm comm, int *priority);
typedef int (*lam_ssi_coll_end_query_fn_t)(MPI_Comm comm);

typedef int 
  (*lam_ssi_coll_init_1_0_0_fn_t)
  (MPI_Comm comm, const struct lam_ssi_coll_actions_1_0_0 **new_actions);
typedef int 
  (*lam_ssi_coll_init_1_1_0_fn_t)
  (MPI_Comm comm, const struct lam_ssi_coll_actions_1_1_0 **new_actions);
typedef int (*lam_ssi_coll_finalize_fn_t)(MPI_Comm comm);

typedef int (*lam_ssi_coll_checkpoint_fn_t)(MPI_Comm comm);
typedef int (*lam_ssi_coll_continue_fn_t)(MPI_Comm comm);
typedef int (*lam_ssi_coll_restart_fn_t)(MPI_Comm comm);
typedef int (*lam_ssi_coll_interrupt_fn_t)(void);

typedef int (*lam_ssi_coll_allgather_fn_t)(void *sbuf, int scount, 
					   MPI_Datatype sdtype, void *rbuf, 
					   int rcount, MPI_Datatype rdtype, 
					   MPI_Comm comm);
typedef int (*lam_ssi_coll_allgatherv_fn_t)(void *sbuf, int scount, 
					    MPI_Datatype sdtype, void * rbuf, 
					    int *rcounts, int *disps, 
					    MPI_Datatype rdtype, 
					    MPI_Comm comm);
typedef int (*lam_ssi_coll_allreduce_fn_t)(void *sbuf, void *rbuf, int count, 
					   MPI_Datatype dtype, MPI_Op op, 
					   MPI_Comm comm);
typedef int (*lam_ssi_coll_alltoall_fn_t)(void *sbuf, int scount, 
					  MPI_Datatype sdtype, void* rbuf, 
					  int rcount, MPI_Datatype rdtype, 
					  MPI_Comm comm);
typedef int (*lam_ssi_coll_alltoallv_fn_t)(void *sbuf, int *scounts, 
					   int *sdisps, MPI_Datatype sdtype, 
					   void *rbuf, int *rcounts, 
					   int *rdisps, MPI_Datatype rdtype, 
					   MPI_Comm comm);
typedef int (*lam_ssi_coll_alltoallw_fn_t)(void *sbuf, int *scounts, 
					   int *sdisps, MPI_Datatype *sdtypes, 
					   void *rbuf, int *rcounts, 
					   int *rdisps, MPI_Datatype *rdtypes, 
					   MPI_Comm comm);
typedef int (*lam_ssi_coll_barrier_fn_t)(MPI_Comm comm);
typedef int (*lam_ssi_coll_bcast_fn_t)(void *buff, int count, 
				       MPI_Datatype datatype, int root, 
				       MPI_Comm comm);
typedef int (*lam_ssi_coll_exscan_fn_t)(void *sbuf, void *rbuf, int count, 
					MPI_Datatype dtype, MPI_Op op, 
					MPI_Comm comm);
typedef int (*lam_ssi_coll_gather_fn_t)(void *sbuf, int scount, 
					MPI_Datatype sdtype, void *rbuf, 
					int rcount, MPI_Datatype rdtype, 
					int root, MPI_Comm comm);
typedef int (*lam_ssi_coll_gatherv_fn_t)(void *sbuf, int scount, 
					 MPI_Datatype sdtype, void *rbuf, 
					 int *rcounts, int *disps, 
					 MPI_Datatype rdtype, int root, 
					 MPI_Comm comm);
typedef int (*lam_ssi_coll_reduce_fn_t)(void *sbuf, void* rbuf, int count, 
					MPI_Datatype dtype, MPI_Op op, 
					int root, MPI_Comm comm);
typedef int (*lam_ssi_coll_reduce_scatter_fn_t)(void *sbuf, void *rbuf, 
						int *rcounts, 
						MPI_Datatype dtype, 
						MPI_Op op, MPI_Comm comm);
typedef int (*lam_ssi_coll_scan_fn_t)(void *sbuf, void *rbuf, int count, 
				      MPI_Datatype dtype, MPI_Op op, 
				      MPI_Comm comm);
typedef int (*lam_ssi_coll_scatter_fn_t)(void *sbuf, int scount, 
					 MPI_Datatype sdtype, void *rbuf, 
					 int rcount, MPI_Datatype rdtype, 
					 int root, MPI_Comm comm);
typedef int (*lam_ssi_coll_scatterv_fn_t)(void *sbuf, int *scounts, 
					  int *disps, MPI_Datatype sdtype, 
					  void* rbuf, int rcount, 
					  MPI_Datatype rdtype, int root, 
					  MPI_Comm comm);


/*
 * Struct used to pass coll SSI information from the each SSI instance
 * back to the SSI glue.
 */

typedef struct lam_ssi_coll_1_0_0 {
  lam_ssi_t lsc_meta_info;

  /* Initialization / querying functions */

  lam_ssi_coll_thread_query_fn_t lsc_thread_query;
  lam_ssi_coll_query_1_0_0_fn_t lsc_query;
  lam_ssi_coll_end_query_fn_t lsc_end_query;

  /* Flags */

  int lsc_has_checkpoint;
} lam_ssi_coll_1_0_0_t;


/*
 * This struct is hung on the communicator (struct _comm) by the
 * winning coll module after the negotiation.  It has pointers for all
 * the collective functions, as well as a "finalize" function for when
 * the communicator is freed.
 */

typedef struct lam_ssi_coll_actions_1_0_0 {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_init_1_0_0_fn_t lsca_init;
  lam_ssi_coll_finalize_fn_t lsca_finalize;

  /* Checkpoint / restart functions */
  
  lam_ssi_coll_checkpoint_fn_t lsca_checkpoint;
  lam_ssi_coll_continue_fn_t lsca_continue;
  lam_ssi_coll_restart_fn_t lsca_restart;

  /* Collective function pointers */

  lam_ssi_coll_allgather_fn_t lsca_allgather;
  lam_ssi_coll_allgatherv_fn_t lsca_allgatherv;
  lam_ssi_coll_allreduce_fn_t lsca_allreduce;
  lam_ssi_coll_alltoall_fn_t lsca_alltoall;
  lam_ssi_coll_alltoallv_fn_t lsca_alltoallv;
  lam_ssi_coll_alltoallw_fn_t lsca_alltoallw;
  lam_ssi_coll_barrier_fn_t lsca_barrier;
  int lsca_bcast_optimization;
  lam_ssi_coll_bcast_fn_t lsca_bcast;
  lam_ssi_coll_exscan_fn_t lsca_exscan;
  lam_ssi_coll_gather_fn_t lsca_gather;
  lam_ssi_coll_gatherv_fn_t lsca_gatherv;
  int lsca_reduce_optimization;
  lam_ssi_coll_reduce_fn_t lsca_reduce;
  lam_ssi_coll_reduce_scatter_fn_t lsca_reduce_scatter;
  lam_ssi_coll_scan_fn_t lsca_scan;
  lam_ssi_coll_scatter_fn_t lsca_scatter;
  lam_ssi_coll_scatterv_fn_t lsca_scatterv;

} lam_ssi_coll_actions_1_0_0_t;


/*
 * Ver 1.1.0
 * Changes:
 *
 * - New type for lsc_query (different return type; return new 1_1_0
 *   actions struct pointer)
 * - New lsca_interrupt action function
 */

typedef struct lam_ssi_coll_1_1_0 {
  lam_ssi_t lsc_meta_info;

  /* Initialization / querying functions */

  lam_ssi_coll_thread_query_fn_t lsc_thread_query;
  lam_ssi_coll_query_1_1_0_fn_t lsc_query;
  lam_ssi_coll_end_query_fn_t lsc_end_query;

  /* Flags */

  int lsc_has_checkpoint;
} lam_ssi_coll_1_1_0_t;


/*
 * This struct is hung on the communicator (struct _comm) by the
 * winning coll module after the negotiation.  It has pointers for all
 * the collective functions, as well as a "finalize" function for when
 * the communicator is freed.
 */

typedef struct lam_ssi_coll_actions_1_1_0 {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_init_1_1_0_fn_t lsca_init;
  lam_ssi_coll_finalize_fn_t lsca_finalize;

  /* Checkpoint / restart functions */

  lam_ssi_coll_checkpoint_fn_t lsca_checkpoint;
  lam_ssi_coll_continue_fn_t lsca_continue;
  lam_ssi_coll_restart_fn_t lsca_restart;
  lam_ssi_coll_interrupt_fn_t lsca_interrupt;

  /* Collective function pointers */

  lam_ssi_coll_allgather_fn_t lsca_allgather;
  lam_ssi_coll_allgatherv_fn_t lsca_allgatherv;
  lam_ssi_coll_allreduce_fn_t lsca_allreduce;
  lam_ssi_coll_alltoall_fn_t lsca_alltoall;
  lam_ssi_coll_alltoallv_fn_t lsca_alltoallv;
  lam_ssi_coll_alltoallw_fn_t lsca_alltoallw;
  lam_ssi_coll_barrier_fn_t lsca_barrier;
  int lsca_bcast_optimization;
  lam_ssi_coll_bcast_fn_t lsca_bcast;
  lam_ssi_coll_exscan_fn_t lsca_exscan;
  lam_ssi_coll_gather_fn_t lsca_gather;
  lam_ssi_coll_gatherv_fn_t lsca_gatherv;
  int lsca_reduce_optimization;
  lam_ssi_coll_reduce_fn_t lsca_reduce;
  lam_ssi_coll_reduce_scatter_fn_t lsca_reduce_scatter;
  lam_ssi_coll_scan_fn_t lsca_scan;
  lam_ssi_coll_scatter_fn_t lsca_scatter;
  lam_ssi_coll_scatterv_fn_t lsca_scatterv;
} lam_ssi_coll_actions_1_1_0_t;

/*
 * Global functions for SSI overall collective open and close
 */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int lam_ssi_coll_base_close(void);
  int lam_ssi_coll_base_init_comm(MPI_Comm comm);
  int lam_ssi_coll_base_finalize_comm(MPI_Comm comm);
  int lam_ssi_coll_base_get_param(MPI_Comm comm, int keyval);
  int lam_ssi_coll_base_open(OPT *aod);
  int lam_ssi_coll_base_query(void);

  int lam_ssi_coll_base_empty_checkpoint(MPI_Comm comm);
  int lam_ssi_coll_base_empty_continue(MPI_Comm comm);
  int lam_ssi_coll_base_empty_restart(MPI_Comm comm);
  int lam_ssi_coll_base_empty_interrupt(void);


  /*
   * This is technically part of the lam_basic module, but since it
   * ships with LAM, and other modules may use the lam_basic module
   * for query/init functionality, prototype this function here.
   */

  const lam_ssi_coll_actions_1_1_0_t *
    lam_ssi_coll_lam_basic_query(MPI_Comm comm, int *priority);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif


/*
 * Public variables
 */

extern int lam_ssi_coll_verbose;
extern int lam_ssi_coll_did;
extern int lam_ssi_coll_base_crossover;
extern int lam_ssi_coll_base_associative;
extern int lam_ssi_coll_base_reduce_crossover;
extern int lam_ssi_coll_base_shmem_message_pool_size;
extern int lam_ssi_coll_base_shmem_num_segments;
extern LIST *lam_ssi_coll_base_opened;
extern LIST *lam_ssi_coll_base_available;

/*
 * WARNING: This variable will someday go away.  It is *only* for a
 * special case override that is necessary in MPI_INIT of IMPI-enabled
 * jobs.  It *will* disappear when/if IMPI is re-architected.  Do not
 * use this variable.
 */
extern char *lam_ssi_coll_base_override;

/*
 * Global instance of array of pointers to lam_ssi_coll_t.  Will
 * effectively be filled in by configure.
 */

extern const lam_ssi_t **lam_ssi_coll_modules;


#endif /* LAM_SSI_COLL_H */
