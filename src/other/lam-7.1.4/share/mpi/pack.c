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
 * $Id: pack.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- pack from data buffer
 *	Accepts:	- data buffer
 *			- data count
 *			- data type
 *			- packing buffer
 *			- packing buffer size
 *			- packing buffer position (in-out)
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Pack - Packs a datatype into contiguous memory

Input Parameters:
+ buf - input buffer start (choice) 
. count - number of input data items (integer) 
. dtype - datatype of each input data item (handle) 
. packsize - output buffer size, in bytes (integer) 
- comm - communicator for packed message (handle) 

Input/Output Parameter:
. packpos - current position in buffer, in bytes (integer) 

Output Parameter:
. packbuf - output buffer start (choice) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_BUFFER
.N MPI_ERR_TRUNCATE

.seealso: MPI_Pack_size, MPI_Unpack

.N ACK
@*/
int MPI_Pack(void *buf, int count, MPI_Datatype dtype, 
	     void *packbuf, int packsize, int *packpos, 
	     MPI_Comm comm)
{
	int		size;
	char		*p;

	lam_initerr();
	lam_setfunc(BLKMPIPACK);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIPACK, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit)) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((packsize < 0) || (packpos == 0)) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((*packpos < 0) || ((*packpos >= packsize) && (packsize > 0))) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if ((packsize > 0) && (packbuf == 0)) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_BUFFER, EINVAL)));
	}
/*
 * Pack the data.
 */
	p = ((char *) packbuf) + *packpos;
	size = packsize - *packpos;

	size = lam_pack((char *) buf, count, dtype, p, size);

	if (size < 0) {
		return(lam_errfunc(comm,
			BLKMPIPACK, lam_mkerr(MPI_ERR_TRUNCATE, EIO)));
	}

	*packpos += size;

	lam_resetfunc(BLKMPIPACK);
	return(MPI_SUCCESS);
}
