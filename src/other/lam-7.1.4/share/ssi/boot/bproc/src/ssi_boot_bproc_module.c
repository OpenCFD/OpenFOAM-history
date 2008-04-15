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
 * $Id: ssi_boot_bproc_module.c,v 1.4 2003/07/11 19:40:25 jsquyres Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-boot-bproc-config.h>

#include <stdlib.h>

#include <sfh.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lam-ssi-boot-bproc.h>


/*
 * Public string showing the bproc module version number
 */
const char *lam_ssi_boot_bproc_module_version =
  "LAM/MPI SSI boot bproc module version " LAM_SSI_BOOT_BPROC_VERSION;

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_boot_1_1_0_t lam_ssi_boot_bproc_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
   /* SSI API version */

   1, 0, 0,

   /* Module kind name and version */

   "boot",
   1, 1, 0,

   /* Module name and version */

   "bproc",
   LAM_SSI_BOOT_BPROC_MAJOR_VERSION,
   LAM_SSI_BOOT_BPROC_MINOR_VERSION,
   LAM_SSI_BOOT_BPROC_RELEASE_VERSION,

   /* Module open and close functions */

   lam_ssi_boot_bproc_open_module,
   NULL
  },

  /* boot API function pointers */

  lam_ssi_boot_bproc_init,
  lam_ssi_boot_bproc_finalize
};
