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
 *      Copyright 1996 The Ohio State University
 *      GDB/RBD
 *
 *	$Id: TRIL.c,v 6.5 2002/10/09 21:00:46 brbarret Exp $
 *
 *	Function:	- f2c F77 interface for libtrillium functions
 */

#include <stdlib.h>

#include <lam_config.h>

#include <portable.h>
#include <preq.h>
#include <terror.h>

/*
 * external functions
 */
extern char		*f2cstring();

/*
 * F77/C interface
 */
#if LAM_FORTRANNOUNDERSCORE
#define rpwt_		rpwt
#define rpldgo_		rpldgo
#endif

/*
 * simple wrappers
 */
void rpwt_(n, p, s, ie) int4 *n, *p, *s, *ie;
		{ *ie = rpwait(n, p, s) ? errno : 0; }

/*
 * thicker wrappers
 */
void
rpldgo_(srcnode, destnode, filename, flags, pid, idx, ierror, lstr)

int4			*srcnode;
int4			*destnode;
char			*filename;
int4			*flags;
int4			*pid;
int4			*idx;
int4			*ierror;
unsigned int		lstr;

{
	char		*argv[2];
	char		*cstr;
	int		r;

	if ((cstr = f2cstring(filename, lstr)) == 0) {
		*ierror = errno;
		return;
	}

	argv[0] = cstr;
	argv[1] = 0;

	r = rploadgo(*srcnode, *destnode, cstr, *flags, argv, pid, idx);
	*ierror = r ? errno : 0;

	free(cstr);
}
