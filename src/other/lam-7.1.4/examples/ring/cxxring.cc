// -*- c++ -*-
//
// $Id: cxxring.cc,v 1.4 2002/10/09 20:55:30 brbarret Exp $
//
// Copyright (c) 2001-2002 The Trustees of Indiana University.  
//                         All rights reserved.
// Copyright (c) 1998-2001 University of Notre Dame. 
//                         All rights reserved.
// Copyright (c) 1994-1998 The Ohio State University.  
//                         All rights reserved.
// 
// This file is part of the LAM/MPI software package.  For license
// information, see the LICENSE file in the top level directory of the
// LAM/MPI source distribution.
// 
// $HEADER$
//

#include <iostream> 
#include <mpi.h>

using namespace std;

//
// The general scheme of this program is to send a message around a
// "ring" of N MPI processes.  Thank is, rank 0 sends to rank 1, rank
// 1 sends to rank 2, ..., rank (N-1) sends back to rank 0.  This
// process is repeated M times.  The first integer of the message is
// used as a counter.  The first time around the ring, it equals M.
// When rank 0 receives the message, it decrements this first integer.
// When the counter reaches 0, each process sends the message on to
// the next process in the ring and then exits.
//


int
main(int argc, char *argv[])
{
  int message;
  int rank, size, tag, to, from;

  // Start up MPI

  MPI::Init(argc, argv);
  rank = MPI::COMM_WORLD.Get_rank();
  size = MPI::COMM_WORLD.Get_size();

  // Arbitrarily choose 201 to be our tag.  Calculate the rank of the
  // to process in the ring.  Use the modulus operator so that the
  // last process "wraps around" to rank zero.

  message = 0;
  tag = 201;
  to = (rank + 1) % size;
  from = (size + rank - 1) % size;
  
  // If we are the "console" process, get a integer from the user to
  // specify how many times we want to go around the ring

  if (rank == 0) {

    /* Send the message around the ring 5 times */

    message = 5;
    cout << "Rank 0 starting message around the ring -- 1st of "
	 << message << endl;
    MPI::COMM_WORLD.Send(&message, 1, MPI_INT, to, tag);
  }

  // Pass the message around the ring.  The exit mechanism works as
  // follows: the message (a positive integer) is passed around the
  // ring.  Each time is passes rank 0, it is decremented.  When each
  // processes receives the 0 message, it passes it on to the to
  // process and then quits.  By passing the 0 first, every process
  // gets the 0 message and can quit normally.

  // Note our use of ignoring the MPI::Status argument here.  Since
  // we're not using the MPI::Status object at all, we can tell MPI to
  // not even bother to fill one in by leaving off the argument.

  while (1) {
    MPI::COMM_WORLD.Recv(&message, 1, MPI_INT, from, tag);
    cout << "Rank " << rank << " received message of \"" << message
	 << "\" from rank " << from << endl;

    if (rank == 0)
      message--;

    cout << "Rank " << rank << " sending message of \"" << message
	 << "\" to rank " << to << endl;
    MPI::COMM_WORLD.Send(&message, 1, MPI_INT, to, tag);

    if (message == 0) {
      cout << "Rank " << rank << " all done; quitting loop" << endl;
      break;
    }
  }

  // Do the final receive

  if (rank == 0) {
    MPI::COMM_WORLD.Recv(&message, 1, MPI_INT, from, tag);
    cout << "Rank 0 consumed final message" << endl;
  }

  // Quit

  MPI::Finalize();
  return 0;
}
