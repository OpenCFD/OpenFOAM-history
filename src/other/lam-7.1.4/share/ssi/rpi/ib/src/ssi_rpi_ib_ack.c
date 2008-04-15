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
 * $Id: ssi_rpi_ib_ack.c,v 1.3 2003/12/17 21:47:58 vsahay Exp $
 *
 *	Function:	- sending ACKs
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_ack.h>
#include <rpi_ib_actions.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_util.h>
#include <rpi_ib_proc.h>
#include <rpi_ib_send_queue.h>


/* 
 * private variables
 */
static int num_recv_free_slots;
static int num_sent_env;
static int num_recvd_env;

/*
 * private functions
 */
static int send_ack(MPI_Request req);


/*
 * This function is invoked by a receive request that finds that it
 * needs to send an ACK back to the sender, because the sender used
 * synchronous mode to send the message.  Once this ACK is finished,
 * the entire request is finished because the entire message has
 * already been received.
 */
int 
lam_ssi_rpi_ib_send_ack_done(MPI_Request req, 
                             struct lam_ssi_rpi_ib_envl *env)
{
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;
    
  /* Use the existing request -- it's ok to trash the fields because
     the message has already been fully received and the MPI_Status
     has already been filled in.  Pretend it's a tiny message with no
     payload. */

  req->rq_packsize = 0;
  req->rq_rpi->cq_envbuf->ge_env.ce_flags =
    (env->ge_env.ce_flags | C2CACK) & (~C2CBUFFERED);
  req->rq_rpi->cq_envbuf->ge_env.ce_rank = req->rq_comm->c_group->g_myrank;
  req->rq_rpi->cq_envbuf->ge_env.ce_cid = env->ge_env.ce_cid;
  req->rq_rpi->cq_envbuf->ge_target = NULL;

  req->rq_rpi->cq_envbuf->ge_src_request = env->ge_src_request;

  /* making it to point to self req of the sender, which can be then
     used in completion to mark this request done once ACK sent */

  req->rq_rpi->cq_envbuf->ge_dest_request = req;

  /* These are not used in an ACK, so purify them clean */

  LAM_ZERO_ME(req->rq_rpi->cq_envbuf->ge_env.ce_len);

  /* JMS: An optimization here would be to make a new request to send
     the ACK back (and be later reaped as an orphan) and let the
     current request finish immediately.  This would potentially
     improve the latency of receivers of synchronous messages.
     However, for now, I'm still working on correctness, so I'll save
     the optimization for later.  :-) */
  /* If we can't actually send now, set the next action to be to send
     the actual ACK. */

  num_recvd_env = *(destproc->fc_remote_recv_env_cnt);
  num_sent_env = destproc->fc_local_sent_env_cnt;

  num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
  
  /* We cant post to the send queue now since the buffer on the receiver
     is not free to accept more packets. So just queue it up */

  /* VPS: For CR we need to check PUB(enqueue_sends) here too  */
  if (num_recv_free_slots == 0) {
    req->rq_rpi->send_advance_fn = send_ack;
    LAM_SSI_RPI_IB_ACK_QUEUE_ADD(req);  
    return 0;
  }

  /* num_sent_env >= num_recvd_env always holds true */

  /* Post it to the send queue now  */
  else {
    lam_debug_cond((PUB(did), "incremented num_sent_env : %d", num_sent_env));
    req->rq_rpi->send_advance_fn = NULL;
    return send_ack(req);
  }

  /* Never exit through here */
}



/*
 * <RECEIVER>
 *
 * This function is invoked when an ACK envelope has finished sending.
 * In this case, the request is done.
 */
void 
lam_ssi_rpi_ib_ack_send_env_callback(MPI_Request req)
{

  
  lam_debug_cond((lam_ssi_rpi_ib_did, 
		  "<ack>send_env_callback: all done! %p\n", req));

  /* The request is now done */

  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;

  /* Mark that this c2c RPI has advanced */

  lam_ssi_rpi_ib_haveadv = 1;
}


/************************************************************************/


/*
 * <SENDER>
 *
 * This function is a wrapper around the actual ib send -- all the
 * setup has been done.
 */
static int 
send_ack(MPI_Request req)
{
  if (lam_ssi_rpi_ib_push_envelope(req, 0) != 0)
    return LAMERROR;

  return 0;
}

