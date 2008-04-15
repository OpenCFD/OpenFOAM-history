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
 *      $Id: wcallerr_f.c,v 1.2 2003/08/05 23:19:15 vsahay Exp $
 *
 *      Function:       - MPI_Win_callerr F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_win_call_errhandler_(int *w, int *e, int *ierr)
{
  *ierr = MPI_Win_call_errhandler(GETHDL(*w), *e);
}
