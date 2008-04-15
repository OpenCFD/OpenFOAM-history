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
 * $Id: ssi_rpi_sysv.c,v 1.36 2004/01/02 00:14:04 jsquyres Exp $
 *
 *	Function:	- shared memory and TCP client-to-client interface
 */

#include <lam_config.h>
#include <lam-ssi-rpi-sysv-config.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>		       /* LINUX FD_SET etc. */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(HAVE_NETINET_TCP_H) && HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#include <unistd.h>

#if LAM_HAVE_FIONBIO
#include <sys/ioctl.h>
#endif

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <sfh.h>
#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <dl_inet.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <lamdebug.h>

#if LAM_WANT_IMPI
#include <impi.h>
#endif

#include <rpi_sysv.h>
#include <sysv_low.h>
#include <sysv_shmem.h>
#include <sysv_common.h>

/*
 * We somewhat abuse the SSI RPI system here -- this RPI is tied to
 * the TCP RPI.  Hence, we know some of the variables and functions
 * that it exports.
 */
#include <rpi_tcp_macros.h>
#include <tcp_low.h>


/*
 * private functions
 */
static void add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static void add_read_any_src(MPI_Request req);
static void add_write(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int send_to_self(MPI_Request req_top, MPI_Request send);
static int send_to_self_match(MPI_Request send, MPI_Request recv);
static int connect_all(void);
static void fill_sync(struct _proc *src,
		      struct _proc *dest, struct nmsg *head);
static void proc_init(struct _proc *p);
static int finalize1(struct _proc *p);


/*
 * Private variables
 */
static int param_poll_yield;
static int param_poolsize;
static int param_maxalloc;
static int param_short;
static int param_tcp_short;
static int param_tcp_sockbuf;
static int tcp_sockbuf;
static int param_priority;
static const lam_ssi_rpi_actions_1_0_0_t module_actions = {

  /* RPI API functions */

  lam_ssi_rpi_sysv_addprocs,
  lam_ssi_rpi_sysv_finalize,

  lam_ssi_rpi_sysv_build,
  lam_ssi_rpi_sysv_start,
  lam_ssi_rpi_sysv_advance,
  lam_ssi_rpi_sysv_destroy,

  lam_ssi_rpi_sysv_iprobe,

  lam_ssi_rpi_sysv_fastrecv,
  lam_ssi_rpi_sysv_fastsend,

  lam_ssi_rpi_base_alloc_mem,
  lam_ssi_rpi_base_free_mem,

  NULL,
  NULL,
  NULL,

  /* Flags */
  /* Totalview support */

  1
};
/*
 * Open the module and register an SSI parameter
 */
int
lam_ssi_rpi_sysv_open_module(OPT *ad)
{
  param_poll_yield =
    lam_ssi_base_param_register_int("rpi", "sysv", "pollyield", NULL, 1);
  param_poolsize =
    lam_ssi_base_param_register_int("rpi", "sysv", "poolsize", NULL,
                                    LAM_SHMPOOLSIZE);
  param_maxalloc =
    lam_ssi_base_param_register_int("rpi", "sysv", "maxalloc", NULL,
                                    LAM_SHMMAXALLOC);
  param_short = 
    lam_ssi_base_param_register_int("rpi", "sysv", "short", NULL,
                                    LAM_SHMSHORTMSGLEN);

  param_tcp_short = 
    lam_ssi_base_param_register_int("rpi", "tcp", "short", NULL,
                                    LAM_TCPSHORTMSGLEN);
  param_tcp_sockbuf = 
    lam_ssi_base_param_register_int("rpi", "tcp", "sockbuf", NULL, -1);

  param_priority = 
    lam_ssi_base_param_register_int("rpi", "sysv", "priority", NULL,
                                    LAM_SSI_RPI_SYSV_DEFAULT ? 75 : 30);

  return 1;
}


/*
 * lam_ssi_rpi_sysv_query
 *
 * Function: determine if this module wants to run
 */
int 
lam_ssi_rpi_sysv_query(int *priority, int *thread_min, int *thread_max)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return 0;

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "sysv: module initializing");
    lam_debug(lam_ssi_rpi_did, "sysv:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "sysv:priority: %d", *priority);
  }

  /* Return 1 saying that we want to be considered */

  return 1;
}


/*
 *	lam_ssi_rpi_sysv_init
 *
 *	Function:	- primary initialiation of RPI subsystem
 *			- initialize buffering and socket connections
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_rpi_actions_1_0_0_t *
lam_ssi_rpi_sysv_init(struct _proc **procs, int nprocs, 
		      int *maxtag, int *maxcid)
{
  /* Set tunable parameters */

  lam_ssi_rpi_sysv_poll_yield =
    lam_ssi_base_param_lookup_int(param_poll_yield);
  lam_ssi_rpi_sysv_short =
    lam_ssi_base_param_lookup_int(param_short);
  lam_ssi_rpi_sysv_poolsize =
    lam_ssi_base_param_lookup_int(param_poolsize);
  lam_ssi_rpi_sysv_maxalloc =
    lam_ssi_base_param_lookup_int(param_maxalloc);

  lam_ssi_rpi_tcp_short =
    lam_ssi_base_param_lookup_int(param_tcp_short);
  tcp_sockbuf = lam_ssi_base_param_lookup_int(param_tcp_sockbuf);
  if (tcp_sockbuf < 0) {
    tcp_sockbuf = lam_ssi_rpi_tcp_short;
  }

  lam_ssi_rpi_sysv_maxalloc = lam_ssi_rpi_sysv_maxalloc >> LAM_LOG_ALIGN;

  if (lam_ssi_rpi_verbose >= 0) {
    lam_debug(lam_ssi_rpi_did, "sysv: module initializing");
    lam_debug(lam_ssi_rpi_did, "sysv:pollyield: %d", 
	      lam_ssi_rpi_sysv_poll_yield);
    lam_debug(lam_ssi_rpi_did, "sysv:short: %d bytes", 
	      lam_ssi_rpi_sysv_short);
    lam_debug(lam_ssi_rpi_did, "sysv:shmpoolsize: %d bytes", 
	      lam_ssi_rpi_sysv_poolsize);
    lam_debug(lam_ssi_rpi_did, "sysv:shmmaxalloc: %d bytes", 
	      lam_ssi_rpi_sysv_maxalloc);
    lam_debug(lam_ssi_rpi_did, "tcp:short: %d bytes", 
	      lam_ssi_rpi_tcp_short);
    lam_debug(lam_ssi_rpi_did, "tcp:sockbuf: %d bytes", tcp_sockbuf);
  }

  /* Set up all processes for client-to-client communication. */

  if (lam_ssi_rpi_sysv_addprocs(procs, nprocs))
    return NULL;

  return &module_actions;
}


/*
 *	lam_ssi_rpi_sysv_addprocs
 *
 *	Function:	- setup for new processes
 *			- makes socket connections
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_addprocs(struct _proc **procs, int nprocs)
{
  int i, added = 0;

  /* Initialize the local procs array if we need to */

  if (lam_ssi_rpi_tcp_procs == 0)
    lam_ssi_rpi_tcp_procs = lam_arr_init(sizeof(struct _proc *), NULL);

  /* Add these procs into lam_ssi_rpi_tcp_procs.  Make sure that
     they're not already in there first. */

  for (i = 0; i < nprocs; ++i)
    if (lam_arr_find(lam_ssi_rpi_tcp_procs, &procs[i]) == LAMERROR) {
      lam_arr_append(lam_ssi_rpi_tcp_procs, &procs[i]);
      added = 1;
    }

  /* Ensure that the list is sorted so that things like connect_all()
     will all do things in the same order in all connected processes */

  if (added == 1)
    lam_procsort(lam_arr_get(lam_ssi_rpi_tcp_procs),
		 lam_arr_size(lam_ssi_rpi_tcp_procs));

  /* Now actually open connections to them */

  return connect_all();
}


/*
 *	lam_ssi_rpi_sysv_build
 *
 *	Function:	- builds RPI portion of a request from scratch
 *			- one-time cost separated from lam_ssi_rpi_sysv_start()
 *			  to optimize persistent requests
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_build(MPI_Request req)
{
  req->rq_rpi = malloc(sizeof(struct lam_ssi_rpi_req));
  if (req->rq_rpi == 0)
    return LAMERROR;

  req->rq_rpi->cq_envbuf = (char *) &req->rq_rpi->cq_env;
  return (0);
}


/*
 *	lam_ssi_rpi_sysv_start
 *
 *	Function:	- initializes RPI dependent aspects of a request
 *			- cost per request start - separated from
 *			  lam_ssi_rpi_sysv_build() to optimize persistent requests
 *	Accepts:	- request list
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_cbuf_msg *bmsg;	/* buffered message */

  if (lam_ssi_rpi_comm_dead_m(req)) {
    lam_ssi_rpi_tcp_haveadv = 1;
    return 0;
  }
/*
 * Set common synchronization and communication parameters. The peer in
 * a receive request may be a wildcard but will be set to the actual
 * peer upon matching with an incoming mesage.
 */
  req->rq_rpi->cq_peer = req->rq_rank;
  req->rq_rpi->cq_env.ce_flags = 0;
  req->rq_rpi->cq_env.ce_tag = req->rq_tag;
  req->rq_rpi->cq_env.ce_cid = req->rq_cid;
/*
 * receive specific initialization
 */
  if ((req->rq_type == LAM_RQIRECV) || (req->rq_type == LAM_RQIPROBE)) {
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
/*
 * Check for matching buffered envelope/message and advance according to
 * protocol with sender. The TCP protocol takes care of receiving from self.
 */
    if ((bmsg = lam_ssi_rpi_cbuf_find(&req->rq_rpi->cq_env))) {
      lam_ssi_rpi_sysv_fill_wildcards(req, &bmsg->cm_env);
      if (req->rq_proc->p_rpi->cp_inbox != 0)
	return (lam_ssi_rpi_sysv_buffered_adv(req, bmsg));
      else
	return (lam_ssi_rpi_tcp_buffered_adv(req, bmsg));
    }
/*
 * Set function to advance request once a matching request is read in.
 * This function will demux the protocols.
 */
    req->rq_rpi->cq_adv = (req->rq_type == LAM_RQIRECV)
      ? lam_ssi_rpi_sysv_tcp_req_recv : lam_ssi_rpi_sysv_shmtcp_req_probe;
  }
/*
 * send specific initialization
 */
  else {
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
    req->rq_rpi->cq_env.ce_seq = req->rq_seq;
    req->rq_rpi->cq_env.ce_len = req->rq_packsize;

    if (req->rq_proc == lam_myproc) {
/*
 * send to self
 */
      return (send_to_self(req_top, req));
    } else {
/*
 * send to another process
 */
      req->rq_rpi->cq_state = C2CWRITE;
      req->rq_rpi->cq_msgbuf = req->rq_packbuf;

      if (req->rq_proc->p_rpi->cp_inbox != 0) {
/*
 * using shared memory
 */
	if (req->rq_packsize > lam_ssi_rpi_sysv_short) {
/*
 * long message protocol
 */
	  req->rq_rpi->cq_nmsgout = lam_ssi_rpi_sysv_short;
	  req->rq_rpi->cq_nenvout = sizeof(struct lam_ssi_rpi_envl);
	  req->rq_rpi->cq_env.ce_flags |= C2CLONG;
	  req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_long;

	} else {
/*
 * short message protocol
 */
	  req->rq_rpi->cq_nmsgout = req->rq_packsize;
	  req->rq_rpi->cq_nenvout = sizeof(struct lam_ssi_rpi_envl);

	  if (req->rq_type == LAM_RQISSEND) {
	    req->rq_rpi->cq_env.ce_flags |= C2CSSEND;
	    req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_synch;
	  } else
	    req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_short;
	}
      } else {
/*
 * using TCP
 */
	if (req->rq_packsize > lam_ssi_rpi_tcp_short) {
/*
 * long message protocol
 */
	  req->rq_rpi->cq_env.ce_flags |= C2CLONG;
	  req->rq_rpi->cq_adv = lam_ssi_rpi_tcp_req_send_long;

	} else {
/*
 * short message protocol
 */
	  req->rq_rpi->cq_nmsgout = req->rq_packsize;

	  if (req->rq_type == LAM_RQISSEND) {
	    req->rq_rpi->cq_env.ce_flags |= C2CSSEND;
	    req->rq_rpi->cq_adv = lam_ssi_rpi_tcp_req_send_synch;
	  } else
	    req->rq_rpi->cq_adv = lam_ssi_rpi_tcp_req_send_short;
	}

	lam_set_out_envelope_m(req->rq_rpi);
      }
    }
  }

  return (0);
}


/*
 *	lam_ssi_rpi_sysv_destroy
 *
 *	Function:	- destroys RPI portion of request
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_destroy(MPI_Request req)
{
  if (req->rq_rpi != 0)
    free(req->rq_rpi);
  req->rq_rpi = 0;
  return (0);
}


/*
 *	lam_ssi_rpi_sysv_advance
 *
 *	Function:	- advances requests in c2c mode
 *			- we try to advance all requests as far as possible
 *			  as allowed by RPI
 *	Accepts:	- request list
 *			- block enable flag
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_advance(MPI_Request req_top, int fl_block)
{
  MPI_Request req;		/* current request */
/*
 * Find which requests require IO.
 */
  FD_ZERO(&lam_ssi_rpi_tcp_read);
  FD_ZERO(&lam_ssi_rpi_tcp_write);
  FD_ZERO(&lam_ssi_rpi_tcp_except);
  FD_ZERO(&lam_ssi_rpi_tcp_eoferr);
  lam_ssi_rpi_tcp_flblock = fl_block;
  lam_ssi_rpi_tcp_haveadv = 0;
  lam_ssi_rpi_sysv_nwrite = 0;
  lam_ssi_rpi_sysv_nread = 0;
  lam_ssi_rpi_tcp_nio = 0;
  lam_ssi_rpi_tcp_sockmax = -1;

  for (req = req_top; req; req = req->rq_next) {
/*
 * Start requests that are in the init state.
 */
    if (req->rq_state == LAM_RQSINIT)
      if (_mpi_req_start(req) != MPI_SUCCESS)
	return (LAMERROR);
/*
 * If a blocking request is done we may no longer block.
 */
    if (req->rq_state == LAM_RQSDONE) {
      if (req->rq_flags & LAM_RQFBLOCK)
	lam_ssi_rpi_tcp_flblock = 0;
      continue;
    }

    if (lam_ssi_rpi_comm_dead_m(req)) {
      lam_ssi_rpi_tcp_haveadv = 1;
      continue;
    }

    if (req->rq_rpi->cq_state == C2CWRITE)
      add_write(req->rq_proc->p_rpi, req);
    else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0)
	add_read_any_src(req);
      else
	add_read(req->rq_proc->p_rpi, req);
    }
  }

  if (lam_ger && lam_ssi_rpi_tcp_flblock
      && (lam_ssi_rpi_sysv_nread + lam_ssi_rpi_sysv_nwrite +
	  lam_ssi_rpi_tcp_nio == 0)) {
    errno = EGERFLOW;
    return (LAMERROR);
  }

  return (lam_ssi_rpi_sysv_advance_common());
}


/*
 *	lam_ssi_rpi_sysv_iprobe
 *
 *	Function:	- non-blocking probe
 *			- public interface for peculiar MPI_Iprobe() which
 *			  does not return a request to the user
 *	Accepts:	- request
 *	Returns:	- 0: no msg, 1: msg, -1: error
 */
int
lam_ssi_rpi_sysv_iprobe(MPI_Request req)
{
  int err;			/* error code */
/*
 * Link the probe request and advance as far as possible.
 */
  _mpi_req_add_m(req);
  _mpi_req_blkclr_m();
  err = _mpi_req_advance();
  if (err != MPI_SUCCESS)
    return (-1);
/*
 * Unlink the request.
 */
  _mpi_req_rem_m(req);
/*
 * A message was found if the request is in the done state.
 */
  return ((req->rq_state == LAM_RQSDONE) ? 1 : 0);
}


/*
 *	lam_ssi_rpi_sysv_finalize
 *
 *	Function:	- c2c cleanup
 *	Accepts:	- process to cleanup (0 => all processes)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_finalize(struct _proc *p)
{
  if (p)
    return (finalize1(p));
  else {
/*
 * Cleanup shared memory amd locks.
 */
    if (lam_ssi_rpi_sysv_membase)
      if (lam_ssi_rpi_sysv_global_cleanup(lam_myproc->p_rpi))
	return LAMERROR;
/*
 * Loop through all processes closing connections.
 */
    while (lam_arr_size(lam_ssi_rpi_tcp_procs) > 0)
      if (finalize1(((struct _proc **) 
		     lam_arr_get(lam_ssi_rpi_tcp_procs))[0]))
	return LAMERROR;
    lam_arr_free(lam_ssi_rpi_tcp_procs);
    free(lam_ssi_rpi_sysv_read);
    free(lam_ssi_rpi_sysv_write);

    if (lam_ssi_verbose >= 0)
      lam_debug(lam_ssi_did, "sysv: module finalizing");
  }

  return 0;
}


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
  MPI_Request recv;		/* receive request */
  struct lam_ssi_rpi_cbuf_msg msg;	/* buffer list entry */
/*
 * Look for inactive matching receive/probe and advance if found.
 */
  for (recv = req_top; recv; recv = recv->rq_next)
    if ((recv->rq_state == LAM_RQSSTART) &&
	(recv->rq_rpi->cq_state == C2CREAD) &&
	!lam_ssi_rpi_envl_cmp(&send->rq_rpi->cq_env,
			      &recv->rq_rpi->cq_env))
      if (send_to_self_match(send, recv))
	return (0);
/*
 * No matching receive found, buffer the whole message and the send is
 * done unless its a synchronous send in which case we use the user
 * buffer and the send only completes once a matching receive is posted.  
 */
  msg.cm_env = send->rq_rpi->cq_env;
  msg.cm_proc = 0;
  msg.cm_dont_delete = 1;

  if (send->rq_type == LAM_RQISSEND) {
    send->rq_rpi->cq_state = C2CSENDSELF;
    msg.cm_buf = send->rq_packbuf;
    msg.cm_req = send;
  } else {
    if (send->rq_packsize > 0) {
      if ((msg.cm_buf = (char *) malloc(send->rq_packsize)) == 0)
	return (LAMERROR);
      msg.cm_dont_delete = 0;
      lam_memcpy(msg.cm_buf, send->rq_packbuf, send->rq_packsize);
    } else
      msg.cm_buf = 0;

    msg.cm_req = 0;
    send->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
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
  int len;			/* # bytes to transfer */

  recv->rq_seq = send->rq_seq;
  if (recv->rq_type == LAM_RQIPROBE) {
/*
 * The receive is actually a probe so the send is not complete.
 */
    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_env.ce_rank,
				send->rq_rpi->cq_env.ce_tag,
				send->rq_rpi->cq_env.ce_len);

    recv->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
    return (0);
  } else {
/*
 * It's really a receive. Do the data transfer.
 * Check for mismatched message lengths.
 */
    if (send->rq_packsize > recv->rq_packsize) {
      recv->rq_flags |= LAM_RQFTRUNC;
      len = recv->rq_packsize;
    } else
      len = send->rq_packsize;

    lam_memcpy(recv->rq_packbuf, send->rq_packbuf, len);

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_env.ce_rank,
				send->rq_rpi->cq_env.ce_tag, len);

    send->rq_state = recv->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
    return (1);
  }
}


/*
 *	add_write
 *
 *	Function:	- add process to write advance list
 *	Accepts:	- process
 *			- writing request
 */
static void
add_write(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  if (ps->cp_inbox != 0) {
/*
 * using shared memory
 */
    if (ps->cp_write)
      return;
/*
 * Associate request with process.
 */
    if (!ps->cp_wreq)
      ps->cp_wreq = req;

    ps->cp_write = 1;
    lam_ssi_rpi_sysv_write[lam_ssi_rpi_sysv_nwrite++] = ps;
  } else {
/*
 * using TCP
 */
    if (FD_ISSET(ps->cp_sock, &lam_ssi_rpi_tcp_write))
      return;
/*
 * Associate request with process.
 */
    if (!ps->cp_wreq)
      ps->cp_wreq = req;

    lam_ssi_rpi_tcp_nio++;
    lam_ssi_rpi_tcp_lastreq = req;
    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_write);
    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_except);

    if (ps->cp_sock > lam_ssi_rpi_tcp_sockmax)
      lam_ssi_rpi_tcp_sockmax = ps->cp_sock;
  }
}


/*
 *	add_read
 *
 *	Function:	- add process to read advance list
 *			- do not add in case process is self
 *	Accepts:	- process
 *			- request to start matching from
 */
static void
add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  if (lam_ger && ps->cp_nbfde >= MPI_GER)
    return;

  if (ps->cp_inbox != 0) {
/*
 * using shared memory
 */
    if (ps->cp_read)
      return;

    ps->cp_read = 1;
    ps->cp_mreq = req;
    lam_ssi_rpi_sysv_read[lam_ssi_rpi_sysv_nread++] = ps;
  } else if (ps->cp_sock >= 0) {
/*
 * using TCP
 */
    if (FD_ISSET(ps->cp_sock, &lam_ssi_rpi_tcp_read))
      return;

    lam_ssi_rpi_tcp_nio++;
    lam_ssi_rpi_tcp_lastreq = req;
    ps->cp_mreq = req;
    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_read);
    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_except);

    if (ps->cp_sock > lam_ssi_rpi_tcp_sockmax)
      lam_ssi_rpi_tcp_sockmax = ps->cp_sock;
  }
}


/*
 *	add_read_any_src
 *
 *	Function:	- add to the read advance list all processes in
 *			  the peer group of a receive request on MPI_ANY_SOURCE
 *	Accepts:	- request
 */
static void
add_read_any_src(MPI_Request req)
{
  struct _group *g;		/* peer group */
  struct _proc **p;
  int i;

  g = (LAM_IS_INTER(req->rq_comm))
    ? req->rq_comm->c_rgroup : req->rq_comm->c_group;

  for (i = g->g_nprocs, p = g->g_procs; i > 0; i--, p++) {

#if LAM_WANT_IMPI
/*
 * Special case for IMPI -- if we're receiving from the impid proxy
 * entry in the group, replace it with the One True IMPID Proc.
 */
    if (LAM_GPSCMP(&(*p)->p_gps, &gimpid) == 0 && lam_impid_proc != 0)
      add_read(lam_impid_proc->p_rpi, req);
    else
      add_read((*p)->p_rpi, req);
#else
    add_read((*p)->p_rpi, req);
#endif
  }
}


/*
 *	connect_all
 *
 *	Function:	- make tcp or shm connections to all other processes
 *	Returns:	- 0 or LAMERROR
 */
static int
connect_all(void)
{
  struct _proc *p;
  struct _proc **array;
  struct _proc *lowp = 0;	/* lowest proc. rank on node */
  struct _gps *mygps;		/* my GPS */
  struct nmsg inmsg;		/* incoming network msg hdr */
  struct nmsg outmsg;		/* outgoing network msg hdr */
  struct dolink *links;		/* links to neighbours */
  int4 nlinks;			/* number of links */
  int sock;			/* socket descriptor */
  int servsockd = -1;		/* server socket descriptor */
  int servport = 0;		/* server port number */
  int rnode;			/* remote node */
  int rport;			/* remote port */
  int flag;			/* for setting socket opts */
  int bufsize = 0;		/* c2c socket buffer size */
  int nshm = 0;			/* number shared mem procs */
  int mynshm = 0;		/* number shared mem procs */
  unsigned char *raddr;		/* remote host address */
  int i;
  struct in_addr translated_addr;

  mygps = &lam_myproc->p_gps;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);
  array = lam_arr_get(lam_ssi_rpi_tcp_procs);

  if (lam_arr_size(lam_ssi_rpi_tcp_procs) > 1) {
/*
 * Get links to neighbours, initialize server socket, message headers, etc.
 */
    if (ldogetlinks(&links, &nlinks))
      return (LAMERROR);

    servsockd = sfh_sock_open_srv_inet_stm(&servport, -1);
    if (servsockd < 0)
      return (LAMERROR);
/*
 * Initialize initialization message headers.
 */
    inmsg.nh_flags = 0;
    inmsg.nh_length = 0;
    outmsg.nh_flags = 0;
    outmsg.nh_length = 0;
    outmsg.nh_data[0] = (int4) servport;
    bufsize = tcp_sockbuf + sizeof(struct lam_ssi_rpi_envl);
  }
/*
 * If this is the first initialization (i.e. we are in MPI_Init() and no
 * communicators have been initialized) then determine if the process is
 * the lowest ranking process on the node and if there are more than one
 * processes on this node.  If so then initialize the shared memory
 * area.  Currently we do not use shared memory to communicate with
 * processes on the same node which are spawned or connected to via
 * accept/connect/join.	 Such processes have the LAM_PCLIENT mode bit
 * set.	 
 */
  if (lam_comms == 0) {

    /* First, count how many there will be and figure out who the low
       process is */

    for (i = 0; i < lam_arr_size(lam_ssi_rpi_tcp_procs); ++i) {
      p = array[i];
      if (p->p_gps.gps_node == mygps->gps_node && !(p->p_mode & LAM_PCLIENT)) {
	if (p == lam_myproc)
	  mynshm = nshm;
	++nshm;
	if (lowp == 0 || LAM_GPSCMP(&p->p_gps, &lowp->p_gps) < 0)
	  lowp = p;
      } 
    }

    /* Now have the low process go create the area */

    if (nshm > 1 && lowp == lam_myproc) {
      proc_init(lam_myproc);
      lam_myproc->p_rpi->cp_shmidx = mynshm;
      if (lam_ssi_rpi_sysv_create_area(nshm, lam_myproc->p_rpi, &outmsg))
	return (LAMERROR);
    }
  }
/*
 * Loop through all processes, initializing the process data and
 * connecting to those on remote nodes not already connected to.
 */
  nshm = 0;
  for (i = 0; i < lam_arr_size(lam_ssi_rpi_tcp_procs); ++i) {
    p = array[i];
    if (p->p_gps.gps_node == mygps->gps_node && !(p->p_mode & LAM_PCLIENT))
      ++nshm;

    if (p->p_mode & LAM_PRPIINIT)
      continue;

    proc_init(p);

    /* Can't set cp_shmidx until after proc_init (because of the
       malloc required to setup p->p_rpi), so had to defer setting it
       until now.  This is why we have all this bizarre logic about
       nshm here (nshm - 1, etc.). */

    p->p_rpi->cp_shmidx = (p->p_gps.gps_node == 
			   mygps->gps_node && !(p->p_mode & LAM_PCLIENT)) ?
      (nshm - 1) : -1;
      
    if (p == lam_myproc)
      continue;

    if (LAM_GPSCMP(mygps, &p->p_gps) >= 0) {
/*
 * Act as a client.
 */
      fill_sync(p, lam_myproc, &inmsg);
      if (nrecv(&inmsg))
	return (LAMERROR);

      rnode = p->p_gps.gps_node;
      if (rnode > nlinks)
	return (LAMERROR);
/*
 * If on the same node make a shared memory connection (except in a
 * accept/connect or spawn), otherwise make a TCP connection.  
 */
      if ((rnode != mygps->gps_node) || (p->p_mode & LAM_PCLIENT)) {

	rport = (int) inmsg.nh_data[0];

        /* See if we need to map this IP address to an alternate */

        if (lam_ssi_rpi_verbose >= 5) {
          lam_debug(lam_ssi_rpi_did, "sysv: creating outgoing socket to %s",
                    inet_ntoa(links[rnode].dol_addr.sin_addr));
        }
        translated_addr = 
          lam_ssi_base_hostmap(&links[rnode].dol_addr.sin_addr, "mpi");
	raddr = (unsigned char *) &translated_addr;

        /* Now open the socket */

	sock = sfh_sock_open_clt_inet_stm(raddr, rport);
	if (sock < 0)
	  return (LAMERROR);
      } else {
	if (lam_ssi_rpi_sysv_clientinit(p->p_rpi, &inmsg))
	  return (LAMERROR);

	lam_ssi_rpi_sysv_nprocs++;
	continue;
      }
    } else {
/*
 * Act as a server.
 *
 * If on the same node make a shared memory connection otherwise
 * make a TCP connection.
 */
      if (!(p->p_mode & LAM_PCLIENT)
	  && (p->p_gps.gps_node == mygps->gps_node)) {

	if (lam_ssi_rpi_sysv_serverinit(p->p_rpi, &outmsg))
	  return (LAMERROR);
	lam_ssi_rpi_sysv_nprocs++;
      }

      fill_sync(lam_myproc, p, &outmsg);
      if (nsend(&outmsg))
	return (LAMERROR);

      if ((p->p_mode & LAM_PCLIENT)
	  || (p->p_gps.gps_node != mygps->gps_node)) {

	sock = sfh_sock_accept_tmout(servsockd, -1);
	if (sock < 0)
	  return (LAMERROR);
      } else
	continue;
    }
/*
 * Only get here if it's a successful TCP connection.
 */
    p->p_rpi->cp_sock = sock;
    lam_ssi_rpi_tcp_smap[sock] = p->p_rpi;
/*
 * Set sockets in non-blocking mode and set the send and receive buffer sizes.
 */
    flag = 1;

#if LAM_HAVE_FIONBIO
    if (ioctl(sock, FIONBIO, &flag) == -1)
      return (LAMERROR);
#else
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
      return (LAMERROR);
#endif

    FD_CLR(sock, &lam_ssi_rpi_tcp_block);

    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
		   (char *) &flag, sizeof(flag)))
      return (LAMERROR);

    if (sfh_sock_set_buf_size(sock, SFH_INET, SO_SNDBUF, bufsize))
      return (LAMERROR);

    if (sfh_sock_set_buf_size(sock, SFH_INET, SO_RCVBUF, bufsize))
      return (LAMERROR);
  }
/*
 * The lowest shared memory process now sends information about the
 * shared memory area to all other processes on the same node which
 * are using shared memory.  Only do this during MPI_INIT -- when
 * lam_comms is 0.
 */
  if (lam_comms == 0 && nshm > 1) {
    if (lam_myproc == lowp) {
      for (i = 0; i < lam_arr_size(lam_ssi_rpi_tcp_procs); ++i) {
	p = array[i];
	if (p == lam_myproc)
	  continue;

	if (p->p_rpi->cp_shmidx >= 0) {
	  fill_sync(lam_myproc, p, &outmsg);
	  if (nsend(&outmsg))
	    return (LAMERROR);
	}
      }
    } else if (lam_myproc->p_rpi->cp_shmidx >= 0) {
      fill_sync(lowp, lam_myproc, &inmsg);
      if (nrecv(&inmsg))
	return (LAMERROR);

      if (lam_ssi_rpi_sysv_attach_area(nshm, lam_myproc->p_rpi, &inmsg))
	return (LAMERROR);
    }
/*
 * Initialize the shared memory in and out short message postboxes for each
 * peer process being communicated with via shared memory.
 */
    for (i = 0; i < lam_arr_size(lam_ssi_rpi_tcp_procs); ++i) {
      p = array[i];
      if (p == lam_myproc || p->p_rpi->cp_shmidx < 0)
	continue;

      p->p_rpi->cp_inbox = (lam_ssi_rpi_sysv_postbox_t *) 
	(lam_ssi_rpi_sysv_membase + CACHELINESIZE + SHMBOXSIZE *
	 (lam_myproc->p_rpi->cp_shmidx * nshm + p->p_rpi->cp_shmidx));
      p->p_rpi->cp_outbox = (lam_ssi_rpi_sysv_postbox_t *) 
	(lam_ssi_rpi_sysv_membase + CACHELINESIZE + SHMBOXSIZE * 
	 (p->p_rpi->cp_shmidx * nshm + lam_myproc->p_rpi->cp_shmidx));
    }
  }

  if (lam_arr_size(lam_ssi_rpi_tcp_procs) > 1) {
    close(servsockd);
    if (links != NULL)
      free(links);
  }
/*
 * Set up or enlarge the read/write arrays if necessary.
 */
  if (lam_ssi_rpi_sysv_nprocs > lam_ssi_rpi_sysv_narray) {
    if (lam_ssi_rpi_sysv_narray == 0) {
      lam_ssi_rpi_sysv_read = (struct lam_ssi_rpi_proc **)
	malloc(lam_ssi_rpi_sysv_nprocs *
	       sizeof(struct lam_ssi_rpi_proc *));
      lam_ssi_rpi_sysv_write = (struct lam_ssi_rpi_proc **)
	malloc(lam_ssi_rpi_sysv_nprocs *
	       sizeof(struct lam_ssi_rpi_proc *));
    } else {
      lam_ssi_rpi_sysv_read = (struct lam_ssi_rpi_proc **)
	realloc(lam_ssi_rpi_sysv_read,
		lam_ssi_rpi_sysv_nprocs *
		sizeof(struct lam_ssi_rpi_proc *));
      lam_ssi_rpi_sysv_write = (struct lam_ssi_rpi_proc **)
	realloc(lam_ssi_rpi_sysv_write,
		lam_ssi_rpi_sysv_nprocs *
		sizeof(struct lam_ssi_rpi_proc *));
    }

    if (lam_ssi_rpi_sysv_read == 0 || lam_ssi_rpi_sysv_write == 0)
      return (LAMERROR);
    lam_ssi_rpi_sysv_narray = lam_ssi_rpi_sysv_nprocs;
  }

  return (0);
}


/*
 *	proc_init
 *
 *	Function:	- initialize local specific process data
 *	Accepts:	- process
 */
static void
proc_init(struct _proc *p)
{
  p->p_mode |= LAM_PRPIINIT;

  p->p_rpi = malloc(sizeof(struct lam_ssi_rpi_proc));
  if (p->p_rpi == 0)
    return;

  p->p_rpi->cp_proc = p;
  p->p_rpi->cp_sock = -1;
  p->p_rpi->cp_mreq = 0;
  p->p_rpi->cp_inbox = 0;
  p->p_rpi->cp_outbox = 0;
  p->p_rpi->cp_locked = 0;
  p->p_rpi->cp_write = 0;
  p->p_rpi->cp_read = 0;
  p->p_rpi->cp_rreq = 0;
  p->p_rpi->cp_wreq = 0;
  p->p_rpi->cp_nbfde = 0;
  p->p_rpi->cp_extra = 0;
/*
 * Set up to read in an envelope.
 */
  p->p_rpi->cp_nenvin = sizeof(struct lam_ssi_rpi_envl);
  if (!(p->p_mode & LAM_PCLIENT)
      && (p->p_gps.gps_node == lam_myproc->p_gps.gps_node)) {
    p->p_rpi->cp_readfn = lam_ssi_rpi_sysv_proc_read_env;
  } else {
    p->p_rpi->cp_readfn = lam_ssi_rpi_tcp_proc_read_env;
    p->p_rpi->cp_envbuf = (char *) &p->p_rpi->cp_env;
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
/*
 * This uses in effect synchronization MPI_COMM_WORLD and tag 0.
 */
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

  lam_arr_remove(lam_ssi_rpi_tcp_procs, &p);

  /* Double check to ensure this proc still needs to be cleaned up */

  if (p->p_rpi != 0) {
    if (p->p_rpi->cp_sock >= 0) {
      shutdown(p->p_rpi->cp_sock, 2);
      close(p->p_rpi->cp_sock);
      p->p_rpi->cp_sock = -1;
    }
    
    if (p->p_rpi->cp_inbox) {
      if (lam_ssi_rpi_sysv_cleanup(p->p_rpi))
	return (LAMERROR);
      p->p_rpi->cp_inbox = 0;
    }
    
    free(p->p_rpi);
    p->p_rpi = 0;
  }

  return 0;
}


/*
 *	lam_ssi_rpi_sysv_fastsend
 *
 *	Function:	- fast blocking send
 *	Accepts:	- buffer to send
 *			- message count
 *			- message datatype
 *			- destination process rank
 *			- message tag
 *			- message communicator
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_sysv_fastsend(char *buf, int count, MPI_Datatype dtype,
			  int dest, int tag, MPI_Comm comm)
{
  double local_rep;		/* local data representation */
  double net_rep;		/* net data representation */
  struct _proc *destproc;
  char *packbuf;
  int packsize;
  int err;
/*
 * Check common arguments.
 */
  if (count < 0)
    return (lam_mkerr(MPI_ERR_COUNT, 0));

  if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit))
    return (lam_mkerr(MPI_ERR_TYPE, 0));

  if (LAM_IS_INTER(comm)) {
    if ((dest < 0) || (dest >= comm->c_rgroup->g_nprocs))
      return (lam_mkerr(MPI_ERR_RANK, 0));

    destproc = comm->c_rgroup->g_procs[dest];
  } else {
    if ((dest < 0) || (dest >= comm->c_group->g_nprocs))
      return (lam_mkerr(MPI_ERR_RANK, 0));

    destproc = comm->c_group->g_procs[dest];
  }
/*
 * Handle zero length messages.
 */
  if (count == 0 || dtype->dt_size == 0) {
    packbuf = buf;
    packsize = 0;
  } else {
    local_rep = 1.1;
    ltotf8(&local_rep, &net_rep);
/*
 * If contiguous, use the caller's buffer.
 */
    packsize = count * dtype->dt_size;

    if ((dtype->dt_flags & LAM_DTNOPACK)
	&& ((dtype->dt_flags & LAM_DTNOXADJ) || count == 1)
	&& ((local_rep == net_rep) || lam_homog)) {
      packbuf = buf;
/*
 * Check for bad buffer.
 */
      if (packbuf == 0)
	return (lam_mkerr(MPI_ERR_BUFFER, 0));
    }
/*
 * Otherwise allocate a buffer and pack the message into it.
 */
    else {
      packbuf = malloc(packsize);
      if (packbuf == 0)
	return (lam_mkerr(MPI_ERR_OTHER, errno));

      if (lam_pack(buf, count, dtype, packbuf, packsize) < 0)
	return (lam_mkerr(MPI_ERR_INTERN, errno));
    }
  }
/*
 * Call appropriate transport protocol.
 */
  if (destproc->p_rpi->cp_inbox != 0)
    err = lam_ssi_rpi_sysv_low_fastsend(packbuf, packsize,
					destproc->p_rpi, dest, tag, comm);
  else
    err = lam_ssi_rpi_tcp_low_fastsend(packbuf, packsize,
				       destproc->p_rpi, dest, tag, comm);

  if (packbuf != buf)
    free(packbuf);

  return (err);
}


/*
 *	lam_ssi_rpi_sysv_fastrecv
 *
 *	Function:	- fast blocking receive
 *	Accepts:	- buffer to receive into
 *			- message count
 *			- message datatype
 *			- source process rank
 *			- message tag (inout)
 *			- message communicator
 *			- status (out)
 *			- seqnum (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_sysv_fastrecv(char *buf, int count, MPI_Datatype dtype,
			  int src, int *tag, MPI_Comm comm,
			  MPI_Status * stat, int *seqnum)
{
  double local_rep;		/* local data representation */
  double net_rep;		/* net data representation */
  struct _proc *srcproc;
  char *packbuf;
  int packsize;
  int err;
/*
 * Check common arguments.
 */
  if (count < 0) {
    return (lam_mkerr(MPI_ERR_COUNT, 0));
  }

  if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit)) {
    return (lam_mkerr(MPI_ERR_TYPE, 0));
  }

  if (LAM_IS_INTER(comm)) {
    if ((src < 0) || (src >= comm->c_rgroup->g_nprocs)) {
      return (lam_mkerr(MPI_ERR_RANK, 0));
    }

    srcproc = comm->c_rgroup->g_procs[src];
  } else {
    if ((src < 0) || (src >= comm->c_group->g_nprocs)) {
      return (lam_mkerr(MPI_ERR_RANK, 0));
    }

    srcproc = comm->c_group->g_procs[src];
  }
/*
 * Handle zero length messages.
 */
  if (count == 0 || dtype->dt_size == 0) {
    packbuf = buf;
    packsize = 0;
  } else {
    local_rep = 1.1;
    ltotf8(&local_rep, &net_rep);
/*
 * If contiguous, use the caller's buffer.
 */
    packsize = count * dtype->dt_size;

    if ((dtype->dt_flags & LAM_DTNOPACK)
	&& ((dtype->dt_flags & LAM_DTNOXADJ) || count == 1)
	&& ((local_rep == net_rep) || lam_homog)) {
      packbuf = buf;
/*
 * Check for bad buffer.
 */
      if (packbuf == 0) {
	return (lam_mkerr(MPI_ERR_BUFFER, 0));
      }
    }
/*
 * Otherwise allocate a buffer.
 */
    else {
      packbuf = malloc(packsize);
      if (packbuf == 0) {
	return (lam_mkerr(MPI_ERR_OTHER, errno));
      }
    }
  }
/*
 * Call appropriate transport protocol.
 */
  if (srcproc->p_rpi->cp_sock < 0) {
    err = lam_ssi_rpi_sysv_low_fastrecv(packbuf, &packsize,
					srcproc->p_rpi, src, tag, comm,
					seqnum);
  } else {
    err = lam_ssi_rpi_tcp_low_fastrecv(packbuf, &packsize,
				       srcproc->p_rpi, src, tag, comm, seqnum);
  }

  if (stat != MPI_STATUS_IGNORE) {
    stat->MPI_SOURCE = src;
    stat->MPI_TAG = *tag;
    stat->MPI_ERROR = err;
    stat->st_length = packsize;
  }
/*
 * Unpack received message into user's buffer if necessary.
 */
  if (packbuf != buf) {
    if (lam_unpack(packbuf, packsize, buf, count, dtype) < 0) {
      return (lam_mkerr(MPI_ERR_INTERN, errno));
    }

    free(packbuf);
  }

  return (err);
}
