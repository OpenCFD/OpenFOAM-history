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
 * $Id: alltoall.c,v 1.4 2002/11/23 04:06:57 jsquyres Exp $
 *
 * Example program that sends a message from each node each other node
 * in a blocking pattern.
 */

#include <stdio.h>
#include <mpi.h>


int
main(int argc, char* argv[])
{
  int i;
  int rank;
  int size;
  int tag = 201;
  int receive_message;

  /* Normal MPI startup */

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Check to ensure that we have 2 ranks */

  if (size < 2) {
    printf("This program must be run with 2 or more ranks.\n");
    MPI_Finalize();
    return 1;
  }

  /* Loop over all ranks */

  /* Note that we carefully distinguish between the order of sending
     and receiving.  It is *not* sufficient to have a single loop
     where each rank does a simple "MPI_Send(...); MPI_Recv(...);" --
     doing so can lead to deadlock.  This is a subtlety in the MPI
     standard -- MPI_Send() *may* or *may not* block.  In LAM,
     MPI_Send() will block if a message is too long; it will wait
     until the message has been started to be received on the target
     rank.  Under that size, MPI_Send() will [most likely] return
     immediately regardless of what the target rank is doing.

     Note that this is not a LAM-specific issue; every MPI
     implementation is coded in this way -- that MPI_Send tries to
     complete immediately, but above a certain threshhold (usually
     related a combination of message size and the cumulative size of
     unreceived messages to that target), MPI_Send may block until the
     target rank starts receiving. */

  /* Notice also the use of MPI_STATUS_IGNORE.  Since this example
     program doesn't use the MPI_Status object at all, we use the
     special constant MPI_STATUS_IGNORE to tell MPI not to fill it
     in. */

  /* Finally, notice that this program is actually a poor example of
     the all-to-all communication pattern -- there is some degree of
     serialization in the sending of messages.  For example, rank N
     has to wait for rank 0 to send to it before it can continue.
     That is, rank 0 causes a daisy-chain of reactions that allow the
     rest of the ranks to continue -- each rank will not "start" until
     rank 0 contacts it. */

  for (i = 0; i < size; i++) {
    if (i == rank) {

      /* Skip communicating with myself */

      printf("Rank %d not sending to myself\n", i);
      continue;
    } else if (i < rank) {

      /* If our rank number is *greater* than the target rank, we send
         first, receive second. */

      printf("Rank %d sending message \"%d\" to rank %d\n",
	     rank, rank, i);
      MPI_Send(&rank, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      MPI_Recv(&receive_message, 1, MPI_INT, i, tag, MPI_COMM_WORLD, 
	       MPI_STATUS_IGNORE);
      printf("Rank %d received message \"%d\" from rank %d\n",
	     rank, receive_message, i);
    } else {

      /* If our rank number is *smaller* than the target rank, we
         receive first, send second. */

      MPI_Recv(&receive_message, 1, MPI_INT, i, tag, MPI_COMM_WORLD, 
	       MPI_STATUS_IGNORE);
      printf("Rank %d received message \"%d\" from rank %d\n",
	     rank, receive_message, i);
      printf("Rank %d sending message \"%d\" to rank %d\n",
	     rank, rank, i);
      MPI_Send(&rank, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
    }
  }

  /* All done */

  printf("Rank %d finished\n", rank);
  MPI_Finalize();
  return 0;
}
