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
 * $Id: ssi_crlam_close.c,v 1.4 2003/12/04 19:54:49 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>

#include <lam-ssi-cr.h>
#include <lamdebug.h>


int
lam_ssi_crlam_base_close(void)
{
  int ret = 0;

  /* SS: currently, the only module available is blcr, and it requires
     the PTHREADS package. However, in future, this condition might
     have to be modified in the context of other modules becoming
     available. */

#if LAM_HAVE_POSIX_THREADS
  /* Call close on the active module */

  if (lam_ssi_crlam_base_module.lscrl_meta_info.ssi_close_module != NULL)
    lam_ssi_crlam_base_module.lscrl_meta_info.ssi_close_module();

  /* Close the module if it's dynamic */

  lam_ssi_base_module_registry_unuse((lam_ssi_t *) &lam_ssi_crlam_base_module);

  /* Close the debug stream */

  if (lam_ssi_cr_verbose >= 1)
    lam_debug(lam_ssi_cr_did, " Closing");
  if (lam_ssi_cr_did >= 0) {
    lam_debug_close(lam_ssi_cr_did);
    lam_ssi_cr_did = -1;
  }
#endif

  return ret;
}
