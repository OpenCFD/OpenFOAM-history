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
 * $Id: ssi_rpi_gm_tiny.c,v 1.23 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function: - everything to do with tiny messages.  Functions in
 *	this file are marked with "<SENDER>" or "<RECEIVER>" to
 *	indicate which process they will be invoked it.
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_gm.h>
#include <rpi_gm_tiny.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_send_queue.h>
#include <rpi_gm_recv_queue.h>
#include <rpi_gm_util.h>
#include <rpi_gm_ack.h>
#include <rpi_gm_actions.h>


/*
 * private functions
 */
static int send_tiny(MPI_Request req);
static int tiny_ack_received(MPI_Request req, char *env);
static void tiny_send_env_callback(struct gm_port *port, void *context, 
				   gm_status_t status);


/*
 * <SENDER>
 *
 * Try to send a tiny message:
 *
 * - See if there are any other messages ahead of this one that are
 * waiting to be sent.  If so, add this request onto the queue and try
 * to advance it.
 *
 * - Get one send token.
 *
 * - If we can't get one send token, queue this message up for later
 * sending.
 *
 * - Send the message
 *
 * This is an MPI_Request->send_advance_fn function.  Therefore, it
 * needs to reset MPI_Request->send_advance_fn when it completes.
 *
 * Returns 0 or LAMERROR.
 */
int 
lam_ssi_rpi_gm_tiny_send(MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_gm_did,
		  "send_tiny: sending request %p\n", req));

  /* Mark this request as in the progression engine */

  req->rq_rpi->is_advancing = 1;

  /* As soon as we transfer some bytes or queue up the envelope to go,
     we're non-cancellable */

  req->rq_state = LAM_RQSACTIVE;

  /* If we can't actually send now, set the next action to be to send
     the actual message. */

  if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() || 
      (PUB(stokens) < 1) || 
      (req->rq_proc->p_rpi->num_dropped_sends > 0) ||
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    req->rq_rpi->send_advance_fn = send_tiny;
    LAM_SSI_RPI_GM_SEND_QUEUE_ADD(req);

    /* Don't bother trying to advance the queue now -- every time we
       get a send token back, the queue is advanced.  Hence, we are
       guaranteed to not have enough send tokens to advance anything
       in the queue right now.  So just return. */

    return 0;
  } 

  /* If we actually send it now, there's no next action -- this
     request is done when the gm_send completes. */

  else {
    req->rq_rpi->send_advance_fn = NULL;
    lam_debug_cond((PUB(did), "decremented stokens 1: %d", 
                    PUB(stokens)));

    /* We could just call send_tiny() here, but all that does it make
       one function call to push_envelope, so save a little overhead
       and just call that directly here */

    lam_ssi_rpi_gm_push_envelope(req, tiny_send_env_callback);
    return 0;
  }

  /* Never exit through here */
}


/*
 * <SENDER>
 *
 * Try to send a tiny synchronous message:
 *
 * - Essentially the same as sending tiny messages, but at the end,
 * queue this message up to receive an ACK later.
 *
 * This is an MPI_Request->send_advance_fn function.  Therefore, it
 * needs to reset MPI_Request->send_advance_fn when it completes.
 *
 * Returns 0 or LAMERROR.
 */
int 
lam_ssi_rpi_gm_tiny_send_sync(MPI_Request req)
{
  /* When the ACK is received, it is technically a "receive" action.
     So set the function to complete this request on the recv_fn
     function.  When the ACK is received, this request is finished. */

  req->rq_rpi->recv_advance_fn = tiny_ack_received;

  /* Call the normal tiny send -- use exactly the same code */

  return lam_ssi_rpi_gm_tiny_send(req);
}


/*
 * <RECEIVER>
 *
 * This function is invoked when we have received an envelope for a
 * tiny message.  Therefore, we've received an entire tiny message.
 */
int 
lam_ssi_rpi_gm_tiny_receive_env(struct lam_ssi_rpi_gm_envl *env, 
                                MPI_Request req)
{
  /* Copy the message to the destination buffer.  Sender made right,
     so we don't need to worry about endian here.  */
  /* Check for length mismatch */

  if (env->ge_len > 0) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
		    "receive_tiny: len payload>0: %d -- "
		    "posted rcv for len %d",
		    env->ge_len, req->rq_packsize));
    if (env->ge_len <= req->rq_packsize)
      memcpy(req->rq_packbuf, env + 1, env->ge_len);
    else {
      memcpy(req->rq_packbuf, env + 1, req->rq_packsize);
      req->rq_flags |= LAM_RQFTRUNC;
    }
  }
  lam_ssi_rpi_fill_mpi_status(req, env->ge_rank, env->ge_tag, env->ge_len);
  lam_debug_cond((lam_ssi_rpi_gm_did, 
		  "receive_tiny: filled status: rank:%d, tag:%d, len:%d",
		  env->ge_rank, env->ge_tag, env->ge_len));
  lam_ssi_rpi_gm_haveadv = 1;

  /* The receiving portion of this request is finished */

  req->rq_rpi->recv_advance_fn = NULL;

  /* Was this a sync mode send that requires an ACK?  If so, the
     request won't be finished until the ACK has successfully been
     sent.  Otherwise, it's done now. */

  if (env->ge_flags & C2CSSEND) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
		    "receive_tiny: sending an ACK because this was an ssend"));
    if (lam_ssi_rpi_gm_send_ack_done(req, env) != 0)
      return LAMERROR;
    lam_debug_cond((lam_ssi_rpi_gm_did, "receive_tiny: ACK queued up"));
  } else {
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;
  }

  return 0;
}


/************************************************************************/

/*
 * <SENDER>
 *
 * This function is a wrapper around the actual gm send -- all the
 * setup has been done.
 */
static int 
send_tiny(MPI_Request req)
{
  req->rq_rpi->send_advance_fn = NULL;
  lam_ssi_rpi_gm_push_envelope(req, tiny_send_env_callback);
  return 0;
}


/*
 * <SENDER>
 *
 * This function is invoked after an ACK is received.  It means that
 * the request is now finished, and we should move it into the done
 * state.  No data will be passed through the second argument.
 */
static int 
tiny_ack_received(MPI_Request req, char *env)
{
  lam_debug_cond((lam_ssi_rpi_gm_did, "<tiny>completed send request"));

  /* This request is done */

  req->rq_state = LAM_RQSDONE;
  lam_ssi_rpi_gm_haveadv = 1;
  --lam_rq_nactv;

  return 0;
}


/*
 * <SENDER>
 *
 * This function is invoked when an envelope has finished sending.  In
 * this case, the entire message has been sent, so the request is
 * done, unless we're still waiting for the ACK back from a
 * synchronous send.
 */
static void 
tiny_send_env_callback(struct gm_port *port, void *context, gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(tiny_send_env_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
		  "<tiny>send_env_callback: all done! %p\n", req));

  /* 
   * Now that the envelope has finished, what do we do?  
   *
   * If this was a non-synchronous message, it is now done.
   * Move it into the done state.
   *
   * If it was a synchronous message, we don't need to do anything
   * here -- the ACK will contain a pointer to the source request, and
   * its recv_advance_fn has already been set to tiny_ack_received,
   * above.  So when the ACK arrives, all the Right things will
   * happen.
   */

  if ((req->rq_rpi->cq_envbuf->ge_flags & C2CSSEND) == 0) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "<tiny>send_env_callback: not a sync, so done!"));
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;
  }

  /* Increment the send tokens and mark that this c2c RPI has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}
