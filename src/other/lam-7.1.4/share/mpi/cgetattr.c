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
 * $Id: cgetattr.c,v 1.6 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get communicator attribute value
 *	Accepts:	- communicator
 *			- attribute key
 *			- attribute value (out)
 *			- found flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_get_attr - Get communicator attribute value

Input Parameters:
+ comm - Communicator to which attribute is attached
- key - key value

Output Parameters:
+ value - attribute value, unless found = false
- found - false if no attribute is associated with the key 

.N IMPI_MPI2

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_KEYVAL
.N MPI_ERR_ARG

.seealso MPI_Comm_create_keyval(3), MPI_Comm_free_keyval(3), MPI_Comm_set_attr(3), MPI_Comm_delete_attr(3)

.N WEB
@*/

int
MPI_Comm_get_attr(MPI_Comm comm, int key, void *value, int *found)
{
        struct _attrkey *pk;
	struct _attr    *p;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMGETATTR);
/*
* Check the arguments.
*/
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_COMM, EINVAL,
					"null handle"));
	}

	if ((value == 0) || (found == 0)) {
		return(lam_err_comm(comm, MPI_ERR_ARG, EINVAL, 
				    "null pointer"));
	}
	
	pk = lam_getattr(key);
	if (pk == 0) {
		return(lam_err_comm(comm, MPI_ERR_KEYVAL, EINVAL, ""));
	}

	if (!(pk->ak_flags & LAM_AKCOMM)) {
		return(lam_err_comm(comm, MPI_ERR_KEYVAL, EINVAL,
					"not a communicator key"));
	}
/*
 * Check if the key is attached to the communicator.
 */
	p = lam_getkey(comm->c_keys, key);
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

	lam_resetfunc_m(BLKMPICOMMGETATTR);
	return(MPI_SUCCESS);
}
