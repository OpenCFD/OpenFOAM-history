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
 * $Id: lam-ssi-crmpi-blcr.h,v 1.3 2003/07/11 19:44:37 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_CRMPI_BLCR_H
#define LAM_SSI_CRMPI_BLCR_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>


extern int lam_ssi_cr_verbose;
extern int lam_ssi_cr_did;


/*
 * Globally exported functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_crmpi_blcr_open_module(OPT *ad);
  int lam_ssi_crmpi_blcr_query(int *priority, int *thread_min,
                               int *thread_max);

  /* CR action functions */

  int lam_ssi_crmpi_blcr_finalize(void);
  const lam_ssi_crmpi_actions_t *lam_ssi_crmpi_blcr_init(void);
  void lam_ssi_crmpi_blcr_app_suspend(void);

#ifdef __cplusplus
}
#endif

#endif
