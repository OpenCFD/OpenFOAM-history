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
 *	$Id: lammutex.c,v 1.5 2003/04/09 17:37:14 brbarret Exp $
 *
 *	Function:	- LAM interface for mutexes
 */

#include <lam_config.h>

#include <stdio.h>

#include <lamthreads.h>


/*
 *	lam_mutex_init
 *
 *	Function:	- initialize the LAM mutex interface
 *      Accepts:        - pointer to mutex
 */
void
lam_mutex_init(lam_mutex_t *mutex)
{
#if LAM_HAVE_SOLARIS_THREADS
  mutex_init(&(mutex->mutex), NULL, NULL);
#elif LAM_HAVE_POSIX_THREADS
  pthread_mutex_init(&(mutex->mutex), 0);
#else
  fprintf(stderr, "LAM/MPI WARNING: attempt to use mutex without thread support");
#endif
  mutex->thread = (lam_thread_t) -1;
}


/*
 *	lam_mutex_lock
 *
 *	Function:	- lock a LAM mutex
 *      Accepts:        - pointer to mutex
 */
void
lam_mutex_lock(lam_mutex_t *mutex)
{
  if (mutex->thread != (lam_thread_t) -1 &&
      mutex->thread == lam_thread_self()) {
    fprintf(stderr, "LAM/MPI WARNING: Some thread attempting to lock a mutex that it already has!\n");
  }

#if LAM_HAVE_SOLARIS_THREADS
  mutex_lock(&(mutex->mutex));
#elif LAM_HAVE_POSIX_THREADS
  pthread_mutex_lock(&(mutex->mutex));
#else
  fprintf(stderr, "LAM/MPI WARNING: attempt to use mutex without thread support");
#endif
  mutex->thread = lam_thread_self();
}


/*
 *	lam_mutex_trylock
 *
 *	Function:	- lock a LAM mutex if not already locked
 *      Accepts:        - pointer to mutex
 *      Returns:	- 0 or errorcode 
 */
int
lam_mutex_trylock(lam_mutex_t *mutex)
{
  int ret;

  if (mutex->thread != (lam_thread_t) -1 &&
      mutex->thread == lam_thread_self()) {
    fprintf(stderr, "LAM/MPI WARNING: Some thread attempting to lock a mutex that it already has!\n");
  }

#if LAM_HAVE_SOLARIS_THREADS
  ret = mutex_trylock(&(mutex->mutex));
#elif LAM_HAVE_POSIX_THREADS
  ret = pthread_mutex_trylock(&(mutex->mutex));
#else
  fprintf(stderr, "LAM/MPI WARNING: attempt to use mutex without thread support");
  ret = -1;
#endif
  if (ret == 0)
    mutex->thread = lam_thread_self();
  return ret;
}


/*
 *	lam_mutex_is_owner
 *
 *	Function:	- determine if this thread owns the mutex
 *      Accepts:        - pointer to mutex
 *      Returns:        - 1 if this thread owns the mutex, 0 otherwise
 */
int
lam_mutex_is_owner(lam_mutex_t *mutex)
{
  return lam_thread_compare(mutex->thread, lam_thread_self());
}


/*
 *	lam_mutex_unlock
 *
 *	Function:	- unlock a LAM mutex
 *      Accepts:        - pointer to mutex
 */
void
lam_mutex_unlock(lam_mutex_t *mutex)
{
  if (mutex->thread != lam_thread_self()) {
    fprintf(stderr, "LAM/MPI WARNING: Some thread unlocking a mutex that it doesn't own\n");
  } else {
    mutex->thread = (lam_thread_t) -1;
  }

#if LAM_HAVE_SOLARIS_THREADS
  mutex_unlock(&(mutex->mutex));
#elif LAM_HAVE_POSIX_THREADS
  pthread_mutex_unlock(&(mutex->mutex));
#else
  fprintf(stderr, "LAM/MPI WARNING: attempt to use mutex without thread support");
#endif
}


/*
 *	lam_mutex_finalize
 *
 *	Function:	- destroy a LAM mutex
 *      Accepts:        - pointer to mutex
 */
void
lam_mutex_destroy(lam_mutex_t *mutex)
{
  if (mutex->thread != (lam_thread_t) -1) {
    fprintf(stderr, "LAM/MPI WARNING: Some thread destroying a mutex that is in use!\n");
  }

#if LAM_HAVE_SOLARIS_THREADS
  mutex_destroy(&(mutex->mutex));
#elif LAM_HAVE_POSIX_THREADS
  pthread_mutex_destroy(&(mutex->mutex));
#else
  fprintf(stderr, "LAM/MPI WARNING: attempt to use mutex without thread support");
#endif
}
