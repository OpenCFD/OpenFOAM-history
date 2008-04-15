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
 * $Id: rpi_ib_actions.h,v 1.5 2003/12/27 04:32:13 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_IB_ACTIONS_H
#define LAM_SSI_RPI_IB_ACTIONS_H

#include <lam-ssi-rpi-ib-config.h>

#include <mpisys.h>
#include <rpisys.h>
#include <rpi_ib_api_map.h>
#include <rpi_ib.h>

/*
 * public typedefs
 */

#define SINGLE_Q 1

/*
 * public functions
 */

void lam_ssi_rpi_ib_params_register(void);

int lam_ssi_rpi_ib_ib_setup(void);

int lam_ssi_rpi_ib_get_port(void);

int lam_ssi_rpi_ib_initialize_ib(int appending, int num_appended);

int lam_ssi_rpi_ib_ib_advance(void);

int lam_ssi_rpi_ib_connect_peer(struct _proc *p);

int lam_ssi_rpi_ib_push_envelope(MPI_Request req, int sync);

int lam_ssi_rpi_ib_post_recv_buf(int n, struct lam_ssi_rpi_proc *p);

int lam_ssi_rpi_ib_post_returned_recv_buf(struct lam_ssi_rpi_ib_envl *env,
					  struct lam_ssi_rpi_proc *rpi);

int lam_ssi_rpi_ib_fc_recv_buffer_available(struct lam_ssi_rpi_proc *destproc);

int lam_ssi_rpi_ib_fc_post_rdma_send(struct lam_ssi_rpi_proc *p);

void lam_ssi_rpi_ib_ib_finalize(void);

int lam_ssi_rpi_ib_get_long_body(char *raddr, LAM_IB_rkey rkey, int recv_eln,
				 MPI_Request req);

#endif /* LAM_SSI_RPI_IB_ACTIONS_H */
