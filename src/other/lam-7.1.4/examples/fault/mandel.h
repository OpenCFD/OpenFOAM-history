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
 * $Id: mandel.h,v 6.6 2003/04/23 22:32:12 jsquyres Exp $
 *
 *	Function:	- Mandelbrot demo constants and templates
 */

#ifndef _MANDEL
#define _MANDEL

/*
 * constants
 */
#define	MAX_ITERATIONS	1000

#define	DIETAG		1
#define WORKTAG		2
#define REPLYTAG	3

/*
 * structures
 */
typedef struct {
	float		r;
	float		i;
} complex;

#endif	/* _MANDEL */
