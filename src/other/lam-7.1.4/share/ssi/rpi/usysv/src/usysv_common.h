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
 * $Id: usysv_common.h,v 1.3 2003/07/11 19:48:19 jsquyres Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_RPI_USYSV_COMMON_H
#define LAM_RPI_USYSV_COMMON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#include <mpi.h>
#include <net.h>
#include <lam-ssi.h>
#include <lam-ssi-rpi.h>


/*
 * Public functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

  int lam_ssi_rpi_usysv_advance_common(void);
  int lam_ssi_rpi_usysv_shmtcp_req_probe(struct lam_ssi_rpi_proc *,
					 MPI_Request);
  int lam_ssi_rpi_usysv_buffered_adv(MPI_Request,
				     struct lam_ssi_rpi_cbuf_msg *);
  int lam_ssi_rpi_usysv_match_adv(struct lam_ssi_rpi_proc *);
  int lam_ssi_rpi_usysv_req_send_ack_long(struct lam_ssi_rpi_proc *,
					  MPI_Request);
  int lam_ssi_rpi_usysv_req_send_ack_only(struct lam_ssi_rpi_proc *,
					  MPI_Request);
  int lam_ssi_rpi_usysv_req_rcvd_body_synch(struct lam_ssi_rpi_proc *,
					    MPI_Request);
  int lam_ssi_rpi_usysv_req_send_body_first(struct lam_ssi_rpi_proc *,
					    MPI_Request);
  int lam_ssi_rpi_usysv_req_send_long(struct lam_ssi_rpi_proc *,
				      MPI_Request);
  int lam_ssi_rpi_usysv_req_send_short(struct lam_ssi_rpi_proc *,
				       MPI_Request);
  int lam_ssi_rpi_usysv_req_send_synch(struct lam_ssi_rpi_proc *,
				       MPI_Request);
  int lam_ssi_rpi_usysv_shmalloc(unsigned int *, int *);
  void lam_ssi_rpi_usysv_shfree(void *);
  int lam_ssi_rpi_usysv_create_area(int, struct lam_ssi_rpi_proc *,
				    struct nmsg *);
  int lam_ssi_rpi_usysv_attach_area(int, struct lam_ssi_rpi_proc *,
				    struct nmsg *);

#ifdef __cplusplus
}
#endif


/*
 * public variables
 */

extern char *lam_ssi_rpi_usysv_membase;	/* base of shared memory */
extern struct lam_ssi_rpi_proc **lam_ssi_rpi_usysv_read;	/* reading shm processes */
extern struct lam_ssi_rpi_proc **lam_ssi_rpi_usysv_write;	/* writing shm processes */
extern int lam_ssi_rpi_usysv_nread;	/* # reading shm processes */
extern int lam_ssi_rpi_usysv_nwrite;	/* # writing shm processes */
extern int lam_ssi_rpi_usysv_nprocs;	/* number of shm processes */
extern int lam_ssi_rpi_usysv_narray;	/* shm read/write array sizes */
extern int lam_ssi_rpi_usysv_poll_yield;/* yield in poll loop */
extern int lam_ssi_rpi_usysv_poolsize;	/* size of long message pool */
extern int lam_ssi_rpi_usysv_maxalloc;
extern int lam_ssi_rpi_usysv_short;     /* short message len */


#endif /* LAM_SSI_RPI_USYSV_COMMON_H */
