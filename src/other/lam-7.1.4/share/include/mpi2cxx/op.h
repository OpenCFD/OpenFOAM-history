// -*- c++ -*-
//
// $Id: op.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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

class Op {
public:

  // construction
  Op();
  Op(const MPI_Op &i);
  Op(const Op& op);
#if LAM_WANT_PROFILE
  Op(const PMPI::Op& op) : pmpi_op(op) { }
#endif
  // destruction
  virtual ~Op();
  // assignment
  Op& operator=(const Op& op);
  Op& operator= (const MPI_Op &i);
  // comparison
  inline MPI2CPP_BOOL_T operator== (const Op &a);
  inline MPI2CPP_BOOL_T operator!= (const Op &a);
  // conversion functions for inter-language operability
  inline operator MPI_Op () const;
  //  inline operator MPI_Op* (); //JGS const
#if LAM_WANT_PROFILE
  inline operator const PMPI::Op&() const { return pmpi_op; }
#endif
  // Collective Communication
  //JGS took const out
  virtual void Init(User_function *func, MPI2CPP_BOOL_T commute);
  virtual void Free();
 
#if ! LAM_WANT_PROFILE
  User_function *op_user_function; //JGS move to private
protected:
  MPI_Op mpi_op;
#endif

#if LAM_WANT_PROFILE
private:
  PMPI::Op pmpi_op;
#endif

};

