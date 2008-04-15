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
 *	$Id: preq.h,v 6.11 2003/03/10 18:05:27 brbarret Exp $
 *
 *	Function:	- constants and templates for the
 *			  process control server (aka kenya)
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#ifndef _PREQ
#define _PREQ

#include "kio.h"
#include "kreq.h"
#include "portable.h"

/*
 * Events recogized by the kenyad
 */
#define PQCREATE	1			/* create a process in place */
#define PQDOOM		2			/* doom a process */
#define PQSTATE		3			/* find state of a process */
#define PQSPAWN		4			/* spawn an existing process */
#define PQDETACH	5			/* detach an existing process */
#define PQATTACH	6			/* attach an existing process */
#define PQDEATHNOTICE   7                       /* The kenyad signal
                                                 *  handler got a
                                                 *  SIGCHLD, and sent
                                                 *  us a report.
                                                 *  SHOULD NOT BE USED
                                                 *  OUTSIDE THE KENYAD
                                                 *  AND ITS SIGNAL
                                                 *  HANDLER FOR
                                                 *  SIGCHLD! 
						 */
#define PREQS		7			/* number of kenya requests */

#ifndef NOTPROCESS
#define NOTPROCESS	0x80000000
#endif
#define NOTSTATUS	0x80000000
#define PSMAXNAME	256
#define PMAX		256

/*
 * process request selection flags
 */
#define SELECT_PID	0x01
#define SELECT_INDEX	0x02
#define SELECT_MPI	0x04
#define SELECT_ALL	0x10
#define SELECT_SYS	0x20
#define SELECT_APPL	0x40

/*
 * exit status handling
 */
#define _LAM_WIFSIGNALED	0x10000
#define _LAM_WIFNODETACH	0x20000
#define LAM_WIFSIGNALED(s)	((s) & _LAM_WIFSIGNALED)
#define LAM_WIFNODETACH(s)	((s) & _LAM_WIFNODETACH)
#define LAM_WTERMSIG(s)		((s) & 0xFFFF)
#define LAM_WEXITSTATUS(s)	LAM_WTERMSIG(s)

/*
 * kenyad request
 */
struct preq {
	int4		pq_src_node;
	int4		pq_src_event;
	int4		pq_req;
	int4		pq_sel_method;		/* process selection method */
	int4		pq_sel_value;		/* process selection value */
	int4		pq_loadpt;		/* load module flat tag */
	int4		pq_rtf;			/* process runtime flags */
	int4		pq_status;		/* exit status */
	int4		pq_signal;		/* deliver these signals */
	int4		pq_maxproc;		/* limit processes affected */
#define pq_rank		pq_maxproc		/* process rank (-1 if none) */
	int4		pq_argv;		/* argument/env structure tag */
	int4		pq_envsize;		/* size of environemnt */
	int4		pq_nodeid;		/* spawn target nodeid */
	int4		pq_pid;			/* detach this process ID */
	struct jobid_t	pq_jobid;		/* job identifier */
	int4		pq_stdin;		/* std input desc. */
	int4		pq_stdout;		/* std output desc. */
	int4		pq_stderr;		/* std error desc. */
	int4		pq_world;		/* world population */
	int4		pq_parent;		/* parent group population */
	char		pq_name[PSMAXNAME];	/* program name */
	char		pq_wrkdir[LAM_PATH_MAX];/* program working dir */
};

/*
 * kenyad reply
 */
struct preply {
	int4		pr_reply;
	int4		pr_pid;			/* kenyad process ID */
	int4		pr_index;		/* created process index */
	int4		pr_nodeid;		/* kenyad node ID */
};

/*
 * kenayd process descriptor
 */
struct pprocess {
	int4		p_pid;
	char		*p_loadpt;
	int4		p_argc;
	char		**p_argv;
	int4		p_rtf;
	struct jobid_t	p_jobid;
	int4		p_nodeid;		/* parent node ID */
	int4		p_event;		/* parent's reply event */
	int4		p_status;
};

/*
 * kenyad reply for the PQSTATE request
 */
struct pstate {
	int4		ps_reply;
	int4		ps_index;
	int4		ps_pid;
	int4		ps_rtf;
	int4		ps_nodeid;
	int4		ps_event;
	struct kstatus	ps_kernel;
	char		ps_name[PSMAXNAME];
	char		ps_loadpt[PSMAXNAME];
};

/*
 * kenyad process environment
 */
struct penv {
	int4		pe_rank;
	char		**pe_envv;
	char		pe_wrkdir[LAM_PATH_MAX];
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		lpattach(const char *name);
extern int		lpdetach(int pid, int status);
extern int		rpcreate(int4 node, const char *file, int4 rtf,
					char **argv, int4 *pid, int4 *idx);
extern int		rpcreatev(int4 node, const char *file, int4 rtf,
					char **argv, struct penv *env,
    					int4 *pid, int4 *idx);
extern int		rpdoom(int4 node, int4 sflags,
					int4 svalue, int4 signal);
extern int		rpgo(int4 node, int4 loadtag, int4 rtf,
					int4 arvtag, int4 *pid, int4 *idx);
extern int		rpgov(int4 node, int4 loadtag, int4 rtf,
					int4 arvtag, int4 envsize, char *wrkdir,
    					int4 rank, int4 *pid, int4 *idx);
extern int		rploadgo(int4 srcnode, int4 destnode,
					const char *file, int4 rtf,
					char **argv, int4 *pid, int4 *idx);
extern int		rploadgov(int4 srcnode, int4 destnode,
					const char *file, int4 rtf,
					char **argv, struct penv *envv,
    					int4 *pid, int4 *idx);
extern int		rpspawn(int4 srcnode, int4 destnode,
					int4 sflags, int4 sval, char **argv,
					int4 *pid, int4 *idx);
extern int		rpwait(int4 *node, int *pid, int *status);
extern int4		rpstate(int4 node, int4 sflags, int4 sval,
					struct pstate *table, int4 maxproc);

#ifdef __cplusplus
}
#endif

#endif
