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
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: kenyad.c,v 6.7 2002/10/09 20:56:42 brbarret Exp $
 * 
 *	Function:	- OTB process management server
 *			- higher level control and monitoring of processes
 */

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <events.h>
#include <kio.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>

/*
 * global functions
 */
void			psendr();
void			(*(p_init()))();
void			(*(kenyad()))();
void			pwipe();
int			pmatch();
int			pfind();

/*
 * external functions
 */
extern int		kio_intr();
extern void		_lam_atexit();
extern void		childdetach();
extern void		childtrap();
extern void		chldpipe_init();
extern void		pqcreate();		/* kenya requests */
extern void		pqdoom();
extern void		pqstate();
extern void		pqdetach();
extern void		pqattach();

/*
 * local functions
 */
static void		pqspawn();
static void		precvq();

/*
 * local variables
 */
static struct nmsg	nheadq;			/* request message */
static void		(*pqfunc[PREQS])() = {
				pqcreate,
				pqdoom,
				pqstate,
				pqspawn,
				pqdetach,
				pqattach
			};
/*
 * global variables
 */
struct pprocess		ptable[PMAX];	/* the process table */
struct preq		pq;		/* request from client */
struct preply		pr;		/* reply to client */
int			diedfd;		/* end of pipe for queuing SIGCHLDS */
int			diesigfd;	/* end of pipe for queuing SIGCHLDS */

/*
 *	p_init
 *
 *	Function:	- kenyad initialization
 */
void (*(
p_init()))()

{
    struct sigaction	chld;		/* SIGCHLD action */
    struct pprocess	*p;
    int			i;
/*
 * Set up pipe and handler sigchilds.
 */
    chldpipe_init();
    chld.sa_handler = childtrap;
    chld.sa_flags = SA_NOCLDSTOP;
#if LAM_HAVE_SA_RESTART
    chld.sa_flags |= SA_RESTART;
#endif
    sigemptyset(&chld.sa_mask);
    sigaction(SIGCHLD, &chld, (struct sigaction *) 0);

    _lam_atexit(pwipe);
/*
 * Initialize process table.
 */
    for (p = ptable, i = 0; i < PMAX; ++i, ++p) {
	p->p_loadpt = 0;
	p->p_rtf = 0;
	p->p_argc = 0;
	p->p_argv = 0;
	p->p_pid = NOTPROCESS;
	p->p_status = NOTSTATUS;
    }

    precvq();

    return((void (*)()) kenyad);
}

/*
 *	kenyad
 *
 *	Function:	- kenyad main loop
 */
void (*(
kenyad()))()

{
	mttoli4(&pq, (sizeof(pq)
		- sizeof(pq.pq_name) - sizeof(pq.pq_wrkdir)) / sizeof(int4));

	if ((pq.pq_req >= 1) && (pq.pq_req <= PREQS)) {
		(*pqfunc[pq.pq_req - 1])();
	}

	childdetach();
	precvq();
	return((void (*)()) kenyad);
}

/*
 *	precvq
 *
 *	Function:	- receives a kenyad request from a client
 */
static void
precvq()

{
	nheadq.nh_event = EVKENYAD;
	nheadq.nh_type = 0;
	nheadq.nh_flags = 0;
	nheadq.nh_length = sizeof(pq);
	nheadq.nh_msg = (char *) &pq;

	if (nrecv(&nheadq)) lampanic("kenyad (nrecv)");
}

/*
 *	psendr
 *
 *	Function:	- sends a kenyad reply to a client
 *	Accepts:	- kenyad reply ptr
 *			- kenyad reply length
 *			- buffering flag
 */
void
psendr(ppr, length, flags)

char			*ppr;
int4			length;
int4			flags;

{
	struct nmsg	nhead;		/* reply message */

	LAM_ZERO_ME(nhead);
	nhead.nh_node = pq.pq_src_node;
	nhead.nh_event = pq.pq_src_event;
	nhead.nh_type = 0;
	nhead.nh_flags = flags | NREEL;
	nhead.nh_length = length;
	nhead.nh_msg = ppr;

	if (nsend(&nhead)) lampanic("kenyad (nsend)");
}

/*
 *	pfind
 *
 *	Function:	- finds a process by ID
 *			- uses simple linear search
 *	Accepts:	- process ID key
 *	Returns:	- a process descriptor index or ERROR
 */
int
pfind(key_pid)

int4			key_pid;	/* pid to be found */

{
	int		i;		/* favourite index */

	for (i = 0; (i < PMAX) && (ptable[i].p_pid != key_pid); ++i)
	  continue;

	return((i < PMAX) ? i : ERROR);
}

/*
 *	pmatch
 *
 *	Function:	- Does a given process descriptor match the
 *			  request pattern?
 *			- abstracts the vagueries of process specification
 *			  options
 *
 *	Accepts:	- process descriptor ptr
 *			- matching flags
 *			- matching value (meaning varies with flags)
 *
 *	Returns:	- TRUE or FALSE
 */
int
pmatch(p, mflags, mvalue)

struct pprocess		*p;
int4			mflags;
int4			mvalue;

{
	int		matched = FALSE;	/* match? */

	if (p->p_pid == NOTPROCESS) {
		return(FALSE);
	}

	switch (mflags) {

		case SELECT_ALL:
		matched = TRUE;
		break;

		case SELECT_SYS:
		matched = (p->p_rtf & RTF_SYSGEN) ? TRUE : FALSE;
		break;

		case SELECT_APPL:
		matched = (p->p_rtf & RTF_SYSGEN) ? FALSE : TRUE;
		break;

		case SELECT_PID:
		matched = (p->p_pid == mvalue) ? TRUE : FALSE;
		break;

		case SELECT_INDEX:
		--mvalue;
		matched = ((ptable + mvalue) == p) ? TRUE : FALSE;
		break;

		case SELECT_MPI:
		matched = (p->p_rtf & RTF_MPI) ? TRUE : FALSE;
		break;

		default:
		break;
	}

	return(matched);
}

/*
 *	pqspawn
 *
 *	Function:	- now implemented in terms of other services
 */
static void
pqspawn()

{
	pr.pr_reply = ltot(ESERVICE);
	psendr((char *) &pr, (int4) sizeof(pr), 0);
}

/*
 *	pwipe
 *
 *	Function:	- cleans up daemon
 *			- should be a request someday
 */
void
pwipe()

{
	int		i;

	for (i = 0; i < PMAX; ++i) {

		if ((ptable[i].p_pid != NOTPROCESS) &&
				(ptable[i].p_rtf & RTF_FLAT)) {
			unlink(ptable[i].p_loadpt);
		}

		ptable[i].p_loadpt = 0;
		ptable[i].p_rtf = 0;
		ptable[i].p_argc = 0;
		ptable[i].p_argv = 0;
		ptable[i].p_pid = NOTPROCESS;
		ptable[i].p_status = NOTSTATUS;
	}
}

/*
 *	died
 *
 *	Function:	- die pseudo daemon server loop
 */
void (*(
died()))()
{
    return((void (*)()) died);
}

/*
 *	die_init
 *
 *	Function:	- death pseudo daemon initialization
 */
void (*(
die_init()))()
{
    extern void childdeath(void);

    kio_intr(diedfd, childdeath, FALSE);
    return(died());
}
