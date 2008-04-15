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
 * $Id: testany.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- check if one request completed
 *	Accepts:	- # of requests
 *			- array of requests
 *			- index (out)
 *			- flag (out)
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>


/*@
    MPI_Testany - Tests for completion of any previdously initiated 
                  communication

Input Parameters:
+ count - list length (integer) 
- reqs - array of requests (array of handles) 

Output Parameters:
+ index - index of operation that completed, or 'MPI_UNDEFINED'  if none 
  completed (integer) 
. flag - true if one of the operations is complete (logical) 
- stat - status object (Status), which may be the MPI constant
'MPI_STATUS_IGNORE'

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COUNT
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Testany(int count, MPI_Request *reqs, 
		int *index, int *flag, MPI_Status *stat)
{
	int		i;			/* favourite index */
	int		fl_done;		/* request done flag */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	MPI_Comm	comm = 0;		/* communicator */
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPITESTANY);

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTANY,
				lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((count > 0) && (reqs == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTANY,
				lam_mkerr(MPI_ERR_REQUEST, EINVAL)));
	}

	if ((flag == 0) || (index == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITESTANY,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPITESTANY);
	}
/*
 * Loop testing all requests.
 * Skip inactive requests.
 */
	if (stat != MPI_STATUS_IGNORE) {
		lam_emptystat(stat);
	}

	*flag = 1;
	*index = MPI_UNDEFINED;
	err = MPI_SUCCESS;

	for (i = 0; i < count; ++i, ++reqs) {

		if ((*reqs == MPI_REQUEST_NULL) ||
			((*reqs)->rq_state == LAM_RQSINIT)) continue;

		*flag = 0;
		comm = (*reqs)->rq_comm;

		err = MPI_Test(reqs, &fl_done, stat);
		if (err != MPI_SUCCESS) break;

		if (fl_done) {
			*flag = 1;
			*index = i;
			break;
		}
	}
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
		lam_tr_wrapend(BLKMPITESTANY);
	}

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPITESTANY, err));
	}

	lam_resetfunc_m(BLKMPITESTANY);
	return(MPI_SUCCESS);
}
