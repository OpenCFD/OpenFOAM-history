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
 * $Id: lamthreads.c,v 1.3 2003/04/09 17:37:14 brbarret Exp $
 *
 * Function:
 *  - LAM interface for threads control in LAM layer
 */

#include <lam_config.h>

#include <lamthreads.h>


/*
 * TO-DO: Attribute Interface
 */

/*
 * lam_thread_create
 *
 * FUNCTION: Create a thread
 *
 * NOTES:
 *  - Needs attribute argument
 */
int
lam_thread_create(lam_thread_t *new_thread, void *(*start_func)(void *),
		  void *arg)
{
  int ret = 0;

#if LAM_HAVE_POSIX_THREADS
  ret = pthread_create(new_thread, NULL, start_func, arg);
#elif LAM_HAVE_SOLARIS_THREADS
  ret = thr_create(NULL, 0, start_func, arg, NULL, new_thread);
#else
  ret = 1;
#endif

  return ret;
}

/*
 * lam_thread_exit
 *
 * Function:
 *  - terminate calling thread
 *
 * Notes:
 */
void
lam_thread_exit(void *ret_val)
{
#if LAM_HAVE_POSIX_THREADS
  pthread_exit(ret_val);
#elif LAM_HAVE_SOLARIS_THREADS
  thr_exit(ret_val);
#endif
}

/*
 * lam_thread_join
 *
 * Function:
 *  - wait for thread termination
 *
 * Notes:
 */
int
lam_thread_join(lam_thread_t thread, void **value)
{
  int ret;

#if LAM_HAVE_POSIX_THREADS
  ret = pthread_join(thread, value);
#elif LAM_HAVE_SOLARIS_THREADS
  ret = thr_join(thread, NULL, value);
#else
  ret = -1;
#endif

  return ret;
}


/*
 *	lam_thread_self
 *
 *	Function:	- get thread specific identifier
 *      Returns:        - thread specific identifier
 */
lam_thread_t
lam_thread_self(void)
{
#if LAM_HAVE_SOLARIS_THREADS
  return thr_self();
#elif LAM_HAVE_POSIX_THREADS
  return pthread_self();
#else
  return -1;
#endif
}


/*
 *	lam_thread_compare
 *
 *	Function:	- compare two thread IDs
 *	Returns:	- 1 if they are the same, 0 otherwise
 *			- 1 if threading is not enabled (on assumption 
 *			  that there is only one thread)
 */
int
lam_thread_compare(lam_thread_t a, lam_thread_t b)
{
#if LAM_HAVE_SOLARIS_THREADS
  return (a == b);
#elif LAM_HAVE_POSIX_THREADS
  return pthread_equal(a, b);
#else
  return (a == b);
#endif
}
