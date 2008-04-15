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
 * $Id: ssi_rpi_ib.c,v 1.8 2003/12/29 22:00:41 vsahay Exp $
 *
 *	Function:	- IB client-to-client interface
 */

#include <lam_config.h>
#include <lam-ssi-rpi-ib-config.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <dl_inet.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <all_array.h>
#include <all_list.h>
#include <malloc_wrapper.h>

#include <rpi_ib.h>
#include <rpi_ib_actions.h>
#include <rpi_ib_interval.h>
#include <rpi_ib_recv_queue.h>
#include <rpi_ib_recv_events.h>
#include <rpi_ib_send_queue.h>
#include <rpi_ib_tiny.h>
#include <rpi_ib_long.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_unexpected.h>
#include <rpi_ib_util.h>
#include <rpi_ib_cbuf.h>


/* 
 * Private fucntions
 */

static int send_to_self(MPI_Request req_top, MPI_Request send);
static int send_to_self_match(MPI_Request send, MPI_Request recv);
static int connect_all(int happy);
static void fill_sync(struct _proc *src, struct _proc *dest, 
		      struct nmsg  *head);
static int finalize1(struct _proc *p);
static int mem_list_comp(void *a, void *b);


/* 
 * Global variables
 */

LAM_IB_ret ret;
int lam_ssi_rpi_ib_mtu;
LAM_IB_hca_id lam_ssi_rpi_ib_hca_id; /* a string internally malloced */
LAM_IB_hca_hndl lam_ssi_rpi_ib_hca_hndl = LAM_IB_INVAL_HNDL;
LAM_IB_pd_hndl lam_ssi_rpi_ib_pd_hndl = LAM_IB_INVAL_HNDL;
LAM_IB_port lam_ssi_rpi_ib_port_num;
LAM_IB_lid lam_ssi_rpi_ib_lid;
LAM_IB_qp_attr_mask lam_ssi_rpi_ib_qp_attr_mask;
LAM_IB_qp_attr lam_ssi_rpi_ib_qp_attr;

int lam_ssi_rpi_ib_haveadv = 0;
int lam_ssi_rpi_ib_flblock = 0;
int lam_ssi_rpi_ib_num_envelopes;
int lam_ssi_rpi_ib_reg_env_len;
int *PUB(fc_remote_recv_env_cnt);
PUB(fc_addr_key) *PUB(fc_remote_info);
int *PUB(fc_post_env_cnt);
lam_ssi_rpi_ib_bitmap *PUB(pending_send_proc_bitmap);
lam_ssi_rpi_ib_bitmap *PUB(pending_ack_proc_bitmap);
lam_array_t *PUB(procs) = NULL;
int PUB(param_priority);
int PUB(req_mid) = -1;

/* This is not datatyped as LAM_IB_port since that is supposedly unsigned int 
   and we need -1 to declare that we need to search port at runtime */
int lam_ssi_rpi_ib_port;

int lam_ssi_rpi_ib_did = -1;
int lam_ssi_rpi_ib_tinymsglen = 0;

static LIST *mpi_alloced_mem = NULL;
typedef struct allocmem {
  void *am_base;
  long am_length;
  int am_registered;
  ib_mem_keypair am_kp;
} allocmem_t;


/*
 * private varaibles
 */
static lam_debug_stream_info_t lds = {
  1, 0, 0, NULL, NULL,
  /* flag stdout */ 0,
  /* flag stderr */ 0,
  /* flag file   */ 1,
  0, NULL
};

static const lam_ssi_rpi_actions_1_1_0_t module_actions = {
  /* RPI API functions */

  lam_ssi_rpi_ib_addprocs,
  lam_ssi_rpi_ib_finalize,

  lam_ssi_rpi_ib_build,
  lam_ssi_rpi_ib_start,
  lam_ssi_rpi_ib_advance,
  lam_ssi_rpi_ib_destroy,

  lam_ssi_rpi_ib_iprobe,

  NULL,
  NULL,

  lam_ssi_rpi_ib_alloc_mem,
  lam_ssi_rpi_ib_free_mem,

  NULL,
  NULL,
  NULL,
  NULL,

  /* Flags */
  /* Totalview support */

  1
};


/*
 * lam_ssi_rpi_ib_query
 *
 * Function: determine if this module wants to run
 */
int
lam_ssi_rpi_ib_query(int *priority, int *thread_min, int *thread_max)
{
  LAM_IB_ret ret;
  int param_hca_id;
  struct _proc *p;
  LAM_IB_hca_id hca_id_buf[LAM_SSI_RPI_IB_MAX_HCA];
  int num_hca_got;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(PUB(param_priority));
  if (*priority < 0)
    return 0;

  /* See if all the procs are the same endian.  This RPI currently can
     only do same-endian transfers. */

  for (p = lam_topproc(); p != NULL; p = lam_nextproc()) {
    if ((p->p_mode & LAM_PHOMOG) == 0) {
      if (lam_ssi_rpi_verbose >= 5)
        lam_debug(lam_ssi_rpi_did, "ib: can't run in heterogeneous jobs");
      return 0;
    }
  }

  /* Register a NULL hca id parameter */

  param_hca_id =
    lam_ssi_base_param_register_string("rpi", "ib", "hca_id", NULL, "X");

  /* Look if user provided some hca id to use */

  strcpy(lam_ssi_rpi_ib_hca_id,lam_ssi_base_param_lookup_string(param_hca_id));

  if (strcmp(lam_ssi_rpi_ib_hca_id, "X") == 0) {
    
    /* See if we have ib hardware -- get a listing of the HCAs */

    ret = LAM_IB_list_hcas(LAM_SSI_RPI_IB_MAX_HCA, &num_hca_got, hca_id_buf);
    if (ret != LAM_IB_OK || num_hca_got == 0)
      return 0;

    /* We have got the list of HCAs. For now take the first one to be our
     * HCA. At sometime got to add an SSI parameter to allow user to specify
     * which HCA he wants in case of multiple HCAs */

    strcpy(lam_ssi_rpi_ib_hca_id, hca_id_buf[0]);
  } 

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "ib: module initializing");
    lam_debug(lam_ssi_rpi_did, "ib:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "ib:priority: %d", *priority);
  }

  /* Setup a request memory pool */

  PUB(req_mid) = lam_mp_init(sizeof(struct lam_ssi_rpi_req),
                             NULL, NULL, 10, 10, 0);

  /* Return 1 saying that we want to be considered */

  return 1;
}


/*
 *	lam_ssi_rpi_ib_init
 *
 *	Function:	- primary initialiation of RPI subsystem
 *			- initialize buffering and socket connections
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_rpi_actions_t *
lam_ssi_rpi_ib_init(struct _proc **procs, int nprocs, 
		    int *max_tag, int *max_cid)
{
#if 1
  /* Setup the debug file */

  char filename[128];
  char prefix[128];
  snprintf(filename, 128, "rpi-ib-rank-%d.txt", lam_myproc->p_gps.gps_grank);
  snprintf(prefix, 128, "ib rank %d:" , lam_myproc->p_gps.gps_grank);
  lds.lds_prefix = prefix;
  lds.lds_file_suffix = filename;
#if 0
  PUB(did) = lam_debug_open(&lds);
  lam_debug_cond((PUB(did), "pid %d starting", lam_getpid()));
#else
  PUB(did) = -1;
#endif
#endif

  /* Open and get the HCA handle  */

  ret = LAM_IB_open_hca(lam_ssi_rpi_ib_hca_id, NULL, NULL);

  ret = LAM_IB_get_hca_hndl(lam_ssi_rpi_ib_hca_id, 
			    &lam_ssi_rpi_ib_hca_hndl);

  if(ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "hca-hndl-fail");
    return NULL;
  }
  
  /* Check which port is active. Take the first active port. If none is active
     then can't run IB programs -- simply exit */

  if (lam_ssi_rpi_ib_get_port() == LAMERROR)
    return NULL;
  
  lam_debug_cond((PUB(did), "port num %d ",lam_ssi_rpi_ib_port_num));

  /* Get protection domain */

  ret = LAM_IB_alloc_pd(lam_ssi_rpi_ib_hca_hndl, &lam_ssi_rpi_ib_pd_hndl);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "invalid-pd");
    return NULL;
  }

  /* Register the memory handler */

  lam_register_free_handler(lam_ssi_rpi_ib_interval_unregister);

  /* Setup my _proc */

  lam_ssi_rpi_ib_proc_init(lam_myproc);

  /* Initalize IB stuff.  If we fail, we still have to call
     connect_all() so that we can let the other nodes know of our
     failure. */

  if (lam_ssi_rpi_ib_ib_setup() == LAMERROR) {
    connect_all(FALSE);
    return NULL;
  }

  /* Initialize the lower layers of the ib RPI */

  /* Set up all processes for client-to-client communication. 
     This does a connect_all to get the queues up and running */

  if (lam_ssi_rpi_ib_addprocs(procs, nprocs) == LAMERROR) 
    return NULL;

  lam_debug_cond((PUB(did), " Done with addprocs"));

  if (lam_ssi_rpi_ib_send_queue_init() != 0)
    return NULL;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {

    lam_debug(PUB(did), "ib:max_tiny_size: %d", 
              lam_ssi_rpi_ib_tinymsglen);
  }

  return &module_actions;
}


/*
 *	lam_ssi_rpi_ib_addprocs
 *
 *	Function:	- setup for new processes
 *			- makes socket connections
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_addprocs(struct _proc **procs, int nprocs)
{
  int i, added = 0;
  int appending = 1;
  int num_appended = 0;

  if (PUB(procs) == 0) {
    /* The whole set of procs is new. So we are not adding to the
       already created proc list */
    appending = 0;
    PUB(procs) = lam_arr_init(sizeof(struct _proc *), NULL);
  }

  /* Add these procs into local procs array.  Make sure that they're
     not already in there first. */

  lam_debug_cond((PUB(did), "In ADDPROCS "));

  for (i = 0; i < nprocs; ++i) {
    if (lam_arr_find(PUB(procs), &procs[i]) == LAMERROR) {
      procs[i]->p_mode |= LAM_PRPIADDED;
      lam_arr_append(PUB(procs), &procs[i]);
      ++num_appended;
      added = 1;
    }
  }

  /* Create CQs/QPs/pool buffers/bitmap and other IB stuff for newly added
     procs */
  if (num_appended)
    if (lam_ssi_rpi_ib_initialize_ib(appending, num_appended) == LAMERROR)
      return LAMERROR;

  if (added)
    lam_procsort(lam_arr_get(PUB(procs)), lam_arr_size(PUB(procs)));

  return connect_all(TRUE);
}


/*
 *	lam_ssi_rpi_ib_build
 *
 *	Function:	- builds RPI portion of a request from scratch
 *			- one-time cost separated from lam_ssi_rpi_ib_start()
 *			  to optimize persistent requests
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_build(MPI_Request req)
{
  req->rq_rpi = lam_mp_malloc(PUB(req_mid));
  if (req->rq_rpi == NULL)
    return LAMERROR;

  /* Because of @#%@#$% MPI_Bsend (more specifically, MPI_Ibsend), we
     may actually never invoke lam_ssi_rpi_ib_start(), so ensure that
     we can call destroy cleanly if so. */

  req->rq_rpi->cq_envbuf = NULL;

  return 0;
}


/*
 *	lam_ssi_rpi_ib_start
 *
 *	Function:	- initializes RPI dependent aspects of a request
 *			- cost per request start - separated from
 *			  lam_ssi_rpi_ib_build() to optimize persistent 
 *                        requests
 *	Accepts:	- request list
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_cbuf_msg *bmsg;
  struct lam_ssi_rpi_req *rpi = req->rq_rpi;
  ib_mem_keypair kp;

  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "rpilam_ssi_rpi_start: starting req %p\n", req));
  if (lam_ssi_rpi_comm_dead_m(req)) {
    lam_ssi_rpi_ib_haveadv = 1;
    return 0;
  }

  /* Set common synchronization and communication parameters. The peer
     in a receive request may be a wildcard but will be set to the
     actual peer upon matching with an incoming mesage.  */

  rpi->cq_peer = req->rq_rank;
  rpi->is_advancing = 0;
  rpi->send_advance_fn = NULL;
  rpi->recv_advance_fn = NULL;

  /* VPS: Allocate new buffer now if no buffer left on pool. Someday
   will keep the req in the queue and will require a event handler when a 
   buffer is freed */

  rpi->cq_envbuf = (struct lam_ssi_rpi_ib_envl *)
    lam_ssi_rpi_ib_reg_env_malloc(&kp); 
  
  if (rpi->cq_envbuf == NULL) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "no-pool-buffer");
    return LAMERROR;
  }
  
  lam_debug_cond((PUB(did), "POOL: Successfully allocated buffer "));
  
  rpi->env_lkey = kp.lkey;
  rpi->env_src_rkey = kp.rkey;

  rpi->cq_envbuf->ge_env.ce_flags = 0;
  rpi->cq_envbuf->ge_env.ce_tag = req->rq_tag;
  rpi->cq_envbuf->ge_env.ce_cid = req->rq_cid;
  rpi->cq_envbuf->ge_env.ce_seq = -1;
  rpi->cq_envbuf->ge_target = NULL;
  rpi->cq_envbuf->ge_src_request = req;
  rpi->cq_envbuf->ge_dest_request = NULL;

  /* receive specific initialization */

  if ((req->rq_type == LAM_RQIRECV) || (req->rq_type == LAM_RQIPROBE)) {
    rpi->cq_state = C2CREAD;
    rpi->cq_envbuf->ge_env.ce_rank = rpi->cq_peer;

    /* If the message is long, pin the user buffer */
    if (req->rq_packsize > lam_ssi_rpi_ib_tinymsglen) {

      /* Pin the user long buffer  */
      if (lam_ssi_rpi_ib_interval_use(req->rq_packbuf, req->rq_packsize,
				      &kp) != 0) {
	show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", 
		       "register-mem-fail");
	return LAMERROR;
      }
      rpi->long_lkey = kp.lkey;
      rpi->long_src_rkey = kp.rkey;
    }

    /* Check for matching buffered envelope/message. If one is found
       fill in any receive request wildcards and advance the request.  */

    if ((bmsg = 
         lam_ssi_rpi_cbuf_find(&(rpi->cq_envbuf->ge_env))) != NULL) {
      lam_ssi_rpi_ib_fill_wildcards(req, &bmsg->cm_env);
      return lam_ssi_rpi_ib_unexpected_advance(req, bmsg);
    }

    /* No match was found. This request will be placed on the pending
       recv queue(s) of the appropriate process(es). */

    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "rpilam_ssi_rpi_start: didn't find a buffered "
                    "message to receive\n"));

  }

  /* send specific initialization */

  else {
    rpi->cq_envbuf->ge_env.ce_rank = req->rq_comm->c_group->g_myrank;
    rpi->cq_envbuf->ge_env.ce_len = req->rq_packsize;

    if (req->rq_proc == lam_myproc) {

      /* send to self */

      return(send_to_self(req_top, req));


    } else {

      /* send to another process */

      rpi->cq_state = C2CWRITE;

      /* long message protocol */
      /* need to pin or setup DMA buffer */

      if (req->rq_packsize > lam_ssi_rpi_ib_tinymsglen) {

	/* LONG  */
	rpi->cq_envbuf->ge_env.ce_flags |= C2CLONG;
	rpi->send_advance_fn = lam_ssi_rpi_ib_long_send;

	/* Pin the user long buffer  */
	if (lam_ssi_rpi_ib_interval_use(req->rq_packbuf, req->rq_packsize,
					&kp) != 0) {
	  show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", 
			 "register-mem-fail");
	  return LAMERROR;
	}
	rpi->long_lkey = kp.lkey;
	rpi->long_src_rkey = kp.rkey;
      }

      /* tiny message protocol */
      /* don't use pinning or DMA buffers */

      else {
	if (req->rq_type == LAM_RQISSEND) {
	  rpi->cq_envbuf->ge_env.ce_flags |= C2CSSEND;
	  rpi->send_advance_fn = lam_ssi_rpi_ib_tiny_send_sync;
	} else
	  rpi->send_advance_fn = lam_ssi_rpi_ib_tiny_send;
      }
    }
  }
  
  /* All done */

  return 0;
}


/*
 *	lam_ssi_rpi_ib_destroy
 *
 *	Function:	- destroys RPI portion of request
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_destroy(MPI_Request req)
{
  int need_remove = 0;

  /* Note that this request may still be on some pending receive
     lists.  For example, MPI_Win_create posts a persistent ANY_SOURCE
     receive that is destroyed upon MPI_Win_free.  MPI_Cancel also
     by-passes the normal progression.  Ignore any errors (e.g., the
     request is not actually in the list) when removing from the
     lists. */

  if ((req->rq_flags & LAM_RQFCANCEL) != 0)
    need_remove = 1;
  if (req->rq_state != LAM_RQSDONE && req->rq_state != LAM_RQSINIT)
    if (req->rq_type == LAM_RQIPROBE || req->rq_type == LAM_RQIRECV)
      need_remove = 1;

  if (need_remove == 1) {
    if (req->rq_rank == MPI_ANY_SOURCE)
      lam_ssi_rpi_ib_rm_read_any_src(req, NULL);
    else 
      lam_ssi_rpi_ib_rm_read_req(req->rq_proc->p_rpi, req);
  }

  /* Return the envelope to the pool.  Due to the strange bsend beast
     (e.g., the bsend_free lamtest), this request may actually never
     have called the rpi_ib_start() RPI API call.  Hence, cq_envbuf
     will be NULL. */
  /* (I don't think it can ever be C2CBUFFERED... but just in case!) */

  if (req->rq_rpi->cq_envbuf != NULL &&
      (req->rq_rpi->cq_envbuf->ge_env.ce_flags & C2CBUFFERED) == 0)
    lam_ssi_rpi_ib_reg_env_free(req->rq_rpi->cq_envbuf);
  req->rq_rpi->cq_envbuf = NULL;
  
  /* Free the extra state that this RPI caches on the request */

  lam_debug_cond((lam_ssi_rpi_ib_did, "Rank %d: destroying req %p rq_rpi %p\n", 
                  lam_myproc->p_gps.gps_grank, req, req->rq_rpi));
  if (req->rq_rpi != NULL)
    lam_mp_free(PUB(req_mid), req->rq_rpi);

  lam_debug_cond((lam_ssi_rpi_ib_did, "Rank %d: destroyed req %p\n",
		  lam_myproc->p_gps.gps_grank, req));

  req->rq_rpi = NULL;

  return 0;
}


/*
 *	lam_ssi_rpi_ib_advance
 *
 *	Function:	- advances requests
 *			- we try to advance all requests as far as possible
 *			  as allowed by RPI
 *	Accepts:	- request list
 *			- block enable flag
 *	Returns:	- 1: state change, 0: no state change, LAMERROR: error
 */
int
lam_ssi_rpi_ib_advance(MPI_Request req_top, int fl_block)
{
  MPI_Request req;

  lam_ssi_rpi_ib_flblock = fl_block;
  lam_ssi_rpi_ib_haveadv = 0;

  /* Find which requests require IO.  */

  lam_debug_cond((lam_ssi_rpi_ib_did, "advance: starting req loop"));

  for (req = req_top; req; req = req->rq_next) {
  
    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "advance: looping through: req %p", req));

    /* Start requests that are in the init state.  */

    if (req->rq_state == LAM_RQSINIT)
      if (_mpi_req_start(req) != MPI_SUCCESS)
	return(LAMERROR);

    /* If a blocking request is done, then we may no longer block.  */

    if (req->rq_state == LAM_RQSDONE) {
      lam_debug_cond((lam_ssi_rpi_ib_did, "advance: req %p is done!", req));

      if (req->rq_flags & LAM_RQFBLOCK)
	lam_ssi_rpi_ib_flblock = 0;
      continue;
    }

    /* If we've already starting advancing this request (since we
       already established above that it's not done), skip it */

    if (req->rq_rpi->is_advancing) {
      lam_debug_cond((lam_ssi_rpi_ib_did,
                      "advance: req %p is already advancing", req));
      continue;
    }

    /* If it's a write, call the advance function, which will try to
       actually send (depending if we can get a token, etc.) */

    else if (req->rq_rpi->cq_state == C2CWRITE) {
      lam_debug_cond((lam_ssi_rpi_ib_did, 
                      "advance: calling write advance on req %p - %s", 
                      req, MPIL_Request_get_name(req)));
      /* XXX: SS: no error-checking done here */
      req->rq_rpi->send_advance_fn(req);
    } 

    /* Otherwise, if it's a read, add it to the list of requests that
       are eligible to be read */

    else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0) {
	lam_debug_cond((lam_ssi_rpi_ib_did, 
                        "advance: adding read_anysource - %p - %s",
                        req, MPIL_Request_get_name(req)));
	lam_ssi_rpi_ib_add_read_any_src(req);
      } else {
	lam_debug_cond((lam_ssi_rpi_ib_did, 
                        "advance: adding read - %p - %s",
                        req, MPIL_Request_get_name(req)));
	lam_ssi_rpi_ib_add_read(req->rq_proc->p_rpi, req);
      }
    }
  }
  lam_debug_cond((lam_ssi_rpi_ib_did, "advance: req loop done"));

  /* Check for and process any pending ib events */

  do {
    if (lam_ssi_rpi_ib_ib_advance() != 0) {
      lam_debug_cond((lam_ssi_rpi_ib_did, 
                      "advance: returns LAMERROR after ib_advance"));
      return(LAMERROR);
    }
  } while (lam_ssi_rpi_ib_flblock && !lam_ssi_rpi_ib_haveadv);

  lam_debug_cond((lam_ssi_rpi_ib_did, 
                  "advance: completed: lam_ssi_rpi_ib_haveadv %d", 
                  lam_ssi_rpi_ib_haveadv));
		
  return(lam_ssi_rpi_ib_haveadv);
}


/*
 *      lam_ssi_rpi_ib_iprobe
 *
 *      Function:       - non-blocking probe
 *                      - public interface for peculiar MPI_Iprobe() which
 *                        does not return a request to the user
 *      Accepts:        - request
 * Returns: - 0: no msg, 1: msg, LAMERROR: error 
 */
int
lam_ssi_rpi_ib_iprobe(MPI_Request req)
{
  int err;

  /* Link the probe request and advance as far as possible.  */

  _mpi_req_add(req);
  _mpi_req_blkclr();
  err = _mpi_req_advance();
  if (err != MPI_SUCCESS) {
    return(LAMERROR);
  }

  /* Unlink the request. */

  if (req->rq_state != LAM_RQSDONE) {
    if (req->rq_rank == MPI_ANY_SOURCE)
      lam_ssi_rpi_ib_rm_read_any_src(req, NULL);
    else
      lam_ssi_rpi_ib_rm_read_req(req->rq_proc->p_rpi, req);
  }
  _mpi_req_rem(req);

  /* A message was found if the request is in the done state.  */

  return((req->rq_state == LAM_RQSDONE) ? 1 : 0);
}


/*
 *	lam_ssi_rpi_ib_module_open
 *
 *	Function:	- open the ib RPI module
 *	Accepts:	- OPT pointer
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_module_open(OPT *ad)
{
  lam_ssi_rpi_ib_params_register();

  return 1;
}


/*
 *	lam_ssi_rpi_ib_module_close
 *
 *	Function:	- close the ib RPI module
 *	Accepts:	- void
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_module_close(void)
{
  /* This function is called when the module is closed.  It is always
     the last function that is called in this module for the duration
     of the process.

     If this module was *used* during the run, then this function is
     redundant. 

     Nothing for IB
  */
  return 0;
}


/*
 *	lam_ssi_rpi_ib_finalize
 *
 *	Function:	- final cleanup
 *	Accepts:	- process to cleanup (0 => all processes)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_finalize(struct _proc *p)
{

  if (p) {
    lam_debug_cond((PUB(did), "FINALIZE:--proc %x getting finalized", p));
    return(finalize1(p));
  }
  else {

    /* When p == 0, this is the "final" finalize -- the RPI won't be
       used any more after this. */

    /* Do a barrier, 'cause we have to wait for everything to finish
       finish finish -- not just finish on this side.  This is unlike
       TCP where a sender can close their end of the socket and still
       have messages that are in flight still make it to the
       destination.  In ib, if you send to a port that is closed,
       you'll get an error. */

      lam_debug_cond((PUB(did), "FINALIZE: doing a barrier "));
      lam_nbarrier(lam_arr_get(PUB(procs)), lam_arr_size(PUB(procs)));

      /* Unregister the memory handler - this should be before
	 interval finalize */
      lam_register_free_handler(NULL);

    /* clean up pinned memory pool */
      lam_debug_cond((PUB(did), "FINALIZE: before interval fin "));
    lam_ssi_rpi_ib_interval_finalize();
    lam_debug_cond((PUB(did), "FINALIZE: interval destroyed "));

    /* Loop through all processes closing connections.  */

    while (lam_arr_size(PUB(procs)) > 0)
      if (finalize1(((struct _proc **) lam_arr_get(PUB(procs)))[0]))
	return (LAMERROR);
    lam_arr_free(PUB(procs));

    lam_ssi_rpi_ib_bitmap_destroy(PUB(pending_send_proc_bitmap));
    lam_ssi_rpi_ib_bitmap_destroy(PUB(pending_ack_proc_bitmap));

    lam_ssi_rpi_ib_reg_destroy();
    lam_debug_cond((PUB(did), "FINALIZE: reg destroyed "));

    lam_ssi_rpi_ib_ib_finalize();

    /* Free our aux list of alloced memory */
    if (mpi_alloced_mem != NULL)
      al_free(mpi_alloced_mem);

    /* Send last debugging message */

    lam_debug_cond((lam_ssi_rpi_ib_did, "Last debugging message..."));
    lam_debug_close(lam_ssi_rpi_ib_did);

  }

  return 0;
}


/************************************************************************/

/*
 *	send_to_self
 *
 *	Function:	- advance send to self
 *	Accepts:	- request list
 *			- send request
 */
static int
send_to_self(MPI_Request req_top, MPI_Request send)
{
  MPI_Request     	recv;			/* receive request */
  struct lam_ssi_rpi_cbuf_msg 	msg;		/* buffer list entry */

  /* Look for inactive matching receive/probe and advance if found.  */

  for (recv = req_top; recv; recv = recv->rq_next)
    if ((recv->rq_state == LAM_RQSSTART) &&
	(recv->rq_rpi->cq_state == C2CREAD) &&
	(!lam_ssi_rpi_envl_cmp(&(send->rq_rpi->cq_envbuf->ge_env), 
                               &(recv->rq_rpi->cq_envbuf->ge_env)))) {
      lam_debug_cond((lam_ssi_rpi_ib_did, 
                      "send_to_self: found matching recv posted, "
                      "send req %p, recv req %p", send, recv));
      if (send_to_self_match(send, recv)) {
	lam_debug_cond((lam_ssi_rpi_ib_did, 
                        "send_to_self: successful send to self, "
                        "send req %p, recv req %p", send, recv));
	return(0);
      }
    }

  /* No matching receive found, buffer the whole message and the send
     is done unless its a synchronous send in which case we use the
     user buffer and the send only completes once a matching receive
     is posted.  */

  msg.cm_extra = malloc(sizeof(struct lam_ssi_rpi_cbuf));
  if (msg.cm_extra == NULL)
    return LAMERROR;

  msg.cm_extra->lsrc_src_request = send;
  msg.cm_env = send->rq_rpi->cq_envbuf->ge_env;
  msg.cm_proc = 0;
  msg.cm_dont_delete = 1;

  if (send->rq_type == LAM_RQISSEND) {
    send->rq_rpi->cq_state = C2CSENDSELF;
    msg.cm_buf = send->rq_packbuf;
    msg.cm_req = send;
    msg.cm_extra->was_sent_to_self_non_sync = 0;
  } else {
    if (send->rq_packsize > 0) {
      if ((msg.cm_buf = (char *) malloc(send->rq_packsize)) == 0)
	return(LAMERROR);
      lam_memcpy(msg.cm_buf, send->rq_packbuf, send->rq_packsize);
    } else
      msg.cm_buf = 0;
    msg.cm_extra->was_sent_to_self_non_sync = 1;
    msg.cm_req = MPI_REQUEST_NULL;
    send->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return(lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
}


/*
 *	send_to_self_match
 *
 *	Function:	- advance send and matching receive/probe
 *	Accepts:	- send request
 *			- receive/probe request
 *	Returns:	- 1: matched a receive, 0: matched a probe
 */
static int
send_to_self_match(MPI_Request send, MPI_Request recv)
{
  int len, ret;

  lam_debug_cond((PUB(did), "In send to self !!!"));
  recv->rq_seq = send->rq_seq;
  if (recv->rq_type == LAM_RQIPROBE) {

    /* The receive is actually a probe so the send is not complete.  */

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_envbuf->ge_env.ce_rank,
				send->rq_rpi->cq_envbuf->ge_env.ce_tag, 
				send->rq_rpi->cq_envbuf->ge_env.ce_len);
    
    recv->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
    return(0);
  } else {

    /* It's really a receive. Do the data transfer.  Check for
       mismatched message lengths.  */

    if (send->rq_packsize > recv->rq_packsize) {
      recv->rq_flags |= LAM_RQFTRUNC;
      len = recv->rq_packsize;
    } else
      len = send->rq_packsize;

    lam_memcpy(recv->rq_packbuf, send->rq_packbuf, len);

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_envbuf->ge_env.ce_rank,
				send->rq_rpi->cq_envbuf->ge_env.ce_tag, len);

    /* Need to remove these pending receives from the receive queues.
       This is somewhat inefficient, because we're scanning down the
       proc pending recv queue essentially a second time. */

    lam_debug_cond((lam_ssi_rpi_ib_did, 
                    "send_to_self_match: removing read req %p", 
                    recv));
    if (recv->rq_rank == MPI_ANY_SOURCE)
      ret = lam_ssi_rpi_ib_rm_read_any_src(recv, NULL);
    else
      ret = lam_ssi_rpi_ib_rm_read_req(recv->rq_proc->p_rpi, recv);
    if (ret == LAMERROR)
      return 0;

    send->rq_state = recv->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
    return(1);
  }
}


/*
 *	connect_all
 *
 *	Function:	- make ib connections to all other processes
 *	Returns:	- 0 or LAMERROR
 */
static int
connect_all(int happy)
{
  struct _proc *p;
  struct _proc **array;
  struct _gps *mygps;
  struct nmsg inmsg;
  struct nmsg outmsg;
  struct dolink *links = 0;
  int4 nlinks;
  int4 rlid;
  int node;
  int4 rqp_num;
  int i;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);

  /* A failure may have occurred during startup, during which we
     haven't even established the procs yet.  If this happens, just
     return without doing anything.  connect_all() is called even in
     failure cases so that startup protocols can be completed
     properly, and errors can be propogated.  But in this case,
     there's no one to propogate the error to, so just do nothing. */

  if (PUB(procs) == NULL)
    return LAMERROR;

  array = lam_arr_get(PUB(procs));

  /* Things to make -Wall not complain */

  mygps = &lam_myproc->p_gps;

  if (lam_arr_size(PUB(procs)) > 1) {

    
    /* get links to neighbors */

    if (ldogetlinks(&links, &nlinks)) 
      return(LAMERROR);

    /* init message headers */

    inmsg.nh_flags = 0;
    inmsg.nh_length = 0;
    outmsg.nh_length = 0;
    outmsg.nh_flags = 0;
    if (happy) {
      outmsg.nh_data[0] = (int4) lam_myproc->p_rpi->cp_ib_lid;
    } else {
      outmsg.nh_data[0] = (int4) -1;
      outmsg.nh_data[1] = (int4) -1;
    }
  }

  /* Loop through all processes, initializing the process data and
     send connection data to everyone.  */

  for (i = 0; i < lam_arr_size(PUB(procs)); ++i) {
    p = array[i];
      
    /* Check and see if this proc is already initialized and connected */
    if ((p->p_mode & LAM_PRPIINIT) && (p->p_mode & LAM_PRPICONNECT))
      continue;

    /* initialize process */
    
    if (p != lam_myproc) {

      /* Fill in the qp_num in second outmsg data  */
      outmsg.nh_data[1] = (int4) p->p_rpi->cp_ib_my_qp_num;

      /* Send the Data */
      
      fill_sync(lam_myproc, p, &outmsg);
      if (nsend(&outmsg)) 
	return(LAMERROR);
      
      /* Recieve data */
      
      fill_sync(p, lam_myproc, &inmsg);
      if (nrecv(&inmsg)) 
	return(LAMERROR);
      
      rlid = (int) inmsg.nh_data[0];
      rqp_num = (int) inmsg.nh_data[1];
       
      /* store data in RPI private area */

      node = p->p_gps.gps_node;
      p->p_rpi->cp_ib_lid = rlid;
      p->p_rpi->cp_ib_dest_qp_num = rqp_num;

      if (rlid == -1 && rqp_num == -1)
	happy = FALSE;
       
     lam_debug_cond((PUB(did), "connect_all: recv" 
		     "(lid %d , qp_num %d from rank %d\n", 
		     rlid, rqp_num, p->p_gps.gps_grank));

      if (node > nlinks) 
	return(LAMERROR);

      /* Now synchronize the work queues of the peers */
      if (lam_ssi_rpi_ib_connect_peer(p) == LAMERROR)
	return LAMERROR;
    }
    p->p_mode |= LAM_PRPICONNECT;
  }

  /* Did we (or someone else) fail?  If so, clean up any memory, and
     then return a failure */

  if (happy == FALSE) {
    if (links != 0)
      free(links);
    return LAMERROR;
  }

  /* Cleanup */

  if (lam_arr_size(PUB(procs)) > 1)
    if (links != 0)
      free(links);
  
  lam_debug_cond((lam_ssi_rpi_ib_did, "connect_all: done"));
  return 0;
}


/*
 *	lam_ssi_rpi_ib_proc_init
 *
 *	Function:	- initialize ib-specific process data
 *	Accepts:	- process
 */
void
lam_ssi_rpi_ib_proc_init(struct _proc *p)
{
  p->p_mode |= LAM_PRPIINIT;

  /* Initialize the p_rpi pointer */

  p->p_rpi = malloc(sizeof(struct lam_ssi_rpi_proc));
  if (p->p_rpi == 0)
    return;

  p->p_rpi->cp_proc = p;
  p->p_rpi->cp_bmsg = NULL;

  if (p == lam_myproc)
    p->p_rpi->cp_ib_lid = lam_ssi_rpi_ib_lid;

  lam_ssi_rpi_ib_reqlist_init(&(p->p_rpi->cp_pending_recvs));

  p->p_rpi->cp_current_recv_req = NULL;
}


/*
 *	fill_sync
 *
 *	Function:	- fill in network message sync for connecting
 *	Accepts:	- source process
 *			- destination process
 *			- network message header (filled)
 */
static void
fill_sync(struct _proc *src, struct _proc *dest, struct nmsg *head)
{
  /* This uses in effect synchronization MPI_COMM_WORLD and tag 0.  */

  _m2l_fillsys(src->p_gps.gps_node, src->p_gps.gps_idx,
	       dest->p_gps.gps_node, dest->p_gps.gps_idx, 0, 
	       (src->p_gps.gps_node & 0xFFFF00) >> 8, head);
}


/*
 *	finalize1
 *
 *	Function:	- cleanup a process
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
static int
finalize1(struct _proc *p)
{
  struct lam_ssi_rpi_proc *rpi;
  int j;

  /* Remove this proc from the array of valid procs that this RPI is
     reponsible for */

  lam_arr_remove(PUB(procs), &p);

  /* Double check to ensure this proc still needs to be cleaned up */

  if (p->p_rpi != NULL) {

    if (p != lam_myproc) {
      rpi = p->p_rpi;
      lam_debug_cond((PUB(did), " FINALIZE: in finalize 1"));

      /* Free the preposted envs */
      for (j = 0; j < PUB(num_envelopes); ++j) {
	  if (rpi->preposted_envs[j].freed == 0) {
	      lam_debug_cond((PUB(did), "FIN: FREE: freeing mem %x index %d freed %d",
			      rpi->preposted_envs[j].env, j, 
			      rpi->preposted_envs[j].freed));
	      lam_ssi_rpi_ib_reg_env_free(rpi->preposted_envs[j].env);
	  }
      }

      /* Free the preposted env tracking data structures */
      free(rpi->preposted_envs);

      /* Destroy all the queues  */
      if (rpi->cp_ib_qp_hndl != LAM_IB_INVAL_HNDL)
	ret = LAM_IB_destroy_qp(lam_ssi_rpi_ib_hca_hndl, rpi->cp_ib_qp_hndl);

      lam_debug_cond((PUB(did), "FINALIZE: qp destroy ret = %d", ret));

      if (rpi->cp_ib_cq_hndl != LAM_IB_INVAL_HNDL)
	ret = LAM_IB_destroy_cq(lam_ssi_rpi_ib_hca_hndl, rpi->cp_ib_cq_hndl);
      lam_debug_cond((PUB(did), "FINALIZE: cq destroy ret = %d", ret));
      
      lam_debug_cond((PUB(did), "IB finalize: destroyed QPs and CQs"));
    }

    /* For debugging */
    LAM_ZERO_ME(*(p->p_rpi));
    free(p->p_rpi);
    p->p_rpi = NULL;
  }
  
  return 0;
}

/*
 *	lam_ssi_rpi_ib_alloc_mem
 *
 *	Function:	- default alloc_mem function; wrapper to malloc
 *	Accepts:	- size to allocate
 *                      - info arg (ignored here)
 *	Returns:	- ptr to allocated memory
 */
int 
lam_ssi_rpi_ib_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr)
{
  allocmem_t entry;
  entry.am_length = size;

  /* Ensure the list is initialized */

  if (mpi_alloced_mem == NULL)
    mpi_alloced_mem = al_init(sizeof(allocmem_t), mem_list_comp);
  
  /* If we support arbitrary pinning, do a standard malloc */
  
  
  *((void **) baseptr) = malloc(size);
  if (*((void **) baseptr) == NULL)
    return(lam_mkerr(MPI_ERR_NO_MEM, errno));
  entry.am_base = *((void **) baseptr);
  entry.am_registered = 1;

  /* Ensure that we got something back */

  if (entry.am_base == NULL)
    return(lam_mkerr(MPI_ERR_NO_MEM, ENOMEM));

  /* Now register the interval and mark it "LRU not ok" so that it
     won't get freed by LRU sweeps. */

  if (lam_ssi_rpi_ib_interval_use(*((void **) baseptr), 
                                  size, &entry.am_kp) == LAMERROR) {

    free(*((void **) baseptr));
  
    *((void **) baseptr) = NULL;
    return(lam_mkerr(MPI_ERR_NO_MEM, errno));
  }

  /* All done */

  al_insert(mpi_alloced_mem, &entry);
  return MPI_SUCCESS;
}


/*
 *	lam_ssi_rpi_ib_free_mem
 *
 *	Function:	- default free_mem function; wrapper to free
 *	Accepts:	- ptr to memory to free
 *                        do nothing if the memory was not alloced by 
 *                        lam_ssi_rpi_ib_alloc_mem()
 *	Returns:	- MPI_SUCCESS
 */
int 
lam_ssi_rpi_ib_free_mem(void *base)
{
  allocmem_t *entry, find;

  /* Unpin it.  List search is not efficient, but it's not important
     here. :-) */

  find.am_base = base;
  entry = al_find(mpi_alloced_mem, &find);
  if (entry != NULL) {

    /* Mark the interval as not being used (i.e., decrease the
       reference count) */

    lam_ssi_rpi_ib_interval_unuse(base, entry->am_length);

    /* If it was simply registered above, then just free it */

    if (entry->am_registered == 1)
      free(base);

    al_delete(mpi_alloced_mem, entry);
  }

  /* All done */

  return MPI_SUCCESS;
}

static int 
mem_list_comp(void *a, void *b)
{
  allocmem_t *aa = (allocmem_t *) a;
  allocmem_t *bb = (allocmem_t *) b;

  return (((long) aa->am_base) - ((long) bb->am_base));
}

