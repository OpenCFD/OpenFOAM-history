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
 * $Id: graphget.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get nodes/edges in graph topology
 *	Accepts:	- communicator
 *			- size of nodes array
 *			- size of edges array
 *			- nodes array
 *			- edges array
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Graph_get - Retrieves graph topology information associated with a 
                communicator

Input Parameters:
+ comm - communicator with graph structure (handle) 
. maxnodes - length of vector 'index' in the calling program  (integer) 
- maxedges - length of vector 'edges' in the calling program  (integer) 

Output Parameter:
+ nodes - array of integers containing the graph structure (for details see the definition of 'MPI_GRAPH_CREATE') 
- edges - array of integers containing the graph structure 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Graph_get(MPI_Comm comm, int maxnodes, int maxedges, 
		  int *nodes, int *edges)
{
	int		i;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPIGRAPHGET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_GRAPH(comm)) {
		return(lam_errfunc(comm, BLKMPIGRAPHGET,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((maxnodes < 0) || (maxedges < 0) || (nodes == 0) || (edges == 0)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHGET, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Fill the nodes and edges arrays.
 */
	p = comm->c_topo_index;
	for (i = 0; (i < comm->c_topo_nprocs) && (i < maxnodes); ++i, ++p) {
		*nodes++ = *p;
	}

	p = comm->c_topo_edges;
	for (i = 0; (i < comm->c_topo_nedges) && (i < maxedges); ++i, ++p) {
		*edges++ = *p;
	}

	lam_resetfunc(BLKMPIGRAPHGET);
	return(MPI_SUCCESS);
}
