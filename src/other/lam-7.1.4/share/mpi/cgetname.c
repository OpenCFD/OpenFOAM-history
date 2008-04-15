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
 * $Id: cgetname.c,v 1.7 2003/06/01 14:04:07 jsquyres Exp $
 *
 *	Function:	- get communicator name
 *	Accepts:	- communicator
 *			- communicator name (out)
 *			- communicator name length (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <etc_misc.h>


/*@
   MPI_Comm_get_name - Get the name associated with a communicator

Input Parameters:
. comm - Communicator

Output Parameters:
+ name - Name of the communicator
- length - Integer length of the resulting name

Notes:

This function returns the name of a communicator that was previously
set with 'MPI_Comm_set_name'.  If a name was not previously associated
with a communicator, the returned string will be empty and 'length'
will be 0.  The retuned name will be a maximum of
'MPI_MAX_OBJECT_NAME' characters long.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso MPI_Comm_set_name, MPI_Type_get_name, MPI_Type_set_name, MPI_Win_get_name, MPI_Win_set_name

.N WEB
@*/
int
MPI_Comm_get_name(MPI_Comm comm, char *name, int *length)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMGETNAME);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMGETNAME, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if ((name == 0) || (length == 0)) {
		return(lam_errfunc(comm,
			BLKMPICOMMGETNAME, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_strncpy(name, comm->c_name, MPI_MAX_OBJECT_NAME);
	name[MPI_MAX_OBJECT_NAME - 1] = 0;
	*length = strlen(name);

	lam_resetfunc_m(BLKMPICOMMGETNAME);
	return(MPI_SUCCESS);
}
