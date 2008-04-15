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
 * $Id: sysv_shmem.h,v 1.2 2002/10/09 21:00:35 brbarret Exp $
 */

#ifndef LAM_RPI_SYSV_SHMEM_H
#define LAM_RPI_SYSV_SHMEM_H

#include <mpi.h>


/*
 * public functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_rpi_sysv_serverinit(struct lam_ssi_rpi_proc *ps, 
				  struct nmsg *msg);
  int lam_ssi_rpi_sysv_clientinit(struct lam_ssi_rpi_proc *ps, 
				  struct nmsg *msg);
  int lam_ssi_rpi_sysv_cleanup(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_sysv_global_cleanup(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_sysv_readlock(struct lam_ssi_rpi_proc *p);
  int lam_ssi_rpi_sysv_writelock(struct lam_ssi_rpi_proc *p);

#ifdef __cplusplus
}
#endif
  
#endif /* LAM_SSI_RPI_SYSV_SHMEM_H */
