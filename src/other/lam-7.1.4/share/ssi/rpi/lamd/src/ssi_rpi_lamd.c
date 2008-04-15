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
 * $Id: ssi_rpi_lamd.c,v 1.18 2004/02/26 06:12:15 pcharapa Exp $
 *
 *	Function:	- RPI implementation for LAM daemon
 */

#include <lam_config.h>
#include <lam-ssi-rpi-lamd-config.h>

#include <signal.h>
#include <stdlib.h>

#include <bfreq.h>
#include <events.h>
#include <kreq.h>
#include <ksignal.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <lamdebug.h>
#include <all_array.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-rpi-lamd.h>
#include <rpi_lamd.h>


/*
 * private functions
 */
static int lamd_adv1(MPI_Request req, int flag_block);
static int lamd_testany(MPI_Request req_top, int nreq_recv);
static int lamd_waitany(MPI_Request req_top, int nreq_recv);
static int lamd_findfwd(MPI_Request req_top, int isel_ready);
static int lamd_probe(MPI_Request req, int flag_block);
static int lamd_send(MPI_Request req, int flag_block);
static int lamd_recv(MPI_Request req, int flag_block);
static int lamd_bfrecv(struct nmsg *msg, int flag_block);
static int lamd_dsend(struct nmsg *msg, int flag_block);
static int lamd_commdead(MPI_Request req);
static int lamd_route(struct nmsg *msg);
static int lamd_finalize1(struct _proc *p);
static struct bfsync *lamd_fillsel(MPI_Request req_top, int nreq_recv);
static void lamd_fillfyi(MPI_Request req);
static void lamd_noretry(void);
static void lamd_retry(void);
static void lamd_preempt(int flag);
static void lamd_shrink(int);


/*
 * external functions
 */
extern void _m2l_ack();
extern void _m2l_fill();
extern void _m2l_tail();
extern int _m2l_gettag();
extern int microsleep();
extern struct _proc *_m2l_getsource();


/*
 * local variables
 */
static int param_priority;
static int delay_start;		/* starting poll delay */
static int mask_retry;		/* signal retry mask */
static void (*oldhandler) ();	/* previous shrink handler */
static lam_array_t *procs = NULL;
static int MPI_GER_CID = -1;
static const lam_ssi_rpi_actions_1_0_0_t module_actions = {

  /* RPI API functions */

  lam_ssi_rpi_lamd_addprocs,
  lam_ssi_rpi_lamd_finalize,

  lam_ssi_rpi_lamd_build,
  lam_ssi_rpi_lamd_start,
  lam_ssi_rpi_lamd_advance,
  lam_ssi_rpi_lamd_destroy,

  lam_ssi_rpi_lamd_iprobe,

  NULL,
  NULL,

  lam_ssi_rpi_base_alloc_mem,
  lam_ssi_rpi_base_free_mem,

  NULL,
  NULL,
  NULL,

  /* Flags */
  /* Totalview support */

  0
};


/*
 * Open the module and register an SSI parameter
 */
int
lam_ssi_rpi_lamd_open_module(OPT *ad)
{
  param_priority = 
    lam_ssi_base_param_register_int("rpi", "lamd", "priority", NULL,
                                    LAM_SSI_RPI_LAMD_DEFAULT ? 75 : 20);

  return 1;
}


/*
 * lam_ssi_rpi_lamd_query
 *
 * Function: determine if this module wants to run
 */
int
lam_ssi_rpi_lamd_query(int *priority, int *thread_min, int *thread_max)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return 0;

  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_SERIALIZED;

  /* If we're verbose, print stuff out */

  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "lamd: module initializing");
    lam_debug(lam_ssi_rpi_did, "lamd:verbose: %d", 
	      lam_ssi_rpi_verbose);
    lam_debug(lam_ssi_rpi_did, "lamd:priority: %d", *priority);
  }

  /* Return 1 saying that we want to be considered */

  return 1;
}


/*
 *	lam_ssi_rpi_lamd_init
 *
 *	Function:	- primary initialiation of RPI subsystem
 */
const lam_ssi_rpi_actions_1_0_0_t *
lam_ssi_rpi_lamd_init(struct _proc **arg_procs, int arg_nprocs, 
		      int *maxtag, int *maxcid)
{
  struct _proc *p;
  int i, nlocal;

  if (lam_ssi_rpi_verbose >= 0)
    lam_debug(lam_ssi_rpi_did, "lamd: module initializing");

  /* Add all the procs into the local cache */

  if (lam_ssi_rpi_lamd_addprocs(arg_procs, arg_nprocs) != 0)
    return NULL;

  /* Calculate the polling delay. */

  for (nlocal = i = 0; i < arg_nprocs; ++i) {
    p = arg_procs[i];
    if ((p->p_gps.gps_node == LOCAL) || (p->p_gps.gps_node == getnodeid()))
      nlocal++;
  }

  if (nlocal > 1)
    delay_start = MPIL_LPD_BASE + (nlocal * nlocal);
  else
    delay_start = 1;
/*
 * For fault tolerance, catch LAM_SIGSHRINK.
 */
  oldhandler = lam_ksignal(LAM_SIGSHRINK, lamd_shrink);
  if (oldhandler == (void (*)()) -1)
    return NULL;

  /* Override the default max tag and max CID settings */

  *maxtag = (1 << 15) - 1;
  *maxcid = (1 << 12) - 1;
  MPI_GER_CID = *maxcid + 1;

  return &module_actions;
}


/*
 *	lam_ssi_rpi_lamd_addprocs
 *
 *	Function:	- setup for new processes
 *			- nothing for lamd
 */
int
lam_ssi_rpi_lamd_addprocs(struct _proc **arg_procs, int arg_nprocs)
{
  int i;

  /* Initialize the local procs array if we need to */

  if (procs == 0)
    procs = lam_arr_init(sizeof(struct _proc *), NULL);

  /* Add these procs into the local procs array.  Make sure that
     they're not already in there first. */

  for (i = 0; i < arg_nprocs; ++i)
    if (lam_arr_find(procs, &arg_procs[i]) == LAMERROR)
      lam_arr_append(procs, &arg_procs[i]);

  return 0;
}


/*
 *	lam_ssi_rpi_lamd_build
 *
 *	Function:	- builds RPI portion of a request from scratch
 *			- one-time cost separated from _rpi_lamd_start()
 *			  to optimize persistent requests
 *	Accepts:	- nothing for lamd
 */
int
lam_ssi_rpi_lamd_build(MPI_Request req)
{
  struct lam_ssi_rpi_req *lmq;		/* lamd request */

  lmq = req->rq_rpi = malloc(sizeof(struct lam_ssi_rpi_req));
  if (lmq == 0)
    return LAMERROR;

  lmq->lmq_flags = 0;
  LAM_ZERO_ME(lmq->lmq_msg_data);
  LAM_ZERO_ME(lmq->lmq_msg_ack);
  LAM_ZERO_ME(lmq->lmq_msg_ger);
  return (0);
}


/*
 *	lam_ssi_rpi_lamd_start
 *
 *	Function:	- initializes RPI dependent aspects of a request
 *			- cost per request start - separated from
 *			  _rpi_lamd_build() to optimize persistent requests
 *	Accepts:	- top of request list (unused)
 *			- request
 */
int
lam_ssi_rpi_lamd_start(MPI_Request req_top, MPI_Request req)
{
  struct lam_ssi_rpi_req *lmq;		/* lamd request */
  struct nmsg *msg_data;	/* data message */
  struct _fyimsg *pmfyi;	/* message FYI */
  int r;

  lmq = req->rq_rpi;
  lmq->lmq_state = LAMD_START;
  lmq->lmq_ndata = req->rq_packsize;
/*
 * data message
 */
  msg_data = &lmq->lmq_msg_data;
  msg_data->nh_flags =
    (((_kio.ki_rtf & RTF_TRON) == RTF_TRON) ? KTRACE : 0) | KSYNCSQL;
  msg_data->nh_length = (lmq->lmq_ndata > MAXNMSGLEN) ?
    MAXNMSGLEN : lmq->lmq_ndata;
  msg_data->nh_msg = req->rq_packbuf;
/*
 * ack message
 */
  lmq->lmq_msg_ack.nh_flags = lmq->lmq_msg_data.nh_flags;
  lmq->lmq_msg_ack.nh_length = 0;
/*
 * GER message
 */
  lmq->lmq_msg_ger.nh_flags = lmq->lmq_msg_data.nh_flags;
  lmq->lmq_msg_ger.nh_length = 0;
/*
 * receive specific initialization
 */
  if (req->rq_marks & LAM_RQFDEST) {
/*
 * Turn on the NMORE flag so that we can detect an incoming long message.
 */
    msg_data->nh_flags |= NMORE;
/*
 * Send the destination data length in the ack message.
 */
    lmq->lmq_msg_ack.nh_data[0] = lmq->lmq_ndata;
/*
 * Fill the LAM network message with MPI synchronization.
 */
    _m2l_fill(req->rq_proc, lam_myproc, req->rq_tag,
	      req->rq_cid, msg_data);
  }
/*
 * send specific initialization
 */
  else {
/*
 * Route the data message.
 */
    msg_data->nh_node = req->rq_proc->p_gps.gps_node;
    if (lamd_route(msg_data))
      return (LAMERROR);
/*
 * Fill the LAM network message with MPI synchronization.
 */
    _m2l_fill(lam_myproc, req->rq_proc, req->rq_tag,
	      req->rq_cid, msg_data);
    _m2l_fill(req->rq_proc, lam_myproc, req->rq_tag,
	      req->rq_cid, &lmq->lmq_msg_ack);
    _m2l_fill(req->rq_proc, lam_myproc, MPI_GER_TAG,
	      MPI_GER_CID, &lmq->lmq_msg_ger);
/*
 * Add MPI acknowledgement synchronization to the LAM network message.
 */
    _m2l_ack(&lmq->lmq_msg_ack);

    if (lmq->lmq_ndata > MAXNMSGLEN) {
      msg_data->nh_flags |= NMORE;
    }
/*
 * Set any LAM built-in data representation flags.
 */
    msg_data->nh_flags |= req->rq_flags & DMSKMSG;
/*
 * We have to pass a few extra things to the destination process:
 * - the total length of the source's message, in case the destination
 *   needs to reduce the number of packets it will receive
 * - our rank: not strictly necessary but avoids looping at destination
 * - a flag indicating a synchronous send, which demands an ack message
 *   even if the message is short
 * - a sequence number for debugging and cancellation
 */
    msg_data->nh_data[0] = lmq->lmq_ndata;
    msg_data->nh_data[1] = req->rq_comm->c_group->g_myrank;
    msg_data->nh_data[2] = (req->rq_type == LAM_RQISSEND) ? LAMD_SSEND : 0;
    msg_data->nh_data[3] = req->rq_seq;
/*
 * Fill message FYI values.
 */
    pmfyi = (struct _fyimsg *) &msg_data->nh_data[4];
    pmfyi->fym_src = (lam_myproc->p_gps.gps_grank << 16) |
      req->rq_comm->c_group->g_myrank;
    pmfyi->fym_dest = (req->rq_proc->p_gps.gps_grank << 16) | req->rq_rank;
    pmfyi->fym_count = req->rq_count;
    pmfyi->fym_dtpid = (req->rq_dtype->dt_label << 16) |
      lam_myproc->p_gps.gps_pid;
/*
 * We are within one slot of the GER limit.  Blocking sends can pass
 * because they will not return to user code until the request is done
 * and we have received a flow control message indicating another slot
 * is free at the destination.  Non-blocking sends must ensure another
 * envelope slot now in a non-blocking operation - otherwise it's an error.
 */
    if (lam_ger && (req->rq_proc->p_ger_nsnd == (MPI_GER - 1))
	&& !(req->rq_marks & LAM_RQFBLKTYPE)) {
      r = lamd_bfrecv(&lmq->lmq_msg_ger, FALSE);
      if (r < 0)
	return (LAMERROR);

      if (r == 0) {
	errno = EGERFLOW;
	return (LAMERROR);
      }

      req->rq_proc->p_ger_nsnd--;
    }

  }

  return (0);
}


/*
 *	lam_ssi_rpi_lamd_destroy
 *
 *	Function:	- destroys RPI portion of request
 *	Accepts:	- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_lamd_destroy(MPI_Request req)
{
  if (req->rq_rpi != 0)
    free(req->rq_rpi);
  req->rq_rpi = 0;
  return (0);
}


/*
 *	lam_ssi_rpi_lamd_advance
 *
 *	Function:	- selects and advances a request via LAM daemon
 *			- RPI permits us to advance multiple requests but
 *			  we advance at most one request.  We have to
 *			  re-evaluate the entire request list after each
 *			  advancement so we rely on the fact that the caller
 *			  will loop until she gets what she wants.
 *			- The blocking flag, when set, indicates that
 *			  we are allowed to block indefinitely to advance
 *			  a request - the first time.
 *			- Blocking via LAM is only possible if all of the
 *			  requests are in a receive state or if we have
 *			  exactly one request.
 *			- When reset (== 0), the blocking flag indicates that
 *			  we must return after one attempt to advance each
 *			  request, even if no request advanced.
 *			- All receive requests must be polled or blocked by
 *			  one call (atomic operation wrt arriving messages)
 *			  to the buffer daemon to ensure ordering.
 *			  The buffer daemon must also consider the receive
 *			  requests in the order given.  These requirements
 *			  are met by LAM bfiselect() and bfselect().
 *
 *	Accepts:	- request list
 *			- block enable flag
 *			  We don't need this flag because we loop over
 *			  the requests but other implementations might
 *			  be able to avoid the loop.
 *	Returns:	- 0 (no movement), 1 (movement), -1 (error)
 */
int
lam_ssi_rpi_lamd_advance(MPI_Request req_top, int flag_block)
{
  MPI_Request req;		/* current pointer */
  MPI_Request req_active = MPI_REQUEST_NULL;	/* active request in list */
  MPI_Request req_lastblk = MPI_REQUEST_NULL;	/* last blocking request */
  int delay_loop;		/* delay in polling loop */
  int nreq;			/* # uncompleted requests */
  int nreq_recv;		/* # reqs in receive state */
  int flag_trace;		/* tracing is on? */
  int r;
  int ret_movement;		/* did we ever move? */
  double time_start = 0.0;	/* when blocking began */
  struct lam_ssi_rpi_req *lmq;		/* lamd request */

  flag_trace = (_kio.ki_rtf & RTF_TRON) == RTF_TRON;
/*
 * Count and mark the requests in a receive state.
 */
  nreq = 0;
  nreq_recv = 0;

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
 * Ignore completed requests.  This is new business.
 */
    if (req->rq_state == LAM_RQSDONE)
      continue;

    nreq++;
    req_active = req;

    if (req->rq_flags & LAM_RQFBLOCK) {
      req_lastblk = req;
    }
/*
 * Mark a request in a receive or probe (i.e. not a send) state.
 */
    lmq = req->rq_rpi;
    if (((req->rq_marks & LAM_RQFDEST) == LAM_RQFDEST) ^
	((lmq->lmq_state == LAMD_ACK) || lmq->lmq_state == LAMD_GER)) {
      lmq->lmq_flags |= LAMD_RECVMODE;
      nreq_recv++;
    } else {
      lmq->lmq_flags &= ~LAMD_RECVMODE;
    }
  }
/*
 * There are no incomplete requests, so we return no movement.
 * Obviously, we must have been in non-blocking mode.
 */
  if (nreq == 0) {
    return (0);
  }
/*
 * There is only one incomplete request.  Advance it using
 * optimal single request methods.
 */
  if (nreq == 1) {
/*
 * In blocking mode, keep moving until the request is done.
 */
    if (flag_block) {

      do {
	if (lamd_adv1(req_active, TRUE) < 0)
	  return (LAMERROR);
      } while (req_active->rq_state != LAM_RQSDONE);

      return (1);
    }
/*
 * In non-blocking mode, advance until the request is done or cannot move.
 */
    else {
      ret_movement = 0;

      do {
	r = lamd_adv1(req_active, FALSE);
	if (r < 0)
	  return (LAMERROR);

	if (r == 1) {
	  ret_movement = 1;
	}
      } while ((req_active->rq_state != LAM_RQSDONE) && (r == 1));

      return (ret_movement);
    }
  }
/*
 * We are in non-blocking mode.  Make one pass and return the result.
 */
  else if (!flag_block) {
    return (lamd_testany(req_top, nreq_recv));
  }
/*
 * We are in blocking mode but we have some send mode requests.
 * Poll through the requests until something moves.
 */
  else if (nreq_recv < nreq) {

    r = lamd_testany(req_top, nreq_recv);
    if (r < 0)
      return (LAMERROR);

    if (r == 0) {
      delay_loop = delay_start;

      if (flag_trace) {
	time_start = MPI_Wtime();
      }

      do {
	microsleep(delay_loop);
	delay_loop += delay_start;
	delay_loop = LAM_min(MPIL_LPD_MAX, delay_loop);
	r = lamd_testany(req_top, nreq_recv);
	if (r < 0)
	  return (LAMERROR);
      } while (r == 0);

      if (flag_trace) {
	_kio.ki_blktime += MPI_Wtime() - time_start;
      }
    }

    return (1);
  }
/*
 * We are in blocking mode and all requests are in receive mode.
 * Use optimal blocking features for multiple receives in LAM.
 */
  else {
    if (req_lastblk != MPI_REQUEST_NULL)
      lamd_fillfyi(req_lastblk);
    if (lamd_waitany(req_top, nreq_recv))
      return (LAMERROR);
    return (1);
  }
}


/*
 *	lam_ssi_rpi_lamd_finalize
 *
 *	Function:	- cleans up flow messages from all destinations
 *	Accepts:	- process to cleanup (0 => all processes)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_lamd_finalize(struct _proc *p)
{
  if (p)
    return (lamd_finalize1(p));
  else {
    while (lam_arr_size(procs) > 0)
      if (lamd_finalize1(((struct _proc **) lam_arr_get(procs))[0]))
	return (LAMERROR);
    lam_arr_free(procs);

    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(lam_ssi_rpi_did, "lamd: module finalizing");
  }

  return 0;
}


/*
 *	lamd_adv1
 *
 *	Function:	- advances one request, one step
 *	Accepts:	- request
 *			- block flag: 0 (no), 1 (yes)
 *	Returns:	- 0 (no movement), 1 (movement), -1 (error)
 */
static int
lamd_adv1(MPI_Request req, int flag_block)
{
  int r;
  int (*transfer) ();

  if (req->rq_type == LAM_RQIRECV) {
    transfer = lamd_recv;
  } else if (req->rq_type == LAM_RQIPROBE) {
    transfer = lamd_probe;
  } else {
    transfer = lamd_send;
  }
/*
 * fault tolerant loop
 */
  do {
    lamd_preempt(FALSE);

    if (lamd_commdead(req)) {
      lamd_preempt(TRUE);
      return (1);
    }

    lamd_noretry();
    r = (*transfer) (req, flag_block);
    lamd_retry();
  } while ((r < 0) && (errno == EINTR));

  return (r);
}


/*
 *	lamd_testany
 *
 *	Function:	- polls all uncompleted requests
 *			- forwards a ready request
 *	Accepts:	- request list
 *			- # requests in receive state
 *	Returns:	- 0 (no movement), 1 (movement), -1 (error)
 */
static int
lamd_testany(MPI_Request req_top, int nreq_recv)
{
  struct bfsync *sel;		/* selected sync points */
  MPI_Request req;		/* current request */
  int is_local;			/* local node destination? */
  int xx_local;			/* local node destination? */
  int xx_remote;		/* remote node destination? */
  int isel_ready;		/* ready sync index */
  int r;
/*
 * Attempt to forward each send request.
 * In lieu of an atomic multiple try_send operation, attempt each forwarding
 * process only once to keep messages in order.
 */
  xx_local = FALSE;
  xx_remote = FALSE;

  for (req = req_top; req; req = req->rq_next) {
    if (req->rq_state == LAM_RQSDONE)
      continue;

    if (req->rq_rpi->lmq_flags & LAMD_RECVMODE)
      continue;

    is_local = (req->rq_proc->p_gps.gps_node == LOCAL) ||
      (req->rq_proc->p_gps.gps_node == getnodeid());

    if (is_local && xx_local)
      continue;
    if (!is_local && xx_remote)
      continue;

    r = lamd_adv1(req, FALSE);

    if ((r < 0) || (r == 1))
      return (r);

    if (is_local) {
      xx_local = TRUE;
    } else {
      xx_remote = TRUE;
    }
  }
/*
 * Aggregate and atomically forward receive requests.
 */
  if (nreq_recv == 0) {
    return (0);
  }
/*
 * fault tolerant loop
 */
  do {
    lamd_preempt(FALSE);
    sel = lamd_fillsel(req_top, nreq_recv);

    if (sel == 0) {
      lamd_preempt(TRUE);
      return ((errno == EDEADNODE) ? 1 : LAMERROR);
    }

    lamd_noretry();
    r = bfiselect(sel, nreq_recv, KSYNCSQL, &isel_ready);
    free((char *) sel);
    lamd_retry();
  } while ((r < 0) && (errno == EINTR));

  if (r < 0)
    return (LAMERROR);
/*
 * If a request is ready, find it, forward it, and return it.
 */
  if (isel_ready >= 0) {

    if (lamd_findfwd(req_top, isel_ready) < 0)
      return (LAMERROR);

    return (1);
  }

  return (0);
}


/*
 *	lamd_waitany
 *
 *	Function:	- blocks on all uncompleted requests
 *			- forwards a ready request
 *			- assumes all uncompleted requests are in receive state
 *	Accepts:	- request list
 *			- # requests in receive state
 *	Returns:	- 0 or LAMERROR
 */
static int
lamd_waitany(MPI_Request req_top, int nreq_recv)
{
  struct bfsync *sel;		/* selected sync points */
  int isel_ready;		/* ready sync index */
  int r;
/*
 * fault tolerant loop
 */
  do {
    lamd_preempt(FALSE);
    sel = lamd_fillsel(req_top, nreq_recv);

    if (sel == 0) {
      lamd_preempt(TRUE);
      return ((errno == EDEADNODE) ? 0 : LAMERROR);
    }

    lamd_noretry();
    r = bfselect(sel, nreq_recv, KSYNCSQL |
		 ((_kio.ki_rtf & RTF_TRON) == RTF_TRON) ?
		 KTRACE : 0, &isel_ready);
    free((char *) sel);
    lamd_retry();
  } while ((r < 0) && (errno == EINTR));

  if (r < 0)
    return (LAMERROR);
/*
 * A request is ready.  Find it, forward it, and return it.
 */
  return (lamd_findfwd(req_top, isel_ready));
}


/*
 *	lamd_fillsel
 *
 *	Function:	- fills selection array with synchronizationd data
 *			  for bufferd
 *			- builds array from requests in receive state
 *	Accepts:	- request list
 *			- # requests in receive state
 *	Returns:	- bufferd sync array
 */
static struct bfsync *
lamd_fillsel(MPI_Request req_top, int nreq_recv)
{
  struct bfsync *sel;		/* selected sync points */
  MPI_Request req;		/* current request */
  int i;
  struct lam_ssi_rpi_req *lmq;		/* lamd request */

  sel = (struct bfsync *) malloc((unsigned) nreq_recv *
				 sizeof(struct bfsync));
  if (sel == 0)
    return (0);

  for (req = req_top, i = 0; req; req = req->rq_next) {
    if (req->rq_state == LAM_RQSDONE)
      continue;

    lmq = req->rq_rpi;
    if (!(lmq->lmq_flags & LAMD_RECVMODE))
      continue;
/*
 * Check for dead communicator.
 */
    if (lamd_commdead(req)) {
      errno = EDEADNODE;
      free((char *) sel);
      return (0);
    }

    if (lmq->lmq_state == LAMD_ACK) {
      sel[i].bfk_event = lmq->lmq_msg_ack.nh_event;
      sel[i].bfk_type = lmq->lmq_msg_ack.nh_type;
    } else if (lmq->lmq_state == LAMD_GER) {
      sel[i].bfk_event = lmq->lmq_msg_ger.nh_event;
      sel[i].bfk_type = lmq->lmq_msg_ger.nh_type;
    } else {
      sel[i].bfk_event = lmq->lmq_msg_data.nh_event;
      sel[i].bfk_type = lmq->lmq_msg_data.nh_type;
    }

    ++i;
  }

  return (sel);
}


/*
 *	lamd_fillfyi
 *
 *	Function:	- fills process FYI space in KIO, which is carried
 *			  to the kernel during kernel calls
 *			- called before a blocking function
 *			- This should really be done on a case by case
 *			  basis in top level functions.
 *	Accepts:	- MPI request
 */
static void
lamd_fillfyi(MPI_Request req)
{
  struct _fyiproc *fyi;		/* FYI space in KIO */
  struct _proc *peer;		/* peer process */

  fyi = (struct _fyiproc *) &_kio.ki_fyi[0];

  lam_getparam(&(fyi->fyp_root), &(fyi->fyp_rootgps));
  fyi->fyp_func = lam_getfunc();

  peer = req->rq_proc;
  fyi->fyp_me = (lam_myproc->p_gps.gps_grank << 16) |
    req->rq_comm->c_group->g_myrank;
  fyi->fyp_peer = req->rq_rank | ((peer == 0) ? -1 :
				  peer->p_gps.gps_grank << 16);
  fyi->fyp_peergps = ((peer == 0) ? -1 :
		      (peer->p_gps.gps_node << 16) | peer->p_gps.gps_idx);
  fyi->fyp_cidtag = (req->rq_cid << 16) | (req->rq_tag & 0xFFFF);
  fyi->fyp_count = req->rq_count;
  fyi->fyp_dtype = req->rq_dtype->dt_label;
}


/*
 *	lamd_findfwd
 *
 *	Function:	- finds the ready request matching the ready index
 *			  in the synchronization array
 *			- forwards ready request
 *	Accepts:	- request list
 *			- bufferd synchronization array
 *			- ready index in sync array
 *	Returns:	- 0 or LAMERROR
 */
static int
lamd_findfwd(MPI_Request req_top, int isel_ready)
{
  MPI_Request req;		/* current MPI request */
  int i;

  for (i = 0, req = req_top;; req = req->rq_next) {
    if (req->rq_state == LAM_RQSDONE)
      continue;

    if (req->rq_rpi->lmq_flags & LAMD_RECVMODE) {
      if (isel_ready == i) {
	break;
      } else {
	i++;
      }
    }
  }
/*
 * Forward the ready request, blocking.
 */
  if (lamd_adv1(req, TRUE) < 0)
    return (LAMERROR);

  return (0);
}


/*
 *	lam_ssi_rpi_lamd_iprobe
 *
 *	Function:	- non-blocking probe
 *			- public interface for peculiar MPI_Iprobe() which
 *			  does not return a request to the user
 *	Accepts:	- request
 *	Returns:	- 0: no msg, 1: msg, -1: error
 */
int
lam_ssi_rpi_lamd_iprobe(MPI_Request req)
{
  return (lamd_probe(req, FALSE));
}


/*
 *	lamd_probe
 *
 *	Function:	- probes for a message
 *			- message info returned in rq_status
 *	Accepts:	- request
 *			- blocking allowed?
 *	Returns:	- 0: no movement, 1: movement, -1: error
 */
static int
lamd_probe(MPI_Request req, int flag_block)
{
  int isel;			/* selected sync index */
  int r;
  struct bfsync sel;		/* select entry */
  struct nmsg *msg_data;	/* data message */
/*
 * I believe the following condition should never happen.
 */
  if (req->rq_state == LAM_RQSDONE)
    return (0);

  msg_data = &(req->rq_rpi->lmq_msg_data);

  if (flag_block) {
    lamd_fillfyi(req);
    sel.bfk_event = msg_data->nh_event;
    sel.bfk_type = msg_data->nh_type;

    if (bfselect(&sel, 1, msg_data->nh_flags, &isel))
      return (LAMERROR);
  }
/*
 * Do the non-blocking probe.
 */
  r = bfiprobe(msg_data);
  if (r < 0)
    return (LAMERROR);
/*
 * Fill in the MPI status object.
 */
  if (r > 0) {

    if (req->rq_tag == MPI_ANY_TAG) {
      req->rq_status.MPI_TAG = _m2l_gettag(msg_data);
    } else {
      req->rq_status.MPI_TAG = req->rq_tag;
    }

    if (req->rq_rank == MPI_ANY_SOURCE) {
      req->rq_status.MPI_SOURCE = msg_data->nh_data[1];
    } else {
      req->rq_status.MPI_SOURCE = req->rq_rank;
    }

    req->rq_status.st_length = msg_data->nh_length;

    req->rq_rpi->lmq_state = LAMD_DONE;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (r);
}


/*
 *	lamd_send
 *
 *	Function:	- moves one send request
 *	Accepts:	- request
 *			- blocking allowed?
 *	Returns:	- 0: no movement, 1: movement, -1: error
 */
static int
lamd_send(MPI_Request req, int flag_block)
{
  struct lam_ssi_rpi_req *lmq;
  struct nmsg *msg_data;
  int r;
/*
 * I believe the following condition should never happen.
 */
  if (req->rq_state == LAM_RQSDONE)
    return (0);

  lmq = req->rq_rpi;
  msg_data = &(req->rq_rpi->lmq_msg_data);

  if (flag_block) {
    lamd_fillfyi(req);
  }
/*
 * Send the short message.
 */
  if (lmq->lmq_state == LAMD_START) {
    r = lamd_dsend(msg_data, flag_block);
    if (r < 1)
      return (r);

    req->rq_state = LAM_RQSACTIVE;
    msg_data->nh_msg += msg_data->nh_length;
    lmq->lmq_ndata_moved = msg_data->nh_length;

    if (lam_ger) {
      req->rq_proc->p_ger_nsnd++;
    }
/*
 * We need an ack for this message.
 */
    if ((lmq->lmq_ndata > MAXNMSGLEN) || (req->rq_type == LAM_RQISSEND)) {
      lmq->lmq_state = LAMD_ACK;
    }
/*
 * We still need a flow message indicating an envelope slot is free.
 */
    else if (lam_ger && (req->rq_marks & LAM_RQFBLKTYPE) &&
	     (req->rq_proc->p_ger_nsnd == MPI_GER)) {
      lmq->lmq_state = LAMD_GER;
    } else {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }
/*
 * The envelope plus short message data has been sent.
 * The message is long, so receive the ack.
 */
  else if (lmq->lmq_state == LAMD_ACK) {
    r = lamd_bfrecv(&lmq->lmq_msg_ack, flag_block);
    if (r < 1)
      return (r);

    if (lam_ger) {
      req->rq_proc->p_ger_nsnd--;
    }
/*
 * Adjust message length according to information received from the
 * destination in the ack.
 */
    if (lmq->lmq_msg_ack.nh_data[0] < MAXNMSGLEN) {
      lmq->lmq_ndata = msg_data->nh_length;
    } else if (lmq->lmq_msg_ack.nh_data[0] < lmq->lmq_ndata) {
      lmq->lmq_ndata = lmq->lmq_msg_ack.nh_data[0];
    }
/*
 * If there is nothing more to send, we are done.
 */
    if (lmq->lmq_ndata_moved == lmq->lmq_ndata) {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    } else {
      lmq->lmq_state = LAMD_TAIL;
      msg_data->nh_flags |= N2ND;
      _m2l_tail(msg_data);
    }
  }
/*
 * The ack has been received.
 * The message is long, so send the message tail.
 */
  else if (lmq->lmq_state == LAMD_TAIL) {

    if ((lmq->lmq_ndata - lmq->lmq_ndata_moved) <= MAXNMSGLEN) {
      msg_data->nh_flags &= ~NMORE;
      msg_data->nh_length = lmq->lmq_ndata - lmq->lmq_ndata_moved;
    }

    r = lamd_dsend(msg_data, flag_block);
    if (r < 1)
      return (r);

    msg_data->nh_msg += msg_data->nh_length;
    lmq->lmq_ndata_moved += msg_data->nh_length;

    if (lmq->lmq_ndata_moved == lmq->lmq_ndata) {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }
/*
 * LAMD_GER:  Receive a flow control protocol message.
 */
  else {
    r = lamd_bfrecv(&lmq->lmq_msg_ger, flag_block);
    if (r < 1)
      return (r);

    req->rq_proc->p_ger_nsnd--;
    lmq->lmq_state = LAMD_DONE;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (1);
}


/*
 *	lamd_recv
 *
 *	Function:	- moves one recv request
 *	Accepts:	- request
 *			- blocking permission flag
 *	Returns:	- 0: no movement, 1: movement, -1: error
 */
static int
lamd_recv(MPI_Request req, int flag_block)
{
  struct lam_ssi_rpi_req *lmq;		/* lamd request */
  struct nmsg *msg_data;	/* data message */
  struct nmsg *msg_ack;		/* ack message */
  struct nmsg *msg_ger;		/* GER message */
  int i;
  int r;

  if (req->rq_state == LAM_RQSDONE)
    return (0);

  lmq = req->rq_rpi;
  msg_data = &(req->rq_rpi->lmq_msg_data);

  if (flag_block) {
    lamd_fillfyi(req);
  }
/*
 * Nothing has been received.
 */
  if (lmq->lmq_state == LAMD_START) {
    r = lamd_bfrecv(msg_data, flag_block);
    if (r < 1)
      return (r);

    req->rq_state = LAM_RQSACTIVE;
    req->rq_seq = msg_data->nh_data[3];
/*
 * If wildcards were present in nh_type, specific values will
 * overwrite them after a successful lamd_bfrecv().
 * Write the current values into the MPI_Status object.
 */
    if (req->rq_tag == MPI_ANY_TAG) {
      req->rq_status.MPI_TAG = _m2l_gettag(msg_data);
    } else {
      req->rq_status.MPI_TAG = req->rq_tag;
    }

    if (req->rq_rank == MPI_ANY_SOURCE) {
      i = msg_data->nh_data[1];
      req->rq_status.MPI_SOURCE = i;
      req->rq_proc = (LAM_IS_INTER(req->rq_comm)) ?
	req->rq_comm->c_rgroup->g_procs[i] :
	req->rq_comm->c_group->g_procs[i];
    } else {
      req->rq_status.MPI_SOURCE = req->rq_rank;
    }
/*
 * Check for truncation or reduce the destination data length.
 */
    if (msg_data->nh_length > lmq->lmq_ndata) {
      req->rq_flags |= LAM_RQFTRUNC;
      msg_data->nh_length = lmq->lmq_ndata;
    } else if (msg_data->nh_data[0] > lmq->lmq_ndata) {
      req->rq_flags |= LAM_RQFTRUNC;
    } else if (msg_data->nh_data[0] < lmq->lmq_ndata) {
      lmq->lmq_ndata = msg_data->nh_data[0];
    }

    msg_data->nh_msg += msg_data->nh_length;
    lmq->lmq_ndata_moved = msg_data->nh_length;

    req->rq_status.st_length = lmq->lmq_ndata;
/*
 * Move to the ack state if the source expects an ack.
 */
    if ((msg_data->nh_flags & NMORE) ||
	(msg_data->nh_data[2] & LAMD_SSEND)) {
      lmq->lmq_state = LAMD_ACK;
/*
 * Route the ack message.
 */
      msg_ack = &lmq->lmq_msg_ack;
      msg_ack->nh_node = req->rq_proc->p_gps.gps_node;
      if (lamd_route(msg_ack))
	return (LAMERROR);
/*
 * Fill the ack message with MPI synchronization.
 */
      _m2l_fill(lam_myproc, req->rq_proc,
		req->rq_status.MPI_TAG, req->rq_cid, msg_ack);
      _m2l_ack(msg_ack);
    }
/*
 * Move to the GER state.
 */
    else if (lam_ger) {
      lmq->lmq_state = LAMD_GER;
/*
 * Route the GER message.
 */
      msg_ger = &lmq->lmq_msg_ger;
      msg_ger->nh_node = req->rq_proc->p_gps.gps_node;
      if (lamd_route(msg_ger))
	return (LAMERROR);
/*
 * Fill the GER message with MPI/GER synchronization.
 */
      _m2l_fill(lam_myproc, req->rq_proc, MPI_GER_TAG,
		MPI_GER_CID, msg_ger);
    }

    else {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }
/*
 * The envelope plus short message data has been received.
 * The message is long, so send the ack.
 */
  else if (lmq->lmq_state == LAMD_ACK) {
    r = lamd_dsend(&lmq->lmq_msg_ack, flag_block);
    if (r < 1)
      return (r);

    if (lmq->lmq_ndata == lmq->lmq_ndata_moved) {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    } else {
      _m2l_tail(msg_data);
      lmq->lmq_state = LAMD_TAIL;
    }
  }
/*
 * The ack has been sent.
 * The message is long, so receive the message tail.
 */
  else if (lmq->lmq_state == LAMD_TAIL) {
    if ((lmq->lmq_ndata - lmq->lmq_ndata_moved) <= MAXNMSGLEN) {
      msg_data->nh_flags &= ~NMORE;
      msg_data->nh_length = lmq->lmq_ndata - lmq->lmq_ndata_moved;
    }

    r = lamd_bfrecv(msg_data, flag_block);
    if (r < 1)
      return (r);

    msg_data->nh_msg += msg_data->nh_length;
    lmq->lmq_ndata_moved += msg_data->nh_length;

    if (!(msg_data->nh_flags & NMORE)) {
      lmq->lmq_state = LAMD_DONE;
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }
/*
 * The message has been completely received.  Return a GER flow control
 * message to the source.
 */
  else {
    r = lamd_dsend(&lmq->lmq_msg_ger, flag_block);
    if (r < 1)
      return (r);

    lmq->lmq_state = LAMD_DONE;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (1);
}


/*
 *	lamd_bfrecv
 *
 *	Function:	- receives one packet from bufferd
 *			- indicates NMORE
 *	Accepts:	- LAM message
 *			- blocking flag
 *	Returns:	- 0 (no movement), 1 (recv'd pkt), -1 (error)
 */
static int
lamd_bfrecv(struct nmsg *msg, int flag_block)
{
  int4 save_flags;
  int4 save_length;
  int r;

  save_flags = msg->nh_flags;
/*
 * If non-blocking, probe first.
 */
  if (!flag_block) {

    r = bfiprobe(msg);
    if (r < 0)
      return (LAMERROR);

    if (r == 0) {
      return (0);
    }

    msg->nh_flags = save_flags;
  }
/*
 * Receive packet.
 */
  save_length = msg->nh_length;
  r = bfrecv(msg);

  if (r < 0) {
    msg->nh_flags = save_flags;
    return (LAMERROR);
  }
/*
 * If the caller specifies NMORE, return the NMORE flag of the incoming msg.
 */
  if (save_flags & NMORE) {
    msg->nh_flags = (save_flags & ~NMORE) | (msg->nh_flags & NMORE);
  } else {
    msg->nh_flags = save_flags;
  }

  msg->nh_length = LAM_min(save_length, msg->nh_length);
  return (1);
}


/*
 *	lamd_dsend
 *
 *	Function:	- sends one packet
 *	Accepts:	- LAM message
 *			- blocking flag
 *	Returns:	- 0 (no movement), 1 (recv'd pkt), -1 (error)
 */
static int
lamd_dsend(struct nmsg *msg, int flag_block)
{
  if (flag_block) {
    msg->nh_flags &= ~KTRY;
  } else {
    msg->nh_flags |= KTRY;
  }

  if (dsend(msg)) {

    if (errno == EWOULDBLOCK) {
      return (0);
    } else {
      return (LAMERROR);
    }
  }

  return (1);
}


/*
 *	lamd_noretry
 *
 *	Function:	- disables retry on LAM_SIGSHRINK
 */
static void
lamd_noretry(void)
{
  mask_retry = lam_ksigretry(0);
  mask_retry &= ~(lam_ksigmask(LAM_SIGSHRINK));
  mask_retry = lam_ksigsetretry(mask_retry);
}


/*
 *	lamd_retry
 *
 *	Function:	- enables retry on LAM_SIGSHRINK
 */
static void
lamd_retry(void)
{
  mask_retry = lam_ksigretry(lam_ksigmask(LAM_SIGSHRINK));
}


/*
 *	lamd_preempt
 *
 *	Function:	- disables/enables Trollius signal preemption
 *			- signals will still be picked up at kernel calls
 *	Accepts:	- preempt flag
 */
static void
lamd_preempt(int flag)
{
  sigset_t sigs_preempt;

  sigemptyset(&sigs_preempt);
  sigaddset(&sigs_preempt, LAM_SIGUSR);
  sigprocmask(flag ? SIG_UNBLOCK : SIG_BLOCK, &sigs_preempt,
	      (sigset_t *) 0);
}


/*
 *	lamd_commdead
 *
 *	Function:	- checks request for dead communicator
 *	Accepts:	- request
 *	Returns:	- 1 if dead, 0 if OK
 */
static int
lamd_commdead(MPI_Request req)
{
  if (req->rq_comm->c_flags & (LAM_CLDEAD | LAM_CRDEAD)) {
    if (req->rq_state != LAM_RQSDONE && req->rq_state != LAM_RQSINIT) {
      lam_rq_nactv--;
    }

    req->rq_state = LAM_RQSDONE;

    if (req->rq_comm->c_flags & LAM_CLDEAD) {
      req->rq_status.MPI_ERROR = lam_mkerr(MPI_ERR_LOCALDEAD, 0);
    } else {
      req->rq_status.MPI_ERROR = lam_mkerr(MPI_ERR_REMOTEDEAD, 0);
    }
    return (1);
  } else {
    return (0);
  }
}


/*
 *	lamd_shrink
 *
 *	Function:	- fault tolerance handler for SIGSHRINK
 */
static void   
lamd_shrink(int dummy_value)
{
  int i;
  struct _proc *proc;		/* a process */
  struct _proc **array;
  struct route rinfo;		/* router info */

  /* Invoke old handler to flush the route cache. */

  (*oldhandler)();

  array = lam_arr_get(procs);
  for (i = 0; i < lam_arr_size(procs); ++i) {
    proc = array[i];
    rinfo.r_nodeid = proc->p_gps.gps_node;

    if (getrent(&rinfo) && (errno == EBADNODE)) {
      proc->p_mode |= LAM_PDEAD;
      lam_ssi_rpi_commfault(rinfo.r_nodeid);
    }
  }
}


/*
 *	lamd_route
 *
 *	Function:	- routes an outgoing message
 *			- is wary of EBADNODE errors - lets them
 *			  pass through for later detection
 *	Accepts:	- LAM message
 */
static int
lamd_route(struct nmsg *msg)
{
  if ((msg->nh_node == LOCAL) || (msg->nh_node == getnodeid())) {
    msg->nh_dl_event = EVBUFFERD;
  } else {

    if (getroute(msg)) {

      if (errno != EBADNODE) {
	return (LAMERROR);
      }
    }
  }

  return (0);
}


/*
 *	lamd_finalize1
 *
 *	Function:	- cleans up process flow messages
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
static int
lamd_finalize1(struct _proc *p)
{
  struct nmsg msg_ger;
  int flag_block;
  int r;

  /* Remove this proc from the array of valid procs that this RPI is
     reponsible for */

  lam_arr_remove(procs, &p);

  /* Suck in all outstanding GER flow control messages. */

  if (lam_ger) {
    LAM_ZERO_ME(msg_ger);
    _m2l_fill(p, lam_myproc, MPI_GER_TAG, MPI_GER_CID, &msg_ger);
    msg_ger.nh_flags = (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
			? KTRACE : 0) | KSYNCSQL;
    msg_ger.nh_length = 0;

    flag_block = !(p->p_mode & LAM_PDEAD);

    /* Set process FYI to indicate we are finalizing. */

    ((struct _fyiproc *) &_kio.ki_fyi[0])->fyp_func = lam_getfunc();

    while (p->p_ger_nsnd > 0) {
      r = lamd_bfrecv(&msg_ger, flag_block);
      if (r < 0)
	return (LAMERROR);

      /* We don't insist on all flow messages from a dead node.  If it
         isn't in the local bufferd, it won't be coming. */

      if (r == 0) {
	p->p_ger_nsnd = 0;
      } else {
	p->p_ger_nsnd--;
      }
    }
  }

  return 0;
}
