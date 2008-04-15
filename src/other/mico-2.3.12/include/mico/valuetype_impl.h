// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __mico_valuetype_impl_h__
#define __mico_valuetype_impl_h__

namespace CORBA {

class StringValue_Factory : virtual public ValueFactoryBase
{
public:
  ValueBase * create_for_unmarshal ();
};

class WStringValue_Factory : virtual public ValueFactoryBase
{
public:
  ValueBase * create_for_unmarshal ();
};

class DataOutputStream_impl :
  virtual public DataOutputStream,
  virtual public DefaultValueRefCountBase
{
public:
  DataOutputStream_impl (DataEncoder &);
  ~DataOutputStream_impl ();

  void write_any( const CORBA::Any& value );
  void write_boolean( CORBA::Boolean value );
  void write_char( CORBA::Char value );
  void write_wchar( CORBA::WChar value );
  void write_octet( CORBA::Octet value );
  void write_short( CORBA::Short value );
  void write_ushort( CORBA::UShort value );
  void write_long( CORBA::Long value );
  void write_ulong( CORBA::ULong value );
  void write_longlong( CORBA::LongLong value );
  void write_ulonglong( CORBA::ULongLong value );
  void write_float( CORBA::Float value );
  void write_double( CORBA::Double value );
  void write_longdouble( CORBA::LongDouble value );
  void write_string( const char* value );
  void write_wstring( const CORBA::WChar* value );
  void write_Object( CORBA::Object_ptr value );
  void write_Abstract( CORBA::AbstractBase* value );
  void write_Value( CORBA::ValueBase* value );
  void write_TypeCode( CORBA::TypeCode_ptr value );
  void write_any_array( const AnySeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_boolean_array( const BooleanSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_char_array( const CharSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_wchar_array( const WCharSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_octet_array( const OctetSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_short_array( const ShortSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_ushort_array( const UShortSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_long_array( const LongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_ulong_array( const ULongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_longlong_array( const LongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_ulonglong_array( const ULongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_float_array( const FloatSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void write_double_array( const DoubleSeq& seq, CORBA::ULong offset, CORBA::ULong length );

private:
  DataEncoder *ec;
};

class DataInputStream_impl :
  virtual public DataInputStream,
  virtual public DefaultValueRefCountBase
{
public:
  DataInputStream_impl (DataDecoder &);
  ~DataInputStream_impl ();

  CORBA::Boolean is_okay () const;

  CORBA::Any* read_any();
  CORBA::Boolean read_boolean();
  CORBA::Char read_char();
  CORBA::WChar read_wchar();
  CORBA::Octet read_octet();
  CORBA::Short read_short();
  CORBA::UShort read_ushort();
  CORBA::Long read_long();
  CORBA::ULong read_ulong();
  CORBA::LongLong read_longlong();
  CORBA::ULongLong read_ulonglong();
  CORBA::Float read_float();
  CORBA::Double read_double();
  CORBA::LongDouble read_longdouble();
  char* read_string();
  CORBA::WChar* read_wstring();
  CORBA::Object_ptr read_Object();
  CORBA::AbstractBase* read_Abstract();
  CORBA::ValueBase* read_Value();
  CORBA::TypeCode_ptr read_TypeCode();
  void read_any_array( AnySeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_boolean_array( BooleanSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_char_array( CharSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_wchar_array( WCharSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_octet_array( OctetSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_short_array( ShortSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_ushort_array( UShortSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_long_array( LongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_ulong_array( ULongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_longlong_array( LongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_ulonglong_array( ULongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_float_array( FloatSeq& seq, CORBA::ULong offset, CORBA::ULong length );
  void read_double_array( DoubleSeq& seq, CORBA::ULong offset, CORBA::ULong length );

private:
  Boolean okay;
  DataDecoder *dc;
};

}

#endif

