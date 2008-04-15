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
 * $Id: probe.c,v 6.10 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- blocking check of incoming messages
 *			- message is not received
 *	Accepts:	- source rank
 *			- message tag
 *			- communicator
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>


/*@
    MPI_Probe - Blocking test for a message

Input Parameters:
+ src - source rank, or 'MPI_ANY_SOURCE' (integer) 
. tag - tag value or 'MPI_ANY_TAG' (integer) 
- comm - communicator (handle) 

Output Parameter:
. stat - status object (Status), which may be the MPI constant
'MPI_STATUS_IGNORE'

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Probe(int src, int tag, MPI_Comm comm, 
	      MPI_Status *stat)
{
	struct _req	request;		/* request structure */
	MPI_Request	req;			/* request */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIPROBE);

	if ((tag < 0 && tag != MPI_ANY_TAG) || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIPROBE,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}
/*
 * Handle the trivial case.
 */
	if (src == MPI_PROC_NULL) {
		if (stat != MPI_STATUS_IGNORE) {
			stat->MPI_ERROR = MPI_SUCCESS;
			stat->MPI_SOURCE = MPI_PROC_NULL;
			stat->MPI_TAG = MPI_ANY_TAG;
			stat->st_length = 0;
		}

		lam_resetfunc_m(BLKMPIPROBE);
		return(MPI_SUCCESS);
	}

	if ((fl_trace = LAM_TRACE_TOP())) {
		startt = MPI_Wtime();
		_kio.ki_blktime = 0.0;
	}
/*
 * Build a probe request.
 * We pass MPI_BYTE just to keep the argument checker quiet.
 */
	LAM_ZERO_ME(request);
	req = &request;
	err = _mpi_req_build((char *) 0, 0, MPI_BYTE, src, tag, comm,
			LAM_RQIPROBE, &req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIPROBE, err));
	
	err = _mpi_req_start(req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIPROBE, err));

	_mpi_req_add(req);
	_mpi_req_blkclr();
	_mpi_req_blkset(req);

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIPROBE, err));

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIPROBE, err));

	if (stat != MPI_STATUS_IGNORE) {
		*stat = req->rq_status;
	}

	_mpi_req_rem(req);

	err = _mpi_req_destroy(&req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIPROBE, err));
/*
 * Generate a run time trace.
 */
	if (fl_trace) {
		lam_tr_msg(TRTNOIO, startt,
			LAM_S2US(MPI_Wtime() - startt - _kio.ki_blktime),
			LAM_S2US(_kio.ki_blktime), src, tag, comm,
			0, 0, 0, 0, 0, LAM_RQIPROBE);
	}

	lam_resetfunc_m(BLKMPIPROBE);
	return(MPI_SUCCESS);
}
