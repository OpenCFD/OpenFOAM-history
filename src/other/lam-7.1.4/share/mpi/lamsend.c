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
 * $Id: lamsend.c,v 6.28 2003/10/28 12:39:44 jsquyres Exp $
 *
 *	Function:	- shared code for send functions
 */

#include <errno.h>

#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#include <typical.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>
#include <blktype.h>
#include <lam-ssi-rpi.h>

/*
 * external variables
 */
extern struct _proc     *lam_impid_proc;


/*
 *	lam_send
 *
 *	Function:	- send a blocking MPI message
 *	Accepts:	- MPI send args
 *			- request type
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_send(void *buf, int count, MPI_Datatype dtype, int dest, int tag,
	 MPI_Comm comm, int reqtype)
{
	int		err;
	int		fl_trace;		/* do tracing? */
	int		req_err;		/* recoverable error */
	int		seqnum;			/* message sequence number */
	struct _req	req_storage;
	MPI_Request	req;
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */
#if LAM_WANT_IMPI
	int		flag;			
#endif
/*
 * Handle the trivial cases.
 */
	if (dest == MPI_ANY_SOURCE) 
	  return(lam_mkerr(MPI_ERR_RANK, EINVAL));

	if (dest == MPI_PROC_NULL) 
	  return(MPI_SUCCESS);

	if ((fl_trace = LAM_TRACE_TOP())) {
		startt = MPI_Wtime();
		_kio.ki_blktime = 0.0;
	}

#if LAM_WANT_IMPI
/* 
 * If IMPI_Send_lamgiappe returns flag=1, this is an IMPI proxy send.
 * Need to re-route to the impid.
 */
	err = IMPI_Send_lamgiappe(reqtype, count, dtype, dest, tag, comm, 
				  &flag);
	if (err != MPI_SUCCESS)
	  return err;
	if (flag == 1) {
	  dest = impid_comm->c_group->g_nprocs - 1;
	  tag  = IMPI_MESSAGE_TAG;
	  comm = impid_comm;
	}
#endif

	if (RPI_HAS_FASTSEND(comm, dest) && reqtype == LAM_RQISEND && 
	    lam_rq_nactv == 0) {
	    if (comm == MPI_COMM_NULL)
		return(lam_mkerr(MPI_ERR_COMM, EINVAL));

	    if (LAM_IS_INTER(comm) || dest != comm->c_group->g_myrank) {
                errno = 0;
		err = RPI_FASTSEND(buf, count, dtype, dest, tag, comm);

                /* The RPI's "fast" methods may be temporarily
                   unavailable.  If the "fast" method returns
                   something other than MPI_SUCCESS, and errno ==
                   EBUSY, then just fail gracefully over to the normal
                   send.  In all other cases, handle the
                   success/failure here. */

                if (err == MPI_SUCCESS ||
                    (err != MPI_SUCCESS && errno != EBUSY)) {

                  /* Generate a trace */

                  if (fl_trace) {
                    finisht = MPI_Wtime();

                    lam_tr_msg(TRTOUTPUT, startt,
                               LAM_S2US(finisht - startt - _kio.ki_blktime),
                               LAM_S2US(_kio.ki_blktime), dest, tag,
                               comm, dtype, count, 0, 0, lam_seqnum-1, 
                               reqtype);
                  }

                  return(err);
                }
	    }
	}

	LAM_ZERO_ME(req_storage);
	req = &req_storage;
	err = _mpi_req_build(buf, count, dtype, dest, tag, comm, reqtype, 
			     &req);
	if (err != MPI_SUCCESS) 
	  return(err);

	req->rq_marks |= (LAM_RQFBLKTYPE | LAM_RQFMAND);

	err = _mpi_req_start(req);
	if (err != MPI_SUCCESS) 
	  return(err);

	_mpi_req_add_m(req);
	_mpi_req_blkclr_m();
	_mpi_req_blkset_m(req);

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) 
	  return(err);

	req_err = req->rq_status.MPI_ERROR;
	seqnum = req->rq_seq;

	_mpi_req_rem_m(req);

	err = _mpi_req_destroy(&req);
	if (err != MPI_SUCCESS) 
	  return(err);
/*
 * Generate a trace.
 */
	if (fl_trace) {
		finisht = MPI_Wtime();

		lam_tr_msg(TRTOUTPUT, startt,
				LAM_S2US(finisht - startt - _kio.ki_blktime),
				LAM_S2US(_kio.ki_blktime), dest, tag,
				comm, dtype, count, 0, 0, seqnum, reqtype);
	}

	return(req_err);
}


/*
 *	lam_isend
 *
 *	Function:	- send a non-blocking MPI message
 *	Accepts:	- MPI isend args
 *			- request type
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_isend(void *buf, int count, MPI_Datatype dtype, int dest, int tag,
	  MPI_Comm comm, MPI_Request *preq, int reqtype)
{
	int		err;
#if LAM_WANT_IMPI
	MPI_Request     lreq;
#endif

	if (dest == MPI_ANY_SOURCE) 
	  return(lam_mkerr(MPI_ERR_RANK, EINVAL));

#if LAM_WANT_IMPI
	/* If IMPI_Isend_lamgiappe fills in a request, this is an IMPI
           proxy send.  Need to re-route to the impid.  */

	/* Values that reqtype can be: lam_rqibsend, lam_rqirsend,
           lam_rqisend, lam_rqissend */
	IMPI_Isend_lamgiappe(reqtype, count, dtype, dest, tag, 
			     comm, &lreq);
	if (lreq != MPI_REQUEST_NULL) {
	  dest = impid_comm->c_group->g_nprocs - 1;
	  tag  = IMPI_MESSAGE_TAG;
	  comm = impid_comm;
	}
#endif

	*preq = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, dest, tag, comm,
			     reqtype, preq);
	if (err != MPI_SUCCESS) 
	  return(err);

	(*preq)->rq_marks |= LAM_RQFMAND;

	err = _mpi_req_start(*preq);
	if (err != MPI_SUCCESS) 
	  return(err);

#if LAM_WANT_IMPI
/*
 * Important to add the shadow *after* it has been started (so that
 * the shadow doesn't get restarted)
 */
	(*preq)->rq_shadow = lreq;
#endif

	_mpi_req_add_m(*preq);
	_mpi_req_blkclr_m();

	err = _mpi_req_advance();

	return(err);
}
