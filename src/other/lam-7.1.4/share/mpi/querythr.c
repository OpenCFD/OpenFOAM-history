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
 * $Id: querythr.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- query level of thread support in MPI
 *	Accepts:	- ptr to provided level
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdio.h>

#include <mpi.h>
#include <mpisys.h>
#include <lammpithreads.h>
#include <blktype.h>


/*@
   MPI_Query_thread - Check level of thread support in MPI

Output Parameter:
. pprovided - provided level of thread support

Description:

This function is mainly here for link-compatability.  It will
[currently] only ever return 'MPI_THREAD_SINGLE' in 'pprovided'.
Future versions of LAM/MPI will support multi-threaded user programs,
in which case 'MPI_Init_thread' must be used to initialize MPI.  Hence,
programmers can use this function now in order to program for future
flexibility.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso MPI_Init_thread(3), MPI_Is_thread_main(3)

.N WEB
@*/
int
MPI_Query_thread(int *pprovided)
{
  lam_setfunc(BLKMPIQUERYTHREAD);

  if (pprovided == NULL) {
    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIQUERYTHREAD,
		       lam_mkerr(MPI_ERR_ARG, EMPIINIT)));
  }
  
  *pprovided = lam_mpi_thread_level;
  
  lam_resetfunc(BLKMPIQUERYTHREAD);
  return(MPI_SUCCESS);
}

