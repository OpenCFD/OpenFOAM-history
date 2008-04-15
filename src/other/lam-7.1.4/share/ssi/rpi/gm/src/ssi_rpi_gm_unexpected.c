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
 * $Id: ssi_rpi_gm_unexpected.c,v 1.16 2003/11/12 04:14:54 jsquyres Exp $
 *
 *	Function:	- handle gm unexpected messages
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_gm.h>
#include <rpi_gm_unexpected.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_long.h>
#include <rpi_gm_ack.h>
#include <rpi_gm_util.h>
#include <rpi_gm_cbuf.h>


/*
 * Advance a request based on a buffered (i.e., previously unexpected)
 * message.
 */
int 
lam_ssi_rpi_gm_unexpected_advance(MPI_Request req, 
				  struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_gm_envl gmenv;
  
  gmenv.ge_len = msg->cm_env.ce_len;
  gmenv.ge_tag = msg->cm_env.ce_tag;
  gmenv.ge_flags = msg->cm_env.ce_flags;
  gmenv.ge_rank = msg->cm_env.ce_rank;
  gmenv.ge_cid = msg->cm_env.ce_cid;
  gmenv.ge_src_request = msg->cm_extra->lsrc_src_request;
#if LAM_SSI_RPI_GM_HAVE_RDMA_GET
  gmenv.ge_target = msg->cm_extra->lsrc_target;
#else
  /* This field is only used if we don't have RDMA get, and therefore
     use an alternate long message protocol. */
  gmenv.ge_dest_request = NULL;
#endif

  /* If the matched request is a probe, then set the status and leave
     the envelope buffered */

  if (req->rq_type == LAM_RQIPROBE) {
    lam_debug_cond((lam_ssi_rpi_gm_did,
                    "unexpected_advance: matched a probe"));
    lam_ssi_rpi_fill_mpi_status(req, gmenv.ge_rank, gmenv.ge_tag, 
                                gmenv.ge_len);
    req->rq_state = LAM_RQSDONE;
    --lam_rq_nactv;

    /* Return here, because dropping out of the bottom of this
       function will remove the message from the unexpected message
       queue. */

    return 0;
  }

  /* Special case of a synchronous send with the sender being the same
     as the receiver.  Copy the message data directly from the
     sender's buffer and both requests are done.  */

  else if (msg->cm_req != MPI_REQUEST_NULL) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "unexpected_advance: matched sync send to self"));
    if (gmenv.ge_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      gmenv.ge_len = req->rq_packsize;
    }
    
    if (gmenv.ge_len > 0)
      lam_memcpy(req->rq_packbuf, msg->cm_buf, gmenv.ge_len);

    lam_ssi_rpi_fill_mpi_status(req, gmenv.ge_rank, gmenv.ge_tag, 
                                gmenv.ge_len);
    req->rq_state = LAM_RQSDONE;
    msg->cm_req->rq_state = LAM_RQSDONE;
    lam_rq_nactv -= 2;
  }

  /* If the matches request is a long message, jump into the long
     message protocol (i.e., send an ACK) */

  else if (gmenv.ge_flags & C2CLONG) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "unexpected_advance: matched long"));

    /* Remove this envelope from the will-be-stale list */

    if (PUB(cr_available) == 1)
      lam_arr_remove(req->rq_proc->p_rpi->cp_unexpected_long_recvs, &msg);

    /* We already have a pinned buffer to receive into (whether it's
       the user's buffer or a temp buffer;
       lam_ssi_rpi_gm_setup_request_dma() was invoked in the rpi_start
       call).  Proceed as normal with the long protocol */

    if (lam_ssi_rpi_gm_long_receive_env_first(&gmenv, req) != 0)
      return LAMERROR;
  }

  /* Otherwise, it was a tiny message */
  
  else {
    lam_debug_cond((lam_ssi_rpi_gm_did,
                    "unexpected_advance: matched short/tiny"));

    /* If we've got the whole message, then the cm_proc pointer will
       be set to the sentinel value of NULL.  We can copy the message
       over to the destination and advance the request (potentially
       sending an ACK).  Don't worry about freeing the cm_buf; that
       will be handled in lam_ssi_rpi_cbuf_delete. */

    if (msg->cm_proc == NULL) {
      lam_debug_cond((lam_ssi_rpi_gm_did,
                      "unexpected_advance: have entire message"));
      if (gmenv.ge_len > req->rq_packsize) {
	req->rq_flags |= LAM_RQFTRUNC;
	gmenv.ge_len = req->rq_packsize;
      }
      
      if (gmenv.ge_len > 0)
	lam_memcpy(req->rq_packbuf, msg->cm_buf, gmenv.ge_len);
      lam_ssi_rpi_fill_mpi_status(req, gmenv.ge_rank, gmenv.ge_tag, 
                                  gmenv.ge_len);
      
      /* If this was not a synchronous send, it is done.  If it was a
         syncronous send, then we need to send an ACK back */

      if (gmenv.ge_flags & C2CSSEND) {
	req->rq_state = LAM_RQSACTIVE;
	req->rq_rpi->is_advancing = 1;
	if (lam_ssi_rpi_gm_send_ack_done(req, &gmenv) != 0)
	  return LAMERROR;
	lam_debug_cond((lam_ssi_rpi_gm_did,
                        "unexepcted_advance: sync ACK queued up"));
      } else {
	lam_debug_cond((lam_ssi_rpi_gm_did,
                        "unexpected_advance: all done!"));
	req->rq_state = LAM_RQSDONE;
	--lam_rq_nactv;
      }
    }
  }

  /* Discard the buffered message */

  free(msg->cm_extra);
  lam_ssi_rpi_cbuf_delete(msg);

  return 0;
}


/*
 * We have received an envelope for which we did not already have a
 * matching receive posted.  Look at the envelope and decide how to
 * buffer it.
 */
int 
lam_ssi_rpi_gm_unexpected_receive_env(struct lam_ssi_rpi_proc *src_proc, 
				      struct lam_ssi_rpi_gm_envl *env)
{
  struct lam_ssi_rpi_cbuf_msg cm;
  struct lam_ssi_rpi_cbuf_msg *saved_cm;

  /* In 64 bit mode, this struct may have extra padding */

  LAM_ZERO_ME(cm);
  cm.cm_env.ce_len = env->ge_len;
  cm.cm_env.ce_tag = env->ge_tag;
  cm.cm_env.ce_flags = env->ge_flags;
  cm.cm_env.ce_rank = env->ge_rank;
  cm.cm_env.ce_cid = env->ge_cid;
  cm.cm_req = MPI_REQUEST_NULL;
  cm.cm_dont_delete = 0;
  cm.cm_extra = malloc(sizeof(struct lam_ssi_rpi_cbuf));
  if (cm.cm_extra == NULL)
    return LAMERROR;
  cm.cm_extra->lsrc_src_request = env->ge_src_request;
#if LAM_SSI_RPI_GM_HAVE_RDMA_GET
  cm.cm_extra->lsrc_target = env->ge_target;
#endif

  /* Note that we add the C2CBUFFERED flag to the ce_flags so that we
     know later that this envelope buffer is not a pinned buffer. */
  
  cm.cm_env.ce_flags |= C2CBUFFERED;
  
  /* If it's a tiny message, then this is easy -- the entire message
     has already arrived.  Buffer the envelope and the message. */

  if (env->ge_len <= lam_ssi_rpi_gm_tinymsglen) {
    lam_debug_cond((lam_ssi_rpi_gm_did, "unexpected_recv: got tiny"));

    /* Set the cm_proc to be NULL as a sentinel that the entire body
       of the message has already been read in. */

    cm.cm_proc = NULL;

    /* Allocate a buffer just large enough and memcpy the body over */

    cm.cm_buf = malloc(env->ge_len);
    if (cm.cm_buf == NULL) {
      free(cm.cm_extra);
      return LAMERROR;
    }
    lam_memcpy(cm.cm_buf, env + 1, env->ge_len);

    /* Now add this message to the unexpected list */

    if (lam_ssi_rpi_cbuf_append(&cm) == NULL) {
      free(cm.cm_extra);
      free(cm.cm_buf);
      return LAMERROR;
    }
  }

  /* Otherwise, it's a long message */
  
  else {
    lam_debug_cond((lam_ssi_rpi_gm_did, "unexpected_recv: got long"));

    /* Just buffer up the envelope and don't send an ACK back -- we'll
       send it when a matching receive is posted. */

    cm.cm_buf = NULL;
    cm.cm_proc = NULL;

    /* Now add this message to the unexpected list */

    if ((saved_cm = lam_ssi_rpi_cbuf_append(&cm)) == NULL) {
      free(cm.cm_extra);
      return LAMERROR;
    }

    /* This is one of the situations where a checkpoint would cause a
       stale pointer upon continue/restart (i.e., the pointer to the
       remote buffer contained in this envelope).  So save this
       envelope on a list so that it can be chased down and refreshed
       upon continue/restart.*/

    if (PUB(cr_available) == 1)
      lam_arr_append(src_proc->cp_unexpected_long_recvs, &saved_cm);
  }

  /* All done */

  return 0;
}
