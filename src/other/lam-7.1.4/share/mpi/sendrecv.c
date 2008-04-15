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
 * $Id: sendrecv.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- send and receive
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- destination
 *			- send tag
 *			- receive buffer
 *			- receive count
 *			- receive datatype
 *			- source
 *			- receive tag
 *			- communicator
 *			- status (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Sendrecv - Sends and receives a message

Input Parameters:
+ sbuf - initial address of send buffer (choice) 
. scount - number of elements in send buffer (integer) 
. sdtype - type of elements in send buffer (handle) 
. dest - rank of destination (integer) 
. stag - send tag (integer) 
. rcount - number of elements in receive buffer (integer) 
. rdtype - type of elements in receive buffer (handle) 
. src - rank of source (integer) 
. rtag - receive tag (integer) 
- comm - communicator (handle) 

Output Parameters:
+ rbuf - initial address of receive buffer (choice) 
- status - status object (Status).  This refers to the receive
operation.  Can also be the MPI constant 'MPI_STATUS_IGNORE', if the
return status is not desired.

Notes:

To dispell a common misconception: 'src' and 'dest' do not have to be
the same.  Additionally, a common mistake when using this function is
to mismatch the tags with the source and destination ranks, which can
result in deadlock.

This function is guaranteed not to deadlock in situations where pairs
of blocking sends and receives may deadlock.  For example, the
following code may deadlock if all ranks in 'MPI_COMM_WORLD' execute
it simultaneously

.vb
    int rank, size, to, from;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  to = (rank + 1) % size;
  from = (rank + size - 1) % size;

  MPI_Send(send_buffer, ..., to, tag, MPI_COMM_WORLD);
  MPI_Recv(recv_buffer, ..., from, tag, MPI_COMM_WORLD);
.ve

If even one rank''s 'MPI_Send' blocks and never completes, the entire
operation may deadlock.  One alternative is to use 'MPI_Sendrecv' in
this situation because it is guaranteed not to deadlock.
  
.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.seealso MPI_Sendrecv_replace

.N ACK
@*/
int MPI_Sendrecv(void *sbuf, int scount, MPI_Datatype sdtype, 
		 int dest, int stag, void *rbuf, int rcount, 
		 MPI_Datatype rdtype, int src, int rtag, 
		 MPI_Comm comm, MPI_Status *status)
{
	MPI_Request	req;			/* request */
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */

	lam_initerr_m();
	lam_setfunc_m(BLKMPISENDRECV);
/*
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPISENDRECV);
	}
/*
 * Send & receive.  The arguments will be checked in the send/recv calls.
 */
	err = MPI_Irecv(rbuf, rcount, rdtype, src, rtag, comm, &req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPISENDRECV, err));
	}

	err = MPI_Send(sbuf, scount, sdtype, dest, stag, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPISENDRECV, err));
	}

	err = MPI_Wait(&req, status);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPISENDRECV, err));
	}
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) lam_tr_wrapend(BLKMPISENDRECV);

	lam_resetfunc_m(BLKMPISENDRECV);
	return(MPI_SUCCESS);
}
