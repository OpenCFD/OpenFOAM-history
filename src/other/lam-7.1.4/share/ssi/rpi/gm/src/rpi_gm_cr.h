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
 *	Function:	- GM bookmarks header
 */

#ifndef LAM_SSI_RPI_GM_CR_H
#define LAM_SSI_RPI_GM_CR_H

#include <lam-ssi-rpi-gm-config.h>

#include <gm.h>

#include <rpi_gm.h>


/****************************************************************************
 *************  THIS NEEDS TO MOVE TO ssi/include/lam-ssi-cr.h **************
 ***************************************************************************/

/* bookmark payload data structure */

typedef struct bookmark_payload {
  int bp_proc_index;
  unsigned int bp_out_nbytes;
} bookmark_payload_t;

/* Bookmark exchange functions */

int PUB(bookmark_exchange_alltoall)(struct _proc **procs, int nprocs, 
                                    bookmark_payload_t *my_bookmarks);

#endif	/* LAM_SSI_RPI_GM_CR_H */
