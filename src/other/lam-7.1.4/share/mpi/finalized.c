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
 * $Id: finalized.c,v 6.4 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- check if MPI has been finalized
 *	Accepts:	- result flag (out)
 *	Returns:	- MPI_SUCCESS
 */

#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Finalized - Indicates whether 'MPI_Finalize' has completed.

Output Parameter:

. flag - Flag is true (1) if 'MPI_Finalize' has been called and false (0)
otherwise.

Notes:

Note that this function does not indicate whether 'MPI_INIT' has been
called.  It only indicates whethere 'MPI_FINALIZE' has been called.

This function always returns 'MPI_SUCCESS'.

.N fortran

.N ACK
@*/
int MPI_Finalized(int *flag)
{
  /*
   * This function (like MPI_INITIALIZED) is called from MPI_INIT and
   * MPI_FINALIZE.  Because of this, badness may occur if
   * lam_initerr() is called within this function.
   */

  if (flag)
    *flag = lam_finalized();
  
  return(MPI_SUCCESS);
}
