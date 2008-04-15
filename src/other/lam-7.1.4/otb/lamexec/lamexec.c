/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 * $Id: lamexec.c,v 1.26 2003/11/15 05:40:53 pkambadu Exp $
 *
 *	Function:	- run commands on a LAM
 */

#include <lam_config.h>

#include <sys/types.h>
#include <sys/signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <all_list.h>
#include <all_opt.h>
#include <app_mgmt.h>
#include <app_schema.h>
#include <args.h>
#include <events.h>
#include <freq.h>
#include <kio.h>
#include <laminternal.h>
#include <ndi.h>
#include <net.h>
#include <portable.h>
#include <priority.h>
#include <preq.h>
#include <sfh.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * private functions
 */
static int pwait(int4 nwait, int *childstat);
static int set_mode(void);
static int set_stdio(void);
static void reset_stdio(void);
static void first_signal_handler(int sig); 
static void signal_handler(int sig);
static LIST *build_job(int argc, char **argv, char **env);
static char *locate_aschema(char *filename);
static void node_ranges(char *buf, int4 initnode, int4 initindex);
static void error_cleanup(void);

/*
 * external variables
 */
extern struct kio_t	_kio;			/* kernel I/O block */
extern struct fclient	_ufd[FUMAX];		/* user file desc. */

/*
 * private variables
 */
static struct _gps	*job;			/* job GPS array */
static int		job_n;			/* size of job */
static int		fl_fdesc;		/* don't open stdio */
static int		fl_nprocs;		/* # procs specified */
static int		fl_verbose;		/* verbose mode */
static int		fl_wait = 1;		/* wait for completion */
static int4		nprocs;			/* # procs */
static int4		rtf;			/* runtime flags */
static char		*wrkdir;		/* working dir */
static char		fmtbuf[512];		/* formatting buffer */
static char		smallbuf[128];		/* small extra buffer */
static OPT		*ad;			/* argument descriptor */

static char		*syntaxerr =
	"lamexec: application schema syntax error, line %d\n";

static char		*usage =
	"lamexec [options] [<app>] [[<nodes>] <prog> [<args>]]\n";

int
main(int argc, char **argv)
{
    LIST	*app;			/* application desc. */
    int		status;			/* return status */
    char	**env = 0;
/*
 * Parse the command line.
 */
    ad = ao_init();
    if (ad == 0) {
	perror("lamexec (ao_init)");
	exit(errno);
    }
    ao_setopt1(ad, "fhvwD", 0, 0, 0);
    ao_setopt(ad, "s", 0, 1, 0);
    ao_setopt(ad, "c", 0, 1, AOINT);
    ao_setopt(ad, "np", "c", 1, AOINT);
    ao_setopt(ad, "nw", "w", 0, 0);
    ao_setopt(ad, "x", 0, 1, 0);
    ao_setopt(ad, "wd", "D", 1, 0);
    ao_setopt(ad, "pty", 0, 0, 0);
    ao_setopt(ad, "npty", "pty", 0, 0);

    if (asc_compat(&argc, &argv, ad)) {
	perror("lamexec");
	exit(errno);
    }

    if (ao_parse(ad, &argc, argv))  {
	fprintf(stderr, usage);
	exit(errno);
    }
/*
 * Check for help request.
 */
    if (ao_taken(ad, "h")) {
      show_help("lamexec", "usage", NULL);
      exit(0);
    }
/*
 * Set the operating mode.
 */
    if (set_mode()) {
	terror("lamexec (set_mode)");
	exit(errno);
    }
/*
 * Attach to kernel.
 */
    if (kinit(PRCMD)) {
      char hostname[MAXHOSTNAMELEN];
      gethostname(hostname, MAXHOSTNAMELEN);

      show_help(NULL, "no-lamd", "lamexec", hostname, NULL);
      exit(errno);
    }
/*
 * Set job identifier to be inherited by the programs.
 */
    _kio.ki_jobid.jid_node = getnodeid();
    _kio.ki_jobid.jid_pid = lam_getpid();
/*
 * Get the job environment.
 */
    if (asc_environment(0,
	    ao_taken(ad, "x") ? ao_param(ad, "x", 0, 0) : NULL, &env))
	lamfail("lamexec");
/*
 * Build the job.
 */
    if ((app = build_job(argc, argv, env)) == 0) {
	kexit(errno);
    }

    job_n = al_count(app);
    if ((job = malloc(job_n * sizeof(struct _gps))) == 0)
	lamfail("lamexec");
  
/* We need to have different signal handlers till we can cleanup all
 * the socket structures we need. This is a vulnerable period to recieve
 * signals. Some sockets may remain open. This signal handler will generate
 * a warning on the first occurance of the signal and on the second occurance
 * it will call the generic "signal_handler()"
 */
 if (fl_wait) {
   _lam_signal(SIGINT, first_signal_handler);
	 _lam_signal(SIGTERM, first_signal_handler);
 }

/*
 * Set job stdio.
 */
    if (set_stdio())
	lamfail("lamexec (set_stdio)");
/*
 * Run the job.
 */
    if (asc_run(app, 0, rtf, fl_verbose, TRUE, job)) {
	reset_stdio();
	kexit(errno);
    }
/*
 * Set signal handler to terminate the job.
 * Now we change the signal handler to the original one since the 
 * vulnerable period is over
 */
    if (fl_wait) {
	_lam_signal(SIGINT, signal_handler);
	_lam_signal(SIGTERM, signal_handler);
    }

    reset_stdio();
/*
 * If needed, wait for the job to terminate.
 */
    status = 0;

    if (fl_wait) {
	if (pwait(job_n, &status)) {
	    app_doom(job_n, job, -15);
	    error_cleanup();
	}
    }

    kexit(status);
    return(0);
}

/*
 *	build_job
 *
 *	Function:	- build job from command line specification
 *	Accepts:	- command line argument count
 *			- command line argument vector
 *	Returns:	- application descriptor or 0
 */
static LIST *
build_job(int argc, char **argv, char **env)
{
	LIST		*app;			/* application */
	LIST		*app_sched;		/* scheduled application */
	int4		nodeid;			/* target node */
	int4		index;			/* for getnode() */
	int4		nflags;			/* for getnode() */
	int		lineno;			/* line number */
	int		argtailc;		/* argument tail count */
	char		**argtailv;		/* argument tail vector */
	char		*aschema;		/* application schema */
	char		*qarg;			/* quoted argument */

	if (nid_parse(&argc, argv) || (errno = (argc > 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		return(0);
	}
/*
 * Application schema or command line?
 */
	nid_get(&index, &nodeid, &nflags);

	if ((index < 0) && !fl_nprocs) {
/*
 * Parse the app. schema.
 */
		if ((argc != 2) || ao_taken(ad, "s")) {
			fprintf(stderr, usage);
			kexit(EUSAGE);
		}

		aschema = locate_aschema(argv[1]);
		if (aschema == 0) {
			fprintf(stderr, "lamexec (locate_aschema): %s: ",
				argv[1]);
			terror("");
			return(0);
		}

		app = asc_parse(aschema, &lineno, env);
	}
	else {
/*
 * Parse the "command line" application specification.
 * Reconstruct its relevant parts from the parsed information.
 * It was parsed to distinguish between the appl/cmdline cases.
 *
 * Format: [<nodes>] program [-s <srcnode>] [-c #] [-- args]
 *
 */
		fmtbuf[0] = 0;

		if (fl_nprocs) {
			sprintf(smallbuf, "-c %d ", nprocs);
			strcat(fmtbuf, smallbuf);
		}

		if (ao_taken(ad, "s")) {
			strcat(fmtbuf, "-s ");
			strcat(fmtbuf, ao_param(ad, "s", 0, 0));
			strcat(fmtbuf, " ");
		}

		if (index == 0) {
			if (nodeid == LOCAL) nodeid = getnodeid();
			node_ranges(fmtbuf, nodeid, index);
		}

		for (--argc, ++argv; argc > 0; --argc, ++argv) {
			qarg = sfh_argv_quote(*argv, "\\'\"");
			if (qarg == 0) {
				terror("lamexec (sfh_argv_quote)");
				return(0);
			}
			strcat(fmtbuf, qarg);
			strcat(fmtbuf, " ");
			free(qarg);
		}

		ao_tail(ad, &argtailc, &argtailv);
		if (argtailc > 0) strcat(fmtbuf, "-- ");

		for ( ; argtailc > 0; --argtailc, ++argtailv) {
			qarg = sfh_argv_quote(*argtailv, "\\'\"");
			if (qarg == 0) {
				terror("lamexec (sfh_argv_quote)");
				return(0);
			}
			strcat(fmtbuf, qarg);
			strcat(fmtbuf, " ");
			free(qarg);
		}
/*
 * Parse the command line.
 */
		app = asc_bufparse(fmtbuf, strlen(fmtbuf), &lineno, env);
		lineno = 0;
	}

	if (app == 0) {
		if (lineno > 0) {
			fprintf(stderr, syntaxerr, lineno);
			errno = EUSAGE;
		} else {
			fprintf(stderr, "lamexec: cannot parse: ");
			terror("");
		}

		return(0);
	}
/*
 * Set working dir.
 */
	if (wrkdir) {
	    if (asc_workdir(app, wrkdir)) {
		terror("lamexec (asc_workdir)");
		return(0);
	    }
	}
/*
 * Schedule the application.
 */
	app_sched = asc_schedule(app);
	asc_free(app);

	if (app_sched == 0) {
		terror("lamexec (asc_schedule)");
		return(0);
	} else {
		return(app_sched);
	}
}

/*
 *	pwait
 *
 *	Function:	- waits for processes to complete
 *	Accepts:	- # of processes
 *			- return status of first child in error, else 0
 *	Returns:	- 0 or LAMERROR
 */
static int
pwait(int4 nwait, int *childstat)
{
	int4		nodeid;			/* child's node ID */
	int		pid;			/* child's process ID */
	int		status;			/* return status */

	*childstat = 0;

	for ( ; nwait > 0; --nwait) {

		if (rpwait(&nodeid, &pid, &status)) {
			*childstat = errno;
			terror("lamexec (rpwait)");
			return(LAMERROR);
		}

		status &= ~(_LAM_WIFNODETACH);
		if (status) {
		    if (LAM_WIFSIGNALED(status)) {
			printf("%d (n%d) exited due to signal %d\n",
				pid, nodeid, LAM_WTERMSIG(status));
			*childstat = -1;
		    } else {
			status = LAM_WEXITSTATUS(status);
			printf("%d (n%d) exited with status %d\n",
				pid, nodeid, status);
			*childstat = status;
		    }
		    return(LAMERROR);
		}
	}

	return(0);
}

/*
 *	set_mode
 *
 *	Function:	- set the operating mode
 */
static int
set_mode(void)
{
/*
 * flags
 */
    fl_fdesc = ao_taken(ad, "f");
    fl_verbose = ao_taken(ad, "v");
    if (ao_taken(ad, "nw"))
	fl_wait = 0;
/*
 * followed options
 */
    nprocs = -1;
    if (ao_taken(ad, "c")) {
	ao_intparam(ad, "c", 0, 0, &nprocs);
	fl_nprocs = 1;
    } else if (ao_taken(ad, "np")) {
	ao_intparam(ad, "np", 0, 0, &nprocs);
	fl_nprocs = 1;
    }
    
    if (ao_taken(ad, "wd")) {
	wrkdir = ao_param(ad, "wd", 0, 0);
    }
/*
 * runtime flags
 */
    if (fl_wait)
	rtf |= RTF_WAIT;
    if (ao_taken(ad, "D"))
	rtf |= RTF_APPWD;
    if (ao_taken(ad, "O"))
	rtf |= RTF_HOMOG;
    if (isatty(1))
	rtf |= RTF_TTYOUT;
    if (!fl_fdesc)
	rtf |= RTF_IO;
    if (!ao_taken(ad, "npty"))
	rtf |= RTF_PTYS;

    return(0);
}

/*
 *	set_stdio
 *
 *	Function:	- set up application stdio
 *	Returns:	- 0 or LAMERROR
 */
static int
set_stdio(void)
{
	char		server[LAM_PATH_MAX];	/* fd server socket name */

	if (fl_fdesc) return(0);

/*
 * Pass stdin, stdout and stderr to filed.
 */
	if (lam_mktmpid((int) lam_getpid(), server, sizeof(server))) {
		return(LAMERROR);
	}

	if (lam_lfopenfd(server)) {
		return(LAMERROR);
	}
/*
 * Set LAM file descriptors to the passed file descriptors.  The call to
 * lam_lfopenfd() takes care of the case when stdin is a tty.
 */
	_kio.ki_stdin = _ufd[0].fu_tfd;
	_kio.ki_stdout = _ufd[1].fu_tfd;
	_kio.ki_stderr = _ufd[2].fu_tfd;
	return(0);
}

/*
 *	reset_stdio
 *
 *	Function:	- reset stdio so rfatexit will clean it up
 *	Returns:	- 0 or LAMERROR
 */
static void
reset_stdio(void)
{
	_kio.ki_stdin = 0;
	_kio.ki_stdout = 1;
	_kio.ki_stderr = 2;
}

/*
 *	locate_aschema
 *
 *	Function:	- locate an application schema
 *	Accepts:	- filename
 *	Returns:	- full pathname or NULL
 */
static char *
locate_aschema(char *filename)
{
	int		pathc = 0;		/* paths argc */
	char		**pathv = 0;		/* paths argv */
	char		*appdir;		/* application dir */
	char		*fullpath;		/* full pathname */

	if ((appdir = getenv("LAMAPPLDIR"))) {
		sfh_argv_add(&pathc, &pathv, appdir);
	}
	sfh_argv_add(&pathc, &pathv, "");
	sfh_argv_add(&pathc, &pathv, "$LAMHOME/etc");
	sfh_argv_add(&pathc, &pathv, "$TROLLIUSHOME/etc");
	sfh_argv_add(&pathc, &pathv, LAM_SYSCONFDIR);

	fullpath = sfh_path_find(filename, pathv, R_OK);
	sfh_argv_free(pathv);
	return(fullpath);
}

/*
 *	node_ranges
 *
 *	Function:	- fill buffer with list of nodes
 *			- compact nodes to ranges
 *			- formatting buffer assumed large enough
 *	Accepts:	- formatting buffer
 *			- initial node
 *			- initial index
 */
static void
node_ranges(char *buf, int4 initnode, int4 initindex)
{
	int4		node;			/* target node */
	int4		index;			/* for getnode() */
	int4		flags;			/* for getnode() */
	int4		next;			/* next expected node */
	int		fl_flush = 0;		/* flush node flag */

	sprintf(smallbuf, "n%d", initnode);
	strcat(buf, smallbuf);
/*
 * Loop over the nodes.
 */
	next = initnode + 1;

	nid_get(&index, &node, &flags);

	while (index != initindex) {
		if (node != next) {
			if (next > initnode + 1) {
/*
 * We have a "hole" ending a range of length >= 2.
 */
				sprintf(smallbuf, "-%d,%d", next - 1, node);
				strcat(buf, smallbuf);
			}
			else {
/*
 * We have a "hole" ending a range of length 1.
 */
				sprintf(smallbuf, ",%d", node);
				strcat(buf, smallbuf);
			}
			initnode = node;
			next = node + 1;
			fl_flush = 0;
		}
/*
 * Otherwise, keep incrementing.
 */
		else {
			++next;
			fl_flush = 1;
		}

		nid_get(&index, &node, &flags);
	}
/*
 * If we were still in a range, flush it out.
 */
	if (fl_flush) {
		sprintf(smallbuf, "-%d", next - 1);
		strcat(buf, smallbuf);
	}

	strcat(buf, " ");
}

/*
 *	signal_handler
 *
 *	Function:	- dooms application started by lamexec
 *	Accepts:	- signal
 */
static void
signal_handler(int sig)
{
	static int	numsigs = 0;		/* # times signal caught */
	int		pid;

	if (sig != SIGINT && sig != SIGTERM) 
	  return;
/*
 * Second time around just exit.
 */
	if (numsigs++ > 0) {
		exit(1);
	}

	if ((pid = fork()) < 0) {
		return;
	}
	else if (pid == 0) {
		lam_reset_pid_cache();
		if (kinit(PRCMD)) {
			exit(errno);
		}

		app_doom(job_n, job, -15);	/* deliver SIGTERM */
		kexit(0);
	}
}

/*
 *	error_cleanup
 *
 *	Function:	- try to clean up wait messages
 *			- this is not foolproof but better than nothing
 */
static void
error_cleanup(void)
{
    struct nmsg		msg;
/*
 * Wait a little while.
 */
    sleep(1);

    LAM_ZERO_ME(msg);
    msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
    msg.nh_length = 0;
    msg.nh_flags = 0;
/*
 * Loop trying to receive wait messages.
 */
    while (1) {
	msg.nh_type = 1;
	if (ntry_recv(&msg))
	    break;
    }
}

static void
first_signal_handler(int sig) {
  static int numsig = 0;
  static char error_message[] = "\
******************** WARNING ************************\n\
This is a vulnerable region. Exiting the application\n\
now may lead to improper cleanup of temporary objects\n\
To exit the application, press Ctrl-C again\n\
******************** WARNING *************************\n";

  if (sig != SIGINT && sig != SIGTERM) {
     return;
   }
 /* He really wants to quit, call the generic signal handler and
  * Note: signal_handler() does not return
  */
   if (numsig++ > 0) {
     signal_handler (sig);
   }
  /* This is the first time we have caught the signal */
  else {
    write (1, error_message, sizeof (error_message));
    return;
  }
}

