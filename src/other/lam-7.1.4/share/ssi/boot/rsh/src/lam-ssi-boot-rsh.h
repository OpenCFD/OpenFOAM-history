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
 * $Id: lam-ssi-boot-rsh.h,v 1.11 2004/03/06 21:17:32 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_BOOT_RSH_EXPORT_H
#define LAM_SSI_BOOT_RSH_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lamnet.h>


/*
 * Globally exported variables
 */

extern const lam_ssi_boot_1_1_0_t lam_ssi_boot_rsh_module;
extern int lam_ssi_boot_rsh_param_no_n;
extern int lam_ssi_boot_rsh_param_no_profile;
extern int lam_ssi_boot_rsh_param_fast;
extern int lam_ssi_boot_rsh_param_ignore_stderr;
extern char **lam_ssi_boot_rsh_agent;


/*
 * Globally exported functions
 * SSI module functions
 * boot API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_boot_rsh_open_module(OPT *ad);
  const lam_ssi_boot_actions_t *
    lam_ssi_boot_rsh_init(lam_ssi_boot_location_t where, int *priority);
  int lam_ssi_boot_rsh_finalize(void);

  int lam_ssi_boot_rsh_parse_options(OPT *aod, int bhost_schema_args);
  int lam_ssi_boot_rsh_allocate_nodes(struct lamnode **nodes, int *nnodes,
				      int *origin);
  int lam_ssi_boot_rsh_verify_nodes(struct lamnode *nodes, int nnodes);
  int lam_ssi_boot_rsh_prepare_boot(void);
  int lam_ssi_boot_rsh_start_rte_procs(struct lamnode *nodes, int nnodes,
				       lam_ssi_boot_proc_t which,
				       int *num_started);
  int lam_ssi_boot_rsh_deallocate_nodes(struct lamnode **nodes, int *nnodes);

  int lam_ssi_boot_rsh_start_application(char ***argv, int *argc, 
					 int num_procs, struct lamnode *node);
  int lam_ssi_boot_rsh_start_rte_proc(struct lamnode *node,
				      lam_ssi_boot_proc_t which);

  /*
   * Other utility functions
   */

  int lam_ssi_boot_rsh_inetexec(struct lamnode *node, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_BOOT_RSH_EXPORT_H */
