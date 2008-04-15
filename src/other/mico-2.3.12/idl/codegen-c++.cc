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
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include "codegen-c++.h"
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-misc.h>

#endif // FAST_PCH


using namespace std;

static const char *copyright[] = {
  "/*",
  " *  MICO --- an Open Source CORBA implementation",
  " *  Copyright (c) 1997-2006 by The Mico Team",
  " *",
  " *  This file was automatically generated. DO NOT EDIT!",
  " */"
};

const int copyright_size = sizeof( copyright ) / sizeof( char * );



CodeGenCPP::CodeGenCPP( DB &db, IDLParam &params, CORBA::Container_ptr con )
  : CodeGen( con ),
    CodeGenCPPUtil( db, params, con ),
    CodeGenCPPCommon( db, params, con ),
    CodeGenCPPStub( db, params, con ),
    CodeGenCPPSkel( db, params, con ),
    CodeGenCPPImpl( db, params, con )
{
}


void CodeGenCPP::emit( string &fn )
{
  string fnbase       = fn;
  string fnHeader     = fnbase + "." + _params.hh_suffix;
  string fnStub       = fnbase + "." + _params.cpp_suffix;
  string fnSkel       = fnbase + _params.skel_suffix + "." + _params.cpp_suffix;
  string fnImplSuffix = "_impl";
  string fnImplH      = fnbase + fnImplSuffix + "." + _params.hh_suffix;
  string fnImplCPP    = fnbase + fnImplSuffix + "." + _params.cpp_suffix;

  for( string::size_type i = 0; i < fnbase.length(); i++ ) {
    fnbase[ i ] = isalnum( fnbase[ i ] ) ? toupper( fnbase[ i ] ) : '_';
  }
  string ifdef_name  = 
      (_params.ifdef_prefix == "") ? fnbase 
                                   : _params.ifdef_prefix + "_" + fnbase;

  CPPTypeFolder folder( _idl_objs );
  
  ofstream header( (_params.output_dir + fnHeader).c_str() );

  if (!header) {
    cerr << "error: cannot open file " << fnHeader << " for writing"
	 << endl;
    return;
  }

  for( int i0 = 0; i0 < copyright_size; i0++ )
    header << copyright[ i0 ] << endl;
  
  header << endl;
  header << "#include <CORBA.h>" << endl;
  if( _params.no_exceptions )
    header << "#define MICO_CONF_NO_EXCEPTIONS" << endl;
  if (!_params.mico_core)
    header << "#include <mico/throw.h>" << endl;

  header << endl;
  header << "#ifndef __" << ifdef_name << "_H__" << endl;
  header << "#define __" << ifdef_name << "_H__" << endl;
  header << endl << endl;

  if (_params.windows_dll || _params.windows_dll_with_export) {
      // win32 export/import handling
      string compile_def = "BUILD_" + _params.dll_def_prefix + "_DLL";
      string export_def = _params.dll_def_prefix + "_EXPORT";
      header << "#ifdef _WIN32" << endl;
      if (!_params.mico_core) {
	  header << "// If you compile this file and would like to link" << endl
		 << "// it into the DLL, then please define ``" << compile_def << "'' symbol" << endl
		 << "// on your C++ compiler command-line." << endl
		 << "// Depending on your compiler, you can use either" << endl
		 << "// ``-D" << compile_def << "'' or ``/D" << compile_def << "'' parameter" << endl;
      }
      header << "#ifdef " << compile_def << endl;
      if (_params.windows_dll_with_export) {
	  header << "#define " << export_def << " __declspec(dllexport)" << endl;
      }
      else {
	  if (!_params.mico_core) {
	      header << "// Please do not forget to use gendef tool to correctly" << endl
		     << "// export all symbols from the resulting object file." << endl
		     << "// If you would like to use more common __declspec(dllexport) way," << endl
		     << "// then please use --windows-dll-with-export <name>" << endl
		     << "// IDL compiler parameter" << endl;
	  }
	  header << "#define " << export_def << " /**/" << endl;
      }
      header << "#else // " << compile_def << endl;
      header << "#define " << export_def << " __declspec(dllimport)" << endl;
      header << "#endif // " << compile_def << endl;
      header << "#else // _WIN32" << endl;
      if (!_params.mico_core) {
	  header << "// all other platforms" << endl;
      }
      header << "#define " << export_def << " /**/" << endl;
      header << "#endif // _WIN32" << endl;
      _params.tc_export = "extern " + export_def;
      if (_params.windows_dll_with_export)
	  _params.class_export = export_def + " ";
      if (_params.windows_dll_with_export && export_def != "")
	  _params.operator_export = export_def + " ";
      if (_params.windows_dll && export_def != "")
	  _params.static_tc_export = "static " + export_def;
  }
  header << endl << endl;

  if (_params.reflection) {
    header << "#include <mico/reflection.h>" << endl << endl;
  }

  o.start_output( header );
  emit_common();
  o.stop_output();

  header << "#endif" << endl;
  
  ofstream impl( (_params.output_dir + fnStub).c_str() );

  if (!impl) {
    cerr << "error: cannot open file " << fnStub << " for writing"
	 << endl;
    return;
  }

  for( int i1 = 0; i1 < copyright_size; i1++ )
    impl << copyright[ i1 ] << endl;

  impl << endl;

  if (!_params.mico_core) {
    impl << "#include ";
    impl << ( _params.use_quotes ? "\"" : "<" );
    impl << _params.hh_prefix << fnHeader;
    impl << ( _params.use_quotes ? "\"" : ">" );
    impl << endl << endl;
  }
  else {
    impl << "#include <CORBA.h>" << endl;
    if( _params.no_exceptions ) {
      impl << "#define MICO_CONF_NO_EXCEPTIONS" << endl;
    }
    impl << "#include <mico/throw.h>" << endl;
    impl << "#include <mico/template_impl.h>" << endl << endl;
  }

  if (_params.reflection) {
    impl << "#include <mico/reflection.h>" << endl << endl;
  }

  impl << endl;
  impl << "using namespace std;" << endl;
  impl << endl;

  impl << "//--------------------------------------------------------" << endl;
  impl << "//  Implementation of stubs" << endl;
  impl << "//--------------------------------------------------------" << endl;

  o.start_output( impl );
#ifdef WINDOWS_TC
  o.switchStream( 1 );
  o << "struct __tc_init_" << fnbase << " {" << endl;
  o << indent << "__tc_init_" << fnbase << "()" << endl;
  o << BL_OPEN;

  o.switchStream( 2 );
  o << indent << indent;
  
  o.switchStream( 3 );
  o << indent << "~__tc_init_" << fnbase << "()" << endl;
  o << BL_OPEN;

  o.switchStream( 0 );
#endif
  
  emit_stub();

#ifdef WINDOWS_TC
  o.switchStream( 2 );
  o << BL_CLOSE << endl;

  o.switchStream( 3 );
  o << BL_CLOSE << BL_CLOSE_SEMI << endl;

  o << "static __tc_init_" << fnbase << " __init_" << fnbase << ";" << endl;
  o << endl;
  o.switchStream( 0 );
#endif
  o.stop_output();

  if (_params.pseudo) {
    return;
  }

  if (_params.cpp_skel) {
    ofstream skel( (_params.output_dir + fnSkel).c_str() );

    if (!skel) {
      cerr << "error: cannot open file " << fnSkel << " for writing"
	   << endl;
      return;
    }

    for( int i1 = 0; i1 < copyright_size; i1++ )
      skel << copyright[ i1 ] << endl;
  
    if (!_params.mico_core) {
      skel << endl;
      skel << "#include ";
      skel << ( _params.use_quotes ? "\"" : "<" );
      skel << _params.hh_prefix << fnHeader;
      skel << ( _params.use_quotes ? "\"" : ">" );
      skel << endl << endl;
    }
    else {
      skel << endl;
      skel << "#include <CORBA.h>" << endl;
      if( _params.no_exceptions ) {
        skel << "#ifdef HAVE_EXCEPTIONS" << endl;
        skel << "#undef HAVE_EXCEPTIONS" << endl;
        skel << "#include <mico/throw.h>" << endl;
        skel << "#define HAVE_EXCEPTIONS 1" << endl;
        skel << "#else" << endl;
        skel << "#include <mico/throw.h>" << endl;
        skel << "#endif" << endl;
      } else {
        skel << "#include <mico/throw.h>" << endl;
      }
      skel << "#include <mico/template_impl.h>" << endl << endl;
    }

    if (_params.reflection) {
      skel << "#include <mico/reflection.h>" << endl << endl;
    }

    skel << "//--------------------------------------------------------"
	 << endl;
    skel << "//  Implementation of skeletons"
	 << endl;
    skel << "//--------------------------------------------------------"
	 << endl;

    o.start_output( skel );
    emit_skel();
    o.stop_output();
  } else {
    impl << "//--------------------------------------------------------"
	 << endl;
    impl << "//  Implementation of skeletons"
	 << endl;
    impl << "//--------------------------------------------------------"
	 << endl;

    o.start_output( impl );
    emit_skel();
    o.stop_output();
  }


  if( !_params.cpp_impl ) {
    // no c++ implementation generation requested
    return;
  }

  // ------- implementation generator: start --------
  // Never overwrite files, since this could cause
  // a programmer to loose his entire implementation

  if (OSMisc::access (fnImplH.c_str(), OSMisc::ACCESS_READ) == 0) {
    cerr << "warning: C++ implementation generation: "
	 << "file " << fnImplH << " already exists!" << endl;
    cerr << "warning: C++ implementation generation cancelled!" << endl;
    return;
  }

  if (OSMisc::access (fnImplCPP.c_str(), OSMisc::ACCESS_READ) == 0) {
    cerr << "warning: C++ implementation generation: "
	 << "file " << fnImplCPP << " already exists!" << endl;
    cerr << "warning: C++ implementation generation cancelled!" << endl;
    return;
  }

  ofstream implheader( (_params.output_dir + fnImplH).c_str() );

  if (!implheader) {
    cerr << "error: cannot open file " << fnImplH << " for writing"
	 << endl;
    return;
  }

  ofstream implcpp( (_params.output_dir + fnImplCPP).c_str() );

  if (!implcpp) {
    cerr << "error: cannot open file " << fnImplCPP << " for writing"
	 << endl;
    return;
  }

  // ----- implementation generator: header -------
  implheader << endl;
  implheader << "#ifndef __" << ifdef_name << "_IMPL_H__" << endl;
  implheader << "#define __" << ifdef_name << "_IMPL_H__" << endl;
  implheader << endl;
  implheader << "#include ";
  implheader << ( _params.use_quotes ? "\"" : "<" );
  implheader << _params.hh_prefix << fnHeader;
  implheader << ( _params.use_quotes ? "\"" : ">" );
  implheader << endl << endl;

  o.start_output( implheader );
  emit_impl_h();
  o.stop_output();

  implheader << endl << "#endif" << endl;
  
  // ---- implementation generator: c++-file ------
  implcpp << endl;
  implcpp << "#include ";
  implcpp << ( _params.use_quotes ? "\"" : "<" );
  implcpp << _params.hh_prefix << fnImplH;
  implcpp << ( _params.use_quotes ? "\"" : ">" );
  implcpp << endl << endl;

  o.start_output( implcpp );
  emit_impl_cpp();
  o.stop_output();
}
