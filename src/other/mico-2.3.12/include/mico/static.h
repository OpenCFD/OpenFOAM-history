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

#ifndef __mico_static_h__
#define __mico_static_h__

namespace CORBA {

typedef void *StaticValueType;
typedef const void *ConstStaticValueType;

class StaticTypeInfo {
protected:
    virtual ~StaticTypeInfo ();
public:
    typedef CORBA::StaticValueType StaticValueType;

    virtual StaticValueType create () const = 0;
    virtual StaticValueType copy (const StaticValueType) const;
    virtual void assign (StaticValueType dst, const StaticValueType src) const = 0;
    virtual void free (StaticValueType) const = 0;
    virtual void release (StaticValueType v) const;
    virtual CORBA::Boolean demarshal (CORBA::DataDecoder &,
				      StaticValueType) const = 0;
    virtual void marshal (CORBA::DataEncoder &, StaticValueType) const = 0;
    virtual CORBA::TypeCode_ptr typecode ();
};

extern MICO_EXPORT StaticTypeInfo *_stc_void;
extern MICO_EXPORT StaticTypeInfo *_stc_short;
extern MICO_EXPORT StaticTypeInfo *_stc_long;
extern MICO_EXPORT StaticTypeInfo *_stc_longlong;
extern MICO_EXPORT StaticTypeInfo *_stc_ushort;
extern MICO_EXPORT StaticTypeInfo *_stc_ulong;
extern MICO_EXPORT StaticTypeInfo *_stc_ulonglong;
extern MICO_EXPORT StaticTypeInfo *_stc_float;
extern MICO_EXPORT StaticTypeInfo *_stc_double;
extern MICO_EXPORT StaticTypeInfo *_stc_longdouble;
extern MICO_EXPORT StaticTypeInfo *_stc_char;
extern MICO_EXPORT StaticTypeInfo *_stc_wchar;
extern MICO_EXPORT StaticTypeInfo *_stc_octet;
extern MICO_EXPORT StaticTypeInfo *_stc_boolean;
extern MICO_EXPORT StaticTypeInfo *_stc_any;
extern MICO_EXPORT StaticTypeInfo *_stc_TypeCode;
extern MICO_EXPORT StaticTypeInfo *_stc_Principal;
extern MICO_EXPORT StaticTypeInfo *_stc_Context;
extern MICO_EXPORT StaticTypeInfo *_stc_Object;
extern MICO_EXPORT StaticTypeInfo *_stc_ValueBase;
extern MICO_EXPORT StaticTypeInfo *_stc_AbstractBase;
extern MICO_EXPORT StaticTypeInfo *_stc_string;
extern MICO_EXPORT StaticTypeInfo *_stc_wstring;

extern MICO_EXPORT StaticTypeInfo *_stcseq_short;
extern MICO_EXPORT StaticTypeInfo *_stcseq_long;
extern MICO_EXPORT StaticTypeInfo *_stcseq_longlong;
extern MICO_EXPORT StaticTypeInfo *_stcseq_ushort;
extern MICO_EXPORT StaticTypeInfo *_stcseq_ulong;
extern MICO_EXPORT StaticTypeInfo *_stcseq_ulonglong;
extern MICO_EXPORT StaticTypeInfo *_stcseq_float;
extern MICO_EXPORT StaticTypeInfo *_stcseq_double;
extern MICO_EXPORT StaticTypeInfo *_stcseq_longdouble;
extern MICO_EXPORT StaticTypeInfo *_stcseq_char;
extern MICO_EXPORT StaticTypeInfo *_stcseq_wchar;
extern MICO_EXPORT StaticTypeInfo *_stcseq_octet;
extern MICO_EXPORT StaticTypeInfo *_stcseq_boolean;
extern MICO_EXPORT StaticTypeInfo *_stcseq_any;
extern MICO_EXPORT StaticTypeInfo *_stcseq_string;
extern MICO_EXPORT StaticTypeInfo *_stcseq_wstring;
extern MICO_EXPORT StaticTypeInfo *_stcseq_Object;
extern MICO_EXPORT StaticTypeInfo *_stcseq_ValueBase;
extern MICO_EXPORT StaticTypeInfo *_stcseq_AbstractBase;


class StaticAny {
    StaticTypeInfo *_info;
    StaticValueType _val;
    CORBA::Boolean _dofree;
    CORBA::Flags _flags;
public:
    StaticAny (StaticTypeInfo *info = _stc_void, ConstStaticValueType val = 0,
	       CORBA::Boolean dofree = FALSE)
	: _info (info), _val ((StaticValueType)val), _dofree (dofree)
    {}

    StaticAny (const StaticAny &a);
    StaticAny &operator= (const StaticAny &);
    ~StaticAny ();

    StaticTypeInfo *type ()
    { return _info; }

    CORBA::TypeCode_ptr typecode ()
    { return _info->typecode(); }

    StaticValueType value ()
    { return _val; }

    void release ()
    { _info->release (_val); }
    
    StaticValueType _retn ();

    void value (StaticTypeInfo *info /*= _stc_void*/,
		ConstStaticValueType val = 0,
		CORBA::Boolean dofree = FALSE);

    void flags (CORBA::Flags f)
    { _flags = f; }

    CORBA::Flags flags () const
    { return _flags; }

    CORBA::Boolean demarshal (CORBA::DataDecoder &);
    CORBA::Boolean marshal (CORBA::DataEncoder &);
};


class StaticRequest;
typedef StaticRequest *StaticRequest_ptr;

class StaticRequest : public CORBA::ORBRequest {
    CORBA::String_var _opname;
    StaticAnyList _args;
    StaticAny *_res;
    CORBA::Context_ptr _ctx;
    CORBA::Environment_ptr _env;
    CORBA::ContextList_ptr _ctx_list;
    CORBA::Object_var _obj;
    CORBA::ServerlessObject_ptr _iceptreq;
    CORBA::ORBMsgId _id;
    // PI client interceptor request info
    PInterceptor::ClientRequestInfo_impl* _cri;

    CORBA::Boolean copy (StaticAnyList *t, StaticAnyList *f, CORBA::Flags);
    CORBA::Environment_ptr env ();
public:
    StaticRequest (CORBA::Object_ptr, const char *opname);
    ~StaticRequest ();

    // ServerRequestBase methods
    CORBA::Environment_ptr environment ();

    // ORBRequest methods
    const char *op_name();

    CORBA::Context_ptr ctx ()
    {
	return CORBA::Context::_duplicate(_ctx);
    }

    CORBA::ContextList_ptr ctx_list()
    {
	return CORBA::ContextList::_duplicate(_ctx_list);
    }
    
    CORBA::Boolean get_in_args (NVList_ptr iparams, Context_ptr &ctx);
    CORBA::Boolean get_in_args (StaticAnyList *iparams,
                                CORBA::Context_ptr &ctx);
    CORBA::Boolean get_in_args (DataEncoder *);
    
    CORBA::Boolean get_out_args (Any *res, NVList_ptr oparams,
                                 Exception *&ex);
    CORBA::Boolean get_out_args (CORBA::StaticAny *res, StaticAnyList *oparams,
                                 CORBA::Exception *&ex);
    CORBA::Boolean get_out_args (DataEncoder *, Boolean &is_except);
    
    CORBA::Boolean set_out_args (Any *res, NVList_ptr oparams);
    CORBA::Boolean set_out_args (CORBA::StaticAny *res,
                                 StaticAnyList *oparams);
    void set_out_args (Exception *ex);
    CORBA::Boolean set_out_args (CORBA::DataDecoder *,
                                 CORBA::Boolean is_except);
    
    CORBA::Boolean copy_out_args (ORBRequest *);
    CORBA::Boolean copy_in_args (ORBRequest *);
    
    const char *type ();

    // own methods
    void add_in_arg (StaticAny *);
    void add_out_arg (StaticAny *);
    void add_inout_arg (StaticAny *);
    void set_context (CORBA::Context_ptr);
    void set_context_list (CORBA::ContextList_ptr);
    void set_result (StaticAny *a)
    {
	_res = a;
    }

    CORBA::Exception *exception ();
    void exception (CORBA::Exception *);

    void invoke ();
    void oneway ();
  
    void send_deferred ();
    void get_response ();

    static StaticRequest_ptr _duplicate (StaticRequest_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static StaticRequest_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<StaticRequest> StaticRequest_var;


class StaticServerRequest : public CORBA::ServerRequestBase {
    typedef std::vector<StaticAny *> StaticAnyList;

    CORBA::ObjectAdapter *_oa;
    CORBA::Object_ptr _obj;
    CORBA::Context_ptr _ctx;
    CORBA::Environment _env;
    ORBRequest *_req;
    StaticAnyList _args;
    StaticAny *_res;
    CORBA::ORBMsgId _id;
    CORBA::ServerlessObject_ptr _iceptreq;
    CORBA::Boolean _aborted;
public:
    StaticServerRequest (ORBRequest *req, Object_ptr obj, CORBA::ORBMsgId id,
			 CORBA::ObjectAdapter *oa, CORBA::Principal_ptr);
    ~StaticServerRequest ();

    const char *op_name()
    {
	return _req->op_name();
    }
    CORBA::Context_ptr ctx ()
    {
	return _ctx;
    }

    void add_in_arg (StaticAny *);
    void add_out_arg (StaticAny *);
    void add_inout_arg (StaticAny *);

    void set_result (StaticAny *a)
    {
	_res = a;
    }
    void set_exception (CORBA::Exception *e)
    {
	_env.exception (e);
    }

    CORBA::Boolean read_args ();
    void write_results ();

    CORBA::Environment_ptr environment ()
    {
	return &_env;
    }

    static StaticServerRequest_ptr _duplicate (StaticServerRequest_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static StaticServerRequest_ptr _nil ()
    {
	return 0;
    }
};

typedef ObjVar<StaticServerRequest> StaticServerRequest_var;


}

#endif // __mico_static_h__
