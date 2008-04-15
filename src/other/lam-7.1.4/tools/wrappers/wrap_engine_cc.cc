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
//	$Id: wrap_engine.cc,v 1.7 2003/09/19 20:56:48 jsquyres Exp $
//
//	Function: - wrapper for C program compilation.  This is the
//	back-end engine for all the wrapper compilers.  It takes
//	parameters for each different wrapper compiler and invokes the
//	rest of the wrapper compiler functionality.
//

#include <lam_config.h>

#include <iostream>

#include <lamwrap.h>


using namespace std;


//
// lam_wrap_engine
//
// Back-end entry point for the wrapper compiler functionality.  All
// three wrapper compilers invoke this function.
//
int
lam_wrap_engine(int argc, char* argv[],
		const lam_sv_t& env_vars, const string& default_compiler,
		bool want_cxx_includes, bool want_cxx_libs,
		bool want_f77_includes,
		const string& extra_args)
{
  int ret(0);

  string prefix;
  bool want_flags;

  lam_sv_t str_vec;

  lam_sv_t compiler;
  lam_sv_t cflags;
  lam_sv_t user_args;
  lam_sv_t ldflags;
  lam_sv_t libs;
  lam_sv_t extra_flags;

  lam_sv_t cmd_line;

  // Parse command line

  lam_wrap_parse_args(argc, argv, want_flags);

  // Get the compiler

  lam_wrap_get_compiler(env_vars, default_compiler, compiler);  

  // Build the CFLAGS

  lam_wrap_build_cflags(want_cxx_includes, want_f77_includes, prefix, cflags);

  // Build the user arguments

  lam_wrap_build_user_args(argc, argv, user_args);

  // Build the LDFLAGS

  lam_wrap_build_ldflags(prefix, ldflags);

  // Build the LIBS

  lam_wrap_build_libs(prefix, want_cxx_libs, libs);

  // Build any extra arguments

  lam_wrap_build_extra_flags(extra_args, extra_flags);

  // Now assemble the command line

  cmd_line.clear();
  lam_wrap_append_sv(compiler, cmd_line);
  if (want_flags) {
    lam_wrap_append_sv(cflags, cmd_line);
    lam_wrap_append_sv(extra_flags, cmd_line);
  }
  lam_wrap_append_sv(user_args, cmd_line);
  if (want_flags) {
    lam_wrap_append_sv(ldflags, cmd_line);
    lam_wrap_append_sv(libs, cmd_line);
  }

  // Display or execute?

  if (showme_cmd)
    lam_wrap_print_sv(compiler);
  if (showme_cmd || showme_compile) {
    lam_wrap_print_sv(cflags);
    lam_wrap_print_sv(extra_flags);
  }
  if (showme_cmd || showme_compile || showme_link)
    lam_wrap_print_sv(user_args);
  if (showme_cmd || showme_link) {
    lam_wrap_print_sv(ldflags);
    lam_wrap_print_sv(libs);
  }
  if (showme_cmd || showme_compile || showme_link)
    cout << endl;
  else
    ret = lam_wrap_exec_sv(cmd_line);

  // That's all she wrote

  return ret;
}
