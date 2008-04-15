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
 * $Id: ssi_boot_globus_inetexec.c,v 1.8 2004/03/06 21:17:32 jsquyres Exp $
 * 
 */

#include <lam_config.h>
#include <lam-ssi-boot-globus-config.h>

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
#include <sfh.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>
#include <lam-ssi-boot-globus.h>


/*
 * static functions
 */

static void add_globusrun(int *argc, char ***argv, char *globusloc);


int
lam_ssi_boot_globus_inetexec(struct lamnode *node, char **argv)
{
  char **cmdv;			/* globus command argv */
  int cmdc;			/* globus command argc */
  int i;			/* favorite counter */
  char printable[BUFSIZ];	/* command to exec */
  char remote_host[BUFSIZ];	/* username@hostname */
  char *cmdv0;			/* Copy of cmdv[0] */
  ELEM search;
  ELEM *contact;

  snprintf(remote_host, BUFSIZ, "%s", node->lnd_hname);
  remote_host[BUFSIZ - 1] = '\0';

  /* Remotely execute the command */

  cmdc = 0;
  cmdv = 0;

  add_globusrun(&cmdc, &cmdv, argv[0]);
  /* get the Globus contact string if available, else the hostname is
     the contact string */
  search.key = "contact";
  contact = al_find(node->lnd_keyval, &search);
  if (contact != NULL) {
    /* got contact entry in keyval list */
    sfh_argv_add(&cmdc, &cmdv, contact->value);
  } else {
    sfh_argv_add(&cmdc, &cmdv, node->lnd_hname);
  }
 
  for (i = 1; argv[i]; ++i)
    sfh_argv_add(&cmdc, &cmdv, argv[i]);

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did,
	      "globus: attempting to execute \"%s\"",
	      sfh_argv_glue(cmdv, ' ', 0));

  /* Build the sample command to pass to the error routine.  Have to
     do this ahead of time, because ioexecvp frees the argv array. */

  printable[0] = '\0';
  for (i = 0; i < cmdc; i++) {
    strncat(printable, cmdv[i], BUFSIZ);
    strncat(printable, " ", BUFSIZ);
  }

  cmdv0 = strdup(cmdv[0]);
  i = lam_ssi_boot_base_ioexecvp(cmdv, 1, (char *) 0, 0, 1);

  /* Do we need to print an error message? */

  if (i) {
    show_help_file("lam-ssi-boot-globus-helpfile", "boot", 
		   "remote-boot-fail-globus", remote_host, cmdv0,
		   argv[1], printable, NULL);
    free(cmdv0);
    return (LAMERROR);
  }

  free(cmdv0);
  return 0;
}


/*
 *	add_globusrun
 *
 *	Function:	- add "globus-job-run" to the argc/argv array
 *			- be sure to parse into individual words to add nicely
 *	Accepts		- ptr to length of array
 *			- ptr to array
 *                      - ptr to globuslocation
 */
static void
add_globusrun(int *cmdc, char ***cmdv, char *globuslocation)
{
  *cmdv = sfh_argv_break(globuslocation, ' ');
  *cmdc = sfh_argv_count(*cmdv);
}
