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

#ifndef __mico_intercept_h__
#define __mico_intercept_h__

// mapped by hand from intercept.idl

// workaround for flaw in X11 headers
#undef Status

namespace Interceptor {
    enum Status {
        INVOKE_CONTINUE = 0,
        INVOKE_ABORT,
        INVOKE_RETRY,
	INVOKE_BREAK
    };


    class Root;
    typedef Root *Root_ptr;
    typedef ObjVar<Root> Root_var;

    class Root : public CORBA::ServerlessObject {
    public:
        typedef CORBA::ULong Priority;
        //const Priority LowestPriority = 0;
        //const Priority HighestPriority = 0x7fffffffL;
        enum {
            LowestPriority = 0,
            HighestPriority = 0x7fffffff
        };
    private:
	Priority _prio;
	CORBA::Boolean _is_active;
    public:
        Root ();
        Root (Priority p);
        virtual ~Root ();

        virtual const char *_repoid() const;
        static Root_ptr _nil ()
	{
	    return 0;
	}
        static Root_ptr _duplicate (Root_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static Root_ptr _narrow (Root_ptr);

        Priority prio();

        virtual void activate (Priority p);
        virtual void deactivate ();
        CORBA::Boolean is_active ();
    };

    
    class Context;
    typedef Context *Context_ptr;
    typedef ObjVar<Context> Context_var;
    typedef ObjOut<Context> Context_out;

    class Context : public CORBA::ServerlessObject {
    public:
        Context ();
        virtual ~Context ();

        virtual const char *_repoid() const;
        static Context_ptr _nil ()
	{
	    return 0;
	}
        static Context_ptr _duplicate (Context_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static Context_ptr _narrow (Context_ptr);

        void destroy ();
    };

    
    typedef IOP::ServiceID ServiceID;
    typedef SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> ContextData;
    typedef TSeqVar<ContextData> ContextData_var;


    class LWRootRequest;
    typedef LWRootRequest *LWRootRequest_ptr;
    typedef ObjVar<LWRootRequest> LWRootRequest_var;
    typedef ObjOut<LWRootRequest> LWRootRequest_out;

    class LWRootRequest : public CORBA::ServerlessObject {
    private:
	typedef std::map<Root_ptr, Context_var, std::less<Root_ptr> > CtxMap;
	CtxMap _ctxs;
	IOP::ServiceContextList &_svc;
	CORBA::String_var &_op;
	CORBA::Object_var &_obj;
    public:
        LWRootRequest (CORBA::Object_var &obj,
		       CORBA::String_var &op,
		       IOP::ServiceContextList &svc);
        virtual ~LWRootRequest ();

        virtual const char *_repoid() const;
        static LWRootRequest_ptr _nil ()
	{
	    return 0;
	}
        static LWRootRequest_ptr _duplicate (LWRootRequest_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static LWRootRequest_ptr _narrow (LWRootRequest_ptr);

        CORBA::Object_ptr target();
        void target (CORBA::Object_ptr);
        CORBA::Identifier operation();
        void operation (const CORBA::Identifier);

        void set_service_context (ServiceID id,
                                  CORBA::Long flags,
                                  const ContextData &d);
        ContextData *get_service_context (ServiceID id,
                                          CORBA::Long flags);
        void remove_service_context (ServiceID id);
        CORBA::Boolean has_service_context (ServiceID id);

        void set_context (Root_ptr interceptor, Context_ptr ctx);
        Context_ptr get_context (Root_ptr interceptor);
    };

    
    class LWRequest;
    typedef LWRequest *LWRequest_ptr;
    typedef ObjVar<LWRequest> LWRequest_var;
    typedef ObjOut<LWRequest> LWRequest_out;

    class LWRequest : public LWRootRequest {
	CORBA::Request_ptr _req;
    public:
        LWRequest (CORBA::Object_var &obj,
		   CORBA::String_var &op,
		   IOP::ServiceContextList &svc,
		   CORBA::Request_ptr req);
        virtual ~LWRequest ();

        virtual const char *_repoid() const;
        static LWRequest_ptr _nil ()
	{
	    return 0;
	}
        static LWRequest_ptr _duplicate (LWRequest_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static LWRequest_ptr _narrow (LWRootRequest_ptr);

        CORBA::Request_ptr request ();
    };

    
    class LWServerRequest;
    typedef LWServerRequest *LWServerRequest_ptr;
    typedef ObjVar<LWServerRequest> LWServerRequest_var;
    typedef ObjOut<LWServerRequest> LWServerRequest_out;

    class LWServerRequest : public LWRootRequest {
	CORBA::ServerRequest_ptr _req;
	CORBA::Object_var _obj;
	CORBA::String_var _op;
    public:
        LWServerRequest (CORBA::Object_ptr obj,
			 const char *op,
			 IOP::ServiceContextList &svc,
			 CORBA::ServerRequest_ptr req);
        virtual ~LWServerRequest ();

        virtual const char *_repoid() const;
        static LWServerRequest_ptr _nil ()
	{
	    return 0;
	}
        static LWServerRequest_ptr _duplicate (LWServerRequest_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static LWServerRequest_ptr _narrow (LWRootRequest_ptr);

        CORBA::ServerRequest_ptr request ();
    };


    class ClientInterceptor;
    typedef ClientInterceptor *ClientInterceptor_ptr;
    typedef ObjVar<ClientInterceptor> ClientInterceptor_var;
    typedef ObjOut<ClientInterceptor> ClientInterceptor_out;

    class ClientInterceptor : public Root {
	static std::list<ClientInterceptor_ptr> *_interceptors;
        static std::list<ClientInterceptor_ptr> &_ics ();
    public:
        ClientInterceptor ();
        ClientInterceptor (Priority p);
        virtual ~ClientInterceptor ();

        virtual const char *_repoid() const;
        static ClientInterceptor_ptr _nil ()
	{
	    return 0;
	}
        static ClientInterceptor_ptr _duplicate (ClientInterceptor_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static ClientInterceptor_ptr _narrow (Root_ptr);

        virtual Status initialize_request (LWRequest_ptr req,
					   CORBA::Environment_ptr env);
        virtual Status after_marshal (LWRequest_ptr req,
				      CORBA::Environment_ptr env);
        virtual Status output_message (CORBA::Buffer *buf,
				       CORBA::Environment_ptr env);
        virtual Status input_message (CORBA::Buffer *buf,
				      CORBA::Environment_ptr env);
        virtual Status before_unmarshal (LWRequest_ptr req,
					 CORBA::Environment_ptr env);
        virtual Status finish_request (LWRequest_ptr req,
				       CORBA::Environment_ptr env);

	// override Root:: methods
        void activate (Priority p);
        void deactivate ();

	typedef Status (ClientInterceptor::*InterceptorMethod1) (
	    LWRequest_ptr,
	    CORBA::Environment_ptr);

	typedef Status (ClientInterceptor::*InterceptorMethod2) (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

	static CORBA::Boolean _exec (
	    LWRequest_ptr,
	    CORBA::Environment_ptr,
	    InterceptorMethod1 m);

	static CORBA::Boolean _exec (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr,
	    InterceptorMethod2 m);

        static CORBA::Boolean _exec_initialize_request (
	    LWRequest_ptr,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_after_marshal (
	    LWRequest_ptr,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_output_message (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_input_message (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_before_unmarshal (
	    LWRequest_ptr,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_finish_request (
	    LWRequest_ptr,
	    CORBA::Environment_ptr);

	static LWRequest_ptr _create_request (
	    CORBA::Object_var &obj,
	    CORBA::String_var &op,
	    IOP::ServiceContextList &svc,
	    CORBA::Request_ptr req);

	static CORBA::Boolean _idle ()
	{ return _ics().size() == 0; }
    };


    class ServerInterceptor;
    typedef ServerInterceptor *ServerInterceptor_ptr;
    typedef ObjVar<ServerInterceptor> ServerInterceptor_var;
    typedef ObjOut<ServerInterceptor> ServerInterceptor_out;

    class ServerInterceptor : public Root {
	static std::list<ServerInterceptor_ptr> *_interceptors;
        static std::list<ServerInterceptor_ptr> &_ics ();
    public:
        ServerInterceptor ();
        ServerInterceptor (Priority p);
        virtual ~ServerInterceptor ();

        virtual const char *_repoid() const;
        static ServerInterceptor_ptr _nil ()
	{
	    return 0;
	}
        static ServerInterceptor_ptr _duplicate (ServerInterceptor_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static ServerInterceptor_ptr _narrow (Root_ptr);

        virtual Status input_message (CORBA::Buffer *buf,
				      CORBA::Environment_ptr env);
        virtual Status initialize_request (LWServerRequest_ptr req,
					   CORBA::Environment_ptr env);
        virtual Status after_unmarshal (LWServerRequest_ptr req,
					CORBA::Environment_ptr env);
        virtual Status before_marshal (LWServerRequest_ptr req,
				       CORBA::Environment_ptr env);
        virtual Status finish_request (LWServerRequest_ptr req,
				       CORBA::Environment_ptr env);
        virtual Status output_message (CORBA::Buffer *buf,
				       CORBA::Environment_ptr env);

	// override Root:: methods
        void activate (Priority p);
        void deactivate ();

	typedef Status (ServerInterceptor::*InterceptorMethod1) (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr);

	typedef Status (ServerInterceptor::*InterceptorMethod2) (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

	static CORBA::Boolean _exec (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr,
	    InterceptorMethod1 m);

	static CORBA::Boolean _exec (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr,
	    InterceptorMethod2 m);

        static CORBA::Boolean _exec_input_message (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

        static CORBA::Boolean _exec_initialize_request (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr env);

        static CORBA::Boolean _exec_after_unmarshal (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr env);

        static CORBA::Boolean _exec_before_marshal (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr env);

        static CORBA::Boolean _exec_finish_request (
	    LWServerRequest_ptr,
	    CORBA::Environment_ptr env);

	static CORBA::Boolean _exec_output_message (
	    CORBA::Buffer *,
	    CORBA::Environment_ptr);

	static LWServerRequest_ptr _create_request (
	    CORBA::Object_ptr obj,
	    const char *op,
	    IOP::ServiceContextList &svc,
	    CORBA::ServerRequest_ptr req);

	static CORBA::Boolean _idle ()
	{ return _ics().size() == 0; }
    };

    
    class InitInterceptor;
    typedef InitInterceptor *InitInterceptor_ptr;
    typedef ObjVar<InitInterceptor> InitInterceptor_var;
    typedef ObjOut<InitInterceptor> InitInterceptor_out;

    class InitInterceptor : public Root {
	static std::list<InitInterceptor_ptr> *_interceptors;
        static std::list<InitInterceptor_ptr> &_ics ();
    public:
        InitInterceptor ();
        InitInterceptor (Priority p);
        virtual ~InitInterceptor ();

        virtual const char *_repoid() const;
        static InitInterceptor_ptr _nil ()
	{
	    return 0;
	}
        static InitInterceptor_ptr _duplicate (InitInterceptor_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static InitInterceptor_ptr _narrow (Root_ptr);

        virtual Status initialize (CORBA::ORB_ptr orb,
				   const char *id,
				   int &argc, char *argv[]);

	// override Root:: methods
        void activate (Priority p);
        void deactivate ();

	static CORBA::Boolean _exec_initialize (CORBA::ORB_ptr orb,
						const char *id,
						int &argc, char *argv[]);
    };


    class ConnInterceptor;
    typedef ConnInterceptor *ConnInterceptor_ptr;
    typedef ObjVar<ConnInterceptor> ConnInterceptor_var;
    typedef ObjOut<ConnInterceptor> ConnInterceptor_out;

    class ConnInterceptor : public Root {
	static std::list<ConnInterceptor_ptr> *_interceptors;
        static std::list<ConnInterceptor_ptr> &_ics ();
    public:
        ConnInterceptor ();
        ConnInterceptor (Priority p);
        virtual ~ConnInterceptor ();

        virtual const char *_repoid() const;
        static ConnInterceptor_ptr _nil ()
	{
	    return 0;
	}
        static ConnInterceptor_ptr _duplicate (ConnInterceptor_ptr o)
	{
	    if (o)
		o->_ref();
	    return o;
	}
        static ConnInterceptor_ptr _narrow (Root_ptr);

        virtual Status client_connect (const char *addr);
        virtual Status client_disconnect (const char *addr);

	// override Root:: methods
        void activate (Priority p);
        void deactivate ();

	static CORBA::Boolean _exec_client_connect (const char *addr);
	static CORBA::Boolean _exec_client_disconnect (const char *addr);
    };
} 

#endif // __mico_intercept_h__
