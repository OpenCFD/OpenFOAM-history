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
 * $Id: strncpy.c,v 6.2 2003/05/31 22:56:19 jsquyres Exp $ 
 *
 * This file is only here because some platforms have a broken strncpy
 * (e.g., Itanium with RedHat Advanced Server glibc).
 */

#include <lam_config.h>

#include <string.h>

#include <etc_misc.h>


char *
lam_strncpy(char *dest, const char *src, int len)
{
  int i;
  int pad = 0;
  char *new_dest = dest;

  for (i = 0; i < len; ++i, ++src, ++new_dest) {
    if (pad)
      *new_dest = '\0';
    else {
      *new_dest = *src;
      if (*src == '\0')
        pad = 1;
    }
  }

  return dest;
}
