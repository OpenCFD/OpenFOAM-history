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
 * $Id: strncpy.c,v 6.2 2003/05/31 22:56:19 jsquyres Exp $ 
 *
 * hide all the portability junk for determining the number of cpus in
 * the local box.  Returns 1 if it can't find a way to get the ncpus
 * count.
 */

#include <lam_config.h>
#include <etc_misc.h>


long
lam_get_ncpus()
{
  long ncpus;

#if LAM_HAVE_SYSCONF && defined(_SC_NPROCESSORS_ONLN)
  /* Linux, most normal unix */
  ncpus = sysconf(_SC_NPROCESSORS_ONLN);

#elif LAM_HAVE_SYSCTL && defined(CTL_HW) && defined(HW_NCPU)
  /* Mac OS X */
  int mib[2], ret;
  unsigned int miblen;
  size_t len;

  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  miblen = 2;
  len = sizeof(ncpus);
  ret = sysctl(mib, miblen, &ncpus, &len, NULL, 0);
  if (ret != 0) ncpus = 1;

#else
  ncpus = 1;

#endif

  return ncpus;
}
