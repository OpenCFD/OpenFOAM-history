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
 * $Id: gtranks.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get process ranks in a second group
 *	Accepts:	- first group
 *			- # of processes
 *			- array of ranks in first group
 *			- second group
 *			- array of ranks in second group (returned values)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Group_translate_ranks - Translates the ranks of processes in one
group to those in another group

Input Parameters:
+ g1 - group1 (handle) 
. n - number of ranks in 'r1' and 'r2' arrays (integer)
. r1 - array of zero or more valid ranks in 'g1' 
- g2 - group2 (handle) 

Output Parameter:
. r2 - array of corresponding ranks in 'g2', 'MPI_UNDEFINED' when no
correspondence exists.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Group_translate_ranks(MPI_Group g1, int n, 
			      int *r1, MPI_Group g2, int *r2)
{
	int		i, j;			/* favourite indices */
	int		rank;			/* process rank */
	struct _proc	**p1, **p2;		/* gps pointers */

	lam_initerr();
	lam_setfunc(BLKMPIGROUPTRANKS);
/*
 * Check given arguments.
 */
	if ((g1 == MPI_GROUP_NULL) || (g2 == MPI_GROUP_NULL)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPTRANKS, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (n == 0) {
		lam_resetfunc(BLKMPIGROUPTRANKS);
		return(MPI_SUCCESS);
	}

	if ((n < 0) || (r1 == 0) || (r2 == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPTRANKS, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Loop over the processes, translating their ranks.
 */
	for (i = 0; i < n; ++i) {
		rank = r1[i];

		if ((rank >= 0) && (rank < g1->g_nprocs)) {

			p1 = g1->g_procs + rank;
			p2 = g2->g_procs;

			for (j = 0; j < g2->g_nprocs; ++j, ++p2) {
				if (*p1 == *p2) break;
			}

			r2[i] = (j < g2->g_nprocs) ? j : MPI_UNDEFINED;
		}
		else {
			return(lam_errfunc(MPI_COMM_WORLD,
					   BLKMPIGROUPTRANKS, 
					   lam_mkerr(MPI_ERR_RANK, EINVAL)));
		}
	}

	lam_resetfunc(BLKMPIGROUPTRANKS);
	return(MPI_SUCCESS);
}
