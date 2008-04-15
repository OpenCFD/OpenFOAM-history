// -*- c++ -*-
//
// $Id: errhandler_inln.h,v 1.4 2002/10/09 20:57:36 brbarret Exp $
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

#if LAM_WANT_PROFILE

inline PMPI::Errhandler::Errhandler(const PMPI::Errhandler& e)
  : handler_fn(e.handler_fn), mpi_errhandler(e.mpi_errhandler) { }

inline PMPI::Errhandler&
PMPI::Errhandler::operator=(const PMPI::Errhandler& e)
{
  handler_fn = e.handler_fn;
  mpi_errhandler = e.mpi_errhandler;
  return *this;
}

inline bool
PMPI::Errhandler::operator==(const PMPI::Errhandler &a)
{
  return (MPI2CPP_BOOL_T)(mpi_errhandler == a.mpi_errhandler);
}

#endif

inline void
_REAL_MPI_::Errhandler::Free()
{
  (void)MPI_Errhandler_free(&mpi_errhandler);
}




