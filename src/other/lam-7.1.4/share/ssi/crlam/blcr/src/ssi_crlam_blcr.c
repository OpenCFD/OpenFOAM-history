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
 * $Id: ssi_crlam_blcr.c,v 1.16 2004/03/07 07:05:53 pcharapa Exp $
 *
 *	Function:	- BLCR crlam module 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <etc_misc.h>
#include <priority.h>
#include <freq.h>
#include <preq.h>
#include <sfh.h>

#include <lam-ssi-cr.h>
#include <lam-ssi-crlam-blcr.h>
#include <lam-ssi-crlam-blcr-config.h>

#include <libcr.h>


/*
 * local functions
 */
 static int crlam_thread_callback(void *arg);
 static int crlam_signal_callback(void *arg);
 static int cr_signal_app(struct _gps *, int);
 static char *get_checkpoint_file(int node, int pid, const char *suffix);
 static char *trim_filename(const char *name, const char *suffix);
 static char *get_cr_util(const char* util);
 static char *create_app_schema(struct _gps *, int);
 static void cleanup(char **fname_arr, struct _gps *world, int n);


/*
 * local variables
 */
static lam_ssi_crlam_actions_t module_actions = {
  lam_ssi_crlam_blcr_checkpoint,
  lam_ssi_crlam_blcr_continue,
  lam_ssi_crlam_blcr_disable_checkpoint,
  lam_ssi_crlam_blcr_enable_checkpoint,
  lam_ssi_crlam_blcr_finalize,
  lam_ssi_crlam_blcr_init,
  /* use the base utility function for restart action */
  lam_ssi_crlam_base_do_exec,
  lam_ssi_crlam_blcr_lamcheckpoint,
  lam_ssi_crlam_blcr_lamrestart
};

static int param_priority;
static int param_base_dir;
static struct _gps *cr_world = NULL;
static int cr_world_n = -1;
static int app_session = 0;
static char *chkpnt_app_schema = NULL;
static char *executable;
static cr_client_id_t client_id;


/*
 * Open the module and register SSI parameters
 */
int
lam_ssi_crlam_blcr_open_module(OPT *ad)
{
  param_base_dir = 
    lam_ssi_base_param_register_string("cr", "blcr", "base_dir", NULL, NULL);
  param_priority = 
    lam_ssi_base_param_register_int("cr", "blcr", "priority", NULL,
                                    LAM_SSI_CRLAM_BLCR_DEFAULT ? 75 : 50);
  
  return 1;
}


/*
 * lam_ssi_crlam_blcr_query
 *
 * Function: determine if the module wants to run.
 */
const lam_ssi_crlam_actions_t *
lam_ssi_crlam_blcr_query(int *priority)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  /* If we're verbose, print stuff out */
  if (lam_ssi_cr_verbose >= 5) {
    lam_debug(lam_ssi_cr_did, "blcr: module initializing");
    lam_debug(lam_ssi_cr_did, "blcr:verbose: %d",
              lam_ssi_cr_verbose);
    lam_debug(lam_ssi_cr_did, "blcr:priority: %d", *priority);
  }
  
  /* Return the struct with the function pointers in it for all the APIs */
  return (&module_actions);
}


/*
 *	lam_ssi_crlam_blcr_init
 *
 *	Function:	- primary initialization of CR subsystem
 *	Returns		- 0 or LAMERROR
 */
int
lam_ssi_crlam_blcr_init(char *path, char **argv, OPT *ad, 
                        struct _gps *world, int world_n)
{
  cr_callback_id_t cr_thread_callback_id, cr_signal_callback_id;
  void *cr_thread_callback_arg = NULL;
  void *cr_signal_callback_arg = NULL;

  client_id = cr_init();
  if (client_id < 0)
    return LAMERROR;

  /* 
   * Disable checkpoints till the caller explicitly invokes
   * _enable_checkpoint(). 
   */
  lam_ssi_crlam_blcr_disable_checkpoint();

  /*
   * Register the checkpoint signal handler
   */
  cr_thread_callback_id = cr_register_callback(crlam_thread_callback, 
                                               cr_thread_callback_arg, 
                                               CR_THREAD_CONTEXT);
  cr_signal_callback_id = cr_register_callback(crlam_signal_callback, 
                                               cr_signal_callback_arg, 
                                               CR_SIGNAL_CONTEXT);

  if (lam_ssi_cr_verbose >= 40)
    lam_debug(lam_ssi_cr_did, "Registered C/R handlers\n");

  cr_world = world;
  cr_world_n = world_n;
  executable = strdup(path);

  /* setup the argv to be used at restart */
  if (lam_ssi_crlam_base_create_restart_argv(argv, ad) < 0)
    return LAMERROR;

  return 0;
}


/*
 *	lam_ssi_crlam_blcr_finalize
 *
 *	Function:	- crlam cleanup
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_crlam_blcr_finalize(void)
{
  /* This function doesn't really need to do anything. Just return 0. */
  return 0;
}


/*
 *      lam_ssi_crlam_blcr_checkpoint
 *
 *      Function:       - crlam checkpoint
 *      Returns:        - 0 or LAMERROR
 */
int lam_ssi_crlam_blcr_checkpoint(void)
{
  pid_t pid;
  int status;

  /* set the session id */
  app_session = lam_getpid();

  /* 
   * fork a child to proppagate the checkpoint request to all the MPI
   * aprocesses. 
   */

  if ((pid = fork()) < 0) {
    lam_perror("fork() in async handler");
    return LAMERROR;

  } else if (0 == pid) {
    /* Child process */

    /* Need seperate pid so that we can attach to the lamd */
    lam_reset_pid_cache();
    if (kinit(PRCMD)) {
      /* well, we can't recover from this - abort */
      lam_perror("kinit failed");
      kexit(1);
    }

    if (cr_signal_app(cr_world, cr_world_n) != 0)
      kexit(1);

    if (lam_ssi_cr_verbose >= 40)
      lam_debug(lam_ssi_cr_did, "Child exiting\n");
    kexit(0);

  } 

  /* Parent process */

  if (lam_ssi_cr_verbose >= 40)
    lam_debug(lam_ssi_cr_did, "Parent waiting for pid %d\n", pid);
  if (waitpid(pid, &status, 0) != pid) {
    lam_perror("async_checkpoint waitpid");
    lam_debug(lam_ssi_cr_did, "waitpid(%d, ...) failed: %d",
              pid, errno);
    return LAMERROR;
  }
  
  if (lam_ssi_cr_verbose >= 40)
    lam_debug(lam_ssi_cr_did, "Parent reaped pid %d\n", pid);
  
  if ((WIFEXITED(status) != 0) && (WEXITSTATUS(status) != 0)) {
    lam_debug(lam_ssi_cr_did, "Bad exit status from child: %d.",
              WEXITSTATUS(status));
    return LAMERROR;
  }
  
  /*
   * create an app_schema to be used at restart, and store it in memory.  At
   * restart, this will be written to file and passed to mpirun.
   */
  chkpnt_app_schema = create_app_schema(cr_world, cr_world_n);
  if (chkpnt_app_schema == NULL) {
    perror("create_app_schema");
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "Couldn't make app schema!");
    return LAMERROR;
  }
  
  return 0; 
}


/* lam_ssi_crlam_blcr_continue
 *
 * Function:        - crlam continue
 * Returns:         - 0 or LAMERROR
 */
int
lam_ssi_crlam_blcr_continue(void)
{
  /* no-op.*/
  return 0;
}


/* 
 * lam_ssi_crlam_blcr_disable_checkpoint
 *
 * Function:        - disable checkpoints till the caller explicitly
 *                    calls _enable_checkpoint to enable it.
 */
void
lam_ssi_crlam_blcr_disable_checkpoint(void)
{
  /* pass the non-negative int that was returned from cr_init() as the arg */
  cr_enter_cs(client_id);

  return;
}


/*
 * lam_ssi_crlam_blcr_enable_checkpoint
 *
 * Function:        - enable checkpointing
 */
void
lam_ssi_crlam_blcr_enable_checkpoint(void)
{
  /* pass the same arg that was passed to the call to cr_enter_cs() */
  cr_leave_cs(client_id);

  return;
}

  
/*
 * crlam_thread_callback
 *
 * Function:
 *  
 */
static int
crlam_thread_callback(void *arg)
{
  int ret;

  if (lam_ssi_cr_verbose >= 0)
    lam_debug(lam_ssi_cr_did, "Started async C/R handler");

  /* call the glue checkpoint function */
  if (lam_ssi_crlam_base_checkpoint(cr_world, cr_world_n) != 0) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "mpirun_ASYNC%d: prepare for chkpt failed.",
              getpid());
    kexit(1);
  }

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "mpirun_async calling cr_checkpoint()");
  ret = cr_checkpoint(0);
    
  if (ret < 0) {
    /* BWB: error handling */
    /* SS: we should probably continue, instead of aborting. */
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "mpirun_async failed checkpoint - %d", ret);
    kexit(1);
  } else if (ret != 0) {
      /* no-op. restart-work is done only in the signal-handler context */
  } else {
    if (lam_ssi_crlam_base_continue() != 0) {
      if (lam_ssi_cr_verbose >= 0)
        lam_debug(lam_ssi_cr_did, "mpirun_ASYNC%d: prepare for continue "
                  "failed.", getpid());
      kexit(1);
    }
  }

  if (lam_ssi_cr_verbose >= 0)
    lam_debug(lam_ssi_cr_did, "Exiting mpirun_async C/R handler");
  return 0;
}


static int
crlam_signal_callback(void *arg)
{
  int ret;

  lam_debug(lam_ssi_cr_did, "Started sync C/R handler");
  
  /* nothing to be done before checkpointing. */
  ret = cr_checkpoint(0);

  if (ret < 0) {
    /* BWB: error handling */
    /* SS: we should probably continue, instead of aborting. */
    kexit(1);
  } else if (ret != 0) {
    /* just call the glue function to do all the Right Things at restart. */
    if (lam_ssi_crlam_base_restart(executable, chkpnt_app_schema) != 0)
      lam_debug(lam_ssi_cr_did, "mpirun_SYNC: restart failed.");
  } else {
    /* no-op. */
    if (lam_ssi_cr_verbose >= 10)
      lam_debug(lam_ssi_cr_did, "mpirun_SYNC: continuing...");
  }

  lam_debug(lam_ssi_cr_did, "Exiting sync C/R handler");
  return 0;
}


/*
 * cr_signal_app
 *
 * Function:
 * Signal all processes running under itself to checkpoint
 */
static int
cr_signal_app(struct _gps *cr_world, int cr_world_n)
{
  int i;
  char **cr_argv = NULL;
  int cr_argc = 0;
  struct penv cr_env;
  char pid_arg[sizeof(int) * 8];
  char *cr_checkpoint, **cr_filearr;
  struct _gps       *p;
  int cr_pid, cr_idx;
  int nid, pid, status;
  char *suffix = ".phase1";

  cr_checkpoint = get_cr_util("cr_checkpoint");
  if (cr_checkpoint == NULL) {
    lam_perror("Getting cr_checkpoint");
    return LAMERROR;
  }

  /* Initialize the array of checkpoint-file names */
  cr_filearr = (char **) malloc(sizeof(char *) * cr_world_n);

  for(i = 0, p = cr_world; i < cr_world_n; ++i, ++p) {
    /* setup kenya request */
    cr_env.pe_rank = p->gps_node;
    strcpy(cr_env.pe_wrkdir, "");
    cr_env.pe_envv = 0;

    /* Set up program arguments */

    sfh_argv_add(&cr_argc, &cr_argv, cr_checkpoint);

    snprintf(pid_arg, sizeof(pid_arg), "%d", p->gps_pid);
    sfh_argv_add(&cr_argc, &cr_argv, pid_arg);

    /*
     * Saving of checkpoint-files follows a two-phase commit approach.
     * While the  approach used here gives a reasonable  guarantee of a
     * consistent set of saved checkpoints, there is atleast one case in
     * which the global state represented by these checkpoints might not be
     * consistent.  
     *
     * The checkpoints are initially saved with a ".phase1" suffix.  Once it
     * has been determined that all the ranks saved the checkpoints
     * properly, the checkpoint-files are renamed by stripping the suffix.
     *
     * The rename of checkpoint-files is not atomic.  So, if there is a
     * failure in mpirun (atleast for now, it's only mpirun) during this
     * time, the saved set of checkpoints is not guaranteed to be
     * consistent.
     */

    cr_filearr[i] = get_checkpoint_file(p->gps_node, p->gps_pid, suffix);

    if (cr_filearr[i] != NULL) {
      if (lam_ssi_cr_verbose >= 40)
        lam_debug(lam_ssi_cr_did, "checkpoint file: %s\n", cr_filearr[i]);
      sfh_argv_add(&cr_argc, &cr_argv, "-f");
      sfh_argv_add(&cr_argc, &cr_argv, cr_filearr[i]);
    } else {
      int j;
      /* cleanup the array of checkpoint-file names */
      for (j = 0; j < i; ++j)
        free(cr_filearr[j]);
      free(cr_filearr);
      lam_perror("Getting checkpoint file");
      return LAMERROR;
    }
    
    /*
     * It is safe to expect cr kernel module to be present on all nodes, so
     * src_node can be the same as dst_node when invoking rploadgo
     */
    if (lam_ssi_cr_verbose >= 10)
      lam_debug(lam_ssi_cr_did, "Invoking %s %d -f %s on node:%d\n",
                cr_checkpoint, p->gps_pid, cr_filearr[i], p->gps_node);
    
    if (rploadgov(p->gps_node, p->gps_node, cr_checkpoint, RTF_WAIT, cr_argv,
                  &cr_env, &cr_pid, &cr_idx)) {
      /* We can't do anything more than abort... */
      lam_perror("rploadgov failed.");
      cleanup(cr_filearr, cr_world, i);
      return LAMERROR;
    }
    
    free(cr_argv);
    cr_argv = NULL;
    cr_argc = 0;
  }
  
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "Child rpwaiting\n");
  
  /* a stripped down version of the pwait() function used in
     mpirun. */

  for (i = 0; i < cr_world_n; ++i) {
    if (rpwait(&nid, &pid, &status)) {
      lam_perror("rpwait failed");
      cleanup(cr_filearr, cr_world, cr_world_n);
      free(cr_checkpoint);
      return LAMERROR;
    }

    /* if return status of the child is non-zero, then cr_checkpoint
       did not complete successfully. */

    if (status != 0) {
      lam_perror("rpwait failed");
      cleanup(cr_filearr, cr_world, cr_world_n);
      free(cr_checkpoint);
      return LAMERROR;
    }
  }

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "Child done rpwaiting\n");

  /* Now that we know that all the local checkpoints were successful,
     we can perform the second phase of the 2-phase commit now.  Loop
     over renaming the checkpoint-files of all ranks. */

  for (i = 0, p = cr_world; i < cr_world_n; ++i, ++p) {

    char *src, *dest;
    int len;

    /* Assume nid takes 8 * sizeof(int) chars to print */

    len = strlen(cr_filearr[i]) + sizeof(int) * 8 + 2;
    src = (char *) malloc(sizeof(char) * len);

    snprintf(src, len, "%d:%s", p->gps_node, cr_filearr[i]);
    dest = trim_filename(src, suffix);

    if (lam_ssi_cr_verbose >= 40)
      lam_debug(lam_ssi_cr_did, "Renaming %s to %s on node %d", 
                src, dest, p->gps_node);
    
    if (lam_rfrename(src, dest) < 0) {

      /* Unless this is the first iteration of the loop, the set of
         saved checkpoints is not consistent anymore.  We are doomed
         :-( */

      cleanup(cr_filearr, cr_world, cr_world_n);
      free(src);
      free(dest);
      free(cr_checkpoint);
      return LAMERROR;
    }

    free(src);
    free(dest);
  }
  
  /* Unlink phase-1 checkpoints on all nodes and cleanup the array of
     checkpoint-file names */

  cleanup(cr_filearr, cr_world, cr_world_n);
  free(cr_checkpoint);
  return 0;
}


/*
 * get_checkpoint_file
 *
 * Function:
 *  Find out where to put checkpoint files:
 *   1) Specified on mpirun command line (handled in MPIRUN)
 *   2) LAM_MPI_SSI_cr_base_dir env variable
 *   3) LAM_CHECKPOINT_FILE_DIR, set at configure time
 *   4) $HOME
 *
 * Notes:
 *  caller must free() returned string
 */
static char*
get_checkpoint_file(int node, int pid, const char *suffix)
{
  char *tmp = NULL;
  char *crdir = NULL;
  int str_len = 0;
  struct stat crd;

  crdir = lam_ssi_base_param_lookup_string(param_base_dir);

  if ((crdir != NULL) && (strlen(crdir) > 0)
      && (0 == stat(crdir, &crd)) && (S_ISDIR(crd.st_mode))) {

    /* Empty conditional here makes it easier to make the env variable
       / SSI parameter the default choice */
  }

#ifdef LAM_CHECKPOINT_FILE_DIR
  else if ((strlen(LAM_CHECKPOINT_FILE_DIR) != 0)
             && (0 == stat(LAM_CHECKPOINT_FILE_DIR, &crd))
             && (S_ISDIR(crd.st_mode))) {
    crdir = LAM_CHECKPOINT_FILE_DIR;
  } 
#endif

  /* Otherwise, use the $HOME directory */

  else
    crdir = getenv("HOME");

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "setting checkpoint_file_dir to %s", crdir);

  /* crdir now points to the base directory where to store the context
     files.  Assume that p->gps_pid could take sizeof(int) * 8 char to
     print */

  str_len = strlen(crdir) + strlen("/context.") + sizeof(int) * 8 * 3 + 4;
  if (suffix != NULL)
    str_len += strlen(suffix);
  tmp = malloc(sizeof(char) * str_len);
  if (tmp == NULL)
    return NULL;

  snprintf(tmp, str_len, "%s/context.%d-n%d-%d",
           crdir, app_session, node, pid);
  if (suffix != NULL)
    strcat(tmp, suffix);
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "setting checkpoint_file to %s", tmp);

  return tmp;
}


static char *
trim_filename(const char *name, const char *suffix)
{
  int len;
  char *dst;
  char *pos;

  pos = strstr(name, suffix);
  if (pos == NULL)
    return strdup(name);

  len = pos - name;
  dst = (char *) malloc((len + 1) * sizeof(char));
  strncpy(dst, name, len);
  dst[len] = 0;

  return dst;
}


static char *
get_cr_util(const char *util)
{
  char *tmp;

#ifdef LAM_LBNL_LIBCR_PATH
  int str_len;

  /* Hard-code directory where to find cr_checkpoint binary.  BWB:
     This should be removed after development */
  /* JMS: Yes, it should.  But other than checking for a blank dir, at
     this point, it probably won't be. ;-) */

  str_len = strlen(LAM_LBNL_LIBCR_PATH);
  if (str_len > 0) {
      str_len += strlen("/bin/") + strlen(util) + 1;

      tmp = malloc(sizeof(char) * str_len);
      if (tmp == NULL)
          return tmp;

      snprintf(tmp, str_len, "%s%s%s", LAM_LBNL_LIBCR_PATH, "/bin/", util);
  } else {
      tmp = strdup(util);
  }
#else
  tmp = strdup(util);
#endif

  return tmp;
}


static char *
create_app_schema(struct _gps *cr_world, int cr_world_n)
{
  int i;
  char *cr_file = NULL;
  char *cr_restart = NULL;
  char *asc_line = NULL;
  int asc_line_len = 0;
  struct _gps       *p;

  char *tmp_as = NULL;
  int tmp_as_len = 0;

  cr_restart = get_cr_util("cr_restart");
  if (cr_restart == NULL) {
    lam_perror("Getting cr_restart");
    kexit(1);
  }

  for (i = 0, p = cr_world; i < cr_world_n; ++i, ++p) {
    /* get file name */
    cr_file = get_checkpoint_file(p->gps_node, p->gps_pid, NULL);
    if (cr_file == NULL) {
      lam_perror("Getting checkpoint file name");
      kexit(1);
    }

    /* allocate space for this line */
    asc_line_len = strlen(cr_file) + strlen(cr_restart) + 5 +
      sizeof(int) * 8 + 1;
    asc_line = malloc(sizeof(char) * asc_line_len);
    if (asc_line == NULL) {
      return NULL;
    }

    /* make the line */
    snprintf(asc_line, asc_line_len, "n%d %s %s\n",
             p->gps_node, cr_restart, cr_file);

    /* add line to schema */
    tmp_as_len += asc_line_len;
    tmp_as = realloc(tmp_as, sizeof(char) * tmp_as_len);
    if (tmp_as == NULL) {
      return NULL;
    }
    if (tmp_as_len == asc_line_len) {
      tmp_as[0] = '\0';
    }

    strcat(tmp_as, asc_line);

    free(cr_file);
    free(asc_line);
    asc_line_len = 0;
  }

  chkpnt_app_schema = tmp_as;

  if (chkpnt_app_schema == NULL) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "checkpoint app schema build failed");
    return NULL;
  }

  free(cr_restart);

  return tmp_as;
}


static void
cleanup(char **fname_arr, struct _gps *world, int n)
{
  int i;
  struct _gps *p;

  /* 
   * Unlink phase-1 checkpoints of ranks 0 through n-1 and cleanup the
   * array of checkpoint-file names.
   */
  for (i = 0, p = world; i < n; ++i, ++p) {
    char *fname;
    int len;

    len = strlen(fname_arr[i]) + sizeof(int) * 8 + 2;
    fname = (char *) malloc(sizeof(char) * len);

    snprintf(fname, len, "%d:%s", p->gps_node, fname_arr[i]);
    if (lam_ssi_cr_verbose >= 40)
      lam_debug(lam_ssi_cr_did, "unlinking %s", fname);
    lam_rfunlink(fname);
    free(fname);
    free(fname_arr[i]);
  }
  
  free(fname_arr);
  return;
} 

/*
 * lam_ssi_crlam_blcr_lamcheckpoint
 * 
 * Function - Calls cr_checkpoint 
 * Accepts - the pid of mpirun
 */ 
int
lam_ssi_crlam_blcr_lamcheckpoint(pid_t pid)
{
  char *call_checkpoint;
  int str_len = 0;
  char *tmp = NULL;
  char *crdir = NULL;
  struct stat crd;
  
  crdir = lam_ssi_base_param_lookup_string(param_base_dir);
  
  if ((crdir != NULL) && (strlen(crdir) > 0)
      && (0 == stat(crdir, &crd)) && (S_ISDIR(crd.st_mode))) {
    
    /* Empty conditional here makes it easier to make the env variable
       / SSI parameter the default choice */
  }
  
#ifdef LAM_CHECKPOINT_FILE_DIR
  else if ((strlen(LAM_CHECKPOINT_FILE_DIR) != 0)
	   && (0 == stat(LAM_CHECKPOINT_FILE_DIR, &crd))
	   && (S_ISDIR(crd.st_mode))) {
    crdir = LAM_CHECKPOINT_FILE_DIR;
  } 
#endif
  
  /* Otherwise, use the $HOME directory */
  
  else
    crdir = getenv("HOME");
  
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "setting checkpoint_file_dir to %s", crdir);
  
  
  /* crdir now points to the base directory where to store the context
     files.  Assume that pid could take sizeof(int) * 8 char to
     print */
  
  str_len = strlen(crdir) + strlen("/context.") + strlen("mpirun.") 
    + (sizeof(int) * 8) + 1;
  
  tmp = malloc(sizeof(char) * str_len);
  
  if (tmp == NULL) {
    lam_perror("mallocing temp");
    kexit(1);
  }
  
  snprintf(tmp, str_len, "%s/context.%s.%d",
           crdir, "mpirun", pid);
  
  str_len += strlen("cr_checkpoint -f ") + (sizeof(int) * 8) + strlen(" ") + 1;
  call_checkpoint = malloc(sizeof(char) * str_len);
  
  if (call_checkpoint == NULL) {
    free(tmp);			
    return -1;
  }
  
  snprintf(call_checkpoint, str_len, "%s%s%s%d", 
	   "cr_checkpoint -f ", tmp, " ", pid); 
  
  if (system(call_checkpoint) == -1) {
    free(call_checkpoint);
    free(tmp);
    return -1;
  }
  
  free(tmp);
  free(call_checkpoint);
  return 0;
}


/*
 * lam_ssi_crlam_blcr_lamrestart
 * 
 * Function - Calls cr_restart
 */ 
int 
lam_ssi_crlam_blcr_lamrestart(char *temp)
{
  int param_context_file;
  int str_len = 0;
  char *context_file;
  char *tmp;

  /* Register the parameter for the name of the Context file */  
  param_context_file = lam_ssi_base_param_register_string("cr", "blcr", 
      "context_file", NULL, NULL);

  /* Lookup the SSI parameter for the name of the Context file */
  context_file =
    lam_ssi_base_param_lookup_string(param_context_file);

  if (context_file != NULL && strlen(context_file) != 0) {
    str_len = strlen("cr_restart ") + strlen(context_file) + 1;
    
    tmp = malloc(str_len * sizeof(char));
    
    if (tmp == NULL) {
      lam_perror("mallocing temp");
      free(context_file);
      kexit(1);
    }
    
    /* cr_restart takes the name of the Context file as argument */
    snprintf(tmp, str_len, "%s%s", "cr_restart ", context_file);
 
    if (system(tmp) == -1) {
      free(tmp);
      free(context_file);
      lam_perror("Delivering cr_restart");
      kexit(1);
    }
  } else {
    /* Nothing we can do, except to abort */
    show_help("lamrestart", "usage", NULL);
    kexit(1);
  }

  free(tmp);
  free(context_file);
	return 0;
}
