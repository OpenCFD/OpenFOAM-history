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
 * $Id: wait.c,v 6.20 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- wait for completion of immediate request
 *	Accepts:	- request
 *			- status
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#include <lamdebug.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif

/*
 * local variables
 */
#if LAM_WANT_IMPI
static int fl_debug = 0;
#endif


/*
 
The new plan (27 Sep 1999):

- In request chains, there are mandatory and non-mandatory requests.
- All top-level requests should be marked as RQFMAND; need to
  investigate this (I think setting it in lamsend.c should be
  sufficient) -- this would make the logic below easier.  Just need to
  ensure that non-mandatory requests don't inadvenrtantly get RQFMAND
  set (perhaps they should explicitly unset it, like we explicitly set
  RQFSHADOW right now?) --- DONE
- In MPI_Wait, we should check each one of them.
- If a request goes into a done state and has RQFACK set, do an 
  MPI_Send of the ACK.
- When all the mandatatory requests are in the done state, the overall
  request is done
- When the overall request is done, go through each request:
  - if it is persistent, restart it
  - if it is not persistent, whack it
- We will never have persistent shadows with a non-persistent main; that
  should make logic a bit easier

*/

/*@
    MPI_Wait  - Waits for an MPI send or receive to complete

Input Parameter:
. preq - request (handle) 

Output Parameter:
. stat - status object (Status), which may be the MPI constant
'MPI_STATUS_IGNORE'

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Wait(MPI_Request *preq, MPI_Status *stat)
{
	MPI_Request	req;			/* request */
	MPI_Comm	comm;			/* communicator */
	int		errstat = MPI_SUCCESS;	/* error in status */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */
#if LAM_WANT_IMPI
	MPI_Request     prev, cur, next;	/* requests */
	int		flag;			/* flag for test */
	int		num_mandatory;		/* how many must finish */
#endif

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWAIT);

	if (preq == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAIT,
				lam_mkerr(MPI_ERR_REQUEST, EINVAL)));
	}
/*
 * Inactive requests get an empty status.
 */
	req = *preq;

	if ((req == MPI_REQUEST_NULL) || (req->rq_state == LAM_RQSINIT)) {
		if (stat != MPI_STATUS_IGNORE) {
			lam_emptystat(stat);
		}
		lam_resetfunc_m(BLKMPIWAIT);
		return(MPI_SUCCESS);
	}

	if ((fl_trace = LAM_TRACE_TOP())) {
		startt = MPI_Wtime();
		_kio.ki_blktime = 0.0;
	}
	comm = req->rq_comm;

#if LAM_WANT_IMPI
/*
 * Set to block on all the shadows that have LAM_RQFMAND set; this
 * entire request cannot finish until all of them finish (including
 * the main request).  If any of the other shadows finish, that's what
 * we call an "extra bonus". :-)
 */
	_mpi_req_blkclr_m();
	flag = 0;
	num_mandatory = 0;
	for (cur = req; cur != MPI_REQUEST_NULL; cur = cur->rq_shadow) {
/*
 * Only count mandatory requests that are not yet done.  Mark already
 * done ones so that we know which ones we have already seen.
 */
	  if (cur->rq_state == LAM_RQSDONE) {
	    cur->rq_flags |= LAM_RQFMARK;
	    _mpi_req_rem_m(cur);
/*
 * If an ACK was sent, we have to be able to block on it as well
 */
	    if ((cur->rq_flags & LAM_RQFACKDONE) != 0 && cur->rq_extra != 0) {
	      _mpi_req_blkset_m((MPI_Request) cur->rq_extra);
	      flag = 1;
	    }
	  } else if ((cur->rq_marks & LAM_RQFMAND) != 0) {
	    _mpi_req_blkset_m(cur);
	    num_mandatory++;
	  } 
	}
/*
 * If we don't have any mandatory requests left, it still may be
 * possible that a synchronous send ack has not been sent yet (i.e.,
 * this is a receive request that matched a synchronous send from the
 * unexpected receive queue, and we never gave the sync ack shadow
 * request a chance to go).  So do an _mpi_req_advance() to send it
 * out. 
 */
	if (num_mandatory == 0 && flag) {
	  DBUG("SPECIAL CASE WAITING FOR ACK TO GO OUT\n");
	  err = _mpi_req_advance();
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIWAIT, err));
	}
/*
 * Advance the system until all mandatory requests are done.  We may
 * pick up some of the non-mandatory requests in the meantime.  We'll
 * call this an "extra bonus".
 */
	DBUG("WAITING ON MANDATORY REQUESTS: %d\n", num_mandatory);
	while (num_mandatory > 0) {
	  err = _mpi_req_advance();
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIWAIT, err));
	  DBUG("MPI_REQ_ADVANCE RETURNED IN MANDATORY LOOP\n");
/*
 * Find the request that just finished
 */
	  for (cur = (*preq); cur != MPI_REQUEST_NULL; cur = cur->rq_shadow) {
	    if (cur->rq_state == LAM_RQSDONE && 
		(cur->rq_flags & LAM_RQFMARK) == 0) {
	      cur->rq_flags |= LAM_RQFMARK;
	      _mpi_req_rem_m(cur);
/*
 * If this was a mandatory request, decrease the count
 */
	      if ((cur->rq_marks & LAM_RQFMAND) != 0) {
		DBUG("MPI_REQ_ADVANCE IN MANDATORY LOOP: found mand req done: 0x%x\n", (int) cur);
		num_mandatory--;
	      }
	    }
	  }
	  DBUG("MANDATORY REQUEST LOOP: looping %d\n", num_mandatory);
	}
	DBUG("MANDATORY REQUESTS FINISHED\n");
/*
 * Generate a run time trace except in the case of a cancelled request or
 * a request where the peer is MPI_PROC_NULL.  
 */
	if (fl_trace && (req->rq_rank != MPI_PROC_NULL) &&
	    !(req->rq_flags & LAM_RQFCANCEL)) {
	  finisht = MPI_Wtime();
	  
	  lam_tr_msg((req->rq_type == LAM_RQIRECV) ? TRTINPUT : TRTNOIO,
		     startt, LAM_S2US(finisht - startt - _kio.ki_blktime),
		     LAM_S2US(_kio.ki_blktime), req->rq_rank, req->rq_tag,
		     req->rq_comm, req->rq_dtype, req->rq_count,
		     req->rq_status.MPI_SOURCE, req->rq_status.MPI_TAG,
		     req->rq_seq, req->rq_type);
	}
/*
 * Now that all the mandatory requests are done, traverse the list
 * again:
 *
 * 1. --ACKs have already been sent from within RPI (they're attached
 * as handlers to the ping receive requests)
 * 2. Remove all requests from active list
 * 3. Save status info for main request 
 * 4. restart any persistent requests that have completed,
 * 5. leave persistent requests that have not completed alone
 * 6. destroy any non-persistent requests (completed or not), 
 *
 * Simplifying the logic a little -- we will never have a
 * non-persistent top-level request (i.e., (*preq)) with persistent
 * shadow requests.  So if the top-level request gets whacked, they
 * will all get whacked.
 */
	for (prev = MPI_REQUEST_NULL, cur = req;
	     cur != MPI_REQUEST_NULL; cur = next) {
	  next = cur->rq_shadow;
/*
 * If this request finished, it has already been taken out of the
 * active list, so end it and save the status
 */
	  if (cur->rq_state == LAM_RQSDONE) {
	    err = _mpi_req_end(cur);
	    if (err != MPI_SUCCESS) 
	      return(lam_errfunc(comm, BLKMPIWAIT, err));
/*
 * Save the status information if this was the top-level request
 */
	    if (cur == (*preq)) {
	      errstat = req->rq_status.MPI_ERROR;
	      if (stat != MPI_STATUS_IGNORE) {
		*stat = req->rq_status;
	      }
	    }
/*
 * If persistent and completed, restart it
 */
	    if (cur->rq_marks & LAM_RQFPERSIST) {
	      req->rq_state = LAM_RQSINIT;
	      prev = cur;
	    }
/* 
 * Oherwise, it's not persistent, so whack it
 */
	    else {
	      if (cur == (*preq))
		err = _mpi_req_destroy(preq);
	      else
		err = _mpi_req_destroy(&cur);
	      if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWAIT, err));
	      }
	    }
	  }
/*
 * If this request has not finished, take it out of the active list,
 * and handle its termination.
 */
	  else {
	    _mpi_req_rem(cur);
/*
 * This request has not completed, so kill it.  Cancel it first since
 * it hasn't completed.  Keep the destroying here vs lam_test() (so
 * that lam_test doesn't potentially whack any shadows of this
 * request).  But destroy it only if it isn't persistent.
 */
	    MPI_Cancel(&cur);
	    lam_test(&cur, 0, &flag, MPI_STATUS_IGNORE);
/*
 * If we're destroying the top-level request, be sure to destroy it so
 * that it gets set to MPI_REQUEST_NULL
 */
	    if ((cur->rq_marks & LAM_RQFPERSIST) == 0) {
	      if (cur == (*preq))
		err = _mpi_req_destroy(preq);
	      else
		err = _mpi_req_destroy(&cur);
	      if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWAIT, err));
	      }
	    } else {
	      req->rq_state = LAM_RQSINIT;
	      prev = cur;
	    }
/*
 * If this request wasn't destroyed, save it in the new chain
 */
	    if (prev != MPI_REQUEST_NULL)
	      prev->rq_shadow = next;
	  } 
	}
#else
/*
 * Block till main request is done
 */
	if (req->rq_state != LAM_RQSDONE) {
	    _mpi_req_blkclr_m();
	    _mpi_req_blkset_m(req);
	    
	    err = _mpi_req_advance();
	    if (err != MPI_SUCCESS) {
	      return(lam_errfunc(comm, BLKMPIWAIT, err));
	    }
	}

	_mpi_req_rem_m(req);

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIWAIT, err));

	errstat = req->rq_status.MPI_ERROR;
	if (stat != MPI_STATUS_IGNORE) {
		*stat = req->rq_status;
	}
/*
 * Generate a run time trace except in the case of a cancelled request or
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
/*
 * Reset persistent requests.
 * Destroy requests that are not persistent.
 * Must go through all the shadows as well.
 */
	if ((req->rq_marks & LAM_RQFPERSIST) != 0) {
		req->rq_state = LAM_RQSINIT;
	} else {
		err = _mpi_req_destroy(preq);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPIWAIT, err));
		}
	}
#endif

	if (errstat != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWAIT, errstat));
	}

	lam_resetfunc_m(BLKMPIWAIT);
	return(MPI_SUCCESS);
}
