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
 * $Id: ssi_rpi_gm_send_queue.c,v 1.11 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function: - queue for sends that were not able to go right
 *	away because of lack of resources (send tokens or long message
 *	tags)
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_gm_send_queue.h>


/*
 * public variables
 *
 * This variable is only public for a slight optimization -- the
 * "function" to add a request to the send queue is actually a #define
 * since it's only a one liner.  As such, all modules need to "see"
 * this variable.
 */
struct lam_ssi_rpi_gm_reqlist_t lam_ssi_rpi_gm_send_queue;
struct lam_ssi_rpi_gm_reqlist_t lam_ssi_rpi_gm_ack_queue;

/*
 * local functions
 */
static int advance_acks(void);
static int advance_sends(void);

/*
 * initialize this module
 */
int
lam_ssi_rpi_gm_send_queue_init(void)
{
  lam_ssi_rpi_gm_reqlist_init(&lam_ssi_rpi_gm_send_queue);
  lam_ssi_rpi_gm_reqlist_init(&lam_ssi_rpi_gm_ack_queue);

  return 0;
}


/*
 * Try to advance any pending sends that were not originally sent
 * because we did not have enough resources.
 *
 * The pending-send list is maintained locally here in this file.
 * This function can get called from a callback of a completed send,
 * or the callback of a completed long receive.
 */
int
lam_ssi_rpi_gm_send_queue_advance(void)
{
  int ret;

  lam_debug_cond((lam_ssi_rpi_gm_did, "send_advance: starting, stokens %d, "
                  "ack queue size %d, send queue size %d",
                  PUB(stokens), 
                  lam_ssi_rpi_gm_ack_queue.size,
                  lam_ssi_rpi_gm_send_queue.size));

  /* If we have any send tokens and the ack queue is non-empty, then
     try to advance it */

  if (!LAM_SSI_RPI_GM_ACK_QUEUE_EMPTY() && 
      (PUB(stokens) > 0) && 
      (0 == lam_ssi_rpi_gm_enqueue_sends)) {
    ret = advance_acks();
    if (ret != 0)
      return ret;
  }

  /* If we have any send tokens and the pending send queue is
     non-empty, then try to advance it */

  if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() && 
      (PUB(stokens) > 0) && 
      (0 == lam_ssi_rpi_gm_enqueue_sends)) {
    ret = advance_sends();
    if (ret != 0)
      return ret;
  }

  lam_debug_cond((lam_ssi_rpi_gm_did, "send_advance: finished, stokens %d, "
                  "ack queue size %d, send queue size %d",
                  PUB(stokens), 
                  lam_ssi_rpi_gm_ack_queue.size,
                  lam_ssi_rpi_gm_send_queue.size));
  return 0;
}


/************************************************************************/


/*
 * Back-end advance of the pending acks queue
 */
static int
advance_acks(void)
{
  int ret;
  MPI_Request req;
  struct lam_ssi_rpi_gm_reqlist_item_t *rli, *rli_prev;

  lam_debug_cond((lam_ssi_rpi_gm_did, "send_advance: advancing pending acks"));
  for (rli = lam_ssi_rpi_gm_ack_queue.top; rli != NULL; ) {
    rli_prev = rli;
    req = rli->req;

    /* Do we have enough send tokens to send the ACK? */

    if ((PUB(stokens) < 1) || 
	(req->rq_proc->p_rpi->num_dropped_sends > 0) ||
        (1 == lam_ssi_rpi_gm_enqueue_sends)) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "send_advance (ack): either don't have enough tokens "
                      "or sends have been disabled; need 1 token, have %d, "
                      "and enqueue_sends = %d", 
                      PUB(stokens), 
                      lam_ssi_rpi_gm_enqueue_sends));
      break;
    }

    /* We the necessary resources, so send the message */

    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "send_advance (ack): calling send_advance_fn"));
    if (req->rq_rpi->send_advance_fn == NULL ||
	(ret = req->rq_rpi->send_advance_fn(req)) != 0) {

#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
      /* Special case -- if we get a return values of -LAMERROR, then
         just bail out of the loop and let this request be tried again
         later.  This allows the send_advance_fn to check for other
         resources other than just send tokens (e.g., long message
         tags) that may not be available now, but may be available
         later. */

      if (ret == -LAMERROR) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "send_advance (ack): send_advance_fn -- break case"));
	break;
      }
#endif

      /* Otherwise, it was an error */
      
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "send_advance (ack): send_advance_fn -- error"));
      return LAMERROR;
    }

    /* Iterate rli to the next item before removing it from the list,
       because the rm_item call will free() the rli instance (and it
       just doesn't make sense to iterate something that's no longer
       in a list, anyway!) */

    rli = rli->next;
    lam_ssi_rpi_gm_reqlist_rm_item(&lam_ssi_rpi_gm_ack_queue, rli_prev);
  }

  return 0;
}


/*
 * Back-end advance of the pending sends queue
 */
static int
advance_sends(void)
{
  int ret;
  MPI_Request req;
  struct lam_ssi_rpi_gm_reqlist_item_t *rli, *rli_prev;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "send_advance: advancing pending sends"));
  for (rli = lam_ssi_rpi_gm_send_queue.top; rli != NULL; ) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "send_advance: checking req %p", rli->req));
    rli_prev = rli;
    req = rli->req;

    /* Do we have enough send tokens?  If not, stop trying to advance
       the queue.  We'll be called again when we reclaim a send
       token. */

    if (PUB(stokens) < 1 || 
	(req->rq_proc->p_rpi->num_dropped_sends > 0) ||
        (1 == lam_ssi_rpi_gm_enqueue_sends)) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "send_advance: don't have enough tokens, or sends "
                      "have been disabled; need 1 tokens, have %d, and "
                      "enqueue_sends = ", 
                      PUB(stokens), 
                      lam_ssi_rpi_gm_enqueue_sends));
      break;
    }

    /* We have the necessary resources, so send the message */

    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "send_advance: calling send_advance_fn"));
    if (req->rq_rpi->send_advance_fn == NULL ||
	(ret = req->rq_rpi->send_advance_fn(req)) != 0) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "send_advance: send_advance_fn -- error"));
      return LAMERROR;
    }

    /* Iterate rli to the next item before removing it from the list,
       because the rm_item call will free() the rli instance (and it
       just doesn't make sense to iterate something that's no longer
       in a list, anyway!) */

    rli = rli->next;
    lam_ssi_rpi_gm_reqlist_rm_item(&lam_ssi_rpi_gm_send_queue, rli_prev);
  }

  return 0;
}
