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

%{

#include <CORBA.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef _WIN32
#include <malloc.h>
#endif

#include "parser.h"
#include "parsenode.h"
#include "prepro.h"
#include <mico/template_impl.h>


using namespace std;

extern int yylex();

void yyerror( char *s )
{
  theParser->parse_error( idl_lexFile, s, idl_line_no );
}

%}


%union 
{
  char              *ident;
  ParseNode         *node;
  CORBA::ULongLong  _int;
  char             *_str;
  CORBA::WChar     *_wstr;
  CORBA::Char       _char;
  CORBA::WChar      _wchar;
  CORBA::LongDouble _float;
  CORBA::LongDouble _fixed;
}


%token T_AMPERSAND
%token T_ANY
%token T_ASTERISK
%token T_ATTRIBUTE
%token T_BOOLEAN
%token T_CASE
%token T_CHAR
%token <_char> T_CHARACTER_LITERAL
%token <_wchar> T_WCHARACTER_LITERAL
%token T_CIRCUMFLEX
%token T_COLON
%token T_COMMA
%token T_CONST
%token T_CONTEXT
%token T_DEFAULT
%token T_DOUBLE
%token T_ENUM
%token T_EQUAL
%token T_EXCEPTION
%token T_FALSE
%token T_FIXED
%token <_fixed> T_FIXED_PT_LITERAL
%token T_FLOAT
%token <_float> T_FLOATING_PT_LITERAL
%token T_GREATER_THAN_SIGN
%token <ident> T_IDENTIFIER
%token T_IN
%token T_INOUT
%token <_int> T_INTEGER_LITERAL
%token T_INTERFACE
%token T_LEFT_CURLY_BRACKET
%token T_LEFT_PARANTHESIS
%token T_LEFT_SQUARE_BRACKET
%token T_LESS_THAN_SIGN
%token T_LONG
%token T_MINUS_SIGN
%token T_MODULE
%token T_OCTET
%token T_ONEWAY
%token T_OUT
%token T_PERCENT_SIGN
%token T_PLUS_SIGN
%token T_RAISES
%token T_READONLY
%token T_RIGHT_CURLY_BRACKET
%token T_RIGHT_PARANTHESIS
%token T_RIGHT_SQUARE_BRACKET
%token T_SCOPE
%token T_SEMICOLON
%token T_SEQUENCE
%token T_SHIFTLEFT
%token T_SHIFTRIGHT
%token T_SHORT
%token T_SOLIDUS
%token T_STRING
%token <_str> T_STRING_LITERAL
%token <_wstr> T_WSTRING_LITERAL
%token <_str> T_PRAGMA
%token <_str> T_INCLUDE
%token T_STRUCT
%token T_SWITCH
%token T_TILDE
%token T_TRUE
%token T_OBJECT
%token T_TYPEDEF
%token T_UNION
%token T_UNSIGNED
%token T_VERTICAL_LINE
%token T_VOID
%token T_WCHAR
%token T_WSTRING
%token T_UNKNOWN
%token T_ABSTRACT
%token T_VALUETYPE
%token T_TRUNCATABLE
%token T_SUPPORTS
%token T_CUSTOM
%token T_PUBLIC
%token T_PRIVATE
%token T_FACTORY
%token T_NATIVE
%token T_VALUEBASE
%token T_IMPORT
%token T_TYPEID
%token T_TYPEPREFIX
%token T_GETRAISES
%token T_SETRAISES
%token T_LOCAL
%token T_COMPONENT
%token T_PROVIDES
%token T_USES
%token T_MULTIPLE
%token T_EMITS
%token T_PUBLISHES
%token T_CONSUMES
%token T_HOME
%token T_MANAGES
%token T_PRIMARYKEY
%token T_FINDER
%token T_EVENTTYPE


%type <node>   definitions
%type <node>   definition
%type <node>   interface
%type <node>   interface_dcl
%type <node>   interface_header
%type <node>   interface_body
%type <node>   interface_names
%type <node>   exports
%type <node>   export
%type <node>   op_dcl
%type <node>   op_attribute
%type <node>   op_type_spec
%type <node>   parameter_dcls
%type <node>   except_dcl
%type <node>   raises_expr
%type <node>   context_expr
%type <node>   param_attribute
%type <node>   param_type_spec
%type <node>   floating_pt_type
%type <node>   fixed_pt_type
%type <node>   fixed_pt_const_type
%type <node>   integer_type
%type <node>   signed_int
%type <node>   signed_long_int
%type <node>   signed_longlong_int
%type <node>   signed_short_int
%type <node>   unsigned_int
%type <node>   unsigned_long_int
%type <node>   unsigned_longlong_int
%type <node>   unsigned_short_int
%type <node>   char_type
%type <node>   wide_char_type
%type <node>   boolean_type
%type <node>   octet_type
%type <node>   base_type_spec
%type <node>   param_dcl
%type <node>   simple_declarator
%type <node>   complex_declarator
%type <node>   array_declarator
%type <node>   fixed_array_sizes
%type <node>   fixed_array_size
%type <node>   param_dcls
%type <node>   const_exp
%type <node>   const_type
%type <node>   const_dcl
%type <node>   or_expr
%type <node>   xor_expr
%type <node>   and_expr
%type <node>   shift_expr
%type <node>   add_expr
%type <node>   mult_expr
%type <node>   unary_expr
%type <node>   primary_expr
%type <node>   literal
%type <node>   positive_int_const
%type <node>   string_type
%type <node>   wide_string_type
%type <node>   type_dcl
%type <node>   struct_type
%type <node>   member_list
%type <node>   member
%type <node>   members
%type <node>   type_spec
%type <node>   simple_type_spec
%type <node>   constr_type_spec
%type <node>   declarators
%type <node>   declarator
%type <node>   scoped_name
%type <node>   template_type_spec
%type <node>   enumerator
%type <node>   enumerators
%type <node>   enum_type
%type <node>   module
%type <node>   sequence_type
%type <node>   simple_declarators
%type <node>   attr_dcl
%type <node>   any_type
%type <node>   object_type
%type <node>   scoped_names
%type <node>   interface_inheritance_spec
%type <node>   forward_dcl
%type <node>   union_type
%type <node>   switch_type_spec
%type <node>   switch_body
%type <node>   case
%type <node>   case_label
%type <node>   element_spec
%type <node>   string_literals
%type <_str>   T_string_literal
%type <_wstr>  T_wstring_literal
%type <node>   value
%type <node>   value_dcl
%type <node>   value_abs_dcl
%type <node>   value_box_dcl
%type <node>   value_forward_dcl
%type <node>   value_header
%type <node>   value_inheritance_spec
%type <node>   value_inheritance_bases
%type <node>   value_names
%type <node>   value_name
%type <node>   value_body
%type <node>   value_elements
%type <node>   value_element
%type <node>   state_member
%type <node>   init_dcl
%type <node>   init_param_decls
%type <node>   init_param_decl
%type <node>   init_param_attribute
%type <node>   value_base_type
%type <node>   type_id_dcl
%type <node>   type_prefix_dcl
%type <node>   import
%type <node>   imported_scope
%type <node>   readonly_attr_spec
%type <node>   readonly_attr_declarator
%type <node>   attr_spec
%type <node>   attr_declarator
%type <node>   attr_raises_expr
%type <node>   get_excep_expr
%type <node>   set_excep_expr
%type <node>   exception_list
%type <node>   constr_forward_decl
%type <node>   component
%type <node>   component_forward_dcl
%type <node>   component_dcl
%type <node>   component_header
%type <node>   supported_interface_spec
%type <node>   component_inheritance_spec
%type <node>   component_body
%type <node>   component_export
%type <node>   provides_dcl
%type <node>   interface_type
%type <node>   uses_dcl
%type <node>   emits_dcl
%type <node>   publishes_dcl
%type <node>   consumes_dcl
%type <node>   home_dcl
%type <node>   home_header
%type <node>   home_inheritance_spec
%type <node>   primary_key_spec
%type <node>   home_body
%type <node>   home_exports
%type <node>   home_export
%type <node>   factory_dcl
%type <node>   finder_dcl
%type <node>   event
%type <node>   event_forward_dcl
%type <node>   event_abs_dcl
%type <node>   event_dcl
%type <node>   event_header

%%

/*1*/
specification
	: /*empty*/
	  {
	    theParser->setRootNode( NULL );
	  }
	| definitions
          {
	    theParser->setRootNode( $1 );
	  }
	;


definitions
	: definition
          {
	    $$ = new ParseNode( definitions, $1 );
	  }
	| definition definitions
          {
	    $$ = new ParseNode( definitions, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*2*/
definition
	: type_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| const_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| except_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| interface T_SEMICOLON
          {
	    $$ = $1;
	  }
	| module T_SEMICOLON
          {
	    $$ = $1;
	  }
	| value T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| type_id_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| type_prefix_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| event T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| component T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| home_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| import T_SEMICOLON
	  {
	    $$ = $1;
	  }
        | T_PRAGMA      /*New*/
          {
	    $$ = new ParseNode( t_pragma );
	    $$->setIdent( $1 );
	  }
        | T_INCLUDE      /*New*/
          {
	    $$ = new ParseNode( t_include );
	    $$->setIdent( $1 );
	  }
	;

/*3*/
module
	: T_MODULE T_IDENTIFIER T_LEFT_CURLY_BRACKET
                   definitions T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( t_module, $4 );
	    $$->setIdent( $2 );
	  }
	;

/*4*/
interface
	: interface_dcl
          {
	    $$ = $1;
	  }
	| forward_dcl
          {
	    $$ = $1;
	  }
	;

/*5*/
interface_dcl
	: interface_header T_LEFT_CURLY_BRACKET interface_body
                                         T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( interface_dcl, $1, $3 );
	  }
	;

/*6*/
forward_dcl
	: T_INTERFACE T_IDENTIFIER
          {
	    $$ = new ParseNode( forward_dcl );
	    $$->setIdent( $2 );
	  }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER
	  {
	    $$ = new ParseNode( abstract_forward_dcl );
	    $$->setIdent( $3 );
	  }
	| T_LOCAL T_INTERFACE T_IDENTIFIER
	  {
	    $$ = new ParseNode( local_forward_dcl );
	    $$->setIdent( $3 );
	  }
	;

/*7*/
interface_header
	: T_INTERFACE T_IDENTIFIER
          {
	    $$ = new ParseNode( interface_header );
	    $$->setIdent( $2 );
	  }
	| T_INTERFACE T_IDENTIFIER interface_inheritance_spec
          {
	    $$ = new ParseNode( interface_header );
	    $$->setIdent( $2 );
	    $$->setBranch( 1, $3 );
	  }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER
	  {
	    $$ = new ParseNode( abstract_interface_header );
	    $$->setIdent( $3 );
	  }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER interface_inheritance_spec
          {
	    $$ = new ParseNode( abstract_interface_header );
	    $$->setIdent( $3 );
	    $$->setBranch( 1, $4 );
	  }
	| T_LOCAL T_INTERFACE T_IDENTIFIER
	  {
	    $$ = new ParseNode( local_interface_header );
	    $$->setIdent( $3 );
	  }
	| T_LOCAL T_INTERFACE T_IDENTIFIER interface_inheritance_spec
          {
	    $$ = new ParseNode( local_interface_header );
	    $$->setIdent( $3 );
	    $$->setBranch( 1, $4 );
	  }
	; 

/*8*/
interface_body
	: /*empty*/
          {
	    $$ = NULL;
	  }
	| exports
          {
	    $$ = $1;
	  }
	;

exports
	: export
          {
	    $$ = new ParseNode( exports, $1 );
	  }
	| export exports
          {
	    $$ = new ParseNode( exports, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*9*/
export
	: type_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| const_dcl T_SEMICOLON 
          {
	    $$ = $1;
	  }
	| except_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| attr_dcl T_SEMICOLON
          {
	    $$ = $1;
	  }
	| op_dcl T_SEMICOLON 
          {
	    $$ = $1;
	  }
	| type_id_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| type_prefix_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
        | T_PRAGMA      /*New*/
          {
	    $$ = new ParseNode( t_pragma );
	    $$->setIdent( $1 );
	  }
        | T_INCLUDE      /*New*/
          {
	    $$ = new ParseNode( t_include );
	    $$->setIdent( $1 );
	  }
	;

/*10*/
interface_inheritance_spec
	: T_COLON interface_names
          {
	    $$ = $2;
	  }
	;

interface_names
	: scoped_names
	;

scoped_names
	: scoped_name
          {
	    $$ = new ParseNode( scoped_names, $1 );
	  }
	| scoped_name T_COMMA scoped_names
          {
	    $$ = new ParseNode( scoped_names, $1 );
	    $$->setBranch( 2, $3 );
	  }
	;

/*12*/
scoped_name
	: T_IDENTIFIER
          {
	    $$ = new ParseNode( scoped_name );
	    $$->setIdent( $1 );
	  }
        | T_SCOPE T_IDENTIFIER
          {
	    $$ = new ParseNode( scoped_name );
	    string name = "::";
	    name += $2;
	    CORBA::string_free( $2 );
	    $$->setIdent( CORBA::string_dup( name.c_str() ) );
	  }
	| scoped_name T_SCOPE T_IDENTIFIER
          {
	    $$ = $1;
	    string name = $$->getIdent();
	    name += "::";
	    name += $3;
	    CORBA::string_free( $3 );
	    $$->setIdent( CORBA::string_dup( name.c_str() ) );
	  }
	;

/*13*/
value
	: value_dcl
	| value_abs_dcl
	| value_box_dcl
	| value_forward_dcl
	;

/*14*/
value_forward_dcl
	: T_VALUETYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode( value_forward_dcl );
	    $$->setIdent( $2 );
	  }
	| T_ABSTRACT T_VALUETYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode( abstract_value_forward_dcl );
	    $$->setIdent( $3 );
	  }
	;

/*15*/
value_box_dcl
	: T_VALUETYPE T_IDENTIFIER type_spec
	  {
	    $$ = new ParseNode( value_box_dcl, $3 );
	    $$->setIdent( $2 );
	  }
	;

/*16*/
value_abs_dcl
	: T_ABSTRACT T_VALUETYPE T_IDENTIFIER
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode( abstract_value_dcl );
	    $$->setBranch( 1, new ParseNode( value_header ) );
	    $$->getBranch( 1 )->setIdent( $3 );
	    $$->setBranch( 2, $5 );
	  }
	| T_ABSTRACT T_VALUETYPE T_IDENTIFIER value_inheritance_spec
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode( abstract_value_dcl );
	    $$->setBranch( 1, new ParseNode( value_header ) );
	    $$->getBranch( 1 )->setIdent( $3 );
	    $$->getBranch( 1 )->setBranch( 1, $4 );
	    $$->setBranch( 2, $6 );
	  }
	;

value_body
	: /*empty*/
	  {
	    $$ = NULL;
	  }
	| exports
	  {
	    $$ = $1;
	  }
	;

/*17*/
value_dcl
	: value_header T_LEFT_CURLY_BRACKET value_elements
		T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode( value_dcl, $1, $3 );
	  }
	| value_header T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode( value_dcl, $1 );
	  }
	;

value_elements
	: value_element
	  {
	    $$ = new ParseNode( value_elements, $1 );
	  }
	| value_element value_elements
	  {
	    $$ = new ParseNode( value_elements, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*18*/
value_header
	: T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	  {
	    $$ = new ParseNode( value_header, $3 );
	    $$->setIdent( $2 );
	  }
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	  {
	    $$ = new ParseNode( custom_value_header, $4 );
	    $$->setIdent( $3 );
	  }
	| T_VALUETYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode( value_header );
	    $$->setIdent( $2 );
	  }
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode( custom_value_header );
	    $$->setIdent( $3 );
	  }
	;

/*19*/
value_inheritance_spec
	: T_COLON value_inheritance_bases
	  {
	    $$ = $2;
	  }
	| T_COLON value_inheritance_bases T_SUPPORTS interface_names
	  {
	    $$ = $2;
	    $$->setBranch( 4, $4 );
	  }
	| T_SUPPORTS interface_names
	  {
	    $$ = new ParseNode( value_inheritance_spec );
	    $$->setBranch( 4, $2 );
	  }
	;

value_inheritance_bases
	: value_name
	  {
	    $$ = new ParseNode( value_inheritance_spec );
	    $$->setBranch( 1, $1 );
	  }
	| value_name T_COMMA value_names
	  {
	    $$ = new ParseNode( value_inheritance_spec );
	    $$->setBranch( 1, $1 );
	    $$->setBranch( 3, $3 );
	  }
	| T_TRUNCATABLE value_name
	  {
	    $$ = new ParseNode( value_inheritance_spec );
	    $$->setBranch( 2, $2 );
	  }
	| T_TRUNCATABLE value_name T_COMMA value_names
	  {
	    $$ = new ParseNode( value_inheritance_spec );
	    $$->setBranch( 2, $2 );
	    $$->setBranch( 3, $4 );
	  }
	;

value_names
	: scoped_names
	;

/*20*/
value_name
	: scoped_name
	;

/*21*/
value_element
	: export
	| state_member
	| init_dcl
	;

/*22*/
state_member
	: T_PUBLIC type_spec declarators T_SEMICOLON
	  {
	    $$ = new ParseNode( public_state_member, $2, $3 );
	  }
	| T_PRIVATE type_spec declarators T_SEMICOLON
	  {
	    $$ = new ParseNode( private_state_member, $2, $3 );
	  }
	;

/*23*/
init_dcl
	: T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
		T_SEMICOLON
	  {
	    $$ = new ParseNode( init_dcl, $4 );
	    $$->setIdent( $2 );
	  }
	| T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
		T_SEMICOLON
	  {
	    $$ = new ParseNode( init_dcl );
	    $$->setIdent( $2 );
	  }
	;

/*24*/
init_param_decls
	: init_param_decl
	  {
	    $$ = new ParseNode( init_param_decls, $1 );
	  }
	| init_param_decl T_COMMA init_param_decls
	  {
	    $$ = new ParseNode( init_param_decls, $1 );
	    $$->setBranch( 2, $3 );
	  }
	;

/*25*/
init_param_decl
	: init_param_attribute param_type_spec simple_declarator
	  {
	    $$ = new ParseNode( init_param_decl, $2, $3 );
	  }
	;

/*26*/
init_param_attribute
	: T_IN
	  {
	    $$ = NULL;
	  }
	;

/*27*/
const_dcl
	: T_CONST const_type T_IDENTIFIER T_EQUAL const_exp
          {
	    $$ = new ParseNode( const_dcl, $2, $5 );
	    $$->setIdent( $3 );
	  }
	;

/*28*/
const_type
	: integer_type
          {
	    $$ = $1;
	  }
	| char_type
          {
	    $$ = $1;
	  }
	| wide_char_type
          {
	    $$ = $1;
	  }
	| boolean_type
          {
	    $$ = $1;
	  }
	| floating_pt_type
          {
	    $$ = $1;
	  }
	| string_type
          {
	    $$ = $1;
	  }
	| wide_string_type
          {
	    $$ = $1;
	  }
	| fixed_pt_const_type
          {
	    $$ = $1;
	  }
	| scoped_name 
          {
	    $$ = $1;
	  }
	| octet_type
	  {
	    $$ = $1;
	  }
	;

/*29*/
const_exp
	: or_expr
          {
	    $$ = $1;
	  }
	;

/*30*/
or_expr
	: xor_expr
          {
	    $$ = $1;
	  }
	| or_expr T_VERTICAL_LINE xor_expr
          {
	    $$ = new ParseNode( t_vertical_line, $1, $3 );
	  }
	;

/*31*/
xor_expr
	: and_expr
          {
	    $$ = $1;
	  }
	| xor_expr T_CIRCUMFLEX and_expr
          {
	    $$ = new ParseNode( t_circumflex, $1, $3 );
	  }
	;

/*32*/
and_expr
	: shift_expr
          {
	    $$ = $1;
	  }
	| and_expr T_AMPERSAND shift_expr
          {
	    $$ = new ParseNode( t_ampersand, $1, $3 );
	  }
	;

/*33*/
shift_expr
	: add_expr
          {
	    $$ = $1;
	  }
	| shift_expr T_SHIFTRIGHT add_expr
          {
	    $$ = new ParseNode( t_shiftright, $1, $3 );
	  }
	| shift_expr T_SHIFTLEFT add_expr
          {
	    $$ = new ParseNode( t_shiftleft, $1, $3 );
	  }
	;

/*34*/
add_expr
	: mult_expr
          {
	    $$ = $1;
	  }
	| add_expr T_PLUS_SIGN mult_expr
          {
	    $$ = new ParseNode( t_plus_sign, $1, $3 );
	  }
	| add_expr T_MINUS_SIGN mult_expr
          {
	    $$ = new ParseNode( t_minus_sign, $1, $3 );
	  }
	;

/*35*/
mult_expr
	: unary_expr
          {
	    $$ = $1;
	  }
	| mult_expr T_ASTERISK unary_expr
          {
	    $$ = new ParseNode( t_asterik, $1, $3 );
	  }
	| mult_expr T_SOLIDUS unary_expr
          {
	    $$ = new ParseNode( t_solidus, $1, $3 );
	  }
	| mult_expr T_PERCENT_SIGN unary_expr
          {
	    $$ = new ParseNode( t_percent_sign, $1, $3 );
	  }
	;

/*36*/
/*37*/
unary_expr
	: T_MINUS_SIGN primary_expr
          {
	    $$ = new ParseNode( t_minus_sign, $2 );
	  }
	| T_PLUS_SIGN primary_expr
          {
	    $$ = new ParseNode( t_plus_sign, $2 );
	  }
	| T_TILDE primary_expr
          {
	    $$ = new ParseNode( t_tilde, $2 );
	  }
	| primary_expr
          {
	    $$ = $1;
	  }
	;

/*38*/
primary_expr
	: scoped_name
          {
	    $$ = $1;
	  }
	| literal
          {
	    $$ = $1;
	  }
	| T_LEFT_PARANTHESIS const_exp T_RIGHT_PARANTHESIS
          {
	    $$ = $2;
	  }
	;

/*39*/
/*40*/
literal
	: T_INTEGER_LITERAL
          {
	    $$ = new ParseNode( t_integer_literal );
	    $$->setIntegerLiteral( $1 );
	  }
	| T_string_literal
          {
	    $$ = new ParseNode( t_string_literal );
	    $$->setStringLiteral( $1 );
	  }
	| T_wstring_literal
          {
	    $$ = new ParseNode( t_wstring_literal );
	    $$->setWStringLiteral( $1 );
	  }
	| T_CHARACTER_LITERAL
          {
	    $$ = new ParseNode( t_character_literal );
	    $$->setCharacterLiteral( $1 );
	  }
	| T_WCHARACTER_LITERAL
          {
	    $$ = new ParseNode( t_wcharacter_literal );
	    $$->setWCharacterLiteral( $1 );
	  }
	| T_FIXED_PT_LITERAL
          {
	    $$ = new ParseNode( t_fixed_pt_literal );
	    $$->setFixedPtLiteral( $1 );
	  }
	| T_FLOATING_PT_LITERAL
          {
	    $$ = new ParseNode( t_floating_pt_literal );
	    $$->setFloatingPtLiteral( $1 );
	  }
	| T_TRUE  /*boolean_literal*/
          {
	    $$ = new ParseNode( t_boolean_literal );
	    $$->setBooleanLiteral( true );
	  }
	| T_FALSE /*boolean_literal*/
          {
	    $$ = new ParseNode( t_boolean_literal );
	    $$->setBooleanLiteral( false );
	  }
	;

/*41*/
positive_int_const
	: const_exp
          {
	    $$ = $1;
	  }
	;

/*42*/
/*43*/
type_dcl
	: T_TYPEDEF type_spec declarators
          {
	    $$ = new ParseNode( t_typedef, $2, $3 );
	  }
	| struct_type
          {
	    $$ = $1;
	  }
	| union_type
          {
	    $$ = $1;
	  }
	| enum_type
          {
	    $$ = $1;
	  }
	| T_NATIVE simple_declarator
	  {
	    $$ = new ParseNode( t_native, $2 );
	  }
	| constr_forward_decl
	;

/*44*/
type_spec
	: simple_type_spec
          {
	    $$ = $1;
	  }
	| constr_type_spec 
          {
	    $$ = $1;
	  }
	;

/*45*/
simple_type_spec
	: base_type_spec
          {
	    $$ = $1;
	  }
	| template_type_spec
          {
	    $$ = $1;
	  }
	| scoped_name
          {
	    $$ = $1;
	  }
	;

/*46*/
base_type_spec
	: floating_pt_type
          {
	    $$ = $1;
	  }
	| integer_type
          {
	    $$ = $1;
	  }
	| char_type
          {
	    $$ = $1;
	  }
	| wide_char_type
          {
	    $$ = $1;
	  }
	| boolean_type
          {
	    $$ = $1;
	  }
	| octet_type
          {
	    $$ = $1;
	  }
	| any_type
          {
	    $$ = $1;
	  }
	| object_type
          {
	    $$ = $1;
	  }
	| value_base_type
	  {
	    $$ = $1;
	  }
	;

/*47*/
template_type_spec
	: sequence_type
          {
	    $$ = $1;
	  }
	| string_type
          {
	    $$ = $1;
	  }
	| wide_string_type
          {
	    $$ = $1;
	  }
	| fixed_pt_type
          {
	    $$ = $1;
	  }
	;

/*48*/
constr_type_spec
	: struct_type
          {
	    $$ = $1;
	  }
	| union_type
          {
	    $$ = $1;
	  }
	| enum_type
          {
	    $$ = $1;
	  }
	;

/*49*/
declarators
	: declarator
          {
	    $$ = new ParseNode( declarators, $1 );
	  }
	| declarator T_COMMA declarators
          {
	    $$ = new ParseNode( declarators, $1 );
	    $$->setBranch( 2, $3 );
	  }
	;

/*50*/
declarator
	: simple_declarator
          {
	    $$ = $1;
	  }
	| complex_declarator
          {
	    $$ = $1;
	  }
	;

/*51*/
simple_declarator
	: T_IDENTIFIER
          {
	    $$ = new ParseNode( simple_declarator );
	    $$->setIdent( $1 );
	  }
	;

/*52*/
complex_declarator
	: array_declarator
          {
	    $$ = $1;
	  }
	;

/*53*/
floating_pt_type
	: T_FLOAT
          {
	    $$ = new ParseNode( t_float );
	  }
	| T_DOUBLE
          {
	    $$ = new ParseNode( t_double );
	  }
	| T_LONG T_DOUBLE
          {
	    $$ = new ParseNode( t_long_double );
	  }
	;

/*54*/
integer_type
	: signed_int
          {
	    $$ = $1;
	  }
	| unsigned_int
          {
	    $$ = $1;
	  }
	;

/*55*/
signed_int
	: signed_long_int
          {
	    $$ = $1;
	  }
	| signed_short_int
          {
	    $$ = $1;
	  }
	| signed_longlong_int
          {
	    $$ = $1;
	  }
	;

/*56*/
signed_short_int
	: T_SHORT
          {
	    $$ = new ParseNode( t_short );
	  }
	;

/*57*/
signed_long_int
	: T_LONG
          {
	    $$ = new ParseNode( t_long );
	  }
	;

/*58*/
signed_longlong_int
	: T_LONG T_LONG
          {
	    $$ = new ParseNode( t_longlong );
	  }
	;

/*59*/
unsigned_int
	: unsigned_long_int
          {
	    $$ = $1;
	  }
	| unsigned_short_int
          {
	    $$ = $1;
	  }
	| unsigned_longlong_int
          {
	    $$ = $1;
	  }
	;

/*60*/
unsigned_short_int
	: T_UNSIGNED T_SHORT
          {
	    $$ = new ParseNode( t_unsigned_short );
	  }
	;

/*61*/
unsigned_long_int
	: T_UNSIGNED T_LONG
          {
	    $$ = new ParseNode( t_unsigned_long );
	  }
	;

/*62*/
unsigned_longlong_int
	: T_UNSIGNED T_LONG T_LONG
          {
	    $$ = new ParseNode( t_unsigned_longlong );
	  }
	;

/*63*/
char_type
	: T_CHAR
          {
	    $$ = new ParseNode( t_char );
	  }
	;

/*64*/
wide_char_type
	: T_WCHAR
          {
	    $$ = new ParseNode( t_wide_char );
	  }
	;

/*65*/
boolean_type
	: T_BOOLEAN
          {
	    $$ = new ParseNode( t_boolean );
	  }
	;

/*66*/
octet_type
	: T_OCTET
          {
	    $$ = new ParseNode( t_octet );
	  }
	;

/*67*/
any_type
	: T_ANY
          {
	    $$ = new ParseNode( t_any );
	  }
	;

/*68*/
object_type
	: T_OBJECT
          {
	    $$ = new ParseNode( t_object );
	  }
	;

/*69*/
struct_type
	: T_STRUCT T_IDENTIFIER T_LEFT_CURLY_BRACKET member_list 
                                               T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( struct_type, $4 );
	    $$->setIdent( $2 );
	  }
	;

/*70*/
member_list
	: member
          {
	    $$ = new ParseNode( member_list, $1 );
	  }
	| member member_list
          {
	    $$ = new ParseNode( member_list, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*71*/
member
	: type_spec declarators T_SEMICOLON
          {
	    $$ = new ParseNode( member, $1, $2 );
	  }
        | T_PRAGMA      /*New*/
          {
	    $$ = new ParseNode( t_pragma );
	    $$->setIdent( $1 );
	  }
        | T_INCLUDE      /*New*/
          {
	    $$ = new ParseNode( t_include );
	    $$->setIdent( $1 );
	  }
	;

/*72*/
union_type
	: T_UNION T_IDENTIFIER T_SWITCH T_LEFT_PARANTHESIS
          switch_type_spec T_RIGHT_PARANTHESIS T_LEFT_CURLY_BRACKET
          switch_body T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( union_type, $5, $8 );
	    $$->setIdent( $2 );
	  }
	; 

/*73*/
switch_type_spec
	: integer_type
          {
	    $$ = $1;
	  }
	| char_type
          {
	    $$ = $1;
	  }
	| boolean_type
          {
	    $$ = $1;
	  }
	| enum_type
          {
	    $$ = $1;
	  }
	| scoped_name
          {
	    $$ = $1;
	  }
	;

/*74*/
switch_body
	: case
          {
	    $$ = new ParseNode( switch_body, $1 );
	  }
	| case switch_body
          {
	    $$ = new ParseNode( switch_body, $1, $2 );
	  }
	;

/*75*/
case	
	: case_label case
          {
	    $$ = new ParseNode( _case, $1, $2 );
	  }
	| case_label element_spec T_SEMICOLON
          {
	    $$ = new ParseNode( _case, $1 );
	    $$->setBranch( 3, $2 );
	  }
	| case_label T_PRAGMA element_spec T_SEMICOLON   /* New */
          {
	    $$ = new ParseNode( _case, $1 );
	    $$->setBranch( 3, $3 );
	    ParseNode * pnode = new ParseNode (t_pragma);
	    pnode->setIdent ($2);
	    $$->setBranch( 4, pnode);
	  }
	;

/*76*/
case_label
	: T_CASE const_exp T_COLON 
          {
	    $$ = $2;
	  }
	| T_DEFAULT T_COLON
          {
	    $$ = new ParseNode( t_default );
	  }
	;

/*77*/
element_spec
	: type_spec declarator
          {
	    $$ = new ParseNode( element_spec, $1, $2 );
	  }
	;

/*78*/
enum_type
	: T_ENUM T_IDENTIFIER T_LEFT_CURLY_BRACKET enumerators
                                         T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( enum_type, $4 );
	    $$->setIdent( $2 );
	  }
	;

enumerators
	: enumerator
          {
	    $$ = $1;
	  }
	| enumerator T_COMMA enumerators
          {
	    $$ = new ParseNode( enumerators, $1, $3 );
	  }
	| T_PRAGMA enumerators
	  {
	    ParseNode *pnode = new ParseNode( t_pragma );
	    pnode->setIdent( $1 );
	    $$ = new ParseNode( enumerators, pnode, $2 );
	  }
	;

/*79*/
enumerator
	: T_IDENTIFIER
          {
	    $$ = new ParseNode( enumerator );
	    $$->setIdent( $1 );
	  }
	;

/*80*/
sequence_type
	: T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_COMMA
                        positive_int_const T_GREATER_THAN_SIGN
          {
	    $$ = new ParseNode( t_sequence, $3, $5 );
	  }
	| T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_GREATER_THAN_SIGN
          {
	    $$ = new ParseNode( t_sequence, $3 );
	  }
	;

/*81*/
string_type
	: T_STRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN
          {
	    $$ = new ParseNode( t_string, $3 );
	  }
	| T_STRING
          {
	    $$ = new ParseNode( t_string );
	  }
	;

/*82*/
wide_string_type
	: T_WSTRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN
          {
	    $$ = new ParseNode( t_wide_string, $3 );
	  }
	| T_WSTRING
          {
	    $$ = new ParseNode( t_wide_string );
	  }
	;

/*83*/
array_declarator
	: T_IDENTIFIER fixed_array_sizes
          {
	    $$ = new ParseNode( array_declarator, $2 );
	    $$->setIdent( $1 );
	  }
	;

fixed_array_sizes
	: fixed_array_size
          {
	    $$ = new ParseNode( fixed_array_sizes, $1 );
	  }
	| fixed_array_size fixed_array_sizes
          {
	    $$ = new ParseNode( fixed_array_sizes, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*84*/
fixed_array_size
	: T_LEFT_SQUARE_BRACKET positive_int_const T_RIGHT_SQUARE_BRACKET
          {
	    $$ = $2;
	  }
	;

/*85*/
attr_dcl
	: readonly_attr_spec
	| attr_spec
	;

/*86*/
except_dcl
	: T_EXCEPTION T_IDENTIFIER T_LEFT_CURLY_BRACKET members
                                          T_RIGHT_CURLY_BRACKET
          {
	    $$ = new ParseNode( except_dcl, $4 );
	    $$->setIdent( $2 );
	  }
	;

members
	: /*empty*/
          {
	    $$ = NULL;
	  }
	| member members
          {
	    $$ = new ParseNode( member_list, $1 );
	    $$->setBranch( 2, $2 );
	  }
	;

/*87*/
op_dcl
	: op_attribute op_type_spec T_IDENTIFIER parameter_dcls
                                       raises_expr context_expr
          {
	    $$ = new ParseNode( op_dcl, $1, $2, $4, $5, $6 );
	    $$->setIdent( $3 );
	  }
	| op_attribute op_type_spec T_IDENTIFIER parameter_dcls
                                       context_expr
          {
	    $$ = new ParseNode( op_dcl, $1, $2, $4, 0, $5 );
	    $$->setIdent( $3 );
	  }
	| op_attribute op_type_spec T_IDENTIFIER parameter_dcls
                                       raises_expr
          {
	    $$ = new ParseNode( op_dcl, $1, $2, $4, $5 );
	    $$->setIdent( $3 );
	  }
	| op_attribute op_type_spec T_IDENTIFIER parameter_dcls
          {
	    $$ = new ParseNode( op_dcl, $1, $2, $4 );
	    $$->setIdent( $3 );
	  }
	;

/*88*/
op_attribute
	: /*empty*/
          {
	    $$ = NULL;
	  }
	| T_ONEWAY
          {
	    $$ = new ParseNode( t_oneway );
	  }
	;

/*89*/
op_type_spec	
	: param_type_spec
          {
              $$ = new ParseNode( op_type_spec, $1 );
	  }
	| T_VOID
          {
              $$ = new ParseNode( t_void );
	  }
	;

/*90*/
parameter_dcls
	: T_LEFT_PARANTHESIS param_dcls T_RIGHT_PARANTHESIS
          {
	    $$ = $2;
	  }
	| T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
          {
	    $$ = NULL;
	  }
	;

param_dcls
	: param_dcl
          {
	    $$ = new ParseNode( param_dcls, $1 );
	  }
	| param_dcl T_COMMA param_dcls
          {
	    $$ = new ParseNode( param_dcls, $1 );
	    $$->setBranch( 2, $3 );
	  }
	;

/*91*/
param_dcl
	: param_attribute param_type_spec simple_declarator
          {
	    $$ = new ParseNode( param_dcl, $1, $2, $3 );
	  }
	;

/*92*/
param_attribute
	: T_IN
          {
	    $$ = new ParseNode( t_in );
	  }
	| T_OUT
          {
	    $$ = new ParseNode( t_out );
	  }
	| T_INOUT
          {
	    $$ = new ParseNode( t_inout );
	  }
	;

/*93*/
raises_expr
	: T_RAISES T_LEFT_PARANTHESIS scoped_names T_RIGHT_PARANTHESIS
          {
	    $$ = new ParseNode( raises_expr, $3 );
	  }
	;

/*94*/
context_expr
	: T_CONTEXT T_LEFT_PARANTHESIS string_literals T_RIGHT_PARANTHESIS
          {
	    $$ = $3;
	  }
	;

string_literals
	: T_string_literal
          {
	    $$ = new ParseNode( string_literals );
	    $$->setIdent( $1 );
	  }
	| T_string_literal T_COMMA string_literals
          {
	    $$ = new ParseNode( string_literals, $3 );
	    $$->setIdent( $1 );
	  }
	;

T_string_literal
	: T_STRING_LITERAL
	  {
	    $$ = $1;
	  }
	| T_STRING_LITERAL T_string_literal
	  {
	    string s1 = string($1);
	    string s2 = string($2);
	    CORBA::string_free( $1 );
	    CORBA::string_free( $2 );
	    s1 += s2;
	    $$ = CORBA::string_dup( s1.c_str() );
	  }
	;

T_wstring_literal
	: T_WSTRING_LITERAL
	  {
	    $$ = $1;
	  }
	| T_WSTRING_LITERAL T_wstring_literal
	  {
	    CORBA::WChar *w1, *res;
	    CORBA::ULong cnt;

	    for (cnt=0, w1=$1; *w1; w1++, cnt++);
	    for (w1=$2; *w1; w1++, cnt++);

	    res = CORBA::wstring_alloc (cnt);

	    for (cnt=0, w1=$1; *w1; w1++, cnt++) {
	      res[cnt] = *w1;
	    }

	    for (w1=$2; *w1; w1++, cnt++) {
	      res[cnt] = *w1;
	    }

	    res[cnt] = 0;

	    CORBA::wstring_free( $1 );
	    CORBA::wstring_free( $2 );
	    $$ = res;
	  }
	;

/*95*/
param_type_spec
	: base_type_spec
          {
	    $$ = $1;
	  }
	| template_type_spec
          {
	    $$ = $1;
	  }
	| scoped_name
          {
	    $$ = $1;
	  }
	;

/*96*/
fixed_pt_type
	: T_FIXED T_LESS_THAN_SIGN positive_int_const T_COMMA
              T_INTEGER_LITERAL T_GREATER_THAN_SIGN
          {
	    $$ = new ParseNode( fixed_pt_type, $3 );
	    $$->setIntegerLiteral( $5 );
	  }
	;

/*97*/
fixed_pt_const_type
	: T_FIXED
          {
	    $$ = new ParseNode( fixed_pt_type );
	  }
	;

/*98*/
value_base_type
	: T_VALUEBASE
	  {
	    $$ = new ParseNode( t_valuebase );
	  }
	;

/*99 A (from 2.4, not in CCM) */
constr_forward_decl
	: T_STRUCT T_IDENTIFIER
	  {
	    $$ = new ParseNode (struct_forward_decl);
	    $$->setIdent ($2);
	  }
	| T_UNION T_IDENTIFIER
	  {
	    $$ = new ParseNode (union_forward_decl);
	    $$->setIdent ($2);
	  }
	;

/*99*/
/*100*/
import
	: T_IMPORT imported_scope
	  {
	    $$ = new ParseNode (t_import, $2);
	  }
	;

/*100*/
imported_scope
	: scoped_name
	| T_string_literal
	  {
	    $$ = new ParseNode (string_literals);
	    $$->setIdent ($1);
	  }
	;

/*101*/
type_id_dcl
	: T_TYPEID scoped_name T_string_literal
	  {
	    $$ = new ParseNode (t_typeid, $2);
	    $$->setIdent ($3);
	  }
	;

/*102*/
type_prefix_dcl
	: T_TYPEPREFIX scoped_name T_string_literal
	  {
	    $$ = new ParseNode (t_typeprefix, $2);
	    $$->setIdent ($3);
	  }
	;

/*103*/
readonly_attr_spec
	: T_READONLY T_ATTRIBUTE param_type_spec readonly_attr_declarator
	  {
	    $$ = new ParseNode (t_readonly_attribute, $3, $4);
	  }
	;

/*104*/
readonly_attr_declarator
	: simple_declarator raises_expr
	  {
	    $$ = new ParseNode (simple_declarators, $1);
	    $$->setBranch (3, new ParseNode (t_attr_raises, $2));
	  }
	| simple_declarators
	;

simple_declarators
	: simple_declarator
          {
	    $$ = new ParseNode( simple_declarators, $1 );
	  }
	| simple_declarator T_COMMA simple_declarators
          {
	    $$ = new ParseNode( simple_declarators, $1 );
	    $$->setBranch( 2, $3 );
	  }
	;

/*105*/
attr_spec
	: T_ATTRIBUTE param_type_spec attr_declarator
	  {
	    $$ = new ParseNode (t_attribute, $2, $3);
	  }
	;

/*106*/
attr_declarator
	: simple_declarator attr_raises_expr
	  {
	    $$ = new ParseNode (simple_declarators, $1);
	    $$->setBranch (3, $2);
	  }
	| simple_declarators
	;

/*107*/
attr_raises_expr
	: get_excep_expr
	  {
	    $$ = new ParseNode (t_attr_raises, $1);
	  }
	| set_excep_expr
	  {
	    $$ = new ParseNode (t_attr_raises, 0, $1);
	  }
	| get_excep_expr set_excep_expr
	  {
	    $$ = new ParseNode (t_attr_raises, $1, $2);
	  }
	| set_excep_expr get_excep_expr
	  {
	    $$ = new ParseNode (t_attr_raises, $2, $1);
	  }
	;

/*108*/
get_excep_expr
	: T_GETRAISES exception_list
	  {
	    $$ = $2;
	  }
	;

/*109*/
set_excep_expr
	: T_SETRAISES exception_list
	  {
	    $$ = $2;
	  }
	;

/*110*/
exception_list
	: T_LEFT_PARANTHESIS scoped_names T_RIGHT_PARANTHESIS
	  {
	    $$ = new ParseNode (raises_expr, $2);
	  }
	;

/*111*/
component
	: component_dcl
	| component_forward_dcl
	;

/*112*/
component_forward_dcl
	: T_COMPONENT T_IDENTIFIER
	  {
	    $$ = new ParseNode (component_forward_dcl);
	    $$->setIdent ($2);
	  }
	;

/*113*/
component_dcl
	: component_header
		T_LEFT_CURLY_BRACKET component_body T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (component_dcl, $1, $3);
	  }
	| component_header T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (component_dcl, $1);
	  }
	;

/*114*/
component_header
	: T_COMPONENT T_IDENTIFIER
	  {
	    $$ = new ParseNode (component_header);
	    $$->setIdent ($2);
	  }
	| T_COMPONENT T_IDENTIFIER component_inheritance_spec
	  {
	    $$ = new ParseNode (component_header, $3);
	    $$->setIdent ($2);
	  }
	| T_COMPONENT T_IDENTIFIER supported_interface_spec
	  {
	    $$ = new ParseNode (component_header);
	    $$->setIdent ($2);
	    $$->setBranch (2, $3);
	  }
	| T_COMPONENT T_IDENTIFIER
		component_inheritance_spec supported_interface_spec
	  {
	    $$ = new ParseNode (component_header, $3, $4);
	    $$->setIdent ($2);
	  }
	;

/*115*/
supported_interface_spec
	: T_SUPPORTS interface_names
	  {
	    $$ = $2;
	  }
	;

/*116*/
component_inheritance_spec
	: T_COLON scoped_name
	  {
	    $$ = $2;
	  }
	;

/*117*/
component_body
	: component_export
	  {
	    $$ = new ParseNode (exports, $1);
	  }
	| component_export component_body
	  {
	    $$ = new ParseNode (exports, $1, $2);
	  }
	;

/*118*/
component_export
	: provides_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| uses_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| emits_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| publishes_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| consumes_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| attr_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	;

/*119*/
provides_dcl
	: T_PROVIDES interface_type T_IDENTIFIER
	  {
	    $$ = new ParseNode (provides_dcl, $2);
	    $$->setIdent ($3);
	  }
	;

/*120*/
interface_type
	: scoped_name
	| T_OBJECT
	  {
	    $$ = new ParseNode (t_object);
	  }
	;

/*121*/
uses_dcl
	: T_USES interface_type T_IDENTIFIER
	  {
	    $$ = new ParseNode (uses_dcl, $2);
	    $$->setIdent ($3);
	  }
	| T_USES T_MULTIPLE interface_type T_IDENTIFIER
	  {
	    $$ = new ParseNode (uses_multiple_dcl, $3);
	    $$->setIdent ($4);
	  }
	;

/*122*/
emits_dcl
	: T_EMITS scoped_name T_IDENTIFIER
	  {
	    $$ = new ParseNode (emits_dcl, $2);
	    $$->setIdent ($3);
	  }
	;

/*123*/
publishes_dcl
	: T_PUBLISHES scoped_name T_IDENTIFIER
	  {
	    $$ = new ParseNode (publishes_dcl, $2);
	    $$->setIdent ($3);
	  }
	;

/*124*/
consumes_dcl
	: T_CONSUMES scoped_name T_IDENTIFIER
	  {
	    $$ = new ParseNode (consumes_dcl, $2);
	    $$->setIdent ($3);
	  }
	;

/*125*/
home_dcl
	: home_header home_body
	  {
	    $$ = new ParseNode (home_dcl, $1, $2);
	  }
	;

/*126*/
home_header
	: T_HOME T_IDENTIFIER T_MANAGES scoped_name
	  {
	    $$ = new ParseNode (home_header);
	    $$->setBranch (2, $4);
	    $$->setIdent ($2);
	  }
	| T_HOME T_IDENTIFIER home_inheritance_spec T_MANAGES scoped_name
	  {
	    $$ = new ParseNode (home_header, $3, $5);
	    $$->setIdent ($2);
	  }
	| T_HOME T_IDENTIFIER T_MANAGES scoped_name primary_key_spec
	  {
	    $$ = new ParseNode (home_header);
	    $$->setIdent ($2);
	    $$->setBranch (2, $4);
	    $$->setBranch (3, $5);
	  }
	| T_HOME T_IDENTIFIER home_inheritance_spec
		T_MANAGES scoped_name primary_key_spec
	  {
	    $$ = new ParseNode (home_header, $3, $5, $6);
	    $$->setIdent ($2);
	  }
	| T_HOME T_IDENTIFIER supported_interface_spec T_MANAGES scoped_name
	  {
	    $$ = new ParseNode (home_header);
	    $$->setBranch (2, $5);
	    $$->setBranch (4, $3);
	    $$->setIdent ($2);
	  }
	| T_HOME T_IDENTIFIER home_inheritance_spec
		supported_interface_spec T_MANAGES scoped_name
	  {
	    $$ = new ParseNode (home_header, $3, $6);
	    $$->setBranch (4, $4);
	    $$->setIdent ($2);
	  }
	| T_HOME T_IDENTIFIER supported_interface_spec
		T_MANAGES scoped_name primary_key_spec
	  {
	    $$ = new ParseNode (home_header);
	    $$->setIdent ($2);
	    $$->setBranch (2, $5);
	    $$->setBranch (3, $6);
	    $$->setBranch (4, $3);
	  }
	| T_HOME T_IDENTIFIER home_inheritance_spec
		supported_interface_spec T_MANAGES scoped_name
		primary_key_spec
	  {
	    $$ = new ParseNode (home_header, $3, $6, $7, $4);
	    $$->setIdent ($2);
	  }
	;

/*127*/
home_inheritance_spec
	: T_COLON scoped_name
	  {
	    $$ = $2;
	  }
	;

/*128*/
primary_key_spec
	: T_PRIMARYKEY scoped_name
	  {
	    $$ = $2;
	  }
	;

/*129*/
home_body
	: T_LEFT_CURLY_BRACKET home_exports T_RIGHT_CURLY_BRACKET
	  {
	    $$ = $2;
	  }
	| T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	  {
	    $$ = NULL;
	  }
	;

/*130*/
home_exports
	: home_export
	  {
	    $$ = new ParseNode (exports, $1);
	  }
	| home_export home_exports
	  {
	    $$ = new ParseNode (exports, $1, $2);
	  }
	;

home_export
	: export
	| factory_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	| finder_dcl T_SEMICOLON
	  {
	    $$ = $1;
	  }
	;

/*131*/
factory_dcl
	: T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
		raises_expr
	  {
	    $$ = new ParseNode (factory_dcl, $4, $6);
	    $$->setIdent ($2);
	  }
	| T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
		raises_expr
	  {
	    $$ = new ParseNode (factory_dcl);
	    $$->setBranch (2, $5);
	    $$->setIdent ($2);
	  }
	| T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
	  {
	    $$ = new ParseNode (factory_dcl, $4);
	    $$->setIdent ($2);
	  }
	| T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
	  {
	    $$ = new ParseNode (factory_dcl);
	    $$->setIdent ($2);
	  }
	;

/*132*/
finder_dcl
	: T_FINDER T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
		raises_expr
	  {
	    $$ = new ParseNode (finder_dcl, $4, $6);
	    $$->setIdent ($2);
	  }
	| T_FINDER T_IDENTIFIER
		T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
		raises_expr
	  {
	    $$ = new ParseNode (finder_dcl);
	    $$->setBranch (2, $5);
	    $$->setIdent ($2);
	  }
	| T_FINDER T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
	  {
	    $$ = new ParseNode (finder_dcl, $4);
	    $$->setIdent ($2);
	  }
	| T_FINDER T_IDENTIFIER
		T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
	  {
	    $$ = new ParseNode (finder_dcl);
	    $$->setIdent ($2);
	  }
	;

/*134*/
event
	: event_dcl
	| event_abs_dcl
	| event_forward_dcl
	;

/*135*/
event_forward_dcl
	: T_EVENTTYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode (event_forward_dcl);
	    $$->setIdent ($2);
	  }
	| T_ABSTRACT T_EVENTTYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode (abstract_event_forward_dcl);
	    $$->setIdent ($3);
	  }
	;

/*136*/
event_abs_dcl
	: T_ABSTRACT T_EVENTTYPE T_IDENTIFIER
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (abstract_event_dcl);
	    $$->setBranch (1, new ParseNode (value_header));
	    $$->getBranch (1)->setIdent ($3);
	    $$->setBranch (2, $5);
	  }
	| T_ABSTRACT T_EVENTTYPE T_IDENTIFIER value_inheritance_spec
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (abstract_event_dcl);
	    $$->setBranch (1, new ParseNode (value_header));
	    $$->getBranch (1)->setIdent ($3);
	    $$->getBranch (1)->setBranch (1, $4);
	    $$->setBranch (2, $6);
	  }
	;

/*137*/
event_dcl
	: event_header T_LEFT_CURLY_BRACKET value_elements
		T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (event_dcl, $1, $3);
	  }
	| event_header T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	  {
	    $$ = new ParseNode (event_dcl, $1);
	  }
	;

/*138*/
event_header
	: T_EVENTTYPE T_IDENTIFIER value_inheritance_spec
	  {
	    $$ = new ParseNode (value_header, $3);
	    $$->setIdent ($2);
	  }
	| T_CUSTOM T_EVENTTYPE T_IDENTIFIER value_inheritance_spec
	  {
	    $$ = new ParseNode (custom_value_header, $4);
	    $$->setIdent ($3);
	  }
	| T_EVENTTYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode (value_header);
	    $$->setIdent ($2);
	  }
	| T_CUSTOM T_EVENTTYPE T_IDENTIFIER
	  {
	    $$ = new ParseNode (custom_value_header);
	    $$->setIdent ($3);
	  }
	;

%%
