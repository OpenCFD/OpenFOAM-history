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
 * $Id: rev.c,v 6.5 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- reverse byte ordering of data types
 *			- handles 4 and 8-byte types in fast manner
 *			- handles all other types as byte arrays
 */

#include <portable.h>
#include <t_types.h>

/*
 *	rev4
 *
 *	Function:	- reverse byte ordering of a 4-byte array
 *			- array is treated as an unsigned 4-byte integer
 *	Accepts:	- ptr source array
 *			- ptr destination array
 */
void
rev4(void *src, void *dest)
{
	uint4		old;
	uint4		*d;

	old = *((uint4 *) src);
	d = (uint4 *) dest;

	*d = (old & 0x000000FF) << 16;
	*d = (*d | (old & 0x0000FF00)) << 8;
	old >>= 8;
	*d |= (old & 0x0000FF00);
	old >>= 16;
	*d |= (old & 0x000000FF);
}

/*
 *	rev8
 *
 *	Function:	- reverse byte ordering of a 8-byte array
 *			- array is treated as 2 unsigned 4-byte integers
 *	Accepts:	- ptr source array
 *			- ptr destination array
 */
void
rev8(void *src, void *dest)
{
	uint4		old;
	char		*d;

	old = *((uint4 *) src + 1);
	d = (char *) dest + 4;

	rev4(src, (void *) d);
	rev4((void *) &old, dest);
}

/*
 *	revn
 *
 *	Function:	- reverse byte ordering of a multi-byte array
 *	Accepts:	- ptr source array
 *			- ptr destination array
 *			- # of bytes
 */
void
revn(void *src, void *dest, int nbytes)
{
	char		temp;
	char		*s, *d;

	s = (char *) src;
	d = (char *) dest + nbytes - 1;

	if (src == dest) {
/*
 * Same array, swap bytes from both ends.
 */
		for (nbytes /= 2; nbytes > 0; --nbytes, ++s, --d) {
			temp = *s; *s = *d; *d = temp;
		}
	}
	else {
/*
 * Different arrays, just copy the reversed order.
 * This assumes the arrays do not overlap.
 */
		for ( ; nbytes > 0; --nbytes, ++s, --d) {
			*d = *s;
		}
	}
}

/*
 *	mrev4
 *
 *	Function:	- reverse byte ordering of an array of 4-byte elements
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mrev4(void *array, int num)
{
	uint4		old;
	uint4		*d;

	d = (uint4 *) array;

	for ( ; num > 0; --num, ++d) {
		old = *d;
		*d = (old & 0x000000FF) << 16;
		*d = (*d | (old & 0x0000FF00)) << 8;
		old >>= 8;
		*d |= (old & 0x0000FF00);
		old >>= 16;
		*d |= (old & 0x000000FF);
	}
}

/*
 *	mrev8
 *
 *	Function:	- reverse byte ordering of an array of 8-byte elements
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mrev8(void *array, int num)
{
	uint4		old;
	uint4		tmp;
	uint4		*d;

	for (d = (uint4 *) array; num > 0; --num, ++d) {

		tmp = *d;
		
		old = *(d+1);
		*d = (old & 0x000000FF) << 16;
		*d = (*d | (old & 0x0000FF00)) << 8;
		old >>= 8;
		*d |= (old & 0x0000FF00);
		old >>= 16;
		*d |= (old & 0x000000FF);

		++d;
		*d = (tmp & 0x000000FF) << 16;
		*d = (*d | (tmp & 0x0000FF00)) << 8;
		tmp >>= 8;
		*d |= (tmp & 0x0000FF00);
		tmp >>= 16;
		*d |= (tmp & 0x000000FF);
	}
}

/*
 *	mrevn
 *
 *	Function:	- reverse byte ordering of an array of n-byte elements
 *	Accepts:	- ptr array
 *			- element size
 *			- # elements in array
 */
void
mrevn(void *array, int size, int num)
{
	char		*p, *s, *d;
	char		temp;
	int		nbytes;

	p = (char *) array;
	for ( ; num > 0; --num, p += size) {
/*
 * Swap bytes from both ends.
 */
		s = p;
		d = p + size - 1;
		for (nbytes = size / 2; nbytes > 0; --nbytes, ++s, --d) {
			temp = *s; *s = *d; *d = temp;
		}
	}
}

/*
 *	rw
 *
 *	Function:	- reverse word byte order (old Trollius call)
 *	Accepts:	- original word
 *	Returns:	- word with reversed byte order
 */
uint4
rw(uint4 oword)
{
	uint4		rword;

	rword = (oword & 0x000000FF) << 16;
	rword = (rword | (oword & 0x0000FF00)) << 8;
	oword >>= 8;
	rword |= (oword & 0x0000FF00);
	oword >>= 16;
	rword |= (oword & 0x000000FF);

	return(rword);
}
