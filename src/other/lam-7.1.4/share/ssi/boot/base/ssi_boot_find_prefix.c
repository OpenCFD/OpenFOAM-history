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
 * $Id: ssi_boot_find_prefix.c,v 1.2 2004/03/12 14:20:45 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sfh.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>


/*
 * Common utility function for boot SSI modules.
 *
 * Check to see if prefix is provided in the hostfile.
 * If so, it must have been attached to the lamnet struct.
 *
 */

char *
lam_ssi_boot_base_find_prefix(LIST *keyval) {
  
  ELEM *prefix_keyval;
  ELEM search;

  if (keyval != NULL) {
    search.key = "prefix";
    prefix_keyval = al_find(keyval, &search);
  
    if (prefix_keyval != NULL) 
      return prefix_keyval->value;
  }
  
  return NULL;
}
