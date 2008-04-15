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
 * $Id: cfree.c,v 6.12 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- free a communicator
 *	Accepts:	- communicator (inout)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*
 * external functions
 */
extern int		lam_comm_free();


/*@

MPI_Comm_free - Marks the communicator object for deallocation

Input Parameter:
. comm - communicator to be destroyed (handle) 

Notes:

'comm' will be set to 'MPI_COMM_NULL' upon successful return.

Null Handles:

The MPI 1.1 specification, in the section on opaque objects,
explicitly disallows freeing a null communicator.  The text from the
standard is:

.vb
 "A null handle argument is an erroneous IN argument in MPI calls,
 unless an exception is explicitly stated in the text that defines the
 function. Such exception is allowed for handles to request objects in
 Wait and Test calls (sections Communication Completion and Multiple
 Completions ). Otherwise, a null handle can only be passed to a
 function that allocates a new object and returns a reference to it in
 the handle."
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM

.N ACK
@*/
int
MPI_Comm_free(MPI_Comm *comm)
{
	int		err;

	lam_initerr();
	lam_setfunc(BLKMPICOMMFREE);
/*
 * Check the arguments.
 */
	if (comm == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMFREE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (*comm == MPI_COMM_NULL || ((*comm)->c_flags & LAM_PREDEF)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMFREE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
/*
 * Once parent communicator is freed subsequent calls to MPI_Comm_get_parent()
 * must return MPI_COMM_NULL. See MPI-2 Standard, Section 5.3.2.
 */
	if (*comm == lam_comm_parent) {
		lam_comm_parent = MPI_COMM_NULL;
	}
/*
 * Delete the communicator if it has no pending requests.
 */
	if (--((*comm)->c_refcount) == 0) {

	  /* Tell the coll SSI module that this comm is now gone */

	  if ((err = lam_ssi_coll_base_finalize_comm(*comm)) != 0)
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMFREE, err));

#if LAM_WANT_IMPI
		if (LAM_IS_IMPI(*comm))
			MPI_Comm_free(&(*comm)->c_shadow);
#endif
		err = lam_comm_free(*comm);
		*comm = MPI_COMM_NULL;

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPICOMMFREE, err));
		}
	} else {
		*comm = MPI_COMM_NULL;
	}
	
	lam_resetfunc(BLKMPICOMMFREE);
	return(MPI_SUCCESS);
}
