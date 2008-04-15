// -*- c++ -*-
//
// $Id: pexception.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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


class Exception {
public:
  inline Exception(int ec) : error_code(ec) {
    (void)MPI_Error_class(error_code, &error_class);
    int resultlen;
    error_string = new char[MPI_MAX_ERROR_STRING];
    (void)MPI_Error_string(error_code, error_string, &resultlen);
  }

  inline int Get_error_code() const { return error_code; }

  inline int Get_error_class() const { return error_class; }
  
  inline const char* Get_error_string() const { return error_string; }
  
protected:
  int error_code;
  char* error_string;
  int error_class;
};
