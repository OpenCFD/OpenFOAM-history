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
 * $Id: gfree.c,v 6.7 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- free a group
 *	Accepts:	- group (in-out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <MPISYSF.h>
#include <rpisys.h>

/*@

MPI_Group_free - Frees a group

Input Parameter
. grp - group (handle) 

Notes:

On output, 'grp' is set to 'MPI_GROUP_NULL'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_PERM_GROUP

.N ACK
@*/
int MPI_Group_free(MPI_Group *grp)
{
	struct _proc	**p;
	int		i;
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIGROUPFREE);

	if (grp == 0 || *grp == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPFREE, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	} else {
		if ((--(*grp)->g_refcount) == 0) {
/*
 * We don't check for the empty group before since a user may
 * legitimately free a copy of it.  The problem is now that we won't
 * catch an explicit free of MPI_GROUP_EMPTY when copies of it exist.
 */
			if (*grp == MPI_GROUP_EMPTY) {
				(*grp)->g_refcount++;
				return(lam_errfunc(MPI_COMM_WORLD,
					BLKMPIGROUPFREE,
					lam_mkerr(MPI_ERR_GROUP, EINVAL)));
			}

			for (i = (*grp)->g_nprocs, p = (*grp)->g_procs; i > 0;
			     i--, p++) {

				if ((--(*p)->p_refcount) == 0) {
					lam_procfree(*p);
				}
			}

			if ((*grp)->g_f77handle >= 0) {
				lam_F_free_hdl((*grp)->g_f77handle);
			}

			free((char *) *grp);
		}

		*grp = MPI_GROUP_NULL;
	}

	lam_resetfunc_m(BLKMPIGROUPFREE);
	return(MPI_SUCCESS);
}
