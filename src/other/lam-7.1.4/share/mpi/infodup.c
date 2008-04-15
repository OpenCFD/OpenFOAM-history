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
 * $Id: infodup.c,v 6.7 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- duplicate an info object
 *	Accepts:	- info
 *			- duplicate info (in-out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
  MPI_Info_dup - Duplicate an 'MPI_Info' object

Input Parameter:
. info - source info object (handle)

Output Parameters:
. newinfo - destination info object (handle)

Notes:

Not only will the (key, value) pairs be duplicated, the order of keys
will be the same in 'newinfo' as it is in 'info'.

When an info object is no longer being used, it should be freed with
'MPI_Info_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso: MPI_Info_free

.N WEB
@*/
int MPI_Info_dup(MPI_Info info, MPI_Info *newinfo)
{
	struct _infoent	*p;			/* key/value pair */
	int		err;			/* error code */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFODUP);

	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODUP,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	err = MPI_Info_create(newinfo);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODUP, err));
	}
/*
 * Duplicate all the entries.
 */
	for (p = al_top(info->info_list); p; p = al_next(info->info_list, p)) {

		err = MPI_Info_set(*newinfo, p->ie_key, p->ie_value);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPIINFODUP, err));
		}
	}

	lam_resetfunc(BLKMPIINFODUP);
	return(MPI_SUCCESS);
}
