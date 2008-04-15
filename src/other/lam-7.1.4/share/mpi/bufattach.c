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
 * $Id: bufattach.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- attach buffer to process
 *	Accepts:	- buffer address
 *			- buffer size
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
  MPI_Buffer_attach - Attaches a user-defined buffer for sending

Input Parameters:
+ buf - initial buffer address (choice) 
- size - buffer size, in bytes (integer) 

Notes:

The size given should be the sum of the sizes of all outstanding
Bsends that you intend to have, plus a few hundred bytes for each
Bsend that you do.  For the purposes of calculating size, you should
use 'MPI_Pack_size'.  In other words, in the code

.vb
     MPI_Buffer_attach( buffer, size );
     MPI_Bsend( ..., count=20, datatype=type1,  ... );
     ...
     MPI_Bsend( ..., count=40, datatype=type2, ... );
.ve

the value of 'size' in the 'MPI_Buffer_attach' call should be greater
than the value computed by

.vb
     MPI_Pack_size(20, type1, comm, &s1);
     MPI_Pack_size(40, type2, comm, &s2);
     size = s1 + s2 + 2 * MPI_BSEND_OVERHEAD;
.ve    

'MPI_Bsend_overhead' gives the maximum amount of space that may be
used in the buffer for use by the 'Bsend' routines in using the
buffer.  This value is in 'mpi.h' (for C) and 'mpif.h' (for Fortran).

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_BUFFER
.N MPI_ERR_INTERN

.seealso: MPI_Buffer_detach, MPI_Bsend

.N ACK
@*/
int MPI_Buffer_attach(void *buf, int size)
{
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPIBUFATTACH);

	err = lam_bufattach(buf, size);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIBUFATTACH, err));
	}

	lam_resetfunc(BLKMPIBUFATTACH);
	return(MPI_SUCCESS);
}
