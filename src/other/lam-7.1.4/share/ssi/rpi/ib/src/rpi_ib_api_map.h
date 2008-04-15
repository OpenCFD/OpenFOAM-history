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
 * $Id: rpi_ib_api_map.h,v 1.4 2003/12/27 04:32:13 vsahay Exp $
 *
 *	Function:	- IB API map
 */

#include <vapi.h>
#include <evapi.h>
#include <vapi_common.h>

#define MAX_DEBUG 0
#define MAX_TRACE 0
#define VAPI 1


#if VAPI

#define LAM_IB_LOG2_LONG 5  /* log base 2 of 32  */
#define LAM_IB_OK VAPI_OK
#define LAM_IB_SIGNAL_REQ_WR VAPI_SIGNAL_REQ_WR
#define LAM_IB_SIGNAL_ALL_WR VAPI_SIGNAL_ALL_WR
#define LAM_IB_TS_RC VAPI_TS_RC
#define LAM_IB_DEVMEM EVAPI_DEVMEM_EXT_DRAM
#define LAM_IB_MR VAPI_MR;
#define LAM_IB_EN_REMOTE_READ VAPI_EN_REMOTE_READ
#define LAM_IB_EN_REMOTE_WRITE VAPI_EN_REMOTE_WRITE
#define LAM_IB_EN_LOCAL_WRITE VAPI_EN_LOCAL_WRITE
#define LAM_IB_EN_REMOTE_ATOM VAPI_EN_REMOTE_ATOM
#define LAM_IB_SEND VAPI_SEND
#define LAM_IB_RDMA_WRITE VAPI_RDMA_WRITE
#define LAM_IB_RDMA_READ VAPI_RDMA_READ
#define LAM_IB_RECEIVE VAPI_RECEIVE
#define LAM_IB_ATOMIC_FETCH_AND_ADD VAPI_ATOMIC_FETCH_AND_ADD
#define LAM_IB_SUCCESS VAPI_SUCCESS
#define LAM_IB_SIGNALED VAPI_SIGNALED
#define LAM_IB_UNSIGNALED VAPI_UNSIGNALED
#define LAM_IB_INVAL_HNDL VAPI_INVAL_HNDL
#define LAM_IB_CQ_EMPTY VAPI_CQ_EMPTY


/* datatypes  */
#define LAM_IB_hca_id VAPI_hca_id_t
#define LAM_IB_hca_vendor VAPI_hca_vendor_t
#define LAM_IB_hca_hndl VAPI_hca_hndl_t
#define LAM_IB_hca_port VAPI_hca_port_t
#define LAM_IB_hca_cap VAPI_hca_cap_t
#define LAM_IB_port IB_port_t
#define LAM_IB_lid IB_lid_t
#define LAM_IB_pd_hndl VAPI_pd_hndl_t
#define LAM_IB_cqe_num VAPI_cqe_num_t
#define LAM_IB_cq_hndl VAPI_cq_hndl_t
#define LAM_IB_mrw VAPI_mrw_t
#define LAM_IB_mr_hndl VAPI_mr_hndl_t
#define LAM_IB_qp_init_attr VAPI_qp_init_attr_t
#define LAM_IB_qp_prop VAPI_qp_prop_t
#define LAM_IB_qp_hndl VAPI_qp_hndl_t
#define LAM_IB_qp_num VAPI_qp_num_t
#define LAM_IB_qp_attr_mask VAPI_qp_attr_mask_t
#define LAM_IB_qp_attr VAPI_qp_attr_t
#define LAM_IB_qp_cap VAPI_qp_cap_t
#define LAM_IB_sr_desc VAPI_sr_desc_t
#define LAM_IB_rr_desc VAPI_rr_desc_t
#define LAM_IB_sg_lst_entry VAPI_sg_lst_entry_t
#define LAM_IB_wc_desc VAPI_wc_desc_t
#define LAM_IB_lkey VAPI_lkey_t
#define LAM_IB_rkey VAPI_rkey_t
#define LAM_IB_virt_addr VAPI_virt_addr_t
#define LAM_IB_ret VAPI_ret_t

/* Completion queue opcodes */
#define LAM_IB_SQ_SEND_DATA VAPI_CQE_SQ_SEND_DATA
#define LAM_IB_SQ_RDMA_WRITE VAPI_CQE_SQ_RDMA_WRITE
#define LAM_IB_SQ_RDMA_READ VAPI_CQE_SQ_RDMA_READ
#define LAM_IB_RQ_SEND_DATA VAPI_CQE_RQ_SEND_DATA
#define LAM_IB_INVAL_OPCODE VAPI_CQE_INVAL_OPCODE

/* functions */

/* HCA  */
#define LAM_IB_get_hca_hndl(a, b) EVAPI_get_hca_hndl((a), (b))
#define LAM_IB_list_hcas(a, b, c) EVAPI_list_hcas((a), (b), (c))
#define LAM_IB_open_hca(a, b, c) EVAPI_open_hca((a), (b), (c))
#define LAM_IB_query_hca_cap(a, b, c) VAPI_query_hca_cap((a), (b), (c))
#define LAM_IB_query_hca_port_prop(a, b, c) \
        VAPI_query_hca_port_prop((a), (b), (c))
#define LAM_IB_alloc_pd(a, b) VAPI_alloc_pd((a), (b))
#define LAM_IB_release_hca_hndl(a) EVAPI_release_hca_hndl((a))
#define LAM_IB_dealloc_pd(a, b) VAPI_dealloc_pd((a), (b))


/* Queue  */
#define LAM_IB_create_cq(a, b, c, d) VAPI_create_cq((a), (b), (c), (d))
#define LAM_IB_create_qp(a, b, c, d) VAPI_create_qp((a), (b), (c), (d))
#define LAM_IB_modify_qp(a, b, c, d, e) VAPI_modify_qp((a), (b), (c), (d), (e))
#define LAM_IB_destroy_cq(a, b) VAPI_destroy_cq((a), (b))
#define LAM_IB_destroy_qp(a, b) VAPI_destroy_qp((a), (b))

/* Memory  */
#define LAM_IB_register_mr(a, b, c, d) VAPI_register_mr((a), (b), (c), (d))
#define LAM_IB_deregister_mr(a, b) VAPI_deregister_mr((a), (b))
#define LAM_IB_reregister_mr(a, b, c, d, e, f) \
        VAPI_reregister_mr((a), (b), (c), (d), (e), (f))
#define LAM_IB_alloc_device_mem(a, b, c, d, e, f) \
        EVAPI_alloc_map_devmem((a), (b), (c), (d), (e), (f))

/* Work request  */
#define LAM_IB_post_sr(a, b, c) VAPI_post_sr((a), (b), (c))
#define LAM_IB_post_rr(a, b, c) VAPI_post_rr((a), (b), (c))
#define LAM_IB_post_rr_list(a, b, c, d) EVAPI_post_rr_list((a), (b), (c), (d))
#define LAM_IB_poll_cq(a, b, c) VAPI_poll_cq((a), (b), (c))

/* Error  */
#define LAM_IB_err(a) VAPI_strerror((a))

/* Address */
#define LAM_IB_virt_addr VAPI_virt_addr_t

#else

#error "Error: Unknown IB API. Cannot continue"

#endif

