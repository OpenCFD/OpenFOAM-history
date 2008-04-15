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
 * $Id: ssi_boot_tm.c,v 1.40 2004/03/06 21:17:32 jsquyres Exp $
 *
 *	Function:	- tm boot SSI module
 */

#include <lam_config.h>
#include <lam-ssi-boot-tm-config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <tm.h>

#include <ssi_boot_tm_lamnode.h>
#include <lamnet.h>
#include <args.h>
#include <net.h>
#include <sfh.h>
#include <lamdebug.h>
#include <boot.h>
#include <proc_schema.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <lam-ssi-boot-tm.h>


/*
 * Local variables
 */
static int param_priority;
static int param_first;
static int origin = -1;
static struct lamnode origin_node;
static const lam_ssi_boot_actions_1_1_0_t module_actions = {

  lam_ssi_boot_tm_parse_options,
  lam_ssi_boot_tm_allocate_nodes,
  lam_ssi_boot_tm_verify_nodes,
  lam_ssi_boot_tm_prepare_boot,
  lam_ssi_boot_tm_start_rte_procs,
  lam_ssi_boot_tm_deallocate_nodes,

  lam_ssi_boot_tm_start_application,
  lam_ssi_boot_tm_start_rte_proc,

  /* The rest of the functions use the generic TCP protocol functions
     that are common to all boot SSI's */

  lam_ssi_boot_base_get_nodeid,
  lam_ssi_boot_base_open_srv_connection,
  lam_ssi_boot_base_send_lamd_info,
  lam_ssi_boot_base_receive_lamd_info,
  lam_ssi_boot_base_close_srv_connection,

  lam_ssi_boot_base_send_universe_info,
  lam_ssi_boot_base_receive_universe_info,
};


/*
 * External variables
 */
extern char **environ;


int 
lam_ssi_boot_tm_open_module(OPT *ad)
{
  param_priority =
    lam_ssi_base_param_register_int("boot", "tm", "priority", NULL, 
                                    LAM_SSI_BOOT_TM_DEFAULT ? 75 : 50);
  param_first =
    lam_ssi_base_param_register_int("boot", "tm", "first", NULL, -1);

  return 1;
}


/***********************************************************************
 *
 * External interface
 *
 ***********************************************************************/

/*
 * lam_ssi_boot_tm_init
 *
 * DESCRIPTION:
 *  Initialize the module.  Any work to allow other steps to complete
 *  is done here (ie, initialize the TM interface)
 *
 * RETURNS:
 *  0        - on success
 *  LAMERROR - on failure
 *
 */
const lam_ssi_boot_actions_t *
lam_ssi_boot_tm_init(lam_ssi_boot_location_t where, int *priority)
{
  struct tm_roots root;
  int ret;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  /* Are we running in a PBS environment? */

  if (getenv("PBS_ENVIRONMENT") == NULL) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did, "tm: not running under PBS");
    *priority = -1;
    return NULL;
  }

  /* Fire up TM interface -- but only on the root! */

  if (where == LAM_SSI_BOOT_LOCATION_ROOT) {
    ret = tm_init(NULL, &root);
    if (ret != TM_SUCCESS) {
      return NULL;
    }
  }

  /* If we're verbose, print stuff out */

  if (lam_ssi_boot_verbose >= 5) {
    lam_debug(lam_ssi_boot_did, "tm: module initializing");
    lam_debug(lam_ssi_boot_did, "tm:verbose: %d", 
	      lam_ssi_boot_verbose);
    lam_debug(lam_ssi_boot_did, "tm:priority: %d", *priority);
  }

  /* Return the struct with the function pointers in it for all the
     API functions */

  return &module_actions;
}


/*
 * lam_ssi_boot_tm_parse_options
 *
 * DESCRIPTION:
 *  Parse options
 *
 * RETURNS:
 *  0   - success
 *
 */
int 
lam_ssi_boot_tm_parse_options(OPT *aod, int schema)
{
  if (schema == 1)
    return 0;
  else {
    show_help_file("lam-ssi-boot-tm-helpfile", "parse-options",
                   "lamgrow-makes-no-sense", NULL);
    return LAMERROR;
  }
}


/*
 * lam_ssi_boot_tm_allocate_nodes
 *
 * DESCRIPTION:
 *  Create and fill in a lamnode structure for each node to be booted.
 *  Most of the work happens in lam_ssi_boot_tm_bhostparse, which is a
 *  lot like the bhostparse() used in the RSH boot style, but slightly
 *  different :)
 *
 * RETURNS:
 *  0        - success
 *  LAMERROR - failure
 */
int 
lam_ssi_boot_tm_allocate_nodes(struct lamnode **nodes, int *nnodes,
				int *origin_arg)
{
  int i;
  struct lamnode *n;

  /* Parse the system file */
  if (lam_ssi_boot_tm_bhostparse(nodes, nnodes) != 0)
    return 1;
  n = *nodes;

  i = lam_ssi_base_param_lookup_int(param_first);
  if (i != -1) {
    *nnodes = i;
  }

  /* Print it out if our verbosity is high enough.  No need to check
     for lnd_type == NOTNODEID here because we'll never be called for
     lamgrow. */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, 
	      "tm: found the following %d hosts:", *nnodes);
    for (i = 0; i < *nnodes; i++)
      lam_debug(lam_ssi_boot_did, "tm:   n%d %s (cpu=%d)", 
		i, n[i].lnd_hname, n[i].lnd_ncpus);
  }

  /* Resolve the host nodes.  Errors will be printed by the functions
     themselves */
  if (lamnet_findhosts(*nodes, *nnodes, &i) != 0)
    return LAMERROR;

  /* Ensure that we have an origin node; set it in the lamnet struct */
  *origin_arg = origin = lamnet_findorig(*nodes, *nnodes);
  if (origin < 0) {
    show_help("boot", "no-localhost", "The boot SSI tm module", 
	      "PBS_NODEFILE", NULL);
    return LAMERROR;
  }
  nodes[origin]->lnd_type |= NT_ORIGIN | NT_ME;

  /* Save a copy for use down below -- we'll need to know the origin's
     IP address */
  origin_node = *(nodes[origin]);

  return 0;
}


/*
 * lam_ssi_boot_tm_verify_nodes
 *
 * DESCRIPTION:
 *  do nothing - this doesn't make sense in the current context.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_tm_verify_nodes(struct lamnode *nodes, int nnodes)
{
  /* Don't really have to do anything - I don't think that the info
     above can change... */
  return 0;
}


/*
 * lam_ssi_boot_tm_prepare_boot
 *
 * DESCRIPTION:
 *  Perform any work that might be needed in start_rte_procs(0, but
 *  only needs to be done once.  This probably made more sense as a
 *  function before we remembered the whole wipe / lamgrow / lamshrink
 *  issue.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_tm_prepare_boot(void)
{
  return 0;
}


/*
 * lam_ssi_boot_tm_start_rte_procs
 *
 * DESCRIPTION:
 *  Takes a nodes arreay and starts whatever needs to be started where
 *  ever it needs to be started
 *
 * RETURNS:
 *  0       - success
 *  nonzero - error from lower layers
 */
int 
lam_ssi_boot_tm_start_rte_procs(struct lamnode *nodes, int nnodes,
				 lam_ssi_boot_proc_t which,
				 int *num_started)
{
  int ret;

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "tm: starting RTE procs");
  /* This doesn't seem quite right to me - why specify the type above
     (in init()) only to specify it again in the call type here.  Oh
     well, what can you do? */
  ret = 
    lam_ssi_boot_base_alg_linear_windowed(nodes, nnodes, 
                                          (which == LAM_SSI_BOOT_PROC_LAMD),
                                          which, num_started);
  if (lam_ssi_boot_verbose >= 1 && ret == 0 && *num_started == nnodes)
    lam_debug(lam_ssi_boot_did, "tm: all RTE procs started");

  return ret;
}


/*
 * lam_ssi_boot_tm_deallocate_nodes
 *
 * DESCRIPTION:
 *  Clean up any resources from the lamnode structs that we may need
 *  to clean up.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_tm_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  lam_deallocate_nodes(nodes, nnodes);
  return 0;
}


/*
 * lam_ssi_boot_tm_finalize
 *
 * DESCRIPTION:
 *  Finalize use of module.  Close down the TM interface.
 *
 * RETURNS:
 *  0        - success
 *  non-zero - failure (caused by TM)
 */
int 
lam_ssi_boot_tm_finalize(void)
{
  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "tm: finalizing");

  return tm_finalize();
}


/***********************************************************************
 *
 * Internal interface
 *
 * Upcalls from the algorithm functions
 *
 ***********************************************************************/


/*
 * This is the main workhorse for starting applications on remote
 * nodes.  Takes an argv/arc and starts it up on the target lamnode.
 *
 * Called by either tm_start_rte_proc() (below) to start a run-time
 * environment process on a remote node, or by the booting algorithm
 * to start a helper application on a remote node.  
 */
int 
lam_ssi_boot_tm_start_application(char ***argv, int *argc, 
				       int num_procs, struct lamnode *node)
{
  return lam_ssi_boot_tm_start_application_wait(argv, argc, num_procs, node, 0);
}


int 
lam_ssi_boot_tm_start_application_wait(char ***argv, int *argc, 
				       int num_procs, struct lamnode *node,
				       int wait)
{
  int i;
  int ret = 0;
  char *full, *old;
  tm_task_id tid;
  tm_event_t event;
  int tm_errno;                 /* TM's special errno */
  int obit;

  for (i = 0 ; i < num_procs ; ++i) {
    /* we need the full path name to be able to tm_spawn() */
    full = sfh_path_env_find((*argv)[0], R_OK | X_OK);
    if (full == NULL) {
      fprintf(stderr, "Can't find executable for %s\n", argv[i][0]);
      return LAMERROR;
    }

    /* So, we really can't clean up after ourselves on this argv thing
     * (who knws if argv[0] is free()able).  So we cheat-n-restore.
     * No harm, no foul. */
    old = argv[i][0];
    argv[i][0] = full;

    /* No need to check for lnd_type == NOTNODEID because we'll never
       be called in for lamgrow. */

    if (lam_ssi_boot_verbose >= 1) {
      lam_debug(lam_ssi_boot_did, "tm: starting on n%d (%s): %s",
		node->lnd_nodeid, node->lnd_hname, 
		sfh_argv_glue(argv[i], ' ', 0));
    }

    ret = tm_spawn(argc[i], argv[i], environ, 
		   node->lnd_ssi->tm_node_num,
		   &tid, &event);

    argv[i][0] = old;
    free(full);

    if (ret != TM_SUCCESS) {
      lam_debug(lam_ssi_boot_did, 
		"tm: failed launched on n%d (%s)",
		node->lnd_nodeid, node->lnd_hname);
      return 1;
    }

    ret = tm_poll(TM_NULL_EVENT, &event, 1, &tm_errno);
    if (ret != TM_SUCCESS) {
      return 1;
    }

    /* Be verbose */
    if (ret == 0 && lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, 
		"tm: successfully launched on n%d (%s)",
		node->lnd_nodeid, node->lnd_hname);

    if (wait != 0) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "tm: waiting for completion on n%d (%s)",
		  node->lnd_nodeid, node->lnd_hname);

      ret = tm_obit(tid, &obit, &event);
      if (ret != TM_SUCCESS) {
	return 1;
      }

      ret = tm_poll(TM_NULL_EVENT, &event, 1, &tm_errno);
      if (ret != TM_SUCCESS) {
	return 1;
      }

      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "tm: finished on n%d (%s)",
		  node->lnd_nodeid, node->lnd_hname);
    }
  }

  return 0;
}


/*
 * Start up a given RTE proc on a target node.  This function
 * essentially seeds the argc/argv and then calls
 * tm_start_application() (above).
 */
int 
lam_ssi_boot_tm_start_rte_proc(struct lamnode *node, 
				lam_ssi_boot_proc_t which)
{
  int ret = 0;
  int argc = 0;
  char **argv = 0;
  char *inet_buf;
  char **av_cmd;
  int ac_cmd;
  LIST *list_psc;
  struct psc *p;


  /* Setup argc/argv */

  switch (which) {
  case LAM_SSI_BOOT_PROC_LAMD:

    /* Mwah ha ha... kill whatever was there */

    ret = lam_ssi_boot_tm_start_rte_proc(node, LAM_SSI_BOOT_PROC_WIPE);
    if (ret != 0) {
      return LAMERROR;
    }

    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "tm: starting lamd on (%s)",
		node->lnd_hname);

    /* Build inet topo */

    inet_buf = lam_ssi_boot_build_inet_topo(node, origin_node, origin);
    if (inet_buf == NULL) {
      fprintf(stderr, "Could not form inet_topo\n");
      perror("boot tm");
      return LAMERROR;
    }

    /* get list of things we should do */

    if (hbootparse(lam_ssi_boot_did, lam_ssi_boot_optd,  
		   inet_buf, NULL, &list_psc)) {
      show_help("hboot", "cant-parse-config", NULL);
      exit(1);
    }

    for (p = al_top(list_psc); p; p = al_next(list_psc, p)) {
      /* build up the argv array */
      av_cmd = sfh_argv_dup(p->psc_argv);
      ac_cmd = p->psc_argc;

      ret += lam_ssi_boot_tm_start_application(&av_cmd, &ac_cmd,
					       1, node);
      if (ret != 0) {
	return LAMERROR;
      }
    }

    al_free(list_psc);
    break;

  case LAM_SSI_BOOT_PROC_RECON:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "tm: starting recon on (%s)",
		node->lnd_hname);

    sfh_argv_add(&argc, &argv, DEFTTKILL);
    sfh_argv_add(&argc, &argv, "-N");
    break;

  case LAM_SSI_BOOT_PROC_WIPE:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "tm: starting wipe on (%s)",
		node->lnd_hname);
    sfh_argv_add(&argc, &argv, DEFTTKILL);
    sfh_argv_add(&argc, &argv, "-setsid");

    /* Common args */
    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);
    break;

  default:
    show_help("ssi-boot", "start-unknown-rte-proc", NULL);
    ret = 1;
    break;
  }

  /* If we've got a valid argc/argv, boot it (locally or remotely) */
  /* no start lamd - already done */
  if (argc > 0 &&  which != LAM_SSI_BOOT_PROC_LAMD)
    ret = lam_ssi_boot_tm_start_application_wait(&argv, &argc, 1, node, 1);

  return ret;
}
