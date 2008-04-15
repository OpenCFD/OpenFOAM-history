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
 * $Id: waitall.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- wait for all requests to finish
 *	Accepts:	- # of requests
 *			- array of requests
 *			- array of status structures (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
    MPI_Waitall - Waits for all given communications to complete

Input Parameters:
+ count - lists length (integer) 
- reqs - array of requests (array of handles) 

Output Parameter:
. stats - array of status objects (array of Status), which may be the
MPI constant 'MPI_STATUSES_IGNORE'

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG
.N MPI_ERR_COUNT
.N MPI_ERR_IN_STATUS
.N MPI_ERR_PENDING

.N ACK
@*/
int MPI_Waitall(int count, MPI_Request *reqs, 
		MPI_Status *stats)
{
	int		i;			/* favourite index */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	int		first;			/* first request ready */
	MPI_Status	status;
	MPI_Status	*firststat;		/* status of first request */
	MPI_Status	*p;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWAITALL);

	if (count < 0) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITALL,
		    lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && (reqs == 0)) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITALL,
		    lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
	    lam_tr_wrapstart(BLKMPIWAITALL);
	}
/*
 * Initialize all status members.
 */
	if (stats != MPI_STATUSES_IGNORE) {
	    for (i = 0, p = stats; i < count; ++i, ++p) {
		lam_emptystat(p);
	    }
	    firststat = &status;
	} else {
	    firststat = MPI_STATUS_IGNORE;
	}
/*
 * Loop waiting for all requests.
 */
	for (i = 0; i < count; ++i) {
	    err = MPI_Waitany(count, reqs, &first, firststat);

	    if (err != MPI_SUCCESS) {

		if (first == MPI_UNDEFINED) {
		    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITALL, err));
		}

		else {
		    if (stats != MPI_STATUSES_IGNORE) {
			stats[first].MPI_ERROR = err;
		    }

		    if (fl_trace) {
			lam_tr_wrapend(BLKMPIWAITALL);
		    }

		    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITALL,
			    lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
		}
	    }

	    if (first == MPI_UNDEFINED) {

		if (fl_trace) {
		    lam_tr_wrapend(BLKMPIWAITALL);
		}

		lam_resetfunc_m(BLKMPIWAITALL);
		return(MPI_SUCCESS);
	    }

	    if (stats != MPI_STATUSES_IGNORE) {
		stats[first] = *firststat;
		stats[first].MPI_ERROR = MPI_SUCCESS;
	    }
	}
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
	    lam_tr_wrapend(BLKMPIWAITALL);
	}

	lam_resetfunc_m(BLKMPIWAITALL);
	return(MPI_SUCCESS);
}
