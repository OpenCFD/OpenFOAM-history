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
 * $Id: ssi_rpi_gm_cr.c,v 1.7 2003/11/12 15:48:07 jsquyres Exp $
 *
 *    Function:       - GM checkpoint/restart interface
 *
 * Checkpoint/Continue/Restart scheme for gm
 * *****************************************
 *
 * Checkpoint:
 * ----------- 
 *   When a checkpoint request comes in, the appropriate crmpi SSI
 *   module invokes the lam_ssi_rpi_gm_checkpoint() function to enable
 *   the gm rpi SSI module to shutdown before taking the checkpoint.
 *
 *   The first thing that is done in the rpi during checkpoint is the
 *   exchange of bookmarks; each process sends out a count of the
 *   number of bytes it has sent to each peer (send bookmark).
 *
 *   If the number of bytes received from a peer till now (receive
 *   bookmark) is less than the actual number of bytes sent by that
 *   peer, these pending bytes need to be received at this point.
 *   This is achieved as follows:
 *   - A global variable (lam_ssi_rpi_gm_enqueue_sends) is set to
 *     indicate that no messages will be sent on the wire before
 *     checkpoint.
 *   - The gm state machine is advanced by invoking
 *     lam_ssi_rpi_gm_advance(); all sends are queued up to be sent
 *     later, while all the pending messages are received till all the
 *     receive bookmarks match the corresponding numbers sent by the
 *     respective peer.
 *    
 *   Then, a shutdown of the gm network is performed.  This involves
 *   backing up the DMA memory and destroying the pre-allocated env
 *   and short DMA buffers (DMA memory is not preserved across a
 *   checkpoint), deleting the pinned memory lists, and closing the GM
 *   port.
 *
 * Continue:
 * ---------
 *   GM is re-initialized.  This involves pre-allocating env and short
 *   DMA buffers, restoring the backed up DMA memory, initializing the
 *   pinned memory lists, opening a new GM port, providing a fixed
 *   number of env and short receive buffers, and exchanging the new
 *   port information with all other processes.
 *
 * Restart:
 * --------
 *   Restarting after a checkpoint involves performing most of the
 *   initialization that is done during MPI_Init, and also the
 *   GM-specific re-initialization that is done while continuing from
 *   a checkpoint (see above).
 *  
 * Note that while the scheme described above works as is for tiny and
 * short messages, there is some extra work that needs to be done to
 * handle pending requests associated with long messages during
 * checkpoint.
 *
 * There are actually two different protocols for long messages in
 * LAM/MPI.  Versions of gm prior to 2.0 did not include RDMA "get"
 * functionality, and therefore long messages use the following
 * put-based protocol:
 *
 * 1. A SENDER of a long message sends an envelope to the receiver
 *    indicating that there is a long message intended for the receiver.
 * 2. When the RECEIVER posts a receive for this long message, a DMA
 *    buffer of appropriate size is found or allocated, and a pointer to
 *    this buffer is sent in an ACK to the sender.
 * 3. Upon receiving this ACK, the SENDER initiates a put from the
 *    source buffer to the target buffer.  This writes the message
 *    directly to the remote DMA memory location at the RECEIVER's
 *    end.
 * 4. When the put completes, the SENDER sends an envelope to the
 *    RECEIVER to indicate the completion of the put, and both
 *    requests can complete.
 *
 * However, if the gm rpi module configure script determines that
 * gm_get() is available (it first became available in gm 2.0), the
 * following algorithm is used instead:
 *
 * 1. A SENDER of a long message sends an envelope to the receiver
 *    indicating that there is a long message intended for the receiver.
 *    The envelope includes a pointer to the source buffer.
 * 2. When the RECEIVER posts a receive for this long message, a DMA
 *    buffer of appropriate size is found or allocated.  An RDMA get
 *    is intiated to retrieve the message body from the source buffer
 *    in the remote process to the target buffer.
 * 3. When the RDMA get completes, the RECEIVER sends an ACK back to
 *    the sender, indicating that the transfer has completed, and both
 *    requests can complete.
 *
 * Checkpoint/restart is not implemented for the put-based protocol
 * (therefore, LAM/MPI jobs using gm before version 2.0 are not
 * checkpointable).  The get-based protocol checkpoint/restart
 * implementation is outlined below.
 *
 * RDMA creates a problem for checkpoint/restart.  Essentially: as
 * soon as a pointer to a buffer is sent to a remote process, if a
 * checkpoint request arrives, that pointer will be stale upon
 * continue/restart (since DMA-able buffers are not necessarily
 * maintained across continue/restart).
 *
 * Here's a schematic of all points in the get-based long protocol
 * (more detailed than the outline, above), broken down more-or-less
 * by atomic functions:
 *
 * 1. Sender sends (envelope+pointer).
 *    Sender increments sent bookmark (for the envelope+pointer).
 * 2. Sender completes sending (envelope+pointer).
 * 3. Receiver receives the (envelope+pointer).
 *    Receiver increments received bookmark (for the envelope+pointer).
 * 4. Receiver initiates RDMA get.
 * 5. Receiver completes RDMA get.
 * 6. Receiver sends ACK.
 *    Receiver increments sent bookmark (for the ACK).
 *    Receiver increments received bookmark (for the get).
 * 7. Receiver completes sending ACK.
 * 8. Sender receives ACK.
 *    Sender increments received bookmark (for the ACK).
 *    Sender increments sent bookmark (for the get).
 *
 * Here's a breakdown of what happens if a checkpoint arrives
 * immediately after each point:
 *
 * 1. The bookmark exchange will force progression to #3.
 * 2. The bookmark exchange will force progression to #3.
 * 3. ...........??? this is the question right now
 * 4. Callback completion will force progression to #5.
 * 5. Ok to checkpoint here.
 * 6. The bookmark exchange will force progression to #8.
 * 7. The bookmark exchange will force progression to #8.
 * 8. Ok to checkpoint here.
 *
 * So there's really only one "bad" place to [ultimately] receive a
 * checkpoint -- #3.  This is where an envelope has been received and
 * it contains a pointer that will be stale upon restart.  The
 * situation gets a little more complicated because at #3, the message
 * may be expected or unexpected by the receiver.
 *
 * Unexpected: The envelope is placed on the unexepcted queue.  We
 * *could* simiply initialy an RDMA get and have a special, small
 * gm_receive() loop just waiting for that get to complete, and then
 * attach it to the proper envelope in the unexpected queue.  However,
 * we might run out of resources in the process (DMA-able or regular
 * memory).  This would introduce an error that is specific to
 * checkpointing the application (i.e., it might not happen if we had
 * not checkpointed -- if the user has carefully ordered very large
 * sends and receives), and is therefore not acceptable.  Indeed,
 * failing *during* a checkpoint pretty much defeats the point.
 *
 * Expected: A matching request is found and we're ready to initiate
 * the RDMA get.  But the global flag will be set and the get "send"
 * request will simply be queued up (bookmarks will match, allowing
 * this to happen).
 *
 * It seems that the best solution for both cases is to refresh the
 * stale pointer during continue/restart.  This allows normal message
 * transfer semantics (i.e., no additional temporary buffers are
 * necessary).  This is a bit more annoying implementation-wise,
 * because we have to chase down all such pointers that need to be
 * refreshed, Unfortunately, only the receiver knows if this case
 * occurs.  So it's the receiver's responsibility to chase them all
 * down and request refreshes from the sender.
 *
 * Hence, during continue/restart, each process does a pairwise
 * exchange itemizing all stale pointers that need to be refreshed.
 * Each process responds to its peer's request(s) with new pointers.
 *
 * *Then* we can continue.
 *
 * Isn't that simple?  :-)
 */ 

#include <lam_config.h>
#include <sys/param.h>

#include <lam_network.h>
#include <blktype.h>
#include <etc_misc.h>
#include <rpisys.h>

#include <lam-ssi-rpi-gm.h>
#include <rpi_gm.h>
#include <rpi_gm_actions.h>
#include <rpi_gm_interval.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_cr.h>
#include <rpi_gm_cbuf.h>
#include <net.h>


/*
 * Global variables 
 */
bookmark_payload_t *PUB(bookmarks) = NULL;


/*
 * private variables
 */
static int nprocs = -1;
static struct _proc **procarr = NULL;


/*
 * private functions 
 */
static int exchange_bookmarks(void);
static int advance_receives(void);
static int do_gm_shutdown(void);
static int do_gm_reinit(struct _proc**, int);
static int backup_dma_memory(void);
static int restore_dma_memory(void);
static int refresh_stale_pointers(struct _proc**, int);
static int send_stale_unexpected(struct _proc *p, struct nmsg *inmsg, 
                                 struct nmsg *outmsg, void **buffer);
static int send_stale_pending(struct _proc *p, struct nmsg *inmsg, 
                              struct nmsg *outmsg, void **buffer);
static int receive_stale(struct _proc *p, struct nmsg *inmsg, 
                         struct nmsg *outmsg, void **buffer);


/*
 * external functions
 */
extern int lam_send_pid_idx(struct nmsg *nhead);
extern int lam_send_selected_ssi_modules(void);


/*
 *    lam_ssi_rpi_gm_checkpoint
 *
 *    Function:       - perform actions for checkpoint
 *    Returns:        - 0 on success, LAMERROR on error
 */
int 
lam_ssi_rpi_gm_checkpoint(void)
{ 
  procarr = lam_arr_get(PUB(procs));
  nprocs = lam_arr_size(PUB(procs));
 
  /* First, advance until all gm callbacks are done */

  lam_debug(PUB(did), 
            "checkpoint: waiting for callbacks to complete (%d vs. %d)",
            PUB(max_stokens), PUB(stokens));
  while (PUB(max_stokens) < PUB(stokens)) {
    if (lam_ssi_rpi_gm_advance(lam_rq_top, 1) < 0)
      return LAMERROR;
  }

#if 1
  if (lam_ssi_rpi_verbose >= 40) {
    int i;
    lam_debug(PUB(did), "checkpoint: GM port info for all procs:");
    for (i = 0; i < nprocs; ++i)
      lam_debug(PUB(did), "checkpoint:  Idx %d (Rank %d): GM port id: %d",
                i, 
                procarr[i]->p_gps.gps_grank, 
                procarr[i]->p_rpi->cp_gm_port.rgp_port_id); 
  }
#endif

  /* Allocate the array of bookmarks */

  PUB(bookmarks) = malloc(sizeof(bookmark_payload_t) * nprocs);
  if (NULL == PUB(bookmarks))
    return LAMERROR;

  if (exchange_bookmarks() < 0)
    return LAMERROR;
#if 1
  if (lam_ssi_rpi_verbose >= 40) {
    int i;
    for (i = 0; i < nprocs; ++i)
      lam_debug(PUB(did), "checkpoint: received from %d: (%u); actual: (%u)",
                i, 
                PUB(bookmarks)[i].bp_out_nbytes,
                procarr[i]->p_rpi->cp_in_nbytes);
  }
#endif

  /* Now that we've sent/received all the bookmarks, advance receives
     until we match.  Do this in order to avoid deadlock. */

  if (advance_receives() < 0)
    return LAMERROR;
  
  /* Ditch the bookmarks */

  free(PUB(bookmarks));

  if (lam_ssi_rpi_verbose >= 20)
    lam_debug(PUB(did), "checkpoint: complete");

  /* shutdown gm - backup and free dma memory, free interval lists,
     and close gm port */

  return do_gm_shutdown();
}


/*
 *    lam_ssi_rpi_gm_continue
 *
 *    Function:       - perform actions for continue
 *    Returns:        - 0 on success, LAMERROR on error
 */
int 
lam_ssi_rpi_gm_continue(void)
{ 
#if 1
  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "continue: cr_checkpoint returned CONTINUE");
#endif

  /* Re-init gm, re-open a port, obtain port info from all procs, and
     do the "right" things to pinned/dma memory. */

  if (do_gm_reinit(procarr, nprocs) < 0)
    return LAMERROR;

#if 1
  if (lam_ssi_rpi_verbose >= 40) {
    int i;
    lam_debug(PUB(did), "continue: GM port info for all procs:");
    for (i = 0; i < nprocs; ++i)
      lam_debug(PUB(did), "continue:  Idx %d (Rank %d): GM port id: %d",
                i, 
                procarr[i]->p_gps.gps_grank, 
                procarr[i]->p_rpi->cp_gm_port.rgp_port_id); 
    lam_debug(PUB(did), "continue: returning control to the app_thread");
  }
#endif

  return 0;
}


/*
 *    lam_ssi_rpi_gm_restart
 *
 *    Function:       - perform actions for restart
 *    Returns:        - 0 on success, LAMERROR on error
 */
int 
lam_ssi_rpi_gm_restart(void)
{ 
  struct nmsg nhead;
  struct _gps *procs;
  int procs_n;
  int world_n, cid;
  struct _proc **gm_procs = NULL;
  int i;
  
  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "restart: cr_checkpoint returned RESTART");

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
      if (lam_ssi_rpi_verbose >= 20)
        lam_debug(PUB(did), num, 127, "%d", KPMAX);
      show_help("MPI", "lamd-full", "MPI_Init", num, hostname, NULL);
    }
  }

  /*
   * send back my real PID and IDX to mpirun. 
   */
  LAM_ZERO_ME(nhead);
  if (lam_send_pid_idx(&nhead)) {
    if (lam_ssi_rpi_verbose >= 30)
      lam_debug(PUB(did), "restart: Init failure at restart (send_pid_idx)");
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
    if (lam_ssi_rpi_verbose >= 30)
      lam_debug(PUB(did), "restart: Init failure at restart "
                "(lam_get_proc_array)");
    return LAMERROR;
  }

  nhead.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  nhead.nh_type = BLKMPIINIT;
  nhead.nh_flags = DINT4DATA;
  nhead.nh_length = procs_n * sizeof(struct _gps);
  nhead.nh_msg = (char *) procs;
  
  if (nrecv(&nhead)) {
    free((char *) procs);
    if (lam_ssi_rpi_verbose >= 30)
      lam_debug(PUB(did), "restart: Initfailure at restart" 
                "(receive of procs array)");
    return LAMERROR;
  }
  
  /*
   * Synchronize clocks across the nodes.
   */
  if (_kio.ki_rtf & RTF_TRACE) {
    if (lam_clocksync(procs_n, procs, &lam_clockskew)) {
      if (lam_ssi_rpi_verbose >= 30)
        lam_debug(PUB(did), "restart: lam_clocksync failed at "
                  "restart");
      return LAMERROR;
    }
  }

  /* Figure out endian-ness of all my peers */

  gm_procs = malloc(sizeof(struct _proc *) * nprocs);
  if (NULL == gm_procs) {
    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(PUB(did), "restart: alloc of gm_procs failed.");
    return LAMERROR;
  }

  /* XXX: SS: why do we do this??? */
  for (i = 0; i < nprocs; ++i)
    gm_procs[i] = procarr[i];

  lam_exchange_endian_info(gm_procs, nprocs);

  /* 
   * gm interface-specific re-initialization 
   * 
   * Init gm, open a gm port, obtain port info from all procs, and do the
   * "right" things to pinned/dma memory.  Note that unlike in the case of
   * MPI_Init, this port is not opened during _query.  Hence, the port
   * information is directly written to lam_myproc->p_rpi->cp_gm_port,
   * instead of using a local var to store this information and then copying
   * it over to lam_myproc->p_rpi->cp_gm_port 
   *
   * XXX: SS: need to replace gm_procs with procarr.
   */
  if (do_gm_reinit(gm_procs, nprocs) < 0)
    return LAMERROR;

  /*
   * send the list of selected SSI modules to mpirun.  This step is bound
   * to succeed, 'cos we're coming back from a checkpoint after all.
   * However, mpirun is waiting on an nrecv() from us.  So we need to send
   * some message to it to satisfy its nrecv()
   */
  if (lam_send_selected_ssi_modules() < 0) {
    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(PUB(did), "restart: Send of selected SSI modules "
                "to mpirun failed.");
    return LAMERROR;
  }
  
#if 1
  /*
   * Synchronize clocks across the nodes.
   */
  if (_kio.ki_rtf & RTF_TRACE) {
    if (lam_clocksync(procs_n, procs, &lam_clockskew)) {
      if (lam_ssi_rpi_verbose >= 20)
        lam_debug(PUB(did), "restart: lam_clocksync failed at restart");
      return LAMERROR;
    }
  }
#endif
  
  return 0;
}


int
lam_ssi_rpi_gm_interrupt(void)
{
  /* The gm RPI module doesn't use any blocking calls. */

  return 0;
}


/* 
 * XXX: SS: this is an exact replica of the one in crtcp, except for the
 * additional procs parameters (this disparity needs to go away and crtcp
 * must also take procs parameters instead of using PUB(procs)).  Need to do
 * something to eliminate this duplicated code.
 */
static int
exchange_bookmarks(void)
{
  if (PUB(bookmark_exchange_alltoall)(procarr, nprocs, PUB(bookmarks)) < 0) {
    free(PUB(bookmarks));
    return LAMERROR;
  }

  return 0;
}


static int
advance_receives(void)
{
  int cur = 0;
  int flblock_save = lam_ssi_rpi_gm_flblock; 

  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "iterating over advance");

  lam_ssi_rpi_gm_enqueue_sends = 1;
  while (cur < nprocs) {

    /* Advance if one or more bookmarks don't match. */

    if ((procarr[cur] == lam_myproc) || 
        (procarr[cur]->p_rpi->cp_in_nbytes == 
         PUB(bookmarks)[cur].bp_out_nbytes)) {
        if (lam_ssi_rpi_verbose >= 20)
          lam_debug(PUB(did), "quiesce: done with index %d", cur);
        ++cur;
        continue;
    } else {
      if (lam_ssi_rpi_verbose >= 20)
        lam_debug(PUB(did), "quiesce: NOT DONE with index %d -- "
                  "at %u, bookmark = %u",
                  cur,
                  procarr[cur]->p_rpi->cp_in_nbytes,
                  PUB(bookmarks)[cur].bp_out_nbytes);

      /* Advance */

      if (lam_ssi_rpi_gm_advance(lam_rq_top, 0) < 0)
        return LAMERROR;

      if (lam_ssi_rpi_verbose >= 30)
        lam_debug(PUB(did), "quiesce: gm advanced receives; index %d "
                  " now at %u, haveadv: %d",
                  cur,
                  procarr[cur]->p_rpi->cp_in_nbytes,
                  lam_ssi_rpi_gm_haveadv); 
    }        
  }

  /* restore lam_ssi_rpi_gm_flblock and reset
     lam_ssi_rpi_gm_enqueue_sends */

  lam_ssi_rpi_gm_flblock = flblock_save;
  lam_ssi_rpi_gm_enqueue_sends = 0; 

  return 0;
}


static int
do_gm_shutdown(void)
{
  int i;

  /* mark all processes as not connected */

  for (i = 0; i < nprocs; ++i)
    procarr[i]->p_mode &= ~LAM_PRPICONNECT;

  /* 
   * - Memory obtained using gm_dma_malloc() will not be accessible at
   *   restart.  So backup envelope(s) of all requests to be restored at
   *   restart.  For short/long messages where dma_data_buffer != NULL, a
   *   copy of the message body is present in rq_packbuf anyway; so they
   *   need not be backed up.  
   *
   * - Memory pinned using gm_register_memory() will be accessible at
   *   restart, but will need to be re-pinned.
   *
   * Note that MPI_Alloc_mem does not return gm_dma_malloc'd memory to the
   * user if CR support is enabled; so we don't have to worry about handling
   * such a case.
   *
   * XXX: SS: do we need to clear the pinned memory lists now??... or maybe
   * we're better off doing a lazy destroy of pinned lists at restart??? 
   */
  if (backup_dma_memory() < 0)
    return LAMERROR;

  /* destroy the env and short lookaside dma buffers */

  lam_ssi_rpi_gm_dma_destroy();

  /* destroy the pinned memory list */

  lam_ssi_rpi_gm_interval_destroy();

  /* close my GM port before returning. */

  lam_ssi_rpi_gm_gm_finalize(&lam_myproc->p_rpi->cp_gm_port);

  return 0;
}


/* 
 * gm re-initialization common to continue/restart 
 */
static int
do_gm_reinit(struct _proc **procs, int nprocs)
{
  int i;
  int rtokens;
  struct lam_ssi_rpi_gm_envl *env;
  
  if (lam_ssi_rpi_did >= 40)
    lam_debug(PUB(did), "re-init: Re-initializing gm...");

  /* Init gm */
  if (gm_init() != GM_SUCCESS) {
    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(PUB(did), "re-init: gm_init failed.");
    return LAMERROR;
  }

#if LAM_WANT_DEBUG && 0
  if (0 == lam_myproc->p_gps.gps_grank) {
    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(PUB(did), "re-init: MCW rank 0 sleeping before open_port");
    sleep(5);
  }
#endif

  /* open a gm port */
  if (lam_ssi_rpi_gm_get_port(lam_myproc, &lam_myproc->p_rpi->cp_gm_port) 
      == LAMERROR) {
    if (lam_ssi_rpi_verbose >= 0)
      lam_debug(PUB(did), "re-init: Failed to get gm port.");
    gm_finalize();
    return LAMERROR;
  }

  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "re-init: re-opened gm port with id %d",
              lam_myproc->p_rpi->cp_gm_port.rgp_port_id);

  /* XXX: SS: Need to do dma/memory-pinning related re-initialization
     here. */

  /* Setup pinned interval registration */

  if (LAMERROR == lam_ssi_rpi_gm_interval_init(lam_myproc->p_rpi)) {
    if (lam_ssi_rpi_verbose >= 20)
      lam_debug(PUB(did), "re-init: interval_init failed");
    return LAMERROR;
  }

  /* Setup a lookaside table for quick messages */

  if (LAMERROR == lam_ssi_rpi_gm_dma_init(lam_myproc->p_rpi)) {
    if (lam_ssi_rpi_verbose >= 20)
      lam_debug(PUB(did), "re-init: dma_init failed");
    return LAMERROR;
  }

  /* Provide recv buffers for envelopes and short messages.  Use all
     available receive tokens -- long messages are sent using RMA
     PUTs. */

  rtokens = gm_num_receive_tokens(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port);
  for (i = 0; i < rtokens; ++i) {
    env = lam_ssi_rpi_gm_dma_env_malloc();
    gm_provide_receive_buffer(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
                              env,
                              lam_ssi_rpi_gm_env_min_size_for_length,
                              LAM_SSI_RPI_GM_PRIORITY);
  }

  /* restore dma memory - dma envs and pinned long messages. */

  if (restore_dma_memory() < 0) {
    if (lam_ssi_rpi_verbose >= 20)
      lam_debug(PUB(did), "re-init: retore of dma memory failed");
    return LAMERROR;
  }

  /* send/recv port information to/from all procs */

  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "re-init: before gm_addprocs (MCW rank %d)",
              lam_myproc->p_gps.gps_grank);

  if (lam_ssi_rpi_gm_addprocs(procs, nprocs)) {
    if (lam_ssi_rpi_verbose >= 20)
      lam_debug(PUB(did), "re-init: gm_addprocs failed");
    return LAMERROR; 
  }

  /* Finally, refresh any stale long message pointers */

  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did),
              "re-init: refreshing stale long message pointers (MCW rank %d)",
              lam_myproc->p_gps.gps_grank);
  if (refresh_stale_pointers(procs, nprocs)) {
    if (lam_ssi_rpi_verbose >= 20)
      lam_debug(PUB(did), 
                "re-init: refresh stale long message pointers failed");
    return LAMERROR; 
  }

  return 0;
}


/*
 * Traverse all active requests and copy out all envelopes
 */
static int
backup_dma_memory(void)
{
  MPI_Request req;

  if (lam_ssi_rpi_verbose >= 40)
    lam_debug(PUB(did), "backup-dma: backing up dma memory", getpid()); 

  /* JMS: This will need to change when we do multi-RPI */

  for (req = lam_rq_top; req; req = req->rq_next) {

    /* Don't bother saving envelopes of requests that are done.
       Envelopes of requests that are in init state are NULL
       anyway. */

    if ((req->rq_state != LAM_RQSINIT) && (req->rq_state != LAM_RQSDONE)) {
      req->rq_rpi->cq_envbuf_save = malloc(sizeof(struct lam_ssi_rpi_gm_envl));
      if (req->rq_rpi->cq_envbuf_save == NULL)
        return LAMERROR;
      lam_memcpy(req->rq_rpi->cq_envbuf_save, 
                 req->rq_rpi->cq_envbuf,
                 sizeof(struct lam_ssi_rpi_gm_envl));
    }
  }

  return 0;
}


/*
 * Traverse active requests and restore DMA-able envelopes.  This
 * occurs during continue/restart.
 */
static int
restore_dma_memory(void)
{
  MPI_Request req;
  
  /* Restore dma memory that was lost during checkpoint and re-pin the
     memory that was pinned earlier. */

  for (req = lam_rq_top; req; req = req->rq_next) {

    /* If request is not in init/done state, allocate dma memory for
       the envelope(s). */

    if ((req->rq_state != LAM_RQSINIT) && (req->rq_state != LAM_RQSDONE)) {

      /* allocate dma memory and restore envelope(s) */

      req->rq_rpi->cq_envbuf = lam_ssi_rpi_gm_dma_env_malloc();
      if (req->rq_rpi->cq_envbuf == NULL)
        return LAMERROR;
      lam_memcpy(req->rq_rpi->cq_envbuf, 
                 req->rq_rpi->cq_envbuf_save,
                 sizeof(struct lam_ssi_rpi_gm_envl));
      free(req->rq_rpi->cq_envbuf_save);
      req->rq_rpi->cq_envbuf_save = NULL;

      /* If dma_data_buffer == NULL, then user memory was pinned way
         back in req_start for short or long messages (and nothing
         done for tiny messages); re-pin this memory. */

      if (req->rq_rpi->dma_data_buf == NULL) {

        /* if this is a tiny message, we don't need to pin packbuf. */

        if ((req->rq_packsize > lam_ssi_rpi_gm_tinymsglen) && 
            (lam_ssi_rpi_gm_interval_use(req->rq_packbuf,
                                         req->rq_packsize, 1, 1) < 0))
          return LAMERROR;
      } 

      /* else dma memory was allocated in req_start; re-allocate dma
         memory and re-copy the data into it. */

      else {
        req->rq_rpi->dma_data_buf = 
          lam_ssi_rpi_gm_dma_malloc(req->rq_packsize);

        if (req->rq_rpi->dma_data_buf == NULL)
          return LAMERROR;
        lam_memcpy(req->rq_rpi->dma_data_buf, req->rq_packbuf,
                   req->rq_packsize);
      }

    }
  }

  return 0;
}


/*
 * Per the lengthy comment at the beginning of this file, there are
 * two cases where there can be stale pointers during a
 * continue/restart.  This function is invoked during a
 * continue/restart to chase them all down and refresh them by
 * querying what the new values should be from the remote node.
 */
static int 
refresh_stale_pointers(struct _proc **procs, int nprocs)
{
  int i, myindex;
  struct nmsg inmsg;
  struct nmsg outmsg;
  void **buffer;

  /* Find my index */

  for (myindex = -1, i = 0; i < nprocs; ++i)
    if (procs[i] == lam_myproc) {
      myindex = i;
      break;
    }
  if (-1 == myindex)
    return LAMERROR;

  /* Setup */

  buffer = malloc(MAXNMSGLEN);
  if (buffer == NULL)
    return LAMERROR;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);
  inmsg.nh_flags = 0;
  inmsg.nh_msg = (char *) buffer;
  outmsg.nh_flags = 0;
  outmsg.nh_msg = (char *) buffer;

  /* Loop through all procs.  This is not a very efficient data
     exchange pattern (it gets serialized because everyone tries to
     hit process 0 first); should probably fix someday.  But for now,
     reliably functionality is important. */

  for (i = 0; i < nprocs; ++i) {
    if (i == myindex)
      continue;

    /* Setup the messages.  This uses, in effect, synchronization
       MPI_COMM_WORLD and tag 0. */

    _m2l_fillsys(procs[i]->p_gps.gps_node, procs[i]->p_gps.gps_idx,
                 lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx, 0,
		 (procs[i]->p_gps.gps_node & 0xFFFF00) >> 8, &inmsg);
    _m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
                 procs[i]->p_gps.gps_node, procs[i]->p_gps.gps_idx, 0,
		 (lam_myproc->p_gps.gps_node & 0xFFFF00) >> 8, &outmsg);
    
    /* Do the send/receive */

    if (myindex < i) {
      if (send_stale_unexpected(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          receive_stale(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          send_stale_pending(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          receive_stale(procs[i], &inmsg, &outmsg, buffer) != 0) {
        free(buffer);
        return LAMERROR;
      }
    } else {
      if (receive_stale(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          send_stale_unexpected(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          receive_stale(procs[i], &inmsg, &outmsg, buffer) != 0 ||
          send_stale_pending(procs[i], &inmsg, &outmsg, buffer) != 0) {
        free(buffer);
        return LAMERROR;
      }
    }
  }

  /* All done */

  free(buffer);
  return 0;
}


/*
 * Send one or more packets of MPI_Request pointers from the
 * unexpected queue to the source asking for new data target pointers.
 * Then receive the reply from the source and update the unexpected
 * queue with the new target pointer values.
 *
 * This function and send_stale_unexpected() (above) *could* have been
 * combined into a single send_stale() function because much of the
 * logic is similar. But the complexity would have grown beyond any
 * perceived optimization (i.e., read / write from two data sources,
 * packetize across both, etc.).  In the interest of code clarity,
 * keep them separate.
 */
static int
send_stale_unexpected(struct _proc *p, struct nmsg *inmsg, 
                      struct nmsg *outmsg, void **buffer)
{
  int i, pos;
  int num_left;
  int num_in_msg;
  struct lam_ssi_rpi_cbuf_msg **cmsgs;

  /* Iterate over the array of stale unexpected long envelopes */
  
  cmsgs = lam_arr_get(p->p_rpi->cp_unexpected_long_recvs);
  num_left = lam_arr_size(p->p_rpi->cp_unexpected_long_recvs);

  /* Special case for num_left == 0 */

  if (num_left == 0) {
    outmsg->nh_data[0] = 0;
    outmsg->nh_length = 0;
    return nsend(outmsg);
  }

  /* Normal case of iterating over the array sending it,
     MAXNMSGLEN-sized packets at a time (holy smokes -- if you've got
     more than MAXNMSGLEN/sizeof(void*) outstanding long messages,
     you've got issues). */

  pos = 0;
  do {
    if (sizeof(struct lam_ssi_rpi_cbuf_msg *) * num_left > MAXNMSGLEN) {
      num_in_msg = MAXNMSGLEN / sizeof(MPI_Request);
      outmsg->nh_data[0] = 1;
    } else {
      num_in_msg = num_left;
      outmsg->nh_data[0] = 0;
    }

    /* We send an array of source MPI_Request pointers -- pointers to
       the MPI_Request objects on the source machine where we're
       supposed to RDMA get from.  This request pointer will not be
       stale -- the target address that we have for the data is stale.
       So we send over the MPI_Request pointer(s) and ask for new
       target addresses. */

    for (i = pos; i < pos + num_in_msg; ++i)
      buffer[i - pos] = cmsgs[i]->cm_extra->lsrc_src_request;

    /* Now send this packet and receive the response */

    outmsg->nh_length = inmsg->nh_length = num_in_msg * sizeof(MPI_Request);
    if (nsend(outmsg) != 0 || nrecv(inmsg) != 0)
      return LAMERROR;

    /* The peer returned a new set of target addresses. Refresh the
       stale pointers with the values from the message we just
       received. */

    for (i = pos; i < pos + num_in_msg; ++i)
      cmsgs[i]->cm_extra->lsrc_target = buffer[i - pos];

    /* Reset for the next iteration */

    num_left -= num_in_msg;
    pos += num_in_msg;
  } while (num_left > 0);

  /* All done */

  return 0;
}


/*
 * Essentially the same as send_stale_unexpected(), above, but
 * traverse the pending long message list instead.  i.e., the data
 * source of the MPI_Request pointers is slightly different, and of
 * course we need to update a different place upon the reply message.
 *
 * This function and send_stale_unexpected() (above) *could* have been
 * combined into a single send_stale() function because much of the
 * logic is similar. But the complexity would have grown beyond any
 * perceived optimization (i.e., read / write from two data sources,
 * packetize across both, etc.).  In the interest of code clarity,
 * keep them separate.
 */
static int
send_stale_pending(struct _proc *p, struct nmsg *inmsg, 
                   struct nmsg *outmsg, void **buffer)
{
  int i, pos;
  int num_left;
  int num_in_msg;
  MPI_Request *reqs;

  /* Iterate over the array of stale pending long envelopes */
  
  reqs = lam_arr_get(p->p_rpi->cp_pending_long_recvs);
  num_left = lam_arr_size(p->p_rpi->cp_pending_long_recvs);

  /* Special case for num_left == 0 */

  if (num_left == 0) {
    outmsg->nh_data[0] = 0;
    outmsg->nh_length = 0;
    return nsend(outmsg);
  }

  /* Normal case of iterating over the array sending it,
     MAXNMSGLEN-sized packets at a time (holy smokes -- if you've got
     more than MAXNMSGLEN/sizeof(void*) outstanding long messages,
     you've got issues). */

  pos = 0;
  do {
    if (sizeof(struct lam_ssi_rpi_cbuf_msg *) * num_left > MAXNMSGLEN) {
      num_in_msg = MAXNMSGLEN / sizeof(MPI_Request);
      outmsg->nh_data[0] = 1;
    } else {
      num_in_msg = num_left;
      outmsg->nh_data[0] = 0;
    }

    /* We send an array of source MPI_Request pointers -- pointers to
       the MPI_Request objects on the source machine where we're
       supposed to RDMA get from.  This request pointer will not be
       stale -- the target address that we have for the data is stale.
       So we send over the MPI_Request pointer(s) and ask for new
       target addresses. */

    for (i = pos; i < pos + num_in_msg; ++i)
      buffer[i - pos] = reqs[i]->rq_rpi->cq_envbuf->ge_src_request;

    /* Now send this packet and receive the response */

    outmsg->nh_length = inmsg->nh_length = num_in_msg * sizeof(MPI_Request);
    if (nsend(outmsg) != 0 || nrecv(inmsg) != 0)
      return LAMERROR;

    /* The peer returned a new set of target addresses. Refresh the
       stale pointers with the values from the message we just
       received. */

    for (i = pos; i < pos + num_in_msg; ++i)
      reqs[i]->rq_rpi->cq_envbuf->ge_target = buffer[i - pos];

    /* Reset for the next iteration */

    num_left -= num_in_msg;
    pos += num_in_msg;
  } while (num_left > 0);

  /* All done */

  return 0;
}


/*
 * Receiver one or more packets of MPI_Request pointers to the source
 * asking for new data target pointers, then reply with new data
 * pointers.
 */
static int
receive_stale(struct _proc *p, struct nmsg *inmsg, struct nmsg *outmsg,
              void **buffer)
{
  int i;
  MPI_Request req;

  /* Loop receiving packets until there are no more */

  do {
    inmsg->nh_length = MAXNMSGLEN;
    if (nrecv(inmsg) != 0)
      return LAMERROR;

    /* If there's no data (this will only happen on the first packet),
       we're done */

    if (inmsg->nh_length == 0)
      return 0;

    /* The message that we receive will be full of pointers to
       requests.  We look up each of those requests, find the new data
       pointer, and replace the request pointer with the data pointer.
       The fully-overwritten buffer is then returned to the sender. */

    for (i = 0; i < inmsg->nh_length / sizeof(MPI_Request); ++i) {
      req = (MPI_Request) buffer[i];
      if (req->rq_rpi->dma_data_buf != NULL)
        buffer[i] = req->rq_rpi->dma_data_buf;
      else
        buffer[i] = req->rq_packbuf;
    }

    /* Now send the message back with all the new target pointers. */

    outmsg->nh_length = inmsg->nh_length;
    if (nsend(outmsg) != 0)
      return LAMERROR;
  } while (inmsg->nh_data[0] != 0);

  /* All done */

  return 0;
}
