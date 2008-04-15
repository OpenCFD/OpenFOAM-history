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
 * $Id: lam-ssi-crlam-self.h,v 1.2 2004/03/09 16:56:14 pcharapa Exp $
 *
 */

#ifndef LAM_SSI_CR_SELF_H
#define LAM_SSI_CR_SELF_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>


/*
 * Globally exported functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_crlam_self_open_module(OPT *ad);
  const lam_ssi_crlam_actions_t *lam_ssi_crlam_self_query(int *priority);

  /* CR action functions */

  int lam_ssi_crlam_self_checkpoint(void);
  int lam_ssi_crlam_self_continue(void);
  void lam_ssi_crlam_self_disable_checkpoint(void);
  void lam_ssi_crlam_self_enable_checkpoint(void);
  int lam_ssi_crlam_self_finalize(void);
  int lam_ssi_crlam_self_init(char *, char **, OPT *, struct _gps *, int);

  int lam_ssi_crlam_self_lamcheckpoint(pid_t);
  int lam_ssi_crlam_self_lamrestart(char *);
#ifdef __cplusplus
}
#endif

#endif
