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
 * $Id: rpi_gm_interval.h,v 1.3 2003/11/06 18:58:11 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_INTERVAL_H
#define LAM_SSI_RPI_GM_INTERVAL_H

#include <mpisys.h>


/*
 * public functions
 */

int lam_ssi_rpi_gm_interval_init(struct lam_ssi_rpi_proc *p);
void lam_ssi_rpi_gm_interval_destroy(void);

int lam_ssi_rpi_gm_interval_use(void *buf, size_t length, int lru_ok,
                                int need_to_pin);
int lam_ssi_rpi_gm_interval_check(void *buf, size_t length);
int lam_ssi_rpi_gm_interval_unuse(void *buf, size_t length);
void lam_ssi_rpi_gm_interval_free(void *buf, size_t length);

#endif /* LAM_GM_INTERVAL_H */
