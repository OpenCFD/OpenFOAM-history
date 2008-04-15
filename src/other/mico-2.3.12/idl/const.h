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

#ifndef __IDLCONST_H__
#define __IDLCONST_H__

#include <CORBA.h>
#include "parsenode.h"
#include "db.h"


class Const 
{
private:
  CORBA::Any _value;
  ParseNode* _node;
  
  void copy_const( const Const &c );

  CORBA::Any do_coercion( CORBA::TypeCode_ptr target_type,
			  CORBA::Any& val );
  void coercion_error( CORBA::TypeCode_ptr from, CORBA::TypeCode_ptr to );
  
  bool is_floating_pt();
  bool is_signed();
  
public:
  Const();
  Const( CORBA::TypeCode_ptr target_type, CORBA::ULongLong l,
	 ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, char *s, ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, CORBA::WChar *s, ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_char c,
	 ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_wchar c,
	 ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, CORBA::LongDouble ld,
	 ParseNode* node );
  Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_boolean b,
	 ParseNode* node );
  Const( const Const& c );
  Const( CORBA::Any *a, ParseNode* node );
  Const &operator=( const Const& c );

  CORBA::Any get_value( CORBA::TypeCode_ptr target_type );
  CORBA::Any get_any();
  CORBA::Long get_int();
  ParseNode* get_parse_node();
  
  Const &operator|( Const& c );
  Const &operator^( Const& c );
  Const &operator&( Const& c );
  Const &operator>>( Const& c );
  Const &operator<<( Const& c );
  Const &operator+( Const& c );
  Const &operator-();
  Const &operator-( Const& c );
  Const &operator*( Const& c );
  Const &operator/( Const& c );
  Const &operator%( Const& c );
  Const &operator~();
};


#endif
