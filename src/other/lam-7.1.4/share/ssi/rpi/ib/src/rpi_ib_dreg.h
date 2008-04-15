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
 * $Id: rpi_ib_dreg.h,v 1.3 2003/12/15 02:49:16 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_IB_DREG_H
#define LAM_SSI_RPI_IB_DREG_H

/*
 * public functions
 */
void lam_ssi_rpi_ib_reg_init(void);
int lam_ssi_rpi_ib_reg_add(int nprocs, int num_bufs);

struct lam_ssi_rpi_ib_envl *lam_ssi_rpi_ib_reg_env_malloc(ib_mem_keypair *kp);
void lam_ssi_rpi_ib_reg_env_free(struct lam_ssi_rpi_ib_envl* buf);

char *lam_ssi_rpi_ib_reg_malloc(long length, ib_mem_keypair *kp);
void lam_ssi_rpi_ib_reg_free(char *buf);

void lam_ssi_rpi_ib_reg_destroy(void);

#endif /* LAM_SSI_RPI_IB_DREG_H */
