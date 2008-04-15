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
 * $Id: ssi_crlam_blcr_module.c,v 1.4 2003/12/19 05:23:26 pcharapa Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-crlam-blcr-config.h>

#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>
#include <lam-ssi-crlam-blcr.h>


/*
 * Public string showing the BLCR module version number
 */
const char *lam_ssi_crlam_blcr_module_version =
  "LAM/MPI BLCR crlam SSI module version " LAM_SSI_CRLAM_BLCR_VERSION;


/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */
const lam_ssi_crlam_1_1_0_t lam_ssi_crlam_blcr_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */
  {
   /* SSI API version */
   1, 0, 0,

   /* Module kind name and version */
   "crlam",
   1, 1, 0,

   /* Module name and version */
   "blcr",
   LAM_SSI_CRLAM_BLCR_MAJOR_VERSION,
   LAM_SSI_CRLAM_BLCR_MINOR_VERSION,
   LAM_SSI_CRLAM_BLCR_RELEASE_VERSION,

   /* Module open and close functions */
   lam_ssi_crlam_blcr_open_module,
   NULL
  },

  /* Initialization / querying functions */

  lam_ssi_crlam_blcr_query
};
