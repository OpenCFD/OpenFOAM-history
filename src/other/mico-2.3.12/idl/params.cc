/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */


#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <stdio.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <unistd.h>
#include <mico/ir_creator.h>
#include <mico/util.h>
#include "params.h"

#endif // FAST_PCH


using namespace std;

IDLParam::IDLParam( int argc, char *argv[] )
{
  _argc = argc;
  _argv = argv;
  _cpp_only = false;
  set_defaults();
  parse_params();
  check_params();
}

IDLParam::IDLParam()
{
  _argc = 0;
  _argv = NULL;
  _cpp_only = false;
  set_defaults();
}

void
IDLParam::set_defaults()
{
#ifndef _WIN32
  cpp = "mico-cpp";
#else // _WIN32
  cpp = "mico-cpp.exe";
  char sz_path[256] = "";
  GetModuleFileName(0, sz_path, 255);
  string p(sz_path);
  p.erase(p.find_last_of("\\") + 1, p.length());
  cpp = "\"" + p + cpp + "\"";
#endif // _WIN32

  base_dir = "";
  cpp_suffix = "cc";
  hh_prefix.clear();
  hh_suffix = "h";
  use_quotes = false;
  feed_ir = false;
  feed_included_defs = true;
  codegen_cpp = false;
  cpp_impl = false;
  cpp_skel = false;
  codegen_idl = false;
  codegen_midl = false;
  codegen_wsdl = false;
  emit_repoids = false;
  query_server_for_narrow = true;
#ifdef HAVE_EXCEPTIONS
  no_exceptions = false;
#else
  no_exceptions = true;
#endif
  missing_nscorba = true;
  support_id = false;
  wsi_wsdl = false;
  pseudo = false;
  boa = false;
  poa = true;
  any = false;
  typecode = false;
  poa_ties = false;
  poa_stubs = true;
  windows_dll=false;
  windows_dll_with_export = false;
  mico_core = false;
  gen_included_defs = false;
  gen_full_dispatcher = false;
  throw_decl = false;
  idl3toidl2 = false;
  no_path_in_include = false;
  reflection = false;
  vc_sequence_reference_bug_workaround = false;
  i_prefixes.clear();
  i_postfixes.clear();
  output_dir = "";
  skel_suffix = "_skel";
  dll_def_prefix = "";
  tc_export = "extern";
  static_tc_export = "static";
  class_export = "";
  operator_export = "";
}

void
IDLParam::parse_params()
{
  MICOGetOpt::OptMap opts;
  opts["--feed-ir"]               = "";
  opts["--feed-included-defs"]    = "";
  opts["--do-not-feed-included-defs"] = "";
  opts["--no-exceptions"]         = "";
  opts["--codegen-c++"]           = "";
  opts["--no-codegen-c++"]        = "";
  opts["--codegen-idl"]           = "";
  opts["--no-codegen-idl"]        = "";
  opts["--codegen-midl"]          = "";
  opts["--no-codegen-midl"]       = "";
  opts["--codegen-wsdl"]          = "";
  opts["--no-codegen-wsdl"]       = "";
  opts["--c++-suffix"]            = "arg-expected";
  opts["--c++-impl"]              = "";
  opts["--c++-skel"]              = "";
  opts["--hh-prefix"]             = "arg-expected";
  opts["--hh-suffix"]             = "arg-expected";
  opts["--use-quotes"]            = "";
  opts["--relative-paths"]            = "";
  opts["--emit-repoids"]          = "";
  opts["--do-not-query-server-for-narrow"] = "";
  opts["--repo-id"]               = "arg-expected";
  opts["--name"]                  = "arg-expected";
  opts["--wsdl-map"]              = "arg-expected";
  opts["--ifdef-prefix"]          = "arg-expected";
  opts["--version"]               = "";
  opts["--config"]                = "";
  opts["--help"]                  = "";
  opts["--poa"]                   = "";
  opts["--no-poa"]                = "";
  opts["--boa"]                   = "";
  opts["--no-boa"]                = "";
  opts["--pseudo"]                = "";
  opts["--any"]                   = "";
  opts["--no-any"]                = "";
  opts["--typecode"]              = "";
  opts["--no-typecode"]           = "";
  opts["--poa-ties"]              = "";
  opts["--no-poa-ties"]           = "";
  opts["--no-poa-stubs"]          = "";
  opts["--windows-dll"]           = "arg-expected";
  opts["--windows-dll-with-export"] = "arg-expected";
  opts["--mico-core"]             = "";
  opts["--gen-included-defs"]     = "";
  opts["--gen-full-dispatcher"]   = "";
  opts["--throw-decl"]            = "";
  opts["--no-throw-decl"]         = "";
  opts["--idl3toidl2"]            = "";
  opts["--missing-nscorba"]       = "";
  opts["--not-missing-nscorba"]   = "";
  opts["--support-id"]            = "";
  opts["--dont-support-id"]       = "";
  opts["--wsi-wsdl"]              = "";
  opts["--no-wsi-wsdl"]           = "";
  opts["--cpp"]                   = "arg-expected";
  opts["--include-prefix"]        = "arg-expected";
  opts["--include-postfix"]       = "arg-expected";
  opts["--no-paths"]              = "";
  opts["--output-dir"]       	  = "arg-expected";
  opts["--skel-suffix"]       	  = "arg-expected";
  opts["--reflection"]            = "";
  opts["--no-reflection"]         = "";
  opts["--vc-sequence-reference-bug-workaround"] = "";
  opts["--no-vc-sequence-reference-bug-workaround"] = "";
  
  // Preprocessor options
  opts["-B"]                      = "arg-expected";
  opts["-I"]                      = "arg-expected";
  opts["-D"]                      = "arg-expected";
  opts["-E"]                      = "";
  opts["-M"]                      = "";
  opts["-MG"]                     = "";
  opts["-MM"]                     = "";
  opts["-MD"]                     = "";
  opts["-MMD"]                    = "";

  if (_argc < 2) {
    usage();
  }
  
  MICOGetOpt opt_parser( opts );
  if( !opt_parser.parse( _argc, _argv ) )
    usage();

  for( MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); i++ ) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "-B") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      base_dir = val;
    } else if (arg == "-I") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      cpp_options += " ";
      cpp_options += arg;
      cpp_options += val.empty() ? string(".") : val;
      // --include-prefix and --include-postfix refer to inc_paths.back()
      inc_paths.push_back (val);
      // default include path substitution is empty
      i_prefixes[val] = string();
    } else if (arg == "-D") {
      cpp_options += " ";
      cpp_options += arg + val;
    } else if( arg == "-E" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if( arg == "-M" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if( arg == "-MG" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if( arg == "-MM" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if( arg == "-MD" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if( arg == "-MMD" ) {
      cpp_options += " ";
      cpp_options += arg;
      _cpp_only = true;
    } else if (arg == "--feed-ir") {
      feed_ir = true;
    } else if( arg == "--feed-included-defs" ) {
      feed_included_defs = true;
    } else if( arg == "--do-not-feed-included-defs" ) {
      feed_included_defs = false;
    } else if (arg == "--no-exceptions") {
      no_exceptions = true;
    } else if (arg == "--codegen-c++") {
      codegen_cpp = true;
    } else if (arg == "--no-codegen-c++") {
      codegen_cpp = false;
    } else if (arg == "--codegen-idl") {
      codegen_idl = true;
    } else if (arg == "--no-codegen-idl") {
      codegen_idl = false;
    } else if (arg == "--codegen-midl") {
      codegen_midl = true;
    } else if (arg == "--codegen-wsdl") {
      codegen_wsdl = true;
    } else if (arg == "--no-codegen-midl") {
      codegen_midl = false;
    } else if (arg == "--no-codegen-wsdl") {
      codegen_wsdl = false;
    } else if( arg == "--c++-suffix" ) {
      cpp_suffix = val;
    } else if( arg == "--c++-impl" ) {
      cpp_impl = true;
    } else if( arg == "--c++-skel" ) {
      cpp_skel = true;
    } else if( arg == "--output-dir" ) {
      output_dir = val;
      if ( !output_dir.empty() && output_dir[output_dir.length() - 1] != '/' )
        output_dir += '/';
    } else if( arg == "--skel-suffix" ) {
      skel_suffix = val;
    } else if( arg == "--hh-prefix" ) {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      hh_prefix = val;
    } else if( arg == "--hh-suffix" ) {
      hh_suffix = val;
    } else if( arg == "--use-quotes" ) {
      use_quotes = true;
    } else if( arg == "--no-paths" ) {
      no_path_in_include = true;
    } else if( arg == "--relative-paths" ) {
      cerr << endl
	   << "WARNING: generation of include directives relative to default and" << endl
	   << "WARNING: user-defined include paths is now the standard mode. Please" << endl
	   << "WARNING: refer to the manual pages to learn about new options for" << endl
	   << "WARNING: modifying the generation of include directives." << endl
	   << "WARNING: To avoid this warning please remove ``--relative-paths'' parameter" << endl
	   << "WARNING: from the IDL compiler command-line." << endl;
    } else if (arg == "--emit-repoids") {
      emit_repoids = true;
    } else if( arg == "--do-not-query-server-for-narrow" ) {
      query_server_for_narrow = false;
    } else if (arg == "--repo-id") {
      repo_id = val;
    } else if (arg == "--name") {
      name = val;
    } else if (arg == "--wsdl-map") {
      wsdl_map = val;
    } else if (arg == "--ifdef-prefix") {
      ifdef_prefix = val;
    } else if (arg == "--boa") {
	cerr << "error: --boa option is no longer supported" << endl
	     << "       (if you need BOA support, please use MICO release older than 2.3.12)" << endl;
	exit(1);
    } else if (arg == "--no-boa") {
	cerr << "warning: --no-boa option is ignored" << endl
	     << "         (MICO release later than 2.3.11 does not support BOA anymore)" << endl;
      boa = false;
    } else if (arg == "--poa") {
      poa = true;
    } else if (arg == "--no-poa") {
      poa = false;
    } else if (arg == "--pseudo") {
      pseudo = true;
    } else if (arg == "--any") {
      any = true;
    } else if (arg == "--no-any") {
      any = false;
    } else if (arg == "--windows-dll") {
      windows_dll = true;
      dll_def_prefix = "";
      for (unsigned long i = 0; i < val.size(); i++) {
	  dll_def_prefix += toupper(val[i]);
      }
    } else if (arg == "--windows-dll-with-export") {
      windows_dll_with_export = true;
      dll_def_prefix = "";
      for (unsigned long i = 0; i < val.size(); i++) {
	  dll_def_prefix += toupper(val[i]);
      }
    } else if (arg == "--typecode") {
      typecode = true;
    } else if (arg == "--no-typecode") {
      typecode = false;
    } else if (arg == "--poa-ties") {
      poa_ties = true;
    } else if (arg == "--no-poa-ties") {
      poa_ties = false;
    } else if (arg == "--poa-stubs") {
      poa_stubs = true;
    } else if (arg == "--no-poa-stubs") {
      poa_stubs = false;
    } else if (arg == "--mico-core") {
      mico_core = true;
    } else if (arg == "--gen-included-defs") {
      gen_included_defs = true;
    } else if (arg == "--gen-full-dispatcher") {
      gen_full_dispatcher = true;
    } else if (arg == "--throw-decl") {
      throw_decl = true;
    } else if (arg == "--no-throw-decl") {
      throw_decl = false;
    } else if (arg == "--idl3toidl2") {
      idl3toidl2 = true;
    } else if (arg == "--reflection") {
      reflection = true;
    } else if (arg == "--no-reflection") {
      reflection = false;
    } else if (arg == "--vc-sequence-reference-bug-workaround") {
      vc_sequence_reference_bug_workaround = true;
    } else if (arg == "--no-vc-sequence-reference-bug-workaround") {
      vc_sequence_reference_bug_workaround = false;
    } else if (arg == "--missing-nscorba") {
      missing_nscorba = true;
    } else if (arg == "--not-missing-nscorba") {
      missing_nscorba = false;
    } else if (arg == "--support-id") {
      support_id = true;
    } else if (arg == "--dont-support-id") {
      support_id = false;
    } else if (arg == "--wsi-wsdl") {
      wsi_wsdl = true;
    } else if (arg == "--no-wsi-wsdl") {
      wsi_wsdl = false;
    } else if( arg == "--cpp" ) {
      cpp = val;
    } else if (arg == "--include-prefix") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      if (inc_paths.empty())
	usage();
      i_prefixes[inc_paths.back()] = val;
    } else if (arg == "--include-postfix") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      if (inc_paths.empty())
	usage();
      i_postfixes[inc_paths.back()] = val;
    } else if (arg == "--version") {
      cout << "MICO Version " << MICO_VERSION << endl;
      exit( 0 );
    } else if (arg == "--config") {
      config();
    } else if (arg == "--help") {
      usage();
    } else {
      usage();
    }
  }
  if (_argc > 2) {
    usage();
  }
  if (_argc == 2) {
    if (name == "")
      name = _argv[1];
    file = _argv[1];
  }
}

bool
IDLParam::cpp_only()
{
  return _cpp_only;
}

void
IDLParam::check_params()
{
  if (!codegen_idl && !codegen_midl && !codegen_wsdl && !codegen_cpp && !feed_ir) {

    if (file.length() == 0) {
      usage ();
    }

    codegen_cpp = true;
  }
#ifdef HAVE_EXCEPTIONS
  if( no_exceptions && !codegen_cpp ) {
    cerr << "error: --no-exceptions can only be used in conjunction ";
    cerr << "with --codegen-c++" << endl;
    exit( 1 );
  }
#endif
  if( feed_ir && file.length() == 0) {
    cerr << "error: need file name for --feed-ir" << endl;
    exit( 1 );
  }
  if( emit_repoids && !codegen_idl ) {
    cerr << "error: use --emit-repoids only in conjunction with --codegen-idl";
    cerr << endl;
    exit( 1 );
  }
  if( file.length() > 0 && name == file && codegen_idl ) {
    cerr << "error: use --name to specify a filename prefix";
    cerr << endl;
    exit( 1 );
  }
  if( pseudo && query_server_for_narrow ) {
    query_server_for_narrow = FALSE;
  }
  if( pseudo && !codegen_cpp ) {
    cerr << "error: --pseudo only works with --codegen-c++";
    cerr << endl;
    exit( 1 );
  }
  if( cpp_impl && !codegen_cpp ) {
    cerr << "error: --c++-impl can only be used in conjunction ";
    cerr << "with --codegen-c++" << endl;
    exit( 1 );
  }
  if( cpp_skel && !codegen_cpp ) {
    cerr << "error: --c++-skel can only be used in conjunction ";
    cerr << "with --codegen-c++" << endl;
    exit( 1 );
  }
  if( codegen_wsdl && wsdl_map == "" ) {
    cerr << "warning: --codegen-wsdl without --wsdl-map: wsdl services not emitted" << endl;
  }

  if( any )
    typecode = true;
}

void
IDLParam::usage()
{
  cerr << "usage: " << _argv[ 0 ] << " [<options>] [<file>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --help" << endl;
  cerr << "    --version" << endl;
  cerr << "    --config" << endl;
  cerr << "    --feed-ir" << endl;
  cerr << "    --feed-included-defs" << endl;
  cerr << "    --do-not-feed-included-defs" << endl;
  cerr << "    --no-exceptions" << endl;
  cerr << "    --codegen-c++" << endl;
  cerr << "    --no-codegen-c++" << endl;
  cerr << "    --codegen-idl" << endl;
  cerr << "    --no-codegen-idl" << endl;
  cerr << "    --codegen-midl" << endl;
  cerr << "    --no-codegen-midl" << endl;
  cerr << "    --cpp <cpp>" << endl;
  cerr << "    --c++-suffix <filename-suffix>" << endl;
  cerr << "    --c++-impl" << endl;
  cerr << "    --c++-skel" << endl;
  cerr << "    --codegen-wsdl" << endl;
  cerr << "    --no-codegen-wsdl" << endl;
  cerr << "    --hh-prefix <hh-prefix>" << endl;
  cerr << "    --hh-suffix <filename-suffix>" << endl;
  cerr << "    --use-quotes" << endl;
  cerr << "    --no-paths" << endl;
  cerr << "    --emit-repoids" << endl;
  cerr << "    --do-not-query-server-for-narrow" << endl;
  cerr << "    --repo-id <repository-id>" << endl;
  cerr << "    --name <filename-prefix>" << endl;
  cerr << "    --wsdl-map <filename>" << endl;
  cerr << "    --ifdef-prefix <prefix>" << endl;
  cerr << "    --poa" << endl;
  cerr << "    --no-poa" << endl;
  cerr << "    --boa    --    (deprecated)" << endl;
  cerr << "    --no-boa" << endl;
  cerr << "    --no-poa-ties" << endl;
  cerr << "    --no-poa-stubs" << endl;
  cerr << "    --pseudo" << endl;
  cerr << "    --any" << endl;
  cerr << "    --typecode" << endl;
  cerr << "    --windows-dll <dll-prefix>" << endl;
  cerr << "    --windows-dll-with-export <dll-prefix>" << endl;
  cerr << "    --mico-core" << endl;
  cerr << "    --gen-included-defs" << endl;
  cerr << "    --gen-full-dispatcher" << endl;
  cerr << "    --missing-nscorba" << endl;
  cerr << "    --not-missing-nscorba" << endl;
  cerr << "    --support-id" << endl;
  cerr << "    --dont-support-id" << endl;
  cerr << "    --wsi-wsdl" << endl;
  cerr << "    --no-wsi-wsdl" << endl;
  cerr << "    --include-prefix <include-prefix>" << endl;
  cerr << "    --include-postfix <include-postfix>" << endl;
  cerr << "    --reflection" << endl;
  cerr << "    --no-reflection" << endl;
  cerr << "    --vc-sequence-reference-bug-workaround" << endl;
  cerr << "    --no-vc-sequence-reference-bug-workaround" << endl;
  exit( 1 );
}

void
IDLParam::config()
{
  cout << "MICO version...........: " << MICO_VERSION;
#ifdef HAVE_THREADS
  cout << "(MT)";
#endif // HAVE_THREADS
  cout << endl;

  cout << "supported CORBA version: ";
  string version = MICO_VERSION;
  int i = version.find_last_of( "." );
  cout << version.substr( 0, i ) << endl;

  cout << "CCM support............: ";
#ifdef USE_CCM
  cout << "yes" << endl;
#else // USE_CCM
  cout << "no" << endl;
#endif // USE_CCM

  cout << "exceptions.............: ";
#ifdef HAVE_EXCEPTIONS
#ifdef HAVE_STD_EH
  cout << "CORBA compliant" << endl;
#else
  cout << "MICO specific" << endl;
#endif
#else
  cout << "no" << endl;
#endif

  cout << "modules are mapped to..: namespaces" << endl;

  cout << "STL is.................: ";
#ifdef HAVE_MINI_STL
  cout << "miniSTL" << endl;
#else
  cout << "system supplied" << endl;
#endif

  cout << "Security support.......: ";
#ifdef HAVE_SSL
  cout << "TLS/SSL";
#ifdef USE_CSIV2
  cout << ", CSIv2";
#endif // USE_CSIV2
#ifdef USE_CSL2
  cout << ", CSL2";
#endif // USE_CSL2
#else // HAVE_SSL
  cout << "no";
#endif
  cout << endl;

  cout << "loadable modules.......: ";
#ifdef HAVE_DYNAMIC
  cout << "yes" << endl;
#else
  cout << "no" << endl;
#endif

  cout << "multi-threading........: ";
#ifdef HAVE_THREADS
  cout << "yes (";
#else
  cout << "no" << endl;
#endif

#ifdef HAVE_PTHREADS
  cout << "Posix Draft 10 PThreads";
#endif // HAVE_PTHREADS

#ifdef HAVE_DCETHREADS
  cout << "OSF/Draft 4 (DCE) Threads";
#endif // HAVE_DCETHREADS

#ifdef HAVE_SOLARIS_THREADS
  cout << "Solaris Threads";
#endif // HAVE_SOLARIS_THREADS

#ifdef HAVE_PTH_THREADS
  cout << "GNU Pth Threads";
#endif // HAVE_PTH_THREADS

#ifdef HAVE_WIN_THREADS
  cout << "\"Windows\" Threads";
#endif // HAVE_WIN_THREADS

#ifdef HAVE_THREADS
  cout << ")" << endl;
#endif // HAVE_THREADS

  exit (0);
}

// rewrite path according to --no-paths / --include-prefix / --include-postfix
string
IDLParam::rewrite_include_path(string fname) const
{
  if (no_path_in_include) {
    string::size_type last_slash = fname.rfind('/');
    if (string::npos == last_slash) {
      return fname;
    } else if (fname.length() - 1 <= last_slash) {
      return string();
    } else {
      return fname.substr(last_slash + 1);
    }
    return fname;
  }

  // remove ./'s
  int pos;
  while ((pos = fname.find ("/./")) >= 0)
    fname.replace (pos, 2, "", 0);
  if (fname.find ("./") == 0)
    fname.replace (0, 2, "", 0);

  bool relativeFname = fname[0] != '/';

  map<string, string, std::less<std::string> >::const_iterator
    i_prefixes_match = i_prefixes.end();
  map<string, string, std::less<std::string> >::const_iterator
    i_postfixes_match = i_postfixes.end();
  for (map<string, string, std::less<std::string> >::const_iterator prefix_it = i_prefixes.begin();
    prefix_it != i_prefixes.end(); ++prefix_it) {
    if (fname.find(prefix_it->first) == 0) {
      if (i_prefixes.end() == i_prefixes_match ||
        prefix_it->first.length() > i_prefixes_match->first.length()) {
        i_prefixes_match = prefix_it;
      }
    }
  }
  for (map<string, string, std::less<std::string> >::const_iterator postfix_it = i_postfixes.begin();
    postfix_it != i_postfixes.end(); ++postfix_it) {
    if (fname.find(postfix_it->first) == 0) {
      if (i_postfixes.end() == i_postfixes_match ||
        postfix_it->first.length() > i_postfixes_match->first.length()) {
        i_postfixes_match = postfix_it;
      }
    }
  }
  if (i_prefixes.end() != i_prefixes_match &&
    (i_prefixes_match->first.length() || relativeFname)) {
    fname = fname.substr(i_prefixes_match->first.length());
    while (fname[0] == '/') {
      fname = fname.substr (1);
    }
    fname.insert(0, i_prefixes_match->second);
  }
  if (i_postfixes.end() != i_postfixes_match &&
    (i_postfixes_match->first.length() || relativeFname)) {
    std::string::size_type last_slash = fname.rfind('/');
    if (last_slash == std::string::npos) {
      last_slash = 0;
    } else {
      ++last_slash;
    }
    fname.insert(last_slash, i_postfixes_match->second);
  }
  return fname;
}
