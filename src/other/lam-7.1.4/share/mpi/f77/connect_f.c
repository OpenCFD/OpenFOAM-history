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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: connect_f.c,v 6.5 2002/10/09 20:59:02 brbarret Exp $
 *
 *	Function:	- MPI_Comm_connect F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_comm_connect_(port, info, root, comm, icomm, ierr, n)

char			*port;
int			*info, *root, *comm, *icomm, *ierr;
int			n;

{
	MPI_Comm	intercomm;
	char		*port_name;

	port_name = lam_F2C_string(port, n);
	if (port_name == 0) {
		lam_setfunc_m(BLKMPICOMMCONNECT);
		*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr = MPI_Comm_connect(port_name, GETHDL(*info),
				*root, GETHDL(*comm), &intercomm);
	
	if (lam_F_make_hdl(icomm, ierr, intercomm, BLKMPICOMMCONNECT)) {
		intercomm->c_f77handle = *icomm;
	}

	free(port_name);
}
