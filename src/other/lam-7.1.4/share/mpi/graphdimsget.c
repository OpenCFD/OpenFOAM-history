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
 * $Id: graphdimsget.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get # nodes/edges in graph topology
 *	Accepts:	- communicator
 *			- ptr # nodes (returned value)
 *			- ptr # edges (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Graphdims_get - Retrieves graph topology information associated with a 
                    communicator

Input Parameters:
. comm - communicator for group with graph structure (handle) 

Output Parameter:
+ pnodes - number of nodes in graph (integer) 
- pedges - number of edges in graph (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Graphdims_get(MPI_Comm comm, int *pnodes, int *pedges)
{
	lam_initerr();
	lam_setfunc(BLKMPIGRAPHDIMSGET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHDIMSGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHDIMSGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_GRAPH(comm)) {
		return(lam_errfunc(comm, BLKMPIGRAPHDIMSGET,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((pnodes == 0) || (pedges == 0)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHDIMSGET, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pnodes = comm->c_topo_nprocs;
	*pedges = comm->c_topo_nedges;

	lam_resetfunc(BLKMPIGRAPHDIMSGET);
	return(MPI_SUCCESS);
}
