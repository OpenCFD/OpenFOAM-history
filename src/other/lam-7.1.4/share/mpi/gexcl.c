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
 * $Id: gexcl.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form new group by exclusion
 *	Accepts:	- original group
 *			- # of processes to exclude
 *			- ranks
 *			- ptr new group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>

/*@

MPI_Group_excl - Produces a group by reordering an existing group and
taking only unlisted members

Input Parameters:
+ g - group (handle) 
. n - number of elements in array 'ranks' (integer) 
- ranks - array of integer ranks in 'g' not to appear in 'png' 

Output Parameter:
. png - new group derived from above, preserving the order defined by
'group' (handle)

Notes:

Each of the ranks to exclude must be a valid rank in the group and all
elements must be distinct or the function is erroneous.

When a group is no longer being used, it should be freed with
'MPI_Group_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG
.N MPI_ERR_RANK

.seealso: MPI_Group_free

.N ACK
@*/
int MPI_Group_excl(MPI_Group g, int n, int *ranks, 
		   MPI_Group *png)
{
	int		i, j;
	int		*r;
	int		nproc;			/* # processes */
	struct _proc	**pi, **pj;		/* processes in groups */
	MPI_Group	ng;			/* new group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPEXCL);
/*
 * Check the arguments.
 */
	if (g == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPEXCL, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if ((png == 0) || ((n > 0) && (ranks == 0))
			|| (n < 0) || (n > g->g_nprocs)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPEXCL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (n == 0) {
		*png = g;
		g->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPEXCL);
		return(MPI_SUCCESS);
	}
/*
 * Get the # of processes in the new group.
 * Flag the excluded processes.
 */
	nproc = g->g_nprocs - n;
	pi = g->g_procs;

	for (r = ranks, i = 0; i < n; ++i, ++r) {

		if ((*r < 0) || (*r >= g->g_nprocs)
				|| (pi[*r]->p_mode & LAM_PFLAG)) {
			
			for (r = ranks, j = 0; j < i; ++j, ++r) {
				pi[*r]->p_mode &= ~LAM_PFLAG;
			}
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPEXCL,
					   lam_mkerr(MPI_ERR_RANK, EINVAL)));
		} else {
			pi[*r]->p_mode |= LAM_PFLAG;
		}
	}
/*
 * Handle the empty difference case.
 */
	if (nproc == 0) {
		*png = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;

		for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		lam_resetfunc_m(BLKMPIGROUPEXCL);
		return(MPI_SUCCESS);
	}
/*
 * Allocate a new group.
 */
	ng = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(nproc * sizeof(struct _proc *)));
	if (ng == 0) {
		for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPEXCL,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*png = ng;
	ng->g_nprocs = nproc;
	ng->g_myrank = MPI_UNDEFINED;
	ng->g_refcount = 1;
	ng->g_f77handle = -1;
	ng->g_procs = (struct _proc **) ((char *) ng + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	pj = ng->g_procs;

	for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {

		if ((*pi)->p_mode & LAM_PFLAG) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		} else {
			if (*pi == lam_myproc) {
				ng->g_myrank = (int) (pj - ng->g_procs);
			}
			(*pi)->p_refcount++;
			*pj = *pi;
			++pj;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPEXCL);
	return(MPI_SUCCESS);
}
