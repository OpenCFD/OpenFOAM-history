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
 * $Id: ssi_rpi_mem.c,v 1.2 2002/10/09 21:00:14 brbarret Exp $
 *
 *	Function: - default lam_ssi_rpi_c2c_* memory functions;
 *                  wrappers to malloc() and free().
 */

#include <lam_config.h>

#include <mpi.h>
#include <mpisys.h>
#include <stdlib.h>


/*
 *	lam_ssi_rpi_alloc_mem
 *
 *	Function:	- default alloc_mem function; wrapper to malloc
 *	Accepts:	- size to allocate
 *                      - info arg (ignored here)
 *	Returns:	- ptr to allocated memory
 */
int
lam_ssi_rpi_base_alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr)
{
  void *ret = malloc(size);
  if (ret == 0)
    return LAMERROR;

  *((void **) baseptr) = ret;

  return MPI_SUCCESS;
}


/*
 *	lam_ssi_rpi_free_mem
 *
 *	Function:	- default free_mem function; wrapper to free
 *	Accepts:	- ptr to memory to free
 *	Returns:	- MPI_SUCCESS
 */
int 
lam_ssi_rpi_base_free_mem(void *base)
{
  free(base);

  return MPI_SUCCESS;
}


