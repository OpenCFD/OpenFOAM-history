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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_INTERCEPT
#define MICO_CONF_IR
#include <CORBA.h>
#include <mico/pi_impl.h>
#include <mico/impl.h>
#include <mico/poa_impl.h>
#include <mico/ssl.h>

#endif // FAST_PCH

using namespace PortableInterceptor;
using namespace std;

//
// Global ORB initializer
//

PInterceptor::Current_impl* PInterceptor::Current_impl::S_current_ = NULL;
vector<PortableInterceptor::ORBInitializer_var>
PInterceptor::PI::S_initializers_;
vector<PortableInterceptor::ORBInitializer_var>
PInterceptor::PI::S_initializers_backup_;
PortableInterceptor::SlotId PInterceptor::PI::S_max_slot_id_ = 0;
PInterceptor::PI::PolicyFactoryMap PInterceptor::PI::S_pfmap_;
MICOMT::Mutex PInterceptor::PI::S_client_lock_(FALSE, MICOMT::Mutex::Recursive);
MICOMT::Mutex PInterceptor::PI::S_server_lock_(FALSE, MICOMT::Mutex::Recursive);
PInterceptor::PI::IORList PInterceptor::PI::S_ior_interceptors_;
PInterceptor::PI::IORList PInterceptor::PI::S_ior_interceptors_prepared_;

vector<PortableInterceptor::ClientRequestInterceptor_ptr>
PInterceptor::PI::S_client_req_int_;
vector<PortableInterceptor::ClientRequestInterceptor_ptr>
PInterceptor::PI::S_client_req_int_prepared_;
vector<PortableInterceptor::ServerRequestInterceptor_ptr>
PInterceptor::PI::S_server_req_int_;
vector<PortableInterceptor::ServerRequestInterceptor_ptr>
PInterceptor::PI::S_server_req_int_prepared_;

stack<PInterceptor::PI::ClientFlowStack*> PInterceptor::PI::S_client_stack_;
stack<PInterceptor::PI::ServerFlowStack*> PInterceptor::PI::S_server_stack_;

void
PortableInterceptor::register_orb_initializer
(PortableInterceptor::ORBInitializer_ptr init)
{
    PInterceptor::PI::S_initializers_.push_back
	(PortableInterceptor::ORBInitializer::_duplicate(init));
}

void
PortableInterceptor::destroy_all_interceptors()
{
    PInterceptor::PI::IORList::iterator it_ior;
    for (it_ior = PInterceptor::PI::S_ior_interceptors_.begin();
  	 it_ior != PInterceptor::PI::S_ior_interceptors_.end();
	 it_ior++) {
	(*it_ior)->destroy();
    }
    PInterceptor::PI::S_ior_interceptors_.erase
	(PInterceptor::PI::S_ior_interceptors_.begin(),
	 PInterceptor::PI::S_ior_interceptors_.end());
    vector<ClientRequestInterceptor_ptr>::iterator it_client;
    for (it_client = PInterceptor::PI::S_client_req_int_.begin();
	 it_client != PInterceptor::PI::S_client_req_int_.end();
	 it_client++) {
	(*it_client)->destroy();
	CORBA::release(*it_client);
    }
    PInterceptor::PI::S_client_req_int_.erase
	(PInterceptor::PI::S_client_req_int_.begin(),
	 PInterceptor::PI::S_client_req_int_.end());
    vector<ServerRequestInterceptor_ptr>::iterator it_server;
    for (it_server = PInterceptor::PI::S_server_req_int_.begin();
	 it_server != PInterceptor::PI::S_server_req_int_.end();
	 it_server++) {
	(*it_server)->destroy();
	CORBA::release(*it_server);
    }
    PInterceptor::PI::S_server_req_int_.erase
	(PInterceptor::PI::S_server_req_int_.begin(),
	 PInterceptor::PI::S_server_req_int_.end());
}


void
PortableInterceptor::backup_initializers()
{
    PInterceptor::PI::S_initializers_backup_ = PInterceptor::PI::S_initializers_;
}


void
PortableInterceptor::restore_initializers()
{
    PInterceptor::PI::S_initializers_ = PInterceptor::PI::S_initializers_backup_;
}


void
PortableInterceptor::commit_initialized_interceptors()
{
    PInterceptor::PI::S_client_req_int_
        = PInterceptor::PI::S_client_req_int_prepared_;
    PInterceptor::PI::S_client_req_int_prepared_.erase
        (PInterceptor::PI::S_client_req_int_prepared_.begin(),
         PInterceptor::PI::S_client_req_int_prepared_.end());
    PInterceptor::PI::S_server_req_int_
        = PInterceptor::PI::S_server_req_int_prepared_;
    PInterceptor::PI::S_server_req_int_prepared_.erase
        (PInterceptor::PI::S_server_req_int_prepared_.begin(),
         PInterceptor::PI::S_server_req_int_prepared_.end());
    PInterceptor::PI::S_ior_interceptors_
        = PInterceptor::PI::S_ior_interceptors_prepared_;
    PInterceptor::PI::S_ior_interceptors_prepared_.erase
        (PInterceptor::PI::S_ior_interceptors_prepared_.begin(),
         PInterceptor::PI::S_ior_interceptors_prepared_.end());
}

#ifdef HAVE_THREADS
// cleanup function for per-thread slots
void
PInterceptor::cleanup_slots(void* value)
{
    CORBA::AnySeq* slots = static_cast<CORBA::AnySeq*>(value);
    if (slots != NULL)
	delete slots;
}
#endif // HAVE_THREADS
//
// CodecFactory_impl
//

PICodec::CodecFactory_impl::CodecFactory_impl()
{
}

PICodec::CodecFactory_impl::~CodecFactory_impl()
{
}

IOP::Codec_ptr
PICodec::CodecFactory_impl::create_codec(const IOP::Encoding& enc)
{
    IOP::CodecFactory::UnknownEncoding ex;
    if (enc.format != IOP::ENCODING_CDR_ENCAPS
	&& enc.major_version != (CORBA::Octet)1) {
	mico_user_throw_macro(ex);
	return IOP::Codec::_nil();
    }
    if (enc.minor_version <= (CORBA::Octet)2)
	return (new PICodec::Codec_impl(enc));
    else 
	mico_throw(ex);
    return IOP::Codec::_nil();
}

//
// Codec_impl
//

PICodec::Codec_impl::Codec_impl()
{
    encoding_.format = IOP::ENCODING_CDR_ENCAPS;
    encoding_.major_version = (CORBA::Octet)1;
    encoding_.minor_version = (CORBA::Octet)0;
}

PICodec::Codec_impl::Codec_impl(const IOP::Encoding& enc)
{
    encoding_ = enc;
}

PICodec::Codec_impl::~Codec_impl()
{
}

CORBA::OctetSeq*
PICodec::Codec_impl::encode(const CORBA::Any& data)
{
    CORBA::TypeCode_var tc = data.type();
    if (encoding_.minor_version < (CORBA::Octet)2 && this->find_tc(tc)) {
	mico_throw(IOP::Codec::InvalidTypeForEncoding());
	return 0;
    }

    CORBA::DataEncoder* ec = this->get_ec();

    CORBA::Boolean endian = FALSE; // big endian
    if (ec->byteorder() == CORBA::LittleEndian)
	endian = TRUE;
    ec->put_boolean(endian);

    ec->put_any(data);
    CORBA::Buffer* buf = ec->buffer();
    buf->rseek_beg(0);
    CORBA::ULong length = buf->length();
    CORBA::Octet* t_buf = buf->buffer();
    CORBA::OctetSeq* retval = new CORBA::OctetSeq(length, length, t_buf, FALSE);
    delete ec;
    return retval;
}


CORBA::Any*
PICodec::Codec_impl::decode(const CORBA::OctetSeq& data)
{
    CORBA::ULong length = data.length();
    if (length == 0) {
	mico_throw(IOP::Codec::FormatMismatch());
	return 0;
    }
    CORBA::Buffer* buf = new CORBA::Buffer;
    for (CORBA::ULong i=0; i<length; i++)
	buf->put(data[i]);
	
    CORBA::DataDecoder* dc = this->get_dc(buf);
    CORBA::Boolean endian;
    if (dc->get_boolean(endian)) {
	if (endian) {
	    dc->byteorder(CORBA::LittleEndian);
	}
	else {
	    dc->byteorder(CORBA::BigEndian);
	}
    }
    else {
	mico_throw(IOP::Codec::FormatMismatch());
    }
    CORBA::Any* any = new CORBA::Any;
    if (!dc->get_any(*any)) {
	delete dc;
	mico_throw(IOP::Codec::FormatMismatch());
	return 0;
    }
    delete dc;
    return any;
}


CORBA::OctetSeq*
PICodec::Codec_impl::encode_value(const CORBA::Any& data)
{
    CORBA::TypeCode_var tc = data.type();
    if (encoding_.minor_version < (CORBA::Octet)2 && this->find_tc(tc)) {
	mico_throw(IOP::Codec::InvalidTypeForEncoding());
	return 0;
    }

    CORBA::DataEncoder* ec = this->get_ec();
	
    CORBA::Boolean endian = FALSE; // big endian
    if (ec->byteorder() == CORBA::LittleEndian)
	endian = TRUE;
    ec->put_boolean(endian);

    CORBA::Any any = data;
    any.marshal(*ec);

    CORBA::Buffer* buf = ec->buffer();
    buf->rseek_beg(0);
    CORBA::ULong length = buf->length();
    CORBA::Octet* t_buf = buf->buffer();
    CORBA::OctetSeq* retval = new CORBA::OctetSeq(length, length, t_buf, FALSE);
    delete ec;
    return retval;
}


CORBA::Any*
PICodec::Codec_impl::decode_value
(const CORBA::OctetSeq& data,
 CORBA::TypeCode_ptr tc)
{
    CORBA::ULong length = data.length();
    if (length == 0) {
	mico_throw(IOP::Codec::FormatMismatch());
	return 0;
    }
    CORBA::Buffer* buf = new CORBA::Buffer;
    for (CORBA::ULong i=0; i<length; i++)
	buf->put(data[i]);
	
    CORBA::DataDecoder* dc = this->get_dc(buf);
    CORBA::Boolean endian;
    if (dc->get_boolean(endian)) {
	if (endian) {
	    dc->byteorder(CORBA::LittleEndian);
	}
	else {
	    dc->byteorder(CORBA::BigEndian);
	}
    }
    else {
	mico_throw(IOP::Codec::FormatMismatch());
    }
    CORBA::Any* any = new CORBA::Any;
    if (!any->demarshal(tc, *dc)) {
	delete dc;
	mico_throw(IOP::Codec::FormatMismatch());
	return 0;
    }
    delete dc;
    return any;
}


CORBA::Boolean
PICodec::Codec_impl::find_tc(CORBA::TypeCode_ptr tc)
{
    CORBA::ULong i;
    CORBA::Boolean retval = FALSE;
    switch (tc->unalias()->kind()) {
    case CORBA::tk_wstring :
    case CORBA::tk_wchar : 
	retval = TRUE;
	break;
    case CORBA::tk_struct :
    case CORBA::tk_except :
    case CORBA::tk_union :
	for (i=0; i<tc->member_count(); i++) {
	    if (this->find_tc(tc->member_type(i)))
		return TRUE;
	}
	break;
    case CORBA::tk_sequence :
    case CORBA::tk_array :
	if (this->find_tc(tc->content_type()))
	    return TRUE;
    default:
	break;
    }
    return retval;
}


CORBA::DataEncoder *
PICodec::Codec_impl::get_ec()
{
    CORBA::Codeset::CodesetId csid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultCS)->id();
    CORBA::Codeset::CodesetId wcsid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultWCS)->id();

    CORBA::CodeSetCoder* conv = NULL;
    if (encoding_.minor_version == (CORBA::Octet)0)
	conv = new MICO::GIOP_1_0_CodeSetCoder();
    else if (encoding_.minor_version == (CORBA::Octet)1)
	conv = new MICO::GIOP_1_1_CodeSetCoder(csid);
    else if (encoding_.minor_version == (CORBA::Octet)2)
	conv = new MICO::GIOP_1_2_CodeSetCoder(csid, wcsid);

    CORBA::Buffer* buf = new CORBA::Buffer;
    return (new MICO::CDREncoder(buf, TRUE, CORBA::DefaultEndian, conv));
}

CORBA::DataDecoder *
PICodec::Codec_impl::get_dc(CORBA::Buffer* buf)
{
    CORBA::Codeset::CodesetId csid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultCS)->id();
    CORBA::Codeset::CodesetId wcsid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultWCS)->id();
	
    CORBA::CodeSetCoder* conv = NULL;
    if (encoding_.minor_version == (CORBA::Octet)0)
	conv = new MICO::GIOP_1_0_CodeSetCoder();
    else if (encoding_.minor_version == (CORBA::Octet)1)
	conv = new MICO::GIOP_1_1_CodeSetCoder(csid);
    else if (encoding_.minor_version == (CORBA::Octet)2)
	conv = new MICO::GIOP_1_2_CodeSetCoder(csid, wcsid);
    return (new MICO::CDRDecoder(buf, TRUE, CORBA::DefaultEndian, conv));
}


//
// RequestInfo_impl
//

PInterceptor::RequestInfo_impl::RequestInfo_impl()
{
    ctx_list_orig_ = CORBA::ContextList::_nil();
    exception_ = NULL;
}

PInterceptor::RequestInfo_impl::~RequestInfo_impl()
{
    if (!CORBA::is_nil(object_))
	CORBA::release(object_);
    if (!CORBA::is_nil(fwd_object_))
	CORBA::release(fwd_object_);
    if (this->exception_ != NULL)
        delete exception_;
}

CORBA::ULong
PInterceptor::RequestInfo_impl::request_id()
{
    return req_id_;
}

char*
PInterceptor::RequestInfo_impl::operation()
{
    return CORBA::string_dup(op_name_.c_str());
}

Dynamic::ParameterList*
PInterceptor::RequestInfo_impl::arguments()
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::RECEIVE_EXCEPTION
	||icept_oper_ == PInterceptor::SEND_EXCEPTION
	||icept_oper_ == PInterceptor::RECEIVE_OTHER
	||icept_oper_ == PInterceptor::SEND_OTHER
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return new Dynamic::ParameterList(param_list_);
}

Dynamic::ExceptionList*
PInterceptor::RequestInfo_impl::exceptions()
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    if (exc_list_defined_) {
	return new Dynamic::ExceptionList(exc_list_);
    }
    else {
	mico_throw(CORBA::NO_RESOURCES(1, this->completion_status()));
    }
    // never reached, just to avoid warning
    assert(0);
    return NULL;
}

Dynamic::ContextList*
PInterceptor::RequestInfo_impl::contexts()
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return new Dynamic::ContextList(ctx_list_);
}

Dynamic::RequestContext*
PInterceptor::RequestInfo_impl::operation_context()
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS
	||icept_oper_ == PInterceptor::SEND_EXCEPTION
	||icept_oper_ == PInterceptor::SEND_OTHER)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return new Dynamic::RequestContext(req_ctx_);
}

CORBA::Any*
PInterceptor::RequestInfo_impl::result()
{
    if (icept_oper_ == PInterceptor::RECEIVE_REPLY
	||icept_oper_ == PInterceptor::SEND_REPLY)
	return new CORBA::Any(result_);
    mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    // never reached
    return NULL;
}

// extension
void
PInterceptor::RequestInfo_impl::result(const CORBA::Any& result)
{
    result_ = result;
}
// end

CORBA::Boolean
PInterceptor::RequestInfo_impl::response_expected()
{
    return response_;
}

Messaging::SyncScope
PInterceptor::RequestInfo_impl::sync_scope()
{
    if (icept_oper_ == PInterceptor::SEND_POLL)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return sync_scope_;
}

PortableInterceptor::ReplyStatus
PInterceptor::RequestInfo_impl::reply_status()
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::SEND_REQUEST
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return reply_status_;
}

CORBA::Object_ptr
PInterceptor::RequestInfo_impl::forward_reference()
{
    if (icept_oper_ != PInterceptor::RECEIVE_OTHER 
	&&icept_oper_ != PInterceptor::SEND_OTHER)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    if (reply_status_ != PortableInterceptor::LOCATION_FORWARD
	&&reply_status_ != PortableInterceptor::LOCATION_FORWARD_PERMANENT)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    return CORBA::Object::_duplicate(fwd_object_);
}

CORBA::Any*
PInterceptor::RequestInfo_impl::get_slot(PortableInterceptor::SlotId id)
{
    if (id >= slots_.length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
	return 0;
    }
    return new CORBA::Any(slots_[id]);
}

IOP::ServiceContext*
PInterceptor::RequestInfo_impl::get_request_service_context
(IOP::ServiceID id)
{
    if (icept_oper_ == PInterceptor::SEND_POLL)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));

    if (req_scl_ == NULL)
	mico_throw(CORBA::BAD_PARAM(123456, this->completion_status()));
    IOP::ServiceContext* retval = NULL;
    for (CORBA::ULong i=0; i<req_scl_->length(); i++) {
	if ((*req_scl_)[i].context_id == id)
	    retval = new IOP::ServiceContext((*req_scl_)[i]);
    }
    if (retval == NULL)
	mico_throw(CORBA::BAD_PARAM(23, this->completion_status()));
    return retval;
}

IOP::ServiceContext*
PInterceptor::RequestInfo_impl::get_reply_service_context(IOP::ServiceID id)
{
    if (icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::SEND_REQUEST
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    if (reply_scl_ == NULL)
	mico_throw(CORBA::BAD_PARAM());
    IOP::ServiceContext* retval = NULL;
    for (CORBA::ULong i=0; i<reply_scl_->length(); i++) {
	if ((*reply_scl_)[i].context_id == id)
	    retval = new IOP::ServiceContext((*reply_scl_)[i]);
    }
    if (retval == NULL)
	mico_throw(CORBA::BAD_PARAM(23, this->completion_status()));
    return retval;
}

// extensions

void
PInterceptor::RequestInfo_impl::arguments(const StaticAnyList& args, CORBA::Boolean valid_out)
{
    this->sanylist_to_parlist(param_list_, args, valid_out);
}

void
PInterceptor::RequestInfo_impl::arguments
(CORBA::NVList_ptr plst,
 CORBA::Boolean valid_out)
{
    this->nvlist_to_parlist(param_list_, plst, valid_out);
}

void
PInterceptor::RequestInfo_impl::request_id(CORBA::ULong id)
{
    req_id_ = id;
}

void
PInterceptor::RequestInfo_impl::response_expected(CORBA::Boolean resp)
{
    response_ = resp;
}

void
PInterceptor::RequestInfo_impl::sync_scope(Messaging::SyncScope ss)
{
    sync_scope_ = ss;
}

void
PInterceptor::RequestInfo_impl::reply_status
(PortableInterceptor::ReplyStatus reply_status)
{
    reply_status_ = reply_status;
}

void
PInterceptor::RequestInfo_impl::icept_oper(PInterceptor::PIState state)
{
    icept_oper_ = state;
}

void
PInterceptor::RequestInfo_impl::contexts(CORBA::ContextList_ptr clist)
{
    if (!CORBA::is_nil(clist)) {
	this->cctxl_to_dctxl(ctx_list_, clist);
	ctx_list_orig_ = CORBA::ContextList::_duplicate(clist);
    }
}

void
PInterceptor::RequestInfo_impl::operation_context(CORBA::Context_ptr ctx)
{
    if (!CORBA::is_nil(ctx)) {
	this->cctx_to_dctx(req_ctx_, ctx, object_);
    }
}

void
PInterceptor::RequestInfo_impl::slots(const CORBA::AnySeq& value)
{
    slots_ = value;
}

CORBA::AnySeq*
PInterceptor::RequestInfo_impl::slots()
{
    return new CORBA::AnySeq(slots_);
}

// protected (conversions)

void
PInterceptor::RequestInfo_impl::nvlist_to_parlist
(Dynamic::ParameterList& parlst,
 CORBA::NVList_ptr plst,
 CORBA::Boolean valid_out)
{
    CORBA::ULong length = plst->count();
    parlst.length(length);
    Dynamic::Parameter param;
    for (CORBA::ULong i=0; i<length; i++) {
	if ((!valid_out) && (plst->item(i)->flags() & CORBA::ARG_OUT)) {
	    // out param in send or receive request interception points
	    CORBA::TypeCode_var tc = new CORBA::TypeCode(CORBA::tk_null);
	    CORBA::Any any(tc, NULL);
	    param.argument = any;
	}
	else {
	    // other interception point than send request or receive request
	    // or other param than out
	    param.argument = *(plst->item(i)->value());
	}
	if (plst->item(i)->flags() & CORBA::ARG_IN)
	    param.mode = CORBA::PARAM_IN;
	else if (plst->item(i)->flags() & CORBA::ARG_INOUT)
	    param.mode = CORBA::PARAM_INOUT;
	else if (plst->item(i)->flags() & CORBA::ARG_OUT)
	    param.mode = CORBA::PARAM_OUT;
	else {
	    assert(0);
	}
	parlst[i] = param;
    }
}

void
PInterceptor::RequestInfo_impl::sanylist_to_parlist
(Dynamic::ParameterList& parlst,
 const StaticAnyList& args,
 CORBA::Boolean valid_out)
{
    CORBA::ULong length = args.size();
    parlst.length(length);
    Dynamic::Parameter param;
    for (CORBA::ULong i=0; i<length; i++) {
	if ((!valid_out) && (args[i]->flags() & CORBA::ARG_OUT)) {
	    // out param in send and receive request interception points
	    CORBA::TypeCode_var tc = new CORBA::TypeCode(CORBA::tk_null);
	    CORBA::Any any(tc, NULL);
	    param.argument = any;
	}
	else {
	    // other interception point than send request and receive request
	    // or other param than out
	    param.argument.from_static_any(*(args[i])); 
	}
	if (args[i]->flags() & CORBA::ARG_IN)
	    param.mode = CORBA::PARAM_IN;
	else if (args[i]->flags() & CORBA::ARG_INOUT)
	    param.mode = CORBA::PARAM_INOUT;
	else if (args[i]->flags() & CORBA::ARG_OUT)
	    param.mode = CORBA::PARAM_OUT;
	else {
	    assert(0);
	}
	parlst[i] = param;
    }
}

void
PInterceptor::RequestInfo_impl::cexlist_to_dexlist
(Dynamic::ExceptionList& dyn_exc_list,
 CORBA::ExceptionList_ptr exc_list)
{
    CORBA::ULong length = exc_list->count();
    dyn_exc_list.length(length);
    for (CORBA::ULong i=0; i<length; i++) {
	dyn_exc_list[i] = exc_list->item(i);
    }
}

void
PInterceptor::RequestInfo_impl::cctxl_to_dctxl
(Dynamic::ContextList& dyn_ctx_list,
 CORBA::ContextList_ptr ctx_list)
{
    CORBA::ULong length = ctx_list->count();
    dyn_ctx_list.length(length);
    for (CORBA::ULong i=0; i<length; i++) {
	dyn_ctx_list[i] = ctx_list->item(i);
    }
}

void
PInterceptor::RequestInfo_impl::cctx_to_dctx
(Dynamic::RequestContext& req_ctx,
 CORBA::Context_ptr ctx,
 CORBA::Object_ptr object)
{
    if (CORBA::is_nil(ctx))
	return;
    CORBA::NVList_var out;
    if (!CORBA::is_nil(ctx_list_orig_)) {
	ctx->get_values ("", 0, ctx_list_orig_, out);
    }
    else {
	ctx->get_values ("", 0, "*", out);
    }
    CORBA::ULong length = 2 * out->count();
    req_ctx.length(length);
    CORBA::ULong i, j;
    for (i=0, j=0; i<out->count(); i++, j += 2) {
	req_ctx[j] = out->item(i)->name();
	const char* s;
	CORBA::Boolean r = (*out->item(i)->value() >>= s);
	if (r)
	    req_ctx[j+1] = s;
    }
}								

CORBA::IORProfile *
PInterceptor::RequestInfo_impl::get_profile()
{
    MICO::IIOPProfile* iioprof = NULL;
#ifdef HAVE_SSL
    MICOSSL::SSLProfile* sslprof = NULL;
#endif
    CORBA::Policy_var policy;
    MICOPolicy::TransportPrefPolicy_var tpp;
    MICOPolicy::TransportPrefPolicy::ProfileTagSeq* prefs;
    CORBA::IORProfile* prof = object_->_ior_fwd()->active_profile();
    if (prof != NULL && prof->id() == CORBA::IORProfile::TAG_INTERNET_IOP) {
	iioprof = dynamic_cast<MICO::IIOPProfile*>(prof);
	assert(iioprof != NULL);
	return iioprof;
    }
#ifdef HAVE_SSL
    else if (prof != NULL
	     && prof->id() == CORBA::IORProfile::TAG_SSL_INTERNET_IOP) {
	sslprof = dynamic_cast<MICOSSL::SSLProfile*>(prof);
	assert(sslprof != NULL);
	return sslprof;
    }	
#endif
    else {
	policy = object_->_get_policy
	    (MICOPolicy::TRANSPORTPREF_POLICY_TYPE);
	tpp = MICOPolicy::TransportPrefPolicy::_narrow (policy);
	assert(!CORBA::is_nil(tpp));
	prefs = tpp->preferences_nocopy();
	for (CORBA::ULong i=0; i<prefs->length(); i++) {
	    prof = NULL;
	    prof = object_->_ior_fwd()->profile((*prefs)[i]);
	    if (prof != NULL) {
		if (prof->id() == CORBA::IORProfile::TAG_INTERNET_IOP) {
		    iioprof = dynamic_cast<MICO::IIOPProfile*>(prof);
		    assert(iioprof != NULL);
		    return iioprof;
		}
#ifdef HAVE_SSL
		if (prof->id() == CORBA::IORProfile::TAG_SSL_INTERNET_IOP) {
		    sslprof = dynamic_cast<MICOSSL::SSLProfile*>(prof);
		    assert(sslprof != NULL);
		    return sslprof;
		}
#endif
	    }
	}
    }
    return prof;
}

CORBA::IORProfile*
PInterceptor::RequestInfo_impl::get_components_profile()
{
    int ix=0;
    CORBA::IORProfile* tp;
    while ((tp = object_->_ior()->get_profile(ix)) != NULL) {
	if (tp->id() == CORBA::IORProfile::TAG_MULTIPLE_COMPONENTS)
	    return tp;
	ix++;
    }
    return NULL;
}

CORBA::DataEncoder *
PInterceptor::RequestInfo_impl::get_enc(CORBA::IORProfile*& prof)
{
    MICO::IIOPProfile* iioprof = NULL;
    prof = this->get_profile();
    if (prof == NULL)
	return 0;
    CORBA::UShort version = 0x0100;
    if (prof->id() == CORBA::IORProfile::TAG_INTERNET_IOP) {
	iioprof = dynamic_cast<MICO::IIOPProfile*>(prof);
	assert(iioprof != NULL);
	version = iioprof->iiop_version();
    }
    CORBA::CodeSetCoder* conv = NULL;
    CORBA::Codeset::CodesetId csid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultCS)->id();
    CORBA::Codeset::CodesetId wcsid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultWCS)->id();

    if (version == 0x0100)
	conv = new MICO::GIOP_1_0_CodeSetCoder ();
    else if (version == 0x0101)
	conv = new MICO::GIOP_1_1_CodeSetCoder (csid);
    else if (version >= 0x0102)
	conv = new MICO::GIOP_1_2_CodeSetCoder (csid, wcsid);
    return new MICO::CDREncoder(new CORBA::Buffer, TRUE,
				CORBA::DefaultEndian, conv);
}


CORBA::CompletionStatus
PInterceptor::RequestInfo_impl::completion_status()
{
    if (icept_oper_ == PInterceptor::SEND_REQUEST
	||icept_oper_ == PInterceptor::SEND_POLL
	||icept_oper_ == PInterceptor::SEND_OTHER
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS
	||icept_oper_ == PInterceptor::RECEIVE_REQUEST
	||icept_oper_ == PInterceptor::RECEIVE_OTHER)
	return CORBA::COMPLETED_NO;
    else if (icept_oper_ == PInterceptor::SEND_REPLY
	     ||icept_oper_ == PInterceptor::RECEIVE_REPLY)
	return CORBA::COMPLETED_YES;
    else if (icept_oper_ == PInterceptor::SEND_EXCEPTION
	     ||icept_oper_ == PInterceptor::RECEIVE_EXCEPTION) {
	if (reply_status_ == PortableInterceptor::USER_EXCEPTION)
	    return CORBA::COMPLETED_YES;
	else if (reply_status_ == PortableInterceptor::SYSTEM_EXCEPTION) {
	    CORBA::SystemException* sys = CORBA::SystemException::_downcast
		(this->exception());
	    assert(sys != NULL);
	    return sys->completed();
	}
	else {
	    // neither SYSTEM nor USER exception
	    assert(0);
	}
    }
    assert(0);
    // never reached, just to avoid warning
    return CORBA::COMPLETED_NO;
}

void
PInterceptor::RequestInfo_impl::exception(CORBA::Exception* exc)
{
    if (exception_ != NULL) {
	delete exception_;
	exception_ = NULL;
    }
    exception_ = exc;
}

CORBA::Exception*
PInterceptor::RequestInfo_impl::exception()
{
    return exception_;
}

//
// ClientRequestInfo_impl
//

PInterceptor::ClientRequestInfo_impl::~ClientRequestInfo_impl()
{
    if (reply_scl_ != NULL)
	delete reply_scl_;
}

PInterceptor::ClientRequestInfo_impl::ClientRequestInfo_impl()
{
}

PInterceptor::ClientRequestInfo_impl::ClientRequestInfo_impl
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ULong req_id,
 CORBA::NVList_ptr param_list,
 CORBA::ExceptionList_ptr exc_list,
 CORBA::ContextList_ptr ctx_list,
 CORBA::Context_ptr ctx,
 CORBA::NamedValue_ptr result,
 CORBA::Boolean response)
{
    object_ = CORBA::Object::_duplicate(object);
    op_name_ = op_name;
    if (!CORBA::is_nil(param_list))
	this->nvlist_to_parlist(param_list_, param_list, FALSE);
    if (!CORBA::is_nil(exc_list)) {
	this->cexlist_to_dexlist(exc_list_, exc_list);
	exc_list_defined_ = TRUE;
    }
    else {
	exc_list_defined_ = FALSE;
    }
    if (!CORBA::is_nil(ctx_list)) {
	ctx_list_orig_ = CORBA::ContextList::_duplicate(ctx_list);
	this->cctxl_to_dctxl(ctx_list_, ctx_list);
    }
    if (!CORBA::is_nil(ctx))
	this->cctx_to_dctx(req_ctx_, ctx, object);
    if (!CORBA::is_nil(result))
	result_ = *(result->value());
    req_id_ = req_id;
    response_ = response;
    fwd_object_ = CORBA::Object::_nil();
    req_scl_ = NULL;
    reply_scl_ = NULL;
}

PInterceptor::ClientRequestInfo_impl::ClientRequestInfo_impl
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ExceptionList_ptr exc_list,
 CORBA::NamedValue_ptr result)
{
    object_ = CORBA::Object::_duplicate(object);
    op_name_ = op_name;
    if (!CORBA::is_nil(exc_list)) {
	this->cexlist_to_dexlist(exc_list_, exc_list);
	exc_list_defined_ = TRUE;
    }
    else {
	exc_list_defined_ = FALSE;
    }
    if (!CORBA::is_nil(result))
	result_ = *(result->value());
    fwd_object_ = CORBA::Object::_nil();
    req_scl_ = NULL;
    reply_scl_ = NULL;
}

// extension
void
PInterceptor::ClientRequestInfo_impl::set_request_service_context_list
(IOP::ServiceContextList* scl)
{
    req_scl_ = scl;
}

void
PInterceptor::ClientRequestInfo_impl::set_reply_service_context_list
(IOP::ServiceContextList* scl)
{
    reply_scl_ = new IOP::ServiceContextList(*scl);
}
// end extension

CORBA::Object_ptr
PInterceptor::ClientRequestInfo_impl::target()
{
    return CORBA::Object::_duplicate(object_);
}

CORBA::Object_ptr
PInterceptor::ClientRequestInfo_impl::effective_target()
{
    return CORBA::Object::_duplicate(object_);
}

void
PInterceptor::ClientRequestInfo_impl::effective_target(CORBA::Object_ptr obj)
{
    if (!CORBA::is_nil(object_))
	CORBA::release(object_);
    if (!CORBA::is_nil(fwd_object_))
	CORBA::release(fwd_object_);
    object_ = CORBA::Object::_duplicate(obj);
    fwd_object_ = CORBA::Object::_duplicate(obj);
}

// useless
IOP::TaggedProfile*
PInterceptor::ClientRequestInfo_impl::effective_profile()
{
    //MICO::IIOPProfile* iioprof = NULL;
    CORBA::IORProfile* prof = NULL;
    CORBA::DataEncoder* ec = this->get_enc(prof);
    if (prof == NULL)
	return 0;
    prof->encode(*ec);
    CORBA::Buffer* buf = ec->buffer();
    buf->rseek_beg(0);
    CORBA::ULong length = buf->length();
    IOP::TaggedProfile* retval = new IOP::TaggedProfile;
    retval->profile_data.length(length);
    retval->tag = prof->id();
    CORBA::Octet* t_buf = buf->buffer();
    for (CORBA::ULong i=0; i<length; i++)
	retval->profile_data[i] = t_buf[i];
    delete ec;
    return retval;
}

CORBA::Any*
PInterceptor::ClientRequestInfo_impl::received_exception()
{
    if (icept_oper_ != PInterceptor::RECEIVE_EXCEPTION) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if (this->exception() == NULL)
	return 0;
    CORBA::SystemException* sys_exc = CORBA::SystemException::_downcast
	(this->exception());
    if (sys_exc != NULL) {
	CORBA::Any* any = new CORBA::Any;
	(*any) <<= (*this->exception());
	return any;
    }

    if (exc_list_.length() > 0) {
	CORBA::UnknownUserException* uu_exc
	    = CORBA::UnknownUserException::_downcast(this->exception());
	if (uu_exc != NULL) {
	    const char* repoid = uu_exc->_except_repoid();
	    CORBA::TypeCode_ptr tc;
	    for (CORBA::ULong i=0; i<exc_list_.length(); i++) {
		tc = exc_list_[i];
		if (!strcmp (repoid, tc->id())) {
		    return new CORBA::Any(uu_exc->exception(tc));
		}
	    }
	}
    }
    CORBA::UNKNOWN sx(0, this->completion_status());
    CORBA::Any* any = new CORBA::Any;
    *any <<= sx;
    return any;
}

char*
PInterceptor::ClientRequestInfo_impl::received_exception_id()
{
    if (icept_oper_ != PInterceptor::RECEIVE_EXCEPTION) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    CORBA::UnknownUserException* uu_exc 
	= CORBA::UnknownUserException::_downcast(this->exception());
    if (uu_exc != NULL)
	return CORBA::string_dup(uu_exc->_except_repoid());
    return CORBA::string_dup(this->exception()->_repoid());
}

IOP::TaggedComponent*
PInterceptor::ClientRequestInfo_impl::get_effective_component
(IOP::ComponentId id)
{
    if (icept_oper_ == PInterceptor::SEND_POLL)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    CORBA::IORProfile* prof = NULL;
    CORBA::DataEncoder* ec = this->get_enc(prof);
    if (prof == NULL)
	return 0;
    CORBA::Component* ct = NULL;
    CORBA::MultiComponent* comps = prof->components();
    ct = comps->component(id);
    if (ct == NULL) {
	// there isn't such component in active profile
	// => we'll try to find multicomponent
	// profile
	if (MICO::Logger::IsLogged(MICO::Logger::PI))
	    MICO::Logger::Stream(MICO::Logger::PI)
		<< "component(id:" << id << ") is not in active profile."
		<< endl;
	prof = NULL;
	prof = this->get_components_profile();
	if (prof == NULL)
	    mico_throw(CORBA::BAD_PARAM(25, this->completion_status()));
	if (MICO::Logger::IsLogged(MICO::Logger::PI))
	    MICO::Logger::Stream(MICO::Logger::PI)
		<< "Found MultiComponents profile" << endl;
	comps = prof->components();
	ct = comps->component(id);
	if (ct == NULL)
	    mico_throw(CORBA::BAD_PARAM(25, this->completion_status()));
	if (MICO::Logger::IsLogged(MICO::Logger::PI))
	    MICO::Logger::Stream(MICO::Logger::PI)
		<< "Found component(id:" << id << ") in MC profile" << endl;
    }
    else {
	if (MICO::Logger::IsLogged(MICO::Logger::PI))
	    MICO::Logger::Stream(MICO::Logger::PI)
		<< "Found component(id:" << id << ") in active profile"
		<< endl;
    }
    ct->encode(*ec);
    CORBA::Buffer* buf = ec->buffer();
    buf->rseek_beg(0);
    CORBA::ULong length = buf->length();
    IOP::TaggedComponent* retval = new IOP::TaggedComponent;
    retval->component_data.length(length);
    retval->tag = ct->id();
    CORBA::Octet* t_buf = buf->buffer();
    for (CORBA::ULong i=0; i<length; i++)
	retval->component_data[i] = t_buf[i];
    delete ec;
    return retval;
}

IOP::TaggedComponentSeq*
PInterceptor::ClientRequestInfo_impl::get_effective_components
(IOP::ComponentId id)
{
    if (icept_oper_ == PInterceptor::SEND_POLL)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    CORBA::IORProfile* prof = NULL;
    CORBA::DataEncoder* ec = this->get_enc(prof);
    if (prof == NULL)
	return 0;
    IOP::TaggedComponentSeq* retval = new IOP::TaggedComponentSeq;
    retval->length(0);
    CORBA::ULong length = 0;
    CORBA::ULong l;
    CORBA::Component* ct = NULL;
    IOP::TaggedComponent tc;
    CORBA::Buffer* buf;
    CORBA::MultiComponent* comps = prof->components();
//      for (mico_vec_size_type i = 0; i < comps->size(); i++) {
    for (CORBA::ULong i = 0; i<comps->size(); i++) {
	if (comps->component(i)->id() == id) {
	    ec->buffer()->wseek_beg(0);
	    ec->buffer()->rseek_beg(0);
	    ct = comps->component(i);
	    ct->encode(*ec);
	    tc.tag = ct->id();
	    buf = ec->buffer();
	    l = buf->length();
	    tc.component_data.length(l);
	    CORBA::Octet* t_buf = buf->buffer();
	    for (CORBA::ULong j=0; j<l; j++)
		tc.component_data[j] = t_buf[j];
	    retval->length(length + 1);
	    (*retval)[length] = tc;
	    length++;
	}
    }
    delete ec;
    if (retval->length() == 0) {
	delete retval;
	mico_throw(CORBA::BAD_PARAM(25, this->completion_status()));
	return 0;
    }
    return retval;
}

CORBA::Policy_ptr
PInterceptor::ClientRequestInfo_impl::get_request_policy
(CORBA::PolicyType type)
{
    CORBA::Policy_ptr retval = CORBA::Policy::_nil();
    if (icept_oper_ == PInterceptor::SEND_POLL)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    try {
	retval = object_->_get_policy(type);
    } catch (...) {
	mico_throw(CORBA::INV_POLICY(1, this->completion_status()));
    }
    return retval;
}

void
PInterceptor::ClientRequestInfo_impl::add_request_service_context
(const IOP::ServiceContext& service_context,
 CORBA::Boolean replace)
{
    if (icept_oper_ != PInterceptor::SEND_REQUEST)
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    if (req_scl_ == NULL) {
	mico_throw(CORBA::BAD_INV_ORDER(123456, this->completion_status()));
    }
    CORBA::ULong length = req_scl_->length();
    for (CORBA::ULong i=0; i<length; i++) {
	if ((*req_scl_)[i].context_id == service_context.context_id) {
	    if (replace)
		(*req_scl_)[i] = service_context;
	    else
		mico_throw(CORBA::BAD_INV_ORDER(11, this->completion_status()));
	    return;
	}
    }
    req_scl_->length(length+1);
    (*req_scl_)[length] = service_context;
}

//
// ServerRequestInfo_impl
//

PInterceptor::ServerRequestInfo_impl::ServerRequestInfo_impl()
{
}

PInterceptor::ServerRequestInfo_impl::ServerRequestInfo_impl
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ULong req_id,
 CORBA::Boolean response,
 IOP::ServiceContextList* scl,
 CORBA::NVList_ptr param_list,
 CORBA::ExceptionList_ptr exc_list,
 CORBA::ContextList_ptr ctx_list,
 CORBA::Context_ptr ctx,
 CORBA::NamedValue_ptr result)
{
    object_ = CORBA::Object::_duplicate(object);
    op_name_ = op_name;
    if (!CORBA::is_nil(param_list))
	this->nvlist_to_parlist(param_list_, param_list, FALSE);
    if (!CORBA::is_nil(exc_list)) {
	this->cexlist_to_dexlist(exc_list_, exc_list);
	exc_list_defined_ = TRUE;
    }
    else {
	exc_list_defined_ = FALSE;
    }
    if (!CORBA::is_nil(ctx_list))
	this->cctxl_to_dctxl(ctx_list_, ctx_list);
    if (!CORBA::is_nil(ctx))
	this->cctx_to_dctx(req_ctx_, ctx, object);
    if (!CORBA::is_nil(result))
	result_ = *(result->value());
    req_id_ = req_id;
    response_ = response;
    fwd_object_ = CORBA::Object::_nil();
    //req_scl_ = NULL;
    reply_scl_ = NULL;
    object_adapter_ = NULL;
    sync_scope_ = Messaging::SYNC_NONE;
    req_scl_ = new IOP::ServiceContextList(*scl);
    own_exception_ = FALSE;
    object_id_.length(0);
}


PInterceptor::ServerRequestInfo_impl::~ServerRequestInfo_impl()
{
    if (req_scl_) 
	delete req_scl_;
}

// extensions
void
PInterceptor::ServerRequestInfo_impl::set_reply_service_context_list
(IOP::ServiceContextList * scl)
{
    reply_scl_ = scl;
}

void
PInterceptor::ServerRequestInfo_impl::effective_target(CORBA::Object_ptr obj)
{
    if (!CORBA::is_nil(object_))
	CORBA::release(object_);
    if (!CORBA::is_nil(fwd_object_))
	CORBA::release(fwd_object_);
    object_ = CORBA::Object::_duplicate(obj);
    fwd_object_ = CORBA::Object::_duplicate(obj);
}

void
PInterceptor::ServerRequestInfo_impl::set_oa(CORBA::ObjectAdapter* oa)
{
    object_adapter_ = oa;
    MICOPOA::POA_impl* poa = dynamic_cast<MICOPOA::POA_impl*>(oa);
    if (poa != NULL) {
	try {
	    PortableServer::ObjectId_var oid = poa->reference_to_id(object_);
	    object_id_ = oid;
	} catch (PortableServer::POA::WrongAdapter_catch& ex) {
	} catch (PortableServer::POA::WrongPolicy_catch& ex) {
	}
    }
}

CORBA::Any*
PInterceptor::ServerRequestInfo_impl::sending_exception()
{
    if (icept_oper_ != PInterceptor::SEND_EXCEPTION) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if (this->exception() == NULL)
	return NULL;
    CORBA::SystemException* sys_exc = CORBA::SystemException::_downcast
	(this->exception());
    if (sys_exc != NULL) {
	CORBA::Any* any = new CORBA::Any;
	(*any) <<= (*this->exception());
	return any;
    }
    CORBA::UserException* usr_exc = CORBA::UserException::_downcast
	(this->exception());
    if (usr_exc != NULL) {
	CORBA::Any* any = new CORBA::Any;
	(*any) <<= (*this->exception());
	return any;
    }
    CORBA::UNKNOWN sx(0, this->completion_status());
    CORBA::Any* any = new CORBA::Any;
    (*any) <<= (sx);
    return any;
}

CORBA::OctetSeq*
PInterceptor::ServerRequestInfo_impl::object_id()
{
    if (icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if ((icept_oper_ == PInterceptor::SEND_EXCEPTION
	 ||icept_oper_ == PInterceptor::SEND_OTHER)
	&& (reply_status_ == PortableInterceptor::LOCATION_FORWARD
	    ||reply_status_ == PortableInterceptor::
	    LOCATION_FORWARD_PERMANENT)) {
	mico_throw(CORBA::NO_RESOURCES(1, this->completion_status()));
    }
    CORBA::OctetSeq* retval = new CORBA::OctetSeq(object_id_);
    return retval;
}

CORBA::OctetSeq*
PInterceptor::ServerRequestInfo_impl::adapter_id()
{
    if (icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if ((icept_oper_ == PInterceptor::SEND_EXCEPTION
	 ||icept_oper_ == PInterceptor::SEND_OTHER)
	&&(reply_status_ == PortableInterceptor::LOCATION_FORWARD
	   ||reply_status_ == PortableInterceptor::
	   LOCATION_FORWARD_PERMANENT)) {
	mico_throw(CORBA::NO_RESOURCES());
    }
    CORBA::OctetSeq* retval = new CORBA::OctetSeq;
    if (object_adapter_ == NULL)
	return retval;
    const char* id = object_adapter_->get_oaid();
    if (id == NULL)
	return retval;		
    int length = strlen(id);
    retval->length(length);
    for (int i=0; i<length; i++)
	(*retval)[i] = id[i];
    return retval;
}

char*
PInterceptor::ServerRequestInfo_impl::target_most_derived_interface()
{
    if (icept_oper_ != PInterceptor::RECEIVE_REQUEST) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if (object_adapter_ == NULL)
	return CORBA::string_dup("");
    const char* oaid = object_adapter_->get_oaid();
    if (oaid == NULL)
	return CORBA::string_dup("");
    char* ret = CORBA::string_dup("");
    MICOPOA::POA_impl* poa = dynamic_cast<MICOPOA::POA_impl*>
	(object_adapter_);
    assert(poa != NULL);
    PortableServer::Servant serv = NULL;
    CORBA::Boolean sc = TRUE;
    PortableServer::ObjectId_var oid = NULL;
    try {
	oid = poa->reference_to_id(object_);	
	serv = poa->id_to_servant(oid);
    } catch (...) {
	try {
	    serv = poa->get_servant();
	} catch (...) {
            // handle servant locator case
            CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
            assert(!CORBA::is_nil(orb));
            CORBA::Object_var obj = orb->resolve_initial_references("POACurrent");
            PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
            assert(!CORBA::is_nil(current));
            try {
                serv = current->get_servant();
            }
            catch (...) {
                sc = FALSE;
            }
	}
    }
    if (!sc) {
	// do something else
	return ret;
    }
    delete ret;
    ret = serv->_primary_interface(oid, poa);
//      if (oid)
//  	delete oid;
    return ret;	
}

CORBA::Policy_ptr
PInterceptor::ServerRequestInfo_impl::get_server_policy
(CORBA::PolicyType type)
{
    if (PI::S_pfmap_.find(type) == PI::S_pfmap_.end())
	mico_throw(CORBA::INV_POLICY(2, this->completion_status()));
    if (object_adapter_ == NULL)
	return CORBA::Policy::_nil();
    const char* oaid = object_adapter_->get_oaid();
    if (oaid == NULL || !strcmp(oaid, "mico-local-boa"))
	return CORBA::Policy::_nil();
    PortableServer::POA* poa = dynamic_cast<PortableServer::POA*>
	(object_adapter_);
    assert(poa != NULL);
    return poa->_get_policy(type);
}

void
PInterceptor::ServerRequestInfo_impl::set_slot
(PortableInterceptor::SlotId id,
 const CORBA::Any& data)
{
    if (id >= slots_.length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
	return;
    }
    slots_[id] = data;
}

CORBA::Boolean
PInterceptor::ServerRequestInfo_impl::target_is_a(const char* id)
{
    if (icept_oper_ == PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS) {
	mico_throw(CORBA::BAD_INV_ORDER(10, this->completion_status()));
    }
    if ((icept_oper_ == PInterceptor::SEND_EXCEPTION
	 ||icept_oper_ == PInterceptor::SEND_OTHER)
	&&(reply_status_ == PortableInterceptor::LOCATION_FORWARD
	   ||reply_status_ == PortableInterceptor::
	   LOCATION_FORWARD_PERMANENT)) {
	mico_throw(CORBA::NO_RESOURCES());
    }
    if (CORBA::is_nil(object_))
	return FALSE;
    return object_->_is_a(id);
}

void
PInterceptor::ServerRequestInfo_impl::add_reply_service_context
(const IOP::ServiceContext& service_context,
 CORBA::Boolean replace)
{
    if (reply_scl_ == NULL) {
	mico_throw(CORBA::BAD_INV_ORDER());
    }
    CORBA::ULong length = reply_scl_->length();
    for (CORBA::ULong i=0; i<length; i++) {
	if ((*reply_scl_)[i].context_id == service_context.context_id) {
	    if (replace)
		(*reply_scl_)[i] = service_context;
	    else
		mico_throw(CORBA::BAD_INV_ORDER(11, this->completion_status()));
	    return;
	}
    }
    reply_scl_->length(length+1);
    (*reply_scl_)[length] = service_context;
}

//
// overloaded from RequestInfo to throw NO_RESOURCES exception
//
Dynamic::ContextList*
PInterceptor::ServerRequestInfo_impl::contexts()
{
    mico_throw(CORBA::NO_RESOURCES(1, this->completion_status()));
    // never reached, just to avoid warning
    return NULL;
}


//
// IORInfo_impl
//

PInterceptor::IORInfo_impl::IORInfo_impl()
    : poa_(PortableServer::POA::_nil()), ior_(NULL), conv_(NULL)
{
}

PInterceptor::IORInfo_impl::IORInfo_impl
(PortableServer::POA_ptr poa,
 CORBA::IOR* ior)
{
    poa_ = PortableServer::POA::_duplicate(poa);
    ior_ = ior;
    CORBA::UShort version = 0x0100;
    if (ior_ != NULL) {
	CORBA::IORProfile* iorprof = ior_->profile
	    (CORBA::IORProfile::TAG_INTERNET_IOP);
	if (iorprof != NULL) {
	    MICO::IIOPProfile* iioprof = dynamic_cast<MICO::IIOPProfile*>
		(iorprof);
	    assert(iioprof != NULL);
	    version = iioprof->iiop_version();
	}
    }
    CORBA::Codeset::CodesetId csid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultCS)->id();
    CORBA::Codeset::CodesetId wcsid = CORBA::Codeset::special_cs
	(CORBA::Codeset::DefaultWCS)->id();
    if (version == 0x0100)
	conv_ = new MICO::GIOP_1_0_CodeSetCoder();
    else if (version == 0x0101)
	conv_ = new MICO::GIOP_1_1_CodeSetCoder(csid);
    else if (version >= 0x0102)
	conv_ = new MICO::GIOP_1_2_CodeSetCoder (csid, wcsid);
    else
	conv_ = NULL;
}

PInterceptor::IORInfo_impl::~IORInfo_impl()
{
    if (!CORBA::is_nil(poa_))
	CORBA::release(poa_);
    if (conv_ != NULL)
	delete conv_;
}

CORBA::Policy_ptr
PInterceptor::IORInfo_impl::get_effective_policy(CORBA::PolicyType type)
{
    CORBA::Policy_ptr retval = CORBA::Policy::_nil();
    try {
	retval = poa_->_get_policy(type);
    } catch (...) {
	mico_throw(CORBA::INV_POLICY());
    }
    return retval;
}

void
PInterceptor::IORInfo_impl::add_ior_component
(const IOP::TaggedComponent& comp)
{
    CORBA::IORProfile* iorprof;
    CORBA::ULong pid;
    for (CORBA::ULong i=0; !!(iorprof = ior_->get_profile(i)); i++) {
	pid = iorprof->id();
	try {
	    this->add_ior_component_to_profile(comp, pid);
	} catch (...) {
	}
    }
}

void
PInterceptor::IORInfo_impl::add_ior_component_to_profile
(const IOP::TaggedComponent& comp,
 IOP::ProfileId profile_id)
{
    CORBA::IORProfile* iorprof = ior_->profile(profile_id, TRUE);
    if (iorprof == NULL)
	mico_throw(CORBA::BAD_PARAM());

    CORBA::MultiComponent* mc = iorprof->components();
    if (mc == NULL)
	mico_throw(CORBA::BAD_PARAM());

    CORBA::ULong length = comp.component_data.length();
    CORBA::Buffer* buf = new CORBA::Buffer;
    for (CORBA::ULong i=0; i<length; i++)
	buf->put(comp.component_data[i]);

    assert(conv_ != NULL);
    MICO::CDRDecoder* dc = new MICO::CDRDecoder
	(buf, TRUE, CORBA::DefaultEndian, conv_, FALSE);
    CORBA::Component* cmp = CORBA::Component::decode_body
	(*dc, comp.tag, length);
    if (MICO::Logger::IsLogged(MICO::Logger::PI)) {
	MICO::Logger::Stream(MICO::Logger::PI)
	    << "added component into profile(" << profile_id << "):" << endl;
	cmp->print(MICO::Logger::Stream(MICO::Logger::PI));
    }
    if (cmp == NULL) {
	delete dc;
	mico_throw(CORBA::BAD_PARAM());
    }
    mc->add_component(cmp);
    delete dc;
}


//
// ORB Init Info
// During initialization ORB_init goes through the list of ORBInitializers
// created by application, instantiates ORBInitInfo and calls pre_init() and
// post_init() methods of every ORBInitializer.
//

PInterceptor::ORBInitInfo_impl::ORBInitInfo_impl
(CORBA::ORB_ptr orb,
 const char* id,
 int& argc,
 char** argv)
{
    orb_ = CORBA::ORB::_duplicate(orb);
    orb_id_ = CORBA::string_dup(id);
    CORBA::ULong length = 0;
    args_.length(length);
    for (int i=1; i<argc; i++) {
	args_.length(length+1);
	args_[length] = (const char *)argv[i];
	length++;
    }
    // _args requires further processing via MICOGetOpt!	
}

PInterceptor::ORBInitInfo_impl::~ORBInitInfo_impl()
{
}

CORBA::StringSeq*
PInterceptor::ORBInitInfo_impl::arguments()
{
    return new CORBA::StringSeq(args_);
}

char*
PInterceptor::ORBInitInfo_impl::orb_id()
{
    return CORBA::string_dup(orb_id_.c_str());
}

IOP::CodecFactory_ptr
PInterceptor::ORBInitInfo_impl::codec_factory()
{
    return new PICodec::CodecFactory_impl;
}

void
PInterceptor::ORBInitInfo_impl::register_initial_reference
(const char* id,
 CORBA::Object_ptr obj)
{
    orb_->register_initial_reference(id, obj);
}

CORBA::Object_ptr
PInterceptor::ORBInitInfo_impl::resolve_initial_references(const char* id)
{
    if (!CORBA::is_nil(orb_))
	return orb_->resolve_initial_references(id);
    mico_throw(PortableInterceptor::ORBInitInfo::InvalidName());
    return CORBA::Object::_nil();
}

void
PInterceptor::ORBInitInfo_impl::add_client_request_interceptor
(PortableInterceptor::ClientRequestInterceptor_ptr interceptor)
{
    PI::_exec_add_client_interceptor(interceptor);
}

void
PInterceptor::ORBInitInfo_impl::add_server_request_interceptor
(PortableInterceptor::ServerRequestInterceptor_ptr interceptor)
{
    PI::_exec_add_server_interceptor(interceptor);
}

void
PInterceptor::ORBInitInfo_impl::add_ior_interceptor
(PortableInterceptor::IORInterceptor_ptr interceptor)
{
    PI::_exec_add_ior_interceptor(interceptor);
}

PortableInterceptor::SlotId
PInterceptor::ORBInitInfo_impl::allocate_slot_id()
{
    return PInterceptor::PI::S_max_slot_id_++;
}

void
PInterceptor::ORBInitInfo_impl::register_policy_factory
(CORBA::PolicyType type,
 PortableInterceptor::PolicyFactory_ptr factory)
{
    if (PI::S_pfmap_.find(type) != PI::S_pfmap_.end())
	mico_throw(CORBA::BAD_INV_ORDER());
    PI::S_pfmap_[type] = PolicyFactory::_duplicate(factory);
}


//
// PICurrent
//

#ifndef HAVE_THREADS
//
// single-threaded PICurrent
//
PInterceptor::Current_impl::~Current_impl()
{
}

PInterceptor::Current_impl::Current_impl()
{
    slots_ = NULL;
    S_current_ = this;
}
	
CORBA::Any*
PInterceptor::Current_impl::get_slot(PortableInterceptor::SlotId id)
{
    const CORBA::AnySeq* slots = this->slots();
    if (id >= slots->length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
    }
    return new CORBA::Any((*slots)[id]);
}

void
PInterceptor::Current_impl::set_slot
(PortableInterceptor::SlotId id,
 const CORBA::Any& data)
{
    CORBA::AnySeq* slots = const_cast<CORBA::AnySeq*>(this->slots());
    if (id >= slots->length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
    }
    (*slots)[id] = data;
}

// extension

const CORBA::AnySeq*
PInterceptor::Current_impl::slots()
{
    if (slots_ == NULL) {
	// lazy init
	slots_ = new CORBA::AnySeq;
	slots_->length(PInterceptor::PI::S_max_slot_id_);
    }
    return slots_;
}

void
PInterceptor::Current_impl::slots(const CORBA::AnySeq& value)
{
    if (slots_ != NULL)
	delete slots_;
    slots_ = new CORBA::AnySeq(value);
}

PInterceptor::Current_impl*
PInterceptor::Current_impl::current()
{
    return S_current_;
}

#else // HAVE_THREADS
//
// multi-threaded PICurrent
//
PInterceptor::Current_impl::~Current_impl()
{
}

PInterceptor::Current_impl::Current_impl()
{
    MICOMT::Thread::create_key(slots_key_, cleanup_slots);
    S_current_ = this;
}
	
CORBA::Any*
PInterceptor::Current_impl::get_slot(PortableInterceptor::SlotId id)
{
    const CORBA::AnySeq* slots = this->slots();
    if (id >= slots->length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
    }
    return new CORBA::Any((*slots)[id]);
}

void
PInterceptor::Current_impl::set_slot
(PortableInterceptor::SlotId id,
 const CORBA::Any& data)
{
    CORBA::AnySeq* slots = const_cast<CORBA::AnySeq*>(this->slots());
    if (id >= slots->length()) {
	mico_throw(PortableInterceptor::InvalidSlot());
    }
    (*slots)[id] = data;
}

// extension

const CORBA::AnySeq*
PInterceptor::Current_impl::slots()
{
    CORBA::AnySeq* slots = static_cast<CORBA::AnySeq*>
	(MICOMT::Thread::get_specific(slots_key_));
    if (slots == NULL) {
	// lazy init
	slots = new CORBA::AnySeq;
	slots->length(PInterceptor::PI::S_max_slot_id_);
	MICOMT::Thread::set_specific(slots_key_, slots);
    }
    return slots;
}

void
PInterceptor::Current_impl::slots(const CORBA::AnySeq& value)
{
    CORBA::AnySeq* slots = new CORBA::AnySeq(value);
    CORBA::AnySeq* old_slots = static_cast<CORBA::AnySeq*>
	(MICOMT::Thread::get_specific(slots_key_));
    if (old_slots != NULL)
	delete old_slots;
    MICOMT::Thread::set_specific(slots_key_, slots);
}

PInterceptor::Current_impl*
PInterceptor::Current_impl::current()
{
    return S_current_;
}

#endif // HAVE_THREADS

//
// PInterceptor::PI
//

void
PInterceptor::PI::_init()
{
}

void
PInterceptor::PI::_exec_client
(PortableInterceptor::ClientRequestInfo_ptr ri,
 PortableInterceptor::ClientMethod m,
 InterceptionPoint ip)
{
    MICOMT::AutoLock lock(PInterceptor::PI::S_client_lock_);
    if (ip == START) {
	ClientFlowStack* fl = new ClientFlowStack(S_client_req_int_);
	S_client_stack_.push(fl);
	while (!fl->start_ip()->empty()) {
	    ClientRequestInterceptor_ptr cri = fl->start_ip()->top();
	    fl->start_ip()->pop();
	    ((cri)->*m)(ri);
	    fl->end_ip()->push(cri);
	}
    } else if (ip == INTERMEDIATE) {
	// incorect value for client side for CORBA 2.6.1 spec.
	assert(0);
    } else if (ip == END) {
	ClientFlowStack* fl = S_client_stack_.top();
	while (!fl->end_ip()->empty()) {
	    ClientRequestInterceptor_ptr cri = fl->end_ip()->top();
	    fl->end_ip()->pop();
	    ((cri)->*m)(ri);
	}
	S_client_stack_.pop();
	delete fl;
    }
}

void
PInterceptor::PI::_exec_server
(PortableInterceptor::ServerRequestInfo_ptr ri,
 PortableInterceptor::ServerMethod m,
 InterceptionPoint ip)
{
    MICOMT::AutoLock lock(PInterceptor::PI::S_server_lock_);
    if (ip == START) {
	ServerFlowStack* fl = new ServerFlowStack(S_server_req_int_);
	S_server_stack_.push(fl);
	while (!fl->start_ip()->empty()) {
	    ServerRequestInterceptor_ptr sri = fl->start_ip()->top();
	    fl->start_ip()->pop();
	    ((sri)->*m)(ri);
	    fl->end_ip()->push(sri);
	}
    } else if (ip == INTERMEDIATE) {
	vector<ServerRequestInterceptor_ptr>::iterator it;
	for (it = S_server_req_int_.begin(); it != S_server_req_int_.end(); it++) {
	    ServerRequestInterceptor_ptr sri = *it;
	    ((sri)->*m)(ri);	    
	}
    } else if (ip == END) {
	ServerFlowStack* fl = S_server_stack_.top();
	while (!fl->end_ip()->empty()) {
	    ServerRequestInterceptor_ptr sri = fl->end_ip()->top();
	    fl->end_ip()->pop();
	    ((sri)->*m)(ri);
	}
	S_server_stack_.pop();
	delete fl;
    }
}

void
PInterceptor::PI::_exec_send_request
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_client
	    (ri,
	     &PortableInterceptor::ClientRequestInterceptor::send_request,
	     START);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ClientRequestInfo_impl* ri_impl 
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::RECEIVE_OTHER);
	PInterceptor::PI::_exec_receive_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_send_poll
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_client
	    (ri,
	     &PortableInterceptor::ClientRequestInterceptor::send_poll,
	     START);
    } catch (CORBA::SystemException_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_receive_reply
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_client
	    (ri,
	     &PortableInterceptor::ClientRequestInterceptor::receive_reply,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw ex;
    }
}


void
PInterceptor::PI::_exec_receive_exception
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_client
	    (ri,
	     &PortableInterceptor::ClientRequestInterceptor::receive_exception,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::RECEIVE_OTHER);
	PInterceptor::PI::_exec_receive_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_receive_other
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_client
	    (ri,
	     &PortableInterceptor::ClientRequestInterceptor::receive_other,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::RECEIVE_OTHER);
	PInterceptor::PI::_exec_receive_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ClientRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ClientRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	PInterceptor::PI::_exec_receive_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_send_poll_ip
(ClientRequestInfo_impl* cri,
 CORBA::ULong request_id)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->request_id(request_id);
	cri->icept_oper(PInterceptor::SEND_POLL);
	cri->sync_scope(Messaging::SYNC_NONE);
	if (PInterceptor::Current_impl::current() == NULL) {
	    // resolving current is enough to get static reference later
	    CORBA::Object_var obj = cri->target();
	    CORBA::Object_var obj2
		= obj->_orbnc()->resolve_initial_references("PICurrent");
	}
	assert(PInterceptor::Current_impl::current() != NULL);
	// kcg: fix current usage
	const CORBA::AnySeq* s = PInterceptor::Current_impl::current()->slots();
	cri->slots(*s);
	PInterceptor::PI::_exec_send_poll(cri);
    }
}

void
PInterceptor::PI::_receive_exception_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::Exception* exception,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(PortableInterceptor::USER_EXCEPTION);
	cri->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	cri->exception(exception->_clone());
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_exception(cri); // end point
    }
}

void
PInterceptor::PI::_receive_exception_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::Exception* exception,
 CORBA::ContextList_ptr contexts,
 CORBA::Context_ptr operation_context,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(PortableInterceptor::USER_EXCEPTION);
	cri->icept_oper(PInterceptor::RECEIVE_EXCEPTION);
	cri->exception(exception->_clone());
	cri->contexts(contexts);
	cri->operation_context(operation_context);
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_exception(cri); // end point
    }
}

void
PInterceptor::PI::_receive_other_ip(ClientRequestInfo_impl* cri)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->icept_oper(PInterceptor::RECEIVE_OTHER);
	PInterceptor::PI::_exec_receive_other(cri); // end point	
    }
}

void
PInterceptor::PI::_receive_other_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(reply_status);
	cri->icept_oper(PInterceptor::RECEIVE_OTHER);
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_other(cri); // end point	
    }
}

void
PInterceptor::PI::_receive_other_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::Object_ptr effective_target,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(reply_status);
	cri->icept_oper(PInterceptor::RECEIVE_OTHER);
	cri->effective_target(effective_target);
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_other(cri); // end point	
    }
}

void
PInterceptor::PI::_receive_other_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::ContextList_ptr contexts,
 CORBA::Context_ptr operation_context,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(reply_status);
	cri->icept_oper(PInterceptor::RECEIVE_OTHER);
	cri->contexts(contexts);
	cri->operation_context(operation_context);
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_other(cri); // end point	
    }
}

void
PInterceptor::PI::_receive_other_ip
(ClientRequestInfo_impl* cri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::Object_ptr effective_target,
 CORBA::ContextList_ptr contexts,
 CORBA::Context_ptr operation_context,
 IOP::ServiceContextList* reply_scl)
{
    if (cri != NULL) {
	// any client interceptor registered?
	cri->reply_status(reply_status);
	cri->icept_oper(PInterceptor::RECEIVE_OTHER);
	cri->effective_target(effective_target);
	cri->contexts(contexts);
	cri->operation_context(operation_context);
	cri->set_reply_service_context_list(reply_scl);
	PInterceptor::PI::_exec_receive_other(cri); // end point	
    }
}

void
PInterceptor::PI::_exec_receive_request_service_contexts
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_server
	    (ri,
	     &PortableInterceptor::ServerRequestInterceptor
	     ::receive_request_service_contexts,
	     START);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	PInterceptor::PI::_exec_send_exception(ri);
	throw; // take care below!
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_OTHER);
	PInterceptor::PI::_exec_send_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	PInterceptor::PI::_exec_send_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_receive_request
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_server
	    (ri,
	     &PortableInterceptor::ServerRequestInterceptor::receive_request,
	     INTERMEDIATE);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl *>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->own_exception(TRUE);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::SEND_OTHER);
	PInterceptor::PI::_exec_send_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
       	PInterceptor::PI::_exec_send_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_send_reply
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    try	{
	PInterceptor::PI::_exec_server
	    (ri,
	     &PortableInterceptor::ServerRequestInterceptor::send_reply,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_send_exception
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_server
	    (ri,
	     &PortableInterceptor::ServerRequestInterceptor::send_exception,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->own_exception(TRUE);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::SEND_OTHER);
	PInterceptor::PI::_exec_send_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
       	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->own_exception(TRUE);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
       	PInterceptor::PI::_exec_send_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_exec_send_other
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    try {
	PInterceptor::PI::_exec_server
	    (ri,
	     &PortableInterceptor::ServerRequestInterceptor::send_other,
	     END);
    } catch (CORBA::SystemException_catch& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (PortableInterceptor::ForwardRequest_catch& ex) {
	ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<ServerRequestInfo_impl *>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex->_clone());
	ri_impl->effective_target(ex->forward);
	ri_impl->own_exception(TRUE);
	ri_impl->reply_status(PortableInterceptor::LOCATION_FORWARD);
	ri_impl->icept_oper(PInterceptor::SEND_OTHER);
	PInterceptor::PI::_exec_send_other(ri);
	throw;
    } catch (CORBA::UserException& ex) {
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::USER_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
       	PInterceptor::PI::_exec_send_exception(ri);
	throw;
    } catch (...) {
	CORBA::UNKNOWN ex;
	PInterceptor::ServerRequestInfo_impl* ri_impl
	    = dynamic_cast<PInterceptor::ServerRequestInfo_impl*>(ri);
	assert(!CORBA::is_nil(ri_impl));
	ri_impl->exception(ex._clone());
	ri_impl->reply_status(PortableInterceptor::SYSTEM_EXCEPTION);
	ri_impl->icept_oper(PInterceptor::SEND_EXCEPTION);
	ri_impl->own_exception(TRUE);
	PInterceptor::PI::_exec_send_exception(ri);
	throw ex;
    }
}

void
PInterceptor::PI::_receive_request_service_contexts_ip
(ServerRequestInfo_impl* sri,
 IOP::ServiceContextList* reply_scl,
 CORBA::ObjectAdapter* oa)
{
    if (sri != NULL) {
	// any sri registered?
	sri->set_reply_service_context_list(reply_scl);
	sri->icept_oper(PInterceptor::RECEIVE_REQUEST_SERVICE_CONTEXTS);
	sri->set_oa(oa);
	CORBA::AnySeq slots(0);
	slots.length(PInterceptor::PI::S_max_slot_id_);
	sri->slots(slots);
	PInterceptor::PI::_exec_receive_request_service_contexts(sri);
    }
}

void
PInterceptor::PI::_set_sri_exception
(ServerRequestInfo_impl* sri,
 const CORBA::Exception* exc)
{
    if (sri != NULL) {
	// any sri registered?
	if (exc != NULL)
	    sri->exception(exc->_clone());
	else
	    sri->exception(NULL);
    }
}

void
PInterceptor::PI::_send_reply_ip(ServerRequestInfo_impl* sri)
{
    if (sri != NULL) {
	// any sri registered?
	sri->reply_status(PortableInterceptor::SUCCESSFUL);
	sri->icept_oper(PInterceptor::SEND_REPLY);
	if (PInterceptor::Current_impl::current() == NULL) {
	    // resolving current is enough to get static reference later
	    CORBA::ORB_ptr orb = CORBA::ORB_instance
		("mico-local-orb", FALSE);
	    CORBA::Object_var obj2
		= orb->resolve_initial_references("PICurrent");
	}
	assert(PInterceptor::Current_impl::current() != NULL);
	// kcg: fix current usage
	const CORBA::AnySeq* s = PInterceptor::Current_impl::current()->slots();
	sri->slots(*s);
	PInterceptor::PI::_exec_send_reply(sri);
    }
}

void
PInterceptor::PI::_send_exception_ip
(ServerRequestInfo_impl* sri,
 PortableInterceptor::ReplyStatus reply_status)
{
    if (sri != NULL) {
	// any sri registered?
	sri->reply_status(reply_status);
	sri->icept_oper(PInterceptor::SEND_EXCEPTION);
	if (PInterceptor::Current_impl::current() == NULL) {
	    // resolving current is enough to get static reference later
	    CORBA::ORB_ptr orb = CORBA::ORB_instance
		("mico-local-orb", FALSE);
	    CORBA::Object_var obj2
		= orb->resolve_initial_references("PICurrent");
	}
	assert(PInterceptor::Current_impl::current() != NULL);
	// kcg: fix current usage
	const CORBA::AnySeq* s = PInterceptor::Current_impl::current()->slots();
	sri->slots(*s);
	PInterceptor::PI::_exec_send_exception(sri);
    }
}

void
PInterceptor::PI::_send_other_ip
(ServerRequestInfo_impl* sri,
 PortableInterceptor::ReplyStatus reply_status,
 CORBA::Object_ptr obj)
{
    if (sri != NULL) {
	// any sri registered?
	sri->reply_status(reply_status);
	sri->icept_oper(PInterceptor::SEND_OTHER);
	if (PInterceptor::Current_impl::current() == NULL) {
	    // resolving current is enough to get static reference later
	    CORBA::ORB_ptr orb = CORBA::ORB_instance
		("mico-local-orb", FALSE);
	    CORBA::Object_var obj2
		= orb->resolve_initial_references("PICurrent");
	}
	assert(PInterceptor::Current_impl::current() != NULL);
	// kcg: fix current usage
	const CORBA::AnySeq* s = PInterceptor::Current_impl::current()->slots();
	sri->slots(*s);
	if (reply_status == PortableInterceptor::LOCATION_FORWARD
	    && !CORBA::is_nil(obj)) {
	    sri->effective_target(obj);
	}
	PInterceptor::PI::_exec_send_other(sri);
    }
}



void
PInterceptor::PI::_exec_establish_components
(PortableInterceptor::IORInfo_ptr info)
{
    for (IORList::iterator it = S_ior_interceptors_.begin();
	 it != S_ior_interceptors_.end();
	 it++) {
	try {
	    (*it)->establish_components(info);
	} catch(...) {
	}
    }
}

void
PInterceptor::PI::_exec_add_client_interceptor
(PortableInterceptor::ClientRequestInterceptor_ptr interceptor)
{
    string i_name = interceptor->name();
    if (i_name != "") {
	// it's non-anonymous interceptor
	// so we have to check if it has unique name
	// as CORBA 2.6 (21.7.2.1) says
	vector<PortableInterceptor::ClientRequestInterceptor_ptr>::iterator it;
	for (it = S_client_req_int_.begin();
	     it != S_client_req_int_.end();
	     it++) {
	    string s = (*it)->name();
	    if (i_name == s) {
		PortableInterceptor::ORBInitInfo::DuplicateName ex;
		ex.name = s.c_str();
		mico_throw(ex);
	    }
	}
    }
    S_client_req_int_prepared_.push_back
	(ClientRequestInterceptor::_duplicate(interceptor));
}

void
PInterceptor::PI::_exec_add_server_interceptor
(PortableInterceptor::ServerRequestInterceptor_ptr interceptor)
{
    string i_name = interceptor->name();
    if (i_name != "") {
	// it's non-anonymous interceptor
	// so we have to check if it has unique name
	// as CORBA 2.6 (21.7.2.1) says
	vector<PortableInterceptor::ServerRequestInterceptor_ptr>::iterator it;
	for (it = S_server_req_int_.begin();
	     it != S_server_req_int_.end();
	     it++) {
	    string s = (*it)->name();
	    if (i_name == s) {
		PortableInterceptor::ORBInitInfo::DuplicateName ex;
		ex.name = s.c_str();
		mico_throw(ex);
	    }
	}
    }
    S_server_req_int_prepared_.push_back
	(ServerRequestInterceptor::_duplicate(interceptor));
}

void
PInterceptor::PI::_exec_add_ior_interceptor
(PortableInterceptor::IORInterceptor_ptr interceptor)
{
    IORList::iterator it;
    string i_name = interceptor->name();
    if (i_name != "") {
	// it's non-anonymous interceptor
	// so we have to check if it has unique name
	// as CORBA 2.6 (21.7.2.1) says
	for (it = S_ior_interceptors_.begin();
	     it != S_ior_interceptors_.end();
	     it++) {
	    string s = (*it)->name();
	    if (i_name == s) {
		PortableInterceptor::ORBInitInfo::DuplicateName ex;
		ex.name = s.c_str();
		mico_throw(ex);
	    }
	}
    }
    S_ior_interceptors_prepared_.push_back(IORInterceptor::_duplicate(interceptor));
}

PInterceptor::ServerRequestInfo_impl*
PInterceptor::PI::_create_sri
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ULong req_id,
 CORBA::Boolean resp,
 IOP::ServiceContextList* scl)
{
    if (S_server_req_int_.size() > 0) {
	return new ServerRequestInfo_impl(object, op_name, req_id, resp, scl);
    }
    return NULL;
}

PInterceptor::ClientRequestInfo_impl*
PInterceptor::PI::_create_cri
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ULong req_id,
 CORBA::NVList_ptr param_list,
 CORBA::ExceptionList_ptr exc_list,
 CORBA::ContextList_ptr ctx_list,
 CORBA::Context_ptr ctx,
 CORBA::NamedValue_ptr res,
 CORBA::Boolean resp)
{
    if (S_client_req_int_.size() > 0) {
	return new ClientRequestInfo_impl
	    (object, op_name, req_id, param_list,
	     exc_list, ctx_list, ctx, res, resp);
    }
    return NULL;
}

PInterceptor::ClientRequestInfo_impl*
PInterceptor::PI::_create_cri
(CORBA::Object_ptr object,
 const char* op_name,
 CORBA::ExceptionList_ptr exc_list,
 CORBA::NamedValue_ptr res)
{
    if (S_client_req_int_.size() > 0) {
	return new ClientRequestInfo_impl(object, op_name, exc_list, res);
    }
    return NULL;
}
