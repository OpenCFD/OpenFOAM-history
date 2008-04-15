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
 * $Id: ssi_coll_close.c,v 1.11 2003/12/04 19:53:14 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>

#include <all_list.h>
#include <lam-ssi-coll.h>
#include <lamdebug.h>


int
lam_ssi_coll_base_close(void)
{
  int i, ret = 0;

  /* Close all the SSI collective modules.  */
  /* Note that we do *not* unregister all the SSI dynamic collective
     modules here because there may be complicated dependencies that
     may force specific ordering of the closing.  Hence, we'll just
     leave them open for the moment.  This function is called as part
     of MPI_Finalize, so at the end of MPI_Finalize, lam_ssi_close()
     will be invoked, which will unregister any remaining dynamic
     modules, and do so in the correct order. */

  for (i = 0; lam_ssi_coll_modules[i] != NULL; ++i)
    if (lam_ssi_coll_modules[i]->ssi_close_module != NULL)
      ret += lam_ssi_coll_modules[i]->ssi_close_module();

  /* Free the available list */

  if (lam_ssi_coll_base_available != NULL)
    al_free(lam_ssi_coll_base_available);

  /* Free the opened list if it wasn't already (e.g., laminfo) */

  if (lam_ssi_coll_base_opened != NULL)
    al_free(lam_ssi_coll_base_opened);

  /* Free the array of modules */

  if (lam_ssi_coll_modules != NULL)
    free(lam_ssi_coll_modules);

  /* Close the debug stream */

  if (lam_ssi_coll_verbose >= 0)
    lam_debug(lam_ssi_coll_did, "close: Closing");
  if (lam_ssi_coll_did >= 0) {
    lam_debug_close(lam_ssi_coll_did);
    lam_ssi_coll_did = -1;
  }

  /* All done */

  return ret;
}
