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
 * $Id: ssi_crlam_self.c,v 1.2 2004/03/09 16:56:14 pcharapa Exp $
 *
 *	Function:	- SELF crlam module 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#include <lam_ltdl.h>
#include <lam_config.h>
#include <app_mgmt.h>
#include <etc_misc.h>
#include <priority.h>
#include <freq.h>
#include <preq.h>
#include <sfh.h>

#include <lam-ssi-cr.h>
#include <lam-ssi-crlam-self.h>
#include <lam-ssi-crlam-self-config.h>
#include <lamthreads.h>

#define signal_puts(S)  write(STDOUT_FILENO, (S), strlen(S))

/*
 * local functions
 */

static int signal_app(struct _gps *, int);
static void signal_handler(int sig);
static void *thread_handler(void *);
static char *get_cr_util(const char* util);
static char *create_app_schema(struct _gps *, int);
static char *get_checkpoint_file(int node, int pid, const char *suffix);
static char *extract_argv(char **argv);

/*
 * local variables
 */
static lam_ssi_crlam_actions_t module_actions = {
  lam_ssi_crlam_self_checkpoint,
  lam_ssi_crlam_self_continue,
  lam_ssi_crlam_self_disable_checkpoint,
  lam_ssi_crlam_self_enable_checkpoint,
  lam_ssi_crlam_self_finalize,
  lam_ssi_crlam_self_init,
  /* use the base utility function for restart action */
  lam_ssi_crlam_base_do_exec,
  lam_ssi_crlam_self_lamcheckpoint,
  lam_ssi_crlam_self_lamrestart
};


static char *chkpnt_app_schema = NULL;
static int app_session = 0;
static int param_priority;
static int param_base_dir;
static int param_restart_args;  

static char *appschema_argv_line; 
static struct _gps *cr_world = NULL;
static int cr_world_n = -1;
static int crlam_self_filedesc[2];
lam_thread_t crlam_self_thread;

typedef enum {
  LAM_SSI_CRLAM_SELF_THREAD_STATUS_GO,
  LAM_SSI_CRLAM_SELF_THREAD_STATUS_DONE
} lam_ssi_crlam_self_thread_status_t; 

/*
 * Open the module and register SSI parameters
 */
  int
lam_ssi_crlam_self_open_module(OPT *ad)
{
  param_base_dir = 
    lam_ssi_base_param_register_string("cr", "self", "base_dir", NULL, NULL);

  param_priority = 
    lam_ssi_base_param_register_int("cr", "self", "priority", NULL,
        LAM_SSI_CRLAM_SELF_DEFAULT ? 75 : 75);

  /* arguments to be passed to the application on restart */

  param_restart_args =
    lam_ssi_base_param_register_string("cr", "self", "restart_args", 
                                       NULL, NULL);
  lam_ssi_base_param_register_int("cr", "self", "do_restart", NULL, 0);

  return 1;
}


/*
 * lam_ssi_crlam_self_query
 *
 * Function: determine if the module wants to run.
 */
const lam_ssi_crlam_actions_t *
lam_ssi_crlam_self_query(int *priority)
{
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  /* If we're verbose, print stuff out */
  if (lam_ssi_cr_verbose >= 5) {
    lam_debug(lam_ssi_cr_did, "self: module initializing");
    lam_debug(lam_ssi_cr_did, "self:verbose: %d",
        lam_ssi_cr_verbose);
    lam_debug(lam_ssi_cr_did, "self:priority: %d", *priority);
  }

  /* Return the struct with the function pointers in it for all the APIs */
  return (&module_actions);
}


/*
 *	lam_ssi_crlam_self_init
 *
 *	Function:	- primary initialization of CR subsystem
 *	Returns		- 0 or LAMERROR
 */
int
lam_ssi_crlam_self_init(char *path, char **argv, OPT *ad, 
    struct _gps *world, int world_n)
{

  /* 
   * Create a pipe for the communication between the Thread hanlder 
   * and the Signal handler 
   */

  if (pipe(crlam_self_filedesc) != 0) {    
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-init",
        "pipe-creation-fail", NULL);

    return LAMERROR;
  }

  /*
   * get the values of the number of processes and their gps information
   */

  cr_world = world;
  cr_world_n = world_n;

  /*
   * Register the checkpoint signal handler
   */

  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-init", 
        "pipe-creation-fail", NULL); 

    close(crlam_self_filedesc[0]);
    close(crlam_self_filedesc[1]);
    return LAMERROR;
  }

  /*
   * Create a Thread, the passed function argument is a function that blocks
   * this thread on the pipe we just created
   */

  if (lam_thread_create(&crlam_self_thread, thread_handler, 0) != 0) {
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-init",
        "thread-creation-fail", NULL);

    close(crlam_self_filedesc[0]);
    close(crlam_self_filedesc[1]);
    return LAMERROR;
  } 

  appschema_argv_line = extract_argv(argv);

  /* setup the argv to be used at restart */

  if (lam_ssi_crlam_base_create_restart_argv(argv, ad) < 0)
    return LAMERROR;

  return 0;
}

static char*
extract_argv(char **argv) 
{
  char *tmp = NULL;
  char *tmp_argv;
  int i;
  int str_len = 0;
  int argc = sfh_argv_count(argv);	

  for (i = 1; i < argc; ++i) {
    str_len += strlen(argv[i]) + 1;
    if (tmp != NULL) {
      tmp_argv = strdup(tmp);
      tmp = realloc(tmp, sizeof(char) * str_len);
    } else {
      str_len += strlen("# ") + 1; 
      tmp_argv = malloc(sizeof(char) * str_len);
      snprintf(tmp_argv, str_len, "%s", "# ");
      tmp = malloc(sizeof(char) * str_len);
    }
    snprintf(tmp, str_len, "%s %s", tmp_argv, argv[i]); 
  }

  /*
   * add a new line char to the end of the string, this hopefully
   * should make it easier when adding the argv string to the app_schema
   */ 

  tmp_argv = strdup(tmp);
  str_len += strlen("\n");
  tmp = realloc(tmp, str_len * sizeof(char));
  snprintf(tmp, str_len, "%s\n", tmp_argv); 
  free(tmp_argv);
  return tmp;
}

/*
 *	lam_ssi_crlam_self_finalize
 *
 *	Function:	- crlam cleanup
 *	Returns:	- 0 or LAMERROR
 */

int
lam_ssi_crlam_self_finalize(void)
{
  lam_ssi_crlam_self_thread_status_t status = 
    LAM_SSI_CRLAM_SELF_THREAD_STATUS_DONE;

  /* Tell the thread to exit */

  if (write(crlam_self_filedesc[1], &status, sizeof(status)) != 
      sizeof(status)) {
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-signal",
        "write-fail", NULL);

    close(crlam_self_filedesc[0]);
    close(crlam_self_filedesc[1]);
    return LAMERROR;
  }

  /* Wait for the thread to exit */

  if (lam_thread_join(crlam_self_thread, NULL) == -1) {
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-init",
        "thread-join-fail", NULL);
    return LAMERROR;
  } 

  /* All done */

  return 0;
}


/*
 *      lam_ssi_crlam_self_checkpoint
 *
 *      Function:       - crlam checkpoint
 *      Returns:        - 0 or LAMERROR
 */

int 
lam_ssi_crlam_self_checkpoint(void)
{
  pid_t pid;
  int status;

  /* 
   * fork a child to propagate the checkpoint request to all the MPI
   * processes. 
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

    if (signal_app(cr_world, cr_world_n) != 0)
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


/* lam_ssi_crlam_self_continue
 *
 * Function:        - crlam continue
 * Returns:         - 0 or LAMERROR
 */
int
lam_ssi_crlam_self_continue(void)
{
  /* no-op.*/
  return 0;
}


/* 
 * lam_ssi_crlam_self_disable_checkpoint
 *
 * Function:        - disable checkpoints till the caller explicitly
 *                    calls _enable_checkpoint to enable it.
 */
void
lam_ssi_crlam_self_disable_checkpoint(void)
{
  return;
}


/*
 * lam_ssi_crlam_self_enable_checkpoint
 *
 * Function:        - enable checkpointing
 */
void
lam_ssi_crlam_self_enable_checkpoint(void)
{
  /* pass the same arg that was passed to the call to cr_enter_cs() */

  return;
}


/*
 *	signal_handler
 *
 *	Function:	- unblock the Thread by writing 'go'
 *	Returns		- 0 or LAMERROR
 */

static void
signal_handler(int sig)
{
  lam_ssi_crlam_self_thread_status_t status = 
    LAM_SSI_CRLAM_SELF_THREAD_STATUS_GO;

  /* 
   * Write to the Pipe to unblock the Thread
   */

  if (write(crlam_self_filedesc[1], &status, sizeof(status)) != 
      sizeof(status)) {
    show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-signal",
        "write-fail", NULL);

    close(crlam_self_filedesc[0]);
    close(crlam_self_filedesc[1]);
    /* JMS: Need to do something here...*/
  }
}


/*
 *	thread_handler
 *
 *	Function:	- Thread handler code
 *	Returns		- 
 */
static void *
thread_handler(void *foo)
{
  lam_ssi_crlam_self_thread_status_t status;

  /* 
   * Loop forever blocking on the Pipe, if the status read is 'go'
   * print something out, if it's 'done', exit
   */
  while (1) {
    /*
     * Block by reading on the Pipe until the Signal handler writes
     * something to the Pipe
     */
    if (read(crlam_self_filedesc[0], &status, sizeof(status)) == -1) {
      show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-thread",
          "read-fail", NULL);

      close(crlam_self_filedesc[0]);
      close(crlam_self_filedesc[1]);
      printf("Something bad has happened here\n");
    }

    /*
     * If the status is 'done', exit
     */
    if (status == LAM_SSI_CRLAM_SELF_THREAD_STATUS_DONE) {
      printf("All done!  CRLAM thread handler exiting\n");
      lam_thread_exit(NULL);
    }

    /*
     * Call the glue checkpoint
     */
    if (lam_ssi_crlam_base_checkpoint(cr_world, cr_world_n) != 0) {
      show_help_file("lam-ssi-crlam-self-helpfile", "cr-self-thread",
          "checkpoint-fail", NULL);
    }

    if (lam_ssi_crlam_base_continue() != 0) {
      if (lam_ssi_cr_verbose >= 0) 
        lam_debug(lam_ssi_cr_did, "mpirun_ASYNC%d: prepare for continue "
            "failed.", getpid());
      kexit(1);
    }

  } /* end of for loop */

  return (void *) status;
}


/*
 * signal_app
 *
 * Function:
 * Signal all processes running under itself to checkpoint
 */
static int
signal_app(struct _gps *cr_world, int cr_world_n)
{
  int i;
  char **cr_argv = NULL;
  int cr_argc = 0;
  struct penv cr_env;
  char pid_arg[sizeof(int) * 8];
  char *cr_self_checkpoint;
  struct _gps *p;
  int cr_pid, cr_idx;

  cr_self_checkpoint = strdup("kill");

  for(i = 0, p = cr_world; i < cr_world_n; ++i, ++p) {
    /* setup kenya request */
    cr_env.pe_rank = p->gps_node;
    strcpy(cr_env.pe_wrkdir, "");
    cr_env.pe_envv = 0;

    /* Set up program arguments */
    sfh_argv_add(&cr_argc, &cr_argv, "kill");
    sfh_argv_add(&cr_argc, &cr_argv, "-USR1");

    snprintf(pid_arg, sizeof(pid_arg), "%d", p->gps_pid);
    sfh_argv_add(&cr_argc, &cr_argv, pid_arg);

    if (rploadgov(p->gps_node, p->gps_node, cr_self_checkpoint, RTF_WAIT, 
          cr_argv, &cr_env, &cr_pid, &cr_idx)) {
      /* We can't do anything more than abort... */
      lam_perror("rploadgov failed.");
      return LAMERROR;
    }

    free(cr_argv);
    cr_argv = NULL;
    cr_argc = 0;
  }

  free(cr_self_checkpoint);
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
  tmp = malloc(sizeof(char) * str_len);
  if (tmp == NULL)
    return NULL;

  snprintf(tmp, str_len, "%s/context.%d-n%d-%d",
      crdir, app_session, node, pid);

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "setting checkpoint_file to %s", tmp);

  return tmp;
}


static char *
get_cr_util(const char *util)
{
  char *tmp;
  tmp = strdup(util);
  return tmp;
}


static char *
create_app_schema(struct _gps *cr_world, int cr_world_n)
{
  int i;
  char *cr_self_file = NULL;
  char *cr_self_restart = NULL;
  char *asc_line = NULL;
  int asc_line_len = 0;
  struct _gps       *p;
  char *tmp_as = NULL;
  int tmp_as_len = 0;

  /*
   * PC: This is the new section of the code wherein we add
   * the line of argvs to the first line of the app_schema
   */

  cr_self_restart = get_cr_util("cr_self_restart");
  if (cr_self_restart == NULL) {
    lam_perror("Getting cr_self_restart");
    kexit(1);
  }

  for (i = 0, p = cr_world; i < cr_world_n; ++i, ++p) {
    /* get file name */
    cr_self_file = get_checkpoint_file(p->gps_node, p->gps_pid, NULL);
    if (cr_self_file == NULL) {
      lam_perror("Getting checkpoint file name");
      kexit(1);
    }

    /* allocate space for this line */
    asc_line_len = strlen(cr_self_file) + strlen(cr_self_restart) + 5 +
      sizeof(int) * 8 + 1;
    asc_line = malloc(sizeof(char) * asc_line_len);
    if (asc_line == NULL) {
      return NULL;
    }

    /* make the line */
    snprintf(asc_line, asc_line_len, "n%d %s %s\n",
        p->gps_node, cr_self_restart, cr_self_file);

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

    free(cr_self_file);
    free(asc_line);
    asc_line_len = 0;
  }

  chkpnt_app_schema = tmp_as;

  if (chkpnt_app_schema == NULL) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "checkpoint app schema build failed");
    return NULL;
  }

  free(cr_self_restart);

  strcat(appschema_argv_line, tmp_as);
  free(tmp_as);
  tmp_as = strdup(appschema_argv_line);

  return tmp_as;
}

int
lam_ssi_crlam_self_lamcheckpoint(pid_t pid)
{
  if (kill(pid, SIGUSR1) == -1) {
    lam_perror("No such process");
    return -1;
  }

  return 0; 
}

int
lam_ssi_crlam_self_lamrestart(char *executable)
{
  char *restart_args;
  char **argv;
  char *temp;
  int str_len = 0;
  char *args;
  char *env[] = {(char *) 0};

  /* lookup the restart args */ 
  restart_args = 
    lam_ssi_base_param_lookup_string(param_restart_args);

  lam_ssi_base_param_set_int("cr_self_do_restart", 1);

  /* exec a brand new mpirun
   *
   * NOTE: POSIX.1 guarantees only execle and execve to be reentrant
   * This means we need a full path to mpirun. So, we assume the
   * installation directory is still correct. */

  if ((restart_args != NULL) && (strlen(restart_args) != 0)) {
    str_len = strlen("mpirun") + strlen(restart_args) + 2;
    temp = malloc(str_len * sizeof(char));

    snprintf(temp, str_len, "%s %s", "mpirun", restart_args);

    args = strdup(temp);
    argv = sfh_argv_break(args, ' ');
    free(args);
    free(temp);  

    if (execve(executable, argv, env) < 0) {
      /* error in execve */
      signal_puts("Error in execve\n");
      free(restart_args);
      free(argv);
      exit(errno);
    }
  } else {
    /* Nothing can be done if no arguments are given, exit */  
    show_help("lamrestart", "usage", NULL);
    free(restart_args);
    exit(1);
  }

  free(restart_args);
  free(argv);
  return 0;
}

