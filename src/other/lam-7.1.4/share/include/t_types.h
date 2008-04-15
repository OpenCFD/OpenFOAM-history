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
 *	$Id: t_types.h,v 6.8 2002/10/21 04:37:07 brbarret Exp $
 *
 *	Function:	- defines type conversions
 */

#ifndef _T_TYPES
#define _T_TYPES

#include <lam_config.h>
#include <portable.h>

/*
 * Integer conversions.
 */
#if LAM_WORDS_BIGENDIAN

#define ltot(x)		(x)
#define ttol(x)		(x)
#define ltoti2(x, y)	*((uint2 *) (y)) = *((uint2 *) (x))
#define ttoli2(x, y)	*((uint2 *) (y)) = *((uint2 *) (x))
#define ltoti4(x, y)	*((uint4 *) (y)) = *((uint4 *) (x))
#define ttoli4(x, y)	*((uint4 *) (y)) = *((uint4 *) (x))
#if (LAM_SIZEOF_LONG_LONG == 8) || (LAM_SIZEOF_LONG == 8) || (LAM_SIZEOF_INT == 8)
#define ltoti8(x, y)	*((uint8 *) (y)) = *((uint8 *) (x))
#define ttoli8(x, y)	*((uint8 *) (y)) = *((uint8 *) (x))
#endif
#define mltoti2(x, l)
#define mttoli2(x, l)
#define mltoti4(x, l)
#define mttoli4(x, l)
#define mltoti8(x, l)
#define mttoli8(x, l)


#else /* not LAM_WORDS_BIGENDIAN */

#define ltot(x)		rw(x)
#define ttol(x)		rw(x)
#define ltoti2(x, y)	revn(x, y, 2)
#define ttoli2(x, y)	revn(x, y, 2)
#define ltoti4(x, y)	rev4(x, y)
#define ttoli4(x, y)	rev4(x, y)
#define ltoti8(x, y)	rev8(x, y)
#define ttoli8(x, y)	rev8(x, y)
#define mltoti2(x, l)	mrevn(x, 2, l)
#define mttoli2(x, l)	mrevn(x, 2, l)
#define mltoti4(x, l)	mrev4(x, l)
#define mttoli4(x, l)	mrev4(x, l)
#define mltoti8(x, l)	mrev8(x, l)
#define mttoli8(x, l)	mrev8(x, l)

#endif /* not LAM_WORDS_BIGENDIAN */

/*
 * Floating point conversions.
 */
#if HAVE_IEEE_FP

#if LAM_WORDS_BIGENDIAN

#define ltotf4(x, y)	*((float4 *) (y)) = *((float4 *) (x))
#define ttolf4(x, y)	*((float4 *) (y)) = *((float4 *) (x))
#define ltotf8(x, y)	*((float8 *) (y)) = *((float8 *) (x))
#define ttolf8(x, y)	*((float8 *) (y)) = *((float8 *) (x))
#define mltotf4(x, l)
#define mttolf4(x, l)
#define mltotf8(x, l)
#define mttolf8(x, l)

#else /* not LAM_WORDS_BIGENDIAN */

#define ltotf4(x, y)	rev4(x, y)
#define ttolf4(x, y)	rev4(x, y)
#define ltotf8(x, y)	rev8(x, y)
#define ttolf8(x, y)	rev8(x, y)
#define mltotf4(x, l)	mrev4(x, l)
#define mttolf4(x, l)	mrev4(x, l)
#define mltotf8(x, l)	mrev8(x, l)
#define mttolf8(x, l)	mrev8(x, l)

#endif /* not LAM_WORDS_BIGENDIAN */

#else /* not HAVE_IEEE_FP */

#define ltotf4(x, y)	ltoieeef(x, y)
#define ttolf4(x, y)	ieeetolf(x, y)
#define ltotf8(x, y)	ltoieeef(x, y)
#define ttolf8(x, y)	ieeetolf(x, y)
#define mltotf4(x, l)	mltoieeef(x, l)
#define mttolf4(x, l)	mieeetolf(x, l)
#define mltotf8(x, l)	mltoieeef(x, l)
#define mttolf8(x, l)	mieeetolf(x, l)

#endif /* not HAVE_IEEE_FP */

#ifdef __cplusplus
extern "C" {
#endif

extern uint4		rw __ARGS((uint4 oword));
extern void		rev4 __ARGS((void *src, void *dest));
extern void		rev8 __ARGS((void *src, void *dest));
extern void		revn __ARGS((void *src, void *dest, int size));
extern void		mrev4 __ARGS((void *array, int num));
extern void		mrev8 __ARGS((void *array, int num));
extern void		mrevn __ARGS((void *array, int size, int num));
extern void		ltoieeef __ARGS((float *src, void *dest));
extern void		ltoieeed __ARGS((double *src, void *dest));
extern void		ieeetolf __ARGS((void *src, float *dest));
extern void		ieeetold __ARGS((void *src, double *dest));
extern void		mltoieeef __ARGS((float *array, int num));
extern void		mltoieeed __ARGS((double *array, int num));
extern void		mieeetolf __ARGS((float *array, int num));
extern void		mieeetold __ARGS((double *array, int num));

#if LAM_SIZEOF_LONG_DOUBLE
extern void		ltoieeeld __ARGS((LAM_LONG_DOUBLE *src, void *dest));
extern void		ieeetolld __ARGS((void *src, LAM_LONG_DOUBLE *dest));
extern void		mltoieeeld __ARGS((LAM_LONG_DOUBLE *array, int num));
extern void		mieeetolld __ARGS((LAM_LONG_DOUBLE *array, int num));
#endif

#ifdef __cplusplus
}
#endif

#endif /* _T_TYPES */
