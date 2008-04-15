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
 *	$Id: tkill.c,v 6.33 2003/04/09 15:26:28 brbarret Exp $
 *
 *	Function:	- kills OTB environment
 *			- reads process IDs from the kill file, aka
 *			  reset file, aka lock file for historical reasons
 *			- makes a very good attempt at ensuring the
 *			  death of each pid in the file
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#include <all_opt.h>
#include <args.h>
#include <lamdebug.h>
#include <terror.h>
#include <laminternal.h>
#include <etc_misc.h>

#define TIMEOUT		10000000	/* timeout on process death */


/*
 * global variables
 */
int		fl_debug;	/* debugging option */
int             fl_verbose;     /* debugging option */

int
main(int argc, char *argv[])
{
	FILE		*fp_kill;	/* kill file ptr */
	int		fd_kill;	/* kill file desc. */
	int		fl_pretend;	/* pretend option */
	int		n;		/* favourite counter */
	int		pid;		/* lots of space to hold a PID */
	unsigned int	usec;		/* microseconds to sleep */
	char		*f_kill;	/* kill file */
	char		*f_sock;	/* socket file */
	char		*f_iosock;	/* io daemon socket file */
	char            extra[1024];    /* place for ASCII messages */
	char *prefix = NULL;
	char *suffix = NULL;
        OPT             *ad = NULL;
	int             ret;
	int             debug;
	lam_debug_stream_info_t debug_lds;
	char            *debug_loc;

/*
 * Initialize option parser.
 */
	ad = ao_init();
	if (ad == 0) {
	  ret = errno;
	  perror("hboot (ao_init)");
	  exit(ret);
	}
	ao_setflags(ad, AOPRESERVE_ARGV);
	ao_setopt1(ad, "dhvN", 0, 0, 0);
	ao_setopt(ad, "sessionprefix", 0, 1, 0);
	ao_setopt(ad, "sessionsuffix", 0, 1, 0);
	ao_setopt(ad, "setsid", 0, 0, 0);
	ao_setopt(ad, "f", 0, 1, 0);
	ao_setopt(ad, "debug-output", 0, 1, 0);

	if (ao_parse(ad, &argc, argv)) {
	  show_help("tkill", "usage", NULL);
	  exit(EUSAGE);
	}

	if (ao_taken(ad, "setsid"))
	  setsid();

	/* Root can only run this program if "-N" is supplied (i.e., if it
	   was launched from recon */

	fl_pretend = ao_taken(ad, "N");
#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (!fl_pretend && (getuid() == 0 || geteuid() == 0)) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

	if (ao_taken(ad, "h")) {
	    show_help("tkill", "usage", NULL);
	    ao_free(ad);
	    exit(0);
	}

	/*
	 * debugging option parsing code
	 */
	fl_debug = ao_taken(ad, "d");
	fl_verbose = ao_taken(ad, "v") || fl_debug;

	debug_lds.lds_fl_debug = 0;
	debug_lds.lds_fl_syslog = 0;
	debug_lds.lds_fl_stdout = 0;
	debug_lds.lds_fl_stderr = 0;
	debug_lds.lds_fl_file = 0;
	debug_lds.lds_prefix = "tkill: ";

	if (ao_taken(ad, "debug-output")) {
	  debug_loc = ao_param(ad, "debug-output", 0, 0);
	  if (!strcmp(debug_loc, "stdout")) {
	    debug_lds.lds_fl_stdout = 1;
	  } else if (!strcmp(debug_loc, "syslog")) {
	    debug_lds.lds_fl_syslog = 1;
	    debug_lds.lds_syslog_priority = LOG_INFO;
	    debug_lds.lds_syslog_ident = argv[0];
	  } else {
	    show_help("tkill", "usage", NULL);
	    exit(EUSAGE);
	  }
	} else {
	  debug_lds.lds_fl_stdout = 1;
	}
	debug = lam_debug_open(&debug_lds);
	/* turn-off at init appears "not good" */
	lam_debug_switch(debug, fl_debug);

	/*
	 * Batch system gorp
	 */
	if (ao_taken(ad, "sessionprefix")) {
	  prefix = ao_param(ad, "sessionprefix", 0, 0);
	} else {
	  prefix = NULL;
	}

	if (ao_taken(ad, "sessionsuffix")) {
	  suffix = ao_param(ad, "sessionsuffix", 0, 0);
	} else {
	  suffix = NULL;
	}

	lam_debug(debug, "setting prefix to %s",
		  (prefix == NULL) ? "(null)" : prefix);
	lam_debug(debug, "setting suffix to %s",
		  (suffix == NULL) ? "(null)" : suffix);
	lam_tmpdir_init(prefix, suffix);
	f_kill = lam_get_killfile();
	lam_debug(debug, "got killname back: %s\n", f_kill);
	free(f_kill);

/*
 * Get the kill filename.
 */
	if (ao_taken(ad, "f")) {
	    f_kill = ao_param(ad, "f", 0, 0);
	} else {
	    f_kill = lam_get_killfile();

	    if (f_kill == 0) {
	      show_help(NULL, "lib-call-fail", "killname", NULL);
	      exit(errno);
	    }
	}
/*
 * Dump debugging information.
 */
	lam_debug(debug, "f_kill = \"%s\"\n", f_kill);

/*
 * Try to open the file.
 */
	if ((fd_kill = open(f_kill, O_RDWR, 0)) < 0) {

	    if (errno == ENOENT) {
	      lam_tmpdir_remove();
	      lam_debug(debug, "nothing to kill: \"%s\"\n", f_kill);
              lam_debug_close(debug);
	      if (!ao_taken(ad, "f"))
		free(f_kill);
              ao_free(ad);
	      exit(0);
	    } else {
	      snprintf(extra, 1024, "open(\"%s\", O_RDWR)", f_kill);
	      show_help(NULL, "system-call-fail", extra, NULL);
	      exit(errno);
	    }
	}

	fp_kill = fdopen(fd_kill, "r");

	if (! fp_kill) {
	  snprintf(extra, 1024, "fdopen(\"%s\", \"r\")", f_kill);
	  show_help(NULL, "lib-call-fail", extra, NULL);
	  exit(errno);
	}

	lam_debug(debug, "killing LAM...\n");

	n = fscanf(fp_kill, "%d", &pid);

	while (n > 0) {
	  lam_debug(debug, "killing PID (SIGHUP) %d ...", pid);

/*
 * Send SIGHUP to the process.
 */
	    if (fl_pretend) {
	        lam_debug(debug, "\n");
		n = fscanf(fp_kill, "%d", &pid);
		continue;
	    }

	    if (kill(pid, SIGHUP)) {

		if (errno == ESRCH) {
		    lam_debug(debug, " already dead");
                } else if (errno == EPERM) {
                    /* 
                     * So if something happens to a process and its
                     * pid ends up being reused and we try to kill it
                     * and it's someone else's process, tkill used to
                     * get annoyed and complain on stderr.  The
                     * problem is that lamboot takes that to mean
                     * something really bad happens and aborts the
                     * lamboot.  Since that seems silly, we are
                     * instead going to print a warning and continue
                     * on.
                     */
                    lam_debug(debug, " permission denied (ignoring pid)");
		} else {
		    lam_debug(debug, "\n");
		    perror("tkill (kill)");
		    exit(errno);
		}
	    } else {
		usec = 2;

		while ((kill(pid, SIGHUP) == 0) && (usec <= TIMEOUT)) {
		    microsleep(usec);
		    usec *= 2;
		}
	
		if (usec > TIMEOUT) {
		    lam_debug(debug, "trying -9 ...");
		    kill(pid, SIGKILL);
		    microsleep(500000);

		    if (kill(pid, SIGHUP)) {
			lam_debug(debug, "killed");
		    } else {
			lam_debug(debug, "cannot kill");

			snprintf(extra, 1024, "%d", pid);
			show_help("tkill", "cannot-kill", extra, NULL);
		    }
		} else {
		    lam_debug(debug, "killed");
		}
	    }
		
	    n = fscanf(fp_kill, "%d", &pid);
	}

	if (fclose(fp_kill)) {
	  snprintf(extra, 1024, "fclose(for file \"%s\")", f_kill);
	  show_help(NULL, "lib-call-fail", extra, NULL);
	  exit(errno);
	}

	if (fl_pretend) {
	    exit(0);
	}
/*
 * Cleanup all registered objects.
 */
	lam_cleanup_objects();

/*
 * Remove the socket file.
 */
	lam_debug(debug, "removing socket file ...\n");
	f_sock = lam_get_sockname();

	if (f_sock == 0) {
	  show_help("lam-temp-files", "badness", "tkill/sockname", NULL);
	  exit(errno);
	}

	if (!fl_pretend) {
	  lam_debug(debug, "socket file: %sd\n", f_sock);
	    if (unlink(f_sock)) {
		if (errno != ENOENT) {
		  snprintf(extra, 1024, "unlink(\"%s\")", f_sock);
		  show_help(NULL, "system-call-fail", extra, NULL);
		}
	    }
	}
/*
 * Remove the IO daemon socket file.
 */
	lam_debug(debug, "removing IO daemon socket file ...\n");
	f_iosock = lam_get_iosockname();

	if (f_iosock == 0) {
	  show_help("lam-temp-files", "badness", "tkill/iosockname", NULL);
	  exit(errno);
	}

	if (!fl_pretend) {
	    lam_debug(debug, "IO daemon socket file: %s\n", f_iosock);
	    if (unlink(f_iosock)) {
		if (errno != ENOENT) {
		  snprintf(extra, 1024, "unlink(\"%s\")", f_iosock);
		  show_help(NULL, "system-call-fail", extra, NULL);
		}
	    }
	}

/*
 * Remove the session directory.  Go remove all entries in it first.
 */
	lam_tmpdir_remove();

	if (fl_debug) {
	  lam_debug(debug, "all finished\n");
	  fflush(stdout); 
          lam_debug_close(debug);
	}

	if (!ao_taken(ad, "f"))
	  free(f_kill);
	free(f_sock);
	free(f_iosock);
        ao_free(ad);
	return(0);
}
