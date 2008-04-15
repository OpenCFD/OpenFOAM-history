/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: lamthreads.c,v 6.19 2003/04/09 17:25:04 brbarret Exp $
 *
 *	Function:	- LAM interface for threads in MPI layer
 *                      - Anything applicable to threads in general
 *                        should not be here, but should be in
 *                        share/threads/
 */

#include <lam_config.h>

#include <stdio.h>
#include <mpi.h>
#include <lammpithreads.h>


/*
 * Exported variables
 */
int lam_mpi_thread_level = MPI_THREAD_SINGLE;
lam_thread_t lam_mpi_main_thread;
lam_mutex_t lam_mpi_mutex;


/*
 *	lam_thread_init
 *
 *	Function:	- initialize the LAM thread interface
 */
void
lam_mpi_thread_init(void)
{
#if LAM_HAVE_THREADS

  /* Initialize the global mutex */

  if (lam_mpi_thread_level >= MPI_THREAD_SERIALIZED)
    lam_mutex_init(&lam_mpi_mutex);
#endif

  /* Set the main thread variable -- even if we don't have threads, so
     that MPI_IS_THREAD_MAIN will return something predicable, at
     least. */

  lam_mpi_main_thread = lam_thread_self();
}


/*
 *	lam_thread_finalize
 *
 *	Function:	- finalize the LAM thread interface
 */
void
lam_mpi_thread_finalize(void)
{
#if LAM_HAVE_THREADS
  if (lam_mpi_thread_level >= MPI_THREAD_SERIALIZED)
    lam_mutex_destroy(&lam_mpi_mutex);
#endif
}
