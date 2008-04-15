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
 * $Id: ccmp.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- compare two communicators
 *	Accepts:	- first communicator
 *			- second communicator
 *			- comparison result (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>

/*@

MPI_Comm_compare - Compares two communicators

Input Parameters:
+ c1 - comm1 (handle) 
- c2 - comm2 (handle) 

Output Parameter:
. result - integer which is 'MPI_IDENT' if the contexts and groups are
the same, 'MPI_CONGRUENT' if different contexts but identical groups,
'MPI_SIMILAR' if different contexts but similar groups, and
'MPI_UNEQUAL' otherwise

Using 'MPI_COMM_NULL' with 'MPI_Comm_compare':

It is an error to use 'MPI_COMM_NULL' as one of the arguments to
'MPI_Comm_compare'.  The relevant sections of the MPI standard are

(2.4.1 Opaque Objects)
A null handle argument is an erroneous 'IN' argument in MPI calls,
unless an exception is explicitly stated in the text that defines the
function.

(5.4.1. Communicator Accessors)
<no text in 'MPI_COMM_COMPARE' allowing a null handle>

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_compare(MPI_Comm c1, MPI_Comm c2, 
		     int *result)
{
	int		gcmp;			/* local group comparison */
	int		grcmp;			/* remote group comparison */
	int		err;

	lam_initerr();
	lam_setfunc(BLKMPICOMMCMP);
/*
 * Check the arguments.
 */
	if (result == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMCMP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (c1 == MPI_COMM_NULL || c2 == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMCMP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
/*
 * Handle the trivial cases.
 */
	if (c1 == c2) {
		*result = MPI_IDENT;
		lam_resetfunc(BLKMPICOMMCMP);
		return(MPI_SUCCESS);
	}

	if ((LAM_IS_INTER(c1) && LAM_IS_INTRA(c2))
			|| (LAM_IS_INTRA(c1) && LAM_IS_INTER(c2))) {
		*result = MPI_UNEQUAL;
		lam_resetfunc(BLKMPICOMMCMP);
		return(MPI_SUCCESS);
	}
/*
 * Compare the groups.
 */
	err = MPI_Group_compare(c1->c_group, c2->c_group, &gcmp);
	if (err) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCMP, err));
	}

	if (LAM_IS_INTER(c1) && (gcmp != MPI_UNEQUAL)) {
		err = MPI_Group_compare(c1->c_rgroup, c2->c_rgroup, &grcmp);
		if (err) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCMP, err));
		}

		gcmp = LAM_max(gcmp, grcmp);
	}

	*result = (gcmp == MPI_IDENT) ? MPI_CONGRUENT: gcmp;

	lam_resetfunc(BLKMPICOMMCMP);
	return(MPI_SUCCESS);
}
