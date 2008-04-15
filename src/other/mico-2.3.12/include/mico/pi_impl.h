// -*- c++ -*-
//
//  MICO PI --- an Open Source Portable Interceptor implementation
//  Copyright (C) 2001, 2002, 2003, 2004, 2005 ObjectSecurity Ltd.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  Send comments and/or bug reports to:
//                 micosec@objectsecurity.com
//

#ifndef __PI_IMPL_H__
#define __PI_IMPL_H__

#include <stack>

namespace PortableInterceptor
{
    extern void
    register_orb_initializer(PortableInterceptor::ORBInitializer_ptr init);

    // extension
    extern void
    destroy_all_interceptors();

    extern void
    backup_initializers();

    extern void
    restore_initializers();

    extern void
    commit_initialized_interceptors();

    typedef void (ClientRequestInterceptor::*ClientMethod)
	(PortableInterceptor::ClientRequestInfo_ptr ri);

    typedef void (ServerRequestInterceptor::*ServerMethod)
	(PortableInterceptor::ServerRequestInfo_ptr ri);
} // PortableInterceptor

namespace PICodec
{
    //
    // Codec_impl
    //

    class Codec_impl
	: virtual public IOP::Codec,
	  virtual public CORBA::LocalObject
    {
    private:
  	IOP::Encoding encoding_;
    public:
	Codec_impl();
	Codec_impl(const IOP::Encoding& enc);
	virtual
	~Codec_impl();

	virtual CORBA::OctetSeq*
	encode(const CORBA::Any& data);

	virtual CORBA::Any*
	decode(const CORBA::OctetSeq& data);

	virtual CORBA::OctetSeq*
	encode_value(const CORBA::Any& data);

	virtual CORBA::Any*
	decode_value(const CORBA::OctetSeq& data, CORBA::TypeCode_ptr tc);
    protected:
	CORBA::Boolean
	find_tc(CORBA::TypeCode_ptr tc);

	CORBA::DataEncoder*
	get_ec();

	CORBA::DataDecoder*
	get_dc(CORBA::Buffer* w_buf);
    };

    //
    // CodecFactory_impl
    //

    class CodecFactory_impl
	: virtual public IOP::CodecFactory,
	  virtual public CORBA::LocalObject
    {
    public:
	CodecFactory_impl();
	virtual
	~CodecFactory_impl();

	virtual IOP::Codec_ptr
	create_codec(const IOP::Encoding& enc);
    };

} // PICodec


namespace PInterceptor
{
    //
    // Current_impl
    //

#ifdef HAVE_THREADS
    // cleanup function for per-thread slots
    extern void
    cleanup_slots(void* value);
#endif // HAVE_THREADS

    class Current_impl
	: virtual public PortableInterceptor::Current,
	  virtual public CORBA::LocalObject
    {
    private:
	static Current_impl* S_current_;
#ifndef HAVE_THREADS
	CORBA::AnySeq* slots_;
#else // HAVE_THREADS
	MICOMT::Thread::ThreadKey slots_key_;
#endif // HAVE_THREADS
    public:
	Current_impl();
	virtual
	~Current_impl();

	virtual CORBA::Any*
	get_slot(PortableInterceptor::SlotId id);

	virtual void
	set_slot(PortableInterceptor::SlotId id, const CORBA::Any& data);

	// extension
	const CORBA::AnySeq*
	slots();

	void
	slots(const CORBA::AnySeq& value);

	static Current_impl*
	current();
    };

    enum PIState
    {
	SEND_REQUEST = 0,
	SEND_POLL,
	RECEIVE_REPLY,
	RECEIVE_EXCEPTION,
	RECEIVE_OTHER,
	RECEIVE_REQUEST_SERVICE_CONTEXTS,
	RECEIVE_REQUEST,
	SEND_REPLY,
	SEND_EXCEPTION,
	SEND_OTHER
    };

    enum InterceptionPoint
    {
	START = 0,
	INTERMEDIATE,
	END
    };

    typedef std::vector<CORBA::StaticAny*> StaticAnyList;

    class RequestInfo_impl
	: virtual public PortableInterceptor::RequestInfo,
	  virtual public CORBA::LocalObject
    {
    private:
	Dynamic::ParameterList dummy_list_;
    public:
	RequestInfo_impl();
	virtual
	~RequestInfo_impl();

	virtual CORBA::ULong
	request_id();

	virtual char*
	operation();

	virtual Dynamic::ParameterList*
	arguments();

	virtual Dynamic::ExceptionList*
	exceptions();

	virtual Dynamic::ContextList*
	contexts();

	virtual Dynamic::RequestContext*
	operation_context();

	virtual CORBA::Any*
	result();

	virtual CORBA::Boolean
	response_expected();

	virtual Messaging::SyncScope
	sync_scope();

	virtual PortableInterceptor::ReplyStatus
	reply_status();

	virtual CORBA::Object_ptr
	forward_reference();

	virtual CORBA::Any*
	get_slot(PortableInterceptor::SlotId id);

	virtual IOP::ServiceContext*
	get_request_service_context(IOP::ServiceID id);

	virtual IOP::ServiceContext*
	get_reply_service_context(IOP::ServiceID id);

	// extensions
	void
	arguments(const StaticAnyList& args, CORBA::Boolean valid_out);

	void
	arguments(CORBA::NVList_ptr param_list, CORBA::Boolean valid_out);

	void
	request_id(CORBA::ULong req_id);

	void
	contexts(CORBA::ContextList_ptr clist);

	void
	operation_context(CORBA::Context_ptr ctx);

	void
	response_expected(CORBA::Boolean resp);
	
	void
	sync_scope(Messaging::SyncScope ss);

	void
	icept_oper(PIState state);

	void
	reply_status(PortableInterceptor::ReplyStatus reply_status);

	void
	result(const CORBA::Any& res);

	CORBA::AnySeq*
	slots();

	void
	slots(const CORBA::AnySeq& value);

	void
	exception(CORBA::Exception* exc);

	CORBA::Exception*
	exception();
    protected:
  	CORBA::Object_ptr object_;
  	CORBA::ULong req_id_;
  	std::string op_name_;
  	Dynamic::ParameterList param_list_;
  	Dynamic::ExceptionList exc_list_;
	CORBA::Boolean exc_list_defined_;
  	Dynamic::ContextList ctx_list_;
	// we need it for computing right operation_context result
	CORBA::ContextList_var ctx_list_orig_;
	Dynamic::RequestContext req_ctx_;
	CORBA::Any result_;
	CORBA::Boolean response_;
	Messaging::SyncScope sync_scope_;
	PortableInterceptor::ReplyStatus reply_status_;
	CORBA::Object_ptr fwd_object_;
	IOP::ServiceContextList* req_scl_;
	IOP::ServiceContextList* reply_scl_;
  	PIState icept_oper_;
	CORBA::AnySeq slots_;
  	CORBA::Exception* exception_;

	void
	nvlist_to_parlist(Dynamic::ParameterList& param_list,
			  CORBA::NVList_ptr list,
			  CORBA::Boolean valid_out);

	void
	sanylist_to_parlist(Dynamic::ParameterList& param_list,
			    const StaticAnyList& args,
			    CORBA::Boolean valid_out);

	void
	cexlist_to_dexlist(Dynamic::ExceptionList& exc_list,
			   CORBA::ExceptionList_ptr list);

	void
	cctxl_to_dctxl(Dynamic::ContextList& ctx_list,
		       CORBA::ContextList_ptr list);

	void
	cctx_to_dctx(Dynamic::RequestContext& req_ctx,
		     CORBA::Context_ptr ctx,
		     CORBA::Object_ptr object);

	CORBA::IORProfile*
	get_profile();

	CORBA::IORProfile*
	get_components_profile();

	CORBA::DataEncoder* get_enc
	(CORBA::IORProfile*& prof);

	CORBA::CompletionStatus
	completion_status();
    };

    //
    // ClientRequestInfo_impl
    //

    class ClientRequestInfo_impl
	: virtual public RequestInfo_impl,
	  virtual public PortableInterceptor::ClientRequestInfo,
	  virtual public CORBA::LocalObject
    {
    public:
	ClientRequestInfo_impl();

	ClientRequestInfo_impl
	(CORBA::Object_ptr object,
	 const char* op_name,
	 CORBA::ULong req_id = 0,
	 CORBA::NVList_ptr param_list = CORBA::NVList::_nil(),
	 CORBA::ExceptionList_ptr exc_list = CORBA::ExceptionList::_nil(),
	 CORBA::ContextList_ptr ctx_list = CORBA::ContextList::_nil(),
	 CORBA::Context_ptr ctx = CORBA::Context::_nil(),
	 CORBA::NamedValue_ptr res = CORBA::NamedValue::_nil(),
	 CORBA::Boolean resp = TRUE);

	ClientRequestInfo_impl
	(CORBA::Object_ptr object,
	 const char* op_name,
	 CORBA::ExceptionList_ptr exc_list,
	 CORBA::NamedValue_ptr res);

	virtual
	~ClientRequestInfo_impl();

	virtual CORBA::Object_ptr
	target();

	virtual CORBA::Object_ptr
	effective_target();

	virtual IOP::TaggedProfile*
	effective_profile();

	virtual CORBA::Any*
	received_exception();

	virtual char*
	received_exception_id();

	virtual IOP::TaggedComponent*
	get_effective_component(IOP::ComponentId id);

	virtual IOP::TaggedComponentSeq*
	get_effective_components(IOP::ComponentId id);

	virtual CORBA::Policy_ptr
	get_request_policy(CORBA::PolicyType type);

	virtual void
	add_request_service_context(const IOP::ServiceContext& ctx,
				    CORBA::Boolean replace);
	// extensions
	void
	set_request_service_context_list(IOP::ServiceContextList* req_scl);

	void
	set_reply_service_context_list(IOP::ServiceContextList* reply_scl);

	void
	effective_target(CORBA::Object_ptr object);
    };

    //
    // ServerRequestInfo_impl
    //

    class ServerRequestInfo_impl
	: virtual public RequestInfo_impl,
	  virtual public PortableInterceptor::ServerRequestInfo,
	  virtual public CORBA::LocalObject
    {
    private:
	CORBA::ObjectAdapter* object_adapter_;
  	CORBA::Boolean own_exception_;
	CORBA::OctetSeq object_id_;
    public:
  	ServerRequestInfo_impl();
	ServerRequestInfo_impl
	(CORBA::Object_ptr object, const char* op_name, CORBA::ULong req_id,
	 CORBA::Boolean resp, IOP::ServiceContextList* scl,
	 CORBA::NVList_ptr param_list = CORBA::NVList::_nil(),
	 CORBA::ExceptionList_ptr exc_list = CORBA::ExceptionList::_nil(),
	 CORBA::ContextList_ptr ctx_list = CORBA::ContextList::_nil(),
	 CORBA::Context_ptr ctx = CORBA::Context::_nil(),
	 CORBA::NamedValue_ptr res = CORBA::NamedValue::_nil());
	virtual
	~ServerRequestInfo_impl();

	virtual CORBA::Any*
	sending_exception();

	virtual CORBA::OctetSeq*
	object_id();

	virtual CORBA::OctetSeq*
	adapter_id();

	virtual char*
	target_most_derived_interface();

	virtual CORBA::Policy_ptr
	get_server_policy(CORBA::PolicyType type);

	virtual void
	set_slot(PortableInterceptor::SlotId id, const CORBA::Any& data);

	virtual CORBA::Boolean
	target_is_a(const char* id);

	virtual void
	add_reply_service_context(const IOP::ServiceContext& ctx,
				  CORBA::Boolean replace);

	// overloaded from RequestInfo to throw NO_RESOURCES exception
	virtual Dynamic::ContextList*
	contexts();

	// extension
	void
	set_reply_service_context_list(IOP::ServiceContextList* scl);

	void
	effective_target(CORBA::Object_ptr object);

	void
	set_oa(CORBA::ObjectAdapter* oa);

	void
	own_exception(CORBA::Boolean own_exception)
	{ own_exception_ = own_exception; }

	CORBA::Boolean
	own_exception()
	{ return own_exception_; }
    
    };

    //
    // IORInfo_impl
    //

    class IORInfo_impl
	: virtual public PortableInterceptor::IORInfo,
	  virtual public CORBA::LocalObject
    {
    private:
	PortableServer::POA_ptr poa_;
	CORBA::IOR* ior_;
	CORBA::CodeSetCoder* conv_;
    public:
  	IORInfo_impl();
  	IORInfo_impl(PortableServer::POA_ptr poa, CORBA::IOR* ior);
	virtual
	~IORInfo_impl();

	virtual CORBA::Policy_ptr
	get_effective_policy(CORBA::PolicyType type);

	virtual void
	add_ior_component(const IOP::TaggedComponent& comp);

	virtual void
	add_ior_component_to_profile(const IOP::TaggedComponent& comp,
				     IOP::ProfileId prof_id);
    };

    //
    // ORBInitInfo_impl
    //

    class ORBInitInfo_impl
	: virtual public PortableInterceptor::ORBInitInfo,
	virtual public CORBA::LocalObject
    {
    private:
	CORBA::ORB_var orb_;
	std::string orb_id_;
	CORBA::StringSeq args_;
    public:
  	ORBInitInfo_impl(CORBA::ORB_ptr orb, const char *id, int& argc,
			 char** argv);
	virtual
	~ORBInitInfo_impl();

	virtual CORBA::StringSeq*
	arguments();

	virtual char*
	orb_id();

	virtual IOP::CodecFactory_ptr
	codec_factory();

	virtual void
	register_initial_reference(const char* id, CORBA::Object_ptr obj);

	virtual CORBA::Object_ptr
	resolve_initial_references(const char* id);

	virtual void
	add_client_request_interceptor
	(PortableInterceptor::ClientRequestInterceptor_ptr cri);

	virtual void
	add_server_request_interceptor
	(PortableInterceptor::ServerRequestInterceptor_ptr sri);

	virtual void
	add_ior_interceptor(PortableInterceptor::IORInterceptor_ptr ior_pi);

	virtual PortableInterceptor::SlotId
	allocate_slot_id();

	virtual void
	register_policy_factory
	(CORBA::PolicyType type,
	 PortableInterceptor::PolicyFactory_ptr factory);
    };

    template<typename T>
    class FlowStack
    {
	std::stack<T> start_ip_;
	std::stack<T> end_ip_;
    public:
	FlowStack(std::vector<T>& init)
	{
	    typename std::vector<T>::iterator i;
	    for (i = init.begin(); i != init.end(); i++)
		start_ip_.push(*i);
	}

	std::stack<T>*
	start_ip()
	{ return &start_ip_; }

	std::stack<T>*
	end_ip()
	{ return &end_ip_; }
    };

    struct PI {
	static std::vector<PortableInterceptor::ORBInitializer_var> S_initializers_;
	static std::vector<PortableInterceptor::ORBInitializer_var> S_initializers_backup_;
	static PortableInterceptor::SlotId S_max_slot_id_;
	typedef std::list<PortableInterceptor::IORInterceptor_var> IORList;
	typedef std::map<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr,
			 std::less<CORBA::PolicyType> > PolicyFactoryMap;
	static PolicyFactoryMap S_pfmap_;

	static std::vector<PortableInterceptor::ClientRequestInterceptor_ptr>
	S_client_req_int_;
	static std::vector<PortableInterceptor::ClientRequestInterceptor_ptr>
	S_client_req_int_prepared_;
	static std::vector<PortableInterceptor::ServerRequestInterceptor_ptr>
	S_server_req_int_;
	static std::vector<PortableInterceptor::ServerRequestInterceptor_ptr>
	S_server_req_int_prepared_;

	typedef FlowStack<PortableInterceptor::ClientRequestInterceptor_ptr>
	ClientFlowStack;
	typedef FlowStack<PortableInterceptor::ServerRequestInterceptor_ptr>
	ServerFlowStack;
	static std::stack<ClientFlowStack*> S_client_stack_;
	static std::stack<ServerFlowStack*> S_server_stack_;

	static MICOMT::Mutex S_client_lock_;
	static MICOMT::Mutex S_server_lock_;

	static IORList S_ior_interceptors_;
        static IORList S_ior_interceptors_prepared_;
	
	static void
	_init ();
	
	static void
	_exec_client(PortableInterceptor::ClientRequestInfo_ptr ri,
		     PortableInterceptor::ClientMethod m,
		     InterceptionPoint ip);

	static void
	_exec_server(PortableInterceptor::ServerRequestInfo_ptr ri,
		     PortableInterceptor::ServerMethod m,
		     InterceptionPoint ip);

	// client interceptor methods

	static void
	_exec_send_request(PortableInterceptor::ClientRequestInfo_ptr ri);
	
	static void
	_exec_send_poll(PortableInterceptor::ClientRequestInfo_ptr ri);

	static void
	_exec_receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri);

	static void
	_exec_receive_exception
	(PortableInterceptor::ClientRequestInfo_ptr ri); 

	static void
	_exec_receive_other(PortableInterceptor::ClientRequestInfo_ptr ri);

	template<typename T>
	static void
	_send_request_ip
	(ClientRequestInfo_impl* cri,
	 CORBA::ULong request_id,
	 T args,
	 CORBA::ContextList_ptr contexts,
	 CORBA::Context_ptr operation_context,
	 IOP::ServiceContextList* request_scl,
	 CORBA::Boolean response_expected = TRUE)
	{
	    if (PI::S_client_req_int_.size() > 0) {
		// any client interceptor registered?
		cri->request_id(request_id);
		cri->response_expected(response_expected);
		cri->icept_oper(PInterceptor::SEND_REQUEST);
		if (response_expected) {
		    // two way request
		    cri->sync_scope(Messaging::SYNC_WITH_TARGET);
		}
		else {
		    // oneway request
		    cri->sync_scope(Messaging::SYNC_WITH_TRANSPORT);
		}
		cri->arguments(args, FALSE);
		cri->contexts(contexts);
		cri->operation_context(operation_context);
		cri->set_request_service_context_list(request_scl);
		if (PInterceptor::Current_impl::current() == NULL) {
		    // resolving current is enough to get static reference later
		    CORBA::Object_var obj = cri->target();
		    CORBA::Object_var obj2
			= obj->_orbnc()->resolve_initial_references("PICurrent");
		}
		assert(PInterceptor::Current_impl::current() != NULL);
		const CORBA::AnySeq* s
		    = PInterceptor::Current_impl::current()->slots();
		cri->slots(*s);
		PInterceptor::PI::_exec_send_request(cri);
	    }
	}

	static void
	_send_poll_ip
	(ClientRequestInfo_impl* cri, CORBA::ULong request_id);

	template <typename T>
	static void
	_receive_reply_ip
	(ClientRequestInfo_impl* cri,
	 const CORBA::Any& result,
	 T args,
	 IOP::ServiceContextList* reply_scl,
	 CORBA::Boolean use_result)
	{
	    if (PI::S_client_req_int_.size() > 0) {
		// any client interceptor registered?
		cri->reply_status(PortableInterceptor::SUCCESSFUL);
		cri->set_reply_service_context_list(reply_scl);
		cri->arguments(args, TRUE);
		cri->icept_oper(PInterceptor::RECEIVE_REPLY);
		if (use_result)
		    cri->result(result);
		PInterceptor::PI::_exec_receive_reply(cri);
	    }
	}

	template <typename T>
	static void
	_receive_reply_ip
	(ClientRequestInfo_impl* cri,
	 const CORBA::Any& result,
	 T args,
	 CORBA::ContextList_ptr contexts,
	 CORBA::Context_ptr operation_context,
	 IOP::ServiceContextList* reply_scl,
	 CORBA::Boolean use_result)
	{
	    if (PI::S_client_req_int_.size() > 0) {
		// any client interceptor registered?
		cri->reply_status(PortableInterceptor::SUCCESSFUL);
		cri->contexts(contexts);
		cri->operation_context(operation_context);
		cri->set_reply_service_context_list(reply_scl);
		cri->arguments(args, TRUE);
		cri->icept_oper(PInterceptor::RECEIVE_REPLY);
		if (use_result)
		    cri->result(result);
		PInterceptor::PI::_exec_receive_reply(cri);
	    }
	}

	static void
	_receive_exception_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::Exception* exception,
	 IOP::ServiceContextList* reply_scl);

	static void
	_receive_exception_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::Exception* exception,
	 CORBA::ContextList_ptr contexts,
	 CORBA::Context_ptr operation_context,
	 IOP::ServiceContextList* reply_scl);

	static void
	_receive_other_ip(ClientRequestInfo_impl* cri);

	static void
	_receive_other_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 IOP::ServiceContextList* reply_scl);

	static void
	_receive_other_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::Object_ptr effective_target,
	 IOP::ServiceContextList* reply_scl);

	static void
	_receive_other_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::ContextList_ptr contexts,
	 CORBA::Context_ptr operation_context,
	 IOP::ServiceContextList* reply_scl);

	static void
	_receive_other_ip
	(ClientRequestInfo_impl* cri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::Object_ptr effective_target,
	 CORBA::ContextList_ptr contexts,
	 CORBA::Context_ptr operation_context,
	 IOP::ServiceContextList* reply_scl);

	// server interceptor methods

	static void
	_exec_receive_request_service_contexts
	(PortableInterceptor::ServerRequestInfo_ptr ri);
	
	static void
	_exec_receive_request(PortableInterceptor::ServerRequestInfo_ptr ri);

	static void
	_exec_send_reply(PortableInterceptor::ServerRequestInfo_ptr ri);

	static void
	_exec_send_exception(PortableInterceptor::ServerRequestInfo_ptr ri);

	static void
	_exec_send_other(PortableInterceptor::ServerRequestInfo_ptr ri);

	static void
	_receive_request_service_contexts_ip
	(ServerRequestInfo_impl* sri,
	 IOP::ServiceContextList* reply_scl,
	 CORBA::ObjectAdapter* oa);

	template <typename T>
	static void
	_receive_request_ip
	(ServerRequestInfo_impl* sri,
	 T args,
	 CORBA::Context_ptr operation_context,
	 CORBA::ObjectAdapter* oa,
	 CORBA::Object_ptr effective_target)
	{
	    if (S_server_req_int_.size() > 0) {
		// any sri registered?
		sri->arguments(args, FALSE);
		sri->icept_oper(PInterceptor::RECEIVE_REQUEST);
		sri->operation_context(operation_context);
		sri->set_oa(oa);
		sri->effective_target(effective_target);
		if (PInterceptor::Current_impl::current() == NULL) {
		    // resolving current is enough to get static reference later
		    CORBA::ORB_ptr orb = CORBA::ORB_instance
			("mico-local-orb", FALSE);
		    CORBA::Object_var obj2
			= orb->resolve_initial_references("PICurrent");
		}
		assert(PInterceptor::Current_impl::current() != NULL);
		CORBA::AnySeq_var s = sri->slots();
		PInterceptor::Current_impl::current()->slots(s.in());
		PInterceptor::PI::_exec_receive_request(sri);
	    }
	}

	static void
	_send_reply_ip
	(ServerRequestInfo_impl* sri);

	static void
	_send_exception_ip
	(ServerRequestInfo_impl* sri,
	 PortableInterceptor::ReplyStatus reply_status);

	static void
	_send_other_ip
	(ServerRequestInfo_impl* sri,
	 PortableInterceptor::ReplyStatus reply_status,
	 CORBA::Object_ptr obj = CORBA::Object::_nil());

	// IOR Interceptor method
	static void
	_exec_establish_components(PortableInterceptor::IORInfo_ptr info);
			
	// add interceptor methods
	static void
	_exec_add_client_interceptor
	(PortableInterceptor::ClientRequestInterceptor_ptr interceptor);
	
	static void
	_exec_add_server_interceptor
	(PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
	
	static void
	_exec_add_ior_interceptor
	(PortableInterceptor::IORInterceptor_ptr interceptor);

	// SRI/CRI creation methods
	static ServerRequestInfo_impl*
	_create_sri
	(CORBA::Object_ptr object,
	 const char* op_name,
	 CORBA::ULong req_id,
	 CORBA::Boolean resp,
	 IOP::ServiceContextList* scl);

	static ClientRequestInfo_impl*
	_create_cri
	(CORBA::Object_ptr object,
	 const char* op_name,
	 CORBA::ULong req_id = 0,
	 CORBA::NVList_ptr param_list = CORBA::NVList::_nil(),
	 CORBA::ExceptionList_ptr exc_list = CORBA::ExceptionList::_nil(),
	 CORBA::ContextList_ptr ctx_list = CORBA::ContextList::_nil(),
	 CORBA::Context_ptr ctx = CORBA::Context::_nil(),
	 CORBA::NamedValue_ptr res = CORBA::NamedValue::_nil(),
	 CORBA::Boolean resp = TRUE);

	static ClientRequestInfo_impl*
	_create_cri
	(CORBA::Object_ptr object,
	 const char* op_name,
	 CORBA::ExceptionList_ptr exc_list,
	 CORBA::NamedValue_ptr res);

	// SRI/CRI helper methods
	static void
	_set_sri_exception(ServerRequestInfo_impl* sri, const CORBA::Exception* exc);

	static void
	_set_sri_result(ServerRequestInfo_impl* sri, const CORBA::Any& res)
	{
	    if (sri != NULL)
		sri->result(res);
	}

	template <typename T>
	static void
	_set_sri_args(ServerRequestInfo_impl* sri, T args, CORBA::Boolean valid)
	{
	    if (sri != NULL)
		sri->arguments(args, valid);
	}
    };

} // PInterceptor

#endif


