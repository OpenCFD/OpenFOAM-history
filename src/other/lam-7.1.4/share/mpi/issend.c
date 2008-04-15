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
 * $Id: issend.c,v 6.11 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- immediate synchronous send
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- tag
 *			- communicator
 *			- request (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>


/*@
    MPI_Issend - Starts a nonblocking synchronous send

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (integer) 
. dtype - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Output Parameter:
. req - communication request (handle) 

Notes:

The communication started with this function is not guaranteed to
progress or complete until 'req' has been given to one of the test or
wait functions ('MPI_Test', 'MPI_Testall', 'MPI_Testany',
'MPI_Testsome', 'MPI_Wait', 'MPI_Waitall', 'MPI_Waitany',
'MPI_Waitsome').

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK
.N MPI_ERR_EXHAUSTED

.seealso MPI_Ssend, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Issend(void *buf, int count, MPI_Datatype dtype, 
	       int dest, int tag, MPI_Comm comm, 
	       MPI_Request *req)
{
	int		err;
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIISSEND);

	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIISSEND,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	if ((fl_trace = LAM_TRACE_TOP())) startt = MPI_Wtime();
/*
 * Start the send.
 */
	err = lam_isend(buf, count, dtype, dest, tag, comm, req, LAM_RQISSEND);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIISSEND, err));
	}
/*
 * Generate a run time trace.
 */
	if (fl_trace && dest != MPI_PROC_NULL) {
		finisht = MPI_Wtime();
		lam_tr_msg(TRTOUTPUT, startt, LAM_S2US(finisht - startt), 0,
				dest, tag, comm, dtype, count, 0, 0,
				(*req)->rq_seq, LAM_RQISSEND);
	}

	lam_resetfunc_m(BLKMPIISSEND);
	return(MPI_SUCCESS);
}
