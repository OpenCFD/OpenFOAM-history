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

#ifndef __mico_dsi_h__
#define __mico_dsi_h__

namespace CORBA {

class ObjectAdapter;
class ORBRequest;

class ServerRequestBase : public CORBA::ServerlessObject {
protected:
    CORBA::Boolean _canceled;
public:
    ServerRequestBase ()
        : _canceled (FALSE)
    {}
    virtual ~ServerRequestBase ();

    virtual CORBA::Environment_ptr environment () = 0;
    void cancel ()
    { _canceled = TRUE; }

    static ServerRequestBase_ptr _duplicate (ServerRequestBase_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static ServerRequestBase_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<ServerRequestBase> ServerRequestBase_var;


/*
 * XXX CORBA spec says all methods may throw SystemException (18-6) ...
 *
 * memory management:
 * - ServerRequest owns res and except, even dir_params (the NVList
 *   passed to params()) is freed by ServerRequest [18-6]
 * - params() must always be called by DIR exactly once [14-26]
 */

class ServerRequest : public ServerRequestBase {
    typedef CORBA::ULong MsgId;
    CORBA::ObjectAdapter *_oa;
    CORBA::ORBMsgId _id;
    Object_ptr _obj;
    ORBRequest *_req;
    NVList_ptr _dir_params;
    Context_ptr _context;
    Any_var _res;
    Environment _env;
    ServerlessObject_ptr _iceptreq;
    CORBA::Boolean _aborted;
public:
    // begin-mico-extension
    ServerRequest (ORBRequest *req, Object_ptr obj, CORBA::ORBMsgId id,
                   CORBA::ObjectAdapter *oa, CORBA::Principal_ptr);
    ~ServerRequest ();

    ORBRequest *request ();
    void set_out_args ();
    NVList_ptr arguments ();
    Any *result ();
    Exception *exception ();
    Environment_ptr environment ();
    // end-mico-extension

    // CORBA 2.2 mapping [20.36.1]
    const char * operation ();
    void arguments (NVList_ptr &);
    void set_result (const Any &);
    void set_exception (const Any &);
    // Context_ptr ctx () as below

    const char *op_name ();
    OperationDef_ptr op_def ();
    Context_ptr ctx ();
    CORBA::Boolean params (NVList_ptr params);
    void result (Any *val);
    void exception (Any *val);
    void exception (Exception *ex);

    static ServerRequest_ptr _duplicate (ServerRequest_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static ServerRequest_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<ServerRequest> ServerRequest_var;

}

#endif // __mico_dsi_h__
