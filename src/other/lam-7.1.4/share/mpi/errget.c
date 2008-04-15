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
 * $Id: errget.c,v 6.7 2002/12/11 19:15:10 jsquyres Exp $
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
  MPI_Errhandler_get - Gets the error handler for a communicator

Input Parameter:
. comm - communicator to get the error handler from (handle) 

Output Parameter:
. errhandler - MPI error handler currently associated with communicator
(handle) 

.N Wrapper MPI_Comm_get_errhandler

.N ACK
@*/
int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhdl)
{
	lam_initerr();
	lam_setfunc(BLKMPIERRGET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_COMM, EINVAL,
			"null handle"));
	}

	if (errhdl == 0) {
		return(lam_err_comm(comm, MPI_ERR_ARG, EINVAL, 
				    "null pointer"));
	}
/*
 * Get the communicator's error handler.
 */
	*errhdl = comm->c_errhdl;
	(*errhdl)->eh_refcount++;

	lam_resetfunc(BLKMPIERRGET);
	return(MPI_SUCCESS);
}
