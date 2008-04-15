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
 * $Id: rpi_gm_actions.h,v 1.12 2004/02/12 17:49:10 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_GM_ACTIONS_H
#define LAM_SSI_RPI_GM_ACTIONS_H

#include <lam-ssi-rpi-gm-config.h>

#include <gm.h>

#include <mpisys.h>
#include <rpisys.h>
#include <rpi_gm.h>


/*
 * public typedefs
 */

typedef struct lam_ssi_rpi_gm_port {
  struct gm_port *rgp_gm_port;
  int rgp_port_id;
  unsigned int rgp_local_node_id;
  unsigned int rgp_global_node_id;
} lam_ssi_rpi_gm_port_t;


/*
 * public functions
 */

void lam_ssi_rpi_gm_params_register(void);
int lam_ssi_rpi_gm_gm_setup(struct _proc *p);
int lam_ssi_rpi_gm_get_port(struct _proc *p, lam_ssi_rpi_gm_port_t *port);
int lam_ssi_rpi_gm_gm_finalize(lam_ssi_rpi_gm_port_t *port);

int lam_ssi_rpi_gm_gm_advance_blocking(void);
int lam_ssi_rpi_gm_gm_advance_nonblocking(void);

void lam_ssi_rpi_gm_push_envelope(MPI_Request req, 
                                  gm_send_completion_callback_t callback);

/*
 * If we have RDMA get (gm 2.x and above), then use the first function
 * for getting long messages.  Otherwise, use the second one that uses
 * RDMA put functionality (available starting with gm 1.6, and we
 * don't support versions prior to that).
 */
#if LAM_SSI_RPI_GM_HAVE_RDMA_GET
void lam_ssi_rpi_gm_get_long_body(MPI_Request req, void *source,
                                  int length,
                                  gm_send_completion_callback_t cbk_unpin,
                                  gm_send_completion_callback_t cbk_no_unpin);
#else
int lam_ssi_rpi_gm_put_long_body(MPI_Request req,
                                 gm_send_completion_callback_t cbk_unpin,
                                 gm_send_completion_callback_t cbk_no_unpin);
#endif

int lam_ssi_rpi_gm_resend(MPI_Request req);

void lam_ssi_rpi_gm_dropped_send_callback(struct gm_port *port, void *context,
					  gm_status_t status);


#endif /* LAM_SSI_RPI_GM_ACTIONS_H */
