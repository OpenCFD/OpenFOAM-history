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
 * $Id: wtick.c,v 6.6 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- return timer resolution
 *	Returns:	- time resolution in seconds
 */

#include <mpi.h>
#include <mpisys.h>
#include <net.h>

/*@
  MPI_Wtick - Returns the resolution of MPI_Wtime

Return value:

Time in seconds of resolution of 'MPI_Wtime'.

Notes for Fortran:

This is function is declared as 'DOUBLE PRECISION MPI_WTICK()' in
Fortran.

.seealso: MPI_Wtime  

.N ACK
@*/
double MPI_Wtick(void)
{
	static double	tick = 0.0;
	double		t;
	int		i;

	if (tick == 0.0) {

		tick = ttime();
		tick = ttime() - tick;
		
		for (i = 0; i < 10; i++) {
			t = ttime();
			t = ttime() - t;

			if (t < tick) {
				tick = t;
			}
		}

		tick = (tick > 0.0) ? tick : 1.0e-6;
	}

	return(tick);
}
