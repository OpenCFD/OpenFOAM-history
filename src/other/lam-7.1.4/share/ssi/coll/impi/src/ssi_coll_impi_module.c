/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 */

#include <lam_config.h>
#include <lam-ssi-coll-impi-config.h>

#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-impi.h>


/*
 * Public string showing the module version number
 */
const char *lam_ssi_coll_impi_module_version =
  "IMPI collective SSI module version " LAM_SSI_COLL_IMPI_VERSION;


/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_coll_1_1_0_t lam_ssi_coll_impi_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
   /* SSI API version */

   1, 0, 0,

   /* Module kind name and version */

   "coll",
   1, 1, 0,

   /* Module name and version */

   "impi",
   LAM_SSI_COLL_IMPI_MAJOR_VERSION,
   LAM_SSI_COLL_IMPI_MINOR_VERSION,
   LAM_SSI_COLL_IMPI_RELEASE_VERSION,

   /* Module open and close functions */

   NULL,
   NULL
  },

  /* Initialization / querying functions */

  lam_ssi_coll_impi_thread_query,
  lam_ssi_coll_impi_query,
  lam_ssi_coll_impi_end_query,

  /* Flags */
  /* Has checkpoint */

  1
};
