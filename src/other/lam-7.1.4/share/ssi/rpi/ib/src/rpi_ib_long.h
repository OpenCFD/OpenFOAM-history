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
 * $Id: rpi_ib_long.h,v 1.2 2003/11/27 23:09:40 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_IB_LONG_H
#define LAM_SSI_RPI_IB_LONG_H

#include <lam-ssi-rpi-ib-config.h>

#include <mpisys.h>
#include <rpisys.h>


/*
 * public functions
 */

/*
 * <SENDER> functions
 */
int lam_ssi_rpi_ib_long_send(MPI_Request req);


/*
 * <RECEIVER> functions
 */
int lam_ssi_rpi_ib_long_receive_env_first(struct lam_ssi_rpi_ib_envl *env, 
					  MPI_Request req);

void lam_ssi_rpi_ib_receiver_get_body_callback(MPI_Request req);

#define LAM_SSI_RPI_IB_LONG_SEND_CALLBACK lam_ssi_rpi_ib_haveadv = 1

#endif /* LAM_SSI_RPI_IB_LONG_H */
