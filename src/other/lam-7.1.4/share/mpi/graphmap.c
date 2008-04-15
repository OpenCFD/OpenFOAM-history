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
 * $Id: graphmap.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- map processes to graph coordinates
 *	Accepts:	- old communicator
 *			- # processes
 *			- node index array
 *			- array of edges
 *			- ptr caller's new rank
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Graph_map - Maps process to graph topology information

Input Parameters:
+ comm - input communicator (handle) 
. nnodes - number of graph nodes (integer) 
. index - integer array specifying the graph structure, see
'MPI_GRAPH_CREATE'
- edges - integer array specifying the graph structure 

Output Parameter:
. pnewrank - reordered rank of the calling process; 'MPI_UNDEFINED' if
the calling process does not belong to graph (integer)
 
.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Graph_map(MPI_Comm comm, int nnodes, int *index, 
		  int *edges, int *pnewrank)
{
	int		errcode;
	int		myrank;

	lam_initerr();
	lam_setfunc(BLKMPIGRAPHMAP);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHMAP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHMAP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if ((nnodes < 1) || (index == 0) || (edges == 0) || (pnewrank == 0)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHMAP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Assign new rank.
 */
	errcode = MPI_Comm_rank(comm, &myrank);
	if (errcode != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIGRAPHMAP, errcode));
	}

	*pnewrank =
		((myrank < 0) || (myrank >= nnodes)) ? MPI_UNDEFINED : myrank;

	lam_resetfunc(BLKMPIGRAPHMAP);
	return(MPI_SUCCESS);
}
