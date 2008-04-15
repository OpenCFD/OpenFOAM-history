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
 * $Id: ssi_boot_globus_module.c,v 1.5 2003/07/11 19:40:25 jsquyres Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-boot-globus-config.h>

#include <stdlib.h>

#include <sfh.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lam-ssi-boot-globus.h>


/*
 * Public string showing the globus module version number
 */
const char *lam_ssi_boot_globus_module_version =
  "LAM/MPI SSI boot globus module version " LAM_SSI_BOOT_GLOBUS_VERSION;

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_boot_1_1_0_t lam_ssi_boot_globus_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
   /* SSI API version */

   1, 0, 0,

   /* Module kind name and version */

   "boot",
   1, 1, 0,

   /* Module name and version */

   "globus",
   LAM_SSI_BOOT_GLOBUS_MAJOR_VERSION,
   LAM_SSI_BOOT_GLOBUS_MINOR_VERSION,
   LAM_SSI_BOOT_GLOBUS_RELEASE_VERSION,

   /* Module open and close functions */

   lam_ssi_boot_globus_open_module,
   NULL
  },

  /* Initialize / finalize functions */

  lam_ssi_boot_globus_init,
  lam_ssi_boot_globus_finalize
};
