// -*- c++ -*-
//
// $Id: mpicxx.h,v 6.10 2003/08/13 22:00:57 vsahay Exp $
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

#ifndef MPIPP_H
#define MPIPP_H

// 
// Let's ensure that we're really in C++, and some errant programmer
// hasn't included <mpicxx.h> just "for completeness"
//

#if defined(__cplusplus) || defined(c_plusplus) 

#include "lam_config.h"

// VPS: this is added for MPI::File C++ binding--to make references
// to MPIO_Request point to MPI_Request, just for success in compiling. 
// Will have to be removed once ROMIO is integrated into LAM
#if LAM_WANT_CPPFILE
#define ADIOI_RequestD _req 
#define MPIO_Request MPI_Request
#endif

#include <mpi.h>

#include <stdarg.h>
#include "mpi2cxx/mpi2cxx_map.h"


//JGS: this is used for implementing user functions for MPI::Op
extern "C" void
op_intercept(void *invec, void *outvec, int *len, MPI_Datatype *datatype);

//JGS: this is used as the MPI_Handler_function for
// the mpi_errhandler in ERRORS_THROW_EXCEPTIONS
extern "C" void
throw_excptn_fctn(MPI_Comm* comm, int* errcode, ...);

extern "C" void
errhandler_intercept(MPI_Comm * mpi_comm, int * err, ...);


//used for attr intercept functions
enum CommType { eIntracomm, eIntercomm, eCartcomm, eGraphcomm};

extern "C" int
copy_attr_intercept(MPI_Comm oldcomm, int keyval, 
		    void *extra_state, void *attribute_val_in, 
		    void *attribute_val_out, int *flag);

extern "C" int
delete_attr_intercept(MPI_Comm comm, int keyval, 
		      void *attribute_val, void *extra_state);


#if LAM_WANT_PROFILE
#include "mpi2cxx/pmpicxx.h"
#endif

namespace MPI {

#if ! _MPIPP_USEEXCEPTIONS_
	extern int mpi_errno;
#endif

  class Comm_Null;
  class Comm;
  class Intracomm;
  class Intercomm;
  class Graphcomm;
  class Cartcomm;
  class Datatype;
  class Errhandler;
  class Group;
  class Op;
  class Request;
  class Status;
  class Info;
  class Win;
#if LAM_WANT_CPPFILE
  class File;
#endif


  typedef MPI_Aint Aint;

#if LAM_WANT_CPPFILE
  typedef MPI_Offset Offset; //VPS: for romio 
#endif


#include "mpi2cxx/constants.h"
#include "mpi2cxx/functions.h"
#include "mpi2cxx/datatype.h"

  typedef void User_function(const void* invec, void* inoutvec, int len,
			     const Datatype& datatype);

#include "mpi2cxx/exception.h"
#include "mpi2cxx/op.h"
#include "mpi2cxx/status.h"
#include "mpi2cxx/request.h"   //includes class Prequest
#include "mpi2cxx/group.h" 
#include "mpi2cxx/comm.h"
#include "mpi2cxx/errhandler.h"
#include "mpi2cxx/intracomm.h"
#include "mpi2cxx/topology.h"  //includes Cartcomm and Graphcomm
#include "mpi2cxx/intercomm.h"
#include "mpi2cxx/info.h"
#include "mpi2cxx/win.h"

#if LAM_WANT_CPPFILE
#include "mpi2cxx/file.h" //VPS: included file.h for MPI::File binding
#endif

}

#if LAM_WANT_PROFILE
#include "mpi2cxx/pop_inln.h"
#include "mpi2cxx/pgroup_inln.h"
#include "mpi2cxx/pstatus_inln.h"
#include "mpi2cxx/prequest_inln.h"
#endif

//
// These are the "real" functions, whether prototyping is enabled
// or not. These functions are assigned to either the MPI::XXX class
// or the PMPI::XXX class based on the value of the macro _REAL_MPI_
// which is set in mpi2cxx_config.h.
// If prototyping is enabled, there is a top layer that calls these
// PMPI functions, and this top layer is in the XXX.cc files.
//



#include "mpi2cxx/datatype_inln.h"
#include "mpi2cxx/functions_inln.h"
#include "mpi2cxx/request_inln.h"
#include "mpi2cxx/comm_inln.h"
#include "mpi2cxx/intracomm_inln.h"
#include "mpi2cxx/topology_inln.h"
#include "mpi2cxx/intercomm_inln.h"
#include "mpi2cxx/group_inln.h"
#include "mpi2cxx/op_inln.h"
#include "mpi2cxx/errhandler_inln.h"
#include "mpi2cxx/status_inln.h"
#include "mpi2cxx/info_inln.h"
#include "mpi2cxx/win_inln.h"

#if LAM_WANT_CPPFILE
#include "mpi2cxx/file_inln.h"  //VPS: included for MPI::File binding
#endif

#endif // #if defined(__cplusplus) || defined(c_plusplus) 
#endif // #ifndef MPIPP_H_
