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
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: asc_workdir.c,v 6.2 2002/10/09 20:56:58 brbarret Exp $
 *
 *	Function:	- sets working dir for an application schema
 *			- LAM specific
 *	Accepts:	- parsed application schema
 *			- working dir
 *	Returns:	- 0 or LAMERROR
 */

#include <string.h>

#include "all_list.h"
#include "app_schema.h"

int
asc_workdir(LIST *applist, char *wrkdir)
{
    struct aschema *p;			/* current app process */
/*
 * Set workdir in each entry in the schema.
 */
    p = al_top(applist);
    while (p) {
	if (!p->asc_env->ape_wrkdir) {
	    p->asc_env->ape_wrkdir = strdup(wrkdir);

	    if (!p->asc_env->ape_wrkdir)
		return(LAMERROR);
	}

	p = al_next(applist, p);
    }

    return(0);
}
