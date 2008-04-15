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
 * $Id: tcp_low.h,v 1.6 2003/07/11 19:48:19 jsquyres Exp $
 *
 *	Function:	- TCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_TCP_LOW_H
#define LAM_SSI_RPI_TCP_LOW_H

#include <mpi.h>
#include <all_array.h>

#include <lam-ssi-rpi-tcp.h>


/*
 * public functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_rpi_tcp_advmultiple(void);
  int lam_ssi_rpi_tcp_adv1(void);
  int lam_ssi_rpi_tcp_proc_read_env(struct lam_ssi_rpi_proc *ps);
  int lam_ssi_rpi_tcp_req_send_long(struct lam_ssi_rpi_proc *ps, 
				    MPI_Request req);
  int lam_ssi_rpi_tcp_req_send_short(struct lam_ssi_rpi_proc *ps, 
				     MPI_Request req);
  int lam_ssi_rpi_tcp_req_send_synch(struct lam_ssi_rpi_proc *ps, 
				     MPI_Request req);
  int lam_ssi_rpi_tcp_req_recv(struct lam_ssi_rpi_proc *ps, MPI_Request req);
  int lam_ssi_rpi_tcp_req_probe(struct lam_ssi_rpi_proc *ps, 
				MPI_Request req);
  int lam_ssi_rpi_tcp_buffered_adv(MPI_Request req, 
				   struct lam_ssi_rpi_cbuf_msg *msg);
  int lam_ssi_rpi_tcp_low_fastsend(char *buf, int size, 
				   struct lam_ssi_rpi_proc *ps, 
				   int dest, int tag, MPI_Comm comm);
  int lam_ssi_rpi_tcp_low_fastrecv(char *buf, int *size, 
				   struct lam_ssi_rpi_proc *ps, 
				   int src, int *tag, MPI_Comm comm, 
				   int *seqnum);
  void lam_ssi_rpi_tcp_fill_wildcards(MPI_Request req, 
				      struct lam_ssi_rpi_envl *env);

#ifdef __cplusplus
}
#endif


/*
 * public variables
 */
extern int lam_ssi_rpi_tcp_flblock;
extern int lam_ssi_rpi_tcp_haveadv;
extern int lam_ssi_rpi_tcp_nio;		/* # processes doing tcp io */
extern int lam_ssi_rpi_tcp_sockmax;	/* max. tcp io socket num. */
extern fd_set lam_ssi_rpi_tcp_read;	/* read sockets */
extern fd_set lam_ssi_rpi_tcp_write;	/* write sockets */
extern fd_set lam_ssi_rpi_tcp_except;	/* exception sockets */
extern fd_set lam_ssi_rpi_tcp_block;	/* blocked mode socket? */
extern fd_set lam_ssi_rpi_tcp_eoferr;	/* eof on socket is error? */
extern MPI_Request lam_ssi_rpi_tcp_lastreq; /* last tcp request */
extern struct lam_ssi_rpi_proc *lam_ssi_rpi_tcp_smap[FD_SETSIZE];	/* maps socket fd to process */
extern int lam_ssi_rpi_tcp_short;       /* short message length */
extern lam_array_t *lam_ssi_rpi_tcp_procs;

#endif /* LAM_SSI_RPI_TCP_LOW_H */
