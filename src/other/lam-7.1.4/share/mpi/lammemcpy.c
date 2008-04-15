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
 * $Id: lammemcpy.c,v 6.2 2003/05/21 19:17:02 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>

#include <mpisys.h>

/*
 * Note: this file is only compiled if LAM_WANT_PREFIX_MEMCPY is true.
 * If it isn't, there's a top-level #define that switches lam_memcpy()
 * into a regular memcpy(), and this file is not included in libmpi.  
 */

/*
 * glibc's memcpy() is somewhat broken.  If the size that it is
 * copying is not divisible by 4 bytes, it used a sub-optimal
 * algorithm with noticable performance degredation.  So if we're
 * using glibc and the size is not divisible by 4, manually copy over
 * a few prefix bytes, and then use the real memcpy.  If we're not
 * using glibc, then we probably didn't call this function anyway
 * (there's a top-level #define in mpisys.h), but just in case, simply
 * return the native memcpy().
 *
 * Thanks to Dr. Dave Turner for pointing this out and suggesting
 * fixes.  
 */
void *
lam_memcpy(void *dest, const void *src, size_t size)
{
  int nleft = size % 8;
  size -= nleft;

  /* Copy over the "prefix" bytes (size - (size % 8)).  This will
     allow us to do the bulk copy and then copy the remainder bytes
     afterwards. */
  
  memcpy(dest, src, size);

  /* Now copy the remainder bytes, if any */

  if (nleft > 0)
    memcpy(((char *) dest) + size, ((char *) src) + size, nleft);

  return dest;
}
