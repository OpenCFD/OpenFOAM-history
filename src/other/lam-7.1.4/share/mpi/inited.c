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
 * $Id: inited.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- check if MPI has been initialized
 *	Accepts:	- result flag (out)
 *	Returns:	- MPI_SUCCESS
 */

#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Initialized - Indicates whether 'MPI_Init' has been called.

Output Parameter:

. flag - Flag is true (1) if 'MPI_Init' has been called and false (0)
otherwise.

Notes:

This function always returns 'MPI_SUCCESS'.

.N fortran

.N ACK
@*/
int MPI_Initialized(int *flag)
{
  /*
   * This function (like MPI_FINALIZED) is called from MPI_INIT and
   * MPI_FINALIZE.  Because of this, badness may occur if
   * lam_initerr() is called within this function.
   */

	if (flag) {
		*flag = lam_inited();
	}

	return(MPI_SUCCESS);
}
