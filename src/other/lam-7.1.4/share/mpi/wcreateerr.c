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
 * $Id: wcreateerr.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- create window error handler
 *	Accepts:	- error function
 *			- error handler (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Win_create_errhandler - Creates a window error handler

Input Parameter:
errfunc - user defined error handling procedure (function) 

Output Parameter:
errhdl - MPI error handler (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER

.seealso MPI_Win_set_errhandler(3),  MPI_Win_get_errhandler(3)

.N WEB
@*/

int MPI_Win_create_errhandler(MPI_Win_errhandler_fn *errfunc,
			      MPI_Errhandler *errhdl)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINCREATEERRHDL);
/*
 * Check the arguments.
 */
	if ((errfunc == 0) || (errhdl == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPIWINCREATEERRHDL, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the handler.
 */
	*errhdl = (MPI_Errhandler) malloc((unsigned) sizeof(struct _errhdl));
	if (*errhdl == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWINCREATEERRHDL,
		    	lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*errhdl)->eh_func = (void (*)()) errfunc;
	(*errhdl)->eh_f77handle = -1;
	(*errhdl)->eh_refcount = 1;
	(*errhdl)->eh_flags = LAM_EHWIN;

	lam_resetfunc_m(BLKMPIWINCREATEERRHDL);
	return(MPI_SUCCESS);
}
