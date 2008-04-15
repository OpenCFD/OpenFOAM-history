// -*- c++ -*-
//
// $Id: info.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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
#if LAM_WANT_PROFILE
  //  friend class PMPI::Info;
#endif
  friend class MPI::Comm; //so I can access pmpi_info data member in comm.cc
  friend class MPI::Request; //and also from request.cc

public:
#if LAM_WANT_PROFILE

  // construction / destruction
  Info() { }
  virtual ~Info() {}


  // copy / assignment
  Info(const Info& data) : pmpi_info(data.pmpi_info) { }

  Info(const MPI_Info &i) : pmpi_info(i) { }

  Info& operator=(const Info& data) {
    pmpi_info = data.pmpi_info; return *this; }

  // comparison, don't need for info

  // inter-language operability
  Info& operator= (const MPI_Info &i) {
    pmpi_info = i; return *this; }
  operator MPI_Info () const { return pmpi_info; }
  //  operator MPI_Info* () const { return pmpi_info; }
  operator const PMPI::Info&() const { return pmpi_info; }


#else

  Info() { }
  // copy
  Info(const Info& data) : mpi_info(data.mpi_info) { }

  Info(const MPI_Info &i) : mpi_info(i) { }

  virtual ~Info() {}

  Info& operator=(const Info& data) {
    mpi_info = data.mpi_info; return *this; }

  // comparison, don't need for info

  // inter-language operability
  Info& operator= (const MPI_Info &i) {
    mpi_info = i; return *this; }
  operator MPI_Info () const { return mpi_info; }
  //  operator MPI_Info* () const { return (MPI_Info*)&mpi_info; }

#endif

  static Info Create();

  virtual void Delete(const char* key);

  virtual Info Dup() const; 

  virtual void Free();

  virtual bool Get(const char* key, int valuelen, char* value) const;

  virtual int Get_nkeys() const;

  virtual void Get_nthkey(int n, char* key) const;

  virtual bool Get_valuelen(const char* key, int& valuelen) const;

  virtual void Set(const char* key, const char* value);

protected:
#if LAM_WANT_PROFILE
  PMPI::Info pmpi_info;
#else
  MPI_Info mpi_info;
#endif

};
