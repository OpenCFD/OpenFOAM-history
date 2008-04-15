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
 * $Id: tgetenvl.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- get datatype envelope counts
 *	Accepts:	- MPI datatype
 *			- number of integers (out)
 *			- number of addresses (out)
 *			- number of datatypes (out)
 *			- combiner (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Type_get_envelope - Get datatype envelope contents

Input Parameter:
+ type - Datatype to access

Output Parameters:
+ nints - Number of elements in ints
. naddrs - Number of elments in addrs
. ndtypes - Number of elements in dtypes
- combiner - Combiner reflecting the MPI datatype constructor call that was used in creating datatype

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.seealso MPI_Type_get_contents(3)

.N WEB
@*/


int
MPI_Type_get_envelope(MPI_Datatype dtype, int *nints, int *naddrs, 
		      int *ndtypes, int *combiner)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPITGETENV);
/*
 * Check arguments.
 */
	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETENV,
					lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (nints == 0 || naddrs == 0 || ndtypes == 0 || combiner == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETENV,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (dtype->dt_flags & LAM_DTISDUP) {
		*nints = 0;
		*naddrs = 0;
		*ndtypes = 1;
		*combiner = MPI_COMBINER_DUP;
	}
	else if (dtype->dt_flags & LAM_DTISRESIZE) {
		*nints = 0;
		*naddrs = 2;
		*ndtypes = 1;
		*combiner = MPI_COMBINER_RESIZED;
	}
	else {
		switch (dtype->dt_format) {

		case LAM_DTBASIC:
			*nints = 0;
			*naddrs = 0;
			*ndtypes = 0;
			*combiner = MPI_COMBINER_NAMED;
			break;

		case LAM_DTCONTIG:
			*nints = 1;
			*naddrs = 0;
			*ndtypes = 1;
			*combiner = MPI_COMBINER_CONTIGUOUS;
			break;

		case LAM_DTSTRUCT:
		case LAM_DTSTRUCTCREAT:
			*nints = dtype->dt_count + 1;
			*naddrs = dtype->dt_count;
			*ndtypes = dtype->dt_count;
			if ((dtype->dt_format == LAM_DTSTRUCT)
					&& (dtype->dt_flags & LAM_LANGF77)) {
				*combiner = MPI_COMBINER_STRUCT_INTEGER;
			} else {
				*combiner = MPI_COMBINER_STRUCT;
			}
			break;

		case LAM_DTINDEXED:
			*nints = 2 * dtype->dt_count + 1;
			*naddrs = 0;
			*ndtypes = 1;
			*combiner = MPI_COMBINER_INDEXED;
			break;

		case LAM_DTHINDEXED:
		case LAM_DTHINDEXEDCREAT:
			*nints = dtype->dt_count + 1;
			*naddrs = dtype->dt_count;
			*ndtypes = 1;
			if ((dtype->dt_format == LAM_DTHINDEXED)
					&& (dtype->dt_flags & LAM_LANGF77)) {
				*combiner = MPI_COMBINER_HINDEXED_INTEGER;
			} else {
				*combiner = MPI_COMBINER_HINDEXED;
			}
			break;

		case LAM_DTVECTOR:
			*nints = 3;
			*naddrs = 0;
			*ndtypes = 1;
			*combiner = MPI_COMBINER_VECTOR;
			break;

		case LAM_DTHVECTOR:
		case LAM_DTHVECTORCREAT:
			*nints = 2;
			*naddrs = 1;
			*ndtypes = 1;
			if ((dtype->dt_format == LAM_DTHVECTOR)
					&& (dtype->dt_flags & LAM_LANGF77)) {
				*combiner = MPI_COMBINER_HVECTOR_INTEGER;
			} else {
				*combiner = MPI_COMBINER_HVECTOR;
			}
			break;

		case LAM_DTSUBARRAY:
			*nints = 3 * dtype->dt_uargs[0] + 2;
			*naddrs = 0;
			*ndtypes = 1;
			*combiner = MPI_COMBINER_SUBARRAY;
			break;

		case LAM_DTDARRAY:
			*nints = 4 * dtype->dt_uargs[2] + 4;
			*naddrs = 0;
			*ndtypes = 1;
			*combiner = MPI_COMBINER_DARRAY;
			break;

		case LAM_DTINDEXEDBLK:
		default:
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETENV,
					   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
		}
	}

	lam_resetfunc_m(BLKMPITGETENV);
	return(MPI_SUCCESS);
}
