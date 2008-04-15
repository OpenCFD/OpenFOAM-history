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

#include "const.h"
#include "error.h"

#endif // FAST_PCH


using namespace std;

//-- Const --------------------------------------------------------------

Const::Const()
{
  _node = NULL;
}
  
Const::Const( CORBA::TypeCode_ptr target_type, CORBA::ULongLong l,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= l;
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, char *s, ParseNode* node )
{
  CORBA::Any a;
  a <<= CORBA::Any::from_string( s, 0 );
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, CORBA::WChar *s,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= CORBA::Any::from_wstring( s, 0 );
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_char c,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= c;
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_wchar c,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= c;
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, CORBA::LongDouble ld,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= ld;
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( CORBA::TypeCode_ptr target_type, CORBA::Any::from_boolean b,
	      ParseNode* node )
{
  CORBA::Any a;
  a <<= b;
  _value = do_coercion( target_type, a );
  _node = node;
}

Const::Const( const Const& c )
{
  copy_const( c );
}

Const::Const( CORBA::Any *a, ParseNode* node )
{
  _value = *a;
  delete a;
  _node = node;
}

void Const::copy_const( const Const &c )
{
  _value = c._value;
  _node = c._node;
}

Const &Const::operator=( const Const& c )
{
  copy_const( c );
  return *this;
}

/*
 * Get the content of the Const-Object and deposit into an Any.
 * The parameter "target_type" denotes the desired type. We therefore
 * need to do some type- and range-checking.
 *    const unsigned short c1 = 10;
 *          --------------      --
 *                |             |_ type of the Const Object (long long)
 *                |_______________ desired target type
 */
CORBA::Any Const::do_coercion( CORBA::TypeCode_ptr target_type,
			       CORBA::Any& val )
{
  CORBA::TypeCode_var tc = val.type();
  CORBA::Any coerced_val (target_type, 0);
  if (!val.coerce (coerced_val))
    coercion_error (tc, target_type);
  return coerced_val;
}

bool Const::is_floating_pt()
{
  CORBA::TypeCode_var tc = _value.type();
  return tc->kind() == CORBA::tk_float || tc->kind() == CORBA::tk_double ||
    tc->kind() == CORBA::tk_longdouble || tc->kind() == CORBA::tk_fixed;
}

bool Const::is_signed()
{
  CORBA::TypeCode_var tc = _value.type();
  return tc->kind() == CORBA::tk_long || tc->kind() == CORBA::tk_short ||
    tc->kind() == CORBA::tk_longlong;
}

CORBA::Long Const::get_int()
{
  CORBA::Any val;
  val = get_value( CORBA::_tc_long );
  CORBA::Long l;
  CORBA::Boolean r = ( val >>= l );
  assert (r);
  return l;
}

CORBA::Any Const::get_value( CORBA::TypeCode_ptr target_type )
{
  return do_coercion( target_type, _value );
}

CORBA::Any Const::get_any()
{
  return _value;
}

ParseNode* Const::get_parse_node()
{
  return _node;
}

void Const::coercion_error( CORBA::TypeCode_ptr from, CORBA::TypeCode_ptr to )
{
  char to_buf [30];
  char from_buf [30];
  
  static const char *kind[] = {
    "null",
    "void",
    "short",
    "long",
    "unsigned short",
    "unsigned long",
    "float",
    "double",
    "boolean",
    "char",
    "octet",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "string<%d>",
    "",
    "",
    "",
    "",
    "long long",
    "unsigned long long",
    "long double",
    "wchar",
    "wstring<%d>",
    "fixed"
  };

  const char* to_str   = kind [to->kind()];
  const char* from_str = kind [from->kind()];
  
  if (from->kind() == CORBA::tk_string) {
    sprintf (from_buf, "string<%d>", (unsigned int)from->length());
    from_str = from_buf;
  }
  
  if (from->kind() == CORBA::tk_wstring) {
    sprintf (from_buf, "wstring<%d>", (unsigned int)from->length());
    from_str = from_buf;
  }
  
  if (to->kind() == CORBA::tk_string) {
    sprintf (to_buf, "string<%d>", (unsigned int)to->length());
    to_str = to_buf;
  }
  
  if (to->kind() == CORBA::tk_wstring) {
    sprintf (to_buf, "wstring<%d>", (unsigned int)to->length());
    to_str = to_buf;
  }
  
  if( _node != NULL )
    cerr << IDLError( 10, _node, from_str, to_str ) << endl;
  else
    cerr << IDLError( 10, from_str, to_str ) << endl;
  exit( 1 );
}

Const &Const::operator|( Const &c )
{
  CORBA::Any       a1, a2;
  CORBA::ULongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 | c2;
  _value <<= r;
  return *this;
}

Const &Const::operator^( Const &c )
{
  CORBA::Any       a1, a2;
  CORBA::ULongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 ^ c2;
  _value <<= r;
  return *this;
}

Const &Const::operator&( Const &c )
{
  CORBA::Any       a1, a2;
  CORBA::ULongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 & c2;
  _value <<= r;
  return *this;
}

Const &Const::operator>>( Const &c )
{
  CORBA::Any       a1, a2;
  CORBA::ULongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 >> c2;
  _value <<= r;
  return *this;
}

Const &Const::operator<<( Const &c )
{
  CORBA::Any       a1, a2;
  CORBA::ULongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 << c2;
  _value <<= r;
  return *this;
}

Const &Const::operator+( Const &c )
{
  CORBA::Any a1, a2;

  if( is_floating_pt() || c.is_floating_pt() ) {
    // Coerce operands to long double
    CORBA::LongDouble ld1, ld2, r;
    a1 = do_coercion( CORBA::_tc_longdouble, _value );
    a2 = do_coercion( CORBA::_tc_longdouble, c._value );
    CORBA::Boolean ret = ( a1 >>= ld1 );
    assert (ret);
    ret = ( a2 >>= ld2 );
    assert (ret);
    r = ld1 + ld2;
    _value <<= r;
  } else {
    if( is_signed() || c.is_signed() ) {
      // Coerce operands to signed long long
      CORBA::LongLong ll1, ll2, r;
      a1 = do_coercion( CORBA::_tc_longlong, _value );
      a2 = do_coercion( CORBA::_tc_longlong, c._value );
      CORBA::Boolean ret = ( a1 >>= ll1 );
      assert (ret);
      ret = ( a2 >>= ll2 );
      assert (ret);
      r = ll1 + ll2;
      _value <<= r;
    } else {
      // Coerce operands to unsigned long long
      CORBA::ULongLong ull1, ull2, r;
      a1 = do_coercion( CORBA::_tc_ulonglong, _value );
      a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
      CORBA::Boolean ret = ( a1 >>= ull1 );
      assert (ret);
      ret = ( a2 >>= ull2 );
      assert (ret);
      r = ull1 + ull2;
      _value <<= r;
    }
  }
  return *this;
}

Const &Const::operator-()
{
  CORBA::Any a1;

  if( is_floating_pt() ) {
    // Coerce operands to long double
    CORBA::LongDouble ld1, r;
    a1 = do_coercion( CORBA::_tc_longdouble, _value );
    CORBA::Boolean ret = ( a1 >>= ld1 );
    assert (ret);
    r = - ld1;
    _value <<= r;
  } else {
    if( is_signed() ) {
      // Coerce operands to signed long long
      CORBA::LongLong ll1, r;
      a1 = do_coercion( CORBA::_tc_longlong, _value );
      CORBA::Boolean ret = ( a1 >>= ll1 );
      assert (ret);
      r = - ll1;
      _value <<= r;
    } else {
      // Coerce operands to unsigned long long, result becomes a long long
      CORBA::ULongLong ull1;
      CORBA::LongLong r;
      a1 = do_coercion( CORBA::_tc_ulonglong, _value );
      CORBA::Boolean ret = ( a1 >>= ull1 );
      assert (ret);
      r = - ull1;
      _value <<= r;
    }
  }
  return *this;
}

Const &Const::operator-( Const &c )
{
  CORBA::Any a1, a2;

  if( is_floating_pt() || c.is_floating_pt() ) {
    // Coerce operands to long double
    CORBA::LongDouble ld1, ld2, r;
    a1 = do_coercion( CORBA::_tc_longdouble, _value );
    a2 = do_coercion( CORBA::_tc_longdouble, c._value );
    CORBA::Boolean ret = ( a1 >>= ld1 );
    assert (ret);
    ret = ( a2 >>= ld2 );
    assert (ret);
    r = ld1 - ld2;
    _value <<= r;
  } else {
    if( is_signed() || c.is_signed() ) {
      // Coerce operands to signed long long
      CORBA::LongLong ll1, ll2, r;
      a1 = do_coercion( CORBA::_tc_longlong, _value );
      a2 = do_coercion( CORBA::_tc_longlong, c._value );
      CORBA::Boolean ret = ( a1 >>= ll1 );
      assert (ret);
      ret = ( a2 >>= ll2 );
      assert (ret);
      r = ll1 - ll2;
      _value <<= r;
    } else {
      // Coerce operands to unsigned long long
      CORBA::ULongLong ull1, ull2, r;
      a1 = do_coercion( CORBA::_tc_ulonglong, _value );
      a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
      CORBA::Boolean ret = ( a1 >>= ull1 );
      assert (ret);
      ret = ( a2 >>= ull2 );
      assert (ret);
      r = ull1 - ull2;
      _value <<= r;
    }
  }
  return *this;
}

Const &Const::operator*( Const &c )
{
  CORBA::Any a1, a2;

  if( is_floating_pt() || c.is_floating_pt() ) {
    // Coerce operands to long double
    CORBA::LongDouble ld1, ld2, r;
    a1 = do_coercion( CORBA::_tc_longdouble, _value );
    a2 = do_coercion( CORBA::_tc_longdouble, c._value );
    CORBA::Boolean ret = ( a1 >>= ld1 );
    assert (ret);
    ret = ( a2 >>= ld2 );
    assert (ret);
    r = ld1 * ld2;
    _value <<= r;
  } else {
    if( is_signed() || c.is_signed() ) {
      // Coerce operands to signed long long
      CORBA::LongLong ll1, ll2, r;
      a1 = do_coercion( CORBA::_tc_longlong, _value );
      a2 = do_coercion( CORBA::_tc_longlong, c._value );
      CORBA::Boolean ret = ( a1 >>= ll1 );
      assert (ret);
      ret = ( a2 >>= ll2 );
      assert (ret);
      r = ll1 * ll2;
      _value <<= r;
    } else {
      // Coerce operands to unsigned long long
      CORBA::ULongLong ull1, ull2, r;
      a1 = do_coercion( CORBA::_tc_ulonglong, _value );
      a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
      CORBA::Boolean ret = ( a1 >>= ull1 );
      assert (ret);
      ret = ( a2 >>= ull2 );
      assert (ret);
      r = ull1 * ull2;
      _value <<= r;
    }
  }
  return *this;
}

Const &Const::operator/( Const &c )
{
  CORBA::Any a1, a2;

  if( is_floating_pt() || c.is_floating_pt() ) {
    // Coerce operands to long double
    CORBA::LongDouble ld1, ld2, r;
    a1 = do_coercion( CORBA::_tc_longdouble, _value );
    a2 = do_coercion( CORBA::_tc_longdouble, c._value );
    CORBA::Boolean ret = ( a1 >>= ld1 );
    assert (ret);
    ret = ( a2 >>= ld2 );
    assert (ret);
    assert( ld2 != 0.0 );
    r = ld1 / ld2;
    _value <<= r;
  } else {
    if( is_signed() || c.is_signed() ) {
      // Coerce operands to signed long long
      CORBA::LongLong ll1, ll2, r;
      a1 = do_coercion( CORBA::_tc_longlong, _value );
      a2 = do_coercion( CORBA::_tc_longlong, c._value );
      CORBA::Boolean ret = ( a1 >>= ll1 );
      assert (ret);
      ret = ( a2 >>= ll2 );
      assert (ret);
      assert( ll2 != 0 );
      r = ll1 / ll2;
      _value <<= r;
    } else {
      // Coerce operands to unsigned long long
      CORBA::ULongLong ull1, ull2, r;
      a1 = do_coercion( CORBA::_tc_ulonglong, _value );
      a2 = do_coercion( CORBA::_tc_ulonglong, c._value );
      CORBA::Boolean ret = ( a1 >>= ull1 );
      assert (ret);
      ret = ( a2 >>= ull2 );
      assert (ret);
      assert( ull2 != 0 );
      r = ull1 / ull2;
      _value <<= r;
    }
  }
  return *this;
}

Const &Const::operator%( Const &c )
{
  CORBA::Any      a1, a2;
  CORBA::LongLong c1, c2, r;
  
  a1 = do_coercion( CORBA::_tc_longlong, _value );
  a2 = do_coercion( CORBA::_tc_longlong, c._value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  ret = ( a2 >>= c2 );
  assert (ret);
  r = c1 % c2;
  _value <<= r;
  return *this;
}

Const &Const::operator~()
{
  CORBA::Any       a1;
  CORBA::ULongLong c1, r;
  
  a1 = do_coercion( CORBA::_tc_ulonglong, _value );
  CORBA::Boolean ret = ( a1 >>= c1 );
  assert (ret);
  r = ~c1;
  _value <<= r;
  return *this;
}
