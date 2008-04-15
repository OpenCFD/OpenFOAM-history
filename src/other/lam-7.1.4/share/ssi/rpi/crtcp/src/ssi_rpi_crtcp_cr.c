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
 * $Id: ssi_rpi_crtcp_cr.c,v 1.12 2003/11/04 04:13:31 pcharapa Exp $
 *
 *    Function:       - CRTCP checkpoint/restart interface
 */

#include <lam_config.h>

#include <sys/param.h>
#include <string.h>
#include <signal.h>

#include <lam_network.h>
#include <blktype.h>
#include <etc_misc.h>

#include <lam-ssi-rpi-crtcp.h>
#include <rpi_crtcp.h>
#include <crtcp_low.h>
#include <rpi_crtcp_bookmarks.h>


/*
 * Global variables
 */
bookmark_payload_t *PUB(bookmarks);
#if 0
int PUB(read_and_yield_back) = 0;
#endif


/*
 * external variables
 */
#if 0
extern int PUB(is_sread_in_fast);
#endif


/* 
 * local functions
 */
static int advance_state_machine(void);
#if 0
static int quiesce_advance_fast(void);
#endif
static int exchange_bookmarks(void);
static int advance_receives(void);
static int do_barrier(void);
static int send_pending_long_acks(void);

static void save_sock_info(void);
static int restore_sock_info(void);
static void signal_handler(int);

/*
 * external functions
 */
extern int lam_send_pid_idx(struct nmsg *nhead);
extern int lam_send_selected_ssi_modules(void);


/*
 * local variables
 */
static int signal_handler_done = 0;


/*
 *    lam_ssi_rpi_crtcp_checkpoint
 *
 *    Function:       - perform actions for checkpoint
 *    Returns:        - 0: on success, LAMERROR on error
 */
int
lam_ssi_rpi_crtcp_checkpoint(void)
{
  int i;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "inside crtcp_checkpoint:%d\n", getpid());

  /* Allocate the array of bookmarks */
  PUB(bookmarks) = malloc(sizeof(bookmark_payload_t) * PUB(nprocs));
  if (NULL == PUB(bookmarks))
    return LAMERROR;

  if (exchange_bookmarks() < 0)
    return LAMERROR;

  if (PUB(verbose) >= 40)
    for (i = 0; i < PUB(nprocs); ++i)
      lam_debug(PUB(did), "received from %d: (%u)... actual: (%u)",
                i, 
                PUB(bookmarks)[i].bp_out_nbytes, 
                PUB(procs)[i]->p_rpi->cp_in_nbytes);

  /* Now that we've sent/received all the bookmarks, advance receives
     until we match.  Do this in order to avoid deadlock. */
  if (advance_receives() < 0)
    return LAMERROR;

  /* Ditch the bookmarks */
  free(PUB(bookmarks));

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "quiesce complete");

  /* 
   * Save all socket params before calling back into the kernel 
   * This will be restored only in RESTART, because nothing changes in the
   * CONTINUE case
   */
  save_sock_info();

  return 0;
}


/*
 *    lam_ssi_rpi_crtcp_continue
 *
 *    Function:       - perform actions for continue
 *    Returns:        - 0: on success, LAMERROR on error
 */
int
lam_ssi_rpi_crtcp_continue(void)
{
  /* Don't need to do anything special for FAILURE/CONTINUE */
  if (PUB(verbose) >= 30)
    lam_debug(PUB(did), "APP_ASYNC%d: cr_checkpoint returned CONTINUE...",
              getpid());

  /* do stuff that is common to RESTART/CONTINUE/FAILURE */ 
  if (do_barrier() < 0)
    return LAMERROR;

  if (send_pending_long_acks() < 0)
    return LAMERROR;

  return 0;
}


/*
 *    lam_ssi_rpi_crtcp_restart
 *
 *    Function:       - perform actions for restart
 *    Returns:        - 0: on success, LAMERROR on error
 */
int
lam_ssi_rpi_crtcp_restart(void)
{
  struct nmsg nhead;
  struct _gps *procs;
  int procs_n;
  int world_n, cid;
  struct _proc **crtcp_procs = NULL;
  int i;
  
  if (PUB(verbose) >= 30)
    lam_debug(PUB(did), "APP_ASYNC%d: cr_checkpoint returned RESTART...",
      	getpid());  

  /* 
   * Attach to lamd.  We need to reset _kio.ki_pid, so that we can attach to
   * the lamd afresh.
   */
  _kio.ki_pid = -1;

  if (kenter(lam_argv0, 0)) {
    /* ENOKERNEL = no kernel */
    /* ENOKDESCRIPTORS = kernel descriptors full */
    if (errno == ENOKERNEL) {
      char hostname[MAXHOSTNAMELEN];
      gethostname(hostname, MAXHOSTNAMELEN);
      show_help("MPI", "no-lamd", "MPI_Init", hostname, NULL);
    } else if (errno == ENOKDESCRIPTORS) {
      char hostname[128];
      char num[128];
      lam_gethostname(hostname, 128);
      if (PUB(verbose) >= 30)
        lam_debug(PUB(did), num, 127, "%d", KPMAX);
      show_help("MPI", "lamd-full", "MPI_Init", num, hostname, NULL);
    }
  }

  /*
   * send back my real PID and IDX to mpirun. 
   */
  LAM_ZERO_ME(nhead);
  if (lam_send_pid_idx(&nhead)) {
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: Initialization  failure at restart" 
      	  "(send_pid_idx).", getpid());
    return LAMERROR;
  }

  /*
   * receive the list of GPS from mpirun. However, we don't need to update
   * any data-structure because the GPS is identical to the one before
   * checkpoint.
   */
  procs_n = 0;
  if (lam_get_proc_array(&procs, &world_n, &cid, &procs_n, 0) == LAMERROR) {
    free((char *) procs);
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: Initialization  failure at restart" 
      	  "(lam_get_proc_array).", getpid());
    return LAMERROR;
  }

  nhead.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  nhead.nh_type = BLKMPIINIT;
  nhead.nh_flags = DINT4DATA;
  nhead.nh_length = procs_n * sizeof(struct _gps);
  nhead.nh_msg = (char *) procs;
  
  if (nrecv(&nhead)) {
    free((char *) procs);
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: Initialization  failure at restart" 
      	  "(receive of procs array).", getpid());
    return LAMERROR;
  }
  
  /*
   * Synchronize clocks across the nodes.
   */
  if (_kio.ki_rtf & RTF_TRACE) {
    if (lam_clocksync(procs_n, procs, &lam_clockskew)) {
      if (PUB(verbose) >= 30)
        lam_debug(PUB(did), "APP_ASYNC%d: lam_clocksync failed at restart.", 
                  getpid());
      return LAMERROR;
    }
  }

  /* Figure out endian-ness of all my peers */

  crtcp_procs = malloc(sizeof(struct _proc *) * PUB(nprocs));
  if (NULL == crtcp_procs) {
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), "alloc of crtcp_procs failed.");
    return LAMERROR;
  }

  for (i = 0; i < PUB(nprocs); ++i)
    crtcp_procs[i] = PUB(procs)[i];

  lam_exchange_endian_info(crtcp_procs, PUB(nprocs));

  /*
   * interface specific initialization
   *
   * Currently, we pass a copy of PUB(procs) to the addprocs function.
   * However, if we have to restart on a different set of nodes, we need to
   * pass the appropriate parameters to addprocs
   */

  if (PUB(verbose) >= 30)
    lam_debug(PUB(did), "APP_ASYNC%d: before lam_ssi_rpi_crtcp_addprocs(), "
      	"lam_myproc->p_gps.gps_node = %d\n", 
      	getpid(), lam_myproc->p_gps.gps_node);

  if (lam_ssi_rpi_crtcp_addprocs(crtcp_procs, PUB(nprocs))) {
  
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: lam_ssi_rpi_crtcp_addprocs failed"
                " at restart.", getpid());
    return LAMERROR;
  }
  
  /*
   * send the list of selected SSI modules to mpirun.  This step is bound
   * to succeed, 'cos we're coming back from a checkpoint after all.
   * However, mpirun is waiting on an nrecv() from us.  So we need to send
   * some message to it to satisfy its nrecv()
   */
  if (lam_send_selected_ssi_modules() < 0) {
    if (PUB(verbose) >= 0)
      lam_debug(PUB(did), "APP_ASYNC%d: Send of selected SSI modules to "
              "mpirun failed.", getpid());
    return LAMERROR;
  }
  
  /* 
   * Restore the socket params that were saved before calling
   * cr_checkpoint()
   */
  if (restore_sock_info()) {
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: Initialization  failure at restart" 
      	  "(restore_sock_info)", getpid());
    return LAMERROR;
  }
    
#if 0
  /*
   * Synchronize clocks across the nodes.
   */
  if (_kio.ki_rtf & RTF_TRACE) {
    if (lam_clocksync(procs_n, procs, &lam_clockskew)) {
      if (PUB(verbose) >= 30)
        lam_debug(PUB(did), "APP_ASYNC%d: lam_clocksync failed at restart.", 
                  getpid());
      return LAMERROR;
    }
  }
#endif

  /* do stuff that is common to RESTART/CONTINUE/FAILURE */ 
  if (do_barrier() < 0)
    return LAMERROR;

  if (send_pending_long_acks() < 0)
    return LAMERROR;

  return 0;
}


int 
lam_ssi_rpi_crtcp_interrupt(void)
{
  struct sigaction sa, old_sa;
  if (PUB(verbose) >= 30)
    lam_debug(PUB(did), "APP_ASYNC%d: Interrupt request received from C/R:"
              " killing pid %d", getpid(), lam_getpid());

  
  sa.sa_handler = signal_handler;
  if (sigemptyset(&(sa.sa_mask)) != 0) {
      lam_debug(PUB(did), "%d: Sigempty set failed on %d. Aborting",
                getpid(), lam_getpid());
      return LAMERROR;
  }

  /* register the SIGUSR1 handler here (save the old value) */

  sa.sa_flags = 0;
  if (sigaction(SIGUSR1, &sa, &old_sa) != 0) {
      lam_debug(PUB(did), "%d: Sigaction failed on %d. Aborting",
                getpid(), lam_getpid());
      return LAMERROR;
  }

  /* Kill / interrupt */
 
  signal_handler_done = 0;
  if (kill(lam_getpid(), SIGUSR1)) {
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: kill failed on pid%d. Aborting",
                getpid(), lam_getpid());
  
    if (sigaction(SIGUSR1, &old_sa, (struct sigaction *) 0) != 0) 
      lam_debug(PUB(did), "%d: Sigaction failed on %d. Aborting",
                getpid(), lam_getpid());
    return LAMERROR;
  }


  /* Note that the signal may not be delivered immediately.  So even
     though we get here, the signal handler may not have fired yet.
     Since we need to un-register this handler before we leave this
     function, wait until the signal handler files before
     un-registering it. */

  while (signal_handler_done == 0)
    lam_yield();

  /* Restore the old SIGUSR1 handler */

  if (sigaction(SIGUSR1, &old_sa, (struct sigaction *) 0) != 0) {
    lam_debug(PUB(did), "%d: Sigaction failed on %d. Aborting", 
	      getpid(), lam_getpid()); 
    return LAMERROR;
  }

  return 0;
}



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
#endif

/*
 * Empty signal handler.  We don't need to do anything here -- just need to
 * provide a handler for SIGUSR1 so that we can interrupt the app thread if it
 * is executing a blocking syscall.
 */
static void
signal_handler(int signum)
{
  signal_handler_done = 1;
}

static int
exchange_bookmarks(void)
{
  /*
   * NOTE: the bookmark_exchange functions return the bookmarks in the same
   * order as that of the procs in the PUB(procs) array.
   */
  if (PUB(bookmark_exchange_alltoall)(PUB(bookmarks)) < 0) {
    free(PUB(bookmarks));
    return LAMERROR;
  }

  return 0;
}


static int
advance_receives(void)
{
  int i;
  int done;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "iterating over advance");
  do {

    /* Check all the bookmarks.  Inefficient, but... */
    done = 1;
    for (i = 0; i < PUB(nprocs); ++i)
      if (PUB(procs)[i] != lam_myproc)
        if (PUB(procs)[i]->p_rpi->cp_in_nbytes 
            != PUB(bookmarks)[i].bp_out_nbytes) {
          done = 0;
          if (PUB(verbose) >= 20)
            lam_debug(PUB(did),
		      "quiesce not done; with index %d -- at %u, "
		      "bookmark = %u",
		      i,
		      PUB(procs)[i]->p_rpi->cp_in_nbytes,
		      PUB(bookmarks)[i].bp_out_nbytes);
        }

    /* If we haven't matched all the bookmarks, advance */
    if (!done) {
#if 0
      if (PUB(is_sread_in_fast)) {
        if (quiesce_advance_fast() == LAMERROR)
          return LAMERROR;
      } else {
#endif
        if (advance_state_machine() == LAMERROR)
          return LAMERROR;
#if 0
      }
#endif
    }
  } while (!done);
  
  return 0;
}


static int
do_barrier(void)
{
  if (PUB(verbose) >= 30)
    lam_debug(PUB(did), "APP_ASYNC%d: before low-level barrier", getpid());
  if (lam_nbarrier(PUB(procs), PUB(nprocs))) {
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did), "APP_ASYNC%d: barrier failed at "
                "restart/continue/failure", getpid());
    return LAMERROR;
  }

  return 0;
}


static int
send_pending_long_acks(void)
{
  /*
   * During the quiesce, it is possible that we received the first
   * envelope of a long message, and therefore queued up an ACK to
   * send back to the sender.  This may actually have happened
   * multiple times, and so we may have multiple ACK's queued to
   * send out.  Call back into _low to send out all these ACK's.
   */
  
  if (lam_ssi_rpi_crtcp_send_long_ack_queue() != 0) {
    if (PUB(verbose) >= 30)
      lam_debug(PUB(did),
		"APP_ASYNC%d: lam_ssi_rpi_crtcp_send_long_ack_queue failed"
		"at restart/continue.", getpid());
    return LAMERROR;
  }
  
  return 0;
}


static int
advance_state_machine(void)
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

    if (lam_ssi_rpi_comm_dead_m(req)) {
      PUB(haveadv) = 1;
      continue;
    }

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

  lam_memcpy((char *) &PUB(read), (char*) &PUB(full_fd_set), sizeof(fd_set));
  lam_memcpy((char *) &PUB(except), (char*) &PUB(full_fd_set), sizeof(fd_set));

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
save_sock_info(void)
{
  int i;

  for (i = 0; i < PUB(nprocs); ++i) {
    if (PUB(procs)[i] != lam_myproc) {
      if (FD_ISSET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(read))) {
        PUB(procs)[i]->p_rpi->cp_sockinfo |= ISSET_READ;
      }
      if (FD_ISSET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(write))) {
        PUB(procs)[i]->p_rpi->cp_sockinfo |= ISSET_WRITE;
      }
      if (FD_ISSET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(except))) {
        PUB(procs)[i]->p_rpi->cp_sockinfo |= ISSET_EXCEPT;
      }
      if (FD_ISSET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(eoferr))) {
        PUB(procs)[i]->p_rpi->cp_sockinfo |= ISSET_EOFERR;
      }
      if (FD_ISSET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(block))) {
        PUB(procs)[i]->p_rpi->cp_sockinfo |= ISSET_BLOCK;
      }
    }
  }
  PUB(flblock) = 0;

  return;
}


static int
restore_sock_info(void)
{
  int i;
  int sock_block;

  PUB(sockmax) = -1;

  FD_ZERO(&PUB(read));
  FD_ZERO(&PUB(write));
  FD_ZERO(&PUB(except));
  FD_ZERO(&PUB(eoferr));
  FD_ZERO(&PUB(block));
  for (i = 0; i < PUB(nprocs); ++i) {
    if (PUB(procs)[i] != lam_myproc) {
      if (PUB(procs)[i]->p_rpi->cp_sockinfo & ISSET_READ)
        FD_SET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(read));
      if (PUB(procs)[i]->p_rpi->cp_sockinfo & ISSET_WRITE)
        FD_SET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(write));
      if (PUB(procs)[i]->p_rpi->cp_sockinfo & ISSET_EXCEPT)
        FD_SET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(except));
      if (PUB(procs)[i]->p_rpi->cp_sockinfo & ISSET_EOFERR)
        FD_SET(PUB(procs)[i]->p_rpi->cp_sock, &PUB(eoferr));
      if (PUB(procs)[i]->p_rpi->cp_sockinfo & ISSET_BLOCK)
        sock_block = 1;
      else
        sock_block = 0;

      lam_ssi_rpi_crtcp_setsockblk(PUB(procs)[i]->p_rpi->cp_sock, sock_block);

      /* handling for sockmax */
      if (PUB(procs)[i]->p_rpi->cp_sock > PUB(sockmax))
        PUB(sockmax) = PUB(procs)[i]->p_rpi->cp_sock;

      /* reset cp_sockinfo to 0 after we're done*/
      PUB(procs)[i]->p_rpi->cp_sockinfo = 0;
    }
  }

  return 0;
}
