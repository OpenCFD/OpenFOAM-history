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
 * $Id: opcreate.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- create a reduction operator
 *	Accepts:	- function
 *			- commutative flag
 *			- operator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
  MPI_Op_create - Creates a user-defined combination function handle

Input Parameters:
+ func - user defined function (function) 
- commute -  true if commutative;  false otherwise. 

Output Parameter:
. pop - operation (handle) 

Notes on the user function:

The calling list for the user function type is

.vb
 typedef void (MPI_User_function) ( void * a, 
               void * b, int * len, MPI_Datatype * ); 
.ve

where the operation is 'b[i] = a[i] op b[i]', for 'i = 0, ... ,
len-1'.  A pointer to the datatype given to the MPI collective
computation routine (i.e., 'MPI_Reduce', 'MPI_Allreduce', 'MPI_Scan',
or 'MPI_Reduce_scatter') is also passed to the user-specified routine.

Notes:

When the 'MPI_Op' created by this function is no longer being used, it
should be freed with 'MPI_Op_free'.

.N fortran

.N collops

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OP
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Op_free

.N ACK
@*/
int MPI_Op_create(MPI_User_function func, int commute, 
		  MPI_Op *pop)
{
	int		class;
	MPI_Op		op;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIOPCREATE);

	if ((pop == 0) || (func == 0)) {
		class = (pop == 0) ? MPI_ERR_OP : MPI_ERR_ARG;
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIOPCREATE, lam_mkerr(class, EINVAL)));
	}

	op = (MPI_Op) malloc(sizeof(struct _op));
	if (op == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIOPCREATE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*pop = op;
	op->op_func = func;
	op->op_commute = commute;
	op->op_f77handle = -1;
	op->op_flags = 0;

	lam_resetfunc_m(BLKMPIOPCREATE);
	return(MPI_SUCCESS);
}
