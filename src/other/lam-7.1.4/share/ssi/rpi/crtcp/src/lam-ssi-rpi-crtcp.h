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
 * $Id: lam-ssi-rpi-crtcp.h,v 1.25 2003/09/10 22:01:52 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_RPI_CRTCP_EXPORT_H
#define LAM_SSI_RPI_CRTCP_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>

#include <sys/types.h>
#include <unistd.h>

#define PUB(foo) lam_ssi_rpi_crtcp_##foo


/*
 * Globally exported variables
 */

extern const lam_ssi_rpi_1_1_0_t PUB(module);
extern int PUB(did);
extern int PUB(nprocs);
extern struct _proc **PUB(procs);
extern fd_set PUB(full_fd_set);
extern int PUB(full_sockmax);
extern int PUB(verbose);
#if 0
extern int PUB(is_fastrecv);
extern int PUB(read_and_yield_back);
#endif


/*
 * Globally exported functions
 * SSI module functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Internal utility functions
   */

  const lam_ssi_rpi_actions_1_1_0_t *
    lam_ssi_rpi_crtcp_handler_query(int *priority);

#if 0
  int lam_ssi_rpi_crtcp_handler_init(void);
  int lam_ssi_rpi_crtcp_handler_finalize(void);
  int lam_ssi_rpi_crtcp_quiesce(void);
  void lam_ssi_rpi_crtcp_yield_to_cr_handler(void);
  void lam_ssi_rpi_crtcp_yield_to_app_thread(void);
#endif
  void lam_ssi_rpi_crtcp_add_read(struct lam_ssi_rpi_proc *ps, 
                                  MPI_Request req);
  void lam_ssi_rpi_crtcp_add_read_any_src(MPI_Request req);
  int lam_ssi_rpi_crtcp_send_long_ack_queue(void);
  int lam_ssi_rpi_crtcp_setsockblk(int sock, int fl_block);

  /*
   * RPI functions
   */

  const lam_ssi_rpi_actions_1_1_0_t *
    lam_ssi_rpi_crtcp_init(struct _proc **, int, int *, int *);
  int lam_ssi_rpi_crtcp_query(int *priority, int *thread_min, int *thread_max);

  int lam_ssi_rpi_crtcp_addprocs(struct _proc **, int);
  int lam_ssi_rpi_crtcp_advance(MPI_Request, int);
  int lam_ssi_rpi_crtcp_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
  int lam_ssi_rpi_crtcp_build(MPI_Request);
  int lam_ssi_rpi_crtcp_checkpoint(void);
  int lam_ssi_rpi_crtcp_continue(void);
  int lam_ssi_rpi_crtcp_destroy(MPI_Request);
  int lam_ssi_rpi_crtcp_fastrecv(char *, int, MPI_Datatype, int, int *, 
                                 MPI_Comm, MPI_Status *, int *);
  int lam_ssi_rpi_crtcp_fastsend(char *, int, MPI_Datatype, int, int, 
				 MPI_Comm);
  int lam_ssi_rpi_crtcp_finalize(struct _proc *);
  int lam_ssi_rpi_crtcp_free_mem(void *baseptr);
  int lam_ssi_rpi_crtcp_interrupt(void);
  int lam_ssi_rpi_crtcp_iprobe(MPI_Request);
  int lam_ssi_rpi_crtcp_open_module(OPT *ad);
  int lam_ssi_rpi_crtcp_restart(void);
  int lam_ssi_rpi_crtcp_start(MPI_Request, MPI_Request);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_RPI_CRTCP_EXPORT_H */
