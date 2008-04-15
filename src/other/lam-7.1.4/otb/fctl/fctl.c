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
 * $Id: fctl.c,v 6.14 2003/02/27 03:01:10 jsquyres Exp $
 *
 *	Function:	- command to control certain filed functions
 *	Accepts:	- see the "usage" variable
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <freq.h>
#include <net.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

/*
 * local functions
 */
static void		help(void);

/*
 * local variables
 */
static char		*usage =
		"fctl [-hS] [-s <descno>] [<directory>] <nodes>\n";
static char		pwd[MAXPATHLEN];


int
main(int argc, char *argv[])
{
	int		n_index;		/* index of node entry */
	int		nodeid;			/* value of node entry */
	int		n_flags;		/* extra node entry info */
	int		tfd;			/* filed descriptor # */

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
	validopts("Ssh");
	followed("s");
	exclusive("Ss");

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
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "fctl", hostname, NULL);
	  exit(errno);
	}

	if ((nid_parse(&argc, argv)) || (errno = (argc <= 2) ? 0 : EUSAGE)) {
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
/*
 * Clean up file descriptors in filed.
 */
		if (opt_taken('S')) {
			if (lam_rfrmfd(nodeid, NOTFD)) lamfail("fctl (rfrmfd)");
		} else if (opt_taken('s')) {
		 	intparam('s', &tfd);
			if (lam_rfrmfd(nodeid, tfd)) lamfail("fctl (rfrmfd)");
		}
/*
 * Print the current working directory.
 */
		else if (argc == 1) {
			if (lam_rfgetwd(nodeid, pwd) == 0)
					lamfail("fctl (rfgetwd)");
			printf("%s: %s\n", nid_fmt(nodeid), pwd);
		}
/*
 * Change current working directory.
 */
		else if (argc == 2) {
			if (lam_rfchdir(nodeid, argv[1]))
					lamfail("fctl (rfchdir)");
		}

		nid_get(&n_index, &nodeid, &n_flags);
	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	  help
 *
 *	  Function:	- prints helpful information on the fctl command
 */
static void
help(void)
{
	printf("\nSynopsis:	fctl [options] <nodes> [<dir>]\n");
	printf("\nDescription:	Control file daemon operations.\n");
	printf("\nOptions:	-h	Print this help message.\n");
	printf("\t\t-S	Blow away all open descriptors.\n");
	printf("\t\t-s <#>	Blow away one specific descriptor.\n");
	printf("\t\t<dir>	Change to new working directory.\n");
	printf("\t\tnone	Print current working directory.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nExample:	fctl h -S\n");
	printf("\t\t\t\"Clean up (reset) the file daemon.\"\n");
}
