/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: startall.c,v 6.10 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- start several communication requests
 *	Accepts:	- # requests
 *			- requests array
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <typical.h>

/*
 * external functions
 */
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();
extern void		_mpi_req_add();
extern void		_mpi_req_blkclr();
extern void		lam_tr_startall();
extern int		lam_errfunc();
extern int		lam_mkerr();
extern int		_mpi_req_advance();
extern int		_mpi_req_build();
extern int		_mpi_req_start();

/*
 * local functions
 */
static int		hidden_bsend(MPI_Request req);


/*@
  MPI_Startall - Starts a collection of requests 

Input Parameters:
+ nreq - list length (integer) 
- reqs - array of requests (array of handle) 

Notes:

This function starts a collection of communications that were
previously initialized with and of the 'MPI_*_init' functions.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_COUNT
.N MPI_ERR_REQUEST

.seealso MPI_Bsend_init, MPI_Request_free, MPI_Rsend_init, MPI_Send_init, MPI_Ssend_init, MPI_Startall, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Startall(int nreq, MPI_Request *reqs)
{
	int		err;			/* error code */
	int		i;			/* favourite index */
	MPI_Request	*p;			/* favourite pointer */
	MPI_Request	req;			/* request */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		advt = 0.0;		/* advance time */
	double		finisht;		/* finish time */

	lam_initerr();
	lam_setfunc(BLKMPISTARTALL);
/*
 * Check the arguments.
 */
	if (nreq < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPISTARTALL,
				   lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((nreq > 0) && (reqs == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPISTARTALL,
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((fl_trace = LAM_TRACE_TOP())) startt = MPI_Wtime();
/*
 * Loop linking the requests.
 */
	for (i = 0, p = reqs; i < nreq; ++i, ++p) {

		if ((req = *p) == MPI_REQUEST_NULL) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPISTARTALL,
					   lam_mkerr(MPI_ERR_REQUEST, 
						     EINVAL)));
		}

		if (req->rq_state != LAM_RQSINIT) {
			return(lam_errfunc(req->rq_comm, BLKMPISTARTALL,
					   lam_mkerr(MPI_ERR_REQUEST, 
						     EINVAL)));
		}
/*
 * If a Bsend_init() request is found, declare it done,
 * and create and link its hidden Bsend() counterpart.
 */
		if (req->rq_type == LAM_RQIFAKE) {
			req->rq_state = LAM_RQSDONE;
			err = hidden_bsend(req);
		} else {
			err = _mpi_req_start(req);

			if (err == MPI_SUCCESS) {
				_mpi_req_add(req);
			}
		}

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(req->rq_comm,
					BLKMPISTARTALL, err));
		}
	}
/*
 * Advance the system.
 */
	if (fl_trace) advt = MPI_Wtime();

	_mpi_req_blkclr();

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPISTARTALL, err));
	}
/*
 * Generate a run time trace.
 */
	if (fl_trace) {

		finisht = MPI_Wtime();

		lam_tr_startall(nreq, reqs, startt, advt, finisht);
	}

	lam_resetfunc(BLKMPISTARTALL);
	return(MPI_SUCCESS);
}

/*
 *	hidden_bsend
 *
 *	Function:	- build, start and link in a hidden bsend request
 *	Accepts:	- user bsend request
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
hidden_bsend(MPI_Request req)
{
	MPI_Request	hreq = 0;		/* hidden request */
	int		cidsave;		/* save context ID */
	int		err;			/* error code */
/*
 * Handle special case of peer MPI_PROC_NULL.
 */
	if (req->rq_rank == MPI_PROC_NULL) {
		return(MPI_SUCCESS);
	}
/*
 * Make sure to use the original request's context ID.  This is to take
 * care of the case where an MPI_Bsend_init is done outside all
 * collective calls and then started inside a collective call or
 * vice-versa.
 */
	cidsave = req->rq_comm->c_contextid;
	req->rq_comm->c_contextid = req->rq_cid;
	
	err = _mpi_req_build(req->rq_buf, req->rq_count, req->rq_dtype,
				req->rq_rank, req->rq_tag, req->rq_comm,
				LAM_RQIBSEND, &hreq);

	req->rq_comm->c_contextid = cidsave;
	if (err != MPI_SUCCESS) return(err);
#if LAM_WANT_IMPI
/*
 * Be sure to chain on any shadow requests that are on the original bsend 
 */
	hreq->rq_shadow = req->rq_shadow;
#endif
	err = _mpi_req_start(hreq);
	if (err != MPI_SUCCESS) return(err);

	req->rq_seq = hreq->rq_seq;
	_mpi_req_add(hreq);
	return(MPI_SUCCESS);
}
