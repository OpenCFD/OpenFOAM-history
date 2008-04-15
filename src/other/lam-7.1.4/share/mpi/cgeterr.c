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
 * $Id: cgeterr.c,v 1.3 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get error handler of communicator
 *	Accepts:	- communicator
 *			- error handler (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_get_errhandler - Get error handler of communicator

Input Parameter:
+ comm - Communicator

Output Parameter:
+ errhdl - Error Handler

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso MPI_Comm_create_errhandler(3), MPI_Comm_set_errhandler(3)

.N WEB
@*/

int
MPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *errhdl)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMGETERRHDL);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_COMM, 0,
			"null handle"));
	}

	if (errhdl == 0) {
		return(lam_err_comm(comm, MPI_ERR_ARG, 0, "null pointer"));
	}
/*
 * Get the communicator's error handler.
 */
	*errhdl = comm->c_errhdl;

	lam_resetfunc(BLKMPICOMMGETERRHDL);
	return(MPI_SUCCESS);
}
