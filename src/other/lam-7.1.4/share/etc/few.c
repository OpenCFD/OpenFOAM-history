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
 * $Id: few.c,v 6.7 2003/07/05 21:52:08 jsquyres Exp $
 * 
 *	Function:	- forks, execs and waits for a subordinate program
 *	Accepts:	- argument vector, argv[0] is program
 *	Returns:	- status code or ERROR
 */

#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#include <lam_config.h>
#include <laminternal.h>
#include <sfh.h>
#include <etc_misc.h>

#if LAM_HAVE_BPROC

#include <sys/bproc.h>

/* Global variable for BPROC environment */
extern char** environ;

#endif

int
_lam_few(char *argv[])
{
	int		status;		/* exit status */
	int		pid;		/* whoami? */
        int             ret;

	if ((pid = fork()) < 0) {
		return(pid);
    	}

	else if (pid == 0) {		/* child */
		execvp(argv[0], argv);
		exit(errno);
    	}

	else {				/* parent */
          /* spin in waitpid until we get a valid entry.  If we get an
             error that isn't going to go away, quit. */
          while ((ret = waitpid(pid, &status, 0)) <= 0) {
            if (ret < 0 && (errno == EFAULT || errno == EINVAL)) {
              return -1;
            }
          }
	}

        if (WIFEXITED(status)) {
          ret = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
          ret =  WTERMSIG(status);
        } else {
          ret = -1;
        }

        return ret;
}


#if LAM_HAVE_BPROC

int
_lam_femw(char *argv[], int target_node)
{
  int		status;		/* exit status */
  int		pid;		/* whoami? */
  char *filename;
  char *cwd;
  int ret;

  if ((pid = fork()) < 0) {
    return(pid);
  } else if (pid == 0) {		/* child */
    if (bproc_currnode() != BPROC_NODE_MASTER) {
      /* Migrate to master, if needed */
      ret = bproc_move(BPROC_NODE_MASTER);
      if (ret != 0)
	exit(ret);
    }

    /* Get the full pathname... */
    cwd = getworkdir();
    filename = sfh_path_env_findv(argv[0], 0, environ, cwd);
    if (cwd)
      free(cwd);

    if (!filename) {
      /* BWB: Need something cleaner ? */
      printf("_lam_femw: Got null from sfh_path_env_findv\n");
      exit(-1);
    }

    /* Exec and move! */
    bproc_execmove(target_node, filename, argv, environ);
    exit(errno);
  } else {				/* parent */
    while ((waitpid(pid, &status, 0) != 0) &&
	   (! WIFEXITED(status)));
  }

  return(WEXITSTATUS(status));
}

#endif /* #if LAM_HAVE_BPROC */
