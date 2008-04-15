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
 * $Id: tgetconts.c,v 1.5 2002/12/16 16:23:31 jsquyres Exp $
 *
 *	Function:	- get datatype contents
 *	Accepts:	- MPI datatype
 *			- number of integers
 *			- number of addresses
 *			- number of datatypes
 *			- array of integers (out)
 *			- array of addresses (out)
 *			- array of datatypes (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Type_get_contents - Get datatype contents

Input Parameter:
+ type - Datatype to access
. nints - Number of elements in ints
. naddrs - Number of elments in addrs
- ndtypes - Number of elements in dtypes

Output Parameters:
+ ints - Contains integer arguments used in constructing datatype 
. addrs - Contains address arguments used in constructing datatype
- dtypes - Contains datatype arguments used in constructing datatype

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.seealso MPI_Type_get_envelope(3)

.N WEB
@*/

int
MPI_Type_get_contents(MPI_Datatype type, int nints, int naddrs, 
		      int ndtypes, int *ints, MPI_Aint *addrs, 
		      MPI_Datatype *dtypes)
{
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITGETCONT);
/*
 * Check arguments.
 */
	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETCONT,
					lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (dtypes == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETCONT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (type->dt_flags & LAM_DTISDUP) {
	    dtypes[0] = (MPI_Datatype) type->dt_uargs;
	    dtypes[0]->dt_refcount++;
	}
	else if (type->dt_flags & LAM_DTISRESIZE) {
	    dtypes[0] = (MPI_Datatype) type->dt_uargs;
	    addrs[0] = type->dt_lower;
	    addrs[1] = type->dt_upper - type->dt_lower;
	    dtypes[0]->dt_refcount++;
	}
	else {
	    switch (type->dt_format) {

	    case LAM_DTBASIC:
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETCONT,
		    			lam_mkerr(MPI_ERR_TYPE, EINVAL)));

	    case LAM_DTCONTIG:
		dtypes[0] = type->dt_dtype;
		ints[0] = type->dt_count;
		dtypes[0]->dt_refcount++;
		break;

	    case LAM_DTSTRUCT:
	    case LAM_DTSTRUCTCREAT:
		ints[0] = type->dt_count;
		for (i = 0; i < ints[0]; i++) {
		    ints[i + 1] = type->dt_lengths[i];
		    addrs[i] = type->dt_disps[i];
		    dtypes[i] = type->dt_dtypes[i];
		    dtypes[i]->dt_refcount++;
		}
		break;

	    case LAM_DTINDEXED:
		dtypes[0] = type->dt_dtype;
		dtypes[0]->dt_refcount++;
		ints[0] = type->dt_count;
		for (i = 0; i < ints[0]; i++) {
		    ints[i + 1] = type->dt_lengths[i];
		    ints[ints[0] + 1 + i] = (int) type->dt_disps[i];
		}
		break;

	    case LAM_DTHINDEXED:
	    case LAM_DTHINDEXEDCREAT:
		dtypes[0] = type->dt_dtype;
		ints[0] = type->dt_count;
		for (i = 0; i < ints[0]; i++) {
		    ints[i + 1] = type->dt_lengths[i];
		    addrs[i] = type->dt_disps[i];
		}
		break;

	    case LAM_DTVECTOR:
		dtypes[0] = type->dt_dtype;
		dtypes[0]->dt_refcount++;
		ints[0] = type->dt_count;
		ints[1] = type->dt_length;
		ints[2] = type->dt_stride;
		break;

	    case LAM_DTHVECTOR:
	    case LAM_DTHVECTORCREAT:
		dtypes[0] = type->dt_dtype;
		dtypes[0]->dt_refcount++;
		ints[0] = type->dt_count;
		ints[1] = type->dt_length;
		addrs[0] = type->dt_stride;
		break;

	    case LAM_DTSUBARRAY:
		dtypes[0] = type->dt_dtype;
		dtypes[0]->dt_refcount++;
		lam_memcpy((char *) ints, (char *) type->dt_uargs, 
			   (3 * type->dt_uargs[0] + 2) * sizeof(int));
		break;

	    case LAM_DTDARRAY:
		dtypes[0] = type->dt_dtype;
		dtypes[0]->dt_refcount++;
		lam_memcpy((char *) ints, (char *) type->dt_uargs, 
			   (4 * type->dt_uargs[2] + 4) * sizeof(int));
		break;

	    case LAM_DTINDEXEDBLK:
	    default:
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITGETCONT,
		    			lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	    }
	}

	lam_resetfunc_m(BLKMPITGETCONT);
	return(MPI_SUCCESS);
}
