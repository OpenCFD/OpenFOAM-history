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
 * $Id: rpi_gm_unexpected.h,v 1.5 2003/11/06 18:58:12 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_UNEXPECTED_H
#define LAM_SSI_RPI_GM_UNEXPECTED_H

#include <mpisys.h>
#include <rpisys.h>

#include <rpi_gm.h>


/*
 * public functions
 */

int lam_ssi_rpi_gm_unexpected_advance(MPI_Request req, 
				      struct lam_ssi_rpi_cbuf_msg *bmsg);
int lam_ssi_rpi_gm_unexpected_receive_env(struct lam_ssi_rpi_proc *src_c2c, 
					  struct lam_ssi_rpi_gm_envl *env);


#endif /* LAM_SSI_RPI_GM_UNEXPECTED_H */
