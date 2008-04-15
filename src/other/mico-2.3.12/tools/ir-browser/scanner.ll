/*  -*- c++ -*- */
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

%{
#include <unistd.h>
#include <iostream.h>
#include <fstream.h>
#include <string>

#if defined(_WIN32) && defined(_MSC_VER)
#define isatty _isatty
#endif


using namespace std;

#include "cg.h"
#include "yacc.h"


int line_no = 1;
char *lexFile;

%}

%option c++
%option noyywrap

%x comment


%%

[ \t]			;
[\n]			line_no++;
"/*"			BEGIN(comment);
<comment>[^*\n]*	;
<comment>"*"+[^*/\n]*	;
<comment>\n		line_no++;
<comment>"*"+"/"	BEGIN(INITIAL);


@invoke			return INVOKE;
@bind			return BIND;
@release		return RELEASE;
@error			return ERROR;
@answer-graph		return ANSWER_GRAPH;
@answer-key		return ANSWER_KEY;
@answer-ok		return ANSWER_OK;

("-"|"+")?[0-9]+	{
			   yylval.val = atoi( yytext );
			   return VALUE;
			}
[a-zA-Z][a-zA-Z0-9_-]*	{
                           yylval.str = new string( yytext );
                           return STRING;
                        }
\"[^\"]*\"		{
			   yylval.str =
				new string( string( yytext ).substr( 1,
                                                        strlen(yytext) - 2 ) );
			   return STRING;
			}
\'[^\']*\'		{
			   yylval.str =
				new string( string( yytext ).substr( 1,
                                                        strlen(yytext) - 2 ) );
			   return STRING;
			}
"["			return CONCEPT_OPEN;
"]"			return CONCEPT_CLOSE;
"("			return RELATION_OPEN;
")"			return RELATION_CLOSE;
":"			return COLON;
","			return COMMA;
"->"			return CONNECTS;
"-"			return SUBGRAPH_OPEN;
"."			return SUBGRAPH_CLOSE;
"="			return EQUAL;
.                       {
  //cout << YYText() << endl;
  //return TOKEN_UNDEF;
                        }


