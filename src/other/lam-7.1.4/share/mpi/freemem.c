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
 * $Id: freemem.c,v 6.7 2002/12/19 18:43:01 jsquyres Exp $
 *
 *	Function:	- free memory allocated from MPI_Alloc_mem
 *	Accepts:	- ptr to memory to free
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>


/*@

MPI_Free_mem - Free memory that was previously allocated by MPI_ALLOC_MEM

Input Parameter:
. base - pointer to memory to free

Notes:

For the TCP, SYSV, and USYSV RPIs, this function is simply a wrapper
around 'free'.

However, future RPIs (e.g., for Myrinet and VIA-based RPIs) will
provide "special" memory deallocation routines that will deallocate
memory that was used directly for DMA (i.e., allocated by
'MPI_ALLOC_MEM').

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_NO_MEM

.N ACK
@*/
int MPI_Free_mem(void *base)
{
  lam_initerr();
  lam_setfunc(BLKMPIFREEMEM);
  
  if (base == 0) {
    return(lam_errfunc(MPI_COMM_WORLD,
		       BLKMPIALLOCMEM, lam_mkerr(MPI_ERR_BASE, EINVAL)));
  }
  
  /* Call RPI's free_mem function. */

  if (RPI_FREE_MEM(base) != MPI_SUCCESS)
    return(lam_errfunc(MPI_COMM_WORLD,
		       BLKMPIALLOCMEM, lam_mkerr(MPI_ERR_BASE, EINVAL)));

  lam_resetfunc(BLKMPIFREEMEM);
  return(MPI_SUCCESS);
}
