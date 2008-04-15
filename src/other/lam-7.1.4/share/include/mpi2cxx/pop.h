// -*- c++ -*-
//
// $Id: pop.h,v 1.4 2002/10/09 20:57:38 brbarret Exp $
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
  Op() : mpi_op(MPI_OP_NULL) { }

  Op(const Op& op) : 
    op_user_function(op.op_user_function), 
    mpi_op(op.mpi_op)  { }

  Op(const MPI_Op &i) : mpi_op(i) { }

  // destruction
  virtual ~Op() { }

  // assignment
  Op& operator=(const Op& op);
  Op& operator= (const MPI_Op &i);
  // comparison
  bool operator== (const Op &a);
  bool operator!= (const Op &a);
  // conversion functions for inter-language operability
  operator MPI_Op () const;
  //  operator MPI_Op* () const;

  // Collective Communication  (defined in op_inln.h)
  virtual void Init(User_function *func, bool commute);
  virtual void Free(void);

  User_function *op_user_function;

protected:
  MPI_Op mpi_op;

};

