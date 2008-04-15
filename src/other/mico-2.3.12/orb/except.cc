/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <sstream>
#else
#include <iostream.h>
#include <strstream.h>
#endif
#include <mico/throw.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
#include <execinfo.h>
#endif // HAVE_BACKTRACE && HAVE_BACKTRACE_SYMBOLS
#if defined(HAVE_CXXABI_H) && defined(HAVE___CXA_DEMANGLE)
namespace __cxxabiv1 {
        class __class_type_info;
};
#include <cxxabi.h>
#endif // HAVE_CXXABI_H && HAVE___CXA_DEMANGLE

#endif // FAST_PCH


using namespace std;

/*************************** Exception ***************************/

void (*CORBA::Exception::_old_terminate_handler) () = NULL;

#ifndef HAVE_STD_EH

namespace CORBA {
  char *Exception::_last_except_desc = 0;
};

#endif

CORBA::Exception::Exception ()
{
#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
    if (MICO::Logger::IsLogged(MICO::Logger::Exception)) {
        const int max_length = 500;
        void* array[max_length];
        size_t size = backtrace(array, max_length);
        char** strings = backtrace_symbols(array, size);
        for (size_t i = 0; i < size; i++) {
            stack_trace_.push_back(string(CORBA::string_dup(strings[i])));
        }
        free (strings);
    }
#endif // HAVE_BACKTRACE && HAVE_BACKTRACE_SYMBOLS
}

CORBA::Exception::Exception (const Exception &e)
{
}

CORBA::Exception::~Exception ()
{
}

CORBA::Exception &
CORBA::Exception::operator= (const Exception &e)
{
    return *this;
}

CORBA::Exception *
CORBA::Exception::_decode (Any &a)
{
    Exception *e;
    if ((e = SystemException::_decode (a)))
	return e;
    if ((e = UserException::_decode (a)))
	return e;
    return NULL;
}

CORBA::Exception *
CORBA::Exception::_decode (DataDecoder &dc)
{
    Exception *e;
    ULong rpos = dc.buffer()->rpos();
    if ((e = SystemException::_decode (dc)))
	return e;
    dc.buffer()->rseek_beg (rpos);
    if ((e = UserException::_decode (dc)))
	return e;
    dc.buffer()->rseek_beg (rpos);
    return NULL;
}

void
CORBA::Exception::_print (ostream &os) const
{
    os << _repoid();
}

void
CORBA::Exception::_print_stack_trace (ostream& os) const
{
    if (this->stack_trace_.size() > 0) {
        os << "Exception stack trace:" << endl;
    }
#if defined(HAVE_CXXABI_H) && defined(HAVE___CXA_DEMANGLE)
    int status = 0;
    for (unsigned int i = 0; i < this->stack_trace_.size(); i++) {
        string stack_line = this->stack_trace_[i];
        unsigned long pos1 = stack_line.find('(');
        unsigned long pos2 = stack_line.rfind('+');
        if (pos1 != string::npos) {
            string stack_rec = stack_line.substr(pos1 + 1, pos2 - pos1 - 1);
            string obj_file = stack_line.substr(0, pos1);
            //cerr << "trying to demangle `" << stack_rec << "'" << endl;
            char* demangled_stack_rec = __cxxabiv1::__cxa_demangle(stack_rec.c_str(),0,0,&status);
            os << stack_line.substr(0, pos1 + 1);
            if (status == 0) {
                os << demangled_stack_rec;
                free(demangled_stack_rec);
            }
            else {
                os << stack_rec;
            }
            os << stack_line.substr(pos2) << endl;
        }
        else {
            os << stack_line << endl;
        }
    }
#else
    for (unsigned int i = 0; i < this->stack_trace_.size(); i++) {
        os << this->stack_trace_[i] << endl;
    }
#endif
}

const char *
CORBA::Exception::_repoid () const
{
    return "IDL:omg.org/CORBA/Exception:1.0";
}

CORBA::Exception *
CORBA::Exception::_clone () const
{
    assert (0);
    return (CORBA::Exception*) 0;
}

#ifndef HAVE_STD_EH

void
CORBA::Exception::_prepare_throw () const
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
#else
    ostrstream ostr;
#endif
    _print (ostr);

    if (_last_except_desc)
	CORBA::string_free (_last_except_desc);

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    _last_except_desc = CORBA::string_dup (ostr.str().c_str());
#else
    ostr << "" << ends;
    _last_except_desc = CORBA::string_dup (ostr.str());
    ostr.rdbuf()->freeze (0);
#endif
}

#endif

void 
CORBA::Exception::_throwit () const
{
    assert (0);
}

void
CORBA::Exception::_encode (DataEncoder &ec) const
{
    assert (0);
}

void
CORBA::Exception::_encode_any (Any &a) const
{
    assert (0);
}

void
CORBA::Exception::_throw_failed (const Exception *ex)
{
    cerr << "exception handling not supported, " << endl;
    cerr << "the following exception has been thrown: " << endl;
    ex->_print (cerr);
    cerr << endl;
#ifdef _POCKET_PC
	MICO_NOT_IMPLEMENTED;
#else
    abort ();
#endif
}


/************************* SystemException *************************/


CORBA::SystemException::SystemException ()
    : _minor (0), _completed (COMPLETED_NO)
{
}

CORBA::SystemException::SystemException (const SystemException &e)
    : _minor (e._minor), _completed (e._completed)
{
}

CORBA::SystemException::SystemException (ULong minor, CompletionStatus status)
    : _minor (minor), _completed (status)
{
}

CORBA::SystemException::~SystemException ()
{
}

CORBA::SystemException &
CORBA::SystemException::operator= (const SystemException &e)
{
    MICO_OBJ_CHECK (&e);

    if (this != &e) {
	Exception::operator= (e);
	_minor = e._minor;
	_completed = e._completed;
    }
    return *this;
}

const char *
CORBA::SystemException::_repoid () const
{
    return "IDL:omg.org/CORBA/SystemException:1.0";
}

CORBA::Exception *
CORBA::SystemException::_clone () const
{
    return new SystemException (*this);
}

CORBA::SystemException *
CORBA::SystemException::_create_sysex (const char *repoid, ULong minor,
				       CompletionStatus completed)
{
    if (!strcmp (repoid, "IDL:omg.org/CORBA/SystemException:1.0"))
	return new SystemException (minor, completed);

#define SYSEXC(name) \
    if (!strcmp (repoid, "IDL:omg.org/CORBA/" #name ":1.0")) \
	return new name (minor, completed);

#include <mico/sysexc.h>

#undef SYSEXC

    return NULL;
}

CORBA::Boolean
CORBA::SystemException::_is_sysex (const char *repoid)
{
#define SYSEXC(name) || !strcmp (repoid, "IDL:omg.org/CORBA/" #name ":1.0")

    return (!strcmp (repoid, "IDL:omg.org/CORBA/SystemException:1.0")
#include <mico/sysexc.h>
	);

#undef SYSEXC
}

CORBA::SystemException *
CORBA::SystemException::_downcast(Exception *e)
{
    MICO_OBJ_CHECK (e);

    if (e && _is_sysex (e->_repoid()))
	return (SystemException *)e;
    return NULL;
}

const CORBA::SystemException *
CORBA::SystemException::_downcast(const Exception *e)
{
    MICO_OBJ_CHECK (e);

    if (e && _is_sysex (e->_repoid()))
	return (const SystemException *)e;
    return NULL;
}

CORBA::SystemException *
CORBA::SystemException::_decode (Any &a)
{
    ULong k, minor = 0;
    CompletionStatus completed;
    String_var repoid;

    if (!(a.except_get_begin (repoid.out()) &&
	  (a >>= minor) &&
	  a.enum_get (k) &&
	  a.except_get_end ())) {
	return NULL;
    }
    completed = (CompletionStatus)k;

    return _create_sysex (repoid, minor, completed);
}

CORBA::SystemException *
CORBA::SystemException::_decode (DataDecoder &dc)
{
    ULong k, minor;
    CompletionStatus completed;
    string repoid;

    if (!(dc.except_begin (repoid) &&
	  dc.get_ulong (minor) &&
	  dc.enumeration (k) &&
	  dc.except_end ())) {
	return NULL;
    }
    completed = (CompletionStatus)k;

    return _create_sysex (repoid.c_str(), minor, completed);
}

void
CORBA::SystemException::_print (ostream &os) const
{
    os << _repoid() << " (" << _minor << ", ";
    switch (_completed) {
    case COMPLETED_YES:
	os << "completed";
	break;
    case COMPLETED_NO:
	os << "not-completed";
	break;
    case COMPLETED_MAYBE:
	os << "maybe-completed";
	break;
    default:
	assert (0);
    }
    os << ")";
}

void
CORBA::SystemException::_encode (DataEncoder &ec) const
{
    ec.except_begin (_repoid());
    ec.put_ulong (_minor);
    ec.enumeration ((ULong)_completed);
    ec.except_end ();
}

CORBA::ULong
CORBA::SystemException::minor () const
{
    MICO_OBJ_CHECK (this);

    return _minor;
}

void
CORBA::SystemException::minor (ULong minor)
{
    MICO_OBJ_CHECK (this);

    _minor = minor;
}

CORBA::CompletionStatus
CORBA::SystemException::completed () const
{
    MICO_OBJ_CHECK (this);

    return _completed;
}

void
CORBA::SystemException::completed (CompletionStatus status)
{
    MICO_OBJ_CHECK (this);

    _completed = status;
}


/************************* UserException *************************/


CORBA::UserException::UserException ()
{
}

CORBA::UserException::UserException (const UserException &)
{
}

CORBA::UserException &
CORBA::UserException::operator= (const UserException &e)
{
    MICO_OBJ_CHECK (&e);
    return *this;
}

CORBA::UserException::~UserException ()
{
}

void
CORBA::UserException::_throwit () const
{
    assert (0);
}

void
CORBA::UserException::_print (ostream &os) const
{
    os << _repoid();
}

void
CORBA::UserException::_encode (DataEncoder &ec) const
{
    ec.except_begin ("IDL:omg.org/CORBA/UserException:1.0");
    ec.except_end ();
}

const char *
CORBA::UserException::_repoid () const
{
    return "IDL:omg.org/CORBA/UserException:1.0";
}

CORBA::Exception *
CORBA::UserException::_clone () const
{
    return new UserException (*this);
}

CORBA::UserException *
CORBA::UserException::_downcast (Exception *e)
{
    if (!SystemException::_downcast (e))
	return (UserException *)e;
    return NULL;
}

const CORBA::UserException *
CORBA::UserException::_downcast (const Exception *e)
{
    if (!SystemException::_downcast (e))
	return (const UserException *)e;
    return NULL;
}

CORBA::UserException *
CORBA::UserException::_decode (Any &a)
{
    return new UnknownUserException (new Any (a));
}

CORBA::UserException *
CORBA::UserException::_decode (DataDecoder &dc)
{
    return new UnknownUserException (dc.clone ());
}


/********************** UnknownUserException **********************/


CORBA::UnknownUserException::UnknownUserException (Any *a)
{
    _excpt = a;
    _static_except = 0;
    _dc = 0;
}

CORBA::UnknownUserException::UnknownUserException (DataDecoder *dc)
{
    _excpt = 0;
    _static_except = 0;
    _dc = dc;
}

CORBA::UnknownUserException::UnknownUserException ()
{
    _excpt = new Any;
    _static_except = 0;
    _dc = 0;
}

CORBA::UnknownUserException::UnknownUserException (
    const UnknownUserException &ex)
{
    _excpt = ex._excpt ? new Any (*ex._excpt) : 0;
    _static_except =
	ex._static_except ? new StaticAny (*ex._static_except) : 0;
    _dc = ex._dc ? ex._dc->clone () : 0;
}

CORBA::UnknownUserException &
CORBA::UnknownUserException::operator= (const UnknownUserException &ex)
{
    if (_excpt)
	delete _excpt;
    if (_static_except)
	delete _static_except;
    if (_dc)
	delete _dc;
    _excpt = ex._excpt ? new Any (*ex._excpt) : 0;
    _static_except =
	ex._static_except ? new StaticAny (*ex._static_except) : 0;
    _dc = ex._dc ? ex._dc->clone () : 0;
    return *this;
}

CORBA::UnknownUserException::~UnknownUserException ()
{
    if (_excpt)
	delete _excpt;
    if (_static_except)
	delete _static_except;
    if (_dc)
	delete _dc;
}

CORBA::Any &
CORBA::UnknownUserException::exception (TypeCode_ptr tc)
{
    if (!_excpt) {
	assert (tc);
	assert (_dc);
	_excpt = new Any;
	CORBA::Boolean r = _excpt->demarshal (tc, *_dc);
	assert (r);
    }
    return *_excpt;
}

CORBA::StaticAny &
CORBA::UnknownUserException::exception (StaticTypeInfo *stc)
{
    if (_static_except)
	return *_static_except;

    _static_except = new CORBA::StaticAny (stc);

    if (_excpt) {
	// XXX slow ...
	MICO::CDREncoder ec;
	MICO::CDRDecoder dc (ec.buffer(), FALSE, ec.byteorder(),
			     ec.converter(), FALSE);

	_excpt->marshal (ec);

	CORBA::Boolean ret = _static_except->demarshal (dc);
	assert (ret);
    } else {
	assert (_dc);
	CORBA::Long rpos = _dc->buffer()->rpos();

	CORBA::Boolean ret = _static_except->demarshal (*_dc);
	assert (ret);

	_dc->buffer()->rseek_beg (rpos);
    }
    return *_static_except;
}

void
CORBA::UnknownUserException::_encode_any (Any & any) const
{
  assert (_excpt);
  any = *_excpt;
}

void
CORBA::UnknownUserException::_throwit () const
{
    assert (0);
}

void
CORBA::UnknownUserException::_print (ostream &os) const
{
    os << ((UnknownUserException *)this)->_except_repoid()
       << " within " << _repoid();
}

void
CORBA::UnknownUserException::_encode (DataEncoder &ec) const
{
    if (_excpt) {
	_excpt->marshal (ec);
    } else {
	assert (_dc);
	// XXX alignment, differing codecs ???
	assert (!strcmp (ec.type(), _dc->type()));
	ec.put_octets (_dc->buffer()->data(), _dc->buffer()->length());
    }
}

const char *
CORBA::UnknownUserException::_except_repoid ()
{
    if (_excpt) {
	String_var repoid;
	CORBA::Boolean r = _excpt->except_get_begin (repoid.out());
	assert (r);
	_excpt->rewind ();
	_ex_repoid = repoid;
    } else {
	assert (_dc);
	ULong rpos = _dc->buffer()->rpos();
	CORBA::Boolean r = _dc->except_begin (_ex_repoid);
	assert (r);
	_dc->buffer()->rseek_beg (rpos);
    }
    return _ex_repoid.c_str();
}

const char *
CORBA::UnknownUserException::_repoid () const
{
    return "IDL:omg.org/CORBA/UnknownUserException:1.0";
}

CORBA::Exception *
CORBA::UnknownUserException::_clone () const
{
    return new UnknownUserException (*this);
}

CORBA::UnknownUserException *
CORBA::UnknownUserException::_downcast (Exception *e)
{
    if (e && !strcmp (e->_repoid(),
		      "IDL:omg.org/CORBA/UnknownUserException:1.0"))
	return (UnknownUserException *)e;
    return NULL;
}

const CORBA::UnknownUserException *
CORBA::UnknownUserException::_downcast (const Exception *e)
{
    if (e && !strcmp (e->_repoid(),
		      "IDL:omg.org/CORBA/UnknownUserException:1.0"))
	return (const UnknownUserException *)e;
    return NULL;
}
