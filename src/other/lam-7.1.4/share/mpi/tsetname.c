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
 * $Id: tsetname.c,v 1.5 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- set datatype name
 *	Accepts:	- datatype
 *			- datatype name
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <etc_misc.h>


/*@
   MPI_Type_set_name - Set a name on a datatype

Input Parameters:
+ type - MPI datatype
- name - Pointer to the name of dataname

Notes:

This function associates a name with a datatype, which can be
useful for debugging purposes.  The function 'MPI_Type_get_name' can
be used to retrieve name that was previously set with
'MPI_Type_set_name'.

Named predefined datatypes have the default names of the datatype
name. For example, 'MPI_WCHAR' has the default name of `MPI_WCHAR`.
It is legal to change any of these names with calls to
'MPI_Type_set_name'.

Names passed in C must include the null terminator, and can be of
maximum length 'MPI_MAX_OBJECT_NAME' (including the null terminator).
Names passed in Fortran can also have a maximum length of
'MPI_MAX_OBJECT_NAME'.

Setting a name on a datatype that already has a name associated
with it replaces the old name.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

.seealso MPI_Comm_set_name, MPI_Comm_get_name, MPI_Type_get_name, MPI_Win_get_name, MPI_Win_set_name

.N WEB
@*/
int
MPI_Type_set_name(MPI_Datatype type, char *name)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPITSETNAME);
/*
 * Check the arguments.
 */
	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSETNAME, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (name == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSETNAME, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_strncpy(type->dt_name, name, MPI_MAX_OBJECT_NAME);
	type->dt_name[MPI_MAX_OBJECT_NAME - 1] = 0;

	lam_resetfunc_m(BLKMPITSETNAME);
	return(MPI_SUCCESS);
}
