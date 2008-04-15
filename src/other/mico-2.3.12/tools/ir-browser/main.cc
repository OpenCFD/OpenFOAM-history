/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
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
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream.h>
#include <strstream.h>
#include <CORBA.h>
#include "impl.h"
#include "parser.h" 
#include <mico/template_impl.h>


class Application {
private:
  int       argc;
  char      **argv;
  DII       *dii;
  void processDII( char *Name );
public:
  Application( int an_argc, char *an_argv[] );
  ~Application();
  void run();
};


Application::Application( int an_argc, char *an_argv[] )
{
  argc = an_argc;
  argv = an_argv;
  dii = new DII();
}


Application::~Application()
{
  if( dii )
    delete dii;
}


void Application::processDII( char *name )
{
  ifstream file( name );
  char ch;
  
  if( !file ) {
    printf( "Couldn't open file '%s'\n", name );
    exit( -1 );
  }
  ostrstream ostr;
  while( !file.eof() ) {
    ch = file.get();
    ostr << ch;
  }
  ostr << ends;
  istrstream istr( ostr.str() );
  ostr.rdbuf()->freeze( 0 );

  Parser *parser = new Parser( dii, istr, name );
  parser->parse();
  if( parser->errorOccured() ) {
    cerr << parser->getErrorFile();
    cerr << "(" << parser->getErrorLine() << "): ";
    cerr << parser->getErrorMsg() << endl;
    exit( -1 );
  }
  delete parser;
  dii->invoke();
  dii->getCG()->prettyPrint (cout, 4);
  cout << endl;
}

static void usage (char *appname)
{
  cerr.form( "Usage: %s <input-file>\n", appname );
  exit( 1 );
}

void Application::run()
{
  if( argc != 2 ) {
    usage( argv[ 0 ] );
  }
  processDII( argv[ 1 ] );
}

/***********************************************************************/


int main( int argc, char *argv[] )
{
    CalcImpl impl;    
    Application app( argc, argv );
    
    app.run();
}

