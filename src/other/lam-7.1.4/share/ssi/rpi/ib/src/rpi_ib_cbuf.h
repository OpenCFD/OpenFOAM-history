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
 *	$Id: rpi_ib_cbuf.h,v 1.3 2003/12/15 02:49:16 vsahay Exp $
 *
 *	Function:	- ib proc specification
 */

#ifndef LAM_SSI_RPI_IB_CBUF_H
#define LAM_SSI_RPI_IB_CBUF_H

#include <lam_config.h>
#include <lam-ssi-rpi-ib-config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * ib-specific unexpected message buffering data
 */

struct lam_ssi_rpi_cbuf {

  /* Standard unexpected buffering only saves the LAM-specified
     envelope.  In ib, we need to save a few more pieces of data with
     that. */

  MPI_Request lsrc_src_request;

  void *lsrc_target;    /* Remote address */
  LAM_IB_rkey rkey;     /* Remote key */
  int was_sent_to_self_non_sync; /* Unexpected non-sync send_to_self identifier */
};


#endif	/* LAM_SSI_RPI_IB_CBUF_H */
