/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <typical.h>
#include <mpisys.h>

/* 
 * Public variables
 */
char *lam_ssi_coll_base_override = NULL;

/*
 * Local variables
 */
static lam_ssi_coll_actions_1_1_0_t actions110;
static lam_ssi_coll_actions_1_1_0_t lb_functions = {0};
static lam_ssi_t* lb_coll;
extern int lam_ssi_coll_base_open_lam_basic_for_future;

/*
 * Local functions
 */
static int check_module_name(MPI_Comm comm, char *name);
static int check_all_modules(MPI_Comm comm);
static int check_module(MPI_Comm comm, lam_ssi_t *coll,
                        const lam_ssi_coll_actions_1_1_0_t **actions);
static const lam_ssi_coll_actions_1_1_0_t *
  query_1_0_0(lam_ssi_t *coll, MPI_Comm comm, int *priority);
static const lam_ssi_coll_actions_1_1_0_t *
  query_1_1_0(lam_ssi_t *coll, MPI_Comm comm, int *priority);
static int init_module(lam_ssi_t *module, MPI_Comm comm, 
                       const lam_ssi_coll_actions_1_1_0_t **actions);
static int init_1_0_0(MPI_Comm comm, 
                      const lam_ssi_coll_actions_1_1_0_t **actions);
static int init_1_1_0(MPI_Comm comm, 
                      const lam_ssi_coll_actions_1_1_0_t **actions);
static void convert_actions_1_0_0_to_1_1_0(const lam_ssi_coll_actions_1_0_0_t *actions100, lam_ssi_coll_actions_1_1_0_t *actions110);
static int replace_null_with_lam_basic(lam_ssi_coll_actions_1_1_0_t *sel_module, MPI_Comm comm);

/*
 * This function is called at the initialization time of every
 * communicator.  It is used to select which coll module will be
 * active for a given communicator.
 *
 * Note that this is not thread safe.  It's probably ok to just put a
 * lock around this function, though -- this is only called at the end
 * of a communicator constructor, so making a few threads wait while
 * we construct is probably ok. 
 */
int
lam_ssi_coll_base_init_comm(MPI_Comm comm)
{
  int i;
  int found;
  char name[BUFSIZ];
  char *module_name;

  /* Announce */

  if (lam_ssi_coll_verbose >= 10) {
    if (comm->c_name[0] != '\0')
      snprintf(name, sizeof(name), "%s (cid %d)", comm->c_name, 
	       comm->c_contextid);
    else
      snprintf(name, sizeof(name), "<no name> (cid %d)", comm->c_contextid);
    name[sizeof(name) - 1] = '\0';
    lam_debug(lam_ssi_coll_did, "init_comm: new communicator: %s", name);
  }

  /* WARNING: This will go away someday.  It is *only* here for a
     special case during MPI_Init of IMPI-enabled jobs.  It is not
     thread-safe, and will not be necessary when/if IMPI is
     re-architected.  So don't use it.  Ever. */

  if (lam_ssi_coll_base_override != NULL)
    i = check_module_name(comm, lam_ssi_coll_base_override);

  /* See if a specific module was requested by the magic keyval */

  else {
    MPI_Comm_get_attr(comm, LAM_MPI_SSI_COLL, &module_name, &found);
    if (found == 1)
      i = check_module_name(comm, module_name);
  
    /* Nope -- a specific one was not selected.  Go choose one. */
  
    else
      i = check_all_modules(comm);
  }
  
  /* If we have no collective modules available, it's an error.
     Thanks for playing! */

  if (i == LAMERROR) {
    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, "init_comm: No modules available!");
    show_help("ssi-coll", "none-available", NULL);
    return LAMERROR;
  }

  /* All done */
  
  return 0;
}


/*
 * A specific module was selected on the command line.  If a module by
 * that name is found, call its open (if it exists) and init
 * functions.  If they both return happiness, that module is selected.
 * Otherwise, call its finalize and close functions and return an
 * error (i.e., don't try to find any other available modules).
 */
static int
check_module_name(MPI_Comm comm, char *name)
{
  lam_ssi_module_t *module;
  lam_ssi_t *coll;
  const lam_ssi_coll_actions_1_1_0_t *actions = NULL;

  /* Find the target module by its name */

  for (module = al_top(lam_ssi_coll_base_available); module != NULL;
       module = al_next(lam_ssi_coll_base_available, module)) {
    
    coll = module->lsm_module;
    if (strcmp(coll->ssi_module_name, "lam_basic") == 0) {
      lb_coll = coll;
      if (lam_ssi_coll_base_open_lam_basic_for_future) {
	continue;
      }
    }
    if (strcmp(coll->ssi_module_name, name) == 0) {
      
      /* Found it.  Now check and see if that module wants to run */

      if (check_module(comm, coll, &actions) >= 0) {

	/* Otherwise, we have a winner.  Assign all the function
	   pointers in the comm to that module, and call its init
	   function. */
	
	comm->c_ssi_coll = *actions;
	actions = NULL;
        if (init_module(coll, comm, &actions) != 0)
	  return LAMERROR;
	if (actions != NULL)
	  comm->c_ssi_coll = *actions;

	/* Replace the NULL functions only if the selected module is not
	   lam_basic */
	if (strcmp(coll->ssi_module_name, "lam_basic") != 0)
	  if (replace_null_with_lam_basic(&(comm->c_ssi_coll), comm) 
	      == LAMERROR)
	    return LAMERROR;
	
        /* Announce the winner */
  
        if (lam_ssi_coll_verbose > 0)
            lam_debug(lam_ssi_coll_did, "init_comm: Selected coll module %s", 
                      name);

        /* All done -- happiness */

        return 0;
      }

      /* We found the right module, but it didn't want to run.  Too
	 bad.  So sad. */

      break;
    }
  }

  return LAMERROR;
}


/*
 * Call open on all the available modules (if it exists).  If open
 * returns happiness (or doesn't exist), call the init function to see
 * if that module wants to run.  If it does, do the priority matchup
 * and remember the one with the best priority.  Fill the global
 * structs with the data from the winner.  Call finalize and close on
 * all the losers that we invoked initialize on.
 *
 * Return LAMERROR if there are no modules available.
 */
static int 
check_all_modules(MPI_Comm comm)
{
  int priority, best_priority = -1;
  lam_ssi_t *best_module;
  lam_ssi_module_t *module;
  const lam_ssi_coll_actions_1_1_0_t *cur, *best = NULL;
  char *best_name;

  /* Call the query function in every collective module and see if
     they want to run on this communicator */
  
  for (best_priority = -1, 
         module = al_top(lam_ssi_coll_base_available); module != NULL;
       module = al_next(lam_ssi_coll_base_available, module)) {

    if (strcmp(module->lsm_module->ssi_module_name, "lam_basic") == 0) {
      lb_coll = module->lsm_module;
      if (lam_ssi_coll_base_open_lam_basic_for_future) {
	continue;
      }
    }

    priority = check_module(comm, module->lsm_module, &cur);
    
    if (priority > best_priority) {
      
      if (best_priority != -1) {
	/* Priority of the current module is high so the old best module is
	   not going to run. Call its end query */
	if (best_module->ssi_kind_major_version == 1 &&
	    best_module->ssi_kind_minor_version == 0 &&
	    best_module->ssi_kind_release_version == 0)
	  ((lam_ssi_coll_1_0_0_t *) best_module)->lsc_end_query(comm);
	else if (best_module->ssi_kind_major_version == 1 &&
	       best_module->ssi_kind_minor_version == 1 &&
		 best_module->ssi_kind_release_version == 0)
	  ((lam_ssi_coll_1_1_0_t *) best_module)->lsc_end_query(comm);
      }

      /* Assign current module to the new best module */
      best_priority = priority;
      best_module = module->lsm_module;
      best_name = module->lsm_module->ssi_module_name;
      best = cur;
    } else {
      if (priority != -1) {
	/* Module has less priority so end the query */
	if (module->lsm_module->ssi_kind_major_version == 1 &&
	    module->lsm_module->ssi_kind_minor_version == 0 &&
	    module->lsm_module->ssi_kind_release_version == 0)
	  ((lam_ssi_coll_1_0_0_t *) module->lsm_module)->lsc_end_query(comm);
	else if (module->lsm_module->ssi_kind_major_version == 1 &&
		 module->lsm_module->ssi_kind_minor_version == 1 &&
		 module->lsm_module->ssi_kind_release_version == 0)
	  ((lam_ssi_coll_1_1_0_t *) module->lsm_module)->lsc_end_query(comm);
      }
    }
  }

  /* If we didn't find any available modules, return an error */

  if (best_priority == -1)
    return LAMERROR;

  /* Otherwise, we have a winner.  Assign all the function pointers in
     the comm to that module, and call its init function. */

  comm->c_ssi_coll = *best;
  best = NULL;
  if (init_module(best_module, comm, &best) != 0)
    return LAMERROR;

  if (best != NULL) 
    comm->c_ssi_coll = *best;

  /* Replace all the Non null collective functions by corresponding lam_basic
     ones.  If the communicator is intercommunicator and we are using any
     single function from lam_basic, then remember that we need the local_comm
     associated with the intercomm and hence the local data structure which 
     should be hung on the communicator, we need to set this special flag so
     that during base/finalize, lam_basic finalize will be called to free
     the lam_basic data structure */

  /* Only if lam_basic module is not the best module, replace the NULL
     functions of other module */

  if (strcmp(best_module->ssi_module_name, "lam_basic") != 0)
    if (replace_null_with_lam_basic(&(comm->c_ssi_coll), comm) == LAMERROR)
      return LAMERROR;
  
  /* Announce the winner */
  
  if (lam_ssi_coll_verbose > 0)
      lam_debug(lam_ssi_coll_did, "init_comm: Selected coll module %s", 
                best_name);
  return 0;
}
 

/*
 * Check a single module
 */
static int 
check_module(MPI_Comm comm, lam_ssi_t *coll,
             const lam_ssi_coll_actions_1_1_0_t **actions)
{
  int priority = -1;

  if (coll->ssi_kind_major_version == 1 &&
      coll->ssi_kind_minor_version == 0 &&
      coll->ssi_kind_release_version == 0)
    *actions = query_1_0_0(coll, comm, &priority);
  else if (coll->ssi_kind_major_version == 1 &&
           coll->ssi_kind_minor_version == 1 &&
           coll->ssi_kind_release_version == 0)
    *actions = query_1_1_0(coll, comm, &priority);
  if (*actions != NULL) {
    
    /* If the module is lam_basic then store the pointers so that you can
       replace NULL pointers by lam_basic ones */
    if (strcmp(coll->ssi_module_name, "lam_basic") == 0) {
      lb_functions = **actions;
    }
    priority = LAM_min(priority, 100);
    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, 
		"init_comm: module available: %s, priority: %d", 
		coll->ssi_module_name, priority);
  } else {
    priority = -1;
    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, 
		"init_comm: module not available: %s, priority: %d", 
		coll->ssi_module_name, priority);
  }

  return priority;
}

/**************************************************************************
 * Query functions
 **************************************************************************/

/*
 * Query a 1.0.0 module and convert its returned actions struct to a
 * 1.1.0 actions struct.
 */
static const lam_ssi_coll_actions_1_1_0_t *
query_1_0_0(lam_ssi_t *coll, MPI_Comm comm, int *priority) 
{
  const lam_ssi_coll_actions_1_0_0_t *actions100;

  lam_ssi_coll_1_0_0_t *coll100 = (lam_ssi_coll_1_0_0_t *) coll;

  actions100 = coll100->lsc_query(comm, priority);
  if (actions100 != NULL)
    convert_actions_1_0_0_to_1_1_0(actions100, &actions110);
  return &actions110;
}


/*
 * Query a 1.1.0 module
 */
static const lam_ssi_coll_actions_1_1_0_t *
query_1_1_0(lam_ssi_t *coll, MPI_Comm comm, int *priority)
{
  lam_ssi_coll_1_1_0_t *coll110 = (lam_ssi_coll_1_1_0_t *) coll;

  return coll110->lsc_query(comm, priority);
}


/**************************************************************************
 * Init functions
 **************************************************************************/

/*
 * Initialize a module
 */
static int
init_module(lam_ssi_t *module, MPI_Comm comm, 
            const lam_ssi_coll_actions_1_1_0_t **actions)
{
  if (module->ssi_kind_major_version == 1 &&
      module->ssi_kind_minor_version == 0 &&
      module->ssi_kind_release_version == 0)
    return init_1_0_0(comm, actions);
  else if (module->ssi_kind_major_version == 1 &&
           module->ssi_kind_minor_version == 1 &&
           module->ssi_kind_release_version == 0)
    return init_1_1_0(comm, actions);
  else
    return LAMERROR;
}


/*
 * Initialize a 1.0.0 module, and if it returns a new actions struct,
 * convert it before returning.
 */
static int 
init_1_0_0(MPI_Comm comm, const lam_ssi_coll_actions_1_1_0_t **actions)
{
  const lam_ssi_coll_actions_1_0_0_t *actions100;
  lam_ssi_coll_init_1_0_0_fn_t init100;

  /* The lsca_init function is actually the 1.0.0 function, but it was
     cast to be the 1.1.0 type in the convert() function (below) in
     order to make the assignment properly.  So cast it back to the
     1.0.0 type and invoke it. */

  init100 = (lam_ssi_coll_init_1_0_0_fn_t) comm->c_ssi_coll.lsca_init;

  if (init100(comm, &actions100) != 0)
    return LAMERROR;

  if (actions != NULL) {
    *actions = &actions110;
    convert_actions_1_0_0_to_1_1_0(actions100, &actions110);
  }
  return 0;
}


/*
 * Initialize a 1.1.0 module.
 */
static int 
init_1_1_0(MPI_Comm comm, const lam_ssi_coll_actions_1_1_0_t **actions)
{
  return comm->c_ssi_coll.lsca_init(comm, actions);
}


/**************************************************************************
 * Conversion functions
 **************************************************************************/

/* 
 * Convert a v1.0.0 actions struct to a v1.1.0 actions struct
 */
static void 
convert_actions_1_0_0_to_1_1_0(const lam_ssi_coll_actions_1_0_0_t *actions100,
                               lam_ssi_coll_actions_1_1_0_t *actions110)
{
  /* Copy over all 1.0.0 values into the 1.1.0 struct */
  /* Cast the init function to the right type for a 1.1.0 struct; it
     will be cast back to the 1.0.0 type before it is invoked */

  actions110->lsca_init = (lam_ssi_coll_init_1_1_0_fn_t) actions100->lsca_init;
  actions110->lsca_finalize = actions100->lsca_finalize;

  actions110->lsca_checkpoint = actions100->lsca_checkpoint;
  actions110->lsca_continue = actions100->lsca_continue;
  actions110->lsca_restart = actions100->lsca_restart;

  actions110->lsca_allgather = actions100->lsca_allgather;
  actions110->lsca_allgatherv = actions100->lsca_allgatherv;
  actions110->lsca_allreduce = actions100->lsca_allreduce;
  actions110->lsca_alltoall = actions100->lsca_alltoall;
  actions110->lsca_alltoallv = actions100->lsca_alltoallv;
  actions110->lsca_alltoallw = actions100->lsca_alltoallw;
  actions110->lsca_barrier = actions100->lsca_barrier;
  actions110->lsca_bcast_optimization = actions100->lsca_bcast_optimization;
  actions110->lsca_bcast = actions100->lsca_bcast;
  actions110->lsca_exscan = actions100->lsca_exscan;
  actions110->lsca_gather = actions100->lsca_gather;
  actions110->lsca_gatherv = actions100->lsca_gatherv;
  actions110->lsca_reduce_optimization = actions100->lsca_reduce_optimization;
  actions110->lsca_reduce = actions100->lsca_reduce;
  actions110->lsca_reduce_scatter = 
    actions100->lsca_reduce_scatter;
  actions110->lsca_scan = actions100->lsca_scan;
  actions110->lsca_scatter = actions100->lsca_scatter;
  actions110->lsca_scatterv = actions100->lsca_scatterv;
  /* Make the new 1.1.0 values be NULL */

  actions110->lsca_interrupt = NULL;
}

/* 
 * Replace the NULL pointers by corresponsing lam_basic pointers 
 */

static int 
replace_null_with_lam_basic(lam_ssi_coll_actions_1_1_0_t *selected_module,
			    MPI_Comm comm) {
  int need_replacement_for_intra_and_inter = 0;
  int need_replacement_for_intra = 0;
  int need_replacement_for_inter = 0;
  const lam_ssi_coll_actions_1_1_0_t *actions = NULL;

  /* First see, if the selected module has some functions unimplemented */
  
  need_replacement_for_intra_and_inter =
    (selected_module->lsca_allgather && selected_module->lsca_allreduce && 
     selected_module->lsca_alltoall && selected_module->lsca_alltoallv && 
     selected_module->lsca_alltoallw && selected_module->lsca_barrier &&
     selected_module->lsca_bcast && selected_module->lsca_gather &&
     selected_module->lsca_gatherv && selected_module->lsca_reduce &&
     selected_module->lsca_reduce_scatter && selected_module->lsca_scatter &&
     selected_module->lsca_scatterv) ? 0 : 1;

  need_replacement_for_inter = need_replacement_for_intra_and_inter;
  
  need_replacement_for_intra = 
    (need_replacement_for_intra_and_inter &&
     selected_module->lsca_allgatherv &&
     selected_module->lsca_scan &&
     selected_module->lsca_exscan) ? 0 : 1; 
    
  /* Decide if we really want to grab some functions from lam_basic to replace
     NULL functions from user module */
  if (need_replacement_for_intra || need_replacement_for_inter) {
  
    if (lb_functions.lsca_allgather == NULL) {
      if (lb_coll == NULL)
	return LAMERROR;
      
      /* get the lam_basic functions */
      check_module(comm, lb_coll, &actions);
    }
  
    /* Double check that we have got the lam_basic functions */
    if (lb_functions.lsca_allgather == NULL)
      return LAMERROR;

    if (selected_module->lsca_allgather == NULL) 
      selected_module->lsca_allgather = lb_functions.lsca_allgather;
    if (selected_module->lsca_allgatherv == NULL)
      selected_module->lsca_allgatherv = lb_functions.lsca_allgatherv;
    if (selected_module->lsca_allreduce == NULL) 
      selected_module->lsca_allreduce = lb_functions.lsca_allreduce;
    if (selected_module->lsca_alltoall == NULL) 
      selected_module->lsca_alltoall = lb_functions.lsca_alltoall;
    if (selected_module->lsca_alltoallv == NULL)
      selected_module->lsca_alltoallv = lb_functions.lsca_alltoallv;
    if (selected_module->lsca_alltoallw == NULL)
      selected_module->lsca_alltoallw = lb_functions.lsca_alltoallw;
    if (selected_module->lsca_barrier == NULL) 
      selected_module->lsca_barrier = lb_functions.lsca_barrier;
    if (selected_module->lsca_bcast == NULL)
      selected_module->lsca_bcast = lb_functions.lsca_bcast;
    if (selected_module->lsca_exscan == NULL) 
      selected_module->lsca_exscan = lb_functions.lsca_exscan;
    if (selected_module->lsca_gather == NULL) 
      selected_module->lsca_gather = lb_functions.lsca_gather;
    if (selected_module->lsca_gatherv == NULL) 
      selected_module->lsca_gatherv = lb_functions.lsca_gatherv;
    if (selected_module->lsca_reduce == NULL) 
      selected_module->lsca_reduce = lb_functions.lsca_reduce;
    if (selected_module->lsca_reduce_scatter == NULL)
      selected_module->lsca_reduce_scatter = 
	lb_functions.lsca_reduce_scatter;
    if (selected_module->lsca_scan == NULL)
      selected_module->lsca_scan = lb_functions.lsca_scan;
    if (selected_module->lsca_scatter == NULL) 
      selected_module->lsca_scatter = lb_functions.lsca_scatter;
    if (selected_module->lsca_scatterv == NULL)
      selected_module->lsca_scatterv = lb_functions.lsca_scatterv;

    /* Finally store the lam_basic_finalize pointer */
    comm->lsca_basic_finalize = lb_functions.lsca_finalize;

    comm->c_flags |= LAM_RBASIC;
  }

  return 0;
}
