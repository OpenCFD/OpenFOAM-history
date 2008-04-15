/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 *	Function:	- MPI_Keyval_create F77	wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_keyval_create_(cf, df, k, es, ierr)

void	(*cf) ();
void	(*df) ();
int	*k, *es, *ierr;

{
	MPI_Copy_function 	*cf_C;
	MPI_Delete_function 	*df_C;
	struct _attrkey 	*pk;

	if ((cf == mpi_null_copy_fn_ || cf == mpi_comm_null_copy_fn_)) {
	    cf_C = 0;
	}
#if LAM_FORTRANCAPS
/*
 * In this case the C and Fortran are the same. Check against the C function
 * and replace it with a Fortran version if there is a match.
 */
	else if ((void *) cf == (void *) MPI_DUP_FN) {
	    cf_C = (MPI_Copy_function *) MPI_F_DUP_FN;
	}
	else if (cf == (void *) MPI_COMM_DUP_FN) {
	    cf_C = (MPI_Copy_function *) MPI_F_COMM_DUP_FN;
	}
#endif
	else {
	    cf_C =  (MPI_Copy_function *) cf;
	}

	df_C = (df == mpi_null_delete_fn_ || df == mpi_comm_null_delete_fn_)
	    		? NULL : (MPI_Delete_function *) df;

	*ierr =	MPI_Keyval_create(cf_C, df_C, k, 
				  (void *) LAM_INT_TO_VOID_P *es);

	if (*ierr == MPI_SUCCESS) {
		pk = lam_getattr(*k);
		pk->ak_flags |= LAM_LANGF77;
	}
}
