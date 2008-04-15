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
#include "ir_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <mico/ir_creator.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include <parser.h>
#include <idlparser.h>
#include <codegen-idl.h>
#include <params.h>

#endif // FAST_PCH


using namespace std;

#ifndef CPP
#define CPP "mico-cpp"
#endif


static CORBA::Repository_ptr therepo;
static string thedb;

void save_db ()
{
  if (thedb.length() > 0) {
    cerr << "saving database " << thedb << ".idl ... " << flush;
    IDLParam params;
    DB db;
    params.emit_repoids = true;
    CodeGenIDL gen (db, params, therepo);
    gen.emit (thedb);
    cerr << "done." << endl;  
  }
}

void
sighandler (int)
{
  save_db ();
  exit (0);
}

void usage( const char *progname )
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --help" << endl;
  cerr << "    --db <idl database file>" << endl;
  cerr << "    --ior <IOR ref file>" << endl;
  exit( 1 );
}

int main (int argc, char *_argv[])
{
  signal (SIGINT, sighandler);
  signal (SIGTERM, sighandler);

  /*
   * also add -POAImplName if missing...
   */

  const char * implname = NULL;
  char **argv = new char*[argc+2+1];

  for (int i1=0; i1<argc; ++i1) {
    if (strcmp (_argv[i1], "-POAImplName") == 0 && i1+1<argc) {
      implname = _argv[i1+1];
    }
    argv[i1] = _argv[i1];
  }

  if (!implname) {
    argv[argc++] = "-POAImplName";
    argv[argc++] = "InterfaceRepository";
  }

  argv[argc] = 0;

  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  MICOGetOpt::OptMap opts;
  opts["--help"] = "";
  opts["--db"]   = "arg-expected";
  opts["--ior"]  = "arg-expected";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  string reffile;

  for (MICOGetOpt::OptVec::const_iterator i2 = opt_parser.opts().begin();
       i2 != opt_parser.opts().end(); ++i2) {
    string arg = (*i2).first;
    string val = (*i2).second;

    if (arg == "--db") {
      thedb = val;
    } else if (arg == "--ior") {
      reffile = val;
    } else if (arg == "--help") {
      usage( argv[ 0 ] );
    } else {
      usage( argv[ 0 ] );
    }
  }
  if (argc != 1) {
    usage( argv[ 0 ] );
  }

  therepo = MICO::create_interface_repository (orb, 1);
  orb->set_initial_reference ("InterfaceRepository", therepo);
  assert (!CORBA::is_nil (therepo));

  if (thedb.length() > 0) {
      string fn = thedb + ".idl";

      if (OSMisc::access (fn.c_str(), OSMisc::ACCESS_READ)) {
	  cerr << "warning: database " << fn << " does not exist." << endl;
      } else {
	  string cmd = string("") + CPP + " " + fn;
	  FILE* inp_file = OSMisc::popen( cmd.c_str(), "r" );

	  if (inp_file == NULL) {
	      cerr << "warning: cannot open database: " << fn << endl;
	  } else {
              int dummy_char = fgetc (inp_file);
              if (dummy_char == EOF) {
                  cerr << "error: cannot execute " << CPP << endl;
                  exit( 1 );
              }
              ungetc (dummy_char, inp_file);

	      cerr << "reading in database " << fn << " ... " << flush;
	      Parser parser (inp_file, fn.c_str());
	      parser.parse();

	      DB db;
	      IDLParam params;
	      IDLParser idlparser (db, params);
	      idlparser.collect (therepo, parser.getRootNode());
	      db.set_repoids (therepo);
	      cerr << "done." << endl;
	  }
	  OSMisc::pclose (inp_file);
      }
  }

  if (reffile.length() > 0) {
    CORBA::String_var repoior = orb->object_to_string (therepo);

    if (reffile == "-") {
      cout << repoior << endl;
    }
    else {
      ofstream out (reffile.c_str());
      if (!out) {
	cerr << "error: cannot open output file " << reffile << endl;
      }
      else {
	out << repoior << endl;
	out.close ();
      }
    }
  }

  orb->run ();
  return 0;
}
