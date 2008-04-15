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
 * $Id: mpil_id.c,v 6.5 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- identifers for MPI opaque objects
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 * backward compatibility
 */
int MPI_Comm_get_contextid(comm, cid) MPI_Comm comm; int *cid;
	{ return(MPIL_Comm_id(comm, cid)); }

/*
 *	MPIL_Comm_id
 *
 *	Function:	- returns the context ID
 *	Accepts:	- communicator
 *			- context ID (out)
 */
/*@

MPIL_Comm_id - LAM/MPI-specific function to return the context ID of
the communicator

Input Parameter:
. comm - communicator (handle)

Output Parameter:
. pcid - communicator context id (integer)

Notes:

In the LAM implementation of MPI, each communicator has an integer
context ID associated with it for synchronizing on different contexts.
This ID is global to all the processes in the communicator''s group,
and uniquely identifies that communicator for each process.  These
properties allow the processes to safely exchange messages without
interference from operations on other communicators.
 
The MPI standard does not provide a way to access/view this
implementation-dependent synchronization since communicators are
opaque objects.  Users do not need such access for normal MPI
operations.  On the other hand, when debugging MPI applications, the
opaque nature of communicators hinders the user''s efforts.  This is
especially true on fully observable systems such as LAM, where users
can monitor the full state of the processes and message queues, which
includes the context ID (see 'mpitask'(1) and 'mpimsg'(1)).

This is a LAM/MPI-specific function and is intended mainly for
debugging.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     int cid;
     MPIL_Comm_id(MPI_COMM_WORLD, &cid);
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso: MPIL_Comm_gps, MPIL_Type_id
@*/
int
MPIL_Comm_id(MPI_Comm comm, int *pcid)
{
	lam_initerr();
	lam_setfunc(BLKMPILCOMMID);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			lam_getfunc(), lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (pcid == 0) {
		return(lam_errfunc(comm,
			lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pcid = comm->c_contextid;

	lam_resetfunc(BLKMPILCOMMID);
	return(MPI_SUCCESS);
}

/*
 *	MPIL_Comm_gps
 *
 *	Function:	- translates MPI comm/rank to LAM nid/pid
 *			- if the communicator is an intercommunicator then
 *			  rank is taken to be in the remote group
 *	Accepts:	- communicator
 *			- rank
 *			- node ID (out)
 *			- process ID (out)
 */
/*@

MPIL_Comm_gps - LAM/MPI-specific function to return the GPS information
of a given rank from a communicator

Input Parameters:
+ comm - communicator (handle)
- rank - rank of process to query (integer)

Output Parameters:
+ pnid - LAM node ID
- ppid - LAM process ID

Notes:

In the LAM implementation of MPI, each communicator has an integer
context ID associated with it for synchronizing on different contexts.
This ID is global to all the processes in the communicator''s group,
and uniquely identifies that communicator for each process.  These
properties allow the processes to safely exchange messages without
interference from operations on other communicators.
 
The MPI standard does not provide a way to access/view this
implementation-dependent synchronization since communicators are
opaque objects.  Users do not need such access for normal MPI
operations.  On the other hand, when debugging MPI applications, the
opaque nature of communicators hinders the user''s efforts.  This is
especially true on fully observable systems such as LAM, where users
can monitor the full state of the processes and message queues, which
includes the context ID (see 'mpitask'(1) and 'mpimsg'(1)).

LAM supplementary functions do not operate with communicators and
ranks but with nodes and process identifiers.  'MPIL_Comm_gps' accepts
the MPI values and returns the LAM values.  In the case of an
intercommunicator the values returned are those of the process with
the given rank in the remote group.

This is a LAM/MPI-specific function and is intended mainly for
debugging.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     int nid, pid;
     MPIL_Comm_gps(MPI_COMM_WORLD, 0, &nid, &pid);
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_RANK
.N MPI_ERR_ARG

.seealso MPIL_Comm_id, MPIL_Type_id
@*/
int MPIL_Comm_gps(MPI_Comm comm, int rank, int *pnid, 
		  int *ppid)
{
	MPI_Group	grp;

	lam_initerr();
	lam_setfunc(BLKMPILCOMMGPS);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			lam_getfunc(), lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if ((pnid == 0) || (ppid == 0)) {
		return(lam_errfunc(comm,
			lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	grp = (LAM_IS_INTER(comm)) ? comm->c_rgroup : comm->c_group;
	
	if ((rank == MPI_ANY_SOURCE) || (rank < 0) ||
			(rank >= grp->g_nprocs)) {
		return(lam_errfunc(comm,
			lam_getfunc(), lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	*pnid = grp->g_procs[rank]->p_gps.gps_node;
	*ppid = grp->g_procs[rank]->p_gps.gps_pid;

	lam_resetfunc(BLKMPILCOMMGPS);
	return(MPI_SUCCESS);
}

/*
 *	MPIL_Type_id
 *
 *	Function:	- returns the type ID
 *	Accepts:	- datatype
 *			- type ID (out)
 */
/*@

MPIL_Type_id - LAM/MPI-specific function to return the type ID
of an MPI datatype

Input Parameter:
. dtype - MPI datatype (handle)

Output Parameter:
. ptid - datatype ID (integer)

Notes:

In the LAM implementation of MPI, each communicator has an integer
context ID associated with it for synchronizing on different contexts.
This ID is global to all the processes in the communicator''s group,
and uniquely identifies that communicator for each process.  These
properties allow the processes to safely exchange messages without
interference from operations on other communicators.
 
The MPI standard does not provide a way to access/view this
implementation-dependent synchronization since communicators are
opaque objects.  Users do not need such access for normal MPI
operations.  On the other hand, when debugging MPI applications, the
opaque nature of communicators hinders the user''s efforts.  This is
especially true on fully observable systems such as LAM, where users
can monitor the full state of the processes and message queues, which
includes the context ID (see 'mpitask'(1) and 'mpimsg'(1)).

This is a LAM/MPI-specific function and is intended mainly for
debugging.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     int tid
     MPIL_Type_id(MPI_INT, &tid);
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

.seealso: MPIL_Comm_gps, MPIL_Comm_id
@*/
int MPIL_Type_id(MPI_Datatype dtype, int *ptid)
{
	lam_initerr();
	lam_setfunc(BLKMPILTYPEID);

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			lam_getfunc(), lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (ptid == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*ptid = dtype->dt_label;

	lam_resetfunc(BLKMPILTYPEID);
	return(MPI_SUCCESS);
}
