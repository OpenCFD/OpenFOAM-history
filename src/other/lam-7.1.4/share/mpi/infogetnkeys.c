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
 * $Id: infogetnkeys.c,v 6.7 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get number of keys in an info object
 *	Accepts:	- info
 *			- number of keys (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
MPI_Info_get_nkeys - Returns the number of keys defined on an
'MPI_Info' object

Input Parameter:
. info - info object (handle)

Output Parameter:
. nkeys - number of keys defined on 'info' (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N WEB
@*/
int MPI_Info_get_nkeys(MPI_Info info, int *nkeys)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOGETNKEYS);

	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGETNKEYS,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*nkeys = al_count(info->info_list);

	lam_resetfunc_m(BLKMPIINFOGETNKEYS);
	return(MPI_SUCCESS);
}
