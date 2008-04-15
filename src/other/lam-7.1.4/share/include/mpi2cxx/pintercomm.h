// -*- c++ -*-
//
// $Id: pintercomm.h,v 1.4 2003/04/15 01:37:20 adharurk Exp $
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
public:

  // construction
  Intercomm() : Comm(MPI_COMM_NULL) { }
  // copy
  Intercomm(const Intercomm& data) : Comm(data) { }
  // inter-language operability
  Intercomm(const MPI_Comm& data) : Comm(data) { }

  //
  // Groups, Contexts, and Communicators
  //

  Intercomm Dup() const;

  Intercomm& Clone() const;

  virtual int Get_remote_size() const;

  Group Get_remote_group() const;
  
  virtual Intracomm Merge(bool high);

  //
  // Extended Collective Operations
  //

  virtual Intercomm Create(const Group& group) const;

  virtual Intercomm Split(int color, int key) const;

};
