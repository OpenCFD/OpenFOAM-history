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
 * $Id: hboot.c,v 6.35 2004/01/02 00:20:46 jsquyres Exp $
 *
 *	Function:	- boots OTB operating system
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#include <lam_config.h>
#include <all_list.h>
#include <args.h>
#include <all_opt.h>
#include <boot.h>
#include <lamdebug.h>
#include <kio.h>
#include <portable.h>
#include <proc_schema.h>
#include <sfh.h>
#include <terror.h>
#include <etc_misc.h>

#define MAXCLOSEFD	32			/* max close-on-exec fdecs. */
#define LAM_MAXPATHLEN     255                      /* max path name len */
/*
 * external functions
 */
extern int		_lam_few(char**);
extern int		psc_parse();
extern struct psc	*psc_find();
extern struct psc       *psc_fill();


/*
 * local functions
 */
static void setdefaults(void);


/*
 * local variables
 */
static char		*t_tkill;		/* tkill tool */
static char		rtfbuf[32];		/* RTF env. var. */

static int		fl_debug;		/* debugging option */
static int		fl_verbose;		/* verbose option */
static OPT              *ad = NULL;


int
main(int argc, char *argv[])
{
	LIST		*list_psc;	/* parsed process schema list */
	struct psc	*p;
	int		i, n;
	int		fd;		/* file descriptor for /dev/null */
	int		pid;		/* child PID */
	int		ac_cmd;		/* # command arguments */
	char		**av_cmd;	/* command arguments */
	char		buf[32];	/* formatting buffer */
	char		*tail;		/* tail of full pathname */
	char            *inet_topo;
        char            *rtr_topo;
	int             ret;
	char            *debug_loc;
	char            *prefix = NULL;
	char            *new_path = NULL;
	char            *opt_prefix;
	char            *tkill_path;
	char            *path_env;
	int             prefix_len;
	int             fl_prefix;
	int             tkill_len;
	int             debug;
	int             len;
	lam_debug_stream_info_t debug_lds;

	/* Ensure that we are not root */
#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

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
	ao_setopt1(ad, "dhstvNVn", 0, 0, 0);
	ao_setopt(ad, "c", 0, 1, 0);
	ao_setopt(ad, "I", 0, 1, 0);
	ao_setopt(ad, "R", 0, 1, 0);
	ao_setopt(ad, "sessionprefix", 0, 1, 0);
	ao_setopt(ad, "sessionsuffix", 0, 1, 0);
	ao_setopt(ad, "H", 0, 1, 0);
	ao_setopt(ad, "debug-output", 0, 1, 0);
	ao_setopt(ad, "prefix", 0, 1, 0);

	if (ao_parse(ad, &argc, argv)) {
	  show_help("hboot", "usage", NULL);
	  exit(EUSAGE);
	}

	if (ao_taken(ad, "h")) {
	  show_help("hboot", "usage", NULL);
	  exit(0);
	}

	/* set some generic defaults */
	setdefaults();

	debug_lds.lds_fl_debug = 0;
	debug_lds.lds_fl_syslog = 0;
	debug_lds.lds_fl_stdout = 0;
	debug_lds.lds_fl_stderr = 0;
	debug_lds.lds_fl_file = 0;
	debug_lds.lds_prefix = "hboot: ";

	if (ao_taken(ad, "debug-output")) {
	  debug_loc = ao_param(ad, "debug-output", 0, 0);
	  if (!strcmp(debug_loc, "stdout")) {
	    debug_lds.lds_fl_stdout = 1;
	  } else if (!strcmp(debug_loc, "syslog")) {
	    debug_lds.lds_fl_syslog = 1;
	    debug_lds.lds_syslog_priority = LOG_INFO;
	    debug_lds.lds_syslog_ident = argv[0];
	  } else {
	    show_help("hboot", "usage", NULL);
	    exit(EUSAGE);
	  }
	} else {
	  debug_lds.lds_fl_stdout = 1;
	}
	debug = lam_debug_open(&debug_lds);
	/* turn-off at init appears "not good" */
	lam_debug_switch(debug, fl_debug || fl_verbose);

	if (ao_taken(ad, "I")) {
	  inet_topo = ao_param(ad, "I", 0, 0);
	  /* Remove quotes if enclosed in quotes */
	  if (inet_topo[0] == '"') {
	    inet_topo[strlen(inet_topo) - 1] = '\0';
	    inet_topo = &inet_topo[1];
	  }
	} else {
	  inet_topo = NULL;
	}

	if (ao_taken(ad, "R")) {
	  rtr_topo = ao_param(ad, "R", 0, 0);
	} else {
	  rtr_topo = NULL;
	}

	/* get list of things we should do */
	if (hbootparse(debug, ad, inet_topo, rtr_topo, &list_psc)) {
	  show_help("hboot", "cant-parse-config", NULL);
	  exit(1);
	}

	/* get the path prefix if provided */
	fl_prefix = ao_taken(ad, "prefix");

	if (fl_prefix) {
	  opt_prefix = ao_param(ad, "prefix", 0, 0);
	  prefix_len = strlen(opt_prefix) + strlen("/bin/") + 1;
	  prefix = (char *) malloc (prefix_len * sizeof(char));
	  if (prefix == NULL) {
	    show_help(NULL, "lib-call-fail", "malloc", NULL);
	    return LAMERROR;
	  }
	  snprintf(prefix, prefix_len, "%s/bin/", opt_prefix);

	  /* Prepend the prefix to the existing env path, so that lamd
	     can call the correct tkill during the lamhalt, by
	     looking at the path  */	

          len = strlen(prefix) + 16;
	  if ((path_env = getenv("PATH")) != NULL) {
              len += strlen(path_env);
              new_path = malloc(len);
              if (new_path == NULL) {
                  show_help(NULL, "lib-call-fail", "malloc", NULL);
                  return LAMERROR;
              }
              snprintf(new_path, len, "PATH=%s:%s", prefix, path_env);
          } else {
              new_path = malloc(len);
              if (new_path == NULL) {
                  show_help(NULL, "lib-call-fail", "malloc", NULL);
                  return LAMERROR;
              }
              snprintf(new_path, len, "PATH=%s", prefix);
          }
	
	  if (putenv(new_path) < 0) {
	      show_help(NULL, "lib-call-fail", "putenv", NULL);
	      exit(errno);
	  }
	}
/*
 * Bail out here, if pretending.
 */
	if (ao_taken(ad, "N")) {
	        if (fl_verbose)
		  printf("Fake hboot -- quitting\n");
		exit(0);
	}
/*
 * Tkill if needed.
 */
	if (ao_taken(ad, "t")) {
		DBUG("hboot: performing %s\n", t_tkill);

		ac_cmd = 0;
		av_cmd = 0;
		
		if (fl_prefix) {
		  tkill_len = strlen(prefix) + strlen(t_tkill) + 1;
		  tkill_path = (char *) malloc (tkill_len * sizeof(char));
		  
    	  	  if (tkill_path == NULL) {
	    	    show_help(NULL, "lib-call-fail", "malloc", NULL);
		    return LAMERROR;
	  	  }
		  snprintf(tkill_path, tkill_len, "%s%s", prefix, t_tkill);
		  sfh_argv_add(&ac_cmd, &av_cmd, tkill_path);
		  free(tkill_path);
		  free(prefix);
		}
		else
		  sfh_argv_add(&ac_cmd, &av_cmd, t_tkill);

		if (ao_taken(ad, "sessionprefix")) {
		  sfh_argv_add(&ac_cmd, &av_cmd, "-sessionprefix");
		  sfh_argv_add(&ac_cmd, &av_cmd, 
			  ao_param(ad, "sessionprefix", 0, 0));
		}
		if (ao_taken(ad, "sessionsuffix")) {
		  sfh_argv_add(&ac_cmd, &av_cmd, "-sessionsuffix");
		  sfh_argv_add(&ac_cmd, &av_cmd, 
			  ao_param(ad, "sessionsuffix", 0, 0));
		}
		if (fl_debug) {
		  sfh_argv_add(&ac_cmd, &av_cmd, "-d");
		  printf("hboot: ");
		  for (i = 0; i < ac_cmd; i++)
		    printf("%s ", av_cmd[i]);
		  printf("\n");
		}
		if (_lam_few(av_cmd)) {
		  show_help("hboot", "tkill-fail", NULL);
		  exit(errno);
		}
	} 
	else if (fl_prefix)
	  free(prefix);
/*
 * Boot.
 */
	DBUG("hboot: booting...\n");

	sprintf(rtfbuf, "TROLLIUSRTF=%d", RTF_SYSGEN);

	if (putenv(rtfbuf) < 0) {
	  show_help(NULL, "lib-call-fail", "putenv", NULL);
	  exit(errno);
	}

        /* Take us out of the parent's group so that we don't get
           killed.  NOTE: this does *not* hurt us in environment such
           as TM and SLURM because hboot is not used in these
           environments. */

	setsid();

	if (ao_taken(ad, "s")) {
/*
 * Make any extraneous file descriptors close-on-exec.
 */
		for (i = 3; i < MAXCLOSEFD; ++i) {
			if ((fcntl(i, F_SETFD, 1) != 0) && (errno != EBADF)) {
			  show_help(NULL, "system-call-fail", 
				    "fcntl(set close-on-exec)", NULL);
			  exit(errno);
			}
		}
	}

	n = 0;
/*
 * Loop through all the programs in the parsed config file.
 */
	for (p = al_top(list_psc); p; p = al_next(list_psc, p)) {
		DBUG("hboot: fork %s\n", p->psc_argv[0]);

		if ((pid = fork()) < 0) {
		  show_help(NULL, "system-call-fail", "fork", NULL);
		  exit(errno);
		}

		else if (pid == 0) {		/* child */
                        /* Put this setsid() here mainly for SGE --
                           their tight integration with LAM/MPI does
                           something like this:

                           lamboot -> qrsh (to a remote node) -> hboot
                           -> qrsh -> lamd

                           Without having a setsid() here, there is a
                           race condition between when hboot quits and
                           SGE thinks the job is over (and therefore
                           starts killing things) and when the second
                           qrsh is able to establish itself and/or the
                           lamd and tell SGE that the job is, in fact,
                           *not* over.  So putting a setsid() here in
                           the child, then the hboot child (and
                           therefore the vulnerable period of the 2nd
                           qrsh) escape being killed by SGE while
                           still making progress on the overall
                           lamboot.
                        */
                        setsid();
			if (ao_taken(ad, "s")) {
/*
 * Safely get rid of the stdio descriptors.
 */
				if ((fd = open("/dev/null", O_RDWR)) < 0) {
				  show_help(NULL, "system-call-fail", 
					    "open(\"/dev/null\"/, O_RDWR)", 
					    NULL);
				  exit(errno);
				}

				if ((dup2(fd, 0) < 0) || (dup2(fd, 1) < 0) ||
						(dup2(fd, 2) < 0)) {
				  show_help(NULL, "system-call-fail", "dup2",
					    NULL);
				  exit(errno);
				}

				close(fd);
			}

			if (fl_debug) {
			  printf("hboot: attempting to execute \n");
			}
			execvp(p->psc_argv[0], p->psc_argv);
			exit(errno);
		}

		else {				/* parent */
			n++;

			if (fl_debug) {
				tail = strrchr(p->psc_argv[0], STRDIR);
				tail = (tail) ? tail + 1 : p->psc_argv[0];
				sprintf(buf, "[%d]", n);
				printf("%-4.4s %5d %s", buf, pid, tail);

				for (i = 1; i < p->psc_argc; i++) {
					printf(" %s", p->psc_argv[i]);
				}

				printf("\n");
			}
		}

		if (p->psc_delay > 0) {
			sleep((unsigned int) p->psc_delay);
		}
	}

	al_free(list_psc);

	return(0);
}

/*
 *	setdefaults
 *
 *	Function:	- sets default files and paths
 */
static void
setdefaults(void)
{
/*
 * flags
 */
	fl_debug = ao_taken(ad, "d");;
	fl_verbose = ao_taken(ad, "v");

	t_tkill = DEFTRESETH;
}
