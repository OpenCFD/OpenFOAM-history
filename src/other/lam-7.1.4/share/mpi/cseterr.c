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
 * $Id: cseterr.c,v 1.3 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- set error handler for communicator
 *	Accepts:	- communicator
 *			- error handler
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_ser_errhandler - Set error handler of communicator

Input Parameter:
+ comm - Communicator

Output Parameter:
+ errhdl - Error Handler

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso MPI_Comm_create_errhandler(3), MPI_Comm_get_errhandler(3)

.N WEB
@*/

int
MPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhdl)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMSETERRHDL);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_COMM, EINVAL,
			"null handle"));
	}

	if (errhdl == MPI_ERRHANDLER_NULL || !(errhdl->eh_flags & LAM_EHCOMM)) {
		return(lam_err_comm(comm, MPI_ERR_ARG, EINVAL,
			"illegal error handler"));
	}
/*
 * Free previous error handler.
 */
	err = MPI_Errhandler_free(&(comm->c_errhdl));
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMSETERRHDL, err));
	}
/*
 * Set the communicator's new error handler.
 */
	comm->c_errhdl = errhdl;
	errhdl->eh_refcount++;

	lam_resetfunc_m(BLKMPICOMMSETERRHDL);
	return(MPI_SUCCESS);
}
