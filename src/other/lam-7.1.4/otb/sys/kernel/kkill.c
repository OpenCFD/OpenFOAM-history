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
 * $Id: kkill.c,v 6.9 2003/05/21 19:16:15 jsquyres Exp $
 * 
 *	Function:	- supports client cleanup by the kernel and by tkill(1)
 *			- OTB specific code
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
               
#include <kreq.h>
#include <typical.h>
#include <priority.h>

/*
 * global functions
 */
void kkillall(void);
int kkillclose(void);
int kkillopen(char *f_kill);
int kkillrewrite(struct kproc kptable[]);
int kkillwrite(int4 pid);


/*
 * external variables
 */
extern struct kproc	*pready;
extern struct kproc	*pblock;


/*
 * local variables
 */
static FILE		*fp_kill;	/* kill file ptr */
static int		fd_kill;	/* kill file desc. */


/*
 *	kkillall
 *
 *	Function:	- catches SIGTERM and SIGHUP
 *			- kills all attached processes
 */
void
kkillall(void)
{
	struct kproc	*p;

	for (p = pready; p; p = p->kp_next) {

		if (!(p->kp_flags & KIPROC)) {
			kill(p->kp_pid, SIGCONT);
			kill(p->kp_pid, SIGHUP);
		}
	}

	for (p = pblock; p; p = p->kp_next) {

		if (!(p->kp_flags & KIPROC)) {
			kill(p->kp_pid, SIGCONT);
			kill(p->kp_pid, SIGHUP);
		}
	}
}


/*
 *	kkillopen
 *
 *	Function:	- opens the kill file
 *			- locates the kill directory and constructs
 *			  the full kill pathname by appending the
 *			  userid to the kill directory
 *			- writes the kernel's own pid in the first
 *			  entry of the kill file
 *	Accepts:	- kill filename
 *	Returns:	- 0 or LAMERROR
 */
int
kkillopen(char *f_kill)
{
	fd_kill = open(f_kill, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd_kill < 0)
	  return(LAMERROR);

	/* Make this be close-on-exec so that forked children don't
	   inherit it.  Here's a corner case where it matters: the LAM
	   session directory is actually located on NFS (e.g.,
	   /tmp/lam-username@hostname is on NFS).  When the user
	   invokes lamhalt, which invokes the haltd, which forks off
	   tkill, tkill will go unlink all files in the session
	   directory.  Unfortunately, tkill will have inherited this
	   open file, which is now just removed.  Hence, when tkill
	   tries to remove the directory itself, NFS will complain
	   that the directory is not empty because NFS will
	   automatically create a .nfsXXXXX cache/placeholder file in
	   the directory when the kill file was unlinked but someone
	   (tkill) still had it open.  End result: the session
	   directory is not removed.  And this is bad.  I'm annoyed
	   because it took me so long to track this down! */

	if (fcntl(fd_kill, F_SETFD, 1) != 0)
	  return LAMERROR;

	fp_kill = fdopen(fd_kill, "w");

	if (! fp_kill)
	  return(LAMERROR);
	
	setvbuf(fp_kill, (char *) 0, _IOLBF, 0);
	
	if (fprintf(fp_kill, "%d\n", (int) getpid()) == EOF)
	  return(LAMERROR);
	
	return(0);
}


/*
 *	kkillwrite
 *
 *	Function:	- writes pid into lock file
 *	Accepts:	- pid
 *	Returns:	- 0 or LAMERROR
 */
int
kkillwrite(int4 pid)
{
	if (fprintf(fp_kill, "%d\n", pid) == EOF) {
		return(LAMERROR);
	} else {
		return(0);
	}
}


/*
 *	kkillrewrite
 *
 *	Function:	- rewrites entire kill file from current process list
 *	Accepts:	- kernel process table
 *	Returns:	- 0 or LAMERROR
 */
int
kkillrewrite(struct kproc kptable[])
{
	int		i;
/*
 * Rewind the stdio descriptor.
 */
	rewind(fp_kill);
/*
 * Write the kernel pid first.
 */
	if (fprintf(fp_kill, "%d\n", (int) getpid()) == EOF) {
		return(LAMERROR);
	}
/*
 * Write every current client pid.
 */
	for (i = 0; i < KPMAX; ++i) {

            /* this is a fairly significant misuse of the priority
               field, but lamhalt can't show up in the kill file or it
               will be killed by tkill launched by the haltd, which is
               not what we want to have happen.  So we set lamhalt's
               priority to a special value, and don't write the pid of
               any processes with the "sacred" priority into the kill
               file. */
		if ((kptable[i].kp_state != KSFREE) &&
                    !(kptable[i].kp_flags & KIPROC) &&
                    !(kptable[i].kp_priority == PRLAMHALT)) {

			if (fprintf(fp_kill, "%d\n", kptable[i].kp_pid)
					== EOF) {
				return(LAMERROR);
			}
		}
	}
/*
 * Truncate file to new length.
 */
#ifdef SCO
	if (chsize(fd_kill, ftell(fp_kill)))
#else
	if (ftruncate(fd_kill, ftell(fp_kill)))
#endif
	  return(LAMERROR);

	return(0);
}


/*
 *	kkillclose
 *
 *	Function:	- Flush and close the kill file.
 *	Returns:	- 0 or LAMERROR
 */
int
kkillclose(void)
{
	if (fclose(fp_kill) == EOF) {
		return(LAMERROR);
	} else {
		return(0);
	}
}
