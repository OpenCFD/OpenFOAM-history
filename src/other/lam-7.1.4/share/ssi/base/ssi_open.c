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
 * $Id: ssi_open.c,v 1.7 2003/11/15 13:32:07 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>

#include <typical.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi.h>


/*
 * Global variables
 */

int lam_ssi_verbose = -1;
int lam_ssi_did = -1;
int lam_ssi_base_hostmap_param_index = -1;


/*
 * local variables
 */

static int param_index = -1;
static lam_debug_stream_info_t lds = {
  -1, -1, -1, NULL, NULL, -1, -1, -1, -1, "ssi.txt"
};


/*
 * Main SSI initialization.  
 */
int
lam_ssi_base_open(OPT *aod)
{
  /* Register the ssi_verbose param */

  param_index = lam_ssi_base_param_register_string("base", NULL, "verbose",
                                                   "verbose", NULL);

  /* Register a parameter for mapping alternate IP names/addresses for
     MPI communication */

  lam_ssi_base_hostmap_param_index =
    lam_ssi_base_param_register_string("mpi", NULL, "hostmap", NULL,
                                       LAM_SYSCONFDIR "/lam-hostmap.txt");

  /* What verbosity level do we want? */

  lam_ssi_base_set_verbose(param_index, &lds,
                           &lam_ssi_verbose, &lam_ssi_did);
  if (lam_ssi_verbose >= 0)
    lam_debug(lam_ssi_did, " Opening");

  /* Open up the module registry */

  if (lam_ssi_base_module_registry_init() != 0)
    return LAMERROR;

  /* All done */

  return 0;
}
