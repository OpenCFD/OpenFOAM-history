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
 * $Id: bsend.c,v 6.8 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- buffered send
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- tag
 *			- communicator
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>

/*
 * external functions
 */
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();
extern void		lam_tr_msg();
extern int		lam_errfunc();
extern int		lam_isend();
extern int		lam_tr_incff();

/*@
    MPI_Bsend - Basic send with user-specified buffering

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (nonnegative integer) 
. dtype - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Notes:

This send is provided as a convenience function; it allows the user to
send messages without worring about where they are buffered (because
the user `must` have provided buffer space with 'MPI_Buffer_attach').

In deciding how much buffer space to allocate, remember that the
buffer space is not available for reuse by subsequent 'MPI_Bsend's
unless you are certain that the message has been received (not just
that it should have been received).  For example, this code does not
allocate enough buffer space

.vb
    MPI_Buffer_attach(b, n*sizeof(double) + MPI_BSEND_OVERHEAD);
    for (i = 0; i < m; i++) {
        MPI_Bsend(buf, n, MPI_DOUBLE, ...);
    }
.ve
because only enough buffer space is provided for a single send, and
the loop may start a second 'MPI_Bsend' before the first is done
making use of the buffer.

In C, you can force the messages to be delivered by
.vb
    MPI_Buffer_detach(&b, &n);
    MPI_Buffer_attach(b, n);
.ve
(The 'MPI_Buffer_detach' will not complete until all buffered messages
are delivered.)

It is generally a bad idea to use the 'MPI_Bsend' function, as it
guarantees that the entire message will suffer the overhead of an
additional memory copy.  For large messages, or when shared memory
message transports are being used, this overhead can be quite
expensive. 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_RANK
.N MPI_ERR_TAG

.seealso: MPI_Buffer_attach, MPI_Buffer_detach, MPI_Ibsend, MPI_Bsend_init

.N ACK
@*/
int MPI_Bsend(void *buf, int count, MPI_Datatype dtype, 
	      int dest, int tag, MPI_Comm comm)
{
	int		err;			/* error code */
	MPI_Request	req;			/* request */
	int		fl_trace;		/* do tracing? */
	double		startt = 0.0;		/* start time */
	double		finisht;		/* finish time */

	lam_initerr_m();
/*
 * Handle special case of peer MPI_PROC_NULL.
 */
        if (dest == MPI_PROC_NULL) return(MPI_SUCCESS);

	lam_setfunc_m(BLKMPIBSEND);
/*
 * Check the tag.
 */
	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIBSEND,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}
	
	if ((fl_trace = LAM_TRACE_TOP())) startt = MPI_Wtime();
/*
 * Request will be orphaned, no need to bother with it here.
 */
	req = MPI_REQUEST_NULL;
	err = lam_isend(buf, count, dtype,
			dest, tag, comm, &req, LAM_RQIBSEND);

	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIBSEND, err));
/*
 * Generate a run time trace.
 */
	if (fl_trace && dest != MPI_PROC_NULL) {
		finisht = MPI_Wtime();
		lam_tr_msg(TRTOUTPUT, startt, LAM_S2US(finisht - startt), 0,
				dest, tag, comm, dtype, count, 0, 0, 
				req->rq_seq, LAM_RQIBSEND);
	}

	lam_resetfunc_m(BLKMPIBSEND);
	return(MPI_SUCCESS);
}
