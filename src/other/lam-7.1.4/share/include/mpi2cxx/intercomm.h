// -*- c++ -*-
//
// $Id: intercomm.h,v 1.5 2003/04/15 01:37:20 adharurk Exp $
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


class Intercomm : public Comm {
#if LAM_WANT_PROFILE
  //  friend class PMPI::Intercomm;
#endif
public:

  // construction
  Intercomm() : Comm(MPI_COMM_NULL) { }
  // copy
  Intercomm(const Comm_Null& data) : Comm(data) { }
  // inter-language operability
  Intercomm(const MPI_Comm& data) : Comm(data) { }

#if LAM_WANT_PROFILE
  // copy
  Intercomm(const Intercomm& data) : Comm(data), pmpi_comm(data.pmpi_comm) { }
  Intercomm(const PMPI::Intercomm& d) : 
    Comm((const PMPI::Comm&)d), pmpi_comm(d) { }

  // assignment
  Intercomm& operator=(const Intercomm& data) {
    Comm::operator=(data);
    pmpi_comm = data.pmpi_comm; return *this; }
  Intercomm& operator=(const Comm_Null& data) {
    Comm::operator=(data);
    Intercomm& ic = (Intercomm&)data;
    pmpi_comm = ic.pmpi_comm; return *this; }
  // inter-language operability
  Intercomm& operator=(const MPI_Comm& data) {
    Comm::operator=(data);
    pmpi_comm = PMPI::Intercomm(data); return *this; }
#else
  // copy
  Intercomm(const Intercomm& data) : Comm(data.mpi_comm) { }
  // assignment
  Intercomm& operator=(const Intercomm& data) {
    mpi_comm = data.mpi_comm; return *this; }
  Intercomm& operator=(const Comm_Null& data) {
    mpi_comm = data; return *this; }
  // inter-language operability
  Intercomm& operator=(const MPI_Comm& data) {
    mpi_comm = data; return *this; } 

#endif
  

  //
  // Groups, Contexts, and Communicators
  //

  Intercomm Dup() const;

  virtual Intercomm& Clone() const;

  virtual int Get_remote_size() const;

  virtual Group Get_remote_group() const;

  virtual Intracomm Merge(bool high);

  
  //
  // Extended Collective Operations
  //

  virtual Intercomm Create(const Group& group) const;

  virtual Intercomm Split(int color, int key) const;


  //#if LAM_WANT_PROFILE
  //  virtual const PMPI::Comm& get_pmpi_comm() const { return pmpi_comm; }
  //#endif

#if LAM_WANT_PROFILE
private:
  PMPI::Intercomm pmpi_comm;
#endif
};
