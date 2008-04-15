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
 * $Id: csplit.c,v 6.11 2003/08/20 21:49:32 jsquyres Exp $
 *
 *	Function:	- split communicator into disjoint subsets
 *	Accepts:	- old communicator
 *			- process colour
 *			- process key
 *			- new communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <lam-ssi-coll.h>


/*
 * local functions
 */
static int cmpprocs(const void *a, const void *b);


/*@

MPI_Comm_split - Creates new communicators based on colors and keys

Input Parameters:
+ comm - communicator (handle) 
. colour - control of subset assignment (nonnegative integer) 
- key - control of rank assigment (integer) 

Output Parameter:
. newcomm - new communicator (handle) 

Notes:

The 'colour' must be non-negative or 'MPI_UNDEFINED'.  This is a
collective call over all ranks in 'comm'.

When a communicator is no longer being used, it should be freed with
'MPI_Comm_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_INTERN
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Comm_free

.N ACK
@*/
int MPI_Comm_split(MPI_Comm comm, int colour, int key, 
		   MPI_Comm *newcomm)
{
	MPI_Group	oldgroup;		/* old group */
	MPI_Group	newgroup;		/* new group */
	int		cid;			/* global context ID */
	int		i;			/* favourite index */
	int		err;			/* error code */
	int		myrank;			/* my group rank */
	int		nprocs;			/* # processes */
	int		mynprocs;		/* # procs in new comm */
	int		*procs;			/* procs array */
	int		*myprocs;		/* new comm procs array */
	int		*p, *p2;		/* favourite pointers */

	lam_initerr();
	lam_setfunc(BLKMPICOMMSPLIT);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

#if LAM_WANT_IMPI

	/* Remove this when IMPI collectives are implemented */

        if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, EINVAL, 
			      "Collectives not yet implemented on IMPI communicators");
	}
#endif

/*
 * Set up tracing.
 */
	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMSPLIT));
	MPI_Comm_rank(comm, &myrank);

	/* Collectively decide on a new CID */

	err = lam_coll_alloc_intra_cid(comm, (colour != MPI_UNDEFINED), 
				       BLKMPICOMMSPLIT, &cid);
	if (err != MPI_SUCCESS) {
		LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPLIT, -1, comm, 0, 0));
		lam_resetfunc(BLKMPICOMMSPLIT);
		return(lam_errfunc(comm, BLKMPICOMMSPLIT, err));
	}
/*
 * Create the array of process information.
 */
	MPI_Comm_size(comm, &nprocs);

	procs = (int *) malloc((unsigned) 3 * (nprocs + 1) * sizeof(int));
	if (procs == 0) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Gather all process information at all processes.
 */
	p = &procs[3 * nprocs];
	p[0] = colour;
	p[1] = key;
	p[2] = myrank;

	err = MPI_Allgather(p, 3, MPI_INT, procs, 3, MPI_INT, comm);
	if (err != MPI_SUCCESS) {
		free((char *) procs);
		LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPLIT, -1, comm, 0, 0));
		lam_resetfunc(BLKMPICOMMSPLIT);
		return(lam_errfunc(comm, BLKMPICOMMSPLIT, err));
	}
/*
 * Processes with undefined colour can stop here.
 */
	if (colour == MPI_UNDEFINED) {
		*newcomm = MPI_COMM_NULL;
		free((char *) procs);
		LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPLIT, -1, comm, 0, 0));
		lam_resetfunc(BLKMPICOMMSPLIT);
		return(MPI_SUCCESS);
	}
/*
 * Sort the process information.
 * Locate and count the # of processes having my colour.
 */
	qsort((char *) procs, nprocs, 3 * sizeof(int), cmpprocs);

	myprocs = 0;

	for (i = 0, p = procs; (i < nprocs) && (*p != colour); ++i, p += 3);

	myprocs = p;
	mynprocs = 1;

	for (++i, p += 3; (i < nprocs) && (*p == colour); ++i, p += 3) {
		++mynprocs;
	}
/*
 * Compact the old ranks of my old group in the array.
 */
	p = myprocs;
	p2 = myprocs + 2;

	for (i = 0; i < mynprocs; ++i, ++p, p2 += 3) {
		*p = *p2;
	}
/*
 * Create the new group.
 */
	MPI_Comm_group(comm, &oldgroup);

	err = MPI_Group_incl(oldgroup, mynprocs, myprocs, &newgroup);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT, err));
	}

	free((char *) procs);
	MPI_Group_free(&oldgroup);
/*
 * Create the new communicator.
 * MPI_Comm_create() is not called because we don't want to communicate
 * globally again, we have the new contextid (and the undefined-colour
 * procs have returned), so we win speed and safety.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, newgroup, MPI_GROUP_NULL, 0, newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*newcomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}
	
	if (lam_tr_comm(*newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMSPLIT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  It'll likely be the
	   same as the one that's on this communicator, but we still
	   give them the option to do something different anyway... */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(comm, BLKMPICOMMSPLIT,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMSPLIT, -1, comm, 0, 0));

	lam_resetfunc(BLKMPICOMMSPLIT);
	return(MPI_SUCCESS);
}

/*
 *	cmpprocs
 *
 *	Function:	- compare two process entries
 *	Accepts:	- ptr 1st entry
 *			- ptr 2nd entry
 *	Returns:	- -1, 0, +1 (less, equal, greater)
 */
static int
cmpprocs(const void* a, const void *b)
{
        int cmp;
	int *e1 = (int *) a;
	int *e2 = (int *) b;

	if (*e1 < *e2) {
		cmp = -1;
	} else if (*e1 > *e2) {
		cmp = 1;
	} else if (*(++e1) < *(++e2)) {
		cmp = -1;
	} else if (*e1 > *e2) {
		cmp = 1;
	} else if (*(++e1) < *(++e2)) {
		cmp = -1;
	} else if (*e1 > *e2) {
		cmp = 1;
	} else {
		cmp = 0;
	}

	return(cmp);
}
