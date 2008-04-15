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

#ifndef __mico_throw_h__
#define __mico_throw_h__

#include <stdarg.h>


#ifdef HAVE_EXCEPTIONS
#define MICO_CATCHANY(x) try { x; } catch (...) {}
#else
#define MICO_CATCHANY(x) x;
#endif

// workaround for Sun CC, which cannot call mico_throw() from template members
#ifdef HAVE_EXCEPTIONS
#ifdef HAVE_STD_EH
#define mico_user_throw_macro(ex) \
  ex._throwit();
#else
#define mico_user_throw_macro(ex) \
  ex._prepare_throw(); \
  ex._throwit();
#endif
#else
#define mico_user_throw_macro(ex) \
  CORBA::Exception::_throw_failed (&ex);
#endif

static void
mico_throw (const CORBA::Exception &ex)
{
    // avoid compiler warnings ...
    (void)mico_throw;

#ifdef HAVE_EXCEPTIONS

#ifndef HAVE_STD_EH
    ex._prepare_throw ();
#endif

    // if we dont have support for exceptions in shared libraries
    // we cannot use _throwit() but have to throw them here ...
#ifndef HAVE_SHARED_EXCEPTS
    const CORBA::SystemException *s;
    if ((s = CORBA::SystemException::_downcast (&ex))) {

#ifdef HAVE_STD_EH

#define SYSEXC(name)				\
	if (CORBA::name::_downcast (s))		\
	    throw *CORBA::name::_downcast (s);
#include <mico/sysexc.h>
#undef SYSEXC
	throw CORBA::UNKNOWN();

#else
	throw CORBA::SystemException_var (
	    (CORBA::SystemException *)s->_clone());
#endif

    }
#endif

    ex._throwit ();
#else
    CORBA::Exception::_throw_failed (&ex);
#endif
}

static void
mico_sii_throw (CORBA::StaticRequest *r, ...)
{
    // avoid compiler warnings ...
    (void)mico_sii_throw;

#ifdef HAVE_EXCEPTIONS
    if (r->exception()) {
	CORBA::Exception *ex = r->exception();
	CORBA::UnknownUserException *uuex =
	    CORBA::UnknownUserException::_downcast (ex);
	if (uuex) {
	    va_list args;
	    va_start (args, r);
	    CORBA::StaticTypeInfo *si;
	    const char *repoid;
	    while ((si = va_arg (args, CORBA::StaticTypeInfo *))) {
		repoid = va_arg (args, const char *);
		assert (repoid);
		if (!strcmp (uuex->_except_repoid(), repoid)) {
		    va_end (args);
		    CORBA::StaticAny &a = uuex->exception (si);
		    /*
		     * XXX this assumes that the value of the
		     *     pointer does not change when casting
		     *     the actual exception to CORBA::Exception*, i.e.:
		     *       x == (void *)(CORBA::Exception*)(SomeExcept*)x
		     */
		    mico_throw (*(CORBA::Exception*)a.value());
		}
	    }
	    va_end (args);
	    mico_throw (CORBA::UNKNOWN());
	} else {
	    mico_throw (*ex);
	}
    }
#else
    if (r->exception())
	CORBA::Exception::_throw_failed (r->exception());
#endif
}

inline void
CORBA::Exception::_raise ()
{
    ::mico_throw (*this);
}

#endif // __mico_throw_h__
