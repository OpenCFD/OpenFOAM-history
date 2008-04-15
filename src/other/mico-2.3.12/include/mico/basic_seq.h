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

#if !defined(__BASIC_SEQ_H__) || defined(MICO_NO_TOPLEVEL_MODULES)
#define __BASIC_SEQ_H__

#ifndef MICO_NO_TOPLEVEL_MODULES
namespace CORBA {
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_CORBA)

typedef SequenceTmpl<CORBA::Any,MICO_TID_DEF> AnySeq;
typedef TSeqVar<SequenceTmpl<CORBA::Any,MICO_TID_DEF> > AnySeq_var;
typedef AnySeq_var AnySeq_out;

typedef SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> BooleanSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> > BooleanSeq_var;
typedef BooleanSeq_var BooleanSeq_out;

typedef SequenceTmpl<CORBA::Char,MICO_TID_CHAR> CharSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Char,MICO_TID_CHAR> > CharSeq_var;
typedef CharSeq_var CharSeq_out;

typedef SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> WCharSeq;
typedef TSeqVar<SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> > WCharSeq_var;
typedef WCharSeq_var WCharSeq_out;

typedef SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> OctetSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> > OctetSeq_var;
typedef OctetSeq_var OctetSeq_out;

typedef SequenceTmpl<CORBA::Short,MICO_TID_DEF> ShortSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Short,MICO_TID_DEF> > ShortSeq_var;
typedef ShortSeq_var ShortSeq_out;

typedef SequenceTmpl<CORBA::UShort,MICO_TID_DEF> UShortSeq;
typedef TSeqVar<SequenceTmpl<CORBA::UShort,MICO_TID_DEF> > UShortSeq_var;
typedef UShortSeq_var UShortSeq_out;

typedef SequenceTmpl<CORBA::Long,MICO_TID_DEF> LongSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Long,MICO_TID_DEF> > LongSeq_var;
typedef LongSeq_var LongSeq_out;

typedef SequenceTmpl<CORBA::ULong,MICO_TID_DEF> ULongSeq;
typedef TSeqVar<SequenceTmpl<CORBA::ULong,MICO_TID_DEF> > ULongSeq_var;
typedef ULongSeq_var ULongSeq_out;

typedef SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> LongLongSeq;
typedef TSeqVar<SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> > LongLongSeq_var;
typedef LongLongSeq_var LongLongSeq_out;

typedef SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> ULongLongSeq;
typedef TSeqVar<SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> > ULongLongSeq_var;
typedef ULongLongSeq_var ULongLongSeq_out;

typedef SequenceTmpl<CORBA::Float,MICO_TID_DEF> FloatSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Float,MICO_TID_DEF> > FloatSeq_var;
typedef FloatSeq_var FloatSeq_out;

typedef SequenceTmpl<CORBA::Double,MICO_TID_DEF> DoubleSeq;
typedef TSeqVar<SequenceTmpl<CORBA::Double,MICO_TID_DEF> > DoubleSeq_var;
typedef DoubleSeq_var DoubleSeq_out;

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_CORBA)

#ifndef MICO_NO_TOPLEVEL_MODULES

} 
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Any,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Any,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Any,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Any,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Char,MICO_TID_CHAR> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Char,MICO_TID_CHAR> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Char,MICO_TID_CHAR> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Char,MICO_TID_CHAR> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::UShort,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::UShort,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::UShort,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::UShort,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::ULong,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::ULong,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::ULong,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::ULong,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Short,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Short,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Short,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Short,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Long,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Long,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Long,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Long,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &_a,
		  const SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a,
		  SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &_a,
		  const SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a,
		  SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Float,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Float,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Float,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Float,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &a,
		  const SequenceTmpl<CORBA::Double,MICO_TID_DEF> &s );
void operator<<=( CORBA::Any &a,
		  SequenceTmpl<CORBA::Double,MICO_TID_DEF> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    SequenceTmpl<CORBA::Double,MICO_TID_DEF> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const SequenceTmpl<CORBA::Double,MICO_TID_DEF> *&s );

void operator<<=( CORBA::Any &_a,
		  const SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a,
		  SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &_a,
		  const SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> &_s );
void operator<<=( CORBA::Any &_a,
		  SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> *&_s );

void operator<<=( CORBA::Any &_a,
		  const WStringSequenceTmpl<CORBA::WString_var> &_s );
void operator<<=( CORBA::Any &_a,
		  WStringSequenceTmpl<CORBA::WString_var> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    WStringSequenceTmpl<CORBA::WString_var> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const WStringSequenceTmpl<CORBA::WString_var> *&_s );

void operator<<=( CORBA::Any &a,
		  const StringSequenceTmpl<CORBA::String_var> &s );
void operator<<=( CORBA::Any &a,
		  StringSequenceTmpl<CORBA::String_var> *s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    StringSequenceTmpl<CORBA::String_var> &s );
CORBA::Boolean operator>>=( const CORBA::Any &a,
			    const StringSequenceTmpl<CORBA::String_var> *&s );

void operator<<=( CORBA::Any &_a,
		  const IfaceSequenceTmpl<CORBA::Object_var, CORBA::Object_ptr> &_s );
void operator<<=( CORBA::Any &_a,
		  IfaceSequenceTmpl<CORBA::Object_var, CORBA::Object_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    IfaceSequenceTmpl<CORBA::Object_var,
			                      CORBA::Object_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a,
			    const IfaceSequenceTmpl<CORBA::Object_var,
			                      CORBA::Object_ptr> *&_s );

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)

#endif
