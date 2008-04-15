/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ktrace.h,v 6.5 2004/02/04 03:06:21 jsquyres Exp $
 *
 * Function:	- constants and templates associated with the
 *			  runtime trace system
 */

#ifndef _KTRACE
#define _KTRACE

#include <lam_config.h>
#include <portable.h>

/*
 * trace header
 */
struct trhead {
	char		*trh_buf;	/* trace buffer ptr */
	int4		trh_length;	/* trace buffer length */
};

/*
 * constants
 */
#define TRHMAX		4		/* TRHMAX * MAXNMSGLEN = len limit */
#define TRHQUAN		2		/* TRHQUAN * MAXNMSGLEN = alloc hunk */

#ifdef __cplusplus
extern "C" {
#endif

extern char		*lam_trpoint __ARGS((int4 size));
extern void		lam_trexit __ARGS((void));
extern void		lam_trinit __ARGS((void));
extern void		lam_trunload __ARGS((int));

#ifdef __cplusplus
}
#endif

#endif	/* _KTRACE */
