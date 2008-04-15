// -*- c++ -*-
//
// $Id: cxxhello.cc,v 1.4 2002/11/23 04:06:44 jsquyres Exp $
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


int
main(int argc, char *argv[])
{
  MPI::Init(argc, argv);
  
  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  cout << "Hello World! I am " << rank << " of " << size << endl;
  
  MPI::Finalize();
  return 0;
}
