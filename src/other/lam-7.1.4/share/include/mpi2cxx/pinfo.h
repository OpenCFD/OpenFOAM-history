// -*- c++ -*-
//
// $Id: pinfo.h,v 1.2 2002/10/09 20:57:38 brbarret Exp $
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

class Info {
  friend class PMPI::Comm; //so I can access pmpi_info data member in comm.cc
  friend class PMPI::Request; //and also from request.cc

public:
  // construction
  inline Info() { }
  // copy
  Info(const Info& data) : mpi_info(data.mpi_info) { }

  inline Info(const MPI_Info &i) : mpi_info(i) { }

  inline virtual ~Info() {}

  Info& operator=(const Info& data) {
    mpi_info = data.mpi_info; return *this;
  } 

  // comparison, don't need for info

  // inter-language operability
  inline Info& operator= (const MPI_Info &i) {
    mpi_info = i; return *this; }
  inline operator MPI_Info () const { return mpi_info; }
  inline operator MPI_Info* () const { return (MPI_Info*)&mpi_info; }

  static Info Create();

  inline virtual void Delete(const char* key);

  inline virtual Info Dup() const; 

  inline virtual void Free();

  inline virtual bool Get(const char* key, int valuelen, char* value) const;

  inline virtual int Get_nkeys() const;

  inline virtual void Get_nthkey(int n, char* key) const;

  inline virtual bool Get_valuelen(const char* key, int& valuelen) const;

  inline virtual void Set(const char* key, const char* value);

protected:
  MPI_Info mpi_info;

};
