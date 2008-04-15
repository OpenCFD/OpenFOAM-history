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
 * $Id: rpi_crtcp_req.h,v 1.2 2002/10/09 21:00:17 brbarret Exp $
 *
 *	Function:	- CRTCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_CRTCP_REQ_H
#define LAM_SSI_RPI_CRTCP_REQ_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * CRTCP-specific MPI request data
 */

struct lam_ssi_rpi_req {
  int cq_state;	       /* state of request */
#define	C2CDONE		1		       /* all data transfer done */
#define C2CWRITE	2		       /* writing to socket */
#define C2CREAD		3		       /* reading from socket */
#define C2CSENDSELF	4		       /* sending to self */
  int4 cq_peer;	       /* matched peer */
  struct lam_ssi_rpi_envl cq_env;	       /* envelope (synch + state) */
  struct lam_ssi_rpi_envl cq_outenv;	       /* byte-ordered outgoing env. */
  int cq_nenvout;	       /* # env. bytes left to write */
  int cq_nmsgout;	       /* # msg. bytes left to write */
  char *cq_envbuf;	       /* envelope buffer pointer*/
  char *cq_msgbuf;	       /* message buffer pointer */
  int cq_bufoff;	       /* offset of shared buffer */
  int cq_bufsize;	       /* size of shared buffer */
  int (*cq_adv)();	       /* advance function */
};

#endif	/* LAM_SSI_RPI_CRTCP_REQ_H */
