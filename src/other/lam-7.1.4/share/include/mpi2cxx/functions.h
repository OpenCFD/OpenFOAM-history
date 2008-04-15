// -*- c++ -*-
//
// $Id: functions.h,v 1.4 2003/04/15 01:30:45 adharurk Exp $
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

//
// Point-to-Point Communication
//

void 
Attach_buffer(void* buffer, int size);

int 
Detach_buffer(void*& buffer);

//
// Process Topologies
//

void
Compute_dims(int nnodes, int ndims, int dims[]);

//
// Environmental Inquiry
//

void 
Get_processor_name(char* name, int& resultlen);

void
Get_error_string(int errorcode, char* string, int& resultlen);

int 
Get_error_class(int errorcode);

double 
Wtime();

double 
Wtick();

void
Init(int& argc, char**& argv);

void
Init();

void
Real_init();

void
Finalize();

bool
Is_initialized();

//
// External Interfaces
//

int
Init_thread(int &argc, char**&argv, int required);

int
Init_thread(int required);

bool
Is_thread_main();

int
Query_thread();


//
// Miscellany
//


void*
Alloc_mem(Aint size, const Info& info);


void
Free_mem(void* base);

//
// Process Creation
//

void
Close_port(const char* port_name);


void
Lookup_name(const char* service_name, const Info& info, char* port_name);


void
Open_port(const Info& info, char* port_name);


void
Publish_name(const char* service_name, const Info& info, 
	     const char* port_name);

void 
Unpublish_name(const char* service_name, const Info& info, 
	       const char* port_name);

//
// Profiling
//

void
Pcontrol(const int level, ...);

void
Get_version(int& version, int& subversion);

_REAL_MPI_::Aint
Get_address(void* location);




