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
 * $Id: ssi_boot_rsh.c,v 1.41 2004/03/12 14:26:10 jsquyres Exp $
 *
 *	Function:	- rsh boot SSI module
 */

#include <lam_config.h>
#include <lam-ssi-boot-rsh-config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <args.h>
#include <net.h>
#include <sfh.h>
#include <lamdebug.h>
#include <boot.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <lam-ssi-boot-rsh.h>


/*
 * global variables
 */
int lam_ssi_boot_rsh_param_no_n;
int lam_ssi_boot_rsh_param_no_profile;
int lam_ssi_boot_rsh_param_fast;
int lam_ssi_boot_rsh_param_ignore_stderr;
char **lam_ssi_boot_rsh_agent = NULL;


/*
 * local variables
 */
static int param_username;
static int param_priority;
static char *bhost = NULL;
static char *hostname = NULL;
static char *username = NULL;
static int origin = -1;
static char *lamprefix = NULL;
static struct lamnode origin_node;
static const lam_ssi_boot_actions_1_1_0_t module_actions = {

  /* Boot SSI API functions */

  lam_ssi_boot_rsh_parse_options,
  lam_ssi_boot_rsh_allocate_nodes,
  lam_ssi_boot_rsh_verify_nodes,
  lam_ssi_boot_rsh_prepare_boot,
  lam_ssi_boot_rsh_start_rte_procs,
  lam_ssi_boot_rsh_deallocate_nodes,

  lam_ssi_boot_rsh_start_application,
  lam_ssi_boot_rsh_start_rte_proc,

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
 * Open the module and register an SSI parameter
 */
int
lam_ssi_boot_rsh_open_module(OPT *ad)
{
  param_username =
    lam_ssi_base_param_register_string("boot", "rsh", "username", NULL, NULL);

  /* We don't need to keep this index because, for
     backwards-comptability reasons, we still have to use regular
     getenv() rather than the built-in lookup stuff.  But we still
     want to register this parameter so that it shows up in laminfo,
     etc. */

  lam_ssi_base_param_register_string("boot", "rsh", "agent", NULL, 
                                     LAM_SSI_BOOT_RSH_RSH);

  lam_ssi_boot_rsh_param_no_n =
    lam_ssi_base_param_register_int("boot", "rsh", "no_n", NULL, 0);
  lam_ssi_boot_rsh_param_no_profile =
    lam_ssi_base_param_register_int("boot", "rsh", "no_profile", NULL, 0);
  lam_ssi_boot_rsh_param_fast =
    lam_ssi_base_param_register_int("boot", "rsh", "fast", NULL, 0);
  lam_ssi_boot_rsh_param_ignore_stderr =
    lam_ssi_base_param_register_int("boot", "rsh", "ignore_stderr", NULL, 0);

  param_priority =
    lam_ssi_base_param_register_int("boot", "rsh", "priority", NULL, 
                                    LAM_SSI_BOOT_RSH_DEFAULT ? 75 : 10);

  return 1;
}


/*
 * Initialize the rsh boot SSI
 *
 * Figure out which remote agent we'll be using.  
 */
const lam_ssi_boot_actions_t *
lam_ssi_boot_rsh_init(lam_ssi_boot_location_t where, int *priority)
{
  char *env, *env_dup;
  int ignore_stderr, fast, no_n, no_profile;

  /* Check this module's priority before doing anything else */

  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;

  /* Look for environment variable overrides.  LAMRSH is depricated,
     but we probably can't get rid of it any time soon... */

  username = lam_ssi_base_param_lookup_string(param_username);
  fast =
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_fast);
  no_n =
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_no_n);
  no_profile =
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_no_profile);
  ignore_stderr = 
    lam_ssi_base_param_lookup_int(lam_ssi_boot_rsh_param_ignore_stderr);

  /* Stupid backwards compatability; can't use standard lookup.
     :-( */

  env = getenv("LAM_MPI_SSI_boot_rsh_agent");
  if (env == NULL)
    env = getenv("LAMRSH");
  if (env == NULL)
    env = LAM_SSI_BOOT_RSH_RSH;

  /* Split into individual terms */

  env_dup = strdup(env);
  lam_ssi_boot_rsh_agent = sfh_argv_break(env, ' ');
  free(env_dup);

  /* If we're verbose, print stuff out */

  if (lam_ssi_boot_verbose >= 5) {
    lam_debug(lam_ssi_boot_did, "rsh: module initializing");
    env = sfh_argv_glue(lam_ssi_boot_rsh_agent, ' ', 0);
    lam_debug(lam_ssi_boot_did, "rsh:agent: %s", env);
    lam_debug(lam_ssi_boot_did, "rsh:username: %s", 
	      (username != NULL) ? username : "<same>");
    lam_debug(lam_ssi_boot_did, "rsh:verbose: %d", 
	      lam_ssi_boot_verbose);
    lam_debug(lam_ssi_boot_did, "rsh:algorithm: linear");
    lam_debug(lam_ssi_boot_did, "rsh:no_n: %d", no_n);
    lam_debug(lam_ssi_boot_did, "rsh:no_profile: %d", no_profile);
    lam_debug(lam_ssi_boot_did, "rsh:fast: %d", fast);
    lam_debug(lam_ssi_boot_did, "rsh:ignore_stderr: %d", ignore_stderr);
    lam_debug(lam_ssi_boot_did, "rsh:priority: %d", *priority);
    free(env);
  }

  /* Return the struct with the function pointers in it for all the
     API functions */

  return &module_actions;
}


/*
 * Shut down the rsh boot module
 */
int 
lam_ssi_boot_rsh_finalize(void)
{
  /* Free up a few things */

  sfh_argv_free(lam_ssi_boot_rsh_agent);

  /* All done */

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "rsh: finalizing");

  return 0;
}


/*
 * Parse command line options
 *
 * We need to find if the prefix has been supplied in command line.
 * Also we need here the boot schema filename.  Use a
 * common boot ssi utility function for this (since just about all
 * boot SSI's will have to do this).
 *
 * lam_ssi_boot_base_find_boot_schema() will print any relevant errors.
 */
int 
lam_ssi_boot_rsh_parse_options(OPT *aod, int boot_schema_args)
{
  
  if (ao_taken(aod, "prefix")) 
    lamprefix = ao_param(aod, "prefix", 0, 0);
  else 
    lamprefix = NULL;
  
  /* If we're looking for a boot schema, then use the base function
     for that.  If we get NULL for either of these, return an error.
     Otherwise, return 0. */

  if (boot_schema_args == 1)
    /* JMS: Do we need to free() bhost laster? */
    return ((bhost = lam_ssi_boot_base_find_boot_schema(aod)) == NULL);
  else
    return ((hostname = lam_ssi_boot_base_find_hostname(aod)) == NULL);
}


/*
 * Read the hostfile and generate an array of (struct lamnode)s, one
 * for each entry in the file.  This is also a common utility
 * function, so the meat of the work is not here.
 */
int 
lam_ssi_boot_rsh_allocate_nodes(struct lamnode **nodes, int *nnodes,
				int *origin_arg)
{
  int i;
  struct lamnode *n;
  char *prefix = NULL;

  /* If we got a bhost above, parse the system file */

  if (bhost != NULL) {
    if (bhostparse(bhost, nodes, nnodes) != 0)
      return 1;
    n = *nodes;
  }

  /* Otherwise, build up the list of nodes based on the current LAM
     universe.  Note that passing hostname to boot_base_lamgrow will
     add it into a struct lamnode, so it will be properly free()'ed
     later. */

  else {
    lam_ssi_boot_base_lamgrow(hostname, nodes, nnodes, origin_arg);
    n = *nodes;
  }

  /* If there was an environment variable override, then swap in the
     username.  Note that "username" came from param_lookup_string,
     which is already safely strdup'ed from the original.  Hence, we
     can simply assign it to lnd_uname here, knowing that it will be
     properly free()'ed later. */

  if (username != NULL)
    for (i = 0; i < *nnodes; ++i) {
      if (n[i].lnd_uname != NULL)
	free(n[i].lnd_uname);
      n[i].lnd_uname = username;
    }

  /* Print it out if our verbosity is high enough */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "rsh: found the following hosts:");
    for (i = 0; i < *nnodes; i++) {
      if (n[i].lnd_nodeid != NOTNODEID) {
	prefix = lam_ssi_boot_base_find_prefix(n[i].lnd_keyval);

        lam_debug(lam_ssi_boot_did, "rsh:   n%d %s (cpu=%d) %s%s", 
                  i, n[i].lnd_hname, n[i].lnd_ncpus,
		  (prefix ? "prefix=" : ""),
		  (prefix ? prefix : ""));
      }
      else
        lam_debug(lam_ssi_boot_did, "rsh:   n%d invalid node", i);
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
    show_help("boot", "no-localhost", "The boot SSI rsh module", bhost, NULL);
    return LAMERROR;
  }
  n[origin].lnd_type |= NT_ORIGIN | NT_ME;

  /* Save a copy for use down below -- we'll need to know the origin's
     IP address */

  origin_node = n[origin];

  return 0;
}


/*
 * Error check the array of struct lamnodes.
 */
int 
lam_ssi_boot_rsh_verify_nodes(struct lamnode *nodes, int nnodes)
{
  int i;

  /* Print out if our verbosity is high enough */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "rsh: resolved hosts:");
    for (i = 0; i < nnodes; ++i) {
      if (nodes[i].lnd_nodeid != NOTNODEID)
        lam_debug(lam_ssi_boot_did, "rsh:   n%d %s --> %s%s", 
                  i, nodes[i].lnd_hname, inet_ntoa(nodes[i].lnd_addr.sin_addr),
                  (i == origin) ? " (origin)" : "");
      else
        lam_debug(lam_ssi_boot_did, "rsh:   n%d invalid node", i);
    }
  }

  /* If there is more than one node in the universe -- and we are not
     using the -l option -- ensure that the origin node does not
     resolve to 127.x.y.z, or the remote nodes will NOT be sent info
     back to lamboot */

  if (nnodes > 1 && !ao_taken(lam_ssi_boot_optd, "l")) {
    if (inet_netof(nodes[origin].lnd_addr.sin_addr) == 127) {
      show_help("boot-ssi", "resolve-localhost",
		"The boot SSI rsh module", nodes[origin].lnd_hname,
		inet_ntoa(nodes[origin].lnd_addr.sin_addr));
      return LAMERROR;
    }
  }

  return 0;
}


/*
 * Do anything in here that we might need to do once (since
 * boot_nodes() may be called multiple times).  
 */
int 
lam_ssi_boot_rsh_prepare_boot(void)
{
  /* Right now, we don't need to do anything */

  return 0;
}


/*
 * Start a run-time environment processes on a set of remote nodes
 */
int 
lam_ssi_boot_rsh_start_rte_procs(struct lamnode *nodes, int nnodes,
				 lam_ssi_boot_proc_t which,
				 int *num_started)
{
  int ret;

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "rsh: starting RTE procs");
  ret = lam_ssi_boot_base_alg_linear(nodes, nnodes, 
				     (which == LAM_SSI_BOOT_PROC_LAMD),
				     which, num_started);
  if (lam_ssi_boot_verbose >= 1 && ret == 0 && *num_started == nnodes)
    lam_debug(lam_ssi_boot_did, "rsh: all RTE procs started");

  return ret;
}


/*
 * Any preliminary shutdown actions
 */
int 
lam_ssi_boot_rsh_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  lam_deallocate_nodes(nodes, nnodes);
  return 0;
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
 * Called by either rsh_start_rte_proc() (below) to start a run-time
 * environment process on a remote node, or by the booting algorithm
 * to start a helper application on a remote node.  
 */
int 
lam_ssi_boot_rsh_start_application(char ***argv, int *argc, 
				   int num_procs, struct lamnode *node)
{
  int i;
  int ret = 0;
  int is_local = (node->lnd_type & NT_ME) != 0;
  int num_started = 0;
  char *tmp;

  if (lam_ssi_boot_verbose >= 1) {
    tmp = sfh_argv_glue(*argv, ' ', 0);
    lam_debug(lam_ssi_boot_did, "rsh: starting on n%d (%s): %s",
	      node->lnd_nodeid, node->lnd_hname, tmp);
    free(tmp);
  }

  /* Launch the processs */

  for (i = 0; ret == 0 && i < num_procs; ++i) {
    if (is_local) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, "rsh: launching locally");
      ret = _lam_few(argv[i]);
    } else {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, "rsh: launching remotely");
      ret = lam_ssi_boot_rsh_inetexec(node, argv[i]);
    }
    if (ret != 0)
      break;
    ++num_started;
  }

  /* Be verbose */

  if (ret == 0 && num_started == num_procs && lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, 
	      "rsh: successfully launched on n%d (%s)",
	      node->lnd_nodeid, node->lnd_hname);

  return num_started;
}


/*
 * Start up a given RTE proc on a target node.  This function
 * essentially seeds the argc/argv and then calls
 * rsh_start_application() (above).
 */

int 
lam_ssi_boot_rsh_start_rte_proc(struct lamnode *node, 
				lam_ssi_boot_proc_t which)
{
  int ret = 0;
  int argc = 0;
  char **argv = 0;
  int is_local = (node->lnd_type & NT_ME) != 0;
  char *sep = (is_local) ? "" : "\"";
  char arg_buffer[BUFSIZ];
  char *inet_buf = NULL;
  char *local_lamprefix;
#if 0
  char *session_prefix = NULL;
#endif
  char *session_suffix = NULL;
  ELEM search;
  ELEM *prefix_keyval = NULL;
  char prefix_key[] = "prefix";

  local_lamprefix = lamprefix;

  if (node->lnd_keyval != NULL) {
    search.key = prefix_key;
    prefix_keyval = al_find(node->lnd_keyval, &search);
  }
  if (prefix_keyval != NULL)
    local_lamprefix = prefix_keyval->value;

  /* Setup argc/argv */

  switch (which) {
  case LAM_SSI_BOOT_PROC_LAMD:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "rsh: starting lamd on (%s)",
		node->lnd_hname);
        /* build the hboot path */

    if (local_lamprefix) {
      ret = lam_ssi_boot_base_append_path(local_lamprefix, "bin", "hboot", 
					  &argc, &argv);
      if (ret == LAMERROR)
	return ret;
    }
    else 
      sfh_argv_add(&argc, &argv, "hboot");

    sfh_argv_add(&argc, &argv, "-t");


    /* If we specify a conf file, use that.  Otherwise, use the
       default lam-conf.lamd */

    sfh_argv_add(&argc, &argv, "-c");
    if (ao_taken(lam_ssi_boot_optd, "c"))
      sfh_argv_add(&argc, &argv, ao_param(lam_ssi_boot_optd, "c", 0, 0));
    else {
      if (local_lamprefix) {
	ret = lam_ssi_boot_base_append_path(local_lamprefix, "etc", "lam-conf.lamd", 
				      &argc, &argv);
	 if (ret == LAMERROR)
	   return ret;
      }
      else
	sfh_argv_add(&argc, &argv, "lam-conf.lamd");
    }

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);

    /* Push out the environment variables for the session dir */
#if 0
    /* Ok, so PBS doesn't create session directories on the non-origin
       nodes.  Therefore, I'm going to cheat and just not push this
       out.  Which isn't what I wanted to do (waste time adding unused
       code), but what else can I do? */
    if ((session_prefix = lam_get_tmpdir_prefix()) != NULL) {
      sfh_argv_add(&argc, &argv, "-sessionprefix");
      sfh_argv_add(&argc, &argv, session_prefix);
    }
#endif
    if ((session_suffix = lam_get_tmpdir_suffix()) != NULL) {
      sfh_argv_add(&argc, &argv, "-sessionsuffix");
      sfh_argv_add(&argc, &argv, session_suffix);
    }

    /* If remote node, close stdio of processes. Leave local open,
       unless forced by the command line with -s (see fl_close, above)
       -- this is useful, for example, in "rsh somenode lamboot
       hostfile", because we need the hboot/lamd on somenode to close
       their stdio so that rsh can finish.  */

    if (!is_local || ao_taken(lam_ssi_boot_optd, "s"))
      sfh_argv_add(&argc, &argv, "-s");

    /* Build inet topo */

    inet_buf = lam_ssi_boot_build_inet_topo(node, origin_node, origin);
    if (inet_buf == NULL) {
      fprintf(stderr, "Could not form inet_topo\n");
      perror("boot rsh");
      return LAMERROR;
    }

    if (local_lamprefix)
      sep = "\"";
    
    sfh_argv_add(&argc, &argv, "-I");
    snprintf(arg_buffer, BUFSIZ, "%s%s%s",
	     sep,
	     inet_buf,
	     sep);

    free(inet_buf);

    sfh_argv_add(&argc, &argv, arg_buffer);

    /* Add prefix to locate executables on remote nodes */
    if (local_lamprefix) {
      sfh_argv_add(&argc, &argv, "-prefix");
      sfh_argv_add(&argc, &argv, local_lamprefix);
    }

    break;

  case LAM_SSI_BOOT_PROC_RECON:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "rsh: starting recon on (%s)",
		node->lnd_hname);

    if (local_lamprefix) {
      ret = lam_ssi_boot_base_append_path(local_lamprefix, "bin", DEFTTKILL, 
					  &argc, &argv);
      if (ret == LAMERROR)
	return ret;
    }
    else
      sfh_argv_add(&argc, &argv, DEFTTKILL);
    
    sfh_argv_add(&argc, &argv, "-N");

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);
    break;

  case LAM_SSI_BOOT_PROC_WIPE:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "rsh: starting wipe on (%s)",
		node->lnd_hname);

    if (local_lamprefix) {
      ret = lam_ssi_boot_base_append_path(local_lamprefix, "bin", DEFTTKILL, 
					  &argc, &argv);
      if (ret == LAMERROR)
	return ret;
    }
    else
      sfh_argv_add(&argc, &argv, DEFTTKILL);

    if ((session_suffix = lam_get_tmpdir_suffix()) != NULL) {
      sfh_argv_add(&argc, &argv, "-sessionsuffix");
      sfh_argv_add(&argc, &argv, session_suffix);
    }

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);
    break;

  default:
    show_help("ssi-boot", "start-unknown-rte-proc", NULL);
    ret = 1;
    break;
  }

  /* If we've got a valid argc/argv, boot it (locally or remotely) */

  if (argc > 0) {
    ret = lam_ssi_boot_rsh_start_application(&argv, &argc, 1, node);
    if (ret == 1)
      ret = 0;
    else
      ret = LAMERROR;
    sfh_argv_free(argv);
  }

  return ret;
}
