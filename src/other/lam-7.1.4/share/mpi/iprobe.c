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
 * $Id: iprobe.c,v 6.12 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- non-blocking check of incoming messages
 *			- message is not received
 *	Accepts:	- source rank
 *			- message tag
 *			- communicator
 *			- flag (out)
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>


/*@
    MPI_Iprobe - Nonblocking test for a message

Input Parameters:
+ src - source rank, or  'MPI_ANY_SOURCE' (integer) 
. tag - tag value or  'MPI_ANY_TAG' (integer) 
- comm - communicator (handle) 

Output Parameter: 
+ flag - 1 if the message is ready to be received, 0 if it is not
(logical)
- stat - status object (Status), which may be the MPI constant
'MPI_STATUS_IGNORE'

Notes:

This function does not actually receive a message; it only indicates
that a message matching the signature specified is ready to be
received.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_COMM
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Iprobe(int src, int tag, MPI_Comm comm, 
	       int *flag, MPI_Status *stat)
{
	struct _req	request_storage;	/* request structure */
	MPI_Request	req;			/* request */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	int		r;
	double		startt = 0.0;		/* start time */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIIPROBE);
/*
 * Check additional arguments.
 */
	if (flag == 0) {
		return(lam_errfunc(comm, BLKMPIIPROBE,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((tag < 0 && tag != MPI_ANY_TAG) || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIIPROBE,
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
		
		*flag = 1;
		lam_resetfunc_m(BLKMPIIPROBE);
		return(MPI_SUCCESS);
	}

	if ((fl_trace = LAM_TRACE_TOP())) startt = MPI_Wtime();
/*
 * Advance the system to give the probe a better chance of success.
 */
	_mpi_req_blkclr();

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIPROBE, err));
	}

	LAM_ZERO_ME(request_storage);
	req = &request_storage;
	err = _mpi_req_build((char *) 0, 0, MPI_BYTE, src, tag, comm,
			LAM_RQIPROBE, &req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIPROBE, err));
	}

	err = _mpi_req_start(req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIPROBE, err));
	}

	r = RPI_IPROBE(req);
	if (r < 0) {
		return(lam_errfunc(comm, BLKMPIIPROBE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIPROBE, err));
	}
/*
 * Copy the status info if synchronized.
 */
	if (r == 1) {
		*flag = 1;
		if (stat != MPI_STATUS_IGNORE) {
			*stat = req->rq_status;
		}
	} else {
		*flag = 0;
	}

	err = _mpi_req_destroy(&req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIPROBE, err));
	}
/*
 * Generate a run time trace.
 */
	if (fl_trace) {
		lam_tr_msg(TRTNOIO, startt, LAM_S2US(MPI_Wtime() - startt),
			0, src, tag, comm, 0, 0, 0, 0, 0, LAM_RQIPROBE);
	}

	lam_resetfunc_m(BLKMPIIPROBE);
	return(MPI_SUCCESS);
}
