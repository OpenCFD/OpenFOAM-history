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
 * $Id: ssi_boot_find_boot_schema.c,v 1.4 2003/02/12 20:07:57 jsquyres Exp $
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
 * Access the environment
 */

extern char **environ;



/*
 * Common utility function for boot SSI modules.
 *
 * Search through a list of standard directories (including pwd)
 * looking for a boot schema file.
 */
char *
lam_ssi_boot_base_find_boot_schema(OPT *aod)
{
  char *bhost;
  char *fname;
  char **pathv = 0;
  int pathc = 0;
  int unusedc;
  char **unusedv;

  /* Set the directories for the boot schema file search */

  sfh_argv_add(&pathc, &pathv, "");
  sfh_argv_add(&pathc, &pathv, "$TROLLIUSHOME/etc");
  sfh_argv_add(&pathc, &pathv, "$LAMHOME/etc");
  sfh_argv_add(&pathc, &pathv, LAM_SYSCONFDIR);
  if (lam_ssi_boot_verbose >= 1) {
    int i;
    lam_debug(lam_ssi_boot_did, 
	      "base: looking for boot schema in following directories:");
    lam_debug(lam_ssi_boot_did, "base:   <current directory>");
    for (i = 1; i < pathc; ++i)
      lam_debug(lam_ssi_boot_did, "base:   %s", pathv[i]);
  }

  /* Set the boot schema file names */

  ao_unused(aod, &unusedc, &unusedv);
  if (unusedc == 2) {
    fname = unusedv[1];
  } else if ((fname = getenv("LAMBHOST"))) {
  } else if ((fname = getenv("TROLLIUSBHOST"))) {
  } else {
    fname = DEFFBHOST;
  }
  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "base: looking for boot schema file:");
    lam_debug(lam_ssi_boot_did, "base:   %s", fname);
  }

  /* Search the directories */

  bhost = sfh_path_findv(fname, pathv, R_OK, environ);
  sfh_argv_free(pathv);

  /* Did we find it? */

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "base: found boot schema: %s",
	      (bhost != NULL) ? bhost : "Not found!");
  if (bhost == 0)
    show_help("boot", "find-hostfile", fname, NULL);

  /* Return what we found (which may be NULL) */

  return bhost;
}
