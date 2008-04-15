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
 * $Id: ssi_coll_lam_basic_module.c,v 1.10 2003/09/12 22:34:35 jsquyres Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-coll-lam-basic-config.h>

#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>


/*
 * Public string showing the coll lam_basic module version number
 */
const char *lam_ssi_coll_lam_basic_module_version =
  "LAM/MPI basic collective SSI module version " LAM_SSI_COLL_LAM_BASIC_VERSION;

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_coll_1_1_0_t lam_ssi_coll_lam_basic_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
   /* SSI API version */

   1, 0, 0,

   /* Module kind name and version */

   "coll",
   1, 1, 0,

   /* Module name and version */

   "lam_basic",
   LAM_SSI_COLL_LAM_BASIC_MAJOR_VERSION,
   LAM_SSI_COLL_LAM_BASIC_MINOR_VERSION,
   LAM_SSI_COLL_LAM_BASIC_RELEASE_VERSION,

   /* Module open and close functions */

   NULL,
   NULL
  },

  /* Initialization / querying functions */

  lam_ssi_coll_lam_basic_thread_query,
  lam_ssi_coll_lam_basic_query,
  lam_ssi_coll_lam_basic_end_query,

  /* Flags */
  /* Has checkpoint */

  1
};
