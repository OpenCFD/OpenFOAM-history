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

// workaround for "define char *string" in some g++ versions ...
#define string __ex_string

// prefer exception for SGI
#if defined(HAVE_EXCEPTION)
#include <exception>
#elif defined(HAVE_EXCEPTION_H)
#include <exception.h>
#endif

#undef string


#ifdef HAVE_TERMINATE_H
#include <terminate.h>
#endif


using namespace std;

/*************************** Exception ***************************/

void
CORBA::Exception::_init ()
{
#ifdef HAVE_EXCEPTS
    if (!_old_terminate_handler)
	_old_terminate_handler = set_terminate (_terminate_handler);
#endif
}

void
CORBA::Exception::_terminate_handler ()
{
#ifdef HAVE_STD_EH

   set_terminate (_old_terminate_handler) ;
   try {
       throw;
   } catch (const CORBA::Exception& ex) {
     cerr << "uncaught MICO exception: ";
     ex._print (cerr);
     cerr << endl;
     ex._print_stack_trace (cerr);
     throw;
   }

#else

    cerr << "uncaught MICO exception: ";
    if (_last_except_desc)
	cerr << _last_except_desc << endl;
    (*_old_terminate_handler) ();

#endif
}

void
CORBA::SystemException::_throwit () const
{
#ifdef HAVE_EXCEPTS

#ifdef HAVE_STD_EH
    throw *this;
#else
    throw CORBA::SystemException_var ((CORBA::SystemException *) _clone ());
#endif

#else
    CORBA::Exception::_throw_failed (this);
#endif
}


/*********************** System Exceptions ***********************/


#ifdef HAVE_STD_EH

#ifdef HAVE_EXCEPTS

#define DEFINE_THROWIT(name)                                            \
void CORBA::name::_throwit () const                                     \
{                                                                       \
    throw *this;                                                        \
}

#else

#define DEFINE_THROWIT(name)                                            \
void CORBA::name::_throwit () const                                     \
{                                                                       \
    CORBA::Exception::_throw_failed (this);                             \
}

#endif

#else

#define DEFINE_THROWIT(name)

#endif

#define SYSEXC(name)							  \
									  \
class TC##name : public CORBA::StaticTypeInfo {				  \
    typedef CORBA::name _MICO_T;					  \
public:									  \
    StaticValueType create () const					  \
    {									  \
	return (StaticValueType) new _MICO_T;				  \
    }									  \
    void assign (StaticValueType d, const StaticValueType s) const	  \
    {									  \
	*(_MICO_T*) d = *(_MICO_T*) s;					  \
    }									  \
    void free (StaticValueType v) const					  \
    {									  \
	delete (_MICO_T*) v;						  \
    }									  \
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc,			  \
			      StaticValueType v) const			  \
    {									  \
	string repoid;							  \
	CORBA::ULong ul;						  \
	if (!dc.except_begin (repoid))					  \
	    return FALSE;						  \
	if (!dc.get_ulong (ul))						  \
	    return FALSE;						  \
	((_MICO_T*)v)->minor (ul);					  \
	if (!dc.enumeration (ul))					  \
	    return FALSE;						  \
	((_MICO_T*)v)->completed ((CORBA::CompletionStatus)ul);		  \
	return dc.except_end();						  \
    }									  \
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const	  \
    {									  \
	ec.except_begin( "IDL:omg.org/CORBA/" #name ":1.0" );		  \
	ec.put_ulong (((_MICO_T*)v)->minor());				  \
	ec.enumeration ((CORBA::ULong)((_MICO_T*)v)->completed());	  \
	ec.except_end();						  \
    }									  \
    CORBA::TypeCode_ptr typecode ()					  \
    {									  \
	return CORBA::_tc_##name;					  \
    }									  \
									  \
    static TC##name *instance ();					  \
};									  \
									  \
TC##name *TC##name::instance ()						  \
{									  \
    static TC##name *p = 0;                                               \
    if (!p)                                                               \
        p = new TC##name;                                                 \
    return p;                                                             \
}									  \
									  \
CORBA::name::name ()							  \
{									  \
}									  \
									  \
CORBA::name::name (const name &ex)					  \
    : CORBA::SystemException (ex)					  \
{									  \
}									  \
									  \
CORBA::name::name (ULong minorval, CompletionStatus status)		  \
    : CORBA::SystemException (minorval, status)				  \
{									  \
}									  \
									  \
CORBA::name &								  \
CORBA::name::operator= (const name &ex)					  \
{									  \
    CORBA::SystemException::operator=(ex);			  \
    return *this;							  \
}									  \
									  \
CORBA::name::~name ()							  \
{									  \
}									  \
									  \
CORBA::Exception *							  \
CORBA::name::_clone () const						  \
{									  \
    return new name (*this);						  \
}									  \
									  \
DEFINE_THROWIT(name)							  \
									  \
const char *								  \
CORBA::name::_repoid ()	const						  \
{									  \
    return "IDL:omg.org/CORBA/" #name ":1.0";				  \
}									  \
									  \
void									  \
CORBA::name::_encode_any (Any &a) const					  \
{									  \
    a <<= *this;							  \
}									  \
									  \
CORBA::name *								  \
CORBA::name::_downcast (Exception *ex)				          \
{									  \
    if (ex && !strcmp (ex->_repoid(), "IDL:omg.org/CORBA/" #name ":1.0")) \
	return (name *)ex;						  \
    return NULL;							  \
}									  \
									  \
const CORBA::name *							  \
CORBA::name::_downcast (const Exception *ex)			          \
{									  \
    if (ex && !strcmp (ex->_repoid(), "IDL:omg.org/CORBA/" #name ":1.0")) \
	return (const name *)ex;					  \
    return NULL;							  \
}									  \
									  \
void									  \
operator<<= (CORBA::Any &_a, const CORBA::name &_ex)			  \
{									  \
    CORBA::StaticAny sa (TC##name::instance(), &_ex);			  \
    _a.from_static_any (sa);						  \
}									  \
									  \
void									  \
operator<<= (CORBA::Any &_a, CORBA::name *_ex)				  \
{									  \
    _a <<= *_ex;							  \
    delete _ex;								  \
}									  \
									  \
CORBA::Boolean								  \
operator>>= (const CORBA::Any &_a, CORBA::name &_ex)			  \
{									  \
    CORBA::StaticAny sa (TC##name::instance(), &_ex);			  \
    return _a.to_static_any (sa);					  \
}									  \
									  \
CORBA::Boolean								  \
operator>>= (const CORBA::Any &_a, CORBA::name *&_ex)			  \
{									  \
    return _a.to_static_any (TC##name::instance(), (void *&)_ex);	  \
}

#include <mico/sysexc.h>

#undef SYSEXC
#undef DEFINE_THROWIT
