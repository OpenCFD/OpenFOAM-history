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
 * $Id: wtime.c,v 6.5 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- return wall clock time
 *	Returns:	- wall clock time in seconds
 */

#include <mpi.h>
#include <mpisys.h>
#include <net.h>

/*@
MPI_Wtime - Returns an elapsed time on the calling processor

Return value:

Time in seconds since an arbitrary time in the past.

Notes:

This is intended to be a high-resolution, elapsed (or wall) clock.
See 'MPI_Wtick' to determine the resolution of 'MPI_Wime'.  If the
attribute 'MPI_WTIME_IS_GLOBAL' is defined and true, then the value is
synchronized across all processes in 'MPI_COMM_WORLD'.

Notes for Fortran:

This function is declared as 'DOUBLE PRECISION MPI_WTIME()' in
Fortran.

.seealso: MPI_Wtick, MPI_Comm_get_attr

.N ACK
@*/
double MPI_Wtime(void)
{
	return(ttime());
}
