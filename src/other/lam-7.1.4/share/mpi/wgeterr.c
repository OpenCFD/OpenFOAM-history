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
 *
 * $Id: wgeterr.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- get error handler of window
 *	Accepts:	- communicator
 *			- error handler (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Win_get_errhandler - Get an errorhandler for a window

Input Parameter:
+ win - window (handle) 

Output Parameter:
+ errhdl - error handler currently associated with window (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_ARG

.seealso MPI_Win_create_errhandler(3),  MPI_Win_get_errhandler(3)

.N WEB
@*/

int MPI_Win_get_errhandler(MPI_Win win,
			   MPI_Errhandler *errhdl)
{
	lam_initerr();
	lam_setfunc(BLKMPIWINGETERRHDL);
/*
 * Check the arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINGETERRHDL, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	if (errhdl == 0) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINGETERRHDL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Get the window's error handler.
 */
	*errhdl = win->w_errhdl;

	lam_resetfunc(BLKMPIWINGETERRHDL);
	return(MPI_SUCCESS);
}
