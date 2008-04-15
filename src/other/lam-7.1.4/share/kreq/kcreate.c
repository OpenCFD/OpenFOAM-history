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
 * $Id: kcreate.c,v 1.15 2003/11/04 02:40:26 jsquyres Exp $
 *
 *  Function: - creates an OTB process
 *      - does not make a Trollius kernel request
 *      - essentially a packaged fork and exec
 *      - redirects created process's stdio
 *  Accepts:  - child program name
 *      - argument vector
 *      - environment vector
 *      - stdio file descriptors (0 if no redirection)
 *  Returns:  - (to parent) child pid or ERROR
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "kio.h"
#include "preq.h"
#include "terror.h"
#include "typical.h"
#include "lamdebug.h"

/*
 * external functions
 */
extern void   _cipc_cleanup();
extern int    lam_pty_open();
extern int    liosattach();
extern int    liofdattach();
extern int4   stoi();

/*
 * private functions
 */
static int    redirect_stdio(int, int *);
static int    set_environment(char **);
static void   set_handler_default(int);


int
kcreate(const char *pathname, char **argv)
{
    return kcreatev(pathname, argv, NULL, NULL, NULL, 0);
}

int
kcreatev(const char *pathname, char **argv, char **envv, char *wrkdir, 
         int *fds, int umask_mode)
{
  sigset_t    sigs;
  sigset_t    newset;
  sigset_t    oldset;
  int     pid;
  int     rtf;
  int     errno_save;
  char    *p;
  
  sigemptyset(&newset);
  sigaddset(&newset, SIGTERM);
  sigaddset(&newset, SIGINT);
  sigaddset(&newset, SIGHUP);
  sigprocmask(SIG_BLOCK, &newset, &oldset);
  
  pid = fork();
  
  if (pid != 0) {
    errno_save = errno;
    sigprocmask(SIG_SETMASK, &oldset, (sigset_t *)0);
    if (pid < 0) {
      errno = errno_save;
      return LAMERROR;
    }
    else {
      return pid;
    }
  }

  if (pid == 0) {
/*
 * Set signal handlers back to the default. This must be done before unblocking
 * the signals to avoid a race where a signal delivered to the child can lead
 * to the execution of the kenyad signal handlers and accidental killing
 * of kenyad.
 */
    set_handler_default(SIGTERM);
    set_handler_default(SIGINT);
    set_handler_default(SIGHUP);
    set_handler_default(SIGCHLD);
    set_handler_default(SIGPIPE);
/*
 * Unblock all signals.
 */
    sigprocmask(0, 0, &sigs);
    sigprocmask(SIG_UNBLOCK, &sigs, 0);

  /*
   * set umask if desired
   */
    if (umask_mode > 0)
      umask(umask_mode);

/*
 * Redirect the stdio fd's
 */
    rtf = (p = getenv("TROLLIUSRTF")) ? stoi((unsigned char *) p) : 0;
    if (redirect_stdio(rtf, fds))
      exit(errno);
/*
 * Close all the rest of the file descriptors.
 */
    _cipc_cleanup();
    lamcloselog();

    if (set_environment(envv))
      exit(errno);
    
    if (wrkdir) {
      if (chdir(wrkdir) && !(rtf & RTF_TRYCWD)) {
        fprintf(stderr, "kcreatev: chdir(%s): ", wrkdir);
        terror("");
        exit(errno);
      }
    }

    execvp(pathname, argv);
    exit(errno);
  }

  /* This is never reached, but we put it here because some compilers
     can't see logic like if (x) { ...; return; } else { ...; exit(); }
     and complain because there's no return statement.  [sigh]. */

  return LAMERROR;
}

/*
 *  redirect_stdio
 *
 *  Function: - redirect process stdio
 *  Accepts:  - runtime flags
 *      - passed file descriptors or NULL
 */
static int
redirect_stdio(int rtf, int *fds)
{
  int     ionode;
  int     stdout_fd;
  int     stderr_fd;
  int     master;
  int     slave;
  char    *p;

  if (fds) {
/*
 * Redirect stdio to the passed fds.
 */
    dup2(fds[0], fileno(stdin));
    dup2(fds[1], fileno(stdout));
    dup2(fds[2], fileno(stderr));
  } else {
    if ((rtf&RTF_IO) && !(rtf&RTF_PFDIO) && (p = getenv("TROLLIUSFD"))) {
/*
 * Connect UNIX stderr and stdout to IO daemon.
 */
      ionode = stoi((unsigned char *) p);
      p = strchr(p, ':') + 1;
      p = strchr(p, ':') + 1;     /* skip stdin */
      stdout_fd = stoi((unsigned char *) p);
      p = strchr(p, ':') + 1;
      stderr_fd = stoi((unsigned char *) p);
/*
 * Attach stderr stream to iod nobuffered.
 */
      if (liosattach(stderr, ionode, stderr_fd, _IONBF))
        return(LAMERROR);
/*
 * Try to use a pty for stdout if asked for and if stdout is a tty.
 * pty support is enabled ONLY IF FD passing is supported */
#if LAM_HAVE_FD_PASSING 
      if ((rtf & RTF_PTYS) && (rtf & RTF_TTYOUT)) {
        if (lam_pty_open(&master, &slave)) {
          if (errno == ENOPTYSUPPORT) {
/*
 * No pty support on this node so fall back to no pty IO.
 */
            if (liosattach(stdout, ionode, stdout_fd, _IONBF))
              return(LAMERROR);
          }
        } else {
          if (dup2(slave, fileno(stdout)) != fileno(stdout))
            return(LAMERROR);
          if (liofdattach(master, ionode, stdout_fd))
            return(LAMERROR);
        }
      } else {
        if (liosattach(stdout, ionode, stdout_fd, _IONBF))
          return(LAMERROR);
      }
#else
/* we dont have FD passing so just connect to the IO stream */
      if (liosattach(stdout, ionode, stdout_fd, _IONBF)){
        return (LAMERROR);
      }
#endif

    }
  }

  return(0);
}

/*
 *  set_environment
 *
 *  Function: - set process environment
 *  Accepts:  - environment vector
 */
static int
set_environment(char **env)
{
  int i;

  if (env) {
    for (i = 0; env[i]; i++) {
      if (putenv(env[i]))
        return(LAMERROR);
    }
  }

  return(0);
}

static void
set_handler_default(int sig)
{
  struct sigaction act;

  act.sa_handler = SIG_DFL;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(sig, &act, (struct sigaction *)0);
}
