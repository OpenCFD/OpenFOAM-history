/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
#include <mico/os-misc.h>
#include "parser.h"
#include "idlparser.h"
#include "codegen-c++.h"
#include "codegen-idl.h"
#include "codegen-midl.h"
#include "codegen-wsdl.h"
#include "params.h"
#include "db.h"

#endif // FAST_PCH


using namespace std;

#ifdef USE_CCM
CORBA::Container_ptr
CCMTransform (DB & db, IDLParam & params,
	      CORBA::Repository_ptr repo,
	      CORBA::Container_ptr cont);
#endif

CORBA::Container_ptr
IRCopier (DB & db, IDLParam & params,
	  CORBA::Repository_ptr repo,
	  CORBA::Container_ptr cont);


int main( int argc, char *argv[] )
{
  CORBA::Container_var container;
  CORBA::Repository_var repository, c2repo;
  DB db;

  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

#ifdef _WIN32
  // Some parts of the idl-compiler can't handle backslashes in paths
  // replace all the backslashes by slashes
  // runtime library can handle slashes fine
  // assume that only paths have backslashes(true for now)
  // (changing the argv[] is allowed)
  for(int i=1; i<argc; i++)
  {
      char *argp=argv[i];
      while(argp && *argp)
      {
          if(*argp=='\\')
              *argp='/';
          
          argp++;
      }
  }
  
#endif

  /*
   * Setup
   */

  IDLParam params( argc, argv );

  /*
   * Input file processing, data is loaded into local IR first
   */

  if( params.file.length() > 0 ) {

    repository = MICO::create_interface_repository (orb);
    assert (!CORBA::is_nil (repository));

    /*
     * Step 1: Invoke the Preprocessor
     */

    string cmd = params.cpp;
    cmd += " -D __MICO_IDL__ ";
    cmd += params.cpp_options;
    cmd += ' ';

    string includepath;
    if (params.base_dir.empty()) {
      includepath = ABSEXECDIR;
      if (!includepath.empty() && includepath[includepath.length() - 1] != '/')
        includepath += '/';
    } else {
      includepath = params.base_dir;
    }
    includepath += "include/";
    string micopath = includepath + "mico/";

    if (params.i_prefixes.end() == params.i_prefixes.find(includepath)) {
      params.i_prefixes[includepath] = string();
    }
    if (params.i_prefixes.end() == params.i_prefixes.find(micopath)) {
      params.i_prefixes[micopath] = "mico/";
    }

    cmd += "-isystem ";
    cmd += includepath;
    cmd += ' ';
    cmd += " -isystem ";
    cmd += micopath;
    cmd += ' ';

    cmd += params.file;

    FILE* inp_file = OSMisc::popen( cmd.c_str(), "r" );

    if( inp_file == NULL ) {
      cerr << "error: cannot open input file: " << params.file << endl;
      exit( 1 );
    }
    
    int dummy_char = fgetc (inp_file);
    if (dummy_char == EOF) {
      cerr << "error: cannot execute " << params.cpp << endl;
      exit( 1 );
    }
    ungetc (dummy_char, inp_file);

    if( params.cpp_only() ) {
      int ch = fgetc( inp_file );
      while( ch != EOF ) {
	fputc( ch, stdout );
	ch = fgetc( inp_file );
      }
      exit( 0 );
    }

    /*
     * Step 2: Invoke the Parser (bison)
     */
    
    Parser parser( inp_file, params.file.c_str() );
    parser.parse();

    OSMisc::pclose( inp_file );

    /*
     * Step 3: Traverse the Parse Tree and load Interface Repository
     */

    db.set_toplevel_fname (params.file.c_str());
    IDLParser idlParser (db, params);
    idlParser.collect (repository, parser.getRootNode());
    db.set_repoids (repository);
  }
  else {
    /*
     * If no input file, try to connect to a remote IR
     */

    CORBA::Object_var obj =
      orb->resolve_initial_references ("InterfaceRepository");

    repository = CORBA::Repository::_narrow (obj);

    if (CORBA::is_nil (repository)) {
      cerr << "error: cannot connect to remote interface repository." << endl;
      exit (1);
    }
  }

  /*
   * At this time, we are connected to an Interface Repository, and
   * this IR has been loaded with the contents of the IDL file (if
   * given on the command line).
   */

  /*
   * Determine which portions of the IR to dump. This is selected by
   * using the --repo-id option on the command line, else the whole
   * contents are dumped.
   */

  container = CORBA::Container::_duplicate (repository);
  
  if (params.repo_id != "" && params.repo_id != "::") {
    CORBA::Contained_var contained =
      repository->lookup_id( (char *) params.repo_id.c_str() );
    if( CORBA::is_nil( contained ) ) {
      contained = repository->lookup( (char *) params.repo_id.c_str() );
      if( CORBA::is_nil( contained ) ) {
	cerr << "error: repo-id " << params.repo_id << " not found in IR";
	  cerr << endl;
	  exit( 1 );
      }
    }
    container = CORBA::Container::_narrow( contained );
    if( CORBA::is_nil( container ) ) {
      cerr << "error: repo-id " << params.repo_id << " is not a container";
      cerr << endl;
      exit( 1 );
    }
    if( container->def_kind() != CORBA::dk_Module ) {
      cerr << "error: can only generate code from module";
      cerr << endl;
      exit( 1 );
    }
  }

  /*
   * Determine output file name
   */
  
  string name = params.name;
  string::size_type pos = name.rfind( "." );
  if( (int)pos >= 0 && pos < name.length() )
    name = name.substr( 0, pos );
  pos = name.rfind( "/" );
#ifdef _WIN32
   if (pos==string::npos)
     pos = name.rfind( "\\" );
#endif
  if( (int)pos >=0 && pos < name.length() )
    name = name.substr( pos + 1 );
  
  /*
   * Translate from IDL 3 to IDL 2 if necessary
   */

#ifdef USE_CCM
  if (params.codegen_cpp) {
    /*
     * See if there are Homes or Components in the IFR. If yes, then the
     * CCM Transformer must be invoked.
     *
     * Unfortunately, the IFR does not provide a means of globally searching
     * for some def_kind. So we have to do a classic BFS here.
     */

    vector<CORBA::Container_var> cs;
    CORBA::ULong pos = 0;

    cs.push_back (CORBA::Container::_duplicate (container));

    while (pos < cs.size()) {
      CORBA::ContainedSeq_var tmp = cs[pos]->contents (CORBA::dk_Component, 1);
      if (tmp->length() > 0) {
	params.idl3toidl2 = true;
	break;
      }
      tmp = cs[pos]->contents (CORBA::dk_Home, 1);
      if (tmp->length() > 0) {
	params.idl3toidl2 = true;
	break;
      }
      tmp = cs[pos]->contents (CORBA::dk_Event, 1);
      if (tmp->length() > 0) {
	params.idl3toidl2 = true;
	break;
      }
      tmp = cs[pos]->contents (CORBA::dk_Module, 1);
      for (CORBA::ULong ti=0; ti<tmp->length(); ti++) {
	cs.push_back (CORBA::Container::_narrow (tmp[ti]));
      }
      pos++;
    }
  }

  if (params.idl3toidl2) {
    /*
     * Invoke the CCM Transformer, which duplicates the IFR, applying the
     * equivalent IDL transformations in the process
     */

    c2repo = MICO::create_interface_repository (orb);
    container = CCMTransform (db, params, c2repo, container);
    assert (!CORBA::is_nil (container));
    db.clear_implicits ();
  }
#endif

  /*
   * Invoke Code Generator
   */

  if( params.codegen_cpp ) {
    CodeGen *gen = new CodeGenCPP( db, params, container );
    gen->emit( name );
    delete gen;
  }

  if( params.codegen_idl ) {
    CodeGen *gen = new CodeGenIDL( db, params, container );
    gen->emit( name );
    delete gen;
  }
  
  if( params.codegen_midl ) {
    CodeGen *gen = new CodeGenMIDL( db, container, params.emit_repoids );
    gen->emit( name );
    delete gen;
  }
  
  if( params.codegen_wsdl ) {
    CodeGen *gen = new CodeGenWSDL( db, params, container );
    gen->emit( name );
    delete gen;
  }

  if (params.feed_ir) {
    CORBA::Object_var obj =
      orb->resolve_initial_references ("InterfaceRepository");
    CORBA::Repository_var remorepo = CORBA::Repository::_narrow (obj);

    if (CORBA::is_nil (remorepo)) {
      cerr << "error: cannot connect to remote interface repository." << endl;
      exit (1);
    }

    CORBA::Container_var dummy =
      IRCopier (db, params, remorepo, container);
  }

  return 0;
}
