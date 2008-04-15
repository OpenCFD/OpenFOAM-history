// -*- c++ -*-
//
// $Id: op_inln.h,v 1.4 2003/06/02 03:05:12 jsquyres Exp $
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

#if LAM_WANT_PROFILE

inline
MPI::Op::Op() { }
  
inline
MPI::Op::Op(const MPI::Op& o) : pmpi_op(o.pmpi_op) { }
  
inline
MPI::Op::Op(const MPI_Op& o) : pmpi_op(o) { }

inline
MPI::Op::~Op() { }

inline
MPI::Op& MPI::Op::operator=(const MPI::Op& op) {
  pmpi_op = op.pmpi_op; return *this;
}

// comparison
inline bool
MPI::Op::operator== (const MPI::Op &a) {
  return (bool)(pmpi_op == a.pmpi_op);
}

inline bool
MPI::Op::operator!= (const MPI::Op &a) {
  return (bool)!(*this == a);
}

// inter-language operability
inline MPI::Op&
MPI::Op::operator= (const MPI_Op &i) { pmpi_op = i; return *this; }

inline
MPI::Op::operator MPI_Op () const { return pmpi_op; }

//inline
//MPI::Op::operator MPI_Op* () { return pmpi_op; }


#else  // ============= NO PROFILING ===================================

// construction
inline
MPI::Op::Op() : mpi_op(MPI_OP_NULL) { }

inline
MPI::Op::Op(const MPI_Op &i) : mpi_op(i) { }

inline
MPI::Op::Op(const MPI::Op& op)
  : op_user_function(op.op_user_function), mpi_op(op.mpi_op) { }

inline 
MPI::Op::~Op() 
{ 
#if _MPIPP_DEBUG_
  mpi_op = MPI_OP_NULL;
  op_user_function = 0;
#endif
}  

inline MPI::Op&
MPI::Op::operator=(const MPI::Op& op) {
  mpi_op = op.mpi_op;
  op_user_function = op.op_user_function;
  return *this;
}

// comparison
inline bool
MPI::Op::operator== (const MPI::Op &a) { return (bool)(mpi_op == a.mpi_op); }

inline bool
MPI::Op::operator!= (const MPI::Op &a) { return (bool)!(*this == a); }

// inter-language operability
inline MPI::Op&
MPI::Op::operator= (const MPI_Op &i) { mpi_op = i; return *this; }

inline
MPI::Op::operator MPI_Op () const { return mpi_op; }

//inline
//MPI::Op::operator MPI_Op* () { return &mpi_op; }

#endif

inline void
_REAL_MPI_::Op::Init(_REAL_MPI_::User_function *func, bool commute)
{
  (void)MPI_Op_create(op_intercept , (int) commute, &mpi_op);
  op_user_function = (User_function*)func;
}


inline void
_REAL_MPI_::Op::Free()
{
  (void)MPI_Op_free(&mpi_op);
}
