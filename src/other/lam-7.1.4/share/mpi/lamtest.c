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
 * $Id: lamtest.c,v 6.16 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- test if request is done
 *			- destructive and non-destructive modes
 *	Accepts:	- request
 *			- destructive mode flag
 *			- flag (out)
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#include <terror.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif


/*
 
The new plan (27 Sep 1999):

- In request chains, there are mandatory and non-mandatory requests.
- All top-level requests should be marked as RQFMAND; need to
  investigate this (I think setting it in lamsend.c should be
  sufficient) -- this would make the logic below easier.  Just need to
  ensure that non-mandatory requests don't inadvenrtantly get RQFMAND
  set (perhaps they should explicitly unset it, like we explicitly set
  RQFSHADOW right now?)
- When we call lam_test, we should check each one of them
- If a request goes into a done state and has RQFACK set, do an 
  MPI_Isend of the ACK.  Add a new request to the end of the chain, and 
  set RQF_MAND on it.
- If all the mandatatory requests (including any new MPI_Isends) are
  in the done state, the overall request is done
- If the overall request is not done, return *flag = 0/MPI_SUCCESS
- Otherwise, go through each request:
  - if it is persistent, restart it
  - if it is not persistent, whack it
- We will never have persistent shadows with a non-persistent main; that
  should make logic a bit easier
- Then return *flag = 1/MPI_SUCCESS

*/


/*
 * global functions
 */
int  lam_test_rqfmand(MPI_Request *preq);


int
lam_test(MPI_Request *preq, int destroy, int *flag, MPI_Status *stat)
{
	MPI_Request	req;			/* request */
	int		errstat;		/* error in status */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */
#if LAM_WANT_IMPI
	int		done;			/* if all done */
	int		bogus_flag;		/* flag for lam_test */
	MPI_Request	prev, cur, next;	/* request */
#endif

	if (preq == 0) return(lam_mkerr(MPI_ERR_REQUEST, EINVAL));

	if (flag == 0) return(lam_mkerr(MPI_ERR_ARG, EINVAL));

	if ((fl_trace = LAM_TRACE_TOP())) {
	    startt = MPI_Wtime();
	    _kio.ki_blktime = 0.0;
	}
/*
 * Advance the system.  
 */
	_mpi_req_blkclr_m();

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(err);

	req = *preq;
/*
 * Inactive requests get an empty status.
 */
	if ((req == MPI_REQUEST_NULL) || (req->rq_state == LAM_RQSINIT)) {
	    *flag = 1;
	    if (stat != MPI_STATUS_IGNORE) {
		    lam_emptystat(stat);
	    }
	}
/*
 * If the top-level request is not done, the entire request cannot be
 * done.  So reset the flag.
 */
	else if (req->rq_state != LAM_RQSDONE) {
	    *flag = 0;
	}
#if LAM_WANT_IMPI
/*
 * Need to check if all mandatory requests are done.  See if we need
 * to send out an ACK -- if we do, we have to chain it to the bottom
 * of the request list, and mark it not done.  Be sure to check all
 * requests, regardless if we find a mandatory one that hasn't
 * finished or not.
 */
	else {
	  done = 1;
	  for (cur = req; cur != MPI_REQUEST_NULL; cur = cur->rq_shadow) {
/*
 * If we find a newly finished request, mark it
 */
	    if (cur->rq_state == LAM_RQSDONE)
	      cur->rq_flags |= LAM_RQFMARK;
	    else {
	      if ((cur->rq_marks & LAM_RQFMAND) != 0) {
		done = 0;
		break;
	      }
	    }
	  }
/*
 * Did all the mandatory requests finish?
 */
	  if (done == 0) {
	    *flag = 0;
	    return (MPI_SUCCESS);
	  }
	  *flag = 1;
	  
	  if (destroy) {
/*
 * Fill the status
 */
	    errstat = req->rq_status.MPI_ERROR;
	    if (stat != MPI_STATUS_IGNORE) {
	      *stat = req->rq_status;
	    }
/*
 * Generate a run time trace except in the case of cancelled request or
 * a request where the peer is MPI_PROC_NULL.
 */
	    if (fl_trace && (req->rq_rank != MPI_PROC_NULL) &&
		!(req->rq_flags & LAM_RQFCANCEL)) {
	      finisht = MPI_Wtime();
	      
	      lam_tr_msg( (req->rq_type == LAM_RQIRECV)
			  ? TRTINPUT : TRTNOIO, startt,
			  LAM_S2US(finisht - startt - _kio.ki_blktime),
			  LAM_S2US(_kio.ki_blktime), req->rq_rank,
			  req->rq_tag, req->rq_comm, req->rq_dtype,
			  req->rq_count, req->rq_status.MPI_SOURCE,
			  req->rq_status.MPI_TAG, req->rq_seq, req->rq_type);
	    }
/*
 * Now that all the mandatory requests are done, traverse the list
 * again:
 *
 * 1. restart any persistent requests that have completed,
 * 2. leave persistent requests that have not completed alone
 * 3. destroy any non-persistent requests (completed or not), 
 *
 * Simplifying the logic a little -- we will never have a
 * non-persistent top-level request (i.e., (*preq)) with persistent
 * shadow requests.  So if the top-level request gets whacked, they
 * will all get whacked.
 */
	    for (prev = MPI_REQUEST_NULL, cur = req;
		 cur != MPI_REQUEST_NULL; cur = next) {
	      next = cur->rq_shadow;
	      
	      _mpi_req_rem_m(cur);
	      
	      err = _mpi_req_end(cur);
	      if (err != MPI_SUCCESS) return(err);
/*
 * If persistent and completed, restart it
 */
	      if (cur->rq_marks & LAM_RQFPERSIST) {
		if (cur->rq_state == LAM_RQSDONE)
		  req->rq_state = LAM_RQSINIT;
		prev = cur;
	      } 
/*
 * Otherwise, it's not persistent, so whack it.  May need to cancel it
 * first (if it hasn't completed).  Keep the destroying here vs
 * lam_test() (so that lam_test doesn't potentially whack any shadows
 * of this request).
 */
	      else {
		if (cur->rq_state != LAM_RQSDONE) {
		  MPI_Cancel(&cur);
		  lam_test(&cur, 0, &bogus_flag, MPI_STATUS_IGNORE);
		}
/*
 * If we're destroying the top-level request, be sure to destroy it so
 * that it gets set to MPI_REQUEST_NULL
 */
		if (cur == *preq)
		  err = _mpi_req_destroy(preq);
		else
		  err = _mpi_req_destroy(&cur);
		if (err != MPI_SUCCESS)
		  return err;
		if (prev != MPI_REQUEST_NULL)
		  prev->rq_shadow = next;
	      }
	    }

	    if (errstat != MPI_SUCCESS)
	      return errstat;
	  }
	}
#else
/*
 * Finish the request if in destructive-mode.
 */
	else {
	    *flag = 1;

	    if (destroy) {
		_mpi_req_rem_m(req);

		err = _mpi_req_end(req);
		if (err != MPI_SUCCESS) return(err);

		errstat = req->rq_status.MPI_ERROR;
		if (stat != MPI_STATUS_IGNORE) {
			*stat = req->rq_status;
		}
/*
 * Generate a run time trace except in the case of cancelled request or
 * a request where the peer is MPI_PROC_NULL.
 */
		if (fl_trace && (req->rq_rank != MPI_PROC_NULL)
				&& !(req->rq_flags & LAM_RQFCANCEL)) {
		    finisht = MPI_Wtime();

		    lam_tr_msg( (req->rq_type == LAM_RQIRECV)
			    ? TRTINPUT : TRTNOIO, startt,
			    LAM_S2US(finisht - startt - _kio.ki_blktime),
			    LAM_S2US(_kio.ki_blktime), req->rq_rank,
			    req->rq_tag, req->rq_comm, req->rq_dtype,
			    req->rq_count, req->rq_status.MPI_SOURCE,
			    req->rq_status.MPI_TAG, req->rq_seq, req->rq_type);
		}
/*
 * Reset persistent requests.
 * Destroy requests that are not persistent.
 */
		if (req->rq_marks & LAM_RQFPERSIST) {
		  req->rq_state = LAM_RQSINIT;
		} else {
		  err = _mpi_req_destroy(preq);
		  if (err != MPI_SUCCESS) return(err);
		}
		if (errstat != MPI_SUCCESS) {
		    return(errstat);
		}
	    }
	}
#endif

	return(MPI_SUCCESS);
}


/*
 * Test to see if any shadow request in the chain has LAM_RFQMAND set
 */
int 
lam_test_rqfmand(MPI_Request *preq)
{
  MPI_Request p;

  for (p = (*preq); p != NULL; p = p->rq_shadow)
    if (((*preq)->rq_marks & LAM_RQFMAND))
      return 1;

  return 0;
}

