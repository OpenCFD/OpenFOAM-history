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
 * $Id: testall.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- check if all requests completed
 *	Accepts:	- # of requests
 *			- array of requests
 *			- flag (out)
 *			- array of status structures (modified)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
    MPI_Testall - Tests for the completion of all previously initiated
    communications

Input Parameters:
+ count - lists length (integer) 
- reqs - array of requests (array of handles) 

Output Parameters:
+ flag - (logical) 
- stats - array of status objects (array of Status), which can be the
MPI constant 'MPI_STATUSES_IGNORE'

Notes:

'flag' is true (1) only if all requests have completed.  Otherwise,
'flag' is false (0) and neither 'reqs' nor 'stats' is modified.

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_IN_STATUS

.N ACK
@*/
int MPI_Testall(int count, MPI_Request *reqs, 
		int *flag, MPI_Status *stats)
{
	int		i;			/* favourite index */
	int		fl_done;		/* request done? */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	MPI_Request	*req;			/* ptr request */
	MPI_Status	*stat;			/* ptr status */

	lam_initerr_m();
	lam_setfunc_m(BLKMPITESTALL);

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTALL,
				lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && (reqs == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTALL,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (flag == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTALL,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPITESTALL);
	}
/*
 * Initialize all status members.
 */
	if (stats != MPI_STATUSES_IGNORE) {
		for (i = 0, stat = stats; i < count; ++i, ++stat) {
			lam_emptystat(stat);
		}
	}
/*
 * Loop testing all requests non-destructively.
 * Stop at the first request not done.
 */
	req = reqs;
	stat = stats;

	for (i = 0; i < count; ++i, ++req) {

		err = lam_test(req, 0, &fl_done, stat);

		if (err != MPI_SUCCESS) {
			stat->MPI_ERROR = err;

			if (fl_trace) {
				lam_tr_wrapend(BLKMPITESTALL);
			}

			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTALL,
					lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
		}

		if (fl_done == 0) {
			*flag = 0;

			if (fl_trace) {
				lam_tr_wrapend(BLKMPITESTALL);
			}

			lam_resetfunc_m(BLKMPITESTALL);
			return(MPI_SUCCESS);
		}

		if (stats != MPI_STATUSES_IGNORE) {
			stat++;
		}
	}
/*
 * All active requests are done, loop doing the job.
 */
	req = reqs;
	stat = stats;

	for (i = 0; i < count; ++i, ++req) {
		err = lam_test(req, 1, &fl_done, stat);

		if (err != MPI_SUCCESS) {
			stat->MPI_ERROR = err;

			if (fl_trace) {
				lam_tr_wrapend(BLKMPITESTALL);
			}

			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTALL,
					lam_mkerr(MPI_ERR_IN_STATUS, EINVAL)));
		}

		if (stats != MPI_STATUSES_IGNORE) {
			stat++;
		}
	}

	*flag = 1;
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
		lam_tr_wrapend(BLKMPITESTALL);
	}

	lam_resetfunc_m(BLKMPITESTALL);
	return(MPI_SUCCESS);
}
