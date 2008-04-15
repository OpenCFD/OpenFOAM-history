/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	$Id: rpi_ib_proc.h,v 1.4 2003/12/27 04:32:13 vsahay Exp $
 *
 *	Function:	- ib proc specification
 */

#ifndef LAM_SSI_RPI_IB_PROC_H
#define LAM_SSI_RPI_IB_PROC_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <rpi_ib_req.h>
#include <mpisys.h>


struct lam_ssi_rpi_ib_prepost_track {
    struct lam_ssi_rpi_ib_envl *env;
    int freed;
};


/*
 * ib-specific MPI process data
 */

struct lam_ssi_rpi_proc {
  /* This needs to be here for the unexepcted message handling (i.e.,
     partially received unexpected messages), but cp_nbfde is not used
     in this RPI */

  struct lam_ssi_rpi_cbuf_msg *cp_bmsg;
  int cp_nbfde;

  /* point up to the main _proc so that we can do checks for things
     like LAM_PHOMOG.  This is an optimization -- in the ib RPI, we
     usually pass around just a (struct lam_ssi_rpi_proc*) because we
     do 95% of the work with just this.  But periodically, a few
     things new stuff from the upper-level proc (e.g., p_mode &
     LAM_PHOMOG), so we need to be able to find the upper-level proc
     when we only have the lam_ssi_rpi_proc. */

  struct _proc *cp_proc;

  /* count of bytes sent/received -- required to support checkpoint/restart of
     the ib RPI. */
    unsigned int cp_out_nbytes;
    unsigned int cp_in_nbytes;

  /* ib port lid to use to communicate with the peer.
     For myproc it stores the local node lid, for other procs it
     stores the other node lid got after connect_all */
  LAM_IB_lid cp_ib_lid;

  /* qp numbers for my and destination QP required for sync  */
  LAM_IB_qp_num cp_ib_my_qp_num;
  LAM_IB_qp_num cp_ib_dest_qp_num;
  
  /* handle for the QP and CQ  */
  LAM_IB_cq_hndl cp_ib_cq_hndl;
  LAM_IB_qp_hndl cp_ib_qp_hndl;

  /* remote peer addr and rkey for flow control puts */
  LAM_IB_virt_addr fc_addr;
  LAM_IB_rkey fc_rkey;

  /* locally where will I get the value of remote_env_cnt - not required for
     myproc - that is self */
  int *fc_remote_recv_env_cnt;
  LAM_IB_rkey fc_remote_recv_env_rkey;

  /* Which registered buffer will I use to send the recv_env_cnt to the peer  
     for posting the request - and the corresponding lkey */
  int *fc_post_env_cnt;
  LAM_IB_lkey fc_post_env_cnt_lkey;

  /* Flags to control the FC rdma writes  */
  int fl_fc_am_sending_rdma;
  int fl_fc_want_to_rdma_send;

  /* Number of envelopes sent till now for this proc  */
  int fc_local_sent_env_cnt;

  /* Number of receive buffers remaining in the work queue */
  int rem_recv_bufs;

  /* My index in the proc array  */
  int myindex;

  /* Queues for the pending sends and receives on this proc */
  struct lam_ssi_rpi_ib_reqlist_t cp_pending_sends;
  struct lam_ssi_rpi_ib_reqlist_t cp_pending_acks;

  struct lam_ssi_rpi_ib_reqlist_t cp_pending_recvs;

  /* Request that has been partially received (since it may take
     multiple sends to send an entire message -- but we know that the
     sender will always send them in order, with no other intervening
     sends). */

  MPI_Request cp_current_recv_req;

  /* Start address of the pre-allocated envelope buffers */
  struct lam_ssi_rpi_ib_prepost_track *preposted_envs;
  int preposted_start;
  int preposted_end;
  int prepost_index;
    
};

#endif	/* LAM_SSI_RPI_IB_PROC_H */
