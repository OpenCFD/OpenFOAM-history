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
 * $Id: recvinit.c,v 6.10 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- create a receive request
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- source
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
#endif

/*@
    MPI_Recv_init - Builds a handle for a receive

Input Parameters:
+ buf - initial address of receive buffer (choice) 
. count - number of elements received (integer) 
. dtype - type of each element (handle) 
. src - rank of source or 'MPI_ANY_SOURCE' (integer) 
. tag - message tag or 'MPI_ANY_TAG' (integer) 
- comm - communicator (handle) 

Output Parameter:
. req - communication request (handle) 

Notes:

The communication initialized by this function will not be started
until 'req' is given to 'MPI_Start' or 'MPI_Startall'.  

The communication is then not guaranteed to progress or complete until
'req' has been given to one of the test or wait functions ('MPI_Test',
'MPI_Testall', 'MPI_Testany', 'MPI_Testsome', 'MPI_Wait',
'MPI_Waitall', 'MPI_Waitany', 'MPI_Waitsome').

Once any of the test or wait functions indicate that the communication
has finished, the communication cycle can be started again with
'MPI_Start' or 'MPI_Startall'.

However, once the communication has finished and it is no longer going
to be used, 'req' should be freed with 'MPI_Request_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_RANK
.N MPI_ERR_TAG
.N MPI_ERR_COMM
.N MPI_ERR_EXHAUSTED

.seealso MPI_Recv, MPI_Start, MPI_Startall, MPI_Test, MPI_Testall, MPI_Testany, MPI_Testsome, MPI_Wait, MPI_Waitall, MPI_Waitany, MPI_Waitsome

.N ACK
@*/
int MPI_Recv_init(void *buf, int count, MPI_Datatype dtype, 
		  int src, int tag, MPI_Comm comm, 
		  MPI_Request *req)
{
	int		err;			/* error code */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIRINIT);

	if ((tag < 0 && tag != MPI_ANY_TAG) || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIRINIT,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	*req = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, src, tag, comm,
				LAM_RQIRECV, req);

	if (err != MPI_SUCCESS) 
	  return(lam_errfunc(comm, BLKMPIRINIT, err));

	(*req)->rq_marks |= (LAM_RQFPERSIST | LAM_RQFMAND);
#if LAM_WANT_IMPI
	IMPI_Register_datasync_init(src, tag, comm, &((*req)->rq_shadow));
#endif

	lam_resetfunc_m(BLKMPIRINIT);
	return(MPI_SUCCESS);
}
