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
 * $Id: irecv.c,v 6.23 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- immediate receive
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- source
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
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>


/*@
    MPI_Irecv - Begins a non-blocking receive

Input Parameters:
+ buf - initial address of receive buffer (choice) 
. count - number of elements in receive buffer (integer) 
. dtype - datatype of each receive buffer element (handle) 
. src - rank of source (integer) 
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
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.seealso MPI_Recv, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Irecv(void *buf, int count, MPI_Datatype dtype, 
	      int src, int tag, MPI_Comm comm, 
	      MPI_Request *req)
{
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIIRECV);

	if ((tag < 0 && tag != MPI_ANY_TAG) || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIIRECV,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	if ((fl_trace = LAM_TRACE_TOP())) startt = MPI_Wtime();

	*req = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, src, tag, comm,
			LAM_RQIRECV, req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIIRECV, err));
	
	(*req)->rq_marks |= LAM_RQFMAND;

	err = _mpi_req_start(*req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIIRECV, err));

	_mpi_req_add(*req);
	_mpi_req_blkclr();

#if LAM_WANT_IMPI
	IMPI_Register_datasync(src, tag, comm, &((*req)->rq_shadow));
#endif

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIIRECV, err));
/*
 * Generate a run time trace.  */
	if (fl_trace && src != MPI_PROC_NULL) {
		lam_tr_msg(TRTNOIO, startt, LAM_S2US(MPI_Wtime() - startt),
				0, src, tag, comm, dtype, count, 0, 0,
				(*req)->rq_seq, LAM_RQIRECV);
	}

	lam_resetfunc_m(BLKMPIIRECV);
	return(MPI_SUCCESS);
}
