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
 * $Id: ksignal.h,v 6.7 2004/02/17 02:16:17 jsquyres Exp $
 *
 *	Function:	- defines constants for the software signal package
 *
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#ifndef _KSIGNAL_H
#define _KSIGNAL_H

#include <signal.h>

#include <lam_config.h>
#include <portable.h>

#define LAM_KSIG_DFL	((void (*)(int)) 0)
#define LAM_KSIG_IGN	((void (*)(int)) 1)
#define lam_ksigmask(s)	(1 << ((s) - 1))

#ifndef SIG_DFL
#define SIG_DFL		((void (*)(void)) 0)
#endif

#ifndef SIG_IGN
#define SIG_IGN		((void (*)(void)) 1)
#endif

#ifndef sigmask
#define sigmask(s)	(1 << ((s) - 1))
#endif

#ifndef BADSIG
#define BADSIG		((void (*)(void)) -1)
#endif

/*
 * signals
 */
#define TNSIG		10			/* number of signals */
#define SIGTRACE	1			/* unload trace data */
#define SIGC		2			/* user defined */
#define SIGARECV	3			/* async receive */
#define SIGUDIE		4			/* death */
#define SIGARREST	5			/* stop */
#define SIGRELEASE	6			/* continue stopped process */
#define SIGA		7			/* user defined */
#define SIGB		8			/* user defined */
#define SIGFUSE		9			/* local node about to die */
#define SIGSHRINK	10			/* multicomputer has shrunk */
#define SIG_PRIV	(0)			/* signals which cannot be
						   affected by the user */

#define LAM_SIGTRACE	1			/* unload trace data */
#define LAM_SIGC	2			/* user defined */
#define LAM_SIGARECV	3			/* async receive */
#define LAM_SIGUDIE	4			/* death */
#define LAM_SIGARREST	5			/* stop */
#define LAM_SIGRELEASE	6			/* continue stopped process */
#define LAM_SIGA	7			/* user defined */
#define LAM_SIGB	8			/* user defined */
#define LAM_SIGFUSE	9			/* local node about to die */
#define LAM_SIGSHRINK	10			/* multicomputer has shrunk */

#ifdef __cplusplus
extern "C" {
#endif

extern int		kdoom __ARGS((int pid, int sig));
extern int		kpause __ARGS((void));
extern int4		ksigblock __ARGS((int4 mask));
extern int4		ksigretry __ARGS((int flags));
extern int4		ksigsetmask __ARGS((int4 mask));
extern int4		ksigsetretry __ARGS((int flags));
extern void		(*(ksignal __ARGS((int sig, 
                                           void (*sigfunc)(int sig)))))(int sig);

extern int		lam_kpause __ARGS((void));
extern int4		lam_ksigblock __ARGS((int4 mask));
extern int4		lam_ksigretry __ARGS((int flags));
extern int4		lam_ksigsetmask __ARGS((int4 mask));
extern int4		lam_ksigsetretry __ARGS((int flags));
extern void		(*(lam_ksignal __ARGS((int sig,
                                               void (*sigfunc)(int))))) ();

#ifdef __cplusplus
}
#endif

#endif	/* _KSIGNAL_H */
