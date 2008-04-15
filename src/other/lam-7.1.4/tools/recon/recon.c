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
 * $Id: recon.c,v 6.37 2003/11/24 19:33:07 jsquyres Exp $
 *
 *	Function:	- fully-connected LAM booting tool
 *			- boots Trollius on a network of UNIX hosts
 *			- hosts specified using host file syntax
 *			- uses hboot
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <args.h>
#include <lamnet.h>
#include <etc_misc.h>
#include <lam-ssi.h>
#include <lam-ssi-boot.h>


/*
 * local variables
 */
static OPT *ad = NULL;


int
main(int argc, char *argv[])
{
  int ret;
  int num_started;
  int iorigin;
  int nlamnet;			/* lamnet size */
  struct lamnode *lamnet;	/* network description array */
  int unusedc;
  char **unusedv;

  /* It's ok to run recon as root, but with a short warning */
  
#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0)
    show_help(NULL, "root-warning", NULL);
#endif
  
  /* Parse the command line */

  ad = ao_init();
  if (ad == 0) {
    ret = errno;
    perror("recon (ao_init)");
    exit(ret);
  }
  ao_setflags(ad, AOPRESERVE_ARGV);
  ao_setopt1(ad, "abdhv", 0, 0, 0);
  ao_setopt(ad, "nn", 0, 0, 0);
  ao_setopt(ad, "np", 0, 0, 0);
  ao_setopt(ad, "c", 0, 1, 0);

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  if (ao_parse(ad, &argc, argv)) {
    show_help("recon", "usage", NULL);
    lam_ssi_base_close();
    exit(EUSAGE);
  }

  /* Check for -d and -v *before* we intialize the boot SSI */

  if (ao_taken(ad, "v"))
    putenv("LAM_MPI_SSI_boot_verbose=level:0,stderr");
  if (ao_taken(ad, "d"))
    putenv("LAM_MPI_SSI_boot_verbose=level:1000,stderr");

  /* Compatability arguments, from pre-SSI days (and it's just a
     heckuva lot shorter to type!) */

  if (ao_taken(ad, "nn"))
    putenv("LAM_MPI_SSI_boot_base_no_n=1");
  if (ao_taken(ad, "np"))
    putenv("LAM_MPI_SSI_boot_base_no_profile=1");

  /* Open the boot SSI */

  if (lam_ssi_boot_open(ad) != 0 ||
      lam_ssi_boot_select(LAM_SSI_BOOT_LOCATION_ROOT) != 0) {
    show_help("ssi-boot", "unable-to-init", NULL);
    lam_ssi_base_close();
    exit(1);
  }

  /* Check rest of command line args */

  ao_unused(ad, &unusedc, &unusedv);
  if ((errno = (unusedc <= 2) ? 0 : EUSAGE)) {
    show_help("recon", "usage", NULL);
    lam_ssi_base_close();
    exit(errno);
  }

  if (ao_taken(ad, "h")) {
    show_help("recon", "usage", NULL);
    lam_ssi_base_close();
    exit(0);
  }

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

  /* Now actually have the boot SSI boot the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_start_rte_procs(lamnet, nlamnet, 
				       LAM_SSI_BOOT_PROC_RECON,
				       &num_started) != 0 ||
      num_started != nlamnet) {
    ret = 1;
    show_help("recon", "unhappiness", NULL);
  } else {
    ret = 0;
    show_help("recon", "happiness", NULL);
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
