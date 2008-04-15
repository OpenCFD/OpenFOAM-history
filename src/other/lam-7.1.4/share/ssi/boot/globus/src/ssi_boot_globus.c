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
 * $Id: ssi_boot_globus.c,v 1.7 2004/01/02 00:11:42 jsquyres Exp $
 *
 *	Function:	- globus boot SSI module
 */

#include <lam_config.h>
#include <lam-ssi-boot-globus-config.h>

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
#include <lam-ssi-boot-globus.h>


/*
 * Access the environment
 */
extern char **environ;


/*
 * Local variables
 */
static int param_priority;
static ELEM search;
static ELEM *prefix_keyval;
static char prefix_key[] = "prefix";
static char host_key[] = "hostname";
static char contact_key[] = "contact";
static char *bhost = NULL;
static int origin = -1;
static struct lamnode origin_node;
static char *globuslocation;
static const lam_ssi_boot_actions_1_1_0_t module_actions = {

  /* Boot SSI API functions */

  lam_ssi_boot_globus_parse_options,
  lam_ssi_boot_globus_allocate_nodes,
  lam_ssi_boot_globus_verify_nodes,
  lam_ssi_boot_globus_prepare_boot,
  lam_ssi_boot_globus_start_rte_procs,
  lam_ssi_boot_globus_deallocate_nodes,

  lam_ssi_boot_globus_start_application,
  lam_ssi_boot_globus_start_rte_proc,

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
lam_ssi_boot_globus_open_module(OPT *ad)
{
  param_priority =
    lam_ssi_base_param_register_int("boot", "globus", "priority", NULL, 
                                    LAM_SSI_BOOT_GLOBUS_DEFAULT ? 75 : 3);

  return 1;
}


/*
 * Initialize the globus boot SSI
 *
 * Figure out which remote agent we'll be using.  
 */
const lam_ssi_boot_actions_t *
lam_ssi_boot_globus_init(lam_ssi_boot_location_t where, int *priority)
{
  char **pathv = NULL;
  int pathc = 0;
  char fname[] = "globus-job-run";

  /* Check this module's priority before doing anything else */
  
  *priority = lam_ssi_base_param_lookup_int(param_priority);
  if (*priority < 0)
    return NULL;
 
  /* Do we have Globus installed */

  sfh_argv_add(&pathc, &pathv, "");
  sfh_argv_add(&pathc, &pathv, "$GLOBUS_LOCATION/bin");
  globuslocation = sfh_path_findv(fname, pathv, R_OK, environ);
  sfh_argv_free(pathv);

  if (globuslocation == NULL) {
    if (lam_ssi_boot_verbose >= 5) 
      lam_debug(lam_ssi_boot_did,
		"globus: globus-job-run not found, globus boot will not run"); 
    *priority = -1;
    return NULL;
  }
    
  /* If we're verbose, print stuff out */
    
  if (lam_ssi_boot_verbose >= 5) {
    lam_debug(lam_ssi_boot_did, "globus: module initializing");
    lam_debug(lam_ssi_boot_did, "globus:verbose: %d", 
	      lam_ssi_boot_verbose);
    lam_debug(lam_ssi_boot_did, "globus:priority: %d", *priority);
    lam_debug(lam_ssi_boot_did, "globus:GLOBUS_LOCATION: %s", globuslocation);
  }

  /* Return the struct with the function pointers in it for all the
     API functions */

  return &module_actions;

}


/*
 * Parse command line options
 *
 * The only thing we need here is the boot schema filename.  Use a
 * common boot ssi utility function for this (since just about all
 * boot SSI's will have to do this).
 *
 * lam_ssi_boot_base_find_boot_schema() will print any relevant errors.
 */
int 
lam_ssi_boot_globus_parse_options(OPT *aod, int boot_schema_args)
{
  /* If we're looking for a boot schema, then use the base function
     for that.  If we get NULL for either of these, return an error.
     Otherwise, return 0. */

  if (boot_schema_args == 1)
    /* JMS: Do we need to free() bhost later? */
    return ((bhost = lam_ssi_boot_base_find_boot_schema(aod)) == NULL);
  else {
    /* VPS: show help for lamgrow since it does not work */
    show_help_file("lam-ssi-boot-globus-helpfile", "boot", "no-lamgrow");
    return 1;
  }
}
  

/*
 * Read the hostfile and generate an array of (struct lamnode)s, one
 * for each entry in the file.  This is also a common utility
 * function, so the meat of the work is not here.
 */
int 
lam_ssi_boot_globus_allocate_nodes(struct lamnode **nodes, int *nnodes,
				   int *origin_arg)
{
  int i;
  struct lamnode *n = NULL;
  char *forslash;
  char *colon;
  char *listhost;
  ELEM *host_keyval;
  ELEM keyvalue;
  char *prefix = NULL;

  /* If we got a bhost above, parse the system file */

  if (bhost != NULL) {
    if (bhostparse(bhost, nodes, nnodes) != 0)
      return 1;
    n = *nodes;
  }

  /* Print it out if our verbosity is high enough */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did,
	      "globus: found the following hosts:");
  }

  for (i = 0; i < *nnodes; i++) {

    if (n[i].lnd_nodeid != NOTNODEID) {

      /* get the prefix if there, else throw error */
      /*
	search.key = prefix_key;
	prefix_keyval = al_find(n[i].lnd_keyval, &search);

	if (prefix_keyval != NULL) {
      */
      
      prefix = lam_ssi_boot_base_find_prefix(n[i].lnd_keyval);

      if (lam_ssi_boot_verbose >= 1) 
	lam_debug(lam_ssi_boot_did,
		  "globus:   n%d %s (cpu=%d) (prefix=%s)", 
		  i, n[i].lnd_hname, n[i].lnd_ncpus, 
		  (prefix ? prefix : "NULL"));

      if (!prefix) {
	show_help_file("lam-ssi-boot-globus-helpfile", "boot",
			 "no-prefix", "The boot SSI globus module",
			 bhost, n[i].lnd_hname);
	return LAMERROR;
      }
    
      /* look if we got contact string other than hostname format */
	
      search.key = host_key;
      host_keyval = al_find(n[i].lnd_keyval, &search);
      listhost = host_keyval->value;
      forslash = strchr(listhost, '/');
      colon = strchr(listhost, ':');
	
      if ((colon != NULL) || (forslash != NULL)) {
	  
	/* we got a contact string format */

	keyvalue.key = strdup(contact_key);
	if (keyvalue.key == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(errno);
	}
	  
	keyvalue.value = strdup(listhost);
	if (keyvalue.value == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(errno);
	}
	
	/* insert the contact string into the list */

	al_insert(n[i].lnd_keyval, &keyvalue);
          
	/* get the actual hostname from the contact string */

	if ((colon != NULL) && (forslash != NULL )) {
	
	  if (colon < forslash) 
	    *colon = '\0';
	  else
	    *forslash = '\0';

	} else if ((colon == NULL) && (forslash != NULL)) {
	  *forslash = '\0';
	} else if ((forslash == NULL) && (colon != NULL)) {
	  *colon = '\0';
	} 
	      
	keyvalue.key = strdup(host_key);
	if (keyvalue.key == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(errno);
	}

	keyvalue.value = strdup(listhost);
	if (keyvalue.value == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(errno);
	}

	/* correct the host name in lamnode */

	free(n[i].lnd_hname);
	n[i].lnd_hname = strdup(listhost);
	if (n[i].lnd_hname == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(errno);
	}

	/* delete the previous host entry from the list */

	free(host_keyval->key);
	free(host_keyval->value);
	al_delete(n[i].lnd_keyval, host_keyval);

	/* insert the hostname into the list */

	al_insert(n[i].lnd_keyval, &keyvalue);

      }
    } else {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, "globus:   n%d invalid node", i);
    }
  }
  
  /* Resolve the host nodes.  Errors will be printed by the functions
     themselves */

  if (lamnet_findhosts(*nodes, *nnodes, &i) != 0)
    return LAMERROR;

  /* Ensure that we have an origin node; set it in the lamnet struct */

  *origin_arg = origin = lamnet_findorig(*nodes, *nnodes);
  if (origin < 0) {
    show_help("boot", "no-localhost", "The boot SSI globus module",
	      bhost, NULL);
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
lam_ssi_boot_globus_verify_nodes(struct lamnode *nodes, int nnodes)
{
  int i;

  /* Print out if our verbosity is high enough */

  if (lam_ssi_boot_verbose >= 1) {
    lam_debug(lam_ssi_boot_did, "globus: resolved hosts:");
    for (i = 0; i < nnodes; ++i) {
      if (nodes[i].lnd_nodeid != NOTNODEID)
        lam_debug(lam_ssi_boot_did, "globus:   n%d %s --> %s%s", 
                  i, nodes[i].lnd_hname, inet_ntoa(nodes[i].lnd_addr.sin_addr),
                  (i == origin) ? " (origin)" : "");
      else
        lam_debug(lam_ssi_boot_did, "globus:   n%d invalid node", i);
    }
  }

  /* If there is more than one node in the universe -- and we are not
     using the -l option -- ensure that the origin node does not
     resolve to 127.x.y.z, or the remote nodes will NOT be sent info
     back to lamboot */

  if (nnodes > 1 && !ao_taken(lam_ssi_boot_optd, "l")) {
    if (inet_netof(nodes[origin].lnd_addr.sin_addr) == 127) {
      show_help("boot-ssi", "resolve-localhost",
		"The boot SSI globus module", nodes[origin].lnd_hname,
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
lam_ssi_boot_globus_prepare_boot(void)
{
  /* Right now, we don't need to do anything */

  return 0;
}


/*
 * Start a run-time environment processes on a set of remote nodes
 */
int 
lam_ssi_boot_globus_start_rte_procs(struct lamnode *nodes, int nnodes,
				    lam_ssi_boot_proc_t which,
				    int *num_started)
{
  int ret;
  
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "globus: starting RTE procs");
  ret = lam_ssi_boot_base_alg_linear(nodes, nnodes, 
				     (which == LAM_SSI_BOOT_PROC_LAMD),
				     which, num_started);
  if (lam_ssi_boot_verbose >= 1 && ret == 0 && *num_started == nnodes)
    lam_debug(lam_ssi_boot_did, "globus: all RTE procs started");

  return ret;
}


/*
 * Any preliminary shutdown actions
 */
int 
lam_ssi_boot_globus_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  lam_deallocate_nodes(nodes, nnodes);
  return 0;
}


/*
 * Shut down the globus boot module
 */
int 
lam_ssi_boot_globus_finalize(void)
{
  /* Don't need to do anything */

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "globus: finalizing");

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
 * Called by either globus_start_rte_proc() (below) to start a run-time
 * environment process on a remote node, or by the booting algorithm
 * to start a helper application on a remote node.  
 */
int 
lam_ssi_boot_globus_start_application(char ***argv, int *argc, 
				      int num_procs, struct lamnode *node)
{
  int i;
  int ret = 0;
  int num_started = 0;
  char *tmp;

  if (lam_ssi_boot_verbose >= 1) {
    tmp = sfh_argv_glue(*argv, ' ', 0);
    lam_debug(lam_ssi_boot_did, "globus: starting on n%d (%s): %s",
	      node->lnd_nodeid, node->lnd_hname, tmp);
    free(tmp);
  }

  /* Launch the processs */

  for (i = 0; ret == 0 && i < num_procs; ++i) {
    if (lam_ssi_boot_verbose >= 1) {
	lam_debug(lam_ssi_boot_did, "globus: launching on n%d (%s)",
	      node->lnd_nodeid, node->lnd_hname);
    }

    /* Using globus-job-run to start jobs both locally and remotely
       so that it is part of the Globus environment */

    ret = lam_ssi_boot_globus_inetexec(node, argv[i]);
    
    if (ret != 0)
      break;
    ++num_started;
  }
    
  /* Be verbose */
    
  if (ret == 0 && num_started == num_procs && lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, 
	      "globus: successfully launched on n%d (%s)",
	      node->lnd_nodeid, node->lnd_hname);

  return num_started;
}


/*
 * Start up a given RTE proc on a target node.  This function
 * essentially seeds the argc/argv and then calls
 * globus_start_application() (above).
 */
int 
lam_ssi_boot_globus_start_rte_proc(struct lamnode *node, 
				   lam_ssi_boot_proc_t which)
{
  int ret = 0;
  int argc = 0;
  char **argv = 0;
  int is_local = (node->lnd_type & NT_ME) != 0;
  char *sep = "\"";
  char arg_buffer[BUFSIZ];
  char *inet_buf = NULL;
  char *session_suffix = NULL;
  char *prefix;
  
  search.key = prefix_key;
  prefix_keyval = al_find(node->lnd_keyval, &search);
  prefix = prefix_keyval->value;

  /* Setup argc/argv */

  sfh_argv_add(&argc, &argv, globuslocation);

  switch (which) {
  case LAM_SSI_BOOT_PROC_LAMD:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "globus: starting lamd on (%s)",
		node->lnd_hname);

    /* build the hboot path */

    ret = lam_ssi_boot_base_append_path(prefix, "bin", "hboot", &argc, &argv);
    if (ret == LAMERROR)
      return ret;
    
    sfh_argv_add(&argc, &argv, "-t");

    /* If we specify a conf file, use that.  Otherwise, use the
       default lam-conf.lamd */

    sfh_argv_add(&argc, &argv, "-c");
    if (ao_taken(lam_ssi_boot_optd, "c"))
      sfh_argv_add(&argc, &argv, ao_param(lam_ssi_boot_optd, "c", 0, 0));
    else {
      ret = lam_ssi_boot_base_append_path(prefix, "etc", "lam-conf.lamd", 
					  &argc, &argv);
      if (ret == LAMERROR)
	return ret;
    }

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);

    /* Push out the environment variables for the session dir */

    if ((session_suffix = lam_get_tmpdir_suffix()) != NULL) {
      sfh_argv_add(&argc, &argv, "-sessionsuffix");
      sfh_argv_add(&argc, &argv, session_suffix);
    }

    /* If remote node, close stdio of processes. Leave local open,
       unless forced by the command line with -s (see fl_close, above) */
    
    if (!is_local || ao_taken(lam_ssi_boot_optd, "s"))
      sfh_argv_add(&argc, &argv, "-s");

    /* Build inet topo */

    inet_buf = lam_ssi_boot_build_inet_topo(node, origin_node, origin);
    if (inet_buf == NULL) {
      fprintf(stderr, "Could not form inet_topo\n");
      perror("boot globus");
      return LAMERROR;
    }

    sfh_argv_add(&argc, &argv, "-I");
    snprintf(arg_buffer, BUFSIZ, "%s%s%s",
	     sep,
	     inet_buf,
	     sep);

    free(inet_buf);
    sfh_argv_add(&argc, &argv, arg_buffer);

    /* Add prefix to locate executables on remote nodes */

    sfh_argv_add(&argc, &argv, "-prefix");
    sfh_argv_add(&argc, &argv, prefix);
    break;

  case LAM_SSI_BOOT_PROC_RECON:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "globus: starting recon on (%s)",
		node->lnd_hname);

    ret = lam_ssi_boot_base_append_path(prefix, "bin", DEFTTKILL, 
					&argc, &argv);
    if (ret == LAMERROR)
      return ret;
    
    sfh_argv_add(&argc, &argv, "-N");

    /* Get session suffix */
    lam_tmpdir_init(NULL, NULL);
    if ((session_suffix = lam_get_tmpdir_suffix()) != NULL) {
      sfh_argv_add(&argc, &argv, "-sessionsuffix");
      sfh_argv_add(&argc, &argv, session_suffix);
    }

    /* Common params */

    lam_ssi_boot_do_common_args(lam_ssi_boot_optd, &argc, &argv);
    break;

  case LAM_SSI_BOOT_PROC_WIPE:
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "globus: starting wipe on (%s)",
		node->lnd_hname);

    ret = lam_ssi_boot_base_append_path(prefix, "bin", DEFTTKILL, 
					&argc, &argv);
    if (ret == LAMERROR)
      return ret;

    /* Get session suffix */
    lam_tmpdir_init(NULL, NULL);
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
    ret = lam_ssi_boot_globus_start_application(&argv, &argc, 1, node);
    if (ret == 1)
      ret = 0;
    else
      ret = LAMERROR;
    sfh_argv_free(argv);
  }

  return ret;
}
