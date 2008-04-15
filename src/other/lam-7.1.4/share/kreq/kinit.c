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
 * $Id: kinit.c,v 6.19 2003/03/18 04:03:53 jsquyres Exp $
 *
 *	Function:	- initializes data structures for kernel interface
 *			- calls kattach()
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell Research
 *			  Foundation
 *	Accepts:	- scheduling priority
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <freq.h>
#include <kio.h>
#include <kreq.h>
#include <ktrace.h>
#include <net.h>
#include <priority.h>
#include <typical.h>
#include <vreq.h>
#include <etc_misc.h>


/*
 * external data structures
 */
extern struct kio_t	_kio;		/* Kernel I/O block */
extern struct fclient	_ufd[FUMAX];	/* user file desc. */

/*
 * external functions
 */
extern int		_cipc_init();	/* initialize kernel I/O */
extern void		_ksig_init();	/* initialize signals */
extern void		_vcfree();	/* free v.c. table */


int
kinit(int priority)
{
    int i, ret;
    char *version_string;

    /*
     * Register the pid with the pid cache code
     */
    lam_register_pid(getpid());

    if (_kio.ki_pid != lam_getpid()) {
	LAM_ZERO_ME(_kio);
/*
 * Initialize signal handling module.
 */
	_ksig_init();
/*
 * Initialize the exit table.
 */
	for (i = 0; i < TNEXIT; ++i) {
	    _kio.ki_exit[i] = (void (*)()) 0;
	}
/*
 * Initialize route cache.
 */
	for (i = 0; i < MAXRCACHE; ++i) {
	    _kio.ki_route[i].r_nodeid = NOTNODEID;
	}

	_kio.ki_nodeid = NOTNODEID;
	_kio.ki_origin = NOTNODEID;
	_kio.ki_index = -1;
	_kio.ki_jobid.jid_node = getnodeid();
	_kio.ki_jobid.jid_pid = lam_getpid();
/*
 * Free the virtual circuit table.
 */
	if (_kio.ki_pid < 0) {
	    _vcfree();
	}
/*
 * Initialize kernel request interface.
 */
	if (_cipc_init()) {
	    return(LAMERROR);
	}
/*
 * Initialize user file descriptor table
 */
	for (i = 0; i < FUMAX; ++i) {
	    _ufd[i].fu_tflags = 0;
	}

	_ufd[0].fu_tflags = LAM_O_RDONLY | LAM_O_1WAY;
	_ufd[0].fu_node = _kio.ki_ionode;
	_ufd[0].fu_tfd = _kio.ki_stdin;
	_ufd[1].fu_tflags = LAM_O_WRONLY | LAM_O_1WAY;
	_ufd[1].fu_node = _kio.ki_ionode;
	_ufd[1].fu_tfd = _kio.ki_stdout;
	_ufd[2].fu_tflags = LAM_O_WRONLY | LAM_O_1WAY;
	_ufd[2].fu_node = _kio.ki_ionode;
	_ufd[2].fu_tfd = _kio.ki_stderr;

	atkexit(rfatexit, (char *) 0);

/*
 * Attach process to the kernel.
 */
	if (kattach(priority)) {
	    return(LAMERROR);
	}

	if (priority == PRCMD) {
	  /* Call down to the lamd and ensure that its LAM version
	     matches the LAM version that we were compiled against.
	     Bail if the versions do not match. 
	  
	     It isn't perfect, but we only do this if priority is
	     PRCMD because we don't want to get into a chicken and egg
	     problem if we are trying to bring up the pds as seperate
	     processes.  In this case, the versiond calls kinit, which
	     leads to all kinds of badness.  We only do PRCMD because
	     all the lam utils, MPI_INIT, and pretty much anything
	     else of interest that isn't a pd runs as priority PRCMD
	     and neither Jeff nor I really wanted to figure out the
	     mojo that is pd priority assignments. */

	  if (rversion(&version_string, NULL, NULL, NULL, NULL, 
		       NULL) == LAMERROR) {
            if (errno == EFAULT) {
              show_help(NULL, "version-mismatch", LAM_VERSION, version_string,
                        NULL);
              kexit(1);
            } else {
              show_help(NULL, "unable-to-check-version", LAM_VERSION, NULL);
              kexit(1);
            }
	  }
	}

/*
 * Initialize the trace subsystem.
 */
	if (_kio.ki_rtf & RTF_TRACE) {
	    lam_trinit();
	}

	ret = 0;
    }

    else {
/*
 * Attach process to the kernel.
 */
      ret = kattach(priority);
    }

    return ret;
}
