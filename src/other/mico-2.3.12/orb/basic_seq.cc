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

#include <CORBA-SMALL.h>
#include <mico/template_impl.h>

#endif // FAST_PCH

void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Any,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_any, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Any,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Any,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_any, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Any,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_any, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Char,MICO_TID_CHAR> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_char, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Char,MICO_TID_CHAR> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Char,MICO_TID_CHAR> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_char, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Char,MICO_TID_CHAR> *&s )
{
  return a.to_static_any (CORBA::_stcseq_char, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_boolean, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_boolean, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> *&s )
{
  return a.to_static_any (CORBA::_stcseq_boolean, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_octet, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_octet, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> *&s )
{
  return a.to_static_any (CORBA::_stcseq_octet, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::UShort,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_ushort, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::UShort,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::UShort,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_ushort, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::UShort,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_ushort, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::ULong,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_ulong, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::ULong,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::ULong,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_ulong, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::ULong,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_ulong, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Short,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_short, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Short,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Short,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_short, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Short,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_short, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Long,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_long, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Long,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Long,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_long, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Long,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_long, (void *&)s);
}


void
operator<<=( CORBA::Any &_a, const SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_longlong, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_longlong, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_longlong, (void *&)_s);
}


void
operator<<=( CORBA::Any &_a, const SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_ulonglong, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}


CORBA::Boolean
operator>>=( const CORBA::Any &_a, SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_ulonglong, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_ulonglong, (void *&)_s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Float,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_float, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Float,MICO_TID_DEF> *s )
{
  a <<= *s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Float,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_float, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Float,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_float, (void *&)s);
}


void
operator<<=( CORBA::Any &a, const SequenceTmpl<CORBA::Double,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_double, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, SequenceTmpl<CORBA::Double,MICO_TID_DEF> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, SequenceTmpl<CORBA::Double,MICO_TID_DEF> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_double, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const SequenceTmpl<CORBA::Double,MICO_TID_DEF> *&s )
{
  return a.to_static_any (CORBA::_stcseq_double, (void *&)s);
}


void
operator<<=( CORBA::Any &_a, const SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_longdouble, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}


CORBA::Boolean
operator>>=( const CORBA::Any &_a, SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_longdouble, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_longdouble, (void *&)_s);
}


void
operator<<=( CORBA::Any &_a, const SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_wchar, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_wchar, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_wchar, (void *&)_s);
}


void
operator<<=( CORBA::Any &_a, const WStringSequenceTmpl<CORBA::WString_var> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_wstring, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, WStringSequenceTmpl<CORBA::WString_var> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, WStringSequenceTmpl<CORBA::WString_var> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_wstring, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const WStringSequenceTmpl<CORBA::WString_var> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_wstring, (void *&)_s);
}


void
operator<<=( CORBA::Any &a, const StringSequenceTmpl<CORBA::String_var> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_string, &s);
  a.from_static_any (sa);
}

void
operator<<=( CORBA::Any &a, StringSequenceTmpl<CORBA::String_var> *s )
{
  a <<= *s;
  delete s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &a, StringSequenceTmpl<CORBA::String_var> &s )
{
  CORBA::StaticAny sa (CORBA::_stcseq_string, &s);
  return a.to_static_any (sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &a,
	     const StringSequenceTmpl<CORBA::String_var> *&s )
{
  return a.to_static_any (CORBA::_stcseq_string, (void *&)s);
}


void
operator<<=( CORBA::Any &_a,
	     const IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_Object, &_s);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a,
	     IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> &_s )
{
  CORBA::StaticAny _sa (CORBA::_stcseq_Object, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a,
	     const IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> *&_s )
{
  return _a.to_static_any (CORBA::_stcseq_Object, (void *&)_s);
}
