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

#ifndef __PARSER_H__
#define __PARSER_H__

#include <CORBA.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

#include "parsenode.h"
#include "prepro.h"


class Parser;

extern Parser *theParser;  // Defined in parser.cc
extern int yyparse();      // Defined through yacc.y


class Parser 
{
private:
  ParseNode *rootNode;

public:
  Parser( FILE* inp_file, const char *filename = NULL );
  ~Parser ();
  void parse();

  void setRootNode( ParseNode *node );
  ParseNode *getRootNode();

  void parse_error( char *file, char *err, int line );
};


#endif
