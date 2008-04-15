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
 * $Id: csetname.c,v 1.7 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- set communicator name
 *	Accepts:	- communicator
 *			- communicator name
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <etc_misc.h>


/*@
   MPI_Comm_set_name - Set a name on a communicator

Input Parameters:
+ comm - Communicator
- name - Pointer to the name of communicator

Notes:

This function associates a name with a communicator, which can be
useful for debugging purposes.  The function 'MPI_Comm_get_name' can
be used to retrieve name that was previously set with
'MPI_Comm_set_name'.

The 'MPI_COMM_WORLD', 'MPI_COMM_SELF', and parent communicator (if an
application was launched via 'MPI_Comm_spawn' or
'MPI_Comm_spawn_multiple') have the predefined names `MPI_COMM_WORLD`,
`MPI_COMM_SELF`, and `MPI_COMM_PARENT`, respectively.  It is legal to
change any of these names with calls to 'MPI_Comm_set_name'.

Names passed in C must include the null terminator, and can be of
maximum length 'MPI_MAX_OBJECT_NAME' (including the null terminator).
Names passed in Fortran can also have a maximum length of
'MPI_MAX_OBJECT_NAME'.

Setting a name on a communicator that already has a name associated
with it replaces the old name.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso MPI_Comm_get_name, MPI_Type_get_name, MPI_Type_set_name, MPI_Win_get_name, MPI_Win_set_name

.N WEB
@*/
int
MPI_Comm_set_name(MPI_Comm comm, char *name)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMSETNAME);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMSETNAME, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (name == 0) {
		return(lam_errfunc(comm,
			BLKMPICOMMSETNAME, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_strncpy(comm->c_name, name, MPI_MAX_OBJECT_NAME);
	comm->c_name[MPI_MAX_OBJECT_NAME - 1] = 0;
	if (LAM_TRACE_TOP())
	  lam_tr_commname(comm->c_name, comm->c_contextid, MPI_Wtime());

	lam_resetfunc_m(BLKMPICOMMSETNAME);
	
	/* Force TotalView DLL to take note of this name setting */
	++lam_tv_comm_seqnum;

	return(MPI_SUCCESS);
}
