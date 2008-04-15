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

#ifndef __PARSENODE_H__
#define __PARSENODE_H__

#ifndef assert
#include <assert.h>
#endif

#if defined(_WIN32) && !defined(__MINGW32__)
#include <mico/config-win32.h>
#else
#include <mico/config.h>
#endif

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

#include <string>
#include <keymap.h>

#define MAX_NODE_SIZE 255

typedef enum 
{
  definitions = 0,
  exports,
  interface_dcl,
  interface_header,
  abstract_interface_header,
  abstract_forward_dcl,
  op_dcl,
  t_void,
  t_in,
  t_out,
  t_inout,
  t_float,
  t_double,
  t_long,
  t_short,
  t_unsigned_long,
  t_unsigned_short,
  t_char,
  t_boolean,
  t_octet,
  param_dcl,
  simple_declarator,
  param_dcls,
  op_type_spec,
  t_string,
  t_vertical_line,
  t_circumflex,
  t_ampersand,
  t_shiftright,
  t_shiftleft,
  t_plus_sign,
  t_minus_sign,
  t_asterik,
  t_solidus,
  t_percent_sign,
  t_tilde,
  t_integer_literal,
  t_string_literal,
  t_wstring_literal,
  t_character_literal,
  t_wcharacter_literal,
  t_floating_pt_literal,
  t_boolean_literal,
  struct_type,
  member_list,
  member,
  declarators,
  declarator,
  scoped_name,
  t_typedef,
  enumerator,
  enumerators,
  enum_type,
  t_module,
  t_sequence,
  t_attribute,
  t_readonly_attribute,
  simple_declarators,
  t_any,
  t_object,
  t_principal,
  scoped_names,
  forward_dcl,
  except_dcl,
  raises_expr,
  array_declarator,
  fixed_array_sizes,
  t_oneway,
  const_dcl,
  union_type,
  switch_body,
  _case,
  t_default,
  element_spec,
  enum_case_label,
  string_literals,
  t_pragma,
  t_include,
  t_long_double,
  t_unsigned_longlong,
  t_wide_char,
  t_wide_string,
  fixed_pt_type,
  t_fixed_pt_literal,
  t_longlong,
  value_forward_dcl,
  abstract_value_forward_dcl,
  value_box_dcl,
  abstract_value_dcl,
  value_dcl,
  value_header,
  custom_value_header,
  value_inheritance_spec,
  value_elements,
  value_element,
  public_state_member,
  private_state_member,
  init_dcl,
  init_param_decls,
  init_param_decl,
  t_native,
  t_valuebase,
  t_abstractbase,
  t_import,
  imports,
  local_forward_dcl,
  local_interface_header,
  t_typeid,
  t_typeprefix,
  t_attr_raises,
  struct_forward_decl,
  union_forward_decl,
  component_dcl,
  component_forward_dcl,
  component_header,
  provides_dcl,
  uses_dcl,
  uses_multiple_dcl,
  emits_dcl,
  publishes_dcl,
  consumes_dcl,
  home_dcl,
  home_header,
  factory_dcl,
  finder_dcl,
  event_forward_dcl,
  abstract_event_forward_dcl,
  abstract_event_dcl,
  event_dcl
} ParseNodeType;


class ParseNode
{
private:
  ParseNodeType  type;
  char	        *ident;

  char*          _fname;
  long           _line_no;
  bool           _is_toplevel;
  
  union u {
    CORBA::ULongLong  _int;
    char*             _str;
    CORBA::WChar *    _wstr;
    CORBA::Char       _char;
    CORBA::WChar      _wchar;
    CORBA::Boolean    _bool;
    CORBA::LongDouble _float;
    CORBA::LongDouble _fixed;
  } _u;
  
  ParseNode *b1;
  ParseNode *b2;
  ParseNode *b3;
  ParseNode *b4;
  ParseNode *b5;

  static KeywordMap* _keymap;

  void printBranch( int indent, char *tag, bool detailed );

public:
  ParseNode( ParseNodeType aType, ParseNode *one = NULL,
	     ParseNode *two = NULL, ParseNode *three = NULL,
	     ParseNode *four = NULL, ParseNode *five = NULL );
  ~ParseNode();
  ParseNodeType getType();
  void setIdent( char *anIdent );
  char *getIdent();
  char *getFname();
  long getLineNo();
  bool is_toplevel();
  
  void setIntegerLiteral( CORBA::ULongLong l );
  CORBA::ULongLong getIntegerLiteral();
  void setStringLiteral( char *s );
  char* getStringLiteral();
  void setWStringLiteral( CORBA::WChar *s );
  CORBA::WChar* getWStringLiteral();
  void setCharacterLiteral( CORBA::Char c );
  CORBA::Char getCharacterLiteral();
  void setWCharacterLiteral( CORBA::WChar c );
  CORBA::WChar getWCharacterLiteral();
  void setFloatingPtLiteral( CORBA::LongDouble f );
  CORBA::LongDouble getFloatingPtLiteral();
  void setFixedPtLiteral( CORBA::LongDouble f );
  CORBA::LongDouble getFixedPtLiteral();
  void setBooleanLiteral( CORBA::Boolean b );
  CORBA::Boolean getBooleanLiteral();
  ParseNode *getBranch( int i );
  void setBranch( int i, ParseNode *node );
  void print( bool detailed = false );
};

#endif
