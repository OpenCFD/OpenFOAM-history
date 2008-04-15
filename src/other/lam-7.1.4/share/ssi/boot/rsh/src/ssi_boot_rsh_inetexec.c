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
 * $Id: ssi_boot_rsh_inetexec.c,v 1.16 2004/03/06 21:17:32 jsquyres Exp $
 * 
 */

#include <lam_config.h>
#include <lam-ssi-boot-rsh-config.h>

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
#include <lam-ssi-boot-rsh.h>


/*
 * Local functions
 */
static char *local_argv_glue(char **argv, int delim);


int
lam_ssi_boot_rsh_inetexec(struct lamnode *node, char **argv)
{
  char **cmdv;			/* rsh command argv */
  int cmdc;			/* rsh command argc */
  int fl_csh;			/* csh-flavoured flag */
  int fl_bash;			/* bash-flavoured flag */
  int i;			/* favorite counter */
  char *printable;		/* command to exec */
  char remote_host[BUFSIZ];	/* username@hostname */
  char *cmdv0;			/* Copy of cmdv[0] */
  char shellpath[BUFSIZ];	/* return of 'echo $SHELL' */
  struct passwd *p;
  int fl_fast = ao_taken(lam_ssi_boot_optd, "b");
  int fl_no_n = 0;
  int fl_no_profile = 0;
  int fl_ignore_stderr = 0;
  int stderr_is_err = 1;

  /* Special command line options */

  fl_no_n = 
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_no_n);
  fl_no_profile = 
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_no_profile);
  fl_fast = 
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_fast);
  fl_ignore_stderr =
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_ignore_stderr);
  if (fl_ignore_stderr != 0) {
    stderr_is_err = 0;
  }

  /* Get the user's shell by executing 'echo $SHELL' on remote
     machine, but only if they asked for it via not specifying
     fl_fast.  Since *only* the bourne and korn shells require this,
     and *most* users don't use plain old bourne/korn (bash does *not*
     require explicitly running .profile), only do this if
     specifically requested, 'cause we can ditch one of the 2 rsh's,
     and double the speed of things like lamboot, recon, and wipe.  */

  /* Build the username/hostname to pass to the error routine */

  memset(remote_host, 0, BUFSIZ);
  if (node->lnd_uname != NULL) 
    snprintf(remote_host, BUFSIZ, "%s@%s", node->lnd_uname, node->lnd_hname);
  else
    snprintf(remote_host, BUFSIZ, "%s", node->lnd_hname);
  remote_host[BUFSIZ - 1] = '\0';

  /* If we don't need to look for the .profile, it's a lot easier */

  if (fl_no_profile) {
    fl_csh = 1;
    fl_bash = 0;
  } 

  /* If we didn't specify to be "fast", look for .profile */

  else if (!fl_fast) {
    cmdv = sfh_argv_dup(lam_ssi_boot_rsh_agent);
    cmdc = sfh_argv_count(lam_ssi_boot_rsh_agent);
    sfh_argv_add(&cmdc, &cmdv, node->lnd_hname);
    if (!fl_no_n)
      sfh_argv_add(&cmdc, &cmdv, "-n");
    if (node->lnd_uname != NULL) {
      sfh_argv_add(&cmdc, &cmdv, "-l");
      sfh_argv_add(&cmdc, &cmdv, node->lnd_uname);
    }
    sfh_argv_add(&cmdc, &cmdv, "echo $SHELL");

    /* Assemble a string for possible error usage -- argv gets freed
       in ioexecvp */

    printable = local_argv_glue(cmdv, ' ');
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did,
		"rsh: attempting to execute: %s", printable);

    cmdv0 = strdup(cmdv[0]);
    shellpath[sizeof(shellpath) - 1] = '\0';
    if (lam_ssi_boot_base_ioexecvp(cmdv, 0, shellpath,
				   sizeof(shellpath) - 1, stderr_is_err)) {
      if (errno == EFAULT)
	show_help("boot", "remote-stderr", remote_host, cmdv0,
		  "echo $SHELL", printable, NULL);
      else
	show_help("boot", "remote-shell-fail", remote_host, cmdv0,
		  "echo $SHELL", printable, NULL);
      free(printable);
      free(cmdv0);
      return (LAMERROR);
    }

    /* Did we get valid output? */
    
    if (strlen(shellpath) == 0) {
      show_help("boot", "no-shell", remote_host, cmdv0, "echo $SHELL",
		printable, NULL);
      free(printable);
      free(cmdv0);
      return (LAMERROR);
    }
    free(cmdv0);
    free(printable);
    
    /* Perl chomp */
    
    if (shellpath[strlen(shellpath) - 1] == '\n')
      shellpath[strlen(shellpath) - 1] = '\0';
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "rsh: remote shell %s", shellpath);
    fl_csh = (strstr(shellpath, "csh") != 0) ? TRUE : FALSE;
    if ((strstr(shellpath, "bash") != 0) ||
        (strstr(shellpath, "zsh") != 0)) {
        fl_bash = TRUE;
    } else {
        fl_bash = FALSE;
    }
  }

  /* If we didn't ask for the bourne shell, look up the user's local
     shell and assume they have the same shell on the remote hoted;
     fill in values for fl_csh and fl_bash */

  else {
    p = getpwuid(getuid());
    if (p == NULL)
      return LAMERROR;
    if (lam_ssi_boot_verbose >= 1) {
      lam_debug(lam_ssi_boot_did, 
		"rsh: -b used, assuming same shell on remote nodes");
      lam_debug(lam_ssi_boot_did, "rsh: got local shell %s", 
		p->pw_shell);
    }

    fl_csh = (strstr(p->pw_shell, "csh") != 0) ? TRUE : FALSE;
    if ((strstr(p->pw_shell, "bash") != 0) ||
        (strstr(p->pw_shell, "zsh") != 0)) {
        fl_bash = TRUE;
    } else {
        fl_bash = FALSE;
    }
  }

  /* Remotely execute the command */

  cmdv = sfh_argv_dup(lam_ssi_boot_rsh_agent);
  cmdc = sfh_argv_count(lam_ssi_boot_rsh_agent);
  sfh_argv_add(&cmdc, &cmdv, node->lnd_hname);
  if (!fl_no_n)
    sfh_argv_add(&cmdc, &cmdv, "-n");
  if (node->lnd_uname != NULL) {
    sfh_argv_add(&cmdc, &cmdv, "-l");
    sfh_argv_add(&cmdc, &cmdv, node->lnd_uname);
  }

  /* If the user's shell is not based on "csh" or "bash", force the
     interpretation of the user's .profile script in order to
     initialize the paths. This works for "sh" and "ksh".  */

  if (!(fl_csh || fl_bash))
    sfh_argv_add(&cmdc, &cmdv, "( ! [ -e ./.profile ] || . ./.profile;");
  for (i = 0; argv[i]; ++i)
    sfh_argv_add(&cmdc, &cmdv, argv[i]);
  if (!(fl_csh || fl_bash))
    sfh_argv_add(&cmdc, &cmdv, ")");

  /* Build the sample command to pass to the error routine.  Have to
     do this ahead of time, because ioexecvp frees the argv array. */

  printable = local_argv_glue(cmdv, ' ');
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did,
	      "rsh: attempting to execute: %s", printable);

  cmdv0 = strdup(cmdv[0]);
  i = lam_ssi_boot_base_ioexecvp(cmdv, 1, (char *) 0, 0, stderr_is_err);

  /* Do we need to print an error message? */

  if (i) {

    if (errno == EFAULT)
      show_help("boot", "remote-stderr", remote_host, cmdv0,
		argv[0], printable, NULL);
    else
      show_help("boot", "remote-boot-fail", remote_host, cmdv0,
		argv[0], printable, NULL);

    free(printable);
    free(cmdv0);
    return (LAMERROR);
  }

  free(printable);
  free(cmdv0);
  return 0;
}


/*
 * Local argv_glue function (mainly stolen from all_argv.c) to paste
 * an argv vector into a single string.  Slightly different from the
 * stolen algorithm: individual tokens that contain spaces will now be
 * delimited with '.
 */
static char *
local_argv_glue(char **argv, int delim)
{
  char **p;
  char *str;
  unint str_len = 0;
  unint i;
  int count, need_quote;
  char delim_str[2];

  /* Find the total string length in argv including delimiters.  The
     last delimiter is replaced by the NULL character.  Add 3 spaces
     for each string to allow for it to be surrounded by quotes. */

  for (count = 0, p = argv; *p; ++p, ++count)
    str_len += strlen(*p) + 3;
  
  /* Allocate the string. */

  if ((str = (char*) malloc((unsigned) str_len)) == 0) 
    return NULL;

  /* Loop filling in the string. */

  str[0] = str[--str_len] = '\0';
  p = argv;
  delim_str[0] = delim;
  delim_str[1] = '\0';

  for (i = 0; i < count; ++i) {
    need_quote = 0;
    if (strchr(argv[i], ' ') != NULL || strchr(argv[i], '\t') != NULL) {
      need_quote = 1;
      strcat(str, "\'");
    }
    strcat(str, argv[i]);
    if (need_quote == 1)
      strcat(str, "\'");

    if (i < (count - 1))
      strcat(str, delim_str);
  }

  /* All done */

  return str;
}
