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
 * $Id: ssi_crmpi_self.c,v 1.2 2004/03/09 16:56:17 pcharapa Exp $
 *
 *	Function:	- Self crmpi module 
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
#include <mpisys.h>
#include <lam_ltdl.h>

#include <lam-ssi-cr.h>
#include <lam-ssi-crmpi-self.h>
#include <lam-ssi-crmpi-self-config.h>
#include <lamthreads.h>


/*
 * local types
 */
typedef int (*callback_fn_t)(void);


/*
 * local variables 
 */
static int param_priority;

/*
 * variables for SSI params
 */
static int param_do_restart;
static int param_prefix;

static int param_checkpoint_name;
static int param_continue_name;
static int param_restart_name;

static callback_fn_t user_checkpoint_fn = NULL;
static callback_fn_t user_continue_fn = NULL;
static callback_fn_t user_restart_fn = NULL;

static lam_ssi_crmpi_actions_t module_actions = {
  lam_ssi_crmpi_self_finalize,
  lam_ssi_crmpi_base_app_suspend
};

/*
 * The pipe that the Signal hanlder writes to and
 * the thread that blocks waiting for a signal to arrive
 */ 
static int crmpi_self_filedesc[2];
static lam_thread_t crmpi_self_thread;

typedef enum {
  LAM_SSI_CRMPI_SELF_THREAD_STATUS_GO,
  LAM_SSI_CRMPI_SELF_THREAD_STATUS_DONE
} lam_ssi_crmpi_self_thread_status_t;


/*
 * local functions
 */
static void *thread_handler(void *);
static void signal_handler(int sig);
static callback_fn_t find_function(lt_dlhandle handle, int param, 
                                   char *suffix);

int 
lam_ssi_crmpi_self_open_module(OPT *ad)
{
  param_priority = 
    lam_ssi_base_param_register_int("cr", "self", "priority", NULL,
                                    LAM_SSI_CRMPI_SELF_DEFAULT ? 75 : 25);

  param_do_restart =
    lam_ssi_base_param_register_int("cr", "self", "do_restart", NULL, 0);

  /* SSI parameters for defining the Prefix and the names of the
     Checkpoint, Restart and Continue functions. */ 
  
  param_prefix =
    lam_ssi_base_param_register_string("cr", "self", "prefix", 
				       NULL, "lam_cr_self");
  
  param_checkpoint_name =
    lam_ssi_base_param_register_string("cr", "self", "checkpoint", 
				       NULL, NULL);

  param_continue_name =
    lam_ssi_base_param_register_string("cr", "self", "continue", 
				       NULL, NULL);
  
  param_restart_name =
    lam_ssi_base_param_register_string("cr", "self", "restart", 
				       NULL, NULL);
  
  return 1;
}


/*
 * lam_ssi_crmpi_self_query
 *
 * Function: determine if the module wants to run.  Only let this
 * module run if we were explicitly selected.
 */
int
lam_ssi_crmpi_self_query(int *priority, int *thread_min, int *thread_max)
{
  int param;
  char *cr;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return LAMERROR;

  /* If we're verbose, print stuff out */

  if (lam_ssi_cr_verbose >= 5) {
    lam_debug(lam_ssi_cr_did, "self: module initializing");
    lam_debug(lam_ssi_cr_did, "self:verbose: %d",
              lam_ssi_cr_verbose);
    lam_debug(lam_ssi_cr_did, "self:priority: %d", *priority);
  }

  *thread_min = *thread_max = MPI_THREAD_SERIALIZED;

  /* Were we explicitly selected?  If so, return 1. */

  param = lam_ssi_base_param_find("cr", "base", "module");
  if (param >= 0) {
    cr = lam_ssi_base_param_lookup_string(param);
    if (cr != NULL && strcmp(cr, "self") == 0) {
      return 1;
    }
  }

  /* We were not explicitly selected, so don't allow us to be selected */

  return 0;
}


/*
 *	lam_ssi_crmpi_self_init
 *
 *	Function:	- primary initialiation of CRMPI subsystem
 *	Returns		- 0 or LAMERROR
 */
const lam_ssi_crmpi_actions_t *
lam_ssi_crmpi_self_init(void)
{
  lt_dlhandle executable;
  int do_restart = 0;
  
  /* First, open up the executable with lt_dlopen so that we can
     lookup symbols. */

  executable = lt_dlopen(NULL);
  if (executable == NULL) {
    lam_perror("opening ltdl\n");
    return NULL;
  }
  
  /* Look up the name of the function that will be called during
     MPI_Init on restart. */

  user_checkpoint_fn = find_function(executable, param_checkpoint_name,
				     "checkpoint");
  user_continue_fn = find_function(executable, param_continue_name,
				   "continue");
  user_restart_fn = find_function(executable, param_restart_name,
				  "restart");

  /* All done looking up -- can close the lt library */

  lt_dlclose(executable);

  do_restart = lam_ssi_base_param_lookup_int(param_do_restart);

  if (1 == do_restart) {
      lam_debug(lam_ssi_cr_did,
                "self:thread:%d: do_restart == 1", 
                getpid());
      if (user_restart_fn != NULL) {
          lam_debug(lam_ssi_cr_did,
                    "self:thread:%d: calling user restart function",
                    getpid());
          if (lam_ssi_base_mpi_init_callback(user_restart_fn) != MPI_SUCCESS) {
              return NULL;
          }
      } else {
          lam_debug(lam_ssi_cr_did,
                    "self:thread:%d: WARNING: no user restart function found; skipped",
                    getpid());
      }
  }
	
  /* 
   * Create a pipe for the communication between the Thread hanlder 
   * and the Signal handler 
   */
  
  if (pipe(crmpi_self_filedesc) != 0) {    
    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-init",
		   "pipe-creation-fail", NULL);
    return NULL;
  }
  
  lam_ssi_crmpi_base_init_lock();

  /*
   * Register the checkpoint signal handler
   */
	
  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-init", 
		    "pipe-creation-fail", NULL); 
    
    close(crmpi_self_filedesc[0]);
    close(crmpi_self_filedesc[1]);
    return NULL;
 }

  /*
   * Create a Thread, the passed function argument is a function that blocks
   * this thread on the pipe we just created
   */
	
  if (lam_thread_create(&crmpi_self_thread, thread_handler, 0) != 0) {
    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-init",
		   "thread-creation-fail", NULL);

    close(crmpi_self_filedesc[0]);
    close(crmpi_self_filedesc[1]);
    return NULL;
  } 

  /* Return the struct with the function pointers in it for all the
     APIs */

  return (&module_actions);
}


/*
 *	lam_ssi_crmpi_self_finalize
 *
 *	Function:	- crmpi cleanup
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_crmpi_self_finalize(void)
{
  lam_ssi_crmpi_self_thread_status_t status = 
    LAM_SSI_CRMPI_SELF_THREAD_STATUS_DONE;
  
   lam_ssi_crmpi_base_finalize_lock();

  /* Tell the thread handler to quit */

  if (write(crmpi_self_filedesc[1], &status, sizeof(status)) != 
      sizeof(status)) {
    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-signal",
		   "write-fail", NULL);
            
    close(crmpi_self_filedesc[0]);
    close(crmpi_self_filedesc[1]);
    return LAMERROR;
  }

  /* Wait for the thread handler to quit */

 if (lam_thread_join(crmpi_self_thread, NULL) == -1) {
    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-init",
		   "thread-join-fail", NULL);
    return LAMERROR;
  } 
  
  return 0;
}

static void
signal_handler(int sig)
{
  lam_ssi_crmpi_self_thread_status_t status = 
    LAM_SSI_CRMPI_SELF_THREAD_STATUS_GO;

  if (write(crmpi_self_filedesc[1], &status, sizeof(status)) != 
      sizeof(status)) {

    /* JMS: Technically, this isn't safe, but we're already in an
       error situation... ;-) */

    show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-signal",
		   "write-fail", NULL);
  }

}

static void*
thread_handler(void *foo)
{
  lam_ssi_crmpi_self_thread_status_t status =
    LAM_SSI_CRMPI_SELF_THREAD_STATUS_GO;
  
  /* 
   * Loop forever blocking on the Pipe, if the status read is 'go'
   * call glue checkpoint function, if status is 'done', exit
   */

  while (1) {
    if (read(crmpi_self_filedesc[0], &status, sizeof(status)) == -1) {
      show_help_file("lam-ssi-crmpi-self-helpfile", "cr-self-thread",
		     "read-fail", NULL);
    }

    if (status == LAM_SSI_CRMPI_SELF_THREAD_STATUS_DONE) {
      lam_thread_exit(NULL);
    }

    /* prepare all the modules for checkpoint -- for now, rpi and
       coll */

    if (lam_ssi_crmpi_base_lock_mpi() < 0) {
      show_help_file("lam-ssi-crmpi-self-helpfile", "cr-thread",
		     "acquire-lock-fail", NULL);
      kexit(1);
    }
		
    if (lam_ssi_crmpi_base_checkpoint() != 0) {
      lam_debug(lam_ssi_cr_did, "self:thread:%d: checkpoint failed", getpid());
      show_help_file("lam-ssi-crmpi-self-helpfile", "cr-thread",
		     "chkpt-prepare-fail", NULL);
      kexit(1);
    }

    if (user_checkpoint_fn != NULL) {
        lam_debug(lam_ssi_cr_did,
                  "self:thread:%d: calling user checkpoint function",
                  getpid());
        user_checkpoint_fn(); 
    } else {
        lam_debug(lam_ssi_cr_did,
                  "self:thread:%d: WARNING: no user checkpoint function; skipped",
                  getpid());
    }

    /* FAILURE/CONTINUE */

    if (lam_ssi_crmpi_base_continue() < 0) {
      lam_debug(lam_ssi_cr_did, "APP_ASYNC%d: continue failed.", getpid());
      show_help_file("lam-ssi-crmpi-self-helpfile", "cr-thread",
		     "chkpt-cont-fail", NULL);
      kexit(1);
    }
		
    if (user_continue_fn != NULL) {
        lam_debug(lam_ssi_cr_did, 
                  "self:thread:%d: calling user continue function",
                  getpid());
        user_continue_fn(); 
    } else {
        lam_debug(lam_ssi_cr_did, 
                  "self:thread:%d: WARNING: no user continue function; skipped",
                  getpid());
    }

    if (lam_ssi_crmpi_base_release_mpi() < 0) {
      lam_debug(lam_ssi_cr_did, "APP_ASYNC%d: _release_mpi failed in "
          "continue/restart", getpid());
      show_help_file("lam-ssi-crmpi-self-helpfile", "cr-thread", 
          "release-lock-fail", NULL);
      return NULL;
    }

    signal(SIGUSR1, signal_handler);
  }
  
  return (void *)status;
}


/*
 * Given a parameter name, find the final function name and resolve it
 * to a valid symbol.
 */
static callback_fn_t
find_function(lt_dlhandle handle, int param, char *suffix)
{
  int len;
  char *name = lam_ssi_base_param_lookup_string(param);
  char *prefix = lam_ssi_base_param_lookup_string(param_prefix);
  lt_ptr ptr;

  /* Check for errors */

  if (name != NULL && prefix != NULL) {
    show_help_file("lam-ssi-crmpi-helpfile", "cr-self-init",
                   "name-and-prefix-specified", name, prefix, NULL);
    return NULL;
  }
  if (name == NULL && prefix == NULL) {
    show_help_file("lam-ssi-crmpi-helpfile", "cr-self-init",
                   "name-and-prefix-not-specified", prefix, NULL);
    return NULL;
  }
 
  /* If a prefix was specified, use it */
  
  if (prefix != NULL && strlen(prefix) != 0) {
    len = strlen(prefix) + strlen(suffix) + 16;
    name = malloc(len);
    if (name == NULL)
      return NULL;
    snprintf(name, len, "%s_%s", prefix, suffix);
  }

  /* Now that we have a final function name, lookup its symbol.  If we
     don't find it, just return NULL. */

  ptr = lt_dlsym(handle, name);
  if (NULL == ptr) {
      lam_debug(lam_ssi_cr_did,
                "self:init:%d: WARNING: %s function \"%s\" not found",
                getpid(), suffix, name);
  } else {
      lam_debug(lam_ssi_cr_did,
                "self:init:%d: found %s function \"%s\"",
                getpid(), suffix, name);
  }
  free(name);
  return (callback_fn_t) ptr;
}
