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
 * $Id: errfree.c,v 6.8 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- free error handler
 *	Accepts:	- ptr error handler
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <MPISYSF.h>

/*@
  MPI_Errhandler_free - Frees an MPI-style errorhandler

Input Parameter:
. errhandler - MPI error handler (handle).  Set to 'MPI_ERRHANDLER_NULL' on 
exit.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Errhandler_free(MPI_Errhandler* errhdl)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIERRFREE);
/*
 * Check the arguments.
 */
	if (errhdl == 0 || *errhdl == MPI_ERRHANDLER_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRFREE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Only internal library calls can free predefined handlers.
 */
	if (lam_numfuncs() == 1 && (*errhdl == MPI_ERRORS_ARE_FATAL
			|| *errhdl == MPI_ERRORS_RETURN)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRFREE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Free the handler.
 */
	if (--((*errhdl)->eh_refcount) == 0) {

		if (*errhdl == MPI_ERRORS_ARE_FATAL
				|| *errhdl == MPI_ERRORS_RETURN) {
			return(lam_errfunc(MPI_COMM_WORLD,
					   BLKMPIERRFREE, 
					   lam_mkerr(MPI_ERR_INTERN, EPERM)));
		}

		if ((*errhdl)->eh_f77handle >= 0) {
			lam_F_free_hdl((*errhdl)->eh_f77handle);
		}

		free((char *) *errhdl);
	}

	*errhdl = MPI_ERRHANDLER_NULL;

	lam_resetfunc_m(BLKMPIERRFREE);
	return(MPI_SUCCESS);
}
