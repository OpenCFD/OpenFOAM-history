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
 * $Id: allocmem.c,v 6.7 2002/12/19 18:43:01 jsquyres Exp $
 *
 *	Function:	- allocate memory for the RPI
 *	Accepts:	- size to allocate
 *			- info object
 *			- pointer to memory to allocate
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>


/*@

MPI_Alloc_mem - Allocate memory for use by MPI message passing.

Input Parameters:
. size - number of bytes to allocate
. info - 'MPI_Info' object

Output Parameter:
. baseptr - pointer to address of allocated memory

Notes:

'MPI_INFO_NULL' is always valid for this function.  For the TCP, SYSV,
and USYSV RPIs, the 'info' argument is ignored, and this function is
simply a wrapper around 'malloc'.

However, future RPIs (e.g., for Myrinet and VIA-based RPIs) will
provide "special" memory allocation routines that will allocate memory
that can be used directly for DMA.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_NO_MEM

.N ACK
@*/
int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr)
{
  lam_initerr();
  lam_setfunc(BLKMPIALLOCMEM);
  
  if (size < 0) {
    return(lam_errfunc(MPI_COMM_WORLD,
		       BLKMPIALLOCMEM, lam_mkerr(MPI_ERR_ARG, EINVAL)));
  }
  if (baseptr == 0) {
    return(lam_errfunc(MPI_COMM_WORLD,
		       BLKMPIALLOCMEM, lam_mkerr(MPI_ERR_ARG, EINVAL)));
  }

  /* If we're in lamd mode, do a regular malloc.  Otherwise, call
     the RPI's alloc_mem function. */

  if (RPI_ALLOC_MEM(size, info, baseptr) != MPI_SUCCESS)
    return(lam_errfunc(MPI_COMM_WORLD,
		       BLKMPIALLOCMEM, lam_mkerr(MPI_ERR_NO_MEM, ENOMEM)));

  lam_resetfunc(BLKMPIALLOCMEM);
  return(MPI_SUCCESS);
}
