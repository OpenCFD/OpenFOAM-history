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
 * $Id: getworkdir.c,v 6.6 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- get the user's current working directory
 *			- dynamically allocated space
 *			- use old getwd() for efficiency if possible
 *			- hopefully our MAX is bigger than everybody's MAX
 *			  (someday a _real_ standard will emerge!)
 *	Returns:	- working directory or NULL
 */

#include <lam_config.h>

#include <stdlib.h>
#include <unistd.h>

#include <lam.h>
#include <laminternal.h>

#define PATHSIZE	2048


char *
getworkdir(void)
{
	char		*cwd;		/* working directory name */
/*
 * Allocate a hopefully large enough buffer;
 */
	if ((cwd = malloc((unsigned) PATHSIZE)) == 0) {
		return((char *) 0);
	}
/*
 * Get the working directory.  Work around SunOS bug.
 */
#if LAM_HAVE_GETCWD && !defined(SUNOS)
	if (getcwd(cwd, PATHSIZE)) {
#else
	if (getwd(cwd)) {
#endif
		return(cwd);
	} else {
		free(cwd);
		return((char *) 0);
	}
}
