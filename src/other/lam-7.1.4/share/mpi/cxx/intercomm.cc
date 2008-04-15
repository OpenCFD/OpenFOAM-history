// -*- c++ -*-
//
// $Id: intercomm.cc,v 1.4 2003/04/15 01:22:15 adharurk Exp $
//
// Copyright (c) 2001-2003 The Trustees of Indiana University.  
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

#include "mpicxx.h"

MPI::Intercomm MPI::Intercomm::Dup() const
{
  return pmpi_comm.Dup();
}

MPI::Intercomm& MPI::Intercomm::Clone() const
{
  PMPI::Comm* pmpi_inter = &pmpi_comm.Clone();
  MPI::Intercomm* mpi_inter = new MPI::Intercomm(*(PMPI::Intercomm*)pmpi_inter);
  return *mpi_inter;
}

int MPI::Intercomm::Get_remote_size() const
{
  return pmpi_comm.Get_remote_size();
}

MPI::Group MPI::Intercomm::Get_remote_group() const
{
  return pmpi_comm.Get_remote_group();
}

MPI::Intracomm MPI::Intercomm::Merge(bool high)
{
  return pmpi_comm.Merge(high);
}


//
// Extended Collective Operations

MPI::Intercomm
MPI::Intercomm::Create(const Group& group) const
{
  return pmpi_comm.Create(group);
}


MPI::Intercomm
MPI::Intercomm::Split(int color, int key) const
{
  return pmpi_comm.Split(color, key);
}
