/*
 *  Implementation of COSS Event Service for MICO
 *  Copyright (C) 1997 Kai-Uwe Sattler, Kay Roemer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#define MICO_CONF_IMR
#define MICO_CONF_IR

#include <CORBA.h>
#include "CosEvent_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif


using namespace std;

//------------------------------------------------------------------------

ProxyPushConsumer_impl::ProxyPushConsumer_impl (EventChannel_impl *impl)
{
    channel = impl;
    connected = TRUE;
}

void ProxyPushConsumer_impl::connect_push_supplier 
(CosEventComm::PushSupplier_ptr push_supplier)
{
    if (!CORBA::is_nil (supplier) && !CORBA::is_nil (push_supplier))
        mico_throw (CosEventChannelAdmin::AlreadyConnected ());
    supplier = CosEventComm::PushSupplier::_duplicate (push_supplier);
}

void ProxyPushConsumer_impl::push (const CORBA::Any& data)
{
    if (!connected)
        mico_throw (CosEventComm::Disconnected ());

    channel->notify (data);
}

void ProxyPushConsumer_impl::disconnect_push_consumer ()
{
    if (!CORBA::is_nil (supplier)) {
#ifdef HAVE_EXCEPTIONS
      try {
#endif
	supplier->disconnect_push_supplier();
#ifdef HAVE_EXCEPTIONS
      } catch (...) {
      }
#endif
    }
    supplier = CosEventComm::PushSupplier::_nil();
    connected = FALSE;
}

//------------------------------------------------------------------------

ProxyPushSupplier_impl::ProxyPushSupplier_impl (EventChannel_impl *impl)
{
    channel = impl;
}

void ProxyPushSupplier_impl::connect_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer)
{
    if (CORBA::is_nil(push_consumer))
        return;
    if (!CORBA::is_nil (consumer))
        mico_throw (CosEventChannelAdmin::AlreadyConnected ());
    consumer = CosEventComm::PushConsumer::_duplicate (push_consumer);
    channel->listen (TRUE);
}

void ProxyPushSupplier_impl::disconnect_push_supplier ()
{
    if (!CORBA::is_nil (consumer)) {
#ifdef HAVE_EXCEPTIONS
      try {
#endif
        consumer->disconnect_push_consumer();
#ifdef HAVE_EXCEPTIONS
      } catch (...) {
      }
#endif
    }
    int was_connected = !CORBA::is_nil (consumer);
    consumer = CosEventComm::PushConsumer::_nil();
    if (was_connected)
	channel->listen (FALSE);
}

void ProxyPushSupplier_impl::notify (const CORBA::Any &any)
{
    MICOMT::AutoLock lock(requests);
    if (CORBA::is_nil (consumer) ||
        requests.size() >= channel->max_queue_size())
        return;

    CORBA::Request_ptr req = consumer->_request ("push");
    requests.push_back (req);
    req->add_in_arg ("data") <<= any;
    req->send_deferred (this);
}

void
ProxyPushSupplier_impl::callback (CORBA::Request_ptr req2,
                                  CORBA::RequestCallback::Event ev)
{
    assert (ev == CORBA::RequestCallback::RequestDone);
    MICOMT::AutoLock lock(requests);
    list<CORBA::Request_var>::iterator j = requests.begin();
    for (; j != requests.end(); ++j) {
        CORBA::Request_ptr req = (*j);
        if (req2 == req) {
            MICO_CATCHANY (req->get_response ());
            if (req->env()->exception()) {
		cerr << "eventd: push failed with: "
		     << req->env()->exception() << endl;
                disconnect_push_supplier();
	    }
            requests.erase (j);
            break;
        }
    }
}

//------------------------------------------------------------------------

PullSupplier_skel2::PullSupplier_skel2 ()
{
}

PullSupplier_skel2::PullSupplier_skel2 (CORBA::Object_ptr _obj)
{
}

PullSupplier_skel2::~PullSupplier_skel2()
{
}

void
PullSupplier_skel2::invoke(CORBA::StaticServerRequest_ptr _req)
{
  if( strcmp( _req->op_name(), "pull" ) == 0 ) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      pull (_req);
#ifdef HAVE_EXCEPTIONS
  } catch (CosEventComm::Disconnected &_ex) {
    _req->set_exception(_ex->_clone());
    _req->write_results ();
    return;
  }
#endif
    return;
  }
  POA_CosEventComm::PullSupplier::invoke(_req);
  return;
}

//------------------------------------------------------------------------

ProxyPullSupplier_impl::ProxyPullSupplier_impl (EventChannel_impl *impl)
{
    channel = impl;
    connected = TRUE;
}

void
ProxyPullSupplier_impl::connect_pull_consumer (
    CosEventComm::PullConsumer_ptr pull_consumer)
{
    if (!CORBA::is_nil (consumer))
        mico_throw (CosEventChannelAdmin::AlreadyConnected());

    consumer = CosEventComm::PullConsumer::_duplicate (pull_consumer);
    connected = TRUE;
}

void
ProxyPullSupplier_impl::disconnect_pull_supplier ()
{
    if (!CORBA::is_nil (consumer) && !consumer->_non_existent()) {
	consumer->disconnect_pull_consumer();
    }
    consumer = CosEventComm::PullConsumer::_nil();
    if (connected == TRUE) {
	MICOMT::AutoLock r_lock(requests);
	MICOMT::AutoLock e_lock(events);

        connected = FALSE;

	if (requests.size() > 0)
	  channel->listen(FALSE);

	list<CORBA::StaticServerRequest_var>::iterator i = requests.begin();
        for (; i!=requests.end(); ++i) {
	  (*i)->set_exception(new CosEventComm::Disconnected());
	  (*i)->write_results ();
	}

	requests.clear();
	events.clear();
    }
}


void
ProxyPullSupplier_impl::invoke(CORBA::StaticServerRequest_ptr req)
{
    PullSupplier_skel2::invoke(req);
    return;
}


void
ProxyPullSupplier_impl::pull (CORBA::StaticServerRequest_ptr req)
{
    if (!connected)
        mico_throw (CosEventComm::Disconnected());

    MICOMT::AutoLock requests_lock(requests);
    MICOMT::AutoLock events_lock(events);

    assert(requests.size()==0 || events.size()==0);

    // _duplicate()ing the request will defer completion
    requests.push_back (CORBA::StaticServerRequest::_duplicate (req));

    if (requests.size() == 1)
         channel->listen (TRUE);

    if (events.size() > 0 && requests.size() > 0) {
        CORBA::Any any = events.front();
        events.pop_front();
        notify (any);
    }
}

CORBA::Any*
ProxyPullSupplier_impl::pull ()
{
    assert (0);
    return 0;
}

CORBA::Any*
ProxyPullSupplier_impl::try_pull (CORBA::Boolean &has_event)
{
    if (!connected)
        mico_throw (CosEventComm::Disconnected());

    MICOMT::AutoLock requests_lock(requests);
    MICOMT::AutoLock events_lock(events);

    if (events.size() > 0) {
        CORBA::Any *a = new CORBA::Any (events.front());
        events.pop_front();
        has_event = TRUE;
        return a;
    }

    if (requests.size() == 0) {
	// try to get an event ...
	channel->listen (TRUE);
	channel->listen (FALSE);
    }

    has_event = FALSE;
    return new CORBA::Any;
}

void
ProxyPullSupplier_impl::notify (const CORBA::Any &any)
{
    MICOMT::AutoLock requests_lock(requests);
    MICOMT::AutoLock events_lock(events);

    if (!connected || events.size() >= channel->max_queue_size())
        return;
  
    if (requests.size() == 0) {
        events.push_back (any);
	return;
    }
  
    CORBA::StaticServerRequest_var svreq =
        CORBA::StaticServerRequest::_duplicate (requests.front());
    requests.pop_front();

    if (requests.size() == 0)
        channel->listen(FALSE);
  
    CORBA::StaticAny _res (CORBA::_stc_any, &any);
    svreq->set_result (&_res);
    if (!svreq->read_args())
	assert (0);
    svreq->write_results();
  
    // when leaving scope svreq will be released which will anwer the request
}

//------------------------------------------------------------------------

ProxyPullConsumer_impl::ProxyPullConsumer_impl (EventChannel_impl *impl)
{
    channel = impl;
}

void
ProxyPullConsumer_impl::connect_pull_supplier (
    CosEventComm::PullSupplier_ptr pull_supplier)
{
    if (CORBA::is_nil(pull_supplier))
        return;
    if (!CORBA::is_nil (supplier))
        mico_throw (CosEventChannelAdmin::AlreadyConnected());

    supplier = CosEventComm::PullSupplier::_duplicate (pull_supplier);

    if (channel->listen()) {
	request = supplier->_request ("pull");
	request->result()->value()->set_type (CORBA::_tc_any);
	request->send_deferred (this);
    }
}

void
ProxyPullConsumer_impl::disconnect_pull_consumer ()
{
    if (!CORBA::is_nil (supplier) && !supplier->_non_existent()) {
	supplier->disconnect_pull_supplier();
    }
    supplier = CosEventComm::PullSupplier::_nil();
    request = CORBA::Request::_nil();
}

void
ProxyPullConsumer_impl::callback (CORBA::Request_ptr req,
                                  CORBA::RequestCallback::Event ev)
{
    assert (ev == CORBA::RequestCallback::RequestDone);
    assert (req == request.in());

    MICO_CATCHANY (req->get_response ());
    if (req->env()->exception()) {
	cerr << "eventd: pull failed with: "
	     << req->env()->exception() << endl;
	disconnect_pull_consumer();
	return;
    }
    const CORBA::Any *any = new CORBA::Any;
    *req->result()->value() >>= any;

    channel->notify (*any);

    if (channel->listen()) {
	request = supplier->_request ("pull");
	request->result()->value()->set_type (CORBA::_tc_any);
	request->send_deferred (this);
    } else {
	request = CORBA::Request::_nil();
    }
}

void
ProxyPullConsumer_impl::listen ()
{
    if (CORBA::is_nil (request)) {
	request = supplier->_request ("pull");
	request->result()->value()->set_type (CORBA::_tc_any);
	request->send_deferred (this);
    }
}

//------------------------------------------------------------------------

ConsumerAdmin_impl::ConsumerAdmin_impl (EventChannel_impl *channel)
{
    //  push_supplier = new ProxyPushSupplier_impl (channel);
    event_channel = channel;
}

CosEventChannelAdmin::ProxyPushSupplier_ptr 
ConsumerAdmin_impl::obtain_push_supplier ()
{
    // create a new proxy
    ProxyPushSupplier_impl* impl = new ProxyPushSupplier_impl (event_channel);
    CosEventChannelAdmin::ProxyPushSupplier_ptr ptr = impl->_this();
    // and register it 
    event_channel->_reg_push_supplier (ptr);
    return ptr;
}

CosEventChannelAdmin::ProxyPullSupplier_ptr 
ConsumerAdmin_impl::obtain_pull_supplier ()
{
    // create a new proxy
    ProxyPullSupplier_impl* impl = new ProxyPullSupplier_impl (event_channel);
    CosEventChannelAdmin::ProxyPullSupplier_ptr ptr = impl->_this();
    // and register it 
    event_channel->_reg_pull_supplier (ptr);
    return ptr;
}

//------------------------------------------------------------------------

SupplierAdmin_impl::SupplierAdmin_impl (EventChannel_impl *channel)
{
    event_channel = channel;
}

CosEventChannelAdmin::ProxyPushConsumer_ptr 
SupplierAdmin_impl::obtain_push_consumer ()
{
    // create a new proxy
    ProxyPushConsumer_impl* impl = new ProxyPushConsumer_impl (event_channel);
    CosEventChannelAdmin::ProxyPushConsumer_ptr ptr = impl->_this();
    // and register
    event_channel->_reg_push_consumer (ptr);
    return ptr;
}

CosEventChannelAdmin::ProxyPullConsumer_ptr 
SupplierAdmin_impl::obtain_pull_consumer ()
{
    // create a new proxy
    ProxyPullConsumer_impl* impl = new ProxyPullConsumer_impl (event_channel);
    CosEventChannelAdmin::ProxyPullConsumer_ptr ptr = impl->_this();
    // and register
    event_channel->_reg_pull_consumer (ptr);
    return ptr;
}

//------------------------------------------------------------------------

EventChannel_impl::EventChannel_impl ()
    : _push_supp(FALSE, MICOMT::Mutex::Recursive), _push_cons(FALSE, MICOMT::Mutex::Recursive),
      _pull_supp(FALSE, MICOMT::Mutex::Recursive), _pull_cons(FALSE, MICOMT::Mutex::Recursive)
{
    _listeners = 0;
    _max_queue_size = 0x7fffffffl;
}

EventChannel_impl::EventChannel_impl (CORBA::Object_ptr obj)
    : _push_supp(FALSE, MICOMT::Mutex::Recursive), _push_cons(FALSE, MICOMT::Mutex::Recursive),
      _pull_supp(FALSE, MICOMT::Mutex::Recursive), _pull_cons(FALSE, MICOMT::Mutex::Recursive)
{
  _max_queue_size = 0x7fffffffl;
#if 0
  ifstream in (obj->_ident ());
  cout << "restore persistent state: " << obj->_ident () << endl;
  while (! in.eof ()) {
    int n;
    string objref;

    in >> n;
    for (int i = 0; i < n; i++) {
      in >> objref;
      if (objref.size () == 0) {
	return;
      }

      CORBA::Object_ptr obj = _orbnc()->string_to_object (objref.c_str ());
      CosEventComm::PushConsumer_ptr consumer =
	CosEventComm::PushConsumer::_narrow (obj);
      _add_consumer (consumer);
    }
    in >> n;
    for (int i = 0; i < n; i++) {
      in >> objref;
      CORBA::Object_ptr obj = _orbnc()->string_to_object (objref.c_str ());
      CosEventComm::PushSupplier_ptr supplier =
	CosEventComm::PushSupplier::_narrow (obj);
      _add_supplier (supplier);
    }
  }
#endif
}

EventChannel_impl::EventChannel_impl (CORBA::Long mqs)
    : _push_supp(FALSE, MICOMT::Mutex::Recursive), _push_cons(FALSE, MICOMT::Mutex::Recursive),
      _pull_supp(FALSE, MICOMT::Mutex::Recursive), _pull_cons(FALSE, MICOMT::Mutex::Recursive)
{
    _listeners = 0;
    _max_queue_size = mqs;
    supplier_admin = (new SupplierAdmin_impl (this))->_this();
    consumer_admin = (new ConsumerAdmin_impl (this))->_this();
}

CORBA::Boolean
EventChannel_impl::_save_object ()
{
#if 0
  ofstream out (_ident ());
  out << consumers.size () << endl;
  list<CosEventComm::PushConsumer_ptr>::iterator i1 = consumers.begin ();
  for (; i1 != consumers.end (); i1++) {
    out << _orbnc()->object_to_string (*i1) << endl;
  }
  out << suppliers.size () << endl;
  list<CosEventComm::PushSupplier_ptr>::iterator i2 = suppliers.begin ();
  for (; i2 != suppliers.end (); i2++) {
    out << _orbnc()->object_to_string (*i2) << endl;
  }
  return TRUE;
#endif
  return FALSE;
}

CosEventChannelAdmin::ConsumerAdmin_ptr 
EventChannel_impl::for_consumers ()
{
  return CosEventChannelAdmin::ConsumerAdmin::_duplicate (consumer_admin);
}

CosEventChannelAdmin::SupplierAdmin_ptr 
EventChannel_impl::for_suppliers ()
{
  return CosEventChannelAdmin::SupplierAdmin::_duplicate (supplier_admin);
}

void EventChannel_impl::destroy ()
{
  _disconnect ();
  _remove_ref(); //CORBA::release (this);
}

void
EventChannel_impl::_reg_push_consumer (
    CosEventChannelAdmin::ProxyPushConsumer_ptr push_cons)
{
    MICOMT::AutoLock lock(_push_cons);
    _push_cons.push_back (
        CosEventChannelAdmin::ProxyPushConsumer::_duplicate (push_cons));
}

void
EventChannel_impl::_reg_push_supplier (
    CosEventChannelAdmin::ProxyPushSupplier_ptr push_supp)
{
    MICOMT::AutoLock lock(_push_supp);
    _push_supp.push_back (
        CosEventChannelAdmin::ProxyPushSupplier::_duplicate (push_supp));
}

void
EventChannel_impl::_reg_pull_consumer (
    CosEventChannelAdmin::ProxyPullConsumer_ptr pull_cons)
{
    MICOMT::AutoLock lock(_pull_cons);
    _pull_cons.push_back (
        CosEventChannelAdmin::ProxyPullConsumer::_duplicate (pull_cons));
}

void
EventChannel_impl::_reg_pull_supplier (
    CosEventChannelAdmin::ProxyPullSupplier_ptr pull_supp)
{
    MICOMT::AutoLock lock(_pull_supp);
    _pull_supp.push_back (
        CosEventChannelAdmin::ProxyPullSupplier::_duplicate (pull_supp));
}

void
EventChannel_impl::notify (const CORBA::Any &any)
{
    MICOMT::AutoLock push_lock(_push_supp);
    MICOMT::AutoLock pull_lock(_pull_supp);
    list<CosEventChannelAdmin::ProxyPushSupplier_var>::iterator i;
    for (i = _push_supp.begin(); i != _push_supp.end (); i++)
        (*i)->notify (any);
    list<CosEventChannelAdmin::ProxyPullSupplier_var>::iterator j;
    for (j = _pull_supp.begin(); j != _pull_supp.end (); j++)
        (*j)->notify (any);
}

void
EventChannel_impl::listen (CORBA::Boolean on)
{
    MICOMT::AutoLock lock(_pull_cons);
    MICOMT::AutoLock list_lock(_listeners_mutex);
    if (on && ++_listeners >= 1) {
	list<CosEventChannelAdmin::ProxyPullConsumer_var>::iterator i;
	for (i = _pull_cons.begin(); i != _pull_cons.end (); i++) {
	    (*i)->listen ();
	}
    } else if (!on) {
	assert (_listeners > 0);
	--_listeners;
    }
}

CORBA::Boolean
EventChannel_impl::listen ()
{
    MICOMT::AutoLock list_lock(_listeners_mutex);
    return _listeners > 0;
}

void
EventChannel_impl::_disconnect ()
{
    MICOMT::AutoLock push_supp_lock(_push_supp);
    MICOMT::AutoLock pull_supp_lock(_pull_supp);
    MICOMT::AutoLock push_cons_lock(_push_cons);
    MICOMT::AutoLock pull_cons_lock(_pull_cons);
    list<CosEventChannelAdmin::ProxyPushSupplier_var>::iterator i;
    for (i = _push_supp.begin(); i != _push_supp.end (); i++)
        (*i)->disconnect_push_supplier ();

    list<CosEventChannelAdmin::ProxyPullSupplier_var>::iterator j;
    for (j = _pull_supp.begin(); j != _pull_supp.end (); j++)
        (*j)->disconnect_pull_supplier ();

    list<CosEventChannelAdmin::ProxyPushConsumer_var>::iterator k;
    for (k = _push_cons.begin(); k != _push_cons.end (); k++)
        (*k)->disconnect_push_consumer ();

    list<CosEventChannelAdmin::ProxyPullConsumer_var>::iterator l;
    for (l = _pull_cons.begin(); l != _pull_cons.end (); l++)
        (*l)->disconnect_pull_consumer ();
}

//------------------------------------------------------------------------

EventChannelFactory_impl::EventChannelFactory_impl ()
{
  _max_queue_size = 0x7fffffffl;
}

EventChannelFactory_impl::EventChannelFactory_impl (CORBA::Object_ptr obj)
{
  _max_queue_size = 0x7fffffffl;
}

EventChannelFactory_impl::EventChannelFactory_impl (CORBA::Long mqs)
{
  _max_queue_size = mqs;
}

CosEventChannelAdmin::EventChannel_ptr 
EventChannelFactory_impl::create_eventchannel ()
{
  EventChannel_impl* impl = new EventChannel_impl (_max_queue_size);
  CosEventChannelAdmin::EventChannel_ptr channel = impl->_this();
    

  return CosEventChannelAdmin::EventChannel::_duplicate (channel);
}

void EventChannelFactory_impl::_shutdown ()
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  orb->shutdown (TRUE);
}

