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
 * $Id: usysv_shmem.h,v 1.3 2003/07/11 19:48:20 jsquyres Exp $
 */

#ifndef LAM_RPI_USYSV_SHMEM_H
#define LAM_RPI_USYSV_SHMEM_H

#include <mpi.h>


/*
 * public variables
 */

extern int lam_ssi_rpi_usysv_lock_poll_read;
extern int lam_ssi_rpi_usysv_lock_poll_write;


/*
 * public functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

  int lam_ssi_rpi_usysv_serverinit(struct lam_ssi_rpi_proc *ps,
				   struct nmsg *msg);
  int lam_ssi_rpi_usysv_clientinit(struct lam_ssi_rpi_proc *ps,
				   struct nmsg *msg);
  int lam_ssi_rpi_usysv_cleanup(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_usysv_global_cleanup(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_usysv_readlock(struct lam_ssi_rpi_proc *p);
  int lam_ssi_rpi_usysv_writelock(struct lam_ssi_rpi_proc *p);

#ifdef __cplusplus
}
#endif
#endif				/* LAM_SSI_RPI_USYSV_SHMEM_H */
