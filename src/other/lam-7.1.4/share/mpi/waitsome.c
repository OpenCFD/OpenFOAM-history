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
 * $Id: waitsome.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- wait for at least one request to finish
 *	Accepts:	- # of requests
 *			- array of requests
 *			- # completed requests (out)
 *			- array of indices (out)
 *			- array of status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>


/*@
    MPI_Waitsome - Waits for some given communications to complete

Input Parameters:
+ incount - length of array_of_requests (integer) 
- reqs - array of requests (array of handles) 

Output Parameters:
+ ndone - number of completed requests (integer) 
. indices - array of indices of operations that completed (array of
integers)
- stats - array of status objects for operations that completed (array
of Status), or the MPI constant 'MPI_STATUSES_IGNORE'

Notes:

The array of indicies are in the range '0' to 'incount - 1' for C and
in the range '1' to 'incount' for Fortran.

Null requests are ignored; if all requests are null, then the routine
returns with 'outcount' set to 'MPI_UNDEFINED'.

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_IN_STATUS

.N ACK
@*/
int MPI_Waitsome(int count, MPI_Request* reqs, int *ndone, 
		 int *indices, MPI_Status *stats)
{
	int		i;
	int		err;			/* error code */
	int		done;			/* test return flag */
	int		first;			/* first request ready */
	int		fl_err;			/* error ocurred? */
	int		fl_trace;		/* do tracing? */
	MPI_Status	firststat;		/* status of first request */
	MPI_Status	*p;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWAITSOME);

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITSOME,
				lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && ((reqs == 0) || (indices == 0))) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITSOME,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (ndone == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITSOME,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPIWAITSOME);
	}
/*
 * Loop initializing all status members.
 */
	if (stats != MPI_STATUSES_IGNORE) {
		for (i = 0, p = stats; i < count; ++i, ++p) {
			lam_emptystat(p);
		}
	}
/*
 * Wait till the first request is done.
 */
	err = MPI_Waitany(count, reqs, &first, &firststat);

	if (err != MPI_SUCCESS) {

		if (first == MPI_UNDEFINED) {
			return(lam_errfunc(MPI_COMM_WORLD,
					BLKMPIWAITSOME, err));
		} else {
			if (stats != MPI_STATUSES_IGNORE) {
				stats[first].MPI_ERROR = err;
			}

			if (fl_trace) {
				lam_tr_wrapend(BLKMPIWAITSOME);
			}

			return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITSOME,
					lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
		}
	}

	if (first == MPI_UNDEFINED) {
		*ndone = first;

		if (fl_trace) {
			lam_tr_wrapend(BLKMPIWAITSOME);
		}

		lam_resetfunc_m(BLKMPIWAITSOME);
		return(MPI_SUCCESS);
	}
/*
 * Loop testing if any other request is done and skip inactive requests.
 * I am going to be nice and maintain the sequential order
 * in the result arrays, just as if MPI_Testsome() was used.
 */
	*ndone = 0;
	fl_err = 0;

	for (i = 0; i < count; ++i, ++reqs) {

		if (i == first) {
			++(*ndone);
			*indices++ = i;
			if (stats != MPI_STATUSES_IGNORE) {
				*stats = firststat;
				stats->MPI_ERROR = MPI_SUCCESS;
				++stats;
			}
		}
		else if ((*reqs == MPI_REQUEST_NULL) ||
				((*reqs)->rq_state == LAM_RQSINIT)) {
			continue;
		}
		else {
			err = MPI_Test(reqs, &done, stats);

			if ((err != MPI_SUCCESS) || done) {
				++(*ndone);
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
	}
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
		lam_tr_wrapend(BLKMPIWAITSOME);
	}

	if (fl_err) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWAITSOME,
				lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
	}

	lam_resetfunc_m(BLKMPIWAITSOME);
	return(MPI_SUCCESS);
}
