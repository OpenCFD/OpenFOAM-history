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
 * $Id: cancel.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- marks immediate request for cancellation
 *	Accepts:	- ptr request
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
    MPI_Cancel - Cancels a communication request

Input Parameter:
. preq - communication request (handle) 

Notes:

Cancel has only been implemented for receive requests; it is a no-op
for send requests.  The primary expected use of MPI_Cancel is in
multi-buffering schemes, where speculative 'MPI_Irecv' calls are made.
When the computation completes, some of these receive requests may
remain; using 'MPI_Cancel' allows the user to cancel these unsatisfied
requests.

Cancelling a send operation is `much` more difficult, in large part
because the send will usually be at least partially complete (the
information on the tag, size, and source are usually sent immediately
to the destination).  As such, LAM/MPI has not implemented the
canceling of send operations.

.N NULL

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cancel(MPI_Request *preq)
{
	MPI_Request	req;

	lam_initerr();
	lam_setfunc(BLKMPICANCEL);
/*
 * Check the arguments.
 */
	if (preq == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICANCEL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((req = *preq) == MPI_REQUEST_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICANCEL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (req->rq_state == LAM_RQSINIT) {
		return(lam_errfunc(req->rq_comm,
			BLKMPICANCEL, lam_mkerr(MPI_ERR_REQUEST, EINVAL)));
	}
/*
 * In the start state, nothing has happened, so cancel is easy.
 * Move the request to the done state.
 */
	if (req->rq_state == LAM_RQSSTART) {
		req->rq_flags |= LAM_RQFCANCEL;
		req->rq_status.MPI_SOURCE = MPI_CANCEL_SOURCE;
		req->rq_state = LAM_RQSDONE;
		lam_rq_nactv--;
	}
/*
 * In the active state, it may still be possible to cancel a send operation.
 * This is an immense pain for many C2C implementations and the same
 * resource cleanup goal can be achieved by having the receiver suck
 * in the message.  Cancelling an active send is no problem for lamd.
 * We just make a remote service call to bufferd and ask that the MPI
 * sequence number from this source be nuked.  We set the cancel flag
 * based on the return code.  I could add this in half an hour.  Screw it.
 */
	else if ((req->rq_state == LAM_RQSACTIVE) &&
			(req->rq_marks & LAM_RQFDEST)) {
	}
/*
 * In the done state, the cancel fails.
 */
	else {
	}

	lam_resetfunc(BLKMPICANCEL);
	return(MPI_SUCCESS);
}
