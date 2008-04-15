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
 * $Id: wseterr.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- set error handler for window
 *	Accepts:	- communicator
 *			- error handler
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Win_set_errhandler - Sets a window error handler

Input Parameters:
errhdl - new error handler for window (handle) 
win - window (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_ARG

.seealso MPI_Win_create_errhandler(3),  MPI_Win_get_errhandler(3)

.N WEB
@*/

int MPI_Win_set_errhandler(MPI_Win win,MPI_Errhandler errhdl)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINSETERRHDL);
/*
 * Check the arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINSETERRHDL, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	if (errhdl == MPI_ERRHANDLER_NULL || !(errhdl->eh_flags & LAM_EHWIN)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINSETERRHDL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Free previous error handler.
 */
	err = MPI_Errhandler_free(&(win->w_errhdl));
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIWINSETERRHDL, err));
	}
/*
 * Set the communicator's new error handler.
 */
	win->w_errhdl = errhdl;
	errhdl->eh_refcount++;

	lam_resetfunc_m(BLKMPIWINSETERRHDL);
	return(MPI_SUCCESS);
}
