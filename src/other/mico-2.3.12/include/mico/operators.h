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

#ifndef __mico_operators_h__
#define __mico_operators_h__


// <<= and >>= operators for Any_var 


static inline void
operator<<= (CORBA::Any_var &a, CORBA::Short v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Long v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::LongLong v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::UShort v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::ULong v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::ULongLong v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Float v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Double v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::LongDouble v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, const CORBA::Any & v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any *v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, const CORBA::Exception & v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Exception *v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, const char * v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, const wchar_t * v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_boolean v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_octet v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_char v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_wchar v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_string v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_wstring v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_fixed v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Any::from_object v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Object_ptr v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Object_ptr *v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::TypeCode_ptr v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::TypeCode_ptr *v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Context_ptr v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Context_ptr *v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Principal_ptr v)
{
    a.inout() <<= v;
}

static inline void
operator<<= (CORBA::Any_var &a, CORBA::Principal_ptr *v)
{
    a.inout() <<= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Short &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Long &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::LongLong &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::UShort &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::ULong &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::ULongLong &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Float &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Double &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::LongDouble &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, const CORBA::Any *&v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, const char * &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, const wchar_t * &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_boolean v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_octet v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_char v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_wchar v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_string v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_wstring v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_object v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_abstract_base v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Any::to_fixed v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Object_ptr &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::TypeCode_ptr &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Context_ptr &v)
{
    return a.in() >>= v;
}

static inline CORBA::Boolean
operator>>= (const CORBA::Any_var &a, CORBA::Principal_ptr &v)
{
    return a.in() >>= v;
}


// <<= and >>= operators for system exceptions


#define SYSEXC(name)							\
void operator<<= (CORBA::Any &, const CORBA::name &);			\
void operator<<= (CORBA::Any &, CORBA::name *);				\
CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::name &);		\
CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::name *&);

#include <mico/sysexc.h>

#undef SYSEXC


// iostream operators for various types


static inline std::ostream &
operator<< (std::ostream &os, const CORBA::Exception *ex)
{
    ex->_print (os);
    return os;
}

static inline std::ostream &
operator<< (std::ostream &os, const CORBA::Exception &ex)
{
    ex._print (os);
    return os;
}

static inline std::ostream &
operator<< (std::ostream &os, CORBA::String_var &svar)
{
    return os << svar.in();
}

static inline std::istream &
operator>> (std::istream &is, CORBA::String_var &svar)
{
    std::string s;
    is >> s;
    svar = s.c_str();
    return is;
}

#endif // __mico_operators_h__
