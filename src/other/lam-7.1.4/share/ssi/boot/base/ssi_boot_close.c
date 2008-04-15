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
 * $Id: ssi_boot_close.c,v 1.14 2003/12/04 19:54:45 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>

#include <lam-ssi-boot.h>
#include <lamdebug.h>



int
lam_ssi_boot_close(void)
{
  int ret = 0;

  /* Call finalize on the active boot module */

  if (lam_ssi_boot_base_module.lsb_finalize != NULL)
    lam_ssi_boot_base_module.lsb_finalize();

  /* Call close on the active boot module */

  if (lam_ssi_boot_base_module.lsb_meta_info.ssi_close_module != NULL)
    ret = lam_ssi_boot_base_module.lsb_meta_info.ssi_close_module();

  /* Close the module if it's dynamic */

  lam_ssi_base_module_registry_unuse((lam_ssi_t*) &lam_ssi_boot_base_module);

  /* Finalize the list */

  if (lam_ssi_boot_base_opened != NULL)
    al_free(lam_ssi_boot_base_opened);

  /* Free the array of modules */

  if (lam_ssi_boot_modules != NULL)
    free(lam_ssi_boot_modules);

  /* Close the debug stream */

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, " Closing");
  if (lam_ssi_boot_did >= 0) {
    lam_debug_close(lam_ssi_boot_did);
    lam_ssi_boot_did = -1;
  }

  /* All done */

  return ret;
}
