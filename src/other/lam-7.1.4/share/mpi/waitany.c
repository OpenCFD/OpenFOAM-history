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
 * $Id: waitany.c,v 6.14 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- wait for first request to finish
 *	Accepts:	- # of requests
 *			- array of requests
 *			- index of completed request (out)
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 *			- in the case of an error return index is set to
 *			  MPI_UNDEFINED if the error is not associated with
 *			  the completed request, e.g. an internal error
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#include <lamdebug.h>


/*
 * external variables
 */
extern struct _proc     *lam_impid_proc;


/*@
    MPI_Waitany - Waits for any specified send or receive to complete

Input Parameters:
+ count - list length (integer) 
- reqs - array of requests (array of handles) 

Output Parameters:
+ index - index of handle for operation that completed (integer).  In
the range '0' to 'count-1'.  In Fortran, the range is '1' to 'count'.
- stat - status object (Status), or the MPI constant
'MPI_STATUS_IGNORE'

Notes:

If all of the requests are 'MPI_REQUEST_NULL', then 'index' is
returned as 'MPI_UNDEFINED', and 'stat' is returned as an empty
status.

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_COUNT
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Waitany(int count, MPI_Request *reqs, int *index, 
		MPI_Status *stat)
{
	int		i;
	int		nactive;		/* # active requests */
	int		errstat;		/* error in status */
	int		err;			/* error code */
	MPI_Comm	comm;			/* communicator */
	MPI_Request	req = MPI_REQUEST_NULL;	/* request */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */
#if LAM_WANT_IMPI
	int		flag;			/* lam_test flag */
	MPI_Request	cur, next;		/* request */
#endif

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWAITANY);

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITANY, 
				lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && (reqs == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITANY,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((index == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITANY,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((fl_trace = LAM_TRACE_TOP())) {
		startt = MPI_Wtime();
		_kio.ki_blktime = 0.0;
	}

	if (stat != MPI_STATUS_IGNORE) {
		lam_emptystat(stat);
	}
/*
 * While no request is done.
 */
	while (1) {
/*
 * Loop over the requests.
 */
		_mpi_req_blkclr_m();

		for (i = 0, nactive = 0; i < count; ++i) {
			req = reqs[i];
/*
 * Skip inactive requests.
 */
			if ((req == MPI_REQUEST_NULL) ||
					(req->rq_state == LAM_RQSINIT))
					continue;

#if LAM_WANT_IMPI
/*
 * Need to check entire request, including all shadow requests.
 * lam_test is already set to do this; we just set the destroy flag to
 * 0 so that it doesn't reap the request(s).
 */
			lam_test(&req, 0, &flag, MPI_STATUS_IGNORE);
			if (flag)
			  break;
#else
			if (req->rq_state == LAM_RQSDONE) break;
#endif
/*
 * It's an active request.
 */
			++nactive;
			_mpi_req_blkset_m(req);
#if LAM_WANT_IMPI
/*
 * Let the shadows progress as well
 *
 * Optimization: if we have a shadow and the main request is an
 * issend, we can probably mark the main request as nonblocking; this
 * will let the progress engine block on the shadow rather than
 * spinning over multiple blocking requests.  Didn't do this [yet].
 */
			for (cur = req->rq_shadow; cur != MPI_REQUEST_NULL;
			     cur = cur->rq_shadow)
			  _mpi_req_blkset_m(cur);
#endif
		}
/*
 * We found a completed request.
 */
		if (i < count) {
			*index = i;
			break;
		}
/*
 * If no active requests try to progress and we're done.
 */
		else if (nactive == 0) {
			*index = MPI_UNDEFINED;

			err = _mpi_req_advance();
			if (err != MPI_SUCCESS) {
				*index = MPI_UNDEFINED;
				return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPIWAITANY, err));
			}
			
			lam_resetfunc_m(BLKMPIWAITANY);
			return(MPI_SUCCESS);
		}
/*
 * Otherwise block on any and loop again.
 */
		else {
			err = _mpi_req_advance();
			if (err != MPI_SUCCESS) {
				*index = MPI_UNDEFINED;
				return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPIWAITANY, err));
			}
		}
	}
/*
 * Finish the completed request.
 */
	_mpi_req_rem_m(req);

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITANY, err));
	}

	if (stat != MPI_STATUS_IGNORE) {
		*stat = req->rq_status;
	}
	comm = req->rq_comm;
	errstat = req->rq_status.MPI_ERROR;
/*
 * Generate a run time trace except in the case of cancelled request or
 * a request where the peer is MPI_PROC_NULL.
 */
	if (fl_trace && (req->rq_rank != MPI_PROC_NULL)
			&& !(req->rq_flags & LAM_RQFCANCEL)) {
		finisht = MPI_Wtime();

		lam_tr_msg(
			(req->rq_type == LAM_RQIRECV) ? TRTINPUT : TRTNOIO,
			startt, LAM_S2US(finisht - startt - _kio.ki_blktime),
			LAM_S2US(_kio.ki_blktime), req->rq_rank, req->rq_tag,
			req->rq_comm, req->rq_dtype, req->rq_count,
			req->rq_status.MPI_SOURCE, req->rq_status.MPI_TAG,
			req->rq_seq, req->rq_type);
	}
#if LAM_WANT_IMPI
/*
 * Now that we found a request that is done, see if it has a shadow
 * request that finished.  If it has, reap it.  If not, cancel it.  If
 * LAM_RQFMAND is set, then we won't have gotten here unless all the
 * shadows have completed as well.  If it is not set, we can reap
 * and/or cancel all the shadows.  
 */
	for (cur = req->rq_shadow; cur != MPI_REQUEST_NULL; cur = next) {
	  next = cur->rq_shadow;
          MPI_Cancel(&cur);
	  lam_test(&cur, 1, &flag, MPI_STATUS_IGNORE);
	} 
#endif
/*
 * Reset persistent requests.
 * Destroy requests that are not persistent.
 */
	if (req->rq_marks & LAM_RQFPERSIST) {
		req->rq_state = LAM_RQSINIT;
	} else {
		err = _mpi_req_destroy(&reqs[i]);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(MPI_COMM_WORLD,
					BLKMPIWAITANY, err));
		}
	}

	if (errstat != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWAITANY, errstat));
	}

	lam_resetfunc_m(BLKMPIWAITANY);
	return(MPI_SUCCESS);
}
