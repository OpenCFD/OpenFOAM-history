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

#ifndef __mico_except_h__
#define __mico_except_h__

#undef minor

namespace CORBA {

class Exception : public MagicChecker {

#ifdef _WIN32
  static void (__cdecl * _old_terminate_handler) ();
  static void __cdecl _terminate_handler ();
#else
    static void (*_old_terminate_handler) ();
    static void _terminate_handler ();
#endif
 
#ifndef HAVE_STD_EH
    static char *_last_except_desc;
#endif

    std::vector<std::string> stack_trace_;

public:
    Exception (const Exception &);
    virtual ~Exception ();
    Exception &operator= (const Exception &);

    virtual void _print (std::ostream &) const;
    virtual void _print_stack_trace (std::ostream&) const;

    virtual void _encode (DataEncoder &) const;
    virtual void _encode_any (Any &) const;
    virtual const char *_repoid () const;
    virtual Exception *_clone () const;

#ifndef	HAVE_STD_EH
    void _prepare_throw () const;
#endif

    void _raise ();
    virtual void _throwit () const;

    static Exception *_decode (Any &);
    static Exception *_decode (DataDecoder &);

    static void _throw_failed (const Exception *ex);
    static void _init ();
protected:
    Exception ();
};

enum {
    OMGVMCID = 0x4f4d0000
};

enum completion_status {
    COMPLETED_YES,
    COMPLETED_NO,
    COMPLETED_MAYBE
};

typedef completion_status CompletionStatus;

enum exception_type {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION
};

class SystemException : public Exception {
    ULong _minor;
    CompletionStatus _completed;
public:
    SystemException ();
    SystemException (const SystemException &);
    SystemException (ULong minor, CompletionStatus status);
    SystemException &operator= (const SystemException &);
    virtual ~SystemException ();

#ifdef HAVE_STD_EH
    SystemException *operator->() { return this; }
    SystemException& operator*() { return *this; }
    operator SystemException*() { return this; }
#endif

    ULong minor () const;
    void minor (ULong);

    CompletionStatus completed () const;
    void completed (CompletionStatus);

    virtual void _print (std::ostream &) const;
    virtual void _encode (DataEncoder &) const;
    virtual const char *_repoid () const;
    virtual Exception *_clone () const;
    virtual void _throwit() const;

    static SystemException *_downcast (Exception *);
    static const SystemException *_downcast (const Exception *);
    static SystemException *_decode (Any &);
    static SystemException *_decode (DataDecoder &);

    static SystemException *_create_sysex (const char *repoid, ULong minor,
					   CompletionStatus status);
    static Boolean _is_sysex (const char *repoid);
};

#ifdef HAVE_STD_EH

typedef SystemException SystemException_catch;

#else

typedef ExceptVar<SystemException> SystemException_var;
typedef SystemException_var SystemException_catch;

#endif

class UserException : public Exception {
public:
    UserException ();
    UserException (const UserException &);
    UserException &operator= (const UserException &);
    virtual ~UserException ();

    virtual void _print (std::ostream &) const;
    virtual void _encode (DataEncoder &) const;
    virtual const char *_repoid () const;
    virtual Exception *_clone () const;
    virtual void _throwit () const;

    static UserException *_downcast (Exception *);
    static const UserException *_downcast (const Exception *);
    static UserException *_decode (Any &);
    static UserException *_decode (DataDecoder &);
};

class UnknownUserException : public UserException {
    Any *_excpt;
    StaticAny *_static_except;
    DataDecoder *_dc;
    std::string _ex_repoid;
public:
    UnknownUserException (Any *);
    UnknownUserException (DataDecoder *);
    UnknownUserException ();
    UnknownUserException (const UnknownUserException &);
    UnknownUserException &operator= (const UnknownUserException &);
    virtual ~UnknownUserException ();

    Any &exception (TypeCode_ptr = 0);
    StaticAny &exception (CORBA::StaticTypeInfo *);

    virtual void _throwit () const;
    virtual void _print (std::ostream &) const;
    virtual void _encode (DataEncoder &) const;
    virtual void _encode_any (Any &) const;
    virtual const char *_repoid () const;
    virtual const char *_except_repoid ();
    virtual Exception *_clone () const;

    static UnknownUserException *_downcast (Exception *);
    static const UnknownUserException *_downcast (const Exception *);
};

#ifdef HAVE_STD_EH
#define DECLARE_THROWIT		virtual void _throwit () const;
#else
#define	DECLARE_THROWIT
#endif

#define SYSEXC(name)						\
class name : public SystemException {				\
public:								\
  name ();							\
  name (const name &ex);					\
  name (ULong minor, CompletionStatus status);			\
  name &operator= (const name &ex);				\
  virtual ~name ();						\
  virtual const char *_repoid () const;				\
  virtual Exception *_clone () const;				\
  virtual void _encode_any (Any &) const;			\
  DECLARE_THROWIT						\
  static name *_downcast (Exception *ex);		        \
  static const name *_downcast (const Exception *ex);	        \
};								\
extern MICO_EXPORT TypeCode_ptr _tc_ ## name;

#include <mico/sysexc.h>

#undef SYSEXC
#undef DECLARE_THROWIT

}

#endif // __mico_except_h__
