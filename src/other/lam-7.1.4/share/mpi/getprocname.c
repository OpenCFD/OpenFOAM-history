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
 * $Id: getprocname.c,v 6.6 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get the host name
 *	Accepts:	- name buffer (returned value)
 *			- ptr name length (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

#include <laminternal.h>
#include <lam_network.h>

/*@
  MPI_Get_processor_name - Gets the name of the processor

Output Parameters:
+ name - A unique specifier for the actual (as opposed to virtual) node. 
- len - Length (in characters) of the name 

Notes:

LAM/MPI returns the value from 'gethostname'(3).

.N fortran

.N Errors
.N MPI_ERR_OTHER

.N ACK
@*/
int MPI_Get_processor_name(char *name, int *len)
{
	lam_initerr();
	lam_setfunc(BLKMPIGETPNAME);

	if (lam_gethostname(name, MPI_MAX_PROCESSOR_NAME - 1)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIGETPNAME,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	} else {
		name[MPI_MAX_PROCESSOR_NAME - 1] = '\0';
		*len = strlen(name);
	}

	lam_resetfunc(BLKMPIGETPNAME);
	return(MPI_SUCCESS);
}
