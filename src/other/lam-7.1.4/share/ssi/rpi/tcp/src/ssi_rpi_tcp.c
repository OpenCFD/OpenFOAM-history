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
 * $Id: ssi_rpi_tcp.c,v 1.37 2003/10/27 00:50:42 brbarret Exp $
 *
 *	Function:	- TCP client-to-client interface
 */

#include <lam_config.h>
#include <lam-ssi-rpi-tcp-config.h>

#include <sfh.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif

#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>		       /* LINUX FD_SET etc. */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(HAVE_SYS_RESOURCE_H) && HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#if defined(HAVE_NETINET_TCP_H) && HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#include <sys/uio.h>

#if LAM_HAVE_FIONBIO
#include <sys/ioctl.h>
#endif

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

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
#include <etc_misc.h>
#include <lamdebug.h>

#if LAM_WANT_IMPI
#include <impi.h>
#endif

#include <rpi_tcp.h>
#include <tcp_low.h>


/*
 * private functions
 */
static void add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static void add_write(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static void add_read_any_src(MPI_Request req);
static int send_to_self(MPI_Request req_top, MPI_Request send);
static int send_to_self_match(MPI_Request send, MPI_Request recv);
static int connect_all(void);
static void fill_sync(struct _proc *src, struct _proc *dest,
		      struct nmsg *hea);
static int proc_init(struct _proc *p);
static int finalize1(struct _proc *p);


/*
 * local variables
 */
static int param_short;
static int param_sockbuf;
static int param_priority;
static int tcp_sockbuf;
static const lam_ssi_rpi_actions_1_0_0_t module_actions = {

  /* RPI API functions */

  lam_ssi_rpi_tcp_addprocs,
  lam_ssi_rpi_tcp_finalize,

  lam_ssi_rpi_tcp_build,
  lam_ssi_rpi_tcp_start,
  lam_ssi_rpi_tcp_advance,
  lam_ssi_rpi_tcp_destroy,

  lam_ssi_rpi_tcp_iprobe,

  lam_ssi_rpi_tcp_fastrecv,
  lam_ssi_rpi_tcp_fastsend,

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
lam_ssi_rpi_tcp_open_module(OPT *ad)
{
  param_short = 
    lam_ssi_base_param_register_int("rpi", "tcp", "short", NULL,
                                    LAM_TCPSHORTMSGLEN);
  param_sockbuf = 
    lam_ssi_base_param_register_int("rpi", "tcp", "sockbuf", NULL, -1);
  param_priority = 
    lam_ssi_base_param_register_int("rpi", "tcp", "priority", NULL,
                                    LAM_SSI_RPI_TCP_DEFAULT ? 75 : 20);

  return 1;
}


/*
 * lam_ssi_rpi_tcp_query
 *
 * Function: determine if this module wants to run
 */
int
lam_ssi_rpi_tcp_query(int *priority, int *thread_min, int *thread_max)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return 0;

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "tcp: module initializing");
    lam_debug(lam_ssi_rpi_did, "tcp:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "tcp:priority: %d", *priority);
  }

  /* Return 1 saying that we want to be considered */

  return 1;
}


/*
 *	lam_ssi_rpi_tcp_init
 *
 *	Function:	- primary initialiation of RPI subsystem
 *			- initialize buffering and socket connections
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_rpi_actions_1_0_0_t *
lam_ssi_rpi_tcp_init(struct _proc **tcp_procs, int nprocs, 
		     int *maxtag, int *maxcid)
{
#if LAM_FD_SETSIZE
  struct rlimit rlp;
#endif

  /* Get tunable params from the environment */

  lam_ssi_rpi_tcp_short = 
    lam_ssi_base_param_lookup_int(param_short);
  tcp_sockbuf = lam_ssi_base_param_lookup_int(param_sockbuf);
  if (tcp_sockbuf < 0) {
    tcp_sockbuf = lam_ssi_rpi_tcp_short;
  }

  if (lam_ssi_rpi_verbose >= 0) {
    lam_debug(lam_ssi_rpi_did, "tcp: module initializing");
    lam_debug(lam_ssi_rpi_did, "tcp:short: %d bytes", 
	      lam_ssi_rpi_tcp_short);
    lam_debug(lam_ssi_rpi_did, "tcp:sockbuf: %d bytes", tcp_sockbuf);
  }

#if LAM_FD_SETSIZE
  /* increase fd limit, if so desired... We'll know right away if we need more
   * fds than we can make, so don't bother dealing with return values.*/
  rlp.rlim_cur = rlp.rlim_max = (rlim_t) LAM_FD_SETSIZE;
  if (setrlimit(RLIMIT_NOFILE, &rlp) ) {
    lam_debug(lam_ssi_rpi_did, "tcp: setrlimit failed.");
  }
#endif

  /* Set up all processes for client-to-client communication. */

  if (lam_ssi_rpi_tcp_addprocs(tcp_procs, nprocs))
    return NULL;

  return &module_actions;
}


/*
 *	lam_ssi_rpi_tcp_addprocs
 *
 *	Function:	- setup for new processes
 *			- makes socket connections
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_addprocs(struct _proc **procs, int nprocs)
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

  /* Now actually open sockets to them */
  
  return connect_all();
}


/*
 *	lam_ssi_rpi_tcp_build
 *
 *	Function:	- builds RPI portion of a request from scratch
 *			- one-time cost separated from lam_ssi_rpi_tcp_start()
 *			  to optimize persistent requests
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_build(MPI_Request req)
{
  req->rq_rpi = malloc(sizeof(struct lam_ssi_rpi_req));
  if (req->rq_rpi == 0)
    return LAMERROR;

  req->rq_rpi->cq_envbuf = (char *) &req->rq_rpi->cq_env;
  return 0;
}


/*
 *	lam_ssi_rpi_tcp_start
 *
 *	Function:	- initializes RPI dependent aspects of a request
 *			- cost per request start - separated from
 *			  lam_ssi_rpi_tcp_build() to optimize persistent requests
 *	Accepts:	- request list
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_cbuf_msg *bmsg;	/* buffered message */

  if (lam_ssi_rpi_comm_dead_m(req)) {
    lam_ssi_rpi_tcp_haveadv = 1;
    return (0);
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
     * Check for matching buffered envelope/message. If one is found fill
     * in any receive request wildcards and advance the request.  
     */
    if ((bmsg = lam_ssi_rpi_cbuf_find(&req->rq_rpi->cq_env))) {
      lam_ssi_rpi_tcp_fill_wildcards(req, &bmsg->cm_env);
      return (lam_ssi_rpi_tcp_buffered_adv(req, bmsg));
    }
    /*
     * No match was found. Set the request protocol transition function.
     */
    req->rq_rpi->cq_adv =
      (req->rq_type == LAM_RQIRECV) ?
      lam_ssi_rpi_tcp_req_recv : lam_ssi_rpi_tcp_req_probe;
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
	} else {
	  req->rq_rpi->cq_adv = lam_ssi_rpi_tcp_req_send_short;
	}
      }
      /*
       * prepare for writing of envelope
       */
      lam_set_out_envelope_m(req->rq_rpi);
    }
  }

  return (0);
}


/*
 *	lam_ssi_rpi_tcp_destroy
 *
 *	Function:	- destroys RPI portion of request
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_destroy(MPI_Request req)
{
  if (req->rq_rpi != 0)
    free(req->rq_rpi);
  req->rq_rpi = 0;
  return (0);
}


/*
 *	lam_ssi_rpi_tcp_advance
 *
 *	Function:	- advances requests in c2c mode
 *			- we try to advance all requests as far as possible
 *			  as allowed by RPI
 *	Accepts:	- request list
 *			- block enable flag
 *	Returns:	- 1: state change, 0: no state change, LAMERROR: error
 */
int
lam_ssi_rpi_tcp_advance(MPI_Request req_top, int fl_block)
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
  lam_ssi_rpi_tcp_nio = 0;
  lam_ssi_rpi_tcp_sockmax = -1;

  for (req = req_top; req; req = req->rq_next) {
    /*
     * Start requests that are in the init state.
     */
    if (req->rq_state == LAM_RQSINIT) {
      if (_mpi_req_start(req) != MPI_SUCCESS) {
	return (LAMERROR);
      }
    }
    /*
     * If a blocking request is done we may no longer block.
     */
    if (req->rq_state == LAM_RQSDONE) {
      if (req->rq_flags & LAM_RQFBLOCK) {
	lam_ssi_rpi_tcp_flblock = 0;
      }
      continue;
    }

    if (lam_ssi_rpi_comm_dead_m(req)) {
      lam_ssi_rpi_tcp_haveadv = 1;
      continue;
    }

    if (req->rq_rpi->cq_state == C2CWRITE) {
      add_write(req->rq_proc->p_rpi, req);
    } else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0) {
	add_read_any_src(req);
      } else {
	add_read(req->rq_proc->p_rpi, req);
      }
    }
  }

  if (lam_ssi_rpi_tcp_nio >= 1) {
    do {
      if (lam_ssi_rpi_tcp_nio == 1) {
	if (lam_ssi_rpi_tcp_adv1())
	  return (LAMERROR);
      } else {
	if (lam_ssi_rpi_tcp_advmultiple())
	  return (LAMERROR);
      }
    } while (lam_ssi_rpi_tcp_flblock && !lam_ssi_rpi_tcp_haveadv);
  } else if (lam_ger && lam_ssi_rpi_tcp_flblock) {
    errno = EGERFLOW;
    return (LAMERROR);
  }

  return (lam_ssi_rpi_tcp_haveadv);
}


/*
 *      lam_ssi_rpi_tcp_iprobe
 *
 *      Function:       - non-blocking probe
 *                      - public interface for peculiar MPI_Iprobe() which
 *                        does not return a request to the user
 *      Accepts:        - request
 *      Returns:        - 0: no msg, 1: msg, LAMERROR: error
 */
int
lam_ssi_rpi_tcp_iprobe(MPI_Request req)
{
  int err;			/* error code */
  /*
   * Link the probe request and advance as far as possible.
   */
  _mpi_req_add(req);
  _mpi_req_blkclr();
  err = _mpi_req_advance();
  if (err != MPI_SUCCESS)
    return (LAMERROR);
  /*
   * Unlink the request.
   */
  _mpi_req_rem(req);
  /*
   * A message was found if the request is in the done state.
   */
  return ((req->rq_state == LAM_RQSDONE) ? 1 : 0);
}


/*
 *	lam_ssi_rpi_tcp_finalize
 *
 *	Function:	- c2c cleanup
 *	Accepts:	- process to cleanup (0 => all processes)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_finalize(struct _proc *p)
{
  /* Note that it is possible for finalize1(p) to be called *before*
     MPI_Finalize.  If we add some dynamic processes (e.g., spawn),
     and then later MPI_Comm_free all communicators with those
     processes in them, finalize() will be called with each process
     that is no longer referenced. */

  if (p != NULL)
    return (finalize1(p));
  else {

    /* Loop through all processes closing connections. */

    while (lam_arr_size(PUB(procs)) > 0)
      if (finalize1(((struct _proc **) lam_arr_get(PUB(procs)))[0]))
	return (LAMERROR);
    lam_arr_free(PUB(procs));

    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(lam_ssi_rpi_did, "tcp: module finalizing");
  }

  return (0);
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
  for (recv = req_top; recv; recv = recv->rq_next) {

    if ((recv->rq_state == LAM_RQSSTART)
	&& (recv->rq_rpi->cq_state == C2CREAD)
	&& (!lam_ssi_rpi_envl_cmp(&send->rq_rpi->cq_env, 
				  &recv->rq_rpi->cq_env))) {
      if (send_to_self_match(send, recv)) {
	return (0);
      }
    }
  }
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
     *
     * Check for mismatched message lengths.
     */
    if (send->rq_packsize > recv->rq_packsize) {
      recv->rq_flags |= LAM_RQFTRUNC;
      len = recv->rq_packsize;
    } else {
      len = send->rq_packsize;
    }

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
  /*
   * Already added?
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
  if (lam_ger && ps->cp_proc->p_num_buf_env >= MPI_GER)
    return;

  if (ps->cp_sock >= 0) {
    /*
     * Already added?
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
 *	Function:	- make tcp connections to all other processes
 *	Returns:	- 0 or LAMERROR
 */
static int
connect_all(void)
{
  struct _proc *p;
  struct _proc **array;
  struct _gps *mygps;		/* my GPS */
  struct nmsg inmsg;		/* incoming network msg hdr */
  struct nmsg outmsg;		/* outgoing network msg hdr */
  struct dolink *links;		/* links to neighbours */
  int4 nlinks;			/* number of links */
  int sock;			/* socket descriptor */
  int servsockd;		/* server socket descriptor */
  int servport = 0;		/* server port number */
  int rnode;			/* remote node */
  int rport;			/* remote port */
  int flag;			/* for setting socket opts */
  int bufsize;			/* c2c socket buffer size */
  unsigned char *raddr;		/* remote host address */
  char myrankstr[32];
  int i;
  struct in_addr translated_addr;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);

  /* Things to make -Wall not complain */

  mygps = &lam_myproc->p_gps;
  memset(myrankstr, 0, 32);
  snprintf(myrankstr, 31, "%d", mygps->gps_grank);
  bufsize = tcp_sockbuf + sizeof(struct lam_ssi_rpi_envl);
  servsockd = -1;

  if (lam_arr_size(PUB(procs)) > 1) {

    /* Get links to neighbours, initialize server socket, message
       headers, etc. */

    if (ldogetlinks(&links, &nlinks))
      return (LAMERROR);

    servsockd = sfh_sock_open_srv_inet_stm(&servport, -1);
    if (servsockd < 0) {
      show_help("sfh", "open-server-socket", myrankstr, NULL);
      return (LAMERROR);
    }

    inmsg.nh_flags = DINT4DATA;
    inmsg.nh_length = 0;
    outmsg.nh_flags = DINT4DATA;
    outmsg.nh_data[0] = (int4) servport;
    outmsg.nh_length = 0;
  }

  /* Loop through all processes, initializing the process data and
     connecting to those not already connected to. */

  array = lam_arr_get(PUB(procs));
  for (i = 0; i < lam_arr_size(PUB(procs)); ++i) {
    p = array[i];
    if (p->p_mode & LAM_PRPIINIT)
      continue;
    if (proc_init(p) != 0)
      return LAMERROR;

    if (p != lam_myproc) {

      if (LAM_GPSCMP(mygps, &p->p_gps) >= 0) {

	/* Act as a client. */

	fill_sync(p, lam_myproc, &inmsg);
	if (nrecv(&inmsg))
	  return (LAMERROR);

	rport = (int) inmsg.nh_data[0];
	rnode = p->p_gps.gps_node;

	if (rnode > nlinks)
	  return (LAMERROR);

        /* See if we need to map this IP address to an alternate */

        if (lam_ssi_rpi_verbose >= 5) {
          lam_debug(lam_ssi_rpi_did, "tcp: creating outgoing socket to %s",
                    inet_ntoa(links[rnode].dol_addr.sin_addr));
        }
        translated_addr = 
          lam_ssi_base_hostmap(&links[rnode].dol_addr.sin_addr, "mpi");
	raddr = (unsigned char *) &translated_addr;

        /* Now open the socket */

	sock = sfh_sock_open_clt_inet_stm(raddr, rport);
	if (sock < 0) {
	  int save = errno;
	  if (servsockd > 0)
	    close(servsockd);
	  errno = save;
	  show_help("sfh", "open-client-socket", myrankstr, NULL);
	  return (LAMERROR);
	}
      } else {

	/* Act as a server. */
	fill_sync(lam_myproc, p, &outmsg);
	if (nsend(&outmsg))
	  return (LAMERROR);

	sock = sfh_sock_accept_tmout(servsockd, -1);
	if (sock < 0) {
	  int save = errno;
	  if (servsockd > 0)
	    close(servsockd);
	  errno = save;
	  show_help("sfh", "accept-server-socket", myrankstr, NULL);
	  return (LAMERROR);
	}
      }

      p->p_rpi->cp_sock = sock;
      lam_ssi_rpi_tcp_smap[sock] = p->p_rpi;

      /* Set sockets in non-blocking mode and set the send and receive
	 buffer sizes. */

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
		     (char *) &flag, sizeof(flag))) {
	return (LAMERROR);
      }

      if (sfh_sock_set_buf_size(sock, SFH_INET, SO_SNDBUF, bufsize)) {
	return (LAMERROR);
      }

      if (sfh_sock_set_buf_size(sock, SFH_INET, SO_RCVBUF, bufsize)) {
	return (LAMERROR);
      }
    }
  }

  if (lam_arr_size(PUB(procs)) > 1) {
    close(servsockd);
    if (links != 0)
      free(links);
  }

  return 0;
}


/*
 *	proc_init
 *
 *	Function:	- initialize c2c specific process data
 *	Accepts:	- process
 */
static int
proc_init(struct _proc *p)
{
  p->p_mode |= LAM_PRPIINIT;

  /* Initialize the p_rpi pointer */

  p->p_rpi = malloc(sizeof(struct lam_ssi_rpi_proc));
  if (p->p_rpi == 0)
    return LAMERROR;

  p->p_rpi->cp_proc = p;
  p->p_rpi->cp_sock = -1;
  p->p_rpi->cp_mreq = 0;
  p->p_rpi->cp_rreq = 0;
  p->p_rpi->cp_wreq = 0;
  p->p_rpi->cp_extra = 0;

  /* Set up to read in an envelope */

  p->p_rpi->cp_readfn = lam_ssi_rpi_tcp_proc_read_env;
  p->p_rpi->cp_envbuf = (char *) &p->p_rpi->cp_env;
  p->p_rpi->cp_nenvin = sizeof(struct lam_ssi_rpi_envl);

  return 0;
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
  /* This uses in effect synchronization MPI_COMM_WORLD and tag 0. */

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
    if (p->p_rpi->cp_sock >= 0) {
      shutdown(p->p_rpi->cp_sock, 2);
      close(p->p_rpi->cp_sock);
      p->p_rpi->cp_sock = -1;
    }

    free(p->p_rpi);
    p->p_rpi = NULL;
  }

  return 0;
}


/*
 *	lam_ssi_rpi_tcp_fastsend
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
lam_ssi_rpi_tcp_fastsend(char *buf, int count, MPI_Datatype dtype,
			 int dest, int tag, MPI_Comm comm)
{
  double local_rep;		/* local data representation */
  double net_rep;		/* net data representation */
  struct _proc *destproc;
  char *packbuf;
  int packsize;
  int err;

  /* Check common arguments. */
  if (count < 0)
    return (lam_mkerr(MPI_ERR_COUNT, 0));

  if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit)) {
    return (lam_mkerr(MPI_ERR_TYPE, 0));
  }

  if (LAM_IS_INTER(comm)) {
    if ((dest < 0) || (dest >= comm->c_rgroup->g_nprocs)) {
      return (lam_mkerr(MPI_ERR_RANK, 0));
    }

    destproc = comm->c_rgroup->g_procs[dest];
  } else {
    if ((dest < 0) || (dest >= comm->c_group->g_nprocs)) {
      return (lam_mkerr(MPI_ERR_RANK, 0));
    }

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

    /* If contiguous, use the caller's buffer. */

    packsize = count * dtype->dt_size;
    if ((dtype->dt_flags & LAM_DTNOPACK) &&
	((dtype->dt_flags & LAM_DTNOXADJ) || count == 1) &&
        ((local_rep == net_rep) || lam_homog)) {
      packbuf = buf;

      /* Check for bad buffer. */

      if (packbuf == 0) {
	return (lam_mkerr(MPI_ERR_BUFFER, 0));
      }
    }

    /* Otherwise allocate a buffer. */

    else {
      packbuf = malloc(packsize);
      if (packbuf == 0)
	return (lam_mkerr(MPI_ERR_OTHER, errno));

      if (lam_pack(buf, count, dtype, packbuf, packsize) < 0)
	return (lam_mkerr(MPI_ERR_INTERN, errno));
    }
  }

  err = lam_ssi_rpi_tcp_low_fastsend(packbuf, packsize,
				     destproc->p_rpi, dest, tag, comm);

  if (packbuf != buf) {
    free(packbuf);
  }

  return (err);
}


/*
 *	lam_ssi_rpi_tcp_fastrecv
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
lam_ssi_rpi_tcp_fastrecv(char *buf, int count, MPI_Datatype dtype,
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

  err = lam_ssi_rpi_tcp_low_fastrecv(packbuf, &packsize, srcproc->p_rpi,
				     src, tag, comm, seqnum);

  if (stat != MPI_STATUS_IGNORE) {
    stat->MPI_SOURCE = src;
    stat->MPI_TAG = *tag;
    stat->MPI_ERROR = err;
    stat->st_length = packsize;
  }

  if (packbuf != buf) {
    if (lam_unpack(packbuf, packsize, buf, count, dtype) < 0) {
      return (lam_mkerr(MPI_ERR_INTERN, errno));
    }

    free(packbuf);
  }

  return (err);
}
