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
 * $Id: tindex.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create indexed derived datatype
 *	Accepts:	- # blocks
 *			- block length array
 *			- block displacement array
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Type_indexed - Creates an indexed datatype

Input Parameters:
+ count - number of blocks -- also number of entries in indices and blocklens
. lengths - number of elements in each block (array of nonnegative integers) 
. disps - displacement of each block in multiples of old_type (array of 
  integers)
- oldtype - old datatype (handle) 

Output Parameter:
. newtype - new datatype (handle) 

Notes:

The indices are displacements, and are based on a zero origin.  A common error
is to do something like to following

.vb
    integer a(100)
    integer blens(10), indices(10)
    do i=1,10
         blens(i)   = 1
10       indices(i) = 1 + (i-1)*10
    call MPI_TYPE_INDEXED(10,blens,indices,MPI_INTEGER,newtype,ierr)
    call MPI_TYPE_COMMIT(newtype,ierr)
    call MPI_SEND(a,1,newtype,...)
.ve

expecting this to send 'a(1),a(11),...' because the indices have
values '1,11,...' (and so on).  Because these are `disps` from the
beginning of 'a', it actually sends 'a(1+1),a(1+11),...'.

If you wish to consider the displacements as indices into a Fortran
array, consider declaring the Fortran array with a zero origin

.vb
    integer a(0:99)
.ve

.N fortran

.N Errors
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.N ACK
@*/
int MPI_Type_indexed(int count, int *lengths, int *disps, 
		     MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */
	int		oldext;			/* extent of oldtype */
	int		i;

	lam_initerr();
	lam_setfunc(BLKMPITINDEX);
/*
 * Check the arguments.
 */
	if (oldtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITINDEX, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITINDEX, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((newtype == 0) ||
			((count > 0) && ((lengths == 0) || (disps == 0)))) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITINDEX, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITINDEX, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	lam_type_setdefaults(ntype);

	if (count > 0) {
		ntype->dt_disps = (MPI_Aint *) malloc((unsigned) (count *
					(sizeof(MPI_Aint) + sizeof(int))));

		if (ntype->dt_disps == 0) {
			free((char *) ntype);
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITINDEX,
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
			return(lam_errfunc(MPI_COMM_WORLD,
					   BLKMPITINDEX, 
					   lam_mkerr(MPI_ERR_ARG, EINVAL)));
		}
	}

	ntype->dt_format = LAM_DTINDEXED;
	ntype->dt_count = count;
	ntype->dt_dtype = oldtype;
	oldtype->dt_refcount++;

	if (count > 0) {
		oldext = oldtype->dt_upper - oldtype->dt_lower;
		
		for (i = 0; i < count; ++i) {
			lam_dtblock(ntype, oldtype, lengths[i],
					disps[i] * oldext);
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

	lam_resetfunc(BLKMPITINDEX);
	return(MPI_SUCCESS);
}
