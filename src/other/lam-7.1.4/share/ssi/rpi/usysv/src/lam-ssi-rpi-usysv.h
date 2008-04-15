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
 * $Id: lam-ssi-rpi-usysv.h,v 1.13 2003/09/10 22:03:12 jsquyres Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_USYSV_EXPORT_H
#define LAM_SSI_RPI_USYSV_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>


/*
 * Globally exported variable
 */

extern const lam_ssi_rpi_1_0_0_t lam_ssi_rpi_usysv_module;


/*
 * Globally exported functions
 * SSI module functions
 * RPI functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

  int lam_ssi_rpi_usysv_open_module(OPT *ad);
  int lam_ssi_rpi_usysv_query(int *priority, int *thread_min, int *thread_max);

  int lam_ssi_rpi_usysv_addprocs(struct _proc **procs, int nprocs);
  int lam_ssi_rpi_usysv_advance(MPI_Request, int);
  int lam_ssi_rpi_usysv_alloc_mem(MPI_Aint size, MPI_Info info,
				  void *baseptr);
  int lam_ssi_rpi_usysv_build(MPI_Request);
  int lam_ssi_rpi_usysv_destroy(MPI_Request);
  int lam_ssi_rpi_usysv_fastrecv(char *, int, MPI_Datatype, int, int *,
				 MPI_Comm, MPI_Status *, int *);
  int lam_ssi_rpi_usysv_fastsend(char *, int, MPI_Datatype, int, int,
				 MPI_Comm);
  int lam_ssi_rpi_usysv_finalize(struct _proc *);
  int lam_ssi_rpi_usysv_free_mem(void *baseptr);
  const lam_ssi_rpi_actions_1_0_0_t *
    lam_ssi_rpi_usysv_init(struct _proc **procs, int nprocs,
                           int *maxtag, int *maxcid);
  int lam_ssi_rpi_usysv_iprobe(MPI_Request);
  int lam_ssi_rpi_usysv_start(MPI_Request, MPI_Request);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_RPI_USYSV_EXPORT_H */
