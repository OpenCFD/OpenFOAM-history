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
 * $Id: infocreate.c,v 6.8 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- info object constructor
 *	Accepts:	- info (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*
 * private functions
 */
static int		info_key_cmp(struct _infoent *a, struct _infoent *b);


/*@
  MPI_Info_create - Create and initialize an empty MPI_Info object

Output Parameter:
. info - info object (handle)

Notes:

Upon successful return, the created 'MPI_Info' object will contain no
(key, value) pairs.

When an info object is no longer being used, it should be freed with
'MPI_Info_free'.

.N fortran

.N Errors
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Info_free

.N WEB
@*/
int MPI_Info_create(MPI_Info *info)
{
        lam_initerr_m();
	lam_setfunc_m(BLKMPIINFOCREATE);

	if (info == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOCREATE,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((*info = (MPI_Info) malloc(sizeof(struct _info))) == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*info)->info_list = al_init(sizeof(struct _infoent), info_key_cmp);
	if ((*info)->info_list == 0) {
		free((char *) *info);
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*info)->info_f77handle = -1;

        lam_resetfunc_m(BLKMPIINFOCREATE);
	return(MPI_SUCCESS);
}

/*
 *	info_key_cmp
 *
 *	Function:	- compare two info pairs by key
 *	Accepts:	- ptr to two pairs
 *	Returns:	- string comparison of keys
 */
static int
info_key_cmp(struct _infoent *a, struct _infoent *b)
{
	return(strcmp(a->ie_key, b->ie_key));
}
