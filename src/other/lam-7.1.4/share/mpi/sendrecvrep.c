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
 * $Id: sendrecvrep.c,v 6.7 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- send and receive using same buffer
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- send tag
 *			- source
 *			- receive tag
 *			- communicator
 *			- status (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>


/*@
    MPI_Sendrecv_replace - Sends and receives using a single buffer

Input Parameters:
+ count - number of elements in send and receive buffer (integer) 
. dtype - type of elements in send and receive buffer (handle) 
. dest - rank of destination (integer) 
. stag - send message tag (integer) 
. src - rank of source (integer) 
. rtag - receive message tag (integer) 
- comm - communicator (handle) 

Output Parameters:
+ buf - initial address of send and receive buffer (choice) 
- status - status object (Status) 

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
.N MPI_ERR_TRUNCATE
.N MPI_ERR_EXHAUSTED

.seealso MPI_Sendrecv

.N ACK
@*/
int MPI_Sendrecv_replace(void *buf, int count, 
			 MPI_Datatype dtype, int dest, int stag, 
			 int src, int rtag, MPI_Comm comm, 
			 MPI_Status *status)
{
	int		err;			/* error code */
	int		fl_trace;		/* do tracing? */
	int		pkpos;			/* packing position */
	int		pksize;			/* packing size */
	char		*pkbuf;			/* packing buffer */

	lam_initerr_m();
	lam_setfunc_m(BLKMPISENDRECVREP);
/*
 * Most of the arguments will be checked in the sendrecv call.
 * Generate a start wrapper trace.
 */
	fl_trace = ((_kio.ki_rtf & RTF_TRON) == RTF_TRON);

	if (fl_trace) {
		lam_tr_wrapstart(BLKMPISENDRECVREP);
	}
/*
 * Pack the message to send.
 */
	err = MPI_Pack_size(count, dtype, comm, &pksize);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPISENDRECVREP, err));
	}

	if (pksize > 0) {
		pkbuf = malloc((unsigned) pksize);
		if (pkbuf == 0) {
			return(lam_errfunc(comm, BLKMPISENDRECVREP,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
	} else {
		pkbuf = 0;
	}

	pkpos = 0;

	err = MPI_Pack(buf, count, dtype, pkbuf, pksize, &pkpos, comm);
	if (err != MPI_SUCCESS) {
		if (pkbuf) free(pkbuf);
		return(lam_errfunc(comm, BLKMPISENDRECVREP, err));
	}
/*
 * Send & receive.
 */
	err = MPI_Sendrecv(pkbuf, pksize, MPI_PACKED, dest, stag,
				buf, count, dtype, src, rtag, comm, status);
	if (err != MPI_SUCCESS) {
		if (pkbuf) free(pkbuf);
		return(lam_errfunc(comm, BLKMPISENDRECVREP, err));
	}

	if (pkbuf) free(pkbuf);
/*
 * Generate an end wrapper trace.
 */
	if (fl_trace) {
		lam_tr_wrapend(BLKMPISENDRECVREP);
	}
	
	lam_resetfunc_m(BLKMPISENDRECVREP);
	return(MPI_SUCCESS);
}
