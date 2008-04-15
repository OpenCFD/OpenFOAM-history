// -*- c++ -*-
//
// $Id: perrhandler.h,v 1.4 2003/05/07 17:39:32 jsquyres Exp $
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

class Errhandler {
public:
  // construction
  inline Errhandler()
    : mpi_errhandler(MPI_ERRHANDLER_NULL) {}
  // inter-language operability
  inline Errhandler(const MPI_Errhandler &i)
    : mpi_errhandler(i) {}
  // copy
  inline Errhandler(const Errhandler& e);

  inline virtual ~Errhandler() {}

  inline Errhandler& operator=(const Errhandler& e);

  // comparison
  inline bool operator==(const Errhandler &a);

  inline bool operator!=(const Errhandler &a) {
    return (bool)!(*this == a); }

  // inter-language operability
  inline Errhandler& operator= (const MPI_Errhandler &i) {
    mpi_errhandler = i; return *this; }
 
  inline operator MPI_Errhandler() const { return mpi_errhandler; }
 
  inline operator MPI_Errhandler*() { return &mpi_errhandler; }
  
  //
  // Errhandler access functions
  //
  
  inline virtual void Free(void);

  Comm::Errhandler_fn* handler_fn;

protected:
  MPI_Errhandler mpi_errhandler;

public:
  //this is for ERRORS_THROW_EXCEPTIONS
  //this is called from MPI::Real_init
  // g++ doesn't understand friends so this must be public :(
  inline void init() const {
    (void)MPI_Errhandler_create(&throw_excptn_fctn,
				(MPI_Errhandler *)&mpi_errhandler); 
  }

  //this is for ERRORS_THROW_EXCEPTIONS
  //this is called from MPI::Real_init
  // g++ doesn't understand friends so this must be public :(
  inline void free() const {
    (void)MPI_Errhandler_free((MPI_Errhandler *) &mpi_errhandler); 
  }
};
