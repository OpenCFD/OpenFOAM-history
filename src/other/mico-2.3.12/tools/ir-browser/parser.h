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

#ifndef __PARSER_H__
#define __PARSER_H__

#ifdef _WIN32
#include <iostream.h>
#endif

#include <fstream.h>
#include <FlexLexer.h>
#include "dii.h"

class Parser;

extern char *lexFile;      // Defined in scanner.l
extern Parser *theParser;  // Defined in parser.cc
int yyparse();             // Defined through yacc.y

extern DII *dii;           // Defined in parser.cc

class Parser 
{
private:
  char   *errorFile;
  char   *errorMsg;
  int     errorLine;
public:
  Parser( DII *dii, istream &istr, const char *filename = NULL );
  ~Parser();
  void parse();
  void setError( char *file, char *err, int line );
  bool errorOccured();
  char *getErrorFile();
  char *getErrorMsg();
  int   getErrorLine();
};


#endif
