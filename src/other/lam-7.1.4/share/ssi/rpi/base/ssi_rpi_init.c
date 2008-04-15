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
 * $Id: ssi_rpi_init.c,v 1.8 2003/12/04 19:54:53 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdlib.h>
#include <unistd.h>

#include <all_list.h>
#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * Global variables
 */
lam_ssi_rpi_actions_t lam_ssi_rpi;
lam_ssi_rpi_t lam_ssi_rpi_base_module;


/*
 * Private variables
 */
static lam_ssi_rpi_init_1_0_0_fn_t init_1_0_0 = NULL;
static lam_ssi_rpi_actions_1_1_0_t actions_1_1_0;


/*
 * private functions
 */
static int initialize_1_0_0(struct _proc **to_rpi, lam_ssi_module_t *module);
static int initialize_1_1_0(struct _proc **to_rpi, lam_ssi_module_t *module);
static const struct lam_ssi_rpi_actions_1_1_0 *
init_convert_1_0_0_to_1_1_0(struct _proc **procs, int nprocs,
                            int *max_tag, int *max_cid);


/*
 * This is step 3 of 3 in setting up the RPI.
 *
 * This will need to change when we go multi-RPI -- we'll have have
 * the RPIs let us know which procs it wants to support and at what
vv * priority.  Then we'll have to divy them up and tell them which
 * subset of procs they can have.
 *
 * For now, we just make an array of all procs and give that to the
 * one-and-only-one RPI that we'll be using.
 *
 * The selected RPI module is at the head of the list.
 *
 * If it is a version 1.0.0, we have to convert it to 1.1.0 before
 * continuing.
 */
int
lam_ssi_rpi_base_init(void)
{
  int i, ret;
  struct _proc **to_rpi, *p;
  lam_ssi_module_t *rpi_module, *m;
  int nprocs = lam_nprocs();
  lam_ssi_rpi_t *r;

  to_rpi = malloc(sizeof(struct _proc *) * nprocs);
  if (to_rpi == NULL)
    return LAMERROR;
  for (i = 0, p = lam_topproc(); p != NULL; ++i, p = lam_nextproc())
    to_rpi[i] = p;

  /* Go handle the top pointer based on the version number of its RPI
     API */

  rpi_module = (lam_ssi_module_t*) al_top(lam_ssi_rpi_base_available);
  if (rpi_module->lsm_module->ssi_kind_major_version == 1 &&
      rpi_module->lsm_module->ssi_kind_minor_version == 0 &&
      rpi_module->lsm_module->ssi_kind_release_version == 0)
    ret = initialize_1_0_0(to_rpi, rpi_module);
  else
    ret = initialize_1_1_0(to_rpi, rpi_module);
  if (ret == LAMERROR)
    return ret;

  /* Go through and close all the other modules */

  for (m = (lam_ssi_module_t*) al_next(lam_ssi_rpi_base_available, 
                                       rpi_module); m != NULL;
       m = (lam_ssi_module_t*) al_next(lam_ssi_rpi_base_available, m)) {
    r = (lam_ssi_rpi_t*) m->lsm_module;
    if (r->lsr_meta_info.ssi_close_module != NULL)
      r->lsr_meta_info.ssi_close_module();
    lam_ssi_base_module_registry_unuse(m->lsm_module);
  }

  /* Announce */

  if (lam_ssi_rpi_verbose > 0)
    lam_debug(lam_ssi_rpi_did, " selected RPI module %s", 
              lam_ssi_rpi_base_module.lsr_meta_info.ssi_module_name);

  /* All done */

  free(to_rpi);
  return 0;
}


/*
 * Initialize 1.0.0 rpi module.
 */
static int
initialize_1_0_0(struct _proc **to_rpi, lam_ssi_module_t *module_1_0_0)
{
  lam_ssi_module_t module_1_1_0;
  lam_ssi_rpi_1_0_0_t base_1_0_0;
  lam_ssi_rpi_1_1_0_t base_1_1_0;

  /* Get the 1.0.0 RPI base module.  It's a different type than the
     1.1.0 base module, so we have to convert it. */

  base_1_0_0 = *((lam_ssi_rpi_1_0_0_t*) module_1_0_0->lsm_module);

  /* The only difference between the 1.0.0 and 1.1.0 base module is
     the return type of the init function (because there is a new
     actions struct).  Hence, we make a new module container for 1.1.0
     with a "converter" init function that simply calls the real 1.0.0
     init function and converts the return type to be a 1.1.0 actions
     struct. */

  base_1_1_0.lsr_meta_info = base_1_0_0.lsr_meta_info;
  base_1_1_0.lsr_query = base_1_0_0.lsr_query;
  base_1_1_0.lsr_init = init_convert_1_0_0_to_1_1_0;
  init_1_0_0 = base_1_0_0.lsr_init;

  /* Copy over the module holder and substitute in the new base_1_1_0
     module */

  module_1_1_0 = *module_1_0_0;
  module_1_1_0.lsm_module = (lam_ssi_t*) &base_1_1_0;

  /* Now that that's all setup, we can treat this like a 1.1.0 module.
     So call the initialize_1_1_0 function. */

  return initialize_1_1_0(to_rpi, &module_1_1_0);
}


/*
 * This function is invoked as a converter that calls 1.0.0 init
 * functions and converts their output action structs to 1.1.0 action
 * structs.  Since it is only invoked once per module that it is
 * converting, and we know that initialize_1_0_0 was invoked almost
 * immediately prior to it, the actual 1.0.0 init function pointer is
 * stored in a static global in this file (i.e., no attempt is made to
 * save it after the init function has been called and its output has
 * been converted).
 */
static const struct lam_ssi_rpi_actions_1_1_0 *
init_convert_1_0_0_to_1_1_0(struct _proc **procs, int nprocs,
                            int *max_tag, int *max_cid)
{
  const lam_ssi_rpi_actions_1_0_0_t *actions_1_0_0;

  /* Call the 1.0.0 init function.  The "init_1_0_0" function pointer
     is a variable static to this file, and was loaded back in
     initialize_1_0_0().  The value in this function pointer is only
     used once during this traversal of the call stack (that started
     with initialize_1_0_0()) -- so it's safe to use this value
     here. */

  actions_1_0_0 = init_1_0_0(procs, nprocs, max_tag, max_cid);
  if (actions_1_0_0 == NULL) 
    return NULL;

  /* If we got a valid actions struct back, convert it to a 1.1.0
     actions struct.  Note that we have to return a *pointer*, hence
     the thing pointed to must still exist outside the scope of this
     function.  So it's a static global struct instance
     (actions_1_1_0) that will be copied upon return, so it's ok even
     if this function is invoked multiple times  */

  actions_1_1_0.lsra_addprocs = actions_1_0_0->lsra_addprocs;
  actions_1_1_0.lsra_finalize = actions_1_0_0->lsra_finalize;

  actions_1_1_0.lsra_build = actions_1_0_0->lsra_build;
  actions_1_1_0.lsra_start = actions_1_0_0->lsra_start;
  actions_1_1_0.lsra_advance = actions_1_0_0->lsra_advance;
  actions_1_1_0.lsra_destroy = actions_1_0_0->lsra_destroy;

  actions_1_1_0.lsra_iprobe = actions_1_0_0->lsra_iprobe;

  actions_1_1_0.lsra_fastrecv = actions_1_0_0->lsra_fastrecv;
  actions_1_1_0.lsra_fastsend = actions_1_0_0->lsra_fastsend;

  actions_1_1_0.lsra_alloc_mem = actions_1_0_0->lsra_alloc_mem;
  actions_1_1_0.lsra_free_mem = actions_1_0_0->lsra_free_mem;

  actions_1_1_0.lsra_checkpoint = actions_1_0_0->lsra_checkpoint;
  actions_1_1_0.lsra_continue = actions_1_0_0->lsra_continue;
  actions_1_1_0.lsra_restart = actions_1_0_0->lsra_restart;

  actions_1_1_0.lsra_tv_queue_support = 
    actions_1_0_0->lsra_tv_queue_support;

  /* Initialize the new 1.1.0 members to defaults (since they weren't
     in the returned 1.0.0 actions struct) */

  actions_1_1_0.lsra_interrupt = NULL;

  /* All done */

  return &actions_1_1_0;
}


/*
 * Initialize a 1.1.0 rpi module.
 */
static int
initialize_1_1_0(struct _proc **to_rpi, lam_ssi_module_t *module)
{
  int nprocs = lam_nprocs();
  int maxtag = lam_mpi_max_tag, maxcid = lam_mpi_max_cid;
  const lam_ssi_rpi_actions_1_1_0_t *rpia;

  /* Setup the RPI function pointers and base module. */

  lam_ssi_rpi_base_module = *((lam_ssi_rpi_t*) module->lsm_module);

  /* Save the resulting actions struct in the global variable.  This
     will need to change when we go multi-RPI. */

  rpia = lam_ssi_rpi_base_module.lsr_init(to_rpi, nprocs, &maxtag, &maxcid);
  if (rpia == NULL) {
    free(to_rpi);
    return LAMERROR;
  }
  lam_ssi_rpi = *rpia;
  if (maxtag > 0 && maxtag < lam_mpi_max_tag)
    lam_mpi_max_tag = maxtag;
  if (maxcid > 0 && maxcid < lam_mpi_max_cid)
    lam_mpi_max_cid = maxcid;

  /* All done */

  return 0;
}
