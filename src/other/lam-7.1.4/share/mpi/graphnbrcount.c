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
 * $Id: graphnbrcount.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get # neighbours for a process
 *	Accepts:	- communicator
 *			- rank
 *			- ptr # neighbours
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Graph_neighbors_count - Returns the number of neighbors of a node
                            associated with a graph topology

Input Parameters:
+ comm - communicator with graph topology (handle) 
- rank - rank of process in group of 'comm' (integer) 

Output Parameter:
. pnbr - number of neighbors of specified process (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, 
			      int *pnbr)
{
	lam_initerr();
	lam_setfunc(BLKMPIGRAPHNBRCOUNT);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHNBRCOUNT, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBRCOUNT, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_GRAPH(comm)) {
		return(lam_errfunc(comm, BLKMPIGRAPHNBRCOUNT,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((rank < 0) || (rank >= comm->c_group->g_nprocs)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBRCOUNT, lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	if (pnbr == 0) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBRCOUNT, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pnbr = comm->c_topo_index[rank];
	if (rank > 0) {
		*pnbr -= comm->c_topo_index[rank - 1];
	}

	lam_resetfunc(BLKMPIGRAPHNBRCOUNT);
	return(MPI_SUCCESS);
}
