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
 * $Id: lam-ssi-coll-impi.h,v 1.3 2003/05/05 22:41:11 ssankara Exp $
 *
 */

#ifndef LAM_SSI_COLL_IMPI_EXPORT_H
#define LAM_SSI_COLL_IMPI_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>


/*
 * Globally exported variable
 */

extern const lam_ssi_coll_1_1_0_t lam_ssi_coll_impi_module;
extern int lam_ssi_coll_impi_crossover;


/*
 * Coll API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  const lam_ssi_coll_actions_1_1_0_t *lam_ssi_coll_impi_query(MPI_Comm comm, 
                                                              int *priority);

  int lam_ssi_coll_impi_thread_query(int *thread_min, int *thread_max);
  int lam_ssi_coll_impi_end_query(MPI_Comm comm);

  int lam_ssi_coll_impi_init(MPI_Comm comm,
			     const lam_ssi_coll_actions_1_1_0_t **new_actions);
  int lam_ssi_coll_impi_finalize(MPI_Comm comm);

  int lam_ssi_coll_impi_barrier(MPI_Comm comm);
  int lam_ssi_coll_impi_bcast(void *buff, int count, 
			      MPI_Datatype datatype, int root, 
			      MPI_Comm comm);
  int lam_ssi_coll_impi_reduce(void *sbuf, void* rbuf, int count, 
			       MPI_Datatype dtype, MPI_Op op, 
			       int root, MPI_Comm comm);
  
  extern int lam_ssi_coll_lam_basic_allreduce(void *sbuf, void *rbuf, 
					      int count, 
					      MPI_Datatype dtype, MPI_Op op, 
					      MPI_Comm comm);
#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_COLL_IMPI_EXPORT_H */
