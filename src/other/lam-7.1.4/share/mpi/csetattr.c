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
 * $Id: csetattr.c,v 1.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- put value in attribute
 *			- assign attribute to communicator
 *	Accepts:	- communicator
 *			- attribute key
 *			- attribute value
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_set_attr - MPI_ATTR_PUT replacement

Input Parameters:
+ comm - Communicator to which the attribute will be attached
. key - Key value
- value - Attribute value

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_KEYVAL

.seealso MPI_Comm_create_keyval(3), MPI_Comm_free_keyval(3), MPI_Comm_get_attr(3), MPI_Comm_delete_attr(3)

.N WEB
@*/
int
MPI_Comm_set_attr(MPI_Comm comm, int key, void *value)
{
	struct _attrkey *p;
	struct _attr    *pk;
	int             err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMSETATTR);
/*
* Check the arguments.
*/
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD,
				    MPI_ERR_COMM, EINVAL, "null handle"));
	}

	p = lam_getattr(key);
	if (p == 0 || (p->ak_flags & LAM_PREDEF)
			|| !(p->ak_flags & LAM_AKCOMM)) {
		return(lam_err_comm(comm, MPI_ERR_KEYVAL, EINVAL, ""));
	}
/*
* If the key is not attached to the communicator, attach and fill it.
*/
	pk = (struct _attr *) lam_getkey(comm->c_keys, key);
	if (pk == 0) {
		if (lam_putkey(&comm->c_keys, key)) {
			return(lam_err_comm(comm,
				MPI_ERR_OTHER, errno, "attaching key"));
		}

		pk = (struct _attr *) lam_getkey(comm->c_keys, key);
	}
/*
 * Old attribute, delete previous value and fill new one.
 */
	else if (((MPI_Comm_delete_attr_function *) (p->ak_del))
			!= MPI_COMM_NULL_DELETE_FN) {

		if (p->ak_flags & LAM_LANGF77) {
			(*((MPI_F_delete_function *) (p->ak_del)))
				(&comm->c_f77handle, &key,
				(int *) &pk->a_value,
				(int *) &p->ak_extra, &err);
		} else {
			err = (*((MPI_Comm_delete_attr_function *) (p->ak_del)))
				(comm, key, pk->a_value, p->ak_extra);
		}

		if (err != MPI_SUCCESS) {
			return(lam_err_comm(comm, err, EINVAL,
					    "deleting old attribute"));
		}
	}

	pk->a_value = value;

	lam_resetfunc_m(BLKMPICOMMSETATTR);
	return(MPI_SUCCESS);
}
