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

#include "parser.h"
#include <mico/template_impl.h>


extern int line_no;


static yyFlexLexer *lexer;

DII *dii;
Parser *theParser;



int yylex()
{
    return lexer->yylex();
}


Parser::Parser( DII *_dii, istream &istr, const char *filename )
{
  dii = _dii;
  lexFile = (char *)filename;
  lexer = new yyFlexLexer( &istr );
}


Parser::~Parser()
{
  delete lexer;
}


void Parser::parse()
{
  theParser = this;
  line_no = 0;
  errorMsg = NULL;
  yyparse();
}


void Parser::setError( char *file, char *err, int line )
{
  errorFile = file;
  errorMsg  = err;
  errorLine = line;
}


bool Parser::errorOccured()
{
  return errorMsg != NULL;
}


char *Parser::getErrorFile()
{
  return errorFile;
}


char *Parser::getErrorMsg()
{
  return errorMsg;
}


int Parser::getErrorLine()
{
  return errorLine;
}

