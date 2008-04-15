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
 * $Id: namelook.c,v 6.9 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- get port name associated with service name
 *	Accepts:	- service name
 *			- info
 *			- port name (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <etc_misc.h>


/*@
    MPI_Lookup_name - Lookup a previously published name

Input Parameters:
+ service_name - a service name
- info - implementation-specific information

Output Parameter:
. port_name - a port name

Notes: 

Currently, LAM/MPI ignores any info keys that are passed to this
function.  As such, info keys specific to a different implementation
can safely be passed to this function.  Additionally, the MPI constant
'MPI_INFO_NULL' can also be used.

The scope of the published name is the LAM universe -- MPI processes
running on any node included from the initial lamboot are capable of
retrieving a name published by 'MPI_Publish_name'.  Service names stay
in existence until they are either manually removed with
'MPI_Unpublish_name' or cleaned with the 'lamclean' command.  It is a
common mistake to forget to 'MPI_Unpublish_name' at the end of a run,
and therefore leave published names in the LAM runtime environment.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_NAME

.seealso: MPI_Publish_name, MPI_Unpublish_name

.N WEB
@*/
int
MPI_Lookup_name(char *service_name, MPI_Info info, char *port_name)
{
	char		*port_buf;
	
	lam_initerr();
	lam_setfunc(BLKMPILOOKUPNAME);
/*
 * Check arguments.
 */
	if (port_name == 0 || service_name == 0) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, EINVAL, ""));
	}
/*
 * Get port name.
 */
	if (lam_getpublished(service_name, &port_buf)) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_NAME, errno, "")); 
	}

	lam_strncpy(port_name, port_buf, MPI_MAX_PORT_NAME);
	free(port_buf);

        lam_resetfunc(BLKMPILOOKUPNAME);
	return(MPI_SUCCESS);
}
