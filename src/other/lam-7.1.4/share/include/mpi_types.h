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
 *	$Id: mpi_types.h,v 6.13 2002/10/21 04:37:07 brbarret Exp $
 *
 *	Function:	- type conversions for ANSI C types and alignment
 *			  checking
 */

#ifndef _MPI_TYPES_H
#define _MPI_TYPES_H

#include <lam_config.h>
#include <mpi.h>
#include <t_types.h>

#if LAM_WORDS_BIGENDIAN

/* 
 * We convert everything to LAM's network order, which is the same as
 * big endian.  So these are all no-ops.  
 */

#define	ttol_short(x, y)	(*((short *) (y)) = *((short *) (x)))
#define	ltot_short(x, y)	(*((short *) (y)) = *((short *) (x)))
#define	mttol_short(x, l)
#define	mltot_short(x, l)

#define	ttol_int(x, y)		(*((int *) (y)) = *((int *) (x)))
#define	ltot_int(x, y)		(*((int *) (y)) = *((int *) (x)))
#define	mttol_int(x, l)
#define	mltot_int(x, l)

#define	ttol_wchar(x, y)	(*((wchar_t *) (y)) = *((wchar_t *) (x)))
#define	ltot_wchar(x, y)	(*((wchar_t *) (y)) = *((wchar_t *) (x)))
#define	mttol_wchar(x, l)
#define	mltot_wchar(x, l)

#define	ttol_long(x, y)		(*((long *) (y)) = *((long *) (x)))
#define	ltot_long(x, y)		(*((long *) (y)) = *((long *) (x)))
#define	mttol_long(x, l)
#define	mltot_long(x, l)

#if LAM_SIZEOF_LONG_LONG
#define	ttol_longlong(x, y)	(*((long long *) (y)) = *((long long *) (x)))
#define	ltot_longlong(x, y)	(*((long long *) (y)) = *((long long *) (x)))
#define	mttol_longlong(x, l)
#define	mltot_longlong(x, l)
#endif

#define	ttol_float(x, y)	(*((float *) (y)) = *((float *) (x)))
#define	ltot_float(x, y)	(*((float *) (y)) = *((float *) (x)))
#define	mttol_float(x, l)
#define	mltot_float(x, l)

#define	ttol_double(x, y)	(*((double *) (y)) = *((double *) (x)))
#define	ltot_double(x, y)	(*((double *) (y)) = *((double *) (x)))
#define	mttol_double(x, l)
#define	mltot_double(x, l)

#if LAM_SIZEOF_LONG_DOUBLE
#define ttol_longdbl(x, y)	(*((LAM_LONG_DOUBLE*) (y)) = *((LAM_LONG_DOUBLE*) (x)))
#define ltot_longdbl(x, y)	(*((LAM_LONG_DOUBLE*) (y)) = *((LAM_LONG_DOUBLE*) (x)))
#define	mttol_longdbl(x, l)
#define	mltot_longdbl(x, l)
#endif

#else  /* not LAM_WORDS_BIGENDIAN */

/*
 * Have to switch endianess of everything...
 */

#if LAM_SIZEOF_SHORT == 2
#define	ttol_short(x, y)	revn(x, y, 2)
#define	ltot_short(x, y)	revn(x, y, 2)
#define	mttol_short(x, l)	mrevn(x, 2, l)
#define	mltot_short(x, l)	mrevn(x, 2, l)
#elif LAM_SIZEOF_SHORT == 4
#define	ttol_short(x, y)	rev4(x, y)
#define	ltot_short(x, y)	rev4(x, y)
#define	mttol_short(x, l)	mrev4(x, l)
#define	mltot_short(x, l)	mrev4(x, l)
#else
#error "Error: Unexpected size for short datatype.  Can not continue"
#endif

#if LAM_SIZEOF_INT == 4
#define	ttol_int(x, y)		rev4(x, y)
#define	ltot_int(x, y)		rev4(x, y)
#define	mttol_int(x, l)		mrev4(x, l)
#define	mltot_int(x, l)		mrev4(x, l)
#elif LAM_SIZEOF_INT == 8
#define	ttol_int(x, y)		rev8(x, y)
#define	ltot_int(x, y)		rev8(x, y)
#define	mttol_int(x, l)		mrev8(x, l)
#define	mltot_int(x, l)		mrev8(x, l)
#else
#error "Error: Unexpected size for int datatype.  Can not continue"
#endif

#if LAM_SIZEOF_WCHAR == 4
#define	ttol_wchar(x, y)	rev4(x, y)
#define	ltot_wchar(x, y)	rev4(x, y)
#define	mttol_wchar(x, l)	mrev4(x, l)
#define	mltot_wchar(x, l)	mrev4(x, l)
#elif LAM_SIZEOF_WCHAR
#define	ttol_wchar(x, y)	revn(x, y, LAM_SIZEOF_WCHAR)
#define	ltot_wchar(x, y)	revn(x, y, LAM_SIZEOF_WCHAR)
#define	mttol_wchar(x, l)	mrevn(x, LAM_SIZEOF_WCHAR, l)
#define	mltot_wchar(x, l)	mrevn(x, LAM_SIZEOF_WCHAR, l)
#else
/* I don't know how this could happen, but better safe than sorry */
#error "Error: Unexpected size for wchar datatype.  Can not continue"
#endif

#if LAM_SIZEOF_LONG == 4
#define	ttol_long(x, y)		rev4(x, y)
#define	ltot_long(x, y)		rev4(x, y)
#define	mttol_long(x, l)	mrev4(x, l)
#define	mltot_long(x, l)	mrev4(x, l)
#elif LAM_SIZEOF_LONG == 8
#define	ttol_long(x, y)		rev8(x, y)
#define	ltot_long(x, y)		rev8(x, y)
#define	mttol_long(x, l)	mrev8(x, l)
#define	mltot_long(x, l)	mrev8(x, l)
#elif LAM_SIZEOF_LONG
#define	ttol_long(x, y)		revn(x, y, LAM_SIZEOF_LONG)
#define	ltot_long(x, y)		revn(x, y, LAM_SIZEOF_LONG)
#define	mttol_long(x, l)	mrevn(x, LAM_SIZEOF_LONG, l)
#define	mltot_long(x, l)	mrevn(x, LAM_SIZEOF_LONG, l)
#else
/* I don't know how this could happen, but better safe than sorry */
#error "Error: Unexpected size for long datatype.  Can not continue"
#endif

/* Don't abort on this case, because we might not have long long */
#if LAM_SIZEOF_LONG_LONG == 8
#define	ttol_longlong(x, y)	rev8(x, y)
#define	ltot_longlong(x, y)	rev8(x, y)
#define	mttol_longlong(x, l)	mrev8(x, l)
#define	mltot_longlong(x, l)	mrev8(x, l)
#elif LAM_SIZEOF_LONG_LONG
#define	ttol_longlong(x, y)	revn(x, y, LAM_SIZEOF_LONG_LONG)
#define	ltot_longlong(x, y)	revn(x, y, LAM_SIZEOF_LONG_LONG)
#define	mttol_longlong(x, l)	mrevn(x, LAM_SIZEOF_LONG_LONG, l)
#define	mltot_longlong(x, l)	mrevn(x, LAM_SIZEOF_LONG_LONG, l)
#endif

#if LAM_SIZEOF_FLOAT == 4
#define	ttol_float(x, y)	rev4(x, y)
#define	ltot_float(x, y)	rev4(x, y)
#define	mttol_float(x, l)	mrev4(x, l)
#define	mltot_float(x, l)	mrev4(x, l)
#elif LAM_SIZEOF_FLOAT == 8
#define	ttol_float(x, y)	rev8(x, y)
#define	ltot_float(x, y)	rev8(x, y)
#define	mttol_float(x, l)	mrev8(x, l)
#define	mltot_float(x, l)	mrev8(x, l)
#elif LAM_SIZEOF_FLOAT
#define	ttol_float(x, y)	revn(x, y, LAM_SIZEOF_FLOAT)
#define	ltot_float(x, y)	revn(x, y, LAM_SIZEOF_FLOAT)
#define	mttol_float(x, l)	mrevn(x, LAM_SIZEOF_FLOAT, l)
#define	mltot_float(x, l)	mrevn(x, LAM_SIZEOF_FLOAT, l)
#else
/* I don't know how this could happen, but better safe than sorry */
#error "Error: Unexpected size for float datatype.  Can not continue"
#endif

#if LAM_SIZEOF_DOUBLE == 8
#define	ttol_double(x, y)	rev8(x, y)
#define	ltot_double(x, y)	rev8(x, y)
#define	mttol_double(x, l)	mrev8(x, l)
#define	mltot_double(x, l)	mrev8(x, l)
#elif LAM_SIZEOF_DOUBLE
#define	ttol_double(x, y)	revn(x, y, LAM_SIZEOF_DOUBLE)
#define	ltot_double(x, y)	revn(x, y, LAM_SIZEOF_DOUBLE)
#define	mttol_double(x, l)	mrevn(x, LAM_SIZEOF_DOUBLE, l)
#define	mltot_double(x, l)	mrevn(x, LAM_SIZEOF_DOUBLE, l)
#else
/* I don't know how this could happen, but better safe than sorry */
#error "Error: Unexpected size for double datatype.  Can not continue"
#endif

/* Don't abort on this case, because we might not have long long */
#if LAM_SIZEOF_LONG_DOUBLE == 8
#define	ttol_longdbl(x, y)	rev8(x, y)
#define	ltot_longdbl(x, y)	rev8(x, y)
#define	mttol_longdbl(x, l)	mrev8(x, l)
#define	mltot_longdbl(x, l)	mrev8(x, l)
#elif LAM_SIZEOF_LONG_DOUBLE
#define	ttol_longdbl(x, y)	revn(x, y, LAM_SIZEOF_LONG_DOUBLE)
#define	ltot_longdbl(x, y)	revn(x, y, LAM_SIZEOF_LONG_DOUBLE)
#define	mttol_longdbl(x, l)	mrevn(x, LAM_SIZEOF_LONG_DOUBLE, l)
#define	mltot_longdbl(x, l)	mrevn(x, LAM_SIZEOF_LONG_DOUBLE, l)
#endif

#endif  /* not LAM_WORDS_BIGENDIAN */

#ifdef __cplusplus
extern "C" {
#endif

extern void		lam_copyrev2 __ARGS((void *, void *, int));
extern void		lam_copyrev4 __ARGS((void *, void *, int));
extern void		lam_copyrev8 __ARGS((void *, void *, int));
extern void		lam_copyrevn __ARGS((void *, void *, int, int));

#ifdef __cplusplus
}
#endif

#endif /* _MPI_TYPES_H */
