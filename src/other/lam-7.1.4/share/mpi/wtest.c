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
 * $Id: wtest.c,v 1.5 2003/01/17 08:21:30 vsahay Exp $
 *
 *	Function:	- check for progress on a window.  
 *	                  Currently not implemented.
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
    MPI_Win_Test - Test for progress on a window

Input Parameters:
. win - window (handle)

Output Parameters:
. flag - (integer)

Notes:

This function is currently not implemented in LAM/MPI.  It will return
an MPI_ERR_OTHER run time error.

.N fortran
@*/
int MPI_Win_test(MPI_Win win, int *flag)
{
  int ret;

  lam_initerr_m();
  lam_setfunc_m(BLKMPIWINTEST);

  *flag = 0;
  ret = lam_err_win(win, MPI_ERR_OTHER, ENOT_IMPLEMENTED, 
		    "Not yet implemented in LAM/MPI");
  
  lam_resetfunc_m(BLKMPIWINTEST);
  
  return ret;
}
