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
 * $Id: ssi_rpi_crtcp.c,v 1.68 2003/12/31 16:32:33 jsquyres Exp $
 *
 *    Function:       - CRTCP client-to-client interface
 */

#include <lam_config.h>
#include <lam-ssi-rpi-crtcp-config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>                /* LINUX FD_SET etc. */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
#include <lammpithreads.h>
#include <all_array.h>
#include <sfh.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif

#include <rpi_crtcp.h>
#include <crtcp_low.h>
#include <priority.h>
#include <lam_network.h>
#include <lam-ssi-rpi-crtcp.h>
#include <lam-ssi-cr.h>


/*
 * global variables 
 */
struct _proc **PUB(procs) = NULL;
int PUB(nprocs) = 0;
int PUB(verbose) = -1;
int PUB(did) = -1;
int PUB(full_sockmax) = -1;
fd_set PUB(full_fd_set);


/*
 * local variables
 */
static int param_short;
static int param_priority;
static int param_sockbuf;
static int crtcp_sockbuf;
static lam_array_t *lprocs = NULL;
static lam_ssi_rpi_actions_1_1_0_t module_actions = {

  /* RPI API functions */

  lam_ssi_rpi_crtcp_addprocs,
  lam_ssi_rpi_crtcp_finalize,

  lam_ssi_rpi_crtcp_build,
  lam_ssi_rpi_crtcp_start,
  lam_ssi_rpi_crtcp_advance,
  lam_ssi_rpi_crtcp_destroy,

  lam_ssi_rpi_crtcp_iprobe,

#if CRTCP_DO_FAST
  lam_ssi_rpi_crtcp_fastrecv,
  lam_ssi_rpi_crtcp_fastsend,
#else
  NULL,
  NULL,
#endif

  lam_ssi_rpi_base_alloc_mem,
  lam_ssi_rpi_base_free_mem,

  lam_ssi_rpi_crtcp_interrupt,
  lam_ssi_rpi_crtcp_checkpoint,
  lam_ssi_rpi_crtcp_continue,
  lam_ssi_rpi_crtcp_restart,

  /* Flags */
  /* Totalview support */

  1
};


/*
 * private functions
 */
static void add_write(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int send_to_self(MPI_Request req_top, MPI_Request send);
static int send_to_self_match(MPI_Request send, MPI_Request recv);
static int connect_all(void);
static void fill_sync(struct _proc *src, struct _proc *dest,
                      struct nmsg *hea);
static int proc_init(struct _proc *p);
static int finalize1(struct _proc *p);
#if CRTCP_DO_FAST
static void yield_return(void);
#endif


/*
 * Open the module and register an SSI parameter
 */
int
lam_ssi_rpi_crtcp_open_module(OPT *ad)
{
  param_priority = 
    lam_ssi_base_param_register_int("rpi", "crtcp", "priority", NULL,
                                    LAM_SSI_RPI_CRTCP_DEFAULT ? 75 : 25);
  param_short = 
    lam_ssi_base_param_register_int("rpi", "crtcp", "short", NULL,
                                    LAM_CRTCPSHORTMSGLEN);
  param_short = 
    lam_ssi_base_param_register_int("rpi", "crtcp", "sockbuf", NULL, -1);

  return 1;
}


/*
 * lam_ssi_rpi_crtcp_query
 *
 * Function: determine if this module wants to run
 */
int
lam_ssi_rpi_crtcp_query(int *priority, int *thread_min, int *thread_max)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return 0;

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "crtcp: module initializing");
    lam_debug(lam_ssi_rpi_did, "crtcp:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "crtcp:priority: %d", *priority);
  }

  /* Return 1 to say that we want to be considered. */

  return 1;
}


/*
 *    lam_ssi_rpi_crtcp_init
 *
 *    Function:       - primary initialiation of RPI subsystem
 *                    - initialize buffering and socket connections
 *    Returns         - 0 or LAMERROR
 */
const lam_ssi_rpi_actions_1_1_0_t *
lam_ssi_rpi_crtcp_init(struct _proc **crtcp_procs, int nprocs, 
		       int *max_tag, int *max_cid)
{
#if LAM_WANT_DEBUG
  char prefix[256];
  char suffix[256];
  lam_debug_stream_info_t lds = {0, 0, 0, NULL, NULL, 0, 1, 1, 0, NULL};
#endif

  /* Get tunable params from the environment */

  PUB(short) = lam_ssi_base_param_lookup_int(param_short);
  crtcp_sockbuf = lam_ssi_base_param_lookup_int(param_sockbuf);
  if (crtcp_sockbuf < 0) {
    crtcp_sockbuf = PUB(short);
  }

  if (lam_ssi_verbose >= 0) {
    lam_debug(lam_ssi_did, "crtcp RPI module initializing");
    lam_debug(lam_ssi_did, "rpi:crtcp:short: %d", PUB(short));
    lam_debug(lam_ssi_did, "rpi:crtcp:sockbuf: %d", crtcp_sockbuf);
    lam_debug(lam_ssi_did, "rpi:crtcp:verbose: %d", PUB(verbose));
  }

#if LAM_WANT_DEBUG
  /* gimme a debug stream, just for me */
  snprintf(prefix, sizeof(prefix), "<n%d/%d/%d>crtcp: ",
           getnodeid(), lam_getpid(), lam_myproc->p_gps.gps_grank);
  snprintf(suffix, sizeof(suffix), "crtcp-rank-%d.txt", 
           lam_myproc->p_gps.gps_grank);

  lds.lds_prefix = prefix;
  lds.lds_fl_file_append = 0;
  lds.lds_file_suffix = suffix;
  PUB(did) = lam_debug_open(&lds);
#else
  PUB(did) = -1;
#endif
  PUB(verbose) = lam_ssi_rpi_verbose;

  if (PUB(verbose) >= 10)
    lam_debug(PUB(did), "in crtcp init function");

  /* Set up all processes for client-to-client communication. */
  if (lam_ssi_rpi_crtcp_addprocs(crtcp_procs, nprocs)) {
    if (PUB(verbose) >= 50)
      lam_debug(PUB(did), "ASYNC%d: addprocs failed", getpid());
    return NULL;
  }

  /* All done */

  return &module_actions;
}


/*
 *    lam_ssi_rpi_crtcp_addprocs
 *
 *    Function:       - setup for new processes
 *                    - makes socket connections
 *    Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_addprocs(struct _proc **crtcp_procs, int nprocs)
{
  int i, added = 0;

  /* Initialize the local procs array if we need to */

  if (lprocs == 0)
    lprocs = lam_arr_init(sizeof(struct _proc *), NULL);

  /* Add these procs into local procs array.  Make sure that
     they're not already in there first. */

  for (i = 0; i < nprocs; ++i)
    if (lam_arr_find(lprocs, &crtcp_procs[i]) == LAMERROR) {
      lam_arr_append(lprocs, &crtcp_procs[i]);
      added = 1;
    }

  /* Ensure that the list is sorted so that things like connect_all()
     will all do things in the same order in all connected processes */

  if (added == 1)
    lam_procsort(lam_arr_get(lprocs), lam_arr_size(lprocs));

  /* Assign PUB(nprocs) and PUB(procs) accordingly, because that's
     what we use throughout the rest of crtcp */

  PUB(nprocs) = lam_arr_size(lprocs);
  PUB(procs) = lam_arr_get(lprocs);

  /* connect up all the processes */
  if (connect_all() != 0) {
    if (PUB(verbose) >= 50)
      lam_debug(PUB(did), "connect_all() failed");
    return LAMERROR;
  }

  FD_ZERO(&PUB(full_fd_set));
  PUB(full_sockmax) = -1;

  for (i = 0; i < nprocs; ++i) {
    if (PUB(procs)[i] != lam_myproc) {
      FD_SET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(full_fd_set));
      if ((PUB(procs)[i])->p_rpi->cp_sock > PUB(full_sockmax))
        PUB(full_sockmax) = (PUB(procs)[i])->p_rpi->cp_sock;
    }
  }

  return 0;
}


/*
 *    lam_ssi_rpi_crtcp_build
 *
 *    Function:       - builds RPI portion of a request from scratch
 *                    - one-time cost separated from lam_ssi_rpi_crtcp_start()
 *                      to optimize persistent requests
 *    Accepts:        - request
 *    Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_build(MPI_Request req)
{
  req->rq_rpi = malloc(sizeof(struct lam_ssi_rpi_req));
  if (req->rq_rpi == 0)
    return LAMERROR;

  req->rq_rpi->cq_envbuf = (char *) &req->rq_rpi->cq_env;
  return 0;
}


/*
 *    lam_ssi_rpi_crtcp_start
 *
 *    Function:       - initializes RPI dependent aspects of a request
 *                    - cost per request start - separated from _crtcp_build()
 *                      to optimize persistent requests
 *    Accepts:        - request list
 *                    - request
 *    Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_cbuf_msg *bmsg;  /* buffered message */

  if (lam_ssi_rpi_comm_dead_m(req)) {
    PUB(haveadv) = 1;
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
     * Check for matching buffered envelope/message. If one is found fill
     * in any receive request wildcards and advance the request.  
     */
    if ((bmsg = lam_ssi_rpi_cbuf_find(&req->rq_rpi->cq_env))) {
      lam_ssi_rpi_crtcp_fill_wildcards(req, &bmsg->cm_env);
      return (lam_ssi_rpi_crtcp_buffered_adv(req, bmsg));
    }
    /*
     * No match was found. Set the request protocol transition function.
     */
    req->rq_rpi->cq_adv =
      (req->rq_type == LAM_RQIRECV) ?
      lam_ssi_rpi_crtcp_req_recv : lam_ssi_rpi_crtcp_req_probe;
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

      if (req->rq_packsize > lam_ssi_rpi_crtcp_short) {
        /*
         * long message protocol
         */
        req->rq_rpi->cq_env.ce_flags |= C2CLONG;
        req->rq_rpi->cq_adv = lam_ssi_rpi_crtcp_req_send_long;

      } else {
        /*
         * short message protocol
         */
        req->rq_rpi->cq_nmsgout = req->rq_packsize;

        if (req->rq_type == LAM_RQISSEND) {
          req->rq_rpi->cq_env.ce_flags |= C2CSSEND;
          req->rq_rpi->cq_adv = lam_ssi_rpi_crtcp_req_send_synch;
        } else {
          req->rq_rpi->cq_adv = lam_ssi_rpi_crtcp_req_send_short;
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
 *    lam_ssi_rpi_crtcp_destroy
 *
 *    Function:       - destroys RPI portion of request
 *    Accepts:        - request
 *    Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_destroy(MPI_Request req)
{
  if (req->rq_rpi != 0)
    free(req->rq_rpi);
  req->rq_rpi = 0;
  return (0);
}


/*
 *    lam_ssi_rpi_crtcp_advance
 *
 *    Function:       - advances requests in c2c mode
 *                    - we try to advance all requests as far as possible
 *                      as allowed by RPI
 *    Accepts:        - request list
 *                    - block enable flag
 *    Returns:        - 1: state change, 0: no state change, LAMERROR: error
 */
int
lam_ssi_rpi_crtcp_advance(MPI_Request req_top, int fl_block)
{
  MPI_Request req;
  int deferred_writes = 0;

  /* Find which requests require IO. */

  FD_ZERO(&PUB(read));
  FD_ZERO(&PUB(write));
  FD_ZERO(&PUB(except));
  FD_ZERO(&PUB(eoferr));
  PUB(flblock) = fl_block;
  PUB(haveadv) = 0;
  PUB(nio) = 0;
  PUB(sockmax) = -1;

  for (req = req_top; req; req = req->rq_next) {

    /* Start requests that are in the init state. */

    if (req->rq_state == LAM_RQSINIT) {
      if (_mpi_req_start(req) != MPI_SUCCESS) {
        return (LAMERROR);
      }
    }

    /* If a blocking request is done we may no longer block. */

    if (req->rq_state == LAM_RQSDONE) {
      if (req->rq_flags & LAM_RQFBLOCK) {
        PUB(flblock) = 0;
      }
      continue;
    }

    if (lam_ssi_rpi_comm_dead_m(req)) {
      PUB(haveadv) = 1;
      continue;
    }

    /* Sidenote: if we're in the middle of a quiesce, then we ignore
       all writes because they've been bookmarked already.  Simply by
       ignoring add_write() here, we cover all possible cases:

       - we won't hit adv1() due solely to a write because _nio won't
       be incremented for writes (i.e., add_write() does the _nio
       increment).  Hence, if we end up calling adv1(), it will
       definitely only be for a read request -- not a write request.

       - if we call advmultiple(), there won't be any fd's set in the
       write fd_set because we never called add_write() -- so only
       read requests will progress. 

       Corner case: if there are pending blocking writes and no
       pending reads (blocking or non-blocking), mpi_advance() is
       expecting progress to be made in this function.  Hence, if
       we've received a checkpoint *before* we hit this point (i.e.,
       handler_state is not STATE_IDLE and we therefore don't call
       add_write(), then we could fall through this loop with _nio ==
       0.  Since _nio == 0, we won't call adv1() or advmultiple(), so
       we'll return haveadv == 0, and mpi_advance() will simply loop
       and call us again (since it wants/needs progress, since there
       are pending blocking writes).

       SO: if there are pending blocking writes and no pending reads
       (i.e., _nio == 0 and fl_block == 1), and handler_state ==
       STATE_WAITING, we need to yeild to the CR handler.

       Second corner case: if there are both pending blocking reads
       and blocking writes, and all the pending writes get deferred,
       we could end up in a deadlock situation -- the reads will be
       expected to make progress, but the corresponding writes haven't
       occurred yet.  The following pseudocode tends to trip this
       condition:

       # comm has a size of 4 processes
       if (rank % 2 != 0)
         sleep(10); # crtcp will kill()/interrupt this
       MPI_Alltoall(...);

       What happens here (at least for crtcp) is that procs 0 and 2
       will go into the MPI_Alltoall (assuming the lam_basic coll
       module) right away and eagerly send envelopes to all peers.
       Procs 1 and 3 will enter the sleep().  The crtcp rpi module
       uses a kill() to interrupt blocking reads, but the cr thread
       has no knowledge of whether the app thread is down in the MPI
       layer or in the main user program.  Hence, the cr thread will
       invoke kill() on all 4 processes, regardless of where they are.
       Procs 0 and 2 are already within MPI, possibly down within the
       RPI.  Thet will be ok -- normal c/r mechanisms here in crtcp
       will handle them.

       But procs 1 and 3 will drop out of the sleep() because it was
       interrupted, but there's a race condition -- they may enter
       MPI_Alltoall before the cr thread manages to lock the MPI
       library.  Hence, they get all the way down here into the RPI
       before they realize that there's a cr pending.  So they defer
       all writes but try to advance all reads (remember that alltoall
       will have pending reads and writes from all 4 procs).  Procs 1
       and 3 can advance the reads for 0 and 2 (because the envelopes
       were eagerly sent) and from themselves (send-to-self match).
       But the receive from [1,3] will now be the last pending read
       (remember: all writes were deferred), and it's a blocking read,
       so we'll go into adv1() and do a blocking read() from the peer.

       This will block indefinitely because the matching write has
       never occurred.  Hence, deadlock.

       So to avoid this problem, if we ever have deferred writes, set
       PUB(flblock) to 0 so that we just do one pass through any
       pending reads and then return out of here.  We may return with
       (haveadv == 0), which will be ok.
    */

    if (req->rq_rpi->cq_state == C2CWRITE) {
      if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_IDLE == 
          lam_ssi_crmpi_base_handler_state) {
        add_write(req->rq_proc->p_rpi, req);
      } else {
        deferred_writes = 1;
      }
    } else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0) {
        lam_ssi_rpi_crtcp_add_read_any_src(req);
      } else {
        lam_ssi_rpi_crtcp_add_read(req->rq_proc->p_rpi, req);
      }
    }
  }

  /* IMPORTANT: See the big comment above about the deferred writes
     corner cases */

  if (deferred_writes != 0)
    PUB(flblock) = 0;

  if (PUB(nio) >= 1) {
    do {
      if (PUB(nio) == 1) {
        if (lam_ssi_rpi_crtcp_adv1())
          return (LAMERROR);
      } else {
        if (lam_ssi_rpi_crtcp_advmultiple())
          return (LAMERROR);
      }
    } while (PUB(flblock) && !PUB(haveadv));
  } else if (lam_ger && PUB(flblock)) {
    errno = EGERFLOW;
    return (LAMERROR);
  }

  /* Corner case: see big comment above -- if there are pending
     blocking writes, we need to yield to the CR handler.  We don't
     need to do anything special upon return, however -- we'll drop
     out of the routine with haveadv == 0. */

  if ((((PUB(nio) == 0) && (fl_block == 1)) || deferred_writes != 0) &&
      (LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING ==
       lam_ssi_crmpi_base_handler_state)) {
    lam_ssi_crmpi.lscrma_app_suspend();
  }

  return (PUB(haveadv));
}


/*
 *    lam_ssi_rpi_crtcp_iprobe
 *
 *    Function:       - non-blocking probe
 *                    - public interface for peculiar MPI_Iprobe() which
 *                      does not return a request to the user
 *    Accepts:        - request
 *    Returns:        - 0: no msg, 1: msg, LAMERROR: error 
 */
int
lam_ssi_rpi_crtcp_iprobe(MPI_Request req)
{
  int err;                    /* error code */
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


#if 0
/*
 *    lam_ssi_rpi_crtcp_quiesce
 *
 *    Function:       - quiesce the network
 *                    - shut down all network connections
 *    Returns:        - 0: on success, LAMERROR on error
 */
int
lam_ssi_rpi_crtcp_quiesce(void)
{
  int i;
  int done;
  int count = 0;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "inside quiesce:%d\n", getpid());

  /* Allocate the array of bookmarks */

  bookmarks = malloc(sizeof(struct bookmark_payload_t) * PUB(nprocs));
  if (NULL == bookmarks)
    return LAMERROR;

  /*
   * NOTE: the bookmark_exchange functions return the bookmarks in the same
   * order as that of the procs in the PUB(procs) array.
   */
  if (PUB(bookmark_exchange_alltoall)(bookmarks) < 0) {
    free(bookmarks);
    return LAMERROR;
  }

  if (PUB(verbose) >= 20)
    for (i = 0; i < PUB(nprocs); ++i)
      lam_debug(PUB(did), "received from %d: (%u)... actual: (%u)",
                i, 
                bookmarks[i].bp_out_nbytes, 
                PUB(procs)[i]->p_rpi->cp_in_nbytes);

  /* Now that we've sent/received all the bookmarks, advance receives
     until we match.  Do this in order to avoid deadlock. */

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "iterating over advance");
  do {

    /* Check all the bookmarks.  Inefficient, but... */

    done = 1;
    for (i = 0; i < PUB(nprocs); ++i)
      if (PUB(procs)[i] != lam_myproc)
        if (PUB(procs)[i]->p_rpi->cp_in_nbytes 
            != bookmarks[i].bp_out_nbytes) {
          done = 0;
          if (PUB(verbose) >= 20)
            lam_debug(PUB(did),
		      "quiesce not done; with index %d -- at %u, "
		      "bookmark = %u",
		      i,
		      PUB(procs)[i]->p_rpi->cp_in_nbytes,
		      bookmarks[i].bp_out_nbytes);
          ++count;
        }

    /* If we haven't matched all the bookmarks, advance */

    if (!done) {
      if (PUB(is_sread_in_fast)) {
        if (quiesce_advance_fast() == LAMERROR)
          return LAMERROR;
      } else {
        if (quiesce_advance_state_machine() == LAMERROR)
          return LAMERROR;
      }
    }
  } while (!done);

  /* Ditch the bookmarks */

  free(bookmarks);

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "quiesce complete");

  return 0;
}
#endif

/*
 *    lam_ssi_rpi_crtcp_finalize
 *
 *    Function:       - c2c cleanup
 *    Accepts:        - process to cleanup (0 => all processes)
 *    Returns:        - 0 or LAMERROR
 */

int
lam_ssi_rpi_crtcp_finalize(struct _proc *p)
{
  if (p)
    return (finalize1(p));
  else {

    /* Loop through all processes closing connections. */

    while (lam_arr_size(lprocs) > 0)
      if (finalize1(((struct _proc **) lam_arr_get(lprocs))[0]))
	return (LAMERROR);
    lam_arr_free(lprocs);

    /* Just so that there's no confusion... */

    PUB(procs) = NULL;
    PUB(nprocs) = 0;


    /* All done */

    if (PUB(verbose) >= 0)
      lam_debug(lam_ssi_did, "crtcp RPI module finalizing");
    if (PUB(did) != -1)
      lam_debug_close(PUB(did));
  }

  return (0);
}

/*
 *    send_to_self
 *
 *    Function:       - advance send to self
 *    Accepts:        - request list
 *                    - send request
 */
static int
send_to_self(MPI_Request req_top, MPI_Request send)
{
  MPI_Request recv;
  struct lam_ssi_rpi_cbuf_msg msg;
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
      memcpy(msg.cm_buf, send->rq_packbuf, send->rq_packsize);
    } else {
      msg.cm_buf = 0;
    }

    msg.cm_req = 0;
    send->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
}

/*
 *    send_to_self_match
 *
 *    Function:       - advance send and matching receive/probe
 *    Accepts:        - send request
 *                    - receive/probe request
 *    Returns:        - 1: matched a receive, 0: matched a probe
 */
static int
send_to_self_match(MPI_Request send, MPI_Request recv)
{
  int len;                      /* # bytes to transfer */

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

    memcpy(recv->rq_packbuf, send->rq_packbuf, len);

    lam_ssi_rpi_fill_mpi_status(recv, send->rq_rpi->cq_env.ce_rank,
                                send->rq_rpi->cq_env.ce_tag, len);

    send->rq_state = recv->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
    return (1);
  }
}


/*
 *    add_write
 *
 *    Function:       - add process to write advance list
 *    Accepts:        - process
 *                    - writing request
 */
static void
add_write(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  /*
   * Already added?
   */
  if (FD_ISSET(ps->cp_sock, &PUB(write)))
    return;

  /*
   * Associate request with process.
   */
  if (!ps->cp_wreq)
    ps->cp_wreq = req;

  PUB(nio)++;
  lam_ssi_rpi_crtcp_lastreq = req;
  FD_SET(ps->cp_sock, &PUB(write));
  FD_SET(ps->cp_sock, &PUB(except));

  if (ps->cp_sock > PUB(sockmax))
    PUB(sockmax) = ps->cp_sock;
}


/*
 *    lam_ssi_rpi_crtcp_add_read
 *
 *    Function:       - add process to read advance list
 *                    - do not add in case process is self
 *    Accepts:        - process
 *                    - request to start matching from
 */
void
lam_ssi_rpi_crtcp_add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  if (lam_ger && ps->cp_proc->p_num_buf_env >= MPI_GER)
    return;

  if (ps->cp_sock >= 0) {
    /*
     * Already added?
     */
    if (FD_ISSET(ps->cp_sock, &PUB(read)))
      return;

    PUB(nio)++;
    lam_ssi_rpi_crtcp_lastreq = req;
    ps->cp_mreq = req;
    FD_SET(ps->cp_sock, &PUB(read));
    FD_SET(ps->cp_sock, &PUB(except));

    if (ps->cp_sock > PUB(sockmax))
      PUB(sockmax) = ps->cp_sock;
  }
}


/*
 *    lam_ssi_rpi_crtcp_add_read_any_src
 *
 *    Function:       - add to the read advance list all processes in
 *                      the peer group of a receive request on MPI_ANY_SOURCE
 *    Accepts:        - request
 */
void
lam_ssi_rpi_crtcp_add_read_any_src(MPI_Request req)
{
  struct _group *g;             /* peer group */
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
      lam_ssi_rpi_crtcp_add_read(lam_impid_proc->p_rpi, req);
    else
      lam_ssi_rpi_crtcp_add_read((*p)->p_rpi, req);
#else
    lam_ssi_rpi_crtcp_add_read((*p)->p_rpi, req);
#endif
  }
}


/*
 *    connect_all
 *
 *    Function:       - make tcp connections to all other processes
 *    Returns:        - 0 or LAMERROR
 */
static int
connect_all(void)
{
  struct _gps *mygps;           /* my GPS */
  struct nmsg inmsg;            /* incoming network msg hdr */
  struct nmsg outmsg;           /* outgoing network msg hdr */
  struct dolink *links;         /* links to neighbours */
  int4 nlinks;                  /* number of links */
  int sock;                     /* socket descriptor */
  int servsockd;                /* server socket descriptor */
  int servport = 0;             /* server port number */
  int rnode;                    /* remote node */
  int rport;                    /* remote port */
  int flag;                     /* for setting socket opts */
  int bufsize;                  /* c2c socket buffer size */
  unsigned char *raddr;         /* remote host address */
  char myrankstr[32];
  int i;
  struct in_addr translated_addr;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);
  /*
   * Things to make -Wall not complain
   */
  mygps = &lam_myproc->p_gps;
  memset(myrankstr, 0, 32);
  snprintf(myrankstr, 31, "%d", mygps->gps_grank);
  bufsize = crtcp_sockbuf + sizeof(struct lam_ssi_rpi_envl);
  servsockd = -1;

  if (PUB(nprocs) > 1) {
    /*
     * Get links to neighbours, initialize server socket, message headers, etc.
     */
    if (ldogetlinks(&links, &nlinks))
      return (LAMERROR);

    servsockd = sfh_sock_open_srv_inet_stm(&servport, -1);
    if (servsockd < 0) {
      show_help("sfh", "open-server-socket", myrankstr, NULL);
      return (LAMERROR);
    }

    inmsg.nh_flags = DINT4DATA;
    inmsg.nh_length = 0;
    outmsg.nh_length = 0;
    outmsg.nh_flags = DINT4DATA;
    outmsg.nh_data[0] = (int4) servport;
  }
  /*
   * Loop through all processes, initializing the process data and
   * connecting to those not already connected to.
   */
  for (i = 0; i < PUB(nprocs); ++i) {

    if (((PUB(procs)[i])->p_mode & LAM_PRPIINIT) && 
        ((PUB(procs)[i])->p_mode & LAM_PRPICONNECT))
      continue;
    if (!((PUB(procs)[i])->p_mode & LAM_PRPIINIT))
      if (proc_init(PUB(procs)[i]) != 0)
        return LAMERROR;

    if (PUB(procs)[i] != lam_myproc) {

      if (LAM_GPSCMP(mygps, &(PUB(procs)[i])->p_gps) >= 0) {

        /* Act as a client. */

        fill_sync(PUB(procs)[i], lam_myproc, &inmsg);
        if (nrecv(&inmsg))
          return (LAMERROR);

        rport = (int) inmsg.nh_data[0];

        rnode = (PUB(procs)[i])->p_gps.gps_node;

        if (rnode > nlinks)
          return (LAMERROR);

        /* See if we need to map this IP address to an alternate */

        if (lam_ssi_rpi_verbose >= 5) {
          lam_debug(lam_ssi_rpi_did, "crtcp: creating outgoing socket to %s",
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
        fill_sync(lam_myproc, PUB(procs)[i], &outmsg);
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
      
      (PUB(procs)[i])->p_rpi->cp_sock = sock;
      PUB(smap)[sock] = (PUB(procs)[i])->p_rpi;

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

      FD_CLR(sock, &PUB(block));

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
    (PUB(procs)[i])->p_mode |= LAM_PRPICONNECT;
  }

  if (PUB(nprocs) > 1) {
    close(servsockd);
    if (links != 0)
      free(links);
  }

  return (0);
}


/*
 *    proc_init
 *
 *    Function:       - initialize c2c specific process data
 *    Accepts:        - process
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
  p->p_rpi->cp_sockinfo = 0;
  p->p_rpi->cp_mreq = 0;
  p->p_rpi->cp_rreq = 0;
  p->p_rpi->cp_wreq = 0;
  p->p_rpi->cp_extra = 0;
  p->p_rpi->cp_in_nbytes = p->p_rpi->cp_out_nbytes = 0;

  /* Set up to read in an envelope */

  p->p_rpi->cp_readfn = lam_ssi_rpi_crtcp_proc_read_env;
  p->p_rpi->cp_envbuf = (char *) &p->p_rpi->cp_env;
  p->p_rpi->cp_nenvin = sizeof(struct lam_ssi_rpi_envl);

  return 0;
}


/*
 *    fill_sync
 *
 *    Function:       - fill in network message sync for connecting
 *    Accepts:        - source process
 *                    - destination process
 *                    - network message header (filled)
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
 *    finalize1
 *
 *    Function:       - cleanup a process
 *    Accepts:        - process
 *    Returns:        - 0 or LAMERROR
 */
static int
finalize1(struct _proc *p)
{
  /* Remove this proc from the array of valid procs that this RPI is
     reponsible for */

  lam_arr_remove(lprocs, &p);
  PUB(procs) = lam_arr_get(lprocs);
  PUB(nprocs) = lam_arr_size(lprocs);

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

  return (0);
}


#if CRTCP_DO_FAST
/*
 *    lam_ssi_rpi_crtcp_fastsend
 *
 *    Function:       - fast blocking send
 *    Accepts:        - buffer to send
 *                    - message count
 *                    - message datatype
 *                    - destination process rank
 *                    - message tag
 *                    - message communicator
 *    Returns:        - MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_crtcp_fastsend(char *buf, int count, MPI_Datatype dtype,
                         int dest, int tag, MPI_Comm comm)
{
  double local_rep;             /* local data representation */
  double net_rep;               /* net data representation */
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
    if ((dtype->dt_flags & LAM_DTNOPACK)
        && ((dtype->dt_flags & LAM_DTNOXADJ) || count == 1)
        && ((local_rep == net_rep) || lam_homog)) {
      packbuf = buf;

      /* Check for bad buffer. */

      if (packbuf == 0)
        return (lam_mkerr(MPI_ERR_BUFFER, 0));
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

  err = lam_ssi_rpi_crtcp_low_fastsend(packbuf, packsize,
                                     destproc->p_rpi, dest, tag, comm);

  if (packbuf != buf) {
    free(packbuf);
  }

  return (err);
}


/*
 *    lam_ssi_rpi_crtcp_fastrecv
 *
 *    Function:       - fast blocking receive
 *    Accepts:        - buffer to receive into
 *                    - message count
 *                    - message datatype
 *                    - source process rank
 *                    - message tag (inout)
 *                    - message communicator
 *                    - status (out)
 *                    - seqnum (out)
 *    Returns:        - MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_crtcp_fastrecv(char *buf, int count, MPI_Datatype dtype,
                         int src, int *tag, MPI_Comm comm,
                         MPI_Status * stat, int *seqnum)
{
  double local_rep;             /* local data representation */
  double net_rep;               /* net data representation */
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

  err = lam_ssi_rpi_crtcp_low_fastrecv(packbuf, &packsize, srcproc->p_rpi,
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
#endif

#if 0
static int
quiesce_advance_fast(void)
{
  /*
   * All the work has to be done by the app-thread, because we can't quiesce
   * messages from within the cr_handler and pass them on to the app-thread,
   * which has been interrupted in _fast. 
   *
   * Once the app_thread is done quiescing the pending messages, it returns
   * control to the cr_thread.
   */

  PUB(read_and_yield_back) = 1;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did),
	      "Quiesce_fast: cr yielding to app to quiesce and return"); 
  
  /* set PUB(handler_state) to WAITING before yielding to the app_thread */
  PUB(handler_state) = LAM_SSI_RPI_CRTCP_HANDLER_STATE_WAITING;

  yield_return();

  /* set PUB(handler_state) back to RUNNING */
  PUB(handler_state) = LAM_SSI_RPI_CRTCP_HANDLER_STATE_RUNNING;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "Quiesce_fast: cr returned from app after quiesce"); 

  PUB(read_and_yield_back) = 0;

  return 0;
}


static int
quiesce_advance_state_machine(void)
{
  int i;
  MPI_Request req;
  int flblock_save = PUB(flblock);

  /* 
   * the app_thread is outside of _fast in libmpi, or outside of libmpi itself.
   * 
   * Zero out all the fd_sets.  Do this even though we're going to override pub
   * and except with all sockets because otherwise add_read() won't do anything
   * (i.e., it doesn't add a process to the fd_set twice -- an optimization) 
   */

  FD_ZERO(&PUB(read));
  FD_ZERO(&PUB(except));
  FD_ZERO(&PUB(write));
  FD_ZERO(&PUB(eoferr));
  PUB(flblock) = 0;
  PUB(nio) = 0;
  PUB(sockmax) = PUB(full_sockmax);

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "Quiesce_state_mc: advancing");

  /* Set all procs to have a cp_mreq of NULL */

  for (i = 0; i < PUB(nprocs); ++i)
    PUB(procs[i])->p_rpi->cp_mreq = NULL;

  /* Look for pending reads in the request list */

  for (req = lam_rq_top; req; req = req->rq_next) {

    /* Ignore requests that are in the init or done state.  */

    if (req->rq_state == LAM_RQSINIT ||
        req->rq_state == LAM_RQSDONE)
      continue;

    if (lam_ssi_rpi_comm_dead_m(req))
      continue;

    /* Ignore writes -- we're only trying to complete outstanding
       reads */

    if (req->rq_rpi->cq_state == C2CWRITE)
      continue;
    else if (req->rq_rpi->cq_state == C2CREAD) {
      if (req->rq_proc == 0) {
        lam_ssi_rpi_crtcp_add_read_any_src(req);
      } else {
        lam_ssi_rpi_crtcp_add_read(req->rq_proc->p_rpi, req);
      }
    }
  }

  /* Override what we just set so that we select on *all* possible
     sockets */

  memcpy((char *) &PUB(read), (char*) &PUB(full_fd_set), sizeof(fd_set));
  memcpy((char *) &PUB(except), (char*) &PUB(full_fd_set), sizeof(fd_set));

  /* *ALWAYS* call advmultiple() -- regardless of how many requests we
     found to be ready (0, 1, or many).  There may be data "on the
     wire" that we have not yet posted requests for.  */

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "Quiesce_state_mc: advancing calling advmultiple");
  if (lam_ssi_rpi_crtcp_advmultiple())
    return (LAMERROR);
  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "Quiesce_state_mc: advancing back from advmultiple:"
	      " haveadv: %d", PUB(haveadv));
  PUB(flblock) = flblock_save;
  
  return 0;
}


static void
yield_return(void)
{
  lam_ssi_rpi_crtcp_yield_to_app_thread();
  /* regain control of lam_cr_mutex, so that we can proceed. */
  lam_mutex_lock(&lam_cr_mutex);
  lam_mutex_unlock(&lam_yield_to_cr_mutex);
}


void
lam_ssi_rpi_crtcp_yield_to_app_thread(void)
{
  lam_mutex_lock(&lam_yield_to_app_mutex);
  lam_mutex_unlock(&lam_cr_mutex);
  lam_mutex_lock(&lam_yield_to_cr_mutex);
  lam_mutex_unlock(&lam_yield_to_app_mutex);

}
#endif
