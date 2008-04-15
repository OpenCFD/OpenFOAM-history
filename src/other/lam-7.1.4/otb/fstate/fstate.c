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
 * $Id: fstate.c,v 6.14 2003/02/27 03:01:11 jsquyres Exp $
 * 
 *	Function:	- command to print Trillium filed status info
 *	Accepts:	- see the "usage" variable
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <freq.h>
#include <net.h>
#include <priority.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>

#define FSMAX		256

/*
 * local functions
 */
static void		fstate1(int4);
static void		help(void);

/*
 * local variables
 */
static struct fstate	*ftable;		/* file descriptors */
static int4		fsmax;			/* max entries */

/*
 * syntax for fstate command
 */
static char *usage = "fstate [-ah] [-F <maxdesc>] <nodes>\n";

int
main(int argc, char *argv[])
{
	int		n_index;		/* index of node entry */
	int4		nodeid;			/* value of node entry */
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
	validopts("Cach");
	followed("F");

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
 * Allocate filed state table.
 */
	if (opt_taken('F')) {
		intparam('F', &fsmax);
	} else {
		fsmax = FSMAX;
	}

	ftable = (struct fstate *)
			malloc((unsigned) (fsmax * sizeof(struct fstate)));
	if (ftable == 0) lamfail("fstate (malloc)");
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "fstate", hostname, NULL);
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

	if (n_index >= 0) {
		printf(
	"NODE     FD/COUNT FLAGS    FLOW     CLIENT        NAME\n");

		do {
			fstate1(nodeid);
			nid_get(&n_index, &nodeid, &n_flags);
		} while (n_index);
	}

	else {
		fprintf(stderr, usage);
		kexit(errno);
	}

	kexit(0);
	return(0);
}

/*
 *	fstate1
 *
 *	Function:	- finds and prints the filed state of a remote node
 *	Accepts:	- node identifier
 */
static void
fstate1(int4 nodeid)
{
	int4		count;		/* # of return blocks */
	int4		limit;		/* # of blocks to print */
	int4		i;		/* favourite index */
	char		buf[24];	/* field buffer */
	char		*p;		/* ptr within buf */

	count = lam_rfstate(nodeid, ftable, fsmax);
	if (count < 0) lamfail("fstate (rfstate)");

	limit = (count < fsmax) ? count : fsmax;

	for (i = 0; i < limit; ++i) {

		if (opt_taken('a') && !(ftable[i].fs_tflags & FACTIVE)) {
			continue;
		}

		printf("%-9.9s", nid_fmt(nodeid));

		sprintf(buf, "%d/%d", ftable[i].fs_tfd, ftable[i].fs_count);
		printf("%-8.8s ", buf);

		p = buf;
		if (ftable[i].fs_tflags & LAM_O_WRONLY) {
			*p++ = 'W';
		} else if (ftable[i].fs_tflags & LAM_O_RDWR) {
			*p++ = 'R';
			*p++ = '|';
			*p++ = 'W';
		} else {
			*p++ = 'R';
		}

		if (ftable[i].fs_tflags & FLOCK) {
			*p++ = '|';
			*p++ = 'L';
		} else if (ftable[i].fs_tflags & FACTIVE) {
			*p++ = '|';
			*p++ = 'A';
		} else {
			*p++ = '|';
			*p++ = 'I';
		}

		*p = '\0';
		printf("%-9.9s", buf);

		printf("%-8d ", ftable[i].fs_flow);

		if (ftable[i].fs_src_node == (int4) NOTNODEID) {
			printf("none          ");
		} else if (ftable[i].fs_src_node == (int4) LOCAL) {
			sprintf(buf, "local/%d", ftable[i].fs_src_event);
			printf("%-13.13s ", buf);
		} else {
			sprintf(buf, "n%d/p%d", ftable[i].fs_src_node,
					-ftable[i].fs_src_event);
			printf("%-13.13s ", buf);
		}

		printf("%-30.30s\n", ftable[i].fs_name);
	}

	if (count > fsmax) {
		printf("...and more file descriptors...\n");
	}
}

/*
 *	help
 *
 *	Function:	- prints helpful information on the fstate command
 */
static void
help(void)
{
	printf("\nSynopsis:	fstate [options] <nodes>\n");
	printf("\nDescription:	Print information on file descriptors.\n");
	printf("\nOptions:	-h	Print this help message.\n");
	printf("\t\t-a	Display active descriptors only.\n");
	printf("\t\t-F <#>	Set maximum # of descriptors to print.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nExample:	fstate h\n");
	printf("\t\t\t\"Get file status information on the local node.\"\n");
}
