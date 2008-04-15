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
 * $Id: lam-ssi-crmpi-self.h,v 1.1 2003/12/02 16:28:38 pcharapa Exp $
 *
 */

#ifndef LAM_SSI_CRMPI_SELF_H
#define LAM_SSI_CRMPI_SELF_H

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

  int lam_ssi_crmpi_self_open_module(OPT *ad);
  int lam_ssi_crmpi_self_query(int *priority, int *thread_min,
                               int *thread_max);

  /* CR action functions */

  int lam_ssi_crmpi_self_finalize(void);
  const lam_ssi_crmpi_actions_t *lam_ssi_crmpi_self_init(void);
  void lam_ssi_crmpi_self_app_suspend(void);

#ifdef __cplusplus
}
#endif

#endif
