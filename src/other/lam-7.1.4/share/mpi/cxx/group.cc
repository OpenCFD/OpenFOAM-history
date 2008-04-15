// -*- c++ -*-
//
// $Id: group.cc,v 1.2 2002/10/09 20:58:56 brbarret Exp $
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

int MPI::Group::Get_size() const
{
  return pmpi_group.Get_size();
}

int MPI::Group::Get_rank() const 
{
  return pmpi_group.Get_rank();
}

void MPI::Group::Translate_ranks(const MPI::Group& group1, int n, const int ranks1[], 
				 const MPI::Group& group2, int ranks2[])
{
  PMPI::Group::Translate_ranks(group1, n, ranks1, group2, ranks2);
}

int MPI::Group::Compare(const MPI::Group& group1, const MPI::Group& group2)
{
  return PMPI::Group::Compare(group1, group2);
}

MPI::Group MPI::Group::Union(const MPI::Group &group1, const MPI::Group &group2)
{
  return PMPI::Group::Union(group1, group2);
}

MPI::Group MPI::Group::Intersect(const MPI::Group &group1, const MPI::Group &group2)
{
  return PMPI::Group::Intersect(group1, group2);
}

MPI::Group MPI::Group::Difference(const MPI::Group &group1, const MPI::Group &group2)
{
  return PMPI::Group::Difference(group1, group2);
}

MPI::Group MPI::Group::Incl(int n, const int ranks[]) const
{
  return pmpi_group.Incl(n, ranks);
}

MPI::Group MPI::Group::Excl(int n, const int ranks[]) const
{
  return pmpi_group.Excl(n, ranks);
}

MPI::Group MPI::Group::Range_incl(int n, const int ranges[][3]) const
{
  return pmpi_group.Range_incl(n, ranges);
}

MPI::Group MPI::Group::Range_excl(int n, const int ranges[][3]) const
{
  return pmpi_group.Range_excl(n, ranges);
}

void MPI::Group::Free()
{
  pmpi_group.Free();
}
