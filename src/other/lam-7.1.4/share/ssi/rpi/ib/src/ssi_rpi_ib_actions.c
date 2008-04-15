/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 *      Function:       - calls to ib
 */

#include <lam_config.h>
#include <lam-ssi-rpi-ib-config.h>
#include <lam-ssi-cr.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>
#include <dl_inet.h>
#include <malloc_wrapper.h>

#include <rpi_ib.h>
#include <rpi_ib_unexpected.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_interval.h>
#include <rpi_ib_recv_events.h>
#include <rpi_ib_recv_queue.h>
#include <rpi_ib_send_queue.h>
#include <rpi_ib_tiny.h>
#include <rpi_ib_ack.h>
#include <rpi_ib_bitmap.h>
#include <rpi_ib_actions.h>
#include <rpi_ib_interval.h>
#include <rpi_ib_long.h>

/*
 * local variables
 */
static int param_tinymsglen;
static int param_port_num;
static int param_num_envelopes;
static int param_mtu;

static ib_mem_keypair fc_env_cnt_kp;
static ib_mem_keypair fc_post_env_cnt_kp;
static LAM_IB_qp_init_attr qp_init_attr;
static struct _proc **array;
static int num_procs;
static LAM_IB_rr_desc *rr_post_recv_list;
static LAM_IB_sg_lst_entry *rr_sg_entry_list;
static int *fc_remote_recv_env_cnt[LAM_MAXPOOL];
static int *fc_post_env_cnt[LAM_MAXPOOL];
static int lam_currpool = 0;


/* Local functions  */
int sync_peer(struct _proc *p);
int poll_cq(struct _proc *p);
int fc_send_completion_handler(struct lam_ssi_rpi_proc *p);
int fc_post_rdma_send(struct lam_ssi_rpi_proc *p_rpi);
int fc_post_send_recv(struct _proc *p);
int send_peer_fc_info(struct _proc *p, PUB(fc_addr_key) addr_key);


/*
 * Register SSI parameters for IB 
 */
void
lam_ssi_rpi_ib_params_register(void)
{
  param_tinymsglen = 
    lam_ssi_base_param_register_int("rpi", "ib", "tinymsglen", NULL,
                                    LAM_SSI_RPI_IB_TINYMSGLEN);

  param_port_num =
    lam_ssi_base_param_register_int("rpi", "ib", "port", NULL,
                                    LAM_SSI_RPI_IB_PORT);
  
  param_mtu = 
      lam_ssi_base_param_register_int("rpi", "ib", "mtu", NULL,
				      1024);

  PUB(param_priority) =
      lam_ssi_base_param_register_int("rpi", "ib", "priority", NULL,
				      LAM_SSI_RPI_IB_DEFAULT ? 75 : 50);
  
  param_num_envelopes =
    lam_ssi_base_param_register_int("rpi", "ib", "num_envelopes", NULL, 
				    LAM_SSI_RPI_IB_NUM_ENVELOPES);
}


/*
 * Get the port number for IB hca 
 */
int
lam_ssi_rpi_ib_get_port(void) 
{
  LAM_IB_port port_num;
  LAM_IB_hca_vendor hca_vend;
  LAM_IB_hca_cap hca_cap;
  LAM_IB_hca_port hca_port;
  
  /* Query the HCA to get the number of physical ports */

  ret = LAM_IB_query_hca_cap(lam_ssi_rpi_ib_hca_hndl, &hca_vend, &hca_cap);

  /* Check which port is active. Take the first active port. If none is active
   * then can't run IB programs -- simply exit */

  lam_ssi_rpi_ib_port = lam_ssi_base_param_lookup_int(param_port_num);
  if (lam_ssi_rpi_ib_port == -1) {
    port_num = 0;
    do {
      ++port_num;
      ret = LAM_IB_query_hca_port_prop(lam_ssi_rpi_ib_hca_hndl, 
				       port_num, &hca_port);
    } while (((ret != LAM_IB_OK) || (hca_port.lid <= 0) 
	      || (hca_port.state != PORT_ACTIVE))
	     && (port_num <= hca_cap.phys_port_num));

    if (port_num > hca_cap.phys_port_num) {
      show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "no-free-port");
      return LAMERROR;
    }
  } else {
    port_num = lam_ssi_rpi_ib_port;
    ret = LAM_IB_query_hca_port_prop(lam_ssi_rpi_ib_hca_hndl, 
				     port_num, &hca_port);


    if ((ret != LAM_IB_OK) || (hca_port.lid <= 0) 
	|| (hca_port.state != PORT_ACTIVE)) {
      
      show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "this-port-not-free",
		     port_num, NULL);
      return LAMERROR;
    }
  }

  lam_ssi_rpi_ib_lid = hca_port.lid;
  lam_ssi_rpi_ib_port_num = port_num;
  return 0;
}


/* 
 * Initialize common IB data structs and buffer pools/pinned memory structs
 */
int
lam_ssi_rpi_ib_ib_setup(void) 
{
  int i, mtu;
  char *str_mtu;
  
  /* Get the parameters  */
  PUB(num_envelopes) = lam_ssi_base_param_lookup_int(param_num_envelopes);
  lam_ssi_rpi_ib_tinymsglen = 
      lam_ssi_base_param_lookup_int(param_tinymsglen);
  mtu = lam_ssi_base_param_lookup_int(param_mtu);

  switch (mtu) {
  case 256: 
      PUB(mtu) = MTU256;
      str_mtu = "MTU256";
      break;
  case 512: 
      PUB(mtu) = MTU512;
      str_mtu = "MTU512";
      break;
  case 1024: 
      PUB(mtu) = MTU1024;
      str_mtu = "MTU1024";
      break;
  case 2048: 
      PUB(mtu) = MTU2048;
      str_mtu = "MTU2048";
      break;
  case 4096: 
      PUB(mtu) = MTU4096;
      str_mtu = "MTU4096";
      break;
  default:
      PUB(mtu) = MTU1024;
      str_mtu = "MTU1024";
  }
      
  if (lam_ssi_rpi_verbose >= 5) {
    lam_debug(lam_ssi_rpi_did, "ib:selected port: 0x%x", PUB(port_num));
    lam_debug(lam_ssi_rpi_did, "ib:lid: %d", PUB(lid));
    lam_debug(lam_ssi_rpi_did, "ib:tiny message length: %d bytes",
	      PUB(tinymsglen));
    lam_debug(lam_ssi_rpi_did, "ib:mtu used is: %s", str_mtu);
    lam_debug(lam_ssi_rpi_did, "ib:number of preposted envelopes: %d",
	      PUB(num_envelopes));
  }

  lam_ssi_rpi_ib_reg_env_len = lam_ssi_rpi_ib_tinymsglen + 
    sizeof(struct lam_ssi_rpi_ib_envl);

  /* Initialize list of recv buffer descriptors to be posted */
  rr_post_recv_list = (LAM_IB_rr_desc *) malloc (PUB(num_envelopes) *
						 sizeof(LAM_IB_rr_desc));
  
  rr_sg_entry_list = malloc(PUB(num_envelopes) * sizeof(LAM_IB_sg_lst_entry));

  for (i = 0; i < PUB(num_envelopes); ++i) {
    rr_post_recv_list[i].opcode = LAM_IB_RECEIVE;
    rr_post_recv_list[i].comp_type = LAM_IB_SIGNALED;
    rr_post_recv_list[i].sg_lst_p = &rr_sg_entry_list[i];
    rr_post_recv_list[i].sg_lst_len = 1;
    rr_sg_entry_list[i].len = lam_ssi_rpi_ib_reg_env_len;
  }

  /* One time initialize the qp attributes */
  qp_init_attr.cap.max_oust_wr_sq = LAM_SSI_RPI_IB_WR_MAX;
  qp_init_attr.cap.max_oust_wr_rq = LAM_SSI_RPI_IB_WR_MAX;
  qp_init_attr.cap.max_sg_size_sq = 1;
  qp_init_attr.cap.max_sg_size_rq = 1;
  qp_init_attr.rdd_hndl = 0;
  qp_init_attr.sq_sig_type = LAM_IB_SIGNAL_REQ_WR;
  qp_init_attr.rq_sig_type = LAM_IB_SIGNAL_REQ_WR;
  qp_init_attr.pd_hndl = lam_ssi_rpi_ib_pd_hndl;
  qp_init_attr.ts_type = LAM_IB_TS_RC;

  /* Initialize the buffer pool structs */
  lam_ssi_rpi_ib_reg_init();

  /* Initialize the pinned memory maintenance data structures */
  if (lam_ssi_rpi_ib_interval_init() == LAMERROR) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "interval-init-fail");
    return LAMERROR;
  }

  return 0;
}


/* 
 * Initialize IB related data structures for the newly added procs
 */
int
lam_ssi_rpi_ib_initialize_ib(int has_been_appended, int nprocs)
{
  int num_cqe = LAM_SSI_RPI_IB_CQ_MAX;
  int act_num_cqe;
  int i, status = 0, cnt = 0, index;
  LAM_IB_qp_prop qp_prop;

  array = lam_arr_get(PUB(procs));
  num_procs = lam_arr_size(PUB(procs));

#if 0
  /* Create pool buffers to be used for recvs and sends */
  if (lam_ssi_rpi_ib_reg_add(nprocs, lam_ssi_rpi_ib_num_envelopes) == LAMERROR)
    return LAMERROR;
#endif

  /* Initialize bitmaps for send/ack pending queues */
  if (!has_been_appended) {

      if (lam_ssi_rpi_ib_reg_add(nprocs,
				 lam_ssi_rpi_ib_num_envelopes) == LAMERROR)
	  return LAMERROR;


      PUB(pending_send_proc_bitmap) = lam_ssi_rpi_ib_bitmap_init(num_procs);
      if (PUB(pending_send_proc_bitmap) == NULL) {
	show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail",
		       "bitmap");
	return LAMERROR;
      }
      PUB(pending_ack_proc_bitmap) = lam_ssi_rpi_ib_bitmap_init(num_procs);
      if (PUB(pending_ack_proc_bitmap) == NULL) {
	show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail",
		       "bitmap");
	return LAMERROR;
      }


  } else {

    /* The proc list has been expanded (eg: by MPI-2 dynamic processe) */
    status = lam_ssi_rpi_ib_bitmap_extend(PUB(pending_send_proc_bitmap),
					  nprocs);
    if (status == LAMERROR) {
      show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail",
		     "bitmap");
      return LAMERROR;
    }
    status = lam_ssi_rpi_ib_bitmap_extend(PUB(pending_ack_proc_bitmap),
					  nprocs);
    if (status == LAMERROR) {
      show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail",
		     "bitmap");
      return LAMERROR;
    }
  }

  index = lam_currpool++;
  /* Allocate space for FC env counts from receivers */
   fc_remote_recv_env_cnt[index] = 
     (int *) malloc (nprocs * sizeof(int)); 
   

   if (fc_remote_recv_env_cnt[index] == NULL) {
     show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail", 
		    "flow-control-buffer");
    return LAMERROR;
   }

   lam_debug_cond((PUB(did), "INIT: registering FC from %x to %x of len %d",
		   fc_remote_recv_env_cnt[index],
		   fc_remote_recv_env_cnt[index] + (nprocs * sizeof(int)) - 1,
		   nprocs * sizeof(int)));

   if (lam_ssi_rpi_ib_interval_use(fc_remote_recv_env_cnt[index],
				   nprocs * sizeof(int),
				   &fc_env_cnt_kp) != 0) {
     show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "register-mem-fail");
     return LAMERROR;
   }
   
   /* Allocate space for the buffers which will be used to post the env
     cnt and receiver will directly increment this buffer */

   fc_post_env_cnt[index] = (int *) malloc (nprocs * sizeof(int));
   if (fc_post_env_cnt[index] == NULL) {
     show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "malloc-fail",
		    "flow-control-buffer");
     return LAMERROR;
   }
 
   if (lam_ssi_rpi_ib_interval_use(fc_post_env_cnt[index], 
				   nprocs * sizeof(int),
				   &fc_post_env_cnt_kp) != 0) {
     show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "register-mem-fail");
     return LAMERROR;
   }
   
  /* Hang off the things from proc */
   for (i = 0; i < num_procs; ++i) 
     if ((array[i]->p_mode & LAM_PRPIADDED) && (array[i] != lam_myproc)) {

      /* Turn off the RPIADDED indicator */
      array[i]->p_mode &= ~LAM_PRPIADDED;

      /* Activate the RPI struct in proc */
      lam_ssi_rpi_ib_proc_init(array[i]);

      /* Create a commong CQ for both send and recv completion to
	 reduce the number of queues to poll for */
      ret = LAM_IB_create_cq(lam_ssi_rpi_ib_hca_hndl, num_cqe, 
			     &(array[i]->p_rpi->cp_ib_cq_hndl),
			     &act_num_cqe);
      if (ret != LAM_IB_OK) {
	show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "cq-create-fail",
		       LAM_IB_err(ret));
	return LAMERROR;
      }

      qp_init_attr.sq_cq_hndl = array[i]->p_rpi->cp_ib_cq_hndl;
      qp_init_attr.rq_cq_hndl = array[i]->p_rpi->cp_ib_cq_hndl;

      /* Create QP  */
      ret = LAM_IB_create_qp(lam_ssi_rpi_ib_hca_hndl, &qp_init_attr, 
			     &array[i]->p_rpi->cp_ib_qp_hndl, &qp_prop);
      if (ret != LAM_IB_OK) {
	show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "qp-create-fail",
		       LAM_IB_err(ret));
	return LAMERROR;
      }
    
      /* Extract QP num to help the peer QP syncronize  */
      array[i]->p_rpi->cp_ib_my_qp_num = qp_prop.qp_num;

      /* Initialize number of buffers remaining for this proc - buffers 
	 will be posted during queue setup */
      array[i]->p_rpi->rem_recv_bufs = PUB(num_envelopes);

      /* FC related things */
      array[i]->p_rpi->fc_remote_recv_env_cnt = 
	fc_remote_recv_env_cnt[index] + cnt;

      *(array[i]->p_rpi->fc_remote_recv_env_cnt) = 0;
      array[i]->p_rpi->fc_remote_recv_env_rkey = fc_env_cnt_kp.rkey;

      array[i]->p_rpi->fc_post_env_cnt = fc_post_env_cnt[index] + cnt;
      *(array[i]->p_rpi->fc_post_env_cnt)  = 0;
      array[i]->p_rpi->fc_post_env_cnt_lkey = fc_post_env_cnt_kp.lkey;
      array[i]->p_rpi->fc_local_sent_env_cnt = 0;
      array[i]->p_rpi->fl_fc_am_sending_rdma = 0;
      array[i]->p_rpi->fl_fc_want_to_rdma_send = 0;

      /* Preposted envs array initialization */
      array[i]->p_rpi->preposted_envs = 
	  malloc (PUB(num_envelopes) *
		  sizeof(struct lam_ssi_rpi_ib_prepost_track));
      array[i]->p_rpi->preposted_start = 0;
      array[i]->p_rpi->preposted_end = PUB(num_envelopes) - 1;
      array[i]->p_rpi->prepost_index = 0;
      ++cnt;
    } 
   return 0;
}


/* 
 * Set up the work queues of the peers to connect to each other and get ready
 */
int
lam_ssi_rpi_ib_connect_peer(struct _proc *p)
{
  LAM_IB_qp_cap qp_cap;
  int i;
  
  /* First, connect all the queues for RC  */

  /* goto INIT state */

  QP_ATTR_MASK_CLR_ALL(lam_ssi_rpi_ib_qp_attr_mask);

  lam_ssi_rpi_ib_qp_attr.qp_state = VAPI_INIT;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_QP_STATE);

  lam_ssi_rpi_ib_qp_attr.pkey_ix = 0;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_PKEY_IX);

  lam_ssi_rpi_ib_qp_attr.port = lam_ssi_rpi_ib_port_num;

  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_PORT);

  PUB(qp_attr.remote_atomic_flags) = VAPI_EN_REM_WRITE | VAPI_EN_REM_READ;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_REMOTE_ATOMIC_FLAGS);


  ret = LAM_IB_modify_qp(lam_ssi_rpi_ib_hca_hndl, p->p_rpi->cp_ib_qp_hndl, 
			 &lam_ssi_rpi_ib_qp_attr, 
			 &lam_ssi_rpi_ib_qp_attr_mask, 
			 &qp_cap);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "change-q-state", 
		   LAM_IB_err(ret), "INIT");
    return LAMERROR;
  }
    
  lam_debug_cond((PUB(did), "Done INIT... "));

  /* Goto RTR state */

  QP_ATTR_MASK_CLR_ALL(lam_ssi_rpi_ib_qp_attr_mask);

  lam_ssi_rpi_ib_qp_attr.qp_state = VAPI_RTR;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_QP_STATE);

  lam_ssi_rpi_ib_qp_attr.qp_ous_rd_atom = 1;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_QP_OUS_RD_ATOM);

  lam_ssi_rpi_ib_qp_attr.dest_qp_num = p->p_rpi->cp_ib_dest_qp_num;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_DEST_QP_NUM);
  
  lam_ssi_rpi_ib_qp_attr.av.sl = 0;
  lam_ssi_rpi_ib_qp_attr.av.grh_flag = FALSE;
  lam_ssi_rpi_ib_qp_attr.av.dlid = p->p_rpi->cp_ib_lid;
  lam_ssi_rpi_ib_qp_attr.av.static_rate = 0;
  lam_ssi_rpi_ib_qp_attr.av.src_path_bits = 0;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_AV);

  lam_debug_cond((PUB(did), "proc %d dest qpnum %d dest lid %d",
		  i, p->p_rpi->cp_ib_dest_qp_num, p->p_rpi->cp_ib_lid));

  lam_ssi_rpi_ib_qp_attr.path_mtu = PUB(mtu);

  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_PATH_MTU);

  lam_ssi_rpi_ib_qp_attr.rq_psn = 1;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_RQ_PSN);

  lam_ssi_rpi_ib_qp_attr.pkey_ix = 0;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_PKEY_IX);

  lam_ssi_rpi_ib_qp_attr.min_rnr_timer = 0;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_MIN_RNR_TIMER);
  
  ret = LAM_IB_modify_qp(lam_ssi_rpi_ib_hca_hndl, p->p_rpi->cp_ib_qp_hndl, 
		       &lam_ssi_rpi_ib_qp_attr, &lam_ssi_rpi_ib_qp_attr_mask,
		       &qp_cap);
  if(ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "change-q-state",
		   LAM_IB_err(ret), "Ready To Receive (RTR)");
    return LAMERROR;
  }

  lam_debug_cond((PUB(did), "Done INIT->RTR "));

  /* 
   * Synchronize before goign to RTS state, just to make sure that the peer is
   * atleast in RTR state, so that if I am trying to send after mine RTS state,
   * the peer is not in INIT state 'cause it will drop packets then
   */

  sync_peer(p);

  lam_debug_cond((PUB(did), "Synced with peer... "));

  /* Goto RTS state */

  QP_ATTR_MASK_CLR_ALL(lam_ssi_rpi_ib_qp_attr_mask);

  lam_ssi_rpi_ib_qp_attr.qp_state = VAPI_RTS;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_QP_STATE);

  lam_ssi_rpi_ib_qp_attr.sq_psn = 1;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_SQ_PSN);

  lam_ssi_rpi_ib_qp_attr.timeout = 31;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_TIMEOUT);

  lam_ssi_rpi_ib_qp_attr.retry_count = 3;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_RETRY_COUNT);

  lam_ssi_rpi_ib_qp_attr.rnr_retry = 7;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_RNR_RETRY);

  lam_ssi_rpi_ib_qp_attr.ous_dst_rd_atom = 1;
  QP_ATTR_MASK_SET(lam_ssi_rpi_ib_qp_attr_mask, QP_ATTR_OUS_DST_RD_ATOM);

  ret = LAM_IB_modify_qp(lam_ssi_rpi_ib_hca_hndl, p->p_rpi->cp_ib_qp_hndl, 
		       &lam_ssi_rpi_ib_qp_attr, &lam_ssi_rpi_ib_qp_attr_mask, 
		       &qp_cap);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "change-q-state",
		   LAM_IB_err(ret), "Ready To Send (RTS)");
    return LAMERROR;
  }

  lam_debug_cond((PUB(did), "Done RTR->RTS ... "));


  /* Now send and recv the fc addr and rkey  */
  if (fc_post_send_recv(p) != 0)
    return LAMERROR;

  /* Pre-post a few receives in the receive queue */
  lam_ssi_rpi_ib_post_recv_buf(PUB(num_envelopes), p->p_rpi);

  /* Now sync with peer, because if the peer is fast, it may just move
     ahead and do a send before a receive buffer could be posted at
     the other end */

  sync_peer(p); 
  return 0;
}


/*
 * Prepare and send an envelope to the work queue.  We pass sync here
 * so that later on when we use "unsignalled" for non-syn sends, we can
 * detect between the two conditions
 */
int
lam_ssi_rpi_ib_push_envelope(MPI_Request req, int sync_send)
{
  struct lam_ssi_rpi_ib_envl *env = req->rq_rpi->cq_envbuf;
  char *body = req->rq_packbuf;
  int body_len = req->rq_rpi->cq_envbuf->ge_env.ce_len;
  int copy_len = sizeof(struct lam_ssi_rpi_ib_envl);
  struct lam_ssi_rpi_proc *destproc = req->rq_proc->p_rpi;
  LAM_IB_sr_desc sr;
  LAM_IB_sg_lst_entry sg_entry;
  
  env->ge_peer_proc_id = lam_myproc->p_gps.gps_grank;

  lam_debug_cond((PUB(did), 
                  "ib_push_envelope: rank %d, tag %d, cid %d, flags 0x%x, "
                  "len %d, seq %d body %p",
                  env->ge_env.ce_rank,
                  env->ge_env.ce_tag,
                  env->ge_env.ce_cid,
                  env->ge_env.ce_flags,
                  env->ge_env.ce_len,
                  env->ge_env.ce_seq,
		  body));

  if (body != NULL && body_len > 0 && body_len <= lam_ssi_rpi_ib_tinymsglen) {
    copy_len += body_len;
    lam_memcpy(env + 1, body, body_len);
  }

  /* VPS: Right now we are using SIGNALED for all send requests, but sometime
   to gain performace we need to use UNSIGNALED. Then we need to design a way
   to detect completed sends thru the FC stuff */

  /* Post the send request  */
  
  /* The id contains the buf address  */
  sr.id = (LAM_ADDR_T) env;
  sr.comp_type = LAM_IB_SIGNALED;
  sr.opcode = LAM_IB_SEND;

  sg_entry.addr = (LAM_ADDR_T) env;
  sg_entry.len = copy_len;
  sg_entry.lkey = req->rq_rpi->env_lkey;

  sr.sg_lst_p = &sg_entry;
  sr.sg_lst_len = 1;
  
  lam_debug_cond((PUB(did), "Push envelope: Dest proc lid = %d qpnum = %d"
		  "qp_hndl = %d",
		  destproc->cp_ib_lid, destproc->cp_ib_dest_qp_num, 
		  destproc->cp_ib_qp_hndl));

  ret = LAM_IB_post_sr(PUB(hca_hndl), destproc->cp_ib_qp_hndl, &sr);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "post-req-fail",
		   LAM_IB_err(ret));
    return LAMERROR;
  }

  /* Increment the local sent env count. Posting a req means eventually
     it will go thru and the HCA has control over it. So we need to 
     increment here rather than in the callback */
 
  ++destproc->fc_local_sent_env_cnt;

  return 0;
}


/*
 * Post a set of receive buffers to the receive work queue - all got from pool
 */
int
lam_ssi_rpi_ib_post_recv_buf(int num_posts, struct lam_ssi_rpi_proc *rpi)
{
  int i, j;
  LAM_IB_rr_desc *rr = rr_post_recv_list;
  ib_mem_keypair kp;
  struct  lam_ssi_rpi_ib_envl *env_from_pool;

  j = rpi->preposted_start;

  /* Build the descriptors */
  for (i = 0; i < num_posts; ++i) {
    env_from_pool = PUB(reg_env_malloc(&kp));
    rr_sg_entry_list[i].addr = (LAM_ADDR_T) env_from_pool;

    rpi->preposted_envs[j].env = env_from_pool;
    rpi->preposted_envs[j].freed = 0;
   
    /* Reset this so that recv_events can make use of it */
    rpi->preposted_start = -1;
    rpi->preposted_end = -1;

    if (rr_sg_entry_list[i].addr == (LAM_ADDR_T) NULL)
      return LAMERROR;

    rr_sg_entry_list[i].lkey = kp.lkey;
    rr[i].id = rr_sg_entry_list[i].addr;

    /* Increment or wrap around */
    ++j;
    if (j == PUB(num_envelopes))
	j = 0;
  }

  ret = LAM_IB_post_rr_list(lam_ssi_rpi_ib_hca_hndl, rpi->cp_ib_qp_hndl, 
		       num_posts, rr);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "post-req-fail",
		   LAM_IB_err(ret));
    return LAMERROR;
  }
  return 0;
}


/* 
 * Post a receive buffer to the work queue 
 */
int
lam_ssi_rpi_ib_post_returned_recv_buf(struct lam_ssi_rpi_ib_envl *env,
				      struct lam_ssi_rpi_proc *rpi)
{
  int i;
  LAM_IB_rr_desc *rr = rr_post_recv_list;

  i = 0;

  /* Build the descriptors */
  rr_sg_entry_list[i].addr = (LAM_ADDR_T) env;

  if (rr_sg_entry_list[i].addr == (LAM_ADDR_T) NULL)
      return LAMERROR;
  
  rr_sg_entry_list[i].lkey = env->lkey;
  rr[i].id = rr_sg_entry_list[i].addr;

  ret = LAM_IB_post_rr(lam_ssi_rpi_ib_hca_hndl, rpi->cp_ib_qp_hndl, 
		       &rr[i]);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "post-req-fail",
		   LAM_IB_err(ret));
    return LAMERROR;
  }
  return 0;
}

/*
 * Callback when a receive buffer at the receiving end gets available
 */
int
lam_ssi_rpi_ib_fc_recv_buffer_available(struct lam_ssi_rpi_proc *destproc)
{
  
  ++(*(destproc->fc_post_env_cnt));

  lam_debug_cond((PUB(did), "FC POST: incremented recv env to %d",
		  *(destproc->fc_post_env_cnt)));

  destproc->fl_fc_want_to_rdma_send = 1;

  if (destproc->fl_fc_am_sending_rdma == 0) {
    destproc->fl_fc_am_sending_rdma = 1;
    destproc->fl_fc_want_to_rdma_send = 0;
    if (fc_post_rdma_send(destproc) != 0)
      return LAMERROR;
  }
  return 0;
}


/*
 * Second step for a long receive
 */
int
lam_ssi_rpi_ib_get_long_body(char *raddr, LAM_IB_rkey rkey,
			     int recv_len, MPI_Request req)
			     
{
  LAM_IB_sr_desc sr;
  LAM_IB_sg_lst_entry sg_entry;

  sr.id = (LAM_ADDR_T) req;
  sr.opcode = LAM_IB_RDMA_READ;
  sr.comp_type = LAM_IB_SIGNALED;

  /* Local receive buffer details  */
  sg_entry.addr = (LAM_ADDR_T) req->rq_packbuf;
  sg_entry.len = recv_len; 
  sg_entry.lkey = req->rq_rpi->long_lkey;

  sr.sg_lst_p = &sg_entry;
  sr.sg_lst_len = 1;
  
  /* Remote send buffer details */
  sr.remote_addr = (LAM_ADDR_T) raddr;
  sr.r_key = rkey;

  /* We do not check for recv_env_cnt here since this is RDMA and
     must go through */
  lam_debug_cond((PUB(did), "<long><receiver>: posting RDMA get from"
		  "addr %x key %x req is %x",
		  raddr, rkey, req));
  ret = LAM_IB_post_sr(PUB(hca_hndl), req->rq_proc->p_rpi->cp_ib_qp_hndl, &sr);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "post-req-fail",
		   LAM_IB_err(ret));
    return LAMERROR;
  }
  return 0;
  
}


/*
 * Advance function for all requests
 */
int
lam_ssi_rpi_ib_ib_advance(void)
{
  int there_is_more = 1;
  struct _proc *proc;
  int i, done;

  /* VPS: Logic: If non-blocking, keep looping until no more events;if
     blocking, loop until u get one event, if done and no more event
     pending then exit */

  done = 0;
  if (lam_ssi_rpi_ib_flblock) { /* blocking */

    /* Loop until an event happens. When an event happens, loop until
       no more event happens */
    while (!done) {

      /* First check all the recv queues and send queues of all procs
	 If anytime, an event occured, say done, advance the pending sends
	 and come out. But if nothing occured, advance the pending sends 
	 and loop over again. */

      for (i = 0; i < num_procs; ++i) {
	proc = array[i];
	if (proc != lam_myproc) {

#if 0	
	  lam_debug_cond((PUB(did), "For proc %d -- %x I am goign to poll", 
			  i, proc));
#endif
	  done = poll_cq(proc);
	  if (done == LAMERROR)
	    return LAMERROR;

	  if (done) break;
	}
      }
      if (!LAM_SSI_RPI_IB_SEND_QUEUE_EMPTY() ||
	  !LAM_SSI_RPI_IB_ACK_QUEUE_EMPTY()) {

#if 0
	lam_debug_cond((PUB(did), "Advancing send/ack pending queue "));
#endif
	if (lam_ssi_rpi_ib_send_queue_advance() != 0) {
#if 0
	  lam_debug_cond((lam_ssi_rpi_ib_did, 
			  "ib_advance: send_queue_advance returns LAMERROR"));
#endif
	  return LAMERROR;
	}
      }
    } /* End while */

  } else { /* non-blocking */

    /* If no event happens, come out in 1 iteration, else if an event did
       happen, service it and keep looping till no event happens */

    while(there_is_more) {
    
      there_is_more = 0;

      for (i = 0; i < num_procs; ++i) {
	proc = array[i];
	if (proc != lam_myproc) {
	  there_is_more = poll_cq(proc);
	  if (there_is_more == LAMERROR)
	    return LAMERROR;
	}
      }
    }

    if (!LAM_SSI_RPI_IB_SEND_QUEUE_EMPTY() ||
	!LAM_SSI_RPI_IB_ACK_QUEUE_EMPTY())
      if (lam_ssi_rpi_ib_send_queue_advance() != 0) {
	lam_debug_cond((lam_ssi_rpi_ib_did, 
			"ib_advance: send_queue_advance returns LAMERROR"));
	return LAMERROR;
      }
  }
  return 0;
}


/*
 * Free the data structures and IB handles for finalize
 */
void
lam_ssi_rpi_ib_ib_finalize(void)
{
  int i;

  /* Free all buffers used for FC */
  for (i = 0; i < lam_currpool; ++i) {
    free(fc_remote_recv_env_cnt[i]);
    free(fc_post_env_cnt[i]);
  }

  /* Free all descriptor buffers for receive */
  free(rr_post_recv_list);
  free(rr_sg_entry_list);

  /* Deallocate IB resources */
  if (lam_ssi_rpi_ib_pd_hndl != LAM_IB_INVAL_HNDL)
    LAM_IB_dealloc_pd(PUB(hca_hndl), lam_ssi_rpi_ib_pd_hndl);
  
  lam_debug_cond((PUB(did), "PD destroyed "));

  if (lam_ssi_rpi_ib_hca_hndl != LAM_IB_INVAL_HNDL)
    LAM_IB_release_hca_hndl(PUB(hca_hndl));
}

/* Internal functions  */

/*******************************************************************/

/*
 * Poll the completion queue for callbacks
 */
int 
poll_cq(struct _proc *p) 
{
  LAM_IB_wc_desc cdesc;
  struct lam_ssi_rpi_ib_envl *env;

  ret = LAM_IB_CQ_EMPTY;

  ret = LAM_IB_poll_cq(PUB(hca_hndl), p->p_rpi->cp_ib_cq_hndl, 
			 &cdesc);

  if ((ret == LAM_IB_CQ_EMPTY))
    return 0;

  lam_debug_cond((PUB(did), " POLL: opcode is %d",cdesc.opcode));

  if ((ret != LAM_IB_OK) || (cdesc.status != LAM_IB_SUCCESS)) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "poll-fail", 
		   LAM_IB_err(ret));
    lam_debug_cond((PUB(did), "POLL FAIL: Opcode is %d Status is %d "
		    "Rank is %d "
		    "req is %x peer_rank is %d\n", cdesc.opcode,
		    cdesc.status, lam_myproc->p_gps.gps_grank, 
		    cdesc.id, p->p_gps.gps_grank));

    return LAMERROR;
  }

  switch (cdesc.opcode) {
    
    /* Send completion cases */

  case LAM_IB_SQ_RDMA_WRITE:

    /* FC send completion  */

    lam_debug_cond((PUB(did), "Got send completion for RDMA write: status %d",
		    cdesc.status));

    fc_send_completion_handler(p->p_rpi);
    
    /* Returning 0 because this is not a request generated completion
       It is rpi generated, hence we cannot exit out of the loop - we
       need to go and poll for more */
    return 0;
      
  case LAM_IB_SQ_RDMA_READ:

    /* LONG send completion  */

    lam_debug_cond((PUB(did), "<long>:<receiver>:Got rdma get completion:"
		    "status %d id-req %x",
		    cdesc.status, cdesc.id));

    lam_ssi_rpi_ib_receiver_get_body_callback((MPI_Request)(LAM_ADDR_T) 
						cdesc.id);
    return 1;

  case LAM_IB_SQ_SEND_DATA:

    /* Envelope sends completion */

    lam_debug_cond((PUB(did), "Got completion for env sends"));

    /* The CQ was for env_send - can be any, sycn or non-sync or ack -
       cdesc.id contains the env address */
    
    env = (struct lam_ssi_rpi_ib_envl *)(LAM_ADDR_T) cdesc.id;

    if ((env->ge_env.ce_flags & C2CACK) != 0)
      /* Its completion of ACK just sent for sync send  */
      lam_ssi_rpi_ib_ack_send_env_callback(env->ge_dest_request);
    else
      /* If its LONG, call long send callback, which is a one liner */
      if ((env->ge_env.ce_flags & C2CLONG) != 0)
	LAM_SSI_RPI_IB_LONG_SEND_CALLBACK;
      else
	/* Else its tiny send completion - can be sync or non-sync */
	lam_ssi_rpi_ib_tiny_send_env_callback(env->ge_src_request);
      
    return 1;

 case LAM_IB_INVAL_OPCODE:
   show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "inval-cq-opcode");
   return LAMERROR;

   /* Receive completion case */

 case LAM_IB_RQ_SEND_DATA:

   /* The completion is for env_recvs - the cdesc.id contains the
      env buffer address */

   lam_debug_cond((PUB(did), "POLL: Polling recv queue for"
		   "proc %x lid %d qp_num %d",
		   p, p->p_rpi->cp_ib_lid, p->p_rpi->cp_ib_dest_qp_num));
   
   /* Else go ahead and receive the envelope - adddr in cdesc.id*/

   if (lam_ssi_rpi_ib_receive_envelope(p->p_rpi, 
				       (struct lam_ssi_rpi_ib_envl *)
				       (LAM_ADDR_T) cdesc.id) != 0)
     return LAMERROR;
   return 1;
  }

  return 0;
}  

/*
 * Callback for flow control RDMA send
 */
int
fc_send_completion_handler(struct lam_ssi_rpi_proc *p_rpi) 
{
  p_rpi->fl_fc_am_sending_rdma = 0;

  if (p_rpi->fl_fc_want_to_rdma_send == 1) {

    p_rpi->fl_fc_am_sending_rdma = 1;
    p_rpi->fl_fc_want_to_rdma_send = 0;

    if (fc_post_rdma_send(p_rpi) != 0) {

      /* Could not do RDMA send, so just come out */

      p_rpi->fl_fc_am_sending_rdma = 0;
      return LAMERROR;
    }
  }

  return 0;
}

/*
 * Post a RDMA flow control send
 */
int
fc_post_rdma_send(struct lam_ssi_rpi_proc *p_rpi)
{
  LAM_IB_sr_desc sr;
  LAM_IB_sg_lst_entry sg_entry;

  sr.id = LAM_SSI_RPI_IB_FC_SEND;
  sr.opcode = LAM_IB_RDMA_WRITE;
  sr.comp_type = LAM_IB_SIGNALED;
  
  sg_entry.addr = (LAM_ADDR_T) (p_rpi->fc_post_env_cnt);
  sg_entry.len = sizeof(int);
  sg_entry.lkey = p_rpi->fc_post_env_cnt_lkey;

  sr.sg_lst_p = &sg_entry;
  sr.sg_lst_len = 1;
  sr.remote_addr = p_rpi->fc_addr;
  sr.r_key = p_rpi->fc_rkey;

  lam_debug_cond((PUB(did), "Writing FC RDMA cnt %d to remot addr %x key %x",
		  *(p_rpi->fc_post_env_cnt), p_rpi->fc_addr, 
		  p_rpi->fc_rkey));

  /* We do not check for recv_env_cnt here since this is RDMA and
     must go through */
  ret = LAM_IB_post_sr(PUB(hca_hndl), p_rpi->cp_ib_qp_hndl, &sr);
  if (ret != LAM_IB_OK) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "post-req-fail");
    return LAMERROR;
  }

  return 0;
}


/*
 * Exchange the remote buffer addresses and keys between peer process
 */
int
fc_post_send_recv(struct _proc *p) 
{
  PUB(fc_addr_key) tosend_addr_key;

  tosend_addr_key.addr 
    = (LAM_IB_virt_addr) (LAM_ADDR_T)(p->p_rpi->fc_remote_recv_env_cnt);

  tosend_addr_key.rkey 
    = p->p_rpi->fc_remote_recv_env_rkey;

  /* Send the fc info through the lam daemons */
  if (send_peer_fc_info(p, tosend_addr_key) == LAMERROR) {
    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", "fc-exchange-fail");
    return LAMERROR;
  }
  return 0;
}


/* 
 * Synchronize between myself and my peer, just a simple send and receive
 */

int
sync_peer(struct _proc *p)
{
  struct nmsg inmsg;
  struct nmsg outmsg;
  struct dolink *links = 0;
  int4 nlinks;
  int4 sdata = 1;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);

  /* get links to neighbors */

  if (ldogetlinks(&links, &nlinks)) 
    return(LAMERROR);

  inmsg.nh_flags = 0;
  inmsg.nh_length = 0;
  outmsg.nh_length = 0;
  outmsg.nh_flags = 0;

  outmsg.nh_data[0]= sdata;
  
  _m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
	       p->p_gps.gps_node, p->p_gps.gps_idx, 0, 
	       (lam_myproc->p_gps.gps_node & 0xFFFF00) >> 8, &outmsg);

  if (nsend(&outmsg))
      return LAMERROR;

  _m2l_fillsys(p->p_gps.gps_node, p->p_gps.gps_idx,
	       lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx, 0,
	       (p->p_gps.gps_node & 0xFFFF00) >> 8, &inmsg);

  if (nrecv(&inmsg))
    return LAMERROR;

  if (inmsg.nh_data[0] != sdata) {
    lam_debug_cond((PUB(did), "Error in sync between rank %d and rank %d \n",
		    lam_myproc->p_gps.gps_grank, p->p_gps.gps_grank));
    return LAMERROR;
  }

  if (links != 0)
    free(links);

  return 0;
}

/*
 * Actual exchange of remote addresses and keys through lam daemons
 */
int
send_peer_fc_info(struct _proc *p, PUB(fc_addr_key) addr_key)
{
  struct nmsg inmsg;
  struct nmsg outmsg;
  struct dolink *links = 0;
  int4 nlinks;

  int4 lower32;
  int4 upper32;

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);

  /* get links to neighbors */

  if (ldogetlinks(&links, &nlinks)) 
    return(LAMERROR);

  inmsg.nh_flags = 0;
  inmsg.nh_length = 0;
  outmsg.nh_length = 0;
  outmsg.nh_flags = 0;

  lower32 = addr_key.addr & 0xffffffff;

  /* for 64 bits machine */
#if LAM_SIZEOF_VOID_P == 8
  upper32 = (addr_key.addr >> 32) & 0xffffffff;
#else
  upper32 = 0;
#endif
  
  outmsg.nh_data[0]= lower32;
  outmsg.nh_data[1] = upper32;
  
  lower32 = addr_key.rkey & 0xffffffff;

#if SIZEOF_VAPI_RKEY_T == 8
  upper32 = (addr_key.rkey >> 32) & 0xffffffff;
#else
  upper32 = 0;
#endif
  
  outmsg.nh_data[2]= lower32;
  outmsg.nh_data[3] = upper32;
  lam_debug_cond((PUB(did), "FC: Sent 0 -- %x , 1 -- %x ,2 -- %x ,3-- %x",
		  outmsg.nh_data[0], outmsg.nh_data[1], outmsg.nh_data[2], 
		  outmsg.nh_data[3]));
  
  _m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
	       p->p_gps.gps_node, p->p_gps.gps_idx, 0, 
	       (lam_myproc->p_gps.gps_node & 0xFFFF00) >> 8, &outmsg);

  if (nsend(&outmsg))
      return LAMERROR;

  _m2l_fillsys(p->p_gps.gps_node, p->p_gps.gps_idx,
	       lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx, 0,
	       (p->p_gps.gps_node & 0xFFFF00) >> 8, &inmsg);

  if (nrecv(&inmsg))
    return LAMERROR;

  lam_debug_cond((PUB(did), "FC: recv 0 -- %x , 1 -- %x ,2 -- %x ,3-- %x",
		  inmsg.nh_data[0], inmsg.nh_data[1], inmsg.nh_data[2], 
		  inmsg.nh_data[3]));

  /* Get the addr */
  p->p_rpi->fc_addr = inmsg.nh_data[1];

#if LAM_SIZEOF_VOID_P == 8
  p->p_rpi->fc_addr <<= 32;
#endif

  p->p_rpi->fc_addr |= inmsg.nh_data[0];

  /* Get the rkey */
  p->p_rpi->fc_rkey = inmsg.nh_data[3];

#if SIZEOF_VAPI_RKEY_T == 8
  p->p_rpi->fc_rkey <<= 32;
#endif

  p->p_rpi->fc_rkey |= inmsg.nh_data[2];

  lam_debug_cond((PUB(did), "Got fc addr %p key %p",p->p_rpi->fc_addr,
		  p->p_rpi->fc_rkey));

  if (links != 0)
    free(links);

  return 0;

}
