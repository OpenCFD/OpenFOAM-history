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
 * $Id: testsome.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- check if at least one request completed
 *			- return info on all completed requests
 *	Accepts:	- # of requests
 *			- array of requests
 *			- output count (out)
 *			- array of indices (out)
 *			- array of status structures (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>


/*@
    MPI_Testsome - Tests for some given communications to complete

Input Parameters:
+ count - length of array_of_requests (integer) 
- reqs - array of requests (array of handles) 

Output Parameters:
+ outcount - number of completed requests (integer) 
. indices - array of indices of operations that 
completed (array of integers) 
- stats - array of status objects for operations that completed (array
of Status), or the MPI constant 'MPI_STATUSES_IGNORE'

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_IN_STATUS

.N ACK
@*/
int MPI_Testsome(int count, MPI_Request *reqs, 
		 int *outcount, int *indices, MPI_Status *stats)
{
	int		i;			/* favourite index */
	int		fl_done;		/* request done flag */
	int		fl_err;			/* error flag */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	MPI_Status	*p;			/* ptr status */

	lam_initerr_m();
	lam_setfunc_m(BLKMPITESTSOME);
/*
 * Check the arguments.
 */
	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTSOME,
				lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && ((reqs == 0) || (indices == 0))) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTSOME,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (outcount == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTSOME,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPITESTSOME);
	}
/*
 * Initialize all status members.
 */
	if (stats != MPI_STATUSES_IGNORE) {
		for (i = 0, p = stats; i < count; ++i, ++p) {
			lam_emptystat(p);
		}
	}
/*
 * Loop testing all requests.
 * Skip inactive requests.
 */
	*outcount = MPI_UNDEFINED;
	fl_err = 0;

	for (i = 0; i < count; ++i, ++reqs) {

		if ((*reqs == MPI_REQUEST_NULL) ||
			((*reqs)->rq_state == LAM_RQSINIT)) continue;

		if (*outcount == MPI_UNDEFINED) {
			*outcount = 0;
		}

		err = MPI_Test(reqs, &fl_done, stats);

		if ((err != MPI_SUCCESS) || fl_done) {

			++(*outcount);
			*indices++ = i;

			if (err != MPI_SUCCESS) {
				if (stats != MPI_STATUSES_IGNORE) {
					stats->MPI_ERROR = err;
				}

				fl_err = 1;
			}

			if (stats != MPI_STATUSES_IGNORE) {
				++stats;
			}
		}
	}
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
		lam_tr_wrapend(BLKMPITESTSOME);
	}

	if (fl_err) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTSOME,
				lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
	}

	lam_resetfunc_m(BLKMPITESTSOME);
	return(MPI_SUCCESS);
}
