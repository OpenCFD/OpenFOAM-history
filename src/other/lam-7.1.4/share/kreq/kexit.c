/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: kexit.c,v 6.8 2002/12/12 16:00:06 jsquyres Exp $
 *
 *	Function:	- exit package
 *			- performs all tasks required when a process exits
 */

#include <stdio.h>
#include <unistd.h>

#include <kio.h>
#include <preq.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>
#include <laminternal.h>
#include <mpisys.h>
#include <etc_misc.h>


/*
 *	atkexit
 *
 *	Function:	- add function to cleanup table
 *	Accepts:	- pointer to exit function
 *			- argument to exit function
 *			- does not store a combination of exit function
 *			  and argument more than once
 *	Returns:	- 0 or ERROR
 */
int
atkexit(void (*exitfunc)(), char *arg)
{
	int		i;		/* favourite index variable */

	for (i = 0; (i < TNEXIT) && (_kio.ki_exit[i]) &&
			!((_kio.ki_exit[i] == exitfunc) &&
			(_kio.ki_arg[i] == arg)); ++i);

	if (i < TNEXIT) {

		if (!_kio.ki_exit[i]) {
			_kio.ki_exit[i] = exitfunc;
			_kio.ki_arg[i] = arg;
		}

		return(0);
	}

	else {
		errno = ENOEXDESCRIPTORS;
		return(LAMERROR);
	}
}


/*
 *	lam_kexit
 *
 *	Function:	- terminates Trollius runtime system
 *			- calls cleanup routines
 *			- detaches from kenyad and the kernel
 *	Accepts:	- exit status
 */
void
lam_kexit(int status)
{
	int		i;

	if (_kio.ki_pid == lam_getpid()) {

		for (i = TNEXIT - 1; i >= 0; --i) {

			if (_kio.ki_exit[i]) {
				(*(_kio.ki_exit[i]))(_kio.ki_arg[i], status);
			}
		}

		if (_kio.ki_rtf & RTF_IO) {
			fflush(stdout);
			fflush(stderr);
		}
		
		if (_kio.ki_rtf & RTF_KENYA_ATTACH) {
			lpdetach(0, status);
		}

		kdetach(0);
	}
/*
 * Destroy underlying communication structures and free related resources.
 */
	_cio_cleanup();
}


/*
 *	kexit
 *
 *	Function:	- exits from Trollius
 *			- calls cleanup routines
 *			- does not return
 *	Accepts:	- exit status
 */
void
kexit(int status)
{
	lam_kexit(status);
/*
 * _cio_bail does not return.
 */
	_cio_bail(status);
}


/*
 *	_kexit
 *
 *	Function:	- exits from Trollius
 *			- does not call cleanup routines
 *			- does not return
 *	Accepts:	- exit status
 */
void
_kexit(int status)
{
	if (_kio.ki_pid == lam_getpid()) {

		if (_kio.ki_rtf & RTF_KENYA_ATTACH) {
			lpdetach(0, status);
		}

		kdetach(0);
	}
/*
 * Destroy underlying communication structures and free related resources.
 */
	_cio_cleanup();
/*
 * _cio_bail does not return.
 */
	_cio_bail(status);
}
