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
 * $Id: lam-ssi-rpi-gm.h,v 1.14 2003/10/25 06:16:26 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_RPI_GM_EXPORT_H
#define LAM_SSI_RPI_GM_EXPORT_H

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>

#include <all_array.h>
#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <all_opt.h>

#define PUB(foo) lam_ssi_rpi_gm_##foo


/*
 * Globally exported variable
 */

extern const lam_ssi_rpi_1_1_0_t PUB(module);
extern lam_array_t *PUB(procs);


/*
 * Globally exported functions
 * SSI module functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * RPI functions
   */

  int lam_ssi_rpi_gm_query(int *priority, int *thread_min, int *thread_max);

  int lam_ssi_rpi_gm_addprocs(struct _proc **procs, int nprocs);
  int lam_ssi_rpi_gm_advance(MPI_Request, int);
  int lam_ssi_rpi_gm_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
  int lam_ssi_rpi_gm_build(MPI_Request);
  int lam_ssi_rpi_gm_checkpoint(void);
  int lam_ssi_rpi_gm_continue(void);
  int lam_ssi_rpi_gm_destroy(MPI_Request);
  int lam_ssi_rpi_gm_fastrecv(char *, int, MPI_Datatype, int, int *, 
			       MPI_Comm, MPI_Status *, int *);
  int lam_ssi_rpi_gm_fastsend(char *, int, MPI_Datatype, int, int, MPI_Comm);
  int lam_ssi_rpi_gm_finalize(struct _proc *);
  int lam_ssi_rpi_gm_free_mem(void *baseptr);
  const lam_ssi_rpi_actions_t * lam_ssi_rpi_gm_init(struct _proc **procs, 
                                                    int nprocs, 
                                                    int *maxtag, int *maxcid);
  int lam_ssi_rpi_gm_interrupt(void);
  int lam_ssi_rpi_gm_iprobe(MPI_Request);
  int lam_ssi_rpi_gm_module_close(void);
  int lam_ssi_rpi_gm_module_open(OPT *ad);
  int lam_ssi_rpi_gm_restart(void);
  int lam_ssi_rpi_gm_start(MPI_Request, MPI_Request);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_RPI_GM_EXPORT_H */
