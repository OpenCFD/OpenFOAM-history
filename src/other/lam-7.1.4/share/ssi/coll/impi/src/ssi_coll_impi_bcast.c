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
 * $Id: ssi_coll_impi_bcast.c,v 1.1 2003/02/04 18:32:25 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#include <lam-ssi-coll-impi-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-impi.h>
#include <mpi.h>
#include <blktype.h>
#include <mpisys.h>
#include <impi.h>


/*
 * Local functions
 */
static int master_bcast(void *buff, int count, MPI_Datatype datatype, 
			int root, MPI_Comm comm);


/*
 *	bcast
 *
 *	Function:	- Almost identical to the pseudocode in section
 *                        4.11 of the IMPI standard
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_impi_bcast(void *buff, int count,
			MPI_Datatype datatype, int root,
			MPI_Comm comm)
{
  int ret;
  int local_root;
  int myrank = comm->c_group->g_myrank;

  lam_setfunc(BLKIMPIBCAST);

  /* Switch to use the IMPI collective context ID */

  lam_mkimpicoll(comm);

  /* Global phase */

  if (myrank == root ||
      (IMPI_Is_master(myrank, comm) && !IMPI_Are_local(myrank, root, comm)))
    master_bcast(buff, count, datatype, root, comm);

  /* Switch back to the pt2pt context ID */

  lam_mkpt(comm);

  /* Local phase */

  if (IMPI_Are_local(myrank, root, comm)) {
    local_root = IMPI_Rank_impi2shadow(root, comm);
    ret = MPI_Bcast(buff, count, datatype, local_root, comm->c_shadow);
  }
  else {
    local_root = IMPI_Local_master_rank(myrank, comm);
    local_root = IMPI_Rank_impi2shadow(local_root, comm);
    ret = MPI_Bcast(buff, count, datatype, local_root, comm->c_shadow);
  }

  lam_resetfunc(BLKIMPIBCAST);
  return ret;
}


/*
 * Almost identical to the pseudocode in section 4.11 of the IMPI
 * standard
 */
static int 
master_bcast(void *buff, int count, MPI_Datatype datatype, 
	     int root, MPI_Comm comm)
{
  int myrank, nmasters, mynum, rootnum, vnum, dim, hibit;
  int i, peer, mask;

  myrank = comm->c_group->g_myrank;
  nmasters = IMPI_Num_masters(comm);

  if (nmasters <= IMPI_MAX_LINEAR_BCAST) {
    /* Linear broadcast between masters */

    if (myrank == root) {
      for (i = 0; i < nmasters; i++) {
	if (i == IMPI_Local_master_num(root, comm))
	  continue;

	MPI_Send(buff, count, datatype, IMPI_Master_rank(i, comm),
		 IMPI_BCAST_TAG, comm);
      }
}
    else {
      MPI_Recv(buff, count, datatype, root, IMPI_BCAST_TAG, comm, 
	       MPI_STATUS_IGNORE);
    }
  } else {
    /* Tree broadcast between masters */

    mynum = IMPI_Master_num(myrank, comm);
    rootnum = IMPI_Master_num(root, comm);
    vnum = (mynum + nmasters - rootnum) % nmasters;
    dim = lam_cubedim(nmasters);
    hibit = lam_hibit(vnum, dim);
    --dim;

    /* Receive data from parent in the tree */

    if (vnum > 0) {
      peer = ((vnum & ~(1 << hibit)) + rootnum) % nmasters;
      peer = IMPI_Master_rank(peer, comm);
      if (IMPI_Are_local(peer, root, comm))
	peer = root;

      MPI_Recv(buff, count, datatype, peer, IMPI_BCAST_TAG, comm, 
	       MPI_STATUS_IGNORE);
    }

    /* Send data to children */

    for (i = hibit + 1, mask = 1 << i; i <= dim; ++i, mask <<= 1) {
      peer = vnum | mask;
      if (peer < nmasters) {
	peer = IMPI_Master_rank((peer + rootnum) % nmasters, comm);
	MPI_Send(buff, count, datatype, peer, IMPI_BCAST_TAG, comm);
      }
    }
  }
  
  return MPI_SUCCESS;
}

