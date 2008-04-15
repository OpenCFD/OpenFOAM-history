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
 * $Id: rpi_gm_recv_queue.h,v 1.4 2003/08/06 04:24:31 jsquyres Exp $
 */

#ifndef LAM_SSI_RPI_GM_RECV_QUEUE_H
#define LAM_SSI_RPI_GM_RECV_QUEUE_H

#include <rpisys.h>
#include <rpi_gm_reqlist.h>


/*
 * public functions
 */

int lam_ssi_rpi_gm_add_read(struct lam_ssi_rpi_proc *ps, MPI_Request req);
int lam_ssi_rpi_gm_add_read_any_src(MPI_Request req);
int lam_ssi_rpi_gm_rm_read_any_src(MPI_Request req, struct _proc *exception);


/*
 * For symmetry.  We really don't need a function call for this --
 * just make it a wrapper around the underlying reqlist functionality.
 */
#define lam_ssi_rpi_gm_rm_read(ps, item) \
  lam_ssi_rpi_gm_reqlist_rm_item(&((ps)->cp_pending_recvs), item)
#define lam_ssi_rpi_gm_rm_read_req(ps, req) \
  lam_ssi_rpi_gm_reqlist_rm_req(&((ps)->cp_pending_recvs), req)


#endif /* LAM_SSI_RPI_GM_RECV_QUEUE_H */
