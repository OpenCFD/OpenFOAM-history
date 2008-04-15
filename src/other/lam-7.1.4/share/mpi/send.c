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
 * $Id: send.c,v 6.11 2003/02/20 19:57:13 jsquyres Exp $
 *
 *	Function:	- send a message in standard mode
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- destination
 *			- tag
 *			- communicator
 *	Returns: 	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 * external functions
 */
extern void		lam_initerr();
extern void		lam_resetfunc();
extern void		lam_setfunc();
extern int		lam_send();

/*@
    MPI_Send - Performs a basic send

Input Parameters:
+ buf - initial address of send buffer (choice) 
. count - number of elements in send buffer (nonnegative integer) 
. dtyp - datatype of each send buffer element (handle) 
. dest - rank of destination (integer) 
. tag - message tag (integer) 
- comm - communicator (handle) 

Notes:

This function `may` block until the message is received.  Whether or
not 'MPI_Send' blocks depends on factors such as how large the message
is, how many messages are pending to the specific destination, whether
LAMD or C2C communication is being used, etc.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.seealso: MPI_Isend, MPI_Send_init

.N ACK
@*/
int MPI_Send(void *buf, int count, MPI_Datatype dtype, int dest, 
	     int tag, MPI_Comm comm)
{
	int		err;
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPISEND);

	if (tag < 0 || tag > lam_mpi_max_tag) {
		return(lam_err_comm(comm, MPI_ERR_TAG, EINVAL,
				    "out of range"));
	}

	err = lam_send(buf, count, dtype, dest, tag, comm, LAM_RQISEND);

	if (err != MPI_SUCCESS) return(lam_errfunc(comm, BLKMPISEND, err));

	lam_resetfunc_m(BLKMPISEND);
	return(MPI_SUCCESS);
}
