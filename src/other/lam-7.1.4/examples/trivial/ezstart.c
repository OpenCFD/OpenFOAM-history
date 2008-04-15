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
 *	$Id: ezstart.c,v 6.7 2002/10/09 20:55:33 brbarret Exp $
 *
 */


#include <mpi.h>
#include <unistd.h>

#define WORKTAG		1
#define DIETAG		2
#define NUM_WORK_REQS	200

/*
 * Local functions
 */
static void		master(void);
static void		slave(void);


/*
 * main
 * This program is really MIMD, bit is written SPMD for
 * simplicity in launching the application.
 */
int
main(int argc, char* argv[])
{
	int		myrank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,	/* group of everybody */
			&myrank);	/* 0 thru N-1 */

	if (myrank == 0) {
		master();
	} else {
		slave();
	}

	MPI_Finalize();
	return(0);
}

/*
 * master
 * The master process sends work requests to the slaves
 * and collects results.
 */
static void
master(void)
{
	int		ntasks, rank, work;
	double		result;
	MPI_Status	status;

	MPI_Comm_size(MPI_COMM_WORLD,
			&ntasks);	/* #processes in app */
/*
 * Seed the slaves.
 */
	work = NUM_WORK_REQS;		/* simulated work */

	for (rank = 1; rank < ntasks; ++rank) {

		MPI_Send(&work,		/* message buffer */
			1,		/* one data item */
			MPI_INT,	/* of this type */
			rank,		/* to this rank */
			WORKTAG,	/* a work message */
			MPI_COMM_WORLD);/* always use this */
		work--;
	}
/*
 * Receive a result from any slave and dispatch a new work
 * request until work requests have been exhausted.
 */
	while (work > 0) {

		MPI_Recv(&result,	/* message buffer */
			1,		/* one data item */
			MPI_DOUBLE,	/* of this type */
			MPI_ANY_SOURCE,	/* from anybody */
			MPI_ANY_TAG,	/* any message */
			MPI_COMM_WORLD,	/* communicator */
			&status);	/* recv'd msg info */

		MPI_Send(&work, 1, MPI_INT, status.MPI_SOURCE,
				WORKTAG, MPI_COMM_WORLD);

		work--;			/* simulated work */
	}
/*
 * Receive results for outstanding work requests.
 */
	for (rank = 1; rank < ntasks; ++rank) {
		MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
/*
 * Tell all the slaves to exit.
 */
	for (rank = 1; rank < ntasks; ++rank) {
		MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
	}
}

/*
 * slave
 * Each slave process accepts work requests and returns
 * results until a special termination request is received.
 */
static void
slave(void)
{
	double		result;
	int		work;
	MPI_Status	status;

	for (;;) {
		MPI_Recv(&work, 1, MPI_INT, 0, MPI_ANY_TAG,
				MPI_COMM_WORLD, &status);
/*
 * Check the tag of the received message.
 */
		if (status.MPI_TAG == DIETAG) {
			return;
		}

		sleep(2);
		result = 6.0;		/* simulated result */

		MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
}
