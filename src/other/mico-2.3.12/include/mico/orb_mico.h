// -*- c++ -*-
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

#ifndef __mico_orb_mico_h__
#define __mico_orb_mico_h__

#ifndef HAVE_THREADS
#  define USE_ORB_CACHE
#endif

namespace CORBA {

class ObjectAdapter;
//class ORBRequest;
struct ORBCallback;

typedef ORBRequest *ORBRequest_ptr;
typedef ObjVar<ORBRequest> ORBRequest_var;
typedef ObjOut<ORBRequest> ORBRequest_out;


class PrincipalCurrent_impl : public CORBA::PrincipalCurrent {
public:
    CORBA::Principal_ptr get_principal ();
};


class ORBInvokeRec {
public:
    typedef ULong MsgId;
private:
    MsgId _myid;
    RequestType _type;
    Boolean _have_result;
    Boolean _active;
    Object_ptr _obj;
    Object_ptr _target;
    Principal_ptr _principal;
    Boolean _response_expected;
    std::string _repoid;
    InvokeStatus _invoke_stat;
    LocateStatus _locate_stat;
    ObjectAdapter *_adapter;
    ORBRequest *_req;
    ORBCallback *_cb;
    ORB_ptr _orb;
    OctetSeq _objtag;
    Address *_addr;
    void *_req_hint;
    void *_inv_hint;
 
    GIOP::AddressingDisposition _ad;
    PInterceptor::ServerRequestInfo_impl* _sri;

public:
    ORBInvokeRec (MsgId);
    virtual ~ORBInvokeRec ();

    void free();

    void init_invoke (ORB_ptr, Object_ptr target,
		      ORBRequest *, Principal_ptr pr,
		      Boolean response_expected,
		      ORBCallback *, ObjectAdapter * = 0);

    void init_locate (ORB_ptr, Object_ptr,
		      ORBCallback *, ObjectAdapter * = 0);

    void init_bind (ORB_ptr, const char *,
		    const OctetSeq &tag, Address *, ORBCallback *,
		    ObjectAdapter * = 0);

    void set_request_hint(void *hint) 
    { _req_hint = hint; }
    void *get_request_hint()
    { return _req_hint; }

    void set_invoke_hint(void *hint) 
    { _inv_hint = hint; }
    void *get_invoke_hint()
    { return _inv_hint; }

    Boolean active() const
    { return _active; };
    void deactivate()
    { _active = FALSE; };

    MsgId id () const
    { return _myid; }

    Boolean completed () const
    { return _have_result; }

    ObjectAdapter *oa ()
    { return _adapter; }

    void oa (ObjectAdapter *oaval)
    { _adapter = oaval; }
    
    RequestType request_type ()
    { return _type; }

    ORBRequest *request ()
    { return _req; }

    const OctetSeq &tag () const
    { return _objtag; }

    ORBCallback *callback ()
    { return _cb; }

    Principal_ptr principal ()
    { return _principal; }
    
    PInterceptor::ServerRequestInfo_impl* requestinfo()
    { return _sri; }

    CORBA::Boolean response_expected()
    { return _response_expected; }
    
    void redo ();

    void set_answer_invoke (InvokeStatus, Object_ptr, ORBRequest *,
			    GIOP::AddressingDisposition);
    void set_answer_bind (LocateStatus, Object_ptr);
    void set_answer_locate (LocateStatus, Object_ptr,
			    GIOP::AddressingDisposition);

    Boolean get_answer_invoke (InvokeStatus &, Object_ptr &, ORBRequest *&,
			       GIOP::AddressingDisposition &);
    Boolean get_answer_bind (LocateStatus &, Object_ptr &);
    Boolean get_answer_locate (LocateStatus &, Object_ptr &,
			       GIOP::AddressingDisposition &);
};



class ORB : public ServerlessObject {
public:
    typedef CORBA::ULong MsgId;
private:
#ifdef HAVE_THREADS
    class ResourceManager {
      ULong _connection_limit;
      ULong _request_limit;
      ULong _connection_count;
      MICOMT::Mutex conn_cnt_mutex;
    public:
      ResourceManager () {
        _connection_limit = 10;
        _request_limit = 10;
        _connection_count = 0;
      }
      inline ULong connection_limit (ULong NewLimit) {
        MICOMT::AutoLock l(conn_cnt_mutex);
        return _connection_limit = NewLimit;
      }
      inline ULong connection_limit (void) const {
        return _connection_limit;
      }
      inline ULong request_limit (ULong NewLimit) {
        return _request_limit = NewLimit;
      }
      inline ULong request_limit (void) const {
        return _request_limit;
      }

      //
      // Not used, but might be handy if we ever want to know how many con-
      // nections are open at any point in time...
      //
      inline ULong connection_count (void) const {
        return _connection_count;
      }

      inline CORBA::Boolean acquire_connection (void) {
	MICOMT::AutoLock l(conn_cnt_mutex);
	if (_connection_limit > 0) {
	  if (_connection_count < _connection_limit) {
	    _connection_count++;
	    return TRUE;
	  }
	  return FALSE;
	}
	else {
	  // connection checking disabled by -ORBConnLimit 0
	  _connection_count++;
	  return TRUE;
	}
      }
      inline void release_connection (void) {
        MICOMT::AutoLock l(conn_cnt_mutex);
        _connection_count--;
      }
    } resourceManager;
#endif // HAVE_THREADS
    typedef std::vector<ObjectAdapter *> OAVec;
    typedef std::map<MsgId, ORBInvokeRec *, std::less<MsgId> > InvokeMap;
    typedef std::map<std::string, Object_var, std::less<std::string> >
    InitialRefMap;
    typedef std::map<std::string, ValueFactoryBase_var, std::less<std::string> >
    ValueFactoryMap;
    typedef std::list<std::string> isa_cacheList;

    std::string _default_init_ref;

    MICOMT::Locked<isa_cacheList> _isa_cache;
    std::vector<std::string> _bindaddrs;
    MICOMT::RWLocked<OAVec> _adapters;
    MICOMT::RWLocked<InvokeMap> _invokes;

    Dispatcher *_disp;
    IOR *_tmpl;

    MsgId _theid;
    MICOMT::Mutex _theid_lock;
#ifndef HAVE_THREADS
    std::stack<MsgId> _currentid;
#else // HAVE_THREADS
    MICOMT::Thread::ThreadKey _current_rec_key;
    MICOMT::Mutex run_lock_;
    MICOMT::Thread::ThreadID main_thread_id_;
    MICOMT::Mutex shutdown_lock_;
#endif // HAVE_THREADS
    InitialRefMap _init_refs;
    MICOMT::Mutex _init_refs_lock;

    std::string _rcfile;
    DomainManager_var _def_manager;

    MICOMT::Locked<ValueFactoryMap> _value_facs;

    Boolean _is_plugged;
    Boolean _is_running;
    Boolean _is_shutdown;
    Boolean _is_stopped;
    Boolean _wait_for_completion;
    OAVec _shutting_down_adapters;

    OctetSeq _terminal_id;

#ifdef USE_ORB_CACHE
    ORBInvokeRec *_cache_rec;
    CORBA::Boolean _cache_used;
#endif

    CORBA::DispatcherFactory* dispatcher_factory_;

    Boolean is_local (Object_ptr);
    ObjectAdapter *get_oa (Object_ptr);
    ORBInvokeRec *create_invoke (MsgId);
    void add_invoke (ORBInvokeRec *);
    ORBInvokeRec *get_invoke (MsgId);
    void del_invoke (MsgId);

    // internal ORBMsgId handling
    ORBInvokeRec *get_invoke (ORBMsgId id) { return id; };

    void do_shutdown ();

    CORBA::Object_ptr corbaloc_to_object (const char *);
    CORBA::Object_ptr corbaname_to_object (const char *);
    CORBA::Object_ptr iioploc_to_object (const char *);
    CORBA::Object_ptr iiopname_to_object (const char *);
    CORBA::Object_ptr file_to_object (const char *);
    CORBA::Object_ptr http_to_object (const char *);

    Boolean builtin_invoke (ORBMsgId, Object_ptr,
                            ORBRequest *, Principal_ptr);
    Object_ptr _resolve_initial_references (const char *id);
public:
#ifdef HAVE_THREADS
    inline ResourceManager &resource_manager (void) {
      return resourceManager;
    }

    void
    set_main_thread_id(MICOMT::Thread::ThreadID id)
    { main_thread_id_ = id; }
#endif // HAVE_THREADS
    typedef IfaceSequenceTmpl<Request_var,Request_ptr> RequestSeq;
    typedef TSeqVar<RequestSeq> RequestSeq_var;

    typedef OctetSeq ObjectTag;
    typedef TSeqVar<ObjectTag> ObjectTag_var;

    ORB (int &argc, char **argv, const char *rcfile);
    virtual ~ORB ();

    char *object_to_string (Object_ptr);
    Object_ptr string_to_object (const char *);
    Object_ptr ior_to_object (IOR *);
    Boolean is_impl (Object_ptr);
    const char *rcfile();

    static char *tag_to_string (const ObjectTag &);
    static ObjectTag *string_to_tag (const char *);

    void create_list (Long, NVList_out);
    void create_operation_list (OperationDef *,
				NVList_out);
    void create_named_value (NamedValue_out);
    void create_exception_list (ExceptionList_out);
    void create_context_list (ContextList_out);
    void create_environment (Environment_out);
    
    void get_default_context (Context_out);

    // mico-extension
    void get_default_domain_manager (DomainManager_out);

    CORBA::Boolean get_service_information (
	CORBA::ServiceType service_type,
	CORBA::ServiceInformation_out service_information);

    void send_multiple_requests_oneway (const RequestSeq &);
    void send_multiple_requests_deferred (const RequestSeq &);
    Boolean poll_next_response ();
    void get_next_response (Request_out);

    Boolean work_pending ();
    void perform_work ();
    void run ();
    void shutdown (Boolean wait_for_completion);
    void destroy ();

    typedef char *OAid;
    typedef CORBA::ORB_InvalidName InvalidName;
    typedef CORBA::ORB_InvalidName_catch InvalidName_catch;

    typedef char *ObjectId;
    typedef StringSequenceTmpl<String_var> ObjectIdList;
    typedef ObjectIdList *ObjectIdList_ptr;
    typedef TSeqVar<ObjectIdList> ObjectIdList_var;

    ObjectIdList_ptr list_initial_services ();
    Object_ptr resolve_initial_references (const char *id)
	{
	    MICOMT::AutoLock l(_init_refs_lock);
	    return _resolve_initial_references(id);
	}
    Boolean set_initial_reference (const char *id, Object_ptr obj);
    // added according to PI spec
    void register_initial_reference (const char *id, Object_ptr obj);
    // end
    Boolean set_default_initial_reference (const char *);

    Boolean plugged () { return _is_plugged; }
    void plugged (Boolean which) { _is_plugged = which; }
    const Octet *terminal_id (ULong &length);
    void terminal_id (const Octet *val, ULong len);

    static Boolean is_mobile_key (const CORBA::Octet *);

    virtual TypeCode_ptr create_struct_tc (const char *rep_id,
					   const char *name,
					   const StructMemberSeq &);
    virtual TypeCode_ptr create_union_tc (const char *rep_id,
					  const char *name,
					  TypeCode_ptr disc_type,
					  const UnionMemberSeq &);
    virtual TypeCode_ptr create_enum_tc (const char *rep_id,
					 const char *name,
					 const EnumMemberSeq &);
    virtual TypeCode_ptr create_exception_tc (const char *rep_id,
					      const char *name,
					      const StructMemberSeq &);
    virtual TypeCode_ptr create_alias_tc (const char *rep_id,
					  const char *name,
					  TypeCode_ptr orig_type);
    virtual TypeCode_ptr create_interface_tc (const char *rep_id,
					      const char *name);
    virtual TypeCode_ptr create_string_tc (ULong bound);
    virtual TypeCode_ptr create_wstring_tc (ULong bound);
    virtual TypeCode_ptr create_fixed_tc (UShort digits, Short scale);
    virtual TypeCode_ptr create_sequence_tc (ULong bound,
					     TypeCode_ptr el_type);
    virtual TypeCode_ptr create_recursive_sequence_tc (ULong bound,
						       ULong offset);
    virtual TypeCode_ptr create_array_tc (ULong length,
					  TypeCode_ptr el_type);
    virtual TypeCode_ptr create_value_tc (const char *rep_id, const char *name,
					  ValueModifier mod,
					  TypeCode_ptr concrete_base,
					  const ValueMemberSeq &members);
    virtual TypeCode_ptr create_value_box_tc (const char *rep_id,
					      const char *name,
					      TypeCode_ptr boxed_type);
    virtual TypeCode_ptr create_native_tc (const char *rep_id,
					   const char *name);
    virtual TypeCode_ptr create_recursive_tc (const char *rep_id);
    virtual TypeCode_ptr create_abstract_interface_tc (const char *rep_id,
						       const char *name);

    // begin-value
    ValueFactory register_value_factory (const char *repoid,
					 ValueFactory factory);
    void unregister_value_factory (const char *repoid);
    ValueFactory lookup_value_factory (const char *repoid);
    // end-value

    CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
				     const CORBA::Any &any);

    // begin-mico-extension
    const std::vector<std::string> &bindaddrs () const
    {
	return _bindaddrs;
    }
    void bindaddrs (const std::vector<std::string> &b)
    {
	_bindaddrs = b;
    }

    void dispatcher (Dispatcher *);
    Dispatcher *dispatcher ()
    {
	return _disp;
    }
    IOR *ior_template ()
    {
	return _tmpl;
    }

    void register_oa (ObjectAdapter *);
    void unregister_oa (ObjectAdapter *);

    void register_profile_id (CORBA::ULong id);
    void unregister_profile_id (CORBA::ULong id);

    MsgId new_msgid ();
    
#ifdef USE_CSL2
    Principal_ptr get_principal (Object_ptr);
#endif // USE_CSL2

    ORBMsgId new_orbid (MsgId msgid = 0);
    static MsgId get_msgid (ORBMsgId rec) { return rec ? rec->id() : 0; };
    ORBMsgId get_orbid (MsgId id) { return get_invoke(id); };

    //FIXME: hopefully nobody is going to shot me for this :-)
    void set_request_hint(ORBMsgId id, void *hint) {
	ORBInvokeRec *rec = get_invoke (id);
	assert( rec );
	rec->set_request_hint( hint );
    };
    void *get_request_hint(ORBMsgId id) {
	ORBInvokeRec *rec = get_invoke (id);
	assert( rec );
	return rec->get_request_hint();
    };
    void set_invoke_hint(ORBMsgId id, void *hint) {
	ORBInvokeRec *rec = get_invoke (id);
	assert( rec );
	rec->set_invoke_hint( hint );
    };
    void *get_invoke_hint(ORBMsgId id) {
	ORBInvokeRec *rec = get_invoke (id);
	assert( rec );
	return rec->get_invoke_hint();
    };

    ORBMsgId invoke_async (Object_ptr, ORBRequest *, Principal_ptr,
			   Boolean rply = TRUE, ORBCallback * = 0,
			   ORBMsgId = 0);
    ORBMsgId locate_async (Object_ptr, ORBCallback * = 0, ORBMsgId = 0);
    ORBMsgId bind_async (const char *repoid, const ObjectTag &,
			 Address *, ORBCallback * = 0, ORBMsgId = 0);

    InvokeStatus invoke (Object_ptr &, ORBRequest *, Principal_ptr,
			 Boolean rply = TRUE);
    LocateStatus locate (Object_ptr &);
    LocateStatus bind (const char *repoid, const ObjectTag &,
		       Address *, Object_ptr &);
    Object_ptr bind (const char *repoid, const char *addr = 0);
    Object_ptr bind (const char *repoid, const ObjectTag &,
		     const char *addr = 0);

    void    cancel (ORBMsgId);
    void    cancel (MsgId);
    Boolean wait (ORBMsgId, Long tmout = -1);
    // Long    wait (vector<MsgId> &, Long tmout = -1);

    void answer_invoke (ORBMsgId, InvokeStatus, Object_ptr, ORBRequest *,
			GIOP::AddressingDisposition);
    void answer_bind   (ORBMsgId, LocateStatus, Object_ptr);
    void answer_locate (ORBMsgId, LocateStatus, Object_ptr,
			GIOP::AddressingDisposition);
    void answer_shutdown (ObjectAdapter *);

    InvokeStatus get_invoke_reply (ORBMsgId, Object_out, ORBRequest *&,
				   GIOP::AddressingDisposition &);
    LocateStatus get_locate_reply (ORBMsgId, Object_out,
				   GIOP::AddressingDisposition &);
    LocateStatus get_bind_reply   (ORBMsgId, Object_out);

    ORBRequest *request (ORBMsgId);
    RequestType request_type (ORBMsgId);
    void redo_request (ORBMsgId);

    ImplementationDef    *get_impl (Object_ptr);
    InterfaceDef         *get_iface (Object_ptr);
    Object               *get_component (Object_ptr);
    Boolean               is_a (Object_ptr, const char *repoid);
    Boolean               non_existent (Object_ptr);

    ORBInvokeRec         *get_current_invoke_rec ();


    CORBA::Dispatcher*
    create_dispatcher();

    void
    dispatcher_factory(CORBA::DispatcherFactory* fact);
    // end-mico-extension

    static ORB_ptr _duplicate (ORB_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static ORB_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<ORB> ORB_var;
typedef ObjOut<ORB> ORB_out;


// ORB initialization
typedef char *ORBid;
extern ORB_ptr ORB_init (int &argc, char **argv,
				       const char *id = "");
extern ORB_ptr ORB_instance (const char *id, Boolean create=TRUE);


class ObjectAdapter {
public:
    typedef CORBA::ULong MsgId;

    virtual const char *get_oaid () const = 0;
    virtual Boolean has_object (Object_ptr) = 0;
    virtual Boolean is_local () const = 0;

#ifdef USE_CSL2
    virtual CORBA::Principal_ptr get_principal (CORBA::Object_ptr) = 0;
#endif // USE_CSL2

    virtual Boolean invoke (ORBMsgId, Object_ptr, ORBRequest *,
			    Principal_ptr, Boolean response_exp = TRUE) = 0;
    virtual Boolean bind (ORBMsgId, const char *repoid,
			  const ORB::ObjectTag &,
			  Address *addr) = 0;
    virtual Boolean locate (ORBMsgId, Object_ptr) = 0;
    virtual Object_ptr skeleton (Object_ptr) = 0;
    virtual void cancel (ORBMsgId) = 0;
    virtual void shutdown (Boolean wait_for_completion) = 0;

    virtual void answer_invoke (ORBMsgId, CORBA::Object_ptr,
				CORBA::ORBRequest *,
				CORBA::InvokeStatus) = 0;

    virtual ~ObjectAdapter ();
};

}

#endif // __mico_orb_mico_h__
