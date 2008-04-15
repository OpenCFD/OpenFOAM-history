// -*- c++ -*-
//
// $Id: status_inln.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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
// Point-to-Point Communication
//

inline int
_REAL_MPI_::Status::Get_count(const _REAL_MPI_::Datatype& datatype) const
{
  int count;
  //(MPI_Status*) is to cast away the const
  (void)MPI_Get_count((MPI_Status*)&mpi_status, datatype, &count);
  return count;
}

inline bool
_REAL_MPI_::Status::Is_cancelled() const
{
  int t;
  (void)MPI_Test_cancelled((MPI_Status*)&mpi_status, &t);
  return (bool) t;
}

inline int
_REAL_MPI_::Status::Get_elements(const _REAL_MPI_::Datatype& datatype) const
{
  int count;
  (void)MPI_Get_elements((MPI_Status*)&mpi_status, datatype, &count);
  return count;
}

//
// Status Access
//
inline int
_REAL_MPI_::Status::Get_source() const
{
  int source;
  source = mpi_status.MPI_SOURCE;
  return source;
}

inline void
_REAL_MPI_::Status::Set_source(int source)
{
  mpi_status.MPI_SOURCE = source;
}

inline int
_REAL_MPI_::Status::Get_tag() const
{
  int tag;
  tag = mpi_status.MPI_TAG;
  return tag;
}

inline void
_REAL_MPI_::Status::Set_tag(int tag)
{
  mpi_status.MPI_TAG = tag;
}

inline int
_REAL_MPI_::Status::Get_error() const
{
  int error;
  error = mpi_status.MPI_ERROR;
  return error;
}

inline void
_REAL_MPI_::Status::Set_error(int error)
{
  mpi_status.MPI_ERROR = error;
}
