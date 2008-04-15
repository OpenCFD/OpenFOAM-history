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
 * $Id: cdelattr.c,v 1.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- delete attribute
 *			- detach attribute from communicator
 *	Accepts:	- communicator
 *			- attribute key
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_delete_attr - Delete an attribute

Input Parameters:
+ comm - Communicator from which the attribute is deleted
- key - Key value

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_KEYVAL
.N MPI_ERR_INTERN

.seealso MPI_Comm_create_keyval(3), MPI_Comm_free_keyval(3), MPI_Comm_set_attr(3), MPI_Comm_get_attr(3)

.N WEB
@*/
int
MPI_Comm_delete_attr(MPI_Comm comm, int key)
{
	struct _attrkey	*p;
	struct _attr	*pk;
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMDELETEATTR);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD,
				MPI_ERR_COMM, 0, "null handle"));
	}

	p = lam_getattr(key);
	if (p == 0) {
		return(lam_err_comm(comm, MPI_ERR_KEYVAL, EINVAL, ""));
	}
/*
 * Check if the key is attached to the communicator.
 */
	pk = lam_getkey(comm->c_keys, key);
	if (pk == 0) {
		return(lam_err_comm(comm, MPI_ERR_KEYVAL, EINVAL,
			"not on communicator"));
	}
/*
 * Delete the attribute via the callback function.
 */
	if (((MPI_Comm_delete_attr_function *) (p->ak_del))
			!= MPI_COMM_NULL_DELETE_FN) {

		if (p->ak_flags & LAM_LANGF77) {
			(*((MPI_F_delete_function *) (p->ak_del)))
				(&comm->c_f77handle, &key,
				(int *) &(pk->a_value),
				(int *) &(p->ak_extra), &err);
		} else {
			err = (*((MPI_Comm_delete_attr_function *) 
				 (p->ak_del)))
				(comm, key, pk->a_value, p->ak_extra);
		}

		if (err != MPI_SUCCESS) {
			return(lam_err_comm(comm, err, EINVAL,
					"deleting attribute"));
		}
	}
/*
 * Detach the key from the communicator.
 */
	if (lam_delkey(comm->c_keys, key)) {
		return(lam_err_comm(comm, MPI_ERR_INTERN, errno,
			"detaching key"));
	}

	lam_resetfunc_m(BLKMPICOMMDELETEATTR);
	return(MPI_SUCCESS);
}
