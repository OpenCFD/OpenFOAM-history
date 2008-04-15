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
 * $Id: bw_main.c,v 1.12 2003/03/21 23:24:16 brbarret Exp $
 * 
 *	Function:	- bufferd helper
 */

#include <lam_config.h>
#include <all_opt.h>
#include <etc_misc.h>
#include <kreq.h>
#include <laminternal.h>
#include <lamdebug.h>
#include <priority.h>
#include <terror.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


/*
 * external functions
 */
extern void		(*(bw_init()))();
extern void		(*(bforward()))();

/*
 * global (to lam daemons) options
 */
OPT* lam_daemon_optd;

int
main(int argc, char *argv[])
{
  char *prefix = NULL;
  char *suffix = NULL;

  /* Ensure that we are not root */
#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0) {
    show_help(NULL, "deny-root", NULL);
    exit(EACCES);
  }
#endif

  /* Be safe; blanket coverage */
  umask(077);

  /*
   * Initialize.
   */
  lam_daemon_optd = ao_init();
  ao_setopt1(lam_daemon_optd, "d", 0, 0, 0);
  ao_setopt(lam_daemon_optd, "sessionprefix", 0, 1, 0);
  ao_setopt(lam_daemon_optd, "sessionsuffix", 0, 1, 0);

  if (ao_parse(lam_daemon_optd, &argc, argv)) {
    show_help("bforward", "usage", NULL);
    exit(EUSAGE);
  }

  /*
   * See if we're running under a batch system, and if so, set the unix
   * socket name
   */
  if (ao_taken(lam_daemon_optd, "sessionprefix")) {
    prefix = ao_param(lam_daemon_optd, "sessionprefix", 0, 0);
  }
  if (ao_taken(lam_daemon_optd, "sessionsuffix")) {
    suffix = ao_param(lam_daemon_optd, "sessionsuffix", 0, 0);
  }
  lam_tmpdir_init(prefix, suffix);

  /*
   * If we were started with '-d', setup to do some syslog debugging messages
   */
  if (ao_taken(lam_daemon_optd, "d")) {
    lamopenlog("bforward");
    lamlog("started (%s), uid %d, gid %d", LAM_VERSION,
	   getuid(), getgid());
  }

/*
 * Attach to kernel.
 */
	if (kinit(PRBFORWARD)) {
		lampanic("bforward (kinit)");
	}
/*
 * Initialize.
 */
	bw_init(&argc, argv);
/*
 * server loop
 */
	for (;;) {

		if (!bforward()) {
			kexit(1);
			break;
		}
	}

	return(0);
}
