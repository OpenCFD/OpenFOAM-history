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
 * $Id: getcount.c,v 6.8 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get # top-level elements
 *	Accepts:	- status
 *			- datatype
 *			- count (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
  MPI_Get_count - Gets the number of "top level" elements

Input Parameters:
+ stat - return status of receive operation (Status) 
- dtype - datatype of each receive buffer element (handle) 

Output Parameter:
. count - number of received elements (integer) 

Notes:

It is `not` legal to pass 'MPI_STATUS_IGNORE' as 'stat' to this
function.

If the size of the datatype is zero, this routine will return a count
of zero.  If the amount of data in 'status' is not an exact multiple
of the size of 'datatype' (so that 'count' would not be integral), a
'count' of 'MPI_UNDEFINED' is returned instead.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.N ACK
@*/
int MPI_Get_count(MPI_Status *stat, MPI_Datatype dtype, 
		  int *count)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIGETCOUNT);
/*
 * Check the arguments.
 */
	if ((count == 0) || (stat == 0) || (stat->st_length < 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGETCOUNT, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGETCOUNT, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
/*
 * Find the count.
 */
	if (dtype->dt_size == 0) {
		*count = (stat->st_length == 0) ? 0 : MPI_UNDEFINED;
	}
	else if (stat->st_length % dtype->dt_size != 0) {
		*count = MPI_UNDEFINED;
	}
	else {
		*count = stat->st_length / dtype->dt_size;
	}

	lam_resetfunc_m(BLKMPIGETCOUNT);
	return(MPI_SUCCESS);
}
