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
 * $Id: infogetnth.c,v 6.7 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get n'th key in info object
 *	Accepts:	- info
 *			- n
 *			- key (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
  MPI_Info_get_nthkey - Get a key indexed by integer from an 'MPI_Info' object

int MPI_Info_get_nthkey(MPI_Info info, int n, char *key)

Input Parameters:
+ info - info object (handle)
- n - index of key to retrieve (integer)

Output Parameter:
. key - character string of at least 'MPI_MAX_INFO_KEY' characters

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N WEB
@*/
int MPI_Info_get_nthkey(MPI_Info info, int n, char *key)
{
	struct _infoent	*p;			/* key/value pair */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOGETNTHKEY);

	if (info == 0 || n >= al_count(info->info_list) || key == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGETNTHKEY,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	for (p = al_top(info->info_list); n > 0; n--) {
		p = al_next(info->info_list, p);
	}

	strcpy(key, p->ie_key);

	lam_resetfunc_m(BLKMPIINFOGETNTHKEY);
	return(MPI_SUCCESS);
}
