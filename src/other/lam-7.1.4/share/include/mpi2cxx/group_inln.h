// -*- c++ -*-
//
// $Id: group_inln.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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

//
// Groups, Contexts, and Communicators
//

inline int
_REAL_MPI_::Group::Get_size() const
{
  int size;
  (void)MPI_Group_size(mpi_group, &size);
  return size;
}

inline int
_REAL_MPI_::Group::Get_rank() const 
{
  int rank;
  (void)MPI_Group_rank(mpi_group, &rank);
  return rank;
}

inline void
_REAL_MPI_::Group::Translate_ranks (const _REAL_MPI_::Group& group1, int n,
				    const int ranks1[], 
				    const _REAL_MPI_::Group& group2, int ranks2[])
{
  (void)MPI_Group_translate_ranks(group1, n, (int*)ranks1, group2, (int*)ranks2);
}

inline int
_REAL_MPI_::Group::Compare(const _REAL_MPI_::Group& group1, const _REAL_MPI_::Group& group2)
{
  int result;
  (void)MPI_Group_compare(group1, group2, &result);
  return result;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Union(const _REAL_MPI_::Group &group1, const _REAL_MPI_::Group &group2)
{
  MPI_Group newgroup;
  (void)MPI_Group_union(group1, group2, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Intersect(const _REAL_MPI_::Group &group1, const _REAL_MPI_::Group &group2)
{
  MPI_Group newgroup;
  (void)MPI_Group_intersection( group1,  group2, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Difference(const _REAL_MPI_::Group &group1, const _REAL_MPI_::Group &group2)
{
  MPI_Group newgroup;  
  (void)MPI_Group_difference(group1, group2, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Incl(int n, const int ranks[]) const
{
  MPI_Group newgroup;
  (void)MPI_Group_incl(mpi_group, n, (int*)ranks, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Excl(int n, const int ranks[]) const
{
  MPI_Group newgroup;
  (void)MPI_Group_excl(mpi_group, n, (int*)ranks, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Range_incl(int n, const int ranges[][3]) const
{
  MPI_Group newgroup;
  (void)MPI_Group_range_incl(mpi_group, n, (int(*)[3])ranges, &newgroup);
  return newgroup;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Group::Range_excl(int n, const int ranges[][3]) const
{
  MPI_Group newgroup;
  (void)MPI_Group_range_excl(mpi_group, n, (int(*)[3])ranges, &newgroup);
  return newgroup;
}

inline void
_REAL_MPI_::Group::Free()
{
  (void)MPI_Group_free(&mpi_group);
}
