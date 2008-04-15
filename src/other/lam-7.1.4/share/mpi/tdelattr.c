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
 * $Id: tdelattr.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- delete attribute
 *			- detach attribute from datatype
 *	Accepts:	- datatype
 *			- attribute key
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_delete_attr - Delete attribue

Input Parameters:
+ type - datatype from which the attribute is deleted (handle)
- key - key value (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_KEYVAL
.N MPI_ERR_INTERN

.seealso MPI_Type_free_keyval(3), MPI_Type_get_attr(3), MPI_Type_set_attr(3)

.N WEB
@*/

int
MPI_Type_delete_attr(MPI_Datatype type, int key)
{
	struct _attrkey	*p;
	struct _attr	*pk;
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITDELETEATTR);
/*
 * Check the arguments.
 */
	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITDELETEATTR, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	p = lam_getattr(key);
	if (p == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITDELETEATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
 * Check if the key is attached to the datatype.
 */
	pk = lam_getkey(type->dt_keys, key);
	if (pk == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITDELETEATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
 * Delete the attribute via the callback function.
 */
	if (((MPI_Type_delete_attr_function *) (p->ak_del))
			!= MPI_TYPE_NULL_DELETE_FN) {

		if (p->ak_flags & LAM_LANGF77) {
			(*((MPI_F_delete_function *) (p->ak_del)))
				(&type->dt_f77handle, &key,
				(int *) &(pk->a_value),
                               	(int *) &(p->ak_extra), &err);
		} else {
			err = (*((MPI_Type_delete_attr_function *) (p->ak_del)))
				(type, key, pk->a_value, p->ak_extra);
		}

		if (err != MPI_SUCCESS) {
			return(lam_err_comm(MPI_COMM_WORLD, err, EINVAL,
						"deleting attribute"));
		}
	}
/*
 * Detach the key from the datatype.
 */
	if (lam_delkey(type->dt_keys, key)) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_INTERN, errno,
					"deleting key"));
	}

	lam_resetfunc_m(BLKMPITDELETEATTR);
	return(MPI_SUCCESS);
}
