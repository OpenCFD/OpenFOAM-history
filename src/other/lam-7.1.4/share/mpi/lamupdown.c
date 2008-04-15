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
 * $Id: lamupdown.c,v 6.25 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- take top level info down
 *			- take error info up
 */

#include <lam_config.h>

#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <blktype.h>
#include <lam_config.h>
#include <lammpithreads.h>
#include <all_array.h>

/*
 * global functions
 */
void lam_printfunc(void);


/*
 * global variables
 */
int lam_topfunc = 0;
int lam_toproot;
int lam_toprootgps;


/*
 * private data
 */
static lam_array_t *stack = NULL;
static int *func_stack = NULL;
static int stack_size = 0;


/*
 *      lam_updown_init
 *
 *      Function:       - initialize the updown interface
 */
int
lam_updown_init(void)
{
  stack = lam_arr_init(sizeof(int), NULL);
  if (stack == NULL)
    return LAMERROR;

  return 0;
}


/*
 *      lam_updown_reset_thr_level
 *
 *      Function:       - called during MPI_INIT if we end up running at
 *                        something higher than MPI_THREAD_SINGLE
 */
int
lam_updown_reset_thr_level(void)
{
#if LAM_HAVE_THREADS
  if (lam_mpi_thread_level >= MPI_THREAD_SERIALIZED) {
    lam_mutex_lock(&lam_mpi_mutex);
  }
#endif

  return 0;
}


/*
 *      lam_updown_finalize
 *
 *      Function:       - finalize the updown interface
 */
int
lam_updown_finalize(void)
{
  if (stack != NULL) {
    lam_arr_free(stack);
    stack = NULL;
  }

  return 0;
}


/*
 *	lam_setfunc
 *
 *	Function:	- set top level function
 *	Accepts:	- local function type
 */
void
lam_setfunc(int locfunc)
{
  /* If we have not been initialized yet, then call lamupdown_init()
     to initialize the stack.  We're within MPI_Init/MPI_Init/thread
     here, so we know that there are no other threads active, so this
     is safe to do.  Also, the MPI thread level defaults to
     MPI_THREAD_SINGLE, so we don't have to worry about the locks in
     here yet.  If they get reset, MPI_Init* will do the Right Things
     before calling lam_resetfunc().  */

  if (stack == NULL)
    if (lam_updown_init() == LAMERROR)
      return;

#if LAM_HAVE_THREADS

  /* There are several places where MPI functions call each other --
     we want to allow this.  But we can't let other threads in until
     this one finishes.  The lam_mpi_mutex will be released in
     lam_reset_func(). */

  if (lam_mpi_thread_level == MPI_THREAD_SERIALIZED) {
    if (!lam_mutex_is_owner(&lam_mpi_mutex)) {
      lam_mutex_lock(&lam_mpi_mutex);
    }
  }

#endif

  /* Save the function in the next available slot */

  ++stack_size;
  lam_arr_append(stack, &locfunc);
  func_stack = lam_arr_get(stack);
  if (stack_size == 1)
    ((struct _fyiproc *) _kio.ki_fyi)->fyp_func = locfunc;
  lam_topfunc = locfunc;
}


/*
 *	lam_resetfunc
 *
 *	Function:	- reset function at top level
 *	Accepts:	- local function type
 */
void
lam_resetfunc(int locfunc)
{
  /* We may have had an error, and therefore the stack was nuked.  So
     be sure to only do this if there is something on the stack */

  if (stack_size == 0)
    return;

  lam_arr_remove_index(stack, stack_size - 1);
  --stack_size;

  if (stack_size == 0)
    lam_topfunc = ((struct _fyiproc *) _kio.ki_fyi)->fyp_func = 0;
  else
    lam_topfunc = func_stack[stack_size - 1];

#if LAM_HAVE_THREADS

  /* There are several places where MPI functions call each other --
     we want to allow this.  But we can't let other threads in until
     this one finishes.  lam_mpi_mutex was locked in lam_setfunc().

     Special case: if we're resetting and we're in BLKMPIINITTHREAD
     and the mutex owner is -1, don't try to unlock it.  See
     MPI_Init_thread for the reason why. */

  if (lam_mpi_thread_level == MPI_THREAD_SERIALIZED) {
    if (lam_mutex_is_owner(&lam_mpi_mutex) && stack_size == 0) {
      lam_mutex_unlock(&lam_mpi_mutex);
    }
  }
#endif
}


/*
 *	lam_numfuncs
 *
 *	Function:	- get the size of the stack
 *	Returns:	- size of the stack
 */
int
lam_numfuncs(void)
{
  return stack_size;
}


/*
 *	lam_getfunc
 *
 *	Function:	- get top level function
 *	Returns:	- top level function
 */
int
lam_getfunc(void)
{
  return (stack_size > 0) ? func_stack[stack_size - 1] : 0;
}


/*
 *	lam_setparam
 *
 *	Function:	- set top level parameters
 *	Accepts:	- local function type
 *			- root global/local rank (collective comm.)
 *			- root node/index GPS (collective comm.)
 */
void
lam_setparam(int locfunc, int root, int rootgps)
{
  if (lam_topfunc == locfunc) {
    lam_toproot = root;
    lam_toprootgps = rootgps;
  }
}


/*
 *	lam_getparam
 *
 *	Function:	- get top level parameters
 *	Accepts:	- ptr root ranks
 *			- ptr root GPS
 */
void
lam_getparam(int *proot, int *prootgps)
{
  *proot = lam_toproot;
  *prootgps = lam_toprootgps;
}


/*
 *	lam_mkerr
 *
 *	Function:	- form an error code
 *	Accepts:	- error class
 *			- error value
 *	Returns:	- encoded error code
 */
int
lam_mkerr(int class, int error)
{
  int errcode;

  /* The returned value is encoded as follows:

  - top 15 bits: error code
  - next 9 bits: function enumeration (there are over 300 MPI
    functions, and it takes 9 bits to represent all of them)
  - lowest 8 bits: error class
  */

  errcode = ((error & 0x7FFF) << 9) | (lam_topfunc & 0x1FF);
  errcode = (errcode << 8) | (class & 0xFF);

  return(errcode);
}


/*
 *	lam_bkerr
 *
 *	Function:	- break error code into components
 *	Accepts:	- error code
 *			- ptr class (returned value)
 *			- ptr function (returned value)
 *			- ptr error (returned value)
 */
void
lam_bkerr(int errcode, int *class, int *func, int *error)
{
  /* As setup in lam_mkerr() above:

     - the error class is the lower 8 bits
     - the function enumeration is the next 9 bits
     - the error code is the top 15 bits
  */

  *class = errcode & 0xFF;
  errcode >>= 8;
  *func = errcode & 0x1FF;
  errcode >>= 9;
  *error = errcode & 0x7FFF;
}


/*
 *	lam_errfunc
 *
 *	Function:	- handle MPI errors according to error mode
 *			- pass error up to top level
 *			- call error handler at top level
 *	Accepts:	- communicator
 *			- local function type
 *			- error code
 *	Returns:	- error code
 */
int
lam_errfunc(MPI_Comm errcomm, int locfunc, int errcode)
{
	MPI_Comm	comm;			/* communicator */
	int		class;			/* error class */
	int		func;			/* function type */
	int		error;			/* errno value */
/*
 * This catches some cases where errors are returned in F77 wrappers
 * before the call is made to the C version of the MPI function.  
 */
	lam_initerr_m();

	lam_bkerr(errcode, &class, &func, &error);

	if (func == locfunc) {

		comm = (errcomm) ? errcomm : MPI_COMM_WORLD;

		if (comm->c_window) {
			return(lam_err_win(comm->c_window, class, error, ""));
		} else {
			return(lam_err_comm(comm, class, error, ""));
		}
	}

	return(errcode);
}


/*
 *	lam_printfunc
 *
 *	Function:	- print out the call stack without destroying it
 */
void
lam_printfunc(void)
{
  int i, myrank;

  if (stack_size > 0) {
    myrank = lam_myproc->p_gps.gps_grank;
    printf("Rank (%d, MPI_COMM_WORLD): Call stack within LAM:\n", myrank);
    for (i = --stack_size; i >= 0; i--)
      printf("Rank (%d, MPI_COMM_WORLD):  - %s()\n", myrank, 
	     blktype(func_stack[i]));
    printf("Rank (%d, MPI_COMM_WORLD):  - main()\n", myrank);
  }
}


/*
 *	lam_nukefunc
 *
 *	Function:	- reset top function (cleanup)
 */
void
lam_nukefunc(void)
{
  lam_topfunc = 0;
  ((struct _fyiproc *) _kio.ki_fyi)->fyp_func = 0;
  while (stack_size > 0) {
    lam_arr_remove_index(stack, 0);
    --stack_size;
  }
}


/*
 *	lam_err_comm
 *
 *	Function:	- handle MPI error on a communicator
 *	Accepts:	- communicator
 *			- error class
 *			- errno value
 *			- error message
 *	Returns:	- error class
 */
int
lam_err_comm(MPI_Comm comm, int errclass, int error, char *errmsg)
{
/*
 * Catch the uninitialized case when an error is returned in an F77
 * wrapper before the call is made to the C version of the function.  
 */
	lam_initerr_m();
/*
 * If the top function is zero then the error has already been handled.
 */
	if (lam_topfunc == 0) {
	    return(errclass);
	}

	if (comm == MPI_COMM_NULL) {
		comm = MPI_COMM_WORLD;
	}
/*
 * Compatibility with lam_errfunc() style error handling.  Check if the
 * errorclass has encoded information and if so break it down.
 */
	if (errclass & 0xFFFFFE00) {
		error = (errclass >> 17) & 0x7FFF;
		errclass &= 0xFF;
	}
/*
 * If this is a window communicator invoke the window error handler.
 */
	if (comm->c_window) {
		return(lam_err_win(comm->c_window, errclass, error, errmsg));
	}
/*
 * Invoke communicator error handler taking care with language calling
 * convention.
 */
	if (comm->c_errhdl->eh_flags & LAM_LANGF77) {
		(comm->c_errhdl->eh_func)(&comm->c_f77handle,
						&errclass, error, errmsg);
	} else if (comm->c_errhdl->eh_func) {
		(comm->c_errhdl->eh_func)(&comm, &errclass, error, errmsg);
	} else {
		lam_comm_errfatal(&comm, &errclass, error, errmsg);
	}

	lam_nukefunc();

	return(errclass);
}


/*
 *	lam_err_win
 *
 *	Function:	- handle MPI error on a window
 *	Accepts:	- window
 *			- error class
 *			- errno value
 *			- error message
 *	Returns:	- error class
 */
int
lam_err_win(MPI_Win win, int errclass, int error, char *errmsg)
{
/*
 * Catch the uninitialized case when an error is returned in an F77
 * wrapper before the call is made to the C version of the function.  
 */
	lam_initerr_m();
/*
 * If the top function is zero then the error has already been handled.
 */
	if (lam_topfunc == 0) {
	    return(errclass);
	}
/*
 * Compatibility with lam_errfunc() style error handling.  Check if the
 * errorclass has encoded information and if so break it down.
 */
	if (errclass & 0xFFFFFF00) {
		error = (errclass >> 16) & 0xFFFF;
		errclass &= 0xFF;
	}

	if (win->w_errhdl->eh_flags & LAM_LANGF77) {
		(win->w_errhdl->eh_func)(&win->w_f77handle,
					&errclass, error, errmsg);
	} else if (win->w_errhdl->eh_func) {
		(win->w_errhdl->eh_func)(&win, &errclass, error, errmsg);
	} else {
		lam_win_errfatal(&win, &errclass, error, errmsg);
	}

	lam_nukefunc();
	
	return(errclass);
}
