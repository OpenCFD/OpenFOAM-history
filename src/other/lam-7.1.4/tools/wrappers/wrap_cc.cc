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
// $Id: wrap.cc,v 1.26 2003/09/19 20:56:48 jsquyres Exp $
//
//	Function:	- helper library for wrapper compilers
//

#include <lam_config.h>

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <sfh.h>
#include <etc_misc.h>
#include <lamwrap.h>
#include <lam_config.h>

using namespace std;


//
// External functions
//

extern "C" {
  extern int cnfexec(char *argv[]);
  extern char **environ;
}


//
// Global variables
//

bool fl_libs(true);
bool fl_profile(false);
bool fl_cpp(false);
bool fl_want_show_error(false);

bool showme_cmd(false);
bool showme_compile(false);
bool showme_link(false);


//
// Local variables
//

static string cmd_name("<unknown>");


//
// lam_wrap_parse_args
//
// Parse the command line arguments of the wrapper compiler
//
void
lam_wrap_parse_args(int argc, char* argv[], bool& want_flags)
{
  string str;
  bool have_arg;
  cmd_name = argv[0];

  // Note: only add all the compiler/linker flags if there is an
  // argv[] that doesn not begin with "-" or some form of "-showme" is
  // specified.

  want_flags = false;
  for (int i = 1; i < argc; ++i) {
    str = argv[i];
    if (str.substr(0, 7) == "-showme" || str.substr(0, 8) == "--showme" ||
        str == "-show" || str == "--show") {
      want_flags = true;
      fl_want_show_error = true;
      if (str.substr(0, 8) == "-showme:") {
        str = str.substr(8);
        have_arg = true;
      } else if (str.substr(0, 9) == "--showme:") {
        str = str.substr(9);
        have_arg = true;
      } else {
        showme_cmd = true;
        have_arg = false;
      }

      if (have_arg) {
        if (str == "compile")
          showme_compile = true;
        else if (str == "link")
          showme_link = true;
        else
          showme_cmd = true;
      }
    } else if (str == "-c") {
      fl_libs = false;
    } else if (str == "-E" || str == "-M") {
      fl_libs = false;
      fl_cpp = true;
    } else if (str == "-S") {
      fl_libs = false;
    } else if (str == "-lpmpi") {
      fl_profile = true;
    } else if (str[0] != '-') {
      want_flags = true;
      fl_want_show_error = true;
    }
  }

#if !LAM_WANT_PROFILE
  // Sanity check

  if (fl_profile) {
    show_help("compile", "no-profiling-support", argv[0], NULL);
    fl_profile = false;
  }
#endif
}


//
// lam_wrap_get_compiler
//
// Figure out what the back-end compiler is (even if it's multiple
// tokens)
//
void
lam_wrap_get_compiler(const lam_sv_t& env_list, const string& default_comp,
		      lam_sv_t& out)
{
  int i;
  char *env;
  string comp;
  string temp1, temp2;
  string::size_type pos;
  string compiler0;

  out.clear();

  // Check for environment variable overrides

  for (i = 0; (string::size_type) i < env_list.size(); ++i) {
    env = getenv(env_list[i].c_str());
    if (env != 0) {
      comp = env;
      lam_wrap_split(comp, ' ', out);
      break;
    }
  }

  // If we didn't find any of the environment variables, use the default

  if (out.empty())
    lam_wrap_split(default_comp, ' ', out);

  // If we're preprocessing, we need to know the basename of argv0
  // (see below).

  if (fl_cpp) {
    pos = out[0].find_last_of('/');
    if (pos != string::npos)
      // JMS There has to be a better way to do this
      compiler0 = out[0].substr(pos + 1);
    else
      compiler0 = out[0];
  }
    
  // Ugh.  If we're acting as the preprocessor, ditch any libtool
  // arguments.

  if (fl_cpp) {
    // If we find "libtool", then advance until we find an argument
    // that does not begin with  "--"; the arguments after that will be
    // the compiler (preprocessor) stuff
    
    if (compiler0 == "libtool") {
      i = 1;
      while ((string::size_type) i < out.size() && 
	     out[i].substr(0, 2) == "--")
	++i;
      out.erase(out.begin(), out.begin() + i);
    }
  }
}


//
// lam_wrap_build_cflags
//
// Build up a list of arguments for CFLAGS (a bit of a misnomer,
// because it may actually be CXXFLAGS or FFLAGS, depending on what
// the front-end wrapper compiler is).
//
void
lam_wrap_build_cflags(bool want_cxx_includes, bool want_f77_includes,
		      const string& prefix, lam_sv_t& cflags)
{
  string incdir(LAM_INCDIR);

  cflags.clear();

  // Ensure that we don't -I/usr/include, for the reasons listed
  // above.  EXCEPTION: If this is fortran, then we add -I regardless
  // of what the prefix is, because the fortran compiler will not
  // -I/usr/include automatically.

  if (want_f77_includes || incdir != "/usr/include") 
    cflags.push_back("-I" + incdir);
}


//
// lam_wrap_build_user_args
//
// Build up a list of user arguments (from argc/argv) that will be
// plugged into the command line that will invoke the back-end
// compiler.
//
void
lam_wrap_build_user_args(int argc, char* argv[], lam_sv_t& user_args)
{
  string str;

  // Don't copy -showme* or -lpmpi.  -lpmpi will be
  // insertted elsewhere if necessary.

  for (int i = 1; i < argc; ++i) {
    str = argv[i];
    if (str.substr(0, 7) != "-showme" &&
	str.substr(0, 8) != "--showme" &&
	str != "-show" &&
	str != "--show" &&
	str != "-lpmpi")
      user_args.push_back(str);
  }
}


//
// lam_wrap_build_ldflags
//
// Build up a list of LDFLAGS that will be given to the back-end
// compiler.
//
void
lam_wrap_build_ldflags(const string& prefix, lam_sv_t& ldflags)
{
  string s;
  string libdir(LAM_LIBDIR);
  lam_sv_t sv;
  lam_sv_t::iterator svi;

  ldflags.clear();

  // If we don't want the libs, then we don't want ldflags, either.
  // Hence, return with ldflags empty.

  if (!fl_libs)
    return;

  // Add in the extra flags passed by configure.  Do the same kinds of
  // checks that we do below -- ensure that we don't add a "-L/usr" to
  // the command line.

  s = WRAPPER_EXTRA_LDFLAGS;
  lam_wrap_split(s, ' ', sv);

  if (!sv.empty())
    for (svi = sv.begin(); svi != sv.end(); ++svi) 
      if (*svi != "-L/usr")
	lam_wrap_split_append_sv(*svi, ldflags);

  // Form library directory pathname.
  //
  // Add "-L$prefix/lib".
  //
  // Apply similar logic here as we did with -I -- if the libdir is
  // /usr/lib, there's no need to explicitly add it, because the
  // compiler will already be looking there.

  if (libdir != "/usr/lib")
    ldflags.push_back("-L" + libdir);
}


//
// lam_wrap_build_libs
//
// Build up a list of LIBS that will be given to the back-end
// compiler.
//
void 
lam_wrap_build_libs(const string& prefix, bool want_cxx_libs, lam_sv_t& libs)
{
  string libdir(LAM_LIBDIR);
#if LAM_WANT_ROMIO && HAVE_LIBAIO
  bool want_aio(false);
#endif
  
  libs.clear();

  // If we don't want the libs, then return with libs empty

  if (!fl_libs)
    return;

  // Now we start adding libraries to libs

  // ROMIO comes first.  Check to ensure that it exists (and that the
  // profiling library was previously found).

#if LAM_WANT_ROMIO
  if (!lam_wrap_check_file(libdir, "liblammpio.a") &&
      !lam_wrap_check_file(libdir, "liblammpio.so")) {
    cerr << "WARNING: " << cmd_name
	 << " expected to find liblammpio.* in " << libdir << endl
	 << "WARNING: MPI-2 IO support will be disabled" << endl;
  } else {
    libs.push_back("-llammpio");
#if HAVE_LIBAIO
    want_aio = true;
#endif
  }
#endif

  // The C++ bindings come next

  if (want_cxx_libs) {
    if (!lam_wrap_check_file(libdir, "liblammpi++.a") &&
	!lam_wrap_check_file(libdir, "liblammpi++.so"))
      cerr << "WARNING: " << cmd_name
	   << " expected to find liblammpi++.* in " << libdir << endl
	   << "WARNING: MPI C++ support will be disabled" << endl;
    else
      libs.push_back("-llammpi++");
  }

  // Next comes the fortran MPI library

#if LAM_WANT_FORTRAN
  if (!lam_wrap_check_file(libdir, "liblamf77mpi.a") &&
      !lam_wrap_check_file(libdir, "liblamf77mpi.so"))
      cerr << "WARNING: " << cmd_name
	   << " expected to find liblamf77mpi.* in " << libdir << endl
	   << "WARNING: MPI Fortran support will be disabled" << endl;
  else
    libs.push_back("-llamf77mpi");
#endif

  // Next comes the MPI library

  libs.push_back("-lmpi");

  // Next the LAM library

  libs.push_back("-llam");

  // Finally, any system libraries

#if LAM_WANT_ROMIO && HAVE_LIBAIO
  if (want_aio)
    libs.push_back("-laio");
#endif
  lam_wrap_split_append_sv(WRAPPER_EXTRA_LIBS, libs);
}


//
// lam_wrap_build_extra_flags
//
// Build of a list of extra flags to go to the back-end compiler.
// These are typically extra flags that come from the configure
// script.
//
void
lam_wrap_build_extra_flags(const string& extra_string, lam_sv_t& extra_flags)
{
  if (!extra_string.empty())
    lam_wrap_split_append_sv(extra_string, extra_flags);
}


//
// lam_wrap_print_sv
//
// Print out a vector of strings
//
void
lam_wrap_print_sv(const lam_sv_t& sv)
{
  for (int i = 0; (string::size_type) i < sv.size(); ++i)
    cout << sv[i] << " ";
}


//
// lam_wrap_exec_sv
//
// Execute a vector of strings (ultimately results down to a call to
// some flavor of exec()).
//
int
lam_wrap_exec_sv(const lam_sv_t& sv)
{
  int status;
  int i, ac = 0;
  char **av = 0;
  char *tmp = NULL;

  // Build up a C array of the args

  for (i = 0; (string::size_type) i < sv.size(); ++i)
    sfh_argv_add(&ac, &av, (char*) sv[i].c_str());

  // There is no way to tell whether cnfexec returned non-zero because
  // the called app returned non-zero or if there was a failure in the
  // exec (like the file not being found).  So we look for the
  // compiler first, just to try to eliminate that case.
  tmp = sfh_path_env_findv(av[0], 0, environ, NULL);
  if (tmp == NULL) {
    show_help("compile", "no-compiler-found", av[0], NULL);
    errno = 0;
    status = -1;
  } else {
    free(tmp);

    status = cnfexec(av);
    if (status != 0 && errno != 0 && fl_want_show_error)
      perror(cmd_name.c_str());
  }

  // Free the C array
  sfh_argv_free(av);

  return status;
}


//
// lam_wrap_strip_white
//
// Remove leading and trailing white space from a given string.
// Must be sent a null-terminated string.
//
void 
lam_wrap_strip_white(string& str)
{
  int start, end, size(str.size());

  /* Remove leading whitespace */

  for (start = 0; start < size; ++start)
    if (!isspace(str[start]))
      break;

  /* Remove trailing whitespace */

  for (end = start; end < size; ++end)
    if (isspace(str[end])) 
      break;

  str = str.substr(start, end);
}


//
// lam_wrap_split
//
// Split a string into a vector of strings
//
bool 
lam_wrap_split(const string& str, char c, lam_sv_t& out)
{
  int start, i(0), size(str.size());
  out.clear();

  // Strip off the first bunch of c's

  while (i < size && str[i] == c)
    ++i;

  if (i >= size)
    return false;

  // Now start making a vector of the strings

  while (i < size) {
    start = i;
    while (i < size && str[i] != c)
      ++i;
    out.push_back(str.substr(start, i - start));

    while (i < size && str[i] == c)
      ++i;
  }

  return true;
}


//
// lam_wrap_split_append_sv
//
// Take a string, split it into tokens, and append it to an existing
// vector of strings
//
void
lam_wrap_split_append_sv(const string& str, lam_sv_t& out)
{
  int i;
  lam_sv_t temp;

  lam_wrap_split(str, ' ', temp);
  for (i = 0; (string::size_type) i < temp.size(); ++i)
    out.push_back(temp[i]);
}


//
// lam_wrap_append_sv
//
// Append one vector of strings onto the end of another.
//
void 
lam_wrap_append_sv(const lam_sv_t& in, lam_sv_t& out)
{
  // JMS Is there a better way to do this?
  for (int i = 0; (string::size_type) i < in.size(); ++i)
    out.push_back(in[i]);
}


//
// lam_wrap_check_file
//
// Check for the presence of a file
//
bool
lam_wrap_check_file(const string& dir, const string& file)
{
  int ret;
  struct stat buf;
  string name = dir + "/" + file;

  ret = stat(name.c_str(), &buf);

  return (bool) (ret == 0);
}
