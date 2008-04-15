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
 * $Id: ssi_boot_ioexecvp.c,v 1.5 2004/03/06 21:17:32 jsquyres Exp $
 * 
 */

#include <lam_config.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif

#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <sys/wait.h>
#include <sys/param.h>

#include <args.h>
#include <all_opt.h>
#include <etc_misc.h>
#include <typical.h>
#include <lamdebug.h>
#include <sfh.h>


/*
 *	ioexecvp
 *
 *	Function:	- execute command (similar to cnfexec)
 *			- can direct command stdout to buffer and/or stdout
 *			- stderr is checked and passed through
 *	Accepts		- command argv
 *			- print stdout flag
 *			- ptr to buffer (for stdout data)
 *			- size of buffer
 *	Returns		- 0 or LAMERROR
 */
int
lam_ssi_boot_base_ioexecvp(char **cmdv, int showout, char *outbuff, 
                           int outbuffsize, int stderr_is_err)
{
  int kidstdout[2];		/* child stdout pipe */
  int kidstderr[2];		/* child stderr pipe */
  int ret;			/* read() return value */
  int err;			/* error indicator */
  int status;			/* exit status */
  int pid;			/* child process id */
  char *ptr = 0;		/* buffer pointer */
  fd_set readset;		/* fd's for read select */
  fd_set errset;		/* fd's for error select */
  int nfds = 1;			/* num fd's in readset */
  char temp[256];		/* string holding space */
  int want_out = 0;		/* want stdout in select */
  int stdout_err = 0;
  int stderr_err = 0;
  int i;
  int announce = 0;
  char *stderr_announce;

  if (stderr_is_err == 1) {
    stderr_announce = "ERROR: LAM/MPI unexpectedly received the following on stderr:\n";
  } else {
    stderr_announce = "WARNING: LAM/MPI unexpectedly received the following on stderr:\n";
  }

  /* Create child stdout/stderr pipes and fork the child process
     (command).  */

  if (pipe(kidstdout) || pipe(kidstderr))
    return (LAMERROR);

  if ((pid = fork()) < 0) {
    return (LAMERROR);
  }

  else if (pid == 0) {		       /* child */
    if ((dup2(kidstderr[1], 2) < 0) || (dup2(kidstdout[1], 1) < 0)) {
      perror(cmdv[0]);
      exit(errno);
    }

    if (close(kidstdout[0]) || close(kidstderr[0]) ||
	close(kidstdout[1]) || close(kidstderr[1])) {
      perror(cmdv[0]);
      exit(errno);
    }

    /* Ensure that we close all other file descriptors */

    for (i = 3; i < FD_SETSIZE; i++)
      close(i);

    execvp(cmdv[0], cmdv);
    exit(errno);
  }

  if (close(kidstdout[1]) || close(kidstderr[1]))
    return (LAMERROR);

  sfh_argv_free(cmdv);

  /* We must be able to monitor both stdout and stderr; it is possible
     that we may be trying to capture the stdout but also need to
     monitor output on stderr (e.g., recon, lamboot).  So make a
     FD_SET with potentially both of the file descriptors and do a
     select on it.  */

  FD_ZERO(&readset);
  FD_SET(kidstderr[0], &readset);
  nfds = kidstderr[0] + 1;
  if (showout || (outbuff != 0)) {
    ptr = outbuff;
    FD_SET(kidstdout[0], &readset);
    nfds = (nfds > kidstdout[0] + 1) ? nfds : kidstdout[0] + 1;
    want_out = 1;
  }

  err = 0;
  while (err == 0 && nfds > 0) {

    /* Check to see if select() gets interrupted.  */

    errset = readset;
    ret = select(nfds, &readset, NULL, &errset, NULL);
    if (ret == -1) {
      if (errno == EINTR)
	continue;
      else {

	/* Need to simply break on error instead of returning so that
	   we can still reap the child properly */

	err = LAMERROR;
	break;
      }
    }

    /* Check for error condition on stderr.  Don't need to close it
      here -- it will get closed unconditionally later.  */

    if (FD_ISSET(kidstderr[0], &errset) != 0) {
      stderr_err = 1;
    }

    /* See if there was something on stderr */

    if (FD_ISSET(kidstderr[0], &readset) != 0) {
      while (1) {
	ret = read(kidstderr[0], temp, 256);
	/* Error? */
	if (ret < 0) {
	  if (errno == EINTR)
	    continue;
	  else {
	    stderr_err = 1;
	    err = LAMERROR;
	    break;
	  }
	}
	/* Good bytes */
	else if (ret > 0) {
	  if (announce == 0)
	    write(2, stderr_announce, strlen(stderr_announce));
	  announce = 1;
	  write(2, temp, ret);
	  fflush(stderr);

          if (stderr_is_err == 1) {
            errno = EFAULT;
            err = LAMERROR;
          }
	}
	/* Zero bytes */
	else {
	  /* This is likely to indicate that this pipe has closed */
	  stderr_err = 1;
	  break;
	}
      }
    }

    /* Check for error condition on stdout.  Don't need to close it
       here -- it will get closed unconditionally later.  */

    if (FD_ISSET(kidstdout[0], &errset) != 0)
      stdout_err = 1;

    /* See if there is something on stdout (and if we care) */

    if ((showout || (outbuff != 0)) &&
	FD_ISSET(kidstdout[0], &readset) != 0) {
      while (1) {
	ret = read(kidstdout[0], temp, 256);
	/* Error? */
	if (ret < 0) {
	  if (errno == EINTR)
	    continue;
	  else {
	    stdout_err = 1;
	    err = LAMERROR;
	    break;
	  }
	}
	/* Good bytes */
	else if (ret > 0) {
	  if (outbuffsize > 0) {
	    memcpy(ptr, temp, (ret > outbuffsize) ? outbuffsize : ret);
	    /* Doesn't matter if we overshoot here */
	    outbuffsize -= ret;
	    ptr += ret;
	    if (outbuffsize > 0)
	      *ptr = '\0';
	  }
	  if (showout) {
	    write(1, temp, ret);
	    fflush(stdout);
	  }
	}
	/* Zero bytes */
	else {
	  stdout_err = 1;
	  break;
	}
      }
    }

    /* Reset stderr, 'cause we're always interested in that, unless it
       errored out */

    nfds = 0;
    if (!stderr_err) {
      FD_SET(kidstderr[0], &readset);
      nfds = kidstderr[0] + 1;
    }

    /* See if we want to reset stdout */

    if (!stdout_err && (want_out || outbuffsize > 0)) {
      FD_SET(kidstdout[0], &readset);
      nfds = (nfds > kidstdout[0] + 1) ? nfds : kidstdout[0] + 1;
    }
  }

  /* Close the pipes of the parent process.  */

  if (close(kidstdout[0]) || close(kidstderr[0])) {
    err = LAMERROR;
  }

  /* Wait for the command to exit.  */

  do {
    if (waitpid(pid, &status, 0) < 0) {
      return (LAMERROR);
    }
  } while (!WIFEXITED(status));

  if (WEXITSTATUS(status)) {
    errno = WEXITSTATUS(status);

    if (errno == 1)
      errno = EUNKNOWN;

    return (LAMERROR);
  }

  return (err);
}

