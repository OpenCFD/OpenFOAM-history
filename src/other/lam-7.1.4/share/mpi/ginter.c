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
 * $Id: ginter.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form the intersection of two groups
 *	Accepts:	- first group
 *			- second group
 *			- ptr intersection group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>

/*@

MPI_Group_intersection - Produces a group as the intersection of two existing
                         groups

Input Parameters:
+ g1 - first group (handle) 
- g2 - second group (handle) 

Output Parameter:
. pgi - intersection group (handle) 

Notes:

When a group is no longer being used, it should be freed with
'MPI_Group_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_ARG

.seealso: MPI_Group_free

.N ACK
@*/
int MPI_Group_intersection(MPI_Group g1, MPI_Group g2, 
			   MPI_Group *pgi)
{
	int		i, j;			/* favourite indices */
	int		nproc;			/* # procs in inter. */
	struct _proc	**pi, **pj;		/* favourite pointers */
	MPI_Group	gi;			/* intersection group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPINTER);
/*
 * Check the arguments.
 */
	if ((g1 == MPI_GROUP_NULL) || (g2 == MPI_GROUP_NULL)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPINTER, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (pgi == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPINTER, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (g1->g_nprocs == 0) {
		*pgi = g1;
		g1->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPINTER);
		return(MPI_SUCCESS);
	}

	if (g2->g_nprocs == 0) {
		*pgi = g2;
		g2->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPINTER);
		return(MPI_SUCCESS);
	}
/*
 * Get the # of processes in the intersection.
 * Flag the processes common to both groups.
 */
	nproc = 0;

	for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {

		for (j = 0, pj = g2->g_procs; j < g2->g_nprocs; ++j, ++pj) {

			if (*pi == *pj) {
				(*pi)->p_mode |= LAM_PFLAG;
				++nproc;
				break;
			}
		}
	}
/*
 * Handle the empty intersection case.
 */
	if (nproc == 0) {
		*pgi = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPINTER);
		return(MPI_SUCCESS);
	}
/*
 * Allocate a new group.
 */
	gi = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(nproc * sizeof(struct _proc *)));
	if (gi == 0) {
		for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPINTER,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*pgi = gi;
	gi->g_nprocs = nproc;
	gi->g_myrank = MPI_UNDEFINED;
	gi->g_refcount = 1;
	gi->g_f77handle = -1;
	gi->g_procs = (struct _proc **) ((char *) gi + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	pj = gi->g_procs;

	for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {

		if ((*pi)->p_mode & LAM_PFLAG) {
			if (*pi == lam_myproc) {
				gi->g_myrank = (int) (pj - gi->g_procs);
			}
			(*pi)->p_refcount++;
			(*pi)->p_mode &= ~LAM_PFLAG;
			*pj = *pi;
			++pj;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPINTER);
	return(MPI_SUCCESS);
}
