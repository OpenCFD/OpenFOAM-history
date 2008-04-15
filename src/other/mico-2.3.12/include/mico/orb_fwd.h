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

#ifndef __mico_orb_fwd_h__
#define __mico_orb_fwd_h__

namespace CORBA {

enum InvokeStatus {
    InvokeOk,
    InvokeForward,
    InvokeSysEx,
    InvokeUsrEx,
    InvokeAddrDisp
};

enum LocateStatus {
    LocateUnknown,
    LocateHere,
    LocateForward,
    LocateAddrDisp
};

enum RequestType {
    RequestInvoke,
    RequestBind,
    RequestLocate,
    RequestUnknown
};

class ORBRequest;
class ORBInvokeRec;

typedef ORBInvokeRec* ORBMsgId;

struct ORBCallback {
    enum Event { Invoke, Locate, Bind, Result };
    virtual Boolean waitfor (ORB_ptr, ORBMsgId, Event, Long tmout = -1) = 0;
    virtual void notify  (ORB_ptr, ORBMsgId, Event) = 0;
    virtual ~ORBCallback ();
};

#ifdef HAVE_THREADS

struct ORBAsyncCallback: public ORBCallback {
    Boolean  notified;
    MICOMT::Mutex   cond_mutex;
    MICOMT::CondVar cond;
    Boolean waitfor (ORB_ptr, ORBMsgId, Event, Long tmout = -1);
    void notify  (ORB_ptr, ORBMsgId, Event) {
      MICOMT::AutoLock l(cond_mutex);
      notified = TRUE;
      cond.broadcast();
    };

    ORBAsyncCallback()
	: notified(FALSE), cond(&cond_mutex)
    {};
    virtual ~ORBAsyncCallback ();
};

#endif

class ORBRequest : public ServerlessObject {
protected:
    IOP::ServiceContextList _svc;
    void copy_svc (ORBRequest *r)
    { _svc = r->_svc; }
public:
    typedef std::vector<StaticAny *> StaticAnyList;

    virtual ~ORBRequest ();

    IOP::ServiceContextList *context ()
    { return &_svc; }

    void context (IOP::ServiceContextList *svc)
    { _svc = *svc; }

    virtual const char *op_name() = 0;

    virtual CORBA::Boolean get_in_args (NVList_ptr iparams,
                                        Context_ptr &ctx) = 0;
    virtual CORBA::Boolean get_in_args (StaticAnyList *iparams,
                                        Context_ptr &ctx)=0;
    virtual CORBA::Boolean get_in_args (DataEncoder *) = 0;

    virtual CORBA::Boolean get_out_args (Any *res, NVList_ptr oparams,
                                         Exception *&ex) = 0;
    virtual CORBA::Boolean get_out_args (StaticAny *res,
                                         StaticAnyList *oparams,
                                         Exception *&ex) = 0;
    virtual CORBA::Boolean get_out_args (DataEncoder *,
                                         Boolean &is_except) = 0;

    virtual CORBA::Boolean set_out_args (Any *res, NVList_ptr oparams) = 0;
    virtual CORBA::Boolean set_out_args (StaticAny *res,
                                         StaticAnyList *oparams) = 0;
    virtual void set_out_args (Exception *ex) = 0;
    virtual CORBA::Boolean set_out_args (DataDecoder *, Boolean is_except) = 0;

    virtual CORBA::Boolean copy_out_args (ORBRequest *) = 0;
    virtual CORBA::Boolean copy_in_args (ORBRequest *) = 0;

    virtual const char *type () = 0;

    static ORBRequest* _nil ()
    {
	return 0;
    }
    static ORBRequest* _duplicate (ORBRequest *o)
    {
	if (o)
	    o->_ref();
	return o;
    }
};

}

#endif // __mico_orb_h__
