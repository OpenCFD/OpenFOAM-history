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
 * $Id: ssi_coll_shmem_module.c,v 1.1 2004/02/17 16:40:08 adharurk Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-coll-shmem-config.h>

#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-shmem.h>

#include <all_list.h>
#include <net.h>
#include <rpisys.h>
#include <mpisys.h>


/*
 * Public string showing the module version number
 */
const char *lam_ssi_coll_shmem_module_version =
"LAM/MPI shmem collective SSI module version " LAM_SSI_COLL_SHMEM_VERSION;

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

const lam_ssi_coll_1_1_0_t lam_ssi_coll_shmem_module = {

  /* First, the lam_ssi_1_0_0_t struct containing meta information
     about the module itself */

  {
    /* SSI API version */

    1, 0, 0,

    /* Module kind name and version */

    "coll",
    1, 1, 0,

    /* Module name and version */

    "shmem",
    LAM_SSI_COLL_SHMEM_MAJOR_VERSION,
    LAM_SSI_COLL_SHMEM_MINOR_VERSION,
    LAM_SSI_COLL_SHMEM_RELEASE_VERSION,

    /* Module open and close functions */

    lam_ssi_coll_shmem_open,
    NULL
  },

  /* Collective API function pointers */

  lam_ssi_coll_shmem_thread_query,
  lam_ssi_coll_shmem_query,
  lam_ssi_coll_shmem_end_query,

  /* Flags */
  /* Has checkpoint */

  1
};
