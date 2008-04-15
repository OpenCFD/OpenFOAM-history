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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	RBD/NJN
 *
 *	$Id: ieee.c,v 1.4 2003/10/11 19:15:35 brbarret Exp $
 * 
 *	Function:	- change floating point to/from IEEE big-endian
 *			- handles floats, doubles and long doubles
 *			- requires XDR support for floating point numbers
 *			- OTB version
 */

#include <lam_config.h>

#if !HAVE_IEEE_FP
#include <stdio.h>
#if defined(HAVE_RPC_TYPES_H) && HAVE_RPC_TYPES_H
#include <rpc/types.h>
#endif
#if defined(HAVE_RPC_XDR_H) && HAVE_RPC_XDR_H
#include <rpc/xdr.h>
#endif

/*
 *	ltoieeef
 *
 *	Function:	- change a float from local to IEEE big-endian
 *	Accepts:	- ptr source float
 *			- ptr destination area
 */
void
ltoieeef(src, dest)

float			*src;
void			*dest;

{
	float		orig;
	XDR		xdrs;

	orig = *src;
	xdrmem_create(&xdrs, (char *) dest, sizeof(float), XDR_ENCODE);

	(void) xdr_float(&xdrs, &orig);
	xdr_destroy(&xdrs);
}

/*
 *	ltoieeed
 *
 *	Function:	- change a double from local to IEEE big-endian
 *	Accepts:	- ptr source double
 *			- ptr destination area
 */
void
ltoieeed(src, dest)

double			*src;
void			*dest;

{
	double		orig;
	XDR		xdrs;

	orig = *src;
	xdrmem_create(&xdrs, (char *) dest, sizeof(double), XDR_ENCODE);

	(void) xdr_double(&xdrs, &orig);
	xdr_destroy(&xdrs);
}

#if LAM_SIZEOF_LONG_DOUBLE == 16
/*
 *	ltoieeeld
 *
 *	Function:	- change a long double from local to IEEE big-endian
 *	Accepts:	- ptr source long double
 *			- ptr destination area
 */
void
ltoieeeld(src, dest)

LAM_LONG_DOUBLE		*src;
void			*dest;

{
	LAM_LONG_DOUBLE	orig;
	XDR		xdrs;

	orig = *src;
	xdrmem_create(&xdrs, (char *) dest, sizeof(LAM_LONG_DOUBLE), 
		      XDR_ENCODE);

	(void) xdr_quadruple(&xdrs, &orig);
	xdr_destroy(&xdrs);
}
#endif

/*
 *	ieeetolf
 *
 *	Function:	- change a float from IEEE big-endian to local
 *	Accepts:	- ptr source area
 *			- ptr destination float
 */
void
ieeetolf(src, dest)

void			*src;
float			*dest;

{
	float		result;
	XDR		xdrs;

	xdrmem_create(&xdrs, (char *) src, sizeof(float), XDR_DECODE);

	(void) xdr_float(&xdrs, &result);
	*dest = result;

	xdr_destroy(&xdrs);
}

/*
 *	ieeetold
 *
 *	Function:	- change a double from IEEE big-endian to local
 *	Accepts:	- ptr source area
 *			- ptr destination double
 */
void
ieeetold(src, dest)

void			*src;
double			*dest;

{
	double		result;
	XDR		xdrs;

	xdrmem_create(&xdrs, (char *) src, sizeof(double), XDR_DECODE);

	(void) xdr_double(&xdrs, &result);
	*dest = result;

	xdr_destroy(&xdrs);
}

#if LAM_SIZEOF_LONG_DOUBLE == 16
/*
 *	ieeetolld
 *
 *	Function:	- change a long double from IEEE big-endian to local
 *	Accepts:	- ptr source area
 *			- ptr destination long double
 */
void
ieeetolld(src, dest)

void			*src;
LAM_LONG_DOUBLE		*dest;

{
	LAM_LONG_DOUBLE	result;
	XDR		xdrs;

	xdrmem_create(&xdrs, (char *) src, sizeof(LAM_LONG_DOUBLE), 
		      XDR_DECODE);

	(void) xdr_quadruple(&xdrs, &result);
	*dest = result;

	xdr_destroy(&xdrs);
}
#endif

/*
 *	mltoieeef
 *
 *	Function:	- change array of floats from local to IEEE big-endian
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mltoieeef(array, num)

float			*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ltoieeef(array, (void *) array);
	}
}

/*
 *	mieeetolf
 *
 *	Function:	- change array of floats from IEEE big-endian to local
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mieeetolf(array, num)

float			*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ieeetolf((void *) array, array);
	}
}

/*
 *	mltoieeed
 *
 *	Function:	- change array of doubles from local to IEEE big-endian
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mltoieeed(array, num)

double			*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ltoieeed(array, (void *) array);
	}
}

/*
 *	mieeetold
 *
 *	Function:	- change array of doubles from IEEE big-endian to local
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mieeetold(array, num)

double			*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ieeetold((void *) array, array);
	}
}

#if LAM_SIZEOF_LONG_DOUBLE == 16
/*
 *	mieeetolld
 *
 *	Function:	- change array of long doubles from IEEE big-endian
 *			  to local
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mieeetolld(array, num)

LAM_LONG_DOUBLE		*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ieeetolld((void *) array, array);
	}
}

/*
 *	mltoieeeld
 *
 *	Function:	- change array of long doubles from local
 *			  to IEEE big-endian
 *	Accepts:	- ptr array
 *			- # elements in array
 */
void
mltoieeeld(array, num)

LAM_LONG_DOUBLE		*array;
int			num;

{
	for ( ; num > 0; --num, ++array) {
		ltoieeeld(array, (void *) array);
	}
}
#endif
#endif
