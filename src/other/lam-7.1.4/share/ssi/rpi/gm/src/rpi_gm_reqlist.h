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
 * $Id: rpi_gm_reqlist.h,v 1.4 2003/11/06 18:58:12 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_REQLIST_H
#define LAM_SSI_RPI_GM_REQLIST_H

#include <rpi_gm.h>
#include <rpi_gm_req.h>


/*
 * public functions
 */

int lam_ssi_rpi_gm_reqlist_init(struct lam_ssi_rpi_gm_reqlist_t *rl);
int lam_ssi_rpi_gm_reqlist_append(struct lam_ssi_rpi_gm_reqlist_t *rl, 
				  MPI_Request req);
struct lam_ssi_rpi_gm_reqlist_item_t *
  lam_ssi_rpi_gm_reqlist_match(struct lam_ssi_rpi_gm_reqlist_t *rl,
			       struct lam_ssi_rpi_gm_envl *env);
int lam_ssi_rpi_gm_reqlist_rm_item(struct lam_ssi_rpi_gm_reqlist_t *rl, 
				   struct lam_ssi_rpi_gm_reqlist_item_t *rli);
int lam_ssi_rpi_gm_reqlist_rm_req(struct lam_ssi_rpi_gm_reqlist_t *rl, 
				  MPI_Request req);
int lam_ssi_rpi_gm_reqlist_pop_top(struct lam_ssi_rpi_gm_reqlist_t *rl);

#endif /* LAM_SSI_RPI_GM_REQLIST_H */
