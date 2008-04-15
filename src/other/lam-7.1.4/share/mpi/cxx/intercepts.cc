// -*- c++ -*-
//
// $Id: intercepts.cc,v 1.4 2003/08/13 22:01:57 vsahay Exp $
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


#include "mpicxx.h"
#if MPI2CPP_HPUX_OS & MPI2CPP_LAM
  // #%$^#$%^#$%^$ LAM on HP'S!!!!
#include <mpisys.h>
#undef MIN
#undef MAX
#endif
#include <stdio.h>

extern "C" 
void throw_excptn_fctn(MPI_Comm *, int *errcode, ...)
{
#if _MPIPP_USEEXCEPTIONS_
  throw(MPI::Exception(*errcode));
#else
  // Ick.  This is really ugly, but necesary if someone uses a C compiler
  // and -lmpi++ (which can legally happen in the LAM MPI implementation,
  // and probably in MPICH and others who include -lmpi++ by default in their
  // wrapper compilers)
  fprintf(stderr, "MPI 2 C++ exception throwing is disabled, MPI::mpi_errno has the error code\n"); 
  MPI::mpi_errno = *errcode;
#endif  
}

_REAL_MPI_::Comm::mpi_comm_map_t _REAL_MPI_::Comm::mpi_comm_map;
_REAL_MPI_::Comm::mpi_err_map_t _REAL_MPI_::Comm::mpi_err_map;


extern "C"
void
errhandler_intercept(MPI_Comm *mpi_comm, int *err, ...)
{
  _REAL_MPI_::Comm* comm = _REAL_MPI_::Comm::mpi_err_map[*mpi_comm];
  if (comm && comm->my_errhandler) {
    va_list ap;
    va_start(ap, err);
    comm->my_errhandler->handler_fn(*comm, err, ap);
    va_end(ap);
  }
}

_REAL_MPI_::Op* _REAL_MPI_::Intracomm::current_op;

extern "C" void
op_intercept(void *invec, void *outvec, int *len, MPI_Datatype *datatype)
{
  _REAL_MPI_::Op* op = _REAL_MPI_::Intracomm::current_op;
  MPI::Datatype thedata = *datatype;
  ((MPI::User_function*)op->op_user_function)(invec, outvec, *len, thedata);
  //JGS the above cast is a bit of a hack, I'll explain:
  //  the type for the PMPI::Op::op_user_function is PMPI::User_function
  //  but what it really stores is the user's MPI::User_function supplied when
  //  the user did an Op::Init. We need to cast the function pointer back to
  //  the MPI::User_function. The reason the PMPI::Op::op_user_function was
  //  not declared a MPI::User_function instead of a PMPI::User_function is
  //  that without namespaces we cannot do forward declarations.
  //  Anyway, without the cast the code breaks on HP LAM with the aCC compiler.
}

_REAL_MPI_::Comm::key_fn_map_t _REAL_MPI_::Comm::key_fn_map;

extern "C" int
copy_attr_intercept(MPI_Comm oldcomm, int keyval, 
		    void *extra_state, void *attribute_val_in, 
		    void *attribute_val_out, int *flag)
{
  int ret = 0;
  _REAL_MPI_::Comm::key_pair_t* copy_and_delete = 
    _REAL_MPI_::Comm::key_fn_map[keyval];
  _REAL_MPI_::Comm::Copy_attr_function* copy_fn;
  copy_fn = copy_and_delete->first;

  _REAL_MPI_::Comm::comm_pair_t *comm_type = 
    _REAL_MPI_::Comm::mpi_comm_map[oldcomm];
  
  // Just in case...

  if (comm_type == 0)
    return MPI::ERR_OTHER;

  _REAL_MPI_::Intracomm intracomm;
  _REAL_MPI_::Intercomm intercomm;
  _REAL_MPI_::Graphcomm graphcomm;
  _REAL_MPI_::Cartcomm cartcomm;
  
  int thetype = (MPI2CPP_ATTR)comm_type->second;
  bool bflag = (bool)*flag; 

  switch (thetype) {
  case eIntracomm:
    intracomm = _REAL_MPI_::Intracomm(*comm_type->first);
    ret = copy_fn(intracomm, keyval, extra_state,
		  attribute_val_in, attribute_val_out, bflag);
    break;
  case eIntercomm:
    intercomm = _REAL_MPI_::Intercomm(*comm_type->first);
    ret = copy_fn(intercomm, keyval, extra_state,
		  attribute_val_in, attribute_val_out, bflag);
    break;
  case eGraphcomm:
    graphcomm = _REAL_MPI_::Graphcomm(*comm_type->first);
    ret = copy_fn(graphcomm, keyval, extra_state,
		  attribute_val_in, attribute_val_out, bflag);
    break;
  case eCartcomm:
    cartcomm = _REAL_MPI_::Cartcomm(*comm_type->first);
    ret = copy_fn(cartcomm, keyval, extra_state,
		  attribute_val_in, attribute_val_out, bflag);
    break;
  }

  *flag = (int)bflag;
  return ret;
}

extern "C" int
delete_attr_intercept(MPI_Comm comm, int keyval, 
		      void *attribute_val, void *extra_state)
{
  int ret = 0;

  _REAL_MPI_::Comm::key_pair_t *copy_and_delete = 
    _REAL_MPI_::Comm::key_fn_map[keyval];

  _REAL_MPI_::Comm::Delete_attr_function* delete_fn;  
  delete_fn = copy_and_delete->second;

  _REAL_MPI_::Comm::comm_pair_t *comm_type = 
    _REAL_MPI_::Comm::mpi_comm_map[comm];

  // Just in case...

  if (comm_type == 0)
    return MPI::ERR_OTHER;

  _REAL_MPI_::Intracomm intracomm;
  _REAL_MPI_::Intercomm intercomm;
  _REAL_MPI_::Graphcomm graphcomm;
  _REAL_MPI_::Cartcomm cartcomm;
  
  int thetype = (long)(comm_type->second);

  if (delete_fn > (_REAL_MPI_::Comm::Delete_attr_function*) 100) {
    switch (thetype) {
    case eIntracomm:
      intracomm = _REAL_MPI_::Intracomm(*comm_type->first);
      ret = delete_fn(intracomm, keyval, attribute_val, extra_state);
      break;
    case eIntercomm:
      intercomm = _REAL_MPI_::Intercomm(*comm_type->first);
      ret = delete_fn(intercomm, keyval, attribute_val, extra_state);
      break;
    case eGraphcomm:
      graphcomm = _REAL_MPI_::Graphcomm(*comm_type->first);
      ret = delete_fn(graphcomm, keyval, attribute_val, extra_state);
      break;
    case eCartcomm:
      cartcomm = _REAL_MPI_::Cartcomm(*comm_type->first);
      ret = delete_fn(cartcomm, keyval, attribute_val, extra_state);
      break;
    }
  } else 
    ret = MPI::ERR_OTHER;
  return ret; 
}

