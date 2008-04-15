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
 * $Id: grincl.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form a new group by range inclusion
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
#include <net.h>
#include <terror.h>

/*@

MPI_Group_range_incl - Creates a new group from ranges of ranks in an 
        existing group

Input Parameters:
+ g - group (handle) 
. n - number of triplets in array  'ranges' (integer) 
- ranges - a one-dimensional array of integer triplets, of the 
form (first rank, last rank, stride) indicating ranks in
'g'  or processes to be included in 'png'  

Output Parameter:
. png - new group derived from above, in the order defined by 'ranges'
(handle)

Note:

When a group is no longer being used, it should be freed with
'MPI_Group_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_ARG
.N MPI_ERR_RANK

.seealso: MPI_Group_free

.N ACK
@*/
int MPI_Group_range_incl(MPI_Group g, int n, 
			 int ranges[][3], MPI_Group *png)
{
	int		i, j;
	int		nproc;			/* # processes */
	int		first;			/* range first */
	int		last;			/* range last */
	int		end;			/* calculated end of range */
	int		stride;			/* range stride */
	struct _proc	**p, **q;		/* processes in groups */
	MPI_Group	ng;			/* new group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPRINCL);
/*
 * Check the arguments.
 */
	if (g == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPRINCL, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if ((png == 0) || (n < 0) || ((n > 0) && (ranges == 0))) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPRINCL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	nproc = 0;
	for (i = 0; i < n; ++i) {

		if ((stride = ranges[i][2]) == 0) break;

		first = ranges[i][0];
		last = ranges[i][1];
		end = first + ((last - first) / stride) * stride;

		if (stride > 0) {
			if (first < 0 || first > last || end >= g->g_nprocs) {
				break;
			}
		} else {
			if (first >= g->g_nprocs || first < last || end < 0) {
				break;
			}
		}

		nproc += 1 + ((end - first) / stride);
	}

	if (i < n) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPIGROUPRINCL, 
				   lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (nproc == 0) {
		*png = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPRINCL);
		return(MPI_SUCCESS);
	}
/*
 * Allocate a new group.
 */
	ng = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(nproc * sizeof(struct _proc *)));
	if (ng == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPRINCL, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	ng->g_nprocs = nproc;
	ng->g_myrank = MPI_UNDEFINED;
	ng->g_refcount = 1;
	ng->g_f77handle = -1;
	ng->g_procs = (struct _proc **) ((char *) ng + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	p = ng->g_procs;
	q = g->g_procs;

	for (i = 0; i < n; ++i) {

		stride = ranges[i][2];
		j = ranges[i][0];
		end = j + ((ranges[i][1] - j) / stride) * stride;
		
		for (; (stride > 0) ? (j <= end) : (j >= end); j += stride) {

			if (!(q[j]->p_mode & LAM_PFLAG)) {
				*p = q[j];
				(*p)->p_mode |= LAM_PFLAG;
				if (*p == lam_myproc) {
					ng->g_myrank = (int) (p - ng->g_procs);
				}
				++p;
			} else {
				for (q = ng->g_procs; q < p; ++q) {
					(*q)->p_mode &= ~LAM_PFLAG;
				}
				free(ng);
				return(lam_errfunc(MPI_COMM_WORLD,
						   BLKMPIGROUPRINCL,
						   lam_mkerr(MPI_ERR_RANK, 
							     EINVAL)));
			}
		}
	}

	for (i = 0, p = ng->g_procs; i < nproc; ++i, ++p) {
		(*p)->p_refcount++;
		(*p)->p_mode &= ~LAM_PFLAG;
	}

	*png = ng;

	lam_resetfunc_m(BLKMPIGROUPRINCL);
	return(MPI_SUCCESS);
}
