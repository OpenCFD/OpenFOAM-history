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
 * $Id: infoget.c,v 6.9 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- get info value associated with key 
 *	Accepts:	- info
 *			- key
 *			- maximum length of value
 *			- value (returned)
 *			- key found flag (returned)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <etc_misc.h>


/*@
  MPI_Info_get - Get a (key, value) pair from an 'MPI_Info' object

Input Parameters:
+ info - info object (handle)
. key - null-terminated character string of the index key
. valuelen - maximum length of 'value' (integer)

Output Parameters:
+ value - null-terminated character string of the value
- flag - true (1) if 'key' defined on 'info', false (0) if not
(logical)

Notes:

In C and C++, 'valuelen' should be one less than the allocated space
to allow for for the null terminator.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INFO_KEY

.N WEB
@*/
int MPI_Info_get(MPI_Info info, char *key, 
		 int valuelen, char *value, int *flag)
{
	struct _infoent	search;			/* search element */
	struct _infoent	*found;			/* found key/value pair */
	int		keylen;			/* key length */
	int		vallen;			/* stored value length */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOGET);

	if (info == 0 || value == 0 || valuelen <= 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGET,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	keylen = (key) ? strlen(key) : 0;
	if (keylen == 0 || keylen > MPI_MAX_INFO_KEY) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGET,
					lam_mkerr(MPI_ERR_INFO_KEY, EINVAL)));
	}

	strcpy(search.ie_key, key);

	if ((found = al_find(info->info_list, &search))) {
		*flag = 1;
		vallen = strlen(found->ie_value);
		if (vallen < valuelen) {
		    strcpy(value, found->ie_value);
		} else {
		    lam_strncpy(value, found->ie_value, valuelen);
		    value[valuelen] = 0;
		}
	} else {
		*flag = 0;
	}

	lam_resetfunc_m(BLKMPIINFOGET);
	return(MPI_SUCCESS);
}
