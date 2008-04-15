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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: kiproc.c,v 6.10 2003/02/27 21:35:40 brbarret Exp $
 * 
 *	Function:	- simple message rendez-vous server
 */

#include <signal.h>
#include <stdlib.h>

#include <all_queue.h>
#include <all_opt.h>
#include <freq.h>
#include <kio.h>
#include <kreq.h>
#include <net.h>
#include <priority.h>
#include <terror.h>

/*
 * global variables
 */
struct kio_t		_kio;			/* kernel I/O block */
struct fclient		_ufd[FUMAX];		/* user file desc. */

/*
 * external variables
 */
extern struct kproc	*pready;		/* top of ready list */
extern struct kproc	*pfree;			/* top of free list */
extern struct kproc	*prun;			/* current client */

/*
 * external functions
 */
extern void		(*bf_init())();
extern void		(*bw_init())();
extern void		(*di_init())();
extern void		(*do_init())();
extern void		(*e_init())();
extern void		(*f_init())();
extern void		(*fl_init())();
extern void		(*l_init())();
extern void		(*p_init())();
extern void		(*r_init())();
extern void		(*tr_init())();
extern void		(*io_init())();
extern void		(*die_init())();
extern void		(*h_init())();
extern void		(*v_init())();

extern void		flwipe();
extern void		kpinsert();
extern void		kpdelete();
extern void		(*(_lam_signal()))();
extern void		pwipe();

/*
 * local variables
 */
static struct {
	void		(*((*kb_run)()))();
	int4		kb_priority;
} kboottbl[] = {
	{ r_init, PRROUTER },
	{ bf_init, PRBUFFERD },
	{ di_init, PRDLI },
	{ do_init, PRDLO },
	{ e_init, PRECHOD },
	{ bw_init, PRBFORWARD },
	{ fl_init, PRDAEMON },
	{ l_init, PRLOADD },
	{ p_init, PRDAEMON },
	{ f_init, PRFILED },
	{ tr_init, PRDAEMON },
	{ io_init, PRDAEMON },
	{ die_init, PRDIED },
	{ h_init, PRHALTD },
	{ v_init, PRVERSIOND },
	{ (void (*((*)()))()) 0, 0 }
};

/*
 * local functions
 */
static void		kbbail();

void
kboot(argc, argv)

int			*argc;
char			*argv[];

{
	int		i = 0;
	struct kproc	*pclient;
	_lam_signal(SIGHUP, kbbail);

	while (kboottbl[i].kb_run != 0) {
/*
 * Allocate and initialize a new process descriptor.
 */
		pclient = pfree;
		kpdelete(&pfree, pfree, pfree);
		pclient->kp_priority = kboottbl[i].kb_priority;
		pclient->kp_run = kboottbl[i].kb_run;
		pclient->kp_pid = 100000 + i;
		pclient->kp_flags = KIPROC;
		pclient->kp_signal = 0;
		pclient->kp_msg.k_event = 0;
		pclient->kp_msg.k_type = 0;
		pclient->kp_state = KSREADY;
		pclient->kp_kqueue = aq_init(KLENQ, sizeof(struct kreq));
		kpinsert(&pready, pclient);
		i++;
	}

	_kio.ki_index = 0;
	_kio.ki_rtf = RTF_SYSGEN | RTF_KERNEL;
	_kio.ki_nodeid = NOTNODEID;
	_kio.ki_origin = NOTNODEID;
	_kio.ki_ionode = NOTNODEID;
	_kio.ki_sigretry = 0;
	_kio.ki_stdin = 0;
	_kio.ki_stdout = 1;
	_kio.ki_stderr = 2;
}

/*
 *	kbgetpid
 *
 *	Returns:	- current running internal process ID
 */
int4
kbgetpid()

{
	return(prun->kp_pid);
}

/*
 *	kbbail
 *
 *	Function:	- cleanup before exiting
 */
static void
kbbail()

{
	flwipe();
	pwipe();
	exit(0);
}
