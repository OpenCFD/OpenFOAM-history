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
 *	$Id: rpi_gm_proc.h,v 1.10 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function:	- gm proc specification
 */

#ifndef LAM_SSI_RPI_GM_PROC_H
#define LAM_SSI_RPI_GM_PROC_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <rpi_gm_actions.h>
#include <rpi_gm_req.h>
#include <mpisys.h>
#include <rpi_gm.h>
#include <all_array.h>


/*
 * gm-specific MPI process data
 */

struct lam_ssi_rpi_proc {
  /* This needs to be here for the unexepcted message handling (i.e.,
     partially received unexpected messages), but cp_nbfde is not used
     in this RPI */

  int cp_nbfde;

  /* point up to the main _proc so that we can do checks for things
     like LAM_PHOMOG.  This is an optimization -- in the gm RPI, we
     usually pass around just a (struct lam_ssi_rpi_proc*) because we
     do 95% of the work with just this.  But periodically, a few
     things new stuff from the upper-level proc (e.g., p_mode &
     LAM_PHOMOG), so we need to be able to find the upper-level proc
     when we only have the lam_ssi_rpi_proc. */

  struct _proc *cp_proc;

  /* Pointer to the envelope sender's _proc entry on the remote
     process (exchange/set during addprocs/connect_all).  This value
     is included in envelopes to prevent loop lookups on when this
     peer process receives an envelope from me. */

  struct _proc *cp_peer_proc;

  /* count of bytes sent/received -- required to support
     checkpoint/restart of the gm RPI. */

  unsigned int cp_out_nbytes;
  unsigned int cp_in_nbytes;

  /* gm port to use, with cached values for the port ID and node ID */

  lam_ssi_rpi_gm_port_t cp_gm_port;

  /* Queues for the pending sends and receives on this proc */

  struct lam_ssi_rpi_gm_reqlist_t cp_pending_recvs;

  /* For checkpoint/restart support, there are two cases where an
     incoming checkpoint request will cause a stale pointer at
     continue/restart time.  We keep track of these cases here so that
     they can be chased down/refreshed at continue/restart time.  See
     the lengthy comment at the beginning of ssi_rpi_gm_cr.c for more
     details. */

  lam_array_t *cp_unexpected_long_recvs;
  lam_array_t *cp_pending_long_recvs;

  /* Keep count of the number of pending sends issued to GM */
  int num_pending_sends;
  int num_dropped_sends;

#if 0
  unsigned long send_seq_num;
  unsigned long expected_recv_seq_num;

  /* Queue for buffering out-of-order packets */
  struct lam_ssi_rpi_gm_datalist_t out_of_order_recvs;
#endif

};


#endif	/* LAM_SSI_RPI_GM_PROC_H */
