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
 * $Id: loadgo.c,v 6.19 2003/04/29 00:55:14 brbarret Exp $
 *
 *	Function:	- loads files into nodes using loadd
 *			- loads runtime arguments into nodes using flatd
 *			- creates loaded files as processes
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell Research
 *			  Foundation
 */

#include <lam_config.h>

#include <sys/param.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <all_opt.h>
#include <args.h>
#include <freq.h>
#include <kio.h>
#include <laminternal.h>
#include <lreq.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * local functions
 */
static int		pwait();
static int		set_stdio();
static void		reset_stdio();
static void		fdincr();
static void		help();
static void		setdefaults();

/*
 * external variables
 */
extern int		lam_argtailc;	/* # of arguments to processes */
extern char		**lam_argtailv;	/* ptrs to arguments */
extern struct kio_t	_kio;
extern struct fclient	_ufd[FUMAX];	/* user file desc. */

/*
 * local variables
 */
static int4		rtflags;	/* process runtime flags */
static int4		srcnode;	/* source node ID */

static char		*usage =
	"loadgo [-fhvw] [-s <srcnode>] <nodes> <programs>\n";


int
main(int argc, char **argv)
{
    char		*tmp_argv[2];		/* scratch argv structure */
    char		**proc_argv;		/* process argv structure */
    int			i;
    int			index;			/* parsed node index */
    int			nflags;			/* parsed node flags */
    int4		idx;			/* created process index */
    int4		loadnode;		/* node to load code from */
    int4		pid;			/* created process ID */
    int4		nodeid;			/* destination node */
    int4		niod;			/* # processes using iod */
    int4		nloadgo;		/* # distinct loadgos */
    int4		pfdio;			/* do IO via passed fds */
/*
 * Parse the command line.
 */
    validopts("fhtvw");
    followed("s");

    if (do_args(&argc, argv)) {
	fprintf(stderr, usage);
	exit(errno);
    }
/*
 * Check for help request.
 */
    if (opt_taken('h')) {
	help();
	exit(0);
    }
/*
 * Set the operating parameters.
 */
    setdefaults();
/*
 * Attach to kernel.
 */
    if (kinit(PRCMD)) {
      char hostname[MAXHOSTNAMELEN];
      gethostname(hostname, MAXHOSTNAMELEN);

      show_help(NULL, "no-lamd", "loadgo", hostname, NULL);
      exit(errno);
    }

    if (nid_parse(&argc, argv)) {
	fprintf(stderr, usage);
	kexit(errno);
    }

    nid_get(&index, &nodeid, &nflags);
    ++argv;
    --argc;

    if ((index < 0) || (argc == 0)) {
	fprintf(stderr, usage);
	kexit(EUSAGE);
    }
/*
 *  Set application stdio.
 */
    if (set_stdio()) lamfail("loadgo (set_stdio)");

    _kio.ki_jobid.jid_node = getnodeid();
    _kio.ki_jobid.jid_pid = lam_getpid();
    _kio.ki_world = getncomp();
    _kio.ki_parent = 0;
/*
 * Count the number of processes using IO daemon.
 */
    niod = 0;
    if (rtflags & RTF_IO) {
	do {
	    if (!(nodeid == LOCAL || nodeid == getnodeid())) {
		niod += nid_count();
	    }
	    nid_get(&index, &nodeid, &nflags);
	} while (index);

	niod *= argc;
    }
/*
 * Set file descriptor count.
 */
    if (niod > 0) {
	fdincr(niod);
    }
/*
 * Load every file onto every node.
 */
    nloadgo = 0;

    for (i = 0; i < argc; ++i) {
/*
 * Every new process gets the same arguments.
 */
	if (lam_argtailc) {
	    proc_argv = lam_argtailv - 1;
	    *proc_argv = argv[i];
	} else {
	    proc_argv = tmp_argv;
	    tmp_argv[0] = argv[i];
	    tmp_argv[1] = 0;
	}

	do {
	    if (nodeid == LOCAL) {
		nodeid = getnodeid();
	    }
/*
 * Standard IO is to be mapped to passed file descriptors only if the
 * process is to be run locally and passing of files descriptors
 * is suppported locally.
 */
#if LAM_HAVE_FD_PASSING
	    pfdio = ((rtflags & RTF_IO) && (nodeid == getnodeid()))
		? RTF_PFDIO : 0;
#else
	    pfdio = 0;
#endif
	    loadnode = opt_taken('s') ? srcnode : nodeid;
/*
 * Remotely load and execute the current program on the current node.
 */
	    if (rploadgo(loadnode, nodeid, argv[i], rtflags|pfdio,
					proc_argv, &pid, &idx)) {

		terror("loadgo (rploadgo)");
/*
 * Wait for processes if requested.
 */
		if (pwait(nloadgo)) terror("loadgo (pwait)");
/*
 * Decrement file descriptor counts accordingly.
 */
		if (niod > 0) {
		    fdincr(-niod);
		}

		reset_stdio();
		kexit(errno);
	    }

	    if ((rtflags & RTF_IO) && (nodeid != getnodeid())) {
		niod -= nid_count();
	    }

	    nloadgo += 1;

	    if (opt_taken('v')) {
		printf("[%d] p%d %s running on %s\n",
		    idx, pid, argv[i], nid_fmt(nodeid));
	    }

	    nid_get(&index, &nodeid, &nflags);
	} while (index);
    }
/*
 * Wait for the process(es) to terminate.
 */
    if (pwait(nloadgo)) lamfail("loadgo (pwait)");

    reset_stdio();
    kexit(0);
    return(0);
}

/*
 *	help
 *
 *	Function:	- prints helpful information on this command
 */
static void
help(void)
{
    printf("\nSynopsis:\tloadgo [options] <nodes> <files> [-- <args>]\n");
    printf("\nDescription:\tLoad and execute LAM programs.\n");
    printf("\nOptions:        -h\t\tPrint this help message.\n");
    printf("\t\t-f\t\tDo not open file descriptors.\n");
    printf("\t\t-v\t\tBe verbose.\n");
    printf("\t\t-w\t\tWait for process completion.\n");
    printf("\t\t-s <nodeid>\tLoad file(s) from this source node.\n");
    nid_usage();
    printf("\t\th (local), o (origin), N (all)\n");
    printf("\nExample:        loadgo n0-7 prog1\n");
    printf("\t\t\t\"Execute `prog1' on nodes 0 thru 7.\"\n\n");
}

/*
 *	pwait
 *
 *	Function:	- if requested, waits for processes to complete
 *	Accepts:	- number of processes to wait
 *	Returns:	- 0 or ERROR
 */
static int
pwait(int4 nwait)
{
    int4		nodeid;		/* child's node ID */
    int			pid;		/* child's process ID */
    int			pstatus;	/* child's exit status */

    if (!opt_taken('w')) {
	return(0);
    }

    for ( ; nwait > 0; --nwait) {

	if (rpwait(&nodeid, &pid, &pstatus)) return(LAMERROR);

	if (pstatus) {
	    printf("%d on %s exited with status %d\n",
		pid, nid_fmt(nodeid), pstatus);
	}
    }

    return(0);
}

/*
 *	setdefaults
 *
 *	Function:	- sets the control variables
 */
static void
setdefaults(void)
{
/*
 * Establish the source node.
 */
    if (opt_taken('s')) {
	srcnode = nid_parse1(getparam('s'));
    } else {
	srcnode = NOTNODEID;
    }
/*
 * Set the process runtime flags.
 */
    rtflags = opt_taken('w') ? RTF_WAIT : 0;

    if (isatty(1))
	rtflags |= RTF_TTYOUT;

    if (!opt_taken('f'))
	rtflags |= RTF_IO;
}

/*
 *	set_stdio
 *
 *	Function:	- sets application stdio
 *	Returns:	- 0 or ERROR
 */
static int
set_stdio(void)
{
    char		server[LAM_PATH_MAX];	/* fd server socket name */

    if (opt_taken('f')) return(0);

#if LAM_HAVE_FD_PASSING
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
#endif

    return(0);
}

/*
 *	reset_stdio
 *
 *	Function:	- reset stdio so rfatexit will clean it up
 *	Returns:	- 0 or LAMERROR
 */
static void
reset_stdio(void)
{
    _kio.ki_stdin = 0;
    _kio.ki_stdout = 1;
    _kio.ki_stderr = 2;
}

/*
 *	fdincr
 *
 *	Function:	- increment process stdio file descriptors
 *	Accepts:	- increment value
 */
static void
fdincr(int4 incval)
{
    if (incval != 0) {
	if (_kio.ki_stdout > 2)
	    lam_rfincr(1, incval);
	if (_kio.ki_stderr > 2)
	    lam_rfincr(2, incval);
    }
}
