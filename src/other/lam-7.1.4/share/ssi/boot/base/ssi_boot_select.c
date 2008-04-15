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
 * $Id: ssi_boot_select.c,v 1.6 2003/12/04 19:54:45 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include <typical.h>
#include <net.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>
#include <all_opt.h>
#include <all_list.h>


/*
 * Scan down the list of successfully opened modules and query each of
 * them (the opened list will be one or more modules.  If the user
 * requested a specific module, it will be the only module in the
 * opened list).  
 *
 * Find the module with the highest priority returned from init, and
 * set that in the global variables.  Call finalize/close on all the
 * rest.  Destroy the opened list because it won't be used anymore.
 */
int
lam_ssi_boot_select(lam_ssi_boot_location_t where)
{
  void *p;
  const lam_ssi_t *ls;
  const lam_ssi_boot_t *module, *best_module;
  int cur_priority, best_priority;
  const lam_ssi_boot_actions_t *cur_lba, *best_lba = NULL;

  /* The list of modules that we should check has already been
     established in lam_ssi_boot_base_opened. */

  for (best_module = NULL, best_priority = -1, 
         p = al_top(lam_ssi_boot_base_opened); 
       p != NULL; p = al_next(lam_ssi_boot_base_opened, p)) {
    module = *((lam_ssi_boot_t **) p);
    ls = &(module->lsb_meta_info);

    /* This module has already been successfully opened.  So now
       initialize/query it. */
  
    if (lam_ssi_boot_verbose > 0)
      lam_debug(lam_ssi_boot_did, "select: initializing boot module %s", 
                ls->ssi_module_name);
    cur_lba = module->lsb_init(where, &cur_priority);
    if (cur_lba != NULL) {
      if (lam_ssi_boot_verbose >= 10)
        lam_debug(lam_ssi_boot_did, 
                  "select: boot module available: %s, priority: %d", 
                  ls->ssi_module_name, cur_priority);
	
      cur_priority = LAM_min(cur_priority, 100);
      if (cur_priority > best_priority) {
        best_priority = cur_priority;
        best_lba = cur_lba;
        best_module = module;
      }
    } else {
      if (lam_ssi_boot_verbose >= 10)
        lam_debug(lam_ssi_boot_did, "select: boot module not available: %s",
                  ls->ssi_module_name);
    }
  }

  /* If there were no modules available, it's an error. */

  if (best_module == NULL) {
    if (lam_ssi_boot_verbose > 1)
      lam_debug(lam_ssi_boot_did, "select: no boot moduless available!");
    show_help("ssi-boot", "none-available", NULL);
    return LAMERROR;
  }

  /* Otherwise, load up the global variables for the boot module */

  lam_ssi_boot = *best_lba;
  lam_ssi_boot_base_module = *best_module;
    
  /* Since we have a winner, call finalize and close on all the rest
     to tell them that they will not be run.  */

  for (p = al_top(lam_ssi_boot_base_opened); p != NULL;
       p = al_next(lam_ssi_boot_base_opened, p)) {
    module = *((lam_ssi_boot_t **) p);
    if (module == best_module)
      continue;
    ls = &(module->lsb_meta_info);

    /* Run the finalize method */

    if (lam_ssi_boot_verbose > 0)
      lam_debug(lam_ssi_boot_did, "select: finalizing boot module %s", 
                ls->ssi_module_name);
    if (module->lsb_finalize != NULL)
      module->lsb_finalize();

    /* Run the module_close method */

    if (lam_ssi_boot_verbose > 0)
      lam_debug(lam_ssi_boot_did, "select: closing boot module %s", 
                ls->ssi_module_name);
    if (ls->ssi_close_module != NULL)
      ls->ssi_close_module();

    /* Close the module */

    lam_ssi_base_module_registry_unuse((lam_ssi_t*) ls);
  }

  /* Free the opened list */

  al_free(lam_ssi_boot_base_opened);
  lam_ssi_boot_base_opened = NULL;

  if (lam_ssi_boot_verbose > 0)
    lam_debug(lam_ssi_boot_did, "select: selected boot module %s", 
	      lam_ssi_boot_base_module.lsb_meta_info.ssi_module_name);
  
  return 0;
}
