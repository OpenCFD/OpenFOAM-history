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
 * $Id: ibsend.c,v 6.8 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- immediate buffered send
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- tag
 *			- communicator
 *			- request (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*
 * external functions
 */
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();
extern int		lam_errfunc();
extern int		_mpi_req_build();

/*@
    MPI_Ibsend - Starts a nonblocking buffered send

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (integer) 
. dtype - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Output Parameter:
. preq - communication request (handle) 

Notes:

It is generally a bad idea to use the 'MPI_Ibsend' function, as it
guarantees that the entire message will suffer the overhead of an
additional memory copy.  For large messages, or when shared memory
message transports are being used, this overhead can be quite
expensive.

The communication started with this function is not guaranteed to
progress or complete until 'req' has been given to one of the test or
wait functions ('MPI_Test', 'MPI_Testall', 'MPI_Testany',
'MPI_Testsome', 'MPI_Wait', 'MPI_Waitall', 'MPI_Waitany',
'MPI_Waitsome').

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK
.N MPI_ERR_BUFFER

.seealso MPI_Bsend, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Ibsend(void *buf, int count, MPI_Datatype dtype, 
	       int dest, int tag, MPI_Comm comm, 
	       MPI_Request *preq)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIIBSEND);
/*
 * Check the tag.
 */
	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIIBSEND,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}
/*
 * Create the fake user request.  We do not add it into the
 * request list.  This is an exception to the design that says
 * that all active (inc. done) requests are to be found in the
 * request list but fake requests, really ibsend requests are
 * very exceptional MPI creatures.
 */
	*preq = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, dest, tag, comm,
			LAM_RQIFAKE, preq);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIBSEND, err));
	}

	(*preq)->rq_status.MPI_ERROR = MPI_SUCCESS;
	(*preq)->rq_status.MPI_TAG = MPI_UNDEFINED;
	(*preq)->rq_status.MPI_SOURCE = MPI_PROC_NULL;
	(*preq)->rq_state = LAM_RQSDONE;
/*
 * Bsend() the message.
 */
	err = MPI_Bsend(buf, count, dtype, dest, tag, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIIBSEND, err));
	}

	lam_resetfunc_m(BLKMPIIBSEND);
	return(MPI_SUCCESS);
}
