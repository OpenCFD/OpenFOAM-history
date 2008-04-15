/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: rpi_gm_util.h,v 1.10 2004/02/15 14:02:05 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_UTIL_H
#define LAM_SSI_RPI_GM_UTIL_H

#include <gm.h>

#include <rpi_gm.h>
#include <rpi_gm_actions.h>


/*
 * Common start for all send callbacks
 * Need differences between gm 1.x and 2.x
 */

/*
 * gm 2.x has GM_TIMED_OUT
 */
#define LAM_SSI_RPI_GM_2_x_SEND_CALLBACK_START(cback) \
  MPI_Request req = (MPI_Request) context; \
  req->rq_rpi->callback = cback; \
  if (status == GM_TRY_AGAIN || status == GM_SEND_TIMED_OUT || \
      status == GM_TIMED_OUT) {\
    lam_ssi_rpi_gm_resend(req); \
    return; \
  } else if (status == GM_SEND_DROPPED) { \
    lam_ssi_rpi_gm_dropped_send_callback(port, context, status); \
    return; \
  } else if (status != GM_SUCCESS) { \
    lam_ssi_rpi_gm_send_callback_fail(port, status); \
    return; \
  }

/*
 * gm 1.x doesn't have GM_TIMED_OUT
 */
#define LAM_SSI_RPI_GM_1_x_SEND_CALLBACK_START(cback) \
  MPI_Request req = (MPI_Request) context; \
  req->rq_rpi->callback = cback; \
  if (status == GM_TRY_AGAIN || status == GM_SEND_TIMED_OUT) { \
    lam_ssi_rpi_gm_resend(req); \
    return; \
  } else if (status == GM_SEND_DROPPED) { \
    lam_ssi_rpi_gm_dropped_send_callback(port, context, status); \
    return; \
  } else if (status != GM_SUCCESS) { \
    lam_ssi_rpi_gm_send_callback_fail(port, status); \
    return; \
  }

#if defined(GM_API_VERSION_2_0) && GM_API_VERSION >= GM_API_VERSION_2_0
#define LAM_SSI_RPI_GM_SEND_CALLBACK_START(cback) \
  LAM_SSI_RPI_GM_2_x_SEND_CALLBACK_START(cback)
#else
#define LAM_SSI_RPI_GM_SEND_CALLBACK_START(cback) \
  LAM_SSI_RPI_GM_1_x_SEND_CALLBACK_START(cback)
#endif

/*
 * Common finish for all send callbacks
 *
 * Note that we just got a send token back, but we can't try to
 * advance the pending send queue because we're still in a callback.
 * Hence, the main advance will notice that progress was made on sends
 * and will try to advance the pending send queue.
 */
#define LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH \
  ++lam_ssi_rpi_gm_stokens; \
  lam_debug_cond((PUB(did), "++++incremented stokens to: %d", lam_ssi_rpi_gm_stokens)); \
  --(req->rq_proc->p_rpi->num_pending_sends); \
  lam_ssi_rpi_gm_haveadv = 1;


/*
 * public functions
 */

void lam_ssi_rpi_gm_send_callback_fail(struct gm_port *port, 
				       gm_status_t status);
void lam_ssi_rpi_gm_fill_wildcards(MPI_Request req, 
				   lam_ssi_rpi_gm_envl_t *env);
int lam_ssi_rpi_gm_setup_request_dma(MPI_Request req);


#endif /* LAM_SSI_RPI_GM_UTIL_H */
