// -*- c++ -*-
//
// $Id: errhandler.h,v 1.5 2003/06/02 03:05:12 jsquyres Exp $
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

#if LAM_WANT_PROFILE

  // construction / destruction
  inline Errhandler() { }

  inline virtual ~Errhandler() { }

  inline Errhandler(const MPI_Errhandler &i)
    : pmpi_errhandler(i) { }

 // copy / assignment
  inline Errhandler(const Errhandler& e)
    : pmpi_errhandler(e.pmpi_errhandler) { }

  inline Errhandler(const PMPI::Errhandler& e)
    : pmpi_errhandler(e) { }

  inline Errhandler& operator=(const Errhandler& e) {
    pmpi_errhandler = e.pmpi_errhandler; return *this; }

  // comparison
  inline bool operator==(const Errhandler &a) {
    return (bool)(pmpi_errhandler == a.pmpi_errhandler); }
  
  inline bool operator!=(const Errhandler &a) {
    return (bool)!(*this == a); }

  // inter-language operability
  inline Errhandler& operator= (const MPI_Errhandler &i) {
    pmpi_errhandler = i; return *this; }
 
  inline operator MPI_Errhandler() const { return pmpi_errhandler; }
 
  //  inline operator MPI_Errhandler*() { return pmpi_errhandler; }
  
  inline operator const PMPI::Errhandler&() const { return pmpi_errhandler; }

#else

  // construction / destruction
  inline Errhandler()
    : mpi_errhandler(MPI_ERRHANDLER_NULL) {}

  inline virtual ~Errhandler() { }

  inline Errhandler(const MPI_Errhandler &i)
    : mpi_errhandler(i) {}

 // copy / assignment
  inline Errhandler(const Errhandler& e)
    : handler_fn(e.handler_fn), mpi_errhandler(e.mpi_errhandler) { }

  inline Errhandler& operator=(const Errhandler& e)
  {
    mpi_errhandler = e.mpi_errhandler;
    handler_fn = e.handler_fn;
    return *this;
  }

  // comparison
  inline bool operator==(const Errhandler &a) {
    return (bool)(mpi_errhandler == a.mpi_errhandler); }
  
  inline bool operator!=(const Errhandler &a) {
    return (bool)!(*this == a); }

  // inter-language operability
  inline Errhandler& operator= (const MPI_Errhandler &i) {
    mpi_errhandler = i; return *this; }
 
  inline operator MPI_Errhandler() const { return mpi_errhandler; }
 
  //  inline operator MPI_Errhandler*() { return &mpi_errhandler; }
  
#endif

  //
  // Errhandler access functions
  //
  
  virtual void Free();

#if !LAM_WANT_PROFILE
  Comm::Errhandler_fn* handler_fn;
#endif

protected:
#if LAM_WANT_PROFILE
  PMPI::Errhandler pmpi_errhandler;
#else
  MPI_Errhandler mpi_errhandler;
#endif


public:
  // took out the friend decls
  //private:

  //this is for ERRORS_THROW_EXCEPTIONS
  //this is called from MPI::Real_init
  inline void init() const {
#if ! LAM_WANT_PROFILE
    // $%%@#%# AIX/POE 2.3.0.0 makes us put in this cast here
    (void)MPI_Errhandler_create((MPI_Handler_function*) &throw_excptn_fctn,
				(MPI_Errhandler *) &mpi_errhandler); 
#else
    pmpi_errhandler.init();
#endif
  }

  //this is for ERRORS_THROW_EXCEPTIONS
  //this is called from MPI::Finalize
  inline void free() const {
#if ! LAM_WANT_PROFILE
    (void)MPI_Errhandler_free((MPI_Errhandler *) &mpi_errhandler); 
#else
    pmpi_errhandler.free();
#endif
  }
};
