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
 * $Id: gdiff.c,v 6.6 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- form the difference of two groups
 *	Accepts:	- first group
 *			- second group
 *			- ptr difference group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>

/*@

MPI_Group_difference - Makes a group from the difference of two groups

Input Parameters:
+ g1 - first group (handle) 
- g2 - second group (handle) 

Output Parameter:
. pgd - difference group (handle) 

Notes:

When a group is no longer being used, it should be freed with
'MPI_Group_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Group_free

.N ACK
@*/
int MPI_Group_difference(MPI_Group g1, MPI_Group g2, 
			 MPI_Group *pgd)
{
	int		i, j;			/* favourite indices */
	int		nproc;			/* # procs in difference */
	struct _proc	**pi, **pj;		/* favourite pointers */
	MPI_Group	gd;			/* difference group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPDIFF);
/*
 * Check the arguments.
 */
	if ((g1 == MPI_GROUP_NULL) || (g2 == MPI_GROUP_NULL)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPDIFF, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (pgd == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPDIFF, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if ((g1->g_nprocs == 0) || (g2->g_nprocs == 0)) {
		*pgd = g1;
		g1->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPDIFF);
		return(MPI_SUCCESS);
	}
/*
 * Get the # of processes in the difference.
 * Flag the processes common to both groups.
 */
	nproc = g1->g_nprocs;

	for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {

		for (j = 0, pj = g2->g_procs; j < g2->g_nprocs; ++j, ++pj) {

			if (*pi == *pj) {
				(*pi)->p_mode |= LAM_PFLAG;
				--nproc;
				break;
			}
		}
	}
/*
 * Handle the empty difference case.
 */
	if (nproc == 0) {
		*pgd = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;

		for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		lam_resetfunc_m(BLKMPIGROUPDIFF);
		return(MPI_SUCCESS);
	}
/*
 * Allocate a new group.
 */
	gd = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(nproc * sizeof(struct _proc *)));
	if (gd == 0) {
		for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPDIFF,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*pgd = gd;
	gd->g_nprocs = nproc;
	gd->g_myrank = MPI_UNDEFINED;
	gd->g_refcount = 1;
	gd->g_f77handle = -1;
	gd->g_procs = (struct _proc **) ((char *) gd + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	pj = gd->g_procs;

	for (i = 0, pi = g1->g_procs; i < g1->g_nprocs; ++i, ++pi) {

		if ((*pi)->p_mode & LAM_PFLAG) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		} else {
			if (*pi == lam_myproc) {
				gd->g_myrank = (int) (pj - gd->g_procs);
			}
			(*pi)->p_refcount++;
			*pj = *pi;
			++pj;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPDIFF);
	return(MPI_SUCCESS);
}
