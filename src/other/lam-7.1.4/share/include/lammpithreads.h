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
 * $Id: lammpithreads.h,v 6.4 2003/04/03 18:08:33 jsquyres Exp $
 *
 *	Function:	- header for LAM's MPI thread interface
 */

#ifndef LAM_MPI_THREADS_H_
#define LAM_MPI_THREADS_H_

#include <lam_config.h>
#include <lamthreads.h>

/*
 * Main LAM MPI thread globals
 */
extern int		lam_mpi_thread_level;
extern lam_thread_t	lam_mpi_main_thread;
extern lam_mutex_t	lam_mpi_mutex;


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

  /* LAM MPI locks interface  */

  void lam_mpi_thread_init(void);
  void lam_mpi_thread_finalize(void);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif	/* _LAM_MPI_THREADS_H */
