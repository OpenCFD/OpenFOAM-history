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
 * $Id: lam-ssi-coll-smp.h,v 1.6 2003/10/23 11:22:20 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_COLL_SMP_EXPORT_H
#define LAM_SSI_COLL_SMP_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <all_list.h>

#define PUB(foo) lam_ssi_coll_smp##foo


/*
 * Internal type
 */
typedef struct lam_ssi_coll_data {

  /* Per-communicator tweakable values */
  /* Number of processes in a communicator between switching from
     linear to log-based algorithms.  This impacts both local and
     coordinator sub-communicators. */

  int lcd_crossover;
  int lcd_reduce_crossover;

  /* Size of the outter communicator.  It's also the size of the
     arrays in this struct. */

  int lcd_size;

  /* Communicator for my local communicator.  Array of roots in the
     local communicator, depending on who the root is */

  int lcd_local_size;
  MPI_Comm lcd_local_comm;
  int *lcd_local_roots;

  /* Array of communicators for coordinators, indexed by root.  If a
     given entry is MPI_COMM_NULL, then I'm not a coordinator for that
     root.  Corresponding array of root ranks in the coordinator
     communicators. */

  MPI_Comm *lcd_coord_comms;
  int *lcd_coord_roots;

  /* Integer array storing the order of global ranks(i comm) of the local
     processes corresponding to coordinator 0 to the coordinator n. */ 
 
  int *lcd_coord_global_ranks;
 
  /* Significant only for the main coordinator (rank 0 in lcd_coord_comms[0]).
     Array storing number of processes in the local comm per coordinator */
  
  int *lcd_local_comm_size;

  /* Flag for MPI_Scan */
  int lcd_is_scan_applicable; 
  
  /* A separate communicator is required for implementing MPI_Scan because, 
     process with relarively highest rank becomes a coordinator */
  MPI_Comm lcd_scan_comm;

  /* Store lam_basic function pointers in this structure. This is done to ensure
     that, the module can call lam_basic functions from smp functions when
     required. The file must not include lam-ssi-coll-lam-basic.h 
  */
  lam_ssi_coll_actions_1_1_0_t lb_functions;
} lam_ssi_coll_data_t;


/*
 * Globally exported variables
 */

extern const lam_ssi_coll_1_1_0_t PUB(module);
extern int PUB(nnodes);
extern LIST **PUB(nodes);


/*
 * Globally exported functions
 * SSI module functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * coll API functions
   */

  int lam_ssi_coll_smp_thread_query(int *thread_min, int *thread_max);
  const lam_ssi_coll_actions_1_1_0_t *lam_ssi_coll_smp_query(MPI_Comm comm, 
                                                             int *priority);
  int lam_ssi_coll_smp_end_query(MPI_Comm comm);

  int lam_ssi_coll_smp_init(MPI_Comm comm, 
			    const lam_ssi_coll_actions_1_1_0_t **new_actions);
  int lam_ssi_coll_smp_finalize(MPI_Comm comm);

  int lam_ssi_coll_smp_allgather(void *sbuf, int scount, 
				 MPI_Datatype sdtype, void *rbuf, 
				 int rcount, MPI_Datatype rdtype, 
				 MPI_Comm comm);
  int lam_ssi_coll_smp_allgatherv(void *sbuf, int scount, 
				  MPI_Datatype sdtype, void * rbuf, 
				  int *rcounts, int *disps, 
				  MPI_Datatype rdtype, 
				  MPI_Comm comm);
  int lam_ssi_coll_smp_allreduce(void *sbuf, void *rbuf, int count, 
				 MPI_Datatype dtype, MPI_Op op, 
				 MPI_Comm comm);
  int lam_ssi_coll_smp_barrier(MPI_Comm comm);
  int lam_ssi_coll_smp_bcast(void *buff, int count, 
			     MPI_Datatype datatype, int root, 
			     MPI_Comm comm);
  int lam_ssi_coll_smp_bcast(void *buff, int count, 
			     MPI_Datatype datatype, int root, 
			     MPI_Comm comm);
  int lam_ssi_coll_smp_exscan(void *sbuf, void *rbuf, int count, 
			      MPI_Datatype dtype, MPI_Op op, 
			      MPI_Comm comm);
  int lam_ssi_coll_smp_reduce(void *sbuf, void* rbuf, int count, 
			      MPI_Datatype dtype, MPI_Op op, 
			      int root, MPI_Comm comm);
  int lam_ssi_coll_smp_reduce_scatter(void *sbuf, void *rbuf, 
				      int *rcounts, 
				      MPI_Datatype dtype, 
				      MPI_Op op, MPI_Comm comm);
  int lam_ssi_coll_smp_scan(void *sbuf, void *rbuf, int count, 
			    MPI_Datatype dtype, MPI_Op op, 
			    MPI_Comm comm);

  /*
   * Module utility functions
   */

  int lam_ssi_coll_smp_set_global_ranks(MPI_Comm comm, MPI_Group g2, 
                                        int coord_rank);
  int lam_ssi_coll_smp_check_for_scan(int *ranks, int coord_size, int *sizes);
  int lam_ssi_coll_smp_set_root(MPI_Comm comm, int root);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_COLL_SMP_EXPORT_H */
