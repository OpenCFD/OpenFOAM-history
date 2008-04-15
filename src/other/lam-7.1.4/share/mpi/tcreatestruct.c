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
 * $Id: tcreatestruct.c,v 1.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create extended struct derived datatype
 *	Accepts:	- # blocks
 *			- block length array
 *			- block displacement array (in bytes)
 *			- block element datatype array
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_create_struct - Create extended struct derived datatype

Input Parameters:
+ count - Number of blocks (nonnegative integer)
. lengths - Number of elements in each block (array of integer)
. displacement - Byte displacement of each block (array of integer)
- oldtypes - Type of elements in each block (array of handles to datatype objects)

Output Parameter:
+ newtype - New datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_OTHER

.seealso MPI_Type_create_hvector(3), MPI_Type_create_hindexed(3), MPI_Get_address(3)

.N WEB
@*/


int
MPI_Type_create_struct(int count, int *lengths, MPI_Aint *disps,
		       MPI_Datatype *oldtypes, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITCREATESTRUCT);
/*
 * Check the arguments.
 */
	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
				   lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((newtype == 0) || ((count > 0) &&
		((lengths == 0) || (disps == 0) || (oldtypes == 0)))) {

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	for (i = 0; i < count; ++i) {

		if (oldtypes[i] == MPI_DATATYPE_NULL) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
					   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
		}

		if (lengths[i] < 0) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
					   lam_mkerr(MPI_ERR_ARG, EINVAL)));
		}
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
			lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	LAM_ZERO_ME(*ntype);
	lam_type_setdefaults(ntype);

	if (count > 0) {
		ntype->dt_dtypes = (MPI_Datatype *) malloc((unsigned)
				(count * (sizeof(MPI_Datatype)
				+ sizeof(MPI_Aint) + sizeof(int))));

		if (ntype->dt_dtypes == 0) {
			free((char *) ntype);
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		ntype->dt_disps = (MPI_Aint *) (((char *) ntype->dt_dtypes) +
			(count * sizeof(MPI_Datatype)));
		ntype->dt_lengths = (int *) (((char *) ntype->dt_disps) +
			(count * sizeof(MPI_Aint)));
	} else {
		ntype->dt_flags = LAM_DTNOPACK | LAM_DTNOXADJ;
		ntype->dt_size = 0;
	}

	ntype->dt_format = LAM_DTSTRUCTCREAT;
	ntype->dt_count = count;
/*
 * Compute the extent, # elements, and data size.
 */
	for (i = 0; i < count; ++i) {

		ntype->dt_disps[i] = disps[i];
		ntype->dt_lengths[i] = lengths[i];
		ntype->dt_dtypes[i] = oldtypes[i];
		oldtypes[i]->dt_refcount++;

		lam_dtblock(ntype, oldtypes[i], lengths[i], disps[i]);
	}

	if (ntype->dt_size > 0) {
		if (ntype->dt_datalow != 0 || ntype->dt_lower != 0) {
			ntype->dt_flags &= ~LAM_DTNOPACK;
		}

		lam_dtalign(ntype);
	}

	*newtype = ntype;

	lam_resetfunc_m(BLKMPITCREATESTRUCT);
	return(MPI_SUCCESS);
}
