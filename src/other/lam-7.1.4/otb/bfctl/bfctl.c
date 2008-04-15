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
 * $Id: bfctl.c,v 6.14 2003/02/16 06:52:37 brbarret Exp $
 *
 *	Function:	- buffer daemon control command
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <all_opt.h>
#include <args.h>
#include <bfreq.h>
#include <etc_misc.h>
#include <net.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>

/*
 * local variables
 */
static char		*usage =
    "bfctl [-hR] [-s <size>] [-e <event>] <nodes>\n";

/*
 * local functions
 */
static void		help();


int
main(argc, argv)

int			argc;
char			*argv[];

{
	int		taken;		/* at least one opt taken? */
	int		n_index;	/* for nid_get() */
	int		n_flags;	/* for nid_get() */
	int4		nodeid;		/* target node ID */
	int4		event;		/* target event */
	struct bfparms	parms;		/* bufferd parms */
	OPT		*op;		/* cmd line options */

	/* Ensure that we are not root */

	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}

/*
 * Parse command-line arguments.
 */
	op = ao_init();
	ao_setopt(op, "h", (char *) 0, 0, 0);
	ao_setopt(op, "R", "R", 0, 0);
	ao_setopt(op, "e", "R", 1, AOINT);
	ao_setopt(op, "s", "s", 1, AOINT);

	if (ao_parse(op, &argc, argv)) {
		fprintf(stderr, usage);
		exit(errno);
	}

	if (ao_taken(op, "h")) {
		help();
		exit(0);
	}
/*
 * Initialize and attach to Trollius kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "bfctl", hostname, NULL);
	  exit(errno);
	}

	if (nid_parse(&argc, argv) || (errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		kexit(errno);
	}
/*
 * Loop through all specified nodes.
 */
	nid_get(&n_index, &nodeid, &n_flags);

	if (n_index < 0) {
		fprintf(stderr, usage);
		kexit(EUSAGE);
	}

	do {
		taken = FALSE;
/*
 * Execute the proper buffer request.
 */
		if (ao_taken(op, "s")) {
			taken = TRUE;
			ao_intparam(op, "s", 0, 0, &parms.bfp_maxspace);
			if (rbfparms(nodeid, &parms))
					lamfail("bfctl (rbfparms)");
		}

		if (ao_taken(op, "R")) {
			taken = TRUE;
			if (rbfsweep(nodeid)) lamfail("bfctl (rbfsweep1)");
		}

		else if (ao_taken(op, "e")) {
			taken = TRUE;
			ao_intparam(op, "e", 0, 0, &event);
			if (rbfsweep1(nodeid, event))
					lamfail("bfctl (rbfsweep1)");
		}

		if (! taken) {
			fprintf(stderr,
				"bfctl: Use bfstate for buffer status.\n");
			kexit(EUSAGE);
		}

		nid_get(&n_index, &nodeid, &n_flags);
	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	help
 *
 *	Function:	- prints helpful information on this command
 */
static void
help()

{
	printf("\nSynopsis:	bfctl [options] <nodes>\n");
	printf("\nDescription:	Control LAM message buffers.\n");
	printf("\nOptions:\t-h\t\tPrint this help message.\n");
	printf("\t\t-s #\t\tSet buffer size.\n");
	printf("\t\t-e <event>\tSweep messages of one event.\n");
	printf("\t\t-R\t\tSweep all messages.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nExample:	bfctl N -s 0x100000\n");
	printf("\t\t\t\"Set max buffer space to 1M bytes on all nodes.\"\n");
}
