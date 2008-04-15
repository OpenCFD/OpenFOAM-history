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
 * $Id: ssi_coll_smp_module.c,v 1.7 2003/09/12 22:34:36 jsquyres Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-coll-smp-config.h>

#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>

#include <all_list.h>
#include <net.h>
#include <rpisys.h>
#include <mpisys.h>


/*
 * Public string showing the module version number
 */
const char *lam_ssi_coll_smp_module_version =
  "LAM/MPI basic collective SSI module version " LAM_SSI_COLL_SMP_VERSION;

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_coll_1_1_0_t lam_ssi_coll_smp_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
    /* SSI API version */

    1, 0, 0,

    /* Module kind name and version */

    "coll",
    1, 1, 0,

    /* Module name and version */

    "smp",
    LAM_SSI_COLL_SMP_MAJOR_VERSION,
    LAM_SSI_COLL_SMP_MINOR_VERSION,
    LAM_SSI_COLL_SMP_RELEASE_VERSION,

    /* Module open and close functions */

    NULL,
    NULL
  },

  /* Collective API function pointers */

  lam_ssi_coll_smp_thread_query,
  lam_ssi_coll_smp_query,
  lam_ssi_coll_smp_end_query,

  /* Flags */
  /* Has checkpoint */

  1
};
