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
 *	Function:	- slurm boot SSI module
 *
 * Thanks to Morris Jette and Mark Grondona (The SLURM Guys) for their
 * considerable help with this module.  
 *
 * This module can be invoked in two ways, due to the way that SLURM
 * works:
 *
 * 1. The "Normal" way, where you srun a script that runs on the first
 * node of the allocation and invokes "lamboot".  In this case,
 * lamboot is running on one of the nodes in the SLURM allocation, and
 * this module functions something similar to the PBS/TM boot module.
 *
 * 2. The "NT_WASTE" way, where you use the "-A" option to srun and
 * only get an allocation of nodes, but aren't actually running on any
 * of the nodes in the allocation.  In this case, the module will
 * automatically detect this and automatically add this node to the
 * end of the list of nodes being booted and mark it with NT_WASTE
 * (similar to the bproc module).  Hence, it's possible to run mpirun
 * (and friends) from this node, but by default, MPI jobs won't be
 * allocated to this node.  This is referred to as "the NT WASTE case"
 * throughout comments in the code.
 *
 * Note that the NT_WASTE case causes several special cases throughout
 * the code (and are labeled as such).  It entails at least the
 * following:
 *
 * - it's ok to run lamboot if we're NODEID 0 or in the NT_WASTE case
 *   and we're the highest-ranked node.
 *
 * - in the NT_WASTE case, add an entry with a fake hostname to the
 *   list of nodes in the universe.
 *
 * - when sending lamd info back, if we're the head node in the
 *   NT_WASTE case, we won't have the SLURM_LAUNCH_NODE_IPADDR
 *   environment variable set.  So int his case, re-use the
 *   origin_node IP address (if we use 127.0.0.1, then the boot agent
 *   will get a peer IP address of 127.0.0.1 and will think that the
 *   connection is unexpected).
 *
 * - when srunning in the NT WASTE case, if the number of processes to
 *   srun is not equal to the total number of processes, fork off a
 *   local copy (because it's supposed to run on the head node).  In
 *   this case, note that we have the launch off a shepherd process to
 *   watch for the death of the srun process so that we know when to
 *   kill the forked process (e.g., if the SLURM job dies
 *   unexpectedly, SLURM will take care of killing the srun process
 *   but the locally-forked process would otherwise have become a
 *   child of init and therefore SLURM won't know of its existence
 *   anymore).
 *
 * Environment variables used by this module:
 *
 * SLURM_NNODES: Number of nodes that the job will use, unless we're
 * in the NT_WASTE mode, in which case it'll be one more than this
 * value.
 *
 * SLURM_NODELIST: A list of the nodes allocated to the job in a
 * regexp-kind of specification.
 *
 * SLURM_TASKS_PER_NODE: A specification of the cpu counts in a
 * pseudo-regexp kind of specification.
 *
 * SLURM_NODEID: The node ID of this node (in the range of [0,
 * SLURM_NNODES)).  If it's NULL, then use the value of SLURM_NNODES
 * because that means we're in NT_WASTE mode and this is the booting
 * node.
 *
 * SLURM_LAUNCH_NODE_IPADDR: The IP address of the booting agent.
 */

#include <lam_config.h>
#include <lam-ssi-boot-slurm-config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <netdb.h>

#include <lamnet.h>
#include <args.h>
#include <net.h>
#include <sfh.h>
#include <lamdebug.h>
#include <boot.h>
#include <proc_schema.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <all_array.h>
#include <lam-ssi-boot-slurm.h>


/*
 * Local variables
 */
static int param_priority;
static int origin = -1;
static struct lamnode origin_node;
static const lam_ssi_boot_actions_1_1_0_t module_actions = {

  lam_ssi_boot_slurm_parse_options,
  lam_ssi_boot_slurm_allocate_nodes,
  lam_ssi_boot_slurm_verify_nodes,
  lam_ssi_boot_slurm_prepare_boot,
  lam_ssi_boot_slurm_start_rte_procs,
  lam_ssi_boot_slurm_deallocate_nodes,

  /* We don't use any of the stock algorithms, so we don't need to
     provide these functions */

  NULL,
  NULL,

  /* The rest of the functions use the generic TCP protocol functions
   that are common to all boot SSI's.  The send_lamd_info, however,
   is a slrum-module-specific function that is a thin wrapper around
   send_lamd_info_manual because the target lamd will determine its
   own node ID (rather than obtain it via a command line
   parameter). */

  lam_ssi_boot_slurm_get_nodeid,
  lam_ssi_boot_base_open_srv_connection,
  lam_ssi_boot_slurm_send_lamd_info,
  lam_ssi_boot_base_receive_lamd_info,
  lam_ssi_boot_base_close_srv_connection,

  lam_ssi_boot_base_send_universe_info,
  lam_ssi_boot_base_receive_universe_info,
};


/*
 * Local functions
 */
static int start_lamds(struct lamnode *nodes, int nnodes, int *num_started);
static int srun_procs(char **argv, int argc, struct lamnode *nodes, 
                      int boot_cnt, int want_local_launch, int want_wait);
static int getnodename(char *name, size_t len);
static int wait_die(pid_t pid, int check_return_status);


int 
lam_ssi_boot_slurm_open_module(OPT *ad)
{
  param_priority =
    lam_ssi_base_param_register_int("boot", "slurm", "priority", NULL, 
                                    LAM_SSI_BOOT_SLURM_DEFAULT ? 75 : 50);

  return 1;
}


/***********************************************************************
 *
 * External interface
 *
 ***********************************************************************/

/*
 * lam_ssi_boot_slurm_init
 *
 * DESCRIPTION:
 *  Initialize the module.  Any work to allow other steps to complete
 *  is done here (ie, initialize the SLURM interface)
 *
 * RETURNS:
 *  0        - on success
 *  LAMERROR - on failure
 *
 */
const lam_ssi_boot_actions_t *
lam_ssi_boot_slurm_init(lam_ssi_boot_location_t where, int *priority)
{
  int node_id;
 
  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  /* Are we running in a SLURM environment? */

  if (getenv("SLURM_JOBID") == NULL) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did, "slurm: not running under SLURM");
    *priority = -1;
    return NULL;
  }

  /* Double check for common user error -- we know that we're running
     under SLURM; double check that we're only running on node ID 0 or
     the NT_WASTE node (see comments at the beginning of this file) in
     this job.  If we're not, then just exit.  */

  node_id = lam_ssi_boot_slurm_get_nodeid(NULL);
  if (where == LAM_SSI_BOOT_LOCATION_ROOT &&
      (node_id != 0 && getenv("SLURM_NODEID") != NULL)) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did,
                "slurm: boot agent on SLURM node %d exiting", node_id);
    lamlog("slurm agent on node %d exiting\n", node_id);
    exit(-1);
  }

  /* If we're verbose, print stuff out */

  if (lam_ssi_boot_verbose >= 5) {
    lam_debug(lam_ssi_boot_did, "slurm: module initializing");
    lam_debug(lam_ssi_boot_did, "slurm:verbose: %d", 
	      lam_ssi_boot_verbose);
    lam_debug(lam_ssi_boot_did, "slurm:priority: %d", *priority);
    lam_debug(lam_ssi_boot_did, "slurm:job_id: %s", 
              getenv("SLURM_JOBID"));
  }

  /* Return the struct with the function pointers in it for all the
     API functions */

  return &module_actions;
}


/*
 * lam_ssi_boot_slurm_parse_options
 *
 * DESCRIPTION:
 *  Parse options
 *
 * RETURNS:
 *  0   - success
 *
 */
int 
lam_ssi_boot_slurm_parse_options(OPT *aod, int schema)
{
  if (schema == 1)
    return 0;
  else {
    show_help_file("lam-ssi-boot-slurm-helpfile", "parse-options",
                   "lamgrow-makes-no-sense", NULL);
    return LAMERROR;
  }
}


/*
 * lam_ssi_boot_slurm_allocate_nodes
 *
 * DESCRIPTION:
 *  Create and fill in a lamnode structure for each node to be booted.
 *  Most of the work happens in lam_ssi_boot_slurm_bhostparse, which is a
 *  lot like the bhostparse() used in the RSH boot style, but slightly
 *  different :)
 *
 * RETURNS:
 *  0        - success
 *  LAMERROR - failure
 */
int 
lam_ssi_boot_slurm_allocate_nodes(struct lamnode **nodes_arg, int *nnodes_arg,
                                  int *origin_arg)
{
  int i, base, cnt, reps;
  struct lamnode *nodes;
  char *begptr, *endptr, *tasks_per_node;
  char *slurm_node_str;
  lam_array_t *slurm_nodelist;
  char **host_names;
  struct hostent *host_ptr;
  char short_hostname[BUFSIZ], buf[BUFSIZ];

  /* Get the number of SLURM nodes */

  *nnodes_arg = atoi(getenv("SLURM_NNODES"));
  if (*nnodes_arg <= 0) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                   "bad-value", "SLURM_NNODES", getenv("SLURM_NNODES"), NULL);
    return LAMERROR;
  }

  /* Allocate an array of struct lamnodes */

  *nodes_arg = nodes = (struct lamnode *) calloc(sizeof(struct lamnode),
                                                 (*nnodes_arg + 1)); 
  if (nodes == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    return LAMERROR;
  }

  /* Get the job ID and lookup SLURM node info */

  slurm_node_str = getenv("SLURM_NODELIST");
  if (slurm_node_str == NULL) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                   "not-found", "SLURM_NODELIST", NULL);
    return LAMERROR;
  }
  slurm_nodelist = lam_ssi_boot_slurm_parse_hostlist(slurm_node_str);
  if (slurm_nodelist == NULL) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                   "bad-value", "SLURM_NODELIST", 
                   slurm_node_str, NULL);
    return LAMERROR;
  }
  host_names = lam_arr_get(slurm_nodelist);
  getnodename(short_hostname, sizeof(short_hostname));

  /* Loop over each node and make a struct lamnode for it.  Set ncpus
     to 1; it'll be fixed below. */

  for (i = 0 ; i < *nnodes_arg; ++i) {
    nodes[i].lnd_nodeid = i;
    nodes[i].lnd_type = NT_BOOT;

    /* Create fake node names for prettyprint, but get their real IP
       addresses from the host name. The IP addresses are available 
       directly from the slurm_config_allocation() function, but 
       that adds a dependency upon a SLURM library and complicates 
       the building of LAM/MPI. An example of slurm_config_allocation() 
       is shown below. */

    if ((origin < 0) && (strcmp(host_names[i], short_hostname) == 0)) {
       nodes[i].lnd_type |= NT_ORIGIN | NT_ME;
       *origin_arg = origin = i;
    }

    /* Look up the address for the host name */

    host_ptr = gethostbyname(host_names[i]);
    if (host_ptr == NULL) {
      show_help("boot", "resolve-hostname", host_names[i], NULL);
      return LAMERROR;
    }
    memcpy(&nodes[i].lnd_addr.sin_addr, host_ptr->h_addr_list[0], 
           host_ptr->h_length);
    nodes[i].lnd_addr.sin_family = AF_INET;
    nodes[i].lnd_addr.sin_port = 0;
    nodes[i].lnd_hname = host_names[i];
    nodes[i].lnd_ncpus = 1;
  }
  lam_arr_free(slurm_nodelist);

  /* Fix the ncpu information */

  tasks_per_node = getenv("SLURM_TASKS_PER_NODE");
  if (tasks_per_node == NULL) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                   "not-found", "SLURM_TASKS_PER_NODE", NULL);
    return LAMERROR;
  }
  begptr = tasks_per_node;
  base = 0;
  while (begptr) {
    cnt = strtol(begptr, &endptr, 10);
    if ((endptr[0] == '(') && (endptr[1] == 'x')) {
      reps = strtol((endptr+2), &endptr, 10);
      if (endptr[0] == ')')
        endptr++;
    } else
      reps = 1;
    for (i = 0; i < reps; i++)
      nodes[base++].lnd_ncpus = cnt;
      
    if (*endptr == ',')
      begptr = endptr + 1;
    else if (*endptr == '\0')
      break;
    else {
      show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                     "bad-value", "SLURM_TASKS_PER_NODE", 
                     tasks_per_node, NULL);
      return LAMERROR;
    }
  }

  /* By definition, in SLURM, we are guaranteed to have an origin.
     For batch jobs this will be node zero. For other jobs, it could
     be anywhere, even a node not in the allocation (e.g. a login
     node).  Hence, we can't ask SLURM to launch on that node because
     it's not part of the allocation.  So add an NT_WASTE entry for it,
     and we'll have to fork/exec anything that goes on that node
     (because, by definition, it will be the node where the boot agent
     is running).  See the comments at the beginning of this file. */

  if (origin < 0) {
    *origin_arg = origin = i = (*nnodes_arg)++;
    nodes[i].lnd_nodeid = i;
    nodes[i].lnd_type = NT_WASTE | NT_ORIGIN | NT_ME | NT_BOOT;
    host_ptr = gethostbyname(short_hostname);
    memcpy(&nodes[i].lnd_addr.sin_addr, host_ptr->h_addr_list[0], 
           host_ptr->h_length);
    nodes[i].lnd_addr.sin_family = AF_INET;
    nodes[i].lnd_addr.sin_port = 0;

    snprintf(buf, sizeof(buf) - 1, "slurm_origin_%s", short_hostname);
    buf[sizeof(buf) - 1] = '\0';
    nodes[i].lnd_hname = strdup(buf);
    nodes[i].lnd_ncpus = 1;
  }

  /* Print it out if our verbosity is high enough.  No need to check
     for lnd_type == NOTNODEID here because we'll never be called for
     lamgrow. */
  
  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, 
              "slurm: found the following %d hosts:", *nnodes_arg);
    for (i = 0; i < *nnodes_arg; i++)
      lam_debug(lam_ssi_boot_did, "slurm:   n%d %s %s (cpu=%d)",
                i, nodes[i].lnd_hname, 
                inet_ntop(AF_INET, &nodes[i].lnd_addr.sin_addr, 
                          buf, sizeof(buf)),
                nodes[i].lnd_ncpus);
  }

  /* Save a copy for use down below -- we'll need to know the origin's
     IP address */

  origin_node = nodes[origin];

  return 0;
}


/*
 * lam_ssi_boot_slurm_verify_nodes
 *
 * DESCRIPTION:
 *  do nothing - this doesn't make sense in the current context.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_slurm_verify_nodes(struct lamnode *nodes, int nnodes)
{
  /* Nothing to do here -- don't need to modify the node list */

  return 0;
}


/*
 * lam_ssi_boot_slurm_prepare_boot
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
lam_ssi_boot_slurm_prepare_boot(void)
{
  /* Nothing to do here */

  return 0;
}


/*
 * lam_ssi_boot_slurm_start_rte_procs
 *
 * DESCRIPTION:
 *  Takes a nodes array and starts whatever needs to be started where
 *  ever it needs to be started
 *
 * RETURNS:
 *  0       - success
 *  nonzero - error from lower layers
 */
int 
lam_ssi_boot_slurm_start_rte_procs(struct lamnode *nodes, int nnodes,
                                   lam_ssi_boot_proc_t which,
                                   int *num_started)
{
  int i, ret = 0, boot_cnt = 0;
  int argc = 0;
  char **argv = 0;

  /* Initially say that none started */

  *num_started = 0;

  /* Setup argc/argv */

  switch (which) {
  case LAM_SSI_BOOT_PROC_LAMD:
    /* Use a different function to start lamds -- there's a lot of
       baggage that goes with starting up the RTE. */

    ret = start_lamds(nodes, nnodes, num_started);
    break;

  case LAM_SSI_BOOT_PROC_RECON:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm: starting recon");

    sfh_argv_add(&argc, &argv, DEFTTKILL);
    sfh_argv_add(&argc, &argv, "-N");
    break;

  case LAM_SSI_BOOT_PROC_WIPE:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm: starting wipe");

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

  /* If we've got a valid argc/argv and we're not starting the lamd
     (because that was already done above), boot it (locally or
     remotely) */

  /* Compute how many nodes we should use.  Used to compare to
     determine whether we're running the NT_WASTE case or not. */

  for (i = 0; i < nnodes; i++) {
    if ((nodes[i].lnd_type & NT_WASTE) == 0)
      boot_cnt++;
  }

  if (argc > 0)
    ret = srun_procs(argv, argc, nodes, boot_cnt, 
                     (boot_cnt < nnodes) ? 1 : 0, 1);

  if (ret == 0) {
    *num_started = nnodes;
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm: all RTE procs started");
  } else
    lam_debug(lam_ssi_boot_did, "slurm: RTE proc failure");

  return ret;
}


/*
 * lam_ssi_boot_slurm_deallocate_nodes
 *
 * DESCRIPTION:
 *  Clean up any resources from the lamnode structs that we may need
 *  to clean up.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_slurm_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  lam_deallocate_nodes(nodes, nnodes);

  return 0;
}


/*
 * lam_ssi_boot_slurm_finalize
 *
 * DESCRIPTION:
 *  Finalize use of module.  Close down the SLURM interface.
 *
 * RETURNS:
 *  0        - success
 *  non-zero - failure (caused by SLURM)
 */
int 
lam_ssi_boot_slurm_finalize(void)
{
  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "slurm: finalizing");

  /* Nothing to do */

  return 0;
}


/*
 * This function is a simple abstraction because we need to get the
 * SLURM node ID in multiple places.  Hence, we might as well not
 * duplicate the code necessary to do it.
 */
int
lam_ssi_boot_slurm_get_nodeid(OPT *aod)
{
  char *e;

  /* Get a node ID for both the normal case and the NT_WASTE case.  If
     it's the NT_WASTE case, then just use SLURM_NNODES (because it'll
     be one more than all other values of SLURM_NODEID). */

  if ((e = getenv("SLURM_NODEID")) == NULL) {
    e = getenv("SLURM_NNODES");
    if (e == NULL) {
      show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                     "not-found", "SLURM_NODEID", NULL);
      return -1;
    }
  }
  
  /* Otherwise, we found something */

  return atoi(e);
}


/*
 * Obtain our node ID from $SLURM_NODEID and then invoke the
 * underlying lam_ssi_boot_base_send_lamd_info_args() function.
 */
int
lam_ssi_boot_slurm_send_lamd_info(OPT *aod, int dli_port)
{
  char *tasks_per_node, *begptr, *endptr;
  int base, cnt, reps;
  int node_id, task_cnt = 1;
  unsigned char agent_haddr[4] = { 0, 0, 0, 0 };
  int a, b, c, d;

  /* Use the originating IP address.  If we can't find
     SLURM_LAUNCH_NODE_IPADDR, then assume that this is the NT_WASTE
     node (i.e., we were launched locally) and use
     origin_node.lnd.addr.sin_addr.  We *know* that this is a SLURM
     job, and therefore this node should have a lamd, otherwise this
     boot module wouldn't have been selected.  So this is a safe
     assumption to make. */

  begptr = getenv("SLURM_LAUNCH_NODE_IPADDR");
  if (begptr != NULL) {
    sscanf(begptr, "%d.%d.%d.%d", &a, &b, &c, &d);
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did,
                "slurm: lamboot is SLURM_LAUNCH_NODE_IPADDR=%d.%d.%d.%d", 
                a, b, c, d);
    agent_haddr[0] = (unsigned char) a;
    agent_haddr[1] = (unsigned char) b;
    agent_haddr[2] = (unsigned char) c;
    agent_haddr[3] = (unsigned char) d;
  } else {
    agent_haddr[0] = ((unsigned char *) &origin_node.lnd_addr.sin_addr)[0];
    agent_haddr[1] = ((unsigned char *) &origin_node.lnd_addr.sin_addr)[1];
    agent_haddr[2] = ((unsigned char *) &origin_node.lnd_addr.sin_addr)[2];
    agent_haddr[3] = ((unsigned char *) &origin_node.lnd_addr.sin_addr)[3];
  }

  /* Get our node ID.  Same assumption as above -- if we get -1 back
     (i.e., this environment variable is not set), then assume that
     we're node ID (SLURM_NNODES+1) */

  node_id = lam_ssi_boot_slurm_get_nodeid(NULL);
  if (node_id == -1)
      return LAMERROR;
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "slurm: found SLURM_NODEID=%d", node_id);

  /* get task count for this node */

  tasks_per_node = getenv("SLURM_TASKS_PER_NODE");
  if (tasks_per_node == NULL) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                   "not-found", "SLURM_TASKS_PER_NODE", NULL);
    return LAMERROR;
  }
  begptr = tasks_per_node;
  base = 0;
  while (begptr) {
    cnt = strtol(begptr, &endptr, 10);
    if ((endptr[0] == '(') && (endptr[1] == 'x')) {
      reps = strtol((endptr+2), &endptr, 10);
      if (endptr[0] == ')')
        endptr++;
    } else
      reps = 1;
    if ((node_id >= base) && (node_id < (base + reps))) {
      task_cnt = cnt;
      break;
    }
    if (*endptr == ',')
      begptr = endptr + 1;
    else if (*endptr == '\0')
      break;
    else {
      show_help_file("lam-ssi-boot-slurm-helpfile", "env-var",
                     "bad-value", "SLURM_TASKS_PER_NODE", 
                     tasks_per_node, NULL);
      return LAMERROR;
    }
  }
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "slurm: found SLURM_TASKS_ON_NODE=%d", 
              task_cnt);

  /* Invoke the function to do the work */

  return lam_ssi_boot_base_send_lamd_info_args(aod, agent_haddr, -1,
                                               node_id, dli_port, task_cnt);
}

/*
 * Start up lamds on remote nodes, and do the startup protocols
 */
static int 
start_lamds(struct lamnode *nodes, int nnodes, int *num_started)
{
  int i, ret = 0, boot_cnt = 0;
  int argc = 0;
  char **argv = 0;
  char *inet_buf;
  LIST *list_psc;
  struct psc *p;

  /* Compute how many nodes we should use.  Used to compare to
     determine whethere we're running the NT_WASTE case or not (see
     comments at the beginning of this file). */

  for (i = 0; i < nnodes; i++) {
    if ((nodes[i].lnd_type & NT_WASTE) == 0)
      boot_cnt++;
  }

  /* Mwah ha ha... kill whatever was there */

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "slurm: launching wipe on %d nodes", nnodes);
  ret = lam_ssi_boot_slurm_start_rte_procs(nodes, nnodes, 
                                           LAM_SSI_BOOT_PROC_WIPE, 
                                           num_started);
  if (ret != 0 || *num_started != nnodes)
    return LAMERROR;
  *num_started = 0;

  /* Open up the server connection.  Have to do this before building
     the inet topo so that it can create the string of the server's
     port number that it's listening on. */

  if (lam_ssi_boot.lsba_open_srv_connection(nodes, nnodes) != 0) {
    show_help("boot", "server-socket-fail", NULL);
    return LAMERROR;
  }

  /* Build inet topo.  We knowingly create only create the inet topo
     for node 0 -- the only thing that this affects is the "-n X"
     option that is passed to the lamd executables, specifying the
     target's node ID.  In this slurm boot module, we don't use the
     command line to figure out the node ID -- we can essentially just
     get it from slurm.  This allows us to use a vector-based launch
     method to launch the lamd's in parallel. */

  inet_buf = lam_ssi_boot_build_inet_topo(&(nodes[0]), origin_node, origin);
  if (inet_buf == NULL) {
    lam_ssi_boot.lsba_close_srv_connection();
    return LAMERROR;
  }

  /* get list of things we should do */

  if (hbootparse(lam_ssi_boot_did, lam_ssi_boot_optd,  
                 inet_buf, NULL, &list_psc)) {
    show_help("hboot", "cant-parse-config", NULL);
    exit(1);
  }

  /* Loop over all the processes that we need to start on the remote
     nodes, starting them on all nodes simultaneously (usually it's
     just starting the lamd's on N nodes) */

  for (p = al_top(list_psc); p; p = al_next(list_psc, p)) {

    /* build up the argv array */

    argv = sfh_argv_dup(p->psc_argv);
    argc = p->psc_argc;

    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm: starting %s", argv[0]);
    ret += srun_procs(argv, argc, nodes, boot_cnt, 
                      (boot_cnt < nnodes) ? 1 : 0, 0);
    if (ret != 0) {
      *num_started = 0;
      return LAMERROR;
    }
  }

  /* Now receive all the lamd info */

  for (i = 0; i < nnodes; ++i) {
    if (lam_ssi_boot_verbose >= 10)
      lam_debug(lam_ssi_boot_did, "slurm: waiting for lamd info from %d of %d",
                (i + 1), nnodes);
    if (lam_ssi_boot.lsba_receive_lamd_info(nodes, nnodes) != 0)
      break;
    if (lam_ssi_boot_verbose >= 10)
      lam_debug(lam_ssi_boot_did, "slurm: got lamd info from %d of %d",
                (i + 1), nnodes);
  }
  if (lam_ssi_boot_verbose >= 10)
    lam_debug(lam_ssi_boot_did, "slurm: got all lamd info");

  /* Close down the server connection (ignore errors) */

  lam_ssi_boot.lsba_close_srv_connection();

  /* Finally, send out all the universe info */

  if (i < nnodes) {
    ret = LAMERROR;
  } else {
    ret = 0;
    if (lam_ssi_boot_verbose >= 10)
      lam_debug(lam_ssi_boot_did, "slurm: sending universe info");
    for (i = 0; i < nnodes; ++i) {

      /* Skip nodes that are invalid or already booted */

      if ((nodes[i].lnd_nodeid == NOTNODEID) ||
	  (nodes[i].lnd_type & NT_BOOT) == 0)
	continue;

      if (lam_ssi_boot.lsba_send_universe_info(nodes, nnodes, i) != 0) {
        ret = LAMERROR;
      }

      /* This one successfully booted */

      nodes[i].lnd_type &= ~NT_BOOT;
    }
  }

  /* All done */

  if (lam_ssi_boot_verbose >= 0) {
    if (boot_cnt < nnodes) {
      lam_debug(lam_ssi_boot_did, 
                "slurm: launched on %d SLURM nodes and local node", boot_cnt);
    } else {
      lam_debug(lam_ssi_boot_did, 
                "slurm: launched on %d SLURM nodes", boot_cnt);
    }
  }

  *num_started = boot_cnt;
  al_free(list_psc);
  return ret;
}


/*
 * Run an application (argv) on a set of nodes.  Use the srun command
 * to launch it in parallel.  Wait for the command to finish or not,
 * depending on the value of the wait flag.
 */
static int
srun_procs(char **argv, int argc, struct lamnode *nodes, int boot_cnt, 
           int want_local_launch, int want_wait)
{
  int i, status;
  int ret = 0;
  char **srun_argv = NULL;
  int srun_argc = 0;
  char temp[32];
  pid_t pid[2], p;
  int pipe_fd[2];
  int am_shepherd = 0;

  /* This is fairly complicated.  :-( If we don't want to wait, *and*
     we have a local launch, then we need to do exta stuff.

     This is to catch the case where we're in the NT_WASTE case and
     SLURM kills the job prematurely (i.e., the user doesn't run
     lamhalt).  In this case, SLURM will kill the srun, but the
     locally-launched lamd will no longer be visible to SLURM because
     it's a child of init.  Hence, the locally-launched lamd will keep
     running after the SLURM job is killed.  This is clearly Bad.

     To fix this (only when we a) have a locally-launched process and
     b) didn't wait for it to complete), we fork off a separate
     shepherd process that watches for the death of the srun process.
     When the srun process dies, it's time to kill the other
     process. */

  /* So if we want a shepherd, then the parent does very little */

  if (want_local_launch == 1 && want_wait == 0) {
    if (pipe(pipe_fd) == -1) {
      show_help(NULL, "system-call-fail", "pipe", NULL);
      return LAMERROR;
    }
    p = fork();
    if (p == -1) {
      show_help(NULL, "system-call-fail", "fork", NULL);
      return LAMERROR;
    } else if (p == 0) {
      /* Shepherd process */
      close(pipe_fd[0]);
      am_shepherd = 1;
    } else {
      /* Parent process; wait for the shepherd to indicate that things
         launched successfully or not */
      close(pipe_fd[1]);
      ret = -1;
      read(pipe_fd[0], &ret, sizeof(int));
      close(pipe_fd[0]);
      return ret;
    }
  }

  /* Start a new argv array with the proper srun commands at the
     front.  NOTE: We make the blind assumption that we want to start
     nnodes procs starting contiguously from SLURM node indicated in
     node[0].  This is because of how the LAM boot SSI commands work
     -- it should be a safe assumption. */

  sfh_argv_add(&srun_argc, &srun_argv, "srun");

  /* "--mpi=lam" implies the task count equals the node count */

  sfh_argv_add(&srun_argc, &srun_argv, "--mpi=lam");

  snprintf(temp, sizeof(temp) - 1, "--nodes=%d", boot_cnt);
  temp[sizeof(temp) - 1] = '\0';
  sfh_argv_add(&srun_argc, &srun_argv, temp);

  /* Now fill in the rest of the argv */

  for (i = 0; i < sfh_argv_count(argv); ++i)
    sfh_argv_add(&srun_argc, &srun_argv, argv[i]);

  /* Do we need a shephard? */

  /* No need to check for lnd_type == NOTNODEID because we'll never be
     called in for lamgrow. */

  if (lam_ssi_boot_verbose >= 1) {
    if (want_local_launch == 1) {
      lam_debug(lam_ssi_boot_did, 
                "slurm:srun: launching on %d SLURM nodes and local node: %s",
                boot_cnt, sfh_argv_glue(argv, ' ', 0));
    } else {
      lam_debug(lam_ssi_boot_did, 
                "slurm:srun: launching on %d SLURM nodes: %s",
                boot_cnt, sfh_argv_glue(argv, ' ', 0));
    }
  }

  /* Launch the srun argv in a child process */

  pid[0] = fork();
  if (pid[0] == -1) {
    show_help(NULL, "system-call-fail", "fork", NULL);
    if (am_shepherd == 1) {
      ret = LAMERROR;
      write(pipe_fd[1], &ret, sizeof(int));
      close(pipe_fd[1]);
      exit(LAMERROR);
    } else {
      return LAMERROR;
    }
  } else if (pid[0] == 0) {
    if (ao_taken(lam_ssi_boot_optd, "s")) {
      close(0);
      close(1);
      close(2);
    }
    execvp(srun_argv[0], srun_argv);
    exit(errno);
  }

  /* Do we need to launch locally?  This will only happen for the case
     where we run lamboot from a node that is not part of the
     allocation (see big comment above), and this node becomes an
     NT_WASTE.  Kinda sucks, because we have no reliable way to know
     if this launched successfully or not (i.e., nothing that doesn't
     involve a race condition) because the lamd will either
     [more-or-less] immediately fail or never return.  So we just have
     to fire-n-forget, and hope for the best.  :-\ */

  if (want_local_launch == 1) {
    pid[1] = fork();
    if (pid[1] == -1) {
      show_help(NULL, "system-call-fail", "fork", NULL);
      if (am_shepherd == 1) {
        ret = LAMERROR;
        write(pipe_fd[1], &ret, sizeof(int));
        close(pipe_fd[1]);
        exit(LAMERROR);
      } else {
        return LAMERROR;
      }
    } else if (pid[1] == 0) {
      if (ao_taken(lam_ssi_boot_optd, "s")) {
        close(0);
        close(1);
        close(2);
      }
      execvp(argv[0], argv);
      exit(errno);
    }
  }

  /* Free it all up */

  sfh_argv_free(srun_argv);
  srun_argv = NULL;
  srun_argc = 0;

  /* Be verbose */

  if (ret == 0 && lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "slurm:srun: successfully launched");

  /* Do we want to wait for the child to finish? */

  if (want_wait == 1) {
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm:srun: waiting for completion");

    /* Wait for the srun to die */

    if (wait_die(pid[0], 1) == LAMERROR || 
        (want_local_launch && wait_die(pid[1], 1) == LAMERROR)) {
        return LAMERROR;
    }

    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "slurm:srun: completed");
  }

  /* This is the big long complicated case mentioned at the top of
     this function.  If we got this far, then a) we're the shepherd,
     b) everything launched successfully, and c) we need to tell the
     parent this. */

  else if (am_shepherd == 1) {

    /* Tell the parent that everything launched ok */

    ret = 0;
    write(pipe_fd[1], &ret, sizeof(int));
    close(pipe_fd[1]);

    /* Wait for srun to die, regardless of its exit status.  Ignore
       the return of wait_die() because whatever happened, if srun
       died -- or if some error happened and we can't monitor srun
       anymore, then we need to kill the locally-launched process. */

    wait_die(pid[0], 0);
      
    /* srun is dead, so kill the locally launched process.  There
       *could* be a race condition between when the srun dies and when
       the locally-launched process dies naturally (e.g., if lamhalt
       was used).  So wait a small amount of time before doing
       anything. */

    sleep(1);
    ret = kill(pid[1], SIGTERM);
    if (ret == -1 && errno == ESRCH) {
      /* The locally-launched process is already dead.  We can die
         in peace. */

      exit(0);
    }
      
    /* The process is still alive, and we just signalled it.  See if
       it dies quickly */

    for (i = 0; i < 5; ++i) {
      ret = waitpid(pid[1], &status, WNOHANG);
      if (ret == pid[1]) {
        /* The locally-launched process died, so we can die in
           peace */

        exit (0);
      }
      sleep(1);
    }

    /* It didn't die.  So kill with extreme prejudice */
    
    kill(pid[1], SIGKILL);
    exit(0);
  }

  /* All done */

  return 0;
}


/* getnodename - equivalent to gethostname, but return only the first 
 * component of the fully qualified name 
 * (e.g. "linux123.foo.bar" becomes "linux123") 
 */
static int
getnodename(char *name, size_t len)
{
  int error_code, name_len;
  char *dot_ptr, path_name[BUFSIZ];

  error_code = gethostname(path_name, sizeof(path_name));
  if (error_code) {
    show_help(NULL, "lib-call-fail", "gethostname", NULL);
    return error_code;
  }

  dot_ptr = strchr(path_name, '.');
  if (dot_ptr == NULL)
    dot_ptr = path_name + strlen(path_name);
  else
    dot_ptr[0] = '\0';
  
  name_len = (dot_ptr - path_name);
  if (name_len > len) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "hostname",
                   "too-long", name, NULL);
    return ENAMETOOLONG;
  }
  
  strcpy(name, path_name);
  return 0;
}


/*
 * Wait for a specific process to die
 */
static int 
wait_die(pid_t pid, int check_return_status)
{
  int status;
  int ret;

  /* Wait for the process to die, looping if we get non-fatally
     signalled */

  while (1) {
    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
      if (errno == EINTR)
        continue;

      show_help(NULL, "system-call-fail", "waitpid", NULL);
      return LAMERROR;
    }

    if (ret == pid)
      break;
  }

  /* Check the return status */

  if (check_return_status == 1 && 
      (!WIFEXITED(status) || WEXITSTATUS(status) != 0)) {
    show_help_file("lam-ssi-boot-slurm-helpfile", "process",
                   "bad-exit", NULL);
    return LAMERROR;
  }

  /* All done */

  return 0;
}
