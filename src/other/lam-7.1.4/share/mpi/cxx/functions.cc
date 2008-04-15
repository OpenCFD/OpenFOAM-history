// -*- c++ -*-
//
// $Id: functions.cc,v 1.4 2003/04/15 01:19:43 adharurk Exp $
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

#include "lam_config.h"
#include "mpicxx.h"

#if LAM_WANT_PROFILE

//
// Point-to-Point Communication
//

void 
MPI::Attach_buffer(void* buffer, int size)
{
  PMPI::Attach_buffer(buffer, size);
}

int 
MPI::Detach_buffer(void*& buffer)
{
  return PMPI::Detach_buffer(buffer);
}

//
// Process Topologies
//

void
MPI::Compute_dims(int nnodes, int ndims, int dims[])
{
  PMPI::Compute_dims(nnodes, ndims, dims);
}


//
// Environmental Inquiry
//

void 
MPI::Get_processor_name(char* name, int& resultlen)
{
  PMPI::Get_processor_name(name, resultlen);
}

void
MPI::Get_error_string(int errorcode, char* string, int& resultlen)
{
  PMPI::Get_error_string(errorcode, string, resultlen);
}

int 
MPI::Get_error_class(int errorcode) 
{
  return PMPI::Get_error_class(errorcode);
}

double 
MPI::Wtime()
{
  return PMPI::Wtime();
}

double 
MPI::Wtick()
{
  return PMPI::Wtick();
}


void
MPI::Init(int& argc, char**& argv)
{
  PMPI::Init(argc, argv);
}

void
MPI::Init()
{
  PMPI::Init();
}

void
MPI::Finalize()
{
  PMPI::Finalize();
}

bool
MPI::Is_initialized()
{
  return PMPI::Is_initialized();
}


//
// External Interfaces
//


int
MPI::Init_thread(int required)
{
  return PMPI::Init_thread(required);
}

int
MPI::Init_thread(int& argc, char**& argv, int required)
{
  return PMPI::Init_thread(argc, argv, required);
}

bool
MPI::Is_thread_main()
{
  return PMPI::Is_thread_main();
}

int
MPI::Query_thread()
{
  return PMPI::Query_thread();
}


//
// Miscellany
//

void*
MPI::Alloc_mem(MPI::Aint size, const MPI::Info& info)
{
  return PMPI::Alloc_mem(size, info);
}


void
MPI::Free_mem(void* base)
{
  PMPI::Free_mem(base);
}


//
// Process Creation
//


void
MPI::Close_port(const char* port_name) 
{
  PMPI::Close_port(port_name);
}


void
MPI::Lookup_name(const char * service_name, const MPI::Info& info,
		 char* port_name)
{
  PMPI::Lookup_name(service_name, info, port_name);
}


void
MPI::Open_port(const MPI::Info& info, char* port_name)
{
  PMPI::Open_port(info, port_name);
}


void
MPI::Publish_name(const char* service_name, const MPI::Info& info,
		  const char* port_name)
{
  PMPI::Publish_name(service_name, info, port_name);
}


void
MPI::Unpublish_name(const char* service_name, const MPI::Info& info,
		    const char* port_name)
{
  PMPI::Unpublish_name(service_name, info, port_name);
}



//
// Profiling
//

void
MPI::Pcontrol(const int level, ...)
{
  va_list ap;
  va_start(ap, level);
 
  PMPI::Pcontrol(level, ap);
  va_end(ap);
}

void
MPI::Get_version(int& version, int& subversion)
{
  PMPI::Get_version(version, subversion);
}

MPI::Aint
MPI::Get_address(void* location)
{
  return PMPI::Get_address(location);
}

#endif
