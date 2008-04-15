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
 * $Id: lam-ssi-boot-slurm.h,v 1.1 2003/12/08 21:55:34 jsquyres Exp $
 *
 */

#ifndef LAM_SSI_BOOT_SLURM_EXPORT_H
#define LAM_SSI_BOOT_SLURM_EXPORT_H

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lamnet.h>
#include <all_array.h>


/*
 * Globally exported variables
 */

extern const lam_ssi_boot_1_1_0_t lam_ssi_boot_slurm_module;


/*
 * Globally exported functions
 * SSI module functions
 * boot API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_boot_slurm_open_module(OPT *ad);
  const lam_ssi_boot_actions_t *
    lam_ssi_boot_slurm_init(lam_ssi_boot_location_t where, int *priority);
  int lam_ssi_boot_slurm_finalize(void);

  int lam_ssi_boot_slurm_parse_options(OPT *aod, int schema);
  int lam_ssi_boot_slurm_allocate_nodes(struct lamnode **nodes, int *nnodes,
					int *origin);
  int lam_ssi_boot_slurm_verify_nodes(struct lamnode *nodes, int nnodes);
  int lam_ssi_boot_slurm_prepare_boot(void);
  int lam_ssi_boot_slurm_start_rte_procs(struct lamnode *nodes, int nnodes,
					 lam_ssi_boot_proc_t which,
					 int *num_started);
  int lam_ssi_boot_slurm_deallocate_nodes(struct lamnode **nodes, int *nnodes);

  int lam_ssi_boot_slurm_get_nodeid(OPT *aod);
  int lam_ssi_boot_slurm_send_lamd_info(OPT *aod, int dli_port);

  lam_array_t *lam_ssi_boot_slurm_parse_hostlist(char *str);

#ifdef __cplusplus
}
#endif

#endif /* LAM_SSI_BOOT_SLURM_EXPORT_H */
