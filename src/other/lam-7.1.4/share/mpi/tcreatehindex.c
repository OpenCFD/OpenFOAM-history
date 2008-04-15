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
 * $Id: tcreatehindex.c,v 1.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- create extended hindexed derived datatype
 *	Accepts:	- # blocks
 *			- block length array
 *			- block displacement array (in bytes)
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_create_hindexed - Create extended hindexed derived datatype

Input Parameters:
+ count - Number of blocks
. lengths - Number of elements in each block (array of nonnegative integers) 
. displacements - Byte displacement of each block (array of integer)
- oldtype - Old datatype (handle)

Output Parameter:
+ newtype - New datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_OTHER

.seealso MPI_Type_create_hvector(3), MPI_Type_create_struct(3), MPI_Get_address(3)

.N WEB
@*/

int
MPI_Type_create_hindexed(int count, int *lengths, MPI_Aint *disps,
			 MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */
	int		i;			/* favourite index */

	lam_initerr();
	lam_setfunc(BLKMPITCREATEHINDEX);
/*
 * Check the arguments.
 */
	if (oldtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHINDEX, 
				   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHINDEX, 
				   lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((newtype == 0) ||
			((count > 0) && ((lengths == 0) || (disps == 0)))) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHINDEX, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCREATEHINDEX, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	LAM_ZERO_ME(*ntype);
	lam_type_setdefaults(ntype);

	if (count > 0) {
		ntype->dt_disps = (MPI_Aint *) malloc((unsigned) (count *
					(sizeof(MPI_Aint) + sizeof(int))));

		if (ntype->dt_disps == 0) {
			free((char *) ntype);
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEHINDEX,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		ntype->dt_lengths = (int *) (((char *) ntype->dt_disps) +
						(count * sizeof(MPI_Aint)));
	}

	for (i = 0; i < count; ++i) {
		ntype->dt_disps[i] = disps[i];

		if ((ntype->dt_lengths[i] = lengths[i]) < 0) {
			free((char *) ntype->dt_disps);
			free((char *) ntype);
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEHINDEX,
					   lam_mkerr(MPI_ERR_ARG, EINVAL)));
		}
	}

	ntype->dt_format = LAM_DTHINDEXEDCREAT;
	ntype->dt_count = count;
	ntype->dt_dtype = oldtype;
	oldtype->dt_refcount++;

	if (count > 0) {
		for (i = 0; i < count; ++i) {
			lam_dtblock(ntype, oldtype, lengths[i], disps[i]);
		}

		if (ntype->dt_size > 0 && ntype->dt_lower != 0) {
			ntype->dt_flags &= ~LAM_DTNOPACK;
		}

		lam_dtalign(ntype);
	}
	else {
		ntype->dt_flags = LAM_DTNOPACK | LAM_DTNOXADJ;
		ntype->dt_size = 0;
	}

	*newtype = ntype;

	lam_resetfunc(BLKMPITCREATEHINDEX);
	return(MPI_SUCCESS);
}
