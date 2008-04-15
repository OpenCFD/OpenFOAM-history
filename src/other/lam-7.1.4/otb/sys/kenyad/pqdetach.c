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
 * $Id: pqdetach.c,v 6.20 2004/01/02 00:19:27 jsquyres Exp $
 * 
 *	Function:	- OTB kenyad
 *			- attaches and detaches kenya processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <t_types.h>
#include <unistd.h>

#include <args.h>
#include <events.h>
#include <kio.h>
#include <laminternal.h>
#include <net.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>
#include <lamdebug.h>
#include <sfh.h>

/*
 * external variables
 */
extern struct pprocess	ptable[];	/* kenya process table */
extern struct preq	pq;		/* kenya request */
extern struct preply	pr;		/* kenya reply */
extern int		diedfd;
extern int		diesigfd;

/*
 * external functions
 */
extern void		psendr();
extern int		pfind();

/*
 * global functions
 */
void			childdetach();
void			childtrap();
void                    pdetachindex(int idetach);
void			pqattach();
void			pqdetach();

/*
 * Help in debugging.  This should generally be 0, because turning it
 * to 1 will activate calls to syslog() in an interrupt call, which
 * isn't generally a Good Idea...
 */
#define WANT_SYSLOG_IN_INTERRUPTS 0

/*
 *	detachindex
 *
 *	Function:	- detaches a process identified by table index
 *	Accepts:	- table index
 */
static void
detachindex(int idetach)
{
	struct nmsg	nhead;
	struct preply	reply;			/* kenyad reply */
	struct pprocess	*pdetach;
	int		found = FALSE;		/* found load module flag */
	int		i;

	pdetach = ptable + idetach;
/*
 * Flush IO.
 */
	if (pdetach->p_rtf & RTF_IO) {
	    fflush(stderr);
	    fflush(stdout);
	}

	LAM_ZERO_ME(nhead);
	if (pdetach->p_rtf & RTF_WAIT) {
/*
 * Reply to the parent process. This may not be the process which made the
 * kenyad request so we must save and restore the requester's synchronization.
 */
	    nhead.nh_node = pdetach->p_nodeid;
	    nhead.nh_event = pdetach->p_event & 0xBFFFFFFF;
	    nhead.nh_type = 1;
	    nhead.nh_flags = NREEL;
	    nhead.nh_length = sizeof(reply);
	    nhead.nh_msg = (char *) &reply;

	    reply.pr_nodeid = ltot(getnodeid());
	    reply.pr_reply = ltot(pdetach->p_status);
	    reply.pr_pid = ltot(pdetach->p_pid);

	    if (nsend(&nhead))
		lampanic("kenyad (nsend)");
	}
/*
 * Detach target process.
 */
	pdetach->p_pid = NOTPROCESS;
	pdetach->p_status = NOTSTATUS;

	if (pdetach->p_argc) {
	    for (i = 0; i < pdetach->p_argc; i++)
		free((char *) pdetach->p_argv[i]);
	    free((char *) pdetach->p_argv);
	}
/*
 * Check to see if anybody else is using the load module.
 */
	if (pdetach->p_rtf & RTF_FLAT) {
	    for (i=0; (i<PMAX) && !found; i++) {
		if ((ptable[i].p_pid != NOTPROCESS) && ((ptable + i) != pdetach)
		        && (!strcmp(ptable[i].p_loadpt, pdetach->p_loadpt))) {

		    found = TRUE;
		}
	    }

	    if (!found) {
/*
 * Remove the load module file and free the filename buffer.
 */
		unlink(pdetach->p_loadpt);
		lamlog("kenyad: removing load module \"%s\"",
			pdetach->p_loadpt);
	    }
	}

	free(pdetach->p_loadpt);
}

/*
 *	pdetachindex
 *
 *	Function:	- detaches a process identified by table index
 *	Accepts:	- table index
 */
void
pdetachindex(int idetach)
{
	ptable[idetach].p_status = pq.pq_status;
	detachindex(idetach);
}

void
pqdetach()

{
	int		idetach;	/* descriptor index of target */
/*
 * Find the process descriptor for the given identifier.
 */
	idetach = pfind(pq.pq_pid);
	pr.pr_reply = (idetach < 0) ? ltot(ENOTPROCESS) : 0;
/*
 * Send reply of detach operation.
 */
	psendr((char *) &pr, sizeof(pr), 0);

	/*
	 * If the flatd created a temporary file for this process,
	 * delay the deatch action because some OS's (cough cough
	 * HP-UX cough cough) won't allow unlink()'ing of files while
	 * they are running.  Specifically, we may have received this
	 * message *while the program is still running*.  i.e., the
	 * process did an nsend to send us this message, but it's
	 * still running -- hence, trying to unlink it could result in
	 * an "text file busy" error.  So defer detaching it for now,
	 * and let the died catch it when it actually dies.
	 */

	if (pr.pr_reply == 0) {
	    ptable[idetach].p_status = pq.pq_status;
	    if (!(ptable[idetach].p_rtf & RTF_FLAT)) {
		lamlog("kenyad: did not start this process with the flatd; "
			"detaching now");
		detachindex(idetach);
	    } else {
		lamlog("kenyad: started this process with the flatd; "
			"deferring detaching until later");
	    }
	}
}

/*
 *	pqattach
 *
 *	Function:	- attaches a process to kenyad
 */
void
pqattach()

{
	struct pprocess	*pattach;	/* new process ptr */
	int		iattach;	/* index of new process */

	/* Is the process already in the table?  Processes may already
	   be in the table if the kenyad forked the process, but they
	   not have explicitly attached themselves to the table with
	   lpattach() (e.g., mpirunning a shell script).  */

	iattach = pfind(pq.pq_pid);

	if (iattach >= 0) {
		pr.pr_reply = 0;
		pattach = ptable + iattach;
		pattach->p_rtf = pq.pq_rtf & ~RTF_KENYA_CHILD;
		pattach->p_rtf |= RTF_KENYA_ATTACH;
/*
 * If the request doesn't specify a new name, ignore the name.
 */
		if (pq.pq_name[0] != '\0') {
			free(pattach->p_argv[0]);
			pattach->p_argv[0] = malloc((unsigned)
					strlen(pq.pq_name) + 1);
			if (pattach->p_argv[0] == 0)
					lampanic("kenyad (malloc)");
			strcpy(pattach->p_argv[0], pq.pq_name);
			pattach->p_argc = 1;
		}
		else
			pattach->p_argc = 0;
	}
/*
 * Find a slot in the process table.
 */
	else if ((iattach = pfind((int4) NOTPROCESS)) < 0) {
		pr.pr_reply = ltot(ENOPDESCRIPTORS);
	}
/*
 * Fill in new kenya descriptor.
 */
	else {
		pattach = ptable + iattach;

		pattach->p_loadpt = malloc((unsigned) strlen(pq.pq_name) + 1);
		if (pattach->p_loadpt == 0) lampanic("kenyad (malloc)");

		strcpy(pattach->p_loadpt, pq.pq_name);

		if (pq.pq_name[0] != '\0') {
			pattach->p_argc = 1;
			pattach->p_argv = sfh_argv_break(pq.pq_name, 0xa);
			if (pattach->p_argv == 0)
					lampanic("kenyad (pack_argv)");
		}
		else {
			pattach->p_argc = 0;
		}

		pattach->p_pid = pq.pq_pid;
		pattach->p_rtf = pq.pq_rtf | RTF_KENYA_ATTACH;
		pattach->p_nodeid = NOTNODEID;
		pattach->p_event = NOTEVENT;
		pr.pr_reply = 0;
	}
}

/*
 *	childtrap
 *
 *	Function:	- traps child process termination
 *			- stores exit status in process table
 *	Note:		- this is a signal handler so avoid if possible
 *			  calling non signal-safe functions like syslog
 *			  and printf
 */
void
childtrap()
{
    int			i;		/* index in process table */
    int			pid;		/* child process ID */
    int			status;		/* child exit status */
    int4		lamstatus = LAMERROR; /* equivalent LAM exit status */

    while (1) {
	pid = waitpid(-1, &status, WNOHANG);
		
	if (pid <= 0) {
#if WANT_SYSLOG_IN_INTERRUPTS
	    if (errno != ECHILD && errno != EINVAL)
		lamlog("kenyad: got error for waitpid(), errno %d", errno);
#endif
	    return;
	}

	i = pfind((int4) pid);

	if (i >= 0) {
	    if (WIFSIGNALED(status)) {
#if WANT_SYSLOG_IN_INTERRUPTS
	      lamlog("kenyad: child pid %d died due to signal %d before detaching, with status of %d", pid, WTERMSIG(status), status);
#endif
	      lamstatus = WTERMSIG(status);
	      lamstatus |= _LAM_WIFSIGNALED;
	    } else if (WIFEXITED(status)) {
#if WANT_SYSLOG_IN_INTERRUPTS
	      lamlog("kenyad: child pid %d died with exit code %d before detaching, with status of %d", pid, WEXITSTATUS(status), status);
#endif
	      lamstatus = WEXITSTATUS(status);
	    } else {
		/* should never happen */
#if WANT_SYSLOG_IN_INTERRUPTS
		lamlog("kenyad: child pid %d died with unexpected status of %d",
			pid, status);
#endif
	    }

	    ptable[i].p_status = lamstatus;
	    if (ptable[i].p_rtf & RTF_KENYA_ATTACH)
	      ptable[i].p_status |= _LAM_WIFNODETACH;
#if WANT_SYSLOG_IN_INTERRUPTS
	    lamlog("kenyad: sending back status: %d", ptable[i].p_status);
#endif

	    if (diesigfd >= 0)
		mwrite(diesigfd, (char *)&i, 1);
	} 
    }
}

/*
 *	childdetach
 *
 *	Function:	- detaches child process indicated via SIGCHLD
 */
void
childdetach()

{
	int		i;

	for (i = 0; i < PMAX; ++i) {
		if (ptable[i].p_status != NOTSTATUS) {
			detachindex(i);
		}
	}
}

/*
 *	childdeath
 *
 *	Function:	- handle child death
 *			- this is only used when kenyad is internal
 */
void
childdeath(void)
{
	int	i;

	read(diedfd, &i, sizeof(i));
	lamlog("died: caught child death; trying to detach");

	for (i = 0; i < PMAX; ++i) {
	    if (ptable[i].p_status != NOTSTATUS) {
		lamlog("died: detaching table entry %d", i);
		detachindex(i);
	    }
	}
}
