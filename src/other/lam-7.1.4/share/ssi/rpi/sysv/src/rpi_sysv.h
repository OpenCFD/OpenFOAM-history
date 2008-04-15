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
 *	$Id: rpi_sysv.h,v 1.2 2002/10/09 21:00:34 brbarret Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_RPI_SYSV_H
#define LAM_RPI_SYSV_H

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <mpi.h>
#include <lam-ssi-rpi-sysv.h>
#include <mpisys.h>
#include <rpi_sysv_macros.h>
#include <rpi_sysv_req.h>
#include <rpi_sysv_proc.h>

#endif /* LAM_SSI_RPI_SYSV_H */
