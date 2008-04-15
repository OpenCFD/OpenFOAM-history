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
 *	$Id: rpi_usysv_req.h,v 1.2 2002/10/09 21:00:43 brbarret Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_RPI_USYSV_REQ_H
#define LAM_RPI_USYSV_REQ_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * usysv has none of its own request-specific data; use the same stuff
 * as tcp
 */

#include <rpi_tcp_req.h>

#endif /* LAM_SSI_RPI_USYSV_REQ_H */
