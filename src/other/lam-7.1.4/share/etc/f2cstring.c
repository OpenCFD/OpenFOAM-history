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
 * $Id: f2cstring.c,v 6.5 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- create a C string from a F77 string
 *	Accepts:	- F77 string
 *			- string length
 *	Returns:	- C string or NULL
 */

#include <stdlib.h>
#include <string.h>

#include <etc_misc.h>


char *
f2cstring(char *str, int len)
{
	char		*cstr;
	int		i;
/*
 * Allocate space for the C string.
 */
	if ((cstr = malloc(len + 1)) == 0) {
		return((char *) 0);
	}

	if (len > 0) {
		lam_strncpy(cstr, str, (int) len);
	}
/*
 * Bypass trailing spaces and null-terminate the string.
 * If all characters are spaces, leave the first one in.
 */
	for (i = len - 1; i > 0; --i) {
		if (cstr[i] != ' ') {
			break;
		}
	}

	if ((i >= 0) || (len == 0)) {
		cstr[i + 1] = '\0';
	}

	return(cstr);
}
