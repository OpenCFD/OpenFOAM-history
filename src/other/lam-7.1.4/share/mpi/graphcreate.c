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
 * $Id: graphcreate.c,v 6.6 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- create a communicator with graph mapping
 *	Accepts:	- old communicator
 *			- # nodes
 *			- node index array
 *			- array of edges
 *			- rank reordering flag
 *			- new communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Graph_create - Makes a new communicator to which topology information
                 has been attached

Input Parameters:
+ comm - input communicator without topology (handle) 
. nnodes - number of nodes in graph (integer) 
. index - array of integers describing node degrees (see below) 
. edges - array of integers describing graph edges (see below) 
- reorder - ranking may be reordered (true) or not (false) (logical) 

Output Parameter:
. newcomm - communicator with graph topology added (handle) 

Algorithm:

LAM/MPI currently ignores the 'reorder' flag.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Graph_create(MPI_Comm comm, int nnodes, int *index, 
		     int *edges, int reorder, MPI_Comm *newcomm)
{
	MPI_Group	newgroup;
	int		nedges;
	int		size;
	int		err;
	int		range[1][3];
	int		i;
	int		*topo;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPIGRAPHCREATE);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGRAPHCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if ((nnodes < 1) || (index == 0) || (edges == 0) || (newcomm == 0)) {
		return(lam_errfunc(comm,
			BLKMPIGRAPHCREATE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIGRAPHCREATE));
/*
 * Create and error check the topology information.
 */
	nedges = index[nnodes - 1];

	topo = (int *) malloc((unsigned) (nnodes + nedges) * sizeof(int));
	if (topo == 0) {
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	for (i = 0, p = topo; i < nnodes; ++i, ++p) {
		*p = *index++;
	}

	for (i = 0; i < nedges; ++i, ++p) {
		*p = *edges++;
		if (*p < 0 || *p >= nnodes) {
			free((char *) topo);
			return(lam_errfunc(comm, BLKMPIGRAPHCREATE,
					lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
		}
	}
/*
 * Create the group for the new communicator.
 */
	err = MPI_Comm_size(comm, &size);
	if (err != MPI_SUCCESS) {
		free((char *) topo);
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE, err));
	}

	if (nnodes > size) {
		free((char *) topo);
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (nnodes == size) {
		err = MPI_Comm_group(comm, &newgroup);
	} else {
		range[0][0] = 0;
		range[0][1] = nnodes - 1;
		range[0][2] = 1;
		err = MPI_Group_range_incl(comm->c_group, 1, range, &newgroup);
	}

	if (err != MPI_SUCCESS) {
		free((char *) topo);
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE, err));
	}
/*
 * Create the new communicator.
 */
	err = MPI_Comm_create(comm, newgroup, newcomm);
	if (err != MPI_SUCCESS) {
		free((char *) topo);
		MPI_Group_free(&newgroup);
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE, err));
	}
/*
 * Set the communicator topology information. 
 */
	if (*newcomm != MPI_COMM_NULL) {
		(*newcomm)->c_topo_type = MPI_GRAPH;
		(*newcomm)->c_topo_nprocs = nnodes;
		(*newcomm)->c_topo_nedges = nedges;
		(*newcomm)->c_topo_index = topo;
		(*newcomm)->c_topo_edges = topo + nnodes;
	}

	err = MPI_Group_free(&newgroup);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIGRAPHCREATE, err));
	}

	LAM_TRACE(lam_tr_cffend(BLKMPIGRAPHCREATE, -1, comm, 0, 0));

	lam_resetfunc(BLKMPIGRAPHCREATE);
	return(MPI_SUCCESS);
}
