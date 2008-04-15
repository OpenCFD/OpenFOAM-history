/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 */

#ifndef LAM_SSI_COLL_SHMEM_EXPORT_H
#define LAM_SSI_COLL_SHMEM_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <all_list.h>
#include <all_opt.h>

#define PUB(foo) lam_ssi_coll_shmem##foo

/* Structure for shmem collective module, per communicator. The structure 
   mainly stores memory pointers to the specific poritions in the shared
   memory area. Each shared memory area is reserved for special functions.
   The shared memory is split between two types of areas. One is control 
   section that stores shared flags used during synchronization, while other
   section is purely used to pass messages from one process to other. */

#define CACHELINESIZE 64
#define NUM_HOPS (CACHELINESIZE / sizeof(int))

/* Size of the shared memory will be a ssi parameter. Number of segments will
   be a ssi parameter */

typedef struct lam_ssi_coll_data {
  int lcd_num_procs;
  int lcd_num_cpus;
  int lcd_shmem_size;
  int lcd_control_segment_size;
  int lcd_shmemid;
  int lcd_semid;
  int lcd_message_pool_size;
  int lcd_num_segments;

  char* lcd_shared_segment_base;
  char* lcd_shared_control_base;
  char* lcd_shared_message_pool_base;

  /* Array of integer pointers (size = num_procs) required for a
     barrier operation */
  volatile int **lcd_barrier;

  /* One more array of integer pointers required for a barrier operation */
  volatile int **lcd_collective_track;

  /* Keeps track of which segment will be used during a collective operation */
  int lcd_segment_in_use;

  /* pointers to #NUM_SEGMENTS message segments in the shared memory */
  char **lcd_message_segment;

} lam_ssi_coll_data_t;

/*
 * Globally exported variables
 */

extern const lam_ssi_coll_1_1_0_t PUB(module);
extern int PUB(nnodes);
extern LIST **PUB(nodes);

#ifdef __cplusplus
extern "C" {
#endif

  /* Open and Close functions */
  int lam_ssi_coll_shmem_open(OPT *);
  int lam_ssi_coll_shmem_close(void);

  /*
   * coll API functions
   */

  int lam_ssi_coll_shmem_thread_query(int *thread_min, int *thread_max);
  const lam_ssi_coll_actions_1_1_0_t *lam_ssi_coll_shmem_query(MPI_Comm comm, 
							       int *priority);
  int lam_ssi_coll_shmem_end_query(MPI_Comm comm);

  int lam_ssi_coll_shmem_init(MPI_Comm comm, 
			      const lam_ssi_coll_actions_1_1_0_t **new_actions);
  int lam_ssi_coll_shmem_finalize(MPI_Comm comm);
  
  int lam_ssi_coll_shmem_barrier(MPI_Comm comm);
  int lam_ssi_coll_shmem_bcast(void *buff, int count, 
			       MPI_Datatype datatype, int root, 
			       MPI_Comm comm);
  int lam_ssi_coll_shmem_scatter(void *sbuf, int scount,
				 MPI_Datatype sdtype, void *rbuf,
				 int rcount, MPI_Datatype rdtype,
				 int root, MPI_Comm comm);
  int lam_ssi_coll_shmem_reduce(void *sbuf, void* rbuf, int count, 
				MPI_Datatype dtype, MPI_Op op, 
				int root, MPI_Comm comm);
#if 0
  int lam_ssi_coll_shmem_assc_reduce(void *sbuf, void* rbuf, int count, 
				     MPI_Datatype dtype, MPI_Op op, 
				     int root, MPI_Comm comm);
#endif

  int lam_ssi_coll_shmem_gather(void *sbuf, int scount,
				MPI_Datatype sdtype, void *rbuf,
				int rcount, MPI_Datatype rdtype,
				int root, MPI_Comm comm);
  int lam_ssi_coll_shmem_alltoall(void *sbuf, int scount,
				  MPI_Datatype sdtype, void *rbuf,
				  int rcount, MPI_Datatype rdtype,
				  MPI_Comm comm);
  int lam_ssi_coll_shmem_allgather(void *sbuf, int scount,
				   MPI_Datatype sdtype, void *rbuf,
				   int rcount, MPI_Datatype rdtype,
				   MPI_Comm comm);

  /* Module utility functions */

  /* functions related to shared memory handling */
  int lam_ssi_coll_shmem_create_area(int, char** ptr, int*, int*, int);
  int lam_ssi_coll_shmem_attach_area(int, int, char** ptr);
  int lam_ssi_coll_shmem_free_area(int, int, char*);

  /* functions related to the synchronization in a communicator */
  int lam_ssi_coll_shmem_comm_barrier(MPI_Comm, int, int);
  int lam_ssi_coll_shmem_signal_by_root(MPI_Comm, int, int, int);
  int lam_ssi_coll_shmem_signal_by_non_roots(MPI_Comm, int, int, int);
  int lam_ssi_coll_shmem_get_num_cpu(void);
  int lam_ssi_coll_shmem_increment_segment_in_use(lam_ssi_coll_data_t *lcd);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_COLL_SHMEM_EXPORT_H */




