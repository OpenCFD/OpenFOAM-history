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
#define MICO_CONF_IR
#define MICO_CONF_INTERCEPT
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/intercept.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/pi_impl.h>

#endif // FAST_PCH


using namespace std;


/************************** ServerRequestBase ***************************/


CORBA::ServerRequestBase::~ServerRequestBase ()
{
}


/***************************** ServerRequest ***************************/


CORBA::ServerRequest::ServerRequest (ORBRequest *r, Object_ptr o,
                                     CORBA::ORBMsgId id, CORBA::ObjectAdapter *oa,
				     CORBA::Principal_ptr pr)
    : _oa (oa), _id (id),
      _obj(Object::_duplicate (o)),
      _req(ORBRequest::_duplicate (r)),
      _env (pr)
{
    _aborted = FALSE;
    _dir_params = NVList::_nil();
    _context = Context::_nil();
    _iceptreq = Interceptor::ServerInterceptor::_create_request (
	_obj, r->op_name(), *r->context(), this);
}

CORBA::ServerRequest::~ServerRequest ()
{
    if (!_canceled) {
        // clear service context list
        _req->context()->length (0);

        if (!_aborted && !Interceptor::ServerInterceptor::
            _exec_before_marshal ((Interceptor::LWServerRequest_ptr)_iceptreq,
                                  &_env)) {
            _aborted = TRUE;
        }

        // copy back out args and result into ORBRequest
        set_out_args();

        // tell OA we are done
        CORBA::InvokeStatus stat = CORBA::InvokeOk;
        if (exception()) {
            if (CORBA::SystemException::_downcast (exception())) {
                stat = CORBA::InvokeSysEx;
            } else {
                stat = CORBA::InvokeUsrEx;
            }
        }
	CORBA::ORBInvokeRec* rec = _id;
	if (rec) {
	    PInterceptor::PI::_set_sri_exception
		(rec->requestinfo(), this->exception());
	}
        
        //_oa->answer_invoke (_msgid, _obj, _req, stat);
        _oa->answer_invoke (_id, _obj, _req, stat);

        if (!_aborted && !Interceptor::ServerInterceptor::
            _exec_finish_request ((Interceptor::LWServerRequest_ptr)_iceptreq,
                                  &_env)) {
            // XXX how to abort the request here???
        }
    }

    CORBA::release (_dir_params);
    CORBA::release (_context);
    CORBA::release (_iceptreq);
    CORBA::release (_obj);
    CORBA::release (_req);
}

// begin CORBA 2.2 mapping [20.26.1]
const char *
CORBA::ServerRequest::operation ()
{
  return _req->op_name();
}

void
CORBA::ServerRequest::arguments (NVList_ptr & p)
{
  params (p);
}

void
CORBA::ServerRequest::set_result (const Any & val)
{
  _res = new CORBA::Any (val);
}

void
CORBA::ServerRequest::set_exception (const Any & val)
{
  _res = 0;
  _env.exception (Exception::_decode ((Any &) val));
  assert (_env.exception());
}
// end CORBA 2.2 mapping; ctx () as below

const char *
CORBA::ServerRequest::op_name ()
{
    return _req->op_name();
}

CORBA::OperationDef_ptr
CORBA::ServerRequest::op_def ()
{
    InterfaceDef_var iface = _obj->_get_interface ();
    if (CORBA::is_nil (iface))
	return CORBA::OperationDef::_nil();

    Contained_var cont = iface->lookup ((char *)op_name());
    if (CORBA::is_nil (cont))
	return CORBA::OperationDef::_nil();

    OperationDef_ptr opdef = OperationDef::_narrow (cont);
    return opdef;
}

CORBA::Context_ptr
CORBA::ServerRequest::ctx ()
{
    assert (!CORBA::is_nil (_dir_params));
    return _context;
}

CORBA::Boolean
CORBA::ServerRequest::params (NVList_ptr p)
{
    assert (CORBA::is_nil (_dir_params));
    _dir_params = p;

    if (!Interceptor::ServerInterceptor::
	_exec_initialize_request ((Interceptor::LWServerRequest_ptr)_iceptreq,
				  &_env)) {
	_aborted = TRUE;
	return FALSE;
    }

    if (!_req->get_in_args (_dir_params, _context)) {
      if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Warning)
	  << "Warning: cannot decode args in ServerRequest::params" << endl;
      }
      exception (new CORBA::MARSHAL());
      _aborted = TRUE;
      return FALSE;
    }

    if (!Interceptor::ServerInterceptor::
	_exec_after_unmarshal ((Interceptor::LWServerRequest_ptr)_iceptreq,
			       &_env)) {
	_aborted = TRUE;
	return FALSE;
    }

    CORBA::ORB_ptr orb = _obj->_orbnc();
    CORBA::ORBInvokeRec* rec = _id;
    if (!rec)
    	return TRUE;
    PInterceptor::ServerRequestInfo_impl* sri = rec->requestinfo();
    if (!sri)
	return TRUE;
    // point of receive_request call
    try {
	PInterceptor::PI::_receive_request_ip(sri, _dir_params, _context, _oa, _obj);
    } catch (PortableInterceptor::ForwardRequest_catch& exc) {
	_obj = Object::_duplicate(exc->forward);
	orb->answer_invoke(rec, CORBA::InvokeForward, exc->forward, _req, 0);
    } catch (const CORBA::Exception& ex) {
	this->exception(ex._clone());
	_aborted = TRUE;
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	this->exception(ex._clone());
	_aborted = TRUE;
	throw;
    }
    return TRUE;
}

void
CORBA::ServerRequest::result (Any *val)
{
    _env.clear ();
    _res = val;
}

CORBA::Environment_ptr
CORBA::ServerRequest::environment ()
{
    return &_env;
}

void
CORBA::ServerRequest::exception (Any *val)
{
    _res = 0;
    _env.exception (Exception::_decode (*val));
    assert (_env.exception());
    delete val;
}

void
CORBA::ServerRequest::exception (Exception *except)
{
    _env.exception (except);
}

CORBA::Exception *
CORBA::ServerRequest::exception ()
{
    // maybe _nil(), overship maintained by ServerRequest
    return _env.exception();
}

CORBA::Any *
CORBA::ServerRequest::result ()
{
    // maybe 0, overship maintained by ServerRequest
    return _res;
}

CORBA::NVList_ptr
CORBA::ServerRequest::arguments ()
{
    // maybe _nil(), overship maintained by ServerRequest
    return _dir_params;
}

void
CORBA::ServerRequest::set_out_args ()
{

    if (_env.exception()) {
        _req->set_out_args (_env.exception());
    } 
    else if (!CORBA::is_nil (_dir_params)) {
	CORBA::ORBInvokeRec* rec = _id;
	if (rec) {
	    PInterceptor::ServerRequestInfo_impl* sri = rec->requestinfo();
	    CORBA::TypeCode_var tc;
	    if (_res && (tc = _res->type())) {
		if (tc->kind() != CORBA::tk_void 
		    && tc->kind() != CORBA::tk_null) {
		    PInterceptor::PI::_set_sri_result(sri, *_res);
		}
	    }
	    PInterceptor::PI::_set_sri_args(sri, _dir_params, TRUE);
	}

	if (!_req->set_out_args (_res, _dir_params)) {
	    if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
		MICO::Logger::Stream (MICO::Logger::Warning)
		    << "Warning: cannot ServerRequest::set_out_args" << endl;
	    }
	    CORBA::MARSHAL ex (0, CORBA::COMPLETED_YES);
	    _req->set_out_args (&ex);
        }
    }
}

CORBA::ORBRequest *
CORBA::ServerRequest::request ()
{
    return _req;
}
