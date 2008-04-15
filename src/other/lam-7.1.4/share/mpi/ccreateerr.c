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
 * $Id: ccreateerr.c,v 1.3 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- create communicator error handler
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
    MPI_Comm_create_errhandler - Create communicator error handler

Input Parameter:
+ errfunc - User definer error handling procedure

Output Parameter:
+ errhdl - MPI error handler

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER
.N MPI_ERR_ARG

.seealso MPI_Comm_set_errhandler(3), MPI_Comm_get_errhandler(3)

.N WEB
@*/

int
MPI_Comm_create_errhandler(MPI_Comm_errhandler_fn *errfunc, 
			   MPI_Errhandler *errhdl)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMCREATEERRHDL);
/*
 * Check the arguments.
 */
	if ((errfunc == 0) || (errhdl == 0)) {
		return(lam_err_comm(MPI_COMM_WORLD,
				MPI_ERR_ARG, 0, "null pointer"));
	}
/*
 * Create the handler.
 */
	*errhdl = (MPI_Errhandler) malloc((unsigned) sizeof(struct _errhdl));
	if (*errhdl == 0) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_OTHER, errno, ""));
	}

	(*errhdl)->eh_func = (void (*)()) errfunc;
	(*errhdl)->eh_f77handle = -1;
	(*errhdl)->eh_refcount = 1;
	(*errhdl)->eh_flags = LAM_EHCOMM;

	lam_resetfunc_m(BLKMPICOMMCREATEERRHDL);
	return(MPI_SUCCESS);
}
