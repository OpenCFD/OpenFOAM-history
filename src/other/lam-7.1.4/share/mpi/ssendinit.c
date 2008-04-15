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
 * $Id: ssendinit.c,v 6.16 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- create a synchronous send request
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
#if LAM_WANT_IMPI
#include <impi.h>
#include <lamdebug.h>
#endif


/*@
    MPI_Ssend_init - Builds a handle for a synchronous send

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements sent (integer) 
. dtype - type of each element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Output Parameter:
. req - communication request (handle) 

Notes:

The communication initialized by this function will not be started
until 'req' is given to 'MPI_Start' or 'MPI_Startall'.  

The communication is then not guaranteed to progress or complete until
'req' has been given to one of the test or wait functions ('MPI_Test',
'MPI_Testall', 'MPI_Testany', 'MPI_Testsome', 'MPI_Wait',
'MPI_Waitall', 'MPI_Waitany', 'MPI_Waitsome').  The communication will
not complete until the destination rank `starts` to receive the
message.  Note that this is different than saying that the message has
been received by the destination rank.


Once any of the test or wait functions indicate that the communication
has finished, the communication cycle can be started again with
'MPI_Start' or 'MPI_Startall'.

However, once the communication has finished and it is no longer going
to be used, 'req' should be freed with 'MPI_Request_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.seealso MPI_Ssend, MPI_Start, MPI_Startall, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Ssend_init(void *buf, int count, MPI_Datatype dtype, 
		   int dest, int tag, MPI_Comm comm, 
		   MPI_Request *req)
{
	int		err;			/* error code */
#if LAM_WANT_IMPI
	MPI_Request     lreq;
#endif

	lam_initerr_m();
	lam_setfunc_m(BLKMPISSINIT);

	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPISSINIT,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	if (dest == MPI_ANY_SOURCE) {
		return(lam_errfunc(comm, BLKMPISSINIT,
				lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

#if LAM_WANT_IMPI
	/* If IMPI_Isend_lamgiappe returns != MPI_REQUEST_NULL, this
           is an IMPI proxy send.  Need to re-route to the impid.  */

	IMPI_Isend_lamgiappe_init(LAM_RQISSEND, count, dtype, dest, tag, 
				  comm, &lreq);
	if (lreq != MPI_REQUEST_NULL) {
	  dest = impid_comm->c_group->g_nprocs - 1;
	  tag  = IMPI_MESSAGE_TAG;
	  comm = impid_comm;
	}
#endif

	*req = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, dest, tag, comm,
				LAM_RQISSEND, req);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPISSINIT, err));
	}

	(*req)->rq_marks |= (LAM_RQFPERSIST | LAM_RQFMAND);
#if LAM_WANT_IMPI
	(*req)->rq_shadow = lreq;
#endif

	lam_resetfunc_m(BLKMPISSINIT);
	return(MPI_SUCCESS);
}
