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
 * $Id: bfstate.c,v 6.14 2003/02/27 03:01:10 jsquyres Exp $
 *
 *	Function:	- prints Trollius buffer status
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <bfreq.h>
#include <etc_misc.h>
#include <events.h>
#include <net.h>
#include <priority.h>
#include <terror.h>

/*
 * local functions
 */
static void		help(void);
static void		bfstate(int);

/*
 * local variables
 */
static struct bfstatus	*bftable;		/* buffer state descriptor */
static int		bfmax;			/* max entries */

/*
 * syntax for bfstate command
 */
static char		*usage =
	"bfstate [-ghlp] [-B <maxmsgs>] <nodes>\n";


int
main(int argc, char *argv[])
{
	int		n_index;		/* index of node entry */
	int		nodeid;			/* value of node entry */
	int		n_flags;		/* extra node entry info */

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
	validopts("ghlpB");
	followed("B");

	if (do_args(&argc, argv)) {
		fprintf(stderr, usage);
		exit(errno);
	}

	if (opt_taken('h')) {
		help();
		exit(0);
	}
/*
 * Allocate buffer state table.
 */
	if (opt_taken('B')) {
		intparam('B', &bfmax);
	} else {
		bfmax = BFSMAX;
	}

	if (bfmax > 0) {
		bftable = (struct bfstatus *) malloc((unsigned) (bfmax *
				sizeof(struct bfstatus)));

		if (! bftable) {
			perror("bfstate (malloc)");
			exit(errno);
		}
	}
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "bfstate", hostname, NULL);
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
		errno = EUSAGE;
		fprintf(stderr, usage);
		kexit(errno);
	}

	printf("NODE      DEST      EVENT    TYPE     LENGTH\n");

	do {
		bfstate(nodeid);
		nid_get(&n_index, &nodeid, &n_flags);
	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	bfstate
 *
 *	Function:	- finds and prints the buffer state of a remote node
 *	Accepts:	- node identifier
 */
static void
bfstate(int nodeid)
{
	int		size;		/* # of return blocks */
	int		limit;		/* # of blocks to print */
	int		i;		/* favourite index */
	int4		totlength;	/* total message length */
	struct bfparms	parms;		/* buffer parameters */
	struct bfstatus	*pi;		/* current packet ptr */

	size = rbfstate(nodeid, bftable, bfmax, &parms);
	if (size < 0) lamfail("bfstate (rbfstate)");

	limit = (size < bfmax) ? size : bfmax;

	for (i = 0; i < limit; ++i) {
		pi = bftable + i;

		if ((!opt_taken('p') && (pi->bfs_flags & N2ND)) ||
				(!opt_taken('g') && (pi->bfs_event < 0)))
				continue;
/*
 * Collect all packets from long messages.
 */
		if (!opt_taken('p') && !(pi->bfs_flags & N2ND) &&
				(pi->bfs_flags & NMORE)) {
			totlength = pi->bfs_data[0];
		} else {
			totlength = pi->bfs_length;
		}

		if (totlength >= 0) {
			printf("%-9.9s", nid_fmt(nodeid));
			printf(" %-9.9s", nid_fmt(pi->bfs_node));

			if ((pi->bfs_flags & KSYNCSQL) ||
					(((pi->bfs_event & 0xFFFF0000) != 0)
					&& ((pi->bfs_event & 0xFFFF0000) !=
					0xFFFF0000))) {
				printf(" %-8X", pi->bfs_event);
			} else {
				printf(" %-8d", pi->bfs_event);
			}

			printf(" %-8X", pi->bfs_type);
			printf(" %-7d\n", totlength);
		}
	}

	if (size > bfmax) {
		printf("...and more buffered messages...\n");
	}

	if (opt_taken('l')) {
		printf("%-9.9s", nid_fmt(nodeid));
		printf(" max space = %d, used space = %d\n",
				parms.bfp_maxspace, parms.bfp_nspace);
	}
}

/*
 *	help
 *
 *	Function:	- prints helpful information on the bfstate command
 */
static void
help(void)
{
	printf("\nSynopsis:	bfstate [options] <nodes>\n");
	printf("\nDescription:	Print information on message buffers.\n");
	printf("\nOptions:	-g	Include system messages.\n");
	printf("\t\t-h	Print this help message.\n");
	printf("\t\t-l	Also print buffer levels and maximums.\n");
	printf("\t\t-p	Print each packet.\n");
	printf("\t\t-B <#>	Set maximum # of messages to print.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nExample:	bfstate h n3-9,11\n");
	printf("\t\t\t\"Get buffer status information on the local\n");
	printf("\t\t\t node and nodes 3 thru 9 and 11.\"\n");
}
