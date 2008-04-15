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
 * $Id: infofree.c,v 6.8 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- free an info object
 *	Accepts:	- info (modified)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
  MPI_Info_free - Free an 'MPI_Info' object.

Input/Output Parameters:
. info - info object to be freed (handle)

Notes:

Upon successful completion, 'info' will be set to 'MPI_INFO_NULL'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N WEB
@*/
int MPI_Info_free(MPI_Info *info)
{
	struct _infoent	*p;			/* key/value pair */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOFREE);

	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOFREE,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	for (p = al_top((*info)->info_list); p;
			p = al_next((*info)->info_list, p)) {

		if (p->ie_value) {
			free(p->ie_value);
		}
	}
/*
 * Remove the fortran handle if the user called MPI_Info_f2c on this
 * handle
 */
	if ((*info)->info_f77handle >= 0)
	  lam_F_free_hdl((*info)->info_f77handle);

	(void) al_free((*info)->info_list);
	free((char *) *info);
	*info = MPI_INFO_NULL;

	lam_resetfunc_m(BLKMPIINFOFREE);
	return(MPI_SUCCESS);
}
