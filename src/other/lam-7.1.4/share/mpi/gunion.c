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
 * $Id: gunion.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form the union of two groups
 *	Accepts:	- first group
 *			- second group
 *			- ptr union group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>

/*@

MPI_Group_union - Produces a group by combining two groups

Input Parameters:
+ g1 - first group (handle) 
- g2 - second group (handle) 

Output Parameter:
. pgu - union group (handle) 

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
int MPI_Group_union(MPI_Group g1, MPI_Group g2, 
		    MPI_Group *pgu)
{
	int		i, j;			/* favourite indices */
	int		nproc;			/* # procs in union */
	struct _proc	**pi, **pj;		/* favourite pointers */
	MPI_Group	gu;			/* union group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPUNION);
/*
 * Check the arguments.
 */
	if ((g1 == MPI_GROUP_NULL) || (g2 == MPI_GROUP_NULL)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPUNION, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (pgu == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPUNION, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (g1->g_nprocs == 0) {
		*pgu = g2;
		g2->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPUNION);
		return(MPI_SUCCESS);
	}

	if (g2->g_nprocs == 0) {
		*pgu = g1;
		g1->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPUNION);
		return(MPI_SUCCESS);
	}
/*
 * Get the # of processes in the union.
 * Flag the duplicate processes in second group.
 */
	nproc = g1->g_nprocs + g2->g_nprocs;

	for (i = 0, pi = g2->g_procs; i < g2->g_nprocs; ++i, ++pi) {

		for (j = 0, pj = g1->g_procs; j < g1->g_nprocs; ++j, ++pj) {

			if (*pi == *pj) {
				(*pi)->p_mode |= LAM_PFLAG;
				--nproc;
				break;
			}
		}
	}
/*
 * Allocate a new group.
 */
	gu = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(nproc * sizeof(struct _proc *)));
	if (gu == 0) {
		for (i = 0, pi = g2->g_procs; i < g2->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPUNION,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*pgu = gu;
	gu->g_myrank = MPI_UNDEFINED;
	gu->g_nprocs = nproc;
	gu->g_refcount = 1;
	gu->g_f77handle = -1;
	gu->g_procs = (struct _proc **) ((char *) gu + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	memcpy((char *) gu->g_procs, (char *) g1->g_procs,
				g1->g_nprocs * sizeof(struct _proc *));

	pj = gu->g_procs + g1->g_nprocs;

	for (i = 0, pi = g2->g_procs; i < g2->g_nprocs; ++i, ++pi) {

		if ((*pi)->p_mode & LAM_PFLAG) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		} else {
			*pj = *pi;
			++pj;
		}
	}

	for (i = 0, pi = gu->g_procs; i < gu->g_nprocs; ++i, ++pi) {
		(*pi)->p_refcount++;
		if (*pi == lam_myproc) {
			gu->g_myrank = i;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPUNION);
	return(MPI_SUCCESS);
}
