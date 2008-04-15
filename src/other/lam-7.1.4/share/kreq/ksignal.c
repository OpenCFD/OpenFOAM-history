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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: ksignal.c,v 6.8 2003/11/02 19:48:48 brbarret Exp $
 *
 *	Function:	- signal package
 *			- Any kernel request may return one or more signals.
 *			  A signal is indicated by one bit in a word.  When a
 *			  signal is turned on, it is vectored through a table
 *			  of signal handlers.  By default most signals are
 *			  ignored.  Some cause the process to terminate and a
 *			  few may have meanings particular to specific Trollius
 *			  services, such as the network layer.  Most signals
 *			  may be redirected by the user to user supplied
 *			  handling routines.
 *			- for compatiblity with UNIX, the signals start at 1
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <events.h>
#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t _kio;			/* Kernel Input/Output block */

/*
 * local functions
 */
static void sigign(void);		/* ignore signal */
static void sigexit(void);		/* terminate process */
static void sigflush(void);		/* flush route cache */
static void sigstop(int sig);		/* stop process */

/*
 * local typedef for clean code
 */
typedef void (*lam_void_func)();


/*
 *	_ksig_follow
 *
 *	Function:	- follows signals that may be set in signal word
 */
void
_ksig_follow(void)
{
	int		i;		/* favourite index variable */
	int4		sigbit;		/* signal bit */

	if (_kio.ki_signal) {
/*
 * Check every possible signal.
 */
		for (i = 1; i <= TNSIG; ++i) {
/*
 * If the signal is turned on, call the handling routine.
 * Block the signal from further occurances during handling routine.
 */
			sigbit = sigmask(i);

			if ((_kio.ki_signal & sigbit) &&
				!(_kio.ki_sigmask & sigbit)) {

				_kio.ki_sigmask |= sigbit;
				_kio.ki_signal &= ~sigbit;
				(*(_kio.ki_sig_handlers[i - 1]))(i, 0, 0);
				_kio.ki_sigmask &= ~sigbit;
			}
		}
	}
}

/*
 *	ksignal
 *
 *	Function:	- simple software signal director
 *			- redirects a signal to a user supplied handling
 *			  routine
 *	Accepts:	- signal number
 *			- handler function
 *	Returns:	- previous handler function
 */
lam_void_func 
ksignal(int signo, lam_void_func sigfunc)
{
	lam_void_func oldsigfunc;

	if ((signo < 1) || (signo > TNSIG)) {
		errno = EINVAL;
		return(((void (*)()) -1));
	}

	if (sigmask(signo) & SIG_PRIV) {
		errno = EINVAL;
		return(((void (*)()) -1));
	}

	--signo;
	oldsigfunc = _kio.ki_sig_handlers[signo];

	if (sigfunc == SIG_DFL) {
		_kio.ki_sig_handlers[signo] = _kio.ki_sig_default[signo];
	} else if (sigfunc == SIG_IGN) {
		_kio.ki_sig_handlers[signo] = sigign;
	} else {
		_kio.ki_sig_handlers[signo] = sigfunc;
	}

	return(oldsigfunc);
}

/*
 *	_ksig_init
 *
 *	Function:	- initializes signal handling table
 */
void
_ksig_init(void)
{
	int		i;			/* favourite index variable */
/*
 * Retry all signals except SIGRELEASE.
 * Don't block any signals.
 * No signals are currently pending.
 */
	_kio.ki_sigretry = (int4) ~(sigmask(SIGRELEASE));
	_kio.ki_sigmask = 0;
	_kio.ki_signal = 0;
/*
 * Ignore most signals...
 */
	for (i = 0; i < TNSIG; ++i) {
		_kio.ki_sig_handlers[i] = sigign;
	}
/*
 * ...except for a few special cases.
 */
	_kio.ki_sig_handlers[SIGUDIE - 1] = sigexit;
	_kio.ki_sig_handlers[SIGARREST - 1] = sigstop;
	_kio.ki_sig_handlers[SIGSHRINK - 1] = sigflush;
/*
 * Now copy the current handling table to the default table.
 */
	for (i = 0; i < TNSIG; ++i) {
		_kio.ki_sig_default[i] = _kio.ki_sig_handlers[i];
	}
}

/*
 *	sigflush
 *
 *	Function:	- flushes the route cache
 */
static void
sigflush(void)
{
	int		i;

	_kio.ki_origin = NOTNODEID;

	for (i = 0; i < MAXRCACHE; ++i) {
		_kio.ki_route[i].r_nodeid = NOTNODEID;
	}
}

/*
 *	sigign
 *
 *	Function:	- ignores a signal
 */
static void
sigign(void) 
{
}

/*
 *	sigexit
 *
 *	Function:	- terminates the process
 */
static void
sigexit(void)
{
	kexit(1);
}

/*
 *	sigstop
 *
 *	Function:	- stops or pauses the process
 *			- uses the EVSTOP or EVPAUSE event to indefinitely
 *			  hang until a future signal is received
 *	Accepts:	- signal that caused sigstop to be called
 */
static void
sigstop(int sig)
{
	struct kmsg	forever;		/* local message descriptor */

	forever.k_event = (sig == SIGARREST) ? EVSTOP : EVPAUSE;
	forever.k_type = 0;
	forever.k_flags = 0;
	forever.k_length = 0;
	forever.k_msg = 0;
	krecv(&forever);
}

/*
 *	kpause
 *
 *	Function:	- waits for a signal to occur
 *			- implemented by blocking on an event
 *			- must disable all retry flags while pausing
 *	Returns:	- ERROR
 */
int
kpause(void)
{
	int		save_retry;

	save_retry = _kio.ki_sigretry;
	_kio.ki_sigretry = 0;
	sigstop(0);
	_kio.ki_sigretry = save_retry;
	errno = EINTR;
	return(LAMERROR);
}

/*
 *	ksigsetmask
 *	ksigblock
 *	ksigsetretry
 *	ksigretry
 *
 *	Function:	- operates on signal inhibiting mask and kernel request
 *			  retry flags
 *	Returns:	- old value of mask or flags
 */
int
ksigsetmask(int mask)
{
	int		oldmask;		/* previous signal mask */

	oldmask = _kio.ki_sigmask;
	mask &= ~SIG_PRIV;
	_kio.ki_sigmask = mask;
/*
 * Some pending signals may now be unblocked.
 */
	if (_kio.ki_signal) {
		_ksig_follow();
	}

	return(oldmask);
}

int
ksigblock(int mask)
{
	int		oldmask;		/* previous signal mask */

	oldmask = _kio.ki_sigmask;
	mask &= ~SIG_PRIV;
	_kio.ki_sigmask |= mask;
	return(oldmask);
}

int
ksigsetretry(int flags)
{
	int		oldflags;		/* previous retry flags */

	oldflags = _kio.ki_sigretry;
	flags &= ~(SIG_PRIV | SIGRELEASE);
	_kio.ki_sigretry = flags;
	return(oldflags);
}

int
ksigretry(int flags)
{
	int		oldflags;		/* previous retry flags */

	oldflags = _kio.ki_sigretry;
	flags &= ~(SIG_PRIV | SIGRELEASE);
	_kio.ki_sigretry |= flags;
	return(oldflags);
}

/*
 *	lam_ksignal
 *	lam_ksigsetmask
 *	lam_ksigblock
 *	lam_ksigsetretry
 *	lam_ksigretry
 *	
 *	Function:	- de-rigeur wrappers
 */
lam_void_func
lam_ksignal(int signo, lam_void_func sigfunc)
{ return(ksignal(signo, sigfunc)); }

int lam_kpause() { return(kpause()); }

int lam_ksigsetmask(int mask) { return(ksigsetmask(mask)); }

int lam_ksigblock(int mask) { return(ksigblock(mask)); }

int lam_ksigsetretry(int flags) { return(ksigsetretry(flags)); }

int lam_ksigretry(int flags) { return(ksigretry(flags)); }

/*
 * Fortran wrapper.
 */
#if LAM_FORTRANNOUNDERSCORE
#else
void kpause_()
{
    kpause();
}
#endif
