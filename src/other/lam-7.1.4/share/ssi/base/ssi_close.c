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
 * $Id: ssi_close.c,v 1.5 2003/11/15 13:32:07 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <typical.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi.h>


/*
 * Main SSI shutdown.
 */
int
lam_ssi_base_close(void)
{
  /* Clear out all the registered SSI params */

  lam_ssi_base_param_finalize();

  /* If we had a stream open, close it */

  if (lam_ssi_verbose >= 0)
    lam_debug(lam_ssi_did, " Closing");
  if (lam_ssi_did >= 0) {
    lam_debug_close(lam_ssi_did);
    lam_ssi_verbose = -1;
    lam_ssi_did = -1;
  }

  /* Release all resources allocated by hostmap translation */

  lam_ssi_base_hostmap_finalize();

  /* Close down the module registry */

  lam_ssi_base_module_registry_finalize();

  /* All done */

  return 0;
}
