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
 * $Id: lam-ssi-crlam-blcr.h,v 1.5 2003/12/19 04:58:22 pcharapa Exp $
 *
 */

#ifndef LAM_SSI_CR_BLCR_H
#define LAM_SSI_CR_BLCR_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>


/*
 * Globally exported functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_crlam_blcr_open_module(OPT *ad);
  const lam_ssi_crlam_actions_t *lam_ssi_crlam_blcr_query(int *priority);

  /* CR action functions */

  int lam_ssi_crlam_blcr_checkpoint(void);
  int lam_ssi_crlam_blcr_continue(void);
  void lam_ssi_crlam_blcr_disable_checkpoint(void);
  void lam_ssi_crlam_blcr_enable_checkpoint(void);
  int lam_ssi_crlam_blcr_finalize(void);
  int lam_ssi_crlam_blcr_init(char *, char **, OPT *, struct _gps *, int);

  int lam_ssi_crlam_blcr_lamcheckpoint(int);
  int lam_ssi_crlam_blcr_lamrestart(char *);
#ifdef __cplusplus
}
#endif

#endif
