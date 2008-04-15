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
 * $Id: abort.c,v 6.13 2002/12/11 14:14:08 jsquyres Exp $
 *
 *	Function:	- attempts to abort all processes in group
 *			- prints error message
 *	Accepts:	- communicator
 *			- error code
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>
#include <blktype.h>
#include <ksignal.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <preq.h>
#include <rpisys.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif


/*@
   MPI_Abort - Terminates MPI execution environment

Input Parameters:
+ comm - communicator of tasks to abort 
- errcode - error code to return to invoking environment 

Notes:

Makes a "best attempt" to terminate all MPI processes associated with
the communicator 'comm'; for non-IMPI jobs, this will terminate all
processes.  'MPI_Abort' will hang, however, if the LAM daemon on the
node on which a rank was running has died.  For example, if a node
gets powered off in the middle of an MPI run and 'MPI_Abort' is
invoked, it may hang trying to contact the LAM daemon on the downed
node.

In the case of an IMPI job, in addition to terminating all local IMPI
procs, this function will also send an abort message to the local IMPI
host, which will, in turn send "closing" messages to all the other
IMPI hosts.  Their behavior upon receipt of these "closing" messages
is implementation dependant.

If the LAM IMPI host receives a premature "closing" message from a
remote IMPI host, it will attempt to continue processing.  Any new
messages to that host will likely hang, however.

'errcode' is split into three sections:

+ lower 8 bits: error class
. next 9 bits: function enumeration (there are over 300 MPI functions;
it takes 9 bits to represent all possibilities)
- upper 15 bits: error code

If the error class is nonzero, the error code is the POSIX return
value for the program (i.e., exit(error_code) is eventually called to
terminate the program).  If the error class is zero, 'errcode' is
returned as the POSIX return value for the program.

This is mainly because 'MPI_Abort' is used internally in LAM/MPI to
abort MPI upon error; LAM packs all three pieces of data into the
errcode.  Users who wish to return a specific value from the program
should use something similar to:

.vb
  return ((return_code << 17) + 1);
.ve

.N fortran

.N ACK
@*/
int MPI_Abort(MPI_Comm comm, int errcode)
{
	int		size;			/* group size */
	int		rank;			/* process rank */
	int		err;			/* error code */
	int		class;			/* error class */
	int		i;
	struct _proc	**p;

	lam_initerr();
	lam_setfunc(BLKMPIABORT);
/*
 * Switch error code to extracted errno value.  If the class is 0 then
 * the errno is taken to be the code.
 */
	lam_bkerr(errcode, &class, &i, &err);
	if (class != 0) 
	  errcode = err;

	if (comm == MPI_COMM_NULL) 
	  comm = MPI_COMM_WORLD;

	err = MPI_Comm_size(comm, &size);
	if (err != MPI_SUCCESS) 
	  kexit(errcode);

	err = MPI_Comm_rank(comm, &rank);
	if (err != MPI_SUCCESS) 
	  kexit(errcode);

#if LAM_WANT_IMPI
	/* Tell the IMPI client daemon to abort.  IMPI_Abort simply
	   sends a message to the IMPI client daemon telling it to
	   quit; it does not perform any cleanup stuff. */

	if (LAM_IS_IMPI(MPI_COMM_WORLD))
	  IMPI_Abort();
#endif

/*
 * Doom all other processes in the group if we know their pids.
 */
	if ((_kio.ki_rtf & RTF_MPIRUN) || (_kio.ki_parent > 0)) {
		p = comm->c_group->g_procs;

		for (i = 0; i < size; ++i, ++p) {

			if (i != rank) {
				rpdoom((*p)->p_gps.gps_node, SELECT_PID,
						(*p)->p_gps.gps_pid, SIGUDIE);
			}
		}
	}
/*
 * Clean up any published names.
 */
	lam_nukepublished();

/*
 * Abort the local process if in the group.
 */
	if (rank >= 0) {
		kexit(errcode);
	}

	lam_resetfunc(BLKMPIABORT);
	return(MPI_SUCCESS);
}
