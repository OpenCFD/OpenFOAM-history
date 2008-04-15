/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: getelem.c,v 6.8 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- get # bottom-level elements
 *	Accepts:	- status
 *			- datatype
 *			- count (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*
 * private functions
 */
static int		subcount(int left, MPI_Datatype dtype);


/*@
  MPI_Get_elements - Returns the number of basic elements
                     in a datatype

Input Parameters:
+ stat - return status of receive operation (Status) 
- dtype - datatype used by receive operation (handle) 

Output Parameter:
. count - number of received basic elements (integer) 

Notes:

It is `not` legal to pass 'MPI_STATUS_IGNORE' as 'stat' to this
function.

'count' will be set to 'MPI_UNDEFINED' if the size of the datatype is
zero.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.N ACK
@*/
int MPI_Get_elements(MPI_Status *stat, MPI_Datatype dtype, 
		     int *count)
{
	int		elts;			/* # basic elements */
	int		integral;		/* # integral dtype copies */
	int		left;			/* packed data length left */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGETELEM);
/*
 * Check the arguments.
 */
	if ((count == 0) || (stat == 0) || (stat->st_length < 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGETELEM, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGETELEM, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
/*
 * Compute the element count.
 */ 
	if (dtype->dt_size == 0) {
		*count = MPI_UNDEFINED;
	}
	else if (dtype->dt_format == LAM_DTBASIC) {
		*count = ((stat->st_length % dtype->dt_size) == 0)
			? stat->st_length / dtype->dt_size : MPI_UNDEFINED;
	} else {
		integral = stat->st_length / dtype->dt_size;
		*count = integral * dtype->dt_nelem;
		left = stat->st_length - integral * dtype->dt_size;

		if (left) {
			elts = subcount(left, dtype);

			if (elts == MPI_UNDEFINED) {
				*count = MPI_UNDEFINED;
			} else {
				*count += elts;
			}
		}
	}

	lam_resetfunc_m(BLKMPIGETELEM);
	return(MPI_SUCCESS);
}

/*
 *	subcount
 *
 *	Function:	- get count of bottom level elements in
 *			  fractional part of a datatype
 *			- counting is done recursively
 *	Accepts:	- packed length left
 *			- datatype
 *	Returns:	- count of bottom level elements
 */
static int
subcount(int left, MPI_Datatype dtype)
{
	int		i;
	int		elts;			/* # basic elements */
	int		totelts = 0;		/* total # basic elements */
	int		integral;		/* # integral subtype copies */
	int		blksize;		/* size of subtype block */

	if (dtype->dt_format == LAM_DTBASIC) {
		
	    if (left % dtype->dt_size) {
		totelts = MPI_UNDEFINED;
	    } else {
		totelts += left / dtype->dt_size;
	    }
	}
	else if (dtype->dt_format == LAM_DTSTRUCT
	        || dtype->dt_format == LAM_DTSTRUCTCREAT
	    	|| dtype->dt_format == LAM_DTDARRAY
	    	|| dtype->dt_format == LAM_DTSUBARRAY) {
/*
 * Count sub blocks one by one.
 */
	    for (i = 0; i < dtype->dt_count && left > 0; ++i) {

		blksize = dtype->dt_lengths[i] * dtype->dt_dtypes[i]->dt_size;
		if (blksize == 0) {
		    continue;
		}

		if (left >= blksize) {
		    left -= blksize;
		    totelts += dtype->dt_lengths[i]
				* dtype->dt_dtypes[i]->dt_nelem;
		} else {
/*
 * There is a partial sub block.  Determine the number of elements in
 * the integral sub parts and then recursively count the fractional part
 * that is left over.
 */
		    integral =  left / dtype->dt_dtypes[i]->dt_size;
		    totelts += integral * dtype->dt_dtypes[i]->dt_nelem;
		    left -= integral * dtype->dt_dtypes[i]->dt_size;

		    if (left) {
			elts = subcount(left, dtype->dt_dtypes[i]);

			if (elts == MPI_UNDEFINED) {
			    totelts = MPI_UNDEFINED;
			} else {
			    totelts += elts;
			}
		    }

		    break;
		}
	    }
	}
	else {
/*
 * Type is vector or indexed.  This means that all the next level
 * elements are of the same subtype.
 */
	    integral = left / dtype->dt_dtype->dt_size;
	    totelts += integral * dtype->dt_dtype->dt_nelem;
	    left -= integral * dtype->dt_dtype->dt_size;

	    if (left) {
		elts = subcount(left, dtype->dt_dtype);

		if (elts == MPI_UNDEFINED) {
		    totelts = MPI_UNDEFINED;
		} else {
		    totelts += elts;
		}
	    }
	}

	return(totelts);
}
