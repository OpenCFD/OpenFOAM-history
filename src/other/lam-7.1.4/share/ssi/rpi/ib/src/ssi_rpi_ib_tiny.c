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
 * $Id: ssi_rpi_ib_tiny.c,v 1.5 2003/12/17 21:47:58 vsahay Exp $
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

#include <rpi_ib.h>
#include <rpi_ib_tiny.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_send_queue.h>
#include <rpi_ib_recv_queue.h>
#include <rpi_ib_util.h>
#include <rpi_ib_ack.h>
#include <rpi_ib_actions.h>


/* 
 * private variables
 */
static int num_recv_free_slots;
static int num_sent_env;
static int num_recvd_env;


/*
 * private functions
 */
static int ib_tiny_send(MPI_Request req, int sync);
static int tiny_complete_send_request(MPI_Request req);

/*
 * <SENDER>
 *
 * Try to send a tiny message. This cannot be a #define, since will be
 * used as a function pointer later on.
 *
 * Returns 0 or LAMERROR.
 */

int
lam_ssi_rpi_ib_tiny_send(MPI_Request req)
{
  if (ib_tiny_send(req, 0) != 0)
    return LAMERROR;

  return 0;
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
lam_ssi_rpi_ib_tiny_send_sync(MPI_Request req)
{
  /* When the ACK is received, it is technically a "receive" action.
     So set the function to complete this request on the recv_fn
     function.  When the ACK is received, this request is finished. */

  req->rq_rpi->recv_advance_fn = tiny_complete_send_request;

  /* Call the normal tiny send -- use exactly the same code */

  if (ib_tiny_send(req, 1) != 0)
    return LAMERROR;

  return 0;
}


/*
 * <RECEIVER>
 *
 * This function is invoked when we have received an envelope for a
 * tiny message.  Therefore, we've received an entire tiny message.
 */
int 
lam_ssi_rpi_ib_tiny_receive_env(struct lam_ssi_rpi_ib_envl *env, 
                                MPI_Request req)
{
  /* Copy the message to the destination buffer.  Sender made right,
     so we don't need to worry about endian here.  */
  /* Check for length mismatch */

  if (env->ge_env.ce_len > 0) {
    lam_debug_cond((lam_ssi_rpi_ib_did, 
		    "receive_tiny: len payload>0: %d -- "
		    "posted rcv for len %d",
		    env->ge_env.ce_len, req->rq_packsize));
    if (env->ge_env.ce_len <= req->rq_packsize) {
      lam_memcpy(req->rq_packbuf, env + 1, env->ge_env.ce_len);
      lam_debug_cond((PUB(did), "received msg len is %d msg is %s",
		      env->ge_env.ce_len, req->rq_packbuf));
    } else {
      lam_memcpy(req->rq_packbuf, env + 1, req->rq_packsize);
      req->rq_flags |= LAM_RQFTRUNC;
    }
  }
  lam_ssi_rpi_fill_mpi_status(req, env->ge_env.ce_rank, env->ge_env.ce_tag, 
                              env->ge_env.ce_len);
  lam_debug_cond((lam_ssi_rpi_ib_did, 
		  "receive_tiny: filled status: rank:%d, tag:%d, len:%d",
		  env->ge_env.ce_rank, env->ge_env.ce_tag, 
                  env->ge_env.ce_len));
  lam_ssi_rpi_ib_haveadv = 1;

  /* The receiving portion of this request is finished */

  req->rq_rpi->recv_advance_fn = NULL;

  /* Was this a sync mode send that requires an ACK?  If so, the
     request won't be finished until the ACK has successfully been
     sent.  Otherwise, it's done now. */

  if (env->ge_env.ce_flags & C2CSSEND) {
    lam_debug_cond((lam_ssi_rpi_ib_did, 
		    "receive_tiny: sending an ACK because this was an ssend"));

    /* Send the ACK now  */

    if (lam_ssi_rpi_ib_send_ack_done(req, env) != 0)
      return LAMERROR;
    lam_debug_cond((lam_ssi_rpi_ib_did, "receive_tiny: ACK queued up"));
  } else {
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;
  }  
  
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
void 
lam_ssi_rpi_ib_tiny_send_env_callback(MPI_Request req)
{

  lam_debug_cond((lam_ssi_rpi_ib_did, 
		  "<tiny>send_env_callback: all done! %p\n", req));

  /* Alter the envelope as described in send_tiny() */

  if (req->rq_rpi->cq_envbuf->ge_env.ce_flags & C2CSSEND) {
    req->rq_rpi->cq_envbuf->ge_env.ce_rank = req->rq_rank;
    req->rq_rpi->cq_envbuf->ge_env.ce_flags |= C2CACK;
  }
  
  /* If this is LONG env send callback, simply mark the request
     advancing and return. We cannot mark the request as DONE until
     we get an ACK back from the receiver */

  if (req->rq_rpi->cq_envbuf->ge_env.ce_flags & C2CLONG) {
    lam_debug_cond((PUB(did), "<long>: send env callback"));
    lam_ssi_rpi_ib_haveadv = 1;
    return;
  }

  /* VPS: The only reason for not using UNSIGNALED send is that if we
     mark it as done as soon as it enters the work queue, it may
     happen that in "advance" it will be skipped, and later on the env
     buffer will be destroyed in the rpi_destroy before the send is
     complete - We dont want that - Someday gotto know when send is
     complete using the FC stuff rather than completion entry for
     performance improvement */

  if ((req->rq_rpi->cq_envbuf->ge_env.ce_flags & C2CSSEND) == 0) {
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "<tiny>send_env_callback: not a sync, so done!"));
    req->rq_state = LAM_RQSDONE;

    --lam_rq_nactv;
  }

  /* Mark that this c2c RPI has advanced */

  lam_ssi_rpi_ib_haveadv = 1;
}


/************************************************************************/

/* SENDER
 * 
 * tiny send function common for sync and non-sync tiny send 
 */
int 
ib_tiny_send(MPI_Request req, int sync)
{

  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  lam_debug_cond((lam_ssi_rpi_ib_did,
		  "send_tiny: sending request %p\n", req));

  /* Mark this request as in the progression engine */

  req->rq_rpi->is_advancing = 1;

  /* As soon as we transfer some bytes or queue up the envelope to go,
     we're non-cancellable */

  req->rq_state = LAM_RQSACTIVE;
  num_recvd_env = *(destproc->fc_remote_recv_env_cnt);
  num_sent_env = destproc->fc_local_sent_env_cnt;

  num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
  lam_debug_cond((PUB(did), "tiny send: Num recvd env = %d sent = %d",
		  num_recvd_env, num_sent_env));
  lam_debug_cond((PUB(did), "tiny send: Number of free slots = %d",
		  num_recv_free_slots));

  /* We cant post to the send queue now since the buffer on the receiver
     is not free to accept more packets. So just queue it up */

  /* VPS: For CR we need to check PUB(enqueue_sends) here too  */
  if (num_recv_free_slots == 0) {
    lam_debug_cond((PUB(did), "tiny send: Not sufficient slots...queuing "));
    LAM_SSI_RPI_IB_SEND_QUEUE_ADD(req);  
    return 0;
  }

  /* num_sent_env >= num_recvd_env always holds true */

  /* Post it to the send queue now  */
  else {
    req->rq_rpi->send_advance_fn = NULL;
    if (lam_ssi_rpi_ib_push_envelope(req, sync) != 0)
      return LAMERROR;
    return 0;
  }

  /* Never exit through here */
}


/*
 * <SENDER>
 *
 * This function is invoked after an ACK is received.  It means that
 * the request is now finished, and we should move it into the done
 * state.  No data will be passed through the second argument.
 */
int 
tiny_complete_send_request(MPI_Request req)
{
  lam_debug_cond((lam_ssi_rpi_ib_did, "<tiny>completed send request"));

  /* This request is done */

  req->rq_state = LAM_RQSDONE;
  lam_ssi_rpi_ib_haveadv = 1;
  --lam_rq_nactv;

  return 0;
}

