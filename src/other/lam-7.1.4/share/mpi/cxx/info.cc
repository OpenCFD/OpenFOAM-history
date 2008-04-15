// -*- c++ -*-
//
// $Id: info.cc,v 1.2 2002/10/09 20:58:56 brbarret Exp $
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

MPI::Info
MPI::Info::Create()
{
  return (MPI::Info)PMPI::Info::Create();
}

void
MPI::Info::Delete(const char* key)
{
  pmpi_info.Delete(key);
}

MPI::Info
MPI::Info::Dup() const
{
  return (MPI::Info)pmpi_info.Dup();
}

void
MPI::Info::Free()
{
  pmpi_info.Free();
}

bool
MPI::Info::Get(const char* key, int valuelen, char* value) const
{
  return pmpi_info.Get(key,valuelen,value);
}

int
MPI::Info::Get_nkeys() const
{
  return pmpi_info.Get_nkeys();
}

void
MPI::Info::Get_nthkey(int n, char* key) const
{
  pmpi_info.Get_nthkey(n,key);
}

bool
MPI::Info::Get_valuelen(const char* key, int& valuelen) const
{
  return pmpi_info.Get_valuelen(key, valuelen);
}

void
MPI::Info::Set(const char* key, const char* value)
{
  pmpi_info.Set(key, value);
}
