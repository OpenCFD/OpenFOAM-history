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
 * $Id: isthrmain.c,v 6.6 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- see if this is the main MPI thread
 *	Accepts:	- ptr to flag
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdio.h>

#include <mpi.h>
#include <mpisys.h>
#include <lammpithreads.h>
#include <blktype.h>


/*@
   MPI_Is_thread_main - determine if the caller is the main thread

Output Parameter:
. pflag - true if calling thread is main thread, false otherwise

Description:

This function is mainly here for link-compatability.  Since only the
'MPI_THREAD_SINGLE' level of thread concurrency is supported in
LAM/MPI, this function has limited usefulness.  Future versions of
LAM/MPI will support multi-threaded user programs, in which case
'MPI_Init_thread' must be used to initialize MPI.  Hence, programmers
can use this function now in order to program for future flexibility.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso MPI_Init_thread(3), MPI_Query_thread(3)

.N WEB
@*/
int
MPI_Is_thread_main(int *pflag)
{
  lam_setfunc(BLKMPIISTHREADMAIN);
/*
 * Ensure we got a valid requested and pprovided
 */
  if (pflag == NULL) {
    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIISTHREADMAIN,
		       lam_mkerr(MPI_ERR_ARG, EMPIINIT)));
  }
/*
 * Are we the main thread?
 */
  if (lam_thread_self() == lam_mpi_main_thread)
    *pflag = 1;
  else
    *pflag = 0;
  
  lam_resetfunc(BLKMPIISTHREADMAIN);
  return(MPI_SUCCESS);
}

