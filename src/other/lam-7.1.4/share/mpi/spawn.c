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
 * $Id: spawn.c,v 6.49 2003/09/02 20:48:58 vsahay Exp $
 *
 *	Function:	- spawn MPI program
 *	Accepts:	- program name
 *			- argument vector
 *			- max. number of processes to start
 *			- info
 *			- root in spawning communicator
 *			- spawning communicator
 *			- intercomm between parents and children (out)
 *			- array of error codes (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
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
#include <sfh.h>
#include <terror.h>
#include <typical.h>
#include <lamdebug.h>
#include <lam_config.h>

/*
 * private functions
 */
static int              spawn(LIST *app, MPI_Comm comm, int cid, int *numkids,
			      struct _gps **kids);
static int		build_app(MPI_Comm comm, char *command, char **argv,
				  int maxprocs, MPI_Info info, LIST **app);
static void             set_error_codes(int err, LIST *app, int *errs);
static char 		*locate_aschema(char *filename);
static int              get_sched_rrobin(struct aschema *proc, char *value,
					 int maxprocs, int flag);
static int              find_cpu_node(struct route *routes, int num_routes,
				      int cpu);


/*@
   MPI_Comm_spawn - Spawn a dynamic MPI process

Input Parameters:
+ command - Name of program to spawn (only significant at root)
. argv - arguments to command (only significant at root)
. maxprocs - max number of processes to start (only significant at root)
. info - startup hints
. root - rank of process to perform the spawn
- comm - parent intracommunicator

Output Parameters:
+ intercomm - child intercommunicator containing spawned processes
- errcodes - one code per process

Description:

A group of processes can create another group of processes with
'MPI_Comm_spawn'.  This function is a collective operation over the
parent communicator.  The child group starts up like any MPI
application.  The processes must begin by calling 'MPI_Init', after
which the pre-defined communicator, 'MPI_COMM_WORLD', may be used.
This world communicator contains only the child processes.  It is
distinct from the 'MPI_COMM_WORLD' of the parent processes.
 
'MPI_Comm_spawn_multiple' is used to manually specify a group of
different executables and arguments to spawn.  'MPI_Comm_spawn' is
used to specify one executable and set of arguments (although a
LAM/MPI appschema(5) can be provided to 'MPI_Comm_spawn' via the
"lam_file" info key).
 
Communication With Spawned Processes

The natural communication mechanism between two groups is the
intercommunicator.  The second communicator argument to
'MPI_Comm_spawn' returns an intercommunicator whose local group
contains the parent processes (same as the first communicator
argument) and whose remote group contains child processes. The child
processes can access the same intercommunicator by using the
'MPI_Comm_get_parent' call.  The remote group size of the parent
communicator is zero if the process was created by 'mpirun'(1) instead
of one of the spawn functions.  Both groups can decide to merge the
intercommunicator into an intracommunicator (with the
'MPI_Intercomm_merge' function) and take advantage of other MPI
collective operations.  They can then use the merged intracommunicator
to create new communicators and reach other processes in the MPI
application.
 
Resource Allocation

LAM/MPI offers some MPI_Info keys for the placement of spawned
applications.  Keys are looked for in the order listed below.  The
first key that is found is used; any remaining keys are ignored.

'lam_spawn_file'

The value of this key can be the filename of an appschema(1).  This
allows the programmer to specify an arbitrary set of LAM CPUs or nodes
to spawn MPI processes on.  In this case, only the appschema is used
to spawn the application; 'command', 'argv', and 'maxprocs' are all
ignored (even at the root).  Note that even though 'maxprocs' is
ignored, 'errcodes' must still be an array long enough to hold an
integer error code for every process that tried to launch, or be the
MPI constant 'MPI_ERRCODES_IGNORE'.  Also note that
'MPI_Comm_spawn_multiple' does `not` accept the "lam_spawn_file" info
key.  As such, the "lam_spawn_file" info key to 'MPI_Comm_spawn' is
mainly intended to spawn MPMD applications and/or specify an arbitrary
number of nodes to run on.

Also note that this "lam_spawn_file" key is `not` portable to other
MPI implementations; it is a LAM/MPI-specific info key.  If specifying
exact LAM nodes or CPUs is not necessary, users should probably use
'MPI_Comm_spawn_multiple' to make their program more portable.

'file'

This key is a synonym for "lam_spawn_file".  Since "file" is not a
LAM-specific name, yet this key carries a LAM-specific meaning, its
use is deprecated in favor of "lam_spawn_file".

'lam_spawn_sched_round_robin'

The value of this key is a string representing a LAM CPU or node
(using standard LAM nomenclature -- see mpirun(1)) to begin spawning
on.  The use of this key allows the programmer to indicate which
node/CPU for LAM to start spawning on without having to write out a
temporary app schema file.

The CPU number is relative to the boot schema given to
lamboot(1).  Only a single LAM node/CPU may be specified, such as "n3"
or "c1".  If a node is specified, LAM will spawn one MPI process per
node.  If a CPU is specified, LAM will scedule one MPI process per
CPU.  An error is returned if "N" or "C" is used.

Note that LAM is not involved with run-time scheduling of the MPI
process -- LAM only spawns processes on indicated nodes.  The
operating system schedules these processes for executation just like
any other process.  No attempt is made by LAM to bind processes to
CPUs.  Hence, the "cX" nomenclature is just a convenicence mechanism
to inidicate how many MPI processes should be spawned on a given node;
it is not indicative of operating system scheduling.

For "nX" values, the first MPI process will be spawned on the
indicated node.  The remaining (maxprocs - 1) MPI processes will be
spawned on successive nodes.  Specifically, if X is the starting node
number, process i will be launched on "nK", where K = ((X + i) %
total_nodes).  LAM will modulus the node number with the total number
of nodes in the current LAM universe to prevent errors, thereby
creating a "wraparound" effect.  Hence, this mechanism can be used for
round-robin scheduling, regardless of how many nodes are in the LAM
universe.

For "cX" values, the algorithm is essentially the same, except that
LAM will resolve "cX" to a specific node before spawning, and
successive processes are spawned on the node where "cK" resides, where
K = ((X + i) % total_cpus).

For example, if there are 8 nodes and 16 CPUs in the current LAM
universe (2 CPUs per node), a "lam_spawn_sched_round_robin" key is
given with the value of "c14", and maxprocs is 4, LAM will spawn MPI
processes on:

.vb
CPU  Node  MPI_COMM_WORLD rank
---  ----  -------------------
c14  n7    0
c15  n7    1
c0   n0    2
c1   n0    3
.ve

'lam_no_root_node_schedule'

This key is used to designate that the spawned processes must not be 
spawned or scheduled on the "root node" (the node doing the spawn). There 
is no specific value associated with this key, but it should be given some 
non-null/non-empty dummy value. 

It is a node-specific key and not a CPU-specific one. Hence if the root node
has multiple CPUs, none of the CPUs on this root node will take part in the 
scheduling of the spawned processes.

No keys given

If none of the info keys listed above are used, the value of
'MPI_INFO_NULL' should be given for 'info' (all other keys are
ignored, anyway - there is no harm in providing other keys).  In this
case, LAM schedules the given number of processes onto LAM nodes by
starting with CPU 0 (or the lowest numbered CPU), and continuing
through higher CPU numbers, placing one process on each CPU.  If the
process count is greater than the CPU count, the procedure repeats.

Predefined Attributes

The pre-defined attribute on 'MPI_COMM_WORLD', 'MPI_UNIVERSE_SIZE',
can be useful in determining how many CPUs are currently unused.  For
example, the value in 'MPI_UNIVERSE_SIZE' is the number of CPUs that
LAM was booted with (see MPI_Init(1)).  Subtracting the size of
'MPI_COMM_WORLD' from this value returns the number of CPUs in the
current LAM universe that the current application is not using (and
are therefore likely not being used).

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

The 'maxprocs' parameter to 'MPI_Comm_spawn' specifies the exact
number of processes to be started.  If it is not possible to start the
desired number of processes, 'MPI_Comm_spawn' will return an error
code.  Note that even though 'maxprocs' is only relevant on the root,
all ranks must have an 'errcodes' array long enough to handle an
integer error code for every process that tries to launch, or give MPI
constant 'MPI_ERRCODES_IGNORE' for the 'errcodes' argument.  While
this appears to be a contradiction, it is per the MPI-2 standard.  :-\
 
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

The 'argv' parameter to 'MPI_Comm_spawn' should not contain the
program name since it is given in the first parameter.  The command
line that is passed to the newly launched program will be the program
name followed by the strings in 'argv'.

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_SPAWN
.N MPI_ERR_ARG
.N MPI_ERR_ROOT
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN
.N MPI_ERR_NO_MEM

.seealso appschema(5), bhost(5), lamboot(1), MPI_Comm_get_parent(3), MPI_Intercomm_merge(3), MPI_Comm_spawn_multiple(3), MPI_Info_create(3), MPI_Info_set(3), MPI_Info_delete(3), MPI_Info_free(3), MPI_Init(3), mpirun(1)

.N ACK
@*/
int
MPI_Comm_spawn(char* command, char** argv, int maxprocs, MPI_Info info, 
	       int root, MPI_Comm comm, MPI_Comm *intercomm, 
	       int *errcodes)
{
	MPI_Group	kgrp;			/* child group */
	LIST		*app;			/* application */
	struct _gps	*kids;			/* array of child GPS */
	struct _gps	*g;
	struct _proc	**p;
	int		rank;			/* caller rank */
	int		size;			/* group size */
	int		err;			/* error code */
	int		numkids;		/* num. of children spawned */
	int		mycid;			/* local max context ID */
	int		cid;			/* context ID for intercomm */
	int		msg[3];			/* message buffer */
	int		*ecodes = errcodes;	/* error code array */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMSPAWN);
/*
 * Check the arguments.
 */
	if ((comm == MPI_COMM_NULL) || LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
					lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	MPI_Comm_size(comm, &size);

	if ((root >= size) || (root < 0)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	}

	if (intercomm == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMSPAWN, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

#if LAM_WANT_IMPI
	if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, ENOSYS,
			      "MPI_Comm_spawn not possible on IMPI communicators");
	}
#endif

	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMSPAWN));
/*
 * Set debugging parameters.
 */
	g = &(comm->c_group->g_procs[root]->p_gps);

	lam_setparam(BLKMPICOMMSPAWN, root | (g->gps_grank << 16),
				(g->gps_node << 16) | g->gps_idx);
/*
 * Synchronize all members of the parent group and get the context ID
 * for the parent-child intercommunicator.
 */
	MPI_Comm_rank(comm, &rank);

	mycid = lam_getcid();

	if (mycid < 0) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_INTERN, EFULL)));
	}

	err = MPI_Reduce(&mycid, &cid, 1, MPI_INT, MPI_MAX, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	}

	if (rank == root) {
/*
 * The root does the process spawning.
 */

		err = build_app(comm, command, argv, maxprocs, info, &app);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
		}

		if (spawn(app, comm, cid, &numkids, &kids)) {
			err = lam_mkerr(MPI_ERR_SPAWN, EINVAL);
		}

		if (err != MPI_SUCCESS && errcodes == MPI_ERRCODES_IGNORE) {
			ecodes = malloc(numkids*sizeof(int));
			if (!ecodes)
				return(lam_errfunc(comm, BLKMPICOMMSPAWN,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		set_error_codes(err, app, ecodes);
		asc_free(app);
/*
 * Inform parent group of spawn status.
 */
		msg[0] = numkids;
		msg[1] = cid;
		msg[2] = err;
		if (err == MPI_SUCCESS) {
			err = MPI_Bcast(msg, 3, MPI_INT, root, comm);
			if (err != MPI_SUCCESS)
				return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
		} else {
			MPI_Bcast(msg, 3, MPI_INT, root, comm);
			MPI_Bcast(ecodes, numkids, MPI_INT, root, comm);
			if (errcodes == MPI_ERRCODES_IGNORE)
				free(ecodes);
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
		}
	} else {
/*
 * Get spawn status from root.
 */
		err = MPI_Bcast(msg, 3, MPI_INT, root, comm);
		if (err != MPI_SUCCESS)
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));

		numkids = msg[0];
		cid = msg[1];
		err = msg[2];

		if (err != MPI_SUCCESS) {
			if (errcodes == MPI_ERRCODES_IGNORE) {
				ecodes = malloc(numkids*sizeof(int));
				if (!ecodes)
				    return(lam_errfunc(comm, BLKMPICOMMSPAWN,
					    lam_mkerr(MPI_ERR_OTHER, errno)));
			}

			MPI_Bcast(ecodes, numkids, MPI_INT, root, comm);
			if (errcodes == MPI_ERRCODES_IGNORE)
				free(ecodes);
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
		} else {
			if (errcodes != MPI_ERRCODES_IGNORE) {
				for (i = 0; i < numkids; i++)
					errcodes[i] = MPI_SUCCESS;
			}
		}
/*
 * Allocate buffer to receive array of child GPS.
 */
		kids = (struct _gps *)
			malloc((unsigned) (numkids * sizeof(struct _gps)));
		if (kids == 0)
			return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Broadcast the array of child GPS to parent group.
 */
	err = MPI_Bcast(kids, numkids * sizeof(struct _gps) / sizeof(int),
			MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		free(kids);
		return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	}

/*
 * Create the child group.
 */
	kgrp = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(numkids * sizeof(struct _proc *)));
	if (kgrp == 0) {
		free(kids);
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	kgrp->g_nprocs = numkids;
	kgrp->g_myrank = MPI_UNDEFINED;
	kgrp->g_refcount = 1;
	kgrp->g_f77handle = -1;
	kgrp->g_procs = (struct _proc **)((char *)kgrp + sizeof(struct _group));

	g = kids;
	p = kgrp->g_procs;

	for (i = 0; i < numkids; ++i, ++p, ++g) {

		if ((*p = lam_procadd(g)) == 0) {
			free((char *) kids);
			free((char *) kgrp);
			return(lam_errfunc(comm, BLKMPICOMMSPAWN,
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
        if ((rank == root) &&
            (lam_match_ssi_modules(numkids, kids, "MPI_Comm_spawn")) != 0) {
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
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	comm->c_group->g_refcount++;
	(*intercomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, intercomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	if (lam_tr_comm(*intercomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);

/*
 * Setup the new processes.
 */
	if (RPI_ADDPROCS(kgrp->g_procs, kgrp->g_nprocs)) {
		return(lam_errfunc(comm, BLKMPICOMMSPAWN,
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
		err = MPI_Recv((void *)0, 0, MPI_BYTE, 0, 0, *intercomm, 
			       MPI_STATUS_IGNORE);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
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
	  return(lam_errfunc(comm, BLKMPICOMMSPAWN,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPAWN, root, comm, 0, 0));
	lam_resetfunc_m(BLKMPICOMMSPAWN);
	return(MPI_SUCCESS);
}

/*
 *	spawn
 *
 *	Function:	- spawn MPI processes according to app. descriptor
 *	Accepts:	- application descriptor
 *			- parent communicator
 *			- context ID for parent-child intercommunicator
 *			- number of children (returned)
 *			- array of child GPS (returned)
 *	Returns:	- 0 or LAMERROR
 */
static int
spawn(LIST *app, MPI_Comm comm, int cid, int *numkids, struct _gps **kids)
{
	struct nmsg	nhead;			/* network msg header */
	struct _proc	**g;			/* process in group */
	struct _gps	*world;			/* child world GPS array */
	struct _gps	*worldout;		/* real child GPS array */
	struct _gps	*p;			/* process GPS */
	int4		rtf;			/* child runtime flags */
	int		rank;			/* my (spawner's) rank */
	int		parent_n;		/* size of parent world */
	int		world_n;		/* size of child world */
	int		i;
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
	*numkids = world_n = al_count(app);
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
			      "MPI_Comm_spawn")) {
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
	nhead.nh_data[4] = 0;			/* application number */
/*
 * Loop sending to each child process.
 */
	for (i = 0, p = worldout; i < world_n; ++i, ++p) {
		nhead.nh_node = p->gps_node;
		nhead.nh_event = (-p->gps_pid) & 0xBFFFFFFF;
		if (nsend(&nhead)) {
			free((char *) world);
			return(LAMERROR);
		}
	}

	*kids = worldout;
	return(0);
}

/*
 *	build_app
 *
 *	Function:	- build an application
 *	Accepts:	- communicator
 *			- program name
 *			- argument vector
 *			- max. number of processes to start
 *			- info
 *			- application descriptor (out)
 *	Returns:	- MPI_SUCCESS or MPI error class
 */
static int
build_app(MPI_Comm comm, char *command, char **argv, int maxprocs, 
	  MPI_Info info, LIST **app)
{
    LIST		*app_sched;		/* scheduled application */
    struct apparg 	*procargv;		/* process argv */
    struct aschema	proc;			/* process list entry */
    char		*aschema;		/* application schema */
    int			argv_n;			/* number args in argv */
    int			flag = 0;
    int			err;
    int			ignore;
    int			i;
    char		schema[MPI_MAX_OBJECT_NAME];
    char		value[MPI_MAX_OBJECT_NAME];
    struct appenv       *procenv;		/* process env */
    char                **env = 0;
    void 		**attrp;     
    int 		found;
    int 		max_nodes;
    int 		*nodes = 0;
    int 		mynodeid;
    int 		mynodeindex;
    int 		actual_mynodetype;
    struct route 	*routes = 0;
    int 		sched_round_robin;
    int 		no_root_node_schedule;
  
    
    if (info) {
	err = MPI_Info_get(info, "lam_spawn_file", MPI_MAX_OBJECT_NAME, 
			   schema, &flag);
	if (err != MPI_SUCCESS) {
	    return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	}

	if (flag == 0) {
	  err = MPI_Info_get(info, "file", MPI_MAX_OBJECT_NAME, schema, 
			     &flag);
	  if (err != MPI_SUCCESS) {
	    return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	  }
	}
    }
    
    if (flag) {
	if ((aschema = locate_aschema(schema)) == 0) {
	    return(lam_err_comm(comm, MPI_ERR_ARG, 0,
			"couldn't find application schema"));
	}

	asc_environment(1, NULL, &env);
	*app = asc_parse(aschema, &ignore, env);
	sfh_argv_free(env);
	free(aschema);
    }
    else {
        /* Initially set asc_nodelist to 0 (the application scheduling
	   expands the zero node list into all nodes).  It may get
	   overridden below, with an info key. */
      
        proc.asc_nodelist = 0;
	proc.asc_proc_cnt = maxprocs;
	proc.asc_srcnode = -1;
	proc.asc_errno = 0;

	/* Look for the lam_no_root_node_schedule key */

	no_root_node_schedule = 0;
	if (info != NULL) {
	  err = MPI_Info_get(info, "lam_no_root_node_schedule", 
			     MPI_MAX_OBJECT_NAME, value, 
			     &no_root_node_schedule);

	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	}

        /* Look for the key lam_spawn_sched_round_robin */
	sched_round_robin = 0;
	if (info != NULL) {
	  err = MPI_Info_get(info, "lam_spawn_sched_round_robin", 
			     MPI_MAX_OBJECT_NAME, value, &sched_round_robin);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPICOMMSPAWN, err));
	  
	  if (sched_round_robin == 1) {
	    get_sched_rrobin(&proc, value, maxprocs, no_root_node_schedule);
	  }
	}
	
	/* This will get set in asc_schedule() later, but for
	 * purify reasons, we need to set it to something here 
	 */

	LAM_ZERO_ME(proc.asc_node);

	/*
	 * Build the argument vector.
	 */
	procargv = (struct apparg *) malloc(sizeof(struct apparg));
	if (procargv == 0) {
	  return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	procargv->apa_argv = 0;
	procargv->apa_argc = 0;
	procargv->apa_refcount = 1;

	if (sfh_argv_add(&procargv->apa_argc, &procargv->apa_argv, command)) {
	  sfh_argv_free(procargv->apa_argv);
	  free((char *) procargv);
	  return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	if (argv != MPI_ARGV_NULL) {
	  argv_n = sfh_argv_count(argv);
	  for (i = 0; i < argv_n; i++) {
	    if (sfh_argv_add(&procargv->apa_argc,
			     &procargv->apa_argv, argv[i])) {
	      sfh_argv_free(procargv->apa_argv);
	      free((char *) procargv);
	      return(lam_mkerr(MPI_ERR_OTHER, errno));
	    }
	  }
	}

	proc.asc_args = procargv;

/*
 * Build the process's env structure.
 */
	procenv = (struct appenv *) malloc(sizeof(struct appenv));
	if (procenv == 0) {
	  sfh_argv_free(procargv->apa_argv);
	  free((char *) procargv);
	  return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	procenv->ape_refcount = 1;
	procenv->ape_envv = 0;
	procenv->ape_wrkdir = 0;
	proc.asc_env = procenv;
/*
 * Add on LAM_MPI_* variables and whatnot.
 */
	asc_environment(1, NULL, &procenv->ape_envv);
/*
 * Create a new application.
 */
	*app = al_init(sizeof(struct aschema), (int (*)()) 0);
	if (*app == 0) {
	  sfh_argv_free(procargv->apa_argv);
	  free((char *) procargv);
	  free((char *) procenv);
	  return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
	
	if (al_append(*app, &proc) == 0) {
	  asc_free(*app);
	  sfh_argv_free(procargv->apa_argv);
	  free((char *) procargv);
	  free((char *) procenv);
	  return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	/* 
	 * VPS: Make root node non-schedulable in the lamd route info
	 */

	if ((!sched_round_robin) && (no_root_node_schedule)) {	 

	  MPI_Comm_get_attr(MPI_COMM_WORLD, LAM_UNIVERSE_NNODES, &attrp,
			    &found);
	  if (found == 0)
	    return lam_mkerr(MPI_ERR_INTERN, ENOENT);

	  max_nodes = (int) *attrp;

	  routes = (struct route *) malloc((unsigned) max_nodes *
					   sizeof(struct route));
	  if (routes == NULL) {
	    return lam_mkerr(MPI_ERR_NO_MEM, errno);
	  }
    
	  nodes = (int *) malloc(sizeof(int) * max_nodes);
	  if (nodes == NULL) {
	    return lam_mkerr(MPI_ERR_NO_MEM, errno);
	  }
  
	  getnodes(nodes, max_nodes, 0, NT_CAST);
	  mynodeid = getnodeid();
	  for (i = 0; i < max_nodes; ++i) {
	    routes[i].r_nodeid = nodes[i];
	    getrent(&routes[i]);
	    if (nodes[i] == mynodeid) {
	      actual_mynodetype = routes[i].r_nodetype; 
	      mynodeindex = i;
	      routes[i].r_nodetype = NT_WASTE;	
	    }
	  }
    
	  if (rrsetrents(mynodeid, routes, max_nodes))
	    lampanic("spawn (rrsetrents)");
	}
    }
    /*
     * Schedule the application.
     */
    app_sched = asc_schedule(*app);
    asc_free(*app);
	  
    /* 
     * If we didn't use an app schema, make root back to whatever its
     * nodetype was
     */

    if (!flag) {
        if ((!sched_round_robin) && (no_root_node_schedule)) {	 
            routes[mynodeindex].r_nodetype = actual_mynodetype;
            
            if (rrsetrents(mynodeid, routes, max_nodes))
                lampanic("spawn (rrsetrents)");
            
            free((char *) routes);
        }
    }

    *app = app_sched;
    return((*app) ? MPI_SUCCESS : lam_mkerr(MPI_ERR_OTHER, errno));
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
set_error_codes(int err, LIST *app, int *errs)
{
	int		i;
	struct aschema  *p;

	if (errs == 0)
		return;

	if (err) {
		for (i = 0, p = al_top(app); p; p = al_next(app, p), i++) {
		    errs[i] = lam_mkerr(MPI_ERR_SPAWN, p->asc_errno);
		}
	} else {
		for (i = al_count(app) - 1; i >= 0; i--) {
		    errs[i] = MPI_SUCCESS;
		}
	}
}

/*
 *	locate_aschema
 *
 *	Function:	- locate an application schema
 *	Accepts:	- filename
 *	Returns:	- full pathname or NULL
 */
static char *
locate_aschema(char *filename)
{
	int		pathc = 0;		/* paths argc */
	char		**pathv = 0;		/* paths argv */
	char		*appdir;		/* application dir */
	char		*fullpath;		/* full pathname */


	if ((appdir = getenv("LAMAPPLDIR"))) {
		sfh_argv_add(&pathc, &pathv, appdir);
	}
	sfh_argv_add(&pathc, &pathv, "");
	sfh_argv_add(&pathc, &pathv, "$LAMHOME/etc");
	sfh_argv_add(&pathc, &pathv, "$TROLLIUSHOME/etc");

	fullpath = sfh_path_find(filename, pathv, R_OK);
	sfh_argv_free(pathv);
	return(fullpath);
}


static int
get_sched_rrobin(struct aschema* proc, char* value, int maxprocs, int flag)
{
  void **attrp;
  int i, j, start, num_left, max_cpus, max_nodes, found, node;
  struct ndi n;
  int *nodes = 0;
  struct route *routes = 0;
  int mynodeid;
  
  /* If we got a negative starting number, ignore everything else */

  if (value[0] != 'n' && value[0] != 'c')
    return lam_mkerr(MPI_ERR_ARG, EINVAL);
  start = atoi(value + 1);
  if (start < 0)
    return MPI_SUCCESS;
  
  /* Get the max number of CPUs and nodes.  These predefined
     attributes on MPI_COMM_WORLD -- LAM_UNIVERSIZE_NCPUS, and
     LAM_UNIVERSE_NNODES, respectively.  Note that we have to do the
     double dereferencing scheme (i.e., use "void **attrp" which is
     later dereferenced and cast to an int) because the initial value
     may be stored as a 64 bit quantity, but ints may only be 32 bits.
     Hence, we have to *treat* it like a 64 bit quantity when we
     dereference it, and then cast it down to 32 bit before we use it.
     Ugh! */

  MPI_Comm_get_attr(MPI_COMM_WORLD, LAM_UNIVERSE_NCPUS, &attrp, &found);
  if (found == 0)
    return lam_mkerr(MPI_ERR_INTERN, ENOENT);
  max_cpus = (int) *attrp;
  MPI_Comm_get_attr(MPI_COMM_WORLD, LAM_UNIVERSE_NNODES, &attrp, &found);
  if (found == 0)
    return lam_mkerr(MPI_ERR_INTERN, ENOENT);
  max_nodes = (int) *attrp;
  
  /* Initialize the list */
  
  proc->asc_nodelist = al_init(sizeof(struct ndi), (int (*)()) 0);
  if (proc->asc_nodelist == 0)
    return lam_mkerr(MPI_ERR_NO_MEM, errno);

  /* Get the routing table because it has two things that we need: 1)
     number of CPUs per node, 2) whether each node is scheduleable or
     not. */

  nodes = malloc(sizeof(int) * max_nodes);
  if (nodes == NULL) {
    free(proc->asc_nodelist);
    return lam_mkerr(MPI_ERR_NO_MEM, errno);
  }
  routes = malloc(sizeof(struct route) * (max_nodes));
  if (routes == NULL) {
    free(nodes);
    free(proc->asc_nodelist);
    return lam_mkerr(MPI_ERR_NO_MEM, errno);
  }

  /* Query the lamd to get route information */

  mynodeid = getnodeid();
  getnodes(nodes, max_nodes, 0, NT_CAST);
  for (i = 0; i < max_nodes; ++i) {
    routes[i].r_nodeid = nodes[i];
    getrent(&routes[i]);

    /* VPS: If no-root-node-schedule, then make it non-scheduleable in the local
       copy of routes */
    
    if ((flag) && (nodes[i] == mynodeid)) {
      routes[i].r_nodetype = NT_WASTE;
    }
  }
  free(nodes);

  /* Do a quick check and ensure that there's at least one schedulable
     node. */

  for (i = 0; i < max_nodes; ++i)
    if ((routes[i].r_nodetype & NT_WASTE) == 0)
      break;
  if (i >= max_nodes)
    return lam_mkerr(MPI_ERR_SPAWN, -1);

  /* Make and add entries to the list */
  
  n.ndi_flags = 0;
  n.ndi_count = 1;

  /* If we're scheduling by node, find the starting node, and count
     upwards (wrapping around, of course) until we hit maxprocs.
     Ensure that none of the nodes that we select are marked as
     "no-schedule". */
  /* NOTE: We essentially duplicate some of the logic twice: once for
     schedule-by-node, and once for schedule-by-CPU.  There's two
     loops either way you do this, and some code will have to be
     duplicated.  Command decision: make the add-to-list code be
     duplicated, and keep the schedule-by-[node|CPU] code separate for
     simplicity. */

  if (value[0] == 'n') {
    node = start % max_nodes;
    for (i = 0; i < maxprocs; ++i) {

      /* Find a node that is schedulable */

      while ((routes[node].r_nodetype & NT_WASTE) != 0)
        node = (node + 1) % max_nodes;
      n.ndi_node = node;

      /* Fill in the rest of the entry and add it to the list */

      n.ndi_index = node;
      if (al_append(proc->asc_nodelist, &n) == 0) {
        al_free(proc->asc_nodelist);
        proc->asc_nodelist = 0;
        return lam_mkerr(MPI_ERR_NO_MEM, ENOMEM);
      }

      /* Advance the node count */

      node = (node + 1) % max_nodes;
    }
  }

  /* We're scheduling by CPU.  Take the starting CPU and use it to
     find a schedulable node.  Then increment by the number of CPUs on
     that node and move on to the next node.  Repeat. */

  else {
    node = find_cpu_node(routes, max_nodes, start % max_cpus); 
    
    /* Based on the starting CPU number, find out how many CPUs are
       left on the starting node */

    for (num_left = i = 1; 
         (start + i) < max_cpus &&
           node == find_cpu_node(routes, max_nodes, (start + i) % max_cpus);
         ++i, ++num_left)
      continue;

    for (i = 0; i < maxprocs; ) {

      /* Find a node that is schedulable */

      while ((routes[node].r_nodetype & NT_WASTE) != 0)
        node = (node + 1) % max_nodes;

      /* Fill in the rest of the entry and add it to the list.  Note
         that the first time through this loop (i.e., i == 0), we may
         use less than all availabe CPUs on a given node, based on the
         starting CPU number that was given by the caller.  This is
         what num_left is used for.  In successive iterations, we will
         use (at most) all available CPUs on a node. */

      if (num_left == -1)
        num_left = routes[node].r_ncpus;
      for (j = 0; j < num_left && i < maxprocs; ++j, ++i) {
        n.ndi_node = n.ndi_index = node;
        if (al_append(proc->asc_nodelist, &n) == 0) {
          al_free(proc->asc_nodelist);
          proc->asc_nodelist = 0;
          return lam_mkerr(MPI_ERR_NO_MEM, ENOMEM);
        }
      }
      num_left = -1;

      /* Advance the node count */

      node = (node + 1) % max_nodes;
    }
  }

  /* Clean up */

  if (routes != 0)
    free(routes);

  return MPI_SUCCESS;
}


static int
find_cpu_node(struct route *routes, int num_routes, int cpu)
{
  int i;

  /* Figure out which node a given CPU is on */

  for (i = 0; i < num_routes; ++i) {
    if (routes[i].r_ncpus > cpu)
      return i;
    cpu -= routes[i].r_ncpus;
  }

  return (LAMERROR);
}

