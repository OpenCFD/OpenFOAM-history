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
 * $Id: lamunix.c,v 6.9 2003/07/05 21:52:08 jsquyres Exp $
 * 
 *	Function:	- We make UNIX portable in here!
 *			- #ifdef'ing the universe is cool!
 */

#include <lam_config.h>

#include <signal.h>
#include <stdlib.h>

#include <laminternal.h>
#include <etc_misc.h>

/*
 *	_lam_signal
 *
 *	Function:	- signal() wrapper
 */
void (*(
_lam_signal(int signum, void (*handler)())))()
{
	struct sigaction
			newact, old;

	newact.sa_handler = handler;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;

#if LAM_HAVE_SA_RESTART
	newact.sa_flags |= SA_RESTART;
#endif

	if (sigaction(signum, &newact, &old)) {
		return(SIG_ERR);
	}

	return(old.sa_handler);
}

/*
 *	_lam_atexit
 *
 *	Function:	- wrapper for atexit()
 */
void
_lam_atexit(void (*func)())
{
#if LAM_HAVE_ATEXIT
	atexit(func);
#else
	on_exit((int ((*)())) func, (char *) 0);
#endif
}
