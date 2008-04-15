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
 * $Id: tgetattr.c,v 1.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- get datatype attribute value
 *	Accepts:	- datatype
 *			- attribute key
 *			- attribute value (out)
 *			- found flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_get_attr - Get datatype attribute value

Input Parameters:
+ type - Datatype to which the attribute is attached (handle) 
- key - Key value (integer)

Output Parameters:
+ value - Attribute value, unless flag = false
- found - False if no attribute is associated with the key (logical)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_KEYVAL
.N MPI_ERR_ARG

.seealso MPI_Type_create_keyval(3), MPI_Type_free_keyval(3), MPI_Type_set_attr(3), MPI_Type_delete_attr(3)

.N WEB
@*/

int
MPI_Type_get_attr(MPI_Datatype type, int key, void *value, int *found)
{
	struct _attrkey	*pk;
	struct _attr	*p;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITGETATTR);
/*
 * Check the arguments.
 */
	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITGETATTR, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if ((value == 0) || (found == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITGETATTR, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	pk = lam_getattr(key);
	if (pk == 0 || !(pk->ak_flags & LAM_AKTYPE)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITGETATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
 * Check if the key is attached to the datatype
 */
	p = lam_getkey(type->dt_keys, key);
	if (p == 0) {
		*found = 0;
	} else {
		*found = 1;
/*
 * This stuff is very confusing.  Be sure to see MPI-2 4.12.7.
 */
		*((void **) value) = (pk->ak_flags & LAM_LANGF77)
			? (void *) &p->a_value : p->a_value;
	}

	lam_resetfunc_m(BLKMPITGETATTR);
	return(MPI_SUCCESS);
}
