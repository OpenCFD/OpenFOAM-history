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
 * $Id: prime.c,v 6.4 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- test and generate prime numbers
 *				- is_prime(): test is number is prime
 *				- next_prime(): return the next prime number
 */

#include <portable.h>
#include <typical.h>
#include <etc_misc.h>

/*
 *	is_prime
 *
 *	Function:	- check if the number is prime
 *	Accepts:	- a positive integer
 *	Returns:	- TRUE or FALSE
 */
int
is_prime(int4 n)
{
	int4		i;		/* favourite counter */
	int4		i_2;		/* i squared */

	if (n <= 0) {
		return(FALSE);
	} else if ((n == 1) || (n == 2)) {
		return(TRUE);
	} else if ((n % 2) == 0) {
		return(FALSE);
	}
/*
 * Loop through all odd number smaller than the largest divisor
 * checking if any divides evenly the given number.
 */
	else {
		for (i = 3, i_2 = 9; i_2 <= n; i += 2) {
			if ((n % i) == 0) {
				return(FALSE);
			}

			i_2 += (i + 1) << 2;
		}
	}

	return(TRUE);
}

/*
 *	next_prime
 *
 *	Function:	- get the first prime # >= the given number
 *	Accepts:	- a positive number
 *	Returns:	- a prime number or ERROR
 */
int4
next_prime(int4 n)
{
	if (n < 0) {
		return((int4) ERROR);
	} else if (n < 1) {
		return((int4) 1);
	} else if (n < 2) {
		return((int4) 2);
	} else {
/*
 * Find the next prime number in the general case.
 */
		if ((n % 2) == 0) {
			n++;
		}

		while (! is_prime(n)) {
			n += 2;
		}

		return(n);
	}
}
