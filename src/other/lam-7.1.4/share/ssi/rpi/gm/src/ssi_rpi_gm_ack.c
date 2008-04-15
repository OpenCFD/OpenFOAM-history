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
 * $Id: ssi_rpi_gm_ack.c,v 1.15 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function:	- sending ACKs
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>
#include <lamdebug.h>

#include <rpi_gm.h>
#include <rpi_gm_ack.h>
#include <rpi_gm_actions.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_util.h>
#include <rpi_gm_proc.h>
#include <rpi_gm_send_queue.h>


/*
 * private functions
 */
static int send_ack(MPI_Request req);
static void ack_send_env_callback(struct gm_port *port, void *context, 
				  gm_status_t status);


/*
 * This function is invoked by a receive request that finds that it
 * needs to send an ACK back to the sender, because the sender used
 * synchronous mode to send the message.  Once this ACK is finished,
 * the entire request is finished because the entire message has
 * already been received.
 */
int 
lam_ssi_rpi_gm_send_ack_done(MPI_Request req, 
                             struct lam_ssi_rpi_gm_envl *env)
{
  /* Use the existing request -- it's ok to trash the fields because
     the message has already been fully received and the MPI_Status
     has already been filled in.  Pretend it's a tiny message with no
     payload. */

  req->rq_packsize = 0;
  req->rq_rpi->cq_envbuf->ge_flags =
    (env->ge_flags | C2CACK) & (~C2CBUFFERED);
  req->rq_rpi->cq_envbuf->ge_rank = req->rq_comm->c_group->g_myrank;
  req->rq_rpi->cq_envbuf->ge_cid = env->ge_cid;
  req->rq_rpi->cq_envbuf->ge_target = (gm_remote_ptr_t) NULL;
  req->rq_rpi->cq_envbuf->ge_src_request = env->ge_src_request;

  /* These are not used in an ACK, so purify them clean */

  LAM_ZERO_ME(req->rq_rpi->cq_envbuf->ge_len);

  /* JMS: An optimization here would be to make a new request to send
     the ACK back (and be later reaped as an orphan) and let the
     current request finish immediately.  This would potentially
     improve the latency of receivers of synchronous messages.
     However, for now, I'm still working on correctness, so I'll save
     the optimization for later.  :-) */
  /* If we can't actually send now, set the next action to be to send
     the actual ACK. */

  if ((PUB(stokens) < 1) || 
      (req->rq_proc->p_rpi->num_dropped_sends > 0) ||
      (1 == lam_ssi_rpi_gm_enqueue_sends))  {
    req->rq_rpi->send_advance_fn = send_ack;
    LAM_SSI_RPI_GM_ACK_QUEUE_ADD(req);

    /* Don't bother trying to advance the queue now -- every time we
       get a send token back, the queue is advanced.  Hence, we are
       guaranteed to not have enough send tokens to advance anything
       in the queue right now.  So just return. */

    return 0;
  } 

  /* Otherwise, we have enough tokens to send this request now */

  else {
    return send_ack(req);
  }

  /* Never exit through here */
}


/************************************************************************/


/*
 * <SENDER>
 *
 * This function is a wrapper around the actual gm send -- all the
 * setup has been done.
 */
static int 
send_ack(MPI_Request req)
{
  req->rq_rpi->send_advance_fn = NULL;
  lam_ssi_rpi_gm_push_envelope(req, ack_send_env_callback);
  return 0;
}


/*
 * <RECEIVER>
 *
 * This function is invoked when an ACK envelope has finished sending.
 * In this case, the request is done.
 */
static void 
ack_send_env_callback(struct gm_port *port, void *context, gm_status_t status)
{
  LAM_SSI_RPI_GM_SEND_CALLBACK_START(ack_send_env_callback);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
		  "<ack>send_env_callback: all done! %p\n", req));

  /* The request is now done */

  req->rq_state = LAM_RQSDONE;
  --lam_rq_nactv;

  /* Increment the send tokens and mark that this c2c RPI has advanced */

  LAM_SSI_RPI_GM_SEND_CALLBACK_FINISH;
}
