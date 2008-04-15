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
 *	$Id: wunlock.c,v 1.1 2003/01/17 08:21:30 vsahay Exp $
 *
 *	Function:	- Starts an RMA access epoch.
 *	                  Currently not implemented.
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
    MPI_Win_unlock -  Completes an RMA access epoch started by a call to MPI_WIN_LOCK

Input Parameters:
+ rank - rank of window (nonnegative integer)
- win - window object (handle)

Notes:

This function is currently not implemented in LAM/MPI.  It will return
an MPI_ERR_OTHER run time error.

.N fortran
@*/
int MPI_Win_unlock(int rank, MPI_Win win)
{
  int ret;

  lam_initerr_m();
  lam_setfunc_m(BLKMPIWINUNLOCK);
  
  ret = lam_err_win(win, MPI_ERR_OTHER, ENOT_IMPLEMENTED, 
		    "Not yet implemented in LAM/MPI");
  
  lam_resetfunc_m(BLKMPIWINUNLOCK);
  
  return ret;
}
