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
 * $Id: ssi_coll_init_comm.c,v 1.10 2004/03/02 03:43:34 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <lamdebug.h>
#include <typical.h>
#include <mpisys.h>


int lam_ssi_coll_base_finalize_comm(MPI_Comm comm) {
  char name[BUFSIZ];
  
  if (lam_ssi_coll_verbose >= 10) {
    if (comm->c_name[0] != '\0')
      snprintf(name, sizeof(name), "%s (cid %d)", comm->c_name, 
	       comm->c_contextid);
    else
      snprintf(name, sizeof(name), "<no name> (cid %d)", comm->c_contextid);
    name[sizeof(name) - 1] = '\0';
    lam_debug(lam_ssi_coll_did, "finalize_comm: communicator: %s", name);
  }
  
  if (comm->c_ssi_coll.lsca_finalize(comm) == LAMERROR)
    return LAMERROR;
  
  /* If the LAM_RBASIC bit is set then call lam_basic_finalize also.  This 
     will be typical in case of intercommunicator where some of the lam_basic
     intercomm algorithms are being used */ 
  if (LAM_REQ_COLL_BASIC(comm)) {
    if (comm->lsca_basic_finalize(comm) == LAMERROR)
      return LAMERROR;
  }
  
  return 0;
}
