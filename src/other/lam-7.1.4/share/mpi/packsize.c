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
 * $Id: packsize.c,v 6.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- get size needed to pack data
 *	Accepts:	- count
 *			- datatype
 *			- communicator
 *			- ptr to size
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Pack_size - Returns the upper bound on the amount of space needed
to pack a message

Input Parameters:
+ count - count argument to packing call (integer) 
. dtype - datatype argument to packing call (handle) 
- comm - communicator argument to packing call (handle) 

Output Parameter:
. psize - upper bound on size of packed message, in bytes (integer) 

Notes: 

The MPI standard document describes this in terms of 'MPI_Pack', but
it applies to both the 'MPI_Pack' and 'MPI_Unpack' functions.  That
is, the value 'size' is the maximum that is needed by either
'MPI_Pack' or 'MPI_Unpack'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

.seealso MPI_Pack, MPI_Unpack

.N ACK
@*/
int MPI_Pack_size(int count, MPI_Datatype dtype, 
		  MPI_Comm comm, int *psize)
{
	lam_initerr();
	lam_setfunc(BLKMPIPACKSIZE);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIPACKSIZE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(comm,
			BLKMPIPACKSIZE, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(comm,
			BLKMPIPACKSIZE, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (psize == 0) {
		return(lam_errfunc(comm,
			BLKMPIPACKSIZE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Return the size.
 */
	*psize = count * dtype->dt_size;

	lam_resetfunc(BLKMPIPACKSIZE);
	return(MPI_SUCCESS);
}
