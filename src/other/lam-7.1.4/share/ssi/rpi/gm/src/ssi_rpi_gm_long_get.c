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
 * $Id: ssi_rpi_gm_long_get.c,v 1.9 2004/02/12 17:49:11 vsahay Exp $
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
#include <rpi_gm_cr.h>
#include <rpi_gm_actions.h>


/*
 * private functions
 */
static int sender_send_env(MPI_Request req);
static void sender_send_env_callback(struct gm_port *port, void *context, 
                                      gm_status_t status);

static int receiver_get_body(MPI_Request req);
static void receiver_get_body_callback_unpin(struct gm_port *port, 
                                             void *context, 
                                             gm_status_t status);
static void receiver_get_body_callback_no_unpin(struct gm_port *port, 
                                                void *context, 
                                                gm_status_t status);

static int receiver_send_ack(MPI_Request req);
static int receiver_send_ack_work(MPI_Request req);
static void receiver_send_ack_callback(struct gm_port *port, void *context, 
                                       gm_status_t status);

static int sender_recv_ack(MPI_Request req, char *bogus_recvd_data);


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
                  "<long>sender_send: sending envelope, request %p\n", req));

  /* Mark this request as in the progression engine */

  req->rq_rpi->is_advancing = 1;

  /* Once we send this envelope, the receiver will eventually send us
     an ACK, indicating that they have transferred the message to the
     destination (using an RDMA get).  So the next thing that we'll do
     is receive the ACK. */

  req->rq_rpi->recv_advance_fn = sender_recv_ack;

  /* If the dma_data_buf on the req is not NULL, then we could not pin
     and had to dma_malloc (way back in req_start).  Hence, we need to
     memcpy and setup to dma_free later. */

  if (req->rq_rpi->dma_data_buf != NULL) {
    req->rq_rpi->cq_envbuf->ge_target = req->rq_rpi->dma_data_buf;
    lam_memcpy(req->rq_rpi->dma_data_buf, req->rq_packbuf, req->rq_packsize);
  } else {
    req->rq_rpi->cq_envbuf->ge_target = req->rq_packbuf;
  }

  /* As soon as we transfer some bytes or queue up the envelope to go,
     we're non-cancellable */

  req->rq_state = LAM_RQSACTIVE;

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
     was set above (receive the ACK and complete the request). */

  else {

    /* We could just call sender_send_env() here, but all that does it
       make one function call to push_envelope, so save a little
       overhead and just call that directly here */

    lam_ssi_rpi_gm_push_envelope(req, sender_send_env_callback);
    return 0;
  }

  /* Never exit through here */
}


/*
 * <SENDER>
 *
 * This function is a wrapper around the actual gm send -- all the
 * setup has been done; we have enough send tokens.
 *
 * This is a send_advance_fn.  Reset it to NULL.
 */
static int 
sender_send_env(MPI_Request req)
{
  /* Now actually send the envelope */

  req->rq_rpi->send_advance_fn = NULL;
  lam_debug_cond((PUB(did), "<long>sender_send_env"));
  lam_ssi_rpi_gm_push_envelope(req, sender_send_env_callback);
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
     receive an ACK.  Increment the send tokens and mark that this RPI
     has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/* 
 * <RECEIVER>
 *
 * This function is invoked when the first envelope has been received.
 * It is only invoked when a matching receive has been posted.  For gm
 * 2.x, we initiate an RDMA "get" to transfer the message content from
 * the sender.
 *
 * This is the first "long" function invoked on the receiving side
 * when transferring a long message.
 */
int 
lam_ssi_rpi_gm_long_receive_env_first(struct lam_ssi_rpi_gm_envl *env, 
                                      MPI_Request req)
{
  int recv_len;
  lam_ssi_rpi_gm_envl_t *outenv = req->rq_rpi->cq_envbuf;

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>receiver_long_env_first: got the announce envelope"));

  /* Start the RDMA transfer as soon as possible; most of the
     bookkeeping can wait until we get the gm hardware working on the
     transfer.  However, only do RDMA if we actually have a non-zero
     body length. */

  recv_len = LAM_min(env->ge_len, req->rq_packsize);
  if (recv_len > 0) {

    /* Initiate the RDMA get */

    if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() || 
        (PUB(stokens) < 1) ||
        (1 == lam_ssi_rpi_gm_enqueue_sends)) {
      req->rq_rpi->send_advance_fn = receiver_get_body;
      LAM_SSI_RPI_GM_SEND_QUEUE_ADD(req);

      /* This is one of the two critical areas in a long receive that
         can cause a stale pointer upon continue/restart.  So record
         it. */

      if (PUB(cr_available) == 1)
        lam_arr_append(req->rq_proc->p_rpi->cp_pending_long_recvs, 
                       &req);

      /* Don't bother trying to advance the queue now -- every time we
         get a send token back, the queue is advanced.  Hence, we are
         guaranteed to not have enough send tokens to advance anything
         in the queue right now.  */
    } 

    /* Otherwise, we can do the RDMA get now.  The next action of this
       request will be triggered when the RDMA get completes.  */

    else {

      /* We could just call receiver_get_body() here, but all that
         does it make one function call to get_long_body, so save a
         little overhead and just call that directly here */

      lam_ssi_rpi_gm_get_long_body(req, env->ge_target, recv_len,
                                   receiver_get_body_callback_unpin,
                                   receiver_get_body_callback_no_unpin);
    }
  }

  req->rq_rpi->is_advancing = 1;

  /* Fill in the status */

  lam_ssi_rpi_fill_mpi_status(req, req->rq_rpi->cq_peer, 
                              req->rq_rpi->cq_envbuf->ge_tag, recv_len);
  
  /* Check to see if this is going to be a truncation error. */

  if (recv_len < env->ge_len)
    req->rq_flags |= LAM_RQFTRUNC;

  /* Prepare the ACK to be sent back (even though it won't be sent
     until later).  Include my identity (relative to the local group
     in the request's communicator).  Hence, this works even if this
     is an intercommunicator, because it will be relative to my
     group. */

  outenv->ge_flags |= C2CLONG | C2CACK;
  outenv->ge_len = recv_len;
  outenv->ge_rank = req->rq_comm->c_group->g_myrank;
  outenv->ge_tag = env->ge_tag;
  outenv->ge_cid = env->ge_cid;
  outenv->ge_target = env->ge_target;
  outenv->ge_src_request = env->ge_src_request;

  /* As soon as we transfer some bytes or queue up the RDMA get to go,
     we're non-cancellable */

  req->rq_state = LAM_RQSACTIVE;

  /* Check to see if this request posted a zero-byte receive.  If so,
     we didn't do RDMA above, and all we need to do is send back the
     ACK to complete the request. */

  if (recv_len == 0)
    return receiver_send_ack(req);

  /* All done */

  return 0;
}


/*
 * <RECEIVER>
 *
 * This is the worker function for initiating the RDMA get of the body
 * of the long message from the remote node directly into a target
 * buffer.  It is called after the receiver has received the first
 * envelope in a long send.  It is only invoked when we have enough
 * send tokens.
 *
 * This is a send_advance_fn function.
 */
static int 
receiver_get_body(MPI_Request req)
{
  int length = LAM_min(req->rq_rpi->cq_envbuf->ge_len, req->rq_packsize);

  /* Remove this request from the would-be-stale list */

  if (PUB(cr_available) == 1)
    lam_arr_remove(req->rq_proc->p_rpi->cp_pending_long_recvs, req);

  /* Initiate the RDMA get */

  req->rq_rpi->send_advance_fn = NULL;
  lam_ssi_rpi_gm_get_long_body(req, req->rq_rpi->cq_envbuf->ge_target,
                               length, receiver_get_body_callback_unpin,
                               receiver_get_body_callback_no_unpin);

  return 0;
}


/*
 * <RECEIVER>
 *
 * Callback for when the RDMA get of the body of the long message has
 * completed.  We knew ahead of time that we want to DMA free the
 * receive buffer (because it was a temporary buffer).
 */
static void
receiver_get_body_callback_unpin(struct gm_port *port, void *context, 
                                 gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(receiver_get_body_callback_unpin);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>receiver_get_body_callback_unpin: all done! %p "
                  "from peer %d", 
                  req, req->rq_rank));

  /* Now need to send the ACK back to the sender indicating that the
     message has been transferred.  We can't actually do a gm_send
     from here (because we're already in a callback), so queue it up
     to go after we finish here. */

  req->rq_rpi->send_advance_fn = receiver_send_ack_work;
  LAM_SSI_RPI_GM_ACK_QUEUE_ADD(req);

  /* Increment "receive" bookmark to reflect this received message
     body.  The number of bytes transferred was previously saved in
     the status.  */

  req->rq_proc->p_rpi->cp_in_nbytes += req->rq_status.st_length;
  
  /* memcpy the message to the final buffer and DMA free the data
     buffer (there's no pool to return it to).  The length that was
     transferred was saved in the status. */

  lam_memcpy(req->rq_packbuf, req->rq_rpi->dma_data_buf, 
             req->rq_status.st_length);
  lam_ssi_rpi_gm_dma_free(req->rq_rpi->dma_data_buf, req->rq_packsize);
  req->rq_rpi->dma_data_buf = NULL;

  /* No need to remove this from the receive queue (even if this was
     originally MPI_ANY_SOURCE, that will have been resolved into a
     real process by now, and the req has long since been removed from
     everyone else's receive queues) -- the upper-level
     recv_events.c:receive_envelope() will take care of that for
     us. */

  /* Increment the send tokens and mark that we have advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/*
 * <SENDER>
 *
 * Callback for when the RDMA get of the body of the long message has
 * completed.  We knew ahead of time that we do not want to unpin the
 * receive buffer (because it was a user-pinned buffer).
 */
static void 
receiver_get_body_callback_no_unpin(struct gm_port *port, void *context, 
                                    gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(receiver_get_body_callback_no_unpin);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>send_body_no_unpin_callback: all done! %p "
                  "to peer %d", 
                  req, req->rq_rank));

  /* Now need to send the ACK back to the sender indicating that the
     message has been transferred.  We can't actually do a gm_send
     from here (because we're already in a callback), so queue it up
     to go after we finish here. */

  req->rq_rpi->send_advance_fn = receiver_send_ack_work;
  LAM_SSI_RPI_GM_ACK_QUEUE_ADD(req);

  /* Increment "receive" bookmark to reflect this received message
     body.  The number of bytes transferred was previously saved in
     the status.  */

  req->rq_proc->p_rpi->cp_in_nbytes += req->rq_status.st_length;
  
  /* There's no unpinning to do in this callback.  Just unuse the
     interval.  Regardless of how much data we received, we originally
     pinned req->rq_packsize.  So unuse the whole interval. */

  lam_ssi_rpi_gm_interval_unuse(req->rq_packbuf, req->rq_packsize);

  /* No need to remove this from the receive queue (even if this was
     originally MPI_ANY_SOURCE, that will have been resolved into a
     real process by now, and the req has long since been removed from
     everyone else's receive queues) -- the upper-level
     recv_events.c:receive_envelope() will take care of that for
     us. */

  /* Increment the send tokens and mark that we have advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/************************************************************************/

/*
 * <RECEIVER>
 *
 * This is the top-level function for sending the ACK back to the
 * sender after the receiver has transferred the body of the long
 * message from the sender.  The ACK was previously setup and is ready
 * to be sent.
 *
 * This is a send_advance_fn function.
 */
static int 
receiver_send_ack(MPI_Request req)
{
  /* Similar to normal messages, if we don't have enough send tokens
     or if there's stuff already in the send queue, just queue it up
     for later sending.  ACKs take precedence over normal pending send
     messages, though, so don't bother checking the send queue (and if
     the ACK queue will be empty if there are any stokens available,
     so don't need to check if that's empty, either) */

  if ((PUB(stokens) < 1) || 
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    req->rq_rpi->send_advance_fn = receiver_send_ack_work;
    LAM_SSI_RPI_GM_ACK_QUEUE_ADD(req);

    /* Don't bother trying to advance the queue now -- every time we
       get a send token back, the queue is advanced.  Hence, we are
       guaranteed to not have enough send tokens to advance anything
       in the queue right now.  So just return. */

    return 0;
  } 

  /* Otherwise, we can try to send it now. */

  else {
    return receiver_send_ack_work(req);
  }

  /* Won't exit out of here */
}


/*
 * <RECEIVER>
 *
 * This is the low-level function for sending the ACK back to the
 * sender after the receiver has transferred the body of the long
 * message from the sender.  It will only be invoked when we have
 * enough send tokens.
 */
static int 
receiver_send_ack_work(MPI_Request req)
{
  /* Now try to send the ACK */

  req->rq_rpi->send_advance_fn = NULL;
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>receiver_send_ack_work: sending the ACK"));
  lam_ssi_rpi_gm_push_envelope(req, receiver_send_ack_callback);
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
                  "<long>receiver_send_ack_callback: ce_len %d",
                  req->rq_rpi->cq_envbuf->ge_len));

  /* This request is now done. */

  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;

  /* Increment the send tokens and mark that this RPI has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}


/**************************************************************************/

/*
 * <SENDER>
 *
 * Once the ACK is received, this function completes the request.
 *
 * This is the second "long" function invoked on the sending side when
 * transferring a long message.
 *
 * We got an ACK back from the receiver saying that it has finished
 * the RDMA get, transferring the content of the long message.  Hence,
 * this request is now done.  The ACK contains the actual number of
 * bytes transferred so that we can update our bookmark correctly.
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

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "<long>sender_recv_ack: ACK received for request %p", req));
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "<long>sender_recv_ack: ACK to rank %d, cid %d, tag %d (env tag %d), len %d",
                  req->rq_rank, req->rq_cid, req->rq_tag, 
                  env->ge_tag, env->ge_len));

  /* Update my bookmark; the ACK contains exactly how many bytes were
     transferred (which may be less than what we tried to send, in the
     case of a truncation error). */

  req->rq_proc->p_rpi->cp_out_nbytes += env->ge_len;

  /* Was this sent from a temporary DMA buffer? */

  if (req->rq_rpi->dma_data_buf != NULL) {
    
    /* Yes, this is a temporary DMA buffer.  DMA free it. */

    lam_ssi_rpi_gm_dma_free(req->rq_rpi->dma_data_buf, req->rq_packsize);
    req->rq_rpi->dma_data_buf = NULL;
  } else {

    /* No, this was a user-pinned buffer.  So just unused the whole
       interval (regardless of how many bytes were actually
       transferred). */

    lam_ssi_rpi_gm_interval_unuse(req->rq_packbuf, req->rq_packsize);
  }

  /* This request is now done. */

  req->rq_rpi->recv_advance_fn = NULL;
  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;
  lam_ssi_rpi_gm_haveadv = 1;

  /* All done */

  return 0;
}
