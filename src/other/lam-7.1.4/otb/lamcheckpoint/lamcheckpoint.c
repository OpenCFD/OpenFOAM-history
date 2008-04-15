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
 * $Id: lamcheckpoint.c,v 1.3 2004/03/09 16:56:10 pcharapa Exp $
 * 
 *	Function:	- checkpoints  an MPI job 
 */	


#include <lam_config.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_opt.h>
#include <args.h>
#include <lamdebug.h>
#include <etc_misc.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>

OPT *ad;
int param_crlam;

static int lam_strtonum(const char *, pid_t *);

/*
 * main
 */
int
main(int argc, char *argv[]) 
{
  char *cr_module;
  char *pid_arg;
  pid_t pid;
  int errno_save; 
  
  ad = ao_init();
  if (ad == 0) {
    perror("mpirun (ao_init)");
    exit(errno);
  }
  
  ao_setopt(ad, "ssi", 0, 2, 0);
  ao_setopt(ad, "pid", 0, 1, 0);
  
  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);
  
  if (ao_parse(ad, &argc, argv)) {
    show_help("lamcheckpoint", "usage", NULL);
    lam_ssi_base_close();
    ao_free(ad);
    exit(EUSAGE);
  }

  param_crlam = lam_ssi_base_param_register_string("cr", 
      NULL, NULL, NULL, NULL);
  
  if (!ao_taken(ad, "pid")) {
    show_help("lamcheckpoint", "usage", NULL);
    lam_ssi_base_close();
    ao_free(ad);
    exit(EUSAGE);
  }

  pid_arg = ao_param(ad, "pid", 0, 0);
 
  if (lam_strtonum(pid_arg, &pid) != 0) {
    show_help("lamcheckpoint", "usage", NULL);
    lam_ssi_base_close();
    ao_free(ad);
    exit(EUSAGE);
  } 
  
  lam_ssi_base_ao_process_args(ad);
  
  if (!ao_taken(ad, "ssi")) {
    show_help("lamcheckpoint", "usage", NULL);
    lam_ssi_base_close();
    ao_free(ad);
    exit(1);
  }
  
   /* Open the CRLAM SSI kind for checkpoint/restart support */
  
  cr_module = lam_ssi_base_param_lookup_string(param_crlam);
  
  if (lam_ssi_crlam_base_open(ad, cr_module) != 0) {
    errno_save = errno;
    free(cr_module);
    lam_ssi_base_close();
    ao_free(ad);
    kexit(errno_save);
  }
  
  if (lam_ssi_crlam.lscrla_lamcheckpoint != NULL) {
    lam_ssi_crlam.lscrla_lamcheckpoint(pid); 
  } else {
    free(cr_module);
    lam_ssi_base_close();
    ao_free(ad);
    exit(1);
  }
  
  ao_free(ad);
  free(cr_module);
  lam_ssi_base_close();
  return 0;
}


/*
 *	lam_strtonum
 *
 *	Function:	- convert string to number if possible
 *			- handles decimal/octal/hexadecimal
 *			- uses strtol()
 *	Accepts:	- string
 *			- ptr number (returned value)
 *	Returns:	- 0 or LAMERROR
 */
static int
lam_strtonum(const char *str, pid_t *pnum)
{
	char		*endstr;		/* end of parsed string */

	*pnum = (int) strtol(str, &endstr, 0);

	if (*endstr) {
		errno = EBADASCIINUMB;
		return(LAMERROR);
	}

	return(0);
}
