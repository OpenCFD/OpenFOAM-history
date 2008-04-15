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
 *	$Id: rpi_ib_req.h,v 1.1 2003/11/11 18:39:48 vsahay Exp $
 *
 *	Function:	- ib-specific MPI request information
 */

#ifndef LAM_SSI_RPI_IB_REQ_H
#define LAM_SSI_RPI_IB_REQ_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>

#include <rpi_ib_api_map.h>

/*
 * ib-specific MPI request data
 */

typedef int (*lam_ssi_rpi_ib_send_progress_fn)(MPI_Request req);

typedef int (*lam_ssi_rpi_ib_recv_progress_fn)(MPI_Request req);

struct lam_ssi_rpi_req {
  int cq_state;	       /* state of request */
#define	C2CDONE		1		       /* all data transfer done */
#define C2CWRITE	2		       /* writing to socket */
#define C2CREAD		3		       /* reading from socket */
#define C2CSENDSELF	4		       /* sending to self */
  int cq_peer;	       /* matched peer */
  struct lam_ssi_rpi_ib_envl *cq_envbuf; /* envelope (pinned/DMA-able) */
  struct lam_ssi_rpi_ib_envl *cq_envbuf_save; /* copy of envelope for c/r */
  
  LAM_IB_lkey env_lkey;
  LAM_IB_rkey env_src_rkey;

  /* VPS: not needed now for env, but may be reqd later on when 
     we use RDMA for envelopes*/
  LAM_IB_rkey env_dest_rkey; 

  int is_advancing;
  lam_ssi_rpi_ib_send_progress_fn send_advance_fn;
  lam_ssi_rpi_ib_recv_progress_fn recv_advance_fn;
  LAM_IB_lkey long_lkey;
  LAM_IB_rkey long_src_rkey;
};


/*
 * List of requests
 */

struct lam_ssi_rpi_ib_reqlist_item_t {
  struct _req *req;
  struct lam_ssi_rpi_ib_reqlist_item_t *prev;
  struct lam_ssi_rpi_ib_reqlist_item_t *next;
};
struct lam_ssi_rpi_ib_reqlist_t {
  int size;
  struct lam_ssi_rpi_ib_reqlist_item_t *top;
  struct lam_ssi_rpi_ib_reqlist_item_t *bottom;
};


#endif	/* LAM_SSI_RPI_IB_REQ_H */
