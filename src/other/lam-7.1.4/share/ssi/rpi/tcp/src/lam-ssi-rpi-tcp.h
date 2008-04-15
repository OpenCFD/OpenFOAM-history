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
 * $Id: lam-ssi-rpi-tcp.h,v 1.13 2003/09/10 22:03:12 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_RPI_TCP_EXPORT_H
#define LAM_SSI_RPI_TCP_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>

#define PUB(foo) lam_ssi_rpi_tcp_##foo


/*
 * Globally exported variable
 */

extern const lam_ssi_rpi_1_0_0_t PUB(module);


/*
 * Globally exported functions
 * SSI module functions
 * RPI functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_rpi_tcp_open_module(OPT *ad);
  int lam_ssi_rpi_tcp_query(int *priority, int *thread_min, int *thread_max);

  int lam_ssi_rpi_tcp_addprocs(struct _proc **procs, int nprocs);
  int lam_ssi_rpi_tcp_advance(MPI_Request, int);
  int lam_ssi_rpi_tcp_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
  int lam_ssi_rpi_tcp_build(MPI_Request);
  int lam_ssi_rpi_tcp_destroy(MPI_Request);
  int lam_ssi_rpi_tcp_fastrecv(char *, int, MPI_Datatype, int, int *, 
			       MPI_Comm, MPI_Status *, int *);
  int lam_ssi_rpi_tcp_fastsend(char *, int, MPI_Datatype, int, int, MPI_Comm);
  int lam_ssi_rpi_tcp_finalize(struct _proc *);
  int lam_ssi_rpi_tcp_free_mem(void *baseptr);
  const lam_ssi_rpi_actions_1_0_0_t *
    lam_ssi_rpi_tcp_init(struct _proc **procs, int nprocs,
                         int *maxtag, int *maxcid);
  int lam_ssi_rpi_tcp_iprobe(MPI_Request);
  int lam_ssi_rpi_tcp_start(MPI_Request, MPI_Request);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_RPI_TCP_EXPORT_H */
