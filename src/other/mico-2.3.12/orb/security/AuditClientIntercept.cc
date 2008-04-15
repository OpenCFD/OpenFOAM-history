//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000 ObjectSecurity Ltd.
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


#define MICO_CONF_INTERCEPT
#define MICO_CONF_IMR

#include <mico/security/AuditIntercept.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

static MICOGetOpt::OptVec auditclient_options;
AuditClientInterceptor* AuditClientInterceptor::S_self_;

//
// Audit Interceptor
//
CORBA::Octet *
AuditClientInterceptor::get_audit_id
(PortableInterceptor::ClientRequestInfo_ptr req,
 CORBA::ULong& l)
{
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var securitymanager;
    SecurityLevel2::SecurityManager_var seccur;
    securitymanager = orb->resolve_initial_references ("SecurityManager");
    assert (!CORBA::is_nil (securitymanager)); // change to exc
  
    seccur = SecurityLevel2::SecurityManager::_narrow(securitymanager);
    assert (!CORBA::is_nil (seccur)); // change to exc
  
    SecurityLevel2::TargetCredentials_var tc = seccur->get_target_credentials(req->effective_target());
  
    Security::ExtensibleFamily fam;
    fam.family_definer = 0;
    fam.family = 0;
    Security::AttributeType at;
    at.attribute_family = fam;
    at.attribute_type = Security::AuditId;
    Security::AttributeTypeList atl;
    atl.length(1);
    atl[0] = at;
    // ###ras
    Security::AttributeList_var al =  tc->get_attributes( atl );
    l = (CORBA::ULong)(*al)[0].value.length();
    if (ret_ != NULL) {
	delete[] ret_;
	ret_ = NULL;
    }
    ret_ = new CORBA::Octet[l];
    for (CORBA::ULong i = 0; i < l; i++)
	ret_[i] = (*al)[0].value[i];
    return ret_;
}

CORBA::Octet *
AuditClientInterceptor::get_object_ref
(PortableInterceptor::ClientRequestInfo_ptr req,
 CORBA::ULong& l)
{
    CORBA::Object_ptr target = req->effective_target();
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
  
    CORBA::IORProfile * prf = target->_ior()->profile(CORBA::IORProfile::TAG_SSL_INTERNET_IOP);
    if (!prf)
	prf = target->_ior()->profile(CORBA::IORProfile::TAG_INTERNET_IOP);  // not secure
    const CORBA::Address * addr = prf->addr();
  
    string result ("iioploc://");
    CORBA::Long length;
    const CORBA::Octet * ok = prf->objectkey(length);
    string str_addr = addr->stringify();
    result += str_addr; //.substr(str_addr.find(":")+1);
    if (length > 0) {
	CORBA::String_var url = mico_url_encode(ok,length);
	result += "/";
	result += url.in();
    }
  
    l = result.size();
    if (ret_ != NULL) {
	delete[] ret_;
	ret_ = NULL;
    }
    ret_ = new CORBA::Octet[l];
    result.copy((char *)ret_, l);
    return ret_;
}

CORBA::Octet *
AuditClientInterceptor::get_interface_name
(PortableInterceptor::ClientRequestInfo_ptr req,
 CORBA::ULong& l)
{
    CORBA::Object_ptr target = req->effective_target();
    const char* name = target->_ior()->objid();
    l = (CORBA::ULong)strlen(name);
    if (ret_ != NULL) {
	delete[] ret_;
	ret_ = NULL;
    }
    ret_ = new CORBA::Octet[l];
    for (CORBA::ULong i = 0; i < l; i++)
	ret_[i] = name[i];
    return ret_;
}


AuditClientInterceptor::AuditClientInterceptor()
{
    ret_ = NULL;
    S_self_ = this;
}

AuditClientInterceptor::~AuditClientInterceptor()
{
}

char*
AuditClientInterceptor::name()
{
    return CORBA::string_dup("AuditClient");
}

void
AuditClientInterceptor::send_request
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    // nothing
}

void
AuditClientInterceptor::send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    // nothing
}

void
AuditClientInterceptor::receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    char* s = ri->operation();
    CORBA::ULong l;
    CORBA::ULong j;
    CORBA::Octet* inter = this->get_interface_name(ri, l);
    char* interface = new char[l+1];
    interface[l] = 0;
    CORBA::String_var g1 = interface;
    for (j = 0; j < l; j++)
	interface[j] = (char)inter[j];
    CORBA::Octet* oref = this->get_object_ref(ri, l);
    char* obj_ref = new char[l+1];
    obj_ref[l] = 0;
    CORBA::String_var g2 = obj_ref;
    for (j = 0; j < l; j++)
	obj_ref[j] = (char)oref[j];
    CORBA::Octet* init = this->get_audit_id(ri, l);
    char* initiator = new char[l+1];
    initiator[l] = 0;
    CORBA::String_var g3 = initiator;
    for (j = 0; j < l; j++)
	initiator[j] = (char)init[j];

    this->audit_analyse(Security::AuditInvocation, interface, obj_ref, s, initiator, (short)(1), CLIENT);
}

void
AuditClientInterceptor::receive_exception
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    char* s = ri->operation();
    CORBA::ULong l;
    CORBA::ULong j;
    CORBA::Octet* inter = this->get_interface_name(ri, l);
    char* interface = new char[l+1];
    interface[l] = 0;
    CORBA::String_var g1 = interface;
    for (j = 0; j < l; j++)
	interface[j] = (char)inter[j];
    CORBA::Octet* oref = this->get_object_ref(ri, l);
    char* obj_ref = new char[l+1];
    obj_ref[l] = 0;
    CORBA::String_var g2 = obj_ref;
    for (j = 0; j < l; j++)
	obj_ref[j] = (char)oref[j];
    CORBA::Octet* init = this->get_audit_id(ri, l);
    char* initiator = new char[l+1];
    initiator[l] = 0;
    CORBA::String_var g3 = initiator;
    for (j = 0; j < l; j++)
	initiator[j] = (char)init[j];

    this->audit_analyse (Security::AuditInvocation, interface, obj_ref, s, initiator, (short)(0), CLIENT);
}

void
AuditClientInterceptor::receive_other
(PortableInterceptor::ClientRequestInfo_ptr ri)
{
    char* s = ri->operation();
    CORBA::ULong l;
    CORBA::ULong j;
    CORBA::Octet* inter = this->get_interface_name(ri, l);
    char* interface = new char[l+1];
    interface[l] = 0;
    CORBA::String_var g1 = interface;
    for (j = 0; j < l; j++)
	interface[j] = (char)inter[j];
    CORBA::Octet* oref = this->get_object_ref(ri, l);
    char* obj_ref = new char[l+1];
    obj_ref[l] = 0;
    CORBA::String_var g2 = obj_ref;
    for (j = 0; j < l; j++)
	obj_ref[j] = (char)oref[j];
    CORBA::Octet* init = this->get_audit_id(ri, l);
    char* initiator = new char[l+1];
    initiator[l] = 0;
    CORBA::String_var g3 = initiator;
    for (j = 0; j < l; j++)
	initiator[j] = (char)init[j];

    this->audit_analyse(Security::AuditInvocation, interface, obj_ref, s, initiator, (short)(0), CLIENT);
}

void
AuditClientInterceptor::_exec_principal_auth(const char* cred)
{
    S_self_->audit_analyse(Security::AuditPrincipalAuth, "","","_principalauth",(char *)cred, (short)1, CLIENT);
}

//
// Initializer
//
void
AuditClientInitializer::pre_init(PortableInterceptor::ORBInitInfo_ptr info)
{
    vector<string> vargs;
    CORBA::StringSeq* str_arg = info->arguments();
    for (CORBA::ULong i = 0; i < str_arg->length(); i++)
	vargs.push_back((*str_arg)[i].in());

    CORBA::ORB_var orb = CORBA::ORB_instance(info->orb_id(), FALSE);
	
    MICOGetOpt::OptMap opts; // Audit options
    opts["-AuditType"] = "arg-expected";
    opts["-AuditArchName"] = "arg-expected";
    MICOGetOpt opt_parser(opts);
    CORBA::Boolean r = opt_parser.parse(orb->rcfile(), TRUE);
    if (!r)
    	return;
    r = opt_parser.parse (vargs, TRUE);
    if (!r)
    	return;
    auditclient_options = opt_parser.opts();
    
    const MICOGetOpt::OptVec &o = auditclient_options;
    string type_str;
    string name_str;
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	const string &arg = (*i).first;
	if (arg == "-AuditType")
	    type_str = (*i).second;
	else if (arg == "-AuditArchName")
	    name_str = (*i).second;
    }
    if (type_str == "")
	return;
		
    CORBA::Object_var osecman = orb->resolve_initial_references ("SecurityManager");
    if (CORBA::is_nil(osecman))
	return;
    SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(osecman);
    if (CORBA::is_nil (secman))
	return;
    SecurityLevel2::AuditDecision_var audit_des = secman->audit_decision();
    r = audit_des->create((char*)type_str.c_str(), (char*)name_str.c_str());
    if (!r)
	return;
    AuditClientInterceptor * cicept = new AuditClientInterceptor;
    info->add_client_request_interceptor(cicept);
}

void
AuditClientInitializer::post_init(PortableInterceptor::ORBInitInfo_ptr info)
{
}
