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
 * $Id: recv.c,v 6.29 2003/10/28 12:39:44 jsquyres Exp $
 *
 *	Function:	- receive a message
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- source
 *			- tag
 *			- communicator
 *			- status (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>
#include <lam-ssi-rpi.h>


/*
 * state variable, has to be public.  Ugh. (in irecv.c)
 */
extern int lam_in_impi_irecv;


/*@
    MPI_Recv - Basic receive

Input Parameters:
+ count - maximum number of elements in receive buffer (integer) 
. dtype - datatype of each receive buffer element (handle) 
. src - rank of source (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Output Parameters:
+ buf - initial address of receive buffer (choice) 
- stat - status object (Status), which can be the MPI constant
'MPI_STATUS_IGNORE' if the return status is not desired

Notes:

The 'count' argument indicates the maximum length of a message; the actual 
number can be determined with 'MPI_Get_count'.  

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Recv(void *buf, int count, MPI_Datatype dtype, 
	     int src, int tag, MPI_Comm comm, MPI_Status *stat)
{
	int		err;
	int		staterr;		/* error in status */
	struct _req	req_storage;
	MPI_Request	req;
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIRECV);

	if ((tag < 0 && tag != MPI_ANY_TAG) || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIRECV,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}
/*
 * Check the bozo MPI_PROC_NULL case.  I'm not even tracing this.
 */
	if (src == MPI_PROC_NULL) {
		if (stat != MPI_STATUS_IGNORE) {
			stat->MPI_ERROR = MPI_SUCCESS;
			stat->MPI_SOURCE = MPI_PROC_NULL;
			stat->MPI_TAG = MPI_ANY_TAG;
			stat->st_length = 0;
		}

		lam_resetfunc_m(BLKMPIRECV);
		return(MPI_SUCCESS);
	}

	if ((fl_trace = LAM_TRACE_TOP())) {
		startt = MPI_Wtime();
		_kio.ki_blktime = 0.0;
	}

#if LAM_WANT_IMPI
/*
 * If we could possibly be receiving from the impid, there's a lot of
 * special things that need to happen.  All that special code has been
 * put into MPI_Irecv, so we just call MPI_Irecv() and MPI_Wait().
 */
	if (LAM_IS_IMPI(comm) &&
	    lam_impid_proc != 0 &&
	    (LAM_GPSCMP(&(comm->c_group->g_procs[src]->p_gps),
			&(lam_impid_proc->p_gps)) == 0 ||
	     src == MPI_ANY_SOURCE)) {
	  MPI_Status status;
	  MPI_Request req;

	  /* All the code for the IMPI special case receive is in
             MPI_Irecv, so just relay the request to there, and then
             MPI_Wait on it */

	  MPI_Irecv(buf, count, dtype, src, tag, comm, &req);
	  MPI_Wait(&req, &status);

	  if (stat != MPI_STATUS_IGNORE)
	    *stat = status;

	  /* Check for error */

	  if (status.MPI_ERROR != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIRECV, status.MPI_ERROR));

	  lam_resetfunc_m(BLKMPIRECV);
	  return (MPI_SUCCESS);
	}
#endif

	if (RPI_HAS_FASTRECV(comm, src) && lam_rq_nactv == 0) {
		if (comm == MPI_COMM_NULL) {
			return(lam_mkerr(MPI_ERR_COMM, EINVAL));
		}

		if (src != MPI_ANY_SOURCE && (LAM_IS_INTER(comm)
				|| src != comm->c_group->g_myrank)) {
                  int seqnum;

                  errno = 0;
                  err = RPI_FASTRECV(buf, count, dtype, src, &tag, 
                                     comm, stat, &seqnum);

                  /* The RPI's "fast" methods may be temporarily
                     unavailable.  If the "fast" method returns
                     something other than MPI_SUCCESS, and errno ==
                     EBUSY, then just fail gracefully over to the
                     normal send.  In all other cases, handle the
                     success/failure here. */

                  if (err == MPI_SUCCESS ||
                      (err != MPI_SUCCESS && errno != EBUSY)) {

                    if (fl_trace) {
                      finisht = MPI_Wtime();

                      lam_tr_msg(TRTINPUT, startt,
                                 LAM_S2US(finisht - startt - _kio.ki_blktime),
                                 LAM_S2US(_kio.ki_blktime), src, tag, comm,
                                 dtype, count, src, tag, seqnum, LAM_RQIRECV);
                    }

                    if (err != MPI_SUCCESS)
                      return lam_errfunc(comm, BLKMPIRECV, err);

                    lam_resetfunc_m(BLKMPIRECV);
                    return (MPI_SUCCESS);
                  }
                }
	}

	LAM_ZERO_ME(req_storage);
	req = &req_storage;
	err = _mpi_req_build(buf, count, dtype, 
				src, tag, comm, LAM_RQIRECV, &req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIRECV, err));

	req->rq_marks |= (LAM_RQFBLKTYPE | LAM_RQFMAND);

	err = _mpi_req_start(req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIRECV, err));

	_mpi_req_add_m(req);
	_mpi_req_blkclr_m();
	_mpi_req_blkset_m(req);

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIRECV, err));
	
	_mpi_req_rem_m(req);

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIRECV, err));

	staterr = req->rq_status.MPI_ERROR;
	if (stat != MPI_STATUS_IGNORE) {
		*stat = req->rq_status;
	}
/*
 * Generate a run time trace.
 */
	if (fl_trace) {
		finisht = MPI_Wtime();
		
		lam_tr_msg(TRTINPUT, startt,
			LAM_S2US(finisht - startt - _kio.ki_blktime),
			LAM_S2US(_kio.ki_blktime), src, tag, comm,
			dtype, count, req->rq_status.MPI_SOURCE,
			req->rq_status.MPI_TAG, req->rq_seq, LAM_RQIRECV);
	}

	err = _mpi_req_destroy(&req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIRECV, err));
	}

	if (staterr != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIRECV, staterr));
	}

	lam_resetfunc_m(BLKMPIRECV);
	return(MPI_SUCCESS);
}
