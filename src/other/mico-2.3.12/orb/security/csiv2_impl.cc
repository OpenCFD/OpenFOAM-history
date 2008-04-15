//
//  MICO CSIv2 --- an Open Source CSIv2 implementation
//  Copyright (C) 2002 ObjectSecurity Ltd.
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

//  Written by Karel Gardas, <kgardas@objectsecurity.com>


#include <mico/security/csiv2_impl.h>
#include <mico/codec_impl.h>
#include <mico/util.h>
#include <iostream>
#include <iomanip>

#ifndef HAVE_SSL
// this is an error since CSIv2 depends on MICO SSL support
#error "Please build MICO with SSL support in order to use MICO CSIv2"
#endif // HAVE_SSL

#define USE_ENCODE_VALUE


using namespace std;

//
// This is primitive debug function for printing octetseq in form:
// <hexa> <16 repeat>  <char> <16 repeat>
//

void debug(ostream* out, const CORBA::OctetSeq* seq)
{
    CORBA::ULong i,j;
    for (i=0; i<seq->length(); i+=16) {
	for (j=i; j<i+16; j++) {
	    if ((*seq).length() > j) {
		(*out) << setw(2) << setfill('0') << hex
		       << (short)(*seq)[j] << " ";
	    }
	    else {
		(*out) << "   ";
	    }
	    if ((j+1)%8 == 0)
		(*out) << "  ";
	}
	(*out) << "  ";
	for (j=i; j<seq->length() && j<i+16; j++) {
	    if (isprint((*seq)[j]))
		(*out) << (char)(*seq)[j];
	    else
		(*out) << '.';
	    if ((j+1)%8 == 0)
		(*out) << "  ";
	}
	(*out) << endl;
    }
    // restore settings
    (*out) << setw(0) << dec;
}

CSIv2::CSS_impl::CSS_impl()
    : sec_manager_(SecurityManager::_nil())
{
    CORBA::Any any;
    CSI::SASContextBody body;
    any <<= body;
    sas_body_tc_ = CORBA::TypeCode::_duplicate(any.type());
}


void
CSIv2::CSS_impl::security_manager(SecurityManager_ptr manager)
{
    if (!CORBA::is_nil(sec_manager_)) {
	CORBA::release(sec_manager_);
	sec_manager_ = SecurityManager::_nil();
    }
    sec_manager_ = SecurityManager::_duplicate(manager);
}

CSIv2::SecurityManager_ptr
CSIv2::CSS_impl::security_manager()
{
    return sec_manager_;
}

void
CSIv2::CSS_impl::write_sas_context
(PortableInterceptor::ClientRequestInfo_ptr info,
 const CSIIOP::CompoundSecMechList& plist)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::CSS_impl::write_sas_context" << endl;
    assert(plist.mechanism_list.length() > 0);
    CSIIOP::CompoundSecMech mech = plist.mechanism_list[0];
    CSI::GSS_NT_ExportedName target_name;
    target_name.length(0);
    target_name = mech.as_context_mech.target_name;
    // EstablishContext message
    CSI::EstablishContext msg;
    msg.client_context_id = 0;
    msg.authorization_token = 0;
    // auth layer checking
    CSI::GSSToken* token = NULL;
    CORBA::Boolean auth_mech_matched = FALSE;
    // we are supporting GSSUPMechOID as naming mech for auth layer
    CORBA::String_var mech_oid = ASN1::Codec::decode_oid
	(mech.as_context_mech.client_authentication_mech);
    if (strcmp(mech_oid.in(), GSSUP::GSSUPMechOID) == 0)
	auth_mech_matched = TRUE;
    // checking what tss auth support/require
    CORBA::UShort tss_auth = 0;
    if (mech.as_context_mech.target_supports >= 64) {
	// it supports EstablishTrustInClient
	tss_auth++;
    }
    if (mech.as_context_mech.target_requires >= 64) {
	// it requires EstablishTrustInClient
	tss_auth++;
    }
    // checking if client meets server needs
    if ((tss_auth - this->auth_layer() == 2)
	||((tss_auth == 2) && (!auth_mech_matched))) {
	// no because client doesn't support auth
	// and server requires it
	// or tss requires auth but naming mechanisms do not
	// matched
	CORBA::NO_PERMISSION ex(210301, CORBA::COMPLETED_NO);
	mico_throw(ex);
    }
    token = sec_manager_->get_gss_init_token(target_name);
    if (!token && this->auth_layer() > 0) {
	// CSS supports auth layer but user forgot
	// to suply user name on command line
	cerr << endl
	     << "CSIv2 ERROR: please supply client user name/passwd"
	     << " or disable CSS auth layer" << endl
	     << "             by using appropriate"
	     <<"  command-line parameter." << endl << endl;
	assert(0);
    }
    
    // checking attr layer
    CSI::GSS_NT_ExportedName* id = NULL;

    //int distinguished_name = mech.sas_context_mech.supported_identity_types/8;
    int r1 = mech.sas_context_mech.supported_identity_types%8;
    //int x509_certchain = r1/4;
    int r2 = r1%4;
    int principal_name = r2/2;
    //int r3 = r2%2;
    //int anonymous = r3/1;

    // naming mech.
    CORBA::Boolean attr_mech_matched = FALSE;
    CORBA::ULong i;
    for (i=0;
	 i<mech.sas_context_mech.supported_naming_mechanisms.length();
	 i++) {
	CORBA::String_var sas_mech_name = ASN1::Codec::decode_oid
	    (mech.sas_context_mech.supported_naming_mechanisms[i]);
	if (strcmp(sas_mech_name.in(), GSSUP::GSSUPMechOID) == 0) {
	    // we are supporting only GSSUPMechOID at the moment
	    attr_mech_matched = TRUE;
	}
    }
    CORBA::UShort tss_attr = 0;
    // checking what TSS attr supports/requires
    if (mech.sas_context_mech.target_supports >= 1024) {
	// it supports IdentityAssertion
	tss_attr++;
    }
    if (mech.sas_context_mech.target_requires >= 1024) {
	// it requires IdentityAssertion
	tss_attr++;
    }
    // checking if client meets server needs
    if ((tss_attr - this->attr_layer() == 2)
	||((tss_attr == 2) && (!attr_mech_matched))
	||(principal_name == 0)) {
	// no because client doesn't support attr
	// and server requires it
	// or tss requires attr but naming mechanisms do not
	// matched
	// or tss doesn't support ITTPrincipalName identity type
	CORBA::NO_PERMISSION ex(210302, CORBA::COMPLETED_NO);
	mico_throw(ex);
    }
    id = sec_manager_->client_identity_token();
    if (!id && this->attr_layer() > 0) {
	// CSS supports attr layer but user either 
	// forgot to suply user identity on command line
	// or identity can not be obtained from transport
	// layer
	cerr << endl
	     << "CSIv2 ERROR: please supply user identity or use"
	     << " TLS transport layer with identity" << endl
	     << "             or disable CSS attr layer by using appropriate"
	     << " command-line parameter." << endl << endl;
	assert(0);
    }

    // auth layer
    if (token != NULL && this->auth_layer() > 0 && tss_auth > 0) {
	// TSS & CSS supports auth + token != NULL
	msg.client_authentication_token = (*token);
    }
    else {
	msg.client_authentication_token = 0;
    }
    // attr layer
    if (id != NULL && this->attr_layer() > 0 && tss_attr > 0) {
	// TSS & CSS supports attr + id != NULL
	CSI::IdentityToken id_tok;
	id_tok.principal_name(*id);
	msg.identity_token = id_tok;
    }
    else {
	CSI::IdentityToken id_tok;
	id_tok.absent(TRUE);
	msg.identity_token = id_tok;
    }
    CSI::SASContextBody body;
    body.establish_msg(msg);
    CORBA::Any a;
    IOP::ServiceContext service_context;
    service_context.context_id = IOP::SecurityAttributeService;
    a <<= body;
    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
    CORBA::OctetSeq* data = codec->encode_value(a);
#else
    CORBA::OctetSeq* data = codec->encode(a);
#endif
    service_context.context_data.length(data->length());
    for (i=0; i<data->length(); i++) {
	service_context.context_data[i] = (*data)[i];
    }
    CORBA::release(codec);
    info->add_request_service_context(service_context, FALSE);
    if (id != NULL)
	delete id;
    delete token;
    delete data;
}

void
CSIv2::CSS_impl::send_request(PortableInterceptor::ClientRequestInfo_ptr info)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSS_impl::send_request" << endl;
    CSIIOP::CompoundSecMechList list;
    list.mechanism_list.length(0);
    try {
	IOP::TaggedComponent* c = info->get_effective_component
	    (CSIIOP::TAG_CSI_SEC_MECH_LIST);
//  	CORBA::Any t_any;
//  	t_any <<= list;
//  	CORBA::TypeCode_ptr tc = t_any.type();
//  	IOP::Codec_ptr codec = sec_manager_->codec();
//  	CORBA::Any* any = codec->decode_value(c->component_data, tc);
//  	(*any) >>= list;
	CORBA::Buffer* buf = new CORBA::Buffer;
	for (CORBA::ULong i=0; i<c->component_data.length(); i++)
	    buf->put(c->component_data[i]);
	CORBA::DataDecoder* dec = new MICO::CDRDecoder(buf);
	_marshaller_CSIIOP_CompoundSecMechList->demarshal(*dec, &list);
	delete c;
	delete dec;
    } catch (CORBA::BAD_PARAM& ex) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "catch CORBA::BAD_PARAM" << endl;
	}
    }
    this->write_sas_context(info, list);
}

void
CSIv2::CSS_impl::receive_reply(PortableInterceptor::ClientRequestInfo_ptr info)
{
    assert(!CORBA::is_nil(info));
    IOP::ServiceContext_var sas_context = info->get_request_service_context
	(IOP::SecurityAttributeService);
    assert(sas_context);
    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode_value
	(sas_context->context_data, sas_body_tc_);
#else // USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode(sas_context->context_data);
#endif // USE_ENCODE_VALUE
    CSI::SASContextBody rec_body;
    (*a1) >>= rec_body;
    delete a1;
    if (rec_body._d() == CSI::MTEstablishContext) {
	assert(0);
    } else if (rec_body._d() == CSI::MTMessageInContext) {
	assert(0);
    } else if (rec_body._d() == CSI::MTContextError) {
	assert(0);
    } else if (rec_body._d() == CSI::MTCompleteEstablishContext) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "CSS_impl: received complete establish context msg!" << endl;
    }
}

void
CSIv2::CSS_impl::receive_exception
(PortableInterceptor::ClientRequestInfo_ptr info)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSS_impl::receive_exception" << endl;
    assert(!CORBA::is_nil(info));
    IOP::ServiceContext_var sas_context = info->get_request_service_context
	(IOP::SecurityAttributeService);
    assert(sas_context);
    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode_value
	(sas_context->context_data, sas_body_tc_);
#else // USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode(sas_context->context_data);
#endif // USE_ENCODE_VALUE
    CSI::SASContextBody rec_body;
    (*a1) >>= rec_body;
    delete a1;
    if (rec_body._d() == CSI::MTEstablishContext) {
	// it's ok to receive system exception with establish context msg
	// assert(0);
    } else if (rec_body._d() == CSI::MTMessageInContext) {
	assert(0);
    } else if (rec_body._d() == CSI::MTContextError) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "CSS_impl: received context error msg!" << endl
		<< "client_context_id: "
#ifdef _MSC_VER
		<< (CORBA::Long)rec_body.error_msg().client_context_id << endl
#else // _MSC_VER
		<< rec_body.error_msg().client_context_id << endl
#endif // _MSC_VER
		<< "major_status: "
		<< rec_body.error_msg().major_status << endl
		<< "minor_status: "
		<< rec_body.error_msg().minor_status << endl;
	if (rec_body.error_msg().error_token.length() > 0) {
	    CORBA::Any* any;
	    GSSUP::ErrorToken et;
	    CORBA::Any ta;
	    ta <<= et;
	    CORBA::TypeCode_ptr et_tc = ta.type();
	    IOP::Codec_ptr codec = sec_manager_->codec();
	    any = codec->decode_value(rec_body.error_msg().error_token, et_tc);
	    (*any) >>= et;
	    CORBA::release(codec);
	    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
		if (et.error_code == GSSUP::GSS_UP_S_G_UNSPECIFIED)
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "GSS_UP_S_G_UNSPECIFIED" << endl;
		if (et.error_code == GSSUP::GSS_UP_S_G_NOUSER)
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "GSS_UP_S_G_NOUSER" << endl;
		if (et.error_code == GSSUP::GSS_UP_S_G_BAD_PASSWORD)
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "GSS_UP_S_G_BAD_PASSWORD" << endl;
		if (et.error_code == GSSUP::GSS_UP_S_G_BAD_TARGET)
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "GSS_UP_S_G_BAD_TARGET" << endl;
	    }
	}
    } else if (rec_body._d() == CSI::MTCompleteEstablishContext) {
	assert(0);
    }
}

void
CSIv2::CSS_impl::receive_other(PortableInterceptor::ClientRequestInfo_ptr info)
{
}

CSIv2::TSS_impl::TSS_impl()
    : sec_manager_(CSIv2::SecurityManager::_nil())
{
    CORBA::Any any;
    CSI::SASContextBody body;
    any <<= body;
    sas_body_tc_ = CORBA::TypeCode::_duplicate(any.type());
}

void
CSIv2::TSS_impl::security_manager(SecurityManager_ptr manager)
{
    if (!CORBA::is_nil(sec_manager_)) {
	CORBA::release(sec_manager_);
	sec_manager_ = SecurityManager::_nil();
    }
    sec_manager_ = SecurityManager::_duplicate(manager);
}

CSIv2::SecurityManager_ptr
CSIv2::TSS_impl::security_manager()
{
    return sec_manager_;
}

IOP::ServiceContext*
CSIv2::TSS_impl::accept_context
(PortableInterceptor::ServerRequestInfo_ptr info,
 CORBA::Boolean_out throw_exc)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "TSS_impl::accept_context" << endl;
    IOP::ServiceContext_var sas_context = info->get_request_service_context
	(IOP::SecurityAttributeService);
    assert(sas_context);
    throw_exc = FALSE;
    //CORBA::OctetSeq data = sas_context->context_data;
    IOP::Codec_ptr codec = sec_manager_->codec();
    CSI::SASContextBody rec_body;
#ifdef USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode_value
	(sas_context->context_data, sas_body_tc_);
#else // USE_ENCODE_VALUE
    CORBA::Any* a1 = codec->decode(sas_context->context_data);
#endif // USE_ENCODE_VALUE
    (*a1) >>= rec_body;
    delete a1;
    if (rec_body._d() == CSI::MTEstablishContext) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: received EstablishContext msg" << endl;
	CSI::CompleteEstablishContext* cec = NULL;
	try {
	    cec = this->establish_context(rec_body.establish_msg());
	    // context established
	    CSI::SASContextBody reply_body;
	    reply_body.complete_msg(*cec);
	    delete cec;
	    CORBA::Any a2;
	    a2 <<= reply_body;
	    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
	    CORBA::OctetSeq* data2 = codec->encode_value(a2);
#else // USE_ENCODE_VALUE
	    CORBA::OctetSeq* data2 = codec->encode(a2);
#endif // USE_ENCODE_VALUE
	    IOP::ServiceContext* sc = new IOP::ServiceContext;
	    sc->context_id = IOP::SecurityAttributeService;
	    sc->context_data = (*data2);
	    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "TSS_impl: reply SAS msg data" << endl;
		debug(&(MICO::Logger::Stream(MICO::Logger::Security)), data2);
	    }
	    delete data2;
	    CORBA::release(codec);
	    return sc;
	} catch (CSIv2::GSSAuthError_catch& ex) {
	    // context error
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "context error" << endl;
	    throw_exc = TRUE;
	    CSI::ContextError ctxerr;
	    ctxerr.client_context_id = 0;
	    ctxerr.major_status = 1;
	    ctxerr.minor_status = 1;
	    CORBA::OctetSeq_var e_tok = sec_manager_->error_token(ex.reason);
	    ctxerr.error_token = (*e_tok);
	    CSI::SASContextBody reply_body;
	    reply_body.error_msg(ctxerr);
	    CORBA::Any a3;
	    a3 <<= reply_body;
	    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
	    CORBA::OctetSeq* data3 = codec->encode_value(a3);
#else // USE_ENCODE_VALUE
	    CORBA::OctetSeq* data3 = codec->encode(a3);
#endif
	    IOP::ServiceContext* sc = new IOP::ServiceContext;
	    sc->context_id = IOP::SecurityAttributeService;
	    sc->context_data = (*data3);
	    delete data3;
	    CORBA::release(codec);
	    return sc;
	} catch (CSIv2::TrustIdentityError_catch& ex) {
	    // identity error
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "identity error" << endl;
	    throw_exc = TRUE;
	    // invalid evidence
	    CSI::ContextError ctxerr;
	    ctxerr.client_context_id = 0;
	    ctxerr.major_status = 1;
	    ctxerr.minor_status = 1;
	    CSI::SASContextBody reply_body;
	    reply_body.error_msg(ctxerr);
	    CORBA::Any a;
	    a <<= reply_body;
	    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
	    CORBA::OctetSeq* data = codec->encode_value(a);
#else // USE_ENCODE_VALUE
	    CORBA::OctetSeq* data = codec->encode(a);
#endif
	    IOP::ServiceContext* sc = new IOP::ServiceContext;
	    sc->context_id = IOP::SecurityAttributeService;
	    sc->context_data = (*data);
	    delete data;
	    CORBA::release(codec);
	    return sc;
	} catch (CSIv2::InvalidMechanism_catch& ex) {
	    // invalid mechanism
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "invalid mechanism" << endl;
	    throw_exc = TRUE;
	    // invalid mechanism
	    CSI::ContextError ctxerr;
	    ctxerr.client_context_id = 0;
	    ctxerr.major_status = 2;
	    ctxerr.minor_status = 1;
	    CSI::SASContextBody reply_body;
	    reply_body.error_msg(ctxerr);
	    CORBA::Any a;
	    a <<= reply_body;
	    IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
	    CORBA::OctetSeq* data = codec->encode_value(a);
#else // USE_ENCODE_VALUE
	    CORBA::OctetSeq* data = codec->encode(a);
#endif
	    IOP::ServiceContext* sc = new IOP::ServiceContext;
	    sc->context_id = IOP::SecurityAttributeService;
	    sc->context_data = (*data);
	    delete data;
	    CORBA::release(codec);
	    return sc;
	}
    }
    else if (rec_body._d() == CSI::MTMessageInContext) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: received MessageInContext msg" << endl;
	throw_exc = TRUE;
	CSI::ContextError ctxerr;
	ctxerr.client_context_id = 0;
	ctxerr.major_status = 4;
	ctxerr.minor_status = 1;
	CSI::SASContextBody reply_body;
	reply_body.error_msg(ctxerr);
	CORBA::Any a4;
	a4 <<= reply_body;
	IOP::Codec_ptr codec = sec_manager_->codec();
#ifdef USE_ENCODE_VALUE
	CORBA::OctetSeq* data4 = codec->encode_value(a4);
#else // USE_ENCODE_VALUE
	CORBA::OctetSeq* data4 = codec->encode(a4);
#endif
	IOP::ServiceContext* sc = new IOP::ServiceContext;
	sc->context_id = IOP::SecurityAttributeService;
	sc->context_data = (*data4);
	delete data4;
	CORBA::release(codec);
	return sc;
    }
    else {
	assert(0);
    }
    // never reached
    return 0;
}

CSI::CompleteEstablishContext*
CSIv2::TSS_impl::establish_context(const CSI::EstablishContext& msg)
    throw(CSIv2::GSSAuthError,
	  CSIv2::TrustIdentityError,
	  CSIv2::InvalidMechanism)
{
    if ((msg.client_authentication_token.length() == 0
	 && this->auth_layer() == 2)
	||(msg.client_authentication_token.length() > 0
	   && this->auth_layer() == 0)) {
	// CSS send msg without using auth layer
	// && TSS requires auth layer usage
	// or CSS send msg with auth token but TSS doesn't
	// support auth layer usage
	mico_throw(CSIv2::InvalidMechanism());
    }
    if ((msg.identity_token._d() != CSI::ITTPrincipalName
	 && this->attr_layer() == 2)
	||(msg.identity_token._d() != CSI::ITTAbsent
	   && this->attr_layer() == 0)) {
	// CSS send msg w/o asserted identity but TSS requires id assertion
	// or CSS send some identity but TSS doesn't support identity assertion
	mico_throw(CSIv2::InvalidMechanism());
    }
    CORBA::Boolean tb = FALSE;
    if (msg.client_authentication_token.length() > 0) {
	sec_manager_->auth_token(msg.client_authentication_token);
	tb = TRUE;
    }
    sec_manager_->verify_client_identity(msg.identity_token, tb);
    CSI::CompleteEstablishContext* cec = new CSI::CompleteEstablishContext;
    cec->client_context_id = msg.client_context_id;
    cec->context_stateful = FALSE;
    cec->final_context_token.length(0);
    return cec;
}

void
CSIv2::TSS_impl::accept_transport_context()
{
    CORBA::ORB_ptr orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var obj = orb->resolve_initial_references("PrincipalCurrent");
    CORBA::PrincipalCurrent_var p_current = CORBA::PrincipalCurrent::_narrow
	(obj);
    CORBA::Principal_var princ = p_current->get_principal();
    const char* str;
    CORBA::Any_var t_any;

    t_any = princ->get_property ("auth-method");
    t_any >>= str;
    string auth = str;
    if (auth != "ssl") {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: TLS not used, will throw NO_PERMISSION" << endl;
	mico_throw(CORBA::NO_PERMISSION());
    }
    t_any = princ->get_property ("ssl-x509-subject");
    t_any >>= str;
    CORBA::String_var user_name = str;
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "TSS_impl: accept_transport_context - user name: `"
	    << user_name << "'" << endl;
    CSIv2::DistinguishedNameList* user_list = sec_manager_->tls_user_list();
    for (CORBA::ULong i=0; i<user_list->length(); i++)
	if (user_name == (*user_list)[i])
	    return;
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "TSS_impl: accept_transport_context - name not found!" << endl;
    mico_throw(CORBA::NO_PERMISSION());
}

void
CSIv2::TSS_impl::receive_request_service_contexts
(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

void
CSIv2::TSS_impl::receive_request
(PortableInterceptor::ServerRequestInfo_ptr info)
{
    CORBA::Boolean msg_with_sas_context = FALSE;
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "TSS_impl: received request, will try SAS context" << endl;
    try {
	IOP::ServiceContext_var ctx
	    = info->get_request_service_context(IOP::SecurityAttributeService);
	msg_with_sas_context = TRUE;
    } catch (CORBA::SystemException_catch& ex) {
	// the server run under CSIv2 and receive msg without SAS context.
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: someone is trying to send me msg"
		<< " whithout SAS context." << endl;
    }
    if (msg_with_sas_context) {
	// msg contains SAS context message
	// we will try to enstablish contex
	if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: received request with SAS context" << endl
		<< "received data:" << endl;
	    IOP::ServiceContext_var t = info->get_request_service_context
		(IOP::SecurityAttributeService);
	    debug(&(MICO::Logger::Stream(MICO::Logger::Security)),
		  &(t->context_data));
	}
	CORBA::Boolean throw_exc;
	IOP::ServiceContext* reply_context = this->accept_context
	    (info, throw_exc);
	if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "reply data:" << endl;
	    debug(&(MICO::Logger::Stream(MICO::Logger::Security)),
		  &(reply_context->context_data));
	}
	if (throw_exc) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "TSS_impl: throwing exc from the sri..." << endl;
	    info->add_reply_service_context(*reply_context, TRUE);
	    mico_throw(CORBA::NO_PERMISSION());
	}
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: add context into request map" << endl;
	request_map_[info->request_id()] = (*reply_context);
	delete reply_context;
    }
    else {
	// msg doesn't contain SAS context message
	// we will try to accept context from transport layer

	// accept_transport_context throw appropriate
	// CORBA::NO_PERMISSION
	this->accept_transport_context();
    }
}

void
CSIv2::TSS_impl::send_reply(PortableInterceptor::ServerRequestInfo_ptr info)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "TSS_impl: send reply" << endl;
    if (request_map_.count(info->request_id()) > 0) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "TSS_impl: found SAS context for reply" << endl;
	RequestMapIterator i = request_map_.find(info->request_id());
	info->add_reply_service_context((*i).second, FALSE);
	request_map_.erase(i);
    }
}

void
CSIv2::TSS_impl::send_exception(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

void
CSIv2::TSS_impl::send_other(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

CSIv2::SecurityManager_impl::SecurityManager_impl()
    : csiv2_(FALSE), codec_factory_(IOP::CodecFactory::_nil()),
      codec_(IOP::Codec::_nil())
{
    tss_ = new TSS_impl;
    tss_->security_manager(this);
    css_ = new CSS_impl;
    css_->security_manager(this);
    GSSUP::InitialContextToken tok;
    CORBA::Any any;
    any <<= tok;
    init_token_tc_ = CORBA::TypeCode::_duplicate(any.type());
    csml_.stateful = FALSE;
    CSIIOP::CompoundSecMech mech;
    // transport layer configuration
    mech.transport_mech.tag = CSIIOP::TAG_NULL_TAG;
    csml_.mechanism_list.length(1);
    csml_.mechanism_list[0] = mech;
    // authentication layer configuration
    this->auth_layer(2);
    // attribute layer configuration
    this->attr_layer(1);
    // global compound mechanism target requires
    this->recompute_cm_req();
    // seting default realm
    this->realm_name("@default_realm");
}

void
CSIv2::SecurityManager_impl::tss(TSS_ptr value)
{
    CORBA::release(tss_);
    tss_ = TSS::_duplicate(value);
}

void
CSIv2::SecurityManager_impl::css(CSS_ptr value)
{
    CORBA::release(css_);
    css_ = CSS::_duplicate(value);
}

void
CSIv2::SecurityManager_impl::add_server_user(char* name, char* passwd)
{
    tss_users_.push_back(name);
    tss_passwds_.push_back(passwd);
}

void
CSIv2::SecurityManager_impl::client_user(char* name, char* passwd)
{
    css_user_ = name;
    css_passwd_ = passwd;
}

void
CSIv2::SecurityManager_impl::realm_name(const char* name)
{
    realm_ = name;
    // works only for the first element of csml_ list
    CORBA::OctetSeq* gss_realm_name = this->gss_export_name
	(GSSUP::GSSUPMechOID, name);
    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "encoded realm name: " << endl;
	debug(&(MICO::Logger::Stream(MICO::Logger::Security)), gss_realm_name);
    }
    csml_.mechanism_list[0].as_context_mech.target_name = *gss_realm_name;
}

CORBA::OctetSeq*
CSIv2::SecurityManager_impl::string2octet_seq(string str)
{
    CORBA::OctetSeq* seq = new CORBA::OctetSeq;
    seq->length(str.length());
    for (CORBA::ULong i=0; i<str.length(); i++)
	(*seq)[i] = (CORBA::Octet)str[i];
    return seq;
}

CSI::GSSToken*
CSIv2::SecurityManager_impl::get_gss_init_token
(const CSI::GSS_NT_ExportedName& target_name)
{
    if (css_user_ == "") {
	// the user doesn't set ''user name'' on command-line
	// so we will try to run without using auth layer
	return NULL;
    }
    GSSUP::InitialContextToken init_token;
    CORBA::OctetSeq* name = this->string2octet_seq(css_user_);
    CORBA::OctetSeq* passwd = this->string2octet_seq(css_passwd_);
    init_token.username.length(name->length());
    init_token.password.length(passwd->length());
    if (target_name.length() > 0)
	init_token.target_name = target_name;
    CORBA::ULong i;
    for (i=0; i<name->length(); i++)
	init_token.username[i] = (*name)[i];
    for (i=0; i<passwd->length(); i++)
	init_token.password[i] = (*passwd)[i];
    delete name;
    delete passwd;
    CSI::GSSToken* i_tok;
    CORBA::Any a;
    a <<= init_token;
    IOP::Codec_ptr codec = this->codec();
#ifdef USE_ENCODE_VALUE
    i_tok = codec->encode_value(a);
#else // USE_ENCODE_VALUE
    i_tok = codec->encode(a);
#endif
    CORBA::release(codec);
    CORBA::OctetSeq* oid = ASN1::Codec::encode_oid(GSSUP::GSSUPMechOID);
    CORBA::OctetSeq* ret_tok = new CORBA::OctetSeq;
    ret_tok->length(2 + oid->length() + i_tok->length());
    (*ret_tok)[0] = 0x60;
    assert(oid->length() + i_tok->length() < 128);
    (*ret_tok)[1] = oid->length() + i_tok->length();
    int s_ix = 2;
    for (i=0; i<oid->length(); i++)
	(*ret_tok)[i + s_ix] = (*oid)[i];
    s_ix = 2 + oid->length();
    for (i=0; i<i_tok->length(); i++)
	(*ret_tok)[i + s_ix] = (*i_tok)[i];
    delete i_tok;
    delete oid;
    return ret_tok;
}

void
CSIv2::SecurityManager_impl::codec_factory(IOP::CodecFactory_ptr factory)
{
    if (!CORBA::is_nil(codec_factory_)) {
	CORBA::release(codec_factory_);
	codec_factory_ = IOP::CodecFactory::_nil();
    }
    codec_factory_ = IOP::CodecFactory::_duplicate(factory);
}

IOP::Codec_ptr
CSIv2::SecurityManager_impl::codec()
{
    if (CORBA::is_nil(codec_)) {
	IOP::Encoding encoding;
	encoding.format = IOP::ENCODING_CDR_ENCAPS;
	encoding.major_version = 1;
	//encoding.minor_version = 0;
	encoding.minor_version = giop_version_ % 256;
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "SecurityManager uses GIOP version 1."
		<< (short)encoding.minor_version << endl;
	assert(!CORBA::is_nil(this->codec_factory()));
	codec_ = IOP::Codec::_duplicate
	    (this->codec_factory()->create_codec(encoding));
    }
    return IOP::Codec::_duplicate(codec_);
}

void
CSIv2::SecurityManager_impl::auth_token(const CSI::GSSToken& token)
    throw (CSIv2::GSSAuthError)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "SecurityManager_impl::auth_token: " << endl;
	debug(&MICO::Logger::Stream(MICO::Logger::Security), &token);
    }
    CSIv2::GSSAuthError ex;
    if (token[0] != 0x60) {
	// bad begining of token - it has to be 0x60
	ex.reason = GSSUP::GSS_UP_S_G_UNSPECIFIED;
	mico_throw(ex);
    }
    if (token[2] != 0x06) {
	// object identifier tag
	ex.reason = GSSUP::GSS_UP_S_G_UNSPECIFIED;
	mico_throw(ex);
    }
    CORBA::OctetSeq oid;
    int oid_len = token[3] + 2;  // OID length + tag + length
    oid.length(oid_len);
    for (int j=0; j<oid_len; j++)
	oid[j] = token[j+2];
    CORBA::String_var oid_str = ASN1::Codec::decode_oid(oid);
    if (strcmp(oid_str.in(), GSSUP::GSSUPMechOID) != 0) {
	// we support only GSSUP at the moment
	ex.reason = GSSUP::GSS_UP_S_G_UNSPECIFIED;
	mico_throw(ex);
    }
    CORBA::OctetSeq inner_token;
    int ix = oid_len + 2;
    inner_token.length(token.length() - ix);
    CORBA::ULong i;
    for (i=0; i<inner_token.length(); i++)
	inner_token[i] = token[ix + i];
    IOP::Codec_ptr codec = this->codec();
    CORBA::Any* a;
    GSSUP::InitialContextToken init_token;
#ifdef USE_ENCODE_VALUE
    a = codec->decode_value(inner_token, init_token_tc_);
#else // USE_ENCODE_VALUE
    a = codec->decode(inner_token);
#endif // USE_ENCODE_VALUE
    CORBA::release(codec);
    (*a) >>= init_token;
    string name = "";
    string passwd = "";
    for (i=0; i<init_token.username.length(); i++)
	name += init_token.username[i];
    for (i=0; i<init_token.password.length(); i++)
	passwd += init_token.password[i];
    delete a;
    list<string>::iterator it;
    list<string>::iterator jt;
    //CORBA::Boolean ret_val = FALSE;
    for (it = tss_users_.begin(), jt = tss_passwds_.begin();
	 it != tss_users_.end(), jt != tss_passwds_.end();
	 it++, jt++) {
	if ((*it) == name) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "found auth name: " << name << endl;
	    if ((*jt) == passwd) {
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "passwd is ok!" << endl;
		current_user_ = name;
		//ret_val = TRUE;
		//break;
		return;
	    }
	    else {
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
		        << "bad passwd!" << endl;
		ex.reason = GSSUP::GSS_UP_S_G_BAD_PASSWORD;
		mico_throw(ex);
	    }
	}
    }
    current_user_ = "";
    ex.reason = GSSUP::GSS_UP_S_G_NOUSER;
    mico_throw(ex);
}

IOP::CodecFactory_ptr
CSIv2::SecurityManager_impl::codec_factory()
{
    return codec_factory_;
}

CSI::GSSToken*
CSIv2::SecurityManager_impl::error_token(GSSUP::ErrorCode reason)
{
    CORBA::Any any;
    GSSUP::ErrorToken tok;
    tok.error_code = reason;
    any <<= tok;
    //CORBA::TypeCode_ptr token_tc;
    //token_tc = any.type();
    CSI::GSSToken* ret_token = codec_->encode_value(any);
    return ret_token;
}

CORBA::OctetSeq*
CSIv2::SecurityManager_impl::ior_component_data()
{
    //cerr << "::ior_component_data()" << endl;
//      CORBA::Any any;
//      any <<= csml_;
//      assert(!CORBA::is_nil(this->codec()));
//      CORBA::OctetSeq* data = this->codec()->encode_value(any);

    this->auth_layer(this->tss()->auth_layer());
    this->attr_layer(this->tss()->attr_layer());
    this->recompute_cm_req();

    //
    // This is a hack for the moment. The big item on TODO
    // list is to correct it and consult with spec. acording
    // the encapsulation usage e.g. coding with using IOP::Codec.
    // (above)
    //

    CORBA::Buffer* buf = new CORBA::Buffer;
    MICO::CDREncoder* encoder = new MICO::CDREncoder(buf);
    _marshaller_CSIIOP_CompoundSecMechList->marshal(*encoder, (void*)&csml_);
    buf->rseek_beg(0);
    CORBA::OctetSeq* data = new CORBA::OctetSeq;
    data->length(buf->length());
    for (CORBA::ULong i=0; i<data->length(); i++)
	(*data)[i] = (buf->data())[i];
    return data;
}

CORBA::OctetSeq*
CSIv2::SecurityManager_impl::gss_export_name(const char* oid, const char* name)
{
    //cerr << "gss_export_name(" << oid << ", " << name << ")" << endl;
    CORBA::OctetSeq* ret_val = new CORBA::OctetSeq;
    ret_val->length(2);
    (*ret_val)[0] = 0x04; // token id
    (*ret_val)[1] = 0x01; // token id
    CORBA::OctetSeq* t_oid = ASN1::Codec::encode_oid(oid);
    CORBA::OctetSeq* t_name = this->string2octet_seq(name);
    //cerr << "t_name->length(): " << t_name->length() << endl;
    // token id + OID + string length + string
    ret_val->length(2 + 2 + t_oid->length() + 4 + t_name->length());
    short oid_length = t_oid->length();
    (*ret_val)[2] = oid_length / 0xFF; // length of ASN1 OID
    (*ret_val)[3] = oid_length % 0xFF; // in big-endian order
    int i;
    for (i=0; i<oid_length; i++)
	(*ret_val)[i+4] = (*t_oid)[i];
    int name_length = t_name->length();
    char b4 = name_length / 0xFFFFFF;
    int l2 = name_length % 0xFFFFFF;
    char b3 = l2 / 0xFFFF;
    int l3 = l2 % 0xFFFF;
    char b2 = l3 / 0xFF;
    char b1 = l3 % 0xFF;
    (*ret_val)[4+oid_length+0] = b4;
    (*ret_val)[4+oid_length+1] = b3;
    (*ret_val)[4+oid_length+2] = b2;
    (*ret_val)[4+oid_length+3] = b1;
    for (i=0; i<name_length; i++)
	(*ret_val)[i+4+oid_length+4] = (*t_name)[i];
    delete t_oid;
    delete t_name;
    return ret_val;
}

char*
CSIv2::SecurityManager_impl::gss_import_name
(const char* oid,
 CORBA::OctetSeq* data)
{
    CORBA::ULong oid_length = (*data)[2] * 0xFF + (*data)[3];
    CORBA::OctetSeq oid_data;
    oid_data.length(oid_length);
    CORBA::ULong i;
    for (i=0; i<oid_length; i++)
	oid_data[i] = (*data)[i+4];
    CORBA::String_var s_oid = ASN1::Codec::decode_oid(oid_data);
    if (strcmp(s_oid.in(), oid) != 0) {
	// we are not supporting other mechanism than GSSUP
	return NULL;
    }
    CORBA::ULong name_length = 
	(*data)[4+oid_length+0] * 0xFFFFFF
	+ (*data)[4+oid_length+1] * 0xFFFF
	+ (*data)[4+oid_length+2] * 0xFF
	+ (*data)[4+oid_length+3];
    char* buf = new char[name_length+1];
    buf[name_length] = 0;
    for (i=0; i<name_length; i++)
	buf[i] = (*data)[4+oid_length+4+i];
    return buf;
}

void
CSIv2::SecurityManager_impl::tls_user_list
(const DistinguishedNameList& user_list)
{
    tls_user_list_ = user_list;
}

CSIv2::DistinguishedNameList*
CSIv2::SecurityManager_impl::tls_user_list()
{
    return &tls_user_list_;
}

void
CSIv2::SecurityManager_impl::user_id_list(const UserIdentityList& user_id_list)
{
    user_id_list_ = user_id_list;
}

CSIv2::UserIdentityList*
CSIv2::SecurityManager_impl::user_id_list()
{
    return &user_id_list_;
}

CSI::GSS_NT_ExportedName*
CSIv2::SecurityManager_impl::client_identity_token()
{
    CSI::GSS_NT_ExportedName* name = NULL;
    string id = this->client_identity();
    if (id != "") {
	name = this->gss_export_name
	    (GSSUP::GSSUPMechOID, this->client_identity());
    }
    return name;
}

void
CSIv2::SecurityManager_impl::verify_client_identity
(const CSI::IdentityToken& identity,
 CORBA::Boolean use_auth_layer)
    throw(CSIv2::TrustIdentityError)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "SecurityManager_impl: verify client identity" << endl;
    if (identity._d() == CSI::ITTAbsent) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "SecurityManager_impl: client identity is absent..." << endl;
	return;
    }
    if (identity._d() != CSI::ITTPrincipalName) {
	// throw exception because CSIv2 L0 supports either ITTAbsent
	// or ITTPrincipalName identity types
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "SecurityManager_impl: client identity is other type"
		<< " than ITTAbsent or ITTPrincipalName!" << endl;
	mico_throw(CSIv2::TrustIdentityError());
    }
    if (identity._d() == CSI::ITTPrincipalName) {
	// checking if asserted identity is valid for this client
	CORBA::OctetSeq id_data = identity.principal_name();
	char* t_name = this->gss_import_name(GSSUP::GSSUPMechOID, &id_data);
	if (t_name == NULL) {
	    mico_throw(CSIv2::TrustIdentityError());
	}
	string id = t_name;
	delete t_name;
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "decoded identity string `" << t_name << "'" << endl;
	if (use_auth_layer) {
	    // we'll check identity by using auth layer gss user name
	    for (CORBA::ULong i=0; i<user_id_list_.length(); i++) {
		if (current_user_ == user_id_list_[i].user_name.in()) {
		    if (MICO::Logger::IsLogged(MICO::Logger::Security))
			MICO::Logger::Stream(MICO::Logger::Security)
			    << "found identity user" << endl;
		    if (id == user_id_list_[i].identity_name.in()) {
			if (MICO::Logger::IsLogged(MICO::Logger::Security))
			    MICO::Logger::Stream(MICO::Logger::Security)
				<< "identity is ok!" << endl;
			return;
		    }
		    break;
		}
	    }
	}
	else {
	    // we'll check identity on the basis of TLS
	    // layer subject name
	    CORBA::ORB_ptr orb = CORBA::ORB_instance("mico-local-orb", FALSE);
	    CORBA::Object_var obj
		= orb->resolve_initial_references("PrincipalCurrent");
	    CORBA::PrincipalCurrent_var p_current
		= CORBA::PrincipalCurrent::_narrow(obj);
	    CORBA::Principal_var princ = p_current->get_principal();
	    const char* str;
	    CORBA::Any_var t_any;

	    t_any = princ->get_property ("auth-method");
	    t_any >>= str;
	    string auth = str;
	    if (auth == "ssl") {
		t_any = princ->get_property ("ssl-x509-subject");
		t_any >>= str;
		string tls_user_name = str;
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "TLS user name: `" << tls_user_name << "'" << endl;
		for (CORBA::ULong i=0; i<user_id_list_.length(); i++) {
		    if (tls_user_name == user_id_list_[i].user_name.in()) {
			if (MICO::Logger::IsLogged(MICO::Logger::Security))
			    MICO::Logger::Stream(MICO::Logger::Security)
				<< "found identity user" << endl;
			if (id == user_id_list_[i].identity_name.in()) {
			    if (MICO::Logger::IsLogged(MICO::Logger::Security))
				MICO::Logger::Stream(MICO::Logger::Security)
				    << "identity is ok!" << endl;
			    return;
			}
			break;
		    }
		}
	    }
	}
    }
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "identity is not valid!" << endl;
    mico_throw(CSIv2::TrustIdentityError());
}

void
CSIv2::SecurityManager_impl::client_identity(const char* identity)
{
    client_identity_ = identity;
}

char*
CSIv2::SecurityManager_impl::client_identity()
{
    return client_identity_;
}

void
CSIv2::SecurityManager_impl::auth_layer(CORBA::UShort value)
{
    if (value > 0) {
	// authentication layer configuration
	CSIIOP::AS_ContextSec as_context_mech;
	as_context_mech.target_supports = 64;
	if (value == 2) // requires
	    as_context_mech.target_requires = 64;
	else
	    as_context_mech.target_requires = 0;
	CORBA::OctetSeq* gssup_oid = ASN1::Codec::encode_oid
	    (GSSUP::GSSUPMechOID);
	as_context_mech.client_authentication_mech = (*gssup_oid);
	delete gssup_oid;
	for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++) {
	    as_context_mech.target_name
		= csml_.mechanism_list[i].as_context_mech.target_name;
	    csml_.mechanism_list[i].as_context_mech = as_context_mech;
	}
    }
    else {
	for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++)
	    csml_.mechanism_list[i].as_context_mech.target_supports = 0;
    }	
}

void
CSIv2::SecurityManager_impl::attr_layer(CORBA::UShort value)
{
    if (value > 0) {
	// attribute layer configuration
	CSIIOP::SAS_ContextSec sas_context_mech;
	sas_context_mech.target_supports = 1024;
	if (value == 2) // requires
	    sas_context_mech.target_requires = 1024;
	else
	    sas_context_mech.target_requires = 0;
	sas_context_mech.privilege_authorities.length(0);
	sas_context_mech.supported_naming_mechanisms.length(1);
	CORBA::OctetSeq* gssup_oid = ASN1::Codec::encode_oid
	    (GSSUP::GSSUPMechOID);
	sas_context_mech.supported_naming_mechanisms[0] = (*gssup_oid);
	delete gssup_oid;
	sas_context_mech.supported_identity_types = CSI::ITTPrincipalName;
	for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++)
	    csml_.mechanism_list[0].sas_context_mech = sas_context_mech;
    }
    else {
	for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++)
	    csml_.mechanism_list[0].sas_context_mech.target_supports = 0;
    }
}

void
CSIv2::SecurityManager_impl::recompute_cm_req()
{
    for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++) {
	// global compound mechanism target requires
	CORBA::UShort reqs = 0;
	if (csml_.mechanism_list[i].transport_mech.tag != CSIIOP::TAG_NULL_TAG)
	    reqs = reqs|70;
	else
	    reqs = reqs|64;
	if (csml_.mechanism_list[i].as_context_mech.target_supports != 0)
	    reqs = reqs
		|csml_.mechanism_list[i].as_context_mech.target_requires;
	if (csml_.mechanism_list[i].sas_context_mech.target_supports != 0)
	    reqs = reqs
		|csml_.mechanism_list[i].sas_context_mech.target_requires;
	csml_.mechanism_list[i].target_requires = reqs;
    }
}

void
CSIv2::ServerRequestInterceptor_impl::destroy()
{
    CORBA::release(sec_manager_);
    CORBA::release(codec_factory_);
    CORBA::release(tss_);
}

void
CSIv2::ServerRequestInterceptor_impl::receive_request_service_contexts
(PortableInterceptor::ServerRequestInfo_ptr info)
{
    // it's here because PI has a bug
    // that calling add_reply_service_context
    // will not add context while throwing exception
    // in SRI::receive_context, which is a bug!
    // Here it's safe :-(((
    if (sec_manager_->csiv2()) {
	tss_->receive_request(info);
    }
}

void
CSIv2::ServerRequestInterceptor_impl::receive_request
(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

void
CSIv2::ServerRequestInterceptor_impl::send_reply
(PortableInterceptor::ServerRequestInfo_ptr info)
{
    if (sec_manager_->csiv2()) {
	tss_->send_reply(info);
    }
}

void
CSIv2::ServerRequestInterceptor_impl::send_exception
(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

void
CSIv2::ServerRequestInterceptor_impl::send_other
(PortableInterceptor::ServerRequestInfo_ptr info)
{
}

void
CSIv2::ClientRequestInterceptor_impl::destroy()
{
    CORBA::release(sec_manager_);
    CORBA::release(codec_factory_);
    CORBA::release(css_);
}

void
CSIv2::ClientRequestInterceptor_impl::send_request
(PortableInterceptor::ClientRequestInfo_ptr info)
{
    assert(!CORBA::is_nil(sec_manager_));
    if (sec_manager_->csiv2()) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "client: send_request with SAS service context" << endl;
	// we are using CSIv2
	css_->send_request(info);
    }
}

void
CSIv2::ClientRequestInterceptor_impl::receive_reply
(PortableInterceptor::ClientRequestInfo_ptr info)
{
    if (sec_manager_->csiv2()) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "client: receive_reply" << endl;
	try {
	    IOP::ServiceContext_var ctx
		= info->get_request_service_context(IOP::SecurityAttributeService);
	    css_->receive_reply(info);
	} catch (CORBA::BAD_PARAM& ex) {
	    // service context doesn't contain SAS msg
	}
    }
}

void
CSIv2::ClientRequestInterceptor_impl::receive_exception
(PortableInterceptor::ClientRequestInfo_ptr info)
{
    if (sec_manager_->csiv2()) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "client: receive_exception" << endl;
	try {
	    IOP::ServiceContext_var ctx
		= info->get_request_service_context(IOP::SecurityAttributeService);
	    css_->receive_exception(info);
	} catch (CORBA::BAD_PARAM& ex) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "client: service context doesn't contain SAS msg"
		    << endl;
	}
    }
}

void
CSIv2::ClientRequestInterceptor_impl::receive_other
(PortableInterceptor::ClientRequestInfo_ptr info)
{
    if (sec_manager_->csiv2()) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "client: receive_other" << endl;
    }
}

void
CSIv2::InterceptorInitializer_impl::pre_init
(PortableInterceptor::ORBInitInfo_ptr info)
{
    IOP::CodecFactory_ptr factory = info->codec_factory();
    sec_manager_->codec_factory(factory);
    if (sec_manager_->csiv2()) {
	ClientRequestInterceptor_impl* cri 
	    = new ClientRequestInterceptor_impl
	    ("CSIv2::ClientRequestInterceptor_impl", sec_manager_, factory);
	info->add_client_request_interceptor(cri);
	ServerRequestInterceptor_impl* sri
	    = new ServerRequestInterceptor_impl
	    ("CSIv2::ServerRequestInterceptor_impl", sec_manager_, factory);
	info->add_server_request_interceptor(sri);
	IORInterceptor_impl* ii = new IORInterceptor_impl
	    ("CSIv2::IORInterceptor_impl", sec_manager_);
	info->add_ior_interceptor(ii);
    }
    // it register itself
    CSIv2::ComponentDecoder* dc = new CSIv2::ComponentDecoder;
    dc->codec(sec_manager_->codec());
}

void
CSIv2::IORInterceptor_impl::destroy()
{
    CORBA::release(sec_manager_);
}

void
CSIv2::IORInterceptor_impl::establish_components
(PortableInterceptor::IORInfo_ptr info)
{
    if (sec_manager_->csiv2()) {
	if (MICO::Logger::IsLogged(MICO::Logger::Security))
	    MICO::Logger::Stream(MICO::Logger::Security)
		<< "IOR: establish_components" << endl;
	IOP::TaggedComponent component;
	component.tag = CSIIOP::TAG_CSI_SEC_MECH_LIST;
	CORBA::OctetSeq_var tmp = sec_manager_->ior_component_data();
	component.component_data = tmp;
	if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
	    debug(&(MICO::Logger::Stream(MICO::Logger::Security)),
		  &(component.component_data));
	}
	//info->add_ior_component(component);
	// we need to add this component into IIOP profile
	// we can't simply add it into every profile, because
	// every in MICO means MultiComponent profile too, and
	// as CSIv2 spec. says the usage of MultiComponent profile
	// is not supported at the moment
	try {
	    info->add_ior_component_to_profile
		(component, CORBA::IORProfile::TAG_INTERNET_IOP);
	} catch (CORBA::BAD_PARAM& ex) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "IOR: can't find IIOP profile" << endl;
	}
	try {
	    info->add_ior_component_to_profile
		(component, CORBA::IORProfile::TAG_SSL_INTERNET_IOP);
	} catch (CORBA::BAD_PARAM& ex) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "IOR: can't find SSL IIOP profile" << endl;
	}
    }
}

//
// CSIv2 IOR Component and ComponentDecoder
//

CSIv2::Component::Component(const Component& c)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::Component::Component(const Component& c)" << endl;
    csml_ = c.csml_;
    codec_ = IOP::Codec::_duplicate(c.codec_);
}

void
CSIv2::Component::encode(CORBA::DataEncoder& encoder) const
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::Component::encode(CORBA::DataEncoder& encoder) const"
	    << endl
	    << "stateful: " << (short)csml_.stateful << endl;
    assert(!CORBA::is_nil(codec_));
    _marshaller_CSIIOP_CompoundSecMechList->marshal(encoder, (void*)&csml_);
//      CORBA::Any any;
//      any <<= csml_;
//      CORBA::OctetSeq* t_buf = codec_->encode_value(any);
//      CORBA::Octet* t_octets = new CORBA::Octet[t_buf->length()];
//      for (CORBA::ULong i=0; i<t_buf->length(); i++) {
//  	t_octets[i] = ((*t_buf)[i]);
//  	cerr << (short)t_octets[i] << ", ";
//      }
//      encoder.put_octets(t_octets, t_buf->length());
}

CORBA::Component::ComponentId
CSIv2::Component::id() const
{
    return CSIIOP::TAG_CSI_SEC_MECH_LIST;
}

void
CSIv2::Component::print(ostream& out) const
{
    out << endl;
    out << "              ";
    out << "CSIv2:" << endl;
    out << "              ";
    out << "ComponentId: TAG_CSI_SEC_MECH_LIST" << endl;
    out << "              ";
    out << "   Stateful: ";
    if (csml_.stateful)
	out << "true" << endl;
    else
	out << "false" << endl;
    out << "              ";
    out << "     Length: " << csml_.mechanism_list.length() << endl;
    for (CORBA::ULong i=0; i<csml_.mechanism_list.length(); i++) {
	out << "              ";
	out << "        [" << i << "]: ";
	//out << "              ";
	//out << "              ";
	out << "target_requires: "
	    << csml_.mechanism_list[i].target_requires << endl;
	out << "              ";
	out << "            ";
	out << " transport_mech:  tag: ";
	if (csml_.mechanism_list[i].transport_mech.tag == CSIIOP::TAG_NULL_TAG)
	    out << "TAG_NULL_TAG" << endl;
	else if (csml_.mechanism_list[i].transport_mech.tag
		 == CSIIOP::TAG_SECIOP_SEC_TRANS) {
	    out << "TAG_SECIOP_SEC_TRANS" << endl;
	    out << "              ";
	    out << "              ";
	    out << "             ";
	    out << "  data: `";
	    for (CORBA::ULong j=0;
		 j<csml_.mechanism_list[i].transport_mech
		     .component_data.length();
		 j++) {
		CORBA::Octet c = csml_.mechanism_list[i]
		    .transport_mech.component_data[j];
		if (isprint(c))
		    out << (char)c;
		else
		    out << '.';
	    }
	    out << "'" << endl;
	}
	else if (csml_.mechanism_list[i].transport_mech.tag
		 == CSIIOP::TAG_TLS_SEC_TRANS) {
	    out << "TAG_TLS_SEC_TRANS" << endl;
	    out << "              ";
	    out << "              ";
	    out << "              ";
	    out << "       ";
	    CORBA::Any tc_any;
	    CSIIOP::TLS_SEC_TRANS tls_sec_trans;
	    tc_any <<= tls_sec_trans;
	    CORBA::OctetSeq tls_comp_data
		= csml_.mechanism_list[i].transport_mech.component_data;
	    CORBA::Any* tls_sec_trans_any
		= codec_->decode_value(tls_comp_data, tc_any.type());
	    (*tls_sec_trans_any) >>= tls_sec_trans;
	    out << "target supports: " << tls_sec_trans.target_supports << endl;
	    out << "              ";
	    out << "              ";
	    out << "              ";
	    out << "       ";
	    out << "target requires: " << tls_sec_trans.target_requires << endl;
	    out << "              ";
	    out << "              ";
	    out << "              ";
	    out << "       ";
	    for (CORBA::ULong j = 0; j<tls_sec_trans.addresses.length(); j++) {
		out << "addresses: ";
		out << tls_sec_trans.addresses[j].host_name;
		out << ":";
		out << tls_sec_trans.addresses[j].port;
		if (j+1 < tls_sec_trans.addresses.length())
		    out << ", ";
	    }
	    out << endl;
	}
	else {
	    out << "Unknown tag: "
		<< csml_.mechanism_list[i].transport_mech.tag << endl;
	    out << "              ";
	    out << "              ";
	    out << "             ";
	    out << "  data: `";
	    for (CORBA::ULong j=0;
		 j<csml_.mechanism_list[i]
		     .transport_mech.component_data.length();
		 j++) {
		CORBA::Octet c
		    = csml_.mechanism_list[i].transport_mech.component_data[j];
		if (isprint(c))
		    out << (char)c;
		else
		    out << '.';
	    }
	    out << "'" << endl;
	}
	// AS_ContextSec as_context_mech;
	if (csml_.mechanism_list[i].as_context_mech.target_supports == 0) {
	    out << "              ";
	    out << "             ";
	    out << "as_context_mech.target_supports = 0" << endl;
	}
	else {
	    CSIIOP::AS_ContextSec cs = csml_.mechanism_list[i].as_context_mech;
	    out << "              ";
	    out << "             ";
	    out << "as_context_mech.target_supports: ";
	    out << cs.target_supports << endl;
	    out << "              ";
	    out << "             ";
	    out << "as_context_mech.target_requires: ";
	    out << cs.target_requires << endl;
	    out << "              ";
	    out << "             ";
	    out << "as_context_mech.client_authentication_mech: ";
	    string str = ASN1::Codec::decode_oid(cs.client_authentication_mech);
	    out << str;
	    if (str == GSSUP::GSSUPMechOID)
		out << " (GSSUP::GSSUPMechOID)";
	    else if (str == CSI::KRB5MechOID)
		out << " (CSI::KRB5MechOID)";
	    else if (str == CSI::GSS_NT_Export_Name_OID)
		out << " (CSI::GSS_NT_Export_Name_OID)";
	    else if (str == CSI::GSS_NT_Scoped_Username_OID)
		out << " (CSI::GSS_NT_Scoped_Username_OID)";
	    out << endl;
	    out << "              ";
	    out << "             ";
	    out << "as_context_mech.target_name: `";
	    for (CORBA::ULong j=0; j<cs.target_name.length(); j++) {
		CORBA::Octet c = cs.target_name[j];
		if (isprint(c))
		    out << (char)c;
		else
		    out << '.';
	    }
	    out << "'" << endl;
	}
	if (csml_.mechanism_list[i].sas_context_mech.target_supports == 0) {
	    out << "              ";
	    out << "             ";
	    out << "sas_context_mech.target_supports = 0" << endl;
	}
	else {
	    CSIIOP::SAS_ContextSec scs
		= csml_.mechanism_list[i].sas_context_mech;
	    out << "              ";
	    out << "             ";
	    out << "sas_context_mech.target_supports: ";
	    out << scs.target_supports << endl;
	    out << "              ";
	    out << "             ";
	    out << "sas_context_mech.target_requires: ";
	    out << scs.target_requires << endl;
	    CSIIOP::ServiceConfigurationList pa = scs.privilege_authorities;
	    CORBA::ULong j;
	    for (j=0; j<pa.length(); j++) {
		out << "              ";
		out << "             ";
		out << "sas_context_mech.privilege_authorities[";
		out << j << "].syntax: " << pa[j].syntax << endl;
		out << "              ";
		out << "             ";
		out << "sas_context_mech.privilege_authorities[";
		out << j << "].name: `";
		for (CORBA::ULong k=0; k<pa[j].name.length(); k++) {
		    CORBA::Octet c = pa[j].name[k];
		    if (isprint(c))
			out << (char)c;
		    else
			out << '.';
		}
		out << "'" << endl;
	    }
	    CSI::OIDList oid_list = scs.supported_naming_mechanisms;
	    for (j=0; j<oid_list.length(); j++) {
		out << "              ";
		out << "             ";
		out << "sas_context_mech.supported_naming_mechanisms[";
		out << j << "]: ";
		string str = ASN1::Codec::decode_oid(oid_list[j]);
		out << str;
		if (str == GSSUP::GSSUPMechOID)
		    out << " (GSSUP::GSSUPMechOID)";
		else if (str == CSI::KRB5MechOID)
		    out << " (CSI::KRB5MechOID)";
		else if (str == CSI::GSS_NT_Export_Name_OID)
		    out << " (CSI::GSS_NT_Export_Name_OID)";
		else if (str == CSI::GSS_NT_Scoped_Username_OID)
		    out << " (CSI::GSS_NT_Scoped_Username_OID)";
		out << endl;
	    }
	    out << "              ";
	    out << "             ";
	    out << "sas_context_mech.supported_identity_types: ";
	    out << scs.supported_identity_types << endl;
	}
    }
    out << endl;
}

CORBA::Component*
CSIv2::Component::clone() const
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::Component::clone() const" << endl;
    return new CSIv2::Component(*this);
}

CORBA::Long
CSIv2::Component::compare(const CORBA::Component& component) const
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::Component::compare(const CORBA::Component& component)"
	    << " const" << endl;
    if (this->id() != component.id())
	return (CORBA::Long)this->id() - (CORBA::Long)component.id();
    return 0;
}

CORBA::Boolean
CSIv2::Component::operator==(const CORBA::Component& component) const
{
    return this->compare (component) == 0;
}

CORBA::Boolean
CSIv2::Component::operator<(const CORBA::Component& component) const
{
    return this->compare (component) < 0;
}

void
CSIv2::Component::codec(IOP::Codec_ptr codec)
{
    if (!CORBA::is_nil(codec_))
	CORBA::release(codec_);
    codec_ = IOP::Codec::_duplicate(codec);
}

CSIIOP::CompoundSecMechList*
CSIv2::Component::mech_list()
{
    return &csml_;
}

CSIv2::ComponentDecoder::ComponentDecoder()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::ComponentDecoder::ComponentDecoder()" << endl;
    CORBA::Component::register_decoder(this);
}

CSIv2::ComponentDecoder::~ComponentDecoder()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::ComponentDecoder::~ComponentDecoder()" << endl;
    CORBA::Component::unregister_decoder(this);
}

CORBA::Component*
CSIv2::ComponentDecoder::decode
(CORBA::DataDecoder& decoder,
 ComponentId id,
 CORBA::ULong length) const
{
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "CSIv2::ComponentDecoder::decode(...):"
	    << " length(" << length << ")" << endl;
    CSIIOP::CompoundSecMechList list;
    _marshaller_CSIIOP_CompoundSecMechList->demarshal(decoder, &list);
    CSIv2::Component* comp = new CSIv2::Component(list, codec_);
    if (MICO::Logger::IsLogged(MICO::Logger::Security))
	MICO::Logger::Stream(MICO::Logger::Security)
	    << "comp: " << comp << endl;
    return comp;
//      CORBA::Octet* t_buf = new CORBA::Octet[length];
//      if (!decoder.get_octets(t_buf, length)) {
//  	cerr << "return NULL;" << endl;
//  	return NULL;
//      }
//      CORBA::OctetSeq seq;
//      seq.length(length);
//      for (CORBA::ULong i=0; i<length; i++) {
//  	seq[i] = t_buf[i];
//  	cerr << (short)seq[i] << ", ";
//      }
//      assert(!CORBA::is_nil(codec_));
//      CORBA::Any t_any;
//      CSIIOP::CompoundSecMechList list;
//      t_any <<= list;
//      CORBA::TypeCode_ptr tc = t_any.type();
//      assert(!CORBA::is_nil(tc));
//      CORBA::Any* any = codec_->decode_value(seq, tc);
//      (*any) >>= list;
//      CSIv2::Component* comp = new CSIv2::Component(list, codec_);
//      cerr << "comp: " << comp << endl;
//      return comp;
}

CORBA::Boolean
CSIv2::ComponentDecoder::has_id(CORBA::Component::ComponentId id) const
{
    return id == CSIIOP::TAG_CSI_SEC_MECH_LIST;
}

void
CSIv2::ComponentDecoder::codec(IOP::Codec_ptr codec)
{
    codec_ = IOP::Codec::_duplicate(codec);
}
