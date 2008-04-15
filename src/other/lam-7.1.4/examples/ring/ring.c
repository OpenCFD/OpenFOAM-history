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
 * $Id: ring.c,v 1.4 2002/10/09 20:55:30 brbarret Exp $
 *
 * Simple example of MPI showing the ring message pattern.
 */

#include <stdio.h>
#include "mpi.h"

/*
 * The general scheme of this program is to send a message around a
 * "ring" of N MPI processes.  Thank is, rank 0 sends to rank 1, rank
 * 1 sends to rank 2, ..., rank (N-1) sends back to rank 0.  This
 * process is repeated M times.  The first integer of the message is
 * used as a counter.  The first time around the ring, it equals M.
 * When rank 0 receives the message, it decrements this first integer.
 * When the counter reaches 0, each process sends the message on to
 * the next process in the ring and then exits.
 */


int 
main(int argc, char **argv)
{
  int message;
  int rank, size, tag, to, from;

  /* Start up MPI */

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
 
  /* Arbitrarily choose 201 to be our tag.  Calculate the */
  /* rank of the to process in the ring.  Use the modulus */
  /* operator so that the last process "wraps around" to rank */
  /* zero. */

  message = 0;
  tag = 201;
  to = (rank + 1) % size;
  from = (rank + size - 1) % size;

  /* If we are the "console" process, get a integer from the */
  /* user to specify how many times we want to go around the */
  /* ring */

  if (rank == 0) {

    /* Send the message around the ring 5 times */

    message = 5;
    printf("Rank 0 starting message around the ring -- 1st of %d\n", 
	   message);
    MPI_Send(&message, 1, MPI_INT, to, tag, MPI_COMM_WORLD); 
  }

  /* Pass the message around the ring.  The exit mechanism works */
  /* as follows: the message (a positive integer) is passed */
  /* around the ring.  Each time is passes rank 0, it is decremented. */
  /* When each processes receives the 0 message, it passes it on */
  /* to the to process and then quits.  By passing the 0 first, */
  /* every process gets the 0 message and can quit normally. */

  /* Note our use of MPI_STATUS_IGNORE here.  Since we're not using
     the MPI_Status object at all, we can tell MPI to not even bother
     to fill one in with the special MPI_STATUS_IGNORE constant. */

  while (1) {
    MPI_Recv(&message, 1, MPI_INT, from, tag, MPI_COMM_WORLD, 
	     MPI_STATUS_IGNORE);
    printf("Rank %d received message of \"%d\" from rank %d\n",
	   rank, message, from);

    if (rank == 0)
      message--;

    printf("Rank %d sending message of \"%d\" to rank %d\n",
	   rank, message, to);
    MPI_Send(&message, 1, MPI_INT, to, tag, MPI_COMM_WORLD);

    if (message == 0) {
      printf("Rank %d all done; quitting loop\n", rank);
      break;
    }
  }

  /* Do the final receive */

  if (rank == 0) {
    MPI_Recv(&message, 1, MPI_INT, from, tag, MPI_COMM_WORLD, 
	     MPI_STATUS_IGNORE);
    printf("Rank 0 consumed final message\n");
  }

  /* Quit */

  MPI_Finalize();
  return 0;
}
