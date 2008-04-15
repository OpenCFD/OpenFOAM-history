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

static MICOGetOpt::OptVec auditserver_options;
AuditServerInterceptor* AuditServerInterceptor::S_self_;

void
AuditInterceptor::audit_analyse
(Security::EventType ev_type,
 const char* interface_name,
 const char* object_ref,
 const char* operation,
 const char* initiator,
 short s_f,
 CORBA::Octet c_s)
{
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var secman_obj = orb->resolve_initial_references ("SecurityManager");
    SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(secman_obj);
    if (CORBA::is_nil(secman))
  	return;
    SecurityLevel2::AuditDecision_var adecision = secman->audit_decision();
    SecurityLevel2::AuditChannel_var audit_channel = adecision->audit_channel();
    if (CORBA::is_nil(audit_channel))
	return;
  
    //Security::ExtensibleFamily event_family;
    Security::AuditEventType event_type;
    event_type.event_type = ev_type;
    event_type.event_family.family_definer = 0;
    event_type.event_family.family = 12;
    /*
      CORBA::Object_var policy_current_obj = orb->resolve_initial_references ("PolicyCurrent");
      SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
      assert (!CORBA::is_nil (policy_current));
      
      CORBA::PolicyTypeSeq policy_types;
      policy_types.length(1);
      policy_types[0] = Security::SecTargetInvocationAudit;
      
      CORBA::PolicyList * policies = policy_current -> get_policy_overrides(policy_types);
      CORBA::Policy_ptr policy = (*policies)[0];
      SecurityAdmin::AuditPolicy_ptr audit_policy = SecurityAdmin::AuditPolicy::_narrow(policy);
      
      Security::SelectorValueList* def_sel;
      Security::AuditCombinator audit_combinator;
    
      audit_policy -> get_audit_selectors("", event_type, def_sel, audit_combinator);
      
      Security::SelectorValueList& default_selectors = *def_sel;
      if (default_selectors.length() == 0)
      return;
      
      CORBA::Boolean write_escape = FALSE;
      CORBA::Boolean flag;
      CORBA::Boolean sf_presence = FALSE;
    */
    Security::SelectorValueList selectors; // to fill
    selectors.length(7);
    selectors[0].selector = Security::InterfaceName;
    selectors[0].value <<= interface_name;
    selectors[1].selector = Security::ObjectRef;
    selectors[1].value <<= object_ref;
    selectors[2].selector = Security::Operation;
    selectors[2].value <<= operation;
    selectors[3].selector = Security::Initiator;
    selectors[3].value <<= initiator;
    selectors[4].selector = Security::SuccessFailure;
    selectors[4].value <<= s_f;
    selectors[5].selector = Security::Time;
    time_t t = time(0);
#if defined (__FreeBSD__) && (__FreeBSD__ >= 5)
    selectors[5].value <<= (long int)t;
#else // (__FreeBSD__) && (__FreeBSD__ >= 5)
    selectors[5].value <<= t;
#endif // (__FreeBSD__) && (__FreeBSD__ >= 5)
    selectors[6].selector = Security::DayOfWeek;
    tm* time_struct = localtime(&t);
    selectors[6].value <<= (short)time_struct -> tm_wday;
    SecurityLevel2::CredentialsList creds;
    Security::Opaque event_specific_data;
    event_specific_data.length(1);
    event_specific_data[0] = c_s & (CORBA::Octet)4;
    Security::UtcT time;
    CORBA::Boolean yn = adecision->audit_needed(event_type, selectors);
    if (yn)
	audit_channel -> audit_write(event_type,creds,time,	selectors,event_specific_data);
}

void
AuditServerInterceptor::get_audit_id(CORBA::ULong& l)
{
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
    if (CORBA::is_nil(securitycurrent)) {
  	l = 0;
  	return;
    }
    SecurityLevel2::Current_var seccur = SecurityLevel2::Current::_narrow(securitycurrent);
    if (CORBA::is_nil(seccur)) {
  	l = 0;
  	return;
    }
    SecurityLevel2::ReceivedCredentials_var rc;
    try {
  	rc = seccur->received_credentials();
    } catch(...) {
  	l = 0;
  	return;
    }
  
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
    Security::AttributeList * al =  rc->get_attributes( atl );
    l = (CORBA::ULong)(*al)[0].value.length();
    ret_.length(l);
    for (CORBA::ULong i = 0; i < l; i++)
	ret_[i] = (*al)[0].value[i];
    delete al;
}

void
AuditServerInterceptor::get_object_ref(CORBA::ULong& l)
{
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var poao = orb->resolve_initial_references ("POACurrent");
    PortableServer::Current_var cpoa = PortableServer::Current::_narrow(poao);
    if (CORBA::is_nil(cpoa)) {
        // is it possible to have POACurrent nil? I don't think so
        assert(0);
        l = 0;
  	return;
    }
    CORBA::Object_var target;
    try {
	target = cpoa->get_reference();
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cerr << ex._repoid() << endl;
	assert(0);
    }
  
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
    ret_.length(l);
    for (CORBA::ULong i = 0; i < l; i++)
	ret_[i] = result[i];
}

void
AuditServerInterceptor::get_peer_address(CORBA::ULong& l)
{
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
    if (CORBA::is_nil(securitycurrent)) {
  	l = 0;
  	return;
    }
    SecurityLevel2::Current_var seccur = SecurityLevel2::Current::_narrow(securitycurrent);
    if (CORBA::is_nil(seccur)) {
  	l = 0;
  	return;
    }
    SecurityLevel2::ReceivedCredentials_var rc;
    try {
  	rc = seccur->received_credentials();
    } catch(...) {
  	l = 0;
  	return;
    }
    Security::ExtensibleFamily fam;
    fam.family_definer = 0;
    fam.family = 11;
    Security::AttributeType at;
    at.attribute_family = fam;
    at.attribute_type = Security::PeerAddress;
    Security::AttributeTypeList atl;
    atl.length(1);
    atl[0] = at;
    Security::AttributeList * al =  rc->get_attributes( atl );
    l = (CORBA::ULong)(*al)[0].value.length();
    ret_.length(l);
    for (CORBA::ULong i = 0; i < l; i++)
	ret_[i] = (*al)[0].value[i];
    delete al;
}

time_t
AuditServerInterceptor::timeT2epoch(TimeBase::TimeT timeV)
{
    time_t etime;
  
    //in 100 nano second steps
    timeV /= 1000000;
    //Offset from Unix Epoch to 15 October 1582
    TimeBase::TimeT tmp = 141427;
    tmp *= 86400;
    timeV -= tmp;
    etime = timeV;
    return etime;
}

TimeBase::TimeT AuditServerInterceptor::epoch2timeT(time_t Time_time_t)
{
    TimeBase::TimeT timeV = Time_time_t;
    TimeBase::TimeT tmp = 141427;
    tmp *= 86400;
    timeV += tmp;
    timeV *= 1000000;
    return timeV;
}

AuditServerInterceptor::AuditServerInterceptor(CORBA::Boolean acc)
{
    access_control_ = acc;
}

AuditServerInterceptor::~AuditServerInterceptor()
{
}

char*
AuditServerInterceptor::name()
{
    return CORBA::string_dup("AuditServer");
}

void
AuditServerInterceptor::receive_request_service_contexts
(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    CORBA::ULong l;
    this->get_peer_address(l);
    char* peer = new char[l+1];
    peer[l] = 0;
    CORBA::String_var g1 = peer;
    for (CORBA::ULong i = 0; i < l; i++)
	peer[i] = ret_[i];
    CORBA::String_var op = ri->operation();
    obj_reference_ = "";
    this->audit_analyse
	(Security::AuditSessionAuth, "", "", op, peer, (short)(1));
    //	cout << "rrsc " << op << endl;
}

void
AuditServerInterceptor::receive_request(PortableInterceptor::ServerRequestInfo_ptr ri)
{
    // Authorization

  CORBA::ORB_var orb;
  
  CORBA::String_var s = ri->operation();
  if (!access_control_)
    return;
  
  orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
  CORBA::Object_var securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
  if (CORBA::is_nil (securitycurrent))
    return;
  
  SecurityLevel2::Current_var current = SecurityLevel2::Current::_narrow(securitycurrent);
  if (CORBA::is_nil (current)) 
    return;
  
  CORBA::Object_var securitymanager = orb->resolve_initial_references ("SecurityManager");
  if (CORBA::is_nil (securitymanager))
    return;
  
  SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
  if (CORBA::is_nil (secman))
    return;

  SecurityLevel2::ReceivedCredentials_var rec_cred;
  SecurityLevel2::CredentialsList rec_cred_list;
  CORBA::Boolean rcfail = FALSE;
  try {
  	rec_cred = current->received_credentials();
  } catch (...) {
  	rcfail = TRUE;
  }
  if (!rcfail) {
	SecurityLevel2::Credentials_var cred = SecurityLevel2::Credentials::_narrow(rec_cred);
	rec_cred_list.length(1);
	rec_cred_list[0] = cred;
  }
  
  SecurityLevel2::AccessDecision_var access_des = secman->access_decision();

  CORBA::ULong j,l;
  char * ifc = ri->target_most_derived_interface();
  request_map_[ri->request_id()] = CORBA::string_dup(ifc);
  CORBA::Boolean yn = access_des->access_allowed(rec_cred_list, CORBA::Object::_nil(),
  												 s, (const char *)ifc);
  this->get_object_ref(l);
  obj_reference_ = "";
  for (j = 0; j < l; j++)
    obj_reference_ += (char)ret_[j];

  this->get_audit_id(l);
  initiator_ = "";
  for (j = 0; j < l; j++)
    initiator_ += (char)ret_[j];

  short s_f = yn ? (short)1: (short)0;
  
  this->audit_analyse (Security::AuditAuthorization, ifc, obj_reference_.c_str(), s, initiator_.c_str(), s_f);
  
  delete ifc;
//  cout << "rr " << s << endl;
  if (!yn)
  	mico_throw(CORBA::NO_PERMISSION());
  	
}

void
AuditServerInterceptor::send_reply( PortableInterceptor::ServerRequestInfo_ptr ri ) {
// Invocation  success

    CORBA::String_var s = ri->operation();
    CORBA::String_var ifc = "";
    if (request_map_.count(ri->request_id()) > 0) {
	RequestMapIterator i = request_map_.find(ri->request_id());
	ifc = (*i).second;
	request_map_.erase(i);
    }

    this->audit_analyse (Security::AuditInvocation, ifc, obj_reference_.c_str(), s, initiator_.c_str(), (short)1);
//  cout << "sr " << s << endl;

}

void
AuditServerInterceptor::send_exception( PortableInterceptor::ServerRequestInfo_ptr ri ) {
// Invocation failure

    CORBA::String_var s = ri->operation();
    CORBA::String_var ifc = "";
    if (request_map_.count(ri->request_id()) > 0) {
	RequestMapIterator i = request_map_.find(ri->request_id());
	ifc = (*i).second;
	request_map_.erase(i);
    }
    
    this->audit_analyse (Security::AuditInvocation, ifc, obj_reference_.c_str(), s, initiator_.c_str(), (short)0);
    //  cout << "se " << s << endl;

}

void
AuditServerInterceptor::send_other( PortableInterceptor::ServerRequestInfo_ptr ri )
{
    // Invocation failure
}

void
AuditServerInterceptor::_exec_principal_auth (const char *cred) {

  S_self_->audit_analyse (Security::AuditPrincipalAuth, "","","_principalauth",(char *)cred, (short)1);
}

///////////////////////////////////////////////////////////////////////////////////////////


void
AuditServerInitializer::pre_init(PortableInterceptor::ORBInitInfo_ptr info) {

	vector<string> vargs;
	CORBA::StringSeq * str_arg = info->arguments();
	for (CORBA::ULong i = 0; i < str_arg->length(); i++)
		vargs.push_back((*str_arg)[i].in());

	CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
	
    MICOGetOpt::OptMap opts; // Audit options
    opts["-AuditType"] = "arg-expected";
    opts["-AuditArchName"] = "arg-expected";
    opts["-AccessControl"] = "arg-expected";
    MICOGetOpt opt_parser(opts);
    CORBA::Boolean r = opt_parser.parse(orb->rcfile(), TRUE);
    if (!r)
    	return;
    r = opt_parser.parse (vargs, TRUE);
    if (!r)
    	return;
    auditserver_options = opt_parser.opts();
    
    const MICOGetOpt::OptVec &o = auditserver_options;
    string type_str;
    string name_str;
    string as_str;
    CORBA::Boolean enable_audit = FALSE;
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
      const string &arg = (*i).first;
      if (arg == "-AuditType") {
	  enable_audit = TRUE;
	  type_str = (*i).second;
      } else if (arg == "-AuditArchName") {
	  enable_audit = TRUE;
	  name_str = (*i).second;
      } else if (arg == "-AccessControl") {
	  enable_audit = TRUE;
	  as_str = (*i).second;
      }
    }
		
	CORBA::Object_var osecman = orb->resolve_initial_references ("SecurityManager");
	if (CORBA::is_nil(osecman))
		return;
	SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(osecman);
	if (CORBA::is_nil (secman))
		return;
	CORBA::Boolean access_control;
	if (as_str == "no" || as_str == "off" || as_str == "No" || as_str == "NO" ||
		as_str == "Off" || as_str == "OFF")
	    access_control = FALSE;
	  else
	    access_control = TRUE;
	if (enable_audit) {
	    SecurityLevel2::AuditDecision_var audit_des = secman->audit_decision();
	    r = audit_des->create((char*)type_str.c_str(), (char*)name_str.c_str());
	    AuditServerInterceptor * sicept = new AuditServerInterceptor(access_control);
	    info->add_server_request_interceptor(sicept);
	}
}

void
AuditServerInitializer::post_init(PortableInterceptor::ORBInitInfo_ptr info) {
// nothing
}

///////////////////////////////////////////////////////////////////////////////////
