/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2004 by The Mico Team
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
#define MICO_CONF_IR
#define MICO_CONF_INTERCEPT
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <ctype.h>
#include <mico/throw.h>
#include <mico/impl.h>
#include <mico/intercept.h>
#include <mico/util.h>
#include <mico/template_impl.h>
#include <mico/os-misc.h>
#include <mico/basic_seq.h>
#include <mico/pi_impl.h>

#ifndef USE_CSL2
#  include <mico/ssl.h>
#endif /* NOT USE_CSL2  */

#endif // FAST_PCH


using namespace std;

/*****************************  NamedValue  ***************************/


CORBA::NamedValue::NamedValue(const char *nameval, const Any &val,
			      Flags flagsval)
    : _n (nameval),  _v (val), _f (flagsval)
{
}

CORBA::NamedValue::NamedValue (const char *nameval, Flags flagsval)
    : _n (nameval), _f (flagsval)
{
}

CORBA::NamedValue::NamedValue ()
    : _n ((const char *)0), _f (0)
{
    // vsOrb wants us to initialize the name to NULL
}

CORBA::NamedValue &
CORBA::NamedValue::operator= (const NamedValue &nv)
{
    _n = nv._n;
    _f = nv._f;
    _v = nv._v;
    return *this;
}

CORBA::NamedValue::~NamedValue ()
{
}


/*****************************  NVList  ***************************/


CORBA::NVList::NVList (ULong countval)
{
    _vec.reserve (countval);
}

CORBA::NVList::~NVList ()
{
}

CORBA::NamedValue_ptr
CORBA::NVList::add (Flags flags)
{
    MICO_OBJ_CHECK (this);

    _vec.push_back (new NamedValue ("", flags));
    return _vec.back();
}

CORBA::NamedValue_ptr
CORBA::NVList::add_item (const char *name, Flags flags)
{
    MICO_OBJ_CHECK (this);

    _vec.push_back (new NamedValue (name, flags));
    return _vec.back();
}

CORBA::NamedValue_ptr
CORBA::NVList::add_value (const char *name, const Any &val, Flags flags)
{
    _vec.push_back (new NamedValue (name, val, flags));
    return _vec.back();
}

CORBA::NamedValue_ptr
CORBA::NVList::add_item_consume (char *name, Flags flags)
{
    MICO_OBJ_CHECK (this);

    _vec.push_back (new NamedValue (name, flags));
    CORBA::string_free (name);
    return _vec.back();
}

CORBA::NamedValue_ptr
CORBA::NVList::add_value_consume (char *name, Any *val, Flags flags)
{
    _vec.push_back (new NamedValue (name, *val, flags));
    delete val;
    CORBA::string_free (name);
    return _vec.back();
}

CORBA::NamedValue_ptr
CORBA::NVList::item (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    return _vec[idx];
}

void
CORBA::NVList::remove (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    _vec.erase (_vec.begin() + idx);
}

CORBA::Boolean
CORBA::NVList::copy (NVList_ptr src, Flags f)
{
    NVList_ptr dst = this;

    int dst_count = dst->count ();
    int src_count = src->count ();

    NamedValue_ptr dst_nv, src_nv;

    for (int d = 0, s = 0;; ++d, ++s) {
	dst_nv = NamedValue::_nil();
	for ( ; d < dst_count; ++d) {
	    dst_nv = dst->item (d);
	    if (dst_nv->flags () & f)
		break;
	}
	src_nv = NamedValue::_nil();
	for ( ; s < src_count; ++s) {
	    src_nv = src->item (s);
	    if (src_nv->flags () & f)
		break;
	}
	if (s >= src_count || d >= dst_count) {
	    // assertion fails iff #params-to-copy in dst and src differ.
	    if (!(s >= src_count && d >= dst_count))
                return FALSE;
	    break;
	}
	// make sure we have the same type of args
	if (!((src_nv->flags() & f) == (dst_nv->flags() & f)))
            return FALSE;

	// XXX possibly avoid a copy here ...
	*dst_nv->value() = *src_nv->value();
    }
    return TRUE;
}


/*****************************  Environment  ***************************/


CORBA::Environment::Environment (Principal_ptr pr)
{
    _exceptn = NULL;
    _princ = CORBA::Principal::_duplicate (pr);
}

CORBA::Environment::~Environment ()
{
    if (_exceptn)
	delete _exceptn;
    CORBA::release (_princ);
}

void
CORBA::Environment::exception (Exception *e)
{
    if (_exceptn)
	delete _exceptn;
    _exceptn = e;
}

void
CORBA::Environment::clear ()
{
    MICO_OBJ_CHECK (this);

    if (_exceptn)
	delete _exceptn;
    _exceptn = NULL;
}


/*****************************  Principal  ***************************/


namespace CORBA {
  vector<CORBA::Octet> Principal::_peer_info;
}

CORBA::Principal::Principal (CORBA::Transport *t)
{
    _rep = _peer_info;
    // XXX make sure this isnt released ...
    _transp = t;
}

CORBA::Principal::Principal (DataDecoder &dc, CORBA::Transport *t)
{
    // XXX make sure this isnt released ...
    _transp = t;
    CORBA::Boolean r = decode (dc);
    assert (r);
}

#ifdef USE_CSL2
CORBA::Transport*
CORBA::Principal::transport()
{
	return _transp;
}
#endif /* USE_CSL2  */


CORBA::Principal::~Principal ()
{
}

CORBA::Boolean
CORBA::Principal::operator== (const Principal &p) const
{
    return _rep == p._rep;
}

void
CORBA::Principal::encode (DataEncoder &ec) const
{
    ec.seq_begin (_rep.size());
    {
	if (_rep.size() > 0) {
	    assert (_rep.size() < 2 || &_rep[0] + 1 == &_rep[1]);
	    ec.put_octets (&_rep[0], _rep.size());
	}
    }
    ec.seq_end ();
}

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
CORBA::Principal::decode (DataDecoder &dc)
{
    ULong len;
    check (dc.seq_begin (len));
    {
	_rep.erase (_rep.begin(), _rep.end());
	if (len > 0) {
            if (dc.buffer()->length() < len)
                return FALSE;
	    _rep.insert (_rep.begin(),
			 dc.buffer()->data(),
			 dc.buffer()->data() + len);
	    dc.buffer()->rseek_rel (len);
	}
    }
    check (dc.seq_end ());
    return TRUE;
}

#undef check

CORBA::Principal::PropertyNameList_ptr
CORBA::Principal::list_properties ()
{
    PropertyNameList_ptr pl = new PropertyNameList;
    pl->length (3);

    (*pl)[0] = (const char *)"peer-info";
    (*pl)[1] = (const char *)"auth-method";
    (*pl)[2] = (const char *)"peer-address";
    return pl;
}

#ifdef USE_CSL2
CORBA::Any* CORBA::Principal::get_attribute(unsigned long fam, unsigned long attr) {
  assert (0);
  return (CORBA::Any*)0;
}

CORBA::Boolean CORBA::Principal::check(string& s)
{
	return 0;
}
#endif /* USE_CSL2  */

CORBA::Any*
CORBA::Principal::get_property (const char *prop_name)
{
    if (!strcmp ("peer-info", prop_name)) {
	Any *a = new Any;
        if (_rep.size() > 0) {
            OctetSeq os (_rep.size(), _rep.size(), &_rep[0], FALSE);
            *a <<= os;
        } else {
            *a <<= OctetSeq();
        }
	return a;
    }
    if (!strcmp ("auth-method", prop_name)) {
	Any *a = new Any;
	*a <<= "basic";
	return a;
    }
    if (!strcmp ("peer-address", prop_name)) {
        Any *a = new Any;
        if (_transp) {
            *a <<= _transp->peer()->stringify().c_str();
        } else {
            *a <<= "";
        }
        return a;
    }
    return new Any;
}

void
CORBA::Principal::set_peer_info (const OctetSeq &i)
{
    _peer_info.erase (_peer_info.begin(), _peer_info.end());
    _peer_info.insert (_peer_info.begin(),
		       &i[0], &i[0] + i.length());
}


/*****************************  ContextList  ***************************/


CORBA::ContextList::ContextList ()
{
}

CORBA::ContextList::~ContextList ()
{
}

void
CORBA::ContextList::add (const char *ctxt)
{
    MICO_OBJ_CHECK (this);

    if (!ctxt)
	mico_throw (CORBA::BAD_PARAM());

    _vec.push_back (ctxt);
}

void
CORBA::ContextList::add_consume (char *ctxt)
{
    MICO_OBJ_CHECK (this);

    if (!ctxt)
	mico_throw (CORBA::BAD_PARAM());

    _vec.push_back (ctxt);
    CORBA::string_free (ctxt);
}

const char *
CORBA::ContextList::item (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    return _vec[idx].c_str();
}

void
CORBA::ContextList::remove (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    _vec.erase (_vec.begin() + idx);
}


/*****************************  ExceptionList  ***************************/


CORBA::ExceptionList::ExceptionList ()
{
}

CORBA::ExceptionList::~ExceptionList ()
{
}

void
CORBA::ExceptionList::add (TypeCode_ptr tc)
{
    MICO_OBJ_CHECK (this);

    _vec.push_back (TypeCode::_duplicate (tc));
}

void
CORBA::ExceptionList::add_consume (TypeCode_ptr tc)
{
    MICO_OBJ_CHECK (this);

    _vec.push_back (tc);
}

CORBA::TypeCode_ptr
CORBA::ExceptionList::item (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    return TypeCode::_duplicate (_vec[idx]);
}

void
CORBA::ExceptionList::remove (ULong idx)
{
    MICO_OBJ_CHECK (this);

    if (idx >= _vec.size())
	mico_throw (CORBA::Bounds());
    _vec.erase (_vec.begin() + idx);
}


/*************************  RequestCallback  **************************/


CORBA::RequestCallback::~RequestCallback ()
{
}


/*****************************  Request  ******************************/


CORBA::Request::Request ()
{
    assert (0);
}

CORBA::Request::Request (Object_ptr o, Context_ptr c,
			 const char *op,
			 NVList_ptr argumentsval,
			 NamedValue_ptr resultval,
			 Flags f)
{
    if (CORBA::is_nil (o) || !op)
	mico_throw (CORBA::BAD_PARAM());

    MICO_OBJ_CHECK2 (o, CORBA::OBJECT_NOT_EXIST());

    if (!o->_ior())
	// locality contrained object
	mico_throw (NO_IMPLEMENT());

    _object = Object::_duplicate (o);
    _context = Context::_duplicate (c);
    _opname = op;
    _args = NVList::_duplicate (argumentsval);
    _res = NamedValue::_duplicate (resultval);
    _flags = f;

    _invoke_pending = FALSE;

    ORB_ptr orb = o->_orbnc();

    if (CORBA::is_nil (_args))
	orb->create_list (0, _args);
    if (CORBA::is_nil (_res))
	orb->create_named_value (_res);
    orb->create_environment (_environm);
    // XXX use IR to fill elist/clist
    orb->create_context_list (_clist);
    orb->create_exception_list (_elist);
    _orbreq = new MICO::LocalRequest (this);
    _iceptreq = Interceptor::ClientInterceptor::_create_request
	(_object, _opname, *_orbreq->context(), this);
    _cb = 0;
    _cri = PInterceptor::PI::_create_cri
	(_object, _opname, 0, _args, _elist, _clist, _context, _res, TRUE);
    _invoke_flag = FALSE;
}

CORBA::Request::Request (Object_ptr o,
			 Context_ptr c,
			 const char *op,
			 NVList_ptr argumentsval,
			 NamedValue_ptr resultval,
			 ExceptionList_ptr el,
			 ContextList_ptr cl,
			 Flags f)
{
    if (CORBA::is_nil (o) || !op)
	mico_throw (CORBA::BAD_PARAM());

    MICO_OBJ_CHECK2 (o, CORBA::OBJECT_NOT_EXIST());

    if (!o->_ior())
	// locality contrained object
	mico_throw (NO_IMPLEMENT());

    _object = Object::_duplicate (o);
    _context = Context::_duplicate (c);
    _opname = op;
    _args = NVList::_duplicate (argumentsval);
    _res = NamedValue::_duplicate (resultval);
    _flags = f;
    _elist = ExceptionList::_duplicate (el);
    _clist = ContextList::_duplicate (cl);

    _invoke_pending = FALSE;

    ORB_ptr orb = o->_orbnc();

    if (CORBA::is_nil (_args))
	orb->create_list (0, _args);
    if (CORBA::is_nil (_res))
	orb->create_named_value (_res);
    orb->create_environment (_environm);
    // XXX use IR to fill elist/clist
    if (CORBA::is_nil (_clist))
	orb->create_context_list (_clist);
    if (CORBA::is_nil (_elist))
	orb->create_exception_list (_elist);
    _orbreq = new MICO::LocalRequest (this);
    _iceptreq = Interceptor::ClientInterceptor::_create_request
	(_object, _opname, *_orbreq->context(), this);
    _cb = 0;
    _cri = PInterceptor::PI::_create_cri
	(_object, _opname, 0, _args, _elist, _clist, _context, _res, TRUE);
    _invoke_flag = FALSE;
}

CORBA::Request::Request (Object_ptr o, const char *op)
{
    if (CORBA::is_nil (o) || !op)
	mico_throw (CORBA::BAD_PARAM());

    MICO_OBJ_CHECK2 (o, CORBA::OBJECT_NOT_EXIST());

    if (!o->_ior())
	// locality contrained object
	mico_throw (NO_IMPLEMENT());

    _object = Object::_duplicate (o);
    _opname = op;

    _invoke_pending = FALSE;
	
    ORB_ptr orb = o->_orbnc();

    _context = Context::_nil();
    orb->create_environment (_environm);
    orb->create_list (0, _args);
    orb->create_named_value (_res);
    // XXX use IR to fill elist/clist
    orb->create_context_list (_clist);
    orb->create_exception_list (_elist);
    _orbid = 0;
    _flags = 0;
    _orbreq = new MICO::LocalRequest (this);
    _iceptreq = Interceptor::ClientInterceptor::_create_request
	(_object, _opname, *_orbreq->context(), this);
    _cb = 0;
    _cri = PInterceptor::PI::_create_cri
	(_object, _opname, 0, _args, _elist, _clist, _context, _res, TRUE);
    _invoke_flag = FALSE;
}

CORBA::Request::~Request ()
{
    if (_invoke_pending)
        _object->_orbnc()->cancel (_orbid);
    CORBA::release (_context);
    CORBA::release (_environm);
    CORBA::release (_args);
    CORBA::release (_res);
    CORBA::release (_clist);
    CORBA::release (_elist);
    CORBA::release (_orbreq);
    CORBA::release (_iceptreq);
    CORBA::release (_cri);
}

void
CORBA::Request::decode_user_exception ()
{
  bool known_exception = false;

    Exception *ex = _environm->exception();
    if (ex && _elist->count() > 0) {
	UnknownUserException *uuex =
	  UnknownUserException::_downcast (ex);
	if (uuex) {
	    // decode UnknownUserException using ExceptionList ...
	    const char *repoid = uuex->_except_repoid ();
	    TypeCode_ptr tc;
	    for (CORBA::ULong i = 0; i < _elist->count(); ++i) {
		tc = _elist->item (i);
		if (!strcmp (repoid, tc->id())) {
		    uuex->exception (tc);
		    known_exception = true;
		    break;
		}
	    }
	}
	else {
	    UserException *uex = 
		UserException::_downcast (ex);
	    if (uex) {
		// check UserException using ExceptionList ...
		const char *repoid = uex->_repoid ();
		TypeCode_ptr tc;
		for (CORBA::ULong i = 0; i < _elist->count(); ++i) {
		    tc = _elist->item (i);
		    if (!strcmp (repoid, tc->id())) {
			known_exception = true;
			break;
		    }
		}
	    }
	}
    }

    if (ex && !known_exception) {
      // oops, unknown exception
      CORBA::Exception * nex =
	new CORBA::UNKNOWN (CORBA::OMGVMCID | 1,
			    CORBA::COMPLETED_MAYBE);
      _environm->exception (nex);
    }
}

CORBA::Boolean
CORBA::Request::waitfor (CORBA::ORB_ptr orb, CORBA::ORBMsgId msgid,
			 CORBA::ORBCallback::Event ev, CORBA::Long tmout)
{
#ifdef HAVE_THREADS
    return async_callback.waitfor(orb, msgid, ev, tmout);
#else // HAVE_THREADS
    assert(0);
    return TRUE;
#endif // HAVE_THREADS
}

void
CORBA::Request::notify (ORB_ptr orb, CORBA::ORBMsgId id, ORBCallback::Event ev)
{
    assert (ev == ORBCallback::Invoke);
#ifdef HAVE_THREADS
    async_callback.notify(orb, id, ev);
#endif // HAVE_THREADS
    if (_cb) {
        RequestCallback *tmpcb = _cb;
        _cb = 0;
        tmpcb->callback (this, RequestCallback::RequestDone);
    }
}

void
CORBA::Request::ctx (Context_ptr c)
{
    MICO_OBJ_CHECK (this);

    CORBA::release (_context);
    _context = Context::_duplicate (c);
}

CORBA::Any &
CORBA::Request::add_in_arg ()
{
    return *arguments()->add(ARG_IN)->value();
}

CORBA::Any &
CORBA::Request::add_in_arg (const char *name)
{
    return *arguments()->add_item(name, ARG_IN)->value();
}

CORBA::Any &
CORBA::Request::add_inout_arg ()
{
    return *arguments()->add(ARG_INOUT)->value();
}

CORBA::Any &
CORBA::Request::add_inout_arg (const char *name)
{
    return *arguments()->add_item(name, ARG_INOUT)->value();
}

CORBA::Any &
CORBA::Request::add_out_arg ()
{
    return *arguments()->add(ARG_OUT)->value();
}

CORBA::Any &
CORBA::Request::add_out_arg (const char *name)
{
    return *arguments()->add_item(name, ARG_OUT)->value();
}

void
CORBA::Request::set_return_type (TypeCode_ptr tc)
{
    result()->value()->set_type (tc);
}

CORBA::Any &
CORBA::Request::return_value ()
{
    return *result()->value();
}

void
CORBA::Request::invoke ()
{
    _invoke_flag = TRUE;
    send_deferred ();
    get_response ();
    _invoke_flag = FALSE;
}

void
CORBA::Request::send_oneway ()
{
    MICO_OBJ_CHECK (this);

    if (!Interceptor::ClientInterceptor::
	_exec_initialize_request ((Interceptor::LWRequest_ptr)_iceptreq,
				  _environm)) {
	return;
    }
    ORB_ptr orb = _object->_orbnc();
    try {
	PInterceptor::PI::_send_request_ip
	    (_cri, CORBA::ORB::get_msgid(_orbid), _args, this->contexts(), this->ctx(),
	     _orbreq->context(), FALSE);
    } catch (...) {
	this->env()->exception(_cri->exception()->_clone());
	throw;
    }

    CORBA::ORBMsgId __id = orb->invoke_async
	(_object, _orbreq, Principal::_nil(), FALSE);

//      Corba::ULong mid = orb->invoke_async
//  	(_object, _orbreq, Principal::_nil(), FALSE);
//      if (mid > 0) { // object not exists
    if (__id != NULL) {
	cerr << "dii.cc hack pi...." << endl;
	CORBA::OBJECT_NOT_EXIST ex;
	try {
	    PInterceptor::PI::_receive_exception_ip
		(_cri, PortableInterceptor::SYSTEM_EXCEPTION, ex,
		 _orbreq->context());
	} catch (...) {
	    this->env()->exception(_cri->exception()->_clone());
	    throw;
	}
    }
    else {
	try {
	    PInterceptor::PI::_receive_other_ip(_cri);
	} catch (...) {
	    this->env()->exception(_cri->exception()->_clone());
	    throw;
	}
    }
    Interceptor::ClientInterceptor::
	_exec_after_marshal ((Interceptor::LWRequest_ptr)_iceptreq, _environm);
}

void
CORBA::Request::send_deferred (RequestCallback *cb)
{
    MICO_OBJ_CHECK (this);

    _cb = cb;

    if (!Interceptor::ClientInterceptor::
	_exec_initialize_request ((Interceptor::LWRequest_ptr)_iceptreq,
				  _environm)) {
	if (_cb)
	    _cb->callback (this, RequestCallback::RequestDone);
	_cb = 0;
	return;
    }

    ORB_ptr orb = _object->_orbnc();
    _orbid = orb->new_orbid();
//      orb->invoke_async (_object, _orbreq, Principal::_nil(),
//                         TRUE, _cb ? this : 0, _orbid);

//      _msgid = orb->new_msgid();
    try {
	PInterceptor::PI::_send_request_ip
	    (_cri, CORBA::ORB::get_msgid(_orbid), _args, this->contexts(),
	     this->ctx(), _orbreq->context());
    } catch (...) {
	this->env()->exception(_cri->exception()->_clone());
	throw;
    }
    
    _invoke_pending = TRUE;

    orb->invoke_async(_object, _orbreq, Principal::_nil(),
		      TRUE, _cb ? this : 0, _orbid);
	
    if (_invoke_pending && !Interceptor::ClientInterceptor::
	_exec_after_marshal ((Interceptor::LWRequest_ptr)_iceptreq,
			     _environm)) {
	orb->cancel (_orbid);
	_invoke_pending = FALSE;
	if (_cb)
	    _cb->callback (this, RequestCallback::RequestDone);
	_cb = 0;
	return;
    }
}

void
CORBA::Request::get_response (Boolean block)
{
    MICO_OBJ_CHECK (this);

    if (!_invoke_pending)
	return;

    ORB_ptr orb = _object->_orbnc();
    InvokeStatus rs = InvokeOk;

    while (_invoke_pending) {
	Object_var nobj;
	ORBRequest *dummy;

	if (block) {
	    CORBA::Boolean r = orb->wait (_orbid);
	    assert (r);
	} else {
	    if (!orb->wait (_orbid, 0))
		return;
	}

	// XXX called multiple times in case of Forward
	if (!Interceptor::ClientInterceptor::
	    _exec_before_unmarshal ((Interceptor::LWRequest_ptr)_iceptreq,
				    _environm)) {
	    orb->cancel (_orbid);
	    _invoke_pending = FALSE;
	    _cb = 0;
	    return;
	}

	GIOP::AddressingDisposition ad;
	rs = orb->get_invoke_reply (_orbid, nobj, dummy, ad);
//  	rs = orb->get_invoke_reply (_msgid, nobj, dummy, ad);
	
	switch (rs) {
	case InvokeForward:
	    // XXX what if 'object' is not a stub ???
	    assert (_object.in() && nobj.in());
	    _object->_forward (nobj);
	    try {
		PInterceptor::PI::_receive_other_ip
		    (_cri, PortableInterceptor::LOCATION_FORWARD, _object,
		     dummy->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		throw;
	    }
	    CORBA::release(_cri);
	    //_msgid = orb->new_msgid();
	    _orbid = orb->new_orbid();
	    _cri = PInterceptor::PI::_create_cri
		(_object, _opname, _elist, _res);
	    try {
		PInterceptor::PI::_send_request_ip
		    (_cri, CORBA::ORB::get_msgid(_orbid), _args, this->contexts(), this->ctx(),
		     _orbreq->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		_invoke_pending = FALSE;
		throw;
	    }
	    //_msgid = orb->invoke_async (_object, _orbreq, Principal::_nil(),
	    //TRUE, 0, _msgid);
	    _orbid = orb->invoke_async (_object, _orbreq, Principal::_nil(),
					TRUE, 0, _orbid);
	    break;

	case InvokeOk:
	    try {
		PInterceptor::PI::_receive_reply_ip
		    (_cri, *_res->value(), _args, dummy->context(), TRUE);
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		_invoke_pending = FALSE;
		throw;
	    }
	    _invoke_pending = FALSE;
	    break;

	case CORBA::InvokeSysEx:
	    try {
		PInterceptor::PI::_receive_exception_ip
		    (_cri, PortableInterceptor::SYSTEM_EXCEPTION,
		     _environm->exception(), dummy->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		_invoke_pending = FALSE;
		throw;
	    }
	    if (_object->_is_forwarded()) {
		/*
		 * [15-44] says:
		 * "the only object address a client should
		 *  expect to continue working reliably is
		 *  the initial unforwarded address".
		 *
		 * Therefore when a system exception is raised
		 * during an invocation on the forwarded address
		 * and the invocation is not completed we fall
		 * back to the initial address and retry the
		 * invocation.
		 */
		CORBA::SystemException *sysex =
		    CORBA::SystemException::_downcast (env()->exception());
		assert (sysex);
		if (sysex->completed() == CORBA::COMPLETED_NO &&
                    (CORBA::COMM_FAILURE::_downcast (sysex) ||
                     CORBA::TRANSIENT::_downcast (sysex) ||
                     CORBA::OBJECT_NOT_EXIST::_downcast (sysex))) {
		    _object->_unforward();
		    env()->clear();
		    CORBA::release (_cri);
		    //_msgid = orb->new_msgid();
		    if (_orbid != NULL)
			delete _orbid;
		    _orbid = orb->new_orbid();
		    _cri = PInterceptor::PI::_create_cri
			(_object, _opname, _elist, _res);
		    try {
			PInterceptor::PI::_send_request_ip
			    (_cri, CORBA::ORB::get_msgid(_orbid), _args, _clist,
			     _context, _orbreq->context());
		    } catch (...) {
			this->env()->exception(_cri->exception()->_clone());
			_invoke_pending = FALSE;
			throw;
		    }
		    //_msgid = orb->invoke_async
		    //(_object, _orbreq, Principal::_nil(), TRUE, 0, _msgid);
		    _orbid = orb->invoke_async
			(_object, _orbreq, Principal::_nil(), TRUE, 0, _orbid);
		    break;
		}
	    }
	    _invoke_pending = FALSE;
	    break;

	case InvokeUsrEx:
	    decode_user_exception ();
	    try {
		PInterceptor::PI::_receive_exception_ip
		    (_cri, PortableInterceptor::USER_EXCEPTION,
		     _environm->exception(), dummy->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		_invoke_pending = FALSE;
		throw;
	    }
	    _invoke_pending = FALSE;
	    break;

	case InvokeAddrDisp:
	    _object->_ior_fwd()->addressing_disposition (ad);
	    try {
		PInterceptor::PI::_receive_other_ip
		    (_cri, PortableInterceptor::TRANSPORT_RETRY, dummy->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		throw;
	    }
	    CORBA::release (_cri);
	    //_msgid = orb->new_msgid();
	    if (_orbid != NULL)
		delete _orbid;
	    _orbid = orb->new_orbid();
	    _cri = PInterceptor::PI::_create_cri
		(_object, _opname, _elist, _res);
	    try {
		PInterceptor::PI::_send_request_ip
		    (_cri, CORBA::ORB::get_msgid(_orbid), _args, _clist,
		     _context, _orbreq->context());
	    } catch (...) {
		this->env()->exception(_cri->exception()->_clone());
		throw;
	    }
	    //_msgid = orb->invoke_async (_object, _orbreq, Principal::_nil(),
	    //TRUE, 0, _msgid);
	    _orbid = orb->invoke_async(_object, _orbreq, Principal::_nil(),
				       TRUE, 0, _orbid);
	    break;

	default:
	    assert (0);
	}
    }

    if (!Interceptor::ClientInterceptor::
	_exec_finish_request ((Interceptor::LWRequest_ptr)_iceptreq,
			      _environm)) {
	_cb = 0;
	return;
    }

    _cb = 0;

    if (!_invoke_pending && rs == CORBA::InvokeSysEx)
	env()->exception()->_raise();
}

CORBA::Boolean
CORBA::Request::poll_response ()
{
    // XXX call get_response() to check for redirections...
    this->get_response (FALSE);
    return !_invoke_pending;
}


/************************* LocalRequest ******************************/


MICO::LocalRequest::LocalRequest ()
    : _req (CORBA::Request::_nil()), _have_except (FALSE), _have_result (FALSE)
{
}

MICO::LocalRequest::LocalRequest (CORBA::Request_ptr r)
    : _req (r), _have_except (FALSE), _have_result (FALSE)
{
}

MICO::LocalRequest::~LocalRequest ()
{
}

const char *
MICO::LocalRequest::op_name()
{
    return _req->operation();
}

CORBA::Boolean
MICO::LocalRequest::get_in_args (CORBA::NVList_ptr iparams,
				 CORBA::Context_ptr &ctx)
{
    if (!iparams->copy (_req->arguments(), CORBA::ARG_IN|CORBA::ARG_INOUT))
        return FALSE;
    ctx = CORBA::Context::_duplicate (_req->ctx());
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::get_in_args (StaticAnyList *iparams,
				 CORBA::Context_ptr &ctx)
{
    CORBA::NVList_ptr args = _req->arguments();

    if (args->count() != iparams->size())
	return FALSE;

    CORBA::NamedValue_ptr nv;
    for (mico_vec_size_type i0 = 0; i0 < iparams->size(); ++i0) {
	nv = args->item(i0);
	if ((*iparams)[i0]->flags() != nv->flags())
	    return FALSE;
	if ((*iparams)[i0]->flags() & (CORBA::ARG_IN|CORBA::ARG_INOUT)) {
	    if (!nv->value()->to_static_any (*(*iparams)[i0]))
		return FALSE;
	}
    }
    ctx = CORBA::Context::_duplicate (_req->ctx());
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::get_in_args (CORBA::DataEncoder *ec)
{
    // share one state for all arguments
    CORBA::DataEncoder::ValueState vstate;
    ec->valuestate (&vstate, FALSE);

    CORBA::NVList_ptr nv = _req->arguments();
    for (CORBA::ULong i = 0; i < nv->count(); ++i) {
	if (nv->item(i)->flags() & (CORBA::ARG_IN|CORBA::ARG_INOUT)) {
	    if (!nv->item(i)->value()->marshal (*ec))
		return FALSE;
	}
    }
    if (!CORBA::is_nil (_req->ctx())) {
	ec->put_context (*_req->ctx(), _req->contexts());
    }
    ec->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::get_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams,
				  CORBA::Exception *&except)
{
    if (_have_except) {
	CORBA::Exception *e = _req->env()->exception();
	assert (e);
	except = e->_clone ();
    } else {
	except = 0;
	if (_have_result && res)
	    *res = *_req->result()->value();
	if (!oparams->copy (_req->arguments(), CORBA::ARG_OUT|CORBA::ARG_INOUT))
            return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::get_out_args (CORBA::StaticAny *res,
				  StaticAnyList *oparams,
				  CORBA::Exception *&except)
{
    if (_have_except) {
	CORBA::Exception *e = _req->env()->exception();
	assert (e);
	except = e->_clone ();
	return TRUE;
    }

    except = 0;

    CORBA::NVList_ptr args = _req->arguments();

    if (args->count() != oparams->size())
	return FALSE;

    if (_have_result && res) {
	if (!_req->result()->value()->to_static_any (*res))
	    return FALSE;
    }

    CORBA::NamedValue_ptr nv;
    for (CORBA::ULong i0 = 0; i0 < args->count(); ++i0) {
	nv = args->item(i0);
	if ((*oparams)[i0]->flags() != nv->flags())
	    return FALSE;
	if ((*oparams)[i0]->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
	    if (!nv->value()->to_static_any (*(*oparams)[i0]))
		return FALSE;
	}
    }
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::get_out_args (CORBA::DataEncoder *ec,
				  CORBA::Boolean &is_except)
{
    // share one state for all arguments
    CORBA::DataEncoder::ValueState vstate;
    ec->valuestate (&vstate, FALSE);

    if (_have_except) {
	is_except = TRUE;
	CORBA::Exception *e = _req->env()->exception();
	assert (e);
	e->_encode (*ec);
    } else {
	is_except = FALSE;
	if (_have_result) {
	    if (!_req->result()->value()->marshal (*ec))
		return FALSE;
	}
	CORBA::NVList_ptr nv = _req->arguments();
	for (CORBA::ULong i = 0; i < nv->count(); ++i) {
	    if (nv->item(i)->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
		if (!nv->item(i)->value()->marshal (*ec))
		    return FALSE;
	    }
	}
    }
    ec->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::set_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams)
{
    _have_except = FALSE;
    if (res) {
	_have_result = TRUE;
	*_req->result()->value() = *res;
    }
    return _req->arguments()->copy (oparams, CORBA::ARG_OUT|CORBA::ARG_INOUT);
}

CORBA::Boolean
MICO::LocalRequest::set_out_args (CORBA::StaticAny *res,
				  StaticAnyList *oparams)
{
    CORBA::NVList_ptr args = _req->arguments();

    if (args->count() != oparams->size())
	return FALSE;

    _have_except = FALSE;
    _have_result = !!res;
    if (res)
	_req->result()->value()->from_static_any (*res);

    CORBA::NamedValue_ptr nv;
    for (mico_vec_size_type i0 = 0; i0 < oparams->size(); ++i0) {
	nv = args->item(i0);
	if ((*oparams)[i0]->flags() != nv->flags())
	    return FALSE;
	if ((*oparams)[i0]->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
	    nv->value()->from_static_any (*(*oparams)[i0]);
	}
    }
    return TRUE;
}

void
MICO::LocalRequest::set_out_args (CORBA::Exception *except)
{
    _have_except = TRUE;
    _have_result = FALSE;
    _req->env()->exception (except->_clone());
}

CORBA::Boolean
MICO::LocalRequest::set_out_args (CORBA::DataDecoder *dc, CORBA::Boolean is_ex)
{
    CORBA::Boolean ret;
    CORBA::TypeCode_var tc;

    // share one state for all arguments
    CORBA::DataDecoder::ValueState vstate;
    dc->valuestate (&vstate, FALSE);

    if (is_ex) {
	_have_except = TRUE;
	_have_result = FALSE;
	CORBA::Exception *e = CORBA::Exception::_decode (*dc);
	assert (e);
	_req->env()->exception (e);
    } else {
	_have_except = FALSE;
	_have_result = TRUE;

	tc = _req->result()->value()->type();
	ret = _req->result()->value()->demarshal (tc, *dc);
	if (!ret) {
	    dc->valuestate (0);
            return FALSE;
	}

	CORBA::NVList_ptr nv = _req->arguments();
	for (CORBA::ULong i = 0; i < nv->count(); ++i) {
	    if (nv->item(i)->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
		tc = nv->item(i)->value()->type();
		ret = nv->item(i)->value()->demarshal (tc, *dc);
                if (!ret) {
		    dc->valuestate (0);
		    return FALSE;
		}
	    }
	}
    }
    dc->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::copy_out_args (CORBA::ORBRequest *r)
{
    if (this == r)
	return TRUE;

    // copy service context list
    copy_svc (r);

    CORBA::Exception *ex = 0;
    if (!r->get_out_args (_req->result()->value(), _req->arguments(), ex))
        return FALSE;
    if (ex) {
	_have_except = TRUE;
	_have_result = FALSE;
	_req->env()->exception (ex);
    } else {
	_have_result = TRUE;
	_have_except = FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::LocalRequest::copy_in_args (CORBA::ORBRequest *req)
{
    assert (0);
    return FALSE;
}

const char *
MICO::LocalRequest::type ()
{
    return "local";
}

CORBA::Request_ptr
MICO::LocalRequest::request ()
{
    return _req;
}


