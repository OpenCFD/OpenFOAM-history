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
 * $Id: ssi_rpi_ib_send_queue.c,v 1.4 2003/12/17 21:47:58 vsahay Exp $
 *
 *	Function: - queue for sends that were not able to go right
 *	away because of lack of resources (send tokens or long message
 *	tags)
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_bitmap.h>
#include <rpi_ib_send_queue.h>
#include <rpi_ib_actions.h>

/*
 * public variables
 *
 * This variable is only public for a slight optimization -- the
 * "function" to add a request to the send queue is actually a #define
 * since it's only a one liner.  As such, all modules need to "see"
 * this variable.
 */
struct lam_ssi_rpi_ib_reqlist_t lam_ssi_rpi_ib_send_queue;
struct lam_ssi_rpi_ib_reqlist_t lam_ssi_rpi_ib_ack_queue;

/*
 * Local variables
 */

static int num_procs;

/*
 * local functions
 */
static int advance_acks(void);
static int advance_sends(void);


/*
 * initialize this module
 */
int
lam_ssi_rpi_ib_send_queue_init(void)
{
  int i;
  struct _proc **array, *p;
  array = lam_arr_get(PUB(procs));
  num_procs = lam_arr_size(PUB(procs));

  for (i = 0; i < num_procs; ++i) {
    p = array[i];
    if (p == lam_myproc)
      continue;
    lam_ssi_rpi_ib_reqlist_init(&(p->p_rpi->cp_pending_sends));
    lam_ssi_rpi_ib_reqlist_init(&(p->p_rpi->cp_pending_acks));
  }

  lam_ssi_rpi_ib_reqlist_init(&lam_ssi_rpi_ib_send_queue);
  lam_ssi_rpi_ib_reqlist_init(&lam_ssi_rpi_ib_ack_queue);

  return 0;
}


/*
 * add a request to the send queue
 */
int
lam_ssi_rpi_ib_send_queue_add(MPI_Request req)
{
  /* If entry for the proc already in the common queue, dont add in
     the common queue, just add in the proc queue */
  if (!lam_ssi_rpi_ib_bitmap_is_set(lam_ssi_rpi_ib_pending_send_proc_bitmap, 
				    req->rq_proc->p_rpi->myindex)) {
    lam_ssi_rpi_ib_bitmap_set_elem(lam_ssi_rpi_ib_pending_send_proc_bitmap, 
				   req->rq_proc->p_rpi->myindex);
    lam_ssi_rpi_ib_reqlist_append(&lam_ssi_rpi_ib_send_queue, (req)); 
  }

  lam_ssi_rpi_ib_reqlist_append(&(req->rq_proc->p_rpi->cp_pending_sends), req);
  return 0;
}


/*
 * Add a request to the ack queue
 */
int
lam_ssi_rpi_ib_ack_queue_add(MPI_Request req)
{
 /* If entry for the proc already in the common queue, dont add in
     the common queue, just add in the proc queue */
  if (!lam_ssi_rpi_ib_bitmap_is_set(lam_ssi_rpi_ib_pending_ack_proc_bitmap, 
				    req->rq_proc->p_rpi->myindex)) {
    lam_ssi_rpi_ib_bitmap_set_elem(lam_ssi_rpi_ib_pending_ack_proc_bitmap, 
				   req->rq_proc->p_rpi->myindex);
    lam_ssi_rpi_ib_reqlist_append(&lam_ssi_rpi_ib_ack_queue, (req)); 
  }

  lam_ssi_rpi_ib_reqlist_append(&(req->rq_proc->p_rpi->cp_pending_acks), req);
  return 0;
}
 

/*
 * Advance the send queue
 */
int
lam_ssi_rpi_ib_send_queue_advance(void)
{
  int ret;
  if (!LAM_SSI_RPI_IB_ACK_QUEUE_EMPTY()) {
    ret = advance_acks();
    if (ret != 0)
      return ret;
  }

  if (!LAM_SSI_RPI_IB_SEND_QUEUE_EMPTY()) {
    ret = advance_sends();
    if (ret != 0)
      return ret;
  }

  return 0;
}

/************************************************************************/


/*
 * Back-end advance of the pending acks queue
 */
static int
advance_acks(void)
{
  MPI_Request req;
  struct lam_ssi_rpi_proc *p;
  struct lam_ssi_rpi_ib_reqlist_item_t *rli, *rli_prev, *p_rli;
  int more_in_queue, num_to_send;
  int num_recvd_env, num_sent_env, num_recv_free_slots;
  int j;

  lam_debug_cond((lam_ssi_rpi_ib_did, "send_advance: advancing pending acks"));

  for (rli = lam_ssi_rpi_ib_ack_queue.top; rli != NULL; ) {
    rli_prev = rli;
    req = rli->req;
    p = req->rq_proc->p_rpi;

    /* Go to the proc queue and see how many sends can we do */

    /* We only check this once and do as much as we can right now. 
       It may be possible by the time this completes, more slots may
       be available, but right now we dont go back and check */

    num_recvd_env = *(p->fc_remote_recv_env_cnt);
    num_sent_env = p->fc_local_sent_env_cnt;
    
    num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
    
    if (num_recv_free_slots > 0) {
      /* We have slots, so can send  */
      more_in_queue = (p->cp_pending_acks.size > num_recv_free_slots);
      num_to_send = (more_in_queue ? num_recv_free_slots 
		     : p->cp_pending_acks.size);
      p_rli = p->cp_pending_acks.top;
      for (j = 0; j < num_to_send; ++j) {
	/* Send the request and remove it from the proc queue  */
	
	/* VPS ** You should use send_advance here, see if its required
	   or (say for long or any other thing) */
	if (lam_ssi_rpi_ib_push_envelope(p_rli->req, 0) != 0)
	  return LAMERROR;
	p_rli = p_rli->next;
	lam_ssi_rpi_ib_reqlist_pop_top(&(p->cp_pending_acks));
      }
      
      /* VPS: Check to see if the bitmaps are bein reset elsewhere or not */
      
      /* Proceed the ptr before we delete something from the queue  */
      rli = rli->next;
      
      if (!more_in_queue)
	lam_ssi_rpi_ib_reqlist_pop_top(&lam_ssi_rpi_ib_ack_queue);
      
      /* Else we dont remove entry from the common queue, since we have
	 more left to be sent and no more tokens avaialable for the proc */
    } else {
      rli = rli->next;
    }
  }
  return 0;
}

/*
 * back end advancing of pending send queues
 */
static int
advance_sends(void)
{
  MPI_Request req;
  struct lam_ssi_rpi_proc *p;
  struct lam_ssi_rpi_ib_reqlist_item_t *rli, *rli_prev, *p_rli;
  int more_in_queue, num_to_send;
  int num_recvd_env, num_sent_env, num_recv_free_slots;
  int j;

  lam_debug_cond((PUB(did), "send_advance: advancing pending sends"));

  for (rli = lam_ssi_rpi_ib_send_queue.top; rli != NULL; ) {
    rli_prev = rli;
    req = rli->req;
    p = req->rq_proc->p_rpi;

    /* Go to the proc queue and see how many sends can we do */

    /* We only check this once and do as much as we can right now. 
       It may be possible by the time this completes, more slots may
       be available, but right now we dont go back and check */

    num_recvd_env = *(p->fc_remote_recv_env_cnt);
    num_sent_env = p->fc_local_sent_env_cnt;
    
    num_recv_free_slots = PUB(num_envelopes) - (num_sent_env - num_recvd_env);
    
    if (num_recv_free_slots > 0) {
      /* We have slots, so can send  */
      more_in_queue = (p->cp_pending_sends.size > num_recv_free_slots);
      num_to_send = (more_in_queue ? num_recv_free_slots 
		     : p->cp_pending_sends.size);
      p_rli = p->cp_pending_sends.top;
      for (j = 0; j < num_to_send; ++j) {
	/* Send the request and remove it from the proc queue  */

	/* VPS ** You should use send_advance here, see if its required
	   or (say for long or any other thing) */
	if (lam_ssi_rpi_ib_push_envelope(p_rli->req, 0) != 0)
	  return LAMERROR;
	p_rli = p_rli->next;
	lam_ssi_rpi_ib_reqlist_pop_top(&(p->cp_pending_sends));
    }
      /* VPS: Check to see if the bitmaps are bein reset elsewhere or
	 not Also convert all the common queue entries to procs
	 instead of req. If you wanna write a proclist.h similar to
	 reqlist.h, go ahead -- there may be something int he main MPI
	 layer for the proc list -- check that */
    
    /* Proceed the ptr before we delete something from the queue  */
    rli = rli->next;

    if (!more_in_queue)
      lam_ssi_rpi_ib_reqlist_pop_top(&lam_ssi_rpi_ib_send_queue);

    /* Else we dont remove entry from the common queue, since we have
       more left to be sent and no more tokens avaialable for the proc */
    } else {
      rli = rli->next;
    }
  }
  return 0;
}
