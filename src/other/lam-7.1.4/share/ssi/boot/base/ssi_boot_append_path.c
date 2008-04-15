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
 * $Id: ssi_boot_append_path.c,v 1.1 2003/09/25 20:45:22 vsahay Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <sfh.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>


/*
 * Common utility function for boot SSI modules.
 *
 * Append pathname and prefix to the executable 
 *
 */

int
lam_ssi_boot_base_append_path(char *prefix, char *dir, char *fname,
			      int *argcnt, char ***arglist)
{
  int path_len;
  char *path_var;

  path_len = strlen(prefix) + strlen(dir) + strlen(fname) + 3;
  path_var = (char *) malloc (path_len * sizeof(char));
  if (path_var == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    return LAMERROR;
  }
    
  snprintf(path_var, path_len, "%s/%s/%s", prefix, dir, fname);
  sfh_argv_add(argcnt, arglist, path_var);
  free(path_var);
  return 0;
}
