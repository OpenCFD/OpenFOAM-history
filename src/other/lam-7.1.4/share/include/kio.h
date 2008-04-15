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
 *	$Id: kio.h,v 6.10 2002/10/09 20:57:23 brbarret Exp $
 *
 *	Function:	- defines templates and constants for the
 *			  Kernel Input Output block
 */

#ifndef _KIO
#define _KIO

#include <kreq.h>
#include <ksignal.h>
#include <ktrace.h>
#include <portable.h>
#include <rreq.h>

#define MAXRCACHE	29		/* max # of cached routes */
#define	TNEXIT		32		/* maximum number of exit routines */

/*
 * Runtime Flags
 */
#define RTF_KERNEL	0x000001	/* attached to kernel? */
#define RTF_KENYA_CHILD	0x000002	/* child of the kenyd? */
#define RTF_KENYA_ATTACH 0x000004	/* attached to kenya? */
#define RTF_SYSGEN	0x000008	/* system process? */
#define RTF_WAIT	0x000010	/* background process? */
#define RTF_MPIGER	0x000020	/* guaranteed resources protocol */
#define RTF_DEBUG	0x000040	/* for debuggers */
#define RTF_MPIC2C	0x000080	/* direct client to client comm. */
#define RTF_TRACE	0x000100	/* traced process? */
#define RTF_TRSWITCH	0x000200	/* trace on/off switch */
#define RTF_TRON	0x000300	/* combination of TRACE & TRSWITCH */
#define RTF_MPI		0x000400	/* I am an MPI process. */
#define RTF_FLAT	0x000800	/* loaded by flatd? */
#define RTF_MPIRUN	0x001000	/* started by mpirun? */
#define RTF_HOMOG	0x002000	/* homogeneous cluster */
#define RTF_APPWD	0x004000	/* change directory to applications */
#define RTF_IO		0x008000	/* stdio redirection */
#define RTF_PFDIO	0x010000	/* stdio via passed file descriptors */
#define RTF_TTYOUT	0x020000	/* stdout is ultimately a tty */
#define RTF_PTYS	0x040000	/* use ptys */
#define RTF_CWD		0x080000	/* change directory to cwd */
#define RTF_MPISIGS	0x100000	/* catch signals in MPI applications */
#define RTF_FORKED	0x200000	/* process forked kenyad isn't parent */
#define RTF_TRYCWD	0x400000	/* try to change working dir */
#define RTF_IMPI        0x800000        /* IMPI-enabled process */

struct jobid_t {
	int4		jid_node;	/* nodeid() of job */
	int4		jid_pid;	/* getpid() of job */
};

/*
 * Kernel Interface
 */
struct kio_t {
	int4		ki_priority;	/* my kernel process priority */
	int4		ki_rtf;		/* Run Time Flags */
	int4		ki_pid;		/* kernel registered process ID */
	int4		ki_signal;	/* pending signals */
	int4		ki_sigmask;	/* signal masks */
	int4		ki_sigretry;	/* flags to retry after EINTR */
	int4		ki_index;	/* fast identification in kernel */
	int4		ki_nodeid;	/* my node identifier */
	int4		ki_origin;	/* origin node identifier */
	struct jobid_t	ki_jobid;	/* job identifier */
	int4		ki_ionode;	/* standard io node */
	int4		ki_stdin;	/* standard input descriptor */
	int4		ki_stdout;	/* standard output descriptor */
	int4		ki_stderr;	/* standard error descriptor */
/*
 * Signal Handling Table
 */
	void		((*(ki_sig_handlers[TNSIG]))());
	void		((*(ki_sig_default[TNSIG]))());
/*
 * at exit table
 */
	void		((*(ki_exit[TNEXIT]))());
	char		*ki_arg[TNEXIT];
/*
 * route cache
 */
	struct route	ki_route[MAXRCACHE];
/*
 * trace support
 */
	struct trhead	*ki_trhead;	/* trace buffer headers */
	int4		ki_trcurr;	/* current trace buffer index */
	int4		ki_tid;		/* trace process identifier */

	int4		ki_world;	/* expected world population */
	int4		ki_parent;	/* parent group population */
/*
 * debug support
 */
	int4		ki_fyi[KNFYI];	/* debugging information */
	double		ki_blktime;	/* time spent blocking */
};

#endif
