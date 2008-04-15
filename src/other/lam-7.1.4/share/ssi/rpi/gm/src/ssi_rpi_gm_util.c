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
 * $Id: ssi_rpi_gm_util.c,v 1.10 2003/11/05 13:38:44 jsquyres Exp $
 *
 *	Function:	- everything to do with tiny messages
 */

#include <lam_config.h>

#include <stdio.h>

#include <etc_misc.h>
#include <lamdebug.h>
#include <mpisys.h>
#include <rpisys.h>

#include <rpi_gm_req.h>
#include <rpi_gm_util.h>
#include <rpi_gm_interval.h>
#include <rpi_gm_dreg.h>


/*
 * Generic function called when a send callback fails
 */
void
lam_ssi_rpi_gm_send_callback_fail(struct gm_port *port, gm_status_t status)
{
  gm_u32_t my_id = 0;
  gm_u8_t my_name[GM_MAX_HOST_NAME_LEN + 1] = { 0 };
  char mystring[GM_MAX_HOST_NAME_LEN + 128];

  gm_get_node_id(port, &my_id);
  gm_get_host_name(port, (char *) my_name);

  snprintf(mystring, GM_MAX_HOST_NAME_LEN + 128,
	   "LAM internal GM send: gmID=%d '%s' send failed to "
	   "complete (see kernel log for details)",
	   my_id, my_name);
  gm_perror ((const char *) mystring, status);

  return;
}


/*
 *	lam_ssi_rpi_gm_fill_wildcards
 *
 *	Function:	- replace wildcards in request with matched values
 *			  and fill in the sequence number
 *	Accepts:	- request
 *			- matched envelope
 */
void
lam_ssi_rpi_gm_fill_wildcards(MPI_Request req, lam_ssi_rpi_gm_envl_t *env)
{
  struct _group	*g;
		
  req->rq_seq = -1;
    
  if (req->rq_rpi->cq_envbuf->ge_tag == MPI_ANY_TAG)
    req->rq_rpi->cq_envbuf->ge_tag = env->ge_tag;

  if (req->rq_rpi->cq_peer == MPI_ANY_SOURCE) {
    req->rq_rpi->cq_peer = env->ge_rank;
    req->rq_rpi->cq_envbuf->ge_rank = env->ge_rank;
	
    g = (LAM_IS_INTER(req->rq_comm)) ?
      req->rq_comm->c_rgroup : req->rq_comm->c_group;

    req->rq_proc = g->g_procs[req->rq_rpi->cq_peer];
  }
}


/*
 *	gm_setup_request_dma
 *
 *	Function:	- set up an MPI_Request with a DMA-able buffer
 *                      - either the user's buffer, or if we can't pin 
 *                        memory, get a temporary DMA-able buffer
 *	Accepts:	- request
 *	Returns:	- 0 on success, LAMERROR otherwise
 */
int
lam_ssi_rpi_gm_setup_request_dma(MPI_Request req)
{
  if (req->rq_packsize > 0 && req->rq_packbuf != NULL) {
    if (lam_ssi_rpi_gm_can_register_mem == 1 && 
        lam_ssi_rpi_gm_interval_use(req->rq_packbuf, 
                                    req->rq_packsize, 1, 1) == 0) {

      /* We have both the ability to pin, and the memory was
         successfully pinned.  So don't allocate a temporary
         buffer. */

      req->rq_rpi->dma_data_buf = NULL;
    } else {

      /* We're on a system that does not support pinning arbitrary
         memory.  So see if this buffer is already pinned (e.g., if it
         was the result of some previous call to MPI_MEM_ALLOC). */

      if (lam_ssi_rpi_gm_interval_check(req->rq_packbuf, 
                                        req->rq_packsize) == 1) {

        /* Yes, it's already pinned.  No extra buffer necessary. */

        req->rq_rpi->dma_data_buf = NULL;
      } else {

        /* No, this memory is not already pinned.  So we have to get a
           temporary buffer to use. */

        req->rq_rpi->dma_data_buf = 
          lam_ssi_rpi_gm_dma_malloc(req->rq_packsize);

        if (req->rq_rpi->dma_data_buf == NULL)
          return LAMERROR;
      }
    }
  }

  /* All done */

  return 0;
}
