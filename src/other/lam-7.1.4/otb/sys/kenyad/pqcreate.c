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
 *	Function:	- OTB kenyad
 *			- creates a process
 */

#include "lam_config.h"
#include "sfh.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "args.h"
#include "kio.h"
#include "kreq.h"
#include "laminternal.h"
#include "net.h"
#include "portable.h"
#include "preq.h"
#include "t_types.h"
#include "terror.h"
#include "typical.h"
#include "lamdebug.h"
#include "etc_misc.h"

/*
 * private functions
 */
static int pcreate(char *loadname, int4 argc, char **argv, char **envv);
static char *argsread(char *argvname);
static void get_argv_env(char ***argv, char ***envv);
static int recv_stdio_fds(int *fds);
static void exit_protocol();
static void close_stdio_fds(int *fds);
static void check_dir(char *dir);

/*
 * external variables
 */
extern struct pprocess	ptable[];		/* kenya process table */
extern struct preq	pq;			/* kenya request */
extern struct preply	pr;			/* kenya reply */
extern char		**pathv;		/* argv of search paths */
extern mode_t original_umask;

/*
 * external functions
 */
extern void		psendr();
extern int		pfind();
extern char		*rforget();		/* tag -> filename */

/*
 * local variables
 */
static char		fdbuf[32];		/* TROLLIUSFD env space */
static char		rtfbuf[32];		/* TROLLIUSRTF env space */
static char		jobbuf[32];		/* LAMJOB env space */
static char		pidbuf[32];		/* LAMKENYAPID env space */
static char		parentbuf[32];		/* LAMPARENT env space */
static char		worldbuf[32];		/* LAMWORLD env space */
static char		rankbuf[32];		/* LAMRANK env space */


void
pqcreate(void)
{
    char		**argv = 0;		/* new process args */
    char		**envv = 0;		/* new process environment */
    char		*loadname;		/* load module filename */
    char		*cwd;			/* current working dir */
    int			argc = 0;		/* # args */

    pr.pr_reply = 0;
    loadname = 0;
    lamlog("kenyad: pqcreating with rtf 0x%x\n", pq.pq_rtf);
/*
 * Get argv and environment from tag.
 */
    if (pq.pq_argv) {
	get_argv_env(&argv, &envv);
	argc = sfh_argv_count(argv);
    }
/*
 * If flag set to try parents working dir, check if it exists.
 */
    if (pq.pq_rtf & RTF_TRYCWD) {
      lamlog("kenyad: checking for directory %s", pq.pq_wrkdir);
      check_dir(pq.pq_wrkdir);
    }
/*
 * Find the filename of the load module.
 */
    if (pq.pq_rtf & RTF_FLAT) {
      loadname = rforget(LOCAL, pq.pq_loadpt);
      lamlog("kenyad: kenyad laded executable from remote node: %s and %d",
	     loadname, pq.pq_loadpt);
    } else {
      if (!LAM_isNullStr(pq.pq_wrkdir)) {
	lamlog("kenyad: looking for executable \"%s\" in directory \"%s\"", 
	       pq.pq_name, pq.pq_wrkdir);
	loadname = sfh_path_env_findv(pq.pq_name, X_OK,
				      envv, pq.pq_wrkdir);
      } else {
	cwd = getworkdir();
	lamlog("kenyad: looking for executable \"%s\" in directory \"%s\"", 
	       pq.pq_name, cwd);
	loadname = sfh_path_env_findv(pq.pq_name, X_OK,
				      envv, cwd);
	if (cwd) 
	  free(cwd);
      }
    }

    if (loadname == 0) {
      lamlog("kenyad: could not find %s, errno %d", pq.pq_name, errno);
      pr.pr_reply = ltot(errno);
    } else {
      lamlog("kenyad: found \"%s\"", loadname);
    }
/*
 * Construct an argv from the executable name.
 */
    if (!pq.pq_argv) {
      if (loadname) {
	argc = 1;
	argv = sfh_argv_break(loadname, 0xa);
	if (argv == 0) 
	  lampanic("kenyad (sfh_argv_break)");
      } else {
	argv = 0;
      }
    }
/*
 * Clean up if there was an error.
 */
    if (pr.pr_reply) {
      lamlog("kenyad: badness - can't execute %s", pq.pq_name);
      
      if (loadname) {
	if (pq.pq_rtf & RTF_FLAT) {
	  lamlog("kenyad: unlinking temporary file %s", loadname);
	  unlink(loadname);
	}

	free(loadname);
      }
      
      if (argv) {
	free((char *) argv);
      }
/*
 * The file descriptor passer (filed client) is blocked on an accept.  We must
 * exit and let it continue.  This is done by opening and immediately closing
 * a connection to it.  
 */
      if (pq.pq_rtf & RTF_PFDIO) {
	lamlog("kenyad: telling mpirun that we aborted (i.e., ignoring passed fds)");
	exit_protocol();
      }
    }
/*
 * Create the process.
 */
    else {
      lamlog("kenyad: creating new user process...");
      if (pcreate(loadname, argc, argv, envv)) {
	lamlog("kenyad: create failed, errno %d", errno);
	pr.pr_reply = ltot(errno);
      } else
	lamlog("kenyad: create succeeded, process running");
    }
/*
 * Reply to client.
 */
    psendr((char *) &pr, sizeof(pr), 0);
}

/*
 *	pcreate
 *
 *	Function:	- creates a new process for a given load module
 *	Accepts:	- load module filename
 *			- command line args
 *	Returns:	- 0 or ERROR
 */
static int
pcreate(char *loadname, int4 argc, char **argv, char **envv)
{
  struct pprocess *pnew;	/* newborn process pointer */
  int inew;			/* index of newborn process */
  sigset_t newset;		/* new signal set */
  sigset_t oldset;		/* old signal set */
  int fds[3];			/* stdio file descriptors */
  char *wrkdir = 0;		/* working directory */
  char *p;
/*
 * Find a slot in the process table.
 */
  inew = pfind((int4) NOTPROCESS);
  
  if (inew < 0) {
    lamlog("kenyad: out of process slots; aborting user process (telling mpirun to abort");
    if (pq.pq_rtf & RTF_PFDIO) {
      exit_protocol();
    }
    
    errno = ENOPDESCRIPTORS;
    return(LAMERROR);
  }
  
  pnew = ptable + inew;
  pnew->p_loadpt = loadname;
  pnew->p_rtf = pq.pq_rtf | RTF_KENYA_CHILD;
  pnew->p_argc = argc;
  pnew->p_argv = argv;
  pnew->p_jobid.jid_node = pq.pq_jobid.jid_node;
  pnew->p_jobid.jid_pid = pq.pq_jobid.jid_pid;
  pnew->p_nodeid = pq.pq_src_node;
  pnew->p_event = pq.pq_src_event;
  pnew->p_status = NOTSTATUS;
/*
 * Receive stdio file descriptors. The process about to be created
 * inherits them.
 */
  if (pq.pq_rtf & RTF_PFDIO) {
    lamlog("kenyad: attempting to receive stdout/stderr file descriptors"); 
    if (recv_stdio_fds(fds)) {
      lamlog("kenyad: did not receive stdio file descriptors properly, aborting execution");
      return(LAMERROR);
    }
  }
/*
 * Pass Trillium runtime variables through environment.
 */
  lamlog("kenyad: setting environment variables to pass to new process");
  lamlog("kenyad: setting TROLLIUSFD");
  memset(fdbuf, 0, 32);
  snprintf(fdbuf, 31, "TROLLIUSFD=%d:%d:%d:%d", pq.pq_jobid.jid_node,
	   pq.pq_stdin, pq.pq_stdout, pq.pq_stderr);
  
  if (putenv(fdbuf)) lampanic("kenyad (putenv)");

  lamlog("kenyad: setting TROLLIUSRTF");
  memset(rtfbuf, 0, 32);
  snprintf(rtfbuf, 31, "TROLLIUSRTF=%d", pnew->p_rtf);
  if (putenv(rtfbuf)) lampanic("kenyad (putenv)");
  
  lamlog("kenyad: setting LAMJOBID");
  memset(jobbuf, 0, 32);
  snprintf(jobbuf, 31, "LAMJOBID=%d:%d", pq.pq_jobid.jid_node, 
	  pq.pq_jobid.jid_pid);
  if (putenv(jobbuf)) lampanic("kenyad (putenv)");
  
  lamlog("kenyad: setting LAMKENYAPID");
  memset(pidbuf, 0, 32);
  snprintf(pidbuf, 31, "LAMKENYAPID=%d", (int) getpid());
  if (putenv(pidbuf)) lampanic("kenyad (putenv)");
  
  lamlog("kenyad: setting LAMWORLD");
  memset(worldbuf, 0, 32);
  snprintf(worldbuf, 31, "LAMWORLD=%d", pq.pq_world);
  if (putenv(worldbuf)) lampanic("kenyad (putenv)");
  
  lamlog("kenyad: setting LAMPARENT");
  memset(parentbuf, 0, 32);
  snprintf(parentbuf, 31, "LAMPARENT=%d", pq.pq_parent);
  if (putenv(parentbuf)) lampanic("kenyad (putenv)");
  
  lamlog("kenyad: setting LAMRANK");
  memset(rankbuf, 0, 32);
  snprintf(rankbuf, 31, "LAMRANK=%d", pq.pq_rank);
  if (putenv(rankbuf)) lampanic("kenyad (putenv)");
/*
 * Set up working dir.
 */
  lamlog("kenyad: checking for working directory flag");
  if (pq.pq_rtf & RTF_APPWD) {
    lamlog("kenyad: attempting to set working directory from app name");
    wrkdir = strdup(loadname);
    if (wrkdir == 0) {
      lamlog("kenyad: out of memory -- could not allocate space for string copy");
      return(LAMERROR);
    }
    p = strrchr(wrkdir + 1, STRDIR);
    if (!p)
      p = wrkdir + 1;
    *p = 0;
  } 
  else if (!LAM_isNullStr(pq.pq_wrkdir)) {
    lamlog("kenyad: working directory set explicitly");
    wrkdir = pq.pq_wrkdir;
  }
  if (wrkdir == 0) {
    lamlog("kenyad: running in directory (current directory)");
  } else {
    lamlog("kenyad: running in directory %s", wrkdir);
  }
/*
 * Create the process.
 */
  sigemptyset(&newset);
  sigaddset(&newset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &newset, &oldset);
  
  pnew->p_pid = kcreatev(loadname, argv, envv,
			 wrkdir, (pq.pq_rtf & RTF_PFDIO) ? fds : NULL,
			 original_umask);
  
  sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
/*
 * Close the passed file descriptors.  Kenyad does not use them.
 */
  if (pq.pq_rtf & RTF_PFDIO) {
    close_stdio_fds(fds);
  }
  
  if (pq.pq_rtf & RTF_APPWD) {
    free(wrkdir);
  }
  
  if (pnew->p_pid < 0) {
    lamlog("kenyad: fork/exec of process failed, return %d, errno %d", 
	    pnew->p_pid, errno);
    return(LAMERROR);
  }
  lamlog("kenyad: fork/exec succeeded, pid %d, index %d, rtf 0x%x", 
	  pnew->p_pid, inew + 1, pnew->p_rtf);
  
  pr.pr_pid = ltot(pnew->p_pid);
  pr.pr_index = ltot(inew + 1);
  return(0);
}

/*
 *	argsread
 *
 *	Function:	- reads in glued argv structure from a file
 *	Accepts:	- filename
 *	Returns:	- argv ptr or null ptr
 */
static char *
argsread(char *argvname)
{
	char		*args;			/* glued argv buffer */
	int		avfd;			/* argv file descriptor */
	struct stat	avstat;			/* argv file status */

	if (stat(argvname, &avstat)) return(0);

	args = malloc((unsigned int) avstat.st_size);
	if (args == 0) lampanic("kenyad (malloc)");

	avfd = open(argvname, O_RDONLY, 0);

	if (avfd < 0) {
		free(args);
		return(0);
	}

	if (read(avfd, args, (int) avstat.st_size) < 0) {
		free(args);
		close(avfd);
		return(0);
	}

	close(avfd);
	return(args);
}

/*
 *	get_argv_env
 *
 *	Function:	- reconstruct argv and environment from a file
 *	Accepts:	- argv (out)
 *			- environment (out)
 */
static void
get_argv_env(char ***argv, char ***envv)
{
	char		*args;			/* glued argv and environment */
	char		*argvname;		/* argv filename */
	int		len;

	*argv = 0;
	*envv = 0;
/*
 * Convert argument tag to filename.
 */
	argvname = rforget(LOCAL, pq.pq_argv);

	if (argvname == 0) {
		pr.pr_reply = ltot(errno);
		return;
	}
/*
 * Read in argv structure.
 */
	args = argsread(argvname);

	if (args == 0) {
		pr.pr_reply = ltot(errno);
		unlink(argvname);
		free(argvname);
		return;
	}
/*
 * Reconstruct the argv and environment structure.
 */
	if (pq.pq_envsize) {
		len = strlen(args);
		*envv = sfh_argv_break(args + len - pq.pq_envsize, 0xa);
		if (*envv == 0) lampanic("kenyad (sfh_argv_break)");

		if (pq.pq_envsize < len) {
			args[len - pq.pq_envsize] = 0;
			*argv = sfh_argv_break(args, 0xa);
			if (*argv == 0) lampanic("kenyad (sfh_argv_break)");
		}
	} else {
		*argv = sfh_argv_break(args, 0xa);
		if (*argv == 0) lampanic("kenyad (sfh_argv_break)");
	}

	free(args);
	unlink(argvname);
	free(argvname);
}

/*
 *	recv_stdio_fds
 *
 *	Function:	- read file descriptors from server (filed client)
 *	Accepts:	- file descriptor array (out)
 *	Returns:	- 0 or LAMERROR
 */
static int
recv_stdio_fds(int *fds)
{
	int		pass_fd;		/* stream to receive fds over */
	char		server[LAM_PATH_MAX];	/* fd server socket name */
/*
 * Open connection to server.
 */
	if (lam_mktmpid((int) -pq.pq_src_event, server, sizeof(server))) {
	  lamlog("kenyad: recv_stdio_fds: could not lam_mktmpid properly");
	  return(LAMERROR);
	}
	
        if ((pass_fd = sfh_sock_open_clt_unix_stm(server)) < 0) {
	  lamlog("kenyad: recv_stdio_fds: could not sfh_sock_open_clt_unix_stm properly");
	  return(LAMERROR);
	}
/*
 * Receive the file descriptors.
 */
	if ((fds[0] = sfh_recv_fd(pass_fd)) < 0) {
	  lamlog("kenyad: recv_stdio_fds: could not sfh_recv_fd(1) properly");
	  close(pass_fd);
	  return(LAMERROR);
	}
	
	if ((fds[1] = sfh_recv_fd(pass_fd)) < 0) {
	  lamlog("kenyad: recv_stdio_fds: could not sfh_recv_fd(2) properly");
	  close(fds[0]);
	  close(pass_fd);
	  return(LAMERROR);
	}

	if ((fds[2] = sfh_recv_fd(pass_fd)) < 0) {
	  lamlog("kenyad: recv_stdio_fds: could not sfh_recv_fd(3) properly");
	  close(fds[0]);
	  close(fds[1]);
	  close(pass_fd);
	  return(LAMERROR);
	}
/*
 * Close connection to server.
 */
	lamlog("kenyad: recv_stdio_fds: happiness");
	close(pass_fd);
	return(0);
}

/*
 *	exit_protocol
 *
 *	Function:	- exit the file descriptor passing protocol
 *			  unblocking the server side
 *			- connects to server and immediately closes
 *			  the connection
 */
static void
exit_protocol()
{
  int fd;
  char server[LAM_PATH_MAX];	/* fd server socket name */
  
  if (lam_mktmpid((int) -pq.pq_src_event, server, sizeof(server)) == 0) {
    if ((fd = sfh_sock_open_clt_unix_stm(server)) >= 0) {
      close(fd);
    }
  }
}

/*
 *	close_stdio_fds
 *
 *	Function:	- close passed stdio file descriptors
 *	Accepts:	- file descriptor array 
 */
static void
close_stdio_fds(int *fds)
{
	close(fds[0]);
	close(fds[1]);
	close(fds[2]);
}

/*
 *	check_dir
 *
 *	Function:	- check if directory exists
 *			- if not nulls the directory name
 *	Accepts:	- directory name
 */
static void
check_dir(char *dir)
{
    struct stat		fileinfo;

    if (stat(dir, &fileinfo) || !S_ISDIR(fileinfo.st_mode)) {
	strcpy(dir, "");
    }
}
