/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 * $Id: haltd.c,v 1.25 2003/09/27 05:16:16 brbarret Exp $
 * 
 *	Function:	- halt server
 *			- if receive local message, rebroadcast to all 
 *                        others, then die
 *			- if receive non-local message, die
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <lam_config.h>

#include <all_opt.h>
#include <args.h>
#include <events.h>
#include <dl_inet.h>
#include <portable.h>
#include <hreq.h>
#include <net.h>
#include <preq.h>
#include <terror.h>
#include <lamdebug.h>	
#include <sfh.h>

/*
 * Access the environment
 */
extern char **environ;

/*
 * local variables
 */
static char		msg[MAXNMSGLEN];	/* halt message */
static struct nmsg	nhead;			/* halt network message */

/*
 * global variables
 */
extern OPT* lam_daemon_optd;


/*
 * local functions
 */
static void             diediedie(int pid);


/*
 * global functions
 */
void			(*(h_init()))();
void			(*(haltd()))();

extern struct kproc *kpfind();
extern void knuke();


/*
 *	h_init
 *
 *	Function:	- haltd initialization
 */
void (*(
h_init()))()

{
/*
 * Attach to kernel.
 */
  if (lpattach("haltd")) {
    lampanic("haltd (lpattach)");
  }
/*
 * Receive first request.
 */
  LAM_ZERO_ME(nhead);
  memset((void*) msg, -1, MAXNMSGLEN);
  nhead.nh_event = EVHALTD;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = MAXNMSGLEN;
  nhead.nh_msg = msg;
  
  if (nrecv(&nhead)) {
    lampanic("haltd (nrecv)");
  }
  
  return((void (*)()) haltd);
}

/*
 *	haltd
 *
 *	Function:	- server loop
 *			- if local message, rebroadcast to all links
 *			- if non-local message, die
 */
void
(*(haltd()))()

{
  struct nmsg ack;
  struct hreq *request;		/* request from client */
  int index;

  request = (struct hreq *) nhead.nh_data;

/*
 * If this was the ping, send an ACK back
 */
  if (nhead.nh_type == LAM_HALT_PING) {

    LAM_ZERO_ME(ack);
    ack.nh_event = request->hq_event;
    ack.nh_node = request->hq_node;
    ack.nh_type = 0;
    ack.nh_flags = 0;
    ack.nh_length = 0;
    index = request->hq_index;

    request = (struct hreq *) ack.nh_data;
    request->hq_node = getnodeid();
    request->hq_event = 0;
    request->hq_index = index;
    nsend(&ack);
  }

/*
 * Otherwise, if it was the kill message, die
 */
  else if (nhead.nh_type == LAM_HALT_DIE) {
      if (request->hq_node == getnodeid()) {
          diediedie(request->hq_pid);
      } else {
          diediedie(-1);
      }
  }

  nhead.nh_event = EVHALTD;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = MAXNMSGLEN;
  nhead.nh_msg = msg;
  
  if (nrecv(&nhead)) {
    lampanic("haltd (nrecv)");
  }

  return((void (*)()) haltd);
}


/*
 * kill the lamd
 * simply fork off tkill to do the dirty work, potentially with a -b
 * argument (since tkill knows how to kill the named sockets, etc.)
 */
static void
diediedie(int client_pid)
{
  int argc = 0;
  char **argv = NULL;
  pid_t pid;

#if 0
  char *tkillpath;
  char *prefix;
  int tkill_len;
#endif

  char **pathv = NULL;
  int pathc = 0;
  char *tkillpath;
  char fname[] = "tkill";

  /* 
   * Even though it should be set to close-on-exec, close the
   * syslog/debug log anyway. 
   *
   * Do not try to close the kill file by calling kkillclose() here.
   * It breaks the model of compiling the pseudo-daemons as seperate
   * processes.  The fd is close on exec, so it isn't a big deal.
   * More importantly, the haltd doesn't actually have the kill file
   * open - the kernel has the killfile open.  Repeat after me: I will
   * not break abstraction barriers.
   *
   */
  lamcloselog();

/*
 * Setup tkill to kill us
 */
  sfh_argv_add(&pathc, &pathv, "");
  sfh_argv_add(&pathc, &pathv, "$LAMHOME/bin");
  sfh_argv_add(&pathc, &pathv, LAM_BINDIR);

  tkillpath = sfh_path_findv(fname, pathv, X_OK, environ);
  sfh_argv_free(pathv);

  if (NULL == tkillpath) {
      tkillpath = sfh_path_env_find(fname, X_OK);

      if (NULL == tkillpath) {
          exit(1);
      }
  }
  sfh_argv_add(&argc, &argv, tkillpath);

  if (ao_taken(lam_daemon_optd, "d")) {
    sfh_argv_add(&argc, &argv, "-d");
    sfh_argv_add(&argc, &argv, "-debug-output");
    sfh_argv_add(&argc, &argv, "syslog");
  }

  sfh_argv_add(&argc, &argv, "-setsid");

/*
 * fork off a child to launch tkill in.  If tkill fails to launch,
 * just kill the parent.  Not quite clean, and it leaves the sockets
 * around, but oh well..
 */
  pid = fork();
  if (pid < 0) {
    exit(1);
  } else if (pid == 0) {
/*
 * Give remote processes a moment to clean up
 */
    sleep(1);
    execvp(argv[0], argv);

/*
 * Oops -- execvp failed.  It shouldn't, because tkill should be in
 * the path.  Oh well -- kill the parent (don't kill init by
 * accident!).
 */
    pid = getppid();
    if (pid > 1) 
      if (kill(getppid(), SIGHUP))
        if (kill(getppid(), SIGINT))
          if (kill(getppid(), SIGKILL))
            lampanic("haltd (kill)");
    exit(1);
  }
  
/*
 * tkill should get us, any moment now... 
 */
  while(1)
    sleep(1024);
}
