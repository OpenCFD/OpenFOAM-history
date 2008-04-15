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

#include <string.h>
#include <iostream.h>
#ifdef _WIN32
#include <malloc.h>
#endif

#include "cg.h"
#include "parser.h"
#include <mico/template_impl.h>

extern int yylex();
extern int line_no;
extern char *lexFile;


void yyerror( char *s )
{
  if( strlen( s ) == 0 )
    s = "parse error";
  theParser->setError( lexFile, s, line_no );
}


%}

%union {
	int		    val;
	string             *str;
	SetOfString        *strs;
	Node		   *node;
	ConceptualGraph	   *cg;
}

%token		INVOKE
%token		BIND
%token		RELEASE
%token		ERROR
%token		ANSWER_GRAPH
%token		ANSWER_KEY
%token		ANSWER_OK

%token		TOKEN_UNDEF
%token		CONCEPT_OPEN
%token		CONCEPT_CLOSE
%token		COLON
%token		RELATION_OPEN
%token		RELATION_CLOSE
%token		CONNECTS
%token		COMMA
%token		SUBGRAPH_OPEN
%token		SUBGRAPH_CLOSE
%token		EQUAL

%token		BRACKET_OPEN
%token		BRACKET_CLOSE
%token		SEMICOLON

%token <str>	STRING
%token <val>	VALUE

%type <str>     string
%type <strs>    string_list

%type <node>	concept_node
%type <node>	relation_node
%type <cg>	cg
%type <cg>	con_connects_to
%type <cg>	rel_graph
%type <cg>	con_subgraph
%type <cg>	rel_subgraph

%%

grammar
	:
	| grammar simple_command
	;

simple_command
	: INVOKE string COLON cg
	    {
		DIIError e = dii->invoke (*$2, $4);
		if (e.errorOccured()) {
		    dii->setError (1, e.getErrorMsg());
		} else {
		    dii->setAnswerGraph ($4);
		}
		delete $2;
		delete $4;
	    }
	| BIND string
	    {
		string key;
		DIIError e = dii->bind (*$2, key);
		if (e.errorOccured()) {
		    dii->setError (1, e.getErrorMsg());
		} else {
		    dii->setAnswerKey (key);
		}
		delete $2;
	    }
	| RELEASE string
	    {
		DIIError e = dii->release (*$2);
		if (e.errorOccured()) {
		    dii->setError (1, e.getErrorMsg());
		} else {
		    dii->setAnswerOk ();
		}
		delete $2;
	    }
	| ANSWER_GRAPH EQUAL cg
	    {
		// XXX
		delete $3;
	    }
	| ANSWER_KEY string
	    {
		// XXX
		delete $2;
	    }
	| ANSWER_OK
	    {
		// XXX
	    }
	| ERROR VALUE COLON string
	    {
		// XXX
		delete $4;
	    }
        ;

string
	: STRING
	    {
		$$ = $1;
	    }
	;

string_list
	: string
	    {
		SetOfString *sl = new SetOfString();
		sl->push_back( *$1 );
		delete $1;
		$$ = sl;
	    }
	| string_list COMMA string
	    {
		$1->push_back( *$3 );
		delete $3;
		$$ = $1;
	    }
	;


cg
	: concept_node con_connects_to
	    {
		$2->setNode( $1 );
		$$ = $2;
	    }
	;

con_connects_to
	:
	    {
		$$ = new ConceptualGraph( NULL );
	    }
	| CONNECTS rel_graph
	    {
		$$ = new ConceptualGraph( NULL );
		$$->setWidth( $2 );
	    }
	| SUBGRAPH_OPEN rel_subgraph SUBGRAPH_CLOSE
	    {
		$$ = new ConceptualGraph( NULL );
		$$->setWidth( $2 );
	    }
	;

rel_graph
	: relation_node CONNECTS cg
	    {
		$$ = new ConceptualGraph( $1, $3 );
	    }
	| relation_node SUBGRAPH_OPEN con_subgraph SUBGRAPH_CLOSE
	    {
		$$ = new ConceptualGraph( $1, $3 );
	    }
	;

con_subgraph
	: CONNECTS cg
	    {
		$$ = $2;
	    }
	| CONNECTS cg COMMA con_subgraph
	    {
		$2->setDepth( $4 );
		$$ = $2;
	    }
	;

rel_subgraph
	: CONNECTS rel_graph
	    {
		$$ = $2;
	    }
	| CONNECTS rel_graph COMMA rel_subgraph
	    {
		$2->setDepth( $4 );
		$$ = $2;
	    }
	;

concept_node
	: CONCEPT_OPEN string CONCEPT_CLOSE
	    {
		$$ = new ConceptNode( *$2 );
		delete $2;
	    }
	| CONCEPT_OPEN string COLON VALUE CONCEPT_CLOSE
	    {
		ConceptNode *con = new ConceptNode( *$2 );
		con->setValue( (long) $4 );
		$$ = con;
		delete $2;
	    }
	| CONCEPT_OPEN string COLON string_list CONCEPT_CLOSE
	    {
		$$ = new ConceptNode( *$2, *$4 );
		delete $2;
		delete $4;
	    }
	;

relation_node
	: RELATION_OPEN string RELATION_CLOSE
	    {
		$$ = new RelationNode( *$2 );
		delete $2;
	    }
	;


