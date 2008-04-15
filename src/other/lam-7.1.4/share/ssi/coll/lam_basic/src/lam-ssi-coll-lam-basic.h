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
 * $Id: lam-ssi-coll-lam-basic.h,v 1.8 2003/10/23 23:11:12 adharurk Exp $
 *
 */

#ifndef LAM_SSI_COLL_LAM_BASIC_EXPORT_H
#define LAM_SSI_COLL_LAM_BASIC_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>

#include <all_opt.h>

/*
 * Globally exported variable
 */

extern const lam_ssi_coll_1_1_0_t lam_ssi_coll_lam_basic_module;

/* Communicator specific data */
typedef struct lam_ssi_coll_lam_basic_data {
  /* This data is required only in case of intercomms.  Intercomms are BAD..
     They are causing all the changes in the design */
  MPI_Comm local_comm;
  int is_low;
} lam_ssi_coll_lam_basic_data_t;

/*
 * coll API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  /* lam_basic's query function is prototyped in <lam-ssi-coll.h>
     because other functions may invoke it. */

  int lam_ssi_coll_lam_basic_thread_query(int *thread_min, int *thread_max);
  const lam_ssi_coll_actions_1_1_0_t  *lam_ssi_coll_lam_basic_query(MPI_Comm comm, int *priority);
  int lam_ssi_coll_lam_basic_end_query(MPI_Comm comm);

  int lam_ssi_coll_lam_basic_init(MPI_Comm comm, 
				  const lam_ssi_coll_actions_1_1_0_t **new_actions);
  int lam_ssi_coll_lam_basic_finalize(MPI_Comm comm);

  int lam_ssi_coll_lam_basic_allgather(void *sbuf, int scount, 
				       MPI_Datatype sdtype, void *rbuf, 
				       int rcount, MPI_Datatype rdtype, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_allgatherv(void *sbuf, int scount, 
					MPI_Datatype sdtype, void * rbuf, 
					int *rcounts, int *disps, 
					MPI_Datatype rdtype, 
					MPI_Comm comm);
  int lam_ssi_coll_lam_basic_allreduce(void *sbuf, void *rbuf, int count, 
				       MPI_Datatype dtype, MPI_Op op, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoall(void *sbuf, int scount, 
				      MPI_Datatype sdtype, void* rbuf, 
				      int rcount, MPI_Datatype rdtype, 
				      MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoall_lamd(void *sbuf, int scount, 
					   MPI_Datatype sdtype, void* rbuf, 
					   int rcount, MPI_Datatype rdtype, 
					   MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoallv(void *sbuf, int *scounts, 
				       int *sdisps, MPI_Datatype sdtype, 
				       void *rbuf, int *rcounts, 
				       int *rdisps, MPI_Datatype rdtype, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoallv_lamd(void *sbuf, int *scounts,
					    int *sdisps, MPI_Datatype sdtype,
					    void *rbuf, int *rcounts,
					    int *rdisps, MPI_Datatype rdtype,
					    MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoallw(void *sbuf, int *scounts, 
				       int *sdisps, MPI_Datatype *sdtypes, 
				       void *rbuf, int *rcounts, 
				       int *rdisps, MPI_Datatype *rdtypes, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoallw_lamd(void *sbuf, int *scounts,
					    int *sdisps, MPI_Datatype *sdtypes,
					    void *rbuf, int *rcounts,
					    int *rdisps, MPI_Datatype *rdtypes,
					    MPI_Comm comm);
  int lam_ssi_coll_lam_basic_barrier_lin(MPI_Comm comm);
  int lam_ssi_coll_lam_basic_barrier_log(MPI_Comm comm);
  int lam_ssi_coll_lam_basic_bcast_lin(void *buff, int count, 
				       MPI_Datatype datatype, int root, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_bcast_lin_lamd(void *buff, int count, 
					    MPI_Datatype datatype, int root, 
					    MPI_Comm comm);
  int lam_ssi_coll_lam_basic_bcast_log(void *buff, int count, 
				       MPI_Datatype datatype, int root, 
				       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_bcast_log_lamd(void *buff, int count, 
					    MPI_Datatype datatype, int root, 
					    MPI_Comm comm);
  int lam_ssi_coll_lam_basic_exscan(void *sbuf, void *rbuf, int count, 
				    MPI_Datatype dtype, MPI_Op op, 
				    MPI_Comm comm);
  int lam_ssi_coll_lam_basic_gather(void *sbuf, int scount, 
				    MPI_Datatype sdtype, void *rbuf, 
				    int rcount, MPI_Datatype rdtype, 
				    int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_gatherv(void *sbuf, int scount, 
				     MPI_Datatype sdtype, void *rbuf, 
				     int *rcounts, int *disps, 
				     MPI_Datatype rdtype, int root, 
				     MPI_Comm comm);
  int lam_ssi_coll_lam_basic_reduce_lin(void *sbuf, void* rbuf, int count, 
					MPI_Datatype dtype, MPI_Op op, 
					int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_reduce_log(void *sbuf, void* rbuf, int count, 
					MPI_Datatype dtype, MPI_Op op, 
					int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_reduce_scatter(void *sbuf, void *rbuf, 
					    int *rcounts, 
					    MPI_Datatype dtype, 
					    MPI_Op op, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_scan(void *sbuf, void *rbuf, int count, 
				  MPI_Datatype dtype, MPI_Op op, 
				  MPI_Comm comm);
  int lam_ssi_coll_lam_basic_scatter(void *sbuf, int scount, 
				     MPI_Datatype sdtype, void *rbuf, 
				     int rcount, MPI_Datatype rdtype, 
				     int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_scatterv(void *sbuf, int *scounts, 
				      int *disps, MPI_Datatype sdtype, 
				      void* rbuf, int rcount, 
				      MPI_Datatype rdtype, int root, 
				      MPI_Comm comm);

  /* Intercommunicator functions */
  int lam_ssi_coll_lam_basic_allgather_inter(void *sbuf, int scount, 
					     MPI_Datatype sdtype, void *rbuf, 
					     int rcount, MPI_Datatype rdtype, 
					     MPI_Comm comm);
  int lam_ssi_coll_lam_basic_allreduce_inter(void *sbuf, void *rbuf, 
					     int count, 
					     MPI_Datatype dtype, MPI_Op op, 
					     MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoall_inter(void *sbuf, int scount, 
					    MPI_Datatype sdtype, void* rbuf, 
					    int rcount, MPI_Datatype rdtype, 
					    MPI_Comm comm);
    int lam_ssi_coll_lam_basic_alltoallv_inter(void *sbuf, int *scounts, 
					       int *sdisps, 
					       MPI_Datatype sdtype, 
					       void *rbuf, int *rcounts, 
					       int *rdisps, 
					       MPI_Datatype rdtype, 
					       MPI_Comm comm);
  int lam_ssi_coll_lam_basic_alltoallw_inter(void *sbuf, int *scounts, 
					     int *sdisps, 
					     MPI_Datatype *sdtypes, 
					     void *rbuf, int *rcounts, 
					     int *rdisps, 
					     MPI_Datatype *rdtypes, 
					     MPI_Comm comm);
  int lam_ssi_coll_lam_basic_barrier_inter(MPI_Comm intercomm);
  int lam_ssi_coll_lam_basic_bcast_inter(void *buff, int count, 
					 MPI_Datatype datatype, int root, 
					 MPI_Comm intercomm);
  int lam_ssi_coll_lam_basic_gather_inter(void *sbuf, int scount, 
					  MPI_Datatype sdtype, void *rbuf, 
					  int rcount, MPI_Datatype rdtype, 
					  int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_gatherv_inter(void *sbuf, int scount, 
					   MPI_Datatype sdtype, void *rbuf, 
					   int *rcounts, int *disps, 
					   MPI_Datatype rdtype, int root, 
					   MPI_Comm comm);
  int lam_ssi_coll_lam_basic_reduce_inter(void *sbuf, void* rbuf, int count, 
					  MPI_Datatype dtype, MPI_Op op, 
					  int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_reduce_scatter_inter(void *sbuf, void *rbuf, 
						  int *rcounts, 
						  MPI_Datatype dtype, 
						  MPI_Op op, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_scatter_inter(void *sbuf, int scount, 
					   MPI_Datatype sdtype, void *rbuf, 
					   int rcount, MPI_Datatype rdtype, 
					   int root, MPI_Comm comm);
  int lam_ssi_coll_lam_basic_scatterv_inter(void *sbuf, int *scounts, 
					    int *disps, MPI_Datatype sdtype, 
					    void* rbuf, int rcount, 
					    MPI_Datatype rdtype, int root, 
					    MPI_Comm comm);

  /* Module Utility functions */
  /* For intercommunicator */
  void setup_local_comm_for_intercomm(MPI_Comm intercomm);
  
#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_COLL_LAM_BASIC_EXPORT_H */

