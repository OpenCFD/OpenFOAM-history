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
 * $Id: start.c,v 6.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- start communication request
 *	Accepts:	- ptr to request
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 * external functions
 */
extern int		lam_errfunc();
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();


/*@
    MPI_Start - Initiates a communication with a persistent request handle

Input Parameter:
. req - communication request (handle) 

Notes:

This function starts a communication that was previously initialized
with any of the 'MPI_*_init' functions.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_REQUEST

.seealso MPI_Bsend_init, MPI_Request_free, MPI_Rsend_init, MPI_Send_init, MPI_Ssend_init, MPI_Startall, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Start(MPI_Request *req)
{
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPISTART);

	if (req == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPISTART, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (*req == MPI_REQUEST_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPISTART, lam_mkerr(MPI_ERR_REQUEST, EINVAL)));
	}

	err = MPI_Startall(1, req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc((*req)->rq_comm, BLKMPISTART, err));
	}

	lam_resetfunc(BLKMPISTART);
	return(MPI_SUCCESS);
}
