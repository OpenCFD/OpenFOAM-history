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
 * $Id: ssi_rpi_ib_recv_events.c,v 1.9 2003/12/27 06:20:46 vsahay Exp $
 *
 *	Function:	- IB low-level routines
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_recv_events.h>
#include <rpi_ib_recv_queue.h>
#include <rpi_ib_unexpected.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_tiny.h>
#include <rpi_ib_long.h>
#include <rpi_ib_util.h>
#include <rpi_ib_actions.h>


/*
 * An envelope has been received.  Process it, and find (or create) a
 * matching request.  
 *
 * - If this is only the first part of the message (i.e., long
 * protocol), put the matching request in
 * src_proc->cp_current_recv_req so that the next receive will go
 * straight to that request.
 *
 * - If the message was sent with the synchronous bit enabled, send
 * back an ACK.
 *
 * - If this is a tiny (non-synchronous) message, the message is done.
 *
 * This is an MPI_Request->recv_advance_fn function.  Therefore, it
 * needs to reset MPI_Request->recv_advance_fn when it completes.  
 */
int
lam_ssi_rpi_ib_receive_envelope(struct lam_ssi_rpi_proc *src_proc, 
                 struct lam_ssi_rpi_ib_envl *env)
{
  int ret;
  int num_to_repost;
  MPI_Request req;
  struct lam_ssi_rpi_ib_reqlist_item_t *rli;


  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "receive_envelope: rank %d, tag %d, cid %d, "
                  "flags 0x%x, len %d,",
                  env->ge_env.ce_rank,
                  env->ge_env.ce_tag,
                  env->ge_env.ce_cid,
                  env->ge_env.ce_flags,
                  env->ge_env.ce_len));

  /* Is this an ACK?  If so, it's not on any pending receive lists
     anymore; the ACK itself contains the address of the initiating
     request. */

  if ((env->ge_env.ce_flags & C2CACK) != 0) {
    req = env->ge_src_request;
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "MCW rank %d recv_envelope: got ACK with env: "
		    "len %d, flags %d, req %p",
                    lam_myproc->p_gps.gps_grank, 
                    env->ge_env.ce_len, env->ge_env.ce_flags, req));

    /* Process it */

    ret = req->rq_rpi->recv_advance_fn(req);
  }
  
  else
    /* Otherwise, this is a regular envelope.  Lookup to see if a
       matching receive for this message is already posted. */

    if ((rli = lam_ssi_rpi_ib_reqlist_match(&(src_proc->cp_pending_recvs), 
					    env)) != NULL) {
      lam_debug_cond((lam_ssi_rpi_ib_did, "receiving env: found match!"));
      req = rli->req;
      lam_ssi_rpi_ib_fill_wildcards(req, &(env->ge_env));

      /* First look at the matched request and see if it was a probe.
	 If it was, then mark the request done and turn envelope into an
	 unexpected message. */

      if (req->rq_type == LAM_RQIPROBE) {
	req->rq_state = LAM_RQSDONE;
	lam_ssi_rpi_fill_mpi_status(req, env->ge_env.ce_rank, 
				    env->ge_env.ce_tag, env->ge_env.ce_len);
	lam_ssi_rpi_ib_haveadv = 1;
	--lam_rq_nactv;

	lam_debug_cond((lam_ssi_rpi_ib_did, 
			"receiving env: matched a probe -- "
			"now unexpected env"));
	ret = lam_ssi_rpi_ib_unexpected_receive_env(src_proc, env);
      }
    
      /* Now look at the envelope, and dispatch accordingly.  Hence, we
	 dispatch on what the message actually is, not on what its
	 matched request says it should be -- this takes care of the
	 [erroneous but possible] case where the received message is
	 longer than the posted request. */
      /* NOTE: The ACK received by the <SENDER> from the <RECEIVER> in a
	 long message is handled in the above ACK-handling code; hence
	 it isn't a possibily listed below. */
      /* NOTE: Ditto for the 2nd envelope received by the <RECEIVER>
	 from the <SENDER> in a long message. */

      /* Receipt of a tiny message */

      else if (env->ge_env.ce_len <= lam_ssi_rpi_ib_tinymsglen) {
	lam_debug_cond((lam_ssi_rpi_ib_did,
			"receiving env: receive tiny env"));
	ret = lam_ssi_rpi_ib_tiny_receive_env(env, req);
      }

      /* LONG STARTS HERE  */


      /* Initial <RECEIVER> receipt of an envelope from the <SENDER> of
	 a long message.  Contrary to previous designs, we now treat
	 this just like tiny/short messages and remove this request from
	 all pending receive queues. */

      else {
	lam_debug_cond((lam_ssi_rpi_ib_did, 
			"receiving env: receive first long"));
	ret = lam_ssi_rpi_ib_long_receive_env_first(env, req);
      }

      /* If the posted receive is MPI_ANY_SOURCE, go remove this from
	 the pending_recvs queues on all the procs.  Otherwise, just
	 remove it from the pending_recvs queue on the source proc. */
    
      lam_debug_cond((lam_ssi_rpi_ib_did, 
		      "recv_events: removing req from recv queues"));
      if (rli->req->rq_rank == MPI_ANY_SOURCE)
	ret = lam_ssi_rpi_ib_rm_read_any_src(req, NULL);
      else
	ret = lam_ssi_rpi_ib_rm_read(src_proc, rli);
    }

  /* No, a receive for this envelope was not already posted -- this is
     an unexpected message.  Hence, by definition, it's not on any of
     the receive queues, and so we should not bother trying to remove
     it. */

    else {
      lam_debug_cond((lam_ssi_rpi_ib_did, 
		      "receiving env: receive unexpected env"));
      ret = lam_ssi_rpi_ib_unexpected_receive_env(src_proc, env);
    }

  /* Return the env buffer to the pool and repost a new env entry
     to the work queue */
  
  lam_debug_cond((PUB(did), "In recv event: Freeing buffer env"));

  lam_ssi_rpi_ib_reg_env_free(env);

  src_proc->preposted_envs[src_proc->prepost_index].freed = 1;

  lam_debug_cond((PUB(did),"RECV EVENT FREE: env is %x index is %d"
		  "freed is %d",
		  env, src_proc->prepost_index, 
		  src_proc->preposted_envs[src_proc->prepost_index].freed));

  if (src_proc->preposted_start == -1) {
      src_proc->preposted_start = src_proc->prepost_index;
  }

  ++src_proc->prepost_index;
  if (src_proc->prepost_index == PUB(num_envelopes))
      src_proc->prepost_index = 0;

  --src_proc->rem_recv_bufs;

  if (src_proc->rem_recv_bufs <= (PUB(num_envelopes) / 2)) {
    num_to_repost = PUB(num_envelopes) / 2;

    /* If num envelopes is odd, add 1 more */
    if (PUB(num_envelopes) % 2 == 1) {
      ++num_to_repost;
    }
    if (lam_ssi_rpi_ib_post_recv_buf(num_to_repost, src_proc) != 0)
      return LAMERROR;

    src_proc->rem_recv_bufs += num_to_repost;
  }

  /* Inform the peer about the env_cnt with RDMA  */

  if (lam_ssi_rpi_ib_fc_recv_buffer_available(src_proc) == LAMERROR)
    return LAMERROR;

  /* All done */

  lam_debug_cond((lam_ssi_rpi_ib_did, "receiving env: done"));
  return ret;
}


