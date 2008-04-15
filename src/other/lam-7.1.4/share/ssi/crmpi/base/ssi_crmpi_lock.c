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
 * $Id: ssi_crmpi_lock.c,v 1.5 2004/03/02 03:43:44 jsquyres Exp $
 *
 */

#include <lam-ssi-cr.h>
#include <lam-ssi-rpi.h>

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include <lamthreads.h>
#include <lammpithreads.h>
#include <etc_misc.h>


/* 
 * Local variables 
 */
static lam_mutex_t yield_to_cr_mutex;


/*
 * Private functions
 */
static int acquire_mpi_lock(void);
static int release_mpi_lock(void);

void 
lam_ssi_crmpi_base_init_lock(void)
{
  /* Initialize yield_to_cr_mutex. */
  lam_mutex_init(&yield_to_cr_mutex);

  return;
}


void
lam_ssi_crmpi_base_finalize_lock(void)
{
  /* Destroy yield_to_cr_mutex. */
  lam_mutex_destroy(&yield_to_cr_mutex);

  return;
}


/*
 *      lam_ssi_crmpi_base_app_suspend
 *
 *      Function:       - function for app thread to yield to the crmpi thread
 *      Returns:        - 0 or LAMERROR
 */
void
lam_ssi_crmpi_base_app_suspend(void)
{
  lam_mutex_unlock(&lam_mpi_mutex); 

  lam_mutex_lock(&yield_to_cr_mutex); 
  lam_mutex_unlock(&yield_to_cr_mutex); 

  lam_mutex_lock(&lam_mpi_mutex); 

  return;
}


int 
lam_ssi_crmpi_base_lock_mpi(void)
{
  /* grab the yield_to_cr_mutex first */

  lam_mutex_lock(&yield_to_cr_mutex);

  /* update handler thread state */

  lam_ssi_crmpi_base_handler_state = LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING;
  
  /* acquire mpi lock */

  if (acquire_mpi_lock() < 0) {
    show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread", 
                   "acquire-lock-fail", NULL);
    return LAMERROR;
  }

  /* Now set the state to indicate that we are running. */

  lam_ssi_crmpi_base_handler_state = LAM_SSI_CRMPI_BASE_HANDLER_STATE_RUNNING;

  /* release the yield_to_cr_mutex */

  lam_mutex_unlock(&yield_to_cr_mutex);
 
  return 0;
}


int
lam_ssi_crmpi_base_release_mpi(void)
{
  /* set the handler_state back to IDLE */

  lam_ssi_crmpi_base_handler_state = LAM_SSI_CRMPI_BASE_HANDLER_STATE_IDLE;

  /* Unlock lam_mpi_mutex, and the rest, so that the app_thread can
     resume. */

  if (release_mpi_lock() < 0)
    return LAMERROR;

  return 0;
}


static int
acquire_mpi_lock(void)
{
  int ret;
  struct timeval tv;
  MPI_Comm *comm;

  /* Send signal to interrupt a blocked read/write in the app, if
     any. Once the signal is sent, acquire the lam_mpi_mutex and then
     proceed.  This is used to ensure that the app thread yields to
     the cr_handler thread before resuming a blocked read/write.

     The following happens in a loop to handle the corner case of an
     app blocking indefinitely on a read just *after* the signal is
     delivered, and hence not releasing the lock at all! */
  
  if (lam_ssi_cr_verbose >= 30)
    lam_debug(lam_ssi_cr_did, "mpi_lock: before signalling the app thread");

  while (1) {

    /* Turn around and call the interrupt action function on all the
       active checkpoint-aware MPI SSI modules.  Right now, they are
       rpi and coll. */

    if (lam_ssi_cr_verbose >= 30)
      lam_debug(lam_ssi_cr_did, "mpi_lock: interrupting RPI");
    if (RPI_INTERRUPT() < 0)
      return LAMERROR;

    if (lam_ssi_cr_verbose >= 30)
      lam_debug(lam_ssi_cr_did, "mpi_lock: interrupting coll modules");
    for (comm = (MPI_Comm *) al_top(lam_comms); comm != NULL; 
         comm = al_next(lam_comms, comm))
      if ((*comm)->c_ssi_coll.lsca_interrupt() < 0)
        return LAMERROR;

    if (lam_ssi_cr_verbose >= 30)
      lam_debug(lam_ssi_cr_did, "mpi_lock: trying to lock MPI mutex");
    if ((ret = lam_mutex_trylock(&lam_mpi_mutex)) == EBUSY) { 
      if (lam_ssi_cr_verbose >= 30)
        lam_debug(lam_ssi_cr_did,
                  "mpi_lock: lam_mpi_mutex held by app_thread; try again");
    } else if (ret != 0) {	
      if (lam_ssi_cr_verbose >= 30)
        lam_debug(lam_ssi_cr_did,
                  "mpi_lock: Bad error %d on lam_mutex_trylock of "
                  "lam_mpi_mutex", errno);  
    } else
      break;
    
    /* 
     * sleep before we resend the signal. we need to sleep for > 2ms,
     * otherwise kernel might busy wait without yielding.
     */
    tv.tv_sec = 0;
    tv.tv_usec = 2;
    select(0, NULL, NULL, NULL, &tv);
  }

  if (lam_ssi_cr_verbose >= 30)
    lam_debug(lam_ssi_cr_did, "mpi_lock: locked lam_mpi_mutex");

  return 0;
}


static int
release_mpi_lock(void)
{
  lam_mutex_unlock(&lam_mpi_mutex);
  
  return 0;
}
