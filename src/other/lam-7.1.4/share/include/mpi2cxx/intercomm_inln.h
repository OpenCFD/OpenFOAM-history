// -*- c++ -*-
//
// $Id: intercomm_inln.h,v 1.4 2003/04/15 01:37:20 adharurk Exp $
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

inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intercomm::Dup() const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_dup(mpi_comm, &newcomm);
  return newcomm;
}

inline _REAL_MPI_::Intercomm&
_REAL_MPI_::Intercomm::Clone() const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_dup(mpi_comm, &newcomm);
  _REAL_MPI_::Intercomm* dup = new _REAL_MPI_::Intercomm(newcomm);
  return *dup;
}

inline int
_REAL_MPI_::Intercomm::Get_remote_size() const 
{
  int size;
  (void)MPI_Comm_remote_size(mpi_comm, &size);
  return size;
}

inline _REAL_MPI_::Group
_REAL_MPI_::Intercomm::Get_remote_group() const 
{
  MPI_Group group;
  (void)MPI_Comm_remote_group(mpi_comm, &group);
  return group;
}

inline _REAL_MPI_::Intracomm
_REAL_MPI_::Intercomm::Merge(bool high)
{
  MPI_Comm newcomm;
  (void)MPI_Intercomm_merge(mpi_comm, (int)high, &newcomm);
  return newcomm;
}


//
// Extended Collective Operations
//

inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intercomm::Create(const Group& group) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_create(mpi_comm, (MPI_Group) group, &newcomm);
  return newcomm;
}

inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intercomm::Split(int color, int key) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_split(mpi_comm, color, key, &newcomm);
  return newcomm;
}
