/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 * $Id: mpirun.c,v 6.122 2003/11/15 05:40:53 pkambadu Exp $
 *
 *  Function: - start an MPI application
 */

#include <lam_config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sfh.h>
#include <dl_inet.h>
#include <all_list.h>
#include <all_opt.h>
#include <app_mgmt.h>
#include <app_schema.h>
#include <args.h>
#include <events.h>
#include <freq.h>
#include <kio.h>
#include <laminternal.h>
#include <ndi.h>
#include <net.h>
#include <portable.h>
#include <priority.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>
#include <etc_misc.h>
#include <lam-ssi.h>
#include <mpi_apps.h>
#include <lam-ssi-cr.h>
#include <lam-totalview.h>

/*
 * Leave this here because although PTY support has been added and
 * we'd like to make it the default, it's considered too "iffy" at the
 * moment.  But we may make it the default someday.
 */
#define PTY_IS_DEFAULT 1

/*
 * exported functions
 */
int pwait(int4 *nwait, int *childstat);


/*
 * private functions 
 */
static int set_mode(void);
static int set_stdio(void);
static void reset_stdio(void);
static void signal_handler(int sig);
static void first_signal_handler(int sig);
static LIST *build_app(int argc, char **argv, char **env);
static char *locate_aschema(char *filename);
static void node_ranges(char **buf, int *len, int4 initnode, int4 initindex);
static int get_mpi_world(int4 world_n, struct _gps *world, 
                         struct _gps *mpiworld);
static void error_cleanup(int np);
static void str_append(char **src, int *len, char *suffix);
#if LAM_WANT_IMPI
static int send_impi_params(struct _gps *);
#endif
static void show_status(int pid, int nodeid, int killed_by_signal, int status);
static void node_id_str(int node_id, char* snode, int snode_len);
static int match_ssi_modules(int4, struct _gps *);


/*
 * external variables
 */
extern struct kio_t _kio;     /* kernel I/O block */
extern struct fclient _ufd[FUMAX];    /* user file desc. */
extern char **environ;

/*
 * exported variables
 */
struct _gps *world;     /* world GPS array */
struct _gps *mpiworld;    /* MPI world GPS array */
int   world_n;    /* size of world */
int   checkpoint_world_n; /* size of world */
int              fl_verbose;             /* verbose mode */
int              fl_very_verbose;        /* verbose mode */


/*
 * private variables
 */
static int    fl_fdesc;   /* don't open stdio */
static int    fl_nprocs;    /* # procs specified */
static int    fl_wait = 1;    /* wait for completion */
static int    fl_catch = 0;   /* catch signals */
static int    fl_impi_client = 0; /* IMPI client or not */
static int    fl_impi_server = 0; /* IMPI server or not */
static int4   nprocs;     /* # procs */
static int4   rtf;      /* runtime flags */
static char   *wrkdir = NULL;   /* working dir */
static char   smallbuf[128];    /* small extra buffer */
static char   *syntaxerr =
  "mpirun: application schema syntax error, line %d\n";
OPT   *ad;      /* argument descriptor */
#if LAM_WANT_IMPI
struct _gps *impi_server_gps = 0;
#endif
static char **main_argv = NULL;


/* boolean flags for options given at command line regarding
   displaying exit status for all processes */ 
static int    fl_show_all_status = FALSE; /* display exit status 
                                           * for all processes */ 
static int    fl_status_if_fail = FALSE;  /* display exit status
                                           * for all only if one
                                           * of them fails */
static char   *prefix_str = "mpirun:";    /* prefix string to be 
                                           * displayed at begining
                                           * of each line outputting
                                           * exit status */

/* the name of the selected CRMPI module that needs to be opened in CRLAM */
static char *cr_module;

/* The following structure is used to store the info about the exist
   status of processes. */

struct ProcessStatusStruct {
  int4 nodeid;
  int pid;
  int killed_by_signal;
  int status;
};


/*
 * External functions
 */
extern void mpirun_cr_create_restart_argv(char **, OPT *);


int
main(int argc, char **argv)
{
  LIST *app;
  int status;
  int errno_save;
  char **env;
  sigset_t sigmask;
#if LAM_WANT_IMPI
  char *impi_server = 0;
  int impi_argc = 0;
  char **impi_argv = 0;
  LIST *impi_server_app;
#endif

/*
 * Ensure that we are not root.
 */
#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0) {
      show_help(NULL, "deny-root", NULL);
      exit(EACCES);
  }
#endif

  /* It is possible that this mpriun will have been exec()'ed from a
     re-started mpirun.  In this case, due to a complicated series of
     events (described below), this process may have a boatload of
     signals blocked.  So we always unblock all signals here.  In the
     non-restart case, it does nothing.  But in the restart case,
     we're zeroing out the blocked signal mask so that we can receive
     signals (like ctrl-C/SIGINT). 

     This happens in BLCR, for example, because when mpirun is
     checkpointed, its thread and signal callbacks are invoked.
     Before invoking them, BLCR blocks the boatload of signals
     (presumably to prevent race conditions).  The BLCR CRLAM SSI
     module eventually invokes cr_checkpoint().  Upon restart, we
     return from cr_checkpoint() and still have all the signals
     blocked.  But we don't return from the signal handler -- we
     simply exec() a new mpirun.  Blocked signals are inherited across
     exec(), hence the new mpirun starts up with a whole boatload of
     blocked signals.
  */
  sigemptyset(&sigmask);
  sigprocmask(SIG_SETMASK, &sigmask, NULL);

  main_argv = sfh_argv_dup(argv);

/*
 * Parse the command line.
 */
  ad = ao_init();
  if (ad == 0) {
    perror("mpirun (ao_init)");
    exit(errno);
  }
  ao_setopt1(ad, "bfhtvDO", 0, 0, 0);
  ao_setopt(ad, "client", 0, 2, 0);
  ao_setopt(ad, "server", 0, 1, 0);
  ao_setopt(ad, "s", 0, 1, 0);
  ao_setopt(ad, "c", 0, 1, AOINT);
  ao_setopt(ad, "np", "c", 1, AOINT);
  ao_setopt(ad, "c2c", 0, 0, 0);
  ao_setopt(ad, "lamd", "c2c", 0, 0);
  ao_setopt(ad, "ger", 0, 0, 0);
  ao_setopt(ad, "nger", "ger", 0, 0);
  ao_setopt(ad, "w", 0, 0, 0);
  ao_setopt(ad, "nw", "w", 0, 0);
  ao_setopt(ad, "toff", 0, 0, 0);
  ao_setopt(ad, "ton", "toff", 0, 0);
  ao_setopt(ad, "sigs", 0, 0, 0);
  ao_setopt(ad, "nsigs", "sigs", 0, 0);
  ao_setopt(ad, "x", 0, 1, 0);
  ao_setopt(ad, "vv", 0, 0, 0);
  ao_setopt(ad, "nx", 0, 0, 0);
  ao_setopt(ad, "wd", "D", 1, 0);
  ao_setopt(ad, "pty", 0, 0, 0);
  ao_setopt(ad, "npty", "pty", 0, 0);
  ao_setopt(ad, "l", 0, 0, 0);
  ao_setopt(ad, "rmschema", 0, 0, 0);

  ao_setopt(ad, "p", 0, 1, 0);
  ao_setopt(ad, "sa", "sf", 0, 0);
  ao_setopt(ad, "sf", "sa", 0, 0);

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  /* If we're going to run with totalview support, we need to
     initialize the totalview interface */

  lam_tv_init(argc, argv, ad);
  
  if (asc_compat(&argc, &argv, ad)) {
    errno_save = errno;
    sfh_argv_free(main_argv);
    ao_free(ad);
    errno = errno_save;
    perror("mpirun");
    lam_ssi_base_close();
    exit(errno_save);
  }

  if (ao_parse(ad, &argc, argv))  {
    errno_save = errno;
    show_help("mpirun", "usage", NULL);
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    exit(errno_save);
  }

  /* Check if mpirun was started with -tv option. exec
     "totalview mpirun -a ..." if this convenience argv option
     -tv was given */

  lam_tv_check(ad);

/*
 * Check for help request.
 */
  if (ao_taken(ad, "h")) {
    show_help("mpirun", "usage", NULL);
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    exit(0);
  }
/*
 * Set the operating mode.
 */
  if (set_mode()) {
    errno_save = errno;
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    exit(errno_save);
  }
/*
 * Attach to kernel.
 */
  if (kinit(PRCMD)) {
    char hostname[MAXHOSTNAMELEN];

    errno_save = errno;
    gethostname(hostname, MAXHOSTNAMELEN);
    show_help(NULL, "no-lamd", "mpirun", hostname, NULL);
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    exit(errno_save);
  }
/*
 * Set job identifier to be inherited by the application.
 */
  _kio.ki_jobid.jid_node = getnodeid();
  _kio.ki_jobid.jid_pid = lam_getpid();
/*
 * Get the job environment.
 */
  env = 0;
  if (asc_environment(!ao_taken(ad, "nx"),
                      ao_taken(ad, "x") ? 
                      ao_param(ad, "x", 0, 0) : NULL, &env)) {
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    lamfail("mpirun");
  }

/*
 * Build the application and allocate the world GPS array.
 */
  if ((app = build_app(argc, argv, env)) == 0) {
    lam_ssi_base_close();
    kexit(errno);
  }

  checkpoint_world_n = world_n = al_count(app);
  world = (struct _gps *) malloc(world_n * sizeof(struct _gps));
  mpiworld = (struct _gps *) malloc(world_n * sizeof(struct _gps));
  if (world == 0 || mpiworld == 0)
      lamfail("mpirun");
/*
 * Pre-qualify the GER and set buffer limitations on each node.
 */
#if 0
  if (ao_taken(ad, "lamd") && !ao_taken(ad, "nger")) {
    if (asc_ger(app, TRUE)) {
      errno_save = errno;
      ao_free(ad);
      lam_ssi_base_close();
      kexit(errno_save);
    }
  }
#endif

/* Set the first signal handler. This is an unsafe region to 
 * quit the application. Hence, we have a special signal handler
 * to notify the user of this. A second signal during this period 
 * will cause the generic signal handler "signal_handler()" to
 * be called
 */
 if (fl_wait) {
  _lam_signal (SIGINT, first_signal_handler);
  _lam_signal (SIGTERM, first_signal_handler);
 }

/*
 * Set application stdio.
 */
  if (set_stdio()) {
    lam_ssi_base_close();
    lamfail("mpirun (set_stdio)");
  }

/* Set up boolean variables for displaying exit status 
 * of processes depending on option selected 
 */

  if (ao_taken(ad, "sa") && ao_taken(ad, "sf")) {
    printf("cannot select both options\n");
  }

  if (ao_taken(ad, "sa") && !ao_taken(ad, "sf")) {
    fl_show_all_status = TRUE;
    fl_status_if_fail = FALSE;
  }

  if (!ao_taken(ad, "sa") && ao_taken(ad, "sf")) {
    fl_status_if_fail = TRUE;
    fl_show_all_status = FALSE;
  }

  if (!ao_taken(ad, "sa") && !ao_taken(ad, "sf")) {
    fl_show_all_status = FALSE;
    fl_status_if_fail = FALSE;
  }

/*
 * Run the application.
 */
  if (asc_run(app, 0, rtf, fl_verbose, TRUE, world)) {
      errno_save = errno;
      sfh_argv_free(main_argv);
      ao_free(ad);
      error_cleanup(-1);
      reset_stdio();
      lam_ssi_base_close();
      kexit(errno_save);
  }
/*
 * Set signal handler to terminate the application.
 * Vulnerable period is over. Now, the generic signal
 * handler can take over 
 */
  if (fl_wait) {
    _lam_signal(SIGINT, signal_handler);
    _lam_signal(SIGTERM, signal_handler);
  }

  reset_stdio();
  
/*
 * Read pids and indices from MPI processes.
 */
  if (get_mpi_world(world_n, world, mpiworld)) {
      errno_save = errno;
      ao_free(ad);
      sfh_argv_free(main_argv);
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(errno_save);
  }

  /* Attach total view if we are being debugged */

  lam_tv_attach(app);

/*
 * Send process table to all processes.
 */
  if (app_sendprocs(world_n, mpiworld)) {
      ao_free(ad);
      sfh_argv_free(main_argv);
      terror("mpirun");
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(errno);
  }
#if LAM_WANT_IMPI
/*
 * If this is an IMPI application, send the IMPI params to rank 0 so
 * that it can spawn the impid properly.
 */
  if (fl_impi_client && send_impi_params(mpiworld) != 0) {
    errno_save = errno;
    ao_free(ad);
    sfh_argv_free(main_argv);
    terror("mpirun (send_impi_params)");
    app_doom(world_n, mpiworld, SIGUDIE);
    app_doom(world_n, world, -15);
    error_cleanup(world_n);
    lam_ssi_base_close();
    kexit(errno_save);
  }
/*
 * Start up the IMPI server; all the MPI ranks are now somewhere in
 * MPI_Init, and probably blocking, waiting for the IMPI server to
 * come up.  This flag will only be set if LAM_WANT_IMPI_SERVER, so we
 * don't have to check for this here.
 */
  if (fl_impi_server) {
    impi_server = sfh_path_env_findv(LAM_IMPI_SERVER, R_OK, environ, 0);

    if (impi_server == NULL) {
      show_help("mpirun", "cant-find-impi-server", NULL);
      ao_free(ad);
      sfh_argv_free(main_argv);
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(1);
    }

/*
 * Launch the impi server via the lamd on the localhost.  This allows
 * the impi-server to be cleaned up via lamclean.
 */
    impi_argc = 0;
    impi_argv = 0;
    sfh_argv_add(&impi_argc, &impi_argv, "bogus-ignored-arg");
    sfh_argv_add(&impi_argc, &impi_argv, "h");
    sfh_argv_add(&impi_argc, &impi_argv, impi_server);

    if ((impi_server_app = build_app(impi_argc, impi_argv, env)) == 0) {
      errno_save = errno;
      ao_free(ad);
      sfh_argv_free(main_argv);
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(errno_save);
    }

    impi_server_gps = (struct _gps *) 
      malloc(world_n * sizeof(struct _gps));
    rtf &= ~(RTF_MPI | RTF_WAIT);
    if (asc_run(app, 0, rtf, fl_verbose, TRUE, impi_server_gps)) {
      errno_save = errno;
      ao_free(ad);
      sfh_argv_free(main_argv);
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(errno_save);
    }
  }
#endif

        /* 
         * Check to ensure that the RPI and CRMPI modules selected by the 
         * processes match.
         */
  if (match_ssi_modules(world_n, mpiworld)) {
      errno_save = errno;
      ao_free(ad);
      sfh_argv_free(main_argv);
      app_doom(world_n, mpiworld, SIGUDIE);
      app_doom(world_n, world, -15);
      error_cleanup(world_n);
      lam_ssi_base_close();
      kexit(errno_save);
  }

        /* 
         * open the CRLAM SSI kind for checkpoint/restart support.  Use the 
         * same module that was chosen for the CRMPI kind by all the MPI 
         * processes.
         */
  if (lam_ssi_crlam_base_open(ad, cr_module) < 0) {
          free(cr_module);
          errno_save = errno;
          ao_free(ad);
          sfh_argv_free(main_argv);
          app_doom(world_n, mpiworld, SIGUDIE);
          app_doom(world_n, world, -15);
          error_cleanup(world_n);
          lam_ssi_base_close();
          kexit(errno_save);
  }
  free(cr_module);

  /* initialize the crlam SSI module */
  if (lam_ssi_crlam.lscrla_init != NULL) {  
    if (lam_ssi_crlam.lscrla_init(LAM_BINDIR "/mpirun", 
                                  main_argv, ad, mpiworld, 
                                  checkpoint_world_n)) {
      /* The init function will have already showed the error.  So we
         can just kexit here */
      sfh_argv_free(main_argv);
      ao_free(ad);
      lam_ssi_base_close();
      kexit(1);
    }
  }

  /*
   * If needed, wait for the application to terminate.
   */
  status = 0;

  if (fl_wait) {
    if (pwait(&world_n, &status)) {
      if (fl_very_verbose)
        printf("mpirun: killing application (nicely)\n");
      app_doom(world_n, mpiworld, SIGUDIE);
      if (fl_very_verbose)
        printf("mpirun: killing application (with extreme prejudice)\n");
      app_doom(world_n, world, -15);
#if LAM_WANT_IMPI
      if (impi_server_gps) {
        if (fl_very_verbose)
          printf("mpirun: killing the impid\n");
        app_doom(world_n, impi_server_gps, -15);
      }
#endif
      if (fl_very_verbose)
        printf("mpirun: checking for other error cleanup\n");
      error_cleanup(world_n);
    }

    microsleep(500000);   /* 1/2 sec */
  } else 
    if (fl_very_verbose)
      printf("mpirun: not waiting for application to terminate\n");

  /* free up argv memory allocated for TV */
  lam_tv_finalize();

  ao_free(ad);
  sfh_argv_free(main_argv);
  lam_ssi_base_close();
  kexit(status);
  return(0);
}

/*
 *  build_app
 *
 *  Function: - build application from command line specification
 *  Accepts:  - command line argument count
 *      - command line argument vector
 *  Returns:  - application descriptor or 0
 */
static LIST *
build_app(int argc, char **argv, char **env)
{
  LIST    *app;     /* application */
  LIST    *app_sched;   /* scheduled application */
  int4    nodeid;     /* target node */
  int4    index;      /* for getnode() */
  int4    nflags;     /* for getnode() */
  int   lineno;     /* line number */
  int   argtailc;   /* argument tail count */
  char    **argtailv;   /* argument tail vector */
  char    *aschema;   /* application schema */
  char    *qarg;      /* quoted argument */
  char    *fmtbuf = malloc(512);  /* formatting buffer */
  int              fmtbuf_len = 512;

  if (nid_parse(&argc, argv) || (errno = (argc > 1) ? 0 : EUSAGE)) {
    show_help("mpirun", "usage", NULL);
    return(0);
  }
/*
 * Application schema or command line?
 */
  nid_get(&index, &nodeid, &nflags);

  if ((index < 0) && !fl_nprocs) {
/*
 * Parse the app. schema.
 */
    if ((argc != 2) || ao_taken(ad, "s")) {
      show_help("mpirun", "usage", NULL);
      lam_ssi_base_close();
      kexit(EUSAGE);
    }

    aschema = locate_aschema(argv[1]);
    if (aschema == 0) {
      fprintf(stderr, "mpirun (locate_aschema): %s: ",
              argv[1]);
      terror("");
      return(0);
    }

    app = asc_parse(aschema, &lineno, env);

    /* In checkpoint/restart scenarios, we automatically launch
       another mpirun with a boot schema to restart the checkpointed
       application.  In this scenario, we want to remove the
       automatically-generated appschema file.  Hence, we provide an
       undocumented option (-rmschema) option to do this. */

    if (ao_taken(ad, "rmschema"))
      remove(aschema);
  }
  else {
/*
 * Parse the "command line" application specification.
 * Reconstruct its relevant parts from the parsed information.
 * It was parsed to distinguish between the appl/cmdline cases.
 *
 * Format: [<nodes>] program [-s <srcnode>] [-c #] [-- args]
 *
 */
    fmtbuf[0] = 0;

    if (fl_nprocs) {
      sprintf(smallbuf, "-c %d ", nprocs);
      str_append(&fmtbuf, &fmtbuf_len, smallbuf);
    }

    if (ao_taken(ad, "s")) {
      str_append(&fmtbuf, &fmtbuf_len, "-s ");
      str_append(&fmtbuf, &fmtbuf_len, ao_param(ad, "s", 0, 0));
      str_append(&fmtbuf, &fmtbuf_len, " ");
    }

    if (index == 0) {
      if (nodeid == LOCAL) 
        nodeid = getnodeid();
      node_ranges(&fmtbuf, &fmtbuf_len, nodeid, index);
    }

    for (--argc, ++argv; argc > 0; --argc, ++argv) {
      qarg = sfh_argv_quote(*argv, "\\'\"");
      if (qarg == 0) {
        terror("mpirun (sfh_argv_quote)");
        return(0);
      }

      /* Oh for STL strings... */

      str_append(&fmtbuf, &fmtbuf_len, qarg);
      str_append(&fmtbuf, &fmtbuf_len, " ");
      free(qarg);
    }

    ao_tail(ad, &argtailc, &argtailv);

    if (argtailc > 0) 
      str_append(&fmtbuf, &fmtbuf_len, "-- ");

    for ( ; argtailc > 0; --argtailc, ++argtailv) {
      qarg = sfh_argv_quote(*argtailv, "\\'\"");
      if (qarg == 0) {
        terror("mpirun (sfh_argv_quote)");
        return(0);
      }

      str_append(&fmtbuf, &fmtbuf_len, qarg);
      str_append(&fmtbuf, &fmtbuf_len, " ");
      free(qarg);
    }
/*
 * Parse the command line.
 */
    app = asc_bufparse(fmtbuf, strlen(fmtbuf), &lineno, env);
    lineno = 0;
  }
  free(fmtbuf);
  fmtbuf = 0;
  fmtbuf_len = 0;

  if (app == 0) {
    if (lineno > 0) {
      fprintf(stderr, syntaxerr, lineno);
      errno = EUSAGE;
    } else {
      fprintf(stderr, "mpirun: cannot parse: ");
      terror("");
    }

    return(0);
  }
/*
 * Set working dir.
 */
  if (wrkdir) {
    if (asc_workdir(app, wrkdir)) {
      terror("mpirun (asc_workdir)");
      return(0);
    }
  }
/*
 * Schedule the application.
 */
  app_sched = asc_schedule(app);
  asc_free(app);

  if (app_sched == 0) {
    terror("mpirun (asc_schedule)");
    return(0);
  } else {
    return(app_sched);
  }
}

/*
 *  pwait
 *
 *  Function: - waits for processes to complete
 *  Accepts:  - # of processes
 *      - return status of first child in error, else 0
 *  Returns:  - 0 or LAMERROR
 */
int
pwait(int4 *nwait, int *childstat)
{
  int i, j;
  int pid;
  int status;
  int4 nodeid;
  int first_failure = TRUE;     
  int first_failure_status = 0;
  int killed_by_signal = FALSE;
  char spid[32], snode[48], sstatus[32];
  struct ProcessStatusStruct *procStatStruct;

  /* Malloc the space for above described array */

  if ((procStatStruct = (struct ProcessStatusStruct *) 
       malloc((*nwait) * sizeof(struct ProcessStatusStruct))) == NULL)
    return LAMERROR;

  *childstat = 0;
  if (fl_very_verbose)
    printf("mpirun: waiting for MPI_FINALIZE from %d processes\n", 
           *nwait);
  
  /* Loop over all the processes, waiting for them to finish */

  for (i = 0; *nwait > 0; ++i, --(*nwait)) {
    /* Indicate "ok to checkpoint me" here */
          if ((0 == i) 
              && (lam_ssi_crlam.lscrla_enable_checkpoint != NULL))
            lam_ssi_crlam.lscrla_enable_checkpoint();
              
    if (rpwait(&nodeid, &pid, &status)) {
      *childstat = errno;
      terror("mpirun (rpwait)");
      return(LAMERROR);
    }
    /* JMS: Insert: "do not checkpoint me" here (first time
       through) */
    
    /* Figure out how the process ended */
    
    killed_by_signal = FALSE;
    if (LAM_WIFSIGNALED(status)) {
      killed_by_signal = TRUE;
      status = LAM_WTERMSIG(status);  
    } else {  
      status = LAM_WEXITSTATUS(status); 
      
      /* check to see if they did not call MPI_Finalize */
      
      if (LAM_WIFNODETACH(status)) {
        snprintf(spid, 32, "%d", pid);
        node_id_str(nodeid, snode, 48);
        snprintf(sstatus, 32, "%d", status);
        show_help("mpirun", "no-finalize", spid, snode, 
                  sstatus, NULL);
      } else 
        if (fl_very_verbose)
          printf("mpirun: got MPI_FINALIZE from n%d/PID %d\n", 
                 nodeid, pid);
    }

    /* Record it */

    procStatStruct[i].nodeid = nodeid;
    procStatStruct[i].pid = pid;
    procStatStruct[i].killed_by_signal = killed_by_signal;
    procStatStruct[i].status = status;
    
    /* If status is nonzero, the process failed */
    
    if (status != 0) { 
      
      /* case where neither "-sa" nor "-sf" option
         specified on command line, hence mpirun should
         return exit status of the first process that
         failed */ 

      if ((!fl_show_all_status) && (!fl_status_if_fail)) {
        snprintf(spid, 32, "%d", pid);
        node_id_str(nodeid, snode, 48);
        snprintf(sstatus, 32, "%d", status);
        show_help("mpirun", "abnormal-exit", spid, snode, 
                  (killed_by_signal) ? 
                  "due to signal" : "with exit status",
                  sstatus, NULL);
        *childstat = status;
        errno = EINTR;
        return(LAMERROR);
      }

      /* If this was the first failure, print out all the
         previous successful process terminations, and then kill
         all the other processes */
      
      if (first_failure) {
        first_failure_status = status;
        first_failure = FALSE;

        for (j = 0; j < i; ++j)
          show_status(procStatStruct[i].nodeid, 
                      procStatStruct[i].pid, 0, 0);

        errno = EINTR;
        app_doom(world_n, mpiworld, SIGUDIE);
        app_doom(world_n, world, -15);
#if LAM_WANT_IMPI
        app_doom(world_n, impi_server_gps, -15);
#endif
      }
    }

    /* Do we want to print anything out?  (note that if this is
       the first one, we've already printed out the status) */
    
    if (fl_show_all_status ||
        (fl_status_if_fail && !first_failure))
      show_status(pid, nodeid, killed_by_signal, status);
  }

  /* All done */

  if (!first_failure) {
    error_cleanup(world_n);
    *childstat = first_failure_status;
  }
  free (procStatStruct);
  return(0);
}


/*
 *  set_mode
 *
 *  Function: - set the operating mode
 */
static int
set_mode(void)
{
  int want_lamd = 0, want_c2c = 0;

  /* flags */

  fl_very_verbose = ao_taken(ad, "vv");
  fl_fdesc = ao_taken(ad, "f");
  fl_verbose = ao_taken(ad, "v");
  if (fl_very_verbose == 1)
    fl_verbose = 1;
  if (ao_taken(ad, "b"))
    lam_ssi_base_param_set_string("boot_rsh_fast", "1");
  if (ao_taken(ad, "n"))
    fl_wait = 1;
  if (ao_taken(ad, "nw"))
    fl_wait = 0;
  if (ao_taken(ad, "sigs"))
    fl_catch = 1;
  if (ao_taken(ad, "l")) {
    setvbuf(stdout, 0, _IOLBF, 0);
    setvbuf(stderr, 0, _IOLBF, 0);
  }
  if (ao_taken(ad, "lamd")) {
    fprintf(stderr, "mpirun: Warning: the \"-lamd\" option is deprecated.\n");
    fprintf(stderr, "mpirun: Use \"-ssi rpi lamd\" instead.\n");
    want_lamd = 1;
  }
  if (ao_taken(ad, "c2c")) {
    fprintf(stderr, "mpirun: Warning: the \"-c2c\" option is deprecated.\n");
    fprintf(stderr, "mpirun: Use \"-ssi rpi <name>\" instead, where <name> is a valid RPI name.\n");
    fprintf(stderr, "mpirun: Assuming \"-ssi rpi tcp\".\n");
    want_c2c = 1;
  }

  /* followed options */

  nprocs = -1;
  if (ao_taken(ad, "c")) {
    ao_intparam(ad, "c", 0, 0, &nprocs);
    fl_nprocs = 1;
  } else if (ao_taken(ad, "np")) {
    ao_intparam(ad, "np", 0, 0, &nprocs);
    fl_nprocs = 1;
  }
  
  if (ao_taken(ad, "wd"))
      wrkdir = ao_param(ad, "wd", 0, 0);
  if (ao_taken(ad, "p"))
    prefix_str = ao_param(ad, "p", 0, 0);

  if (want_lamd)
    lam_ssi_base_ao_process_arg("rpi", "lamd");
  if (want_c2c)
    lam_ssi_base_ao_process_arg("rpi", "tcp");
  lam_ssi_base_ao_process_args(ad);

  /* runtime flags */

  rtf = RTF_MPIRUN;
  
  if (fl_wait)
    rtf |= RTF_WAIT;
  if (fl_catch)
    rtf |= RTF_MPISIGS;
  if (ao_taken(ad, "D"))
    rtf |= RTF_APPWD;
  if (ao_taken(ad, "O"))
    rtf |= RTF_HOMOG;
#if 0
  if (!ao_taken(ad, "lamd"))
    rtf |= RTF_MPIC2C;
#endif
#if 0
  if (ao_taken(ad, "ger") ||
      (ao_taken(ad, "lamd") && !ao_taken(ad, "nger")))
    rtf |= RTF_MPIGER;
#endif

  /* Note that there are two levels of tracing:

     Enabled/disabled: Whether *any* tracing support is enabled.
        "Enabled" means that all communicator and datatype stuff will
        be traced, and message stuff will be traced depending on
        whether it is "on" or off" (see below).  "Disabled" means that
        nothing will be saved, regardless of whether it is "on" or
        "off".

     On/Off: Only controls whether message stuff will be saved, and is
        only relevant if tracing is enabled.  

     Hence, if tracing is enabled *and* on, then everything will be
     traced.  If it is only enabled, then only datatype/communicator
     stuff is traced, and message traffic is *not* traced.

     The presence of -t, -ton, or -toff enables tracing.  -t and -ton
     initially turn message tracing on.  -toff initially turns message
     tracing off. */

  if (ao_taken(ad, "t") || ao_taken(ad, "ton") || ao_taken(ad, "toff"))
    rtf |= RTF_TRACE;

  /* This is a little redundant, but it makes the logic crystal
     clear */

  if (ao_taken(ad, "t") || ao_taken(ad, "ton"))
    rtf |= RTF_TRSWITCH;
  else if (ao_taken(ad, "toff"))
    rtf &= ~RTF_TRSWITCH;

  if (isatty(1))
    rtf |= RTF_TTYOUT;
  if (!fl_fdesc)
    rtf |= RTF_IO;
#if LAM_HAVE_PTYS
#if PTY_IS_DEFAULT
  if (!ao_taken(ad, "npty"))
    rtf |= RTF_PTYS;
#else
  if (ao_taken(ad, "pty"))
    rtf |= RTF_PTYS;
#endif
#else

  /* For those systems that do not have pty support */
  
  if (ao_taken(ad, "pty"))
    fprintf(stderr, "Your system does not appear to include pty support."
      "-pty ignored.\n");
  rtf &= ~RTF_PTYS;
#endif
  
  fl_impi_client = ao_taken(ad, "client");
  fl_impi_server = ao_taken(ad, "server");
  if (fl_impi_client || fl_impi_server) {
#if !LAM_WANT_IMPI
    show_help("mpirun", "no-impi-support", NULL);
#else
#if !LAM_WANT_IMPI_SERVER
    if (fl_impi_server)
      show_help("mpirun", "no-impi-server", NULL);
#else
    if (fl_impi_server && !fl_impi_client) {
      show_help("mpirun", "server-without-client", NULL);
      return LAMERROR;
    }
#endif
    rtf |= RTF_IMPI;
    
    /* This seems to be overkill, but we'll leave it here on the
       off chance that we had a really good reason to have it
       here! */
    
    if (ao_taken(ad, "O")) {
      fprintf(stderr, "-O conflicts with -client (IMPI jobs cannot be "
        "guaranteed to be homogeneous)\n");
      fprintf(stderr, "-O disabled.\n");
      rtf &= ~RTF_HOMOG;
    }
#endif
  }

  return(0);
}

/*
 *  set_stdio
 *
 *  Function: - set up application stdio
 *  Returns:  - 0 or LAMERROR
 */
static int
set_stdio(void)
{
  char    server[LAM_PATH_MAX]; /* fd server socket name */

  if (fl_fdesc) return(0);

/*
 * Pass stdin, stdout and stderr to filed.
 */
  if (lam_mktmpid((int) lam_getpid(), server, sizeof(server))) {
    return(LAMERROR);
  }

  if (lam_lfopenfd(server)) {
    return(LAMERROR);
  }
/*
 * Set LAM file descriptors to the passed file descriptors.  The call to
 * lam_lfopenfd() takes care of the case when stdin is a tty.
 */
  _kio.ki_stdin = _ufd[0].fu_tfd;
  _kio.ki_stdout = _ufd[1].fu_tfd;
  _kio.ki_stderr = _ufd[2].fu_tfd;

  return(0);
}

/*
 *  reset_stdio
 *
 *  Function: - reset stdio so rfatexit will clean it up
 *  Returns:  - 0 or LAMERROR
 */
static void
reset_stdio(void)
{
  _kio.ki_stdin = 0;
  _kio.ki_stdout = 1;
  _kio.ki_stderr = 2;
}

/*
 *  locate_aschema
 *
 *  Function: - locate an application schema
 *  Accepts:  - filename
 *  Returns:  - full pathname or NULL
 */
static char *
locate_aschema(char *filename)
{
  int   pathc = 0;    /* paths argc */
  char    **pathv = 0;    /* paths argv */
  char    *appdir;    /* application dir */
  char    *fullpath;    /* full pathname */

  if ((appdir = getenv("LAMAPPLDIR"))) {
    sfh_argv_add(&pathc, &pathv, appdir);
  }
  sfh_argv_add(&pathc, &pathv, "");
  sfh_argv_add(&pathc, &pathv, "$LAMHOME/etc");
  sfh_argv_add(&pathc, &pathv, "$TROLLIUSHOME/etc");
  sfh_argv_add(&pathc, &pathv, LAM_SYSCONFDIR);

  fullpath = sfh_path_find(filename, pathv, R_OK);
  sfh_argv_free(pathv);
  if (fl_very_verbose) {
    if (fullpath != NULL)
      printf("mpirun: found app schema: %s\n", fullpath);
    else
      printf("mpirun: could not find app schema!\n");
  }
  return(fullpath);
}

/*
 *  node_ranges
 *
 *  Function: - fill buffer with list of nodes
 *      - compact nodes to ranges
 *      - formatting buffer assumed large enough
 *  Accepts:  - formatting buffer
 *      - initial node
 *      - initial index
 */
static void
node_ranges(char **buf, int *len, int4 initnode, int4 initindex)
{
  int4    node;     /* target node */
  int4    index;      /* for getnode() */
  int4    flags;      /* for getnode() */
  int4    next;     /* next expected node */
  int   fl_flush = 0;   /* flush node flag */

  sprintf(smallbuf, "n%d", initnode);
  str_append(buf, len, smallbuf);
/*
 * Loop over the nodes.
 */
  next = initnode + 1;

  nid_get(&index, &node, &flags);

  while (index != initindex) {
    if (node != next) {
      if (next > initnode + 1) {
/*
 * We have a "hole" ending a range of length >= 2.
 */
        sprintf(smallbuf, "-%d,%d", next - 1, node);
        str_append(buf, len, smallbuf);
      }
      else {
/*
 * We have a "hole" ending a range of length 1.
 */
        sprintf(smallbuf, ",%d", node);
        str_append(buf, len, smallbuf);
      }
      initnode = node;
      next = node + 1;
      fl_flush = 0;
    }
/*
 * Otherwise, keep incrementing.
 */
    else {
      ++next;
      fl_flush = 1;
    }

    nid_get(&index, &node, &flags);
  }
/*
 * If we were still in a range, flush it out.
 */
  if (fl_flush) {
    sprintf(smallbuf, "-%d", next - 1);
    str_append(buf, len, smallbuf);
  }

  str_append(buf, len, " ");
}

/*
 *  signal_handler
 *
 *  Function: - dooms application started by mpirun
 *  Accepts:  - signal
 */
static void
signal_handler(int sig)
{
  static int  numsigs = 0;    /* # times signal caught */
  int   pid;
  static char  error_message_1[] = {"Exiting mpirun ...\n"};
  static char  error_message_2[] = {"mpirun: caught SIGINT\n"};
  static char  error_message_3[] = {"mpirun: Killing MPI application...\n"};
  static char  error_message_4[] = {"mpirun: MPI application is dead\n"};

  if (sig != SIGINT && sig != SIGTERM) 
    return;
/*
 * Second time around just exit.
 */
  if (numsigs++ > 0) {
    if (fl_very_verbose)
      write (1, error_message_1, sizeof(error_message_1));
    lam_ssi_base_close();
    exit(1);
  }

  if (fl_very_verbose)
    write (1, error_message_2, sizeof(error_message_2));
  if ((pid = fork()) < 0) {
    return;
  }
  else if (pid == 0) {
    lam_reset_pid_cache();
    if (kinit(PRCMD)) {
      lam_ssi_base_close();
      exit(errno);
    }

    if (fl_very_verbose)
      write (1, error_message_3, sizeof(error_message_3));
    app_doom(world_n, world, -15);
    if (fl_very_verbose)
      write (1, error_message_4, sizeof(error_message_4));
    lam_ssi_base_close();
    kexit(0);
  }
}

/*
 *  get_mpi_world
 *
 *  Function: - get MPI world
 *  Accepts:  - size of world
 *      - initial process world
 *      - MPI process world (out)
 *  Returns:  - full pathname or NULL
 */
static int
get_mpi_world(int4 world_n, struct _gps *world, struct _gps *mpiworld)
{
  int i, j;
  struct nmsg msg;
  struct mpi_app_proc_info *mapi;
    
  memcpy(mpiworld, world, world_n * sizeof(struct _gps));
  for (i = 0; i < world_n; i++)
    mpiworld[i].gps_pid = 0;
  
  LAM_ZERO_ME(msg);
  msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  msg.nh_flags = DINT4DATA;
  msg.nh_length = 0;
  mapi = (struct mpi_app_proc_info *) &msg.nh_data;

  if (fl_very_verbose)
    printf("mpirun: waiting for MPI_INIT from %d processes...\n", world_n);
  for (i = 0; i < world_n; i++) {
    msg.nh_type = 3;
    if (nrecv(&msg)) {
      terror("mpirun");
      return LAMERROR;
    }

    if (msg.nh_type == 1) {
      char node[32];
      if (fl_very_verbose)
        printf("mpirun: someone died before MPI_INIT -- rank %d\n", 
               msg.nh_node);
      snprintf(node, sizeof(node), "%d", msg.nh_node);
      show_help("mpirun", "no-init", node, NULL);
      errno = EMPINOINIT;
      return LAMERROR;
    }

    /* Set the MPI process pid and index. */
    
    j = mapi->mapi_mcw_rank;
    if (j < 0 || j >= world_n) {
      if (fl_very_verbose)
        printf("mpirun: got invalid MPI_COMM_WORLD rank number in MPI_INIT "
               "return message\n");
      errno = EIMPOSSIBLE;
      terror("mpirun");
      return LAMERROR;
    }

    mpiworld[j].gps_pid = mapi->mapi_pid;
    mpiworld[j].gps_idx = mapi->mapi_ps_index;

    if (fl_very_verbose)
      printf("mpirun: got successful MPI_INIT from rank %d\n", j);
  }

  return 0;
}


/*
 *  match_ssi_modules
 *
 *  Function: - match the RPI modules sent by the MPI procs and
 *                    also the CRLAM module selected by me with the 
 *                    CRMPI modules sent by the MPI procs. 
 *  Accepts:  - size of world
 *      - MPI process world
 *      - flag indicating if crlam module is available
 *  Returns:  - 0 or LAMERROR
 */
static int
match_ssi_modules(int4 world_n, struct _gps *mpiworld)
{
  int i, max;
  struct nmsg msg;
  struct mpi_app_extra_info save, maei;
  int saved_mcw_rank;
  char arg1[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2],
       arg2[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2],
       arg3[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2],
       arg4[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2];
    
  LAM_ZERO_ME(msg);
  msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  msg.nh_flags = DINT4DATA;
  msg.nh_length = sizeof(maei);
  msg.nh_msg = (char *) &maei;

  if (fl_very_verbose)
    printf("mpirun: waiting for selected SSI modules from %d processes...\n",
           world_n);
  for (i = 0; i < world_n; ++i) {
    msg.nh_type = 3;
    if (nrecv(&msg)) {
      terror("mpirun");
      return LAMERROR;
    }

    if (msg.nh_type == 1) {
      char node[32];
      if (fl_very_verbose)
        printf("mpirun: someone died before MPI_INIT -- rank %d\n", 
               msg.nh_node);
      snprintf(node, sizeof(node), "%d", msg.nh_node);
      show_help("mpirun", "no-init", node, NULL);
      return LAMERROR;
    }
    
    /*
     * Check to ensure that the RPI and CRMPI modules sent by all the 
     * processes match.
     */
    max = (LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2) - 1;

    if (0 == i) {
      save = maei;
      saved_mcw_rank = msg.nh_data[0];

    } else if (strncmp(save.maei_rpi.masi_name, maei.maei_rpi.masi_name,
                       sizeof(save.maei_rpi.masi_name)) != 0 ||
               ttol(save.maei_rpi.masi_major_ver) !=
                 ttol(maei.maei_rpi.masi_major_ver) ||
               ttol(save.maei_rpi.masi_minor_ver) !=
                 ttol(maei.maei_rpi.masi_minor_ver) ||
               ttol(save.maei_rpi.masi_release_ver) !=
                 ttol(maei.maei_rpi.masi_release_ver)) {

      /* Take pains to print a friendly error message */
      snprintf(arg1, max, "%d", saved_mcw_rank);
      snprintf(arg2, max, "%s (v%d.%d.%d)", save.maei_rpi.masi_name,
               ttol(save.maei_rpi.masi_major_ver),
               ttol(save.maei_rpi.masi_minor_ver),
               ttol(save.maei_rpi.masi_release_ver));
      snprintf(arg3, max, "%d", msg.nh_data[0]);
      snprintf(arg4, max, "%s (v%d.%d.%d)", maei.maei_rpi.masi_name,
               ttol(maei.maei_rpi.masi_major_ver),
               ttol(maei.maei_rpi.masi_minor_ver),
               ttol(maei.maei_rpi.masi_release_ver));
      show_help("mpi-init", "rpi-mismatch", arg1, arg2, arg3, arg4, NULL);
      return LAMERROR;

    } else if (strncmp(save.maei_crmpi.masi_name, maei.maei_crmpi.masi_name,
                       sizeof(save.maei_crmpi.masi_name)) != 0 ||
               ttol(save.maei_crmpi.masi_major_ver) !=
                 ttol(maei.maei_crmpi.masi_major_ver) ||
               ttol(save.maei_crmpi.masi_minor_ver) !=
                 ttol(maei.maei_crmpi.masi_minor_ver) ||
               ttol(save.maei_crmpi.masi_release_ver) !=
                 ttol(maei.maei_crmpi.masi_release_ver)) {

      /* Take pains to print a friendly error message */
      snprintf(arg1, max, "%d", saved_mcw_rank);
      snprintf(arg2, max, "%s (v%d.%d.%d)", save.maei_crmpi.masi_name,
               ttol(save.maei_crmpi.masi_major_ver),
               ttol(save.maei_crmpi.masi_minor_ver),
               ttol(save.maei_crmpi.masi_release_ver));
      snprintf(arg3, max, "%d", msg.nh_data[0]);
      snprintf(arg4, max, "%s (v%d.%d.%d)", maei.maei_crmpi.masi_name,
               ttol(maei.maei_crmpi.masi_major_ver),
               ttol(maei.maei_crmpi.masi_minor_ver),
               ttol(maei.maei_crmpi.masi_release_ver));
      show_help("mpi-init", "rpi-mismatch", arg1, arg2, arg3, arg4, NULL);
      return LAMERROR;

    }

  }

  /* 
   * save the name of the CRMPI module.  If this is eventually
   * selected by all the processes, it will be used to open the same 
   * module in the CRLAM SSI kind.
   */
  cr_module = strdup(save.maei_crmpi.masi_name); 

  return 0;
}


/*
 *  error_cleanup
 *
 *  Function: - try to clean up init and wait messages
 *      - this is not foolproof but better than nothing
 *  Accepts:  - # of procs started (-1 means unknown)
 */
static void
error_cleanup(int np)
{
  struct nmsg   msg;

  LAM_ZERO_ME(msg);
  msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  msg.nh_type = 3;
  msg.nh_flags = 0;
  msg.nh_length = 0;
  msg.nh_msg = 0;

  if (np < 0) {
  /*
   * Wait a while and then receive any pending init/wait messages.
   */
    sleep(3);
    while (!ntry_recv(&msg)) {
      msg.nh_type = 3;
    }
  } else {
    if (fl_very_verbose)
      printf("mpirun: receiving %d useless MPI_INIT/MPI_FINALIZE messages...\n",
             np - 1);
    for (--np; np > 0; --np) {
      if (nrecv(&msg))
        break;
      msg.nh_type = 3;
    }
  }
}


/*
 * Safely append to a string.  Of for STL strings...
 */
static void
str_append(char **src, int *len, char *suffix)
{
  int slen1;
  int slen2;

  slen1 = strlen(*src);
  slen2 = strlen(suffix);

  if (slen1 + slen2 > *len) {
    while (slen1 + slen2 > *len)
      *len *= 2;
    *src = realloc(*src, *len);
  }

  strcat(*src, suffix);
}


#if LAM_WANT_IMPI
static int
send_impi_params(struct _gps *world)
{
  char               *rank_string;
  char               *server_string;
  struct nmsg        msg;
  int                size;
  char               *msg_string;

  /* 
   * Get the command line parameters for IMPI client rank and 
   * the servers IP name/number and port
   */
  
  server_string = ao_param(ad, "client", 0, 1);
  if (server_string == NULL) {
    errno = EINVAL;
    return (LAMERROR);
  }
  rank_string = ao_param(ad, "client", 0, 0);
  if (rank_string == NULL) {
    errno = EINVAL;
    return (LAMERROR);
  }

  size = strlen(server_string) + strlen(rank_string) + 4;
  msg_string = malloc(size);
  sprintf(msg_string, "%s\n%s\n", server_string, rank_string);

  /* Send it to rank 0 */

  LAM_ZERO_ME(msg);
  msg.nh_node = world[0].gps_node;
  msg.nh_event = (-world[0].gps_pid) & 0xBFFFFFFF;
  msg.nh_type = 4;
  msg.nh_length = size;
  msg.nh_flags = 0;
  msg.nh_msg = (char *) msg_string;

  nsend(&msg);

  free((char *) msg_string);

  return 0;
}
#endif

static void
show_status(int pid, int nodeid, int killed_by_signal, int status)
{
  printf("%s n%d %d %d %d\n", prefix_str, nodeid, pid, 
         killed_by_signal, status);
}


static
void
node_id_str(int nodeid, char* snode, int snode_len)
{
  struct dolink *do_links;
  int n_do_links;

  /* Try to be nice and get the node's IP addr */
  if (ldogetlinks(&do_links, &n_do_links)) {
    /* Looks like we can't get the IP.  Just print number */
    snprintf(snode, snode_len, "n%d", nodeid);
  } else {
    if (nodeid < n_do_links) {
      /* we should have an IP number */
      snprintf(snode, snode_len, "n%d (%s)", nodeid,
               inet_ntoa(do_links[nodeid].dol_addr.sin_addr));
    } else {
      /* Something happened such that I don't trust the
   status of do_links.  I'm not going to try to
   use it. */
      snprintf(snode, snode_len, "n%d", nodeid);
    }

    free(do_links);
  }
}


static void 
first_signal_handler(int sig) {
  static int numsig = 0;
  static char error_message[] = "\
********************* WARNING ***********************\n\
This is a vulnerable region. Exiting the application\n\
now may lead to improper cleanup of temporary objects\n\
To exit the application, press Ctrl-C again\n\
********************* WARNING ************************\n";

  if (sig != SIGINT && sig != SIGTERM) {
    return;
  }
  /* He really wants to quit, call the generic signal handler and
   * Note: signal_handler() does not return
   */
  if (numsig++ > 0) { 
    signal_handler (sig);
  }
  /* This is the first time we have caught the signal */
  else { 
    write (1, error_message, sizeof (error_message));
    return;
  }
}
