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
 * $Id: di_main.c,v 6.18 2003/05/10 23:37:14 jsquyres Exp $
 * 
 *	Function:	- datalink input proprietor
 */

#include <lam_config.h>
#include <all_opt.h>
#include <etc_misc.h>
#include <kreq.h>
#include <laminternal.h>
#include <lamdebug.h>
#include <priority.h>
#include <terror.h>
#include <kio.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * external functions
 */
void			(*(di_inet()))();
void			(*(di_init()))();

/*
 * external variables (from share/kreq/clientio.c; extern'ed in
 * <mpisys.h>, but don't want to include all of that here)
 */
extern struct kio_t _kio;


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
  ao_setopt1(lam_daemon_optd, "xd", 0, 0, 0);
  ao_setopt(lam_daemon_optd, "n", 0, 1, AOINT);
  ao_setopt(lam_daemon_optd, "c", 0, 1, AOINT);
  ao_setopt(lam_daemon_optd, "o", 0, 1, AOINT);
  ao_setopt(lam_daemon_optd, "p", 0, 1, AOINT);
  ao_setopt(lam_daemon_optd, "H", 0, 1, 0);
  ao_setopt(lam_daemon_optd, "P", 0, 1, AOINT);
  ao_setopt(lam_daemon_optd, "sessionprefix", 0, 1, 0);
  ao_setopt(lam_daemon_optd, "sessionsuffix", 0, 1, 0);

  if (ao_parse(lam_daemon_optd, &argc, argv)) {
    show_help("dli_inet", "usage", NULL);
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
    lamopenlog("dli_inet");
    lamlog("started (%s), uid %d, gid %d", LAM_VERSION,
	   getuid(), getgid());
    lam_comm_debug_open(0, LAM_COMM_DEBUG_DLI);
  }

/*
 * Attach to kernel.
 */
	if (kinit(PRDLI)) {
	  lampanic("dli_inet (kinit)");
	}

        /* Pre-fill the _kio.ki_nodeid value so that we can call
           getnodeid() and not have to call the router to find our
           node value.  Do this because: a) we can know our node ID
           from the command line options that we just parsed, and b)
           the boot SSI module (more specifically: the SSI base
           itself) prints out verbose/debugging messages with "n%d",
           where %d is filled with the result of getnodeid() -- if
           this value is filled in, getnodeid() will just return it
           and won't make a call to the router.  This is *really*
           helpful information to have, particularly when diagnosing
           problems with boot SSI modules. */

        ao_intparam(lam_daemon_optd, "n", 0, 0, &_kio.ki_nodeid);

	di_init(argc, argv);
/*
 * server loop
 */
	for (;;) {

		if (!di_inet()) {
			kexit(1);
			break;
		}
	}

	return(0);
}
