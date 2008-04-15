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
 * $Id: unpack.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- unpack to data buffer
 *	Accepts:	- packed buffer
 *			- packed buffer size
 *			- packed buffer position (in-out)
 *			- data buffer
 *			- data count
 *			- data type
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Unpack - Unpack a datatype into contiguous memory

Input Parameters:
+ packbuf - input buffer start (choice) 
. packsize - size of input buffer, in bytes (integer) 
. count - number of items to be unpacked (integer) 
. dtype - datatype of each output data item (handle) 
- comm - communicator for packed message (handle) 

Input/Output Parameters:
. packpos - current position in bytes (integer) 

Output Parameter:
. buf - output buffer start (choice) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_ARG
.N MPI_ERR_BUFFER
.N MPI_ERR_TRUNCATE

.seealso: MPI_Pack, MPI_Pack_size

.N ACK
@*/
int MPI_Unpack(void *packbuf, int packsize, int *packpos, 
	       void *buf, int count, MPI_Datatype dtype, 
	       MPI_Comm comm)
{
	int		size;
	char		*p;

	lam_initerr();
	lam_setfunc(BLKMPIUNPACK);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit)) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((packsize < 0) || (packpos == 0)) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((*packpos < 0) || ((*packpos >= packsize) && (count > 0))) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((packsize > 0) && (packbuf == 0)) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_BUFFER, EINVAL)));
	}
/*
 * Unpack the data.
 */
	p = ((char *) packbuf) + *packpos;
	size = packsize - *packpos;

	size = lam_unpack(p, size, (char *) buf, count, dtype);

	if (size < 0) {
		return(lam_errfunc(comm,
			BLKMPIUNPACK, lam_mkerr(MPI_ERR_TRUNCATE, EIO)));
	}

	*packpos += size;

	lam_resetfunc(BLKMPIUNPACK);
	return(MPI_SUCCESS);
}
