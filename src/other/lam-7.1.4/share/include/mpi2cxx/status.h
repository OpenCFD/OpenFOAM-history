// -*- c++ -*-
//
// $Id: status.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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


class Status {
#if LAM_WANT_PROFILE
  //  friend class PMPI::Status;
#endif
  friend class MPI::Comm; //so I can access pmpi_status data member in comm.cc
  friend class MPI::Request; //and also from request.cc

public:
#if LAM_WANT_PROFILE

  // construction / destruction
  Status() { }
  virtual ~Status() {}

  // copy / assignment
  Status(const Status& data) : pmpi_status(data.pmpi_status) { }

  Status(const MPI_Status &i) : pmpi_status(i) { }

  Status& operator=(const Status& data) {
    pmpi_status = data.pmpi_status; return *this; }

  // comparison, don't need for status

  // inter-language operability
  Status& operator= (const MPI_Status &i) {
    pmpi_status = i; return *this; }
  operator MPI_Status () const { return pmpi_status; }
  //  operator MPI_Status* () const { return pmpi_status; }
  operator const PMPI::Status&() const { return pmpi_status; }

#else

  Status() { }
  // copy
  Status(const Status& data) : mpi_status(data.mpi_status) { }

  Status(const MPI_Status &i) : mpi_status(i) { }

  virtual ~Status() {}

  Status& operator=(const Status& data) {
    mpi_status = data.mpi_status; return *this; }

  // comparison, don't need for status

  // inter-language operability
  Status& operator= (const MPI_Status &i) {
    mpi_status = i; return *this; }
  operator MPI_Status () const { return mpi_status; }
  //  operator MPI_Status* () const { return (MPI_Status*)&mpi_status; }

#endif

  //
  // Point-to-Point Communication
  //

  virtual int Get_count(const Datatype& datatype) const;

  virtual bool Is_cancelled() const;

  virtual int Get_elements(const Datatype& datatype) const;

  //
  // Status Access
  //
  virtual int Get_source() const;

  virtual void Set_source(int source);

  virtual int Get_tag() const;
  
  virtual void Set_tag(int tag);
  
  virtual int Get_error() const;

  virtual void Set_error(int error);

protected:
#if LAM_WANT_PROFILE
  PMPI::Status pmpi_status;
#else
  MPI_Status mpi_status;
#endif

};
