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

#include <CORBA.h>
#ifndef _WIN32
#include <string.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/ssl.h>
#ifdef USE_WIRELESS
#include <mico/watm.h>
#endif
#ifdef HAVE_BLUETOOTH
#include <mico/ltp.h>
#endif
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/poa_impl.h>
#include <mico/dynany_impl.h>
#include <mico/pi_impl.h>

#ifdef USE_CSL2
#  include <mico/security/securitylevel1.h>
#  include <mico/security/securitylevel2_impl.h>
#  include <mico/security/AuditIntercept.h>
#  include <mico/security/odm_impl.h>
#  include <mico/security/DomainManager_impl.h>
#endif // USE_CSL2

#ifdef USE_CSIV2
#include <mico/security/csiv2_impl.h>
#endif // USE_CSIV2

#if defined (_WIN32) || defined (_POCKET_PC)
#include <mico/process_impl.h>
#endif
#include <mico/throw.h>
#include <algorithm>
#ifdef USE_MEMTRACE
#include <mico/memtrace.h>
#endif

#endif // FAST_PCH

#ifdef _POCKET_PC
extern char* getenv (const char*);
#endif


using namespace std;

/************************** misc dtors ****************************/


CORBA::ObjectAdapter::~ObjectAdapter ()
{
}

CORBA::ORBCallback::~ORBCallback ()
{
}

#ifdef HAVE_THREADS

CORBA::ORBAsyncCallback::~ORBAsyncCallback ()
{
}

CORBA::Boolean
CORBA::ORBAsyncCallback::waitfor (CORBA::ORB_ptr orb,
				  CORBA::ORBMsgId msgid, 
				  Event ev,
				  CORBA::Long tmout) {
#ifdef MTDEBUG
    MICO_Long stime = OSMisc::timestamp();
#endif // MTDEBUG
    MICOMT::AutoLock l(cond_mutex);

    if (notified) return TRUE;
    if ( tmout != -1 ) {
	cond.timedwait(tmout);
    } else {
	cond.wait();
    }
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged (MICO::Logger::ORB)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::ORB)
	    << "time for wait: " << OSMisc::timestamp() - stime << " ms" << endl;
    }
#endif // MTDEBUG
    return notified;
}

#endif // HAVE_THREADS

/************************** ORBRequest ****************************/


CORBA::ORBRequest::~ORBRequest ()
{
}


/************************* ORBInvokeRecord ***************************/


CORBA::ORBInvokeRec::ORBInvokeRec (MsgId id)
{
    _have_result = FALSE;
    _type = RequestUnknown;
    _myid = id;
    _obj = 0;
    _target = 0;
    _principal = 0;
    _adapter = 0;
    _req = 0;
    _cb = 0;
    _req_hint = 0;
    _inv_hint = 0;
    _active = TRUE;
    _sri = 0;
}

void
CORBA::ORBInvokeRec::init_invoke (ORB_ptr orb,
				  Object_ptr o, ORBRequest *r,
				  Principal_ptr pr,
				  Boolean response,
				  ORBCallback *callback, ObjectAdapter *oa)
{
    _have_result = FALSE;
    _type = RequestInvoke;

    _orb = orb;
    _obj = Object::_nil();
    _target = Object::_duplicate (o);
    _req = CORBA::ORBRequest::_duplicate (r);
    _principal = Principal::_duplicate (pr);
    _response_expected = response;
    _addr = 0;
    _adapter = oa;
    _cb = callback;
    _active = TRUE;
    //
    // kcg: we need to create server request info only when object's oa
    // is local - so it's either BOA or POA. Otherwise (IIOPServer) we have
    // to run w/o any PI invocation
    //
    if (oa && oa->is_local()) {
	_sri = PInterceptor::PI::_create_sri
	    (o, r->op_name(), _myid, response, r->context());
	// we need already initialized reply service context list here
	// because we can call add_reply_service_context from interceptor's
	// receive_request_service_context
	// put receive_request_service_contexts here
	try {
	    PInterceptor::PI::_receive_request_service_contexts_ip
		(_sri, r->context(), _adapter);
	} catch (CORBA::SystemException_catch& ex) {
	    r->set_out_args(ex->_clone());
	    _orb->answer_invoke(this, CORBA::InvokeSysEx, _obj, r, 0);
	    throw;
	} catch (PortableInterceptor::ForwardRequest_catch& exc) {
	    _orb->answer_invoke(this, CORBA::InvokeForward, exc->forward,
				r, 0);
	    throw;
	} catch (CORBA::UserException& ex) {
	    r->set_out_args(ex._clone());
	    _orb->answer_invoke(this, CORBA::InvokeUsrEx, _obj, r, 0);
	    throw;
	} catch (...) {
	    CORBA::UNKNOWN ex;
	    r->set_out_args(ex._clone());
	    _orb->answer_invoke(this, CORBA::InvokeSysEx, _obj, r, 0);
	    throw ex;
	}
    }
    else {
	_sri = 0;
    }
}

void
CORBA::ORBInvokeRec::init_bind (ORB_ptr orb, const char *repo,
				const OctetSeq &tag, Address *a,
				ORBCallback *callback, ObjectAdapter *oa)
{
    _have_result = FALSE;
    _type = RequestBind;
    
    _orb = orb;
    _obj = Object::_nil();
    _target = Object::_nil();
    _req = CORBA::ORBRequest::_nil();
    _principal = Principal::_nil();
    _response_expected = TRUE;
    _repoid = repo;
    _addr = a;
    _adapter = oa;
    _cb = callback;
    _objtag = tag;
    _active = TRUE;
}

void
CORBA::ORBInvokeRec::init_locate (ORB_ptr orb, Object_ptr o,
				  ORBCallback *callback, ObjectAdapter *oa)
{
    _have_result = FALSE;
    _type = RequestLocate;
    
    _orb = orb;
    _obj = Object::_nil();
    _target = Object::_duplicate (o);
    _req = CORBA::ORBRequest::_nil();
    _principal = Principal::_nil();
    _response_expected = TRUE;
    _addr = 0;
    _adapter = oa;
    _cb = callback;
    _active = TRUE;
}

CORBA::ORBInvokeRec::~ORBInvokeRec ()
{
    // if caller needs the object then he must _duplicate() it
    CORBA::release (_obj);
    CORBA::release (_target);
    CORBA::release (_principal);
    CORBA::release (_req);
    CORBA::release (_sri);
#ifdef HAVE_THREADS
    CORBA::ORBAsyncCallback* tmp_cb = dynamic_cast<CORBA::ORBAsyncCallback*>(_cb);
    if (tmp_cb != NULL) {
	delete _cb;
    }
#endif
}

void
CORBA::ORBInvokeRec::free ()
{
    CORBA::release (_obj);
    CORBA::release (_target);
    CORBA::release (_principal);
    CORBA::release (_req);
    CORBA::release (_sri);

    if (_cb)
	delete _cb;
    _have_result = FALSE;
    _type = RequestUnknown;
    _obj = 0;
    _target = 0;
    _principal = 0;
    _req = 0;
    _cb = 0;
    _active = FALSE;
    _sri = 0;
}

void
CORBA::ORBInvokeRec::redo ()
{
    assert (_adapter);
    assert (!_have_result);

    switch (_type) {
    case RequestInvoke:
        // fallback to initial location ...
        _target->_unforward();
	_adapter->invoke (this, _target, _req, _principal,
			  _response_expected);
	break;

    case RequestLocate:
	_adapter->locate (this, _target);
	break;

    case RequestBind:
	_adapter->bind (this, _repoid.c_str(), _objtag, _addr);
	break;

    default:
        assert (0);
    }
}

void
CORBA::ORBInvokeRec::set_answer_invoke (InvokeStatus state,
					Object_ptr o,
					ORBRequest *r,
					GIOP::AddressingDisposition ad)
{
    assert (_type == RequestInvoke);
    assert (!_have_result);

    _invoke_stat = state;
    if (!_sri) {
	switch (state) {
	case InvokeOk:
	case InvokeUsrEx:
	case InvokeSysEx:
	    if (!_req->copy_out_args (r)) {
		CORBA::MARSHAL ex;
		_req->set_out_args (&ex);
		_invoke_stat = InvokeSysEx;
	    }
	    break;

	case InvokeForward:
	    _obj = Object::_duplicate (o);
	    break;
	    
	case InvokeAddrDisp:
	    _ad = ad;
	    break;

	default:
	    assert (0);
	}
	_have_result = TRUE;
	return;
    }
    // I've commented it out because we need initialized reply service 
    // context more early in init_invoke, where we call 
    // receive_request_service_context on interceptor PI spec said that
    // in receive_request_service_context is add_reply_service_context valid
    // operation so we need service context list already initialized
    // before calling any receive_request_service_context
    // such that I've done this change
    //     	sri->set_reply_service_context_list(r->context());
    if (state == InvokeOk || state == InvokeUsrEx || state == InvokeSysEx) {
	if (!_req->copy_out_args (r)) {
	    CORBA::MARSHAL ex;
	    _req->set_out_args (&ex);
	    _invoke_stat = InvokeSysEx;
	    PInterceptor::PI::_set_sri_exception(_sri, &ex);
	}
    }
    switch (state) {
    case InvokeOk:
    	// send_reply
    	try {
	    PInterceptor::PI::_send_reply_ip(_sri);
    	} catch (CORBA::SystemException_catch& ex) {
	    r->set_out_args(ex->_clone());
	    _invoke_stat = InvokeSysEx;
	} catch (CORBA::UserException& ex) {
	    r->set_out_args(ex._clone());
	    _invoke_stat = InvokeUsrEx;
	} catch (...) {
	    CORBA::UNKNOWN ex;
	    r->set_out_args(ex._clone());
	    _invoke_stat = InvokeSysEx;
	}
	break;

    case InvokeUsrEx:
    	// send_exception
    	if (!_sri->own_exception()) {
	    try {
		PInterceptor::PI::_send_exception_ip
		    (_sri, PortableInterceptor::USER_EXCEPTION);
	    } catch (CORBA::SystemException_catch& ex) {
		r->set_out_args(ex->_clone());
		_invoke_stat = InvokeSysEx;
	    } catch (PortableInterceptor::ForwardRequest_catch& exc) {
		_obj = Object::_duplicate(exc->forward);
		_invoke_stat = InvokeForward;
	    } catch (CORBA::UserException& ex) {
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeUsrEx;
	    } catch (...) {
		CORBA::UNKNOWN ex;
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeSysEx;
	    }
	}
	break;

    case InvokeSysEx:
    	// send_exception
    	if (!_sri->own_exception()) {
	    try {
		PInterceptor::PI::_send_exception_ip
		    (_sri, PortableInterceptor::SYSTEM_EXCEPTION);
	    } catch (CORBA::SystemException_catch& ex) {
		r->set_out_args(ex->_clone());
		_invoke_stat = InvokeSysEx;
	    } catch (PortableInterceptor::ForwardRequest_catch& exc) {
		_obj = Object::_duplicate(exc->forward);
		_invoke_stat = InvokeForward;
	    } catch (CORBA::UserException& ex) {
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeUsrEx;
	    } catch (...) {
		CORBA::UNKNOWN ex;
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeSysEx;
	    }
    	}
	break;

    case InvokeForward:
	_obj = Object::_duplicate (o);
	// send_other
    	if (!_sri->own_exception()) {
	    try {
		PInterceptor::PI::_send_other_ip
		    (_sri, PortableInterceptor::LOCATION_FORWARD, _obj);
	    } catch (CORBA::SystemException_catch& ex) {
		r->set_out_args(ex->_clone());
		_invoke_stat = InvokeSysEx;
	    } catch (PortableInterceptor::ForwardRequest_catch& exc) {
		_obj = Object::_duplicate(exc->forward);
		_invoke_stat = InvokeForward;
	    } catch (CORBA::UserException& ex) {
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeUsrEx;
	    } catch (...) {
		CORBA::UNKNOWN ex;
		r->set_out_args(ex._clone());
		_invoke_stat = InvokeSysEx;
	    }
	}
	break;

    case InvokeAddrDisp:
        _ad = ad;
        // send_other
	try {
	    PInterceptor::PI::_send_other_ip
		(_sri, PortableInterceptor::TRANSPORT_RETRY);
	} catch (CORBA::SystemException_catch& ex) {
	    r->set_out_args(ex->_clone());
	    _invoke_stat = InvokeSysEx;
	} catch (PortableInterceptor::ForwardRequest_catch& exc) {
	    _obj = Object::_duplicate(exc->forward);
	    _invoke_stat = InvokeForward;
	} catch (CORBA::UserException& ex) {
	    r->set_out_args(ex._clone());
	    _invoke_stat = InvokeUsrEx;
	} catch (...) {
	    CORBA::UNKNOWN ex;
	    r->set_out_args(ex._clone());
	    _invoke_stat = InvokeSysEx;
	}
	break;

    default:
	assert (0);
    }
    _have_result = TRUE;
}

void
CORBA::ORBInvokeRec::set_answer_bind (LocateStatus state, Object_ptr o)
{
    assert (_type == RequestBind);
    assert (!_have_result);

    _locate_stat = state;

    switch (state) {
    case LocateForward:
    case LocateUnknown:
	break;

    case LocateHere:
	_obj = Object::_duplicate (o);
	break;

    default:
	assert (0);
    }
    _have_result = TRUE;
}

void
CORBA::ORBInvokeRec::set_answer_locate (LocateStatus state, Object_ptr o,
					GIOP::AddressingDisposition ad)
{
    assert (_type == RequestLocate);
    assert (!_have_result);

    _locate_stat = state;

    switch (state) {
    case LocateHere:
    case LocateUnknown:
	break;

    case LocateForward:
	_obj = Object::_duplicate (o);
	break;

    case LocateAddrDisp:
        _ad = ad;
	break;

    default:
	assert (0);
    }
    _have_result = TRUE;
}

CORBA::Boolean
CORBA::ORBInvokeRec::get_answer_invoke (InvokeStatus &state, Object_ptr &o,
					ORBRequest *&r,
					GIOP::AddressingDisposition &ad)
{
    assert (_type == RequestInvoke);
    if (!_have_result)
        return FALSE;

    o = _obj;
    state = _invoke_stat;
    r = _req;
    ad = _ad;
    return TRUE;
}

CORBA::Boolean
CORBA::ORBInvokeRec::get_answer_bind (LocateStatus &state, Object_ptr &o)
{
    assert (_type == RequestBind);
    if (!_have_result)
        return FALSE;

    o = _obj;
    state = _locate_stat;
    return TRUE;
}

CORBA::Boolean
CORBA::ORBInvokeRec::get_answer_locate (LocateStatus &state, Object_ptr &o,
					GIOP::AddressingDisposition &ad)
{
    assert (_type == RequestLocate);
    if (!_have_result)
        return FALSE;

    o = _obj;
    state = _locate_stat;
    ad = _ad;
    return TRUE;
}


/**************************** ORB *********************************/


static CORBA::ORB_ptr orb_instance = CORBA::ORB::_nil();
static MICO::IIOPProxy *iiop_proxy_instance = 0;
static MICO::IIOPServer* iiop_server_instance = 0;

CORBA::ORB::ORB (int &argc, char **argv, const char *rcfile)
    :_init_refs_lock(FALSE, MICOMT::Mutex::Recursive)
{
    dispatcher_factory_ = NULL;
    _disp = NULL;
    _theid = 1;
#ifndef HAVE_THREADS
    //_currentid = 0;
#else // HAVE_THREADS
    MICOMT::Thread::create_key(_current_rec_key, NULL);
#endif // HAVE_THREADS
    _rcfile = rcfile;
    _wait_for_completion = FALSE;
    _is_plugged = TRUE;
    _is_running = FALSE;
    _is_stopped = TRUE;
    _is_shutdown = FALSE;

#ifdef USE_ORB_CACHE
    _cache_used = FALSE;
//      _cache_rec = new ORBInvokeRec(0);
    _cache_rec = NULL;
#endif
#ifdef DEBUG_NAMES
    _isa_cache.name("ORB._isa_cache");
    _adapters.name("ORB._adapters");
    _invokes.name("ORB._invokes");
    _theid_lock.name("ORB._theid_lock");
    _init_refs_lock.name("ORB._init_refs_lock");
    _value_facs.name("ORB._value_facs");
#endif // DEBUG_NAMES
    _tmpl = new IOR;
}

CORBA::ORB::~ORB ()
{
#ifdef USE_ORB_CACHE
    if (_cache_used && _cache_rec != NULL)
	delete _cache_rec;
#endif
    assert(this->_disp != NULL);
    delete _disp;
    delete _tmpl;
    MICOMT::AutoWRLock l(_invokes);
    map<MsgId, ORBInvokeRec *, less<MsgId> >::iterator i;
    for (i = _invokes.begin(); i != _invokes.end(); ++i)
	delete (*i).second;
    if (iiop_proxy_instance != NULL) {
	delete iiop_proxy_instance;
	iiop_proxy_instance = NULL;
    }
    if (iiop_server_instance != NULL) {
	delete iiop_server_instance;
	iiop_server_instance = NULL;
    }
    CORBA::Codeset::free();
#ifdef HAVE_THREADS
    MICO::MTManager::free();
    MICOMT::Thread::delete_key(_current_rec_key);

    assert(this->dispatcher_factory_ != NULL);
    delete this->dispatcher_factory_;
#endif // HAVE_THREADS
    PortableInterceptor::restore_initializers();
}

/*
 * iioploc://[<addrlist>][/<URL-encoded-object-key>]
 *
 * Note: This syntax is from an early INS revision; this code should
 *       be retired. For the moment, a warning is emitted.
 */

CORBA::Object_ptr
CORBA::ORB::iioploc_to_object (const char * str)
{
  string iioploc (str);
  size_t pos = iioploc.find ("//");

  if (strncmp (str, "iioploc:", 8) != 0 || pos != 8) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  string addresses;
  Octet * key;
  ULong len;

  size_t pos2 = iioploc.find ('/', pos+2);

  if (pos2 != (size_t) -1) {
    addresses = iioploc.substr (pos+2, pos2-pos-2);
    key = mico_url_decode (iioploc.substr(pos2+1).c_str(), len);
  }
  else {
    addresses = iioploc.substr (pos+2);
    len = 0;
    key = NULL;
  }

  if (addresses.length() == 0) {
    addresses = MICO::InetAddress::hostname();
  }

  IOR * ior = new IOR;
  ULong profiles=0;

  while (addresses.length()) {
    string version, host, port;

    if ((pos = addresses.find(',')) == (size_t) -1) {
      host = addresses;
      addresses = "";
    }
    else {
      host = addresses.substr (0, pos);
      addresses = addresses.substr (pos+1);
    }

    if ((pos = host.find ('@')) != (size_t) -1) {
      version = host.substr (0, pos);
      host = host.substr (pos+1);
    }
    else {
      version = "1.0";
    }

    if ((pos = host.find (':')) != (size_t) -1) {
      port = host.substr (pos+1);
      host = host.substr (0, pos);
    }
    else {
      port = "9999"; // XXX not fixed yet
    }

    if (host.length() == 0) {
      host = MICO::InetAddress::hostname();
    }

    if ((pos = version.find ('.')) == (size_t) -1) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
    }

    UShort portno = atoi (port.c_str());
    UShort major, minor;

    major = (UShort) atoi (version.c_str());
    minor = (UShort) atoi (version.c_str() + pos + 1);

    // add profile

    MICO::InetAddress * addr = new MICO::InetAddress (host.c_str(), portno);

    if (!addr->valid()) {
      delete addr;
      mico_throw (BAD_PARAM());
    }

    MICO::IIOPProfile * prof = new MICO::IIOPProfile (key, len, *addr,
						      MultiComponent(),
						      major*256+minor);

    ior->add_profile (prof);
    delete addr;
    profiles++;
  }

  CORBA::string_free ((char *) key);
  return ior_to_object (ior);
}

/*
 * iiopname://<addr>,<addr>[/<URL-encoded-object-key>]
 *
 * Note: This syntax is from an early INS revision; this code should
 *       be retired. For the moment, a warning is emitted.
 */

CORBA::Object_ptr
CORBA::ORB::iiopname_to_object (const char * str)
{
  string iiopname (str);
  size_t pos = iiopname.find ("//");

  if (strncmp (str, "iiopname:", 9) != 0 || pos != 9) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  string addresses;
  string key;

  size_t pos2 = iiopname.find ('/', pos+2);

  if (pos2 != (size_t) -1) {
    addresses = iiopname.substr (pos+2, pos2-pos-2);
    key = iiopname.substr (pos2+1);
  }
  else {
    addresses = iiopname.substr (pos+2);
  }

  string nsloc = "iioploc://";
  nsloc += addresses;
  nsloc += '/';
  nsloc += "NameService";

  Object_var nsobj = string_to_object (nsloc.c_str());

  if (CORBA::is_nil (nsobj)) {
    return Object::_nil ();
  }

  /*
   * Empty key represents the root NamingContext itself
   */

  if (key.length() == 0) {
    return Object::_duplicate (nsobj);
  }

  /*
   * Ask Naming Service by DII. That saves a couple of kBs
   * by not needing to compile in the stubs
   */

  Request_var req = nsobj->_request ("resolve_str");
  *(req->arguments()->add(ARG_IN)->value()) <<= key.c_str();
  req->set_return_type (CORBA::_tc_Object);
  MICO_CATCHANY (req->invoke ());

  if (req->env()->exception()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
  }

  Object_var res;
  if (!(req->return_value() >>= CORBA::Any::to_object (res))) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO));
  }

  IOR * ior = new IOR (*res->_ior());
  return ior_to_object (ior);
}

/*
 * ptc/99-12-02:
 *
 * `corbaloc:' <obj_addr_list> [ / <url_encoded_key_string> ]
 *
 * <obj_addr_list>  = [ <obj_addr> `,' ]* <obj_addr>
 * <obj_addr>       = <rir_prot_addr> | <iiop_prot_addr> | <future_prot_addr>
 *
 * <rir_prot_addr>  = `rir:'
 *
 * <iiop_prot_addr> = `:' <iiop_addr> | `iiop:' <iiop_addr>
 * <iiop_addr>      = [ <version> ] <host> [ `:' <port> ]
 * <version>        = <major> `.' <minor> `@'
 */

CORBA::Object_ptr
CORBA::ORB::corbaloc_to_object (const char * str)
{
  if (strncmp (str, "corbaloc:", 9) != 0) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  string corbaloc (str);
  size_t pos = corbaloc.find ('/');

  string obj_addr_list;
  string url_encoded_key_string;
  Octet * key_string;
  ULong key_length;

  if (pos == (size_t) -1) {
    obj_addr_list = corbaloc.substr (9);
    key_string = NULL;
    key_length = 0;
  }
  else {
    obj_addr_list = corbaloc.substr (9, pos-9);
    url_encoded_key_string = corbaloc.substr (pos+1);
    key_string = mico_url_decode (url_encoded_key_string.c_str(), key_length);
  }

  /*
   * The rir protocol can not be used with any other protocol in a URL.
   */

  if (strncmp (obj_addr_list.c_str(), "rir:", 4) == 0) {
    /*
     * Hmm, can we have multiple rir: addresses in a URL? I don't think so.
     */

    if (obj_addr_list.find (',') != (size_t) -1) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
    }

    CORBA::String_var rir = (char *) key_string;

    /*
     * An empty <key_string> is interpreted as the default "NameService"
     */

    if (key_length == 0) {
      rir = CORBA::string_dup ("NameService");
    }

    /*
     * Pass key string to resolve_initial_references
     */

    return resolve_initial_references (rir.in());
  }

  /*
   * Generate new IOR, and add a profile for each <obj_addr>
   */

  IOR * ior = new IOR;
  ULong profiles = 0;

  while (obj_addr_list.length() > 0) {
    string obj_addr;

    if ((pos = obj_addr_list.find (',')) == (size_t) -1) {
      obj_addr = obj_addr_list;
      obj_addr_list = "";
    }
    else {
      obj_addr = obj_addr_list.substr (0, pos);
      obj_addr_list = obj_addr_list.substr (pos+1);
    }

    /*
     * Expecting only <iiop_prot_addr> here
     */

    string iiop_addr;

    if (strncmp (obj_addr.c_str(), ":", 1) == 0) {
      iiop_addr = obj_addr.substr (1);
    }
    else if (strncmp (obj_addr.c_str(), "iiop:", 5) == 0) {
      iiop_addr = obj_addr.substr (5);
    }
    else {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
    }
    
    string version, host, port;

    /*
     * <iiop_addr> = [ <version> `@' ] <host> [ `:' <port> ]
     */

    if ((pos = iiop_addr.find ('@')) != (size_t) -1) {
      version = iiop_addr.substr (0, pos);
      iiop_addr = iiop_addr.substr (pos+1);
    }
    else {
      version = "1.0";
    }

    if ((pos = iiop_addr.find (':')) != (size_t) -1) {
      port = iiop_addr.substr (pos+1);
      iiop_addr = iiop_addr.substr (0, pos);
    }
    else {
      port = "2809";
    }

    host = iiop_addr;

    /*
     * Convert version and port to numbers
     */

    if ((pos = version.find ('.')) == (size_t) -1) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
    }

    UShort portno = atoi (port.c_str());
    UShort major, minor;

    major = (UShort) atoi (version.c_str());
    minor = (UShort) atoi (version.c_str() + pos + 1);

    /*
     * add profile
     */

    MICO::InetAddress * addr = new MICO::InetAddress (host.c_str(), portno);

    if (!addr->valid()) {
      CORBA::string_free ((char *) key_string);
      delete addr;
      delete ior;
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
    }

    MICO::IIOPProfile * prof = new MICO::IIOPProfile (key_string, key_length,
						      *addr, MultiComponent(),
						      major*256+minor);

    ior->add_profile (prof);
    delete addr;
    profiles++;
  }

  CORBA::string_free ((char *) key_string);
  return ior_to_object (ior);
}

/*
 * ptc/99-12-02:
 *
 * `corbaname:' <corbaloc_obj> [ `#' string_name ]
 */

CORBA::Object_ptr
CORBA::ORB::corbaname_to_object (const char * str)
{
  if (strncmp (str, "corbaname:", 10) != 0) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  string corbaname (str);
  size_t pos = corbaname.find ('#');

  string corbaloc_obj;
  string url_name;

  /*
   * First construct a corbaloc URL of the form
   * corbaloc:<corbaloc_obj>
   */

  if (pos == (size_t) -1) {
    corbaloc_obj = corbaname.substr (10);
  }
  else {
    corbaloc_obj = corbaname.substr (10, pos-10);
    url_name = corbaname.substr (pos+1);
  }

  string corbaloc = "corbaloc:";
  corbaloc += corbaloc_obj;

  /*
   * If the <corbaloc_obj> does not contain a key string, a default key of
   * NameService is used.
   */

  if (corbaloc_obj.find ('/') == (size_t) -1) {
    corbaloc += "/NameService";
  }

  /*
   * This is converted to a naming context object reference with
   * CORBA::ORB::string_to_object.
   */

  CORBA::Object_var ncobj = string_to_object (corbaloc.c_str());

  /*
   * Empty <string_name> represents the root NamingContext itself
   */

  if (url_name.length() == 0) {
    return Object::_duplicate (ncobj);
  }

  /*
   * The <string_name> is converted to a CosNaming::Name
   * The resulting name is passed to a resolve operation on the naming context
   */

  CORBA::ULong string_length;
  CORBA::String_var string_name =
    (char *) mico_url_decode (url_name.c_str(), string_length);

  /*
   * Ask Naming Service by DII. That saves a couple of kBs
   * by not needing to compile in the stubs
   */

  Request_var req = ncobj->_request ("resolve_str");
  *(req->arguments()->add(ARG_IN)->value()) <<= string_name.in();
  req->set_return_type (CORBA::_tc_Object);
  MICO_CATCHANY (req->invoke ());

  if (req->env()->exception()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO));
  }

  Object_ptr res;
  if (!(req->return_value() >>= CORBA::Any::to_object (res))) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO));
  }

  /*
   * The object reference returned by the resolve is the result
   */

  return res;
}

/*
 * file://[hostname]<full path name>
 */

CORBA::Object_ptr
CORBA::ORB::file_to_object (const char * str)
{
  string filename (str);
  size_t pos = filename.find ("//");

  if (strncmp (str, "file:", 5) != 0 || pos != 5) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  size_t pos2 = filename.find ('/', pos+2);
  if (pos2 == (size_t) -1) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  string host = filename.substr (pos+2, pos2-pos-2);
  filename = filename.substr (pos2);

  /*
   * Hack: understand file://[pwd] in Windows, which expands to
   *       file://<drive>:/path. The drive letter would be read as
   *       host here.
   */

#ifdef _WIN32
  if (host.length() > 1 && host[1]==':') {
    filename = host + filename;
    host = "";
  }
#endif

  if (host.length() > 0 &&
      strcmp (host.c_str(), "localhost") != 0 &&
      strcmp (host.c_str(), "127.0.0.1") != 0) {
    MICO::InetAddress i1 (host.c_str(), 0);
    MICO::InetAddress i2 ("localhost", 0);
    if (!i1.valid() || !i2.valid() || i1.compare (i2) != 0) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
    }
  }

  CORBA::ULong len;
  CORBA::String_var fn =
    (char *) mico_url_decode (filename.c_str(), len);
  string ntfn (fn.in(), len);

  /*
   * Hack: accept //drive/path in Win32
   */

#ifdef _WIN32
  if (ntfn.length() > 3 && ntfn[0]=='/' && ntfn[1]=='/' && ntfn[3]=='/') {
    ntfn = ntfn.substr(2);
    ntfn[1] = ':';
  }
#endif

  ifstream in (ntfn.c_str());
  string data;
  char buf[257];
  int count;

  if (!in.good()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
  }

  while (in.good() && !in.eof()) {
    count = in.read (buf, 256).gcount();
    buf[count] = '\0';
    data += buf;
  }

  while (data.length() &&
	 (data[data.length()-1] == '\r' || data[data.length()-1] == '\n')) {
    data.resize (data.length()-1);
  }

  return string_to_object (data.c_str());
}

/*
 * http://host[:port][<abs_path>]
 */

CORBA::Object_ptr
CORBA::ORB::http_to_object (const char * str)
{
  string url (str);
  size_t pos = url.find ("//");
  string host, port, abs_path;

  if (pos != 5) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9, CORBA::COMPLETED_NO));
  }

  size_t pos2 = url.find ('/', pos+2);

  if (pos2 != (size_t) -1) {
    abs_path = url.substr (pos2);
    host = url.substr (pos+2, pos2-pos-2);
  }
  else {
    abs_path = "/";
    host = url;
  }

  if ((pos = host.find (':')) != (size_t) -1) {
    port = host.substr (pos+1);
    host = host.substr (0, pos);
  }
  else {
    port = "80";
  }

  UShort portno = atoi (port.c_str());

  MICO::InetAddress * addr = new MICO::InetAddress (host.c_str(), portno);

  if (!addr->valid()) {
    delete addr;
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
  }

  CORBA::Transport * trans = addr->make_transport ();

  if (!trans || trans->bad() || !trans->connect(addr)) {
    delete trans;
    delete addr;
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO));
  }

  delete addr;

  /*
   * For simplicity, speak HTTP/0.9
   */
  CORBA::Long written = 0;
  if (trans->write ("GET ", 4) != 4
      || (written = trans->write ((void *) abs_path.c_str(),
				  abs_path.length())) < 0
      || (CORBA::ULong)written != abs_path.length()
      || trans->write ("\r\n\r\n", 4) != 4) {
    delete trans;
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO));
  }

  /*
   * Read response
   */

  string data;
  char buf[257];

  while (!trans->eof() && !trans->bad()) {
    CORBA::Long got = trans->read (buf, 256);
    if (got > 0) {
      buf[got] = '\0';
      data += buf;
    }
  }

  if (trans->bad()) {
    delete trans;
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO));
  }

  delete trans;

  while (data.length() &&
	 (data[data.length()-1] == '\r' || data[data.length()-1] == '\n')) {
    data.resize (data.length()-1);
  }

  // only use the last line, some http servers send headers even in
  // response to http 0.9 requests

  for (int i = data.length()-1; i >= 0; --i) {
      if (data[i] == '\r' || data[i] == '\n') {
          data = data.substr (i+1);
          break;
      }
  }

  return string_to_object (data.c_str());
}

char *
CORBA::ORB::object_to_string (Object_ptr obj)
{
    MICO_OBJ_CHECK (this);

    if (CORBA::is_nil (obj)) {
	CORBA::IOR ior;
	return CORBA::string_dup (ior.stringify().c_str());
    }

    MICO_OBJ_CHECK2 (obj, CORBA::OBJECT_NOT_EXIST());

    if (!obj->_ior())
	// locality constrained object
	mico_throw (MARSHAL());
    return CORBA::string_dup (obj->_ior()->stringify().c_str());
}

CORBA::Object_ptr
CORBA::ORB::string_to_object (const char *str)
{
    MICO_OBJ_CHECK (this);

    if (!str)
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 7, CORBA::COMPLETED_NO));

    if (strncmp (str, "IOR:", 4) == 0) {
	IOR *ior = new IOR;
	if (!ior->from_string (str)) {
	    delete ior;
	    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 9,
					  CORBA::COMPLETED_NO));
	}
	return ior_to_object (ior);
    } else if (strncmp (str, "corbaname:", 10) == 0) {
      return corbaname_to_object (str);
    } else if (strncmp (str, "corbaloc:", 9) == 0) {
      return corbaloc_to_object (str);
    } else if (strncmp (str, "iioploc:", 8) == 0) {
      if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Warning)
	  << "Warning: use of old iioploc format" << endl;
      }
      return iioploc_to_object (str);
    } else if (strncmp (str, "iiopname:", 9) == 0) {
      if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Warning)
	  << "Warning: use of old iiopname format" << endl;
      }
      return iiopname_to_object (str);
    } else if (strncmp (str, "file:", 5) == 0) {
      return file_to_object (str);
    } else if (strncmp (str, "http:", 5) == 0) {
      return http_to_object (str);
    } else {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 7,
				    CORBA::COMPLETED_NO));
      // to make some compilers happy
      return Object::_nil();
    }
}

CORBA::Object_ptr
CORBA::ORB::ior_to_object (IOR *ior)
{
    if (!ior->profile()) {
	delete ior;
	return CORBA::Object::_nil();
    }

    Object_var obj = new Object (ior);
    obj->_setup_domains (CORBA::Object::_nil());

    ObjectAdapter *oa = get_oa (obj);
    if (oa && oa->is_local ()) {
	CORBA::Object_var skel = oa->skeleton (obj);
	if (!CORBA::is_nil (skel)) {
	    return Object::_duplicate (skel);
	}
    }
    return Object::_duplicate (obj);
}

CORBA::Boolean
CORBA::ORB::is_impl (Object_ptr obj)
{
    if (CORBA::is_nil (obj)) {
      return FALSE;
    }
    ObjectAdapter *oa = get_oa (obj);
    if (!oa || !oa->is_local()) {
      return FALSE;
    }
    CORBA::Object_var skel = oa->skeleton (obj);
    return skel.in() == obj;
}

const char *
CORBA::ORB::rcfile()
{
    return _rcfile.c_str();
}

char *
CORBA::ORB::tag_to_string (const ObjectTag &oid)
{
    string s;

    for (CORBA::ULong i = 0; i < oid.length(); ++i) {
	assert ((CORBA::Octet)oid[i]);
	s += (char)(CORBA::Octet)oid[i];
    }
    return CORBA::string_dup (s.c_str());
}

CORBA::ORB::ObjectTag *
CORBA::ORB::string_to_tag (const char *s)
{
    CORBA::ULong len = strlen (s);

    CORBA::ORB::ObjectTag *oid = new CORBA::ORB::ObjectTag;
    oid->length (len);

    for (CORBA::ULong i = 0; i < len; ++i) {
        (*oid)[i] = (CORBA::Octet) s[i];
    }
    return oid;
}

void
CORBA::ORB::create_list (Long len, NVList_out res)
{
    if (len < 0)
	mico_throw (CORBA::BAD_PARAM());

    res = new NVList (len);
}

void
CORBA::ORB::create_operation_list (OperationDef_ptr op, NVList_out res)
{
    res = new NVList (0);
    ParDescriptionSeq_var params = op->params();
    for (CORBA::ULong i = 0; i < params->length(); ++i) {
	switch (params[i].mode) {
	case PARAM_IN:
	    res->add(ARG_IN)->value()->set_type (params[i].type);
	    break;
	case PARAM_OUT:
	    res->add(ARG_OUT)->value()->set_type (params[i].type);
	    break;
	case PARAM_INOUT:
	    res->add(ARG_INOUT)->value()->set_type (params[i].type);
	    break;
	default:
	    assert (0);
	}
    }
}

void
CORBA::ORB::create_named_value (NamedValue_out res)
{
    res = new NamedValue;
}

void
CORBA::ORB::create_exception_list (ExceptionList_out res)
{
    res = new ExceptionList;
}

void
CORBA::ORB::create_context_list (ContextList_out res)
{
    res = new ContextList;
}

void
CORBA::ORB::create_environment (Environment_out res)
{
    res = new Environment;
}

void
CORBA::ORB::get_default_context (Context_out res)
{
    res = new Context ("");
}

void
CORBA::ORB::get_default_domain_manager (DomainManager_out dm)
{
    MICO_OBJ_CHECK (this);

    if (CORBA::is_nil (_def_manager)) {
	_def_manager = new MICO::DomainManager_impl;

	// install default policies ...
	MICOPolicy::TransportPrefPolicy::ProfileTagSeq prefs;
	prefs.length (7);

	prefs[0] = CORBA::IORProfile::TAG_INTERNET_IOP;
	prefs[1] = CORBA::IORProfile::TAG_UNIX_IOP;
	prefs[2] = CORBA::IORProfile::TAG_UDP_IOP;
	prefs[3] = CORBA::IORProfile::TAG_SSL_INTERNET_IOP;
	prefs[4] = CORBA::IORProfile::TAG_SSL_UNIX_IOP;
	prefs[5] = CORBA::IORProfile::TAG_SSL_UDP_IOP;
	prefs[6] = CORBA::IORProfile::TAG_LTP_IOP;

	MICOPolicy::TransportPrefPolicy_var tpp =
	    new MICO::TransportPrefPolicy_impl (prefs);

	_def_manager->set_domain_policy (tpp);
    }
    dm = CORBA::DomainManager::_duplicate (_def_manager);
}

CORBA::Boolean
CORBA::ORB::get_service_information (
    CORBA::ServiceType service_type,
    CORBA::ServiceInformation_out service_information)
{
#ifdef USE_CSL2
  // ###ras Needs to be adapted
  if (service_type == Security) {
      service_information = new CORBA::ServiceInformation;

      //    CORBA::ServiceInformation srv_info;
      service_information->service_options.length(2);
      service_information->service_options[0] = 1;
      service_information->service_options[1] = 10;

      service_information->service_details.length(2);
      service_information->service_details[0].service_detail_type = 1;
      service_information->service_details[0].service_detail.length(3);
      service_information->service_details[0].service_detail[0] = 'S';
      service_information->service_details[0].service_detail[1] = 'S';
      service_information->service_details[0].service_detail[2] = 'L';

      service_information->service_details[1].service_detail_type = 2;
      service_information->service_details[1].service_detail.length(4);
      service_information->service_details[1].service_detail[0] = 'N';
      service_information->service_details[1].service_detail[1] = 'O';
      service_information->service_details[1].service_detail[2] = 'N';
      service_information->service_details[1].service_detail[3] = 'E';

      return TRUE;
    }
#endif // USE_CSL2

    return FALSE;
}

void
CORBA::ORB::send_multiple_requests_oneway (const RequestSeq &req)
{
    for (CORBA::ULong i = 0; i < req.length(); ++i) {
	req[i]->send_oneway ();
    }
}

void
CORBA::ORB::send_multiple_requests_deferred (const RequestSeq &req)
{
    for (CORBA::ULong i = 0; i < req.length(); ++i) {
	req[i]->send_deferred ();
    }
}

CORBA::Boolean
CORBA::ORB::poll_next_response ()
{
    MICO_OBJ_CHECK (this);

    ORBInvokeRec *rec;
    ORBRequest *orbreq;

    MICOMT::AutoRDLock l(_invokes);

#ifdef USE_ORB_CACHE
    if (_cache_used) {
      if (_cache_rec->request_type() == RequestInvoke &&
	  _cache_rec->completed() &&
	  !strcmp (_cache_rec->request()->type(), "local")) {
	return TRUE;
      }
    }
#endif

    for (InvokeMap::iterator i = _invokes.begin(); i != _invokes.end(); ++i) {
	rec = (*i).second;
	orbreq = rec->request ();
	if (rec->request_type() == RequestInvoke &&
	    rec->completed() &&
	    !strcmp (orbreq->type(), "local")) {
	    // found a local request that has completed
	    return TRUE;
	}
    }
    return FALSE;
}

void
CORBA::ORB::get_next_response (Request_out req)
{
    MICO_OBJ_CHECK (this);

    ORBInvokeRec *rec;
    ORBRequest *orbreq;

    MICOMT::AutoRDLock l(_invokes);

#ifdef USE_ORB_CACHE
    if (_cache_used) {
      if (_cache_rec->request_type() == RequestInvoke &&
	  _cache_rec->completed() &&
	  !strcmp (_cache_rec->request()->type(), "local")) {
	  req = Request::_duplicate
	      (((MICO::LocalRequest *)(_cache_rec->request()))->request());
	  return;
      }
    }
#endif

    for (InvokeMap::iterator i = _invokes.begin(); i != _invokes.end(); ++i) {
	rec = (*i).second;
	orbreq = rec->request ();
	if (rec->request_type() == RequestInvoke &&
	    rec->completed() &&
	    !strcmp (orbreq->type(), "local")) {
	    // found a local request that has completed
	    req = Request::_duplicate
		(((MICO::LocalRequest *)orbreq)->request());
	    return;
	}
    }
    req = Request::_nil();
}

void
CORBA::ORB::do_shutdown ()
{
    if (!_is_shutdown) {
#ifdef HAVE_THREADS
	MICOMT::AutoLock t_lock(shutdown_lock_);
	// double checking after obtaining shutdown lock
	if (!_is_shutdown) {
#endif // HAVE_THREADS
	    _is_shutdown = 1;

	    _adapters.rdlock();

	    if (_wait_for_completion)
		_shutting_down_adapters = _adapters;

	    // work on copy in case OA's unregister ifself during shutdown ...
	    OAVec adapters_copy = _adapters;

	    _adapters.unlock();

	    for (ULong i = 0; i < adapters_copy.size(); ++i) {
		adapters_copy[i]->shutdown (_wait_for_completion);
	    }
	    if (_wait_for_completion) {
		// FIXME this might be unsafe
		while (_shutting_down_adapters.size() > 0)
		    _disp->run (FALSE);
	    }
	    _is_shutdown = 2;
#ifdef HAVE_THREADS
	}
#endif // HAVE_THREADS
    }
}

CORBA::Boolean
CORBA::ORB::work_pending ()
{
    if (_is_shutdown > 1)
        mico_throw (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO));

#ifdef HAVE_THREADS
    // this is a little trick here. We'll return TRUE for call
    // from other thread than main thread
    if (MICOMT::Thread::self() != main_thread_id_) {
	return TRUE;
    }
#endif // HAVE_THREADS
    // if !stopped run() has been called and cares for events ...
#ifdef HAVE_THREADS
    MICOMT::AutoLock t_lock(run_lock_);
    // double checking after obtaining run lock
    if (_is_shutdown > 1)
        mico_throw (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO));
#endif // HAVE_THREADS
    return !_disp->idle();
}

void
CORBA::ORB::perform_work ()
{
#ifdef HAVE_THREADS
    // as per CORBA 2.6.1 4.2.4.2 - do nothing if not called from main thread
    if (MICOMT::Thread::self() != main_thread_id_) {
	// why not sleep for one second when someone calls us from other thread
	// than main thread?
#ifdef _WIN32
	::Sleep(1000);// system sleep for one second
#else // _WIN32
	sleep(1);
#endif // _WIN32
	return;
    }
#endif // HAVE_THREADS
    if (_is_shutdown > 1)
        mico_throw (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO));

    _disp->run (FALSE);
}

void
CORBA::ORB::run ()
{
    // in MT env, ORB::run should be started by multiple threads
#ifdef HAVE_THREADS
    MICOMT::AutoLock t_run_lock(run_lock_);
    {
	MICOMT::AutoLock t_shutdown_lock(shutdown_lock_);
	if (_is_shutdown > 1)
	    mico_throw(CORBA::BAD_INV_ORDER(4, COMPLETED_NO));
    }
#else // HAVE_THREADS
    assert (!_is_running);
#endif // HAVE_THREADS
    _is_running = TRUE;
    _is_stopped = FALSE;
    while (!_is_stopped) {
	_disp->run (FALSE);
    }
    do_shutdown ();
}

void
CORBA::ORB::answer_shutdown (ObjectAdapter *oa)
{
    for (ULong i = 0; i < _shutting_down_adapters.size(); ++i) {
	if (_shutting_down_adapters[i] == oa) {
	    _shutting_down_adapters.erase (_shutting_down_adapters.begin()+i);
	    break;
	}
    }
}

void
CORBA::ORB::shutdown (Boolean wait)
{
    if (wait) {
	// do we serve this request, if so throw BAD_INV_ORDER system exception
	// this is implemented only for POA not for BOA
	if (!CORBA::is_nil(PortableServer::_the_poa_current)
	    && PortableServer::_the_poa_current->iscurrent()) {
	    mico_throw(CORBA::BAD_INV_ORDER(3, COMPLETED_NO));
	}
    }
    _wait_for_completion = wait;
    _is_stopped = TRUE;

    // if run() has been called it will perform the shutdown,
    // otherwise do it right now...
    if (!_is_running)
	do_shutdown();
}

void
CORBA::ORB::destroy ()
{
    if (!_is_stopped)
        shutdown (TRUE);

    if (MICO::Logger::IsLogged (MICO::Logger::Info)) {
       MICO::Logger::Stream (MICO::Logger::Info)
         << "ORB::shutdown : orb_instance->_refcnt() =  "
	 << orb_instance->_refcnt() << endl;
    }
    // destroy all registered PIs
    PortableInterceptor::destroy_all_interceptors();

    CORBA::release(orb_instance);
    orb_instance = _nil();
}

CORBA::ORB::ObjectIdList *
CORBA::ORB::list_initial_services ()
{
    MICOMT::AutoLock l(_init_refs_lock);

    MICO_OBJ_CHECK (this);

    ObjectIdList_ptr idlist = new ObjectIdList;
    idlist->length (_init_refs.size());

    int j;
    InitialRefMap::iterator i;
    for (j = 0, i = _init_refs.begin(); i != _init_refs.end(); ++i, ++j)
	(*idlist)[j] = (*i).first.c_str();

    if (!_init_refs.count ("RootPOA")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"RootPOA";
    }
    if (!_init_refs.count ("POACurrent")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"POACurrent";
    }
    if (!_init_refs.count ("DynAnyFactory")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"DynAnyFactory";
    }
    if (!_init_refs.count ("PrincipalCurrent")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"PrincipalCurrent";
    }
#ifdef USE_CSL2
    if (!_init_refs.count ("SecurityCurrent")) {
        idlist->length (j+1);
        (*idlist)[j++] = (const char *)"SecurityCurrent";
    }
    if (!_init_refs.count ("SecurityManager")) {
        idlist->length (j+1);
        (*idlist)[j++] = (const char *)"SecurityManager";
    }
    if (!_init_refs.count ("PolicyCurrent")) {
        idlist->length (j+1);
        (*idlist)[j++] = (const char *)"PolicyCurrent";
    }
    if (!_init_refs.count ("ODM")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"ODM";
    }
    if (!_init_refs.count ("NameExt")) {
        idlist->length (j+1);
	(*idlist)[j++] = (const char *)"NameExt";
    }
    if (!_init_refs.count ("DomainManagerFactory")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"DomainManagerFactory";
    }
#endif // USE_CSL2
    if (!_init_refs.count ("CodecFactory")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"CodecFactory";
    }
    if (!_init_refs.count ("PICurrent")) {
	idlist->length (j+1);
	(*idlist)[j++] = (const char *)"PICurrent";
    }
    return idlist;
}

CORBA::Object_ptr
CORBA::ORB::_resolve_initial_references (const char *id)
{
    MICO_OBJ_CHECK (this);

    InitialRefMap::iterator i = _init_refs.find (id);

    if (i == _init_refs.end()) {
        if (_default_init_ref.length() > 0) {
	  string loc = _default_init_ref;
	  loc += '/';
	  loc += id;
	  CORBA::Object_ptr res = string_to_object (loc.c_str());
	  if (!CORBA::is_nil (res)) {
	    return res;
	  }
	}

	if (!strcmp (id, "RootPOA")) {
	  PortableServer::POA_var rootpoa = new MICOPOA::POA_impl (this);
	} else if (!strcmp (id, "POACurrent")) {
	  // RootPOA initializes POACurrent
	  PortableServer::POA_var rootpoa = new MICOPOA::POA_impl (this);
#ifdef USE_CSL2
# ifdef HAVE_SSL	  
        } else if (!strcmp (id, "SecurityCurrent")) {
	    // SecurityLevel1::Current_var securitycurrent = new MICOSL1::Current_impl(this);
	    //assert (!CORBA::is_nil (securitycurrent));
	    _init_refs[id] = new MICOSL2::Current_impl(this);//new MICOSL2::Current_impl(this);
	} else if (!strcmp (id, "SecurityManager")) {
	    _init_refs[id] = new MICOSL2::SecurityManager_impl(this);
	} else if (!strcmp (id, "PolicyCurrent")) {
	    _init_refs[id] = new MICOSL2::PolicyCurrent_impl(this);
	} else if (!strcmp (id, "ODM")) {
	    _init_refs[id] = new MICOSODM::ODM_impl;
	} else if (!strcmp (id, "NameExt")) {
	    _init_refs[id] = new MICOSDM::NameExt_impl;
	} else if (!strcmp (id, "DomainManagerFactory")) {
	    _init_refs[id] = new MICOSDM::DomainManagerFactory_impl;
# endif	  
#endif // USE_CSL2
	} else if (!strcmp (id, "DynAnyFactory")) {
	    _init_refs[id] = new DynAnyFactory_impl;
	} else if (!strcmp (id, "PrincipalCurrent")) {
	    _init_refs[id] = new PrincipalCurrent_impl;
	} else if (!strcmp (id, "CodecFactory")) {
	    _init_refs[id] = new PICodec::CodecFactory_impl;
	} else if (!strcmp (id, "PICurrent")) {
	    _init_refs[id] = new PInterceptor::Current_impl;
	} else {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: unknown id " << id
	      << " in resolve_initial_references" << endl;
	  }
	  mico_throw (CORBA::ORB::InvalidName());
	}
	i = _init_refs.find (id);
    }
    return Object::_duplicate ((*i).second);
}

// added according to PI spec
void
CORBA::ORB::register_initial_reference (const char *id, Object_ptr obj)
{
    if (CORBA::is_nil(obj)) {
	mico_throw(CORBA::BAD_PARAM());
    }
    InitialRefMap::iterator i = _init_refs.find (id);
    if (i == _init_refs.end()) {
	if (_default_init_ref.length() > 0) {
	    string loc = _default_init_ref;
	    loc += '/';
	    loc += id;
	    CORBA::Object_ptr res = string_to_object (loc.c_str());
	    if (!CORBA::is_nil (res)) {		
		mico_throw (CORBA::ORB::InvalidName());
	    }
	}
	this->set_initial_reference(id, obj);
    }
    else
	mico_throw (CORBA::ORB::InvalidName());
}
// end

CORBA::Boolean
CORBA::ORB::set_initial_reference (const char *id, Object_ptr obj)
{
    MICOMT::AutoLock l(_init_refs_lock);

    _init_refs[id] = CORBA::Object::_duplicate (obj);
    return TRUE;
}

CORBA::Boolean
CORBA::ORB::set_default_initial_reference (const char * url)
{
    _default_init_ref = url;
    return TRUE;
}

const CORBA::Octet *
CORBA::ORB::terminal_id (CORBA::ULong &length)
{
    length = _terminal_id.length();
    return _terminal_id.get_buffer();
}

void
CORBA::ORB::terminal_id (const CORBA::Octet *val, CORBA::ULong len)
{
    _terminal_id.length(len);
    for (size_t i = 0; i < len; ++i) {
        _terminal_id[i] = val[i];
    }
}
 
CORBA::Boolean
CORBA::ORB::is_mobile_key (const CORBA::Octet * key)
{
    CORBA::Buffer buf (const_cast<CORBA::Octet *> (key));
#ifdef _WIN32
    MICO::CDRDecoder dc (&buf, FALSE, CORBA::DefaultEndian,
			 0, TRUE, 0, TRUE);
#else
    MICO::CDRDecoder dc (&buf, FALSE);
#endif
#define check(exp) if (!(exp)) return FALSE
    CORBA::Octet bo;
    check(dc.get_octet(bo));
    char mior[4];
    check (dc.get_chars_raw (mior, 4));
    check (memcmp (mior, "MIOR", 4) == 0);
#undef check
    return TRUE;
}

CORBA::TypeCode_ptr
CORBA::ORB::create_struct_tc (const char *rep_id,
			      const char *name,
			      const StructMemberSeq &members)
{
    return TypeCode::create_struct_tc (rep_id, name, members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_union_tc (const char *rep_id,
			     const char *name,
			     TypeCode_ptr disc_type,
			     const UnionMemberSeq &members)
{
    return TypeCode::create_union_tc (rep_id, name, disc_type, members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_enum_tc (const char *rep_id,
			    const char *name,
			    const EnumMemberSeq &members)
{
    return TypeCode::create_enum_tc (rep_id, name, members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_exception_tc (const char *rep_id,
				 const char *name,
				 const StructMemberSeq &mems)
{
    return TypeCode::create_exception_tc (rep_id, name, mems);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_alias_tc (const char *rep_id,
			     const char *name,
			     TypeCode_ptr orig_type)
{
    return TypeCode::create_alias_tc (rep_id, name, orig_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_interface_tc (const char *rep_id, const char *name)
{
    return TypeCode::create_interface_tc (rep_id, name);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_string_tc (ULong bound)
{
    return TypeCode::create_string_tc (bound);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_wstring_tc (ULong bound)
{
    return TypeCode::create_wstring_tc (bound);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_fixed_tc (UShort digits, Short scale)
{
    return TypeCode::create_fixed_tc (digits, scale);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_sequence_tc (ULong bound, TypeCode_ptr el_type)
{
    return TypeCode::create_sequence_tc (bound, el_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_recursive_sequence_tc (ULong bound, ULong offset)
{
    return TypeCode::create_recursive_sequence_tc (bound, offset);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_array_tc (ULong length, TypeCode_ptr el_type)
{
    return TypeCode::create_array_tc (length, el_type);
}


CORBA::TypeCode_ptr
CORBA::ORB::create_value_tc (const char *rep_id, const char *name,
			     ValueModifier mod,
			     TypeCode_ptr concrete_base,
			     const ValueMemberSeq &members)
{
    return TypeCode::create_value_tc (rep_id, name, mod,
				      concrete_base, members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_value_box_tc (const char *rep_id,
				 const char *name,
				 TypeCode_ptr boxed_type)
{
    return TypeCode::create_value_box_tc (rep_id, name, boxed_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_native_tc (const char *rep_id,
			      const char *name)
{
    return TypeCode::create_native_tc (rep_id, name);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_recursive_tc (const char *rep_id)
{
    return TypeCode::create_recursive_tc (rep_id);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_abstract_interface_tc (const char *rep_id,
					  const char *name)
{
    return TypeCode::create_abstract_interface_tc (rep_id, name);
}

CORBA::ORB::MsgId
CORBA::ORB::new_msgid ()
{
    MICOMT::AutoLock l(_theid_lock);
    MICOMT::AutoRDLock l2(_invokes);

    while (_invokes.count (++_theid) > 0) {
	if (MICO::Logger::IsLogged(MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream(MICO::Logger::Warning)
		<< "Oops: msgid colision" << endl;
	}
    }
    return _theid;
}

CORBA::ORBMsgId
CORBA::ORB::new_orbid (MsgId msgid)
{
    if (msgid == 0)
	msgid = new_msgid();
    ORBInvokeRec *rec = create_invoke(msgid);
    return rec;
}

CORBA::ORBInvokeRec *
CORBA::ORB::create_invoke (MsgId msgid)
{
#ifdef USE_ORB_CACHE
    if (!_cache_used) {
	_cache_used = TRUE;
	_cache_rec = new ORBInvokeRec(msgid);
	return _cache_rec;
    }
#endif
    return new ORBInvokeRec(msgid);
}

void
CORBA::ORB::add_invoke (ORBInvokeRec *rec)
{
#ifdef USE_ORB_CACHE
    if (rec == _cache_rec)
	return;
#endif
    if (MICO::Logger::IsLogged (MICO::Logger::ORB)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::ORB)
	    << "ORB::add_invoke (MsgId="<< rec->id() << ")" << endl;
    }
    MICOMT::AutoWRLock l(_invokes);

    _invokes[rec->id()] = rec;
}

CORBA::ORBInvokeRec *
CORBA::ORB::get_invoke (MsgId id)
{
    MICOMT::AutoRDLock l(_invokes);

#ifdef USE_ORB_CACHE
    if (_cache_used && _cache_rec->id() == id && _cache_rec->active() )
	return _cache_rec;
#endif

    if (MICO::Logger::IsLogged (MICO::Logger::ORB)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::ORB)
	    << "ORB::get_invoke (MsgId="<< id << ")" << endl;
    }
    map<MsgId, ORBInvokeRec *, less<MsgId> >::iterator i;
    i = _invokes.find (id);
    if (i == _invokes.end()) {
	return NULL;
    }
    CORBA::ORBInvokeRec *rec = (*i).second;
    return rec;
}

void
CORBA::ORB::del_invoke (MsgId id)
{
#ifdef USE_ORB_CACHE
    if (_cache_used && _cache_rec->id() == id) {
	delete _cache_rec;
	_cache_used = FALSE;
	return;
    }
#endif

    if (MICO::Logger::IsLogged (MICO::Logger::ORB)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::ORB)
	    << "ORB::del_invoke (MsgId="<< id << ")" << endl;
    }
    MICOMT::AutoWRLock l(_invokes);

    map<MsgId, ORBInvokeRec *, less<MsgId> >::iterator i;
    i = _invokes.find (id);
    if (i != _invokes.end()) {
	delete (*i).second;
	_invokes.erase (i);
    }
}

void
CORBA::ORB::dispatcher (Dispatcher *new_disp)
{
    if (new_disp && _disp) {
	// move the pending events from disp to _disp
	_disp->move (new_disp);
    }
    if (_disp) {
	delete _disp;
    }
    _disp = new_disp;
}

CORBA::Boolean
CORBA::ORB::is_local (Object_ptr o)
{
  //MT FIXME: maybe we need to look the profiles list
  /*
   * An object is local if at least one of its profiles matches one of
   * the profiles in our ior_template(). Since the profiles are ordered,
   * we can perform a linear comparison.
   */

  /*
   * locality constrained object is always local
   */

  if (!o->_ior()) {
    return TRUE;
  }

  IORProfile *p1 = o->_ior()->get_profile (0);
  IORProfile *p2 = _tmpl->get_profile (0);

  /*
   * If the object does not have any profiles, it must be local
   */

  if (!p1) {
    return TRUE;
  }

  /*
   * If we don't have any profiles for this process, any other object
   * must be remote.
   */

  if (!p2) {
    return FALSE;
  }

  ULong i=0, j=0;

  /*
   * If p1 is a GIOPSimpleProf, then the object reference is incoming
   * from a IIOPServer. Well, if it's delivered here, then it better be
   * local.
   */

  if (p1->id() == 10000) {
    return TRUE;
  }

  while (p1 && p2) {
    if (!p1->reachable()) {
      p1 = o->_ior()->get_profile (++i);
      continue;
    }
    if (!p2->reachable()) {
      p2 = _tmpl->get_profile (++j);
      continue;
    }

    if (p1->addr()->compare (*p2->addr()) == 0) {
      return TRUE;
    }

    Long res = p1->compare (*p2);

    assert (res != 0);
    if (res < 0) {
      p1 = o->_ior()->get_profile (++i);
    }
    else {
      p2 = _tmpl->get_profile (++j);
    }
  }

  return FALSE;
}

void
CORBA::ORB::register_oa (ObjectAdapter *oa)
{
  MICOMT::AutoWRLock l(_adapters);

  _adapters.push_back (oa);
}

void
CORBA::ORB::unregister_oa (ObjectAdapter *oa)
{
    MICOMT::AutoWRLock l(_adapters);

    for (ULong i = 0; i < _adapters.size(); ) {
	if (_adapters[i] == oa)
	    _adapters.erase (_adapters.begin() + i);
	else
	    ++i;
    }
}

void
CORBA::ORB::register_profile_id (CORBA::ULong id)
{
    if (iiop_proxy_instance)
        iiop_proxy_instance->register_profile_id (id);

    CORBA::DomainManager_var dm;
    get_default_domain_manager (dm);

    CORBA::Policy_var p = dm->get_domain_policy (
        MICOPolicy::TRANSPORTPREF_POLICY_TYPE);

    MICOPolicy::TransportPrefPolicy_var tpp =
        MICOPolicy::TransportPrefPolicy::_narrow (p);

    MICOPolicy::TransportPrefPolicy::
        ProfileTagSeq_var prefs = tpp->preferences();

    for (CORBA::ULong i = 0; i < prefs->length(); ++i) {
        if (prefs[i] == id)
            return;
    }

    prefs->length (prefs->length()+1);
    prefs[prefs->length()-1] = id;

    tpp->preferences (prefs.in());
}

void
CORBA::ORB::unregister_profile_id (CORBA::ULong id)
{
    if (iiop_proxy_instance)
        iiop_proxy_instance->unregister_profile_id (id);

    CORBA::DomainManager_var dm;
    get_default_domain_manager (dm);

    CORBA::Policy_var p = dm->get_domain_policy (
        MICOPolicy::TRANSPORTPREF_POLICY_TYPE);

    MICOPolicy::TransportPrefPolicy_var tpp =
        MICOPolicy::TransportPrefPolicy::_narrow (p);

    MICOPolicy::TransportPrefPolicy::
        ProfileTagSeq_var prefs = tpp->preferences();

    for (CORBA::ULong i = 0; i < prefs->length(); ++i) {
        if (prefs[i] == id) {
            for ( ; i < prefs->length()-1; ++i)
                prefs[i] = prefs[i+1];
            prefs->length (prefs->length()-1);
            tpp->preferences (prefs.in());
            break;
        }
    }
}

CORBA::ObjectAdapter *
CORBA::ORB::get_oa (Object_ptr o)
{
  MICOMT::AutoRDLock l(_adapters);

  Boolean local = is_local (o);
  for (ULong i0 = 0; i0 < _adapters.size(); ++i0) {
    if (_adapters[i0]->is_local() == local && _adapters[i0]->has_object (o))
      return _adapters[i0];
  }
  return NULL;
}

CORBA::Boolean
CORBA::ORB::builtin_invoke (ORBMsgId id, Object_ptr obj,
                            ORBRequest *req, Principal_ptr pr)
{
    /*
     * _non_existent is the only special one, _implementation, _interface,
     * and _is_a are just passed through to the OA.
     */
    if (!strcmp (req->op_name(), "_non_existent")) {
        if (!get_oa (obj)) {
            Any res;
            res <<= Any::from_boolean (TRUE);
	    NVList nv;
            if (!req->set_out_args (&res, &nv)) {
                CORBA::MARSHAL ex;
                req->set_out_args (&ex);
                answer_invoke (id, InvokeSysEx, Object::_nil(), req, 0);
            } else {
                answer_invoke (id, InvokeOk, Object::_nil(), req, 0);
            }
            return TRUE;
        }
    }
    return FALSE;
}

#ifdef USE_CSL2
CORBA::Principal_ptr
CORBA::ORB::get_principal(Object_ptr obj)
{
    ObjectAdapter *oa = this->get_oa(obj);
    return oa->get_principal(obj);
}
#endif // USE_CSL2

CORBA::ORBMsgId
CORBA::ORB::invoke_async (Object_ptr obj,
			  ORBRequest *req,
			  Principal_ptr pr,
			  Boolean response_exp,
			  ORBCallback *cb,
			  ORBMsgId id)
{
    MsgId msgid;
    ORBInvokeRec *rec;

    if (!id) {
	msgid = new_msgid();
    } else {
	msgid = get_msgid(id);
    }

    rec = id;
#ifndef HAVE_THREADS
    // XXX has to be changed for MT
    //_currentid = msgid;
    _currentid.push(msgid);

#else // HAVE_THREADS
    stack<CORBA::ORBInvokeRec*>* invs = static_cast<stack<CORBA::ORBInvokeRec*>*>
	(MICOMT::Thread::get_specific(_current_rec_key));
    if (invs == NULL) {
	invs = new stack<CORBA::ORBInvokeRec*>;
	invs->push(rec);
	MICOMT::Thread::set_specific(_current_rec_key, invs);
    }
    else {
	invs->push(rec);
    }
#endif // HAVE_THREADS

    // we need to know OA before calling
    // receive_request_service_context start point
    // - it's called from init_invoke
    ObjectAdapter *oa = NULL;
    if (!builtin_invoke (rec, obj, req, pr))
	oa = get_oa (obj);
#ifdef HAVE_THREADS
	if (!cb && response_exp) 
	    cb = new ORBAsyncCallback;
#endif
    if (!rec)
	rec = create_invoke(msgid);
//  	try {
//  	    rec->init_invoke (this, msgid, obj, req, pr, response_exp, cb, oa);
//  	} catch (...) {
//  	    rec->free();
//  	    return msgid;
//  	}
    try {
	rec->init_invoke (this, obj, req, pr, response_exp, cb, oa);
    } catch (...) {
	return id;
    }

    if (response_exp) {
	add_invoke (rec);
    }

    if (!builtin_invoke (rec, obj, req, pr)) {
	if (!oa) {
            CORBA::OBJECT_NOT_EXIST ex;
	    req->set_out_args (&ex);
	    answer_invoke (rec, InvokeSysEx, Object::_nil(), req, 0);
	    return rec;
	}
	rec->oa (oa);
        oa->invoke (rec, obj, req, pr, response_exp);
	if (!response_exp) {
#ifdef USE_ORB_CACHE
	  if (_cache_used && _cache_rec == rec) {
	    _cache_used = FALSE;
	  }
#endif
	  delete rec;
	}
    }
    return response_exp ? rec : 0;
}

CORBA::ORBMsgId
CORBA::ORB::locate_async (Object_ptr obj, ORBCallback *cb, ORBMsgId id)
{
    ObjectAdapter *oa = get_oa (obj);
    ORBInvokeRec *rec = get_invoke( id );
    if (!rec)
	rec = new_orbid();
#ifdef HAVE_THREADS
    if (!cb) 
	cb = new ORBAsyncCallback;
#endif
    rec->init_locate (this, obj, cb, oa);
    add_invoke (rec);

    if (!oa) {
        answer_locate (rec, LocateUnknown, Object::_nil(), 0);
    } else {
        oa->locate (rec, obj);
    }
    return rec;
}

CORBA::ORBMsgId
CORBA::ORB::bind_async (const char *repoid,
			const ObjectTag &oid,
			Address *addr, ORBCallback *cb,
			ORBMsgId id)
{
    ORBInvokeRec *rec = get_invoke( id );
    if (!rec)
	rec = new_orbid();
#ifdef HAVE_THREADS
    if (!cb) 
	cb = new ORBAsyncCallback;
#endif
    rec->init_bind (this, repoid, oid, addr, cb);
    add_invoke (rec);

    {
	MICOMT::AutoRDLock l(_adapters);

	for (ULong i = 0; i < _adapters.size(); ++i) {
	    rec->oa (_adapters[i]);
	    if (_adapters[i]->bind (rec, repoid, rec->tag(), addr))
		return rec;
	}
    }
    answer_bind (rec, LocateUnknown, Object::_nil());
    return rec;
}

CORBA::InvokeStatus
CORBA::ORB::invoke (Object_ptr &obj, ORBRequest *req,
		    Principal_ptr pr, Boolean reply_exp)
{
    ORBMsgId id = invoke_async (obj, req, pr, reply_exp);
    if (!reply_exp)
	return InvokeOk;
    assert (id != 0);
    CORBA::Boolean r = wait (id);
    assert (r);
    ORBRequest *dummy;
    GIOP::AddressingDisposition adummy;
    return get_invoke_reply (id, obj, dummy, adummy);
}

CORBA::LocateStatus
CORBA::ORB::locate (Object_ptr &obj)
{
    ORBMsgId id = locate_async (obj);
    CORBA::Boolean r = wait (id);
    assert (r);
    GIOP::AddressingDisposition adummy;
    return get_locate_reply (id, obj, adummy);
}

CORBA::LocateStatus
CORBA::ORB::bind (const char *repoid, const ObjectTag &oid,
		  Address *addr, Object_ptr &obj)
{
    ORBMsgId id = bind_async (repoid, oid, addr);
    if (id == 0)
	return LocateUnknown;
    CORBA::Boolean r = wait (id);
    assert (r);
    return get_bind_reply (id, obj);
}

CORBA::Object_ptr
CORBA::ORB::bind (const char *repoid, const ObjectTag &oid, const char *addr)
{
    vector<string> *addrs, _addrs;
    if (addr) {
	// use given address
	_addrs.push_back (addr);
	addrs = &_addrs;
    } else {
	// use default addresses
	addrs = &_bindaddrs;
    }

    Address *a;
    for (mico_vec_size_type i = 0; i < addrs->size(); ++i) {
	a = Address::parse ((*addrs)[i].c_str());
	if (a) {
	    Object_ptr obj;
	    LocateStatus s = bind (repoid, oid, a, obj);
	    delete a;
	    if (s == LocateHere)
		return obj;
	} else {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: bad bind address: " << (*addrs)[i] << endl;
	  }
	}
    }
    return Object::_nil();
}

CORBA::Object_ptr
CORBA::ORB::bind (const char *repoid, const char *addr)
{
    return bind (repoid, ObjectTag(), addr);
}

void
CORBA::ORB::cancel (ORBMsgId id)
{
    ORBInvokeRec *rec = this->get_invoke (id);
    if (rec) {
	rec->deactivate();
        if (rec->oa()) {
	    rec->oa()->cancel ( rec );
	}
#ifndef HAVE_THREADS
	del_invoke ( rec->id() );
#endif
    }
}

void
CORBA::ORB::cancel (MsgId id)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (rec) {
	rec->deactivate();
        if (rec->oa())
            rec->oa()->cancel ( rec );
#ifndef HAVE_THREADS
	del_invoke ( rec->id() );

        // XXX has to be changed for MT
        //_currentid = 0;
	if (!_currentid.empty()) {
	    _currentid.pop();
	}
#else // HAVE_THREADS
	stack<CORBA::ORBInvokeRec*>* invs = static_cast<stack<CORBA::ORBInvokeRec*>*>
	    (MICOMT::Thread::get_specific(_current_rec_key));
	if (invs != NULL && !invs->empty()) {
	    invs->pop();
	}
#endif // HAVE_THREADS
    }
}

#if 0
CORBA::Long
CORBA::ORB::wait (vector<MsgId> &ids, Long tmout)
{
    Timeout t (_disp, tmout);
    ORBInvokeRec *rec;

    while (42) {
	vector<MsgId> ready;
	for (mico_vec_size_type i = 0; i < ids.size(); ++i) {
	    rec = get_invoke (ids[i]);
	    if (!rec || rec->completed())
		ready.push_back (ids[i]);
	}
	if (ready.size() > 0) {
	    ids = ready;
	    return ids.size();
	}
	if (t.done())
	    return 0;
	_disp->run (FALSE);
    }
}
#endif

CORBA::Boolean
CORBA::ORB::wait (ORBMsgId id, Long tmout)
{
    if (MICO::Logger::IsLogged (MICO::Logger::ORB)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::ORB) 
	    << "ORB::wait for " << id << endl;
    }
    ORBInvokeRec *rec = get_invoke (id);

    //FIXME: this is still not right - but OK for the moment
    //       we have to behave the old way, when the ORB thread calls us !!!
#ifdef HAVE_THREADS
    if (rec
        && (MICO::MTManager::blocking_threaded_client()
            || (MICO::MTManager::threaded_client()
                && !MICO::GIOPConn::is_this_reader_thread()))) {
        // kcg: either blocking_threaded or threaded client concurrency
        // model is in use, in case of threaded, we're invoking wait
        // from one of client's thread instead of connection reader thread
        if (rec->active()) {
            assert( rec->callback() );
            return (rec->callback()->waitfor(this, id, CORBA::ORBCallback::Result, tmout));
        }
        return TRUE;
    }
    else if (rec
             && (MICO::MTManager::reactive_client()
                 || (MICO::MTManager::threaded_client()
                     && MICO::GIOPConn::is_this_reader_thread()))) {
        // kcg: either reactive or threaded client concurrency model
        // is used. In case of threaded, we're invoking wait from reader
        // thread, hence we can not block on waitfor above but rather wait
        // on connection's dispather and processing incoming replies this
        // way
#endif // HAVE_THREADS
    if (tmout == 0) {
      if (!rec || rec->completed()) {
	return TRUE;
      }
    }

#ifdef HAVE_THREADS
    Dispatcher* disp_for_waiting = NULL;
    MICO::IIOPProxyInvokeRec* proxy_invoke_rec = NULL;
    if (rec != NULL) {
        proxy_invoke_rec = (MICO::IIOPProxyInvokeRec*)rec->get_invoke_hint();
    }
    if (proxy_invoke_rec != NULL)
        disp_for_waiting = proxy_invoke_rec->conn()->dispatcher();
    else
        disp_for_waiting = this->_disp;
#else // HAVE_THREADS
    Dispatcher* disp_for_waiting = this->_disp;
#endif // HAVE_THREADS

    Timeout t (disp_for_waiting, tmout);

    while (42) {
      if (!rec || rec->completed())
	return TRUE;
      if (t.done())
	return FALSE;
      disp_for_waiting->run (FALSE);
      rec = get_invoke (id);
    }
#ifdef HAVE_THREADS
    }
    else {
        // never happen
        assert(rec != NULL);
        cerr << "client's concurrency model: " << MICO::MTManager::client_concurrency_model() << endl;
        assert(0);
    }
#endif // HAVE_THREADS
    // never happen
    assert(0);
    return FALSE;
}

CORBA::InvokeStatus
CORBA::ORB::get_invoke_reply (ORBMsgId id, Object_out obj, ORBRequest *&r,
			      GIOP::AddressingDisposition &ad)
{
    ORBInvokeRec *rec = get_invoke (id);
    assert (rec);

    InvokeStatus state;
    Object_ptr o;
    CORBA::Boolean ret = rec->get_answer_invoke (state, o, r, ad);
    assert (ret);
    obj = Object::_duplicate (o);
    del_invoke ( rec->id() );

#ifndef HAVE_THREADS
    // XXX has to be changed for MT
    //_currentid = 0;
    if (!_currentid.empty()) {
	_currentid.pop();
    }
#else // HAVE_THREADS
    stack<CORBA::ORBInvokeRec*>* invs = static_cast<stack<CORBA::ORBInvokeRec*>*>
	(MICOMT::Thread::get_specific(_current_rec_key));
    if (invs != NULL && !invs->empty()) {
	invs->pop();
    }
#endif // HAVE_THREADS
    return state;
}

CORBA::LocateStatus
CORBA::ORB::get_locate_reply (ORBMsgId id, Object_out obj,
			      GIOP::AddressingDisposition &ad)
{
    ORBInvokeRec *rec = get_invoke (id);
    assert (rec);

    LocateStatus state;
    Object_ptr o;
    CORBA::Boolean r = rec->get_answer_locate (state, o, ad);
    assert (r);
    obj = Object::_duplicate (o);
    del_invoke ( rec->id() );
    return state;
}

CORBA::LocateStatus
CORBA::ORB::get_bind_reply (ORBMsgId id, Object_out obj)
{
    ORBInvokeRec *rec = get_invoke (id);
    assert (rec);

    LocateStatus state;
    Object_ptr o;
    CORBA::Boolean r = rec->get_answer_bind (state, o);
    assert (r);
    obj = Object::_duplicate (o);
    del_invoke ( rec->id() );
    return state;
}

/*
 * BIG-FIXME: - stage one of async ORB-OA interface rewrite
 *
 * the next will be to pass *rec directly as ORBMsgId and provide
 * a way to assosiate OA specific information with an ORBMsgId
 *
 * some kind of setOAinfo(void *) and void *getOAinfo()
 *
 * (did i hear a scream from Kay ???) 
 */
void
CORBA::ORB::answer_invoke (ORBMsgId id, InvokeStatus stat, Object_ptr obj,
			   ORBRequest *req,
			   GIOP::AddressingDisposition ad)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (rec) {
	rec->set_answer_invoke (stat, obj, req, ad);
	if (rec->callback())
	    rec->callback()->notify (this, rec, ORBCallback::Invoke);
    }
}

void
CORBA::ORB::answer_bind (ORBMsgId id, LocateStatus stat, Object_ptr obj)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (rec) {
	rec->set_answer_bind (stat, obj);
	if (rec->callback())
	    rec->callback()->notify (this, rec, ORBCallback::Bind);
    }
}

void
CORBA::ORB::answer_locate (ORBMsgId id, LocateStatus stat, Object_ptr obj,
			   GIOP::AddressingDisposition ad)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (rec) {
	rec->set_answer_locate (stat, obj, ad);
	if (rec->callback())
	    rec->callback()->notify (this, rec, ORBCallback::Locate);
    }
}

CORBA::ORBRequest *
CORBA::ORB::request (ORBMsgId id)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (!rec)
	return 0;
    return rec->request ();
}

CORBA::RequestType
CORBA::ORB::request_type (ORBMsgId id)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (!rec)
	return RequestUnknown;
    return rec->request_type ();
}

void
CORBA::ORB::redo_request (ORBMsgId id)
{
    ORBInvokeRec *rec = get_invoke (id);
    if (rec && rec->active() ) {
	rec->redo();
    } else {
      if (MICO::Logger::IsLogged (MICO::Logger::Info)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Info)
	  << "Info: no such request " << id << " in redo_request" << endl;
      }
    }
}

CORBA::ImplementationDef_ptr
CORBA::ORB::get_impl (Object_ptr obj)
{
    // [12-17]
    Request_var req = obj->_request ("_implementation");
    req->result()->value()->set_type (CORBA::_tc_ImplementationDef);
    MICO_CATCHANY (req->invoke ());
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    ImplementationDef_ptr impl;
    CORBA::Boolean r = (*req->result()->value() >>= impl);
    assert (r);
    return ImplementationDef::_duplicate (impl);
}

CORBA::InterfaceDef_ptr
CORBA::ORB::get_iface (Object_ptr obj)
{
    // [12-17]
    Request_var req = obj->_request ("_interface");
    req->result()->value()->set_type (CORBA::_tc_InterfaceDef);
    MICO_CATCHANY(req->invoke ());
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    InterfaceDef_ptr iface;
    CORBA::Boolean r = (*req->result()->value() >>= iface);
    assert (r);
    return InterfaceDef::_duplicate (iface);
}

CORBA::Object_ptr
CORBA::ORB::get_component (Object_ptr obj)
{
    // [12-17]
    Request_var req = obj->_request ("_component");
    req->result()->value()->set_type (CORBA::_tc_Object);
    MICO_CATCHANY(req->invoke ());
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    Object_ptr comp;
    CORBA::Boolean r = (*req->result()->value() >>= comp);
    assert (r);
    return Object::_duplicate (comp);
}

CORBA::Boolean
CORBA::ORB::is_a (Object_ptr obj, const char *repo_id)
{
    MICOMT::AutoLock l(_isa_cache);

    // XXX this assumes RepoIds are globally unique
    if (*obj->_repoid()) {
      string key = string(obj->_repoid()) + "$" + repo_id;
      list<string>::iterator i;
      for (i = _isa_cache.begin(); i != _isa_cache.end(); ++i) {
        if (*i == key)
	  break;
      }
      if (i != _isa_cache.end()) {
        _isa_cache.erase (i);
        _isa_cache.push_front (key);
        return TRUE;
      }
    }

    // [12-17]
    Request_var req = obj->_request ("_is_a");
    req->add_in_arg ("logical_type_id") <<= repo_id;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    MICO_CATCHANY(req->invoke ());
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    Boolean res;
    CORBA::Boolean r = (*req->result()->value() >>= Any::to_boolean (res));
    assert (r);

    if (res) {
      if (!*obj->_repoid()) {
	obj->_ior()->objid (repo_id);
      }
      else {
	string key = string(obj->_repoid()) + "$" + repo_id;
        _isa_cache.push_front (key);
        // XXX CACHE size = 50
        if (_isa_cache.size() > 50)
	  _isa_cache.pop_back();
      }
    }
    return res;
}

CORBA::Boolean
CORBA::ORB::non_existent (Object_ptr obj)
{
    // [12-17]
    Request_var req = obj->_request ("_non_existent");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    MICO_CATCHANY(req->invoke ());
    Boolean res;
    if (req->env()->exception()) {
        // an exceptions means we cannot contact the object ...
        res = TRUE;
    } else {
        CORBA::Boolean r = (*req->result()->value() >>= Any::to_boolean (res));
	assert (r);
    }
    return res;
}

CORBA::ORBInvokeRec *
CORBA::ORB::get_current_invoke_rec ()
{
#ifdef HAVE_THREADS
    stack<ORBInvokeRec*>* invs = static_cast<stack<CORBA::ORBInvokeRec*>*>
	(MICOMT::Thread::get_specific(_current_rec_key));
    assert(invs != NULL && !invs->empty());
    return invs->top();
#else // HAVE_THREADS
    // XXX has to be changed for MT
    if (_currentid.empty()) {
        return 0;
    }
    return get_invoke (_currentid.top());
#endif // HAVE_THREADS
}


CORBA::Dispatcher*
CORBA::ORB::create_dispatcher()
{
    assert(this->dispatcher_factory_ != NULL);
    return this->dispatcher_factory_->create();
}


void
CORBA::ORB::dispatcher_factory(CORBA::DispatcherFactory* fact)
{
    if (this->dispatcher_factory_ != NULL)
        delete this->dispatcher_factory_;
    this->dispatcher_factory_ = fact;
}


/************************** PrincipalCurrent *************************/


CORBA::Principal_ptr
CORBA::PrincipalCurrent_impl::get_principal ()
{
    ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    ORBInvokeRec *rec = orb->get_current_invoke_rec();
    if (!rec)
        return CORBA::Principal::_nil();
    return CORBA::Principal::_duplicate (rec->principal());
}


/*********************** ORB initialization **************************/


CORBA::ORB_ptr
CORBA::ORB_instance (const char *id, Boolean create)
{
    if (strcmp ("mico-local-orb", id)) {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: unknown ORB id " << id << " in ORB_instance" << endl;
      }
      mico_throw (CORBA::INITIALIZE());
    }
    if (create && CORBA::is_nil (orb_instance)) {
	int argc = 0;
	char *argv = 0;
	return ORB_init (argc, &argv, id);
    }
    return ORB::_duplicate (orb_instance);
}

void __mtdebug_init();

CORBA::ORB_ptr
CORBA::ORB_init (int &argc, char **argv, const char *_id)
{
    CORBA::ULong pi_init_size = PInterceptor::PI::S_initializers_.size();
    PortableInterceptor::backup_initializers();
#ifdef HAVE_THREADS
    MICOMT::_init();
    MICO::GIOPConn::initialize_reader_key();
#endif
    OSNet::sock_init();

    Exception::_init ();
    Codeset::_init ();
    MICOPOA::_init ();
#ifdef HAVE_SSL
    MICOSSL::_init ();
#endif
#ifdef HAVE_BLUETOOTH
    MICOLTP::_init();
#endif
#ifdef USE_WIRELESS
    WATM::_init();
#endif
#if defined (_WIN32 ) || defined (_POCKET_PC)
    OSMisc::_init();
#if !defined(__MINGW32__) && !defined (_POCKET_PC)
    /*
     * This crashes with Mingw32. This is probably a problem of me not
     * understanding all that Win32 threading/process stuff (__FP__) and
     * should be checked by someone with a better understanding of Win32.
     * See the __MINGW32__ ifdefs in os-windows.cc.
     * This means that micod probably won't work (as expected).
     */
    MICO::UnixProcess::_init();
#endif
#endif
    PInterceptor::PI::_init();
    Boolean run_iiop_server = TRUE;
    Boolean run_iiop_proxy = TRUE;
    Boolean iiop_blocking = FALSE;
    Boolean plugged = TRUE;
    CORBA::Address *fwproxyaddr = 0;
    string imr_ior, imr_addr;
    string ir_ior, ir_addr;
    string naming_ior, naming_addr;
    string trading_ior, trading_addr;
    string mtb_addr_str;
    vector<string> debug_level;
    vector<string> bindaddrs, iiopaddrs;
    string rcfile;
#if defined(_WIN32)
    //getting the path where the file was executed
    char path[256];
    GetModuleFileName(0, path, 255);
    string xpath(path);
    int pos = xpath.find_last_of("\\");
    if (pos != string::npos)	
	xpath.erase(pos, xpath.length());
    xpath += "\\.micorc";
    rcfile = xpath;
#else
    // On UNIX we use standard .micorc in $HOME directory
    rcfile = "~/.micorc";    
#endif

    string id = _id;
    bool memtrace = false;
    vector<string> InitRefs;
    string DefInitRef;
    string terminal_id_str;
    CORBA::UShort giop_ver, iiop_ver;
    string giop_ver_str = "1.0";
    string iiop_ver_str = "1.0";
    string max_message_size_str;
#ifdef HAVE_POLL_H
    Boolean use_poll = FALSE;
#endif // HAVE_POLL_H
#ifdef HAVE_THREADS
    // Connection checking is disabled by default
    // for default thread-pool concurrency model
    // Other concurrency model have default connection limit
    // set to 10.
    ULong conn_limit = 0;
    ULong request_limit = 4; // The most common servers have up to four CPUs
    Boolean thread_pool = TRUE;
    Boolean thread_per_request = FALSE;
    Boolean thread_per_connection = FALSE;
    MICO::MTManager::ConcurrencyModel
        client_concurrency_model = MICO::MTManager::_S_threaded_client;
#endif // HAVE_THREADS
#ifdef MTDEBUG
    __mtdebug_init ();
#endif // MTDEBUG

#ifdef USE_CSIV2
    string client_user_name;
    string client_user_passwd;
    vector<string> server_users;
    vector<string> server_users_passwds;
    CORBA::Boolean use_csiv2 = FALSE;
    string realm_name;
    CSIv2::DistinguishedNameList tls_user_list;
    tls_user_list.length(0);
    CSIv2::UserIdentity t_user_id;
    CSIv2::UserIdentityList user_id_list;
    user_id_list.length(0);
    string client_identity;
    int tss_auth = 2;
    int tss_attr = 1;
    int css_auth = 1;
    int css_attr = 1;
#endif
    const char *rc = getenv ("MICORC");
    if (rc)
        rcfile = rc;

    // look for config file name ...
    {
	MICOGetOpt::OptMap opts;
	opts["-ORBConfFile"] = "arg-expected";

	MICOGetOpt opt_parser (opts);

	if (!opt_parser.parse (argc, argv, TRUE))
	    mico_throw (CORBA::INITIALIZE());
	const MICOGetOpt::OptVec &o = opt_parser.opts();

	MICOGetOpt::OptVec::const_iterator i;
	for (i = o.begin(); i != o.end(); ++i) {
	    string arg = (*i).first;
	    string val = (*i).second;
	    if (arg == "-ORBConfFile") {
		rcfile = val;
	    }
	}
    }

    MICOGetOpt::OptMap opts;
    opts["-ORBNoIIOPServer"]  = "";
    opts["-ORBNoIIOPProxy"]   = "";
    opts["-ORBIIOPAddr"]      = "arg-expected";
    opts["-ORBIIOPVersion"]   = "arg-expected";
    opts["-ORBGIOPVersion"]   = "arg-expected";
    opts["-ORBIIOPProxy"]     = "arg-expected";
    opts["-ORBIIOPBlocking"]  = "";
    opts["-ORBGIOPMaxSize"]   = "arg-expected";
    opts["-ORBId"]            = "arg-expected";
    opts["-ORBConnLimit"]     = "arg-expected";
    opts["-ORBRequestLimit"]  = "arg-expected";
    opts["-ORBImplRepoIOR"]   = "arg-expected";
    opts["-ORBImplRepoAddr"]  = "arg-expected";
    opts["-ORBIfaceRepoIOR"]  = "arg-expected";
    opts["-ORBIfaceRepoAddr"] = "arg-expected";
    opts["-ORBNamingIOR"]     = "arg-expected";
    opts["-ORBNamingAddr"]    = "arg-expected";
    opts["-ORBTradingIOR"]    = "arg-expected";
    opts["-ORBTradingAddr"]   = "arg-expected";
    opts["-ORBDebug"]         = "arg-expected";
    opts["-ORBBindAddr"]      = "arg-expected";
    opts["-ORBInitRef"]       = "arg-expected";
    opts["-ORBDefaultInitRef"]= "arg-expected";
#ifdef USE_WIRELESS
    opts["-ORBTerminalId"]    = "arg-expected";
    opts["-ORBMTBAddr"]       = "arg-expected";
#endif
    opts["-ORBMemTrace"]      = "";
    opts["-ORBNoResolve"]     = "";
    opts["-ORBThreadPool"]    = "";
    opts["-ORBThreadPerConnection"] = "";
    opts["-ORBThreadPerRequest"] = "";
    opts["-ORBClientReactive"] = "";
    opts["-ORBClientThreaded"] = "";
    opts["-ORBClientThreadedBlocking"] = "";
#ifdef HAVE_POLL_H
    opts["-ORBUsePoll"]       = "";
#endif // HAVE_POLL_H
#ifdef USE_CSIV2
    opts["-ORBGSSClientUser"] = "arg-expected";
    opts["-ORBGSSServerUser"] = "arg-expected";
    opts["-ORBCSIv2"] = "";
    opts["-ORBCSIv2Realm"] = "arg-expected";
    opts["-ORBTLSDName"] = "arg-expected";
    opts["-ORBUserIdentity"] = "arg-expected";
    opts["-ORBClientIdentity"] = "arg-expected";
    opts["-ORBTSSNoAuth"] = "";
    opts["-ORBTSSAuthSupported"] = "";
    opts["-ORBTSSAuthRequired"] = "";
    opts["-ORBTSSNoAttr"] = "";
    opts["-ORBTSSAttrSupported"] = "";
    opts["-ORBTSSAttrRequired"] = "";
    opts["-ORBCSSNoAuth"] = "";
    opts["-ORBCSSAuthSupported"] = "";
    opts["-ORBCSSAuthRequired"] = "";
    opts["-ORBCSSNoAttr"] = "";
    opts["-ORBCSSAttrSupported"] = "";
    opts["-ORBCSSAttrRequired"] = "";
#endif // USE_CSIV2

    MICOGetOpt opt_parser (opts);
    if (!opt_parser.parse (rcfile.c_str(), TRUE))
	mico_throw (CORBA::INITIALIZE());
    if (!opt_parser.parse (argc, argv, TRUE))
	mico_throw (CORBA::INITIALIZE());
    const MICOGetOpt::OptVec &o = opt_parser.opts ();

    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;
	if (arg == "-ORBNoIIOPServer") {
	    run_iiop_server = FALSE;
	} else if (arg == "-ORBNoIIOPProxy") {
	    run_iiop_proxy = FALSE;
	} else if (arg == "-ORBIIOPAddr") {
	    iiopaddrs.push_back (val);
        } else if (arg == "-ORBIIOPProxy") {
          fwproxyaddr = Address::parse (val.c_str());
        } else if (arg == "-ORBIIOPBlocking") {
          iiop_blocking = TRUE;
	} else if (arg == "-ORBId") {
	    if (id.length() == 0)
		id = val;
	} else if (arg == "-ORBImplRepoIOR") {
	    imr_ior = val;
	} else if (arg == "-ORBImplRepoAddr") {
	    imr_addr = val;
	} else if (arg == "-ORBIfaceRepoIOR") {
	    ir_ior = val;
	} else if (arg == "-ORBIfaceRepoAddr") {
	    ir_addr = val;
	} else if (arg == "-ORBNamingIOR") {
	    naming_ior = val;
	} else if (arg == "-ORBNamingAddr") {
	    naming_addr = val;
	} else if (arg == "-ORBTradingIOR") {
	    trading_ior = val;
	} else if (arg == "-ORBTradingAddr") {
	    trading_addr = val;
	} else if (arg == "-ORBDebug") {
	    debug_level.push_back (val);
	} else if (arg == "-ORBBindAddr") {
	    bindaddrs.push_back (val);
	} else if (arg == "-ORBInitRef") {
	    InitRefs.push_back (val);
	} else if (arg == "-ORBDefaultInitRef") {
	    DefInitRef = val;
#ifdef USE_WIRELESS
	} else if (arg == "-ORBTerminalId") {
	    terminal_id_str = val;
	} else if (arg == "-ORBMTBAddr") {
	    plugged = FALSE;
	    mtb_addr_str = val;
#endif
        } else if (arg == "-ORBMemTrace") {
            memtrace = true;
	} else if (arg == "-ORBNoResolve") {
	    MICO::InetAddress::resolve (FALSE);
	} else if (arg == "-ORBGIOPVersion") {
            giop_ver_str = val;
        } else if (arg == "-ORBIIOPVersion") {
            iiop_ver_str = val;
	} else if (arg == "-ORBGIOPMaxSize") {
	    max_message_size_str = val;
#ifdef HAVE_POLL_H
	} else if (arg == "-ORBUsePoll") {
	    use_poll = TRUE;
#endif // HAVE_POLL_H
	}
#ifdef HAVE_THREADS
        else if (arg == "-ORBThreadPool") {
	    thread_pool = TRUE;
	    thread_per_connection = FALSE;
	    thread_per_request = FALSE;	    
	    if (conn_limit != 0)
		conn_limit = 0;
	} else if (arg == "-ORBThreadPerConnection") {
	    thread_pool = FALSE;	    
	    thread_per_connection = TRUE;
	    thread_per_request = FALSE;
	    if (conn_limit < 1)
		conn_limit = 10;
	} else if (arg == "-ORBThreadPerRequest") {
	    thread_pool = FALSE;
	    thread_per_connection = FALSE;
	    thread_per_request = TRUE;
	    if (conn_limit < 1)
		conn_limit = 10;
	} else if (arg == "-ORBConnLimit") {
	    conn_limit = atoi (val.c_str ());
	} else if (arg == "-ORBRequestLimit") {
	    request_limit = atoi (val.c_str ());
	} else if (arg == "-ORBClientReactive") {
            client_concurrency_model = MICO::MTManager::_S_reactive_client;
	} else if (arg == "-ORBClientThreaded") {
            client_concurrency_model = MICO::MTManager::_S_threaded_client;
        } else if (arg == "-ORBClientThreadedBlocking") {
            client_concurrency_model = MICO::MTManager::_S_blocking_threaded_client;
        }
#endif // HAVE_THREADS
#ifdef USE_CSIV2
	else if (arg == "-ORBGSSClientUser") {
	    string s = val;
	    int index = s.find_first_of(',');
	    client_user_name = s.substr(0, index);
	    client_user_passwd = s.substr(index+1, s.length());
	} else if (arg == "-ORBGSSServerUser") {
	    string s = val;
	    int index = s.find_first_of(',');
	    string name = s.substr(0, index);
	    string passwd = s.substr(index+1, s.length());
	    server_users.push_back(name);
	    server_users_passwds.push_back(passwd);
	} else if (arg == "-ORBCSIv2") {
	    use_csiv2 = TRUE;
	} else if (arg == "-ORBCSIv2Realm") {
	    realm_name = val;
	} else if (arg == "-ORBTLSDName") {
	    CSIv2::DistinguishedName_var name = val.c_str();
	    tls_user_list.length(tls_user_list.length()+1);
	    tls_user_list[tls_user_list.length()-1] = name;
	} else if (arg == "-ORBUserIdentity") {
	    string s = val;
	    int index = s.find_first_of(',');
	    string t_str;
	    t_str = s.substr(0, index);
	    t_user_id.user_name = t_str.c_str();
	    t_str = s.substr(index+1, s.length());
	    t_user_id.identity_name = t_str.c_str();
	    user_id_list.length(user_id_list.length()+1);
	    user_id_list[user_id_list.length()-1] = t_user_id;
	} else if (arg == "-ORBClientIdentity") {
	    client_identity = val;
	} else if (arg == "-ORBTSSNoAuth") {
	    tss_auth = 0;
	} else if (arg == "-ORBTSSAuthSupported") {
	    tss_auth = 1;
	} else if (arg == "-ORBTSSAuthRequired") {
	    tss_auth = 2;
	} else if (arg == "-ORBTSSNoAttr") {
	    tss_attr = 0;
	} else if (arg == "-ORBTSSAttrSupported") {
	    tss_attr = 1;
	} else if (arg == "-ORBTSSAttrRequired") {
	    tss_attr = 2;
	} else if (arg == "-ORBCSSNoAuth") {
	    css_auth = 0;
	} else if (arg == "-ORBCSSAuthSupported") {
	    css_auth = 1;
	} else if (arg == "-ORBCSSAuthRequired") {
	    css_auth = 2;
	} else if (arg == "-ORBCSSNoAttr") {
	    css_attr = 0;
	} else if (arg == "-ORBCSSAttrSupported") {
	    css_attr = 1;
	} else if (arg == "-ORBCSSAttrRequired") {
	    css_attr = 2;
	}
#endif // USE_CSIV2
    }

    // setup debugging
    for (CORBA::ULong debugs=0; debugs<debug_level.size(); debugs++) {
      size_t pos = debug_level[debugs].find ('=');
      string names, file;
      const char * fname;

      if (pos != (size_t) -1) {
	names = debug_level[debugs].substr (0, pos);
	file  = debug_level[debugs].substr (pos+1, (size_t) -1);
	fname = file.c_str();
      }
      else {
	names = debug_level[debugs];
	fname = 0;
      }

      while (names.length() > 0) {
	string name;
	if ((pos = names.find (',')) == (size_t) -1) {
	  name = names;
	  names = "";
	}
	else {
	  name  = names.substr (0, pos);
	  names = names.substr (pos+1);
	}

	MICO::Logger::Log (name.c_str(), 1, fname);
      }
    }

    if (id.length() > 0 && strcmp ("mico-local-orb", id.c_str())) {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: unknown ORB id " << id << " in ORB_init" << endl;
      }
      mico_throw (CORBA::INITIALIZE());
    }

    if (!CORBA::is_nil (orb_instance))
	return ORB::_duplicate (orb_instance);

    // create ORB
    orb_instance = new ORB (argc, argv, rcfile.c_str());

    // use poll dispatcher?
#ifdef HAVE_POLL_H
    if (use_poll) {
        orb_instance->dispatcher_factory(new MICO::PollDispatcherFactory);
    }
    else {
#endif // HAVE_POLL_H
        orb_instance->dispatcher_factory(new MICO::SelectDispatcherFactory);
#ifdef HAVE_POLL_H
    }
#endif // HAVE_POLL_H

    orb_instance->dispatcher(orb_instance->create_dispatcher());

#ifdef HAVE_THREADS
    // seting main thread id is important for perform_work operation
    // which can be invoked only in main thread
    orb_instance->set_main_thread_id(MICOMT::Thread::self());

    orb_instance->resource_manager ().connection_limit (conn_limit);
    orb_instance->resource_manager ().request_limit (request_limit);
    if (thread_pool) {
	MICO::MTManager::server_concurrency_model(MICO::MTManager::_S_thread_pool);
	if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream(MICO::Logger::Info)
	    << "Using thread-pool concurrency model." << endl;
	}
    }
    else {
	// this is a hack because MICO::SelectDispatcher::_isblocking
        // is private static and method block is not static
        CORBA::Dispatcher* disp;
#ifdef HAVE_POLL_H
        if (!use_poll)
#endif // HAVE_POLL_H
            disp = new MICO::SelectDispatcher;
#ifdef HAVE_POLL_H
        else
            disp = new MICO::PollDispatcher; 
#endif // HAVE_POLL_H
        disp->block(TRUE);
        delete disp;
	if (thread_per_connection) {
	    MICO::MTManager::server_concurrency_model(MICO::MTManager::_S_thread_per_connection);
	    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
		MICOMT::AutoDebugLock lock;
		MICO::Logger::Stream(MICO::Logger::Info)
		    << "Using thread-per-connection concurrency model." << endl;
	    }
	}
	else if (thread_per_request) {
	    MICO::MTManager::server_concurrency_model(MICO::MTManager::_S_thread_per_request);
	    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
		MICOMT::AutoDebugLock lock;
		MICO::Logger::Stream(MICO::Logger::Info)
		    << "Using thread-per-request concurrency model." << endl;
	    }
	}
	else {
	    // unknown concurrency model
	    assert(0);
	}
    }
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
        MICOMT::AutoDebugLock lock;
        MICO::Logger::Stream(MICO::Logger::Info)
            << "Using client concurrency model: "; 
        if (client_concurrency_model == MICO::MTManager::_S_reactive_client) {
            MICO::Logger::Stream(MICO::Logger::Info)
                << "reactive" << endl;
        }
        else if (client_concurrency_model == MICO::MTManager::_S_threaded_client) {
            MICO::Logger::Stream(MICO::Logger::Info)
                << "threaded" << endl;
        }
        else if (client_concurrency_model == MICO::MTManager::_S_blocking_threaded_client) {
            MICO::Logger::Stream(MICO::Logger::Info)
                << "threaded/blocking" << endl;
        }
        else {
            assert(0);
        }
    }
    MICO::MTManager::client_concurrency_model(client_concurrency_model);
    MICO::MTManager::thread_setup (conn_limit, request_limit);
#endif

    // interceptors
#ifdef USE_CSL2
#ifdef HAVE_SSL
//    AuditClientInterceptor* client_intcpt = new AuditClientInterceptor;
//    client_intcpt -> activate (0);
 
//    AuditServerInterceptor* server_intcpt = new AuditServerInterceptor;
//    server_intcpt -> activate (0);
    MICOSL2::_init();
    MICOSODM::_init();
    MICOSDM::_init();
    AuditServerInitializer * ini = new AuditServerInitializer;
    PortableInterceptor::register_orb_initializer(ini);
#endif // HAVE_SSL
#endif // USE_CSL2
#ifdef USE_CSIV2
    // CSIv2 uses IOR profile components
    // but doesn't support comoponents
    // in MultipleComponent profile and so
    // we have to use GIOP 1.1 and greater
    // so why not to use full GIOP 1.2? :-)
    if (use_csiv2) {
	if (giop_ver_str == "1.0") 
	    giop_ver_str = "1.2";
	if (iiop_ver_str == "1.0")
	    iiop_ver_str = "1.2";
    }
#endif // USE_CSIV2
    if (giop_ver_str.length() != 3 ||
        giop_ver_str[0] != '1' ||
        giop_ver_str[1] != '.' ||
        giop_ver_str[2] < '0' || giop_ver_str[2] > '2') {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: ORB_init(): invalid GIOP version: "
	  << giop_ver_str << endl;
      }
      mico_throw (CORBA::INITIALIZE());
    }
    giop_ver = ((CORBA::UShort)(giop_ver_str[0] - '0') << 8) |
        (giop_ver_str[2] - '0');
    
    if (iiop_ver_str.length() != 3 ||
        iiop_ver_str[0] != '1' ||
        iiop_ver_str[1] != '.' ||
        iiop_ver_str[2] < '0' || iiop_ver_str[2] > '2') {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: ORB_init(): invalid IIOP version: "
	  << iiop_ver_str << endl;
      }
      mico_throw (CORBA::INITIALIZE());
    }
    iiop_ver = ((CORBA::UShort)(iiop_ver_str[0] - '0') << 8) |
        (iiop_ver_str[2] - '0');

#ifdef USE_CSIV2
    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	CORBA::ULong i;
	for (i=0; i<tls_user_list.length(); i++)
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "CSIv2: added TLS user name: `" << tls_user_list[i]
		<< "'" << endl;
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2: added client user name: `" << client_user_name
	    << "' passwd: `" << client_user_passwd << "'" << endl;
	for (i=0; i<server_users.size(); i++)
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "CSIv2: added server user name: `" << server_users[i]
		<< "' passwd: `" << server_users_passwds[i] << "'" << endl;
	for (i=0; i<user_id_list.length(); i++)
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "CSIv2: added user identity: `"
		<< user_id_list[i].identity_name
		<< "' for user: `" << user_id_list[i].user_name
		<< "'" << endl;
    }
    CSIv2::SecurityManager_impl* csiv2_manager = NULL;
    csiv2_manager = new CSIv2::SecurityManager_impl;
    csiv2_manager->tss()->auth_layer(tss_auth);
    csiv2_manager->tss()->attr_layer(tss_attr);
    csiv2_manager->css()->auth_layer(css_auth);
    csiv2_manager->css()->attr_layer(css_attr);
    csiv2_manager->client_identity(client_identity.c_str());
    if (use_csiv2) {
	csiv2_manager->csiv2(TRUE);
    }
    CSIv2::InterceptorInitializer_impl* csiv2_initializer
	= new CSIv2::InterceptorInitializer_impl(csiv2_manager);
    PortableInterceptor::register_orb_initializer(csiv2_initializer);
    vector<string>::iterator it = server_users.begin();
    vector<string>::iterator jt = server_users_passwds.begin();
    for ( ; it!=server_users.end(), jt!=server_users_passwds.end();
	  it++, jt++) {
	csiv2_manager->add_server_user
	    ((char*)(*it).c_str(), (char*)(*jt).c_str());
    }
    if (client_user_name != "")
	csiv2_manager->client_user((char*)client_user_name.c_str(),
				     (char*)client_user_passwd.c_str());
    csiv2_manager->giop_version(giop_ver);
    csiv2_manager->tls_user_list(tls_user_list);
    csiv2_manager->user_id_list(user_id_list);
    orb_instance->set_initial_reference
	("CSIv2SecurityManager", csiv2_manager);
#endif // USE_CSIV2
    CORBA::Boolean retval =
	Interceptor::InitInterceptor::_exec_initialize (orb_instance,
							"mico-local-orb",
							argc, argv);
    // XXX what to do on error ??
    if (!retval)
	mico_throw (CORBA::INITIALIZE());

    // PI: invoke pre_init here. post_init is invoked below after resolving
    // all references
    PInterceptor::ORBInitInfo_impl* info = NULL;
    if (PInterceptor::PI::S_initializers_.size() != 0) {
	info = new PInterceptor::ORBInitInfo_impl(orb_instance, id.c_str(),
						  argc, argv);
	// kcg: we ``prioritize'' initializers registered during ORB_init
	// call here, so they are called first and then all other initializers
	// registered before ORB_init call
	for (CORBA::ULong i = pi_init_size;
	     i < PInterceptor::PI::S_initializers_.size();
	     i++) {
	    PInterceptor::PI::S_initializers_[i]->pre_init(info);
	}
	for (CORBA::ULong i = 0; i < pi_init_size; i++) {
	    PInterceptor::PI::S_initializers_[i]->pre_init(info);
	}
    }
#ifdef USE_CSL2
    MICOGetOpt::OptMap csl2_opts;
    // here we need to remove CSL2 parameters, since CSL2 interceptor initializers
    // don't have any chance to remove CSL2's parameters from supplied
    // argv list.
    csl2_opts["-AuditType"] = "arg-expected";
    csl2_opts["-AuditArchName"] = "arg-expected";
    csl2_opts["-AccessControl"]  = "arg-expected";  
    csl2_opts["-AccessConfig"]  = "arg-expected";
    csl2_opts["-AuditConfig"] = "arg-expected";
    csl2_opts["-ODMConfig"]  = "arg-expected";
    csl2_opts["-RightsConfig"] = "arg-expected";
    csl2_opts["-Paranoid"] = "arg-expected";
    MICOGetOpt csl2_opt_parser (csl2_opts);
    if (!csl2_opt_parser.parse (rcfile.c_str(), TRUE))
	mico_throw (CORBA::INITIALIZE());
    if (!csl2_opt_parser.parse (argc, argv, TRUE))
	mico_throw (CORBA::INITIALIZE());
#endif // USE_CSL2
    // set default bind addresses
    orb_instance->bindaddrs (bindaddrs);


#ifdef USE_CSIV2
    if (use_csiv2) {
	if (realm_name != "")
	    csiv2_manager->realm_name((char*)realm_name.c_str());
    }
#endif // USE_CSIV2

    // set maximum size for GIOP messages

    CORBA::ULong max_message_size = 0;
    if (max_message_size_str.length() > 0) {
      const char *p = max_message_size_str.c_str();
      while (*p) {
	switch (*p) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  max_message_size = 10*max_message_size + (int) (*p - '0');
	  break;
	case 'k': case 'K':
	  max_message_size *= 1024;
	  break;
	case 'm': case 'M':
	  max_message_size *= 1024*1024;
	  break;
	default:
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock lock;  
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: ORB_init(): illegal maximum message size "
	      << max_message_size_str << endl;
	  }
	  mico_throw (CORBA::INITIALIZE());
	}
	p++;
      }
    }

    // set plugging status, terminal identifier, and redirect address
    orb_instance->plugged(plugged);
    if (!terminal_id_str.empty()) {
	ULong len = terminal_id_str.length();
	const Octet *str = mico_url_decode(terminal_id_str.c_str(), len);
	orb_instance->terminal_id(str, len);
    }

    CORBA::Address *mtb_addr = NULL;
    if (!mtb_addr_str.empty()) {
	mtb_addr = CORBA::Address::parse(mtb_addr_str.c_str());
    }

    // create IIOP client
    orb_instance->dispatcher()->block (iiop_blocking);
    if (run_iiop_proxy) {
	iiop_proxy_instance = new MICO::IIOPProxy (orb_instance, giop_ver,
						   max_message_size);
	if (!orb_instance->plugged()) {
	    iiop_proxy_instance->redirect(mtb_addr);
	}
    }

    // create IIOP server
    if (run_iiop_server) {
	iiop_server_instance = new MICO::IIOPServer (orb_instance,
						     iiop_ver,
						     max_message_size);
	// server->set_conn_limit(conn_limit);
	for (mico_vec_size_type i = 0; i < iiopaddrs.size(); ++i) {
	    Address *addr = Address::parse (iiopaddrs[i].c_str());
	    if (!addr) {
	      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
		MICOMT::AutoDebugLock lock;
		MICO::Logger::Stream (MICO::Logger::Error)
		  << "Error: ORB_init(): bad address: "
		  << iiopaddrs[i] << endl;
	      }
	      mico_throw (CORBA::INITIALIZE());
	    }
	    // ###ras Bug???
	    if (!iiop_server_instance->listen (addr, fwproxyaddr))
		mico_throw (CORBA::INITIALIZE());
	    delete addr;
	}
	if (iiopaddrs.size() == 0) {
	    iiop_server_instance->listen ();
	}
    } else {
	orb_instance->ior_template()->add_profile (
	    new MICO::LocalProfile ((Octet *)"", 1));
    }

    // connect to / create implementation repository
    Object_var imr;
    if (imr_ior.length() > 0) {
	imr = orb_instance->string_to_object (imr_ior.c_str());
    } else if (imr_addr.length() > 0) {
	imr = orb_instance->bind ("IDL:omg.org/CORBA/ImplRepository:1.0",
				  imr_addr.c_str());
        if (CORBA::is_nil (imr)) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: cannot bind to Implementation Repository at "
	      << imr_addr.c_str() << "." << endl
	      << "Warning: will use a local Implementation Repository"
	      << endl;
	  }
        }
    }

    if (!CORBA::is_nil (imr)) {
      orb_instance->set_initial_reference ("ImplementationRepository", imr);
    }
    else {
      // XXX Hack
      orb_instance->set_initial_reference ("ImplementationRepository",
					   CORBA::Object::_nil());
    }

    // connect to / create interface repository
    Object_var ir;
    if (ir_ior.length() > 0) {
	ir = orb_instance->string_to_object (ir_ior.c_str());
    } else if (ir_addr.length() > 0) {
	ir = orb_instance->bind ("IDL:omg.org/CORBA/Repository:1.0",
				 ir_addr.c_str());
        if (CORBA::is_nil (ir)) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: cannot bind to Interface Repository at "
	      << ir_addr.c_str() << "." << endl
	      << "Warning: will use a local Interface Repository" << endl;
	  }
        }
    }

    if (!CORBA::is_nil (ir)) {
	orb_instance->set_initial_reference ("InterfaceRepository", ir);
    }
    else {
        // XXX Hack
	orb_instance->set_initial_reference ("InterfaceRepository",
					     CORBA::Object::_nil ());
    }

    // connect to naming service
    Object_var naming;
    if (naming_ior.length() > 0) {
	naming = orb_instance->string_to_object (naming_ior.c_str());
    } else if (naming_addr.length() > 0) {
	CORBA::ORB::ObjectTag_var tag = CORBA::ORB::string_to_tag ("NameService");
	naming = orb_instance->bind ("IDL:omg.org/CosNaming/NamingContext:1.0",
				     tag.in(), naming_addr.c_str());
        if (CORBA::is_nil (naming)) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: cannot bind to Naming Service at "
	      << naming_addr.c_str() << "." << endl;
	  }
        }
    }
    if (!CORBA::is_nil (naming)) {
	orb_instance->set_initial_reference ("NameService", naming);
    }

    // connect to trading service
    Object_var trading;
    if (trading_ior.length() > 0) {
	trading = orb_instance->string_to_object (trading_ior.c_str());
    } else if (trading_addr.length() > 0) {
	trading = orb_instance->bind ("IDL:omg.org/CosTrading/Lookup:1.0",
				     trading_addr.c_str());
        if (CORBA::is_nil (trading)) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	    MICOMT::AutoDebugLock lock;
	    MICO::Logger::Stream (MICO::Logger::Warning)
	      << "Warning: cannot bind to Trading Service at "
	      << trading_addr.c_str() << "." << endl;
	  }
	}
    }
    if (!CORBA::is_nil (trading)) {
	orb_instance->set_initial_reference ("TradingService", trading);
    }

    // Resolve -ORBInitRefs (must do that after connecting to NameService)
    for (CORBA::ULong iref=0; iref<InitRefs.size(); iref++) {
      string id, loc;
      size_t pos = InitRefs[iref].find ('=');
      if (pos == (size_t) -1) {
	mico_throw (CORBA::INITIALIZE());
      }
      id = InitRefs[iref].substr (0, pos);
      loc = InitRefs[iref].substr (pos+1, (size_t) -1);
      CORBA::Object_var iro = orb_instance->string_to_object (loc.c_str());

      if (CORBA::is_nil (iro)) {
	if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Warning)
	    << "Warning: cannot connect initial reference " << id
	    << " at " << loc << endl;
	}
      }
      else {
	orb_instance->set_initial_reference (id.c_str(), iro.in());
      }
    }

    if (DefInitRef.length()) {
      orb_instance->set_default_initial_reference (DefInitRef.c_str());
    }

#ifdef USE_WIRELESS
    if (!orb_instance->plugged()) {
	/*
	 * Slight problem here due to legacy code: if MTBAddr is
	 * specified but TerminalId is not, I don't know what is going
	 * to happen.  But this is a bug anyway, so don't bother with
	 * it for now.
	 */
	ULong len;
	const Octet *tid = orb_instance->terminal_id(len);
	orb_instance->ior_template()->add_profile
	    (new WATM::WATMProfile((Octet*)"", 1, tid, len, MultiComponent()));
    }

    if (!orb_instance->plugged()) {
	IORProfile *prof;
	for (ULong i = 0; (prof = orb_instance->ior_template()->get_profile(i)) != NULL;
	     ++i) {
	    prof->prepare_mobile();
	}
    }
#endif

    // Register factories for standard valuetypes
    ValueFactoryBase_var svf = new StringValue_Factory;
    ValueFactoryBase_var wsvf = new WStringValue_Factory;
    orb_instance->register_value_factory ("IDL:omg.org/CORBA/StringValue:1.0",
					  svf);
    orb_instance->register_value_factory ("IDL:omg.org/CORBA/WStringValue:1.0",
					  wsvf);

    // PI: invoke post_init
    if (info != NULL) {
	// kcg: we ``prioritize'' initializers registered during ORB_init
	// call here, so they are called first and then all other initializers
	// registered before ORB_init call
	for (CORBA::ULong i = pi_init_size;
	     i < PInterceptor::PI::S_initializers_.size();
	     i++) {
	    PInterceptor::PI::S_initializers_[i]->post_init(info);
	}
	for (CORBA::ULong i = 0; i < pi_init_size; i++) {
	    PInterceptor::PI::S_initializers_[i]->post_init(info);
	}
	delete info;
    }
    // PI: commit all registered interceptor, so they will be really
    // used from this time
    PortableInterceptor::commit_initialized_interceptors();
#ifdef USE_MEMTRACE
    if (argc > 0 && argv != NULL && *argv != NULL) {
      MemTrace_Init (*argv, (memtrace) ? MEMTRACE_REPORT_ON_EXIT : 0);
      MemTrace_Flush ();
    }
#else
    if (memtrace) {
      if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Warning)
	      << "warning: MemTrace not available" << endl;
      }
    }
#endif	    

    return CORBA::ORB::_duplicate (orb_instance);
}

