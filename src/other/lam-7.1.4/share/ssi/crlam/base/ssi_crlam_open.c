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
 * $Id: ssi_crlam_open.c,v 1.16 2004/01/02 00:18:05 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <typical.h>
#include <etc_misc.h>

#include <lam-ssi.h>
#include <lam-ssi-cr.h>
#include <lamdebug.h>


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * module's public lam_ssi_crlam_t struct.
 */

#include "lam-ssi-crlam-modules.h"


/*
 * Global variables
 */
int lam_ssi_cr_verbose = -1;
int lam_ssi_cr_did = -1;
lam_ssi_crlam_t lam_ssi_crlam_base_module;
lam_ssi_crlam_actions_t lam_ssi_crlam;
const lam_ssi_t **lam_ssi_crlam_modules = NULL;


/*
 * Local variables
 */
#if LAM_HAVE_THREADS
static int param_index = -1;
static lam_debug_stream_info_t lds = {
  0, -1, -1, NULL, "crlam:", -1, -1, -1, -1, "ssi-crlam.txt"
};
#endif


/*
 * Local functions
 */
#if LAM_HAVE_THREADS
static int check_specific_module(OPT *, char *);
#endif


int
lam_ssi_crlam_base_open(OPT *aod, char *name)
{
#if LAM_HAVE_THREADS
  /* The CR modules must have thread support in order to run */

  /* Register SSI parameter */

  param_index = lam_ssi_base_param_register_string("cr", "base", "verbose",
                                                   "cr_verbose", NULL);

  /* Setup verbosity for this kind */

  lam_ssi_base_set_verbose(param_index, &lds,
			   &lam_ssi_cr_verbose, &lam_ssi_cr_did);
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, " Opening");

  /* Find all available modules */

  lam_ssi_base_module_find(NULL, "crlam", 
                           (lam_ssi_t **) lam_ssi_crlam_static_modules, 
                           (lam_ssi_t ***) &lam_ssi_crlam_modules);

  /* See if the one we want is available */

  if (check_specific_module(aod, name) < 0)
    return LAMERROR;

  if (lam_ssi_cr_verbose > 0)
    lam_debug(lam_ssi_cr_did, " Selected crlam module \"%s\"",
              lam_ssi_crlam_base_module.lscrl_meta_info.ssi_module_name);
#else
  /* If we do not have thread support, then simply set the actions to
     NULL */

  char *env;
  
  lam_ssi_crlam.lscrla_checkpoint = NULL;
  lam_ssi_crlam.lscrla_continue = NULL;
  lam_ssi_crlam.lscrla_init = NULL;
  lam_ssi_crlam.lscrla_finalize = NULL;
  lam_ssi_crlam.lscrla_restart = NULL;

  /* If the user requested a specific module, it's an error (because
     we cannot possibly use that module, since there is no thread
     support). */

  if (((env = getenv("LAM_MPI_SSI_cr")) != NULL) 
      && (!strcmp(env, "none"))) {
    show_help("cr-ssi", "specific-module-no-threads", env, NULL);
    return LAMERROR;
  }
#endif

  /* All done */

  return 0;
}


#if LAM_HAVE_THREADS
static int 
check_specific_module(OPT *aod, char *name)
{
  int i;
  int priority;
  const lam_ssi_t *ls;
  const lam_ssi_crlam_t *crlam;
  const lam_ssi_crlam_actions_t *lbcrlam = NULL;

  if (!strcmp(name, "none")) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "module \"none\" explicitly requested. "
                "Disabling cr support");

    lam_ssi_crlam.lscrla_checkpoint = NULL;
    lam_ssi_crlam.lscrla_continue = NULL;
    lam_ssi_crlam.lscrla_init = NULL;
    lam_ssi_crlam.lscrla_finalize = NULL;
    lam_ssi_crlam.lscrla_restart = NULL;
    return 0;
  }

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, " looking for module named %s", name);

  for (i = 0; lam_ssi_crlam_modules[i] != NULL; ++i) {
    ls = lam_ssi_crlam_modules[i];
    if (0 == strcmp(name, ls->ssi_module_name)) {
      if (lam_ssi_cr_verbose > 10)
        lam_debug(lam_ssi_cr_did, " opening module %s", ls->ssi_module_name);
      if (ls->ssi_open_module == NULL ||
          (ls->ssi_open_module != NULL &&
           1 == ls->ssi_open_module(aod))) {

        /* If it's available, run its query function and see if it
           wants to run.  And if so, ignore the priority but get its
           function list. */

        if (lam_ssi_cr_verbose > 10)
          lam_debug(lam_ssi_cr_did, " query module %s",
                    ls->ssi_module_name);
        crlam = (const lam_ssi_crlam_t *) ls;
        lbcrlam = crlam->lscrl_query(&priority);

        /* If we got NULL back, then this module does not want to
           run. Give up in despair. */

        if (NULL == lbcrlam) {
          if (ls->ssi_close_module != NULL)
            ls->ssi_close_module();

          show_help("cr-ssi", "selected-module-unavailable", name);
          lam_ssi_base_module_registry_unuse((lam_ssi_t *) ls);
          return LAMERROR;
        }

        /* Otherwise, we're found the module and it wants to
           run. We're done */
        break;
      }

      /* Otherwise this module's init function didn't want to
         run. Abort. */
      else {
        show_help("cr-ssi", "selected-module-unavailable", name);
        return LAMERROR;
      }
    }
  }

  /* Did we find one ? */

  if (NULL == lam_ssi_crlam_modules[i]) {
    show_help("cr-ssi", "module-not-found", name);
    return LAMERROR;
  }

  /* Found module. Load up the global variables for the crlam module. */

  lam_ssi_crlam = *lbcrlam;
  lam_ssi_crlam_base_module = *((lam_ssi_crlam_t *) lam_ssi_crlam_modules[i]);
  
  return 0;
}
#endif
