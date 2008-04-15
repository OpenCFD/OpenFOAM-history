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
 * $Id: portclose.c,v 6.8 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- close an MPI port
 *	Accepts:	- port name
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <etc_misc.h>


/*@
   MPI_Close_port - Close a server port

Input Parameters:
. port_name - port name

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_ERR_ARG
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso MPI_Comm_accept(3), MPI_Comm_connect(3), MPI_Comm_disconnect(3), MPI_Open_port(3)

.N WEB
@*/
int
MPI_Close_port(char *port_name)
{
	struct _port	search;			/* search key for port */
	struct _port	*port;			/* found port */

	lam_initerr();
	lam_setfunc(BLKMPICLOSEPORT);

	if (strlen(port_name) > LAM_PORTLEN) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICLOSEPORT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Find the port.
 */
	lam_strncpy(search.prt_name, port_name, LAM_PORTLEN);

	port = al_find(lam_ports, &search);
	if (port == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICLOSEPORT,
					lam_mkerr(MPI_ERR_OTHER, ENOENT)));
	}
/*
 * close it
 */
	if (lam_port_close(port->prt_num)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICLOSEPORT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	if (al_delete(lam_ports, port)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICLOSEPORT,
					lam_mkerr(MPI_ERR_INTERN, EINVAL)));
	}

        lam_resetfunc(BLKMPICLOSEPORT);
	return(MPI_SUCCESS);
}
