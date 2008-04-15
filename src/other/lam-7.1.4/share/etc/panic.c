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
 *	$Id: panic.c,v 1.3 2003/09/27 05:16:17 brbarret Exp $
 *
 *	Function:	- exits without using the kernel
 *	Accepts:	- error message prefix
 */

#include <stdlib.h>

#include <terror.h>

void
lampanic(errmsg)

char			*errmsg;

{
	lam_perror(errmsg);

	if (ELOW <= errno && errno <= EHIGH) {
		errno = LAM_EEXIT;
	}

	exit(errno);
}

