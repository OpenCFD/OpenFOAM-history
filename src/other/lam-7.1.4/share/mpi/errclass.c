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
 * $Id: errclass.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get error class of error code
 *	Accepts:	- error code
 *			- ptr error class (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Error_class - Converts an error code into an error class

Input Parameter:
. errcode - Error code returned by an MPI routine 

Output Parameter:
. class - Error class associated with 'errcode'

.N fortran

.N Errors
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Error_class(int errcode, int *class)
{
	int		func;
	int		error;

	lam_initerr();
	lam_setfunc(BLKMPIERRCLASS);
/*
 * Check the arguments.
 */
	if (class == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRCLASS, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_bkerr(errcode, class, &func, &error);

	lam_resetfunc(BLKMPIERRCLASS);
	return(MPI_SUCCESS);
}
