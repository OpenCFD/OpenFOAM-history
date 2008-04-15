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
 * $Id: infodel.c,v 6.7 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- delete info key and associated value
 *	Accepts:	- info (inout)
 *			- key
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
  MPI_Info_delete - Delete a (key, value) pair from an 'MPI_Info' object

Input Parameter:
. key - null-terminated character string of the index key

Input/Output Parameter:
. info - info object (handle)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INFO_KEY
.N MPI_ERR_INFO_NOKEY
.N MPI_ERR_INTERN

.N WEB
@*/
int MPI_Info_delete(MPI_Info info, char *key)
{
	struct _infoent	search;			/* search element */
	struct _infoent	*found;			/* found key/value pair */
	int		keylen;		/* key length */

        lam_initerr_m();
	lam_setfunc_m(BLKMPIINFODEL);
	
	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODEL,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	keylen = (key) ? strlen(key) : 0;
	if (keylen == 0 || keylen > MPI_MAX_INFO_KEY) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODEL,
					lam_mkerr(MPI_ERR_INFO_KEY, EINVAL)));
	}

	strcpy(search.ie_key, key);

	if ((found = al_find(info->info_list, &search))) {
		if (found->ie_value) free(found->ie_value);
		if (al_delete(info->info_list, found) < 0) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODEL,
					   lam_mkerr(MPI_ERR_INTERN, EINVAL)));
		}
	} else {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODEL,
				   lam_mkerr(MPI_ERR_INFO_NOKEY, EINVAL)));
	}

        lam_resetfunc_m(BLKMPIINFODEL);
	return(MPI_SUCCESS);
}
