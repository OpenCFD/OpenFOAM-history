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
 * $Id: reqfree.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- free communication request
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
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();
extern void		_mpi_req_rem();
extern int		lam_errfunc();
extern int		lam_mkerr();
extern int		_mpi_req_destroy();
extern int		_mpi_req_end();

/*@
    MPI_Request_free - Frees a communication request object

Input Parameter:
. preq - communication request (handle) 

Notes:

This routine is normally used to free persistent requests created with
either 'MPI_Recv_init' or 'MPI_Send_init' (and friends).  However, it
can be used to free a request created with 'MPI_Irecv' or 'MPI_Isend'
(and friends); in that case the user can not use the test/wait
routines on the request.

It `is` permitted to free an active request.  However, once freed, you
can not use the request in a wait or test routine (e.g., 'MPI_Wait').

'preq' is set to 'MPI_REQUEST_NULL' upon completion.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG

.see also: MPI_Isend, MPI_Irecv, MPI_Issend, MPI_Ibsend, MPI_Irsend, MPI_Recv_init, MPI_Send_init, MPI_Ssend_init, MPI_Rsend_init, MPI_Wait, MPI_Test, MPI_Waitall, MPI_Waitany, MPI_Waitsome, MPI_Testall, MPI_Testany, MPI_Testsome

.N ACK
@*/
int MPI_Request_free(MPI_Request *preq)
{
	MPI_Request	req;			/* request */
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPIREQFREE);
/*
 * Check arguments.
 */
	if (preq == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIREQFREE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((req = *preq) == MPI_REQUEST_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIREQFREE, lam_mkerr(MPI_ERR_REQUEST, EINVAL)));
	}
/*
 * Destroy all the shadows
 */
	if (req->rq_shadow != MPI_REQUEST_NULL)
	  MPI_Request_free(&req->rq_shadow);
/*
 * If active, flag it for destruction.
 * Otherwise destroy it.
 */
	if (req->rq_state == LAM_RQSACTIVE || req->rq_state == LAM_RQSSTART) {

		req->rq_flags |= LAM_RQFORPHAN;
	}
	else {
		_mpi_req_rem(req);

		if (req->rq_state == LAM_RQSDONE) {
			err = _mpi_req_end(req);
			if (err != MPI_SUCCESS) {
				return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPIREQFREE, err));
			}
		}

		err = _mpi_req_destroy(preq);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(MPI_COMM_WORLD,
					BLKMPIREQFREE, err));
		}
	}

	*preq = MPI_REQUEST_NULL;
	
	lam_resetfunc(BLKMPIREQFREE);
	return(MPI_SUCCESS);
}
