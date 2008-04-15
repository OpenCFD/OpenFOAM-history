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
 *	$Id: portable.h,v 6.8 2002/10/09 20:57:25 brbarret Exp $
 *
 *	Function:	- Put some order into a world of chaos.
 *			- Anybody without an 8 bit byte can get lost.
 */

#ifndef _PORTABLE
#define _PORTABLE

#include <lam_config.h>
#include <limits.h>

/*
 * To const or not to const.
 */
#ifndef CONST
#if __STDC__ || defined(c_plusplus) || defined(__cplusplus)
#define CONST const
#else
#define CONST
#endif
#endif

/*
 * Can you believe that some compilers would default char to unsigned char?
 */
#ifndef SIGNED
#if __STDC__ || defined(c_plusplus) || defined(__cplusplus)
#define SIGNED signed
#else
#define SIGNED
#endif
#endif

/*
 * Path name separator.
 */
#ifndef DOS
#define STRDIR		'/'
#define STRSDIR		"/"
#else
#define STRDIR		'\\'
#define STRSDIR		"\\"
#endif

/*
 * Maximum size of a filename path.
 */
#if defined(PATH_MAX)
#define LAM_PATH_MAX	(PATH_MAX + 1)
#elif defined(_POSIX_PATH_MAX)
#define LAM_PATH_MAX	(_POSIX_PATH_MAX + 1)
#else
#define LAM_PATH_MAX	256
#endif

/*
 * Portable datatypes.
 */
#if LAM_SIZEOF_SHORT == 2
#if !LAM_HAVE_int2
typedef short			int2;
#endif
#if !LAM_HAVE_uint2
typedef unsigned short		uint2;
#endif
#elif LAM_SIZEOF_INT == 2
#if !LAM_HAVE_int2
typedef int			int2;
#endif
#if !LAM_HAVE_uint2
typedef unsigned int		uint2;
#endif
#endif

#if LAM_SIZEOF_INT == 4
#if !LAM_HAVE_int4
typedef int			int4;
#endif
#if !LAM_HAVE_uint4
typedef unsigned int		uint4;
#endif
#define stoi4			stoi
#define atoi4			atoi
#elif LAM_SIZEOF_SHORT == 4
#if !LAM_HAVE_int4
typedef short			int4;
#endif
#if !LAM_HAVE_uint4
typedef unsigned short		uint4;
#endif
#define stoi4			stos
#define atoi4			atos
#elif LAM_SIZEOF_LONG == 4
#if !LAM_HAVE_int4
typedef long			int4;
#endif
#if !LAM_HAVE_uint4
typedef unsigned long		uint4;
#endif
#define stoi4			stol
#define atoi4			atol
#endif

#if LAM_SIZEOF_LONG	== 8
#if !LAM_HAVE_int8
typedef	long			int8;
#endif
#if !LAM_HAVE_uint8
typedef unsigned long		uint8;
#endif
#elif LAM_SIZEOF_INT == 8
#if !LAM_HAVE_int8
typedef int			int8;
#endif
#if !LAM_HAVE_uint8
typedef unsigned int		uint8;
#endif
#elif LAM_SIZEOF_LONG_LONG == 8
#if !LAM_HAVE_int8
typedef long long		int8;
#endif
#if !LAM_HAVE_uint8
typedef unsigned long long	uint8;
#endif
#endif

#if !LAM_HAVE_float4
#if LAM_SIZEOF_FLOAT == 4
typedef float			float4;
#endif
#endif

#if !LAM_HAVE_float8
#if LAM_SIZEOF_FLOAT == 8
typedef float			float8;
#elif LAM_SIZEOF_DOUBLE == 8
typedef double			float8;
#endif
#endif

#define INT4_NIL	((int4) 0)
#define INT4_MAX	((int4) 0x7FFFFFFF)
#define INT4_MIN	((int4) 0x80000000)
#define INT4_LSB	((int4) 0xFF)		/* least significant byte */
#define INT4_LSN	((int4) 0x0F)		/* least significant nibble */
#define INT4_MSN	((int4) 0xF0)		/* most significant nibble */
#define	INT4_SIGN	((int4) 0x80000000)	/* sign bit of an int4 */

#define UINT4_MAX	((uint4) 0xFFFFFFFF)	/* maximum uint4 */

#endif
