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
 * $Id: lamclean.c,v 6.15 2003/07/06 01:28:49 jsquyres Exp $
 *
 *	Function:	- command to clean out several daemons on all nodes
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <bfreq.h>
#include <lamdebug.h>
#include <freq.h>
#include <ksignal.h>
#include <laminternal.h>
#include <net.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <trreq.h>
#include <typical.h>


int
main(int argc, char *argv[])
{
	int4		*all;		/* array of all node IDs */
	int4		i;
	int4		nall;		/* count of all nodes */
	int		fl_verbose;	/* verbosity flag */

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
	validopts("hv");

	if (do_args(&argc, argv) || (argc != 1)) {
		fprintf(stderr, "lamclean [-hv]\n");
		exit(LAM_EEXIT);
	}

	if (opt_taken('h')) {
          show_help("lamclean", "usage", NULL);
          exit(0);
	}

	fl_verbose = opt_taken('v');
/*
 * Initialize and attach to Trollius kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "lamclean", hostname, NULL);
	  exit(LAM_EEXIT);
	}
/*
 * Get an array of all node IDs.
 */
	nall = getnall();
	all = (int4 *) malloc((unsigned) (nall * sizeof(int4)));

	if (all == 0) lamfail("lamclean (malloc)");

	if (getall(all, nall)) lamfail("lamclean (getall)");
/*
 * Doom all appl processes.
 */
	if (fl_verbose) nodespin_init("killing processes,");

	for (i = 0; i < nall; ++i) {

		if (fl_verbose) nodespin_next(all[i]);

		if (rpdoom(all[i], SELECT_APPL, 0, SIGUDIE)) {
			lamfail("lamclean (rpdoom)");
		}
	}
/*
 * Reset file daemons.
 */
	if (fl_verbose) {
		nodespin_end();
		nodespin_init("closing files,");
	}

	for (i = 0; i < nall; ++i) {

		if (fl_verbose) nodespin_next(all[i]);

		if (lam_rfrmfd(all[i], NOTFD)) {
			lamfail("lamclean (rfrmfd)");
		}
	}
/*
 * Reset trace deamons.
 */
	if (fl_verbose) {
		nodespin_end();
		nodespin_init("sweeping traces,");
	}

	for (i = 0; i < nall; ++i) {

		if (fl_verbose) nodespin_next(all[i]);

		if (lam_rtrsweep(all[i], TRANYLIST)) {
			lamfail("lamclean (lam_rtrsweep)");
		}
	}
/*
 * Cleanup registered objects.
 */
	if (fl_verbose) {
		nodespin_end();
		nodespin_init("cleaning up registered objects,");
	}

	for (i = 0; i < nall; ++i) {

		if (fl_verbose) nodespin_next(all[i]);

		if (lam_rtrcleanobjs(all[i])) {
			lamfail("lamclean (lam_rtrcleanobjs)");
		}
	}
/*
 * Reset buffers.
 */
	sleep(1);
	if (fl_verbose) {
		nodespin_end();
		nodespin_init("sweeping messages,");
	}

	for (i = 0; i < nall; ++i) {

		if (fl_verbose) nodespin_next(all[i]);

		if (rbfwipe(all[i])) {
			lamfail("lamclean (rbfwipe)");
		}
	}

	if (fl_verbose) nodespin_end();

	kexit(0);
	return(0);
}
