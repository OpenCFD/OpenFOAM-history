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
 *	$Id: rpi_gm_cbuf.h,v 1.3 2003/11/06 18:58:11 jsquyres Exp $
 *
 *	Function:	- gm proc specification
 */

#ifndef LAM_SSI_RPI_GM_CBUF_H
#define LAM_SSI_RPI_GM_CBUF_H

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * gm-specific unexpected message buffering data
 */

struct lam_ssi_rpi_cbuf {

  /* Standard unexpected buffering only saves the LAM-specified
     envelope.  In gm, we need to save a few more pieces of data with
     that. */

  MPI_Request lsrc_src_request;
#if LAM_SSI_RPI_GM_HAVE_RDMA_GET
  void *lsrc_target;
#endif
};


#endif	/* LAM_SSI_RPI_GM_CBUF_H */
