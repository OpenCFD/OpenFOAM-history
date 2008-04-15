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
 *	GDB
 *
 *	$Id: fail.c,v 1.2 2002/10/09 20:57:01 brbarret Exp $
 *
 *	Function:	- exits using the kernel
 *	Accepts:	- error message prefix
 */

#include <kreq.h>
#include <terror.h>

void
lamfail(errmsg)

char			*errmsg;

{
	lam_perror(errmsg);

	if (ELOW <= errno && errno <= EHIGH) {
		errno = LAM_EEXIT;
	}

	kexit(errno);
}
