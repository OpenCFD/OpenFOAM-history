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
 *	$Id: lamthreads.h,v 6.11 2003/12/03 02:00:40 jsquyres Exp $
 *
 *	Function:	- header for LAM thread interface
 */

#ifndef LAM_THREADS_H_
#define LAM_THREADS_H_

#include <lam_config.h>

/*
 * Threads defines
 */
#if LAM_HAVE_SOLARIS_THREADS
#include <thread.h>
#include <synch.h>

typedef thread_t lam_thread_t;
typedef mutex_t lam_real_mutex_t;
typedef cond_t lam_cond_t;
typedef sema_t lam_sem_t;

#elif LAM_HAVE_POSIX_THREADS
#include <pthread.h>
#include <semaphore.h>

typedef pthread_t lam_thread_t;
typedef pthread_mutex_t lam_real_mutex_t;
typedef pthread_cond_t lam_cond_t;
typedef sem_t lam_sem_t;

#else

typedef int lam_thread_t;
typedef int lam_real_mutex_t;
typedef int lam_cond_t;
typedef int lam_sem_t;

#endif

/*
 * types
 */
typedef struct {
  lam_real_mutex_t mutex;
  lam_thread_t thread;
} lam_mutex_t;


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

  /* LAM thread interface */

  int lam_thread_create(lam_thread_t *new_thread, 
                        void *(*start_func)(void *), void *arg);
  void lam_thread_exit(void *ret_val);
  int lam_thread_join(lam_thread_t thread, void **value);
  lam_thread_t lam_thread_self(void);
  int lam_thread_compare(lam_thread_t a, lam_thread_t b);

  /* LAM mutex interface */

  void lam_mutex_init(lam_mutex_t *mutex);
  void lam_mutex_lock(lam_mutex_t *mutex);
  int lam_mutex_trylock(lam_mutex_t *mutex);
  int lam_mutex_is_owner(lam_mutex_t *mutex);
  void lam_mutex_unlock(lam_mutex_t *mutex);
  void lam_mutex_destroy(lam_mutex_t *mutex);

  /* LAM condition variable interface */

  int lam_cond_init(lam_cond_t *cond);
  int lam_cond_wait(lam_cond_t *cond, lam_mutex_t *mutex);
  int lam_cond_signal(lam_cond_t *cond);
  int lam_cond_broadcast(lam_cond_t *cond);
  int lam_cond_destroy(lam_cond_t *cond);

  /* LAM semaphore interface */

  int lam_sem_init(lam_sem_t *sem, int pshared, unsigned int value);
  int lam_sem_wait(lam_sem_t *sem);
  int lam_sem_trywait(lam_sem_t *sem);
  int lam_sem_post(lam_sem_t *sem);
  int lam_sem_destroy(lam_sem_t *sem);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif	/* LAM_THREADS_H_ */
