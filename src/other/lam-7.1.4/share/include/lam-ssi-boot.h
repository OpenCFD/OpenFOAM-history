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
 * $Id: lam-ssi-boot.h,v 1.8 2004/03/06 21:17:31 jsquyres Exp $
 *
 * Interface for the boot module for the LAM/MPI SSI project.
 */

#ifndef LAM_SSI_BOOT_H
#define LAM_SSI_BOOT_H


#include <lam_config.h>

#include <lam-ssi.h>
#include <lamnet.h>
#include <all_opt.h>
#include <all_list.h>


/*
 * Global functions for SSI overall boot open and close
 */

typedef enum {
  LAM_SSI_BOOT_LOCATION_ROOT,
  LAM_SSI_BOOT_LOCATION_INTERIOR,
  LAM_SSI_BOOT_LOCATION_LEAF,

  LAM_SSI_BOOT_LOCATION_MAX
} lam_ssi_boot_location_t;


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int lam_ssi_boot_open(OPT *aod);
  int lam_ssi_boot_select(lam_ssi_boot_location_t where);
  int lam_ssi_boot_close(void);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif


/***********************************************************************
 *
 * Initialization / finalization
 *
 * Setup the module and decide if it will run or not.
 *
 **********************************************************************/

typedef const struct lam_ssi_boot_actions_1_1_0 *
  (*lam_ssi_boot_init_fn_t)(lam_ssi_boot_location_t where, int *priority);
typedef int (*lam_ssi_boot_finalize_fn_t)(void);


/***********************************************************************
 *
 * User interface
 *
 * What lamboot and others call directly.  See the SSI boot docs for
 * descriptions of these functions.
 *
 **********************************************************************/

typedef enum {
  LAM_SSI_BOOT_PROC_LAMD,
  LAM_SSI_BOOT_PROC_RECON,
  LAM_SSI_BOOT_PROC_WIPE,

  LAM_SSI_BOOT_PROC_MAX
} lam_ssi_boot_proc_t;

typedef int (*lam_ssi_boot_parse_options_fn_t)(OPT *aod, int boot_schema_args);
typedef int (*lam_ssi_boot_allocate_nodes_fn_t)
     (struct lamnode **nodes, int *nnodes, int *origin);
typedef int (*lam_ssi_boot_verify_nodes_fn_t)
     (struct lamnode *nodes, int nnodes);
typedef int (*lam_ssi_boot_prepare_boot_fn_t)(void);
typedef int (*lam_ssi_boot_start_rte_procs_fn_t)
     (struct lamnode *nodes, int nnodes, lam_ssi_boot_proc_t which,
      int *num_started);
typedef int (*lam_ssi_boot_deallocate_nodes_fn_t)
     (struct lamnode **nodes, int *nnodes);


/***********************************************************************
 *
 * Internal interface
 *
 * Things that should never be called outside of the boot SSI modules.
 * These functions are mainly used by the booting algorithms.
 *
 **********************************************************************/

typedef int (*lam_ssi_boot_start_application_fn_t)
     (char ***argv, int *argc, int num_procs, struct lamnode *node);
typedef int (*lam_ssi_boot_start_rte_proc_fn_t)
     (struct lamnode *node, lam_ssi_boot_proc_t which);


/***********************************************************************
 *
 * Startup protocol interface
 *
 * Mainly for starting lamds.  Send information about the
 * newly-started lamd, and receive information about the universe.
 *
 **********************************************************************/

typedef int (*lam_ssi_boot_get_nodeid_fn_t)(OPT *aod);
typedef int (*lam_ssi_boot_open_srv_connection_fn_t)
     (struct lamnode *nodes, int nnodes);
typedef int (*lam_ssi_boot_send_lamd_info_fn_t)(OPT *aod, int dli_port);
typedef int (*lam_ssi_boot_receive_lamd_info_fn_t)
     (struct lamnode *nodes, int nnodes);
typedef int (*lam_ssi_boot_close_srv_connection_fn_t)(void);
typedef int (*lam_ssi_boot_send_universe_info_fn_t)
     (struct lamnode *nodes, int nnodes, int which);
typedef int (*lam_ssi_boot_receive_universe_info_t)
     (struct lamnode **universe, int *universe_size);

/***********************************************************************
 *
 * End of API function typedefs
 *
 **********************************************************************/


/*
 * Algorithm functions provided to all boot SSI modules
 */

typedef int (*lam_ssi_boot_alg_fn_t)
     (struct lamnode *nodes, int nnodes, int want_startup_protocol,
      lam_ssi_boot_proc_t which, int *num_started);

int lam_ssi_boot_base_alg_linear(struct lamnode *nodes, int nnodes, 
				 int want_startup_protocol,
				 lam_ssi_boot_proc_t which,
				 int *num_started);
int lam_ssi_boot_base_alg_linear_windowed(struct lamnode *nodes, int nnodes, 
                                          int want_startup_protocol,
                                          lam_ssi_boot_proc_t which,
                                          int *num_started);
int lam_ssi_boot_base_alg_thread(struct lamnode *nodes, int nnodes, 
				 int want_startup_protocol,
				 lam_ssi_boot_proc_t which,
				 int *num_started);
int lam_ssi_boot_base_alg_tree(struct lamnode *nodes, int nnodes, 
			       int want_startup_protocol,
			       lam_ssi_boot_proc_t which,
			       int *num_started);


/* 
 * Startup protocol functions based on TCP sockets; any boot SSI
 * module can use these
 */

int lam_ssi_boot_base_get_nodeid(OPT *aod);
int lam_ssi_boot_base_open_srv_connection(struct lamnode *nodes, int nnodes);
int lam_ssi_boot_base_send_lamd_info(OPT *aod, int dli_port);
int lam_ssi_boot_base_send_lamd_info_args(OPT *aod,
                                          unsigned char agent_haddr[4],
                                          int agent_port, int node_id, 
                                          int dli_port, int num_cpus);
int lam_ssi_boot_base_receive_lamd_info(struct lamnode *nodes, int nnodes);
int lam_ssi_boot_base_close_srv_connection(void);

int lam_ssi_boot_base_send_universe_info(struct lamnode *nodes,
					 int nnodes, int which);
int lam_ssi_boot_base_receive_universe_info(struct lamnode **universe,
					    int *universe_size);


/*
 * Utiltity functions for use in boot SSI modules
 */

int lam_ssi_boot_base_append_path(char *prefix, char *dir, char *fname,
				  int *argcnt, char ***arglist);

char *lam_ssi_boot_base_find_boot_schema(OPT *aod);
char *lam_ssi_boot_base_find_hostname(OPT *aod);
char *lam_ssi_boot_base_find_prefix(LIST *keyval);
char *lam_ssi_boot_build_inet_topo(struct lamnode *dest_node, 
				   struct lamnode origin_node,
				   int origin);
int lam_ssi_boot_do_common_args(OPT *aod, int *argc, char ***argv);
int lam_ssi_boot_base_check_priority(char *module_name, int base, 
				     int want_default, int *priority);
int lam_ssi_boot_base_ioexecvp(char **cmdv, int showout, char *outbuff,
			       int outbuffsize, int stderr_is_err);
char *lam_ssi_boot_base_lamgrow(char *hostname, struct lamnode **nodes,
                                int *nnodes, int *origin);


/*
 * Struct used to pass boot SSI information from the each SSI instance
 * back to the SSI glue.
 */

typedef struct lam_ssi_boot_1_1_0 {
  lam_ssi_1_0_0_t lsb_meta_info;

  /* Initialize / finalize functions */

  lam_ssi_boot_init_fn_t lsb_init;
  lam_ssi_boot_finalize_fn_t lsb_finalize;
} lam_ssi_boot_1_1_0_t;


typedef struct lam_ssi_boot_actions_1_1_0 {

  /* Boot API function pointers */

  lam_ssi_boot_parse_options_fn_t lsba_parse_options;
  lam_ssi_boot_allocate_nodes_fn_t lsba_allocate_nodes;
  lam_ssi_boot_verify_nodes_fn_t lsba_verify_nodes;
  lam_ssi_boot_prepare_boot_fn_t lsba_prepare_boot;
  lam_ssi_boot_start_rte_procs_fn_t lsba_start_rte_procs;
  lam_ssi_boot_deallocate_nodes_fn_t lsba_deallocate_nodes;

  lam_ssi_boot_start_application_fn_t lsba_start_application;
  lam_ssi_boot_start_rte_proc_fn_t lsba_start_rte_proc;

  lam_ssi_boot_get_nodeid_fn_t lsba_get_nodeid;
  lam_ssi_boot_open_srv_connection_fn_t lsba_open_srv_connection;
  lam_ssi_boot_send_lamd_info_fn_t lsba_send_lamd_info;
  lam_ssi_boot_receive_lamd_info_fn_t lsba_receive_lamd_info;
  lam_ssi_boot_close_srv_connection_fn_t lsba_close_srv_connection;

  lam_ssi_boot_send_universe_info_fn_t lsba_send_universe_info;
  lam_ssi_boot_receive_universe_info_t lsba_receive_universe_info;

} lam_ssi_boot_actions_1_1_0_t;


/*
 * Set the default type to use version 1.1.0 of the SSI boot struct 
 */

typedef lam_ssi_boot_1_1_0_t lam_ssi_boot_t;
typedef lam_ssi_boot_actions_1_1_0_t lam_ssi_boot_actions_t;


/*
 * Global variables
 */

extern int lam_ssi_boot_verbose;
extern int lam_ssi_boot_did;
extern int lam_ssi_boot_base_server_port;
extern lam_ssi_boot_actions_t lam_ssi_boot;
extern lam_ssi_boot_t lam_ssi_boot_base_module;
extern LIST *lam_ssi_boot_base_opened;
extern OPT *lam_ssi_boot_optd;
extern int lam_ssi_boot_base_param_promisc;
extern int lam_ssi_boot_base_param_window_size;


/*
 * Global instance of array of pointers to lam_ssi_boot_t.  Will
 * effectively be filled in by configure.
 */

extern const lam_ssi_t **lam_ssi_boot_modules;


#endif /* #ifndef LAM_SSI_BOOT_H */
