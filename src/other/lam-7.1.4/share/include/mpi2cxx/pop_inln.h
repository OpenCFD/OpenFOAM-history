// -*- c++ -*-
//
// $Id: pop_inln.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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

inline PMPI::Op&
PMPI::Op::operator=(const PMPI::Op& op)
{
  mpi_op = op.mpi_op;
  op_user_function = op.op_user_function;
  return *this;
} 

// comparison
inline bool
PMPI::Op::operator==(const PMPI::Op &a)
{
  return (bool)(mpi_op == a.mpi_op);
}

inline bool //this will never be used?
PMPI::Op::operator!= (const PMPI::Op &a) { return (bool)!(*this == a); }

// inter-language operability
inline PMPI::Op&
PMPI::Op::operator= (const MPI_Op &i) { mpi_op = i; return *this; }

inline
PMPI::Op::operator MPI_Op () const { return mpi_op; }

//inline
//PMPI::Op::operator MPI_Op* () const { return (MPI_Op*)&mpi_op; }


