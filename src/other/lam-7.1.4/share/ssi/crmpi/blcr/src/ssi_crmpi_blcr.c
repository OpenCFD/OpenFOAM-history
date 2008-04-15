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
 * $Id: ssi_crmpi_blcr.c,v 1.11 2003/11/10 23:25:35 jsquyres Exp $
 *
 *	Function:	- BLCR crmpi module 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

#include <lam_config.h>
#include <typical.h>
#include <sfh.h>
#include <etc_misc.h>

#include <lam-ssi-cr.h>
#include <lam-ssi-crmpi-blcr.h>
#include <lam-ssi-crmpi-blcr-config.h>

#include <libcr.h>

#define signal_puts(S)  write(STDOUT_FILENO, (S), strlen(S))

/*
 * local variables 
 */
static int param_priority;
static cr_spinlock_t *handler_spinlock = NULL;
static lam_ssi_crmpi_actions_t module_actions = {
  lam_ssi_crmpi_blcr_finalize,
  lam_ssi_crmpi_base_app_suspend
};


/*
 * local functions
 */
static int crmpi_thread_callback(void *arg);
static int crmpi_signal_callback(void *arg);


int 
lam_ssi_crmpi_blcr_open_module(OPT *ad)
{
  param_priority = 
    lam_ssi_base_param_register_int("cr", "blcr", "priority", NULL,
                                    LAM_SSI_CRMPI_BLCR_DEFAULT ? 75 : 50);

  return 1;
}


/*
 * lam_ssi_crmpi_blcr_query
 *
 * Function: determine if the module wants to run.
 */
int
lam_ssi_crmpi_blcr_query(int *priority, int *thread_min, int *thread_max)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return LAMERROR;

  /* If we're verbose, print stuff out */

  if (lam_ssi_cr_verbose >= 5) {
    lam_debug(lam_ssi_cr_did, "blcr: module initializing");
    lam_debug(lam_ssi_cr_did, "blcr:verbose: %d",
              lam_ssi_cr_verbose);
    lam_debug(lam_ssi_cr_did, "blcr:priority: %d", *priority);
  }

  *thread_min = *thread_max = MPI_THREAD_SERIALIZED;

  /* Return 1 to say that we want to be considered. */

  return 1;
}


/*
 *	lam_ssi_crmpi_blcr_init
 *
 *	Function:	- primary initialiation of CRMPI subsystem
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_crmpi_actions_t *
lam_ssi_crmpi_blcr_init(void)
{
  cr_callback_id_t cr_thread_callback_id, cr_signal_callback_id;
  void *cr_thread_callback_arg = NULL, *cr_signal_callback_arg = NULL;
  
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, " initializing");
  
  if (cr_init() < 0) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "cr_init failed...");

    /* Do a show_help_file here to tell the user that cr_init
       failed */

    show_help_file("lam-ssi-crmpi-blcr-helpfile", "init", "cr-init-fail",
                   NULL);
    return NULL;
  }

  lam_ssi_crmpi_base_init_lock();
  
  handler_spinlock = malloc(sizeof(cr_spinlock_t));
  cr_spinlock_init(handler_spinlock);
  cr_spinlock_lock(handler_spinlock);
  
  /* Register the checkpoint handler callbacks */

  cr_thread_callback_id = cr_register_callback(crmpi_thread_callback,
                                               cr_thread_callback_arg, 
                                               CR_THREAD_CONTEXT);
  cr_signal_callback_id = cr_register_callback(crmpi_signal_callback,
                                               cr_signal_callback_arg, 
                                               CR_SIGNAL_CONTEXT);
  
  /* XXX: Add in env var for the file directory here */
  /* XXX: print out file directory here */

  /* Return the struct with the function pointers in it for all the
     APIs */

  return (&module_actions);
}


/*
 *	lam_ssi_crmpi_blcr_finalize
 *
 *	Function:	- crmpi cleanup
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_crmpi_blcr_finalize(void)
{
  if (handler_spinlock != NULL)
    free(handler_spinlock);

  lam_ssi_crmpi_base_finalize_lock();

  return 0;
}


/*
 * Asynchronous handler thread.  This does most of the checkpoint/restore 
 * functionality.  
 */
static int
crmpi_thread_callback(void *arg)
{
  int rc;
 
  if (lam_ssi_cr_verbose >= 30)
    lam_debug(lam_ssi_cr_did, "thread_callback: active");

  if (lam_ssi_crmpi_base_lock_mpi() < 0) {
    show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread",
                   "acquire-lock-fail", NULL);
    kexit(1);
  }

  /* prepare all the modules for checkpoint -- for now, rpi and
     coll */

  if (lam_ssi_crmpi_base_checkpoint() < 0) {
    lam_debug(lam_ssi_cr_did,
              "thread_callback: prepare for checkpoint failed");
    show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread",
                   "chkpt-prepare-fail", NULL);
    kexit(1);
  }

  /* call into the kernel to drop the context-file */

  if (lam_ssi_cr_verbose >= 30)
    lam_debug(lam_ssi_cr_did, "thread_callback: calling BLCR cr_checkpoint");

  rc = cr_checkpoint(0);
  if (lam_ssi_cr_verbose >= 30)
    lam_debug(lam_ssi_cr_did, "thread_callback: cr_checkpoint returned rc=%d", 
              rc);

  /* 
   * We need to handle FAILURE and CONTINUE in the same way. If checkpoint
   * failed, we will still need to go about our business as if nothing
   * happened. 
   *
   * Common to all, we need to wait for the lam_register_pid() to occur in
   * the async thread.  We need to wait for the app_thread to register its pid
   * in the cache from signal_handler context, before we can attach to the
   * lamd. 
   */
  cr_spinlock_lock(handler_spinlock);
  
  /* First case: RESTART */

  if (rc > 0) {
    if (lam_ssi_crmpi_base_restart() < 0) {
      lam_debug(lam_ssi_cr_did, "thread_callback: restart failed");
      show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread",
                     "chkpt-restart-fail", NULL);
      kexit(1);
    }
  }

  /* Second case: FAILURE/CONTINUE */
  else {
    if (lam_ssi_crmpi_base_continue() < 0) {
      lam_debug(lam_ssi_cr_did, "thread_callback: continue failed");
      show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread",
                     "chkpt-cont-fail", NULL);
      kexit(1);
    }
  }

  if (lam_ssi_crmpi_base_release_mpi() < 0) {
    lam_debug(lam_ssi_cr_did, "thread_callback: _release_mpi failed in "
              "continue/restart");
    show_help_file("lam-ssi-crmpi-blcr-helpfile", "cr-thread", 
                   "release-lock-fail", NULL);
    return LAMERROR;
  }
  
  if (lam_ssi_cr_verbose >= 30) 
    lam_debug(lam_ssi_cr_did, "thread_callback: done");

  return 0;
}


/*
 * Synchronous handler.  Will be invoked in signal context.
 */
static int
crmpi_signal_callback(void *arg)
{
  int rc;
  char pid[10];
  
  if (lam_ssi_cr_verbose >= 30) {
    sfh_itoa(getpid(), pid);
    signal_puts(pid);
    signal_puts(": Inside cr_handler_app_sync\n");
  }

  /*
   * call lam_reset_pid_cache() so that the new pid can be registered at
   * restart
   */
  lam_reset_pid_cache();

  rc = cr_checkpoint(0);

  /* 
   * We need to handle FAILURE, CONTINUE and RESTART in the same way. If
   * checkpoint failed, we will still need to go about our business as if
   * nothing happened. 
   *
   * And in all the cases, we just need to register the pid in the cache. In
   * case of FAILUER/CONTINUE, we are already attached to the lamd. So no
   * problems there. And in the case of RESTART, after we register the pid,
   * the cr_thread will attach to the lamd with kenter/kinit and proceed with
   * what it needs to do.
   * NOTE: we cannot call kinit/kenter from here because these *might not*
   * continue to be signal-safe in the future.
   */

  if (lam_ssi_cr_verbose >= 30) {
    signal_puts(pid);
    signal_puts("signal_callback: cr_checkpoint returned...\n");  
  }

  lam_register_pid(getpid());

  /* We're all done here.  Let the async handler proceed. */
  cr_spinlock_unlock(handler_spinlock);

  if (lam_ssi_cr_verbose >= 30) {
    signal_puts(pid);
    signal_puts(": Leaving cr_handler_app_sync\n");
  }

  return 0;
}

