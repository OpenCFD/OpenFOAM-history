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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <mico/util.h>
#include <mico/impl.h>
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

/**************************** ReqQueueRec ***************************/


MICO::ReqQueueRec::ReqQueueRec (CORBA::ORBMsgId id, CORBA::ORBRequest *req,
				CORBA::Object_ptr obj,
				CORBA::Principal_ptr pr,
				CORBA::Boolean resp)
{
    _id = id;
    _request = CORBA::ORBRequest::_duplicate (req);
    _obj = CORBA::Object::_duplicate (obj);
    _pr = CORBA::Principal::_duplicate (pr);
    _response_exp = resp;
    _type = CORBA::RequestInvoke;
}

MICO::ReqQueueRec::ReqQueueRec (CORBA::ORBMsgId id, const char *repoid,
				const CORBA::ORB::ObjectTag &tag)
{
    _id = id;
    _repoid = repoid;
    _request = CORBA::ORBRequest::_nil();
    _obj = CORBA::Object::_nil();
    _pr = CORBA::Principal::_nil();
    _type = CORBA::RequestBind;
    _tag = tag;
}

MICO::ReqQueueRec::ReqQueueRec (CORBA::ORBMsgId id, CORBA::Object_ptr obj)
{
    _id = id;
    _request = CORBA::ORBRequest::_nil();
    _obj = CORBA::Object::_duplicate (obj);
    _pr = CORBA::Principal::_nil();
    _type = CORBA::RequestLocate;
}

MICO::ReqQueueRec::~ReqQueueRec ()
{
    CORBA::release (_request);
    CORBA::release (_obj);
    CORBA::release (_pr);
}

void
MICO::ReqQueueRec::exec (CORBA::ObjectAdapter *oa, CORBA::ORB_ptr orb)
{
    switch (_type) {
    case CORBA::RequestInvoke:
        oa->invoke (_id, _obj, _request, _pr, _response_exp);
        break;

    case CORBA::RequestBind:
        if (!oa->bind (_id, _repoid.c_str(), _tag, 0)) {
	    orb->answer_bind (_id, CORBA::LocateUnknown,
			      CORBA::Object::_nil());
	}
        break;

    case CORBA::RequestLocate:
        oa->locate (_id, _obj);
        break;

    default:
        assert (0);
    }
}

void
MICO::ReqQueueRec::fail (CORBA::ObjectAdapter *oa, CORBA::ORB_ptr orb)
{
    switch (_type) {
    case CORBA::RequestInvoke: {
        CORBA::COMM_FAILURE ex (0, CORBA::COMPLETED_MAYBE);
	_request->set_out_args (&ex);
	orb->answer_invoke (_id, CORBA::InvokeSysEx,
			    CORBA::Object::_nil(), _request, 0);
        break;
    }
    case CORBA::RequestBind:
        orb->answer_bind (_id, CORBA::LocateUnknown,
			  CORBA::Object::_nil());
        break;

    case CORBA::RequestLocate:
        orb->answer_locate (_id, CORBA::LocateUnknown,
			    CORBA::Object::_nil(), 0);
        break;

    default:
        assert (0);
    }
}

CORBA::Object_ptr
MICO::ReqQueueRec::target ()
{
    return _obj;
}


/****************************** RequestQueue ****************************/


MICO::RequestQueue::RequestQueue (CORBA::ObjectAdapter *oa,
				  CORBA::ORB_ptr orb)
    : _current_id (0), _oa (oa), _orb (orb)
{
}

MICO::RequestQueue::~RequestQueue ()
{
    clear ();
    exec_stop ();
}

void
MICO::RequestQueue::add (ReqQueueRec *r)
{
    _invokes.push_back (r);
}

void
MICO::RequestQueue::exec_now ()
{
    // reissue pending invokes ...
    set<CORBA::ORBMsgId, less<CORBA::ORBMsgId> > seen;
    while (_invokes.size() > 0) {
	ReqQueueRec *inv = _invokes.front();
        _current_id = inv->id();
	if (seen.count (_current_id))
	    break;
	seen.insert (_current_id);
        _invokes.pop_front();
        /*
         * if invoke cannot be executed yet mediator will install
         * new ReqQueueRec ...
         */
	inv->exec (_oa, _orb);
	delete inv;
    }
    _current_id = 0;
}

void
MICO::RequestQueue::exec_later ()
{
    // schedule reexecution of pending requests ...
    if (_invokes.size() > 0) {
	_orb->dispatcher()->remove (this, CORBA::Dispatcher::Timer);
	_orb->dispatcher()->tm_event (this, 0 /* zero timeout */);
    }
}

void
MICO::RequestQueue::exec_stop ()
{
    _orb->dispatcher()->remove (this, CORBA::Dispatcher::Timer);
}

void
MICO::RequestQueue::fail ()
{
    // make pending invokes fail ...
    int sz = _invokes.size();
    while (--sz >= 0 && _invokes.size() > 0) {
        ReqQueueRec *inv = _invokes.front();
        _invokes.pop_front();

	inv->fail (_oa, _orb);
	delete inv;
    }
}

void
MICO::RequestQueue::clear ()
{
    for (InvokeList::iterator i = _invokes.begin(); i != _invokes.end(); ++i)
	delete *i;
    _invokes.erase (_invokes.begin(), _invokes.end());
}

void
MICO::RequestQueue::callback (CORBA::Dispatcher *,
			      CORBA::DispatcherCallback::Event ev)
{
    if (ev == CORBA::Dispatcher::Timer)
        exec_now ();
}
