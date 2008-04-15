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
 * $Id: rpi_ib.h,v 1.4 2003/12/27 02:59:32 vsahay Exp $
 *
 *	Function:	- IB client-to-client interface header
 */

#ifndef LAM_SSI_RPI_IB_H
#define LAM_SSI_RPI_IB_H

#include <lam_config.h>
#include <lam-ssi-rpi-ib-config.h>

/* To get rid of warnings from Mellanox stuff */
#if !defined(MAX_ERROR)
#define MAX_ERROR 0
#endif
#if !defined(MAX_TRACE)
#define MAX_TRACE 0
#endif
#if !defined(MAX_DEBUG)
#define MAX_DEBUG 0
#endif

#include <all_list.h>
#include <mpisys.h>
#include <portable.h>
#include <t_types.h>
#include <etc_misc.h>
#include <rpisys.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-rpi-ib.h>
#include <rpi_ib_proc.h>
#include <rpi_ib_bitmap.h>
#include <rpi_ib_api_map.h>


#if (LAM_SIZEOF_VOID_P == 2)
#define LAM_ADDR_T uint2

#elif (LAM_SIZEOF_VOID_P == 4)
#define LAM_ADDR_T uint4

#elif (LAM_SIZEOF_VOID_P == 8)
#define LAM_ADDR_T uint8
#endif

/* For expandable data structs due to MPI-2 dynamic processes */
#define LAM_MAXPOOL 1024

/* Constants  */
#define LAM_SSI_RPI_IB_WR_MAX 4096
#define LAM_SSI_RPI_IB_CQ_MAX 4096
#define LAM_SSI_RPI_IB_MAX_HCA 10

#define LAM_SSI_RPI_IB_ENV_SEND 1 /* completion - no */
#define LAM_SSI_RPI_IB_ENV_RECV 2 /* completion - yes  */
#define LAM_SSI_RPI_IB_FC_SEND 4 /* completion - yes  */
#define LAM_SSI_RPI_IB_FC_ADDRKEY_SEND 8 /* completion - no  */
#define LAM_SSI_RPI_IB_FC_ADDRKEY_RECV 16 /* completion - yes  */
#define LAM_SSI_RPI_IB_ENV_SYNC_SEND 32 /* completion - yes  */
#define LAM_SSI_RPI_IB_LONG_SEND 64

#define LAM_SSI_RPI_IB_R_CQ 128
#define LAM_SSI_RPI_IB_S_CQ 256

#define LAM_SSI_RPI_IB_TOTAL_CONST 15


/*
 * client-to-client specific process data
 */

typedef struct lam_ssi_rpi_ib_envl {
  struct lam_ssi_rpi_envl ge_env;
  int ge_peer_proc_id;
  char *ge_target;
  LAM_IB_rkey rkey;
  LAM_IB_lkey lkey;

  MPI_Request ge_src_request;
  
  /* VPS : I am using this for the sync send ACK-Send completion, to know
     which request has done this */
  MPI_Request ge_dest_request;

} lam_ssi_rpi_ib_envl_t;

/* Struct to store lkey and rkey for a memory region  */
typedef struct key_pair_t {
  LAM_IB_lkey lkey;
  LAM_IB_rkey rkey;
} ib_mem_keypair;

/* 
 * List of buffers
 */
struct lam_ssi_rpi_ib_buf_t {
  char *buffer;
  LAM_IB_lkey lkey;
  LAM_IB_rkey rkey;
  LAM_IB_lkey *lkey_ptr;
  LAM_IB_rkey *rkey_ptr;
  struct lam_ssi_rpi_ib_buf_t *next;
};

typedef struct PUB(fc_addr_key_t) {
  LAM_ADDR_T addr;
  LAM_IB_rkey rkey;
} PUB(fc_addr_key);


/* Global variables  */

extern lam_array_t *PUB(procs);
extern int lam_ssi_rpi_gm_did;
extern LAM_IB_ret ret;
extern int lam_ssi_rpi_ib_mtu;
extern LAM_IB_hca_id lam_ssi_rpi_ib_hca_id; 
extern LAM_IB_hca_hndl lam_ssi_rpi_ib_hca_hndl;
extern LAM_IB_pd_hndl lam_ssi_rpi_ib_pd_hndl;
extern LAM_IB_port lam_ssi_rpi_ib_port_num;
extern LAM_IB_lid lam_ssi_rpi_ib_lid;
extern LAM_IB_cq_hndl lam_ssi_rpi_ib_cntrl_cq_hndl;
extern LAM_IB_cq_hndl *lam_ssi_rpi_ib_s_cq_hndl;
extern LAM_IB_cq_hndl *lam_ssi_rpi_ib_r_cq_hndl;
extern LAM_IB_qp_hndl *lam_ssi_rpi_ib_qp_hndl;
extern LAM_IB_qp_num *lam_ssi_rpi_ib_qp_num;
extern LAM_IB_qp_attr_mask lam_ssi_rpi_ib_qp_attr_mask;
extern LAM_IB_qp_attr lam_ssi_rpi_ib_qp_attr;

extern int lam_ssi_rpi_ib_haveadv;
extern int lam_ssi_rpi_ib_flblock;
extern int lam_ssi_rpi_ib_num_envelopes;
extern int lam_ssi_rpi_ib_reg_env_len;
extern int *lam_ssi_rpi_ib_fc_remote_recv_env_cnt;
extern PUB(fc_addr_key) *PUB(fc_remote_info);
extern int *PUB(fc_post_env_cnt);
extern lam_ssi_rpi_ib_bitmap *PUB(pending_send_proc_bitmap);
extern lam_ssi_rpi_ib_bitmap *PUB(pending_ack_proc_bitmap);
extern lam_array_t *PUB(procs);
extern int PUB(param_priority);
extern int PUB(req_mid);
extern int lam_ssi_rpi_ib_did;
extern int lam_ssi_rpi_ib_tinymsglen;
extern int lam_ssi_rpi_ib_port;

/* Global functions */
void lam_ssi_rpi_ib_proc_init(struct _proc *p);

#endif
