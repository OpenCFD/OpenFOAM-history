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
 * $Id: lamrestart.c,v 1.2 2004/03/09 16:56:12 pcharapa Exp $
 * 
 *	Function:	- restarts  an MPI job 
 */	


#include <lam_config.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <etc_misc.h>
#include <all_opt.h>
#include <args.h>
#include <lamdebug.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>

OPT *ad;
int param_crlam;

/*
 * main
 */
int
main(int argc, char *argv[]) 
{
  int str_len; 
  int errno_save; 
  char *cr_module;
  char *executable;
  char lampath[] = LAM_BINDIR;  
  
  ad = ao_init();
  if (ad == 0) {
    perror("mpirun (ao_init)");
    exit(errno);
  }
  
  ao_setopt(ad, "ssi", 0, 2, 0);
  
  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);
  
  if (ao_parse(ad, &argc, argv)) {
    show_help("lamrestart", "usage", NULL);
    lam_ssi_base_close();
    ao_free(ad);
    exit(EUSAGE);
  }

  /* register the SSI kind parameter if no SSI parameter is found,
   * show the usage method and exit */
 
  param_crlam = lam_ssi_base_param_register_string("cr", 
      NULL, NULL, NULL, NULL);
 
  lam_ssi_base_ao_process_args(ad);
  
  if (!ao_taken(ad, "ssi")) {
    show_help("lamrestart", "usage", NULL); 
    lam_ssi_base_close();
    ao_free(ad);
    exit(1);
  }
  
  /* lookup the name of the CRLAM SSI kind */
  
  cr_module = lam_ssi_base_param_lookup_string(param_crlam);

  if (cr_module == NULL) {
    show_help("lamrestart", "usage", NULL); 
    lam_ssi_base_close();
    ao_free(ad);
    exit(1);
  }
  
  /* Open the CRLAM SSI kind for checkpoint/restart support */
  
  if (lam_ssi_crlam_base_open(ad, cr_module) != 0) {
    errno_save = errno;
    free(cr_module);
    lam_ssi_base_close();
    ao_free(ad);
    kexit(errno_save);
  }
 
  /* call the appropriate CRLAM SSI kinds lamrestart function with the 
   * path to mpirun */

  if (lam_ssi_crlam.lscrla_lamrestart != NULL) {
    str_len = strlen(lampath) + strlen("/mpirun") + 1;
    executable = malloc(str_len + sizeof(char));
    snprintf(executable, str_len, "%s%s", lampath, "/mpirun"); 
    lam_ssi_crlam.lscrla_lamrestart(executable); 
  } else {
    free(cr_module);
    lam_ssi_base_close();
    ao_free(ad);
    exit(1);
  }
  
  /* all done */
  free(cr_module);
  free(executable);
  lam_ssi_base_close();
  ao_free(ad);
  return 0;
}


