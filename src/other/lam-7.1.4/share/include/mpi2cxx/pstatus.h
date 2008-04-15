// -*- c++ -*-
//
// $Id: pstatus.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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
  friend class PMPI::Comm; //so I can access pmpi_status data member in comm.cc
  friend class PMPI::Request; //and also from request.cc

public:
  // construction
  inline Status() { }
  // copy
  Status(const Status& data) : mpi_status(data.mpi_status) { }

  inline Status(const MPI_Status &i) : mpi_status(i) { }

  inline virtual ~Status() {}

  Status& operator=(const Status& data) {
    mpi_status = data.mpi_status; return *this;
  } 

  // comparison, don't need for status

  // inter-language operability
  inline Status& operator= (const MPI_Status &i) {
    mpi_status = i; return *this; }
  inline operator MPI_Status () const { return mpi_status; }
  inline operator MPI_Status* () const { return (MPI_Status*)&mpi_status; }

  //
  // Point-to-Point Communication
  //

  inline virtual int Get_count(const Datatype& datatype) const;

  inline virtual bool Is_cancelled() const;

  virtual int Get_elements(const Datatype& datatype) const;

  //
  // Status Access
  //
  inline virtual int Get_source() const;

  inline virtual void Set_source(int source);
  
  inline virtual int Get_tag() const;

  inline virtual void Set_tag(int tag);
  
  inline virtual int Get_error() const;

  inline virtual void Set_error(int error);
  
protected:
  MPI_Status mpi_status;

};
