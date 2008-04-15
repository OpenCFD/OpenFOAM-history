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
 *	$Id: trivialc.c,v 6.3 2002/10/09 20:55:33 brbarret Exp $
 *
 *	Transmit a message in a two process system.
 */

#include <stdio.h>
#include <mpi.h>

#define BUFSIZE		64

int			buf[BUFSIZE];

int
main(int argc, char* argv[])
{
	int		size, rank;
	MPI_Status	status;
/*
 * Initialize MPI.
 */
	MPI_Init(&argc, &argv);
/*
 * Error check the number of processes.
 * Determine my rank in the world group.
 * The sender will be rank 0 and the receiver, rank 1.
 */
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size < 2) {
		printf("Need at least 2 processes.\n");
		MPI_Finalize();
		return(1);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
/*
 * As rank 0, send a message to rank 1.
 */
	if (0 == rank) {
		MPI_Send(buf, BUFSIZE, MPI_INT, 1, 11, MPI_COMM_WORLD);
		printf("rank %d sent message\n", rank);
	}
/*
 * As rank 1, receive a message from rank 0.
 */
	else if (1 == rank) {
		MPI_Recv(buf, BUFSIZE, MPI_INT, 0, 11, MPI_COMM_WORLD,
				&status);
		printf("rank %d received message\n", rank);
	}

	MPI_Finalize();
	return(0);
}
