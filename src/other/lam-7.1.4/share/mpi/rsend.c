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
 * $Id: rsend.c,v 6.7 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- ready-mode send
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
#include <rpisys.h>

/*@
    MPI_Rsend - Basic ready send 

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (nonnegative integer) 
. dtype - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

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
int MPI_Rsend(void *buf, int count, MPI_Datatype dtype, 
	      int dest, int tag, MPI_Comm comm)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIRSEND);

	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_errfunc(comm, BLKMPIRSEND,
				lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	err = lam_send(buf, count, dtype, dest, tag, comm, LAM_RQIRSEND);

	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPIRSEND, err));

	lam_resetfunc_m(BLKMPIRSEND);
	return(MPI_SUCCESS);
}
