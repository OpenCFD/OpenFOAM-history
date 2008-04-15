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
 * $Id: lamtrace.c,v 6.16 2003/02/27 03:01:11 jsquyres Exp $
 *
 *	Function:	- unloads trace data
 *	Examples:	- lamtrace n10
 *			  unloads all trace data on node 10
 *			- lamtrace n10 -k
 *			  unloads but does not forget trace data on node 10
 *			- lamtrace n10 -s
 *			  forgets all trace data on node 10
 *			- lamtrace n10 -l 2 p1020
 *			  unloads trace data only from list 2 and process 1020
 *			- lamtrace -mpi
 *			  unloads trace data for an MPI application
 *			- lamtrace -mpi -f 2
 *			  signals to flush, then delay 2 secs, then unload
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <all_opt.h>
#include <app_mgmt.h>
#include <args.h>
#include <lamdebug.h>
#include <lam_ksignal.h>
#include <mpitrace.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <t_types.h>
#include <terror.h>
#include <trreq.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * local functions
 */
static void		drain(int4 node, int4 pid, int fd);
static void		drain_mpi(int4 node, int4 pid, int fd);
static void		sweep(int4 node);

/*
 * external functions
 */
extern int		trdrain_mpi();
extern void		nodespin_end();
extern void		nodespin_init();
extern void		nodespin_next();

/*
 * global variables
 */
OPT			*op;			/* cmd line options */
int			fl_verbose;		/* be verbose? */


int
main(int argc, char *argv[])
{
	int4		n_index, n_flags;	/* node entry info */
	int4		node;			/* value of node entry */
	int4		p_index, p_flags;	/* pid entry info */
	int4		pid;			/* value of pid entry */
	int		fd = -1;		/* output file descriptor */
	int		nodec;			/* default node arguments */
	char		*fname;			/* output file name */
	char		*nodev[3];		/* default node arguments */

	/* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

/*
 * Parse command-line arguments.
 */
	op = ao_init();
	ao_setopt1(op, "hkvR", (char *) 0, 0, 0);
	ao_setopt(op, "l", (char *) 0, 1, AOINT);
	ao_setopt(op, "f", (char *) 0, 1, AOINT);
	ao_setopt(op, "mpi", "l", 0, 0);

	if (ao_parse(op, &argc, argv)) {
	  show_help("lamtrace", "usage", NULL);
		exit(errno);
	}

	if (ao_taken(op, "h")) {
	  show_help("lamtrace", "usage", NULL);
		exit(0);
	}

	fl_verbose = ao_taken(op, "v");
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "lamtrace", hostname, NULL);
	  exit(errno);
	}
/*
 * Parse nodes and processes.
 */
	if (nid_parse(&argc, argv) || pid_parse(&argc, argv) ||
			(errno = (argc > 2) ? EUSAGE : 0)) {
	  show_help("lamtrace", "usage", NULL);
		kexit(errno);
	}
/*
 * Open file if not -s.
 */
	if (!ao_taken(op, "R")) {

		fname = (argc >= 2) ? argv[1] : TRDEFFNAME;
		fd = open(fname, O_RDWR | O_CREAT | O_EXCL, 0644);

		if (fd < 0) {
			fprintf(stderr, "lamtrace (open): %s: ", fname);
			lamfail("");
		}
	}
/*
 * If no nodes are specified, default to all compute nodes.
 */
	nid_get(&n_index, &node, &n_flags);

	if (n_index < 0) {
		nodev[0] = "lamtrace";
		nodev[1] = "C";
		nodev[2] = 0;
		nodec = 2;
		nid_free();

		if (nid_parse(&nodec, nodev))
				lamfail("lamtrace (parse_nodes)");

		nid_get(&n_index, &node, &n_flags);
	}

	pid_get(&p_index, &pid, &p_flags);

	if (p_index < 0) {
		pid = TRANYPID;
	}

	if (ao_taken(op, "R")) {
		sweep(node);
	} else if (ao_taken(op, "mpi")) {
		drain_mpi(node, pid, fd);
	} else {
		drain(node, pid, fd);
	}

	if (!ao_taken(op, "R")) {
		close(fd);
	}

	kexit(0);
	return(0);
}

/*
 *	drain_mpi
 *
 *	Function:	- drains traces for one MPI world
 *			- searches for MPI world trace
 *	Accepts:	- node to confine search
 *			- pid to confine search
 *			- output file desc.
 */
static void
drain_mpi(int4 node, int4 pid, int fd)
{
	struct _gps	*world;			/* GPS array */
	int4		n_index, n_flags;	/* node entry info */
	int4		p_index, p_flags;	/* pid entry info */
	int4		curr_node;		/* current target */
	int4		r;			/* length of traces ret'd */
	int4		nworld;			/* # GPS records */
	int		flush_delay;		/* signal then delay */
	int		i;

	if (fl_verbose) nodespin_init("searching for an MPI world,");

	do {

		do {
			curr_node = node;
			if (fl_verbose) nodespin_next(node);

			if (ao_taken(op, "k")) {
				r = lam_rtrfget(node, TRWORLD, pid, fd);
				if (r < 0) lamfail("lamtrace (lam_rtrfget)");
			} else {
				r = lam_rtrfforget(node, TRWORLD, pid, fd);
				if (r < 0) lamfail("lamtrace (lam_rtrfget)");
			}

			nid_get(&n_index, &node, &n_flags);
		} while (n_index && (r == 0));

		pid_get(&p_index, &pid, &p_flags);
	} while ((p_index > 0) && (r == 0));

	if (fl_verbose) nodespin_end();

	if (r == 0) {
	  show_help("lamtrace", "nompiworld", NULL);
		return;
	}

	VERBOSE("found an MPI world on %s\n", nid_fmt(curr_node));
/*
 * Rewind and read GPS information from the file.
 */
	if (lseek(fd, (off_t) sizeof(int4), SEEK_SET) < 0)
			lamfail("lamtrace (lseek)");
		
	if (read(fd, (char *) &nworld, sizeof(int4)) < 0)
			lamfail("lamtrace (read)");

	nworld = ttol(nworld);
	VERBOSE("MPI world size is %d.\n", nworld);

	world = (struct _gps *) malloc((unsigned) (sizeof(struct _gps) *
			nworld));
	if (world == 0) lamfail("lamtrace (malloc)");
		
	if (read(fd, (char *) world, sizeof(struct _gps) * nworld) < 0)
			lamfail("lamtrace (read)");
/*
 * Convert GPS array to local byte order.
 */
	for (i = 0; i < nworld; ++i) {
		world[i].gps_node = ttol(world[i].gps_node);
		world[i].gps_pid = ttol(world[i].gps_pid);
		world[i].gps_idx = ttol(world[i].gps_idx);
		world[i].gps_grank = ttol(world[i].gps_grank);
	}
/*
 * Determine flush delay.
 */
	if (ao_taken(op, "f")) {
	    ao_intparam(op, "f", 0, 0, &flush_delay);
	} else {
	    flush_delay = -1;
	}

	VERBOSE("draining MPI traces ...\n");

	if (trdrain_mpi(fd, world, nworld, ao_taken(op, "k"), flush_delay))
			lamfail("lamtrace (trdrain_mpi)");
}

/*
 *	drain
 *
 *	Function:	- drains generic traces
 *	Accepts:	- node
 *			- pid
 *			- output file desc.
 */
static void
drain(int4 node, int4 pid, int fd)
{
	int4		list;			/* trace list # */
	int4		n_index, n_flags;	/* node entry info */
	int4		p_index, p_flags;	/* pid entry info */
	int4		r;			/* length of traces ret'd */
	int		flush_delay;		/* signal then delay */
/*
 * Determine list number.
 */
	if (ao_taken(op, "l")) {
	    ao_intparam(op, "l", 0, 0, &list);
	} else {
	    list = TRANYLIST;
	}
/*
 * Determine flush delay.
 */
	if (ao_taken(op, "f")) {
	    ao_intparam(op, "f", 0, 0, &flush_delay);
	} else {
	    flush_delay = -1;
	}

	if (flush_delay >= 0) {
	    if (fl_verbose) nodespin_init("delivering flush signal,");

	    do {

		do {
		    if (fl_verbose) nodespin_next(node);

		    if (pid == TRANYPID) {

			if (rpdoom(node, SELECT_APPL, 0, LAM_SIGTRACE))
				lamfail("lamtrace (rpdoom)");
		    } else {

			if (rpdoom(node, SELECT_PID, pid, LAM_SIGTRACE))
				lamfail("lamtrace (rpdoom)");
		    }

		    nid_get(&n_index, &node, &n_flags);
		} while (n_index);

		pid_get(&p_index, &pid, &p_flags);
	    } while (p_index > 0);

	    if (flush_delay > 0) {
		sleep((unsigned) flush_delay);
	    }

	    if (fl_verbose) nodespin_end();
	}

	if (fl_verbose) nodespin_init("draining traces,");

	do {

	    do {
		if (fl_verbose) nodespin_next(node);

		if (ao_taken(op, "k")) {
		    r = lam_rtrfget(node, list, pid, fd);
		    if (r < 0) lamfail("lamtrace (lam_rtrfget)");
		} else {
		    r = lam_rtrfforget(node, list, pid, fd);
		    if (r < 0) lamfail("lamtrace (lam_rtrfget)");
		}

		nid_get(&n_index, &node, &n_flags);
	    } while (n_index);

	    pid_get(&p_index, &pid, &p_flags);
	} while (p_index > 0);

	if (fl_verbose) nodespin_end();
}

/*
 *	sweep
 *
 *	Function:	- sweeps all traces from given nodes
 *	Accepts:	- node
 */
static void
sweep(int4 node)
{
	int4		list;			/* trace list number */
	int4		n_index, n_flags;	/* node entry info */
/*
 * Determine list number.
 */
	if (ao_taken(op, "l")) {
		ao_intparam(op, "l", 0, 0, &list);
	} else {
		list = TRANYLIST;
	}

	if (fl_verbose) nodespin_init("sweeping traces,");

	do {
		if (fl_verbose) nodespin_next(node);
		if (lam_rtrsweep(node, list))
				lamfail("lamtrace (lam_rtrsweep)");
		nid_get(&n_index, &node, &n_flags);
	} while (n_index);

	if (fl_verbose) nodespin_end();
}
