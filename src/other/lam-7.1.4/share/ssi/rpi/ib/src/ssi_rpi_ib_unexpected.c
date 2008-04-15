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
 * $Id: ssi_rpi_ib_unexpected.c,v 1.5 2003/12/29 22:00:41 vsahay Exp $
 *
 *	Function:	- handle ib unexpected messages
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_unexpected.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_long.h>
#include <rpi_ib_ack.h>
#include <rpi_ib_util.h>
#include <rpi_ib_cbuf.h>


/*
 * Advance a request based on a buffered (i.e., previously unexpected)
 * message.
 */
int 
lam_ssi_rpi_ib_unexpected_advance(MPI_Request req, 
				  struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_ib_envl ibenv;
  struct lam_ssi_rpi_envl *env = &(ibenv.ge_env);
  
  ibenv.ge_env = msg->cm_env;
  ibenv.ge_src_request = msg->cm_extra->lsrc_src_request;

  /* LONG */
  ibenv.ge_target = msg->cm_extra->lsrc_target;
  ibenv.rkey = msg->cm_extra->rkey;

  lam_debug_cond((PUB(did), "MATCHED: target %x rkey %x", 
		  ibenv.ge_target, ibenv.rkey));

  /* If the matched request is a probe, then set the status and leave
     the envelope buffered */

  if (req->rq_type == LAM_RQIPROBE) {
    lam_debug_cond((lam_ssi_rpi_ib_did,
                    "unexpected_advance: matched a probe"));
    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);
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
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "unexpected_advance: matched sync send to self"));
    if (env->ce_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      env->ce_len = req->rq_packsize;
    }
    
    if (env->ce_len > 0)
      lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);
    req->rq_state = LAM_RQSDONE;
    msg->cm_req->rq_state = LAM_RQSDONE;
    lam_rq_nactv -= 2;
  }

  /* if it was send_to_self and non-sync */
  else if (msg->cm_extra->was_sent_to_self_non_sync) {
    lam_debug_cond((PUB(did), "Matched with self - non sync"));
    if (env->ce_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      env->ce_len = req->rq_packsize;
    }
    
    if (env->ce_len > 0)
      lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);
    free(msg->cm_buf);
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }
    
  /* LONG  */

  /* If the matches request is a long message, jump into the long
     message protocol */

  else if (env->ce_len > lam_ssi_rpi_ib_tinymsglen) {
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "unexpected_advance: matched long"));

    /* We already have a pinned buffer to receive into (whether it's
       the user's buffer or a temp buffer;
       lam_ssi_rpi_ib_setup_request_dma() was invoked in the rpi_start
       call).  Proceed as normal with the long protocol */

    if (lam_ssi_rpi_ib_long_receive_env_first(&ibenv, req) != 0)
      return LAMERROR;
  }

  /* Otherwise, it was a tiny message */
  
  else {
    lam_debug_cond((lam_ssi_rpi_ib_did,
                    "unexpected_advance: matched short/tiny"));

    /* If we've got the whole message, then the cm_proc pointer will
       be set to the sentinel value of NULL.  We can copy the message
       over to the destination and advance the request (potentially
       sending an ACK).  Don't worry about freeing the cm_buf; that
       will be handled in lam_ssi_rpi_cbuf_delete. */

    if (msg->cm_proc == NULL) {
      lam_debug_cond((lam_ssi_rpi_ib_did,
                      "unexpected_advance: have entire message"));
      if (env->ce_len > req->rq_packsize) {
	req->rq_flags |= LAM_RQFTRUNC;
	env->ce_len = req->rq_packsize;
      }
      
      if (env->ce_len > 0)
	lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);
      lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);
      
      /* If this was not a synchronous send, it is done.  If it was a
         syncronous send, then we need to send an ACK back */

      if (env->ce_flags & C2CSSEND) {
	req->rq_state = LAM_RQSACTIVE;
	req->rq_rpi->is_advancing = 1;
	if (lam_ssi_rpi_ib_send_ack_done(req, &ibenv) != 0)
	  return LAMERROR;
	lam_debug_cond((lam_ssi_rpi_ib_did,
                        "unexepcted_advance: sync ACK queued up"));
      } else {
	lam_debug_cond((lam_ssi_rpi_ib_did,
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
lam_ssi_rpi_ib_unexpected_receive_env(struct lam_ssi_rpi_proc *src_proc, 
				      struct lam_ssi_rpi_ib_envl *env)
{
  struct lam_ssi_rpi_cbuf_msg cm;

  /* In 64 bit mode, this struct may have extra padding */

  LAM_ZERO_ME(cm);
  cm.cm_env = env->ge_env;
  cm.cm_req = MPI_REQUEST_NULL;
  cm.cm_dont_delete = 0;
  cm.cm_extra = malloc(sizeof(struct lam_ssi_rpi_cbuf));
  if (cm.cm_extra == NULL)
    return LAMERROR;
  cm.cm_extra->lsrc_src_request = env->ge_src_request;

  /* LONG  */
  cm.cm_extra->lsrc_target = env->ge_target;
  cm.cm_extra->rkey = env->rkey;

  cm.cm_extra->was_sent_to_self_non_sync = 0;

  lam_debug_cond((PUB(did), "UNEXPECTED: target %x key %x",
		  cm.cm_extra->lsrc_target, cm.cm_extra->rkey));
  
  /* Note that we add the C2CBUFFERED flag to the ce_flags so that we
     know later that this envelope buffer is not a pinned buffer. */
  
  cm.cm_env.ce_flags |= C2CBUFFERED;
  
  /* If it's a tiny message, then this is easy -- the entire message
     has already arrived.  Buffer the envelope and the message. */

  if (env->ge_env.ce_len <= lam_ssi_rpi_ib_tinymsglen) {
    lam_debug_cond((lam_ssi_rpi_ib_did, "unexpected_recv: got tiny"));

    /* Set the cm_proc to be NULL as a sentinel that the entire body
       of the message has already been read in. */

    cm.cm_proc = NULL;

    /* Allocate a buffer just large enough and memcpy the body over */

    cm.cm_buf = malloc(env->ge_env.ce_len);
    if (cm.cm_buf == NULL) {
      free(cm.cm_extra);
      return LAMERROR;
    }
    lam_memcpy(cm.cm_buf, env + 1, env->ge_env.ce_len);

    /* Now add this message to the unexpected list */

    if (lam_ssi_rpi_cbuf_append(&cm) == NULL) {
      free(cm.cm_extra);
      free(cm.cm_buf);
      return LAMERROR;
    }
  }

  /* LONG  */

  /* Otherwise, it's a long message */
  
  else {
    lam_debug_cond((lam_ssi_rpi_ib_did, "unexpected_recv: got long"));

    /* Just buffer up the envelope and don't send an ACK back -- we'll
       send it when a matching receive is posted. */

    cm.cm_buf = NULL;
    cm.cm_proc = NULL;

    /* Now add this message to the unexpected list */

    if (lam_ssi_rpi_cbuf_append(&cm) == NULL) {
      free(cm.cm_extra);
      return LAMERROR;
    }
  }

  /* All done */

  return 0;
}
