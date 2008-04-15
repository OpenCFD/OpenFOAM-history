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
 * $Id: opfree.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- free a reduction operator
 *	Accepts:	- operator (inout)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
  MPI_Op_free - Frees a user-defined combination function handle

Input Parameter:
. op - operation (handle) 

Notes:

'op' is set to 'MPI_OP_NULL' on exit.

.N NULL

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_PERM_OP

.seealso: MPI_Op_create

.N ACK
@*/
int MPI_Op_free(MPI_Op *op)
{
	lam_initerr();
	lam_setfunc(BLKMPIOPFREE);

	if (op == 0 || (*op == MPI_OP_NULL)
			|| ((*op)->op_flags & LAM_PREDEF)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIOPFREE, lam_mkerr(MPI_ERR_OP, EINVAL)));
	}

/*
 * Remove the fortran handle if the user called MPI_Op_f2c on this
 * handle
 */
	if ((*op)->op_f77handle >= 0)
	  lam_F_free_hdl((*op)->op_f77handle);

	free((char *) *op);
	*op = MPI_OP_NULL;

	lam_resetfunc(BLKMPIOPFREE);
	return(MPI_SUCCESS);
}
