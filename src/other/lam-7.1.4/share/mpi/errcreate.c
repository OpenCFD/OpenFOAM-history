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
 * $Id: errcreate.c,v 6.6 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- create error handler
 *	Accepts:	- error function
 *			- error handler (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
  MPI_Errhandler_create - Creates an MPI-style error handler on a communicator

Input Parameter:
. function - user defined error handling procedure 

Output Parameter:
. errhandler - MPI error handler (handle) 

.N Wrapper MPI_Comm_create_errhandler

.N ACK
@*/
int MPI_Errhandler_create(MPI_Handler_function *errfunc, 
			  MPI_Errhandler *errhdl)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIERRCREATE);
/*
 * Check the arguments.
 */
	if ((errfunc == 0) || (errhdl == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRCREATE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the handler.
 */
	*errhdl = (MPI_Errhandler) malloc((unsigned) sizeof(struct _errhdl));
	if (*errhdl == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRCREATE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*errhdl)->eh_func = (void (*)()) errfunc;
	(*errhdl)->eh_f77handle = -1;
	(*errhdl)->eh_refcount = 1;
	(*errhdl)->eh_flags = LAM_EHCOMM;

	lam_resetfunc_m(BLKMPIERRCREATE);
	return(MPI_SUCCESS);
}
