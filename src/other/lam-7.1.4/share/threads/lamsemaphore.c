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
 * $Id: lamsemaphore.c,v 1.5 2003/04/09 17:40:45 brbarret Exp $
 *
 * Function:
 *  - LAM interface for semaphores (thread related, not SYS-V)
 */

#include <lam_config.h>

#include <lamthreads.h>

int
lam_sem_init(lam_sem_t *sem, int pshared, unsigned int value)
{
  int ret = 0;
#if LAM_HAVE_SOLARIS_THREADS
  int type = 0;

  if (pshared == 1) {
    type = USYNC_PROCESS;
  } else {
    type = USYNC_THREAD;
  }
  ret = sema_init(sem, value, type, NULL);
#elif LAM_HAVE_POSIX_THREADS
  ret = sem_init(sem, pshared, value);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_sem_wait(lam_sem_t *sem)
{
  int ret = 0;

#if LAM_HAVE_SOLARIS_THREADS
  ret = sema_wait(sem);
#elif LAM_HAVE_POSIX_THREADS
  ret = sem_wait(sem);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_sem_trywait(lam_sem_t *sem)
{
  int ret = 0;

#if LAM_HAVE_SOLARIS_THREADS
  ret = sema_trywait(sem);
#elif LAM_HAVE_POSIX_THREADS
  ret = sem_trywait(sem);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_sem_post(lam_sem_t *sem)
{
  int ret = 0;

#if LAM_HAVE_SOLARIS_THREADS
  ret = sema_post(sem);
#elif LAM_HAVE_POSIX_THREADS
  ret = sem_post(sem);
#else
  ret = -1;
#endif

  return ret;
}


int
lam_sem_destroy(lam_sem_t *sem)
{
  int ret = 0;

#if LAM_HAVE_SOLARIS_THREADS
  ret = sema_destroy(sem);
#elif LAM_HAVE_POSIX_THREADS
  ret = sem_destroy(sem);
#else
  ret = -1;
#endif

  return ret;
}
