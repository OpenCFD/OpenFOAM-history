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
 * $Id: rpi_ib_interval.h,v 1.2 2003/12/15 02:49:16 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_IB_INTERVAL_H
#define LAM_SSI_RPI_IB_INTERVAL_H

#include <mpisys.h>
#include <rpi_ib_hashmap.h>
#include <rpi_ib.h>


/*
 * public functions
 */

int lam_ssi_rpi_ib_interval_init(void);
void lam_ssi_rpi_ib_interval_finalize(void);

int lam_ssi_rpi_ib_interval_use(void *buf, size_t length,
				ib_mem_keypair *kp);
int lam_ssi_rpi_ib_interval_unuse(void *buf, size_t length);
void lam_ssi_rpi_ib_interval_unregister(void *buf, size_t length);

#endif /* LAM_IB_INTERVAL_H */
