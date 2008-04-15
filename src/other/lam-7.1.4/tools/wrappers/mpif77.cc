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
//	$Id: mpif77.cc,v 1.5 2003/02/20 12:36:26 jsquyres Exp $
//
//	Function:	- wrapper for fortran program compilation
//

#include <lam_config.h>

#include <etc_misc.h>
#include <lamwrap.h>


int
main(int argc, char *argv[])
{
  // The three wrapper compilers are extremely similar.  So similar,
  // in fact, that they can be parameterized on what is different.
  // Hence, we call the "wrapper compiler engine" to do all the work,
  // and pass in just a few arguments to customize for the language of
  // this wrapper compiler.

  // Fortran is slightly different because it is possible to compile
  // --without-fc, in which case using mpif77 should just outright
  // fail.

#if !LAM_WANT_FORTRAN
  show_help("hf77", "no-fortran-support", NULL);
  return 1;
#else
  lam_sv_t str_vec;

  str_vec.clear();
  str_vec.push_back("LAMMPIF77");
  str_vec.push_back("LAMF77");
  str_vec.push_back("LAMHF77");
  str_vec.push_back("TROLLIUSHF77");

  return lam_wrap_engine(argc, argv,
			 str_vec, LAM_F77, true, false, true,
			 WRAPPER_EXTRA_FFLAGS);
#endif
}
