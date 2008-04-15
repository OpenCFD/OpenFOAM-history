// -*- c++ -*-
//
// $Id: pmpicxx.h,v 1.10 2003/08/13 22:01:16 vsahay Exp $
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


#ifndef PMPIPP_H
#define PMPIPP_H


namespace PMPI {

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
  typedef MPI_Offset Offset; //VPS for romio
#endif

#include "mpi2cxx/functions.h" //profile version same
#include "mpi2cxx/pdatatype.h"

  typedef void User_function(const void* invec, void* inoutvec, int len,
			     const Datatype& datatype);

#include "mpi2cxx/pexception.h"
#include "mpi2cxx/pop.h"
#include "mpi2cxx/pstatus.h"
#include "mpi2cxx/prequest.h"   //includes class Prequest
#include "mpi2cxx/pgroup.h" 
#include "mpi2cxx/pcomm.h"
#include "mpi2cxx/perrhandler.h"
#include "mpi2cxx/pintracomm.h"
#include "mpi2cxx/ptopology.h"  //includes Cartcomm and Graphcomm
#include "mpi2cxx/pintercomm.h"
#include "mpi2cxx/pinfo.h"
#include "mpi2cxx/pwin.h"

#if LAM_WANT_CPPFILE
#include "mpi2cxx/pfile.h"  //VPS: included pfile.h for MPI::File binding
#endif
}


#endif
