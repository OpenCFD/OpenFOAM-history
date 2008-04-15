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
 * $Id: tsetattr.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- put value in attribute
 *			- assign attribute to datatype
 *	Accepts:	- datatype
 *			- attribute key
 *			- attribute value
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_set_attr - Put value in datatype attribute

Input Parameters:
+ type - Datatype to which attribute will be attached (handle)
- key - Key value (integer) 
- value - Attribute value 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_KEYVAL
.N MPI_ERR_OTHER

.seealso MPI_Type_create_keyval(3), MPI_Type_free_keyval(3), MPI_Type_get_attr(3), MPI_Type_delete_attr(3)

.N WEB
@*/

int
MPI_Type_set_attr(MPI_Datatype type, int key, void *value)
{
	struct _attrkey	*p;
	struct _attr	*pk;
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITSETATTR);
/*
 * Check the arguments.
 */
	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSETATTR, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	p = lam_getattr(key);
	if (p == 0 || (p->ak_flags & LAM_PREDEF)
			|| !(p->ak_flags & LAM_AKTYPE)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSETATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
 * If the key is not attached to the datatype, attach and fill it.
 */
	pk = (struct _attr *) lam_getkey(type->dt_keys, key);
	if (pk == 0) {
		if (lam_putkey(&type->dt_keys, key)) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITSETATTR,
				lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		pk = (struct _attr *) lam_getkey(type->dt_keys, key);
	}
/*
 * Old attribute, delete previous value and fill new one.
 */
	else if (((MPI_Type_delete_attr_function *) (p->ak_del))
			!= MPI_TYPE_NULL_DELETE_FN) {

		if (p->ak_flags & LAM_LANGF77) {
			(*((MPI_F_delete_function *) (p->ak_del)))
				(&type->dt_f77handle, &key,
				(int *) &pk->a_value,
				(int *) &p->ak_extra, &err);
		} else {
			err = (*((MPI_Type_delete_attr_function *) (p->ak_del)))
				(type, key, pk->a_value, p->ak_extra);
		}

		if (err != MPI_SUCCESS) {
			return(lam_err_comm(MPI_COMM_WORLD, err, EINVAL,
						"deleting old attribute"));
		}
	}

	pk->a_value = value;

	lam_resetfunc_m(BLKMPITSETATTR);
	return(MPI_SUCCESS);
}
