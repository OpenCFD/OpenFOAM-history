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
 *	$Id: kenter.c,v 6.6 2003/01/13 19:17:43 jsquyres Exp $
 * 
 *	Function:	- calls kinit() and lpattach()
 *	Accepts:	- program name
 *			- scheduling priority
 *	Returns:	- 0 or ERROR
 */

#include <kreq.h>
#include <preq.h>
#include <portable.h>
#include <typical.h>


int
kenter(const char *name, int priority)
{
	if (kinit(priority)) {
		return(ERROR);
	}

	return(lpattach(name));
}
