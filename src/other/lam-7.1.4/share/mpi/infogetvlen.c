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
 * $Id: infogetvlen.c,v 6.9 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get length of info value associated with key
 *	Accepts:	- info
 *			- key
 *			- value length (out)
 *			- key found flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>


/*@
  MPI_Info_get_valuelen - Get the length of a value for a given key in an 'MPI_Info' object

Input Parameters:
+ info - info object (handle)
- key - null-terminated character string of the index key

Output Parameters:
+ valuelen - length of the value associated with 'key' (integer)
- flag - true (1) if 'key' defined on 'info', false (0) if not
(logical)

Notes:

The length returned in C and C++ does not include the end-of-string
character.  If the 'key' is not found on 'info', 'valuelen' is left alone.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INFO_KEY

.N WEB
@*/
int MPI_Info_get_valuelen(MPI_Info info, char *key, 
			  int *valuelen, int *flag)
{
	struct _infoent	search;			/* search element */
	struct _infoent	*found;			/* found key/value pair */
	int		keylen;			/* length of key */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOGETLEN);

	if (info == 0 || valuelen == 0 || flag == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGETLEN,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	keylen = (key) ? strlen(key) : 0;
	if (keylen == 0 || keylen > MPI_MAX_INFO_KEY) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGETLEN,
					lam_mkerr(MPI_ERR_INFO_KEY, EINVAL)));
	}

	strcpy(search.ie_key, key);

	if ((found = al_find(info->info_list, &search))) {
		*flag = 1;
		*valuelen = strlen(found->ie_value);
	} else {
		*flag = 0;
	}

	lam_resetfunc_m(BLKMPIINFOGETLEN);
	return(MPI_SUCCESS);
}
