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
 * $Id: ssi_coll_open.c,v 1.23 2004/03/09 16:49:57 adharurk Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <all_list.h>
#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <typical.h>


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * module's public lam_ssi_coll_t struct.
 */

#include "lam-ssi-coll-modules.h"


/*
 * Global variables
 */
int lam_ssi_coll_verbose = -1;
int lam_ssi_coll_did = -1;
int lam_ssi_coll_base_crossover = 4;
int lam_ssi_coll_base_associative = 0;
int lam_ssi_coll_base_reduce_crossover = 512;
int lam_ssi_coll_base_shmem_message_pool_size = 512 * 256;
int lam_ssi_coll_base_shmem_num_segments = 8;
LIST *lam_ssi_coll_base_opened = NULL;
const lam_ssi_t **lam_ssi_coll_modules = NULL;

int lam_ssi_coll_base_open_lam_basic_for_future = 0;

/*
 * Local variables
 */
static int param_index = -1;
static lam_debug_stream_info_t lds = {
  0, -1, -1, NULL, "coll:", -1, -1, -1, -1, "ssi-coll.txt"
};


/*
 * Local functions
 */
static int parse_names(char *names, LIST **names_list);
static int module_compare(const void *a, const void *b);


int
lam_ssi_coll_base_open(OPT *aod)
{
  char *e;
  const lam_ssi_t *ls;
  int i, found;
  LIST *names_list = NULL;

  /* Register SSI parameter */

  param_index = lam_ssi_base_param_register_string("coll", "base", "verbose",
                                                   "coll_verbose", NULL);

  /* Setup verbosity for this kind */

  lam_ssi_base_set_verbose(param_index, &lds,
			   &lam_ssi_coll_verbose, &lam_ssi_coll_did);
  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "open: Opening");

  /* Look for environment variable overrides */

  if ((e = getenv("LAM_MPI_SSI_coll")) != NULL && strlen(e) > 0) {
    names_list = al_init(LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN, strcmp);
    if (names_list == NULL) {
      show_help(NULL, "system-call-failed", "malloc", NULL);
      return LAMERROR;
    }
    if (parse_names(e, &names_list) != 0)
      return LAMERROR;
  }
  if ((e = getenv("LAM_MPI_SSI_coll_base_crossover")) != NULL)
    lam_ssi_coll_base_crossover = atoi(e);
  if ((e = getenv("LAM_MPI_SSI_coll_base_associative")) != NULL)
    lam_ssi_coll_base_associative = atoi(e);
  if ((e = getenv("LAM_MPI_SSI_coll_base_reduce_crossover")) != NULL)
    lam_ssi_coll_base_reduce_crossover = atoi(e);
  /* Make sure that the message pool size is atleast 1024 bytes */
  if ((e = getenv("LAM_MPI_SSI_coll_base_shmem_message_pool_size")) != NULL)
    if (atoi(e) > 1024)
      lam_ssi_coll_base_shmem_message_pool_size = atoi(e);
  if ((e = getenv("LAM_MPI_SSI_coll_base_shmem_num_segments")) != NULL)
    if (atoi(e) > 0 && atoi(e) <= 16)
      lam_ssi_coll_base_shmem_num_segments = atoi(e);
  if (lam_ssi_coll_verbose >= 0) {
    lam_debug(lam_ssi_coll_did, "open:crossover: %d processes",
	      lam_ssi_coll_base_crossover);
    lam_debug(lam_ssi_coll_did, "open:associative: %d",
	      lam_ssi_coll_base_associative);
  }

  /* Find all available modules */

  lam_ssi_base_module_find(NULL, "coll", 
                           (lam_ssi_t **) lam_ssi_coll_static_modules, 
                           (lam_ssi_t ***) &lam_ssi_coll_modules);

  /* Initialize the list */

  lam_ssi_coll_base_opened = al_init(sizeof(lam_ssi_t*), module_compare);
  if (lam_ssi_coll_base_opened == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* Call the open function in every collective module and see if they
     want to run.  We don't call the module query functions now --
     those happen on a per-communicator basis. */

  for (found = i = 0; lam_ssi_coll_modules[i] != NULL; ++i) {
    ls = lam_ssi_coll_modules[i];

    /* Only look at this module if its name is on the user-specified
       list, or if the user-specified list is empty */

    if (names_list != NULL &&
        al_find(names_list, (char*) ls->ssi_module_name) == NULL) {
      if (strcmp((char*)ls->ssi_module_name, "lam_basic") == 0) {
	/* Going to store this module so that in case lam_basic support
	   is required for a communicator, we can grab the functions */
	lam_ssi_coll_base_open_lam_basic_for_future = 1;
      } else {
	if (lam_ssi_coll_verbose >= 10)
	  lam_debug(lam_ssi_coll_did, "open: skipping non-selected module %s",
		    ls->ssi_module_name);
	continue;
      }
    }

    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, "open: opening coll module %s",
                ls->ssi_module_name);
    if (ls->ssi_open_module == NULL ||
        (ls->ssi_open_module != NULL && ls->ssi_open_module(aod) == 1)) {

      /* If the open function returns happiness, save this module for
         later querying. */
      
      if (lam_ssi_coll_verbose > 10)
        lam_debug(lam_ssi_coll_did, "open: opened coll module %s", 
                  ls->ssi_module_name);
      al_insert(lam_ssi_coll_base_opened, &lam_ssi_coll_modules[i]);

      found = 1;
    } else {
      if (lam_ssi_coll_verbose > 10)
        lam_debug(lam_ssi_coll_did, "open: coll module %s did not open",
                  ls->ssi_module_name);
    }
  }

  /* If we have no collective modules available, it's an error.
     Thanks for playing! */

  if (found == 0) {
    al_free(lam_ssi_coll_base_opened);
    lam_ssi_coll_base_opened = NULL;

    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, "open: no coll modules opened!");
    show_help("ssi-coll", "none-available", NULL);
    return LAMERROR;
  }

  /* Free the list */

  if (names_list != NULL)
    al_free(names_list);

  /* All done */

  return 0;
}


static int
parse_names(char *names, LIST **names_list)
{
  char *comma, *start;
  LIST *nl = *names_list;
  char name[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN];

  /* No env names */

  if (nl == NULL)
    return 0;

  /* Loop over all names */
  /* (yes, this could be more clever, but it's nice and obvious this
     way!) */

  start = names;
  comma = strchr(start, ',');
  while (comma != NULL) {
    *comma = '\0';
    lam_strncpy(name, start, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    al_insert(nl, name);

    start = comma + 1;
    comma = strchr(start, ',');
  }

  /* The last name */

  lam_strncpy(name, start, sizeof(name) - 1);
  name[sizeof(name) - 1] = '\0';
  al_insert(nl, name);

  /* All done */

  return 0;
}


/*
 * Compare two elements in the LIST
 */
static int 
module_compare(const void *a, const void *b)
{
  const lam_ssi_t **aa = (const lam_ssi_t **) a;
  const lam_ssi_t **bb = (const lam_ssi_t **) b;

  return (*aa - *bb);
}
