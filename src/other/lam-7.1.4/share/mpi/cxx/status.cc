// -*- c++ -*-
//
// $Id: status.cc,v 1.3 2002/10/09 20:58:57 brbarret Exp $
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

#include "mpicxx.h"

//
// Point-to-Point Communication
//

int
MPI::Status::Get_count(const MPI::Datatype& datatype) const
{
  return pmpi_status.Get_count(datatype);
}

bool
MPI::Status::Is_cancelled() const
{
  return pmpi_status.Is_cancelled();
}

int
MPI::Status::Get_elements(const MPI::Datatype& datatype) const
{
  return pmpi_status.Get_elements(datatype);
}

//
// Status Access
//
int
MPI::Status::Get_source() const
{
  return pmpi_status.Get_source();
}

void
MPI::Status::Set_source(int source)
{
  pmpi_status.Set_source(source);
}
  
int
MPI::Status::Get_tag() const
{
  return pmpi_status.Get_tag();
}
  
void
MPI::Status::Set_tag(int tag)
{
  pmpi_status.Set_tag(tag);
}
  
int
MPI::Status::Get_error() const
{
  return pmpi_status.Get_error();
}

void
MPI::Status::Set_error(int error)
{
  pmpi_status.Set_error(error);
}
