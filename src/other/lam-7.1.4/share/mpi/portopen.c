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
 * $Id: portopen.c,v 6.10 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- open an MPI port
 *	Accepts:	- info
 *			- port name (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <sfh.h>

#include <string.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>
#include <etc_misc.h>

/*@
   MPI_Open_port - Establish a server port for client MPI connections

Input Parameters:
+ info - extra information handle
- port_name - newly established port

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso MPI_Close_port(3), MPI_Comm_accept(3), MPI_Comm_connect(3), MPI_Comm_disconnect(3)

.N WEB
@*/
int
MPI_Open_port(MPI_Info info, char* port_name)
{
	struct _port	port;			/* new port */
	int		port_num;		/* port number */

	lam_initerr();
	lam_setfunc(BLKMPIOPENPORT);
/*
 * Open a port.
 */
	if ((port_num = lam_port_open()) < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	LAM_ZERO_ME(port);
	port.prt_num = port_num;
/*
 * Add port to list of open ports.
 */
	sprintf(port.prt_name, "n%d:i%d:%d", lam_myproc->p_gps.gps_node,
		lam_myproc->p_gps.gps_idx, port_num);
	
	if (al_find(lam_ports, &port) != 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
					lam_mkerr(MPI_ERR_INTERN, EINVAL)));
	}
	if (al_insert(lam_ports, &port) == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Set return value.  The MPI_MAX_PORT_NAME probably isn't necessary, but
 * hey, let's be safe.
 */
	lam_strncpy(port_name, port.prt_name, MPI_MAX_PORT_NAME);
	
        lam_resetfunc(BLKMPIOPENPORT);
	return(MPI_SUCCESS);
}
