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
 * $Id: ssend.c,v 6.14 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- synchronous send
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- tag
 *			- communicator
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif

#if LAM_WANT_IMPI
/*
 * local variables
 */
static int fl_debug = 0;
#endif


/*@
    MPI_Ssend - Basic synchronous send

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (nonnegative integer) 
. dtype - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Notes:

This function will not complete until the destination rank `starts` to
receive the message.  Note that this is different than saying that the
message has been received by the destination rank.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Ssend(void *buf, int count, MPI_Datatype dtype, 
	      int dest, int tag, MPI_Comm comm)
{
	int		err;
#if LAM_WANT_IMPI 
	MPI_Request     req;
#endif

	lam_initerr_m();
	lam_setfunc_m(BLKMPISSEND);

	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPISSEND,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

#if LAM_WANT_IMPI
/*
 * If this is to the IMPID, we need to wait for a *second* ACK --
 * because the ssend will complete when the IMPID receives our
 * original message.  We need to wait for the IMPI to ACK back to us
 * saying that the remote side has sent back the ack
 * (IMPI_PK_SYNCACK).  [Mis-]Use the shadow communicator for this so
 * that we don't interfere with the real message.
 */
	DBUG("MPI_Ssend registering for syncack\n");
	err = IMPI_Register_syncack(dest, tag, comm, &req);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPISSEND, err));
#endif
	err = lam_send(buf, count, dtype, dest, tag, comm, LAM_RQISSEND);

	if (err != MPI_SUCCESS) 
	  return(lam_errfunc(comm, BLKMPISSEND, err));

#if LAM_WANT_IMPI
	DBUG("MPI_Ssend waiting for syncack\n");
	if (req != MPI_REQUEST_NULL) {
	  DBUG("LAM rank waiting for SYNCACK\n");
	  MPI_Wait(&req, MPI_STATUS_IGNORE);
	  DBUG("LAM rank got SYNCACK from remote host\n");
	}
#endif

	lam_resetfunc_m(BLKMPISSEND);
	return(MPI_SUCCESS);
}
