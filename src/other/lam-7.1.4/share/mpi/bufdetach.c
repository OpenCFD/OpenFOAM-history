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
 * $Id: bufdetach.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- detach buffer from process
 *	Accepts:	- ptr buffer address
 *			- ptr buffer size
 *	Returns:	- MPI_SUCCESS
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
  MPI_Buffer_detach - Removes an existing buffer (for use in 'MPI_Bsend' etc)

Output Parameters:
+ pbuf - initial buffer address (choice) 
- psize - buffer size, in bytes (integer) 

Notes:

The reason that 'MPI_Buffer_detach' returns the address and size of
the buffer being detached is to allow nested libraries to replace and
restore the buffer.  For example, consider

.vb
    int size, mysize, idummy;
    void *ptr, *myptr, *dummy;     
    MPI_Buffer_detach(&ptr, &size);
    MPI_Buffer_attach(myptr, mysize);

    // ...library code...

    MPI_Buffer_detach(&dummy, &idummy);
    MPI_Buffer_attach(ptr, size);
.ve

This is much like the action of the Unix signal routine and has the
same strengths (it is simple) and weaknesses (it only works for nested
usages).

Note that for this approach to work, 'MPI_Buffer_detach' must return
'MPI_SUCCESS' even when there is no buffer to detach.  In that case,
it returns a size of zero.  The MPI 1.1 standard for
'MPI_BUFFER_DETACH' contains the text

.vb
   The statements made in this section describe the behavior of MPI
   for buffered-mode sends. When no buffer is currently associated,
   MPI behaves as if a zero-sized buffer is associated with the
   process.
.ve

This could be read as applying only to the various 'Bsend' routines.
This implementation takes the position that this applies to
'MPI_BUFFER_DETACH' as well.

.N fortran

The Fortran binding for this routine is different.  Because Fortran
does not have pointers, it is impossible to provide a way to use the
output of this routine to exchange buffers.  In this case, only the
size field is set.

Notes for C:

Even though the 'pbuf' argument is declared as 'void *', it is really
the address of a void pointer (i.e., 'void **').  See the rationale in
the MPI Standard for more details.

.N ACK
@*/
int MPI_Buffer_detach(void *pbuf, int *psize)
{
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPIBUFDETACH);

	err = lam_bufdetach(pbuf, psize);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIBUFDETACH, err));
	}

	lam_resetfunc(BLKMPIBUFDETACH);
	return(MPI_SUCCESS);
}
