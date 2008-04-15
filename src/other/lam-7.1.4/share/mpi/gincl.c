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
 * $Id: gincl.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- form a new group by inclusion
 *	Accepts:	- original group
 *			- # of processes
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

MPI_Group_incl - Produces a group by reordering an existing group and taking
        only listed members

Input Parameters:
+ g - group (handle) 
. n - number of elements in array 'ranks' (and size of newgroup )
(integer)
- ranks - ranks of processes in 'g' to appear in 'png' (array of 
integers) 

Output Parameter:
. png - new group derived from above, in the order defined by 'ranks'
(handle)

Notes:

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
int MPI_Group_incl(MPI_Group g, int n, int *ranks, 
		   MPI_Group *png)
{
	int		i, j;
	int		*r;
	struct _proc	**pi, **pj;		/* processes in groups */
	MPI_Group	ng;			/* new group */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPINCL);
/*
 * Check the arguments.
 */
	if (g == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPINCL, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if ((png == 0) || ((n > 0) && (ranks == 0))
			|| (n < 0) || (n > g->g_nprocs)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPINCL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (n == 0) {
		*png = MPI_GROUP_EMPTY;
		MPI_GROUP_EMPTY->g_refcount++;
		lam_resetfunc_m(BLKMPIGROUPINCL);
		return(MPI_SUCCESS);
	}
/*
 * Allocate a new group.
 */
	ng = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(n * sizeof(struct _proc *)));
	if (ng == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPINCL, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*png = ng;
	ng->g_nprocs = n;
	ng->g_myrank = MPI_UNDEFINED;
	ng->g_refcount = 1;
	ng->g_f77handle = -1;
	ng->g_procs = (struct _proc **) ((char *) ng + sizeof(struct _group));
/*
 * Fill the new group processes.
 */
	pj = g->g_procs;

	for (i = 0, pi = ng->g_procs, r = ranks; i < n; ++i, ++pi, ++r) {

		if ((*r < 0) || (*r >= g->g_nprocs)
				|| (pj[*r]->p_mode & LAM_PFLAG)) {

			for (r = ranks, j = 0; j < i; ++j, ++r) {
				pj[*r]->p_mode &= ~LAM_PFLAG;
			}
			free((char *) ng);
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPINCL,
					   lam_mkerr(MPI_ERR_RANK, EINVAL)));
		} else {
			*pi = pj[*r];
			pj[*r]->p_mode |= LAM_PFLAG;
		}
	}

	for (i = 0, pi = ng->g_procs; i < n; ++i, ++pi) {
		(*pi)->p_refcount++;
		(*pi)->p_mode &= ~LAM_PFLAG;
		if (*pi == lam_myproc) {
			ng->g_myrank = i;
		}
	}

	lam_resetfunc_m(BLKMPIGROUPINCL);
	return(MPI_SUCCESS);
}
