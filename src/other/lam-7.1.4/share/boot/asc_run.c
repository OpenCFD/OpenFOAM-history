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
 * $Id: asc_run.c,v 6.17 2003/11/04 04:00:49 pkambadu Exp $
 *
 *	Function:	- application execution
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <all_list.h>
#include <args.h>
#include <app_mgmt.h>
#include <app_schema.h>
#include <bfreq.h>
#include <freq.h>
#include <kio.h>
#include <laminternal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* kernel I/O block */ 

/*
 * local functions
 */
static void fdincr(int4 incr);
static void starterror(char *file, int nodeid, int prerror);

/*
 *	asc_ger
 *
 *	Function:	- sets GER buffer limits for application
 *	Accepts:	- application schema
 *			- error printing flag
 */
int
asc_ger(LIST *appd, int flag_print)
{
	int4		anode;			/* LOCAL filtered out */
	int4		nnodes;			/* # nodes in session */
	int4 *		nodes;			/* the node IDs */
	int		i;
	int *		nprocs_node;		/* # procs on each node */
	int		nprocs;			/* # procs in application */
	struct aschema *p;
	struct bfparms	bfp;			/* bufferd parameters */
/*
 * Get an array of nodeids and allocate a mirror array of process counts.
 */
	nnodes = getnall();
	nodes = (int4 *) malloc((unsigned) (nnodes * sizeof(int4)));
	if (getall(nodes, nnodes)) return(LAMERROR);
	nprocs_node = (int *) malloc((unsigned) (nnodes * sizeof(int)));
	if (nprocs_node == 0) return(LAMERROR);

	for (i = 0; i < nnodes; i++) nprocs_node[i] = 0;

	nprocs = al_count(appd);
/*
 * Tally the number of processes on each node.
 */
	p = (struct aschema *) al_top(appd);

	while (p) {
/*
 * Nodeids are not necessarily 0 to N-1, so we have to search for it.  (BFI)
 */
		for (i = 0; i < nnodes; ++i) {
			anode = (p->asc_node == LOCAL) ? getnodeid() :
					p->asc_node;

			if (anode == nodes[i]) break;
		}

		if (i >= nnodes) {
			errno = EBADNODE;

			if (flag_print) {
				fprintf(stderr, "mpirun: %s: ",
						nid_fmt(p->asc_node));
				terror("");
			}

			return(LAMERROR);
		}

		nprocs_node[i]++;
		p = (struct aschema *) al_next(appd, p);
	}
/*
 * Adjust the buffer limit on each node.
 */
	for (i = 0; i < nnodes; i++) {

		if (nprocs_node[i] == 0) continue;

		bfp.bfp_maxspace =
/*
 * the basic GER requirement
 */
			(MPI_GER * (MAXNMSGLEN + sizeof(struct nmsg)) *
			nprocs * nprocs_node[i]) +
/*
 * GER flow control messages - This one is tricky.  For processes
 * on your own node, you will only see the envelopes or the flow control
 * messages, but not max of both.  Since we have already covered local
 * processes above, we need not count them again here.
 */
			((MPI_GER - 1) * nprocs * (nprocs - nprocs_node[i]) *
			sizeof(struct nmsg)) +
/*
 * one packet for consumables to move through the buffer daemon
 */
			sizeof(struct nmsg) + MAXNMSGLEN +
/*
 * slush to cover up my mistakes
 */
			0x100000;

		bfp.bfp_maxspace = LAM_max(bfp.bfp_maxspace, BFDEFSPACE);

		if (rbfparms(nodes[i], &bfp)) {

			if (flag_print) {
				fprintf(stderr,
			    "mpirun: cannot allocate GER (%d bytes) on %s: ",
						bfp.bfp_maxspace,
						nid_fmt(nodes[i]));
				terror("");
			}

			return(LAMERROR);
		}
	}

	free((char *) nodes);
	free((char *) nprocs_node);

	return(0);
}

/*
 *	asc_run
 *
 *	Function:	- run the described application atomically
 *			- if a process cannot start, doom the running ones
 *	Accepts:	- app. schema desc.
 *			- number of parents in parent world
 *			- run-time environment flag
 *			- verbose flag
 *			- print error flag
 *			- GPS array describing running processes
 *	Returns:	- 0 or LAMERROR
 */
int
asc_run(LIST *appd, int nparent, int4 rtf, int verbose, int prerror,
	struct _gps *pgps)
{
    struct aschema 	*pp;			/* ptr process entry */
    int4		world_save;		/* saved ki_world */
    int4		parent_save;		/* saved ki_parent */
    int			i;			/* favourite index */
    int			temp_rtf;		/* temp rtf holder */
    int			pid;			/* process ID */
    int			idx;			/* process index */
    int			niod;			/* # processes using iod */
    int			remote;			/* remote process */
    int4		pfdio;			/* do IO via passed fds */
    int4		trycwd;			/* try change remote cwd */
    struct penv		env;			/* process environment */
    char		*cwd;

    if ((appd == 0) || (al_count(appd) == 0)) {
	errno = EINVAL;
	return(LAMERROR);
    }

    cwd = getworkdir();
/*
 * Set up the kernel I/O block.
 */
    world_save = _kio.ki_world;
    parent_save = _kio.ki_parent;

    _kio.ki_parent = nparent;
    _kio.ki_world = al_count(appd);

    memset((char *) pgps, 0, _kio.ki_world * sizeof(struct _gps));
/*
 * Count the number of processes using IO daemon.
 */
    niod = 0;
    if (rtf & RTF_IO) {
	for (pp = al_top(appd); pp; pp = al_next(appd, pp)) {
#if LAM_HAVE_FD_PASSING
	    if (pp->asc_node != LOCAL && pp->asc_node != getnodeid()) {
		niod++;
	    }
#else
            ++niod;
#endif
	}
    }
/*
 * Increment the stdio counts.
 */
    fdincr(niod);
/*
 * Loop starting each process.
 */
    for (pp = al_top(appd), i = 0; pp; ++i, pp = al_next(appd, pp)) {
/*
 * Standard IO is to be mapped to passed file descriptors only if the
 * process is to be run locally and passing of file descriptors
 * is suppported locally.
 */
	remote = (pp->asc_node != LOCAL && pp->asc_node != getnodeid());
#if LAM_HAVE_FD_PASSING
	pfdio = ((rtf & RTF_IO) && !remote) ? RTF_PFDIO : 0;
#else
	pfdio = 0;
#endif
/*
 * Start the process and store its pid.
 */
 	env.pe_rank = i;
 	env.pe_envv = pp->asc_env->ape_envv;
	trycwd = 0;
 	if (pp->asc_env->ape_wrkdir) {
 	    lam_strncpy(env.pe_wrkdir, pp->asc_env->ape_wrkdir, LAM_PATH_MAX);
 	} else {
	    if (remote && cwd && !(rtf & RTF_APPWD)) {
		trycwd = RTF_TRYCWD;
		lam_strncpy(env.pe_wrkdir, cwd, LAM_PATH_MAX);
	    } else {
		strcpy(env.pe_wrkdir, "");
	    }
 	}

	temp_rtf = rtf & ~(RTF_PFDIO | RTF_TRYCWD);
	if (rploadgov(pp->asc_srcnode, pp->asc_node,
		pp->asc_args->apa_argv[0], (temp_rtf | pfdio | trycwd),
	        pp->asc_args->apa_argv, &env, &pid, &idx)) {

	    pp->asc_errno = errno;
	    starterror(pp->asc_args->apa_argv[0], pp->asc_node, prerror);
	    break;
	}

	pgps[i].gps_node = (pp->asc_node == LOCAL) ? getnodeid() : pp->asc_node;
	pgps[i].gps_pid = pid;
	pgps[i].gps_idx = idx;
	pgps[i].gps_grank = i;

	if (rtf & RTF_IO) {
	  niod--;
	}

	if (verbose) {
	    printf("%d %s running on %s\n", pid, pp->asc_args->apa_argv[0],
		nid_fmt(pp->asc_node));
	    fflush(stdout);
	}
    }

    _kio.ki_world = world_save;
    _kio.ki_parent = parent_save;

    if (cwd)
	free(cwd);
/*
 * If all processes were not started, cleanup and kill the running ones.
 * This should not really be done here and we should simply return the
 * number of processes successfully started.  To help avoid a race with
 * processes entering the kernel we sleep before dooming them.
 */
    if (i < al_count(appd)) {
	fdincr(-niod);
	sleep(1);
	app_doom(i, pgps, -15);
	return(LAMERROR);
    }

    return(0);
}

/*
 *	fdincr
 *
 *	Function:	- increment stdio file desc.
 *	Accepts:	- increment value
 */
static void
fdincr(int4 incr)
{
    if (incr != 0) {
	if (_kio.ki_stdout > 2)
	    lam_rfincr(1, incr);
	if (_kio.ki_stderr > 2)
	    lam_rfincr(2, incr);
    }
}

/*
 *	starterror
 *
 *	Function:	- print can't start program error
 *	Accepts:	- file name
 *			- ID of node failed on
 *			- print error flag
 */
static void
starterror(char *file, int nodeid, int prerror)
{
    if (prerror) {
	fprintf(stderr, "mpirun: cannot start %s on %s: ",
		file, nid_fmt(nodeid));
	terror("");
    }
}
