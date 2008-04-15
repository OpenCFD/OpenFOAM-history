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
 * $Id: ssi_ao.c,v 1.3 2003/07/05 21:55:45 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>

#include <args.h>
#include <typical.h>
#include <lam-ssi.h>
#include <etc_misc.h>
#include <all_opt.h>
#include <sfh.h>


/* 
 * Private variables
 */
static int ssi_kind_argc = 0;
static char **ssi_kind_argv = 0;
static int ssi_arg_argc = 0;
static char **ssi_arg_argv = 0;


/*
 * Add -ssi to the OPT list
 */
int 
lam_ssi_base_ao_setup(OPT *ad)
{
  return ao_setopt(ad, "ssi", 0, 2, 0);
}


/*
 * Look for and handle any -ssi options on the command line
 */
int 
lam_ssi_base_ao_process_args(OPT *ad)
{
  int i, num_insts;
  char *buf = 0;
  int buflen = 0;

  if (!ao_taken(ad, "ssi"))
    return 0;

  num_insts = ao_ninsts(ad, "ssi");
  for (i = 0; i < num_insts; ++i)
    lam_ssi_base_ao_process_arg(ao_param(ad, "ssi", i, 0), 
				ao_param(ad, "ssi", i, 1));

  /* Now put them in the environment */

  if (ssi_kind_argv == NULL)
    return 0;

  /* Loop through all the -ssi args that we've gotten and make env
     vars of the form LAM_MPI_SSI_*=value.  This is a memory leak, but
     that's how putenv works.  :-( */

  for (i = 0; ssi_kind_argv[i] != NULL; ++i) {
    buflen = strlen(ssi_kind_argv[i]) + strlen(ssi_arg_argv[i]) + 32;
    buf = malloc(buflen);
    if (buf == NULL)
      return LAMERROR;

    snprintf(buf, buflen, "LAM_MPI_SSI_%s=%s", ssi_kind_argv[i], 
	     ssi_arg_argv[i]);
    putenv(buf);
  }

  return 0;
}


/*
 * Process a single SSI argument.  Done as a separate function so that
 * top-level applications can directly inoke this to effect SSI
 * command line arguments.  
 */
int 
lam_ssi_base_ao_process_arg(char *kind, char *arg)
{
  int i, found;

  /* Look to see if we've already got an -ssi argument for the same
     kind.  Check against the list of SSI kind's that we've already
     saved arguments for. */

  found = 0;
  for (i = 0; ssi_kind_argv != NULL && ssi_kind_argv[i] != NULL; ++i)
    if (strcmp(kind, ssi_kind_argv[i]) == 0) {
      found = 1;
      break;
    }

  if (found) {
    fprintf(stderr, "mpirun: Warning: duplicate -ssi arguments found\n");
    fprintf(stderr, "mpirun: Ignoring \"-ssi %s %s\"\n", kind, arg);
    return 0;
  }
  
  /* If we didn't already have an argument for the same kind, save
     this one away */
  
  sfh_argv_add(&ssi_kind_argc, &ssi_kind_argv, kind);
  sfh_argv_add(&ssi_arg_argc, &ssi_arg_argv, arg);

  return 0;
}
