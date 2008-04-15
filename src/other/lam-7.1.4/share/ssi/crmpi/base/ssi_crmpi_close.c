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
 * $Id: ssi_crmpi_close.c,v 1.9 2003/12/04 19:54:51 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>

#include <all_list.h>
#include <lam-ssi-cr.h>
#include <lamdebug.h>


int
lam_ssi_crmpi_base_close(void)
{
  int ret = 0;

#if LAM_HAVE_THREADS
  /* Call close on the active module */

  if (lam_ssi_crmpi_base_module.lscrm_meta_info.ssi_close_module != NULL)
    ret = lam_ssi_crmpi_base_module.lscrm_meta_info.ssi_close_module();

  /* Close the module if it's dynamic */

  lam_ssi_base_module_registry_unuse((lam_ssi_t *) &lam_ssi_crmpi_base_module);

  /* Close the debug stream */

  if (lam_ssi_cr_verbose >= 1)
    lam_debug(lam_ssi_cr_did, " Closing");
  if (lam_ssi_cr_did >= 0) {
    lam_debug_close(lam_ssi_cr_did);
    lam_ssi_cr_did = -1;
  }
#endif

  /* Delete the list of available crmpi modules (allocated in
     rpi_base_open).  All of the un-selected modules were already
     closed, way back in MPI_INIT. */

  if (lam_ssi_crmpi_base_available != NULL)
    al_free(lam_ssi_crmpi_base_available);

  /* Free the opened list if it wasn't already (e.g., laminfo) */

  if (lam_ssi_crmpi_base_opened != NULL)
    al_free(lam_ssi_crmpi_base_opened);

  /* Free the array of modules */

  if (lam_ssi_crmpi_modules != NULL)
    free(lam_ssi_crmpi_modules);

  return ret;
}
