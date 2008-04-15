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
 * $Id: laminit.c,v 6.63 2003/09/11 19:52:02 jsquyres Exp $
 *
 *	Function:	- LAM specific initialization for MPI
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <laminternal.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <mpi_apps.h>
#include <net.h>
#include <preq.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-cr.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <etc_misc.h>
#include <trreq.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>
#include <lam_network.h>
#include <lammpithreads.h>


/*
 * public variables
 */
struct _comm		lam_mpi_comm_world;
struct _comm		lam_mpi_comm_self;

struct _group		lam_mpi_group_empty;
struct _op		lam_mpi_max, lam_mpi_min;
struct _op		lam_mpi_sum, lam_mpi_prod;
struct _op		lam_mpi_land, lam_mpi_band;
struct _op		lam_mpi_lor, lam_mpi_bor;
struct _op		lam_mpi_lxor, lam_mpi_bxor;
struct _op		lam_mpi_maxloc, lam_mpi_minloc;
struct _op		lam_mpi_replace;

struct _dtype		lam_mpi_char, lam_mpi_byte;
struct _dtype		lam_mpi_int, lam_mpi_logic;
struct _dtype		lam_mpi_short, lam_mpi_long;
struct _dtype		lam_mpi_float, lam_mpi_double;
struct _dtype		lam_mpi_long_double;
struct _dtype		lam_mpi_cplex, lam_mpi_packed;
struct _dtype		lam_mpi_unsigned_char;
struct _dtype		lam_mpi_unsigned_short;
struct _dtype		lam_mpi_unsigned;
struct _dtype		lam_mpi_unsigned_long;
struct _dtype		lam_mpi_ub, lam_mpi_lb;
struct _dtype		lam_mpi_float_int, lam_mpi_double_int;
struct _dtype		lam_mpi_long_int, lam_mpi_2int;
struct _dtype		lam_mpi_short_int, lam_mpi_dblcplex;
struct _dtype		lam_mpi_integer, lam_mpi_integer1;
struct _dtype           lam_mpi_integer2, lam_mpi_integer4;
struct _dtype           lam_mpi_integer8, lam_mpi_real;
struct _dtype           lam_mpi_real4, lam_mpi_real8, lam_mpi_real16;
struct _dtype		lam_mpi_dblprec, lam_mpi_character;
struct _dtype		lam_mpi_2real, lam_mpi_2dblprec;
struct _dtype		lam_mpi_2integer, lam_mpi_longdbl_int;
struct _dtype		lam_mpi_wchar, lam_mpi_long_long_int;
struct _dtype		lam_mpi_unsigned_long_long;

struct _dtype lam_mpi_cxx_cplex, lam_mpi_cxx_dblcplex, lam_mpi_cxx_ldblcplex;
struct _dtype lam_mpi_cxx_bool;

struct _errhdl		lam_mpi_errors_are_fatal;
struct _errhdl		lam_mpi_errors_return;

MPI_Comm		lam_comm_parent = MPI_COMM_NULL;
struct _proc		*lam_myproc;
LIST			*lam_comms = 0;
LIST			*lam_ports = 0;
float8			lam_clockskew = 0.0;
int			lam_f77init = 0;
int			lam_c2c = 0;
int			lam_ger = 0;
int			lam_homog = 0;
struct jobid_t		lam_jobid = { 0, 0 };
int			lam_universe_size = -1;
int			lam_universe_ncpus = -1;
int			lam_universe_nnodes = -1;
int			lam_appnum = -1;
int		        lam_mpi_max_tag = INT_MAX;
int		        lam_mpi_max_cid = 65536;

MPI_Fint		*MPI_F_STATUS_IGNORE;
MPI_Fint		*MPI_F_STATUSES_IGNORE;

char                    *lam_argv0 = 0;

OPT *                   lam_mpi_ao = NULL;


#if LAM_WANT_IMPI
struct _gps             gimpid;
#endif


/*
 * private functions
 */
static int get_singleton_idx(int pid);


/*
 * global functions
 */
int lam_send_pid_idx(struct nmsg *nhead);


/*
 *	lam_linit
 *
 *	Function:	- initialize the process
 *	Accepts:	- program name
 *			- # processes in local world (returned)
 *			- # processes in parent world (returned)
 *			- CID to use for parent intercomm (returned)
 *			- parent and new worlds process GPS array (returned)
 *			- root in parent comm if any (returned)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_linit(int argc, char *argv[], char *name, int *world_n, int *parent_n, 
	  int *cid, struct _gps **worlds, int *root)
{
	char		*trworld;		/* world trace */
	int		trwlen;			/* length of world trace */
	struct _gps	*procs;			/* process GPS array */
	struct _gps	*p;			/* favourite pointer */
	struct nmsg	nhead;			/* network message header */
	struct _proc	*proc;			/* process entry */
	int		i;
	int	        procs_n;
/*
 * Attach process to the kernel and turn it into an MPI process.
 */

	lam_argv0 = name;
	if (kenter(name, 0)) {
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
	    snprintf(num, 127, "%d", KPMAX);
	    show_help("MPI", "lamd-full", "MPI_Init", num, hostname, NULL);
	  }

	  errno = ENOKERNEL;
	  return(LAMERROR);
	}

        /* If we were forked (no mpirun), then disable all tracing. */

	if (_kio.ki_rtf & RTF_FORKED) {
	  _kio.ki_rtf &= ~RTF_TRON;
	}

	/* The "C2C" mode is pretty meaningless these days (all MPI
           communications use RPIs, and none is really any different
           than another), but there's still some code that checks this
           kind of stuff.  So always set it. */

        _kio.ki_rtf |= RTF_MPIC2C;
	_kio.ki_rtf |= RTF_MPI;

	/* If process was not launched with mpirun, there will not
	   necessarily be a way to notice signals.  Normally, the lamd
	   will notice that a process died by a signal and propogate
	   that information back to mpirun, who will print out a
	   warning message and then call rpdoom().  If there is no
	   mpirun, we don't have that luxury (particularly in the
	   model where the user launches "./master", who then invokes
	   MPI_COMM_SPAWN to launch a bunch of slaves).  So we have to
	   handle signals here (print out an error message, call
	   rpdoom(), and possibly call the next signal handler in the
	   chain). */
	if ((_kio.ki_rtf & RTF_MPIRUN) == 0)
	  _kio.ki_rtf |= RTF_MPISIGS;
	if (lpattach(name)) 
	  return(LAMERROR);
	lam_flinit = 1;

	if (getenv("LAM_MPI_PAUSE")) {
	    kpause();
	}

	/* Setup the argc/argv argument parsing. */
        /* Update: Oct 8 2004.  lam_mpi_ao is now a dummy / empty OPT.
           We used to parse the (argc, argv) that came in from
           MPI_INIT, but I'm not sure why.  A user has posted a case
           where it breaks, actually -- where argv comes in from
           fortran, a C wrapper munges it into an argv-like structure,
           but does not NULL-terminate the list of pointers.  Down in
           ao_parse(), we sfh_argv_dup() which assumes that the argv
           will be NULL-terminated.  This is just silly to begin with
           -- we shouldn't be looking at the (argc, argv) from
           MPI_INIT anyway.  The only reason that this is done is that
           ssi_module_open() takes argv/argc, but this appears to be
           only for hysterical raisins -- there's no real need for
           this.  However, since we're in the final few releases of
           LAM, it's really not worth changing the ssi_module_open()
           prototype and then going to touch all modules to match.
           Instead, it's safer/easier to just not parse the (argc,
           argv) here and leave lam_mpi_ao be an empty OPT. */

	lam_mpi_ao = ao_init();
	ao_setflags(lam_mpi_ao, AOPRESERVE_ARGV | AOIGN_UNRECOG);

	/* Get the proc array from whoever launched us */

	procs_n = 0; 
	if (lam_get_proc_array(&procs, world_n, cid, &procs_n, 0)
	    == LAMERROR) {
	  free((char *) procs);
	  return(LAMERROR);
	}

/*
 * Set various runtime flags.
 */
	lam_c2c = _kio.ki_rtf & RTF_MPIC2C;
	lam_ger = _kio.ki_rtf & RTF_MPIGER;
/* 
 * If started by mpirun, send back my real PID and IDX to give
 * mpirun the ability to run shell scripts and debuggers.
 */
	LAM_ZERO_ME(nhead);
	if (_kio.ki_rtf & RTF_MPIRUN) {
	    if (lam_send_pid_idx(&nhead)) {
		free((char *) procs);
		return(LAMERROR);
	    }
	}
/*
 * If spawned or started by mpirun, receive the list of GPS.  Local
 * world GPS's are first followed by the parents (if any).  Otherwise if
 * the number of processes is one assume a singleton init, else assume one
 * process per node and pids are not needed.
 */
	if ((_kio.ki_parent > 0) || (_kio.ki_rtf & RTF_MPIRUN)) {

		nhead.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
		nhead.nh_type = BLKMPIINIT;
		nhead.nh_flags = DINT4DATA;
		nhead.nh_length = procs_n * sizeof(struct _gps);
		nhead.nh_msg = (char *) procs;

		if (nrecv(&nhead)) {
			free((char *) procs);
			return(LAMERROR);
		}
/*
 * If spawned save the intercommunicator context ID, spawning root,
 * universe size and application number.  
 */
		if (_kio.ki_parent > 0) {
			*cid = (int) nhead.nh_data[1];
			*root = (int) nhead.nh_data[2];
			lam_universe_size = (int) nhead.nh_data[3];
			lam_appnum = (int) nhead.nh_data[4];
		}
	} else {
		if (procs_n == 1) {
			procs->gps_node = getnodeid();
			procs->gps_pid = lam_getpid();
			procs->gps_idx = get_singleton_idx(procs->gps_pid);
			if (procs->gps_idx < 0)	return(LAMERROR);
		} else {
			for (i = 0, p = procs; i < procs_n; ++i, ++p) {
				p->gps_node = i;
				p->gps_pid = lam_getpid();
				p->gps_idx = 0;
			}
		}
	}

	if (_kio.ki_jobid.jid_node != 0) {
	  lam_jobid.jid_node = _kio.ki_jobid.jid_node;
	  lam_jobid.jid_pid = _kio.ki_jobid.jid_pid;
	} else {
	  lam_jobid.jid_node = procs->gps_node;
	  lam_jobid.jid_pid = procs->gps_pid;
	}
/*
 * Set the global ranks in the local world and add the local world
 * processes to the process list while finding out my identity.
 */
	for (i = 0, p = procs; i < *world_n; ++i, ++p) {
		p->gps_grank = i;
		proc = lam_procadd(p);
		if (proc == 0) 
                  return(LAMERROR);

		if ((p->gps_node == getnodeid()) && 
                    (p->gps_pid == lam_getpid())) {
			lam_myproc = proc;
			lam_myproc->p_mode |= LAM_PHOMOG;
		}

		if (_kio.ki_parent > 0) {
			proc->p_mode |= LAM_PCLIENT;
		}
	}
/*
 * Add the parents (if any) to the process list.
 */
	for (i = 0; i < _kio.ki_parent; ++i, ++p) {
		proc = lam_procadd(p);
		if (proc == 0) {
			return(LAMERROR);
		}
		proc->p_mode |= LAM_PCLIENT;
	}  

/*
 * Synchronize clocks across the nodes.
 */
	if (_kio.ki_rtf & RTF_TRACE) {
		if (lam_clocksync(procs_n, procs, &lam_clockskew))
			return(LAMERROR);
	}
/*
 * Global rank zero records the world trace.
 */
	if ((_kio.ki_rtf & RTF_TRACE)
			&& (lam_myproc->p_gps.gps_grank == 0)) {

		trwlen = 2 * sizeof(int4) + (*world_n) * sizeof(struct _gps);

		trworld = (char *) malloc((unsigned) trwlen);
		if (trworld == 0) return(LAMERROR);

		((int4 *) trworld)[0] = LAM_TRMAGIC;
		((int4 *) trworld)[1] = *world_n;
		memcpy(trworld + 2 * sizeof(int4), (char *) procs,
					(*world_n) * sizeof(struct _gps));

		mltoti4(trworld, trwlen / sizeof(int4));

		if (lam_rtrstore(LOCAL, TRWORLD, lam_myproc->p_gps.gps_pid,
				trworld, trwlen)) {
			free(trworld);
			return(LAMERROR);
		}

		free(trworld);
	}

	*parent_n = _kio.ki_parent;
	*worlds = procs;

	return(0);

}


/*
 *	init_comm_world
 *
 *	Function:	- initialize MPI_COMM_WORLD
 */
int
lam_init_comm_world(struct _gps *mpi_procs, int mpi_nprocs)
{
	MPI_Group	group;			/* a process group */
	MPI_Comm	comm;			/* a communicator */
	int		i;
	struct _gps	*g;
	struct _proc	**p;
	int             cid;

	comm = MPI_COMM_WORLD;

	group = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(mpi_nprocs * sizeof(struct _proc *)));
	if (group == 0) return(LAMERROR);

	group->g_nprocs = mpi_nprocs;
	group->g_myrank = MPI_UNDEFINED;
	group->g_refcount = 1;
	group->g_f77handle = -1;
	group->g_procs = (struct _proc **)
				((char *) group + sizeof(struct _group));
/*
 * Fill the group processes and determine my rank.
 */
	g = mpi_procs;
	p = group->g_procs;

	for (i = 0; i < mpi_nprocs; ++i, ++g, ++p) {

		if ((*p = lam_procfind(g)) == 0) {
			return(LAMERROR);
		}
		if (*p == lam_myproc) {
			group->g_myrank = i;
		}
		(*p)->p_refcount++;
	}

	cid = lam_getcid();
	if (lam_comm_new(cid, group, MPI_GROUP_NULL, LAM_PREDEF, &comm))
		return(LAMERROR);

	lam_setcid(cid);
	comm->c_errhdl->eh_refcount++;
	strcpy(comm->c_name, "MPI_COMM_WORLD");

	if (LAM_TRACE_TOP())
	  lam_tr_commname(comm->c_name, comm->c_contextid, MPI_Wtime());

	if (!al_insert(lam_comms, &comm)) 
	  return(LAMERROR);

	return 0;
}


/*
 *	get_singleton_idx
 *
 *	Function:	- get kenya index of singleton MPI process
 *	Accepts:	- process pid
 *	Returns:	- kenya index or LAMERROR
 */
static int
get_singleton_idx(int pid)
{
	struct pstate	state;			/* state returned from kenyad */
	
	if (rpstate(LOCAL, SELECT_PID, pid, &state, 1) != 1) {
		return(LAMERROR);
	}

	return(state.ps_index);
}


int
lam_get_proc_array(struct _gps **procs, int *world_n, int *cid,
		   int *procs_n, int is_impi)
{
/*
 *  This subroutine is called twice if it is an IMPI job 
 */

/*
 * Get the # of processes in the world.
 * Allocate the array of process GPS.
 */
  
  if (!is_impi) {
    if ((_kio.ki_parent > 0) || (_kio.ki_rtf & RTF_MPIRUN)) {
      *world_n = _kio.ki_world;
    } else {
      *world_n = getenv("LAMWORLDNODES") ? getncomp() : 1;
    }
  } else {
    
#if LAM_WANT_IMPI
    /* 
     * But if this is an IMPI job, that number is possibly wrong --
     * let's check and see if we need to get a new one (from the
     * IMPI server) expand the world number to include all of impi
     * world as well even though the impid will be the proxy entry
     * for them
     */

    *cid = impi_server_info.min_cid;
    lam_mpi_max_tag = impi_server_info.max_tag;
#endif
  }
  
  *procs_n = *world_n + _kio.ki_parent;
  
  *procs = (struct _gps *)
    malloc((unsigned) *procs_n * sizeof(struct _gps));

  if (*procs == 0)
    return(LAMERROR);
  else
    return 0;
}


/*
 * 	lam_send_pid_idx
 *
 * 	Function:	- sends the pid and kenyad index to mpirun
 * 	Accepts:	- network message descriptor
 * 	Returns:	- 0 (on success) or LAMERROR
 */
int 
lam_send_pid_idx(struct nmsg *nhead)
{
  char *env;
  struct pstate	pstate;
  struct mpi_app_proc_info *mapi;

  if ((env = getenv("LAMRANK")) == 0)
    return(LAMERROR);
  
  /*
   * Get our process (kenya) index.
   */
  if (rpstate(LOCAL, SELECT_PID, lam_getpid(), &pstate, 1) < 1)
    return(LAMERROR);
  
  /*
   * The event is -(mpirun's pid). This will distinguish it from user
   * events. The type is 2 to distinguish it from "wait" messages which
   * the same event and type 1.
   */
  LAM_ZERO_ME(mapi);
  nhead->nh_node = _kio.ki_jobid.jid_node;
  nhead->nh_event = (-_kio.ki_jobid.jid_pid) & 0xBFFFFFFF;
  nhead->nh_type = 2;
  nhead->nh_flags = DINT4DATA;
  nhead->nh_length = 0;
  mapi = (struct mpi_app_proc_info *) &nhead->nh_data;
  mapi->mapi_mcw_rank = stoi((unsigned char*) env);
  mapi->mapi_pid = lam_getpid();
  mapi->mapi_ps_index = pstate.ps_index;

  if (nsend(nhead))
    return(LAMERROR);

  return(0);
}
