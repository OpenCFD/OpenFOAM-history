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
 * $Id: ssi_module_check.c,v 1.4 2003/07/11 19:29:51 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <typical.h>
#include <etc_misc.h>
#include <lam-ssi.h>


int
lam_ssi_base_module_check(char *type, char *module, int is_default)
{
  int len;
  char *var_name, *e;

  /* If either of the char args is null, it's an error */

  if (type == NULL || module == NULL)
    return LAMERROR;

  /* Alloc enough space to hold the env variable name */

  len = 32 + strlen(type);
  var_name = malloc(len);
  if (var_name == NULL)
    return 0;

  /* Construct the name LAM_MPI_SSI_<type>, and try to get the env
     variable of that name.  If it doesn't exist, return the
     is_default flag. */

  snprintf(var_name, len, "LAM_MPI_SSI_%s", type);
  e = getenv(var_name);
  free(var_name);
  if (e == NULL)
    return is_default;

  /* If the env varable value equals <module>, then return 1.
     Otherwise, return 0. */

  if (strcasecmp(e, module) == 0)
    return 1;

  return 0;
}
