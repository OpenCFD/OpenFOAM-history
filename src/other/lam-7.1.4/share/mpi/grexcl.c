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
 * $Id: grexcl.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form a new group by range exclusion
 *	Accepts:	- original group
 *			- # of ranges
 *			- rank ranges
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

MPI_Group_range_excl - Produces a group by excluding ranges of processes from
       an existing group

Input Parameters:
+ group - group (handle) 
. n - number of elements in array 'ranges' (integer) 
- ranges - a one-dimensional array of integer triplets of the form
(first rank, last rank, stride), indicating the ranks in 'g' of
processes to be excluded from the output group 'png' .

Output Parameter:
. png - new group derived from above, preserving the order in 'group'
(handle)

Note:  

Each of the ranks to exclude must be a valid rank in the group and all
elements must be distinct or the function is erroneous.

When a group is no longer being used, it should be freed with
'MPI_Group_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_RANK
.N MPI_ERR_ARG

.seealso: MPI_Group_free

.N ACK
@*/
int MPI_Group_range_excl(MPI_Group g, int n, 
			 int ranges[][3], MPI_Group *png)
{
	int		i, j;
	int		nproc;			/* # processes */
	int		first;			/* range first */
	int		last;			/* range last */
	int		stride;			/* range stride */
	int		end;			/* calculated end of range */
	struct _proc	**pi, **pj;		/* processes in groups */
	MPI_Group	ng;			/* new group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPREXCL);
/*
 * Check the arguments.
 */
	if (g == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPREXCL, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if ((png == 0) || (n < 0) || ((n > 0) && (ranges == 0))) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPREXCL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Flag the processes to exclude.
 */
	nproc = g->g_nprocs;
	pi = g->g_procs;

	for (i = 0; i < n; ++i) {

		if ((stride = ranges[i][2]) == 0) break;

		first = ranges[i][0];
		last = ranges[i][1];
		end = first + ((last - first) / stride) * stride;
		
		if (stride > 0) {
			if (first < 0 || first > last || end >= g->g_nprocs) {
				break;
			}

			for (j = first; j <= end; j += stride) {

				if (!(pi[j]->p_mode & LAM_PFLAG)) {
					pi[j]->p_mode |= LAM_PFLAG;
					--nproc;
				} else {
					i = -1;
					break;
				}
			}
		}
		else {
			if (first >= g->g_nprocs || first < last || end < 0) {
				break;
			}
		
			for (j = first; j >= end; j += stride) {

				if (!(pi[j]->p_mode & LAM_PFLAG)) {
					pi[j]->p_mode |= LAM_PFLAG;
					--nproc;
				} else {
					i = -1;
					break;
				}
			}
		}

		if (i < 0) break;
	}
/*
 * If there is an error in the rank ranges clean up and return error.
 */
	if (i < n) {
		for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPREXCL,
					lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (nproc == g->g_nprocs) {
		*png = g;
		g->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPREXCL);
		return(MPI_SUCCESS);
	}

	if (nproc == 0) {
		*png = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;

		for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		}

		lam_resetfunc_m(BLKMPIGROUPREXCL);
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

		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPREXCL, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*png = ng;
	ng->g_nprocs = nproc;
	ng->g_myrank = MPI_UNDEFINED;
	ng->g_refcount = 1;
	ng->g_f77handle = -1;
	ng->g_procs = (struct _proc **) ((char *) ng + sizeof(struct _group));
/*
 * Fill the new group ranks.
 */
	pj = ng->g_procs;

	for (i = 0, pi = g->g_procs; i < g->g_nprocs; ++i, ++pi) {

		if ((*pi)->p_mode & LAM_PFLAG) {
			(*pi)->p_mode &= ~LAM_PFLAG;
		} else {
			*pj = *pi;
			(*pj)->p_refcount++;
			if (*pj == lam_myproc) {
				ng->g_myrank = (int) (pj - ng->g_procs);
			}
			++pj;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPREXCL);
	return(MPI_SUCCESS);
}
