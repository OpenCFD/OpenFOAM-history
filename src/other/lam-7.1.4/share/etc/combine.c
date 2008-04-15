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
 * $Id: combine.c,v 6.6 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- functions to combine arrays of data
 */

#include <string.h>

#include <net.h>
#include <portable.h>
#include <typical.h>
#include <etc_misc.h>

/*
 *	cbn_add
 *
 *	Function:	- combine by adding
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_add(int4 flags, int4 length, char *msg1, char *msg2)
{
	float4		*pf1, *pf2;
	float8		*pd1, *pd2;
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 += *pi2;
		}
		break;

	case DFLT4MSG:
		length /= sizeof(float4);
		pf1 = (float4 *) msg1;
		pf2 = (float4 *) msg2;

		for ( ; length > 0; --length, ++pf1, ++pf2) {
			*pf1 += *pf2;
		}
		break;

	case DFLT8MSG:
		length /= sizeof(float8);
		pd1 = (float8 *) msg1;
		pd2 = (float8 *) msg2;

		for ( ; length > 0; --length, ++pd1, ++pd2) {
			*pd1 += *pd2;
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 += *msg2;
		}
		break;
	}
}

/*
 *	cbn_and
 *
 *	Function:	- combine by ANDing
 *			- works only for int4 and char data types
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_and(int4 flags, int4 length, char *msg1, char *msg2)
{
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 &= *pi2;
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 &= *msg2;
		}
		break;

	case DFLT4MSG:
	case DFLT8MSG:
/*
 * You must be kidding!
 */
		break;
	}
}

/*
 *	cbn_err
 *
 *	Function:	- combine error codes (pass non-zero value)
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_err(int4 flags, int4 length, char *msg1, char *msg2)
{
	float4		*pf1, *pf2;
	float8		*pd1, *pd2;
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		if (length > 0) {
			*pi1 = *pi1 ? *pi1 : *pi2;
		}
		break;

	case DFLT4MSG:
		length /= sizeof(float4);
		pf1 = (float4 *) msg1;
		pf2 = (float4 *) msg2;

		if (length > 0) {
			*pf1 = *pf1 ? *pf1 : *pf2;
		}
		break;

	case DFLT8MSG:
		length /= sizeof(float8);
		pd1 = (float8 *) msg1;
		pd2 = (float8 *) msg2;

		if (length > 0) {
			*pd1 = *pd1 ? *pd1 : *pd2;
		}
		break;

	case DRAWMSG:
		*msg1 = *msg1 ? *msg1 : *msg2;
		break;
	}
}

/*
 *	cbn_max
 *
 *	Function:	- combine by MAXing
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_max(int4 flags, int4 length, char *msg1, char *msg2)
{
	float4		*pf1, *pf2;
	float8		*pd1, *pd2;
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 = LAM_max(*pi1, *pi2);
		}
		break;

	case DFLT4MSG:
		length /= sizeof(float4);
		pf1 = (float4 *) msg1;
		pf2 = (float4 *) msg2;

		for ( ; length > 0; --length, ++pf1, ++pf2) {
			*pf1 = LAM_max(*pf1, *pf2);
		}
		break;

	case DFLT8MSG:
		length /= sizeof(float8);
		pd1 = (float8 *) msg1;
		pd2 = (float8 *) msg2;

		for ( ; length > 0; --length, ++pd1, ++pd2) {
			*pd1 = LAM_max(*pd1, *pd2);
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 = LAM_max(*msg1, *msg2);
		}
		break;
	}
}

/*
 *	cbn_min
 *
 *	Function:	- combine by MINing
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_min(int4 flags, int4 length, char *msg1, char *msg2)
{
	float4		*pf1, *pf2;
	float8		*pd1, *pd2;
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 = LAM_min(*pi1, *pi2);
		}
		break;

	case DFLT4MSG:
		length /= sizeof(float4);
		pf1 = (float4 *) msg1;
		pf2 = (float4 *) msg2;

		for ( ; length > 0; --length, ++pf1, ++pf2) {
			*pf1 = LAM_min(*pf1, *pf2);
		}
		break;

	case DFLT8MSG:
		length /= sizeof(float8);
		pd1 = (float8 *) msg1;
		pd2 = (float8 *) msg2;

		for ( ; length > 0; --length, ++pd1, ++pd2) {
			*pd1 = LAM_min(*pd1, *pd2);
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 = LAM_min(*msg1, *msg2);
		}
		break;
	}
}

/*
 *	cbn_mul
 *
 *	Function:	- combine by multiplying
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_mul(int4 flags, int4 length, char *msg1, char *msg2)
{
	float4		*pf1, *pf2;
	float8		*pd1, *pd2;
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 *= *pi2;
		}
		break;

	case DFLT4MSG:
		length /= sizeof(float4);
		pf1 = (float4 *) msg1;
		pf2 = (float4 *) msg2;

		for ( ; length > 0; --length, ++pf1, ++pf2) {
			*pf1 *= *pf2;
		}
		break;

	case DFLT8MSG:
		length /= sizeof(float8);
		pd1 = (float8 *) msg1;
		pd2 = (float8 *) msg2;

		for ( ; length > 0; --length, ++pd1, ++pd2) {
			*pd1 *= *pd2;
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 *= *msg2;
		}
		break;
	}
}

/*
 *	cbn_or
 *
 *	Function:	- combine by ORing
 *			- works only for int4 and char data types
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_or(int4 flags, int4 length, char *msg1, char *msg2)
{
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 |= *pi2;
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 |= *msg2;
		}
		break;

	case DFLT4MSG:
	case DFLT8MSG:
/*
 * You must be kidding!
 */
		break;
	}
}

/*
 *	cbn_xor
 *
 *	Function:	- combine by exclusive ORing
 *			- works only for int4 and char data types
 *	Accepts:	- message flags
 *			- length of messages (in bytes)
 *			- first message
 *			- second message
 */	
void
cbn_xor(int4 flags, int4 length, char *msg1, char *msg2)
{
	int4		*pi1, *pi2;

	if (length <= 0) {
		return;
	}
/*
 * Combine according to data type.
 */
	switch(flags) {

	case DINT4MSG:
		length /= sizeof(int4);
		pi1 = (int4 *) msg1;
		pi2 = (int4 *) msg2;

		for ( ; length > 0; --length, ++pi1, ++pi2) {
			*pi1 ^= *pi2;
		}
		break;

	case DRAWMSG:
		for ( ; length > 0; --length, ++msg1, ++msg2) {
			*msg1 ^= *msg2;
		}
		break;

	case DFLT4MSG:
	case DFLT8MSG:
/*
 * You must be kidding!
 */
		break;
	}
}

/*
 *	cbn_cat
 *
 *	Function:	- combine by concatenating
 *	Accepts:	- first buffer
 *			- first buffer length
 *			- # bytes full in first buffer
 *			- second buffer
 *			- second buffer length
 *	Returns:	- # bytes full in first buffer
 */	
int4
cbn_cat(char *buf1, int4 len1, int4 nfull, char *buf2, int4 len2)
{
	int4		nbytes;

	nbytes = len1 - nfull;
	if (nbytes > len2) {
		nbytes = len2;
	}

	if (nbytes > 0) {
		memcpy(buf1 + nfull, buf2, nbytes);
	}

	return(nfull + nbytes);
}
