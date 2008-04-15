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
 * $Id: ssi_boot_find_hostname.c,v 1.6 2003/03/24 05:06:59 jsquyres Exp $
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
 * Look for a hostname in argv.
 */
char *
lam_ssi_boot_base_find_hostname(OPT *aod)
{
  int unusedc;
  char **unusedv;

  /* Actually, by the time we get here, unused argc should be 2, and
     the hostname should be in unused argv[1]. */

  ao_unused(aod, &unusedc, &unusedv);
  if (unusedc == 2) {
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, " found boot hostname: %s", unusedv[1]);
    return strdup(unusedv[1]);
  } else {
    show_help("boot", "find-hostname", NULL);
    return NULL;
  }
}
