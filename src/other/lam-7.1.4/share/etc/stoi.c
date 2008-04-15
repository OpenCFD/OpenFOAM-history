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
 * $Id: stoi.c,v 6.5 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- convert decimal or hexadecimal string to integer
 */

#include <lam_config.h>

#include <lam.h>
#include <laminternal.h>


int4
stoi(unsigned char *s) 
{
	int4  		base;			/* base of digits */
	int4  		i;			/* result */
	int4  		sign;			/* sign of result */
	int		valid;			/* valid digit? */
	unsigned char	c;			/* current digit */

	if (s[0] == '-') {
		sign = -1;
		s++;
	} else {
		sign = 1;
	}

	if ((s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X'))) {
		base = 16;
		s += 2;
	} else {
		base = 10;
	}

	i = 0;
	valid = 1;

	while (*s && valid) {
		c = *s;

		if ((c >= '0') && (c <= '9')) {
			i = (i * base) + (c - '0');
		}

		else if (base == 16) {

			if ((c >= 'a') && (c <= 'f')) {
				i = (i * base) + (c - 'a' + 0xa);
			}

			else if ((c >= 'A') && (c <= 'F')) {
				i = (i * base) + (c - 'A' + 0xA);
			}

			else {
				valid = 0;
			}
		} else {
			valid = 0;
		}

		s++;
	}

	i *= sign;

	return(i);
}
