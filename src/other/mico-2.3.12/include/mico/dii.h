// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2003 by The Mico Team
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

#ifndef __mico_dii_h__
#define __mico_dii_h__

namespace CORBA {

// environment
class Environment : public ServerlessObject {
    Exception *_exceptn;
    Principal_ptr _princ;
public:
    void exception (Exception *);
    Exception *exception () const
    { return _exceptn; }

    void clear ();

    // begin-mico-extension
    Environment (Principal_ptr = NULL);
    ~Environment ();

    Principal_ptr principal ()
    { return _princ; }
    // end-mico-extension

    static Environment_ptr _duplicate (Environment_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static Environment_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<Environment> Environment_var;


// named value
class NamedValue : public ServerlessObject {
    String_var _n;
    Any _v;
    Flags _f;
public:
    const char *name () const
    { return _n.in(); }

    Any *value () const
    { return (Any *)&_v; }

    Flags flags () const
    { return _f; }

    // begin-mico-extension
    NamedValue (const char *name, const Any &, Flags f = 0);
    NamedValue (const char *name, Flags f = 0);
    NamedValue (); // vector<NamedValue> needs this
    NamedValue &operator= (const NamedValue &);
    ~NamedValue ();
    // end-mico-extension

    static NamedValue_ptr _duplicate (NamedValue_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static NamedValue_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<NamedValue> NamedValue_var;
typedef ObjOut<NamedValue> NamedValue_out;

enum NVListFlags {
    IN_COPY_VALUE = 0x0001,
    DEPENDENT_LIST = 0x0002
};

// nv list
class NVList : public ServerlessObject {
    std::vector<NamedValue_var> _vec;
public:
    typedef CORBA::Bounds Bounds;
    typedef CORBA::Bounds_catch Bounds_catch;

    ULong count () const
    { MICO_OBJ_CHECK (this); return _vec.size(); }

    NamedValue_ptr add (Flags);
    NamedValue_ptr add_item (const char *, Flags);
    NamedValue_ptr add_value (const char *, const Any &, Flags);
    NamedValue_ptr add_item_consume (char *, Flags);
    NamedValue_ptr add_value_consume (char *, Any *, Flags);
    NamedValue_ptr item (ULong idx);
    void remove (ULong);

    // begin-mico-extension
    NVList (ULong count = 0);
    ~NVList ();

    CORBA::Boolean copy (NVList_ptr, Flags flags);
    // end-mico-extension

    static NVList_ptr _duplicate (NVList_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static NVList_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<NVList> NVList_var;
typedef ObjOut<NVList> NVList_out;


// exception list
class ExceptionList : public ServerlessObject {
    std::vector<TypeCode_var> _vec;

public:
    typedef CORBA::Bounds Bounds;
    typedef CORBA::Bounds_catch Bounds_catch;

    ULong count () const
    { MICO_OBJ_CHECK (this); return _vec.size(); }

    void add (TypeCode_ptr tc);
    void add_consume (TypeCode_ptr tc);
    TypeCode_ptr item (ULong idx);
    void remove (ULong idx);

    // begin-mico-extension
    ExceptionList ();
    ~ExceptionList ();
    // end-mico-extension

    static ExceptionList_ptr _duplicate (ExceptionList_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static ExceptionList_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<ExceptionList> ExceptionList_var;
typedef ObjOut<ExceptionList> ExceptionList_out;


// context list
class ContextList : public ServerlessObject {
    std::vector<std::string> _vec;

public:
    typedef CORBA::Bounds Bounds;
    typedef CORBA::Bounds_catch Bounds_catch;

    ULong count () const
    { MICO_OBJ_CHECK (this); return _vec.size(); }

    void add (const char *ctxt);
    void add_consume (char *ctxt);
    const char *item (ULong idx);
    void remove (ULong idx);

    // begin-mico-extension
    ContextList ();
    ~ContextList ();
    // end-mico-extension

    static ContextList_ptr _duplicate (ContextList_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static ContextList_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<ContextList> ContextList_var;


enum ArgModeFlags {
    ARG_IN = 0x0001,
    ARG_OUT = 0x0002,
    ARG_INOUT = 0x0004
};

enum RequestFlags {
    OUT_LIST_MEMORY = 0x0001,
    INV_NO_RESPONSE = 0x0002,
    INV_TERM_ON_ERR = 0x0004,
    RESP_NO_WAIT = 0x0008
};

struct RequestCallback {
    enum Event { RequestDone };
    virtual void callback (Request_ptr, Event) = 0;
    virtual ~RequestCallback ();
};

// request
class Request : public ServerlessObject, public ORBCallback {

    Object_var _object;
    Context_ptr _context;
    String_var _opname;
    NVList_ptr _args;
    NamedValue_ptr _res;
    Flags _flags;
    ExceptionList_ptr _elist;
    ContextList_ptr _clist;
    Environment_ptr _environm;
    CORBA::ORBMsgId _orbid;
    Boolean _invoke_pending;
    ORBRequest *_orbreq;
    RequestCallback *_cb;
    ServerlessObject_ptr _iceptreq;
    // PI client interceptor request info
    PInterceptor::ClientRequestInfo_impl* _cri;
    CORBA::Boolean _invoke_flag;
#ifdef HAVE_THREADS
    ORBAsyncCallback async_callback;
#endif

    Request (const Request &r)
        : CORBA::ServerlessObject(r),
          CORBA::ORBCallback()
    { assert (0); }

    Request &operator= (const Request &) { assert (0); return *_nil(); }

    void decode_user_exception ();

    virtual Boolean waitfor (ORB_ptr orb, CORBA::ORBMsgId id, ORBCallback::Event ev, Long tmout = -1);
    virtual void notify (ORB_ptr orb, CORBA::ORBMsgId id, ORBCallback::Event ev);
public:
    Object_ptr target () const
    { return _object.in(); }

    const char *operation () const
    { return _opname; }

    NVList_ptr arguments ()
    { MICO_OBJ_CHECK (this); return _args; }

    NamedValue_ptr result ()
    { return _res; }

    Environment_ptr env ()
    { return _environm; }

    ExceptionList_ptr exceptions ()
    { return _elist; }

    ContextList_ptr contexts ()
    { return _clist; }
    
    void ctx (Context_ptr);
    Context_ptr ctx () const
    { return _context; }

    void invoke ();
    void send_oneway ();
    void send_deferred (RequestCallback *cb = 0);
    void get_response (Boolean block = TRUE);
    Boolean poll_response ();

    Any &add_in_arg ();
    Any &add_in_arg (const char *name);
    Any &add_inout_arg ();
    Any &add_inout_arg (const char *name);
    Any &add_out_arg ();
    Any &add_out_arg (const char *name);
    void set_return_type (TypeCode_ptr);
    Any &return_value ();

    // begin-mico-extension
    Request (Object_ptr, Context_ptr, const char *op,
	     NVList_ptr args, NamedValue_ptr result,
	     Flags f);
    Request (Object_ptr, Context_ptr, const char *op,
	     NVList_ptr args, NamedValue_ptr result,
	     ExceptionList_ptr, ContextList_ptr,
	     Flags f);
    Request (Object_ptr, const char *op);
    Request (); // vector<Request> needs this
    ~Request ();
    // end-mico-extension

    static Request_ptr _duplicate (Request_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static Request_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<Request> Request_var;


enum ContextFlags {
    CTX_DELETE_DESCENDENTS = 0x0001,
    CTX_RESTRICT_SCOPE = 0x0002
};

typedef ObjVar<Context> Context_var;
typedef ObjOut<Context> Context_out;

// context
class Context : public ServerlessObject {
    Context_ptr _myparent;
    std::vector<Context_var> _childs;
    NVList_var _properties;
    std::string _name;

    Boolean match (const char *value, const char *pattern);
public:
    const char *context_name () const;
    Context_ptr parent () const;

    void create_child (const char *, Context_out);

    void set_one_value (const char *, const Any &);
    void set_values (NVList_ptr);
    void delete_values (const char *);
    void get_values (const char *, Flags, const char *, NVList_out);
    void get_values (const char *, Flags, ContextList_ptr, NVList_out);

    // begin-mico-extension
    Context (DataDecoder &);
    Context (const char *name, Context_ptr parent = 0);
    ~Context ();

    void encode (DataEncoder &, ContextList_ptr = 0) const;
    Boolean decode (DataDecoder &);
    // end-mico-extension

    static Context_ptr _duplicate (Context_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static Context_ptr _nil ()
    {
	return 0;
    }
};


// principal
class Principal : public ServerlessObject {
    std::vector<Octet> _rep;
    static std::vector<Octet> _peer_info;
    CORBA::Transport *_transp;
public:
    // begin-mico-extension
    Principal (DataDecoder &, CORBA::Transport * = 0);
    Principal (CORBA::Transport * = 0);
    virtual ~Principal ();
#ifdef USE_CSL2
    CORBA::Transport* transport(); // ###ras
#endif /* USE_CSL2  */
    CORBA::Boolean operator== (const Principal &) const;

    void encode (DataEncoder &) const;
    Boolean decode (DataDecoder &);

    typedef StringSequenceTmpl<String_var> PropertyNameList;
    typedef PropertyNameList *PropertyNameList_ptr;
    typedef TVarVar<PropertyNameList> PropertyNameList_var;
    typedef TVarOut<PropertyNameList> PropertyNameList_out;

    virtual PropertyNameList_ptr list_properties ();
    virtual Any* get_property (const char *prop_name);
#ifdef USE_CSL2
    virtual Any* get_attribute(unsigned long fam, unsigned long attr); // ###ras
    virtual CORBA::Boolean check(std::string& s);
#endif /* USE_CSL2  */

    static void set_peer_info (const OctetSeq &);
    // end-mico-extension

    static Principal_ptr _duplicate (Principal_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static Principal_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<Principal> Principal_var;

}

#endif // __mico_dii_h__
