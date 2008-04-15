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
 * $Id: ssi_rpi_ib_long.c,v 1.4 2003/12/17 21:47:58 vsahay Exp $
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

#include <rpi_ib.h>
#include <rpi_ib_actions.h>
#include <rpi_ib_long.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_send_queue.h>
#include <rpi_ib_util.h>
#include <rpi_ib_recv_queue.h>
#include <rpi_ib_interval.h>


/* 
 * private variables
 */
static int num_recv_free_slots;
static int num_sent_env;
static int num_recvd_env;

#define JMS_DEBUG 0

/*
 * private functions
 */
static int receiver_send_ack(MPI_Request req);
static int sender_recv_ack(MPI_Request req);


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
lam_ssi_rpi_ib_long_send(MPI_Request req)
{
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  lam_debug_cond((lam_ssi_rpi_ib_did, 
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

  req->rq_rpi->cq_envbuf->ge_target = req->rq_packbuf;
#if JMS_DEBUG
  /* JMS debug */
  printf("rank %d: sending long buffer for get: %p\n", 
         lam_myproc->p_gps.gps_grank,
         req->rq_packbuf);
#endif
  
  /* Fill in the rkey for remote get needed by peer. This will go
     along with the envelope */
  
  req->rq_rpi->cq_envbuf->rkey = req->rq_rpi->long_src_rkey;
    
  /* As soon as we transfer some bytes or queue up the envelope to go,
     we're non-cancellable */

  req->rq_state = LAM_RQSACTIVE;

  /* We only need one send token here because we're only going to send
     out the envelope right now.  If we have to queue it up, mark the
     next action as sending the first part of the long. */
  /* NOTE: The envelope that is sent has a cr_rank of *my* rank (in
     this communicator) -- i.e., who the long message is *from*. */

  num_recvd_env = *(destproc->fc_remote_recv_env_cnt);
  num_sent_env = destproc->fc_local_sent_env_cnt;
  
  num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
  
  /* We cant post to the send queue now since the buffer on the receiver
     is not free to accept more packets. So just queue it up */

  /* VPS: For CR we need to check PUB(enqueue_sends) here too  */
  if (num_recv_free_slots == 0) {
    LAM_SSI_RPI_IB_SEND_QUEUE_ADD(req);  
    return 0;
  }

  /* Otherwise, we can send it now.  The next action of this request
     will be triggered by the ACK, which is the recv_advance_fn, which
     was set above (receive the ACK and complete the request). */
  
  else {
       
    lam_debug_cond((PUB(did), "<long>: send: sending addr %x rkey %x",
		    req->rq_rpi->cq_envbuf->ge_target,
		    req->rq_rpi->cq_envbuf->rkey));

    return lam_ssi_rpi_ib_push_envelope(req, 0);
  }

  /* Never exit through here */
}


/* 
 * <RECEIVER>
 *
 * This function is invoked when the first envelope has been received.
 * It is only invoked when a matching receive has been posted.  For ib
 * we initiate an RDMA "get" to transfer the message content from
 * the sender.
 *
 * This is the first "long" function invoked on the receiving side
 * when transferring a long message.
 */
int 
lam_ssi_rpi_ib_long_receive_env_first(struct lam_ssi_rpi_ib_envl *env, 
                                      MPI_Request req)
{
  int recv_len;
  lam_ssi_rpi_ib_envl_t *outenv = req->rq_rpi->cq_envbuf;

  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "<long>receiver_long_env_first: got the announce envelope"));

  /* Start the RDMA transfer as soon as possible; most of the
     bookkeeping can wait until we get the ib hardware working on the
     transfer.  However, only do RDMA if we actually have a non-zero
     body length. */

  recv_len = LAM_min(env->ge_env.ce_len, req->rq_packsize);
  if (recv_len > 0) {

    /* Initiate the RDMA get */

#if JMS_DEBUG
    /* JMS debug */
    printf("rank %d: doing a get of %d bytes from addr %p to addr %p\n",
           lam_myproc->p_gps.gps_grank,
           recv_len,
           env->ge_target,
           req->rq_packbuf);
#endif
    lam_ssi_rpi_ib_get_long_body(env->ge_target, env->rkey, 
				 recv_len, req);
  }

  req->rq_rpi->is_advancing = 1;

  /* Fill in the status */

  lam_ssi_rpi_fill_mpi_status(req, req->rq_rpi->cq_peer, 
                              req->rq_rpi->cq_envbuf->ge_env.ce_tag, recv_len);
  
  /* Check to see if this is going to be a truncation error. */

  if (recv_len < env->ge_env.ce_len)
    req->rq_flags |= LAM_RQFTRUNC;

  /* Prepare the ACK to be sent back (even though it won't be sent
     until later).  Include my identity (relative to the local group
     in the request's communicator).  Hence, this works even if this
     is an intercommunicator, because it will be relative to my
     group. */

  outenv->ge_env.ce_flags |= C2CLONG | C2CACK;
  outenv->ge_env.ce_len = recv_len;
  outenv->ge_env.ce_rank = req->rq_comm->c_group->g_myrank;
  outenv->ge_env.ce_tag = env->ge_env.ce_tag;
  outenv->ge_env.ce_cid = env->ge_env.ce_cid;
  outenv->ge_target = env->ge_target;
  outenv->ge_src_request = env->ge_src_request;
  outenv->ge_dest_request = req;

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
 * Callback for when the RDMA get of the body of the long message has
 * completed.  We knew ahead of time that we want to DMA free the
 * receive buffer (because it was a temporary buffer).
 */
void
lam_ssi_rpi_ib_receiver_get_body_callback(MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "<long>receiver_get_body_callback:"
		  "sending ack to sender! %p "
                  "from peer %d", 
                  req, req->rq_rank));

#if JMS_DEBUG
  /* JMS debug */
  printf("rank %d: ib long get callback -- buffer %p: %d %d %d\n", 
         lam_myproc->p_gps.gps_grank,
         req->rq_packbuf,
         req->rq_packbuf[0],
         req->rq_packbuf[1],
         req->rq_packbuf[2]);
#endif

  /* Send the ack to the sender now */
  /* req->env has been filled for C2CACK earlier  */

  receiver_send_ack(req);

  /* Unuse (not unpin) the memory buffer which was pinned */
  lam_ssi_rpi_ib_interval_unuse(req->rq_packbuf, req->rq_packsize);

  /* No need to remove this from the receive queue (even if this was
     originally MPI_ANY_SOURCE, that will have been resolved into a
     real process by now, and the req has long since been removed from
     everyone else's receive queues) -- the upper-level
     recv_events.c:receive_envelope() will take care of that for
     us. */

  /* Mark that we have advanced */

  lam_ssi_rpi_ib_haveadv = 1;
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
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  
  num_recvd_env = *(destproc->fc_remote_recv_env_cnt);
  num_sent_env = destproc->fc_local_sent_env_cnt;

  num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
  
  /* We cant post to the send queue now since the buffer on the receiver
     is not free to accept more packets. So just queue it up */

  /* VPS: For CR we need to check PUB(enqueue_sends) here too  */
  if (num_recv_free_slots == 0) {
    LAM_SSI_RPI_IB_ACK_QUEUE_ADD(req);  
    return 0;
  }

  /* num_sent_env >= num_recvd_env always holds true */

  /* Post it to the send queue now  */
  else {
    lam_debug_cond((PUB(did), "<long> <ack_send> "));
    req->rq_rpi->send_advance_fn = NULL;
    return lam_ssi_rpi_ib_push_envelope(req, 0);
  }


  /* Won't exit out of here */
}


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
sender_recv_ack(MPI_Request req)
{

  lam_debug_cond((lam_ssi_rpi_ib_did,
                  "<long>sender_recv_ack: ACK received for request %p", req));

  /* Update my bookmark; the ACK contains exactly how many bytes were
     transferred (which may be less than what we tried to send, in the
     case of a truncation error). */

#if 0
  req->rq_proc->p_rpi->cp_out_nbytes += env->ge_len;
#endif

  /* This was a user-pinned buffer.  So just unused the whole
     interval (regardless of how many bytes were actually
     transferred). */

  lam_ssi_rpi_ib_interval_unuse(req->rq_packbuf, req->rq_packsize);

  /* This request is now done. */

  req->rq_rpi->recv_advance_fn = NULL;
  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;
  lam_ssi_rpi_ib_haveadv = 1;

  /* All done */

  return 0;
}
