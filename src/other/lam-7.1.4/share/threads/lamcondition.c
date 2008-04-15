/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 * $Id: lamcondition.c,v 1.3 2003/04/09 17:37:14 brbarret Exp $
 *
 * Function:
 *  - LAM interface for condition variables
 */

#include <lam_config.h>

#include <lamthreads.h>

int
lam_cond_init(lam_cond_t *cond)
{
  int ret;

#if LAM_HAVE_SOLARIS_THREADS
  ret = cond_init(cond, NULL, NULL);
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_cond_init(cond, NULL);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_cond_wait(lam_cond_t *cond, lam_mutex_t *mutex)
{
  int ret;

#if LAM_HAVE_SOLARIS_THREADS
  ret = cond_wait(cond, &(mutex->mutex));
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_cond_wait(cond, &(mutex->mutex));
#else
  ret = -1;
#endif

  return ret;
}


int
lam_cond_signal(lam_cond_t *cond)
{
  int ret;

#if LAM_HAVE_SOLARIS_THREADS
  ret = cond_signal(cond);
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_cond_signal(cond);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_cond_broadcast(lam_cond_t *cond)
{
  int ret;

#if LAM_HAVE_SOLARIS_THREADS
  ret = cond_broadcast(cond);
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_cond_broadcast(cond);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_cond_destroy(lam_cond_t *cond)
{
  int ret;

#if LAM_HAVE_SOLARIS_THREADS
  ret = cond_destroy(cond);
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_cond_destroy(cond);
#else
  ret = -1;
#endif

  return ret;
}
