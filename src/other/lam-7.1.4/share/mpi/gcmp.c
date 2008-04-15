/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: gcmp.c,v 6.8 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- compare two groups
 *	Accepts:	- first group
 *			- second group
 *			- ptr comparison result
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*
 * local functions
 */
static int		proccmp(const void *p1, const void *p2);

/*@

MPI_Group_compare - Compares two groups

Input Parameters:
+ g1 - group1 (handle) 
- g2 - group2 (handle) 

Output Parameter:
. pres - integer which is 'MPI_IDENT' if the order and members of the
two groups are the same, 'MPI_SIMILAR' if only the members are the
same, and 'MPI_UNEQUAL' otherwise

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.N ACK
@*/
int MPI_Group_compare(MPI_Group g1, MPI_Group g2, 
		      int *pres)
{
	int		i;			/* favourite index */
	struct _proc	**p1, **p2;		/* favourite pointers */
	struct _proc	**sort1;		/* group1 sort array */
	struct _proc	**sort2;		/* group2 sort array */

	lam_initerr();
	lam_setfunc(BLKMPIGROUPCMP);
/*
 * Check the arguments.
 */
	if ((g1 == MPI_GROUP_NULL) || (g2 == MPI_GROUP_NULL)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPCMP, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (pres == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPCMP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (g1 == g2) {
		*pres = MPI_IDENT;
		lam_resetfunc(BLKMPIGROUPCMP);
		return(MPI_SUCCESS);
	}

	if (g1->g_nprocs != g2->g_nprocs) {
		*pres = MPI_UNEQUAL;
		lam_resetfunc(BLKMPIGROUPCMP);
		return(MPI_SUCCESS);
	}
/*
 * Loop checking if same processes in same order.
 */
	p1 = g1->g_procs;
	p2 = g2->g_procs;

	for (i = 0; i < g1->g_nprocs; ++i, ++p1, ++p2) {
		if (*p1 != *p2) break;
	}

	if (i == g1->g_nprocs) {
		*pres = MPI_IDENT;
		lam_resetfunc(BLKMPIGROUPCMP);
		return(MPI_SUCCESS);
	}
/*
 * Loop checking if same processes in different order.
 * Sort the processes and compare the sorted lists for equality.
 */
	i = g1->g_nprocs * sizeof(struct _proc *);
	sort1 = (struct _proc **) malloc((unsigned) (i + i));

	if (sort1 == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGROUPCMP,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	sort2 = sort1 + g1->g_nprocs;

	memcpy((char *) sort1, (char *) g1->g_procs, i);
	memcpy((char *) sort2, (char *) g2->g_procs, i);

	qsort((char *) sort1, g1->g_nprocs, sizeof(struct _proc *), proccmp);
	qsort((char *) sort2, g2->g_nprocs, sizeof(struct _proc *), proccmp);

	p1 = sort1;
	p2 = sort2;

	for (i = 0; i < g1->g_nprocs; ++i, ++p1, ++p2) {
		if (*p1 != *p2) break;
	}

	*pres = (i == g1->g_nprocs) ? MPI_SIMILAR : MPI_UNEQUAL;

	free((char *) sort1);

	lam_resetfunc(BLKMPIGROUPCMP);
	return(MPI_SUCCESS);
}

/*
 *	proccmp
 *
 *	Function:	- compare 2 process entries
 *	Accepts:	- ptr 2 process pointers
 *	Returns:	- -ve/0/+ve
 */
static int
proccmp(const void *p1, const void *p2)
{
  return(lam_proccmp((struct _proc*) p1, (struct _proc*) p2));
}
