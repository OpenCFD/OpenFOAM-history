/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2003 by The Mico Team
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
#include "parsenode.h"
#include "prepro.h"
#include "error.h"
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

typedef struct 
{
  char *str;
  int   data;     // 0 = no further information
                  // 1 = ident
} ParseNodeDoc;


static ParseNodeDoc parseNodeDoc[] = {
  { "definitions", 0 },
  { "exports", 0 },
  { "interface_dcl", 0 },
  { "interface_header", 1 },
  { "abstract_interface_header", 1 },
  { "abstract_forward_dcl", 1 },
  { "op_dcl", 1 },
  { "t_void", 0 },
  { "t_in", 0 },
  { "t_out", 0 },
  { "t_inout", 0 },
  { "t_float", 0 },
  { "t_double", 0 },
  { "t_long", 0 },
  { "t_short", 0 },
  { "t_unsigned_long", 0 },
  { "t_unsigned_short", 0 },
  { "t_char", 0 },
  { "t_boolean", 0 },
  { "t_octet", 0 },
  { "param_dcl", 0 },
  { "simple_declarator", 1 },
  { "param_dcls", 0 },
  { "op_type_spec", 0 },
  { "t_string", 0 },
  { "t_vertical_line", 0 },
  { "t_circumflex", 0 },
  { "t_ampersand", 0 },
  { "t_shiftright", 0 },
  { "t_shiftleft", 0 },
  { "t_plus_sign", 0 },
  { "t_minus_sign", 0 },
  { "t_asterik", 0 },
  { "t_solidus", 0 },
  { "t_percent_sign", 0 },
  { "t_tilde", 0 },
  { "t_integer_literal", 0 },
  { "t_string_literal", 0 },
  { "t_wstring_literal", 0 },
  { "t_character_literal", 0 },
  { "t_wcharacter_literal", 0 },
  { "t_floating_pt_literal", 0 },
  { "t_boolean_literal", 0 },
  { "struct_type", 1 },
  { "member_list", 0 },
  { "member", 0 },  
  { "declarators", 0 },
  { "declarator", 0 },
  { "scoped_name", 1 },
  { "t_typedef", 0 },
  { "enumerator", 1 },
  { "enumerators", 0 },
  { "enum_type", 1 },
  { "t_module", 1 },
  { "t_sequence", 0 },
  { "t_attribute", 0 },
  { "t_readonly_attribute", 0 },
  { "simple_declarators", 0 },
  { "t_any", 0 },
  { "t_object", 0 },
  { "t_principal", 0 },
  { "scoped_names", 0 },
  { "forward_dcl", 1 },
  { "except_dcl", 1 },
  { "raises_expr", 0 },
  { "array_declarator", 1 },
  { "fixed_array_sizes", 0 },
  { "t_oneway", 0 },
  { "const_dcl", 1 },
  { "union_type", 1 },
  { "switch_body", 0 },
  { "case", 0 },
  { "t_default", 0 },
  { "element_spec", 0 },
  { "enum_case_label", 1 },
  { "string_literals", 1 },
  { "t_pragma", 1 },
  { "t_include", 1 },
  { "t_long_double", 0 },
  { "t_unsigned_longlong", 0 },
  { "t_wide_char", 0 },
  { "t_wide_string", 0 },
  { "fixed_pt_type", 0 },
  { "t_fixed_pt_literal", 0 },
  { "t_longlong", 0 },
  { "value_forward_dcl", 1 },
  { "abstract_value_forward_dcl", 1 },
  { "value_box_dcl", 1 },
  { "abstract_value_dcl", 1 },
  { "value_dcl", 0 },
  { "value_header", 1 },
  { "custom_value_header", 1 },
  { "value_inheritance_spec", 0 },
  { "value_elements", 0 },
  { "value_element", 0 },
  { "public_state_member", 0 },
  { "private_state_member", 0 },
  { "init_dcl", 0 },
  { "init_param_decls", 0 },
  { "init_param_decl", 0 },
  { "t_native", 0 },
  { "t_valuebase", 0 },
  { "t_abstractbase", 0 },
  { "t_import", 0 },
  { "imports", 0 },
  { "local_forward_dcl", 1 },
  { "local_forward_header", 1 },
  { "t_typeid", 1 },
  { "t_typeprefix", 1 },
  { "t_attr_raises", 0 },
  { "struct_forward_decl", 1 },
  { "union_forward_decl", 1 },
  { "component_dcl", 0 },
  { "component_forward_dcl", 1 },
  { "component_header", 1 },
  { "provides_dcl", 1 },
  { "uses_dcl", 1 },
  { "uses_multiple_dcl", 1 },
  { "emits_dcl", 1 },
  { "publishes_dcl", 1 },
  { "consumes_dcl", 1 },
  { "home_dcl", 0 },
  { "home_header", 1 },
  { "factory_dcl", 1 },
  { "finder_dcl", 1 },
  { "event_forward_dcl", 1 },
  { "abstract_event_forward_dcl", 1 },
  { "abstract_event_dcl", 1 },
  { "event_dcl", 0 }
};


KeywordMap* ParseNode::_keymap = new KeywordMap;


ParseNode::ParseNode( ParseNodeType aType, ParseNode *one,
		      ParseNode *two, ParseNode *three,
		      ParseNode *four, ParseNode *five )
{
  type = aType;
  ident = NULL;
  b1 = one;
  b2 = two;
  b3 = three;
  b4 = four;
  b5 = five;
  _fname = idl_lexFile;
  _line_no = idl_line_no;
  _is_toplevel = strcmp( idl_lexFile, toplevelFile ) == 0;
}


ParseNode::~ParseNode()
{
  if( ident ) CORBA::string_free( ident );
  if( b1 ) delete b1;
  if( b2 ) delete b2;
  if( b3 ) delete b3;
  if( b4 ) delete b4;
  if( b5 ) delete b5;
}


void ParseNode::setIdent( char *anIdent )
{
    if (anIdent != NULL) {
	// kcg: this looks like the best place
	// for checking if identifier collides
	// with IDL keyword
	string t = static_cast<const char*>(anIdent);
	string keyword = "";
	if ((keyword = _keymap->keyword(t.c_str())) != "") {
	    cerr << IDLError(41, this, t.c_str(), keyword.c_str()) << endl;
	    exit(1);
	}
    }
    if (ident)
	CORBA::string_free(ident);
    if (anIdent != NULL) {
	// kcg: we need to getrid of leading underscore here
	// this applies also to scoped names
	string ts = anIdent;
	string tmp = "";
	if (ts[0] == '_') {
	    tmp = ts.substr(1);
	    ts = tmp;
	}
	unsigned long pos = 0;
	while((pos = ts.find("::_")) != string::npos) {
	    tmp = ts.substr(0, pos + 2) + ts.substr(pos + 3, ts.size());
	    ts = tmp;
	}
	ident = CORBA::string_dup(ts.c_str());
    }
    else {
	ident = anIdent;
    }
}


char *ParseNode::getIdent()
{
  return ident;
}

char *ParseNode::getFname()
{
  return _fname;
}

long ParseNode::getLineNo()
{
  return _line_no;
}

bool ParseNode::is_toplevel()
{
  return _is_toplevel;
}

void
ParseNode::setIntegerLiteral( CORBA::ULongLong l )
{
  _u._int = l;
}

CORBA::ULongLong
ParseNode::getIntegerLiteral()
{
  return _u._int;
}

void ParseNode::setStringLiteral( char* s )
{
  _u._str = s;
}

char* ParseNode::getStringLiteral()
{
  return _u._str;
}

void
ParseNode::setWStringLiteral( CORBA::WChar * s )
{
  _u._wstr = s;
}

CORBA::WChar*
ParseNode::getWStringLiteral()
{
  return _u._wstr;
}

void ParseNode::setCharacterLiteral( CORBA::Char c )
{
  _u._char = c;
}

CORBA::Char ParseNode::getCharacterLiteral()
{
  return _u._char;
}

void
ParseNode::setWCharacterLiteral( CORBA::WChar c )
{
  _u._wchar = c;
}

CORBA::WChar
ParseNode::getWCharacterLiteral()
{
  return _u._wchar;
}

void ParseNode::setFloatingPtLiteral( CORBA::LongDouble f )
{
  _u._float = f;
}

CORBA::LongDouble ParseNode::getFloatingPtLiteral()
{
  return _u._float;
}

void ParseNode::setFixedPtLiteral( CORBA::LongDouble f )
{
  _u._fixed = f;
}

CORBA::LongDouble ParseNode::getFixedPtLiteral()
{
  return _u._fixed;
}

void ParseNode::setBooleanLiteral( CORBA::Boolean b )
{
  _u._bool = b;
}

CORBA::Boolean ParseNode::getBooleanLiteral()
{
  return _u._bool;
}

ParseNodeType ParseNode::getType()
{
  return type;
}


ParseNode *ParseNode::getBranch( int i )
{
  switch( i ) {
  case 1:
    return b1;
  case 2:
    return b2;
  case 3:
    return b3;
  case 4:
    return b4;
  case 5:
    return b5;
  default:
    assert( 0 );
  }
  return 0;
}


void ParseNode::setBranch( int i, ParseNode *node )
{
  switch( i ) {
  case 1:
    if( b1 ) delete b1;
    b1 = node;
    break;
  case 2:
    if( b2 ) delete b2;
    b2 = node;
    break;
  case 3:
    if( b3 ) delete b3;
    b3 = node;
    break;
  case 4:
    if( b4 ) delete b4;
    b4 = node;
    break;
  case 5:
    if( b5 ) delete b5;
    b5 = node;
    break;
  default:
    assert( 0 );
  }
}


void ParseNode::printBranch( int indent, char *tag, bool detailed )
{
  for( int i = 0; i < indent; i++ )
    cout << " ";
  cout << tag;
  cout << parseNodeDoc[ (int) type ].str;
  switch( parseNodeDoc[ (int) type ].data ) {
  case 0:
    break;
  case 1:
    cout << " (" << ident << ")";
    break;
  default:
    assert( 0 );
  }
  cout << endl;
  if( b1 )
    b1->printBranch( indent + 2, "1: ", detailed );
  if( b2 )
    b2->printBranch( indent + 2, "2: ", detailed );
  if( b3 )
    b3->printBranch( indent + 2, "3: ", detailed );
  if( b4 )
    b4->printBranch( indent + 2, "4: ", detailed );
  if( b5 )
    b5->printBranch( indent + 2, "5: ", detailed );
}


void ParseNode::print( bool detailed )
{
  printBranch( 0, "", detailed );
}
