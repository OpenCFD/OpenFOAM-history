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
 * $Id: hook_functions.c,v 1.1 2003/06/18 03:02:48 jsquyres Exp $
 *
 *	Function:	- Hook functions for ptmalloc module used 
 *                        in gm RPI module
 */

#include <lam_config.h>
#include <malloc_wrapper.h>

#include <unistd.h>
#include <unistd.h>
#include <sys/mman.h>


#if LAM_WANT_PTMALLOC

/*
 * Not all systems have sbrk() declared, since it's technically not a
 * POSIX function.
 */
#if !LAM_HAVE_DECL_SBRK
void *sbrk();
#endif

/*
 * Hook function that is invoked by ptmalloc for sbrk.  When we sbrk()
 * a negative number, need to go unregister any memory that may be in
 * the region that we're letting go.
 */
void *
lam_ptmalloc_sbrk(int inc)
{
  if (inc < 0) {
    long oldp = (long) sbrk(0);
    lam_handle_free((void*) (oldp + inc), -inc);
  }

  return sbrk(inc);
}


/*
 * Same rationale as for sbrk(), above.
 */
int 
lam_ptmalloc_munmap(void *start, size_t length)
{
  lam_handle_free(start, length);
  return munmap(start, length);
}

#endif /* LAM_WANT_PTMALLOC */
