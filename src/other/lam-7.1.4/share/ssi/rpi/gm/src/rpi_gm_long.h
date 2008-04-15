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
 * $Id: rpi_gm_long.h,v 1.6 2003/11/06 18:58:12 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_LONG_H
#define LAM_SSI_RPI_GM_LONG_H

#include <lam-ssi-rpi-gm-config.h>

#include <mpisys.h>
#include <rpisys.h>


/*
 * public functions
 */

/*
 * <SENDER> functions
 */
int lam_ssi_rpi_gm_long_send(MPI_Request req);


/*
 * <RECEIVER> functions
 */
int lam_ssi_rpi_gm_long_receive_env_first(struct lam_ssi_rpi_gm_envl *env, 
					  MPI_Request req);
#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
int lam_ssi_rpi_gm_long_receive_all_done(struct lam_ssi_rpi_gm_envl *env, 
                                         MPI_Request req);
#endif

#endif /* LAM_SSI_RPI_GM_LONG_H */
