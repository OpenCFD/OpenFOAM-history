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
 * $Id: memucmp.c,v 6.4 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- compare arrays of bytes (unsigned characters)
 *	Accepts:	- ptr to first array
 *			- ptr to second array
 *			- # of bytes
 *	Returns:	- -1, 0, +1 (m1 < m2, m1 == m2, m1 > m2)
 */

#include <lam_config.h>

#include <portable.h>
#include <etc_misc.h>


int
memucmp(unsigned char *m1, unsigned char *m2, int4 nbytes)
{
	for (; nbytes > 0; --nbytes, ++m1, ++m2) {

		if (*m1 < *m2) {
			return(-1);
		}
		else if (*m1 > * m2) {
			return(1);
		}
	}

	return(0);
}
