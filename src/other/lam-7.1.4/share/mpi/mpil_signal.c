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
 * $Id: mpil_signal.c,v 6.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- delivers a signal to a process
 *	Accepts:	- communicator
 *			- process rank
 *			- signal number
 */

#include <blktype.h>
#include <lam_ksignal.h>
#include <mpi.h>
#include <mpisys.h>
#include <portable.h>
#include <preq.h>
#include <terror.h>

/*@

MPIL_Signal - LAM/MPI-specific function to send a LAM signal to a rank
in an MPI communicator

Input Parameters:
+ comm - MPI communicator (handle)
. rank - rank to send it to
- signo - signal number

Notes:

An asynchronous signal is delivered from one process to another with
MPIL_Signal().  The target process is selected with a communicator and
a process rank within that communicator.  The remaining argument,
signo, identifies the signal to be delivered.  These signals are
completely apart from the signals provided by the underlying operating
system.  LAM signals, defined in <lam_ksignal.h>, are listed below.
 
.n LAM_SIGTRACE   1    unload trace data
.n LAM_SIGC       2    user defined
.n LAM_SIGARECV   3    async receive pending
.n LAM_SIGUDIE    4    terminate
.n LAM_SIGARREST  5    suspend execution
.n LAM_SIGRELEASE 6    continue execution
.n LAM_SIGA       7    user defined
.n LAM_SIGB       8    user defined
.n LAM_SIGFUSE    9    node about to die
.n LAM_SIGSHRINK  10   another node has died

This is a LAM/MPI-specific function.  Most users should not have use
for this function.  If this function is used, it should be used in
conjunction with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     MPIL_Signal(MPI_COMM_WORLD, 0, LAM_SIGUDIE);
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_RANK
.N MPI_ERR_OTHER

.seealso: lam_ksignal
@*/
int MPIL_Signal(MPI_Comm comm, int rank, int signo)
{
	int		err;		/* MPI error code */
	int		nid;		/* process's node ID */
	int		pid;		/* process ID */

	lam_initerr();
	lam_setfunc(BLKMPILSIGNAL);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			lam_getfunc(), lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if ((rank == MPI_ANY_SOURCE) || (rank < 0)
		|| (LAM_IS_INTRA(comm) && rank >= comm->c_group->g_nprocs)
		|| (LAM_IS_INTER(comm) && rank >= comm->c_rgroup->g_nprocs)) {
		
		return(lam_errfunc(comm, lam_getfunc(), 
					lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	err = MPIL_Comm_gps(comm, rank, &nid, &pid);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPILSIGNAL, err));
	}

	if (rpdoom((int4) nid, (int4) SELECT_PID, (int4) pid, (int4) signo)) {
		return(lam_errfunc(comm, lam_getfunc(),
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	lam_resetfunc(BLKMPILSIGNAL);
	return(MPI_SUCCESS);
}
