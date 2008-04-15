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
 * $Id: wipe.c,v 6.50 2003/11/24 19:33:07 jsquyres Exp $
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

#include <args.h>
#include <lamnet.h>
#include <etc_misc.h>
#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <sfh.h>
#include <laminternal.h>


/*
 * local variables
 */
static OPT *ad = NULL;


int
main(int argc, char *argv[])
{
  int ret = 0;
  int num_started;
  int iorigin;
  int nlamnet;			/* lamnet size */
  struct lamnode *lamnet;	/* network description array */
  int unusedc;
  char **unusedv;
  char *lamprefix = NULL;
  char **cmdv = NULL;
  int cmdc;
  int cmdv0_len;
  char *prefix = NULL;
  char *suffix = NULL;

  /* Parse the command line */

  ad = ao_init();
  if (ad == 0) {
    ret = errno;
    perror("lamwipe (ao_init)");
    exit(ret);
  }
  ao_setflags(ad, AOPRESERVE_ARGV);
  ao_setopt1(ad, "bdhvHV", 0, 0, 0);
  ao_setopt(ad, "nn", 0, 0, 0);
  ao_setopt(ad, "np", 0, 0, 0);
  ao_setopt(ad, "w", 0, 1, AOINT);
  ao_setopt(ad, "sessionprefix", 0, 1, 0);
  ao_setopt(ad, "sessionsuffix", 0, 1, 0);
  ao_setopt(ad, "prefix", 0, 1, 0);
  ao_setopt(ad, "withlamprefixpath", 0, 0, 0);

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  if (ao_parse(ad, &argc, argv)) {
    show_help("lamwipe", "usage", NULL);
    lam_ssi_base_close();
    exit(EUSAGE);
  }

  /* Check for -d and -v *before* we intialize the boot SSI */

  if (ao_taken(ad, "v"))
    putenv("LAM_MPI_SSI_boot_verbose=level:0,stderr");
  if (ao_taken(ad, "d"))
    putenv("LAM_MPI_SSI_boot_verbose=level:1000,stderr");

  /* Check for -withlamprefixpath. If not supplied and prefix supplied
     then we need to call the prefix wipe */

  if (!ao_taken(ad, "withlamprefixpath") && ao_taken(ad, "prefix")) {

    /* We have got the prefix, so launch the prefix wipe */
    lamprefix = ao_param(ad, "prefix", 0, 0);
      
    /* Build the command to be passed to wipe -- add the option 
       -with-lam-prefix-path to make wipe know that it has been called 
       and has been supplied a prefix for locating other binaries */

    cmdv = sfh_argv_dup(argv);
    cmdc = argc;

    /* Replace cmdv[0] with the new wipe path */
    free(cmdv[0]);

    cmdv0_len = strlen("/bin/wipe") + strlen(lamprefix) + 2;
    cmdv[0] = (char *) malloc (cmdv0_len);

    if (cmdv[0] == NULL) {
      show_help(NULL, "system-call-fail", "malloc", NULL);
      return LAMERROR;
    }
    snprintf(cmdv[0], cmdv0_len, "%s%s", lamprefix, "/bin/wipe");

    /* Check to see if path is valid and you got permissions */
    if (access(cmdv[0], X_OK) == -1) {
      show_help("boot","invalid-path-or-no-permission", lamprefix, "wipe");
      return LAMERROR;
    }
    sfh_argv_add(&cmdc, &cmdv, "-withlamprefixpath");

    /* Fork the prefix wipe */
    _lam_few(cmdv); 
    return (0);
  }

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

  /* Compatability arguments, from pre-SSI days (and it's just a
     heckuva lot shorter to type!) */

  if (ao_taken(ad, "nn"))
    putenv("LAM_MPI_SSI_boot_base_no_n=1");
  if (ao_taken(ad, "np"))
    putenv("LAM_MPI_SSI_boot_base_no_profile=1");

  /* Let SSI parse all base SSI-related arguments.  It will print out
     its own error message if necessary, so exiting upon failure is
     sufficient here. */

  if (lam_ssi_base_ao_process_args(ad) != 0) {
    lam_ssi_base_close();
    exit(errno);
  }

  /* Open the boot SSI */

  if (lam_ssi_boot_open(ad) != 0 ||
      lam_ssi_boot_select(LAM_SSI_BOOT_LOCATION_ROOT) != 0) {
    show_help("ssi-boot", "unable-to-init", NULL);
    lam_ssi_base_close();
    exit(1);
  }

  /* Just a version output? */

  if (ao_taken(ad, "V")) {
    lam_show_version(1);
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
  
  /* Check rest of command line args */

  ao_unused(ad, &unusedc, &unusedv);
  if ((errno = (unusedc <= 2) ? 0 : EUSAGE)) {
    show_help("lamwipe", "usage", NULL);
    lam_ssi_base_close();
    exit(errno);
  }

  if (ao_taken(ad, "h")) {
    show_help("lamwipe", "usage", NULL);
    lam_ssi_base_close();
    exit(0);
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

  /* Let the boot SSI prepare to boot.  Relevant errors will be
     displayed before returning from this function. */

  if (lam_ssi_boot.lsba_prepare_boot() != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Do the whole file, or just part of it? */

  if (ao_taken(ad, "w"))
    ao_intparam(ad, "w", 0, 0, &nlamnet);

  /* Now actually have the boot SSI boot the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_start_rte_procs(lamnet, nlamnet, 
                                        LAM_SSI_BOOT_PROC_WIPE,
                                        &num_started) != 0 ||
      num_started != nlamnet) {
    lam_ssi_base_close();
    exit(1);
  }

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

  ao_free(ad);
  lam_ssi_base_close();
  return ret;
}
