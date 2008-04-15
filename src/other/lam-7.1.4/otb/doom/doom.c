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
 * $Id: doom.c,v 6.16 2003/02/27 15:49:51 jsquyres Exp $
 *
 *	Function:	- deliver signals to Trollius processes
 *			- process selection by index, pid or blocked event
 *			- absence of process specifier selects all processes
 *			- nodes are selected in a system dependent fashion
 *			- absence of signal number defaults to SIGUDIE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>

/*
 * local functions
 */
static void		help(void);
static void		doom_lnode(int4);
static void		doom_rnode(int4, int4);

/*
 * local variables
 */
static char		*usage =
	"doom [-hk] [-<lam signal> | -u <unix signal>] <nodes> [<processes>]\n";


int
main(int argc, char *argv[])
{
	int		n_index;	/* index of node entry */
	int		n_flags;	/* extra node entry info */
	int4		nodeid;		/* value of node entry */
	int4		signo;		/* signal number */

	/* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

/*
 * Parse the command line.
 */
	validopts("hk0");
	followed("u");

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
 * Set default signal.
 */
	if (opt_taken('0')) {
		if (opt_taken('u') || intparam('0', &signo) || signo <= 0) {
			fprintf(stderr, usage);
			exit(EUSAGE);
		}
	} else if (opt_taken('u')) {
		if (opt_taken('k') || intparam('u', &signo) || signo < 0) {
			fprintf(stderr, usage);
			exit(EUSAGE);
		} else {
			signo = -signo;
		}
	} else {
		signo = SIGUDIE;
	}
/*
 * Attach to kernel.
 */
	if (kinit(PRDOOM)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "doom", hostname, NULL);
	  exit(errno);
	}

	if ((nid_parse(&argc, argv)) || (pid_parse(&argc, argv)) ||
			(errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		kexit(errno);
	}
/*
 * Loop through all specified nodes.
 */
	nid_get(&n_index, &nodeid, &n_flags);

	if (n_index < 0) {
		fprintf(stderr, "usage: %s\n", usage);
		kexit(errno);
	}

	do {
		if (opt_taken('k')) {
/*
 * Bypass kenyad.  Allowed on the local node only.
 */
			if ((nodeid == LOCAL) || (nodeid == getnodeid())) {
				doom_lnode(signo);
			} else {
				errno = EBADNODE;
				lamfail("doom");
			}
		}

		else {
			doom_rnode(nodeid, signo);
		}

		nid_get(&n_index, &nodeid, &n_flags);

	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	doom_lnode
 *
 *	Function:	- delivers a signal to a local OTB process
 *			- makes doom request directly to local kernel
 *	Accepts:	- signal number
 */
static void
doom_lnode(int4 signo)
{
	int		fnotpid = FALSE;	/* non pid selectors */
	int		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int		p_flags;		/* process selection method */

	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) {
		fnotpid = TRUE;
	} else {

		do {
			if (p_flags == SELECT_PID) {
				if (kdoom(p_value, signo))
						lamfail("doom (kdoom)");
			} else {
				fnotpid = TRUE;
			}

			pid_get(&p_index, &p_value, &p_flags);

		} while(p_index);
	}
/*
 * Were any unsupported host node selection methods used?
 */
	if (fnotpid) {
		fprintf(stderr,
			"\nHost node processes must be selected by PID.\n");
	}
}

/*
 *	doom_rnode
 *
 *	Function:	- sends signal to process on a remote node
 *	Accepts:	- node identifier
 *			- signal number
 */
static void
doom_rnode(int4 nodeid, int4 signo)
{
	int		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int		p_flags;		/* process selection method */

	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) {
		p_flags = SELECT_APPL;
	}

	do {
		if (rpdoom(nodeid, (int4) p_flags, p_value, signo))
				lamfail("doom (rpdoom)");

		pid_get(&p_index, &p_value, &p_flags);
	} while(p_index > 0);
}

/*
 *	help
 *
 *	Function:	- prints helpful information on the doom command
 */
static void
help(void)
{
	printf("\nSynopsis:\tdoom [options] <nodes> [processes]\n");
	printf("\nDescription:	Deliver a signal to LAM processes.\n");
	printf("\nOptions:\t-h\t\tPrint this help message.\n");
	printf("\t\t-k\t\tTarget a local process by PID.\n");
	printf("\t\t-<sig>\t\tDeliver LAM signal <sig>.\n");
	printf("\t\t-u <sig>\tDeliver Unix signal <sig>.\n");
	printf("\nProcesses:	p<list> (identifiers)\n");
	printf("\t\t%%<list> (indices)\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nSignals:\t1 (TRACE), 4 (UDIE), 5 (ARREST), 6 (RELEASE),\n");
	printf("\t\t7 (user defined A), 8 (user defined B)\n");
	printf("\nExample:	doom -5 n0-7,11 %%8 p2340\n");
	printf("\t\t\t\"Deliver signal 5 to PID 2340 and LAM index 8\n");
	printf("\t\t\tfor nodes 0 thru 7 and 11.\"\n");
}
