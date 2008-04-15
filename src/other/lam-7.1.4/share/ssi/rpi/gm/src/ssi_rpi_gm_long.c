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
 * $Id: ssi_rpi_gm_long.c,v 1.29 2004/02/15 14:02:05 jsquyres Exp $
 *
 *	Function: - everything to do with long messages.  Functions in
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
#include <rpi_gm_actions.h>
#include <rpi_gm_long.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_send_queue.h>
#include <rpi_gm_util.h>
#include <rpi_gm_recv_queue.h>
#include <rpi_gm_interval.h>


/*
 * private functions
 */

static int sender_send_env(MPI_Request req);
static void sender_send_env_callback(struct gm_port *port, void *context, 
                                     gm_status_t status);

static int receiver_send_ack(MPI_Request req);
static void receiver_send_ack_callback(struct gm_port *port, void *context, 
                                       gm_status_t status);

static int sender_recv_ack(MPI_Request req, char *raw_env_data);
static int sender_put_body(MPI_Request req);
static void sender_put_body_unpin_callback(struct gm_port *port, 
                                           void *context, gm_status_t status);
static void sender_put_body_no_unpin_callback(struct gm_port *port, 
                                              void *context, 
                                              gm_status_t status);

static int sender_send_all_done(MPI_Request req);
static void  sender_send_all_done_callback(struct gm_port *port, 
                                           void *context, gm_status_t status);

static int receiver_recv_body(MPI_Request req);


/*
 * <SENDER>
 *
 * Start the send of a long message:
 *
 * This is the first "long" function invoked on the sending side when
 * transferring a long message.
 *
 * - First check to see if there are any other messages ahead of this
 * one that are pending to be sent.  If so, queue this one up and try
 * to advance the queue.
 *
 * - Queue this request up for sending the real message when the ACK is
 * received.
 *
 * This is an MPI_Request->send_advance_fn function.  Therefore, it
 * needs to reset MPI_Request->send_advance_fn when it completes.
 *
 * Returns 0 or LAMERROR.  
 */
int 
lam_ssi_rpi_gm_long_send(MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "send_long: sending request %p\n", req));

  /* Mark this request as in the progression engine */

  req->rq_state = LAM_RQSACTIVE;
  req->rq_rpi->is_advancing = 1;

  req->rq_rpi->recv_advance_fn = sender_recv_ack;

  /* We only need one send token here because we're only going to send
     out the envelope right now.  If we have to queue it up, mark the
     next action as sending the first part of the long. */
  /* NOTE: The envelope that is sent has a cr_rank of *my* rank (in
     this communicator) -- i.e., who the long message is *from*. */

  if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() || 
      (PUB(stokens) < 1) || 
      (req->rq_proc->p_rpi->num_dropped_sends > 0) ||
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    req->rq_rpi->send_advance_fn = sender_send_env;
    LAM_SSI_RPI_GM_SEND_QUEUE_ADD(req);

    /* Don't bother trying to advance the queue now -- every time we
       get a send token back, the queue is advanced.  Hence, we are
       guaranteed to not have enough send tokens to advance anything
       in the queue right now.  So just return. */

    return 0;
  } 

  /* Otherwise, we can send it now.  The next action of this request
     will be triggered by the ACK, which is the recv_advance_fn, which
     was set above (send the second part of the message). */

  else {
    req->rq_rpi->send_advance_fn = NULL;
    return sender_send_env(req);
  }

  /* Never exit through here */
}


/*
 * <SENDER>
 *
 * This function is a wrapper around the actual gm send -- all the
 * setup has been done.
 */
static int 
sender_send_env(MPI_Request req)
{
  /* Now actually send the envelope */

  lam_debug_cond((PUB(did), "<long>sender_send_env"));
  lam_ssi_rpi_gm_push_envelope(req, sender_send_env_callback);

  /* Finally -- do a little latency hiding.  If we could not pin the
     original buffer, we have already allocated a DMA-able buffer to
     send from.  Since we just sent the first envelope, do the memcpy
     from the original bummer -> DMA buffer now while we're waiting
     for the envelope to be delivered.  This allows us to send
     immediately upon receive of the ACK from the recipient. */

  if (req->rq_rpi->dma_data_buf != NULL) {
    lam_memcpy(req->rq_rpi->dma_data_buf, req->rq_packbuf, req->rq_packsize);
  }

  return 0;
}


/*
 * <SENDER>
 *
 * Callback for when the sender envelope finishes sending.
 */
static void 
sender_send_env_callback(struct gm_port *port, void *context, 
                         gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(sender_send_env_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>sender_send_env_callback: all done! %p, peer %d",
                  req, req->rq_rank));

  /* Nothing more to do -- this request has already been queued up to
     receive an ACK */
  /* Increment the send tokens and mark that this c2c RPI has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/* 
 * <RECEIVER>
 *
 * This function is invoked when the first envelope has been received.
 * We must answer it with an ACK.
 *
 * This is the first "long" function invoked on the receiving side
 * when transferring a long message.
 */
int 
lam_ssi_rpi_gm_long_receive_env_first(struct lam_ssi_rpi_gm_envl *env, 
                                      MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>long_receive_env_first: got the announce "
                  "envelope -- send back an ACK with myrank: %d",
                  req->rq_comm->c_group->g_myrank));
  req->rq_rpi->is_advancing = 1;

  /* Prepare the ACK to be sent back.  Include my identity (relative
     to the local group in the request's communicator).  Hence, this
     works even if this is an intercommunicator, because it will be
     relative to my group. */

  req->rq_rpi->cq_envbuf->ge_flags |= C2CLONG | C2CACK;
  req->rq_rpi->cq_envbuf->ge_len = LAM_min(env->ge_len, 
                                           req->rq_packsize);
  req->rq_rpi->cq_envbuf->ge_rank = req->rq_comm->c_group->g_myrank;
  req->rq_rpi->cq_envbuf->ge_tag = env->ge_tag;
  req->rq_rpi->cq_envbuf->ge_cid = env->ge_cid;
  req->rq_rpi->cq_envbuf->ge_src_request = env->ge_src_request;
  req->rq_rpi->cq_envbuf->ge_dest_request = req;

  /* Fill in the status */
  
  lam_ssi_rpi_fill_mpi_status(req, req->rq_rpi->cq_peer, 
                              req->rq_rpi->cq_envbuf->ge_tag, 
                              req->rq_rpi->cq_envbuf->ge_len);
  
  /* Check to see if this is going to be a truncation error. */

  if (req->rq_packsize < env->ge_len)
    req->rq_flags |= LAM_RQFTRUNC;

  /* Similar to normal messages, if we don't have enough send tokens
     or if there's stuff already in the send queue, just queue it up
     for later sending.  ACKs take precedence over normal pending send
     messages, though, so don't bother checking the send queue (and if
     the ACK queue will be empty if there are any stokens available,
     so don't need to check if that's empty, either) */

  if ((PUB(stokens) < 1) || 
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    req->rq_rpi->send_advance_fn = receiver_send_ack;
    LAM_SSI_RPI_GM_ACK_QUEUE_ADD(req);

    /* Don't bother trying to advance the queue now -- every time we
       get a send token back, the queue is advanced.  Hence, we are
       guaranteed to not have enough send tokens to advance anything
       in the queue right now.  So just return. */

    return 0;
  } 

  /* Otherwise, we can try to send it now. */

  else {
    return receiver_send_ack(req);
  }

  /* Won't exit out of here */
}


/*
 * <RECEIVER>
 *
 * This is the worker function for sending the ACK back to the sender
 * after the receiver has received the first envelope in a long send.  
 */
static int 
receiver_send_ack(MPI_Request req)
{
  struct lam_ssi_rpi_gm_envl *env = req->rq_rpi->cq_envbuf;

  /* If this request has a posted buffer length of 0, then we don't
     need to post a receive buffer -- just send back the ACK. */

  lam_debug_cond((PUB(did), "<long>receiver_send_ack"));
  if (env->ge_len > 0) {

    /* Figure out which target address to use */

    if (req->rq_rpi->dma_data_buf != NULL)
      env->ge_target = req->rq_rpi->dma_data_buf;
    else
      env->ge_target = req->rq_packbuf;
  }

  /* Now try to send the ACK */

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>receiver_send_ack: sending the ACK back to "
                  "receiver, flags 0x%x", env->ge_flags));
  lam_ssi_rpi_gm_push_envelope(req, receiver_send_ack_callback);
  lam_ssi_rpi_gm_haveadv = 1;
  return 0;
}


/*
 * <RECEIVER>
 * 
 * Callback for then the ACK from the <RECEIVER> to the <SENDER>
 * completes.
 */
static void 
receiver_send_ack_callback(struct gm_port *port, void *context, 
                           gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(receiver_send_ack_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "<long>receiver_send_ack_callback: ce_len %d\n", 
                  req->rq_rpi->cq_envbuf->ge_len));

  /* Reset the req so that it can match the next incoming envelope --
     the one that will have the actual data of the message coming
     right behind it.  Special case, though -- if the posted request
     had a zero length buffer, then this request is done; the sender
     won't send the actual message (and this is a truncation error).
     Also, we won't have posted a receive buffer, nor allocated a long
     message tag -- so there's nothing left to do, and this request is
     done. */

  if (req->rq_rpi->cq_envbuf->ge_len == 0) {
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;
  }

  /* Increment the send tokens and mark that this RPI has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/*
 * <SENDER>
 *
 * Finish the send of a long message:
 *
 * This is the second "long" function invoked on the sending side when
 * transferring a long message.
 *
 * We've got the ACK back from the receiver saying that they're ready
 * to receive the message.  So all we have to do is actually send the
 * envelope again followed by the body of the long message.  The ACK
 * contained a tag that we should use to send the message as well as a
 * maximum length of data to send (in the case of a truncation error).
 * Once we have completed this gm_send, the send is complete.  Note,
 * however, that if the ACK contains a length of 0 (i.e., the case
 * where the receiver posted a buffer of length 0), then the receiver
 * is not expecting anything back from us and this request is now
 * done.
 *
 * This is an MPI_Request->recv_advance_fn function because it is
 * invoked upon receipt of an ACK.  Since we don't need it anymore, we
 * should reset recv_advance_fn to NULL.
 *
 * Returns 0 or LAMERROR.
 */
static int 
sender_recv_ack(MPI_Request req, char *raw_env_data)
{
  struct lam_ssi_rpi_gm_envl *env = 
    (struct lam_ssi_rpi_gm_envl *) raw_env_data;
  req->rq_rpi->recv_advance_fn = NULL;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "<long>sender_recv_ack: sending request %p", req));
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>sender_recv_ack: recvd env: to rank %d, cid %d, tag %d (env tag %d), len %d",
                  req->rq_rank, req->rq_cid, req->rq_tag, 
                  env->ge_tag, env->ge_len));

  /* Only transfer as many bytes as the receiver requests -- which may
     be zero.  If the receiver requests zero bytes, then we are done
     -- don't send a second envelope/body pair. */

  if (env->ge_len == 0) {
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;
    lam_ssi_rpi_gm_haveadv = 1;
    return 0;
  }

  /* Otherwise, set the envelope up for sending again */

  req->rq_rpi->cq_envbuf->ge_len = env->ge_len;
  req->rq_rpi->cq_envbuf->ge_flags |= C2C2ND;
  req->rq_rpi->cq_envbuf->ge_rank = req->rq_comm->c_group->g_myrank;
  req->rq_rpi->cq_envbuf->ge_target = env->ge_target;
  req->rq_rpi->cq_envbuf->ge_dest_request = env->ge_dest_request;

  /* We only need 1 send token right now because we're just going to
     push the body of the message; sending the "all done" envelope
     comes later. */

  if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() || 
      (PUB(stokens) < 1) || 
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    req->rq_rpi->send_advance_fn = sender_put_body;
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
    return sender_put_body(req);
  }

  /* Doesn't reach here */
}


/*
 * <SENDER>
 *
 * This function is a wrapper around the actual gm send -- all the
 * setup has been done.
 */
static int 
sender_put_body(MPI_Request req)
{
  lam_debug_cond((PUB(did), "<long>send_long_second"));

  /* Do a direct DMA PUT to transfer the message to the target buffer
     on the destination node */

  if (lam_ssi_rpi_gm_put_long_body(req, sender_put_body_unpin_callback,
                                   sender_put_body_no_unpin_callback) != 0) {
    return LAMERROR;
  }

  /* All done */

  return 0;
}


/*
 * <SENDER>
 *
 * Callback for when the body of the long message has completed
 * sending.  We knew ahead of time that we want to unpin the send
 * buffer (because it was a temporary buffer).
 */
static void
sender_put_body_unpin_callback(struct gm_port *port, void *context, 
                               gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(sender_put_body_unpin_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>sender_put_body_unpin_callback: all done! %p "
                  "to peer %d", 
                  req, req->rq_rank));

  /* Now need to invoke the send of the envlope to the receiver
     indicating that the message has been transferred.  We can't
     actually do a gm_send from here (because we're already in a
     callback), so queue it up to go after we finish here. */

  req->rq_rpi->send_advance_fn = sender_send_all_done;
  LAM_SSI_RPI_GM_SEND_QUEUE_ADD(req);

  /* Unpin data buffer (there's no pool to return it to) */

  lam_ssi_rpi_gm_dma_free(req->rq_rpi->dma_data_buf,
                          req->rq_packsize);
  req->rq_rpi->dma_data_buf = NULL;

  /* Increment the send tokens and mark that we have advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/*
 * <SENDER>
 *
 * Callback for when the body of the long message has completed
 * sending.  We knew ahead of time that we do not want to unpin the
 * send buffer (because it was a user-pinned buffer).
 */
static void 
sender_put_body_no_unpin_callback(struct gm_port *port, void *context, 
                                  gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(sender_put_body_no_unpin_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>sender_put_body_no_unpin_callback: all done! %p "
                  "to peer %d", 
                  req, req->rq_rank));

  /* Now need to invoke the send of the envlope to the receiver
     indicating that the message has been transferred.  We can't
     actually do a gm_send from here (because we're already in a
     callback), so queue it up to go after we finish here. */

  req->rq_rpi->send_advance_fn = sender_send_all_done;
  LAM_SSI_RPI_GM_SEND_QUEUE_ADD(req);

  /* There's no unpinning to do in this callback.  Just unuse the
     interval.  Regardless of how much data we received, we originally
     pinned req->rq_packsize.  So unuse the whole interval. */

  lam_ssi_rpi_gm_interval_unuse(req->rq_packbuf, req->rq_packsize);

  /* Increment the send tokens and mark that we have advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/*
 * <SENDER>
 *
 * Sent by the <SENDER> when it gets notified by gm that the
 * gm_directed_send() has finished (i.e., the direct DMA PUT into the
 * target buffer's memory).  Upon this notification, we send an
 * envelope to the <RECEIVER> that indicates that the message transfer
 * is done.
 */
static int
sender_send_all_done(MPI_Request req)
{
  /* Send the envelope; it's all setup already. */

  lam_ssi_rpi_gm_push_envelope(req, sender_send_all_done_callback);

  /* 
   * Increment the "send" bookmark to reflect the directed send of the body
   * of the message (in _push_long_body()).  This is done here instead of
   * immediately after the gm_directed_send* to avoid some deadlock
   * situations.
   *
   * XXX: SS: is this a good place?? the other bookmark increments are all
   * in _actions.c, so is there a clean way to move this over to _actions.c
   * as well??
   */
  req->rq_proc->p_rpi->cp_out_nbytes += req->rq_rpi->cq_envbuf->ge_len;

  /* Nothing left to do except wait for the callback from this send */

  req->rq_rpi->send_advance_fn = NULL;

  /* All done */

  return 0;
}


/*
 * <SENDER>
 * 
 * Callback for when the second envelope completed sending.
 */
static void 
sender_send_all_done_callback(struct gm_port *port, void *context, 
			      gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(sender_send_all_done_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "<long>sender_send_all_done_callback: all done! %p\n", 
                  req));

  /* This request is now done (this envelope was sent to notify the
     receiver that their have already received (via DMA put) the
     message content). */

  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;

  /* Increment the send tokens and mark that we have advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/* 
 * <RECEIVER>
 *
 * This function is invoked when the second envelope from the <SENDER>
 * has been received, indicating that the long message has been
 * directly DMA PUT into the target buffer, and the request is now
 * complete.
 */
int 
lam_ssi_rpi_gm_long_receive_all_done(struct lam_ssi_rpi_gm_envl *env, 
                                     MPI_Request req)
{
  lam_debug_cond((PUB(did),
                  "<long>receiver_receive_all_done: got final envelope!"));
  req->rq_rpi->recv_advance_fn = NULL;

  /* Process the long body receive */

  receiver_recv_body(req);
  return 0;
}


/*
 * <RECEIVER>
 *
 * Called after the body of the message has actually been received.
 */
static int
receiver_recv_body(MPI_Request req)
{
  char *body;
  struct lam_ssi_rpi_gm_envl *env = req->rq_rpi->cq_envbuf;

  if (req->rq_rpi->dma_data_buf != NULL)
    body = req->rq_rpi->dma_data_buf;
  else
    body = req->rq_packbuf;

  /* The body of a long message has just been received.  We've
     already received the envelope, and we've already filled in the
     status.  So receive the body into the message buffer. */

  lam_debug_cond((lam_ssi_rpi_gm_did, 
		  "<long>receive_long: payload len: %d -- "
                  "posted rcv for len %d from %d",
		  env->ge_len, req->rq_packsize, req->rq_rank));

  /* increment "receive" bookmark to reflect this received message body */
  req->rq_proc->p_rpi->cp_in_nbytes += env->ge_len;
  
  /* If a temp DMA buffer was used, memcopy the data over to the real
     buffer and then unpin the temp buffer.  We know that we got only
     as many bytes as was posted in the receive -- length mismatch has
     already been handled and set in the status. */

  if (body != req->rq_packbuf) {
    lam_memcpy(req->rq_packbuf, body, env->ge_len);
    lam_ssi_rpi_gm_dma_free(body, req->rq_packsize);
    req->rq_rpi->dma_data_buf = NULL;
  }

  /* Regardless of how much data we received, we originally pinned
     req->rq_packsize.  So unuse the whole interval. */

  else {
    lam_ssi_rpi_gm_interval_unuse(req->rq_packbuf, req->rq_packsize);
  }

  /* The receiving portion of this request is finished */

  req->rq_rpi->recv_advance_fn = NULL;

  /* No need to remove this from the receive queue (even if this was
     originally MPI_ANY_SOURCE, that will have been resolved into a
     real process by now, and the req has long since been removed from
     everyone else's receive queues) -- the upper-level
     recv_events.c:receive_envelope() will take care of that for
     us. */

  /* This request is done -- regardless of whether it was a sync mode
     send or not (because we have already done an ACK with the remote
     side, hence the requirements for a sync send have definitely been
     met). */

  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;
  lam_ssi_rpi_gm_haveadv = 1;

  return 0;
}
