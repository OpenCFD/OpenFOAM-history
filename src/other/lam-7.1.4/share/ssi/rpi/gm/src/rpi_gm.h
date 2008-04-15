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
 * $Id: rpi_gm.h,v 1.21 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function:	- TCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_GM_H
#define LAM_SSI_RPI_GM_H

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>

#include <gm.h>

#include <all_hash.h>
#include <all_list.h>
#include <mpisys.h>
#include <portable.h>
#include <t_types.h>
#include <rpisys.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-rpi-gm.h>
#include <rpi_gm_proc.h>


/*
 * If we're using gm 2.x, we need to make a few changes
 */
#if (GM_API_VERSION >= 0x200)
#define LAM_SSI_RPI_GM_2 1
#else
#define LAM_SSI_RPI_GM_2 0
#endif

#if LAM_SSI_RPI_GM_HAVE_RDMA_PUT
#define LAM_GM_PUT gm_put
#else
#define LAM_GM_PUT gm_directed_send_with_callback
#endif

/* 
 * gm priority 
 */

#define LAM_SSI_RPI_GM_PRIORITY  GM_HIGH_PRIORITY

/*
 * envelope comparison
 */

#define ENVL_COMPARE(env1, env2) \
  (((env1)->ge_cid == (env2)->ge_cid) && \
    (((env1)->ge_rank == (env2)->ge_rank) || \
     ((env2)->ge_rank == MPI_ANY_SOURCE)) && \
    (((env1)->ge_tag == (env2)->ge_tag) || \
     ((env2)->ge_tag == MPI_ANY_TAG)) && \
    (((env1)->ge_flags & C2CACK) == ((env2)->ge_flags & C2CACK)) && \
    (((env1)->ge_flags & C2C2ND) == ((env2)->ge_flags & C2C2ND)))



/*
 * Convenience struct for information exchanged during connect_all
 */

typedef struct lam_ssi_rpi_gm_connect {
  int rpgc_port_id;
  int rpgc_global_node_id;
  struct _proc *rpgc_peer_proc;
} lam_ssi_rpi_gm_connect_t;

/*
 * client-to-client specific process data
 */

typedef struct lam_ssi_rpi_gm_envl {

  /* ID information for the envelope */
  /* It is *EXTREMELY* important that these members match the same
     size and ordering as (lam_ssi_rpi_envl_t), defined in <rpisys.h>
     (with the exception of the sequence number, which is not used in
     the gm RPI) because for optimization reasons, we simply cast this
     evenlpe type to the general LAM envelope type to check the
     unexpected queue (rather than creating a temporary envelpe,
     assigning all the members over, etc.) */

  int4 ge_len;
  int4 ge_tag;
  int4 ge_flags;
  int4 ge_rank;
  int4 ge_cid;

  int4 ge_counter1;
  int4 ge_counter2;

  /* To prevent a lookup loop on the target; pointer to the envelope's
     source underlying _proc on the target.  These pointers were
     exchanged during addprocs; so each proc knows its pointer on each
     of its peers.  These values never change over the duration of a
     process, even if we MPI_Comm_spawn (and therefore add procs, and
     potentially sort the RPI's array of _proc pointers). */

  struct _proc *ge_peer_proc;

  /* Information for the long message protocol */

  char *ge_target;
  MPI_Request ge_src_request;
#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
  /* This field is only used in the long protocol for gm 1.x, where we
     have to use RDMA put. */
  MPI_Request ge_dest_request;
#endif
} lam_ssi_rpi_gm_envl_t;


/* 
 * List of buffers
 */
struct lam_ssi_rpi_gm_buf_t {
  char *buffer;
  struct lam_ssi_rpi_gm_buf_t *next;
};


/*
 * global variables
 */
extern int lam_ssi_rpi_gm_did;
extern int lam_ssi_rpi_gm_haveadv;
extern int lam_ssi_rpi_gm_flblock;
extern int lam_ssi_rpi_gm_stokens;
extern int lam_ssi_rpi_gm_max_stokens;
extern int lam_ssi_rpi_gm_rtokens;
extern int lam_ssi_rpi_gm_max_rtokens;
extern int lam_ssi_rpi_gm_enqueue_sends;
extern int lam_ssi_rpi_gm_max_port_num;
extern int lam_ssi_rpi_gm_port_num;
extern int lam_ssi_rpi_gm_dma_env_len;
extern int lam_ssi_rpi_gm_dma_short_len;
extern int lam_ssi_rpi_gm_tinymsglen;
extern int lam_ssi_rpi_gm_shortmsglen;
extern int lam_ssi_rpi_gm_can_register_mem;
extern unsigned int lam_ssi_rpi_gm_env_min_size_for_length;
extern unsigned int lam_ssi_rpi_gm_data_min_size_for_length;
extern int PUB(req_mid);
extern int PUB(param_priority);
extern int PUB(param_cr);
extern int PUB(param_fast);
extern int PUB(cr_available);
extern int PUB(using_fast);

#endif	/* LAM_SSI_RPI_GM_H */
