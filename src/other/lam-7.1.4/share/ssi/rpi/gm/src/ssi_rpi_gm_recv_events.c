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
 * $Id: ssi_rpi_gm_recv_events.c,v 1.24 2004/02/14 16:02:54 jsquyres Exp $
 *
 *	Function:	- GM low-level routines
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_gm.h>
#include <rpi_gm_recv_events.h>
#include <rpi_gm_recv_queue.h>
#include <rpi_gm_unexpected.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_tiny.h>
#include <rpi_gm_long.h>
#include <rpi_gm_util.h>


/*
 * there was an envelope received on gm -- handle it
 */
int
lam_ssi_rpi_gm_recv_event(gm_recv_event_t *event, int fast)
{
  lam_ssi_rpi_gm_envl_t *env;
  struct lam_ssi_rpi_proc *src_proc;
  int ret;
  MPI_Request req;
  struct lam_ssi_rpi_gm_reqlist_item_t *rli;

  void *recv_buffer = gm_ntohp(event->recv.buffer);

  /* Optimize: use the message directly, and don't copy it to the
     buffer */

  if (fast)
    env = gm_ntohp(event->recv.message);
  else
    env = recv_buffer;

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "receive_envelope: rank %d, tag %d, cid %d, "
                  "flags 0x%x, len %d",
                  env->ge_rank,
                  env->ge_tag,
                  env->ge_cid,
                  env->ge_flags,
                  env->ge_len));

  /* Get source process.  If we could lookup the cid directly, we
     could index the rank in the group and get the _proc that way.
     But since our CID's are not stored in an array, we exchange
     pointers during addprocs.  So it's still a direct lookup, but we
     have to send along the extra ge_peer_proc pointer (32 or 64
     bits). */

  src_proc = env->ge_peer_proc->p_rpi;

  /* update count of bytes received from this proc */

  if (PUB(cr_available) == 1)
    src_proc->cp_in_nbytes += gm_ntoh_u32(event->recv.length);

  /* Is this an ACK?  If so, it's not on any pending receive lists
     anymore; the ACK itself contains the address of the initiating
     request. */

  if ((env->ge_flags & C2CACK) != 0) {
    req = env->ge_src_request;
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "MCW rank %d recv_envelope: got ACK with env: len %d, flags %d, req %p",
                    lam_myproc->p_gps.gps_grank, 
                    env->ge_len, env->ge_flags, req));

    /* Process it */

    ret = req->rq_rpi->recv_advance_fn(req, (char*) env);
  }

#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
  /* This only happens if we don't have RDMA get.  If we *do* have
     RDMA get, then the long message protocol is complete without a
     C2C2ND envelope, and this code is not used. */

  /* If this is the 2nd envelope in a long message protocol, we can
     dispatch immediately to its matching request. */

  else if ((env->ge_flags & C2C2ND) != 0) {
    req = env->ge_dest_request;
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "receiving env: receive second long on request %p", 
                    req));

    /* Process it */

    ret = lam_ssi_rpi_gm_long_receive_all_done(env, req);
  }
#endif

  /* Otherwise, this is a regular envelope.  Lookup to see if a
     matching receive for this message is already posted. */

  else if ((rli = lam_ssi_rpi_gm_reqlist_match(&(src_proc->cp_pending_recvs), 
					       env)) != NULL) {
    lam_debug_cond((lam_ssi_rpi_gm_did, "receiving env: found match!"));
    req = rli->req;
    lam_ssi_rpi_gm_fill_wildcards(req, env);

    /* First look at the matched request and see if it was a probe.
       If it was, then mark the request done and turn envelope into an
       unexpected message. */

    if (req->rq_type == LAM_RQIPROBE) {
      req->rq_state = LAM_RQSDONE;
      lam_ssi_rpi_fill_mpi_status(req, env->ge_rank, 
                                  env->ge_tag, env->ge_len);
      lam_ssi_rpi_gm_haveadv = 1;
      --lam_rq_nactv;

      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "receiving env: matched a probe -- "
                      "now unexpected env"));
      ret = lam_ssi_rpi_gm_unexpected_receive_env(src_proc, env);
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

    else if (env->ge_len <= lam_ssi_rpi_gm_tinymsglen) {
      lam_debug_cond((lam_ssi_rpi_gm_did,
                      "receiving env: receive tiny env"));
      ret = lam_ssi_rpi_gm_tiny_receive_env(env, req);
    }

    /* Initial <RECEIVER> receipt of an envelope from the <SENDER> of
       a long message.  Contrary to previous designs, we now treat
       this just like tiny messages and remove this request from all
       pending receive queues. */

    else {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
		      "receiving env: receive first long"));
      ret = lam_ssi_rpi_gm_long_receive_env_first(env, req);
    }

    /* If the posted receive is MPI_ANY_SOURCE, go remove this from
       the pending_recvs queues on all the procs.  Otherwise, just
       remove it from the pending_recvs queue on the source proc. */
    
    lam_debug_cond((lam_ssi_rpi_gm_did, 
		    "recv_events: removing req from recv queues"));
    if (rli->req->rq_rank == MPI_ANY_SOURCE)
      ret = lam_ssi_rpi_gm_rm_read_any_src(req, NULL);
    else
      ret = lam_ssi_rpi_gm_rm_read(src_proc, rli);
  }

  /* No, a receive for this envelope was not already posted -- this is
     an unexpected message.  Hence, by definition, it's not on any of
     the receive queues, and so we should not bother trying to remove
     it. */

  else {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
		    "receiving env: receive unexpected env"));
    ret = lam_ssi_rpi_gm_unexpected_receive_env(src_proc, env);
  }

  /* We're done with the envelope, but don't bother returning it to
     the pool, because the calling function is just going to post it
     again. */

  /* All done */

  lam_debug_cond((lam_ssi_rpi_gm_did, "receiving env: done"));

  /* We know that we have a receive token, because we just finished a
     receive.  So we can just re-post the old receive buffer. */

  gm_provide_receive_buffer(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
                            (void *) recv_buffer,
                            lam_ssi_rpi_gm_env_min_size_for_length,
                            LAM_SSI_RPI_GM_PRIORITY);

  return ret;
}
