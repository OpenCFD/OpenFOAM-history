// -*- c++ -*-
//
// $Id: chapter_10_mpi2.cc,v 1.4 2002/11/23 04:06:44 jsquyres Exp $
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


void example10_2(void);
void example10_3(void);
void example10_4(void);
void example10_5(void);

// Example 10.1 - Please note foocomm is not foo_comm as on Page 273
// so it will not conflict with later uses of foo_comm.

class foocomm : public MPI::Intracomm {
public:
  void Send(const void* buf, int count, const MPI::Datatype& type,
	    int dest, int tag) const
  {
    // Class library functionality
    MPI::Intracomm::Send(buf, count, type, dest, tag);
    // More class library functionality
  }
};

int
main(int argc, char *argv[])
{
  MPI::Init(argc, argv);

  example10_2();
  
  example10_3();
  
  example10_4();

  example10_5();

  MPI::Finalize();
  return 0;
}
  
// Example 10.2

void
example10_2()
{
  MPI::Intracomm bar;
  if (bar == MPI::COMM_NULL)
    cout << "bar is MPI::COMM_NULL" << endl;
}

// Example 10.3

void
example10_3()
{
  MPI::Intracomm foo_comm, bar_comm, baz_comm;
  foo_comm = MPI::COMM_WORLD;
  bar_comm = MPI::COMM_WORLD.Dup();
  baz_comm = bar_comm;
  cout << "Is foocomm equal barcomm? " << (foo_comm == bar_comm) << endl;
  cout << "Is bazcomm equal barcomm? " << (baz_comm == bar_comm) << endl;
}

// Example 10.4 Erroneous Code

void
example10_4()
{
  MPI::Intracomm intra = MPI::COMM_WORLD.Dup();

  // This will show up as an unused variable if compiled with full
  // warnings on, but we keep it here a) because it's in Chapter 10,
  // and b) to show that it works and compiles properly

  MPI::Cartcomm cart(intra);
}

// Example 10.5

void
example10_5()
{
  MPI::Intercomm comm;
  comm = MPI::COMM_NULL;            // assign with COMM_NULL
  if (comm == MPI::COMM_NULL)       // true
    cout << "comm is NULL" << endl;
  if (MPI::COMM_NULL == comm)       // note -- a different function
    cout << "comm is still NULL" << endl;
}
