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
 * $Id: ssi_rpi_gm.c,v 1.72 2004/02/14 16:01:58 jsquyres Exp $
 *
 *	Function:	- GM client-to-client interface
 */

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>

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

#include <rpi_gm.h>
#include <rpi_gm_actions.h>
#include <rpi_gm_interval.h>
#include <rpi_gm_recv_queue.h>
#include <rpi_gm_recv_events.h>
#include <rpi_gm_send_queue.h>
#include <rpi_gm_tiny.h>
#include <rpi_gm_long.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_unexpected.h>
#include <rpi_gm_util.h>
#include <rpi_gm_cbuf.h>


/*
 * private functions
 */
static int send_to_self(MPI_Request req_top, MPI_Request send);
static int send_to_self_match(MPI_Request send, MPI_Request recv);
static int connect_all(int happy);
static void fill_sync(struct _proc *src, struct _proc *dest, 
		      struct nmsg  *head);
static void proc_init(struct _proc *p);
static int finalize1(struct _proc *p);
static int mem_list_comp(void *a, void *b);


/*
 * global variables
 */
int lam_ssi_rpi_gm_did = -1;
int lam_ssi_rpi_gm_haveadv = 0;
int lam_ssi_rpi_gm_flblock = 0;
int lam_ssi_rpi_gm_stokens = 0;
int lam_ssi_rpi_gm_max_stokens = 0;
int lam_ssi_rpi_gm_rtokens = 0;
int lam_ssi_rpi_gm_max_rtokens = 0;
int lam_ssi_rpi_gm_enqueue_sends = 0;
int lam_ssi_rpi_gm_max_port_num = LAM_SSI_RPI_GM_MAX_PORT_NUM;
int lam_ssi_rpi_gm_port_num = LAM_SSI_RPI_GM_PORT;
int lam_ssi_rpi_gm_dma_env_len;
int lam_ssi_rpi_gm_tinymsglen = 0;
int lam_ssi_rpi_gm_can_register_mem = 0;
unsigned int lam_ssi_rpi_gm_env_min_size_for_length = 0;
unsigned int lam_ssi_rpi_gm_data_min_size_for_length = 0;
lam_array_t *PUB(procs) = NULL;
int PUB(req_mid) = -1;
int PUB(param_priority) = -1;
int PUB(param_cr) = -1;
int PUB(param_fast) = -1;
int PUB(cr_available) = 0;
int PUB(using_fast) = 0;


/*
 * private varaibles
 */
#define WANT_DEBUG_STDOUT 0
#define WANT_DEBUG_STDERR 0
static lam_debug_stream_info_t lds = {
  1, 0, 0, NULL, NULL, WANT_DEBUG_STDOUT, WANT_DEBUG_STDERR, 1, 0, NULL
};
static lam_ssi_rpi_actions_1_1_0_t module_actions = {
  /* RPI API functions */

  lam_ssi_rpi_gm_addprocs,
  lam_ssi_rpi_gm_finalize,

  lam_ssi_rpi_gm_build,
  lam_ssi_rpi_gm_start,
  lam_ssi_rpi_gm_advance,
  lam_ssi_rpi_gm_destroy,

  lam_ssi_rpi_gm_iprobe,

  NULL,
  NULL,

  lam_ssi_rpi_gm_alloc_mem,
  lam_ssi_rpi_gm_free_mem,

#if LAM_SSI_RPI_GM_WANT_CHECKPOINT
  lam_ssi_rpi_gm_interrupt,
  lam_ssi_rpi_gm_checkpoint,
  lam_ssi_rpi_gm_continue,
  lam_ssi_rpi_gm_restart,
#else
  NULL,
  NULL,
  NULL,
  NULL,
#endif

  /* Flags */
  /* Totalview support */

  1
};

static lam_ssi_rpi_gm_port_t myport;
static LIST *mpi_alloced_mem = NULL;
typedef struct allocmem {
  void *am_base;
  long am_length;
  int am_registered;
} allocmem_t;


/*
 * lam_ssi_rpi_gm_query
 *
 * Function: determine if this module wants to run
 */
int
lam_ssi_rpi_gm_query(int *priority, int *thread_min, int *thread_max)
{
  struct _proc *p;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(PUB(param_priority));
  if (*priority < 0)
    return 0;

  /* See if all the procs are the same endian.  This RPI currently can
     only do same-endian transfers. */

  for (p = lam_topproc(); p != NULL; p = lam_nextproc()) {
    if ((p->p_mode & LAM_PHOMOG) == 0) {
      if (lam_ssi_rpi_verbose >= 5)
        lam_debug(lam_ssi_rpi_did, "gm: can't run in heterogeneous jobs");
      return 0;
    }
  }

  /* Ok, now see if we can open a gm port.  If we can't, there's no
     point in continuing.  According to the folks at help@myri.com,
     this is the only way to tell if we actually have Myrinet hardware
     in the machine.  Hence, we allocate a port now.  If we end up not
     using the gm RPI, we'll free it shortly (see the module_close
     function, below). */

  myport.rgp_gm_port = NULL;
  myport.rgp_global_node_id = myport.rgp_local_node_id = 0;
  myport.rgp_port_id = -1;

  if (gm_init() != GM_SUCCESS)
    return 0;

  if (lam_ssi_rpi_gm_get_port(lam_myproc, &myport) == LAMERROR) {
    gm_finalize();
    /* reset myport to its original values */
    myport.rgp_gm_port = NULL;
    myport.rgp_global_node_id = myport.rgp_local_node_id = 0;
    myport.rgp_port_id = -1;
    return 0;
  }

  /* Ok, we've got a GM port.  So we're eligible for selection. */

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we want CR support, then set the minimum thread level to be
     MPI_THREAD_SERIALIZED */

  PUB(cr_available) = lam_ssi_base_param_lookup_int(PUB(param_cr));
  if (PUB(cr_available) == 1)
    *thread_min = MPI_THREAD_SERIALIZED;
  PUB(using_fast) = lam_ssi_base_param_lookup_int(PUB(param_fast));

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "gm: module initializing");
    lam_debug(lam_ssi_rpi_did, "gm:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "gm:priority: %d", *priority);
    lam_debug(lam_ssi_rpi_did, "gm:fast: %d", PUB(using_fast));
  }

  /* Setup a request memory pool */

  PUB(req_mid) = lam_mp_init(sizeof(struct lam_ssi_rpi_req),
                             NULL, NULL, 10, 10, 1);

  /* Return 1 saying that we want to be considered */

  return 1;
}


/*
 *	lam_ssi_rpi_gm_init
 *
 *	Function:	- primary initialiation of RPI subsystem
 *			- initialize buffering and socket connections
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_rpi_actions_t *
lam_ssi_rpi_gm_init(struct _proc **procs, int nprocs, 
		    int *max_tag, int *max_cid)
{
#if LAM_WANT_DEBUG
  /* Setup the debug file */

  char filename[128];
  char prefix[128];
  snprintf(filename, 128, "rpi-gm-rank-%d.txt", lam_myproc->p_gps.gps_grank);
  snprintf(prefix, 128, "gm rank %d:" , lam_myproc->p_gps.gps_grank);
  lds.lds_prefix = prefix;
  lds.lds_file_suffix = filename;
#if 0
  PUB(did) = lam_debug_open(&lds);
  lam_debug(PUB(did), "pid %d starting", lam_getpid());
#else
  PUB(did) = -1;
#endif
#endif

  /* Setup my _proc.  Assign the GM port values that we obtained in
     query(), above. */

  proc_init(lam_myproc);
  lam_myproc->p_rpi->cp_gm_port = myport;
  myport.rgp_gm_port = NULL;

  /* Initalize GM stuff.  If we fail, we still have to call
     connect_all() so that we can let the other nodes know of our
     failure. */

  if (lam_ssi_rpi_gm_gm_setup(lam_myproc) == LAMERROR) {
    show_help_file("lam-ssi-rpi-gm-helpfile", "rpi-gm", "failed-to-init");
    connect_all(FALSE);
    return NULL;
  }

  /* Initialize the lower layers of the gm RPI */

  if (lam_ssi_rpi_gm_send_queue_init() != 0)
    return NULL;

  /* Set up all processes for client-to-client communication.  */

  if (lam_ssi_rpi_gm_addprocs(procs, nprocs)) 
    return NULL;
  
  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "gm:max_tiny_size: %d", 
              lam_ssi_rpi_gm_tinymsglen);
    lam_debug(lam_ssi_rpi_did, "gm:memory_pin: %d", 
              lam_ssi_rpi_gm_can_register_mem);
    lam_debug(lam_ssi_rpi_did, "gm:cr_available: %d", 
              PUB(cr_available));
    lam_debug(lam_ssi_rpi_did, "gm:fast: %d", 
	      PUB(using_fast));
  }

  /* If we have CR available, disable fast */

  if (PUB(cr_available) == 1) {
    module_actions.lsra_fastrecv = NULL;
    module_actions.lsra_fastsend = NULL;
  } else if (PUB(using_fast) == 1) {
    module_actions.lsra_fastsend = lam_ssi_rpi_gm_fastsend;
  } else {
    module_actions.lsra_interrupt = NULL;
    module_actions.lsra_checkpoint = NULL;
    module_actions.lsra_continue = NULL;
    module_actions.lsra_restart = NULL;
  }

  return &module_actions;
}


/*
 *	lam_ssi_rpi_gm_addprocs
 *
 *	Function:	- setup for new processes
 *			- makes socket connections
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_addprocs(struct _proc **procs, int nprocs)
{
  int i, added = 0;

  /* Initialize the local procs array if we need to */

  if (PUB(procs) == 0)
    PUB(procs) = lam_arr_init(sizeof(struct _proc *), NULL);

  /* Add these procs into local procs array.  Make sure that
     they're not already in there first. */

  for (i = 0; i < nprocs; ++i)
    if (lam_arr_find(PUB(procs), &procs[i]) == LAMERROR) {
      lam_arr_append(PUB(procs), &procs[i]);
      added = 1;
    }

  /* Ensure that the list is sorted so that things like connect_all()
     will all do things in the same order in all connected processes */

  if (added == 1)
    lam_procsort(lam_arr_get(PUB(procs)), lam_arr_size(PUB(procs)));

  return connect_all(TRUE);
}


/*
 *	lam_ssi_rpi_gm_build
 *
 *	Function:	- builds RPI portion of a request from scratch
 *			- one-time cost separated from lam_ssi_rpi_gm_start()
 *			  to optimize persistent requests
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_build(MPI_Request req)
{
  req->rq_rpi = lam_mp_malloc(PUB(req_mid));
  if (req->rq_rpi == NULL)
    return LAMERROR;

  /* Because of @#%@#$% MPI_Bsend (more specifically, MPI_Ibsend), we
     may actually never invoke lam_ssi_rpi_gm_start(), so ensure that
     we can call destroy cleanly if so. */

  req->rq_rpi->cq_envbuf = NULL;

  return 0;
}


/*
 *	lam_ssi_rpi_gm_start
 *
 *	Function:	- initializes RPI dependent aspects of a request
 *			- cost per request start - separated from
 *			  lam_ssi_rpi_gm_build() to optimize persistent requests
 *	Accepts:	- request list
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_cbuf_msg *bmsg;
  struct lam_ssi_rpi_req *rpi = req->rq_rpi;

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "rpilam_ssi_rpi_start: starting req %p\n", req));
  if (lam_ssi_rpi_comm_dead_m(req)) {
    lam_ssi_rpi_gm_haveadv = 1;
    return 0;
  }

  /* Set common synchronization and communication parameters. The peer
     in a receive request may be a wildcard but will be set to the
     actual peer upon matching with an incoming mesage.  */

  rpi->cq_peer = req->rq_rank;
  rpi->is_advancing = 0;
  rpi->send_advance_fn = NULL;
  rpi->recv_advance_fn = NULL;
  rpi->dma_data_buf = NULL;

  rpi->cq_envbuf = lam_ssi_rpi_gm_dma_env_malloc();
  if (rpi->cq_envbuf == NULL)
    return LAMERROR;
  rpi->cq_envbuf->ge_flags = 0;
  rpi->cq_envbuf->ge_tag = req->rq_tag;
  rpi->cq_envbuf->ge_cid = req->rq_cid;
  rpi->cq_envbuf->ge_target = NULL;
  rpi->cq_envbuf->ge_src_request = req;
#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
  /* This field is only used if we don't have RDMA get, and therefore
     use an alternate long message protocol. */
  rpi->cq_envbuf->ge_dest_request = NULL;
#endif

  /* <RECEIVER> specific initialization */

  if ((req->rq_type == LAM_RQIRECV) || (req->rq_type == LAM_RQIPROBE)) {
    rpi->cq_state = C2CREAD;
    rpi->cq_envbuf->ge_rank = rpi->cq_peer;

    /* If the message is long, we need to pin or setup a DMA buffer
       for it.  Tiny and short messages don't need pinning/DMA buffers
       (tiny's come in the envelope, shorts use commodity/pooled
       pinned buffers). */

    if (req->rq_packsize > lam_ssi_rpi_gm_tinymsglen) {
      if (lam_ssi_rpi_gm_setup_request_dma(req) != 0)
        return LAMERROR;
    }

    /* Check for matching buffered envelope/message. If one is found
       fill in any receive request wildcards and advance the request.  */

    if ((bmsg = 
         lam_ssi_rpi_cbuf_find((struct lam_ssi_rpi_envl *) 
                               rpi->cq_envbuf)) != NULL) {
      lam_ssi_rpi_gm_fill_wildcards(req, 
                                    (lam_ssi_rpi_gm_envl_t *) &bmsg->cm_env);
      return lam_ssi_rpi_gm_unexpected_advance(req, bmsg);
    }

    /* No match was found. This request will be placed on the pending
       recv queue(s) of the appropriate process(es). */

    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "rpilam_ssi_rpi_start: didn't find a buffered "
                    "message to receive\n"));
  }

  /* <SENDER> specific initialization */

  else {
    rpi->cq_envbuf->ge_rank = req->rq_comm->c_group->g_myrank;
    rpi->cq_envbuf->ge_len = req->rq_packsize;

    if (req->rq_proc == lam_myproc) {

      /* send to self */

      return(send_to_self(req_top, req));
    } else {

      /* send to another process */

      rpi->cq_state = C2CWRITE;

      /* long message protocol */
      /* need to pin or setup DMA buffer */

      if (req->rq_packsize > lam_ssi_rpi_gm_tinymsglen) {
	rpi->cq_envbuf->ge_flags |= C2CLONG;
	rpi->send_advance_fn = lam_ssi_rpi_gm_long_send;

        if (lam_ssi_rpi_gm_setup_request_dma(req) != 0)
          return LAMERROR;
      } 

      /* tiny message protocol */
      /* don't use pinning or DMA buffers */

      else {
	if (req->rq_type == LAM_RQISSEND) {
	  rpi->cq_envbuf->ge_flags |= C2CSSEND;
	  rpi->send_advance_fn = lam_ssi_rpi_gm_tiny_send_sync;
	} else
	  rpi->send_advance_fn = lam_ssi_rpi_gm_tiny_send;
      }
    }
  }

  /* All done */

  return 0;
}


/*
 *	lam_ssi_rpi_gm_destroy
 *
 *	Function:	- destroys RPI portion of request
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_destroy(MPI_Request req)
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
      lam_ssi_rpi_gm_rm_read_any_src(req, NULL);
    else 
      lam_ssi_rpi_gm_rm_read_req(req->rq_proc->p_rpi, req);
  }

  /* Return the envelope to the pool.  Due to the strange bsend beast
     (e.g., the bsend_free lamtest), this request may actually never
     have called the rpi_gm_start() RPI API call.  Hence, cq_envbuf
     will be NULL. */
  /* (I don't think it can ever be C2CBUFFERED... but just in case!) */

  if (req->rq_rpi->cq_envbuf != NULL &&
      (req->rq_rpi->cq_envbuf->ge_flags & C2CBUFFERED) == 0)
    lam_ssi_rpi_gm_dma_env_free(req->rq_rpi->cq_envbuf);
  req->rq_rpi->cq_envbuf = NULL;

  /* There are several cases -- all having to do with unexpected
     receives -- where we may have posted a long receive and DMA
     malloced a pinned buffer (this can only happen on systems where
     we cannot register arbitrary memory, such as Solaris), but then
     received a short message (or anything other than a long message).
     In this case, the DMA-malloced buffer won't have been freed in
     the normal chain of events, so we free it here.  It's easier to
     put this special case code in one place (i.e., here) rather than
     the 3-4 cases that this can happen in the unexepcted receive. */

  if (req->rq_rpi->dma_data_buf != NULL) {
    lam_ssi_rpi_gm_dma_free(req->rq_rpi->dma_data_buf,
                            req->rq_packsize);
    req->rq_rpi->dma_data_buf = NULL;
  }

  /* Free the extra state that this RPI caches on the request */

  lam_debug_cond((lam_ssi_rpi_gm_did, "Rank %d: destroying req %p\n", 
                  lam_myproc->p_gps.gps_grank, req));
  if (req->rq_rpi != NULL)
    lam_mp_free(PUB(req_mid), req->rq_rpi);
  req->rq_rpi = NULL;

  return 0;
}


/*
 *	lam_ssi_rpi_gm_advance
 *
 *	Function:	- advances requests
 *			- we try to advance all requests as far as possible
 *			  as allowed by RPI
 *	Accepts:	- request list
 *			- block enable flag
 *	Returns:	- 1: state change, 0: no state change, LAMERROR: error
 */
int
lam_ssi_rpi_gm_advance(MPI_Request req_top, int fl_block)
{
  MPI_Request req;

  lam_ssi_rpi_gm_flblock = fl_block;
  lam_ssi_rpi_gm_haveadv = 0;

  /* Find which requests require IO.  */

  lam_debug_cond((lam_ssi_rpi_gm_did, "advance: starting req loop"));
  for (req = req_top; req; req = req->rq_next) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "advance: looping through: req %p", req));

    /* Start requests that are in the init state.  */

    if (req->rq_state == LAM_RQSINIT)
      if (_mpi_req_start(req) != MPI_SUCCESS)
	return(LAMERROR);

    /* If a blocking request is done, then we may no longer block.  */

    if (req->rq_state == LAM_RQSDONE) {
      lam_debug_cond((lam_ssi_rpi_gm_did, "advance: req %p is done!", req));
      if (req->rq_flags & LAM_RQFBLOCK)
	lam_ssi_rpi_gm_flblock = 0;
      continue;
    }

    /* If we've already starting advancing this request (since we
       already established above that it's not done), skip it */

    if (req->rq_rpi->is_advancing) {
      lam_debug_cond((lam_ssi_rpi_gm_did,
                      "advance: req %p is already advancing", req));
      continue;
    }

    /* If it's a write, call the advance function, which will try to
       actually send (depending if we can get a token, etc.) */

    else if (req->rq_rpi->cq_state == C2CWRITE) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "advance: calling write advance on req %p - %s", 
                      req, MPIL_Request_get_name(req)));
      /* XXX: SS: no error-checking done here */
      req->rq_rpi->send_advance_fn(req);
    } 

    /* Otherwise, if it's a read, add it to the list of requests that
       are eligible to be read */

    else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "advance: adding read_any_source - %p - %s",
                        req, MPIL_Request_get_name(req)));
	lam_ssi_rpi_gm_add_read_any_src(req);
      } else {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "advance: adding read - %p - %s",
                        req, MPIL_Request_get_name(req)));
	lam_ssi_rpi_gm_add_read(req->rq_proc->p_rpi, req);
      }
    }
  }
  lam_debug_cond((lam_ssi_rpi_gm_did, "advance: req loop done"));

  /* Check for and process any pending gm events */

  if (PUB(flblock) == 1)
    lam_ssi_rpi_gm_gm_advance_blocking();
  else
    lam_ssi_rpi_gm_gm_advance_nonblocking();

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "advance: completed: lam_ssi_rpi_gm_haveadv %d", 
                  lam_ssi_rpi_gm_haveadv));
		
  return PUB(haveadv);
}


/*
 *      lam_ssi_rpi_gm_iprobe
 *
 *      Function:       - non-blocking probe
 *                      - public interface for peculiar MPI_Iprobe() which
 *                        does not return a request to the user
 *      Accepts:        - request
 * Returns: - 0: no msg, 1: msg, LAMERROR: error 
 */
int
lam_ssi_rpi_gm_iprobe(MPI_Request req)
{
  int err;

  /* Link the probe request and advance as far as possible.  */

  _mpi_req_add(req);
  _mpi_req_blkclr();
  err = _mpi_req_advance();
  if (err != MPI_SUCCESS) 
    return(LAMERROR);

  /* Unlink the request. */

  if (req->rq_state != LAM_RQSDONE) {
    if (req->rq_rank == MPI_ANY_SOURCE)
      lam_ssi_rpi_gm_rm_read_any_src(req, NULL);
    else
      lam_ssi_rpi_gm_rm_read_req(req->rq_proc->p_rpi, req);
  }
  _mpi_req_rem(req);

  /* A message was found if the request is in the done state.  */

  return((req->rq_state == LAM_RQSDONE) ? 1 : 0);
}


/*
 *	lam_ssi_rpi_gm_module_open
 *
 *	Function:	- open the gm RPI module
 *	Accepts:	- OPT pointer
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_module_open(OPT *ad)
{
  lam_ssi_rpi_gm_params_register();

  return 1;
}


/*
 *	lam_ssi_rpi_gm_module_close
 *
 *	Function:	- close the gm RPI module
 *	Accepts:	- void
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_module_close(void)
{
  /* This function is called when the module is closed.  It is always
     the last function that is called in this module for the duration
     of the process.

     If this module was *used* during the run, then this function is
     redundant.  However, if the gm RPI query function allocated a GM
     port and then was not selected, this function is invoked during
     MPI_Init to close down this module for the rest of the process
     (and free the gm port that we have allocated). */

  if (myport.rgp_gm_port != NULL) {
    lam_ssi_rpi_gm_gm_finalize(&myport);
    myport.rgp_gm_port = NULL;
  }

  return 0;
}


/*
 *	lam_ssi_rpi_gm_finalize
 *
 *	Function:	- final cleanup
 *	Accepts:	- process to cleanup (0 => all processes)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_finalize(struct _proc *p)
{
  if (p)
    return(finalize1(p));
  else {

    /* When p == 0, this is the "final" finalize -- the RPI won't be
       used any more after this. */

    /* Do a barrier, 'cause we have to wait for everything to finish
       finish finish -- not just finish on this side.  This is unlike
       TCP where a sender can close their end of the socket and still
       have messages that are in flight still make it to the
       destination.  In gm, if you send to a port that is closed,
       you'll get an error. */

    lam_nbarrier(lam_arr_get(PUB(procs)), lam_arr_size(PUB(procs)));

    /* clean up pinned memory pool */
    
    lam_ssi_rpi_gm_dma_destroy();
    lam_ssi_rpi_gm_interval_destroy();

    /* Close down gm.  We pass &lam_myproc->p_rpi->cp_gm_port here
       instead of &myport because it is possible that the application
       was checkpointed at some point; in that case, the port
       information would have changed and the current values will only
       be in lam_myproc->p_rpi->cp_gm_port (and not in myport). */

    lam_ssi_rpi_gm_gm_finalize(&lam_myproc->p_rpi->cp_gm_port);

    /* Free our aux list of MPI_ALLOC_MEM'ed memory */

    if (mpi_alloced_mem != NULL)
      al_free(mpi_alloced_mem);
    
    /* Loop through all processes closing connections.  */

    while (lam_arr_size(PUB(procs)) > 0)
      if (finalize1(((struct _proc **) lam_arr_get(PUB(procs)))[0]))
	return (LAMERROR);
    lam_arr_free(PUB(procs));

    /* Send last debugging message */

    lam_debug_cond((lam_ssi_rpi_gm_did, "Last debugging message..."));
    lam_debug_close(lam_ssi_rpi_gm_did);

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
  struct lam_ssi_rpi_cbuf_msg 	msg;			/* buffer list entry */

  /* Look for inactive matching receive/probe and advance if found.  */

  for (recv = req_top; recv; recv = recv->rq_next)
    if ((recv->rq_state == LAM_RQSSTART) &&
	(recv->rq_rpi->cq_state == C2CREAD) &&
	(ENVL_COMPARE(send->rq_rpi->cq_envbuf, 
                      recv->rq_rpi->cq_envbuf)) == 1) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "send_to_self: found matching recv posted, "
                      "send req %p, recv req %p", send, recv));
      if (send_to_self_match(send, recv)) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
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
  msg.cm_env.ce_len = send->rq_rpi->cq_envbuf->ge_len;
  msg.cm_env.ce_tag = send->rq_rpi->cq_envbuf->ge_tag;
  msg.cm_env.ce_flags = send->rq_rpi->cq_envbuf->ge_flags;
  msg.cm_env.ce_rank = send->rq_rpi->cq_envbuf->ge_rank;
  msg.cm_env.ce_cid = send->rq_rpi->cq_envbuf->ge_cid;
#if LAM_DISINFECT
  msg.cm_env.ce_seq = -1;
#endif
  msg.cm_proc = 0;
  msg.cm_dont_delete = 1;
 
  if (send->rq_type == LAM_RQISSEND) {
    send->rq_rpi->cq_state = C2CSENDSELF;
    msg.cm_buf = send->rq_packbuf;
    msg.cm_req = send;
  } else {
    if (send->rq_packsize > 0) {
      if ((msg.cm_buf = (char *) malloc(send->rq_packsize)) == 0)
	return(LAMERROR);
      lam_memcpy(msg.cm_buf, send->rq_packbuf, send->rq_packsize);
    } else
      msg.cm_buf = 0;

    msg.cm_req = 0;
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

  recv->rq_seq = send->rq_seq;
  if (recv->rq_type == LAM_RQIPROBE) {

    /* The receive is actually a probe so the send is not complete.  */

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_envbuf->ge_rank,
				send->rq_rpi->cq_envbuf->ge_tag, 
				send->rq_rpi->cq_envbuf->ge_len);
    
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

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_envbuf->ge_rank,
				send->rq_rpi->cq_envbuf->ge_tag, len);

    /* Need to remove these pending receives from the receive queues.
       This is somewhat inefficient, because we're scanning down the
       proc pending recv queue essentially a second time. */

    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "send_to_self_match: removing read req %p", 
                    recv));
    if (recv->rq_rank == MPI_ANY_SOURCE)
      ret = lam_ssi_rpi_gm_rm_read_any_src(recv, NULL);
    else
      ret = lam_ssi_rpi_gm_rm_read_req(recv->rq_proc->p_rpi, recv);
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
 *	Function:	- make gm connections to all other processes
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
  int4 rport;
  int node;
  unsigned int rnode;
  int i;
  lam_ssi_rpi_gm_connect_t *coutdata, *cindata;

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
    coutdata = (lam_ssi_rpi_gm_connect_t *) &(outmsg.nh_data[0]);
    if (happy) {
      coutdata->rpgc_port_id = lam_myproc->p_rpi->cp_gm_port.rgp_port_id;
      coutdata->rpgc_global_node_id = 
        lam_myproc->p_rpi->cp_gm_port.rgp_global_node_id;
      lam_debug_cond((PUB(did), 
                      "connect_all: sending my (global) node %d, my port %d",
                      coutdata->rpgc_global_node_id, coutdata->rpgc_port_id));
    } else {
      coutdata->rpgc_port_id = -1;
      coutdata->rpgc_global_node_id = -1;
      coutdata->rpgc_peer_proc = NULL;
    }
  }

  /* Loop through all processes, initializing the process data and
     send connection data to everyone.  */

  cindata = (lam_ssi_rpi_gm_connect_t *) &(inmsg.nh_data[0]);
  for (i = 0; i < lam_arr_size(PUB(procs)); ++i) {
    p = array[i];
      
    /* Check and see if this proc is already initialized and connected */
    if ((p->p_mode & LAM_PRPIINIT) && (p->p_mode & LAM_PRPICONNECT))
      continue;

    /* initialize process */
    if (!(p->p_mode & LAM_PRPIINIT))
      proc_init(p);
    
    if (p != lam_myproc) {

      /* Send the Data */
      
      fill_sync(lam_myproc, p, &outmsg);
      coutdata->rpgc_peer_proc = p;
      if (nsend(&outmsg)) 
	return(LAMERROR);
      
      /* Recieve data */
      
      fill_sync(p, lam_myproc, &inmsg);
      if (nrecv(&inmsg)) 
	return(LAMERROR);
      
      rport = cindata->rpgc_port_id;
      rnode = cindata->rpgc_global_node_id;
      lam_debug_cond((PUB(did), 
                      "connect_all: received (global) node %d, port %d",
                      rnode, rport));
       
      /* store data in RPI private area */

      node = p->p_gps.gps_node;
      p->p_rpi->cp_gm_port.rgp_port_id = rport;
      p->p_rpi->cp_gm_port.rgp_global_node_id = rnode;
      p->p_rpi->cp_peer_proc = cindata->rpgc_peer_proc;

#if LAM_SSI_RPI_GM_2
      /* If we're using gm 2.x, we need to convert the remote global
         node ID into a local node ID */

      if (gm_global_id_to_node_id(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port, 
                                  p->p_rpi->cp_gm_port.rgp_global_node_id,
                                  &(p->p_rpi->cp_gm_port.rgp_local_node_id)) !=
                                  GM_SUCCESS) {
        lam_debug_cond((lam_ssi_rpi_gm_did, "connect_all: "
                        "Unable to convert local ID to global ID"));
        happy = FALSE;
      }
#else
      p->p_rpi->cp_gm_port.rgp_local_node_id = 
        p->p_rpi->cp_gm_port.rgp_global_node_id;
#endif

      if (rport == -1 && rnode == -1)
	happy = FALSE;
      
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "connect_all: recv (port %d, id %d)from rank %d on n%d",
                      rport, 
                      p->p_rpi->cp_gm_port.rgp_local_node_id,
                      p->p_gps.gps_grank,
                      node));
      
      if (node > nlinks) 
	return(LAMERROR);
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
  
  lam_debug_cond((lam_ssi_rpi_gm_did, "connect_all: done"));
  return 0;
}


/*
 *	proc_init
 *
 *	Function:	- initialize gm-specific process data
 *	Accepts:	- process
 */
static void
proc_init(struct _proc *p)
{
  p->p_mode |= LAM_PRPIINIT;

  /* Initialize the p_rpi pointer */

  p->p_rpi = malloc(sizeof(struct lam_ssi_rpi_proc));
  if (p->p_rpi == 0)
    return;

  p->p_rpi->cp_proc = p;
  p->p_rpi->cp_gm_port.rgp_gm_port = NULL;
  p->p_rpi->cp_in_nbytes = p->p_rpi->cp_out_nbytes = 0;
  p->p_rpi->num_pending_sends = 0;
  p->p_rpi->num_dropped_sends = 0;

  lam_ssi_rpi_gm_reqlist_init(&(p->p_rpi->cp_pending_recvs));

  /* Keep track of incoming long messages that are in a critical state
     such that if a checkpoint request arrives it would cause a stale
     pointer upon continue/restart. */

  if (PUB(cr_available) == 1) {
    p->p_rpi->cp_unexpected_long_recvs = 
      lam_arr_init(sizeof(struct lam_rpi_ssi_envl *), NULL);
    p->p_rpi->cp_pending_long_recvs = 
      lam_arr_init(sizeof(MPI_Request), NULL);
  } else {
    p->p_rpi->cp_unexpected_long_recvs = NULL;
    p->p_rpi->cp_pending_long_recvs = NULL;
  }
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
  /* Remove this proc from the array of valid procs that this RPI is
     reponsible for */

  lam_arr_remove(PUB(procs), &p);

  /* Double check to ensure this proc still needs to be cleaned up */

  if (p->p_rpi != NULL) {

    /* Free the checkpoint/restat lists of pending long messages */

    if (PUB(cr_available) == 1) {
      lam_arr_free(p->p_rpi->cp_unexpected_long_recvs);
      lam_arr_free(p->p_rpi->cp_pending_long_recvs);
    }

    /* For debugging */

    LAM_ZERO_ME(*(p->p_rpi));

    /* Free it */

    free(p->p_rpi);
    p->p_rpi = NULL;
  }

  return 0;
}


/*
 *	lam_ssi_rpi_gm_alloc_mem
 *
 *	Function:	- default alloc_mem function; wrapper to malloc
 *	Accepts:	- size to allocate
 *                      - info arg (ignored here)
 *	Returns:	- ptr to allocated memory
 */
int 
lam_ssi_rpi_gm_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr)
{
  allocmem_t entry;
  entry.am_length = size;

  /* Ensure the list is initialized */

  if (mpi_alloced_mem == NULL)
    mpi_alloced_mem = al_init(sizeof(allocmem_t), mem_list_comp);

  /* If we support arbitrary pinning, do a standard malloc */

  if (lam_ssi_rpi_gm_can_register_mem == 1) {
    *((void **) baseptr) = malloc(size);
    if (*((void **) baseptr) == NULL)
      return(lam_mkerr(MPI_ERR_NO_MEM, errno));
    entry.am_base = *((void **) baseptr);
    entry.am_registered = 1;

    /* Now register the interval and mark it "LRU not ok" so that it
       won't get freed by LRU sweeps. */

    if (lam_ssi_rpi_gm_interval_use(*((void **) baseptr), 
                                    size, 0, 1) == LAMERROR) {
      free(*((void **) baseptr));
      *((void **) baseptr) = NULL;
      return lam_mkerr(MPI_ERR_NO_MEM, errno);
    }
  }

  /* Otherwise, use the gm function to allocate pinned memory.  This
     will register the interval for us, so it's not necessary to do
     anything after calling lam_ssi_rpi_gm_dma_malloc(). */

  else {
    entry.am_base = *((void **) baseptr) = lam_ssi_rpi_gm_dma_malloc(size);
    entry.am_registered = 0;
  }

  /* Ensure that we got something back. */
  /* JMS Bad error handling here -- this doesn't clean up
     properly... */

  if (entry.am_base == NULL)
    return lam_mkerr(MPI_ERR_NO_MEM, ENOMEM);

  /* All done */

  al_insert(mpi_alloced_mem, &entry);
  return MPI_SUCCESS;
}


/*
 *	lam_ssi_rpi_gm_free_mem
 *
 *	Function:	- default free_mem function; wrapper to free
 *	Accepts:	- ptr to memory to free
 *                        do nothing if the memory was not alloced by 
 *                        lam_ssi_rpi_gm_alloc_mem()
 *	Returns:	- MPI_SUCCESS
 */
int 
lam_ssi_rpi_gm_free_mem(void *base)
{
  allocmem_t *entry, find;

  /* Unpin it.  List search is not efficient, but it's not important
     here. :-) */

  find.am_base = base;
  entry = al_find(mpi_alloced_mem, &find);
  if (entry != NULL) {

    /* Was this memory registered and the interval manually used above? */

    if (entry->am_registered == 1) {

      /* Mark the interval as not being used (i.e., decrease the
         reference count) */

      lam_ssi_rpi_gm_interval_unuse(base, entry->am_length);

      /* Now actually free the memory */

      free(base);
    }

    /* Otherwise, use the gm function to free it.  This will
       automatically unuse the interval as well. */

    else {
      lam_ssi_rpi_gm_dma_free(base, entry->am_length);
    }

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
