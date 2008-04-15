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
 * $Id: lamboot.c,v 6.72 2004/03/06 19:59:29 jsquyres Exp $
 *
 *	Function:	- fully-connected LAM booting tool
 *			- boots Trollius on a network of UNIX hosts
 *			- hosts specified using host file syntax
 *			- uses hboot
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <all_opt.h>
#include <args.h>
#include <kio.h>
#include <mpisys.h>
#include <lamnet.h>
#include <laminternal.h>
#include <etc_misc.h>
#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <sfh.h>


/*
 * local variables
 */
static int num_started = -1;
static int fl_verbose = 0;
static int fl_debug = 0;
static int main_argc = 0;
static char **main_argv = NULL;
static OPT *ad = NULL;


/* 
 * local functions
 */
static void bail(int);
static void wipe(void);


int
main(int argc, char *argv[])
{
  int i, ret = 0;
  int iorigin;
  int nlamnet;
  struct lamnode *lamnet;
  int unusedc;
  char **unusedv;
  char *prefix = NULL;
  char *suffix = NULL;
  char *lamprefix = NULL;
  char **cmdv = NULL;
  int cmdc;
  int cmdv0_len;

  main_argc = argc;
  main_argv = sfh_argv_dup(argv);

  /* Parse the command line */

  ad = ao_init();
  if (ad == 0) {
    ret = errno;
    perror("lamboot (ao_init)");
    exit(ret);
  }
  ao_setflags(ad, AOPRESERVE_ARGV);
  ao_setopt1(ad, "bdhsvxHVl", 0, 0, 0);
  ao_setopt(ad, "nn", 0, 0, 0);
  ao_setopt(ad, "np", 0, 0, 0);
  ao_setopt(ad, "c", 0, 1, 0);
  ao_setopt(ad, "sessionprefix", 0, 1, 0);
  ao_setopt(ad, "sessionsuffix", 0, 1, 0);
  ao_setopt(ad, "prefix", 0, 1, 0);
  ao_setopt(ad, "withlamprefixpath", 0, 0, 0);

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  if (ao_parse(ad, &argc, argv)) {
    show_help("lamboot", "usage", NULL);
    lam_ssi_base_close();
    exit(EUSAGE);
  }

  /* Check for -d and -v *before* we intialize the boot SSI */

  fl_verbose = ao_taken(ad, "v");
  fl_debug = ao_taken(ad, "d");
  
  if (fl_verbose)
    lam_ssi_base_param_set_string("boot_verbose", "level:0,stderr");
  if (fl_debug)
    lam_ssi_base_param_set_string("boot_verbose", "level:1000,stderr");
  if (ao_taken(ad, "b"))
    lam_ssi_base_param_set_string("boot_rsh_fast", "1");

  /* Check for -withlamprefixpath. If not supplied and prefix supplied
     then we need to call the prefix lamboot */

  if (!ao_taken(ad, "withlamprefixpath") && ao_taken(ad, "prefix")) {

    /* We have got the prefix, so launch the prefix lamboot */
    lamprefix = ao_param(ad, "prefix", 0, 0);
      
    /* Build the command to be passed to lamboot -- add the option 
       -with-lam-prefix-path to make lamboot know that it has been called 
       and has been supplied a prefix for locating other binaries */

    cmdv = sfh_argv_dup(argv);
    cmdc = argc;

    /* Replace cmdv[0] with the new lamboot path */
    free(cmdv[0]);

    cmdv0_len = strlen("/bin/lamboot") + strlen(lamprefix) + 2;
    cmdv[0] = (char *) malloc (cmdv0_len);

    if (cmdv[0] == NULL) {
      show_help(NULL, "system-call-fail", "malloc", NULL);
      return LAMERROR;
    }
    snprintf(cmdv[0], cmdv0_len, "%s%s", lamprefix, "/bin/lamboot");

    /* Check to see if path is valid and you got permissions */
    if (access(cmdv[0], X_OK) == -1) {
      show_help("boot","invalid-path-or-no-permission", lamprefix, "lamboot");
      return LAMERROR;
    }
    sfh_argv_add(&cmdc, &cmdv, "-withlamprefixpath");

    /* Fork the prefix lamboot */
    _lam_few(cmdv); 
    return (0);
  }
  
  /* Compatability arguments, from pre-SSI days (and it's just a
     heckuva lot shorter to type!) */

  if (ao_taken(ad, "nn"))
    lam_ssi_base_param_set_int("rsh_base_no_n", 1);
  if (ao_taken(ad, "np"))
    lam_ssi_base_param_set_int("rsh_base_no_profile", 1);

  /* Batch system gorp */
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
  lam_tmpdir_init_opt(prefix, suffix, 0);
    
  /* Let SSI parse all base SSI-related arguments.  It will print out
     its own error message if necessary, so exiting upon failure is
     sufficient here. */

  if (lam_ssi_base_ao_process_args(ad) != 0) {
    lam_ssi_base_close();
    exit(errno);
  }

  /* Just a version output? */

  if (ao_taken(ad, "V")) {
    lam_show_version(1);

    /* For backwards comparability only -- use laminfo for more detail */

    printf("\tSSI rpi:\t%s\n", RPI_MODULES);
    ao_free(ad);
    if (main_argv != 0)
      sfh_argv_free(main_argv);
    lam_ssi_base_close();
    exit(0);
  }

  /* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0) {
    show_help(NULL, "deny-root", NULL);
    lam_ssi_base_close();
    exit(EACCES);
  }
#endif

  ao_unused(ad, &unusedc, &unusedv);
  if ((errno = (unusedc <= 2) ? 0 : EUSAGE)) {
    show_help("lamboot", "usage", NULL);
    lam_ssi_base_close();
    exit(errno);
  }

  if (ao_taken(ad, "h")) {
    show_help("lamboot", "usage", NULL);
    ao_free(ad);
    if (main_argv != 0)
      sfh_argv_free(main_argv);
    lam_ssi_base_close();
    exit(0);
  }

  /* Open the boot SSI */

  if (lam_ssi_boot_open(ad) != 0 ||
      lam_ssi_boot_select(LAM_SSI_BOOT_LOCATION_ROOT) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  if (!ao_taken(ad, "H"))
    lam_show_version(0);

  /* Let the boot SSI parse the command line.  Relevant errors will be
     displayed before returning from this function. */

  if (lam_ssi_boot.lsba_parse_options(ad, 1) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Let the boot SSI generate a list of nodes.  Relevant errors will
     be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_allocate_nodes(&lamnet, &nlamnet, &iorigin) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Let the boot SSI do an error check on the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_verify_nodes(lamnet, nlamnet) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Setup the _kio struct */

  _kio.ki_origin = iorigin;

  /* Let the boot SSI prepare to boot.  Relevant errors will be
     displayed before returning from this function. */

  if (lam_ssi_boot.lsba_prepare_boot() != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Clean up on interrupt */

  if (signal(SIGINT, bail) == SIG_ERR) {
    show_help(NULL, "system-call-fail", "signal", NULL);
    lam_ssi_base_close();
    exit(1);
  }

  /* Now actually have the boot SSI boot the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_start_rte_procs(lamnet, nlamnet, 
                                        LAM_SSI_BOOT_PROC_LAMD,
                                        &num_started) != 0 ||
      num_started != nlamnet)
    bail(-1);

  /* Now that we've started everyone correctly, check to see if there
     any scheduleable nodes */

  for (i = 0; i < nlamnet; ++i)
    if ((lamnet[i].lnd_type & NT_WASTE) == 0)
      break;
  if (i >= nlamnet)
    show_help("boot", "no-schedulable-nodes", NULL);

  /* Free up resources */

  if (lam_ssi_boot.lsba_deallocate_nodes(&lamnet, &nlamnet) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* All done with the boot SSI module (don't care about errors at
     this point) */

  if (lam_ssi_boot_close() != 0)
    ret = 1;

  /* All done */

  if (main_argv != 0)
    sfh_argv_free(main_argv);

  ao_free(ad);
  lam_ssi_base_close();
  return ret;
}


/*
 *	bail
 *
 *	Function:	- cleans up and bails out
 *	Returns:	- does not return, exits with error code
 */
static void
bail(int sig)
{
  int err_save;			/* saved error code */

  err_save = errno;
  wipe();
  if (fl_verbose || fl_debug) 
    fprintf(stderr, "lamboot did NOT complete successfully\n");
  lam_ssi_base_close();
  if (err_save != 0)
    exit(err_save);
  else
    exit(sig); 
  /* Since we want to return with non zero exit status, exit status is
     set to signal number if its 0 */
}


/*
 *	wipe
 *
 *	Function:	- executes the wipe tool
 *			- kills all host Trollius sessions
 */
static void
wipe(void)
{
  int i;
  int cmdn;
  char **cmdv;

  if (num_started <= 0)
    return;

  show_help("boot", "about-to-lamwipe", "lamboot", NULL);
  cmdn = 0;
  cmdv = 0;
  sfh_argv_add(&cmdn, &cmdv, DEFTWIPE);
  for (i = 1; i < main_argc; ++i)
    sfh_argv_add(&cmdn, &cmdv, main_argv[i]);
  sfh_argv_add(&cmdn, &cmdv, "-H");

  /* This is a last ditch effort, so if _lam_few fails, there's really
     nothing we can do about it.  So just ignore the return value from
     _lam_few(). */

  _lam_few(cmdv);
}
