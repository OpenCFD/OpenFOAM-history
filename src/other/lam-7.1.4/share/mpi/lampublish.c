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
 * $Id: lampublish.c,v 1.2 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- LAM MPI name publishing
 *			- all MPI services are published on node 0
 */

#include <lam_config.h>
#include <app_mgmt.h>
#include <mpisys.h>
#include <rpisys.h>
#include <trreq.h>

/*
 *	lam_publish
 *
 *	Function:	- publish a service
 *	Accepts:	- service name
 *			- service port
 *	Returns:	- 0 or LAMERROR
 */
int
lam_publish(name, port)

char			*name;
char			*port;

{
	int4		id;			/* name publishing ID */

	id = (int4) ((lam_myproc->p_gps.gps_pid << 16)
			| lam_myproc->p_gps.gps_node);

	return(lam_rtrnamepub(0, id, name, port));
}

/*
 *	lam_unpublish
 *
 *	Function:	- un-publish a service
 *	Accepts:	- service name
 *	Returns:	- 0 or LAMERROR
 */
int
lam_unpublish(name)

char			*name;

{
	return(lam_rtrnamedel(0, name));
}

/*
 *	lam_getpublished
 *
 *	Function:	- get a published port
 *			- allocates storage for the port
 *	Accepts:	- service name
 *			- service port (out)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_getpublished(name, port)

char			*name;
char			**port;

{
	return(lam_rtrnameget(0, name, port));
}

/*
 *	lam_nukepublished
 *
 *	Function:	- nuke all names published by caller
 */
void
lam_nukepublished()

{
	int4		id;			/* name publishing ID */

	id = (int4) ((lam_myproc->p_gps.gps_pid << 16)
			| lam_myproc->p_gps.gps_node);

	lam_rtrnamesweep(0, id);
}
