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
 * $Id: lamsig.c,v 1.15 2003/05/21 19:17:03 jsquyres Exp $
 *
 *	Function:	- exception signal catching
 */

#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <etc_misc.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif

typedef void 		(*sighandler_t)(int);

/*
 * public functions
 */
int			lam_mpi_set_sighandlers();
void			lam_mpi_reset_sighandlers();
void                    lam_printfunc(void);

/*
 * private functions
 */
static void		lam_mpi_sig_handler(int sig);

/*
 * external functions
 */
extern void		(*(_lam_signal()))();

/*
 * private variables
 */
static sighandler_t	old_segv_handler;
static sighandler_t	old_bus_handler;
static sighandler_t	old_fpe_handler;
static sighandler_t	old_ill_handler;
static int              in_handler_already = 0;


/*
 *	lam_mpi_set_sighandlers
 *
 *	Function:	- 
 *	Accepts:	-
 *	Returns:	- 
 */
int
lam_mpi_set_sighandlers(void)
{
	old_segv_handler = _lam_signal(SIGSEGV, lam_mpi_sig_handler);
	if (old_segv_handler == SIG_ERR) return(LAMERROR);

	old_bus_handler = _lam_signal(SIGBUS, lam_mpi_sig_handler);
	if (old_bus_handler == SIG_ERR) return(LAMERROR);

	old_fpe_handler = _lam_signal(SIGFPE, lam_mpi_sig_handler);
	if (old_fpe_handler == SIG_ERR) return(LAMERROR);

	old_ill_handler = _lam_signal(SIGILL, lam_mpi_sig_handler);
	if (old_ill_handler == SIG_ERR) return(LAMERROR);

	return(0);
}


/*
 *	lam_mpi_set_sighandlers
 *
 *	Function:	- 
 *	Accepts:	-
 *	Returns:	- 
 */
void
lam_mpi_reset_sighandlers(void)
{
	_lam_signal(SIGSEGV, old_segv_handler);
	_lam_signal(SIGBUS, old_bus_handler);
	_lam_signal(SIGFPE, old_fpe_handler);
	_lam_signal(SIGILL, old_ill_handler);
}


/*
 *	lam_mpi_sig_handler
 *
 *	Function:	- handler for exceptions
 *	Accepts:	- signal number
 */
static void
lam_mpi_sig_handler(int sig)
{
	struct _proc	**p;
	sighandler_t	old_handler;
	CONST char	*err;
	int		mpifn;
	int		i;
	char            prefix[512];
	char            extra[512];

	/* Now go handle this error */

	switch (sig) {

	case SIGSEGV:
		old_handler = old_segv_handler;
		err = "a SIGSEGV";
		break;
	case SIGBUS:
		old_handler = old_bus_handler;
		err = "a SIGBUS"; 
		break;
	case SIGFPE:
		old_handler = old_fpe_handler;
		err = "a SIGFPE";
		break;
	case SIGILL:
		old_handler = old_ill_handler;
		err = "a SIGILL";
		break;
	default:
	        old_handler = SIG_DFL;
	        sprintf(extra, "signal %d", sig);
		err = extra;
		break;
	}

	/* Make sure we don't get caught it a loop.  An alternative
	   way to handle this would be to reset the default signal
	   handlers the first time this function is invoked and
	   therefore let the default sighandler catch the signal next
	   time through.  We decided to print out a friendly message
	   and abort() instead.  It is quite possible that that could
	   be configured via ./configure and/or an environment
	   variable override.  

	   For example, Dave T. at GE wants to get core dumps when
	   things go wrong -- something that the default sig handler
	   would provide This is actually a different issue (i.e.,
	   we'd call the default signal handler at the end of this
	   handler -- it's not a "second invocation of the handler"
	   issue), but it's related so I mention it here. */

	if (in_handler_already == 1) {
	  fprintf(stderr, "\n\n\n------------------------------------------------------------------------\n"
"LAM/MPI ERROR! A signal handler (%s / #%d) was invoked after we\n"
"were already in a signal handler!  I'm aborting before trying to handle it\n"
"to prevent thrashing...  You should check the exit behavior of your\n"
"program, such as C++ destructors, functions registered with\n"
"onexit()/atexit(), etc.\n"
"------------------------------------------------------------------------\n\n\n", 
		  err, sig);
	  abort();
	}
	in_handler_already = 1;

	/* Do something */

	if (old_handler == SIG_IGN) 
	  return;

#if LAM_WANT_IMPI
	if (LAM_GPSCMP(&lam_myproc->p_gps, &gimpid) == 0) {
	  strcpy(prefix, "LAM IMPI client daemon");
	} else
#endif
	sprintf(prefix, "MPI process rank %d", lam_myproc->p_gps.gps_grank);

	if ((mpifn = lam_getfunc_m())) {
		fprintf(stderr,
			"%s (n%d, p%d) caught %s in %s.\n",
			prefix,
			lam_myproc->p_gps.gps_node,
			lam_myproc->p_gps.gps_pid, err, blktype(mpifn));
	} else {
		fprintf(stderr,
			"%s (n%d, p%d) caught %s.\n",
			prefix,
			lam_myproc->p_gps.gps_node,
			lam_myproc->p_gps.gps_pid, err);
	}
	fflush(stderr);
/*
 * Print the call stack
 */
	lam_printfunc();

	if (((_kio.ki_rtf & RTF_MPIRUN) || (_kio.ki_parent > 0)) &&
#if LAM_WANT_IMPI
	    LAM_GPSCMP(&lam_myproc->p_gps, &gimpid) != 0 &&
#endif
	    MPI_COMM_WORLD != 0 && MPI_COMM_WORLD->c_group) {

		if (lam_fork() == 0) {
		  /* clear pid cache, since we want to have new pid */
		  lam_reset_pid_cache();
			if (kinit(PRCMD)) {
				exit(errno);
			}

			p = MPI_COMM_WORLD->c_group->g_procs;
			for (i = 0; i < MPI_COMM_WORLD->c_group->g_nprocs;
					++i, ++p) {

				rpdoom((*p)->p_gps.gps_node, SELECT_PID,
						(*p)->p_gps.gps_pid, SIGUDIE);
			}

			kexit(0);
		}
	}

	kexit(1);
}
