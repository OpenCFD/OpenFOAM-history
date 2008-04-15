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
 * $Id: errset.c,v 6.6 2002/12/11 19:15:10 jsquyres Exp $
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
  MPI_Errhandler_set - Sets the error handler for a communicator

Input Parameters:
+ comm - communicator to set the error handler for (handle) 
- errhandler - new MPI error handler for communicator (handle) 

.N Wrapper MPI_Comm_set_errhandler

.N ACK
@*/
int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhdl)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIERRSET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_COMM, EINVAL,
			"null handle"));
	}

	if (errhdl == MPI_ERRHANDLER_NULL || !(errhdl->eh_flags & LAM_EHCOMM)) {
		return(lam_err_comm(comm, MPI_ERR_ARG, EINVAL,
			"invalid error handler"));
	}
/*
 * Free previous error handler.
 */
	err = MPI_Errhandler_free(&(comm->c_errhdl));
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIERRSET, err));
	}
/*
 * Set the communicator's new error handler.
 */
	comm->c_errhdl = errhdl;
	errhdl->eh_refcount++;

	lam_resetfunc_m(BLKMPIERRSET);
	return(MPI_SUCCESS);
}
