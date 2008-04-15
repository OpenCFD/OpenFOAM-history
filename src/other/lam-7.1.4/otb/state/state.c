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
 * $Id: state.c,v 6.12 2003/02/16 06:59:32 brbarret Exp $
 * 
 *	Function:	- prints Trollius process status information
 *			- processes can be selected by index or identifier
 *			- absence of process specifier selects all
 *			  application processes
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 *	Accepts:	- see the "usage" variable
 *	Examples:	- state n10 %1 %2 p20300
 *			- get status on process indices
 *			  1 & 2 and process ID 20300 on node 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <args.h>
#include <blktype.h>
#include <etc_misc.h>
#include <events.h>			/* for EVSTOP and EVPAUSE */
#include <ksignal.h>			/* signal constants */
#include <net.h>			/* network constants */
#include <preq.h>			/* kenya interface constants */
#include <priority.h>			/* scheduling priority */
#include <terror.h>			/* error codes */
#include <typical.h>			/* commonly used constants */

/*
 * local functions
 */
static void		help();
static void		state_lnode();
static void		state_out();
static void		state_rnode();

/*
 * local variables
 */
static char		*kstates[4] = {
				"R ",
				"BS",
				"BR",
				"F "
			};

static struct pstate	ptable[PMAX];		/* kenya process table */

/*
 * syntax for state command
 */
static char		*usage = "state [-ghklt] <nodes> [<processes>]\n";

/*
 *	main
 */
int
main(argc, argv)

int   			argc;
char   			*argv[];

{
	int4		n_index;		/* index of node entry */
	int4		nodeid;			/* value of node entry */
	int4		n_flags;		/* extra node entry info */
/*
 * Parse the command line.
 */
	validopts("ghklt");

	if (do_args(&argc, argv)) {
		fprintf(stderr, usage);
		exit(errno);
	}

	if (opt_taken('h')) {
		help();
		exit(0);
	}
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  show_help(NULL, "no-lamd", "state", NULL);
	  exit(errno);
	}

	if (nid_parse(&argc, argv) || (pid_parse(&argc, argv)) ||
			(errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		kexit(errno);
	}
/*
 * Loop through all specified nodes.
 */
	nid_get(&n_index, &nodeid, &n_flags);

	if (n_index < 0) {
		fprintf(stderr, usage);
		kexit(errno);
	}

	if (opt_taken('l')) {
		printf("NODE   PID    SIGS     PROGRAM\n");
	} else {
		printf(
		"NODE   INDEX  PID    KPRI  KSTATE                  PROGRAM\n");
	}

	do {

		if (opt_taken('k')) {

			if ((nodeid == LOCAL) || (nodeid == getnodeid())) {
				state_lnode();
			} else {
				errno = EBADNODE;
				lamfail("state");
			}
		} else {
			state_rnode(nodeid);
		}

		nid_get(&n_index, &nodeid, &n_flags);
	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	state_lnode
 *
 *	Function:	- finds and prints the state of a local kernel process
 *			- obtains state information from the kernel
 *			- kenya state is assigned null values
 */
static void
state_lnode()

{
	int		fnotpid = FALSE;	/* non pid selectors */
	int4		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int4		p_flags;		/* process selection method */

	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) {
		fnotpid = TRUE;
	} else if (opt_taken('g') || opt_taken('t')) {
		fnotpid = TRUE;
	} else {

		do {
			if (p_flags == SELECT_PID) {

				if (kstate(p_value, &ptable[0].ps_kernel))
						lamfail("state (kstate)");

				ptable[0].ps_index = 0;
				ptable[0].ps_pid = p_value;
				ptable[0].ps_rtf = 0;
				ptable[0].ps_name[0] = '\0';
				state_out(&ptable[0], LOCAL);
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
			"state: Select kernel (-k) processes by PID.\n");
	}
}

/*
 *	state_rnode
 *
 *	Function:	- finds and prints the state of a remote process
 *	Accepts:	- node identifier
 */
static void
state_rnode(nodeid)

int			nodeid;

{
	int4		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int4		p_flags;		/* process selection method */
	int4		nprocs;			/* # processes returned */
	int4		i;			/* favourite index */

	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) {

		if (opt_taken('g')) {
			p_flags = SELECT_ALL;
		} else if (opt_taken('t')) {
			p_flags = SELECT_SYS;
		} else {
			p_flags = SELECT_APPL;
		}
	}

	do {
		nprocs = rpstate(nodeid, p_flags, p_value,
					ptable, (int4) PMAX);
		if (nprocs < 0) lamfail("state (rpstate)");

		for (i = 0; i < nprocs; ++i) {
			state_out(ptable + i, nodeid);
		}

		pid_get(&p_index, &p_value, &p_flags);
	} while(p_index > 0);
}

/*
 *
 *	  state_out
 *
 *	  Function:	- prints out state information
 *	  Accepts:	- completed kenya state structure
 *			- node identifier
 */
static void
state_out(state, nodeid)

struct pstate		*state;
int			nodeid;

{
	char		buffer[32];	/* used for formatting purposes */
	char		*shortname;	/* program pathname tail */
	char		*p;		/* favourite pointer */
	int		i;
	int4		t;
	char		*fmt;		/* printing format */

	printf("%-6.6s", nid_fmt(nodeid));

	if (opt_taken('l')) {
		printf(" %-6d", state->ps_pid);

		if (state->ps_kernel.ks_signal & ~(-1 << TNSIG)) {
			p = buffer;

			for (i = 1; (i <= TNSIG); ++i) {

				if (state->ps_kernel.ks_signal & 1) {
					sprintf(p, "%1.1d,", i);
					p += 2;
				}

				state->ps_kernel.ks_signal >>= 1;
			}

			*(p - 1) = '\0';
			printf(" %-8.8s", buffer);
		} else {
			printf(" <nil>   ");
		}
	}

	else {
		sprintf(buffer, "[%d]", state->ps_index);
		printf(" %-6.6s", buffer);
		printf(" %-6d", state->ps_pid);
		printf(" %-5d", state->ps_kernel.ks_priority);

		if (state->ps_kernel.ks_event == NOTEVENT ||
				state->ps_kernel.ks_state == KSREADY) {
			sprintf(buffer, "%s",
					kstates[state->ps_kernel.ks_state]);
		}

		else if ((state->ps_kernel.ks_event == EVSTOP) &&
				(state->ps_kernel.ks_state == KSRECV)) {
			sprintf(buffer, "<stopped>");
		}

		else if ((state->ps_kernel.ks_event == EVPAUSE) &&
				(state->ps_kernel.ks_state == KSRECV)) {
			sprintf(buffer, "<paused>");
		}

		else {
			if ((state->ps_kernel.ks_event == - state->ps_pid) &&
					(state->ps_kernel.ks_type != 0)) {
				p = blktype(state->ps_kernel.ks_type);
				sprintf(buffer, "%s (%s)",
					kstates[state->ps_kernel.ks_state], p);
			} else {
				t = state->ps_kernel.ks_event & 0xFFFF0000;

				if ((t != 0) && (t != 0xFFFF0000)) {
					fmt = "%s (%X, %X)";
				} else {
					fmt = "%s (%d, %X)";
				}

				sprintf(buffer, fmt,
					kstates[state->ps_kernel.ks_state],
					state->ps_kernel.ks_event,
					state->ps_kernel.ks_type);
			}
		}

		printf(" %-23.23s", buffer);
	}
/*
 * Print the program name without any pathname prefix.
 */
	if (state->ps_name[0] == '\0') {
		shortname = "<unknown>";
	} else {
		shortname = strrchr(state->ps_name, STRDIR);
		shortname = (shortname == 0) ? state->ps_name : shortname + 1;
	}

	printf(" %.28s\n", shortname);
}

/*
 *	  help
 *
 *	  Function:	- prints helpful information on the state command
 */
static void
help()

{
	printf("\nSynopsis:	state [options] <nodes> [<processes>]\n");
	printf("\nDescription:	Print information on LAM processes.\n");
	printf("\nOptions:	-h	Print this message.\n");
	printf("\t\t-g	Select all processes.\n");
	printf("\t\t-k	Consult local kernel only.\n");
	printf("\t\t-l	Print alternative information.\n");
	printf("\t\t-t	Select only system processes.\n");
	printf("\nProcesses:	p<list> (identifiers)\n");
	printf("\t\t%%<list> (indices)\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nExample:	state n3-9,11 %%1 %%2 p303\n");
	printf("\t\t\t\"Get status information for PID 303 and\n");
	printf("\t\t\tindices 1 & 2 for nodes 3 thru 9 and 11.\"\n");
}
