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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <mico/template_impl.h>

#endif // FAST_PCH

/*
 * StringValue and WStringValue factories
 */

CORBA::ValueBase *
CORBA::StringValue_Factory::create_for_unmarshal ()
{
  return new CORBA::StringValue;
}

CORBA::ValueBase *
CORBA::WStringValue_Factory::create_for_unmarshal ()
{
  return new CORBA::WStringValue;
}

/*
 * Data Output Stream Implementation
 */

CORBA::DataOutputStream_impl::DataOutputStream_impl (DataEncoder &_ec)
{
  ec = &_ec;
}

CORBA::DataOutputStream_impl::~DataOutputStream_impl ()
{
}

void
CORBA::DataOutputStream_impl::write_any (const Any & value)
{
  ec->put_any (value);
}

void
CORBA::DataOutputStream_impl::write_boolean (Boolean value)
{
  ec->put_boolean (value);
}

void
CORBA::DataOutputStream_impl::write_char (Char value)
{
  ec->put_char (value);
}

void
CORBA::DataOutputStream_impl::write_wchar (WChar value)
{
  ec->put_wchar (value);
}

void
CORBA::DataOutputStream_impl::write_octet (Octet value)
{
  ec->put_octet (value);
}

void
CORBA::DataOutputStream_impl::write_short (Short value)
{
  ec->put_short (value);
}

void
CORBA::DataOutputStream_impl::write_ushort (UShort value)
{
  ec->put_ushort (value);
}

void
CORBA::DataOutputStream_impl::write_long (Long value)
{
  ec->put_long (value);
}

void
CORBA::DataOutputStream_impl::write_ulong (ULong value)
{
  ec->put_ulong (value);
}

void
CORBA::DataOutputStream_impl::write_longlong (LongLong value)
{
  ec->put_longlong (value);
}

void
CORBA::DataOutputStream_impl::write_ulonglong (ULongLong value)
{
  ec->put_ulonglong (value);
}

void
CORBA::DataOutputStream_impl::write_float (Float value)
{
  ec->put_float (value);
}

void
CORBA::DataOutputStream_impl::write_double (Double value)
{
  ec->put_double (value);
}

void
CORBA::DataOutputStream_impl::write_longdouble (LongDouble value)
{
  ec->put_longdouble (value);
}

void
CORBA::DataOutputStream_impl::write_string (const char * value)
{
  ec->put_string (value);
}

void
CORBA::DataOutputStream_impl::write_wstring (const WChar * value)
{
  ec->put_wstring (value);
}

void
CORBA::DataOutputStream_impl::write_Object (Object_ptr value)
{
  _stc_Object->marshal (*ec, &value);
}

void
CORBA::DataOutputStream_impl::write_Abstract (AbstractBase* value)
{
  _stc_AbstractBase->marshal (*ec, &value);
}

void
CORBA::DataOutputStream_impl::write_Value (ValueBase* value)
{
  _stc_ValueBase->marshal (*ec, &value);
}

void
CORBA::DataOutputStream_impl::write_TypeCode (TypeCode_ptr value)
{
  _stc_TypeCode->marshal (*ec, &value);
}

void
CORBA::DataOutputStream_impl::write_any_array (const AnySeq& seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  for (ULong i=0; i<length; i++) {
    write_any (seq[offset+i]);
  }
}

void
CORBA::DataOutputStream_impl::write_boolean_array (const BooleanSeq& seq,
						   CORBA::ULong offset,
						   CORBA::ULong length)
{
  if (length > 0)
    ec->put_booleans (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_char_array (const CharSeq& seq,
						CORBA::ULong offset,
						CORBA::ULong length)
{
  if (length > 0)
    ec->put_chars (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_wchar_array (const WCharSeq& seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (length > 0)
    ec->put_wchars (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_octet_array (const OctetSeq& seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (length > 0)
    ec->put_octets (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_short_array (const ShortSeq& seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (length > 0)
    ec->put_shorts (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_ushort_array (const UShortSeq& seq,
						  CORBA::ULong offset,
						  CORBA::ULong length)
{
  if (length > 0)
    ec->put_ushorts (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_long_array (const LongSeq& seq,
						CORBA::ULong offset,
						CORBA::ULong length)
{
  if (length > 0)
    ec->put_longs (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_ulong_array (const ULongSeq& seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (length > 0)
    ec->put_ulongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_longlong_array (const LongLongSeq& seq,
						    CORBA::ULong offset,
						    CORBA::ULong length)
{
  if (length > 0)
    ec->put_longlongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_ulonglong_array (const ULongLongSeq& seq,
						     CORBA::ULong offset,
						     CORBA::ULong length)
{
  if (length > 0)
    ec->put_ulonglongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_float_array (const FloatSeq& seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (length > 0)
    ec->put_floats (seq.get_buffer()+offset, length);
}

void
CORBA::DataOutputStream_impl::write_double_array (const DoubleSeq& seq,
						  CORBA::ULong offset,
						  CORBA::ULong length)
{
  if (length > 0)
    ec->put_doubles (seq.get_buffer()+offset, length);
}

/*
 * Data Input Stream Implementation
 */

CORBA::DataInputStream_impl::DataInputStream_impl (DataDecoder &_dc)
{
  dc = &_dc;
  okay = TRUE;
}

CORBA::DataInputStream_impl::~DataInputStream_impl ()
{
}

CORBA::Boolean
CORBA::DataInputStream_impl::is_okay () const
{
  return okay;
}

CORBA::Any *
CORBA::DataInputStream_impl::read_any ()
{
  Any * value = new Any;
  okay &= dc->get_any (*value);
  return value;
}

CORBA::Boolean
CORBA::DataInputStream_impl::read_boolean ()
{
  Boolean value;
  okay &= dc->get_boolean (value);
  return value;
}

CORBA::Char
CORBA::DataInputStream_impl::read_char ()
{
  Char value;
  okay &= dc->get_char (value);
  return value;
}

CORBA::WChar
CORBA::DataInputStream_impl::read_wchar ()
{
  WChar value;
  okay &= dc->get_wchar (value);
  return value;
}

CORBA::Octet
CORBA::DataInputStream_impl::read_octet ()
{
  Octet value;
  okay &= dc->get_octet (value);
  return value;
}

CORBA::Short
CORBA::DataInputStream_impl::read_short ()
{
  Short value;
  okay &= dc->get_short (value);
  return value;
}

CORBA::UShort
CORBA::DataInputStream_impl::read_ushort ()
{
  UShort value;
  okay &= dc->get_ushort (value);
  return value;
}

CORBA::Long
CORBA::DataInputStream_impl::read_long ()
{
  Long value;
  okay &= dc->get_long (value);
  return value;
}

CORBA::ULong
CORBA::DataInputStream_impl::read_ulong ()
{
  ULong value;
  okay &= dc->get_ulong (value);
  return value;
}

CORBA::LongLong
CORBA::DataInputStream_impl::read_longlong ()
{
  LongLong value;
  okay &= dc->get_longlong (value);
  return value;
}

CORBA::ULongLong
CORBA::DataInputStream_impl::read_ulonglong ()
{
  ULongLong value;
  okay &= dc->get_ulonglong (value);
  return value;
}

CORBA::Float
CORBA::DataInputStream_impl::read_float ()
{
  Float value;
  okay &= dc->get_float (value);
  return value;
}

CORBA::Double
CORBA::DataInputStream_impl::read_double ()
{
  Double value;
  okay &= dc->get_double (value);
  return value;
}

CORBA::LongDouble
CORBA::DataInputStream_impl::read_longdouble ()
{
  LongDouble value;
  okay &= dc->get_longdouble (value);
  return value;
}

char *
CORBA::DataInputStream_impl::read_string ()
{
  char * value;
  okay &= dc->get_string (value);
  return value;
}

CORBA::WChar *
CORBA::DataInputStream_impl::read_wstring ()
{
  WChar * value;
  okay &= dc->get_wstring (value);
  return value;
}

CORBA::Object_ptr
CORBA::DataInputStream_impl::read_Object ()
{
  Object_ptr value;
  okay &= _stc_Object->demarshal (*dc, &value);
  return value;
}

CORBA::AbstractBase*
CORBA::DataInputStream_impl::read_Abstract ()
{
  AbstractBase* value;
  okay &= _stc_AbstractBase->demarshal (*dc, &value);
  return value;
}

CORBA::ValueBase*
CORBA::DataInputStream_impl::read_Value ()
{
  ValueBase* value;
  okay &= _stc_ValueBase->demarshal (*dc, &value);
  return value;
}

CORBA::TypeCode_ptr
CORBA::DataInputStream_impl::read_TypeCode ()
{
  TypeCode_ptr value;
  okay &= _stc_TypeCode->demarshal (*dc, &value);
  return value;
}

void
CORBA::DataInputStream_impl::read_any_array (AnySeq &seq,
					     CORBA::ULong offset,
					     CORBA::ULong length)
{
  for (ULong i=0; i<length; i++) {
    okay &= dc->get_any (seq[offset+i]);
  }
}

void
CORBA::DataInputStream_impl::read_boolean_array (BooleanSeq &seq,
						 CORBA::ULong offset,
						 CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_booleans (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_char_array (CharSeq &seq,
					      CORBA::ULong offset,
					      CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_chars (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_wchar_array (WCharSeq &seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_wchars (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_octet_array (OctetSeq &seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_octets (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_short_array (ShortSeq &seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_shorts (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_ushort_array (UShortSeq &seq,
						CORBA::ULong offset,
						CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_ushorts (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_long_array (LongSeq &seq,
					      CORBA::ULong offset,
					      CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_longs (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_ulong_array (ULongSeq &seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_ulongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_longlong_array (LongLongSeq &seq,
						  CORBA::ULong offset,
						  CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_longlongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_ulonglong_array (ULongLongSeq &seq,
						   CORBA::ULong offset,
						   CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_ulonglongs (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_float_array (FloatSeq &seq,
					       CORBA::ULong offset,
					       CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_floats (seq.get_buffer()+offset, length);
}

void
CORBA::DataInputStream_impl::read_double_array (DoubleSeq &seq,
						CORBA::ULong offset,
						CORBA::ULong length)
{
  if (seq.length() < offset+length) {
    seq.length (offset+length);
  }
  if (length > 0)
    dc->get_doubles (seq.get_buffer()+offset, length);
}

