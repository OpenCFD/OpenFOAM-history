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
//	$Id: mpicxx.cc,v 1.5 2003/02/19 20:52:33 jsquyres Exp $
//
//	Function:	- wrapper for C++ program compilation
//

#include <lam_config.h>

#include <lamwrap.h>


int
main(int argc, char *argv[])
{
  // The three wrapper compilers are extremely similar.  So similar,
  // in fact, that they can be parameterized on what is different.
  // Hence, we call the "wrapper compiler engine" to do all the work,
  // and pass in just a few arguments to customize for the language of
  // this wrapper compiler.

  lam_sv_t str_vec;

  str_vec.clear();
  str_vec.push_back("LAMMPICXX");
  str_vec.push_back("LAMCXX");
  str_vec.push_back("LAMHCP");
  str_vec.push_back("TROLLIUSHCP");

  return lam_wrap_engine(argc, argv,
			 str_vec, LAM_CXX, 
			 LAM_WANT_MPI2CPP, LAM_WANT_MPI2CPP, false,
			 WRAPPER_EXTRA_CXXFLAGS);
}
