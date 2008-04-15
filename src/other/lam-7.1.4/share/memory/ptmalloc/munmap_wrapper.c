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
 *	Function:	- add wrappers around munmap(), as ptmalloc
 *                        doesn't grab these for us
 */

#include <lam_config.h>

#include <unistd.h>
#include <unistd.h>
#include <sys/mman.h>


/*
 * If we have the syscall() interface, use it to snarf munmap() calls.
 */
#if LAM_USE_SYSCALL
#include <syscall.h>

int 
munmap(void *start, size_t length)
{
  lam_ssi_rpi_gm_interval_free(start, length);
  return syscall(__NR_munmap, start, length);
}


int
__munmap(void *start, size_t length)
{
  return munmap(start, length);
}
#endif
