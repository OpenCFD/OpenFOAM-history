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
 * $Id: ssi_boot_bproc.c,v 1.25 2003/07/11 19:40:25 jsquyres Exp $
 *
 *	Function:	- bproc boot SSI module
 */

#include <lam_config.h>
#include <lam-ssi-boot-bproc-config.h>

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
#include <sys/bproc.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#include <ssi_boot_bproc_lamnode.h>
#include <lamnet.h>
#include <args.h>
#include <net.h>
#include <sfh.h>
#include <lamdebug.h>
#include <boot.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <lam-ssi-boot-bproc.h>
#include <proc_schema.h>
#include <all_list.h>


/*
 * Local variables
 */
static int param_priority;
static int param_perm_check;
static int do_perms_checking;
static char *bhost = NULL;
static char *hostname = NULL;
static int origin = -1;
static struct lamnode origin_node;
static int num_alive_pids = 0;
static int using_bjs = 0;

static const lam_ssi_boot_actions_1_1_0_t module_actions = {

  lam_ssi_boot_bproc_parse_options,
  lam_ssi_boot_bproc_allocate_nodes,
  lam_ssi_boot_bproc_verify_nodes,
  lam_ssi_boot_bproc_prepare_boot,
  lam_ssi_boot_bproc_start_rte_procs,
  lam_ssi_boot_bproc_deallocate_nodes,

  NULL,
  lam_ssi_boot_bproc_start_rte_proc,

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
 * Local functions
 */
static int alg_vector(struct lamnode *nodes, int nnodes, 
                      lam_ssi_boot_proc_t which, int *num_started);
static int start_app_many_nodes(char ***argv, int *argc, 
                                int num_procs, int *nodes, int nnodes, 
                                int want_wait, int *num_nodes_started);
static int local_bproc_resolve(char *name);
static char *nodelist_string(int *nodes, int nnodes);


/*
 * External variables
 */
extern char **environ;


int 
lam_ssi_boot_bproc_open_module(OPT *ad)
{
  param_priority =
    lam_ssi_base_param_register_int("boot", "bproc", "priority", NULL, 
                                    LAM_SSI_BOOT_BPROC_DEFAULT ? 75 : 50);

  param_perm_check = 
    lam_ssi_base_param_register_int("boot", "bproc", "perm_check", NULL,
                                    LAM_HAVE_BPROC_NODEINFO);

  return 1;
}


/***********************************************************************
 *
 * External interface
 *
 ***********************************************************************/

/*
 * lam_ssi_boot_bproc_init
 *
 * DESCRIPTION:
 *  Initialize the module.  Any work to allow other steps to complete
 *  is done here (ie, initialize the BPROC interface)
 *
 * RETURNS:
 *  0        - on success
 *  LAMERROR - on failure
 *
 */
const lam_ssi_boot_actions_t *
lam_ssi_boot_bproc_init(lam_ssi_boot_location_t where, int *priority)
{
  struct bproc_version_t vers;
  int ret;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  do_perms_checking = lam_ssi_base_param_lookup_int(param_perm_check);

  /* Fire up BPROC interface -- but only on the root! */

  if (where == LAM_SSI_BOOT_LOCATION_ROOT) {
    ret = bproc_version(&vers);
    if (ret != 0) {
      if (lam_ssi_boot_verbose >= 5)
	lam_debug(lam_ssi_boot_did, "bproc: bproc_version() failed");
      return NULL;
    }

    /* If we're not on the master, forget it */

    if (bproc_currnode() != BPROC_NODE_MASTER) {
      if (lam_ssi_boot_verbose >= 5)
	lam_debug(lam_ssi_boot_did, "bproc: not on BPROC_NODE_MASTER");
      return NULL;
    }
  }

  /* The default priority is 50, meaning that it will usually win out
     over rsh.  However, this can be changed by compile time flags or
     by env variables. */

  /* If we're verbose, print stuff out */

  if (lam_ssi_boot_verbose >= 5) {
    lam_debug(lam_ssi_boot_did, "bproc: module initializing");
    lam_debug(lam_ssi_boot_did, "bproc:verbose: %d", 
	      lam_ssi_boot_verbose);
    lam_debug(lam_ssi_boot_did, "bproc:priority: %d", *priority);
  }

  /* Return the struct with the function pointers in it for all the
     API functions */

  return &module_actions;
}


/*
 * lam_ssi_boot_bproc_parse_options
 *
 * DESCRIPTION:
 *  Parse options
 *
 * RETURNS:
 *  0   - success
 *  LAMERROR - failure
 *
 */
int 
lam_ssi_boot_bproc_parse_options(OPT *aod, int boot_schema_args)
{
  /* If we're looking for a boot schema, then use the base function
     for that.  If we get NULL for either of these, return an error.
     Otherwise, return 0. */

  if (boot_schema_args == 1)
    return ((bhost = lam_ssi_boot_base_find_boot_schema(aod)) == NULL);
  else
    return ((hostname = lam_ssi_boot_base_find_hostname(aod)) == NULL);
}


/*
 * lam_ssi_boot_bproc_allocate_nodes
 *
 * DESCRIPTION:
 *  Create and fill in a lamnode structure for each node to be booted.
 *  Most of the work happens in lam_ssi_boot_bproc_bhostparse, which is a
 *  lot like the bhostparse() used in the BPROC boot style, but slightly
 *  different :)
 *
 * RETURNS:
 *  0        - success
 *  LAMERROR - failure
 */
int 
lam_ssi_boot_bproc_allocate_nodes(struct lamnode **nodes, int *nnodes,
				  int *origin_arg)
{
  int i;
  struct lamnode *n;
  ELEM *keyval;
  ELEM search;

  /* If we got a bhost above, parse the system file */

  if (bhost != NULL) {
    if (bhostparse(bhost, nodes, nnodes) != 0)
      return 1;
    n = *nodes;
  }

  /* Otherwise, build up the list of nodes based on the current
     LAM universe */

  else {
    lam_ssi_boot_base_lamgrow(hostname, nodes, nnodes, origin_arg);
    n = *nodes;
  }

  /* Print it out if our verbosity is high enough */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "bproc: found the following hosts:");
    for (i = 0; i < *nnodes; i++) {
      if (n[i].lnd_type != NOTNODEID)
        lam_debug(lam_ssi_boot_did, "bproc:   n%d %s (cpu=%d)", 
                  i, n[i].lnd_hname, n[i].lnd_ncpus);
      else
        lam_debug(lam_ssi_boot_did, "bproc:   n%d invalid node", i);
    }
  }

  /* Resolve the host nodes.  Errors will be printed by the functions
     themselves */

  if (lamnet_findhosts(*nodes, *nnodes, &i) != 0)
    return LAMERROR;

  /* Ensure that we have an origin node; set it in the lamnet struct.
     This can only happen when we have a bhost file; otherwise we
     already are in a LAM universe that, by definition, must have an
     origin.  */

  *origin_arg = origin = lamnet_findorig(*nodes, *nnodes);
  if (origin < 0) {
    show_help("boot", "no-localhost", "The boot SSI bproc module", 
              bhost, NULL);
    return LAMERROR;
  }
  n[origin].lnd_type |= NT_ORIGIN | NT_ME;

  /* If there's no "schedule" key, or if there's a "schedule" key and
     it's "no", then mark the origin as non-scheduleable.  Otherwise,
     leave the node as schedulable. */

  search.key = "schedule";
  keyval = al_find(n[origin].lnd_keyval, &search);
  if (keyval == NULL ||
      strcasecmp(keyval->value, "no") == 0)
    n[origin].lnd_type |= NT_WASTE;

  /* Save a copy for use down below -- we'll need to know the origin's
     IP address */

  origin_node = n[origin];

  return 0;
}


/*
 * lam_ssi_boot_bproc_verify_nodes
 *
 * DESCRIPTION:
 *  do nothing - this doesn't make sense in the current context.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_bproc_verify_nodes(struct lamnode *nodes, int nnodes)
{
  int i;
  int target_node;
  int node_downc = 0;
  char **node_downv = 0;
  int node_permc = 0;
  char **node_permv = 0;
  struct bproc_node_info_t ninfo;
  uid_t uid;
  gid_t gid;
  char *tmp;
#if ! LAM_HAVE_BPROC3_API
  char node_status[100];
  int ret;
#endif

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "bproc: resolved hosts:");
    for (i = 0; i < nnodes; ++i) {
      if (nodes[i].lnd_nodeid != NOTNODEID)
        lam_debug(lam_ssi_boot_did, "bproc:   n%d %s --> %s%s", 
                  i, nodes[i].lnd_hname, inet_ntoa(nodes[i].lnd_addr.sin_addr),
                  (i == origin) ? " (origin)" : "");
      else
        lam_debug(lam_ssi_boot_did, "bproc:   n%d invalid node", i);
    }
  }

  /* get uid and gid to make sure we have the 'Perms' ( no not the hairdoo ) to
   * run on the nodes */

  uid = getuid();
  gid = getgid();

  /* Call bproc_node_status() on each node and ensure that they're up.
     This isn't a perfect check (i.e., there's still a time window
     where a node could go down after this check but before we try to
     launch on it), but it's probably good enough for most real
     situations.  If any nodes are down, try to construct a helpful
     help message by listing *all* nodes that were down, not just the
     first one that we found. */

  for (i = 0; i < nnodes; ++i) {
    if (nodes[i].lnd_nodeid == NOTNODEID)
      continue;

    target_node = local_bproc_resolve(nodes[i].lnd_hname);
    if (target_node == BPROC_NODE_NONE) {
      /* bproc gets really unhappy if NODE_NONE is used in the
         status calls - make sure that doesn't happen */
      lam_debug(lam_ssi_boot_did, 
                "bproc: %s was not resolveable to node number\n",
                nodes[i].lnd_hname);
      sfh_argv_add(&node_downc, &node_downv, nodes[i].lnd_hname);
      continue;
    } else if (target_node == BPROC_NODE_MASTER) {
      /* master node is up and owned by all if we got this far.
         Some of the bproc calls don't like NODE_MASTER, so keep
	 it away from those functions*/
      lam_debug(lam_ssi_boot_did,
                "bproc: found master node (%s).  Skipping checks.\n",
		nodes[i].lnd_hname);
      continue;
    }
   

#if LAM_HAVE_BPROC3_API
    if (bproc_nodestatus(target_node) != bproc_node_up) {
      sfh_argv_add(&node_downc, &node_downv, nodes[i].lnd_hname);
    }
#else
    ret = bproc_nodestatus(target_node, node_status, 100);
    node_status[99] = '\0';
    if (ret < 0) {
      lam_debug(lam_ssi_boot_did, "bproc: n%d nodestatus failed (%s)\n",
                target_node, strerror(errno));
    } else {
      lam_debug(lam_ssi_boot_did, "bproc: n%d node status: %s\n",
                target_node, node_status);
    }
    if ((ret < 0) || (strcmp(node_status, "down") == 0)) {
      lam_debug(lam_ssi_boot_did, "bproc: n%d node status down, failure\n",
                target_node);
      sfh_argv_add(&node_downc, &node_downv, nodes[i].lnd_hname);
    }
#endif

    else if (do_perms_checking) {
      /* BWB - debugging - think bproc_nodeinfo is going insane.  Let's
         see if this helps with the errno */
      errno = 0;
      if (bproc_nodeinfo(target_node, &ninfo) < 0) {
        lam_debug(lam_ssi_boot_did, "bproc: n%d bproc_nodeinfo failed (%s)\n",
                  target_node, strerror(errno));
        sfh_argv_add(&node_downc, &node_downv, nodes[i].lnd_hname);
        continue;
      }

      /* make sure user, group and mode are ok dokay.  To make this
         test simple, look for any "good" condition, and simply see if
         we have the opposite of that. */

      if (! (((ninfo.user == uid) || (ninfo.mode & S_IXUSR) != 0) ||
             ((ninfo.group == gid) || (ninfo.mode & S_IXGRP) != 0) ||
             (ninfo.mode & S_IXOTH) != 0)) { 
        lam_debug(lam_ssi_boot_did, "bproc: n%d access not sane\n",
                  target_node);
        sfh_argv_add(&node_permc, &node_permv, nodes[i].lnd_hname);
        continue;
      }

      /* Woo!  We passed permissions checks. */
      lam_debug(lam_ssi_boot_did, "bproc: n%d access rights are sane.\n",
                target_node);
    } else {
      lam_debug(lam_ssi_boot_did, "bproc: n%d access rights not checked.\n",
                target_node);
    }
  }

  if (node_downc > 0) {
    tmp = sfh_argv_glue(node_downv, '\n', 0);
    show_help_file("lam-ssi-boot-bproc-helpfile",
		   "verify-nodes", "nodes-not-up", tmp, NULL);
    sfh_argv_free(node_downv);
    free(tmp);
  }

  /* show nodes that you do not own as well -- user gets all error
     messages at once -- avoids that annoying case where you fix one
     error only to get another one when after you fix it */

  if (node_permc > 0){
    tmp = sfh_argv_glue(node_permv, '\n', 0);
    show_help_file("lam-ssi-boot-bproc-helpfile", "verify-nodes", 
                   "no-permission", tmp, NULL);
    sfh_argv_free(node_permv);
    free(tmp);
  }

  return node_downc + node_permc;
}


/*
 * lam_ssi_boot_bproc_prepare_boot
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
lam_ssi_boot_bproc_prepare_boot(void)
{
  return 0;
}


/*
 * lam_ssi_boot_bproc_start_rte_procs
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
lam_ssi_boot_bproc_start_rte_procs(struct lamnode *nodes, int nnodes,
                                   lam_ssi_boot_proc_t which,
                                   int *num_started)
{
  int i, expected, ret;

  for (expected = i = 0; i < nnodes; ++i)
    if ((nodes[i].lnd_type & NT_BOOT) != 0)
      ++expected;

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "bproc: starting RTE procs");

  /* If we are booting the lamd, we have to use a linear windowed
     algorithm */

  if (which == LAM_SSI_BOOT_PROC_LAMD) {

    if (NULL != getenv("NODES")) using_bjs = 1;

    /* First run wipe.  alg_vector does not modify the NT_BOOT flags,
       so this is safe to do. */

    ret = alg_vector(nodes, nnodes, LAM_SSI_BOOT_PROC_WIPE, num_started);
    if (ret == LAMERROR || *num_started != expected)
      return LAMERROR;

    /* Now run the lamds */

    if (using_bjs == 1) {
        pid_t pid = fork();

        int pd[2];

        pipe(pd);

        if (pid < 0) {
            return pid;
        } else if (pid == 0) {
            int val, status;

            setsid();

            close(pd[0]);
            ret = lam_ssi_boot_base_alg_linear_windowed(nodes, nnodes, 1,
                                                        which, num_started);
            val = ret;
            write(pd[1], &val, sizeof(int));
            val = *num_started;
            write(pd[1], &val, sizeof(int));

            num_alive_pids += *num_started;

            while (0 != num_alive_pids) {
                pid = wait(&status);
                if (pid < 0) {
                    num_alive_pids = 0;
                } else if (pid > 0) {
                    num_alive_pids--;
                }
            }

        } else {
            int val;

            close(pd[1]);
            ret = read(pd[0], &val, sizeof(int));
            if (ret != 0) return LAMERROR;
            ret = val;
            ret = read(pd[0], &val, sizeof(int));
            if (ret != 0) return LAMERROR;
            *num_started = val;
        }
    } else {
        ret = lam_ssi_boot_base_alg_linear_windowed(nodes, nnodes, 1,
                                                    which, num_started);
    }
  }

  /* Otherwise, we can use the bproc vectorized algorithm */

  else {
    ret = alg_vector(nodes, nnodes, which, num_started);
  }

  if (lam_ssi_boot_verbose >= 1 && ret == 0 && *num_started == expected)
    lam_debug(lam_ssi_boot_did, "bproc: all RTE procs started");

  return ret;
}


/*
 * lam_ssi_boot_bproc_deallocate_nodes
 *
 * DESCRIPTION:
 *  Clean up any resources from the lamnode structs that we may need
 *  to clean up.
 *
 * RETURNS:
 *  0 - success
 */
int 
lam_ssi_boot_bproc_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  lam_deallocate_nodes(nodes, nnodes);
  return 0;
}


/*
 * Upcall from the linear window algorithm.  This function will *only*
 * be used to start the lamd on a single node.  
 */
int 
lam_ssi_boot_bproc_start_rte_proc(struct lamnode *node,
                                  lam_ssi_boot_proc_t which)
{
  int i, ret = 0;
  LIST *list_psc;
  char *inet_buf;
  struct psc *p;
  int num_started;
  int bproc_node_num;

  if (lam_ssi_boot_verbose >= 0)
    lam_debug(lam_ssi_boot_did, "bproc: starting lamd on (%s)",
              node->lnd_hname);

  /* Build inet topo */

  inet_buf = lam_ssi_boot_build_inet_topo(node, origin_node, origin);
  if (inet_buf == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    return LAMERROR;
  }

  /* get list of things we should do */

  if (hbootparse(lam_ssi_boot_did, lam_ssi_boot_optd,  
                 inet_buf, NULL, &list_psc)) {
    show_help("hboot", "cant-parse-config", NULL);
    exit(1);
  }

  /* Convert the LIST that we got back from hbootparse() into the
     vector of argv and corresponding vector of argc */

  bproc_node_num = local_bproc_resolve(node->lnd_hname);
  for (i = 0, p = al_top(list_psc); p; ++i, p = al_next(list_psc, p)) {
    ret = start_app_many_nodes(&p->psc_argv, &p->psc_argc, 1,
                               &bproc_node_num, 1, 0, &num_started);
    if (ret != 0 || num_started != 1) {
      /* Need to free more here */
      al_free(list_psc);
      return LAMERROR;
    }
  }

  /* Free everything */

  /* Need to free more here */
  al_free(list_psc);
  return ret;
}


/*
 * lam_ssi_boot_bproc_finalize
 *
 * DESCRIPTION:
 *  Finalize use of module.  Close down the BPROC interface.
 *
 * RETURNS:
 *  0        - success
 *  non-zero - failure (caused by BPROC)
 */
int 
lam_ssi_boot_bproc_finalize(void)
{
  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "bproc: finalizing");

  return 0;
}


/*
 * Internal vector boot.  Will not be used to boot lamds, because
 * lamds can't be vector launched (because each one needs a different
 * argv).
 *
 * Returns: 0 on success, LAMERROR on failure.
 */
static int
alg_vector(struct lamnode *nodes, int nnodes, 
           lam_ssi_boot_proc_t which, int *num_started)
{
  int i;
  int ret = 0;
  int *bproc_nodes = 0;
  int bproc_nnodes;
  char *nodelist;
  int argc = 0;
  char **argv = NULL;
  const char *prefix = "bproc:vector:";

  *num_started = 0;

  if ((bproc_nodes = malloc(nnodes * sizeof(int))) == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    return LAMERROR;
  }

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "%s starting", prefix);

  /* Build nodes to boot -- only the ones with NT_BOOT */

  for (bproc_nnodes = i = 0; i < nnodes; ++i)
    if ((nodes[i].lnd_type & NT_BOOT) != 0)
      bproc_nodes[bproc_nnodes++] = local_bproc_resolve(nodes[i].lnd_hname);
    
  /* Announce */

  if (lam_ssi_boot_verbose >= 0) {
    nodelist = nodelist_string(bproc_nodes, bproc_nnodes);
    lam_debug(lam_ssi_boot_did, "%s launching on nodes %s", 
              prefix, nodelist);
  }
  
  /* Build the argc/argv of the RTE procs to start.  Remember that
     this is RECON and WIPE only -- LAMD will never be started from
     here. */

  switch (which) {
  case LAM_SSI_BOOT_PROC_RECON:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "%s starting recon on %s", 
                prefix, nodelist);

    sfh_argv_add(&argc, &argv, DEFTTKILL);
    sfh_argv_add(&argc, &argv, "-N");

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);
    break;

  case LAM_SSI_BOOT_PROC_WIPE:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did,"%s starting wipe on %s", 
                prefix, nodelist);

    sfh_argv_add(&argc, &argv, DEFTTKILL);

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc,&argv);
    break;

  default:
    show_help("ssi-boot", "start-unknown-rte-proc", NULL);
    ret = LAMERROR;
    break;
  }
  if (argc > 0) {
    if (start_app_many_nodes(&argv, &argc, 1, bproc_nodes, bproc_nnodes, 
                             1, num_started) != 0)
      ret = LAMERROR;
  }

  /* All done */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "%s started %d nodes when %d were needed\n", 
              prefix, *num_started, nnodes);
    lam_debug(lam_ssi_boot_did, "%s %s", prefix, 
              (ret == 0) ? "finished" : "aborted!");
  }

  return ret;
}


/* Josh: This function only needed if bproc_vexecmove() isn't native */
#ifndef HAVE_BPROC_VEXECMOVE
/*
 * bproc_vexecmove
 *
 * DESCRIPTION:
 *  Josh: Scyld fork of BProc was based on 3.1.9, therefore it doesn't
 *  have bproc_vexecmove(), so we simulate it by using
 *  bproc_execmove() and manually iterating through the vector ourselves
 */
static void
bproc_vexecmove(int nnodes, int *nodes, int *pid, const char *cmd,
                char * const argv[], char * const envp[])
{
	const char 	*prefix = "bproc:vexecmove:";
	int			i;
	int			thisnode;

	/* don't need bproc if executing on the node we're already on */
	thisnode = bproc_currnode();
	for (i = 0; i < nnodes; i ++)
	{
		/* create each child process */
		pid[i] = fork();
		if (0 == pid[i])
		{
			/* on child */
			if (lam_ssi_boot_verbose >= 1)
			{
				/* log success only if verbose turned on */
				lam_debug(lam_ssi_boot_did, "%s index %d, node %d, child about to exec %s\n",
				 prefix, i, nodes[i], cmd);
			}
			
			/* choose either local or remote exec */
			if (nodes[i] != thisnode && nodes[i] != BPROC_NODE_SELF)
			{
				/* different node, use bproc to reach it */
				bproc_execmove(nodes[i], cmd, argv, envp);
			}
			else
			{
				/* same node, use ordinary execve() instead of bproc */
				execve(cmd, argv, envp);
			}
			
			/* error, should not be here anymore */
			lam_debug(lam_ssi_boot_did, "%s index %d, node %d, child FAILED exec of %s: %s\n",
			 prefix, i, nodes[i], cmd, strerror(errno));

			/* nothing more the child can do */
			exit(LAMERROR);
		}
		else if (pid[i] != -1)
		{
			/* on parent */
			if (lam_ssi_boot_verbose >= 1)
			{
				/* log success only if verbose turned on */
				lam_debug(lam_ssi_boot_did,
				 "%s index %d, node %d, parent did fork of child as pid %d\n",
				 prefix, i, nodes[i], pid[i]);
			}
		}
		else
		{
			/* fork failed, still on parent */
			lam_debug(lam_ssi_boot_did, "%s index %d, node %d, parent FAILED to fork: %s\n",
			 prefix, i, nodes[i], strerror(errno));
		}
	}
}
#endif /* HAVE_BPROC_VEXECMOVE */


/*
 * Use bproc_vexecmove to loop over starting each app on a vector of
 * nodes
 *
 * Returns: 0 on success, LAMERROR on failure
 */
static int 
start_app_many_nodes(char ***argv, int *argc, int num_procs, 
                     int *nodes, int nnodes, int want_wait, 
                     int *num_nodes_started)
{
  int i, j;
  char *full, *save;
  int *pid;
  int status, num_failed_procs;
  char *nodelist;
  char *tmp;
  int errno_save;
  const char *prefix = "bproc:";
  int iter_num_nodes_started;

  /* Alloc out array of pids for the children */
  
  pid = malloc(sizeof(int) * nnodes);
  if (pid == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    return LAMERROR;
  }

  nodelist = nodelist_string(nodes, nnodes);

  *num_nodes_started = 0;
  for (num_failed_procs = i = 0 ; 
       num_failed_procs == 0 && i < num_procs ; ++i) {

    /* we need the full path name to be able to bproc_spawn() */

    full = sfh_path_env_find(argv[i][0], R_OK | X_OK);
    if (full == NULL) {
      fprintf(stderr, "Can't find executable for %s\n", argv[i][0]);
      return LAMERROR;
    }

    if (lam_ssi_boot_verbose >= 1) {
      tmp = sfh_argv_glue(argv[i], ' ', 0);
      lam_debug(lam_ssi_boot_did, "%s execmoving %s to %s",
                prefix, tmp, nodelist);
      free(tmp);
    }

    /* use bproc_vexecmove so we can launch in 'parallel' */

    save = argv[i][0];
    argv[i][0] = full;
    bproc_vexecmove(nnodes, nodes, pid, argv[i][0], argv[i], environ);
    argv[i][0] = save;
    iter_num_nodes_started = nnodes;

    /* Check all pids to see how each process fared */

    errno_save = errno;
    for (j = 0; j < nnodes; j++) {
      if (pid[j] <= 0) {
        ++num_failed_procs;
        --iter_num_nodes_started;
        lam_debug(lam_ssi_boot_did, 
                  "%s failed to bproc_execmove %s to node %d\n",
                  prefix, argv[i][0], nodes[j]);
        show_help(NULL, "lib-call-fail", "bproc_execmove", NULL);
      }
    }
    
    /* Do we want to wait for everyone to die?  Do this even though we
       may have had failures above -- it's social to wait for everyone
       that we *were* able to successfully launch. */

    if (want_wait) {

      /* This is a little perverse: but it's ok to waitpid() for these
         processes in the order that they are in the array, even though
         they may not complete in that order.  This is because the child
         processes will not block waiting for their peers to be
         reaped. */

      for (j = 0; j < nnodes; ++j) {
        if (pid[j] > 0) {
          while (1) {
            if (waitpid(pid[j], &status, 0) == -1) {
              if (errno == EINTR)
                continue;
              show_help(NULL, "system-call-fail", "waitpid", NULL);
              ++num_failed_procs;
              --iter_num_nodes_started;
            } else {
              if (WIFEXITED(status) == 0) {
                show_help(NULL, "abnormal-exit", NULL);
                ++num_failed_procs;
                --iter_num_nodes_started;
              }
            }
            break;
          }
        }
      }
    }
  }

  /* Set how many nodes were successfully started */

  if (num_failed_procs == 0)
    *num_nodes_started = nnodes;
  else if (i == num_procs)
    *num_nodes_started = iter_num_nodes_started;
  else
    *num_nodes_started = 0;

  /* Be verbose */

  if (lam_ssi_boot_verbose >= 1) {
    if (num_failed_procs == 0)
      lam_debug(lam_ssi_boot_did, 
                "%s successfully launched all processes on %s", 
                prefix, nodelist);
    else
      lam_debug(lam_ssi_boot_did, 
                "%s encountered failures launching on %s", 
                prefix, nodelist);
  }

  /* All done */

  free(pid);
  return (num_failed_procs == 0) ? 0 : LAMERROR;
}


static int 
local_bproc_resolve(char *name)
{
#if LAM_HAVE_BPROC3_API
  int node;
  struct sockaddr_in s;
  struct hostent *name_resolver;

  node = bproc_getnodebyname(name);
  if (node <= BPROC_NODE_NONE) {
    if (strcmp(name, "localhost") == 0)
      node = bproc_currnode();
    else {
      name_resolver = gethostbyname(name);
      if (name_resolver == NULL)
	return BPROC_NODE_NONE;

      memcpy(&s.sin_addr, name_resolver->h_addr_list[0], 4);
      s.sin_family = AF_INET;
      node = bproc_nodenumber((struct sockaddr *) &s, sizeof(s));
    }
  } 

  return node;
#else
  struct hostent *name_resolver;
  struct bproc_node_set_t list;
  struct sockaddr_in master_in;
  int master_in_len;  
  int i, j;

  /* add in some shortcut checks */
  if(!strcmp("master",name)) {
    return BPROC_NODE_MASTER;
  } else if(!strcmp("self",name)) {
    return BPROC_NODE_SELF;
  }

  name_resolver = gethostbyname(name);
  if (name_resolver == NULL)
    return BPROC_NODE_NONE;

  /* see if we are the master */
  master_in_len = sizeof(master_in);
  if (bproc_nodeaddr(BPROC_NODE_MASTER, (struct sockaddr*) &master_in, 
                     &master_in_len) < 0) {
    perror("bproc_nodeaddr");
    return BPROC_NODE_NONE;
  }
  for (j = 0 ; name_resolver->h_addr_list[j] != NULL ; ++j) {
    if (memcmp(name_resolver->h_addr_list[j], &master_in.sin_addr, 
               name_resolver->h_length) == 0) {
      return BPROC_NODE_MASTER;
    }
  }

  if (bproc_nodelist(&list) == -1) {
    perror("bproc_nodelist");
    return BPROC_NODE_NONE;
  }

  for (j = 0 ; name_resolver->h_addr_list[j] != NULL ; ++j) {
    for (i=0; i < list.size; i++) {
      struct sockaddr_in *sin = (struct sockaddr_in *)&list.node[i].addr;
      if (memcmp(name_resolver->h_addr_list[j], &sin->sin_addr, 
                 name_resolver->h_length) == 0) {
        return list.node[i].node;
      }
    }
  }
  return BPROC_NODE_NONE;
#endif
}


/*
 * function to do some lam_debugs with pretty nodes lists
 */
static char*
nodelist_string(int *nodes, int nnodes)
{
  int i;
  char *ptr;
  static char nodelist[BUFSIZ];

  ptr = nodelist;
  ptr += sprintf(nodelist, "%d", nodes[0]);
  for (i = 1; i < nnodes; i++)
    ptr += sprintf(ptr, ",%d", nodes[i]);

  return nodelist;
}
