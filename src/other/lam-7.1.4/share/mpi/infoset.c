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
 * $Id: infoset.c,v 6.9 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- set info value associated with key
 *	Accepts:	- info (inout)
 *			- key
 *			- value
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
  MPI_Info_set - Set a (key, value) pair in an 'MPI_Info' object

Input Parameters:
+ key - null-terminated character string of the index key
- value - null-terminated character string of the value

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
int MPI_Info_set(MPI_Info info, char *key, char *value)
{
	struct _infoent	newinfo;		/* new info pair */
	struct _infoent	*old;			/* existing pair with key */
	int		keylen;			/* key length */
	int		vallen;			/* value length */
	char		*newval;		/* storage for new value */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOSET);
/*
 * Check arguments.
 */
	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOSET,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	keylen = (key) ? strlen(key) : 0;
	if (keylen == 0 || keylen > MPI_MAX_INFO_KEY) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOSET,
				lam_mkerr(MPI_ERR_INFO_KEY, EINVAL)));
	}

	vallen = (value) ? strlen(value) : 0;
	if (vallen == 0 || vallen > MPI_MAX_INFO_VAL) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOSET,
					lam_mkerr(MPI_ERR_INFO_VALUE, 
						  EINVAL)));
	}
/*
 * Allocate storage for the new value.
 */
	if ((newval = (char *) malloc(vallen + 1)) == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOSET,
					lam_mkerr(MPI_ERR_OTHER, EINVAL)));
	}

	LAM_ZERO_ME(newinfo);
	strcpy(newinfo.ie_key, key);
	strcpy(newval, value);
/*
 * Check if key already appears in the info object.
 */
	old = al_find(info->info_list, &newinfo);

	if (old) {
/*
 * The key is already in the info object.  Replace the old value with the new.
 */
		free(old->ie_value);
		old->ie_value = newval;
	} else {
/*
 * Add the key/value pair to the info object.
 */
		newinfo.ie_value = newval;
		if (al_append(info->info_list, &newinfo) == 0) {
			return(lam_errfunc(MPI_COMM_WORLD,
					   BLKMPIINFOSET, 
					   lam_mkerr(MPI_ERR_OTHER, EINVAL)));
		}
	}

	lam_resetfunc_m(BLKMPIINFOSET);
	return(MPI_SUCCESS);
}
