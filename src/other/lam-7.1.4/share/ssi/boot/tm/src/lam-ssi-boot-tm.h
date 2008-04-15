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
 * $Id: lam-ssi-boot-tm.h,v 1.9 2003/07/14 23:21:36 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_BOOT_TM_EXPORT_H
#define LAM_SSI_BOOT_TM_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lamnet.h>


/*
 * Globally exported variables
 */

extern const lam_ssi_boot_1_1_0_t lam_ssi_boot_tm_module;


/*
 * Globally exported functions
 * SSI module functions
 * boot API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_boot_tm_open_module(OPT *ad);
  const lam_ssi_boot_actions_t *
    lam_ssi_boot_tm_init(lam_ssi_boot_location_t where, int *priority);
  int lam_ssi_boot_tm_finalize(void);

  int lam_ssi_boot_tm_parse_options(OPT *aod, int schema);
  int lam_ssi_boot_tm_allocate_nodes(struct lamnode **nodes, int *nnodes,
				      int *origin);
  int lam_ssi_boot_tm_verify_nodes(struct lamnode *nodes, int nnodes);
  int lam_ssi_boot_tm_prepare_boot(void);
  int lam_ssi_boot_tm_start_rte_procs(struct lamnode *nodes, int nnodes,
				       lam_ssi_boot_proc_t which,
				       int *num_started);
  int lam_ssi_boot_tm_deallocate_nodes(struct lamnode **nodes, int *nnodes);

  int lam_ssi_boot_tm_start_application(char ***argv, int *argc, 
					 int num_procs, struct lamnode *node);
  int lam_ssi_boot_tm_start_application_wait(char ***argv, int *argc, 
					     int num_procs, 
					     struct lamnode *node,
					     int wait);

  int lam_ssi_boot_tm_start_rte_proc(struct lamnode *node,
				      lam_ssi_boot_proc_t which);

#ifdef __cplusplus
}
#endif

/*
 * Internal functions
 */
#ifdef __cplusplus
extern "C" {
#endif
  int lam_ssi_boot_tm_hboot(struct lamnode *node,
			    struct lamnode origin_node, int origin);

  int lam_ssi_boot_tm_bhostparse(struct lamnode **plamnet, int *pnlamnet);
#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_BOOT_TM_EXPORT_H */
