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

#define MICO_CONF_INTERCEPT
#define MICO_CONF_POA
#define MICO_CONF_IR

#include <CORBA-SMALL.h>
#include <string.h>
#include <mico/throw.h>
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

template<class T>
static inline void
insert_interceptor (list<T> &l, T ic)
{
    typename list<T>::iterator i = l.end();
    if (l.size() > 0) {
	do {
	    --i;
	    if (ic->prio() < (*i)->prio()) {
		++i;
		break;
	    }
	} while (i != l.begin());
    }
    l.insert (i, ic);
}

template<class T>
static inline
void remove_interceptor (list<T> &l, T ic)
{
    for (typename list<T>::iterator i = l.begin(); i != l.end(); ++i) {
	if (*i == ic) {
	    l.erase (i);
	    break;
	}
    }
}


/******************************* Root **********************************/


Interceptor::Root::Root ()
{
    _prio = 0;
    _is_active = FALSE;
}

Interceptor::Root::Root (Priority p)
{
    _prio = p;
    _is_active = TRUE;
}

Interceptor::Root::~Root ()
{
}

const char *
Interceptor::Root::_repoid() const
{
    return "IDL:omg.org/Interceptor/Root:1.0";
}

Interceptor::Root_ptr
Interceptor::Root::_narrow (Root_ptr _obj)
{
    return _duplicate (_obj);
}

Interceptor::Root::Priority
Interceptor::Root::prio()
{
    return _prio;
}

void
Interceptor::Root::activate (Priority p)
{
    _prio = p;
    _is_active = TRUE;
}

void
Interceptor::Root::deactivate ()
{
    _is_active = FALSE;
}

CORBA::Boolean
Interceptor::Root::is_active ()
{
    return _is_active;
}


/******************************* Context **********************************/


Interceptor::Context::Context ()
{
}

Interceptor::Context::~Context ()
{
}

const char *
Interceptor::Context::_repoid() const
{
    return "IDL:omg.org/Interceptor/Context:1.0";
}

Interceptor::Context_ptr
Interceptor::Context::_narrow (Context_ptr _obj)
{
    return _duplicate (_obj);
}

void Interceptor::Context::destroy ()
{
    // XXX ??
    CORBA::release (this);
}


/*************************** LWRootRequest ******************************/


Interceptor::LWRootRequest::LWRootRequest (
    CORBA::Object_var &obj,
    CORBA::String_var &op,
    IOP::ServiceContextList &svc)
    : _svc (svc), _op (op), _obj (obj)
{
}

Interceptor::LWRootRequest::~LWRootRequest ()
{
}

const char *
Interceptor::LWRootRequest::_repoid() const
{
    return "IDL:omg.org/Interceptor/LWRootRequest:1.0";
}

Interceptor::LWRootRequest_ptr
Interceptor::LWRootRequest::_narrow (LWRootRequest_ptr _obj)
{
    return _duplicate (_obj);
}

CORBA::Object_ptr
Interceptor::LWRootRequest::target()
{
    return CORBA::Object::_duplicate ((CORBA::Object_ptr)_obj);
}

void
Interceptor::LWRootRequest::target (CORBA::Object_ptr obj)
{
    _obj = CORBA::Object::_duplicate (obj);
}

CORBA::Identifier
Interceptor::LWRootRequest::operation()
{
    return CORBA::string_dup (_op);
}

void
Interceptor::LWRootRequest::operation (const CORBA::Identifier op)
{
    _op = CORBA::string_dup (op);
}

void
Interceptor::LWRootRequest::set_service_context (ServiceID id,
						 CORBA::Long flags,
						 const ContextData &d)
{
    for (CORBA::ULong i = 0; i < _svc.length(); ++i) {
	if (_svc[i].context_id == id) {
	    if (flags /* & EXCLUSIVE */)
		mico_throw (CORBA::NO_PERMISSION());
	    _svc[i].context_data = d;
	    return;
	}
    }
    CORBA::ULong l = _svc.length();
    _svc.length (l+1);
    _svc[l].context_id = id;
    _svc[l].context_data = d;
}

Interceptor::ContextData *
Interceptor::LWRootRequest::get_service_context (ServiceID id,
						 CORBA::Long flags)
{
    for (CORBA::ULong i = 0; i < _svc.length(); ++i) {
	if (_svc[i].context_id == id)
	    return new ContextData (_svc[i].context_data);
    }
    mico_throw (CORBA::BAD_PARAM());
    return 0;
}

void
Interceptor::LWRootRequest::remove_service_context (ServiceID id)
{
    for (CORBA::ULong i = 0; i < _svc.length(); ++i) {
	if (_svc[i].context_id == id) {
	    if (i != _svc.length()-1)
		_svc[i] = _svc[_svc.length()-1];
	    _svc.length (_svc.length()-1);
	    return;
	}
    }
}

CORBA::Boolean
Interceptor::LWRootRequest::has_service_context (ServiceID id)
{
    for (CORBA::ULong i = 0; i < _svc.length(); ++i) {
	if (_svc[i].context_id == id)
	    return TRUE;
    }
    return FALSE;
}

void
Interceptor::LWRootRequest::set_context (Root_ptr interceptor,
					 Context_ptr ctx)
{
    _ctxs[interceptor] = Context::_duplicate (ctx);
}

Interceptor::Context_ptr
Interceptor::LWRootRequest::get_context (Root_ptr interceptor)
{
    return Context::_duplicate (_ctxs[interceptor]);
}


/**************************** LWRequest *******************************/


Interceptor::LWRequest::LWRequest (
    CORBA::Object_var &obj,
    CORBA::String_var &op,
    IOP::ServiceContextList &svc,
    CORBA::Request_ptr req)
    : Interceptor::LWRootRequest (obj, op, svc), _req (req)
{
}

Interceptor::LWRequest::~LWRequest ()
{
}

const char *
Interceptor::LWRequest::_repoid() const
{
    return "IDL:omg.org/Interceptor/LWRequest:1.0";
}

Interceptor::LWRequest_ptr
Interceptor::LWRequest::_narrow (LWRootRequest_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Interceptor/LWRequest:1.0"))
	return _duplicate ((LWRequest_ptr)_obj);
    return _nil();
}

CORBA::Request_ptr
Interceptor::LWRequest::request ()
{
    return CORBA::Request::_duplicate (_req);
}


/************************** LWServerRequest *****************************/


Interceptor::LWServerRequest::LWServerRequest (
    CORBA::Object_ptr obj,
    const char *op,
    IOP::ServiceContextList &svc,
    CORBA::ServerRequest_ptr req)
    : Interceptor::LWRootRequest (_obj, _op, svc), _req (req),
      _obj (CORBA::Object::_duplicate (obj)), _op (op)
{
}

Interceptor::LWServerRequest::~LWServerRequest ()
{
}

const char *
Interceptor::LWServerRequest::_repoid() const
{
    return "IDL:omg.org/Interceptor/LWServerRequest:1.0";
}

Interceptor::LWServerRequest_ptr
Interceptor::LWServerRequest::_narrow (LWRootRequest_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(),
		 "IDL:omg.org/Interceptor/LWServerRequest:1.0"))
	return _duplicate ((LWServerRequest_ptr)_obj);
    return _nil();
}

CORBA::ServerRequest_ptr
Interceptor::LWServerRequest::request ()
{
    return CORBA::ServerRequest::_duplicate (_req);
}


/************************** ClientInterceptor *****************************/


namespace Interceptor {
  list<Interceptor::ClientInterceptor_ptr> *ClientInterceptor::_interceptors;
}

list<Interceptor::ClientInterceptor_ptr> &
Interceptor::ClientInterceptor::_ics ()
{
    if (!_interceptors)
        _interceptors = new list<Interceptor::ClientInterceptor_ptr>;
    return *_interceptors;
}

Interceptor::ClientInterceptor::ClientInterceptor ()
{
}

Interceptor::ClientInterceptor::ClientInterceptor (Priority p)
    : Interceptor::Root(p)
{
    insert_interceptor (_ics(), this);
}

Interceptor::ClientInterceptor::~ClientInterceptor ()
{
    remove_interceptor (_ics(), this);
}

const char *
Interceptor::ClientInterceptor::_repoid() const
{
    return "IDL:omg.org/Interceptor/ClientInterceptor:1.0";
}

Interceptor::ClientInterceptor_ptr
Interceptor::ClientInterceptor::_narrow (Root_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(),
		 "IDL:omg.org/Interceptor/ClientInterceptor:1.0"))
	return _duplicate ((ClientInterceptor_ptr)_obj);
    return _nil();
}

Interceptor::Status
Interceptor::ClientInterceptor::initialize_request (LWRequest_ptr req,
						    CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ClientInterceptor::after_marshal (LWRequest_ptr req,
					       CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ClientInterceptor::output_message (CORBA::Buffer *buf,
						CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ClientInterceptor::input_message (CORBA::Buffer *buf,
					       CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ClientInterceptor::before_unmarshal (LWRequest_ptr req,
						  CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ClientInterceptor::finish_request (LWRequest_ptr req,
						CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

void
Interceptor::ClientInterceptor::activate (Priority p)
{
    Root::activate (p);
    insert_interceptor (_ics(), this);
}

void
Interceptor::ClientInterceptor::deactivate ()
{
    Root::deactivate ();
    remove_interceptor (_ics(), this);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec (
    LWRequest_ptr req,
    CORBA::Environment_ptr env,
    InterceptorMethod1 m)
{
    if (_ics().size() == 0)
	return TRUE;

    assert (!CORBA::is_nil (req));

    list<ClientInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s = ((*i)->*m) (req, env);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env,
    InterceptorMethod2 m)
{
    if (_ics().size() == 0)
	return TRUE;

    list<ClientInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s = ((*i)->*m) (buf, env);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_initialize_request (
    LWRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ClientInterceptor::initialize_request);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_after_marshal (
    LWRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ClientInterceptor::after_marshal);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_output_message (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env)
{
    return _exec (buf, env, &ClientInterceptor::output_message);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_input_message (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env)
{
    return _exec (buf, env, &ClientInterceptor::input_message);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_before_unmarshal (
    LWRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ClientInterceptor::before_unmarshal);
}

CORBA::Boolean
Interceptor::ClientInterceptor::_exec_finish_request (
    LWRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ClientInterceptor::finish_request);
}

Interceptor::LWRequest_ptr
Interceptor::ClientInterceptor::_create_request (
    CORBA::Object_var &obj,
    CORBA::String_var &op,
    IOP::ServiceContextList &svc,
    CORBA::Request_ptr req)
{
    if (_ics().size() == 0)
	return LWRequest::_nil();
    return new LWRequest (obj, op, svc, req);
}


/************************** ServerInterceptor *****************************/


namespace Interceptor {
  list<Interceptor::ServerInterceptor_ptr> *ServerInterceptor::_interceptors;
}

list<Interceptor::ServerInterceptor_ptr> &
Interceptor::ServerInterceptor::_ics ()
{
    if (!_interceptors)
        _interceptors = new list<Interceptor::ServerInterceptor_ptr>;
    return *_interceptors;
}

Interceptor::ServerInterceptor::ServerInterceptor ()
{
}

Interceptor::ServerInterceptor::ServerInterceptor (Priority p)
    : Interceptor::Root(p)
{
    insert_interceptor (_ics(), this);
}

Interceptor::ServerInterceptor::~ServerInterceptor ()
{
    remove_interceptor (_ics(), this);
}

const char *
Interceptor::ServerInterceptor::_repoid() const
{
    return "IDL:omg.org/Interceptor/ServerInterceptor:1.0";
}

Interceptor::ServerInterceptor_ptr
Interceptor::ServerInterceptor::_narrow (Root_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(),
		 "IDL:omg.org/Interceptor/ServerInterceptor:1.0"))
	return _duplicate ((ServerInterceptor_ptr)_obj);
    return _nil();
}

Interceptor::Status
Interceptor::ServerInterceptor::input_message (CORBA::Buffer *buf,
					       CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ServerInterceptor::initialize_request (LWServerRequest_ptr req,
						    CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ServerInterceptor::after_unmarshal (LWServerRequest_ptr req,
						 CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ServerInterceptor::before_marshal (LWServerRequest_ptr req,
						CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ServerInterceptor::finish_request (LWServerRequest_ptr req,
						CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ServerInterceptor::output_message (CORBA::Buffer *buf,
						CORBA::Environment_ptr env)
{
    return INVOKE_CONTINUE;
}

void
Interceptor::ServerInterceptor::activate (Priority p)
{
    Root::activate (p);
    insert_interceptor (_ics(), this);
}

void
Interceptor::ServerInterceptor::deactivate ()
{
    Root::deactivate ();
    remove_interceptor (_ics(), this);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec (
    LWServerRequest_ptr req,
    CORBA::Environment_ptr env,
    InterceptorMethod1 m)
{
    if (_ics().size() == 0)
	return TRUE;

    assert (!CORBA::is_nil (req));

    list<ServerInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s = ((*i)->*m) (req, env);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env,
    InterceptorMethod2 m)
{
    if (_ics().size() == 0)
	return TRUE;

    list<ServerInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s = ((*i)->*m) (buf, env);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_input_message (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env)
{
    return _exec (buf, env, &ServerInterceptor::input_message);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_initialize_request (
    LWServerRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ServerInterceptor::initialize_request);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_after_unmarshal (
    LWServerRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ServerInterceptor::after_unmarshal);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_before_marshal (
    LWServerRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ServerInterceptor::before_marshal);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_finish_request (
    LWServerRequest_ptr req,
    CORBA::Environment_ptr env)
{
    return _exec (req, env, &ServerInterceptor::finish_request);
}

CORBA::Boolean
Interceptor::ServerInterceptor::_exec_output_message (
    CORBA::Buffer *buf,
    CORBA::Environment_ptr env)
{
    return _exec (buf, env, &ServerInterceptor::output_message);
}

Interceptor::LWServerRequest_ptr
Interceptor::ServerInterceptor::_create_request (
    CORBA::Object_ptr obj,
    const char *op,
    IOP::ServiceContextList &svc,
    CORBA::ServerRequest_ptr req)
{
    if (_ics().size() == 0)
	return LWServerRequest::_nil();
    return new LWServerRequest (obj, op, svc, req);
}


/************************** InitInterceptor *****************************/


namespace Interceptor {
  list<Interceptor::InitInterceptor_ptr> *InitInterceptor::_interceptors;
}

list<Interceptor::InitInterceptor_ptr> &
Interceptor::InitInterceptor::_ics ()
{
    if (!_interceptors)
        _interceptors = new list<Interceptor::InitInterceptor_ptr>;
    return *_interceptors;
}

Interceptor::InitInterceptor::InitInterceptor ()
{
}

Interceptor::InitInterceptor::InitInterceptor (Priority p)
    : Interceptor::Root(p)
{
    insert_interceptor (_ics(), this);
}

Interceptor::InitInterceptor::~InitInterceptor ()
{
    remove_interceptor (_ics(), this);
}

const char *
Interceptor::InitInterceptor::_repoid() const
{
    return "IDL:omg.org/Interceptor/InitInterceptor:1.0";
}

Interceptor::InitInterceptor_ptr
Interceptor::InitInterceptor::_narrow (Root_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(),
		 "IDL:omg.org/Interceptor/InitInterceptor:1.0"))
	return _duplicate ((InitInterceptor_ptr)_obj);
    return _nil();
}

Interceptor::Status
Interceptor::InitInterceptor::initialize (CORBA::ORB_ptr orb,
					  const char *id,
					  int &argc, char *argv[])
{
    return INVOKE_CONTINUE;
}

void
Interceptor::InitInterceptor::activate (Priority p)
{
    Root::activate (p);
    insert_interceptor (_ics(), this);
}

void
Interceptor::InitInterceptor::deactivate ()
{
    Root::deactivate ();
    remove_interceptor (_ics(), this);
}

CORBA::Boolean
Interceptor::InitInterceptor::_exec_initialize (CORBA::ORB_ptr orb,
						const char *id,
						int &argc, char *argv[])
{
    if (_ics().size() == 0)
	return TRUE;

    list<InitInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s =  (*i)->initialize (orb, id, argc, argv);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}


/************************** ConnInterceptor *****************************/


namespace Interceptor {
  list<Interceptor::ConnInterceptor_ptr> *ConnInterceptor::_interceptors;
}

list<Interceptor::ConnInterceptor_ptr> &
Interceptor::ConnInterceptor::_ics ()
{
    if (!_interceptors)
        _interceptors = new list<Interceptor::ConnInterceptor_ptr>;
    return *_interceptors;
}

Interceptor::ConnInterceptor::ConnInterceptor ()
{
}

Interceptor::ConnInterceptor::ConnInterceptor (Priority p)
    : Interceptor::Root(p)
{
    insert_interceptor (_ics(), this);
}

Interceptor::ConnInterceptor::~ConnInterceptor ()
{
    remove_interceptor (_ics(), this);
}

const char *
Interceptor::ConnInterceptor::_repoid() const
{
    return "IDL:omg.org/Interceptor/ConnInterceptor:1.0";
}

Interceptor::ConnInterceptor_ptr
Interceptor::ConnInterceptor::_narrow (Root_ptr _obj)
{
    if (CORBA::is_nil (_obj))
	return _nil();
    if (!strcmp (_obj->_repoid(),
		 "IDL:omg.org/Interceptor/ConnInterceptor:1.0"))
	return _duplicate ((ConnInterceptor_ptr)_obj);
    return _nil();
}

Interceptor::Status
Interceptor::ConnInterceptor::client_connect (const char *addr)
{
    return INVOKE_CONTINUE;
}

Interceptor::Status
Interceptor::ConnInterceptor::client_disconnect (const char *addr)
{
    return INVOKE_CONTINUE;
}

void
Interceptor::ConnInterceptor::activate (Priority p)
{
    Root::activate (p);
    insert_interceptor (_ics(), this);
}

void
Interceptor::ConnInterceptor::deactivate ()
{
    Root::deactivate ();
    remove_interceptor (_ics(), this);
}

CORBA::Boolean
Interceptor::ConnInterceptor::_exec_client_connect (const char *addr)
{
    if (_ics().size() == 0)
	return TRUE;

    list<ConnInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s =  (*i)->client_connect (addr);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}

CORBA::Boolean
Interceptor::ConnInterceptor::_exec_client_disconnect (const char *addr)
{
    if (_ics().size() == 0)
	return TRUE;

    list<ConnInterceptor_ptr>::iterator i;
    for (i = _ics().begin(); i != _ics().end(); ++i) {
	Status s =  (*i)->client_disconnect (addr);
	if (s == INVOKE_ABORT)
	    return FALSE;
	if (s == INVOKE_BREAK)
	    return TRUE;
	if (s == INVOKE_RETRY) {
	    // XXX what to do ???
	    assert (0);
	}
    }
    return TRUE;
}
