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
 * $Id: usysv_low.h,v 1.2 2002/10/09 21:00:44 brbarret Exp $
 */

#ifndef LAM_RPI_USYSV_LOW_H
#define LAM_RPI_USYSV_LOW_H

#include <mpi.h>


/*
 * public functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_rpi_usysv_buffer(struct lam_ssi_rpi_proc *ps);
  void lam_ssi_rpi_usysv_fill_wildcards(MPI_Request req,
				       struct lam_ssi_rpi_envl *env);
  int lam_ssi_rpi_usysv_low_fastsend(char *packbuf, int packsize, 
				    struct lam_ssi_rpi_proc *ps,
				    int dest, int tag, MPI_Comm comm);
  int lam_ssi_rpi_usysv_low_fastrecv(char *packbuf, int *packsize, 
				    struct lam_ssi_rpi_proc *ps, int src,
				    int *tag, MPI_Comm comm, int *seqnum);
  int lam_ssi_rpi_usysv_proc_read_env(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_usysv_proc_read_body_box(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_usysv_proc_read_body_pool(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_usysv_push_body_box(struct lam_ssi_rpi_proc *ps, 
				     MPI_Request req);
  int lam_ssi_rpi_usysv_push_body_pool(struct lam_ssi_rpi_proc *ps, 
				      MPI_Request req);
  int lam_ssi_rpi_usysv_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req);
  int lam_ssi_rpi_usysv_req_done_synch(struct lam_ssi_rpi_proc *ps, 
				      MPI_Request req);
  int lam_ssi_rpi_usysv_req_rcvd_long_ack(struct lam_ssi_rpi_proc *ps, 
					 MPI_Request req);
  int lam_ssi_rpi_usysv_shmtcp_req_recv(struct lam_ssi_rpi_proc *ps, 
					MPI_Request req);
  int lam_ssi_rpi_usysv_tcp_req_recv(struct lam_ssi_rpi_proc *ps, 
				     MPI_Request req);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_RPI_USYSV_LOW_H */
