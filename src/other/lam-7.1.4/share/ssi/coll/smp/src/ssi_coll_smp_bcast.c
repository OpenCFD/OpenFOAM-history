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
 * $Id: ssi_coll_smp_bcast.c,v 1.2 2003/05/28 00:16:32 jsquyres Exp $
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
#include <blktype.h>
#include <mpisys.h>


/*
 *	barrier
 *
 *	Function:	- barrier using SMP-aware
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_bcast(void *buff, int count, MPI_Datatype datatype, 
		       int root, MPI_Comm comm)
{
  int err;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* Set root to setup coordinator comms, etc. */

  lam_ssi_coll_smp_set_root(comm, root);

  /* If I'm a coordinator, participate in the coordinator broadcast */

  if (lcd->lcd_coord_comms[root] != MPI_COMM_NULL) {
    if ((err = MPI_Bcast(buff, count, datatype, lcd->lcd_coord_roots[root], 
			 lcd->lcd_coord_comms[root])) != MPI_SUCCESS)
      return err;
  }

  /* Now local broadcast out from the coordinators */

  if (lcd->lcd_local_size > 1)
    if ((err = MPI_Bcast(buff, count, datatype, lcd->lcd_local_roots[root],
			 lcd->lcd_local_comm)) != MPI_SUCCESS)
      return err;

  return MPI_SUCCESS;
}
