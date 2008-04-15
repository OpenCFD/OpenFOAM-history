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
 * $Id: itoa.c,v 6.4 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- K&R itoa()
 */

#include <lam_config.h>

#include <string.h>

#include <sfh.h>

/*
 * local functions
 */
static void reverse(char s[]);

void
sfh_itoa(int n, char *s)
{
	int		i;
	int		sign;

	if ((sign = n) < 0) n = -n;

	i = 0;

	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0) s[i++] = '-';

	s[i] = '\0';
	reverse(s);
}

/*
 *	reverse
 *
 *	Function:	- reverse string in place
 *	Accepts:	- string
 */
static void
reverse(char s[])
{
	int		c;
	int		i;
	int		j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


#if 0
/*
 * backwards compatibility
 */
void itoa(int n, char *s)
{ sfh_itoa(n, s); }
#endif
