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
 * $Id: initthr.c,v 6.9 2003/04/25 20:38:42 jsquyres Exp $
 *
 *	Function:	- initialize the MPI session for threads
 *			- then calls MPI_Init
 *	Accepts:	- ptr to argc
 *			- ptr to argv
 *			- desired thread level
 *			- provided thread level
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdio.h>

#include <mpi.h>
#include <mpisys.h>
#include <lammpithreads.h>
#include <blktype.h>


/*@
   MPI_Init_thread - Initialize the MPI execution environment

Input Parameters:
+ pargc - Pointer to the number of arguments 
. pargv - Pointer to the argument vector
. requested - Desired level of thread support
- pprovided - Given level of thread support

Notes:

LAM currently supports 'MPI_THREAD_SINGLE', 'MPI_THREAD_SERIALIZED',
and 'MPI_THREAD_FUNNELED'.  'MPI_THREAD_MULTIPLE' is not yet
supported.  The use of 'MPI_INIT_THREAD' is recommended instead of
'MPI_INIT' if an MPI program is going to be multi-threaded.  However,
the 'LAM_MPI_THREAD_LEVEL' environment variable can be used to the
same effect.  See MPI_Init(3) for more details.

There is currently no distinction between 'MPI_THREAD_SINGLE' and
'MPI_THREAD_FUNNELED'.  An application using 'MPI_THREAD_FUNNELED'
promises to never call MPI from a different thread than the one that
invoked 'MPI_INIT' (or 'MPI_INIT_THREAD').  LAM does no checking to
ensure that this is true.  As such, no locks are placed around MPI
calls to ensure that only one thread is making MPI calls at a time
(since the application has promised that only one thread will be
making MPI calls).

Using 'MPI_THREAD_SERIALIZED' will cause LAM to place locks around all
MPI calls such that only one thread will be able to enter the MPI
library at a time; beware of this fact for portability with other MPI
implementations.  Even with multiple threads, deadlock is still
possible when using 'MPI_THREAD_SERIALIZED' -- applications still need
to be aware of this and code appropriately.

Other than this restriction, this function behaves identically to
'MPI_INIT' (see MPI_Init(3)).

Note that both 'MPI_INIT' and 'MPI_INIT_THREAD' are allowed to changed
the requested thread level based on lower-level device and SSI module
initializations.  For example, if a user application invokes
'MPI_Init', some SSI modules may require threads and therefore upgrade
the thread support level to something higher than 'MPI_THREAD_SINGLE'.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INIT
.N MPI_ERR_OTHER

.seealso MPI_Init, MPI_Finalize, lamboot, mpirun, lamhalt, lamssi

.N ACK
@*/
int
MPI_Init_thread(int *pargc, char ***pargv, int requested, int *pprovided)
{
  int err;
  
  lam_setfunc(BLKMPIINITTHREAD);

  /* Ensure we got a valid requested and pprovided */

  if (requested < MPI_THREAD_SINGLE || 
      requested > MPI_THREAD_MULTIPLE ||
      pprovided == NULL) {
    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIINITTHREAD,
		       lam_mkerr(MPI_ERR_ARG, EMPIINIT)));
  }

  /* Call the back-end init function (shared between MPI_INIT and
     MPI_INIT_THREAD) */

  err = lam_mpi_init(pargc, pargv, requested, pprovided, BLKMPIINITTHREAD);

  /* All done */

  lam_resetfunc(BLKMPIINITTHREAD);
  return err;
}
