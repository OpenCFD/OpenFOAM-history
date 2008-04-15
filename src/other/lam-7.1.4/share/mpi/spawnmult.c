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
 * $Id: spawnmult.c,v 6.41 2003/07/05 21:55:23 jsquyres Exp $
 *
 *	Function:	- spawn multiple MPI programs
 *	Accepts:	- number of command lines
 *			- array of program names
 *			- array of argument vectors
 *			- array of max. number of processes to start
 *			- array of info
 *			- root in spawning communicator
 *			- spawning communicator
 *			- intercomm between parents and children (out)
 *			- array of error codes (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <app_schema.h>
#include <args.h>
#include <blktype.h>
#include <etc_misc.h>
#include <mpi.h>
#include <mpisys.h>
#include <ndi.h>
#include <net.h>
#include <portable.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <lam_config.h>
#include <sfh.h>

/*
 * private functions
 */
static int		spawn(LIST *app, MPI_Comm comm, int cid, int *nprocs,
			      int *numkids, struct _gps **kids);
static LIST		*build_app(int count, char **commands, char ***argvs,
				   int *maxprocs, MPI_Info *infos);
static void		set_error_codes(int err, LIST *app, int *errs, 
					int *msg);

/*
 * external functions
 */
int                     lam_get_mpi_world(int4 world_n, struct _gps *world, 
					  struct _gps *mpiworld, 
					  char *prefix);


/*@
   MPI_Comm_spawn_multiple - Spawn a dynamic MPI process from multiple
executables

Input Parameters:
+ count - number of commands (only significant at root)
. commands - commands to be executed (only significant at root)
. argvs - arguments for commands (only significant at root)
. maxprocs - max number of processes for each command (only significant at root)
. infos - startup hints for each command
. root - rank of process to perform the spawn
- comm - parent intracommunicator

Output Parameters:
+ intercomm - child intercommunicator containing spawned processes
- errcodes - one code per process

Description:

A group of processes can create another group of processes with
'MPI_Comm_spawn_multiple'.  This function is a collective operation
over the parent communicator.  The child group starts up like any MPI
application.  The processes must begin by calling 'MPI_Init', after
which the pre-defined communicator, 'MPI_COMM_WORLD', may be used.
This world communicator contains only the child processes.  It is
distinct from the 'MPI_COMM_WORLD' of the parent processes.

'MPI_Comm_spawn_multiple' is used to manually specify a group of
different executables and arguments to spawn.  'MPI_Comm_spawn' is
used to specify one executable and set of arguments (although a
LAM/MPI appschema(5) can be provided to 'MPI_Comm_spawn' via the
"file" info key).
 
Communication With Spawned Processes

The natural communication mechanism between two groups is the
intercommunicator.  The second communicator argument to
'MPI_Comm_spawn_multiple' returns an intercommunicator whose local
group contains the parent processes (same as the first communicator
argument) and whose remote group contains child processes. The child
processes can access the same intercommunicator by using the
'MPI_Comm_get_parent' call.  The remote group size of the parent
communicator is zero if the process was created by 'mpirun'(1) instead
of one of the spawn functions.  Both groups can decide to merge the
intercommunicator into an intracommunicator (with the
'MPI_Intercomm_merge'() function) and take advantage of other MPI
collective operations.  They can then use the merged intracommunicator
to create new communicators and reach other processes in the MPI
application.
 
Resource Allocation

Note that no MPI_Info keys are recognized by this implementation of
'MPI_Comm_spawn_multiple'.  To use the "file" info key to specify an
appschema(5), use LAM''s 'MPI_Comm_spawn'.  This may be preferable to
'MPI_Comm_spawn_multiple' because it allows the arbitrary
specification of what nodes and/or CPUs should be used to launch jobs
(either SPMD or MPMD).  See MPI_Comm_spawn(3) for more details.

The value of 'MPI_INFO_NULL' should be given for each value in 'infos'
(the 'infos' array is not currently examined by LAM/MPI, so specifying
non-NULL values for the array values is not harmful).  LAM schedules
the given number of processes onto LAM nodes by starting with CPU 0
(or the lowest numbered CPU), and continuing through higher CPU
numbers, placing one process on each CPU.  If the process count is
greater than the CPU count, the procedure repeats.

Process Terminiation

Note that the process[es] spawned by 'MPI_COMM_SPAWN' (and
'MPI_COMM_SPAWN_MULTIPLE') effectively become orphans.  That is, the
spawnning MPI application does not wait for the spawned application to
finish.  Hence, there is no guarantee the spawned application has
finished when the spawning completes.  Similarly, killing the spawning
application will also have no effect on the spawned application.

User applications can effect this kind of behavior with 'MPI_BARRIER'
between the spawning and spawned processed before 'MPI_FINALIZE'.

Note that 'lamclean' will kill *all* MPI processes.
 
Process Count

The 'maxprocs' array parameter to 'MPI_Comm_spawn_multiple' specifies
the exact number of processes to be started.  If it is not possible to
start the desired number of processes, 'MPI_Comm_spawn_multiple' will
return an error code.  Note that even though 'maxprocs' is only
relevant on the root, all ranks must have an 'errcodes' array long
enough to handle an integer error code for every process that tries to
launch, or give MPI constant 'MPI_ERRCODES_IGNORE' for the 'errcodes'
argument.  While this appears to be a contradiction, it is per the
MPI-2 standard.  :-\

Frequently, an application wishes to chooses a process count so as to
fill all processors available to a job.  MPI indicates the maximum
number of processes recommended for a job in the pre-defined
attribute, 'MPI_UNIVERSE_SIZE', which is cached on 'MPI_COMM_WORLD'.
The typical usage is to subtract the value of 'MPI_UNIVERSE_SIZE' from
the number of processes currently in the job and spawn the difference.
LAM sets 'MPI_UNIVERSE_SIZE' to the number of CPUs in the user''s LAM
session (as defined in the boot schema [bhost(5)] via 'lamboot'(1)).
 
See MPI_Init(3) for other pre-defined attributes that are helpful when
spawning.
 
Locating an Executable Program

The executable program file must be located on the node(s) where the
process(es) will run.  On any node, the directories specified by the
user''s PATH environment variable are searched to find the program.
 
All MPI runtime options selected by 'mpirun'(1) in the initial
application launch remain in effect for all child processes created by
the spawn functions.

Command-line Arguments

The 'argvs' array parameter to 'MPI_Comm_spawn_multiple' should not
contain the program name since it is given in the first parameter.
The command line that is passed to the newly launched program will be
the program name followed by the strings in corresponding entry in the
'argvs' array.

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_SPAWN
.N MPI_ERR_ARG
.N MPI_ERR_ROOT
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso appschema(5), bhost(5), lamboot(1), MPI_Comm_get_parent(3), MPI_Intercomm_merge(3), MPI_Comm_spawn_multiple(3), MPI_Info_create(3), MPI_Info_set(3), MPI_Info_delete(3), MPI_Info_free(3), MPI_Init(3), mpirun(1)

.N ACK
@*/
int
MPI_Comm_spawn_multiple(int count, char **commands, char ***argvs,
			int *maxprocs, MPI_Info *infos, int root, 
			MPI_Comm comm, MPI_Comm *intercomm, 
			int *errcodes)
{
	MPI_Status	stat;
	MPI_Group	kgrp;			/* child group */
	LIST		*app;			/* application */
	struct _gps	*kids;			/* array of child GPS */
	struct _gps	*g;
	struct _proc	**p;
	int		rank;			/* caller rank */
	int		size;			/* group size */
	int		err;			/* error code */
	int		numkids = 0;		/* num. of children spawned */
	int		mycid;			/* local max context ID */
	int		cid;			/* context ID for intercomm */
	int		*msg;			/* message buffer */
	int		i;
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMSPAWNMULT);
/*
 * Check the arguments.
 */
	if ((comm == MPI_COMM_NULL) || LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				   lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	MPI_Comm_size(comm, &size);

	if ((root >= size) || (root < 0)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				   lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	}

	if (intercomm == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPICOMMSPAWNMULT, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

#if LAM_WANT_IMPI
	if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, ENOSYS,
			      "MPI_Comm_spawn_multiple not possible on IMPI communicators");
	}
#endif
	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMSPAWNMULT));
/*
 * Set debugging parameters.
 */
	g = &(comm->c_group->g_procs[root]->p_gps);

	lam_setparam(BLKMPICOMMSPAWNMULT, root | (g->gps_grank << 16),
				(g->gps_node << 16) | g->gps_idx);
/*
 * Synchronize all members of the parent group and get the context ID
 * for the parent-child intercommunicator.
 */
	MPI_Comm_rank(comm, &rank);

	mycid = lam_getcid();

	if (mycid < 0) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				lam_mkerr(MPI_ERR_INTERN, EFULL)));
	}

	err = MPI_Reduce(&mycid, &cid, 1, MPI_INT, MPI_MAX, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
	}

	for (size = i = 0; i < count; i++)
	  size += maxprocs[i];
	msg = malloc(sizeof(int) * (size + 3));
	memset(msg, -1, sizeof(int) * (size + 3));

	if (rank == root) {
/*
 * The root does the process spawning.
 */
		err = MPI_SUCCESS;
		
		app = build_app(count, commands, argvs, maxprocs, infos);
		if (app == 0) {
			err = lam_mkerr(MPI_ERR_OTHER, errno);
		} else {
			if (spawn(app, comm, cid, maxprocs, &numkids, &kids)) {
			  err = lam_mkerr(MPI_ERR_SPAWN, ECHILD);
			}
			set_error_codes(err, app, errcodes, msg + 3);
			asc_free(app);
		}

		/* Send a "sad" message to the rest of the ranks in
                   the parent group */

		if (err != MPI_SUCCESS) {
			msg[0] = -1; 
			msg[1] = err;
			MPI_Bcast(msg, 3, MPI_INT, root, comm);
			free((char *) msg);
			return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
		}
		
		/* Send a "happy" message to the rest of the ranks in
                   the parent group */

		msg[0] = cid; 
		msg[1] = numkids; 
		msg[2] = size;
	}
/*
 * Broadcast the context ID for the parent-child intercommunicator and the
 * number of children spawned to the parents.  In the case of an error
 * in spawning the root broadcasted an error code.
 */
	err = MPI_Bcast(msg, 3, MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		free((char *) msg);
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
	}
/* 
 * #%$^#$%^$#^ MPI-2!!  Since maxprocs is only significant at the
 * root, we can't depend on the value that the non-root nodes give.
 * So we first have to bcast the maxprocs to all non-root nodes, and
 * *then* send array_of_errcodes.
 */
	if (rank != root)
	  size = msg[2];
	err = MPI_Bcast(msg + 3, size, MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
	  free((char *) msg);
	  return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	}
	
	if (rank != root) {
		cid = msg[0];
		if (errcodes != MPI_ERRCODES_IGNORE)
		  for (i = 0; i < size; i++)
		    errcodes[i] = msg[3 + i];
/*
 * A context ID of -1 means an error occurred in spawning so we
 * return with the error.
 */
		if (cid == -1) {
			err = msg[1];
			free((char *) msg);
			return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
		}
/*
 * Allocate buffer to receive array of child GPS.
 */
		numkids = msg[1];
		kids = (struct _gps *)
			malloc((unsigned) (numkids * sizeof(struct _gps)));
		if (kids == 0) {
			free((char *) msg);
			return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
	}
	free((char *) msg);
/*
 * Broadcast the array of child GPS to parent group.
 */
	err = MPI_Bcast(kids, numkids * sizeof(struct _gps) / sizeof(int),
			MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		free((char *) kids);
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
	}
        
/*
 * Create the child group.
 */
	kgrp = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(numkids * sizeof(struct _proc *)));
	if (kgrp == 0) {
		free((char *) kids);
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	kgrp->g_nprocs = numkids;
	kgrp->g_myrank = MPI_UNDEFINED;
	kgrp->g_refcount = 1;
	kgrp->g_f77handle = -1;
	kgrp->g_procs = (struct _proc **)
				((char *) kgrp + sizeof(struct _group));

	g = kids;
	p = kgrp->g_procs;

	for (i = 0; i < numkids; ++i, ++p, ++g) {

		if ((*p = lam_procadd(g)) == 0) {
			free((char *) kids);
			free((char *) kgrp);
			return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		(*p)->p_mode |= LAM_PCLIENT;
		(*p)->p_refcount++;
	}
	free((char *) kids);

	/*
	 * Get the endian bias and potentially reset lam_homog
	 */
	lam_exchange_endian_info(kgrp->g_procs, numkids);

        /*
         * Check to ensure that the RPI and CRMPI modules selected by the
         * spawned processes match.
         */
        if ((rank == root) && (lam_match_ssi_modules(numkids, kids, "MPI_Comm_spawn"))) {
          free(kids);
          return(lam_errfunc(comm, BLKMPICOMMSPAWN,
                             lam_mkerr(MPI_ERR_OTHER, errno)));
        }

/*
 * Create the parent-child intercommunicator.
 */
	*intercomm = 0;
	if (lam_comm_new(cid, comm->c_group, kgrp, LAM_CINTER, intercomm)) {
		free((char *) kgrp);
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	comm->c_group->g_refcount++;
	(*intercomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, intercomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	if (lam_tr_comm(*intercomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);

/*
 * setup new processes
 */
	if (RPI_ADDPROCS(kgrp->g_procs, kgrp->g_nprocs)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

/*
 * Wait until all the children have initialized.  The root waits for
 * rank 0 in the child world to communicate this fact and then
 * broadcasts it to the other parents.  To make this scheme work
 * nicely with debuggers (and very short running child processes),
 * send a zero byte ACK back to the root in the child world.
 */
	if (rank == root) {
		err = MPI_Recv((void *)0, 0, MPI_BYTE, 0, 0, *intercomm, &stat);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT, err));
		}
		err = MPI_Send((void *)0, 0, MPI_BYTE, 0, 0, *intercomm);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
		}
	}

	err = MPI_Bcast((void *) 0, 0, MPI_BYTE, root, comm);
	
	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  */

	if (lam_ssi_coll_base_init_comm(*intercomm) != 0)
	  return(lam_errfunc(comm, BLKMPICOMMSPAWNMULT,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPAWNMULT, root, comm, 0, 0));
	lam_resetfunc_m(BLKMPICOMMSPAWNMULT);
	return(MPI_SUCCESS);
}

/*
 *	spawn
 *
 *	Function:	- spawn MPI processes according to app. descriptor
 *	Accepts:	- application descriptor
 *			- parent communicator
 *			- context ID for parent-child intercommunicator
 *			- array of number of processes for each command
 *			- number of children (returned)
 *			- array of child GPS (returned)
 *	Returns:	- 0 or LAMERROR
 */
static int
spawn(LIST *app, MPI_Comm comm, int cid, int *nprocs, int *numkids,
      struct _gps **kids)
{
	struct nmsg	nhead;			/* network msg header */
	struct _proc	**g;			/* process in group */
	struct _gps	*world;			/* child world GPS array */
	struct _gps	*worldout;		/* real child GPS array */
	struct _gps	*p;			/* process GPS */
	int4		rtf;			/* child runtime flags */
	int		appnum;			/* child's appl. number */
	int		rank;			/* my (spawner's) rank */
	int		parent_n;		/* size of parent world */
	int		world_n;		/* size of child world */
	int		i, j;
	struct jobid_t	jobid;
/*
 * Set environment inherited by children.  The world spawning them consists
 * solely of the parent group.
 */
	rtf = _kio.ki_rtf;
#ifdef RTF_IMPI
	rtf &= ~(RTF_TRON | RTF_FLAT | RTF_WAIT | RTF_FORKED | RTF_IMPI);
#else
	rtf &= ~(RTF_TRON | RTF_FLAT | RTF_WAIT | RTF_FORKED);
#endif

	MPI_Comm_size(comm, &parent_n);
/*
 * Allocate combined parent and child GPS array.
 */
	world_n = al_count(app);
	world = (struct _gps *)
		malloc((unsigned) (world_n + parent_n) * sizeof(struct _gps));
	if (world == 0) return(LAMERROR);
	worldout = (struct _gps *)
		malloc((unsigned) (world_n + parent_n) * sizeof(struct _gps));
	if (worldout == 0) return(LAMERROR);
/*
 * Set job identifier to be inherited by the application.
 */
	jobid.jid_node = _kio.ki_jobid.jid_node;
	jobid.jid_pid = _kio.ki_jobid.jid_pid;
	_kio.ki_jobid.jid_node = getnodeid();
	_kio.ki_jobid.jid_pid = lam_getpid();
	rtf |= RTF_MPIRUN;
/*
 * Run the application.
 */
	if (asc_run(app, parent_n, rtf, 0, 0, world)) {
		free((char *) world);
		return(LAMERROR);
	}
	_kio.ki_jobid.jid_node = jobid.jid_node;
	_kio.ki_jobid.jid_pid = jobid.jid_pid;
/*
 * Stole this code almost verbatim out of mpirun.c so that we can
 * MPI_Comm_spawn non-MPI jobs (of course, they must eventually run
 * LAM/MPI programs that call MPI_Init).
 */
	if (lam_get_mpi_world(world_n, world, worldout, 
			      "MPI_Comm_spawn_multiple")) {
		free((char *) world);
		return(LAMERROR);
	}
	free((char *) world);
/*
 * Fill in child ranks in their MPI_COMM_WORLD.
 */
	for (i = 0, p = worldout; i < world_n; ++i, ++p) {
		p->gps_grank = i;
	}
/*
 * Fill in the parent world GPS.
 */
	g = comm->c_group->g_procs;

	for (i = 0; i < parent_n; ++i, ++p, ++g) {
		*p = (*g)->p_gps;
	}
/*
 * Set up the message.
 */
	MPI_Comm_rank(comm, &rank);
	LAM_ZERO_ME(nhead);
	nhead.nh_type = 0;
	nhead.nh_flags = DINT4MSG;
	nhead.nh_msg = (char *) worldout;
	nhead.nh_length = (world_n + parent_n) * sizeof(struct _gps);
	nhead.nh_data[1] = (int4) cid;
	nhead.nh_data[2] = (int4) rank;
	nhead.nh_data[3] = (int4) lam_universe_size;
/*
 * Loop sending to each child process.
 */
	for (i = 0, j = 0, appnum = 0, p = worldout; i < world_n; 
	     ++i, ++j, ++p) {
		nhead.nh_node = p->gps_node;
		nhead.nh_event = (-p->gps_pid) & 0xBFFFFFFF;
/*
 * Set application number.
 */
		if (j >= nprocs[appnum]) {
			j = 0;
			++appnum;
		}

		nhead.nh_data[4] = (int4) appnum;

		if (nsend(&nhead)) {
			free((char *) worldout);
			return(LAMERROR);
		}
	}
	
	*numkids = world_n;
	*kids = worldout;
	return(0);
}

/*
 *	build_app
 *
 *	Function:	- build an application
 *	Accepts:	- number of command lines
 *			- array of program names
 *			- array of argument vectors
 *			- array of max. number of processes to start
 *			- array of info
 *	Returns:	- application descriptor or 0
 */
static LIST *
build_app(int count, char **commands, char ***argvs, int *maxprocs,
	  MPI_Info *infos)
{
	LIST		*app;			/* application */
	LIST		*app_sched;		/* scheduled application */
	struct apparg 	*pargv;			/* process argv */
	struct aschema	proc;			/* process list entry */
	int		argv_n;			/* number args in argv */
	int		i, j;
	struct appenv   *procenv;		/* process env */

/*
 * Create new empty application.
 */
	if ((app = al_init(sizeof(struct aschema), (int (*)()) 0)) == 0) {
		return(0);
	}
/*
 * Loop through the command lines adding them to the application.
 */
	for (i = 0; i < count; i++) {
		proc.asc_nodelist = 0;
		proc.asc_proc_cnt = maxprocs[i];
		proc.asc_srcnode = -1;
		proc.asc_errno = 0;

		/* This will get set in asc_schedule() later, but for
		 * purify reasons, we need to set it to something here 
		 */
		LAM_ZERO_ME(proc.asc_node);
/*
 * Build the argument vector.
 */
		pargv = (struct apparg *) malloc(sizeof(struct apparg));
		if (pargv == 0) {
			asc_free(app);
			return(0);
		}
		
		pargv->apa_argv = 0;
		pargv->apa_argc = 0;
		pargv->apa_refcount = 1;
/*
 * Add the command name.
 */
		if (sfh_argv_add(&pargv->apa_argc, &pargv->apa_argv, 
                                 commands[i])) {
			asc_free(app);
			sfh_argv_free(pargv->apa_argv);
			free((char *) pargv);
			return(0);
		}
/*
 * Add the argument vector for this command.
 */
		if (argvs != MPI_ARGVS_NULL) {
			argv_n = sfh_argv_count(argvs[i]);
			for (j = 0; j < argv_n; j++) {
				if (sfh_argv_add(&pargv->apa_argc, 
                                                 &pargv->apa_argv, 
						argvs[i][j])) {
					asc_free(app);
					sfh_argv_free(pargv->apa_argv);
					free((char *) pargv);
					return(0);
				}
			}
		}

		proc.asc_args = pargv;

/*
 * Build the process's env structure.
 */
		procenv = (struct appenv *) malloc(sizeof(struct appenv));
		if (procenv == 0) {
		  asc_free(app);
		  sfh_argv_free(pargv->apa_argv);
		  free((char *) pargv);
		  return(0);
		}
		
		procenv->ape_refcount = 1;
		procenv->ape_envv = 0;
		procenv->ape_wrkdir = 0;
		proc.asc_env = procenv;
/*
 * Add on LAM_MPI_* variables and whatnot.
 */
		asc_environment(1, NULL, &procenv->ape_envv);

		if (al_append(app, &proc) == 0) {
			asc_free(app);
			sfh_argv_free(pargv->apa_argv);
			free((char *) pargv);
			return(0);
		}

	}

/*
 * Schedule the application.
 */
	app_sched = asc_schedule(app);
	asc_free(app);
	
	return(app_sched);
}

/*
 *	set_error_codes
 *
 *	Function:	- set error codes for spawned processes
 *	Accepts:	- spawn error code
 *			- application
 *			- array of error codes (out)
 */
static void
set_error_codes(int err, LIST *app, int *errs, int *msg)
{
	int		i;
	struct aschema  *p;

	if (err) {
		for (i = 0, p = al_top(app); p; p = al_next(app, p), i++) {
		  if (errs != MPI_ERRCODES_IGNORE)
		    errs[i] = lam_mkerr(MPI_ERR_SPAWN, p->asc_errno);
		  msg[i] = lam_mkerr(MPI_ERR_SPAWN, p->asc_errno);
		}
	} else {
		for (i = al_count(app) - 1; i >= 0; i--) {
		  if (errs != MPI_ERRCODES_IGNORE)
		    errs[i] = MPI_SUCCESS;
		  msg[i] = MPI_SUCCESS;
		}
	}
}
