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
 * $Id: ssi_coll_lam_basic_allgatherv.c,v 1.5 2004/03/02 03:43:36 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <mpisys.h>


/*
 *	allgatherv
 *
 *	Function:	- allgather using other MPI collectives
 *	Accepts:	- same as MPI_Allgatherv()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_lam_basic_allgatherv(void *sbuf, int scount, 
				  MPI_Datatype sdtype, void * rbuf, 
				  int *rcounts, int *disps, 
				  MPI_Datatype rdtype, 
				  MPI_Comm comm)
{
  int i, size;
  int err;

  /* Collect all values at each process, one at a time. */

  MPI_Comm_size(comm, &size);
  for (i = 0; i < size; ++i) {
    err = comm->c_ssi_coll.lsca_gatherv(sbuf, scount, sdtype, rbuf,
					rcounts, disps, rdtype, i, comm);
    if (err != MPI_SUCCESS)
      return err;
  }

  return MPI_SUCCESS;
}
