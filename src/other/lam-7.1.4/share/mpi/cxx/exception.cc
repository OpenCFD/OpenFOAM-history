// -*- c++ -*-
//
// $Id: exception.cc,v 1.2 2002/10/09 20:58:56 brbarret Exp $
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

int
MPI::Exception::Get_error_code() const
{
  return pmpi_exception.Get_error_code();
}

int
MPI::Exception::Get_error_class() const
{
  return pmpi_exception.Get_error_class();
}

const char*
MPI::Exception::Get_error_string() const
{
  return pmpi_exception.Get_error_string();
}
