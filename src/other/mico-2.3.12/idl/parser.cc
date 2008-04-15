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
#include "parser.h"
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

extern FILE* yyin;
extern char* yytext;
extern int yylex();
/* extern int yydebug; */

Parser *theParser;


Parser::Parser( FILE *inp_file, const char *filename )
{
  idl_lexFile = toplevelFile = (char *) filename;
  yyin = inp_file;
  rootNode = 0;
}

Parser::~Parser ()
{
  delete rootNode;
}

void Parser::parse()
{
  Parser * oldParser = theParser;
  theParser = this;
  idl_line_no = 0;
  /* yydebug = 1; */
  yyparse();
  theParser = oldParser;
}


void Parser::setRootNode( ParseNode *node )
{
  rootNode = node;
}


ParseNode *Parser::getRootNode()
{
  return rootNode;
}

void Parser::parse_error( char *file, char *err, int line )
{
  string msg;
  
  msg = err;
  msg += " before '";
  msg += yytext;
  msg += "'";
  
  cerr << file << ":" << line << ": " << msg << endl;
  exit( 1 );
}

