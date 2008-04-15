// -*- c++ -*-
//
// $Id: cxxpi.cc,v 1.4 2002/11/23 04:06:44 jsquyres Exp $
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

#include <math.h>
#include <iostream>
#include <mpi.h>

using namespace std;


int
main(int argc, char *argv[])
{
  int n, rank, size, i;
  double PI25DT = 3.141592653589793238462643;
  double mypi, pi, h, sum, x;

  MPI::Init(argc, argv);
  size = MPI::COMM_WORLD.Get_size();
  rank = MPI::COMM_WORLD.Get_rank();

  // Prompt the user for how many iterations to use
  
  if (rank == 0) {
    //cout << "Enter the number of intervals: (0 quits)" << endl;
    //cin >> n;

    // Hardwired for simplicity.  Feel free to uncomment the previous
    // lines to get user input.

    n = 10000;
  }

  // Broadcast the number of intervals
  
  MPI::COMM_WORLD.Bcast(&n, 1, MPI::INT, 0);
  
  if (n > 0) {
    h = 1.0 / (double) n;
    sum = 0.0;
    for (i = rank + 1; i <= n; i += size) {
      x = h * ((double)i - 0.5);
      sum += (4.0 / (1.0 + x*x));
    }
    mypi = h * sum;
    
    // Combine all the partial results
    MPI::COMM_WORLD.Reduce(&mypi, &pi, 1, MPI::DOUBLE, MPI::SUM, 0);
    
    if (rank == 0)
      cout << "After " << n << " iterations, pi is approximately " 
	   << pi << ", Error is " << fabs(pi - PI25DT) << endl;
  }

  MPI::Finalize();
  return 0;
}
