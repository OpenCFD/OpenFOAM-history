// -*- c++ -*-
//
// $Id: info_inln.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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


inline _REAL_MPI_::Info 
_REAL_MPI_::Info::Create()
{
  MPI_Info newinfo;
  (void) MPI_Info_create(&newinfo);
  return newinfo;
}

inline void
_REAL_MPI_::Info::Delete(const char* key)
{
   (void)MPI_Info_delete(mpi_info, (char*)key);
}

inline _REAL_MPI_::Info 
_REAL_MPI_::Info::Dup() const
{
  MPI_Info newinfo;
  (void)MPI_Info_dup(mpi_info, &newinfo);
  return newinfo;
}

inline void
_REAL_MPI_::Info::Free()
{
  (void) MPI_Info_free(&mpi_info);
}

inline bool
_REAL_MPI_::Info::Get(const char* key, int valuelen, char* value) const
{
  int flag;
  (void)MPI_Info_get(mpi_info, (char*)key, valuelen, value, &flag);
  return (bool) flag;
}

inline int
_REAL_MPI_::Info::Get_nkeys() const
{
  int nkeys;
  MPI_Info_get_nkeys(mpi_info, &nkeys);
  return nkeys;
}

inline void
_REAL_MPI_::Info::Get_nthkey(int n, char* key) const
{
  (void) MPI_Info_get_nthkey(mpi_info, n, key);
}

inline bool 
_REAL_MPI_::Info::Get_valuelen(const char* key, int& valuelen) const
{
  int flag;
  (void) MPI_Info_get_valuelen(mpi_info, (char*)key, &valuelen, &flag);
  return (bool) flag;
}

inline void
_REAL_MPI_::Info::Set(const char* key, const char* value)
{
  (void) MPI_Info_set(mpi_info, (char*)key, (char*)value);
}
