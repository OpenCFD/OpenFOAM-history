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
 * $Id: ssi_coll_smp_barrier.c,v 1.8 2004/03/02 03:43:38 jsquyres Exp $
 *
 *	Function:	- SMP-aware collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-smp-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpi.h>
#include <mpisys.h>


/*
 *	barrier
 *
 *	Function:	- barrier using SMP-aware algorithm
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_smp_barrier(MPI_Comm comm)
{
  int err;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* Don't need to set_root because we use coordinator comms as if
     this were rooted at rank==0, and that was setup at init time */

  /* Fan in from the ranks on my local node */

  if (lcd->lcd_local_size > 1)
    if ((err = MPI_Gather(NULL, 0, MPI_BYTE, NULL, 0, MPI_BYTE, 
			   0, lcd->lcd_local_comm)) != MPI_SUCCESS)
      return err;

  /* Fan in/out among coordinators.  */

  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    int rank, rank2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(lcd->lcd_coord_comms[0], &rank2);
    if ((err = MPI_Barrier(lcd->lcd_coord_comms[0])) != MPI_SUCCESS)
      return err;
  }
  
  /* Fan out to the ranks on my local node */

  if (lcd->lcd_local_size > 1) {
    lcd->lcd_local_comm->c_ssi_coll.lsca_bcast_optimization = 0;
    err = MPI_Bcast(NULL, 0, MPI_BYTE, 0, lcd->lcd_local_comm);
    lcd->lcd_local_comm->c_ssi_coll.lsca_bcast_optimization = 1;
    if (err != MPI_SUCCESS)
      return err;
  }

  return MPI_SUCCESS;
}


