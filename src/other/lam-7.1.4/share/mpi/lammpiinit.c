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
 * $Id: lammpiinit.c,v 6.42 2004/03/09 18:01:12 adharurk Exp $
 *
 *	Function:	- initialize the MPI session
 */

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>

#include <all_list.h>
#include <app_mgmt.h>
#include <etc_misc.h>
#include <blktype.h>
#include <laminternal.h>
#include <lam_network.h>
#include <mpi.h>
#include <mpi_apps.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <net.h>
#include <preq.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-cr.h>
#include <lammpithreads.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <malloc_wrapper.h>


/*
 * private variables
 */
static char *name;		/* program name */
static int host;		/* host rank attribute value */
static int io;			/* I/O rank attribute value */
static int wtimeglob;		/* global time attribute */
static int mpi_nprocs;		/* # world processes */
static int mpi_nparent;		/* # parent processes */
static int mpi_cid;		/* CID of parent intercomm */
static struct _gps *mpi_procs;	/* array world & parent GPS */
#if !LAM_WANT_IMPI
/* When The 6.3 and 6.4 distributions merge (i.e., support for IMPI
   becomes part of the default distribution), these variables can go
   away, because they'll be in libimpi.la. */
static int impi_client_size = 1;
static int impi_client_color = 0;
static int impi_host_size = 1;
static int impi_host_color = 0;
#endif


/*
 * external variables
 */

#if LAM_SIZEOF_LONG_DOUBLE == 16
extern LAM_LONG_DOUBLE mpi_bottom_[];
extern LAM_LONG_DOUBLE mpi_argv_null_[];
extern LAM_LONG_DOUBLE mpi_argvs_null_[];
extern LAM_LONG_DOUBLE mpi_errcodes_ignore_[];
extern LAM_LONG_DOUBLE mpi_status_ignore_[];
extern LAM_LONG_DOUBLE mpi_statuses_ignore_[];
#else
extern double mpi_bottom_[];
extern double mpi_argv_null_[];
extern double mpi_argvs_null_[];
extern double mpi_errcodes_ignore_[];
extern double mpi_status_ignore_[];
extern double mpi_statuses_ignore_[];
#endif


/*
 * private functions
 */
static int init_comm(void);
static int init_env(void);
static int init_fcomplex_types(void);
static int init_cxxcomplex_types(void);
static int init_rdtype(void);
static int init_f77(void);
static void init_dtype(void);
static int init_cr(void);
static void init_errhdl(void);
static void init_op(void);
static void make_dtype(MPI_Datatype dtype, int size, int align, int label,
		       char *name);
static void make_op(MPI_Op op, void (*func) ());
static int comm_cmp(MPI_Comm * a, MPI_Comm * b);
static int check_cid(MPI_Comm * a, MPI_Comm * b);
static void fail_stop(const char *program, const char *topic);


/*
 * global functions
 */
int lam_send_selected_ssi_modules(void);


/*
 * local structures for maxloc/minloc operations
 */
struct flt_int
{
  float fi_float;
  int fi_int;
};

struct dbl_int
{
  double di_double;
  int di_int;
};

struct longdbl_int
{
#if LAM_SIZEOF_LONG_DOUBLE
  LAM_LONG_DOUBLE ldi_double;
#else
  double ldi_double;
#endif
  int ldi_int;
};

struct long_int
{
  long li_long;
  int li_int;
};

struct short_int
{
  short li_short;
  int li_int;
};


/*
 * String constant that will show up in a "strings" output of a user
 * program
 */
static const char lam_mpi_version[] =
  "LAM/MPI version " LAM_VERSION " compiled for " LAM_ARCH;

/*
 * Variables visible to the totalview debugger
 *
 * String constant containing the path of the shared library for LAM's 
 * interface to TotalView debugger.  It will end up in user process 
 * image so that the debugger knows where the library is.
 */
#if LAM_HAVE_TV_QUEUE_DLL
/* This should only be here if we built/installed the TV queue
   debugging DLL.  If we didn't, then don't put this symbol in at
   all. */
const char MPIR_dll_name[] = LAM_TOTALVIEW_DLL;
#endif
int MPIR_being_debugged = 0;
int lam_tv_ignore_queue_debugging = 0;
int lam_tv_comm_seqnum = 0;


/*
 * Create the whole MPI universe.  This is the back-end function for
 * both MPI_Init and MPI_Init_thread.  It is still separate from
 * lam_linit (share/mpi/laminit.c) only for hysterical rasins -- they
 * could be combined, but both of these files are already way too
 * long.  Hence, they were left as two separate files and this
 * function still invokes lam_linit() to initialize the communication
 * systems.
 */
int
lam_mpi_init(int *pargc, char ***pargv, int requested, int *pprovided,
	     int blk)
{
  int fl_init;
  int fl_finalized;
  int i, err;
  int root;
  struct _proc **tmp_procs, *p;
  int tmp_nprocs;
  int argc;
  char **argv;

  /* Check if we have been initialized or finalized. */

  MPI_Initialized(&fl_init);
  MPI_Finalized(&fl_finalized);
  if (fl_finalized) {

    /* This is so that we can detect when people are doing silly
       things with trying to re-initizalize MPI (which the standard
       says is a no-no).  Since we are so nice, we are going to
       display a special message to that effect. */

    char *rank = getenv("LAMRANK");
    char host[MAXHOSTNAMELEN];
    char pid[32];

    if (rank == NULL)
      rank = "(unknown)";

    lam_gethostname(host, MAXHOSTNAMELEN);
    LAMSetLastError(EMPIALREADYFINALIZE);
    snprintf(pid, 32, "%d", lam_getpid());
    show_help(NULL, "init-after-finalize", host, pid, rank, NULL);
    exit(LAM_EEXIT);
  } else if (fl_init) {
    return (lam_errfunc(MPI_COMM_WORLD, blk,
			lam_mkerr(MPI_ERR_OTHER, EMPIINIT)));
  }

  /* Load type definitions for TotalView DLL support.  This needs to
     happen before the barrier, and therefore before TV attaches. */
  
  lam_tv_load_type_defs();

  /* Initialize LAM. */

  name = (pargc && pargv && (*pargc > 0)) ? **pargv : NULL;
  if (pargc == NULL || pargv == NULL) {
    argc = 0;
    argv = NULL;
  } else {
    argc = *pargc;
    argv = *pargv;
  }
  if (lam_linit(argc, argv, name, &mpi_nprocs, &mpi_nparent,
		&mpi_cid, &mpi_procs, &root)) {
    if (errno != ENOKERNEL && errno != ENOKDESCRIPTORS)
      show_help("MPI", "bad-mpi-init", NULL);
    exit(errno);
  }

  /* Before we do anything with the RPI, figure out endian-ness of all
     my peers */

  tmp_nprocs = lam_nprocs();
  tmp_procs = malloc(sizeof(struct _proc *) * tmp_nprocs);
  if (tmp_procs == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    fail_stop(NULL, NULL);
  }
  for (i = 0, p = lam_topproc(); i < tmp_nprocs; ++i, p = lam_nextproc())
    tmp_procs[i] = p;
  lam_exchange_endian_info(tmp_procs, tmp_nprocs);
  free(tmp_procs);

  /*
   * make sure we do any memory pinning initialization we might need
   * to do, before we load the ssi modules.  Needs to be here to
   * guarantee the linker doesn't get stupid.
   */
  lam_register_free_handler_init();

  /* Initialize SSI */

  if (lam_ssi_base_open(lam_mpi_ao) == LAMERROR)
    fail_stop("mpi-init", "ssi-fail-open");

  /* Find out what SSI RPI modules are available */
  
  if (lam_ssi_rpi_base_open(lam_mpi_ao) == LAMERROR)
    fail_stop("mpi-init", "ssi-rpi-fail-open");
  if (lam_ssi_rpi_base_query() == LAMERROR)
    fail_stop("mpi-init", "ssi-rpi-fail-query");

  /* Initialize MPI pre-defined "stuff". The order below is important,
     so don't hack without significant care. */

  init_errhdl();
  init_dtype();
  init_op();

  /* Find out which SSI coll modules are available */

  if (lam_ssi_coll_base_open(lam_mpi_ao) == LAMERROR)
    fail_stop("mpi-init", "ssi-coll-fail-open");
  if (lam_ssi_coll_base_query() == LAMERROR)
    fail_stop("mpi-init", "ssi-coll-fail-query");

  /* Find out which SSI crmpi modules are available.  Due to bug 682,
     only do this if we were not MPI_COMM_SPAWN*'ed. */

  if (mpi_nparent == 0) {
    if (lam_ssi_crmpi_base_open(lam_mpi_ao) == LAMERROR)
      fail_stop("mpi-init", "ssi-crmpi-fail-open");
    if (lam_ssi_crmpi_base_query() == LAMERROR)
      fail_stop("mpi-init", "ssi-crmpi-fail-query");
  }

  /* Now that all SSI modules have been opened and we have lists of
     available modules, we can decide on which modules will be
     selected and the final thread level. */

  if (lam_ssi_base_mpi_module_select(requested) == LAMERROR)
    fail_stop("mpi-init", "ssi-select-fail");

  /* Now that the SSI modules have been selected, send the names of the RPI
     and CRMPI modules to mpirun (if we were launched by mpirun) and
     proceed.  If the names of the modules received by mpirun from the
     processes do not match, then mpirun will rpdoom() us.  Otherwise, we
     just go about business as usual.  */

  if (_kio.ki_rtf & RTF_MPIRUN) {
    err = MPI_ERR_OTHER; 
    if (lam_send_selected_ssi_modules() != 0)
      fail_stop("mpi-init", "ssi-module-mismatch");
  }

  /* Now that lam_mpi_thread_level has been determined, setup the MPI
     thread interface (main MPI mutex, thread_main, etc.).  If we need to,
     go reset the updown interface to lock the global MPI mutex since we're
     inside an MPI function. */

  if (pprovided != NULL)
    *pprovided = lam_mpi_thread_level;
  lam_mpi_thread_init();
  lam_updown_reset_thr_level();

  /* Now that we're all done and a specific RPI module has been
     selected, go initialize it */

  if (lam_ssi_rpi_base_init() == LAMERROR)
    fail_stop("mpi-init", "ssi-rpi-init-fail");

  /* If the RPI has totalview queue support, enable it.  If we're
     actually being debugged by totalview, ensure to disable the RPI's
     fast paths. */

  if (RPI_HAS_TOTALVIEW_QUEUE_SUPPORT()) {
    lam_tv_ignore_queue_debugging = 0;
    if (MPIR_being_debugged == 1)
      RPI_DISABLE_FAST();
    ++lam_tv_comm_seqnum;
  } else {
    lam_tv_ignore_queue_debugging = 1;
  }

  /* Finally, if we have one, intialize the selected CR module */

  if (init_cr() == LAMERROR)
    fail_stop("mpi-init", "ssi-crmpi-init-fail");

  /* The coll SSI modules have already been opened and initialized.
     So now initialize CID's and communicators (MPI_COMM_WORLD,
     MPI_COMM_SELF, and MPI_COMM_PARENT). */

  if (lam_init_cid() || init_comm())
    fail_stop("mpi-init", "cid-comm-init-fail");

#if LAM_WANT_IMPI
  /* Check if it is an IMPI job */

  if (_kio.ki_rtf & RTF_IMPI)
    if (IMPI_Init(&mpi_nprocs, &mpi_cid, &mpi_procs)) {
      terror("IMPI_Init: LAM error");
      kexit(errno);
    }
#endif

  if (init_rdtype() || init_fcomplex_types() ||
      init_cxxcomplex_types() || init_env() ||
      lam_init_onesided() || init_f77())
    fail_stop("mpi-init", "dtype-env-one-f77-init-fail");

  lam_ports_init();

  /* Initialize profiling package. */

  MPI_Pcontrol(1);

  /* cleanup */

  free((char *) mpi_procs);

  /* Now that everything is setup, let the coll modules fight over who
     will own MPI_COMM_WORLD, MPI_COMM_SELF, and the parent
     commuicator (if it exists).  The order is important here, because
     they may use communicator attributes and keyvals, and those
     weren't created when we created the communicators. */

#if 0
  /* JMS something along these lines needs to be here -- it has to be
     a subroutine so that we can call it in the spawn, accept,
     connect, and joint functions as well */
  if (lam_ssi_coll_base_xchange_available() != 0)
    return (lam_errfunc(MPI_COMM_NULL,
			blk, lam_mkerr(MPI_ERR_OTHER, errno)));
#endif
  if (lam_ssi_coll_base_init_comm(MPI_COMM_SELF) != 0)
    fail_stop("mpi-init", "mpi-comm-self-init-fail");
  if (lam_ssi_coll_base_init_comm(MPI_COMM_WORLD) != 0)
    fail_stop("mpi-init", "mpi-comm-world-init-fail");

  /* Now that MPI_COMM_WORLD has been finalized (haha), we can put in
     the trace for it, and the parent (if it exists) */

  if (lam_tr_comm(MPI_COMM_WORLD))
    lam_comm_errfatal(0, &err, 0,
                      "Failed to drop MPI_COMM_WORLD trace");
  if (mpi_nparent > 0)
    if (lam_tr_comm(lam_comm_parent))
      lam_comm_errfatal(0, &err, 0,
                        "Failed to drop MPI_COMM_PARENT trace");

  if (_kio.ki_rtf & RTF_MPISIGS) {
    if (lam_mpi_set_sighandlers()) {
      lam_comm_errfatal(0, &err, 0,
                        "Failed to initialize signal handlers");
    }
  }

  /* Make sure everyone else has also initialized.  Rank 0 of a
     spawned world must inform the root parent when this is so.  To
     make this scheme work nicely with debuggers (and short running
     child processes), let's also wait for a zero byte ACK from the
     root of the parent. */

#if LAM_WANT_IMPI
  if (LAM_IS_IMPI(MPI_COMM_WORLD)) {
    err = MPI_Barrier(MPI_COMM_WORLD->c_shadow);
  } else {
    err = MPI_Barrier(MPI_COMM_WORLD);
  }
#else
  err = MPI_Barrier(MPI_COMM_WORLD);
#endif

  if (err != MPI_SUCCESS)
    return err;

  /* Barrier across the parent communicator */

  if ((mpi_nparent > 0) && (MPI_COMM_WORLD->c_group->g_myrank == 0)) {
    err = MPI_Send((void *) 0, 0, MPI_BYTE, root, 0, lam_comm_parent);
    if (err != MPI_SUCCESS)
      return err;
    err = MPI_Recv((void *) 0, 0, MPI_BYTE,
		   root, 0, lam_comm_parent, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS)
      return err;
  }

  /* Once we know that the parent has finished setting up, do the coll
     init */

  if (mpi_nparent > 0) {
    if (lam_ssi_coll_base_init_comm(lam_comm_parent) != 0)
      return (lam_errfunc(lam_comm_parent,
			  blk, lam_mkerr(MPI_ERR_OTHER, errno)));
  }

  /* Record an initialization trace.  If we are tracing and trace
     generation is initially to be on, turn on message tracing (i.e.,
     drop an "on" trace). */

  if (lam_tr_init(name, lam_clockskew))
    return (lam_errfunc(MPI_COMM_WORLD,
			blk, lam_mkerr(MPI_ERR_OTHER, errno)));

  if ((_kio.ki_rtf & RTF_TRON) == RTF_TRON) {
    /* Fake out the lam_tr_on() function by pretending that message
       tracing was off (i.e., lam_tr_on() is smart enough to only turn
       message tracing on if it wasn't already on). */
    _kio.ki_rtf &= ~RTF_TRSWITCH;
    if (lam_tr_on() < 0)
      return (lam_errfunc(MPI_COMM_WORLD,
			  blk, lam_mkerr(MPI_ERR_OTHER, errno)));
  }

  /* Now that MPI_INIT* is effectively done, invoke any callbacks that
     were registered. */

  if (lam_ssi_base_mpi_init_callbacks_invoke() != MPI_SUCCESS)
    return (lam_errfunc(MPI_COMM_WORLD,
			blk, lam_mkerr(MPI_ERR_OTHER, errno)));

  /* All done */

  return (MPI_SUCCESS);
}


int
lam_send_selected_ssi_modules(void)
{
  lam_ssi_module_t *module;
  lam_ssi_rpi_t rpi;
  lam_ssi_crmpi_t crmpi;
  struct nmsg nhead;
  struct mpi_app_extra_info maei;

  if (0 == getenv("LAMRANK"))
    return LAMERROR;

  /*
   * The event is -(mpirun's pid). This will distinguish it from user
   * events. The type is 2 to distinguish it from "wait" messages which
   * the same event and type 1.
   */
  LAM_ZERO_ME(maei);
  LAM_ZERO_ME(nhead);
  nhead.nh_node = _kio.ki_jobid.jid_node;
  nhead.nh_event = (-_kio.ki_jobid.jid_pid) & 0xBFFFFFFF;
  nhead.nh_type = 2;
  nhead.nh_flags = DINT4DATA;

  /* 
   * grank is specific to the communicator.  So, when we move to MPI-2
   * (spawn), this assignment will no longer be correct.  Will need to
   * re-visit this when we get there.
   *
   * But on the other hand, what really matters is that all the processes
   * agree upon the same modules.  So, except for the fact that mpirun will
   * not be able to print the correct identity of this process (when the
   * modules don't match and mpirun prints an error message), this will not
   * impact anything seriously.
   */
  nhead.nh_data[0] = lam_myproc->p_gps.gps_grank;

  nhead.nh_length = sizeof(maei);
  nhead.nh_msg = (char *) &maei;

  /* 
   * At this point, the target RPI module has been selected (although not
   * initialized).  So it's safe to copy the RPI module's name out and send
   * it to mpirun for corroboration.  This will change when we go
   * multi-RPI.
   * 
   */
  module = (lam_ssi_module_t *) al_top(lam_ssi_rpi_base_available);
  rpi = *((lam_ssi_rpi_t *) module->lsm_module);
  memcpy(maei.maei_rpi.masi_name, 
         rpi.lsr_meta_info.ssi_module_name, 
         sizeof(maei.maei_rpi.masi_name)); 
  maei.maei_rpi.masi_major_ver =
    ltot(rpi.lsr_meta_info.ssi_module_major_version);
  maei.maei_rpi.masi_minor_ver =
    ltot(rpi.lsr_meta_info.ssi_module_minor_version);
  maei.maei_rpi.masi_release_ver =
    ltot(rpi.lsr_meta_info.ssi_module_release_version);

  /*
   * Also copy the selected CRMPI module's name to send to mpirun.
   *
   * It is possible that no CR modules were selected. So handle that
   * case.
   *
   * Ensure to only allow checkpointing of non-MPI_COMM_SPAWN*'ed
   * processes.
   */
  if (mpi_nparent == 0) {
    if (lam_ssi_crmpi_base_available != NULL) {
      module = (lam_ssi_module_t *) al_top(lam_ssi_crmpi_base_available);
    }
  } else {
    module = NULL;
  }

  if ((NULL == lam_ssi_crmpi_base_available) || 
      (NULL == module)) {
    strcpy(maei.maei_crmpi.masi_name, "none");
    maei.maei_crmpi.masi_major_ver = -1;
    maei.maei_crmpi.masi_minor_ver = -1;
    maei.maei_crmpi.masi_release_ver = -1;
  } else {
    crmpi = *((lam_ssi_crmpi_t *) module->lsm_module);
    memcpy(maei.maei_crmpi.masi_name,
           crmpi.lscrm_meta_info.ssi_module_name,
           sizeof(maei.maei_crmpi.masi_name));
    maei.maei_crmpi.masi_major_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_major_version);
    maei.maei_crmpi.masi_minor_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_minor_version);
    maei.maei_crmpi.masi_release_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_release_version);
  }

  if (nsend(&nhead))
    return LAMERROR;
 
  return 0;
}


/*
 * This will need to change when we go multi-RPI.  Right now, we just
 * check the one RPI and ensure that it is checkpoint/restart-able.
 * If it is, and if we have a winning cr module, then set it up.
 */
static int
init_cr(void)
{
  int have_cr = 1;
  lam_ssi_module_t *cr_module, *module, *next;
  lam_ssi_crmpi_t *cr;
  lam_ssi_t *coll;
  lam_ssi_coll_1_0_0_t *coll100;
  lam_ssi_coll_1_1_0_t *coll110;
  const lam_ssi_crmpi_actions_t *cra;
  int coll_has_cr;

  /* If we have an empty list, simply return -- there's nothing to
     do */

  cr_module = (lam_ssi_module_t*) al_top(lam_ssi_crmpi_base_available);
  if (cr_module == NULL) {
    lam_ssi_crmpi.lscrma_finalize = NULL;
    lam_ssi_crmpi.lscrma_app_suspend = NULL;
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did,
                " CR support disabled; no CR modules available");
    return 0;
  }

  /* See if the winning RPI is even checkpointable.  If it's not, then
     skip all the CR setup */

  if (!RPI_HAS_CHECKPOINT()) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did,
                " CR support disabled; RPI does not suport checkpoint");
    have_cr = 0;
  }

  /* See if all the available coll modules have checkpoint.  If not,
     then skip all the CR setup */

  else {
    for (module = (lam_ssi_module_t*) al_top(lam_ssi_coll_base_available);
         module != NULL; 
         module = al_next(lam_ssi_coll_base_available, module)) {
      coll = module->lsm_module;
      coll_has_cr = 0;
      if (coll->ssi_kind_major_version == 1 &&
          coll->ssi_kind_minor_version == 0 &&
          coll->ssi_kind_release_version == 0) {
        coll100 = (lam_ssi_coll_1_0_0_t *) coll;
        coll_has_cr = coll100->lsc_has_checkpoint;
      } else if (coll->ssi_kind_major_version == 1 &&
                 coll->ssi_kind_minor_version == 1 &&
                 coll->ssi_kind_release_version == 0) {
        coll110 = (lam_ssi_coll_1_1_0_t *) coll;
        coll_has_cr = coll110->lsc_has_checkpoint;
      }
      if (coll_has_cr == 0) {
        if (lam_ssi_cr_verbose >= 0)
          lam_debug(lam_ssi_cr_did,
                    " CR support disabled; %s coll module does not "
                    "support checkpoint", coll->ssi_module_name);
        have_cr = 0;
        break;
      }
    }
  }

  if (have_cr == 1) {
    
    /* Setup the cr function pointers and base module.  It's not an
       error if there are no cr modules available. */

    lam_ssi_crmpi_base_module = *((lam_ssi_crmpi_t*) cr_module->lsm_module);

    /* Save the resulting actions struct in the global variable.  This
       will need to change when we go multi-RPI. */

    cra = lam_ssi_crmpi_base_module.lscrm_init();
    if (cra == NULL)
      return LAMERROR;

    lam_ssi_crmpi = *cra;
    have_cr = 1;

    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, " CR support enabled (%s)",
                lam_ssi_crmpi_base_module.lscrm_meta_info.ssi_module_name);
  }

  /* Close all the losing modules -- possibly including the selected
     module (if we ended up with have_cr==0) */

  if (have_cr == 0) {
    lam_ssi_crmpi.lscrma_finalize = NULL;
    lam_ssi_crmpi.lscrma_app_suspend = NULL;
    next = cr_module;
  } else {
    next = al_next(lam_ssi_crmpi_base_available, cr_module);
  }
  for (; next != NULL; next = al_next(lam_ssi_crmpi_base_available, next)) {
    cr = (lam_ssi_crmpi_t*) next->lsm_module;
    if (cr->lscrm_meta_info.ssi_close_module != NULL)
      cr->lscrm_meta_info.ssi_close_module();
    lam_ssi_base_module_registry_unuse(next->lsm_module);
  }  
  if (have_cr == 0) {
    al_free(lam_ssi_crmpi_base_available);
    lam_ssi_crmpi_base_available = NULL;
  }

  /* All done */

  return 0;
}


/*
 *	init_errhdl
 *
 *	Function:	- initialize pre-defined error handles
 */
static void
init_errhdl(void)
{
  MPI_ERRORS_ARE_FATAL->eh_func = 0;
  MPI_ERRORS_ARE_FATAL->eh_f77handle = -1;
  MPI_ERRORS_ARE_FATAL->eh_refcount = 1;
  MPI_ERRORS_ARE_FATAL->eh_flags = LAM_EHCOMM | LAM_EHWIN | LAM_EHFILE;

  MPI_ERRORS_RETURN->eh_func = (void (*)()) lam_errreturn;
  MPI_ERRORS_RETURN->eh_f77handle = -1;
  MPI_ERRORS_RETURN->eh_refcount = 1;
  MPI_ERRORS_RETURN->eh_flags = LAM_EHCOMM | LAM_EHWIN | LAM_EHFILE;
}


/*
 *	init_comm
 *
 *	Function:	- initialize pre-defined communicators
 *	Returns:	- 0 or LAMERROR
 */
static int
init_comm(void)
{
  MPI_Group group;		/* a process group */
  MPI_Comm comm;		/* a communicator */
  int i;
  struct _gps *g;
  struct _proc **p;
  int cid;
/*
 * Create the empty group.
 */
  MPI_GROUP_EMPTY->g_nprocs = 0;
  MPI_GROUP_EMPTY->g_myrank = MPI_UNDEFINED;
  MPI_GROUP_EMPTY->g_refcount = 1;
  MPI_GROUP_EMPTY->g_procs = 0;
/*
 * Initialize the list of communicators.
 */
  lam_comms = al_init(sizeof(MPI_Comm), comm_cmp);
  if (lam_comms == 0)
    return (LAMERROR);
/*
 * Create the "world" communicator.
 */
  if (lam_init_comm_world(mpi_procs, mpi_nprocs) != 0)
    return (LAMERROR);
/*
 * Set the pid under which run time traces are stored and write the
 * trace for MPI_COMM_WORLD.
 */
  _kio.ki_tid = lam_myproc->p_gps.gps_pid;
/*
 * Create the "self" communicator.
 */
  comm = MPI_COMM_SELF;

  group = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
			     sizeof(struct _proc *));
  if (group == 0)
    return (LAMERROR);

  group->g_nprocs = 1;
  group->g_myrank = 0;
  group->g_refcount = 1;
  group->g_f77handle = -1;
  group->g_procs = (struct _proc **)
    ((char *) group + sizeof(struct _group));

  group->g_procs[0] = lam_myproc;
  lam_myproc->p_refcount++;

/*
 * Changed MPI_COMM_SELF's context ID to 2 for IMPI
 * Context ID's are now incremented by 2 so that we can have shadow
 * communicators.
 */
  cid = lam_getcid();
  if (lam_comm_new(cid, group, MPI_GROUP_NULL, LAM_PREDEF, &comm)) {
    return (LAMERROR);
  }

  lam_setcid(cid);
  comm->c_errhdl->eh_refcount++;
  strcpy(comm->c_name, "MPI_COMM_SELF");

  if (LAM_TRACE_TOP())
    lam_tr_commname(comm->c_name, comm->c_contextid, MPI_Wtime());

  if (!al_insert(lam_comms, &comm))
    return (LAMERROR);
/*
 * Create the parent intercommunicator if spawned.
 */
  if (mpi_nparent > 0) {

    group = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
			       (mpi_nparent * sizeof(struct _proc *)));
    if (group == 0)
      return (LAMERROR);

    group->g_nprocs = mpi_nparent;
    group->g_myrank = MPI_UNDEFINED;
    group->g_refcount = 1;
    group->g_f77handle = -1;
    group->g_procs = (struct _proc **)
      ((char *) group + sizeof(struct _group));
/*
 * Fill the parent group processes.
 */
    g = mpi_procs + mpi_nprocs;
    p = group->g_procs;

    for (i = 0; i < mpi_nparent; ++i, ++g, ++p) {
      *p = lam_procfind(g);
      if (*p == 0)
	return (LAMERROR);
      (*p)->p_refcount++;
    }

    /* May need to worry about overriding the IMPI min cid
     * here (that was set earlier) -- any IMPI job won't
     * come from a user calling MPI_Spawn, so this
     * couldn't happen. */

    if (lam_comm_new(mpi_cid, MPI_COMM_WORLD->c_group,
		     group, LAM_CINTER, &lam_comm_parent)) {
      return (LAMERROR);
    }

    lam_setcid(mpi_cid);
    MPI_COMM_WORLD->c_group->g_refcount++;
    lam_comm_parent->c_errhdl->eh_refcount++;
    strcpy(lam_comm_parent->c_name, "MPI_COMM_PARENT");

    if (LAM_TRACE_TOP())
      lam_tr_commname(lam_comm_parent->c_name,
		      lam_comm_parent->c_contextid, MPI_Wtime());

    if (!al_insert(lam_comms, &lam_comm_parent))
      return (LAMERROR);
  }

  return (0);
}


/*
 *	init_dtype
 *
 *	Function:	- initialize basic (intrinsic) datatypes
 */
static void
init_dtype(void)
{
/*
 * common datatypes
 */
  make_dtype(MPI_BYTE, sizeof(char), sizeof(char), TRDTBYTE, "MPI_BYTE");
  make_dtype(MPI_PACKED, sizeof(char),
	     sizeof(char), TRDTPACKED, "MPI_PACKED");
  make_dtype(MPI_UB, 0, 1, TRDTUB, "MPI_UB");
  make_dtype(MPI_LB, 0, 1, TRDTLB, "MPI_LB");
/*
 * C datatypes
 */
  make_dtype(MPI_WCHAR, sizeof(wchar_t),
	     sizeof(wchar_t), TRDTWCHAR, "MPI_WCHAR");
  make_dtype(MPI_CHAR, sizeof(char), sizeof(char), TRDTCHAR, "MPI_CHAR");
  make_dtype(MPI_SHORT, sizeof(short),
	     LAM_ALIGNMENT_SHORT, TRDTSHORT, "MPI_SHORT");
  make_dtype(MPI_INT, sizeof(int), LAM_ALIGNMENT_INT, TRDTINT, "MPI_INT");
  make_dtype(MPI_LONG, sizeof(long),
	     LAM_ALIGNMENT_LONG, TRDTLONG, "MPI_LONG");
  make_dtype(MPI_FLOAT, sizeof(float),
	     LAM_ALIGNMENT_FLOAT, TRDTFLOAT, "MPI_FLOAT");
  make_dtype(MPI_DOUBLE, sizeof(double),
	     LAM_ALIGNMENT_DOUBLE, TRDTDOUBLE, "MPI_DOUBLE");

#if LAM_SIZEOF_LONG_DOUBLE
  make_dtype(MPI_LONG_DOUBLE, sizeof(LAM_LONG_DOUBLE),
	     LAM_ALIGNMENT_LONG_DOUBLE, TRDTLONGDOUBLE, "MPI_LONG_DOUBLE");
#else
  make_dtype(MPI_LONG_DOUBLE, sizeof(double),
	     LAM_ALIGNMENT_DOUBLE, TRDTLONGDOUBLE, "MPI_LONG_DOUBLE");
#endif

#if LAM_SIZEOF_LONG_LONG
  make_dtype(MPI_LONG_LONG_INT, sizeof(long long),
	     LAM_ALIGNMENT_LONG_LONG, TRDTLONGLONG, "MPI_LONG_LONG_INT");
  make_dtype(MPI_UNSIGNED_LONG_LONG, sizeof(unsigned long long),
	     LAM_ALIGNMENT_LONG_LONG, TRDTULONGLONG,
	     "MPI_UNSIGNED_LONG_LONG");
#else
  make_dtype(MPI_LONG_LONG_INT, sizeof(long),
	     LAM_ALIGNMENT_LONG, TRDTLONGLONG, "MPI_LONG_LONG_INT");
  make_dtype(MPI_UNSIGNED_LONG_LONG, sizeof(unsigned long),
	     LAM_ALIGNMENT_LONG, TRDTULONGLONG, "MPI_UNSIGNED_LONG_LONG");
#endif
  make_dtype(MPI_UNSIGNED_CHAR, sizeof(unsigned char),
	     sizeof(unsigned char), TRDTUCHAR, "MPI_UNSIGNED_CHAR");
  make_dtype(MPI_UNSIGNED_SHORT, sizeof(unsigned short),
	     LAM_ALIGNMENT_SHORT, TRDTUSHORT, "MPI_UNSIGNED_SHORT");
  make_dtype(MPI_UNSIGNED, sizeof(unsigned int),
	     LAM_ALIGNMENT_INT, TRDTUINT, "MPI_UNSIGNED");
  make_dtype(MPI_UNSIGNED_LONG, sizeof(unsigned long),
	     LAM_ALIGNMENT_LONG, TRDTULONG, "MPI_UNSIGNED_LONG");
/*
 * FORTRAN datatypes
 */
  make_dtype(MPI_F_CHARACTER, sizeof(char),
	     sizeof(char), TRDTFCHARACTER, "MPI_CHARACTER");
  make_dtype(MPI_F_LOGICAL, sizeof(int),
	     LAM_ALIGNMENT_INT, TRDTFLOGICAL, "MPI_LOGICAL");
  make_dtype(MPI_F_INTEGER, sizeof(int),
	     LAM_ALIGNMENT_INT, TRDTFINTEGER, "MPI_INTEGER");
  make_dtype(MPI_F_INTEGER1, sizeof(LAM_FORTRAN_C_TYPE_INTEGER1),
             LAM_ALIGNMENT_FORTRAN_INTEGER1, TRDTFINTEGER1, "MPI_INTEGER1");
  make_dtype(MPI_F_INTEGER2, sizeof(LAM_FORTRAN_C_TYPE_INTEGER2),
             LAM_ALIGNMENT_FORTRAN_INTEGER2, TRDTFINTEGER2, "MPI_INTEGER2");
  make_dtype(MPI_F_INTEGER4, sizeof(LAM_FORTRAN_C_TYPE_INTEGER4),
             LAM_ALIGNMENT_FORTRAN_INTEGER4, TRDTFINTEGER4, "MPI_INTEGER4");
#if LAM_SIZEOF_FORTRAN_INTEGER8 
  make_dtype(MPI_F_INTEGER8, sizeof(LAM_FORTRAN_C_TYPE_INTEGER8),
             LAM_ALIGNMENT_FORTRAN_INTEGER8, TRDTFINTEGER8, "MPI_INTEGER8");
#else
  make_dtype(MPI_F_INTEGER8, 0,
             LAM_ALIGNMENT_INT, TRDTFINTEGER, "MPI_INTEGER8");
#endif

  make_dtype(MPI_F_REAL, sizeof(float),
             LAM_ALIGNMENT_FLOAT, TRDTFREAL, "MPI_REAL");
  make_dtype(MPI_F_REAL4, sizeof(LAM_FORTRAN_C_TYPE_REAL4),
             LAM_ALIGNMENT_FORTRAN_REAL4, TRDTFREAL4, "MPI_REAL4");
  make_dtype(MPI_F_REAL8, sizeof(LAM_FORTRAN_C_TYPE_REAL8),
             LAM_ALIGNMENT_FORTRAN_REAL8, TRDTFREAL8, "MPI_REAL8");
#if LAM_SIZEOF_FORTRAN_REAL16
  make_dtype(MPI_F_REAL16, sizeof(LAM_FORTRAN_C_TYPE_REAL16), 
             LAM_ALIGNMENT_FORTRAN_REAL16, TRDTFREAL16, "MPI_REAL16"); 
#else
  make_dtype(MPI_F_REAL16, 0, 
             LAM_ALIGNMENT_INT, TRDTFINTEGER, "MPI_REAL16");
#endif

  make_dtype(MPI_F_DOUBLE_PRECISION, sizeof(double),
	     LAM_ALIGNMENT_DOUBLE, TRDTFDBLPREC, "MPI_DOUBLE_PRECISION");

/*
 * C++ Primitive datatypes
 */
  make_dtype(MPI_CXX_BOOL, LAM_SIZEOF_CXX_BOOL,
	     LAM_ALIGNMENT_CXX_BOOL, TRDTCXXBOOL, "MPI_BOOL");
}


/*
 *	init_fcomplex_types
 *
 *	Function:	- initialize the FORTRAN complex datatypes
 *	Returns:	- 0 or LAMERROR
 */
static int
init_fcomplex_types(void)
{
  MPI_Datatype newdtype;
  int err;
/*
 * Create MPI_COMPLEX.
 */
  err = MPI_Type_contiguous(2, MPI_F_REAL, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_F_COMPLEX, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_F_COMPLEX->dt_label = TRDTFCOMPLEX;
  MPI_F_COMPLEX->dt_commit = 1;
  MPI_F_COMPLEX->dt_flags |= LAM_PREDEF;
  MPI_F_COMPLEX->dt_nelem = 1;
  strcpy(MPI_F_COMPLEX->dt_name, "MPI_COMPLEX");
/*
 * Create MPI_DOUBLE_COMPLEX.
 */
  err = MPI_Type_contiguous(2, MPI_F_DOUBLE_PRECISION, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_F_DOUBLE_COMPLEX, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_F_DOUBLE_COMPLEX->dt_label = TRDTFDBLCOMPLEX;
  MPI_F_DOUBLE_COMPLEX->dt_commit = 1;
  MPI_F_DOUBLE_COMPLEX->dt_flags |= LAM_PREDEF;
  MPI_F_DOUBLE_COMPLEX->dt_nelem = 1;
  strcpy(MPI_F_DOUBLE_COMPLEX->dt_name, "MPI_DOUBLE_COMPLEX");

  return (0);
}


/*
 *	init_cxxcomplex_types
 *
 *	Function:	- initialize the C++ complex datatypes
 *	Returns:	- 0 or LAMERROR
 */
static int
init_cxxcomplex_types(void)
{
  MPI_Datatype newdtype;
  int err;
/*
 * Create MPI::COMPLEX.
 */
  err = MPI_Type_contiguous(2, MPI_FLOAT, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_CXX_COMPLEX, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_CXX_COMPLEX->dt_label = TRDTCXXCOMPLEX;
  MPI_CXX_COMPLEX->dt_commit = 1;
  MPI_CXX_COMPLEX->dt_flags |= LAM_PREDEF;
  MPI_CXX_COMPLEX->dt_nelem = 1;
  strcpy(MPI_CXX_COMPLEX->dt_name, "MPI_COMPLEX");

/*
 * Create MPI::DOUBLE_COMPLEX.
 */
  err = MPI_Type_contiguous(2, MPI_DOUBLE, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_CXX_DOUBLE_COMPLEX, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_CXX_DOUBLE_COMPLEX->dt_label = TRDTCXXDCOMPLEX;
  MPI_CXX_DOUBLE_COMPLEX->dt_commit = 1;
  MPI_CXX_DOUBLE_COMPLEX->dt_flags |= LAM_PREDEF;
  MPI_CXX_DOUBLE_COMPLEX->dt_nelem = 1;
  strcpy(MPI_CXX_DOUBLE_COMPLEX->dt_name, "MPI_DOUBLE_COMPLEX");

/*
 * Create MPI::LONG_DOUBLE_COMPLEX.
 */
  err = MPI_Type_contiguous(2, MPI_LONG_DOUBLE, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_CXX_LONG_DOUBLE_COMPLEX, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_CXX_LONG_DOUBLE_COMPLEX->dt_label = TRDTCXXLDCOMPLEX;
  MPI_CXX_LONG_DOUBLE_COMPLEX->dt_commit = 1;
  MPI_CXX_LONG_DOUBLE_COMPLEX->dt_flags |= LAM_PREDEF;
  MPI_CXX_LONG_DOUBLE_COMPLEX->dt_nelem = 1;
  strcpy(MPI_CXX_LONG_DOUBLE_COMPLEX->dt_name, "MPI_LONG_DOUBLE_COMPLEX");

  return (0);
}


/*
 *	init_rdtype
 *
 *	Function:	- initialize the reduction datatypes
 *	Returns:	- 0 or LAMERROR
 */
static int
init_rdtype(void)
{
  MPI_Datatype newdtype;	/* new datatype */
  MPI_Datatype types[2];	/* struct datatypes */
  int lengths[2];		/* struct lengths */
  MPI_Aint disps[2];		/* struct displacements */
  struct flt_int fi[2];		/* float_int data */
  struct dbl_int di[2];		/* double_int data */
  struct long_int li[2];	/* long_int data */
  struct short_int si[2];	/* short_int data */
  struct longdbl_int ldi[2];	/* longdbl_int data */
  int err;			/* error code */
/*
 * Create MPI_2INT.
 */
  err = MPI_Type_contiguous(2, MPI_INT, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_2INT, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_2INT->dt_label = TRDT2INT;
  MPI_2INT->dt_commit = 1;
  MPI_2INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_2INT->dt_name, "MPI_2INT");
/*
 * Create MPI_FLOAT_INT.
 */
  types[0] = MPI_FLOAT;
  types[1] = MPI_INT;
  lengths[0] = 1;
  lengths[1] = 1;

  MPI_Address(&fi[0], &disps[0]);
  MPI_Address(&(fi[0].fi_int), &disps[1]);

  disps[1] -= disps[0];
  disps[0] = 0;

  err = MPI_Type_struct(2, lengths, disps, types, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_FLOAT_INT, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_FLOAT_INT->dt_label = TRDTFLOATINT;
  MPI_FLOAT_INT->dt_commit = 1;
  MPI_FLOAT_INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_FLOAT_INT->dt_name, "MPI_FLOAT_INT");
/*
 * Create MPI_DOUBLE_INT.
 */
  types[0] = MPI_DOUBLE;
  types[1] = MPI_INT;
  lengths[0] = 1;
  lengths[1] = 1;

  MPI_Address(&di[0], &disps[0]);
  MPI_Address(&(di[0].di_int), &disps[1]);

  disps[1] -= disps[0];
  disps[0] = 0;

  err = MPI_Type_struct(2, lengths, disps, types, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_DOUBLE_INT, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_DOUBLE_INT->dt_label = TRDTDOUBLEINT;
  MPI_DOUBLE_INT->dt_commit = 1;
  MPI_DOUBLE_INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_DOUBLE_INT->dt_name, "MPI_DOUBLE_INT");
/*
 * Create MPI_LONG_DOUBLE_INT.
 */
  types[0] = MPI_LONG_DOUBLE;
  types[1] = MPI_INT;
  lengths[0] = 1;
  lengths[1] = 1;

  MPI_Address(&ldi[0], &disps[0]);
  MPI_Address(&(ldi[0].ldi_int), &disps[1]);

  disps[1] -= disps[0];
  disps[0] = 0;

  err = MPI_Type_struct(2, lengths, disps, types, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_LONG_DOUBLE_INT,
	 (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_LONG_DOUBLE_INT->dt_label = TRDTLONGDBLINT;
  MPI_LONG_DOUBLE_INT->dt_commit = 1;
  MPI_LONG_DOUBLE_INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_LONG_DOUBLE_INT->dt_name, "MPI_LONG_DOUBLE_INT");
/*
 * Create MPI_LONG_INT.
 */
  types[0] = MPI_LONG;
  types[1] = MPI_INT;
  lengths[0] = 1;
  lengths[1] = 1;

  MPI_Address(&li[0], &disps[0]);
  MPI_Address(&(li[0].li_int), &disps[1]);

  disps[1] -= disps[0];
  disps[0] = 0;

  err = MPI_Type_struct(2, lengths, disps, types, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_LONG_INT, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_LONG_INT->dt_label = TRDTLONGINT;
  MPI_LONG_INT->dt_commit = 1;
  MPI_LONG_INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_LONG_INT->dt_name, "MPI_LONG_INT");
/*
 * Create MPI_SHORT_INT.
 */
  types[0] = MPI_SHORT;
  types[1] = MPI_INT;
  lengths[0] = 1;
  lengths[1] = 1;

  MPI_Address(&si[0], &disps[0]);
  MPI_Address(&(si[0].li_int), &disps[1]);

  disps[1] -= disps[0];
  disps[0] = 0;

  err = MPI_Type_struct(2, lengths, disps, types, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_SHORT_INT, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_SHORT_INT->dt_label = TRDTSHORTINT;
  MPI_SHORT_INT->dt_commit = 1;
  MPI_SHORT_INT->dt_flags |= LAM_PREDEF;
  strcpy(MPI_SHORT_INT->dt_name, "MPI_SHORT_INT");
/*
 * Create MPI_2INTEGER.
 */
  err = MPI_Type_contiguous(2, MPI_F_INTEGER, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_F_2INTEGER, (char *) newdtype,
	 sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_F_2INTEGER->dt_label = TRDTF2INTEGER;
  MPI_F_2INTEGER->dt_commit = 1;
  MPI_F_2INTEGER->dt_flags |= LAM_PREDEF;
  strcpy(MPI_F_2INTEGER->dt_name, "MPI_2INTEGER");
/*
 * Create MPI_2REAL.
 */
  err = MPI_Type_contiguous(2, MPI_F_REAL, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_F_2REAL, (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_F_2REAL->dt_label = TRDTF2REAL;
  MPI_F_2REAL->dt_commit = 1;
  MPI_F_2REAL->dt_flags |= LAM_PREDEF;
  strcpy(MPI_F_2REAL->dt_name, "MPI_2REAL");
/*
 * Create MPI_2DOUBLE_PRECISION.
 */
  err = MPI_Type_contiguous(2, MPI_F_DOUBLE_PRECISION, &newdtype);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  memcpy((char *) MPI_F_2DOUBLE_PRECISION,
	 (char *) newdtype, sizeof(struct _dtype));
  free((char *) newdtype);
  MPI_F_2DOUBLE_PRECISION->dt_label = TRDTF2DBLPREC;
  MPI_F_2DOUBLE_PRECISION->dt_commit = 1;
  MPI_F_2DOUBLE_PRECISION->dt_flags |= LAM_PREDEF;
  strcpy(MPI_F_2DOUBLE_PRECISION->dt_name, "MPI_2DOUBLE_PRECISION");

  return (0);
}


/*
 *	init_op
 *
 *	Function:	- initialize intrinsic reduction operations
 */
static void
init_op(void)
{
  make_op(MPI_MAX, lam_max);
  make_op(MPI_MIN, lam_min);
  make_op(MPI_SUM, lam_sum);
  make_op(MPI_PROD, lam_prod);
  make_op(MPI_LAND, lam_land);
  make_op(MPI_BAND, lam_band);
  make_op(MPI_LOR, lam_lor);
  make_op(MPI_BOR, lam_bor);
  make_op(MPI_LXOR, lam_lxor);
  make_op(MPI_BXOR, lam_bxor);
  make_op(MPI_MAXLOC, lam_maxloc);
  make_op(MPI_MINLOC, lam_minloc);

  make_op(MPI_REPLACE, lam_replace);
  MPI_REPLACE->op_commute = 0;
}


/*
 *	init_env
 *
 *	Function:	- initialize environment attributes
 *	Returns:	- 0 or LAMERROR
 */
static int
init_env(void)
{
  MPI_Group world;		/* world group */
  struct _attrkey *keystate;	/* key state */
  struct _proc **p;		/* process */
  int key;			/* attribute key */
  int err;			/* error code */
  struct route r;		/* router table entry */
  int i;
  int nnodes;			/* total number of nodes */
  int *nodes;			/* node ID array */
/*
 * Create the predefined keyvals for communicators.
 */
  /* We have to use LAM_MPI_C_* because MPISYSF will undef the C
   * versions of the functions if the naming convention is
   * UPPERCASE */

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_TAG_UB))
    return (LAMERROR);

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_HOST))
    return (LAMERROR);

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_IO))
    return (LAMERROR);

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_WTIME_IS_GLOBAL)) {
    return (LAMERROR);
  }

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_UNIVERSE_SIZE)) {
    return (LAMERROR);
  }

  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_APPNUM)) {
    return (LAMERROR);
  }
/*
 * Create the predefined keyvals for windows.
 */
  err = MPI_Win_create_keyval(LAM_MPI_C_WIN_NULL_COPY_FN,
			      LAM_MPI_C_WIN_NULL_DELETE_FN, &key,
			      (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_WIN_BASE))
    return (LAMERROR);

  err = MPI_Win_create_keyval(LAM_MPI_C_WIN_NULL_COPY_FN,
			      LAM_MPI_C_WIN_NULL_DELETE_FN, &key,
			      (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_WIN_SIZE))
    return (LAMERROR);

  err = MPI_Win_create_keyval(LAM_MPI_C_WIN_NULL_COPY_FN,
			      LAM_MPI_C_WIN_NULL_DELETE_FN, &key,
			      (void *) LAM_INT_TO_VOID_P 0);
  if ((err != MPI_SUCCESS) || (key != MPI_WIN_DISP_UNIT)) {
    return (LAMERROR);
  }
/* 
 * Make the predefined keyvals for IMPI attributes
 */
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != IMPI_CLIENT_SIZE))
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != IMPI_CLIENT_COLOR))
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != IMPI_HOST_SIZE))
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != IMPI_HOST_COLOR))
    return LAMERROR;
/* 
 * Make the predefined keyvals for some LAM-specific attributes
 */
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_UNIVERSE_NCPUS))
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN,
			       LAM_MPI_C_NULL_DELETE_FN, &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_UNIVERSE_NNODES))
    return LAMERROR;

  /* Make predefined keyvals for some LAM/MPI-specific attributes */
 	 
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_MPI_SSI_COLL)) 
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_MPI_SSI_COLL_CROSSOVER)) 
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_MPI_SSI_COLL_ASSOCIATIVE)) 
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || 
      (key != LAM_MPI_SSI_COLL_REDUCE_CROSSOVER)) 
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE)) 
    return LAMERROR;
  err = MPI_Comm_create_keyval(LAM_MPI_C_NULL_COPY_FN, 
			       LAM_MPI_C_NULL_DELETE_FN,
			       &key, (void *) 0);
  if ((err != MPI_SUCCESS) || (key != LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS)) 
    return LAMERROR;

 
/*
 * Initialize the attribute values.
 */
  wtimeglob = 0;
/*
 * We interpret MPI_IO to include the ability to print to a terminal.
 * MPI_IO is set to my rank if I'm on the origin node.
 * Otherwise, it's set to MPI_ANY_SOURCE if all processes are on
 * the origin node, MPI_PROC_NULL if no processes are on the
 * origin node or the lowest ranked process on the origin node.
 */
  world = MPI_COMM_WORLD->c_group;
  p = world->g_procs;

  for (i = 0; i < world->g_nprocs; ++i) {
    if ((*p)->p_gps.gps_node != getorigin())
      break;
    p++;
  }

  if (i >= world->g_nprocs) {
    io = MPI_ANY_SOURCE;
  } else if (lam_myproc->p_gps.gps_node == getorigin()) {
    io = world->g_myrank;
  } else {
    p = world->g_procs;

    for (i = 0; i < world->g_nprocs; ++i) {
      if ((*p)->p_gps.gps_node == getorigin())
	break;
      p++;
    }

    io = (i >= world->g_nprocs) ? MPI_PROC_NULL : i;
  }
/*
 * MPI_HOST is similarly related to the origin node, except if all
 * processes are on the origin, we set this attribute to rank 0.
 */
  p = world->g_procs;

  for (i = 0; i < world->g_nprocs; ++i) {
    if ((*p)->p_gps.gps_node == getorigin())
      break;
    p++;
  }

  host = (i >= world->g_nprocs) ? MPI_PROC_NULL : i;
/*
 * The universe size is inherited from the parents or is the number of
 * CPUs in the LAM.
 */
  nnodes = getntype(0, NT_CAST);
  if (lam_universe_size < 0) {
    nodes = malloc(sizeof(int) * nnodes);
    if (nodes == NULL)
      return (LAMERROR);

    getnodes(nodes, nnodes, 0, NT_CAST);
    for (lam_universe_size = i = 0; i < nnodes; i++) {
      r.r_nodeid = nodes[i];
      getrent(&r);
      lam_universe_size += r.r_ncpus;
    }
    free(nodes);
  }

  /* lam_universe_ncpus is the number of CPUs in the LAM
   * universe.  It is always the same as lam_universe_size; we
   * give them two variables because they're two different
   * attributes.  lam_universe_nnodes is the number of nodes
   * in the LAM universe. */

  lam_universe_ncpus = lam_universe_size;
  lam_universe_nnodes = nnodes;
/*
 * Store the attribute values.
 */
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, MPI_TAG_UB,
			  (void *) LAM_INT_TO_VOID_P lam_mpi_max_tag);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD, MPI_HOST,
			  (void *) LAM_INT_TO_VOID_P host);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD, MPI_IO,
			  (void *) LAM_INT_TO_VOID_P io);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD,
			  MPI_WTIME_IS_GLOBAL,
			  (void *) LAM_INT_TO_VOID_P wtimeglob);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD,
			  MPI_UNIVERSE_SIZE,
			  (void *) LAM_INT_TO_VOID_P lam_universe_size);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD,
			  LAM_UNIVERSE_NCPUS,
			  (void *) LAM_INT_TO_VOID_P lam_universe_ncpus);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  err = MPI_Comm_set_attr(MPI_COMM_WORLD,
			  LAM_UNIVERSE_NNODES,
			  (void *) LAM_INT_TO_VOID_P lam_universe_nnodes);
  if (err != MPI_SUCCESS)
    return (LAMERROR);

  if (lam_appnum >= 0) {
    err = MPI_Comm_set_attr(MPI_COMM_WORLD,
			    MPI_APPNUM,
			    (void *) LAM_INT_TO_VOID_P lam_appnum);
    if (err != MPI_SUCCESS)
      return (LAMERROR);
  }
/*
 * Mark them as predefined and mark the style, integer valued is F77 style.
 */
  if ((keystate = lam_getattr(MPI_TAG_UB)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_HOST)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_IO)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_WTIME_IS_GLOBAL)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_UNIVERSE_SIZE)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(LAM_UNIVERSE_NCPUS)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(LAM_UNIVERSE_NNODES)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_APPNUM)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_WIN_BASE)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF;

  if ((keystate = lam_getattr(MPI_WIN_SIZE)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;

  if ((keystate = lam_getattr(MPI_WIN_DISP_UNIT)) == 0)
    return (LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;
/*
 * Go do the same for the IMPI attributes
 *
 * JMS: Note -- we currently have the function call #if'ed because of
 * the two different tarballs that are currently being distirubuted --
 * with and without IMPI support.  Hence, when they merge back into a
 * single distribution (and libimpi.la is always built), we can make
 * this always be a function call (as it should be -- it's IMPI setup,
 * after all, regardless of whether this is an IMPI job, these
 * attributes need to be set, and the code should therefore be in
 * libimpi.la).
 */

#if LAM_WANT_IMPI
  IMPI_Fill_attributes();
#else
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_CLIENT_SIZE,
			  (void *) LAM_INT_TO_VOID_P & impi_client_size);
  if (err != MPI_SUCCESS)
    return LAMERROR;
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_CLIENT_COLOR,
			  (void *) LAM_INT_TO_VOID_P & impi_client_color);
  if (err != MPI_SUCCESS)
    return LAMERROR;
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_HOST_SIZE,
			  (void *) LAM_INT_TO_VOID_P & impi_host_size);
  if (err != MPI_SUCCESS)
    return LAMERROR;
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_HOST_COLOR,
			  (void *) LAM_INT_TO_VOID_P & impi_host_color);
  if (err != MPI_SUCCESS)
    return LAMERROR;
#endif

  return (0);
}


/*
 *	make_dtype
 *
 *	Function:	- create a basic datatype
 *			- no errors happen here
 *	Accepts:	- MPI datatype
 *			- size of datatype
 *			- datatype label
 *			- datatype name
 */
static void
make_dtype(MPI_Datatype dtype, int size, int align, int label, char *name)
{
  lam_type_setdefaults(dtype);

  dtype->dt_format = LAM_DTBASIC;
  dtype->dt_flags = LAM_DTNOPACK | LAM_DTNOXADJ | LAM_PREDEF;
  dtype->dt_commit = 1;
  dtype->dt_label = label;
  dtype->dt_refcount = 1;
  dtype->dt_align = align;
  dtype->dt_upper = size;
  dtype->dt_dataup = size;
  dtype->dt_size = size;
  dtype->dt_nelem = 1;
  strcpy(dtype->dt_name, name);
/*
 * Patch up the weird bound marker types.
 */
  if (dtype == MPI_UB) {
    dtype->dt_flags |= LAM_DTHASUB;
    dtype->dt_nelem = 0;
  } else if (dtype == MPI_LB) {
    dtype->dt_flags |= LAM_DTHASLB;
    dtype->dt_nelem = 0;
  }
}


/*
 *	make_op
 *
 *	Function:	- create intrinsic reduction operation
 *			- no errors happen here
 *	Accepts:	- MPI reduction operation
 *			- reduction function
 */
static void
make_op(MPI_Op op, void (*func) ())
{
  op->op_func = func;
  op->op_commute = 1;
  op->op_f77handle = -1;
  op->op_flags = LAM_PREDEF;
}


/*
 *	comm_cmp
 *
 *	Function:	- compare two communicators
 *	Accepts:	- ptr to two entries
 *	Returns:	- 0 if same communicator, else 1
 */
static int
comm_cmp(MPI_Comm * c1, MPI_Comm * c2)
{
#if LAM_WANT_IMPI
  if ((*c1)->c_flags & LAM_CIMPI) {
    if ((*c2)->c_flags & LAM_CIMPI)
      if (check_cid(c1, c2) == 0) {
	if (memcmp((*c1)->c_reserved, (*c2)->c_reserved,
		   sizeof(IMPI_Uint8)) == 0)
	  return 0;
	else
	  return 1;
      } else
	return 1;
    else
      return 1;
  } else if ((*c2)->c_flags & LAM_CIMPI)
    return 1;
  else
#endif
    return check_cid(c1, c2);
}


/*
 *	check_cid
 *
 *	Function:	- compare two communicators by CID only
 *	Accepts:	- ptr to two entries
 *	Returns:	- 0 if same communicator, else 1
 */
static int
check_cid(MPI_Comm * c1, MPI_Comm * c2)
{
  return (!(((*c1)->c_contextid == (*c2)->c_contextid)
	    || ((*c1)->c_contextid == -((*c2)->c_contextid + 1))));
}


/*
 *	init_f77
 *
 *	Function:	- initialization for F77
 *	Returns:	- 0 or LAMERROR
 */
static int
init_f77(void)
{
/*
 * Initialize the type lookup table.
 */
  lam_F_sizehdlarray = LAM_TYPETBLSIZE;
  lam_F_handles = (void **) malloc(lam_F_sizehdlarray * sizeof(void *));
  if (lam_F_handles == 0) {
    return (LAMERROR);
  }

  memset((char *) lam_F_handles, 0, lam_F_sizehdlarray * sizeof(void *));
  lam_F_nhdl = 0;
/*
 * These must be done in an order that matches the indices given to the
 * F77 constants in mpif.h.
 */
  MPI_COMM_WORLD->c_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_COMM_WORLD;
  MPI_COMM_SELF->c_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_COMM_SELF;

  MPI_GROUP_EMPTY->g_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_GROUP_EMPTY;

  MPI_ERRORS_ARE_FATAL->eh_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_ERRORS_ARE_FATAL;
  MPI_ERRORS_RETURN->eh_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_ERRORS_RETURN;

  MPI_BYTE->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_BYTE;
  MPI_PACKED->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_PACKED;
  MPI_UB->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UB;
  MPI_LB->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LB;
  MPI_F_CHARACTER->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_CHARACTER;
  MPI_F_LOGICAL->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_LOGICAL;
  MPI_F_INTEGER->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_INTEGER;
  MPI_F_INTEGER1->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_INTEGER1;
  MPI_F_INTEGER2->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++]  = MPI_F_INTEGER2; 
  MPI_F_INTEGER4->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++]  = MPI_F_INTEGER4; 
  MPI_F_INTEGER8->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++]  = MPI_F_INTEGER8; 
  MPI_F_REAL->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_REAL;
  MPI_F_REAL4->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_REAL4;
  MPI_F_REAL8->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_REAL8;
  MPI_F_REAL16->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_REAL16;
  MPI_F_DOUBLE_PRECISION->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_DOUBLE_PRECISION;
  MPI_F_COMPLEX->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_COMPLEX;
  MPI_F_DOUBLE_COMPLEX->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_DOUBLE_COMPLEX;
  MPI_F_2REAL->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_2REAL;
  MPI_F_2DOUBLE_PRECISION->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_2DOUBLE_PRECISION;
  MPI_F_2INTEGER->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_F_2INTEGER;

  MPI_MAX->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_MAX;
  MPI_MIN->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_MIN;
  MPI_SUM->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_SUM;
  MPI_PROD->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_PROD;
  MPI_LAND->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LAND;
  MPI_BAND->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_BAND;
  MPI_LOR->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LOR;
  MPI_BOR->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_BOR;
  MPI_LXOR->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LXOR;
  MPI_BXOR->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_BXOR;
  MPI_MAXLOC->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_MAXLOC;
  MPI_MINLOC->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_MINLOC;
  MPI_REPLACE->op_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_REPLACE;
/*
 * We must also register the groups associated with MPI_COMM_WORLD
 * and MPI_COMM_SELF.
 */
  MPI_COMM_WORLD->c_group->g_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_COMM_WORLD->c_group;
  MPI_COMM_SELF->c_group->g_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_COMM_SELF->c_group;
/*
 * The C basic types are also registered to allow use of the F77 handle
 * as an integer type identifier in one-sided communication.
 */
  MPI_CHAR->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_CHAR;
  MPI_SHORT->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_SHORT;
  MPI_INT->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_INT;
  MPI_LONG->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LONG;
  MPI_FLOAT->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_FLOAT;
  MPI_DOUBLE->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_DOUBLE;
  MPI_LONG_DOUBLE->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LONG_DOUBLE;
  MPI_UNSIGNED_CHAR->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UNSIGNED_CHAR;
  MPI_UNSIGNED_SHORT->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UNSIGNED_SHORT;
  MPI_UNSIGNED_LONG->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UNSIGNED_LONG;
  MPI_UNSIGNED->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UNSIGNED;
  MPI_WCHAR->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_WCHAR;
  MPI_LONG_LONG_INT->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_LONG_LONG_INT;
  MPI_UNSIGNED_LONG_LONG->dt_f77handle = lam_F_nhdl;
  lam_F_handles[lam_F_nhdl++] = MPI_UNSIGNED_LONG_LONG;

  lam_F_maxhdl = lam_F_nhdl - 1;
  if (lam_F_nhdl != LAM_MAXF77PREDEF) {
    errno = EIMPOSSIBLE;
    return (LAMERROR);
  }
/*
 * Initialize F77 constants which correspond to addresses.
 */
  lam_F_bottom = (char *) &mpi_bottom_[0];
  lam_F_argvnull = &mpi_argv_null_[0];
  lam_F_argvsnull = &mpi_argvs_null_[0];
  lam_F_errorcodes_ignore = &mpi_errcodes_ignore_[0];
  lam_F_status_ignore = &mpi_status_ignore_[0];
  lam_F_statuses_ignore = &mpi_statuses_ignore_[0];

  MPI_F_STATUS_IGNORE = (MPI_Fint *) lam_F_status_ignore;
  MPI_F_STATUSES_IGNORE = (MPI_Fint *) lam_F_statuses_ignore;

  return (0);
}


/*
 * This function is used in the beginning of MPI_INIT because there is
 * no communicator yet, and therefore we can't call lam_com_errfatal
 * and/or MPI_Abort.
 */
static void 
fail_stop(const char *program, const char *topic)
{
  struct _proc *p;
  char *env, hostname[BUFSIZ], node[BUFSIZ], pid[BUFSIZ], rank[BUFSIZ];

  if (program != NULL && topic != NULL) {
    gethostname(hostname, BUFSIZ - 2);
    hostname[BUFSIZ - 1] = '\0';

    snprintf(node, BUFSIZ - 2, "%d", getnodeid());
    node[BUFSIZ - 1] = '\0';

    snprintf(pid, BUFSIZ - 2, "%d", lam_getpid());
    pid[BUFSIZ - 1] = '\0';

    if ((env = getenv("LAMRANK")) != NULL) {
      lam_strncpy(rank, env, BUFSIZ - 2);
      rank[BUFSIZ - 1] = '\0';
    } else {
      rank[0] = '0';
      rank[0] = '\0';
    }
    show_help(program, topic, hostname, node, pid, rank, NULL);
  }

  /* Kill any peers that I might have. */

  for (p = lam_topproc(); p != NULL; p = lam_nextproc()) {
    if (p != lam_myproc) {
      rpdoom(p->p_gps.gps_node, SELECT_PID, p->p_gps.gps_pid, SIGUDIE);
    }
  }

  /* Ok, now die */

  kexit(-1);
}
