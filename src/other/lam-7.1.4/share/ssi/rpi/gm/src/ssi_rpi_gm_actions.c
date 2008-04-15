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
 * $Id: ssi_rpi_gm_actions.c,v 1.43 2004/02/14 16:02:05 jsquyres Exp $
 *
 *	Function:	- calls to gm
 */

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>
#include <lam-ssi-cr.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>
#include <malloc_wrapper.h>

#include <rpi_gm.h>
#include <rpi_gm_unexpected.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_interval.h>
#include <rpi_gm_recv_events.h>
#include <rpi_gm_recv_queue.h>
#include <rpi_gm_send_queue.h>
#include <rpi_gm_tiny.h>
#include <rpi_gm_long.h>
#include <rpi_gm_actions.h>


/*
 * local variables
 */
static int param_tinymsglen;
static int param_nopin;
static int param_max_port_num;
static int param_port_num;
static char gm_version[BUFSIZ];


/*
 * Local functions
 */
static void resend_callback(struct gm_port *port, void *context, 
                            gm_status_t status);


/*
 * Register gm rpi SSI parameters
 */
void
lam_ssi_rpi_gm_params_register(void)
{
  int nopin_default;

  param_tinymsglen = 
    lam_ssi_base_param_register_int("rpi", "gm", "tinymsglen", NULL,
                                    LAM_SSI_RPI_GM_TINYMSGLEN);

  /* Try to register a bogus memory handler to see if we can pin
     memory.  If we succeed, then we have a valid memory manager, and
     we can register/pin memory.  If we don't have a memory manager,
     then either the user chose not to use one or we're on Solaris,
     and we can't register/pin memory. */

  nopin_default = 0;
  if (lam_register_free_handler(NULL) == LAMERROR) {
    nopin_default = 1;
  }
  param_nopin =
    lam_ssi_base_param_register_int("rpi", "gm", "nopin", NULL, 
                                    nopin_default);

  param_max_port_num =
    lam_ssi_base_param_register_int("rpi", "gm", "maxport", NULL,
                                    LAM_SSI_RPI_GM_MAX_PORT_NUM);
  param_port_num =
    lam_ssi_base_param_register_int("rpi", "gm", "port", NULL,
                                    LAM_SSI_RPI_GM_PORT);
  PUB(param_fast) =
    lam_ssi_base_param_register_int("rpi", "gm", "fast", NULL, 0);
  PUB(param_cr) =
    lam_ssi_base_param_register_int("rpi", "gm", "cr", NULL, 0);

  /* This one must be public because it needs to be seen/used in
     ssi_rpi_gm.c during the query() call. */

  PUB(param_priority) =
    lam_ssi_base_param_register_int("rpi", "gm", "priority", NULL,
                                    LAM_SSI_RPI_GM_DEFAULT ? 75 : 50);

  /* Set a string of the gm version that we were compiled against.  Of
     course, gm keeps changing the format... */

  if (LAM_SSI_RPI_GM_API_VERSION > 0x200) {
    snprintf(gm_version, sizeof(gm_version), "%d.%d.%d", 
             (LAM_SSI_RPI_GM_API_VERSION & 0xFF0000) >> 16,
             (LAM_SSI_RPI_GM_API_VERSION & 0xFF00) >> 8,
             LAM_SSI_RPI_GM_API_VERSION & 0xFF);
  } else {
    snprintf(gm_version, sizeof(gm_version), "%d.%d", 
             LAM_SSI_RPI_GM_API_VERSION >> 8,
             LAM_SSI_RPI_GM_API_VERSION & 0xFF);
  }
  lam_ssi_base_param_register_string("rpi", "gm", "api_version",
                                     LAM_SSI_BASE_PARAM_INFO,
                                     gm_version);
                                     
}


/*
 *       Function: performs inital setup for Myrinet/gm
 *       Accepts:  _proc for process
 *       Returns:  0 or LAMERROR
 */
int
lam_ssi_rpi_gm_gm_setup(struct _proc *p)
{
  int i;
  struct lam_ssi_rpi_gm_envl *env;

  /* Get the number of tokens that gm will give to us */

  PUB(max_rtokens) = PUB(rtokens) = 
    gm_num_receive_tokens(p->p_rpi->cp_gm_port.rgp_gm_port);
  PUB(max_stokens) = PUB(stokens) = 
    gm_num_send_tokens(p->p_rpi->cp_gm_port.rgp_gm_port);

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "setup_gm: starting with %d recv tokens, %d send tokens", 
                  PUB(rtokens), PUB(stokens)));

  /* Check for env variable overrides for tiny and short message size
     lengths.  Ensure that short > tiny. */

  lam_ssi_rpi_gm_tinymsglen = 
    lam_ssi_base_param_lookup_int(param_tinymsglen);
  if (lam_ssi_rpi_gm_tinymsglen < 0)
    lam_ssi_rpi_gm_tinymsglen = LAM_SSI_RPI_GM_TINYMSGLEN;

  lam_ssi_rpi_gm_dma_env_len = lam_ssi_rpi_gm_tinymsglen + 
    sizeof(struct lam_ssi_rpi_gm_envl);

  /* Setup for direct DMA */

  if (gm_allow_remote_memory_access(p->p_rpi->cp_gm_port.rgp_gm_port) 
      != GM_SUCCESS) {
    return LAMERROR;
  }

  /* See if we want to force not to use pinning (and use
     dma_malloc/memcpy instead) */

  lam_ssi_rpi_gm_can_register_mem =
    !lam_ssi_base_param_lookup_int(param_nopin);

  /* Setup pinned interval registration */

  if (lam_ssi_rpi_gm_interval_init(p->p_rpi) == LAMERROR)
    return LAMERROR;

  /* Setup a lookaside table for quick messages */

  if (lam_ssi_rpi_gm_dma_init(p->p_rpi) == LAMERROR)
    return LAMERROR;

  /* Length to use in gm_send_with_callback.  Ensure that they're not
     the same. */

  lam_ssi_rpi_gm_env_min_size_for_length = 
    gm_min_size_for_length(lam_ssi_rpi_gm_dma_env_len);

  /* Provide recv buffers for envelopes and short messages.  Use all
     available receive tokens -- long messages are sent using RMA
     PUTs. */

  for (i = 0; i < PUB(rtokens); ++i) {
    env = lam_ssi_rpi_gm_dma_env_malloc();
    gm_provide_receive_buffer(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
                              env,
                              lam_ssi_rpi_gm_env_min_size_for_length,
                              LAM_SSI_RPI_GM_PRIORITY);
  }

  /* All done */

  return 0;
}


/*
 *      get_gm_port
 *          
 *      Function:       - gets a port for gmnet communications
 *      Accepts:        - _proc for process, struct to hold results
 *      Returns:        - 0 or LAMERROR
 */
int 
lam_ssi_rpi_gm_get_port(struct _proc *p, lam_ssi_rpi_gm_port_t *rgp)
{
  char name[64];
  int result;
  int portNum = 0, cardNum = 0;
  int foundPort = 0;
  int minPort = lam_ssi_rpi_gm_port_num;
  int rank = p->p_gps.gps_grank;

  LAM_ZERO_ME(name);
  snprintf(name, sizeof(name), "LAM/MPI rank-%d", rank);

  /* Check for environment variable overrides */

  lam_ssi_rpi_gm_max_port_num =
    lam_ssi_base_param_lookup_int(param_max_port_num);
  if (lam_ssi_rpi_gm_max_port_num < 1)
    lam_ssi_rpi_gm_max_port_num = LAM_SSI_RPI_GM_MAX_PORT_NUM;

  minPort = lam_ssi_base_param_lookup_int(param_port_num);
  if (minPort < 0)
    minPort = 1;
  else
    lam_ssi_rpi_gm_max_port_num = minPort + 1;

  /* Try to find an available port by scanning from minPort - maxPort */

  for (foundPort = 0, portNum = minPort; 
       portNum < lam_ssi_rpi_gm_max_port_num; ++portNum) {

    /* Ports 0 and 2 are reserved by gm; just skip right over them */

    if (portNum == 0 || portNum == 2)
      continue;

    for (cardNum = 0; cardNum < 8; ++cardNum) {
      if (gm_open(&(rgp->rgp_gm_port),
                  cardNum,
                  portNum,
                  name, 
                  LAM_SSI_RPI_GM_API_VERSION) == GM_SUCCESS) {
        foundPort = 1;
        break;
      }
    }

    if (foundPort == 1)
      break;
  }

  /* if we didn't find one, bail */

  if (!foundPort) {
    lam_debug_cond((lam_ssi_rpi_gm_did,
                    "get_gm_port: error finding port (1-%d tried)", 
                    portNum));
    return(LAMERROR);
  }
  rgp->rgp_port_id = portNum;
    
  /* get the [local] GM node */

  result = gm_get_node_id(rgp->rgp_gm_port, &(rgp->rgp_local_node_id));
  if (result != GM_SUCCESS) {
    lam_debug_cond((lam_ssi_rpi_gm_did,
		    "get_gm_port: Couldn't get my gmID (%d)",
		    result));
    return(LAMERROR);
  }

#if LAM_SSI_RPI_GM_2
  /* If we're using gm 2.x, we need to convert my local node ID to a
     global node ID */

  if (gm_node_id_to_global_id(rgp->rgp_gm_port, 
                              rgp->rgp_local_node_id,
                              &(rgp->rgp_global_node_id)) != GM_SUCCESS) {
    lam_debug_cond((lam_ssi_rpi_gm_did, "connect_all: "
                    "Unable to convert local ID to global ID"));
    return(LAMERROR);
  }
#else
  rgp->rgp_global_node_id = rgp->rgp_local_node_id;
#endif

  lam_debug_cond((lam_ssi_rpi_gm_did, "get_gm_port: gmID=%d port=%d", 
                  rgp->rgp_global_node_id, rgp->rgp_port_id));

  return 0;
}


/*
 * Shut down GM
 */
int
lam_ssi_rpi_gm_gm_finalize(lam_ssi_rpi_gm_port_t *myport)
{
  if (myport != NULL && myport->rgp_gm_port != NULL) {
    gm_close(myport->rgp_gm_port);
    myport->rgp_gm_port = NULL;
    myport->rgp_global_node_id = 0;
    myport->rgp_local_node_id = 0;
    myport->rgp_port_id = -1;
  }
  gm_finalize();

  return 0;
}

/************************************************************************/

/*
 * Check for and act on gm events.  Do so in a blocking manner.
 */
int 
lam_ssi_rpi_gm_gm_advance_blocking(void)
{
  int fast;
  int done = 0;
  gm_recv_event_t *event;
  struct gm_port *port;
  struct lam_ssi_rpi_proc *proc;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "%d: lam_ssi_rpi_gm_gm_advance_blocking(%d) started",
                  lam_myproc->p_gps.gps_grank,
                  lam_ssi_rpi_gm_flblock));

  proc = lam_myproc->p_rpi;
  port = proc->cp_gm_port.rgp_gm_port;

  /* Loop until PUB(haveadv) is 1, meaning that we've made progress on
     some request */

  do {
    fast = 0;
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "%d: lam_ssi_rpi_gm_gm_advance calling gm_*receive",
                    lam_myproc->p_gps.gps_grank));

    event = gm_receive(port);
    switch (GM_RECV_EVENT_TYPE(event)) {
    case GM_FAST_RECV_EVENT:
    case GM_FAST_HIGH_RECV_EVENT:
    case GM_FAST_PEER_RECV_EVENT:
    case GM_FAST_HIGH_PEER_RECV_EVENT:
      fast = 1;
    case GM_RAW_RECV_EVENT:
    case GM_RECV_EVENT:
    case GM_HIGH_RECV_EVENT:
    case GM_PEER_RECV_EVENT:
    case GM_HIGH_PEER_RECV_EVENT:

      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: lam_ssi_rpi_gm_gm_advance handling receive event",
                      lam_myproc->p_gps.gps_grank));
      if (lam_ssi_rpi_gm_recv_event(event, fast) != 0) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "gm_advance: recv_event returns LAMERROR"));
	return LAMERROR;
      }

      done = 1;
      break;

    default:
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: gm_unknown event: %d -- "
                      "calling gm_unknown, haveadv %d", 
                      lam_myproc->p_gps.gps_grank,
                      GM_RECV_EVENT_TYPE(event), lam_ssi_rpi_gm_haveadv));
      gm_unknown(port, event);
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: back from gm_unknown event: %d", 
                      lam_myproc->p_gps.gps_grank,
                      GM_RECV_EVENT_TYPE(event)));
      
      /* Check the lam_ssi_rpi_gm_haveadv value to see if we just
         finished a send in a callback */
      
      done = lam_ssi_rpi_gm_haveadv;
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: back from gm_unknown event: %d -- "
                      "haveadv %d, done %d", 
                      lam_myproc->p_gps.gps_grank,
                      GM_RECV_EVENT_TYPE(event), 
                      lam_ssi_rpi_gm_haveadv, done));

    }

    /* This is not as clean as it should be because the RPI still
       handles global data (the request list).  If the cr thread is
       waiting for us to yield, then do so. */

    if (PUB(cr_available) == 1 &&
        LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING ==
        lam_ssi_crmpi_base_handler_state) {
      lam_ssi_crmpi.lscrma_app_suspend();
      port = proc->cp_gm_port.rgp_gm_port;
      lam_debug_cond((PUB(did), " back from suspend -- haveadv %d\n", 
                      PUB(haveadv)));
    }

    /* If we reclaimed a send token (i.e., some send completed), try
       to advance the pending send queue (if there are any).
       Reference a global variable here to avoid invoking a extra
       function call on every iteration through this loop.  */

    if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() ||
        !LAM_SSI_RPI_GM_ACK_QUEUE_EMPTY())
      if (lam_ssi_rpi_gm_send_queue_advance() != 0) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "gm_advance: send_queue_advance returns LAMERROR"));
	return LAMERROR;
      }
  } while (done == 0);

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "%d: dropping out of lam_ssi_rpi_gm_advance_blocking",
                  lam_myproc->p_gps.gps_grank));

  return 0;
}


/*
 * Check for a gm event.  Loop until there are no pending gm events.
 */
int 
lam_ssi_rpi_gm_gm_advance_nonblocking(void)
{
  int fast;
  gm_recv_event_t *event;
  struct gm_port *port;
  struct lam_ssi_rpi_proc *proc;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "%d: lam_ssi_rpi_gm_gm_advance_nonblocking(%d) started",
                  lam_myproc->p_gps.gps_grank,
                  lam_ssi_rpi_gm_flblock));

  proc = lam_myproc->p_rpi;
  port = proc->cp_gm_port.rgp_gm_port;

  /* If blocking, get one event and then keep going as long as there
     are more pending. If non-blocking, go until a null-event */

  while (gm_receive_pending(port) != 0) {
    fast = 0;
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "%d: lam_ssi_rpi_gm_gm_advance calling gm_*receive",
                    lam_myproc->p_gps.gps_grank));

    event = gm_receive(port);
    switch (GM_RECV_EVENT_TYPE(event)) {
    case GM_FAST_RECV_EVENT:
    case GM_FAST_HIGH_RECV_EVENT:
    case GM_FAST_PEER_RECV_EVENT:
    case GM_FAST_HIGH_PEER_RECV_EVENT:
      fast = 1;
    case GM_RAW_RECV_EVENT:
    case GM_RECV_EVENT:
    case GM_HIGH_RECV_EVENT:
    case GM_PEER_RECV_EVENT:
    case GM_HIGH_PEER_RECV_EVENT:

      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: lam_ssi_rpi_gm_gm_advance handling receive event",
                      lam_myproc->p_gps.gps_grank));
      if (lam_ssi_rpi_gm_recv_event(event, fast) != 0) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "gm_advance: recv_event returns LAMERROR"));
	return LAMERROR;
      }
      break;

    case GM_NO_RECV_EVENT:

      /* There's no gm events pending.  We're done. */

      break;

    default:
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: gm_unknown event: %d -- "
                      "calling gm_unknown, haveadv %d", 
                      lam_myproc->p_gps.gps_grank,
                      GM_RECV_EVENT_TYPE(event), lam_ssi_rpi_gm_haveadv));
      gm_unknown(port, event);
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "%d: back from gm_unknown event: %d", 
                      lam_myproc->p_gps.gps_grank,
                      GM_RECV_EVENT_TYPE(event)));
    }

    /* Don't check for the CR thread here -- this is a non-blocking
       function, so we're going to finish in finite time (i.e., as
       soon as there's no more gm requests, and that's guaranteed to
       be finite because once the app thread leaves the MPI layer, it
       won't be able to get back in -- and therefore no more
       MPI_Requests can be generated. */

    /* If we reclaimed a send token (i.e., some send completed), try
       to advance the pending send queue (if there are any).
       Reference a global variable here to avoid invoking a extra
       function call on every iteration through this loop.  */

    if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() ||
        !LAM_SSI_RPI_GM_ACK_QUEUE_EMPTY())
      if (lam_ssi_rpi_gm_send_queue_advance() != 0) {
	lam_debug_cond((lam_ssi_rpi_gm_did, 
                        "gm_advance: send_queue_advance returns LAMERROR"));
	return LAMERROR;
      }
  }

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "%d: dropping out of lam_ssi_rpi_gm_advance_nonblocking",
                  lam_myproc->p_gps.gps_grank));

  return 0;
}


/************************************************************************/

/*
 * All setup has been done already -- just do the send and setup the
 * callback.
 */
void
lam_ssi_rpi_gm_push_envelope(MPI_Request req, 
                             gm_send_completion_callback_t callback)
{
  struct lam_ssi_rpi_gm_envl *env = req->rq_rpi->cq_envbuf;
  char *body = req->rq_packbuf;
  int body_len = req->rq_rpi->cq_envbuf->ge_len;
  int copy_len = sizeof(struct lam_ssi_rpi_gm_envl);
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  env->ge_peer_proc = destproc->cp_peer_proc;

#if LAM_WANT_DEBUG
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "gm_push_envelope: rank %d, tag %d, cid %d, flags 0x%x, "
                  "len %d", 
                  env->ge_rank,
                  env->ge_tag,
                  env->ge_cid,
                  env->ge_flags,
                  env->ge_len));
#endif

  if (body != NULL && body_len > 0 && body_len <= lam_ssi_rpi_gm_tinymsglen) {
    copy_len += body_len;
    memcpy(env + 1, body, body_len);
  }

  /* Now do the actual send.  The caller has ensured that we already
     have a token, so we're good to go. */

  --PUB(stokens);

  /* Increment the pending sends count */

  ++(destproc->num_pending_sends);
  gm_send_with_callback(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
                        env,
                        lam_ssi_rpi_gm_env_min_size_for_length,
                        copy_len,
                        LAM_SSI_RPI_GM_PRIORITY,
                        destproc->cp_gm_port.rgp_local_node_id,
                        destproc->cp_gm_port.rgp_port_id,
                        callback,
                        req);

  /* increment sent bookmark */

  destproc->cp_out_nbytes += copy_len;

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "did gm_send / env to proc %d, body_len %d, copy_len %d, "
                  "sendbuf %p",
                  destproc->cp_proc->p_gps.gps_grank, body_len, copy_len,
                  env));
}


#if LAM_SSI_RPI_GM_HAVE_RDMA_GET
/*
 * All setup has been done already -- just do the get and setup the
 * callback.  RDMA "get" functionality only exists in gm 2.x.
 */
void
lam_ssi_rpi_gm_get_long_body(MPI_Request req, void *source,
                             int length,
                             gm_send_completion_callback_t callback_unpin,
                             gm_send_completion_callback_t callback_no_unpin)
{
  char *dest;
  gm_send_completion_callback_t callback;
  struct lam_ssi_rpi_proc *srcproc = req->rq_proc->p_rpi;

  /* If the dma_data_buf on the req is not NULL, then we could not pin
     and had to dma_malloc (way back in req_start).  Hence, we need to
     memcpy and setup to dma_free later. */

  if (req->rq_rpi->dma_data_buf != NULL) {
    callback = callback_unpin;
    dest = req->rq_rpi->dma_data_buf;
  }

  /* The message has already been copied from the original buffer to
     the DMA buffer -- it's ready to be sent out. */

  else {
    callback = callback_no_unpin;
    dest = req->rq_packbuf;
  }

  /* post the send.  Note that gm_directed_send...() returns void! */

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "get_long_body: to %d len %d buf 0x%p, target buf 0x%p", 
                  srcproc->cp_gm_port.rgp_local_node_id, length, dest, 
                  req->rq_rpi->cq_envbuf->ge_target));
  --PUB(stokens);
  gm_get(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
         (gm_remote_ptr_t)(gm_up_t) source,
         dest,
         length,
         LAM_SSI_RPI_GM_PRIORITY,
         srcproc->cp_gm_port.rgp_local_node_id,
         srcproc->cp_gm_port.rgp_port_id,
         callback,
         req);
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "did gm_get / long body from src proc %d",
                  srcproc->cp_proc->p_gps.gps_grank));
}

#else /* LAM_SSI_RPI_GM_HAVE_RDMA_GET */

/*
 * All setup has been done already -- just do the send and setup the
 * callback.  This is only for gm 1.x, where we don't have RDMA "get"
 * functionality.
 */
int
lam_ssi_rpi_gm_put_long_body(MPI_Request req,
                             gm_send_completion_callback_t callback_unpin,
                             gm_send_completion_callback_t callback_no_unpin)
{
  char *sendbuf;
  gm_send_completion_callback_t callback;
  int length = req->rq_rpi->cq_envbuf->ge_len;
  gm_remote_ptr_t targetbuf = 
    (gm_remote_ptr_t)(gm_up_t) req->rq_rpi->cq_envbuf->ge_target;
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  /* If the dma_data_buf on the req is not NULL, then we could not pin
     and had to dma_malloc (way back in req_start).  Hence, we need to
     memcpy and setup to dma_free later. */

  if (req->rq_rpi->dma_data_buf != NULL) {
    callback = callback_unpin;
    sendbuf = req->rq_rpi->dma_data_buf;

    /* The message has already been copied from the original buffer to
       the DMA buffer (latency-hiding technique) -- it's ready to be
       sent out. */
  } else {
    callback = callback_no_unpin;
    sendbuf = req->rq_packbuf;
  }

  /* post the send.  Note that gm_directed_send...() returns void! */

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "send_long_body: to %d len %d buf 0x%p, target buf 0x%p", 
                  destproc->cp_gm_port.rgp_local_node_id, length, sendbuf, 
                  req->rq_rpi->cq_envbuf->ge_target));
  --PUB(stokens);
  LAM_GM_PUT(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
             sendbuf,
             targetbuf,
             length,
             LAM_SSI_RPI_GM_PRIORITY,
             destproc->cp_gm_port.rgp_local_node_id,
             destproc->cp_gm_port.rgp_port_id,
             callback,
             req);
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "did LAM_GM_PUT / long body to proc %d",
                  destproc->cp_proc->p_gps.gps_grank));

  return 0;
}
#endif /* LAM_SSI_RPI_GM_HAVE_RDMA_GET */


/* 
 * Take care of the resend and timeouts during gm_sends 
 */

int
lam_ssi_rpi_gm_resend(MPI_Request req)
{
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  /* This send timed out. Drop all the pending sends (there may be
     some for ISends -- and wait for the callback of each one of them
     -- which will be GM_DROPPED_SEND. Take the action of resending in
     that callback */

  destproc->num_dropped_sends = destproc->num_pending_sends;

  lam_debug_cond((PUB(did), "RESEND: num dropped send for proc %x is %d \n",
		  destproc, destproc->num_dropped_sends));

  gm_drop_sends(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
		LAM_SSI_RPI_GM_PRIORITY,
		destproc->cp_gm_port.rgp_local_node_id,
		destproc->cp_gm_port.rgp_port_id,
		resend_callback,
		NULL);
  return 0;
}


/*
 * Invoked as the result of a gm_drop_sends().
 */
static void 
resend_callback(struct gm_port *port, void *context, gm_status_t status)
{
  /* According to the docs for gm_drop_sends(), all we can do in this
     callback is increment the number of send tokens */

  ++PUB(stokens);
}


/* 
 * Callback for a dropped send which will happen during a send timeout 
 */
void 
lam_ssi_rpi_gm_dropped_send_callback(struct gm_port *port, void *context,
				     gm_status_t status)
{
  MPI_Request req = (MPI_Request) context;
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;

  lam_debug_cond((PUB(did), "RESEND: timeout send ... so resending"));

  /* Need to reset the pointer, because it's likely to be NULL at this
     point (because it wasn't needed prior to this) */

  if (req->rq_packsize > lam_ssi_rpi_gm_tinymsglen) {
    req->rq_rpi->send_advance_fn = lam_ssi_rpi_gm_long_send;
  } else {
    if (req->rq_type == LAM_RQISSEND) {
      req->rq_rpi->send_advance_fn = lam_ssi_rpi_gm_tiny_send_sync;
    } else {
      req->rq_rpi->send_advance_fn = lam_ssi_rpi_gm_tiny_send;
    }
  }

  --destproc->num_dropped_sends;

  /* Increment the number of stokens, because they will be decremented in
     push_envelope() */
  ++PUB(stokens);

  /* Now resend the dropped send */
  lam_ssi_rpi_gm_push_envelope(req, req->rq_rpi->callback);
}
