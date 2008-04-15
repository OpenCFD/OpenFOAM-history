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
 * $Id: atos.c,v 6.4 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- converts ascii decimal string to short integer
 */

#include <lam_config.h>

#include <etc_misc.h>


short int
atos(char *s)
{
	short		sign;		/* sign of result */
	short		n;		/* integer result */

	if (*s == '-') {
		sign = -1;
		s++;
    	} else {
		sign = 1;
	}

	for (n = 0; (*s >= '0') && (*s <= '9'); s++) {
		n = (n * 10) + (*s - '0');
	}

	return(n * sign);
}
