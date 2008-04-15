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
 *	$Id: rpi_lamd.h,v 1.3 2002/12/17 17:15:12 jsquyres Exp $
 *
 *	Function:	- RPI implementation for LAM daemon
 *			- constants and templates
 */

#ifndef LAM_SSI_RPI_LAMD_H
#define LAM_SSI_RPI_LAMD_H

#include <net.h>

/*
 * lamd request
 */
struct lam_ssi_rpi_req
{
  struct nmsg lmq_msg_data;	/* data LAM message */
  struct nmsg lmq_msg_ack;	/* ack LAM message */
  struct nmsg lmq_msg_ger;	/* GER LAM message */
  int lmq_state;		/* progress state */
#define LAMD_START	0	       /* no communication yet */
#define LAMD_ACK	1	       /* ship ack next */
#define LAMD_TAIL	2	       /* shipping data tail */
#define LAMD_GER	4	       /* GER protocol */
#define LAMD_DONE	3	       /* completed */
  int lmq_flags;		/* misc. flags */
#define LAMD_RECVMODE	1	       /* recv or probe */
#define LAMD_SSEND	2	       /* msg is synchronous */
  int lmq_ndata;		/* #bytes to ship */
  int lmq_ndata_moved;		/* #bytes shipped */
};

/*
 * polling defaults
 */

#ifndef MPIL_LPD_BASE
#define MPIL_LPD_BASE	50000
#endif

#ifndef MPIL_LPD_MAX
#define MPIL_LPD_MAX	1000000
#endif

#endif /* LAM_SSI_RPI_LAMD_H */
