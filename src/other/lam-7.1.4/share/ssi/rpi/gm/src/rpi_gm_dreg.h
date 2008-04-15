/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: rpi_gm_dreg.h,v 1.6 2003/11/06 18:58:11 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_DREG_H
#define LAM_SSI_RPI_GM_DREG_H

#include <sys/types.h>

#include <lam-ssi-rpi-gm.h>
#include <etc_misc.h>

/*
 * Public variables
 */
extern int PUB(env_mempool);


/*
 * public functions
 */
int lam_ssi_rpi_gm_dma_init(struct lam_ssi_rpi_proc *p);

#define lam_ssi_rpi_gm_dma_env_malloc() \
  lam_mp_malloc(PUB(env_mempool))
#define lam_ssi_rpi_gm_dma_env_free(env) \
  lam_mp_free(PUB(env_mempool), (env))

char *lam_ssi_rpi_gm_dma_malloc(size_t length);
void lam_ssi_rpi_gm_dma_free(char *buf, size_t length);

void lam_ssi_rpi_gm_dma_destroy(void);

#endif /* LAM_SSI_RPI_GM_DREG_H */
