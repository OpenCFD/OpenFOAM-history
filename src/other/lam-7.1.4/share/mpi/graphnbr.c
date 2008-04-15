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
 * $Id: graphnbr.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get neighbours of a process
 *	Accepts:	- communicator
 *			- rank
 *			- size of neighbours array 
 *			- neighbours array (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Graph_neighbors - Returns the neighbors of a node associated 
                      with a graph topology

Input Parameters:
+ comm - communicator with graph topology (handle) 
. rank - rank of process in group of comm (integer) 
- maxnbrs - size of array neighbors (integer) 

Output Parameters:
. nbrs - ranks of processes that are neighbors to specified process
(array of integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_RANK

.N ACK
@*/
int MPI_Graph_neighbors(MPI_Comm comm, int rank, 
			int maxnbrs, int *nbrs)
{
	int		nnbrs;
	int		i;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPIGRAPHNBR);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHNBR, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBR, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_GRAPH(comm)) {
		return(lam_errfunc(comm, BLKMPIGRAPHNBR,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((rank < 0) || (rank >= comm->c_group->g_nprocs)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBR, lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	if ((maxnbrs < 0) || ((maxnbrs > 0) && (nbrs == 0))) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHNBR, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Fill the neighbours.
 */
	nnbrs = comm->c_topo_index[rank];
	p = comm->c_topo_edges;

	if (rank > 0) {
		i = comm->c_topo_index[rank - 1];
		nnbrs -= i;
		p += i; 
	}

	for (i = 0; (i < maxnbrs) && (i < nnbrs); ++i, ++p) {
		*nbrs++ = *p;
	}

	lam_resetfunc(BLKMPIGRAPHNBR);
	return(MPI_SUCCESS);
}
