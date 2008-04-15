// -*- c++ -*-
//
// $Id: exception.h,v 1.2 2002/10/09 20:57:36 brbarret Exp $
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

#if LAM_WANT_PROFILE

  inline Exception(int ec) : pmpi_exception(ec) { }

  int Get_error_code() const;
  
  int Get_error_class() const;
  
  const char* Get_error_string() const;
 
#else

  inline Exception(int ec) : error_code(ec), error_string(0), error_class(-1) {
    (void)MPI_Error_class(error_code, &error_class);
    int resultlen;
    error_string = new char[MAX_ERROR_STRING];
    (void)MPI_Error_string(error_code, error_string, &resultlen);
  }
  inline ~Exception() {
    delete[] error_string;
  }
  // Better put in a copy constructor here since we have a string;
  // copy by value (from the default copy constructor) would be
  // disasterous.
  inline Exception(const Exception& a)
    : error_code(a.error_code), error_class(a.error_class)
  {
    error_string = new char[MAX_ERROR_STRING];
    // Rather that force an include of <string.h>, especially this
    // late in the game (recall that this file is included deep in
    // other .h files), we'll just do the copy ourselves.
    for (int i = 0; i < MAX_ERROR_STRING; i++)
      error_string[i] = a.error_string[i];
  }

  inline int Get_error_code() const { return error_code; }

  inline int Get_error_class() const { return error_class; }
  
  inline const char* Get_error_string() const { return error_string; }

#endif
 
protected:
#if LAM_WANT_PROFILE
  PMPI::Exception pmpi_exception;
#else
  int error_code;
  char* error_string;
  int error_class;
#endif
};
