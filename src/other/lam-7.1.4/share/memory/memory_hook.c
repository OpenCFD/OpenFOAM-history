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

#include "lam_config.h"
#include "malloc_wrapper.h"
#include "laminternal.h"

#include <unistd.h>
#include <sys/mman.h>

static lam_memory_unpin_t *handler_func = NULL;

#if LAM_WANT_DARWIN7MALLOC
  void lam_darwin_malloc_linker_hack(void);
#endif

/*
 * Initialization call to trick linkers
 *
 * Call from somewhere that will be called in all user applications
 * (like MPI_INIT)
 */
void
lam_register_free_handler_init()
{
#if LAM_WANT_DARWIN7MALLOC
  lam_darwin_malloc_linker_hack();
#endif
}


/*
 * Handle requests to do callbacks when memory is unpinned
 */
int
lam_register_free_handler(lam_memory_unpin_t *func)
{
#if LAM_WANT_PTMALLOC || LAM_WANT_PTMALLOC2 || LAM_WANT_DARWIN7MALLOC || LAM_WANT_EXTERNALMALLOC

  /* Do not allow one memory handler to arbitrarily overwrite
     another */

  if (func != NULL && handler_func != NULL) {
    return LAMERROR;
  }

  /* Otherwise, if we're a) setting func==NULL, or b) if handler_func
     is already NULL, then go ahead and allow the assignment. */

  handler_func = func;
  return 0;
#else
  return LAMERROR;
#endif /* LAM_WANT_PTMALLOC */
}


/*
 * Get callback from memory managment code - call our handler, if we
 * have one.
 */
void
lam_handle_free(void *buf, size_t length)
{
  if (handler_func != NULL) {
    handler_func(buf, length);
  }
}
