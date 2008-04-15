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


#include <CORBA.h>
#include <mico/impl.h>
#include <mico/poa_impl.h>
#include <mico/security/securitylevel2_impl.h>
#include <mico/security/AuditIntercept.h>
#undef yyFlexLexer
#define yyFlexLexer rrFlexLexer
#include <FlexLexer.h>
#include <mico/security/RightsConfig.h>
#include <mico/security/odm_impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

// MICOSL2::Credentials_impl
//
// A Credentials object describes the security association between a client
// and a server
// It is used to retrieve the security attributes
// Get the security attributes describing the SSL association

namespace MICOSL2
{
    MICOGetOpt::OptVec acad_options;
    char MICO_defining_authority[] = "";
    MICOSL2::AttributeManager* S_attr_man = NULL;
    CORBA::Boolean paranoid = FALSE;
}

Security::SecAttribute
MICOSL2::Credentials_impl::get_SSL_attribute
(const Security::AttributeType attrtype)
{
    Security::SecAttribute return_attribute;
    CORBA::Any* a;

    assert (!CORBA::is_nil (target_principal_));

    //  target_principal describes the SSL-association between client
    //  and server
    //  get_attribute on this object is in ssl.cc and will contain
    //  the interface between the CORBASec attribute and SSL properties

    if ((attrtype.attribute_family.family == 1)&&(attrtype.attribute_type == 1)) {
	CORBA::Any* b = new CORBA::Any;
	*b <<= public_.c_str();
	a = b;
    }
    else {
	a = target_principal_->get_attribute(attrtype.attribute_family.family,
					     attrtype.attribute_type);
    }

    // Fill up the returned attribute object
    return_attribute.attribute_type = attrtype;
    const char *s;
    *a >>= s;
    int attr_len = strlen(s);
    attr_len++;  // Include NULL
    Security::Opaque value;
    value.length(attr_len);
    for (off_t i = 0; i < attr_len; i++) {
	value[i] = s[i];
    }
    delete a;
    Security::Opaque defining_authority;
    int defauth_len = strlen(MICO_defining_authority);
    defauth_len++;
    defining_authority.length(defauth_len);
    for (off_t i = 0; i < defauth_len; i++) {
	defining_authority[i] = MICO_defining_authority[i];
    }
    return_attribute.defining_authority = defining_authority;
    return_attribute.value = value;
    return return_attribute;
}


MICOSL2::Credentials_impl::~Credentials_impl()
{
    CORBA::release(target_principal_);
    CORBA::release(target_);
}


MICOSL2::Credentials_impl::Credentials_impl()
{
    public_ = "";
    cred_type_ = Security::SecOwnCredentials;
    auth_status_ = Security::SecAuthFailure;
    mech_ = 0;
    target_principal_ = NULL;
    target_ = NULL;
  
    accept_options_supported_ = Security::NoProtection|Security::NoDelegation;
    accept_options_required_ = Security::NoProtection|Security::NoDelegation;
    invoc_options_supported_ = Security::NoProtection|Security::NoDelegation;
    invoc_options_required_ = 0;
}


// Target or Received??
Security::CredentialsType
MICOSL2::Credentials_impl::credentials_type()
{
    return cred_type_;
}


void
MICOSL2::Credentials_impl::set_credentials_type(Security::CredentialsType type)
{
    cred_type_ = type;
}


void
MICOSL2::Credentials_impl::set_mechanism(const char* m)
{
    if (mech_)
	delete mech_;
    mech_ = CORBA::string_dup(m);
}


Security::AuthenticationStatus
MICOSL2::Credentials_impl::authentication_state()
{
    return auth_status_;
}


void
MICOSL2::Credentials_impl::set_authentication_state
(Security::AuthenticationStatus value)
{
    auth_status_ = value;
}


char*
MICOSL2::Credentials_impl::mechanism()
{
    return CORBA::string_dup(mech_);
}


Security::AssociationOptions
MICOSL2::Credentials_impl::accepting_options_supported()
{
    return accept_options_supported_;
}


void
MICOSL2::Credentials_impl::check_set_options(Security::AssociationOptions opts)
{
    if (opts != (opt_supported_ & opts))
	mico_throw(::CORBA::BAD_PARAM());
  
    if (0 != (opts & Security::NoProtection)) {
	if (0 != (opts & (Security::Integrity | Security::Confidentiality)))
	    mico_throw(::CORBA::BAD_PARAM());
    
	if (Security::NoDelegation != (opts &  Security::NoDelegation))
	    mico_throw(::CORBA::BAD_PARAM());
    }
}


void
MICOSL2::Credentials_impl::options_supported(Security::AssociationOptions value)
{
    opt_supported_ = value;
}


void
MICOSL2::Credentials_impl::accepting_options_supported(Security::AssociationOptions value)
{
    if ((accept_options_required_ == 0) && (value != 0))
	mico_throw(::CORBA::BAD_PARAM());

    this->check_set_options(value);
    accept_options_supported_ = value;
    accept_options_required_ = value & accept_options_required_;
}


Security::AssociationOptions
MICOSL2::Credentials_impl::accepting_options_required()
{
    return accept_options_required_;
}


void
MICOSL2::Credentials_impl::accepting_options_required
(Security::AssociationOptions value)
{
    if ((accept_options_supported_ > value)
	|| (accept_options_supported_ 
	    < Security::NoProtection|Security::NoDelegation))
	return;

    accept_options_required_ |= value;
}


Security::AssociationOptions
MICOSL2::Credentials_impl::invocation_options_supported()
{
    return invoc_options_supported_;
}


void
MICOSL2::Credentials_impl::invocation_options_supported
(Security::AssociationOptions value)
{
    if ((invoc_options_required_ == 0) && (value != 0))
	mico_throw(::CORBA::BAD_PARAM());

    this->check_set_options(value);
    invoc_options_supported_ = value;
    invoc_options_required_ = value & invoc_options_required_;

    if (this->credentials_type() != Security::SecOwnCredentials)
	features_.set_from_options(invoc_options_supported_);
}


Security::AssociationOptions
MICOSL2::Credentials_impl::invocation_options_required()
{
    return invoc_options_required_;
}


void
MICOSL2::Credentials_impl::invocation_options_required
(Security::AssociationOptions value)
{
    if ((invoc_options_supported_ > value)
	|| (invoc_options_supported_ < 0 /*Security::NoProtection|Security::NoDelegation*/))
	return;
  
    invoc_options_required_ |= value;
}


SecurityLevel2::Credentials_ptr
MICOSL2::Credentials_impl::copy()
{
    MICOSL2::Credentials_impl* cred = new MICOSL2::Credentials_impl;
    cred->target_principal_ = CORBA::Principal::_duplicate(target_principal_);
    cred->target_ =  CORBA::Object::_duplicate(target_);
    cred->cred_type_ = cred_type_;
    cred->auth_status_ = auth_status_;
    return cred;
}


void
MICOSL2::Credentials_impl::destroy()
{
    // maybe CORBA::release(this) is better here
    delete this;
}


CORBA::Boolean
MICOSL2::Credentials_impl::get_security_feature
(Security::CommunicationDirection direction,
 Security::SecurityFeature feature)
{
    if (direction != Security::SecDirectionBoth)
	mico_throw(::CORBA::BAD_PARAM());

    return features_.get_security_feature(feature);
}


// This is the main interface for user programs to get security attributes
Security::AttributeList*
MICOSL2::Credentials_impl::get_attributes
(const Security::AttributeTypeList& attributes)
{
    Security::AttributeList* result = new Security::AttributeList;
    assert(!CORBA::is_nil(target_principal_));
    Security::AttributeTypeList* attr_list;
    assert(S_attr_man != 0);
    attr_list = S_attr_man->filter(attributes);
    if (attr_list == 0)
	mico_throw(::CORBA::BAD_PARAM());

    int len = attr_list->length();
    //result -> length(len);
    int nlen = 0;

    for (off_t i = 0; i < len; i++) {
	//Security::SecAttribute att = (*result)[i];
	Security::AttributeType att_type = (*attr_list)[i];
	try {
	    Security::SecAttribute t_attr = this->get_SSL_attribute((*attr_list)[i]);
	    nlen++;
	    result->length(nlen);
	    (*result)[nlen - 1] = t_attr;
	} catch (...) {
	}
    }
    delete attr_list; 
    return result;
}


CORBA::Boolean
MICOSL2::Credentials_impl::set_attributes
(const Security::AttributeList& requested_attributes,
 Security::AttributeList_out actual_attributes)
{
    mico_throw (::CORBA::NO_IMPLEMENT());
    return FALSE;
}


CORBA::Boolean
MICOSL2::Credentials_impl::refresh
(const Security::Opaque& refresh_data)
{
    mico_throw (::CORBA::NO_IMPLEMENT ());
    return FALSE;
}


CORBA::Boolean
MICOSL2::Credentials_impl::is_valid(Security::UtcT& expiry_time)
{
    assert(!CORBA::is_nil(target_principal_));
    string v;
    CORBA::Boolean result = target_principal_->check(v);

    for (int i=0; i<10; i++)
	if ((v[i] > '9') || (v[i] < '0'))
	    return result;

    tm time;

    time.tm_year = (v[0]-'0')*10+(v[1]-'0');
    if (time.tm_year < 50)
	time.tm_year+=100;
  
    time.tm_mon= (v[2]-'0')*10+(v[3]-'0');
    if ((time.tm_mon > 12) || (time.tm_mon < 1))
	return result;

    time.tm_mday = (v[4]-'0')*10+(v[5]-'0');
    time.tm_hour = (v[6]-'0')*10+(v[7]-'0');
    time.tm_min=  (v[8]-'0')*10+(v[9]-'0');

    if ( (v[10] >= '0') && (v[10] <= '9') &&
	 (v[11] >= '0') && (v[11] <= '9'))
	time.tm_sec=  (v[10]-'0')*10+(v[11]-'0');

    time_t Time_time_t = mktime(&time);
    TimeBase::TimeT timeV = Time_time_t;
    TimeBase::TimeT tmp = 141427;
    tmp *= 86400;
    timeV += tmp;
    timeV *= 1000000;
    expiry_time.time =  timeV;
    TimeBase::TimeT ainaccurV = 10000000;
    expiry_time.inacclo = CORBA::UShort((ainaccurV <<32)>>32);
    expiry_time.inacchi = CORBA::UShort(ainaccurV >> 32) & 0xFFFF;
    expiry_time.tdf = 0;
    return result;
}


CORBA::Principal_ptr
MICOSL2::Credentials_impl::get_principal()
{
    return CORBA::Principal::_duplicate(target_principal_);
    //return target_principal_;
}


void
MICOSL2::Credentials_impl::set_principal(CORBA::Principal_ptr principal)
{
    if (!CORBA::is_nil(target_principal_))
	CORBA::release(target_principal_);
    target_principal_ = CORBA::Principal::_duplicate(principal);
}

//
// MICOSL2::ReceivedCredentials_impl
//
// Server Side Credential, inherited from Credentials
//
MICOSL2::ReceivedCredentials_impl::~ReceivedCredentials_impl()
{
}


MICOSL2::ReceivedCredentials_impl::ReceivedCredentials_impl(CORBA::ORB_ptr _orb)
{
    // There is only exactly one client invocing operations on the server,
    // so it is not necessary to distinguise different clients
    target_ = NULL;
    // Get the SSL-Principal
    // The principal is used to get the SSL-properties
    CORBA::Object_var p_obj = _orb->resolve_initial_references("PrincipalCurrent");
    CORBA::PrincipalCurrent_var p_current = CORBA::PrincipalCurrent::_narrow(p_obj);
    target_principal_ = p_current->get_principal();
    if (CORBA::is_nil(target_principal_))
  	mico_throw(CORBA::BAD_PARAM());
    cred_type_ = Security::SecReceivedCredentials;
    string probe;
    target_principal_->check(probe); // is it SSLPrincipal?
    if (probe == "")
	mico_throw(CORBA::BAD_PARAM());
    //We deal with SSL only
    MICOSSL::SSLTransport* transp 
	= dynamic_cast<MICOSSL::SSLTransport*>(target_principal_->transport());
    SSL* ssl = transp->get_ssl();
    if (!ssl)
	mico_throw(CORBA::BAD_PARAM());

    assoc_options_used_ =  Security::Integrity
	|Security::Confidentiality
	|Security::DetectReplay
	|Security::DetectMisordering
	|Security::NoDelegation;
  
    int verify_mode = ssl->verify_mode;
    if (SSL_VERIFY_PEER == verify_mode & SSL_VERIFY_PEER)
	assoc_options_used_ |= Security::EstablishTrustInClient;

    if ((SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT) == verify_mode
	& (SSL_VERIFY_PEER| SSL_VERIFY_FAIL_IF_NO_PEER_CERT))
	assoc_options_used_ |= Security::EstablishTrustInTarget;

    del_state_ = Security::SecInitiator;
    del_mode_ = Security::SecDelModeNoDelegation;
  
    accept_options_supported_ = 0;
    accept_options_required_ = 0;
    invoc_options_supported_ = 0;
    invoc_options_required_ = 0;
  
    CORBA::Object_var secman_obj = _orb->resolve_initial_references("SecurityManager");
    SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(secman_obj);
    assert(!CORBA::is_nil(secman));

    SecurityLevel2::CredentialsList_var own_cred = secman->own_credentials();
    //SecurityLevel2::CredentialsList* own_cred = secman->own_credentials();
    int len = own_cred->length();
    accept_cred_list_.length(len);
    for (CORBA::Long i = 0; i<len; i++) {
	accept_cred_list_[i] = own_cred[i]->copy();
	//accept_cred_list_[i] = (*own_cred)[i]->copy();
    }
}


SecurityLevel2::CredentialsList*
MICOSL2::ReceivedCredentials_impl::accepting_credentials()
{
    return new SecurityLevel2::CredentialsList(accept_cred_list_);
}


Security::AssociationOptions
MICOSL2::ReceivedCredentials_impl::association_options_used()
{
    return assoc_options_used_;
}

Security::DelegationState
MICOSL2::ReceivedCredentials_impl::delegation_state()
{
    return del_state_;
}


Security::DelegationMode
MICOSL2::ReceivedCredentials_impl::delegation_mode()
{
    return del_mode_;
}

//
// MICOSL2::TargetCredentials_impl
//
// Client Side Credential, inherited from Credentials

MICOSL2::TargetCredentials_impl::~TargetCredentials_impl()
{
}


MICOSL2::TargetCredentials_impl::TargetCredentials_impl
(CORBA::ORB_ptr _orb,
 CORBA::Object_ptr _target)
{
    // A client can call operations on many targets, there must be a
    // TargetCredentials object for each of these contexts
    target_ = CORBA::Object::_duplicate(_target);
    target_principal_ = _orb->get_principal(_target);
    assert(!CORBA::is_nil(target_principal_));
    cred_type_ = Security::SecTargetCredentials;

    //We deal with SSL only
    MICOSSL::SSLTransport* transp 
	= dynamic_cast<MICOSSL::SSLTransport*>(target_principal_->transport());
    SSL* ssl = transp->get_ssl();
    if (!ssl)
	mico_throw(CORBA::BAD_PARAM());

    assoc_options_used_ = Security::Integrity
	| Security::Confidentiality
	| Security::DetectReplay
	| Security::DetectMisordering
	| Security::NoDelegation;

    int verify_mode = ssl->verify_mode;
    if (SSL_VERIFY_PEER == verify_mode & SSL_VERIFY_PEER)
	assoc_options_used_ |= Security::EstablishTrustInClient;
  
    if ((SSL_VERIFY_PEER| SSL_VERIFY_FAIL_IF_NO_PEER_CERT) == verify_mode
	& (SSL_VERIFY_PEER| SSL_VERIFY_FAIL_IF_NO_PEER_CERT))
	assoc_options_used_ |= Security::EstablishTrustInTarget;

    accept_options_supported_ = 0;
    accept_options_required_ = 0;
    invoc_options_supported_ = 0;
    invoc_options_required_ = 0;
  
    CORBA::Object_var secman_obj = _orb->resolve_initial_references ("SecurityManager");
    SecurityLevel2::SecurityManager_var secman;
    secman = SecurityLevel2::SecurityManager::_narrow(secman_obj);
    assert (!CORBA::is_nil(secman));

    SecurityLevel2::CredentialsList_var own_cred = secman->own_credentials();
    //SecurityLevel2::CredentialsList* own_cred = secman->own_credentials();
    int len = own_cred->length();
    init_cred_list_.length(len);
    for (CORBA::Long i = 0; i<len; i++) {
	init_cred_list_[i] = own_cred[i]->copy();
	//init_cred_list_[i] = (*own_cred)[i]->copy();
    }
}


SecurityLevel2::CredentialsList*
MICOSL2::TargetCredentials_impl::initiating_credentials()
{
    return new SecurityLevel2::CredentialsList(init_cred_list_);
}


Security::AssociationOptions
MICOSL2::TargetCredentials_impl::association_options_used()
{
    return assoc_options_used_;
}

// AttributeManager provides attribute
// types management - storing, adding, validation, etc.
// It allows to operate with attribute types dynamically
// and is available for credentials and principals to get SecAttribute 
// by AttributeType

//
MICOSL2::AttributeManager::AttributeManager()
{
    p_main_list_ = new MICOSL2::AttributeTypeListList;
}


MICOSL2::AttributeManager::~AttributeManager()
{
    delete p_main_list_;
}


void
MICOSL2::AttributeManager::init()
{
    unsigned int family0[] = {
	Security::AuditId,
	Security::AccountingId,
	Security::NonRepudiationId
    };
  
    unsigned int family1[] = {
	Security::Public,
	Security::AccessId,
	Security::PrimaryGroupId,
	Security::GroupId,
	Security::Role,
	Security::AttributeSet,
	Security::Clearance,
	Security::Capability
    };

    unsigned int family10[] = {
	Security::X509Subject,
	Security::X509Issuer,
	Security::X509Cipher,
	Security::X509Subject_CN,
	Security::X509Subject_C,
	Security::X509Subject_L,
	Security::X509Subject_ST,
	Security::X509Subject_O,
	Security::X509Subject_OU,
	Security::X509Issuer_CN,
	Security::X509Issuer_C,
	Security::X509Issuer_L,
	Security::X509Issuer_ST,
	Security::X509Issuer_O,
	Security::X509Issuer_OU
    };

    unsigned int family11[] = {
	Security::AuthMethod,
	Security::PeerAddress
    };

    this->add_attr_types(0,3, family0);
    this->add_attr_types(1,8, family1);
    this->add_attr_types(10,15, family10);
    this->add_attr_types(11,2, family11);
}


CORBA::Long
MICOSL2::AttributeManager::add_empty_family
(const Security::ExtensibleFamily& family)
{
    // kcg: is this for something usefull? ie. why family param
    CORBA::Long len = p_main_list_->length();
    p_main_list_->length(len+1);
    (*p_main_list_)[len] = new Security::AttributeTypeList;
    return len;
}


void
MICOSL2::AttributeManager::add_attr_types
(const Security::AttributeTypeList& attr_type_list)
{
    CORBA::Long len = attr_type_list.length();
    for (CORBA::Long i = 0; i < len; i++)  {
	CORBA::Long fam_index = this->find_attr_type(attr_type_list[i]);
	if (fam_index != -1)
	    break; //if such attribute is already in list
	fam_index = this->find_family_index(attr_type_list[i].attribute_family);
	if (fam_index == -1)
	    fam_index = this->add_empty_family(attr_type_list[i].attribute_family);

	Security::AttributeTypeList& curr_attr_list = *((*p_main_list_)[fam_index]);
	int l = curr_attr_list.length();
	curr_attr_list.length(l+1);
	curr_attr_list[l] = attr_type_list[i];
    }
}


//the base method we need from Attribute Manager
Security::AttributeTypeList*
MICOSL2::AttributeManager::filter
(const Security::AttributeTypeList& attr_type_list)
{
    Security::AttributeTypeList* result_list;
    CORBA::Long result_i = 0;
    if (attr_type_list.length() == 0) {
	result_list = this->get_all_attr_types();
    }
    else {
	result_list = new Security::AttributeTypeList;
	result_list->length(0);
	for (CORBA::ULong i = 0; i < attr_type_list.length(); i++) {
	    CORBA::Long fam_index = this->find_attr_type(attr_type_list[i]);
	    if (fam_index == -1)
		continue;
	    result_list ->length(result_i+1);
	    (*result_list)[result_i] = attr_type_list[i];
	    result_i++;
	}
    }
    if (result_list->length() == 0)
	return NULL;
    return result_list;
}


Security::AttributeTypeList*
MICOSL2::AttributeManager::get_family_attr_types
(const Security::ExtensibleFamily& family)
{
    CORBA::Long fam_index = this->find_family_index(family);
    if (fam_index == -1)
	return 0; //hasn't such a family
    return new Security::AttributeTypeList(*(*p_main_list_)[fam_index]);
}


void
MICOSL2::AttributeManager::add_attr_types
(unsigned int family,
 int len,
 unsigned int* attributes)
{
    Security::AttributeTypeList attr_type_list;
    attr_type_list.length(len);
  
    Security::AttributeType arrt_type;
    Security::ExtensibleFamily fam;
  
    fam.family_definer = 0;
    fam.family = family;
    arrt_type.attribute_family = fam;
    for (int i = 0; i<len; i++) {
	arrt_type.attribute_type = attributes[i];
	attr_type_list[i] = arrt_type;
    }
    this->add_attr_types(attr_type_list);
}


CORBA::Long
MICOSL2::AttributeManager::find_attr_type
(const Security::AttributeType& attr_type)
{
    MICOSL2::AttributeTypeListList& main_list = *p_main_list_;

    //finding family
    CORBA::Long len = main_list.length();
    CORBA::Boolean exist = FALSE;
    CORBA::Long fam_index = 0;
    for (;fam_index < len; fam_index ++)
	if (main_list[fam_index] != 0) {
	    Security::AttributeType curr_attr_type = (*(main_list)[fam_index])[0];
	    if ((curr_attr_type.attribute_family.family == attr_type.attribute_family.family)) {
		exist = TRUE;
		break;
	    }
	}
    if (!exist)
	return -1;
  
    Security::AttributeTypeList& fam_list = *(main_list[fam_index]);
    for (CORBA::ULong type_index = 0; type_index < fam_list.length(); type_index++)
	if ((fam_list[type_index].attribute_family.family == attr_type.attribute_family.family)&&
	    (fam_list[type_index].attribute_type == attr_type.attribute_type))
	    return fam_index;
    return -1;
}

CORBA::Long
MICOSL2::AttributeManager::find_family_index(const Security::ExtensibleFamily& family)
{
    MICOSL2::AttributeTypeListList& main_list = *p_main_list_;

    CORBA::Long len = main_list.length();
    for (CORBA::Long i = 0; i<len; i++) {
	if (main_list[i])
	    if ((* main_list[i])[0].attribute_family.family == family.family)
		return i;
    }
    return -1;
}


Security::AttributeTypeList*
MICOSL2::AttributeManager::get_all_attr_types()
{
    MICOSL2::AttributeTypeListList& main_list = *p_main_list_;
    Security::AttributeTypeList* result_list = new Security::AttributeTypeList;
    CORBA::ULong len = 0;
    for (CORBA::ULong fam_index = 0; fam_index < main_list.length(); fam_index++) {
	CORBA::ULong add_len = (*main_list[fam_index]).length();
	result_list->length(len + add_len);
	for (CORBA::ULong attr_index = 0; attr_index < add_len; attr_index++)
	    (*result_list)[len + attr_index] = (*main_list[fam_index])[attr_index];
	len = len+add_len;
    }
    return result_list;
}


//policies
MICOSL2::MechanismPolicy_impl::MechanismPolicy_impl()
    : MICO::Policy_impl(Security::SecMechanismPolicy)
{
}


MICOSL2::MechanismPolicy_impl::MechanismPolicy_impl
(Security::MechanismTypeList* mech_list_in)
    : MICO::Policy_impl(Security::SecMechanismPolicy)
{
    CORBA::ULong len = mech_list_in->length();
    mechanisms_list_.length(len);
    for (CORBA::ULong i = 0; i<len; i++)
	mechanisms_list_[i] = CORBA::string_dup((*mech_list_in)[i]); //(*mech_list_in)[i];
}


MICOSL2::MechanismPolicy_impl::~MechanismPolicy_impl()
{
}


Security::MechanismTypeList*
MICOSL2::MechanismPolicy_impl::mechanisms()
{
    return new Security::MechanismTypeList(mechanisms_list_);
}


CORBA::Policy_ptr
MICOSL2::MechanismPolicy_impl::copy()
{
    MICOSL2::MechanismPolicy_impl* mp = new MICOSL2::MechanismPolicy_impl;
    CORBA::ULong len = mechanisms_list_.length();
    mp->mechanisms_list_.length(len);
    for (CORBA::ULong i = 0; i < len; i++)
	mp->mechanisms_list_[i] = CORBA::string_dup(mechanisms_list_[i]);
    return mp;
}


MICOSL2::InvocationCredentialsPolicy_impl::InvocationCredentialsPolicy_impl()
    : MICO::Policy_impl(Security::SecInvocationCredentialsPolicy)
{
}


MICOSL2::InvocationCredentialsPolicy_impl::InvocationCredentialsPolicy_impl
(SecurityLevel2::CredentialsList* cred_list_in)
    : MICO::Policy_impl(Security::SecInvocationCredentialsPolicy)
{
    CORBA::ULong len = cred_list_in->length();
    cred_list_.length(len);
    for (CORBA::ULong i = 0; i<len; i++)
	cred_list_[i] = ((*cred_list_in)[i])->copy();
}


SecurityLevel2::CredentialsList*
MICOSL2::InvocationCredentialsPolicy_impl::creds()
{
    return new SecurityLevel2::CredentialsList(cred_list_);
}


CORBA::Policy_ptr
MICOSL2::InvocationCredentialsPolicy_impl::copy()
{
    MICOSL2::InvocationCredentialsPolicy_impl* icp = new MICOSL2::InvocationCredentialsPolicy_impl;
    CORBA::ULong len = cred_list_.length();
    icp->cred_list_.length(len);
    for (CORBA::ULong i = 0; i < len; i++)
	icp->cred_list_[i] = cred_list_[i]->copy();
    return icp;
}


MICOSL2::QOPPolicy_impl::QOPPolicy_impl()
    : MICO::Policy_impl(Security::SecQOPPolicy)
{
    qop_ = Security::SecQOPNoProtection; //default
}


MICOSL2::QOPPolicy_impl::QOPPolicy_impl(Security::QOP qop)
    : MICO::Policy_impl(Security::SecQOPPolicy)
{
    qop_ = qop;
}


MICOSL2::QOPPolicy_impl::~QOPPolicy_impl()
{
}


Security::QOP
MICOSL2::QOPPolicy_impl::qop()
{
    return qop_;
}


CORBA::Policy_ptr
MICOSL2::QOPPolicy_impl::copy()
{
    MICOSL2::QOPPolicy_impl* qop = new MICOSL2::QOPPolicy_impl(qop_);
    return qop;
}


MICOSL2::DelegationDirectivePolicy_impl::DelegationDirectivePolicy_impl()
  : MICO::Policy_impl(Security::SecDelegationDirectivePolicy)
{
    //Delegate, NoDelegate
    deleg_mode_ = Security::NoDelegate;
}


MICOSL2::DelegationDirectivePolicy_impl::DelegationDirectivePolicy_impl
(Security::DelegationDirective deleg_mode_in)
    : MICO::Policy_impl(Security::SecDelegationDirectivePolicy)
{
    deleg_mode_ = deleg_mode_in;
}


MICOSL2::DelegationDirectivePolicy_impl::~DelegationDirectivePolicy_impl()
{
}


Security::DelegationDirective
MICOSL2::DelegationDirectivePolicy_impl::delegation_mode()
{
    return deleg_mode_;
}


CORBA::Policy_ptr
MICOSL2::DelegationDirectivePolicy_impl::copy()
{
    MICOSL2::DelegationDirectivePolicy_impl* ddp;
    ddp = new MICOSL2::DelegationDirectivePolicy_impl(deleg_mode_);
    return ddp;
}


MICOSL2::EstablishTrustPolicy_impl::EstablishTrustPolicy_impl()
  : MICO::Policy_impl(Security:: SecEstablishTrustPolicy)
{
    //default
    trust_.trust_in_client = TRUE;
    trust_.trust_in_target = FALSE;
}


MICOSL2::EstablishTrustPolicy_impl::EstablishTrustPolicy_impl
(Security::EstablishTrust trust)
    : MICO::Policy_impl(Security:: SecEstablishTrustPolicy)
{
    trust_ = trust;
}


MICOSL2::EstablishTrustPolicy_impl::~EstablishTrustPolicy_impl()
{
}


Security::EstablishTrust MICOSL2::EstablishTrustPolicy_impl::trust()
{
    return trust_;
}


CORBA::Policy_ptr
MICOSL2::EstablishTrustPolicy_impl::copy()
{
    MICOSL2::EstablishTrustPolicy_impl* etp = new MICOSL2::EstablishTrustPolicy_impl(trust_);
    return etp;
}


//MICOSL2::PolicyCurrent_impl
MICOSL2::PolicyCurrent_impl::PolicyCurrent_impl(CORBA::ORB_ptr _orb)
{
    orb_ = CORBA::ORB::_duplicate(_orb);
    this->create_default_policy_list();
    orb_->set_initial_reference ("PolicyCurrent", this);
}


MICOSL2::PolicyCurrent_impl::~PolicyCurrent_impl()
{
}


void
MICOSL2::PolicyCurrent_impl::create_default_policy_list()
{
    policies_.length(5);
    policies_[0] = new MICOSL2::EstablishTrustPolicy_impl;
    policies_[1] = new MICOSL2::DelegationDirectivePolicy_impl;
    policies_[2] = new MICOSL2::QOPPolicy_impl;
    // ###ras
    MICOSA::AuditClientPolicy_impl* pol = new MICOSA::AuditClientPolicy_impl;
    policies_[3] = pol;
    // ###ras
    policies_[4] = new MICOSA::AuditTargetPolicy_impl;
}


// ###ras: was Policy_ptr
CORBA::Boolean
MICOSL2::PolicyCurrent_impl::policy_type_allowed(const CORBA::Policy_var policy)
{
    if ((policy->policy_type() != Security::SecMechanismPolicy)
	&& (policy->policy_type() != Security::SecInvocationCredentialsPolicy)
	&& (policy->policy_type() != Security::SecQOPPolicy)
	&& (policy->policy_type() != Security::SecDelegationDirectivePolicy)
	&& (policy->policy_type() != Security::SecEstablishTrustPolicy))
	return FALSE;
    return TRUE;
}


void
MICOSL2::PolicyCurrent_impl::set_policy_overrides
(const CORBA::PolicyList& policies,
 CORBA::SetOverrideType override_type)
{
    if (override_type == CORBA::SET_OVERRIDE) {
	policies_.length(0);
	int len = policies.length();
	policies_.length(len);
	CORBA::Boolean done = FALSE;
	for (int i = 0; i < len; i++) {
	    // ###ras
	    if (this->policy_type_allowed(policies[i])) {
		policies_[i] = policies[i];
		done = TRUE;
		break;
	    }
	}
	if (!done)
	    mico_throw (::CORBA::INV_POLICY());
    }
    //ADD_OVERRIDE
    else{
	//int len = policies.length();
	for (CORBA::ULong i = 0; i < policies.length(); i++) {
	    CORBA::Boolean done = FALSE;
	    for (CORBA::ULong j = 0; j < policies_.length(); j++) {
		if (policies[i]->policy_type() == policies_[j]->policy_type()) {
		    //CORBA::Policy_ptr policy = policies_[j];
		    //delete policy;
		    // release is better
		    //CORBA::release(policy);
		    // ###ras
		    policies_[j] = policies[i];
		    done = TRUE;
		    break;
		}
	    }
	    if (!done) {
		if (!this->policy_type_allowed (policies[i]))
		    mico_throw (::CORBA::INV_POLICY());
		int len = policies_.length();
		policies_.length(len+1);
		// ###ras
		policies_[len] = policies[i];
	    }
	}
    }
}


CORBA::PolicyList*
MICOSL2::PolicyCurrent_impl::get_policy_overrides
(const CORBA::PolicyTypeSeq& policy_types)
{
    CORBA::PolicyList_var result_policies = new CORBA::PolicyList;
    if (policy_types.length() == 0) {
	int len = policies_.length();
	result_policies->length(len);
	for (CORBA::Long i = 0; i < len; i++)
	    result_policies[i] = policies_[i];
	return result_policies._retn();
    }
    for (CORBA::ULong i = 0; i < policy_types.length(); i++) {
	CORBA::Boolean done = FALSE;
	for (CORBA::ULong j = 0; j < policies_.length(); j++) {
	    if (policy_types[i] == policies_[j]->policy_type()) {
		CORBA::ULong len = result_policies->length();
		result_policies->length(len+1);
		result_policies[len] = policies_[j];
		done = TRUE;
		break;
	    }
	}
	if (!done)
	    mico_throw (::CORBA::INV_POLICY());
    }
    return result_policies._retn();
}

//
// MICOSL2::SecurityManager_impl
//
// is used to get the Credentials of Clients and Servers
// Servants use Current
MICOSL2::SecurityManager_impl::SecurityManager_impl(CORBA::ORB_ptr _orb)
{
    S_attr_man = new AttributeManager;
    attr_man_ = S_attr_man;
    S_attr_man->init();
    do {
	//mechanism list
	SSL_CTX* ctx = NULL;
	SSL* ssl = NULL;
	//char* ciphers = NULL;
	STACK_OF(SSL_CIPHER)* sk;
	char buf[512];

	OpenSSL_add_ssl_algorithms();
	ctx = SSL_CTX_new(SSLv23_method());
	if (ctx == NULL)
	    break;
  
	ssl = SSL_new(ctx);
	if (ssl == NULL)
	    break;

	sk = SSL_get_ciphers(ssl);
	char* result;
	Security::MechandOptions mech_opt;
	for (int i=0; i < sk_SSL_CIPHER_num(sk); i++) {
	    result = SSL_CIPHER_description(sk_SSL_CIPHER_value(sk,i), buf, 512);
	    if (!result)
		break;

	    (*strchr(result, ' '))= 0;

	    //MechanismType_var mechanism_type;
	    mech_opt.mechanism_type = CORBA::string_dup(result);
   
	    mech_opt.options_supported = Security::Integrity
		|Security::Confidentiality
		|Security::DetectReplay
		|Security::DetectMisordering
		|Security::EstablishTrustInTarget
		|Security::EstablishTrustInClient
		|Security::NoDelegation;

	    CORBA::ULong len = mech_list_.length();
	    mech_list_.length(len+1);
	    mech_list_[len] = mech_opt;
	}
	SSL_shutdown(ssl);
	SSL_free(ssl);
    } while(0);
 
    orb_ = CORBA::ORB::_duplicate(_orb);
    audit_des_ = new MICOSL2::AuditDecision_impl;
    const MICOGetOpt::OptVec &o = MICOSL2::acad_options;
    //  string adtype_str;
    //  string adname_str;
    string rconf_str;
    string par_str;
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	const string &arg = (*i).first;
	//    if (arg == "-AuditType")
	//      adtype_str = (*i).second;	 else
	//    if (arg == "-AuditArchName")
	//      adname_str = (*i).second;	 else
	if (arg == "-RightsConfig")
	    rconf_str = (*i).second;
	else
	    if (arg == "-Paranoid") {
		par_str = (*i).second;
		if (par_str == "yes" || par_str == "Yes" || par_str == "YES")
		    MICOSL2::paranoid = TRUE;
	    }
    }
 
    //  const char * type = adtype_str.c_str();
    //  const char * name = adname_str.c_str();
    //  if (adtype_str.length() != 0 && adname_str.length() != 0) {
    //    CORBA::Boolean audit= audit_des  -> create((char*)type, (char*)name);
    //    if (!audit)
    //      cout << "Can't initialize audit" << endl;
    //  }
    
    // for new mapping
    //rrights = new MICOSL2::RequiredRights_impl;
    rrights_ = new MICOSL2::AccessRights_impl;
    adecision_ = new MICOSL2::AccessDecision_impl;
    if (rconf_str.length() != 0) {
	CORBA::Boolean rr = rrights_->load_config_file(rconf_str.c_str());
	if (!rr)
	    cout << "Can't initialize Access Rights" << endl;
    }
	
    //  orb->set_initial_reference ("SecurityManager", this);
    prin_auth_.set_manager(this);
}


MICOSL2::AttributeManager*
MICOSL2::SecurityManager_impl::attribute_manager()
{
    // not CORBA object so w/o _duplicate
    return attr_man_;
}


MICOSL2::SecurityManager_impl::~SecurityManager_impl()
{
    // not CORBA object so w/o release here
    delete attr_man_;
    CORBA::release(audit_des_);
    CORBA::release(rrights_);
    CORBA::release(adecision_);
}


SecurityLevel2::PrincipalAuthenticator_ptr
MICOSL2::SecurityManager_impl::principal_authenticator()
{
    return SecurityLevel2::PrincipalAuthenticator::_duplicate(&prin_auth_);
}


CORBA::Any*
MICOSL2::SecurityManager_impl::get_method_data
(Security::AuthenticationMethod method)
{
    if (method == SecurityLevel2::KeyCertCAPass) {
	Security::SSLKeyCertCAPass str;
	any_ <<= str;
    }
    return new CORBA::Any(any_);
}


Security::MechandOptionsList*
MICOSL2::SecurityManager_impl::supported_mechanisms()
{
    return new Security::MechandOptionsList(mech_list_);
}


SecurityLevel2::AuditDecision_ptr
MICOSL2::SecurityManager_impl::audit_decision()
{
    return SecurityLevel2::AuditDecision::_duplicate(audit_des_);
}


// for new mapping
//SecurityLevel2::RequiredRights_ptr
//MICOSL2::SecurityManager_impl::required_rights() {
//	
//	return rrights;
//}

SecurityLevel2::AccessRights_ptr
MICOSL2::SecurityManager_impl::access_rights()
{
    return SecurityLevel2::AccessRights::_duplicate(rrights_);
}
	
SecurityLevel2::AccessDecision_ptr
MICOSL2::SecurityManager_impl::access_decision()
{
    return SecurityLevel2::AccessDecision::_duplicate(adecision_);
}


SecurityLevel2::CredentialsList*
MICOSL2::SecurityManager_impl::own_credentials()
{
    //cerr << "SecurityManager_impl::own_credentials() -> " << cred_list_.length() << endl;
    return new SecurityLevel2::CredentialsList(cred_list_);
    //return &cred_list_;
}


// Get the TargetCredentials in a client which are associated with the
// target object

SecurityLevel2::TargetCredentials_ptr
MICOSL2::SecurityManager_impl::get_target_credentials(CORBA::Object_ptr target)
{
    SecurityLevel2::TargetCredentials_ptr tc = new MICOSL2::TargetCredentials_impl
	(orb_, target);
    return tc;
}


void
MICOSL2::SecurityManager_impl::remove_own_credentials
(SecurityLevel2::Credentials_ptr creds)
{
    // kcg: looks strange to remove credentials from list by
    // deleting them and leave invalid pointer in the list
    for (CORBA::ULong i=0; i < cred_list_.length(); i++) {
	SecurityLevel2::Credentials_ptr own_cred = cred_list_[i];
	if (own_cred == creds)
	    delete own_cred;
    }
}


void
MICOSL2::SecurityManager_impl::set_own_credentials
(const SecurityLevel2::CredentialsList& creds)
{
    cred_list_ = creds;
}

CORBA::Policy_ptr
MICOSL2::SecurityManager_impl::get_security_policy
(CORBA::PolicyType policy_type)
{
    mico_throw(::CORBA::NO_IMPLEMENT ());
    return (CORBA::Policy_ptr)0;
}


//
// MICOSL2::Current_impl
//
// Like SecurityManager, but in Servants

MICOSL2::Current_impl::Current_impl(CORBA::ORB_ptr _orb)
    : MICOSL1::Current_impl(_orb)
{
    MICOSL2::S_attr_man = new AttributeManager;
    MICOSL2::S_attr_man->init();
    orb_ = CORBA::ORB::_duplicate(_orb);
    orb_->set_initial_reference("SecurityCurrent", this);
}


MICOSL2::Current_impl::~Current_impl()
{
    delete S_attr_man;
}


SecurityLevel2::TargetCredentials_ptr
MICOSL2::Current_impl::get_target_credentials(CORBA::Object_ptr target)
{
    assert(0);
    return NULL;
}


SecurityLevel2::ReceivedCredentials_ptr
MICOSL2::Current_impl::received_credentials()
{
    SecurityLevel2::ReceivedCredentials_ptr rc
	= new MICOSL2::ReceivedCredentials_impl(orb_);
    return rc;
}


MICOSL2::PrincipalAuthenticator_impl::PrincipalAuthenticator_impl()
{
    method_list_.length(1);
    method_list_[0] 
	= (Security::AuthenticationMethod)SecurityLevel2::KeyCertCAPass;
}


MICOSL2::PrincipalAuthenticator_impl::PrincipalAuthenticator_impl
(SecurityLevel2::SecurityManager* secman)
{
    secman_ = SecurityLevel2::SecurityManager::_duplicate(secman);
}


MICOSL2::PrincipalAuthenticator_impl::~PrincipalAuthenticator_impl()
{
}


void
MICOSL2::PrincipalAuthenticator_impl::set_manager
(SecurityLevel2::SecurityManager* secman)
{
    secman_ = SecurityLevel2::SecurityManager::_duplicate(secman);
}

//Orb calls this method to authenticate itself, load all the data and form own credentials

Security::AuthenticationStatus
MICOSL2::PrincipalAuthenticator_impl::authenticate
(Security::AuthenticationMethod method,
 const char* mechanism,
 const char* security_name,
 const CORBA::Any& auth_data,
 const Security::AttributeList& privileges,
 SecurityLevel2::Credentials_out creds,
 CORBA::Any_out continuation_data,
 CORBA::Any_out auth_specific_data)
{
    // kcg: strange method - it takes 3 out params but do set none!
    string security_name_str(security_name);
    Security::AuthenticationStatus status = Security::SecAuthFailure;
  
    if (security_name_str == "ssl") {
	if (method == SecurityLevel2::KeyCertCAPass) {
	    Security::SSLKeyCertCAPass method_struct;
	    auth_data >>= method_struct;
	    try {
		status = MICOSSL::SSLTransport::setup_ctx
		    ((char*)mechanism, method_struct)
		    ? Security::SecAuthSuccess : Security::SecAuthFailure;
	    }
	    catch (...) {
		mico_throw(CORBA::BAD_PARAM());
	    }
	    X509* peer = MICOSSL::SSLTransport::load_cert(method_struct.cert);

	    if (!peer)
		return Security::SecAuthFailure;

	    MICOSSL::SSLPrincipal* principal = new MICOSSL::SSLPrincipal(peer, mechanism);
      
	    MICOSL2::Credentials_impl* own_cred = new MICOSL2::Credentials_impl;
	    own_cred->set_principal(principal);
	    own_cred->set_credentials_type(Security::SecOwnCredentials);
	    own_cred->set_authentication_state(status);
	    own_cred->set_mechanism(mechanism);

	    Security::MechandOptionsList* p_mech_list = secman_->supported_mechanisms();
	    Security::MechandOptions mech_opt;
	    for (CORBA::ULong i = 0; i < p_mech_list->length(); i++) {
		mech_opt = (*p_mech_list)[i];
		if (strcmp(mech_opt.mechanism_type,mechanism)) {
		    own_cred->options_supported(mech_opt.options_supported);
		    own_cred->accepting_options_required(Security::SecNoDelegation|Security::SecNoProtection);
		    own_cred->accepting_options_supported(mech_opt.options_supported);
		    break;
		}
	    }
      
	    SecurityLevel2::CredentialsList_var p_cred_list = secman_->own_credentials();
	    CORBA::Long len = p_cred_list->length();
	    p_cred_list->length(len+1);
	    p_cred_list[len] = own_cred;
	    secman_->set_own_credentials(p_cred_list);

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
	    Security::AttributeList* al =  own_cred->get_attributes( atl );
	    CORBA::ULong l = (*al)[0].value.length();
	    CORBA::Octet* creds = new CORBA::Octet[l + 1];
	    creds[l] = 0;
	    for (CORBA::ULong i = 0; i < l; i++)
		creds[i] = (*al)[0].value[i];
	    delete al;
	    AuditServerInterceptor::_exec_principal_auth((const char *)creds);
	    delete[] creds;
	    //AuditClientInterceptor::_exec_principal_auth ((const char *)creds);
	}
    }
    //init InvocationCredPolicy and MechanizmPolicy from PolicyCurrent

    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", FALSE);
    CORBA::Object_var policy_current_obj = orb->resolve_initial_references("PolicyCurrent");
    SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
    assert (!CORBA::is_nil(policy_current));

    Security::MechanismTypeList* mech_list;
    mech_list = new Security::MechanismTypeList;
    Security::MechandOptionsList* p_mech_list = secman_->supported_mechanisms();
    int len = p_mech_list->length();
    mech_list->length(len);
    Security::MechandOptions mech_opt;
    for (int i=0; i < len; i++) {
	mech_opt = (*p_mech_list)[i];
	(*mech_list)[i] = CORBA::string_dup(mech_opt.mechanism_type);
    }
  
    CORBA::PolicyList policies;
    policies.length(2);
    policies[0] = new MICOSL2::MechanismPolicy_impl(mech_list);
    policies[1] = new MICOSL2::InvocationCredentialsPolicy_impl(secman_->own_credentials());
    policy_current->set_policy_overrides(policies, CORBA::ADD_OVERRIDE);
    return status;
}


Security::AuthenticationStatus
MICOSL2::PrincipalAuthenticator_impl::continue_authentication
(const CORBA::Any& response_data,
 SecurityLevel2::Credentials_ptr creds,
 CORBA::Any_out continuation_data,
 CORBA::Any_out auth_specific_data)
{
    mico_throw(::CORBA::NO_IMPLEMENT());
    return Security::SecAuthFailure;
}


Security::AuthenticationMethodList*
MICOSL2::PrincipalAuthenticator_impl::get_supported_authen_methods
(const char* mechanism)
{
    SSL_CTX* ctx = NULL;
    SSL* ssl = NULL;
    //char *ciphers = NULL;
    STACK_OF(SSL_CIPHER)* sk;
    char buf[512];
  
    OpenSSL_add_ssl_algorithms();
    ctx = SSL_CTX_new(SSLv23_method());
    if (ctx == NULL)
	mico_throw(CORBA::BAD_PARAM());
    ssl = SSL_new(ctx);
    if (ssl == NULL)
	mico_throw(CORBA::BAD_PARAM());
    sk = SSL_get_ciphers(ssl);
    char* result;
    for (int i = 0; i < sk_SSL_CIPHER_num(sk); i++) {
	result = SSL_CIPHER_description(sk_SSL_CIPHER_value(sk,i), buf, 512);
	if (!result)
	    break;
	(*strchr(result, ' '))= 0;
	if (strcmp(result, mechanism) == 0) {
	    SSL_shutdown(ssl);
	    SSL_free(ssl);
	    return new Security::AuthenticationMethodList(method_list_);
	}
    }
  
    SSL_shutdown(ssl);
    SSL_free(ssl);
    mico_throw(CORBA::BAD_PARAM());
    return NULL;
}


MICOSL2::SecurityFeatures::SecurityFeatures()
{
    len_ = 11;
    features_ = new CORBA::Boolean[len_];
    for (int i = 0; i<len_; i++)
	features_[i] = FALSE;
}


MICOSL2::SecurityFeatures::~SecurityFeatures()
{
    delete[] features_;
}


void
MICOSL2::SecurityFeatures::set_from_options(Security::AssociationOptions opt)
{
    features_[(int)Security::SecNoDelegation] = opt & Security::NoDelegation;
    features_[(int)Security::SecSimpleDelegation] = opt & Security::SimpleDelegation;
    features_[(int)Security::SecCompositeDelegation] = opt & Security::CompositeDelegation;
    features_[(int)Security::SecNoProtection] = opt & Security::NoProtection;
    features_[(int)Security::SecIntegrity] = opt & Security::Integrity;
    features_[(int)Security::SecConfidentiality] = opt & Security::Confidentiality;
    features_[(int)Security::SecIntegrityAndConfidentiality] = FALSE; //opt & Security::IntegrityAndConfidentiality;
    features_[(int)Security::SecDetectReplay] = opt & Security::DetectReplay;
    features_[(int)Security::SecDetectMisordering] = opt & Security::DetectMisordering;
    features_[(int)Security::SecEstablishTrustInTarget] = opt & Security::EstablishTrustInTarget;
    features_[(int)Security::SecEstablishTrustInClient] = opt & Security::EstablishTrustInClient;
}


CORBA::Boolean
MICOSL2::SecurityFeatures::get_security_feature
(Security::SecurityFeature sec_feature)
{
    if ((sec_feature < 0) || (sec_feature >= len_))
	mico_throw(CORBA::BAD_PARAM());
    return features_[sec_feature];
}


//
// Reguired Rights
//

// Rihgths record structure
// for new mapping
/*
MICOSL2::RightsRecord::RightsRecord( const RightsRecord& _s ) {

  opname = ((RightsRecord&)_s).opname;
  rightslist = ((RightsRecord&)_s).rightslist;
  combinator = ((RightsRecord&)_s).combinator;
}

MICOSL2::RightsRecord&
MICOSL2::RightsRecord::operator=( const RightsRecord& _s ) {

  opname = ((RightsRecord&)_s).opname;
  rightslist = ((RightsRecord&)_s).rightslist;
  combinator = ((RightsRecord&)_s).combinator;
  return *this;
}
*/
// Required rights
// for new mapping
/*
  MICOSL2::RequiredRights_impl::RequiredRights_impl() {
  
  }
  
  MICOSL2::RequiredRights_impl::~RequiredRights_impl() {
  
  }

void
MICOSL2::RequiredRights_impl::get_required_rights( CORBA::Object_ptr obj, const char* operation_name,
                     const char* interface_name, Security::RightsList*& rights,
                     Security::RightsCombinator& rights_combinator )
{
	RightsRecord * rec;
	string key;

	const char * iname = obj->_ior()->objid();
	if (iname == NULL || strlen(iname) == 0)
		key = interface_name;
	else
		key = iname;
	
	key += "~";
	key += operation_name;
	RightsMap::iterator it = rightsmap.find(key);
	if (it != rightsmap.end()) {
		rec = (*it).second;
		if (rec->opname == operation_name) {
			rights_combinator = rec->combinator;
			rights = new Security::RightsList(rec->rightslist);
		}
	}
}


void
MICOSL2::RequiredRights_impl::set_required_rights( const char* operation_name, const char* interface_name,
                     const Security::RightsList& rights,
                     Security::RightsCombinator rights_combinator )
{
	string key;
	
	key = interface_name;
	key += "~";
	key += operation_name;
	RightsRecord * rec = new RightsRecord;
	rec->rightslist = rights;
	rec->opname = CORBA::string_dup(operation_name);
	rec->combinator = rights_combinator;
	rightsmap[key] = rec;
}

CORBA::Boolean
MICOSL2::RequiredRights_impl::loadConfigFile( const char* filename ) {

	if (filename != NULL) {
		ifstream input(filename);
		if (!input.is_open())
			return FALSE;
		RightsConfig parser(&input);
		parser.startParse(this);
		if (parser.geterror() != 0)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}
*/


// for new mapping
MICOSL2::AccessRights_impl::AccessRights_impl()
{
}


MICOSL2::AccessRights_impl::~AccessRights_impl()
{
}


void
MICOSL2::AccessRights_impl::grant_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    Security::RightsList* irights;
	
    this->attribute_to_key(priv_attr); 
    SecAttrMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	irights = (*it).second;
	for (CORBA::ULong i = 0; i < rights.length(); i++) {
	    CORBA::Boolean f = TRUE;
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if (rights[i].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		    && rights[i].rights_family.family == (*irights)[j].rights_family.family) {
		    if (strcmp(rights[i].rights_list, (*irights)[j].rights_list) == 0) { // has it already
			f = FALSE;
			break;
		    }
		}
	    }
	    if (f) { // needs to be added
		int len = irights->length();
		irights->length(len + 1);
		(*irights)[len] = rights[i];
	    }
	}
    }
    else {
	atrmap_[key_] = new Security::RightsList(rights); // not found key, new
    }
}


void
MICOSL2::AccessRights_impl::revoke_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    Security::RightsList* irights;
    this->attribute_to_key(priv_attr); 
    SecAttrMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	irights = (*it).second;
	for (CORBA::ULong i = 0; i < rights.length(); i++) {
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if (rights[i].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		    && rights[i].rights_family.family == (*irights)[j].rights_family.family) {
		    if (strcmp(rights[i].rights_list, (*irights)[j].rights_list) == 0) {
			CORBA::ULong len = irights->length(); // remove it
			for (CORBA::ULong k = j; k < len - 1; k++) {
			    (*irights)[k] = (*irights)[k + 1];
			}
			irights->length(len -1);
			break;
		    }
		}
	    }
	}
    }
}


void
MICOSL2::AccessRights_impl::replace_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    //Security::RightsList* irights;
    this->attribute_to_key(priv_attr); 
    SecAttrMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	atrmap_.erase(it);
	atrmap_[key_] = new Security::RightsList(rights);
    }
}


Security::RightsList*
MICOSL2::AccessRights_impl::get_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::ExtensibleFamily& rights_family)
{
    Security::RightsList* retRights = new Security::RightsList;
    Security::RightsList* irights;
    this->attribute_to_key(priv_attr); 
    SecAttrMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) {
	irights = (*it).second;
	int len = retRights->length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    if ((*irights)[j].rights_family.family_definer == rights_family.family_definer
		&& (*irights)[j].rights_family.family == rights_family.family) {
		len++;
		retRights->length(len);
		(*retRights)[len - 1] = (*irights)[j];
	    }
	}
    }
    return retRights;
}


Security::RightsList*
MICOSL2::AccessRights_impl::get_all_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state)
{
    Security::RightsList* retRights = new Security::RightsList;
    Security::RightsList* irights;
    this->attribute_to_key(priv_attr); 
    SecAttrMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) {
	irights = (*it).second;
	int len = retRights->length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    len++;
	    retRights->length(len);
	    (*retRights)[len - 1] = (*irights)[j];
	}
    }
    return retRights;
}


Security::RightsList*
MICOSL2::AccessRights_impl::get_effective_rights
(const Security::AttributeList& attribute_list,
 const Security::ExtensibleFamily& rights_family)
{
    Security::RightsList* irights;
    Security::RightsList* retRights = new Security::RightsList;
  
    for (CORBA::ULong i = 0; i < attribute_list.length(); i++) {
	this->attribute_to_key(attribute_list[i]);
	SecAttrMap::iterator it = atrmap_.find(key_);
	if (it != atrmap_.end()) {
	    irights = (*it).second;
	    int len = retRights->length();
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if ((*irights)[i].rights_family.family_definer == rights_family.family_definer
		    && (*irights)[i].rights_family.family == rights_family.family) {
		    len++;
		    retRights->length(len);
		    (*retRights)[len - 1] = (*irights)[j];
		}
	    }
	}
    }
    return retRights;
}


Security::RightsList*
MICOSL2::AccessRights_impl::get_all_effective_rights
(const Security::AttributeList& attrib_list)
{
    Security::RightsList* irights;
    Security::RightsList* retRights = new Security::RightsList;
  
    for (CORBA::ULong i = 0; i < attrib_list.length(); i++) {
	this->attribute_to_key(attrib_list[i]); 
	SecAttrMap::iterator it = atrmap_.find(key_);
	if (it != atrmap_.end()) {
	    irights = (*it).second;
	    int len = retRights->length();
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		len++;
		retRights->length(len);
		(*retRights)[len - 1] = (*irights)[j];
	    }
	}
    }
    return retRights;
}


void
MICOSL2::AccessRights_impl::attribute_to_key
(const Security::SecAttribute& attr)
{
    char buf[512];
    CORBA::ULong i;
    key_ = "";
  
    sprintf(buf, "%d~", attr.attribute_type.attribute_family.family_definer);
    key_ += (char *)buf;
    sprintf(buf, "%d~", attr.attribute_type.attribute_family.family);
    key_ += (char *)buf;
    sprintf(buf, "%ld~", attr.attribute_type.attribute_type);
    key_ += (char *)buf;
    //for (i = 0; i < attr.defining_authority.length(); i++) {
    //	_key += attr.defining_authority[i];
    //}
    //_key += "~";
    for (i = 0; i < attr.value.length(); i++) {
	key_ += attr.value[i];
    }
}


CORBA::Boolean
MICOSL2::AccessRights_impl::load_config_file(const char* filename)
{
    if (filename != NULL) {
	std::ifstream input(filename);
	if (!input.is_open())
	    return FALSE;
	RightsConfig parser(&input);
	parser.startParse(this);
	if (parser.geterror() != 0)
	    return FALSE;
	return TRUE;
    }
    return FALSE;
}


// Access Decision
MICOSL2::AccessDecision_impl::AccessDecision_impl()
{
}


MICOSL2::AccessDecision_impl::~AccessDecision_impl()
{
}


CORBA::Boolean
MICOSL2::AccessDecision_impl::access_allowed
(const SecurityLevel2::CredentialsList& cred_list,
 CORBA::Object_ptr target,
 const char* operation_name,
 const char* target_interface_name)
{
    CORBA::ORB_var orb;
    SecurityLevel2::SecurityManager_var secman;
	
    orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var securitymanager = orb->resolve_initial_references ("SecurityManager");
    if (CORBA::is_nil (securitymanager))
	return FALSE; // for now
  
    secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
    if (CORBA::is_nil (secman))
	return FALSE; // for now
  
    // make a domain key
    // first part of our key
    SecurityLevel2::Credentials_var own_cred;
    //SecurityLevel2::CredentialsList* own_cred_list = secman->own_credentials();
    SecurityLevel2::CredentialsList_var own_cred_list = secman->own_credentials();
    if (own_cred_list->length() == 0)
  	return TRUE; // not secure server
    //own_cred = (*own_cred_list)[0];
    own_cred = SecurityLevel2::Credentials::_duplicate(own_cred_list[(CORBA::ULong)0]);
    //we can request own (our server) credentials attributes, part of the domain key
    Security::ExtensibleFamily fam1;
    fam1.family_definer = 0;
    fam1.family = 1;
    Security::AttributeType at1;
    at1.attribute_family = fam1;
    at1.attribute_type = Security::AccessId;
    Security::AttributeTypeList atl1;
    atl1.length(1);
    atl1[0]=at1;

    //getting  the attributes from the Credentials
    //Security::AttributeList_var al1 = own_cred->get_attributes(atl1);
    Security::AttributeList* al1 = own_cred->get_attributes(atl1);
    string key1 = "[";
    for (CORBA::ULong ctr = 0; ctr < (*al1).length(); ctr++) {
	key1 += (char *)(&(*al1)[ctr].value[0]);
    }
    delete al1;
    key1 += "] ";
    string key11 = key1;
    string keyforoot = key1;
    keyforoot += "/";
    string rootkey = "/";
    // second part of our key
    PortableServer::POA_var poa;
    try {
	CORBA::Object_var poao = orb->resolve_initial_references ("POACurrent");
	PortableServer::Current_var cpoa = PortableServer::Current::_narrow(poao);
	assert(!CORBA::is_nil(cpoa));
	poa = cpoa->get_POA();
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cerr << ex._repoid() << endl;
	assert(0);
    }
    PortableServer::POA_var np = poa; // ptr
    string key2;
    string key22;
    CORBA::String_var c_str = "";
    string tstr;
    while (!CORBA::is_nil(np)) {
	c_str = np->the_name();
	tstr = c_str.in();
	if (key2.length() > 0)
	    tstr += '/';
	tstr += key2;
	key2 = tstr;
	np = np->the_parent();
    }
    np = poa;
    while (!CORBA::is_nil(np)) {
	c_str = np->the_name();
	tstr = c_str.in();
	if (tstr == "RootPOA")
	    break;
	if (key22.length() > 0)
	    tstr += '/';
	tstr += key22;
	key22 = tstr;
	np = np->the_parent();
    }

    key1 += key2; // got a key
    key11 += key22;
	
    Security::Opaque okey;
    Security::Opaque okey2;
    Security::Opaque okey3;
    int len = key1.length();
    okey.length(len);
    for (int i = 0; i < len; i++)
	okey[i] = key1[i];
    int len2 = key11.length();
    okey2.length(len2);
    for (int i = 0; i < len2; i++)
	okey2[i] = key11[i];	
    int len3 = keyforoot.length();
    okey3.length(len3);
    for (int i = 0; i < len3; i++)
	okey3[i] = keyforoot[i];
    ObjectDomainMapping::Manager_var dmanager = poa->get_ODM();
    SecurityDomain::NameList_var list; // domain name list
    CORBA::Boolean rootflag = FALSE;
    PortableServer::LifespanPolicyValue polval = PortableServer::TRANSIENT;
    if (CORBA::is_nil(dmanager)) {  // we don't have domain manager
	// poa without ODM (probably)
	CORBA::Object_var objodm = orb->resolve_initial_references ("ODM");
	ObjectDomainMapping::ODM_var odm = ObjectDomainMapping::ODM::_narrow(objodm);
	MICOSODM::Factory_impl* factory = dynamic_cast<MICOSODM::Factory_impl*>
	    (odm->current());
	if (!factory)
	    // return FALSE;
	    return TRUE; // we didn't specify domain stuff for access control
    
	MICOSODM::DomainMap::iterator it = (factory->domains_->get_domain_map()).find(key2); //!
	if (it == (factory->domains_->get_domain_map()).end()) {
	    it = (factory->domains_->get_domain_map()).find(key22);
	    if (it == (factory->domains_->get_domain_map()).end()) {
		it = (factory->domains_->get_domain_map()).find(rootkey);
		if (it == (factory->domains_->get_domain_map()).end())
		    return FALSE;
		else
		    rootflag = TRUE;
	    }
	    else
		polval = PortableServer::PERSISTENT;
	}
	dmanager = ObjectDomainMapping::Manager::_duplicate
	    (((*it).second)->dm_);
    }
    //else
    //PortableServer::LifespanPolicyValue polval = MICOSODM::get_lspolicy((MICOSODM::Manager_impl *) dmanager);
    if (rootflag)
	list = dmanager->get_domain_names(okey3);
    else if (polval == PortableServer::TRANSIENT)
	list = dmanager->get_domain_names(okey);
    else
	list = dmanager->get_domain_names(okey2);

    // list contains appr. domain name list, go to get domain manager
    // first check do we have access domain
  
    SecurityDomain::Name nm;
    for (CORBA::ULong i = 0; i < list->length(); i++) {
	if (strcmp((list[i])[0].kind, "Access") == 0) {
	    nm = list[i];
	    break;
	}
    }
  
    if (nm.length() == 0) { // not mapped to Access Domain 
	return FALSE;
    }
    string fullname;
    for (CORBA::ULong i = 0; i < nm.length(); i++) {
	fullname += '/';
	fullname += nm[i].id;
    }
    /*	MICODebug::instance()->debugger()
     */

    // correct the name (exclude root name)
    int ln = nm.length();
    if (ln == 1) {
	nm.length(0); // mapped to root domain
    }
    else {
	for (int k = 0; k < ln -1; k++)
	    nm[k] = nm[k + 1];
	nm.length(ln - 1);
    }
    CORBA::Object_var factobj = orb->resolve_initial_references("DomainManagerFactory");
    if (CORBA::is_nil (factobj))
	return FALSE; // for now
    SecurityDomain::DomainManagerFactory_var dmfactory = SecurityDomain::DomainManagerFactory::_narrow(factobj);
    if (CORBA::is_nil (dmfactory))
	return FALSE; // for now

    SecurityDomain::DomainManagerAdmin_var dmrootobj = dmfactory->get_root_domain_manager("Access");
    if (CORBA::is_nil(dmrootobj))
	return FALSE; //!!
    SecurityDomain::DomainAuthority_var admroot = SecurityDomain::DomainAuthority::_narrow(dmrootobj);

    SecurityDomain::DomainManagerAdmin_var dm;
    if (nm.length() == 0) // root domain
	dm = SecurityDomain::DomainManagerAdmin::_narrow(admroot);
    else
	dm = admroot->get_domain_manager(nm); // got it
    // check return value
    if (CORBA::is_nil (dm))
	return FALSE; // domain stuff is broken, no such path, poa is registered with non-existent domain

    SecurityDomain::DomainAuthority_var adm = SecurityDomain::DomainAuthority::_narrow(dm);
    SecurityDomain::PolicyCombinator comb;
    try {
	comb = adm->get_policy_combinator(Security::SecTargetInvocationAccess);
    } catch (...) {
	return FALSE;
    }
    result_rights_.length(0);
    fl_ = TRUE;
    map_flag_ = TRUE;
    // call policy combinator
    // for new mapping
    //policy_combinator(cred_list, adm, comb);
    Security::RightsCombinator rcombinator;
    this->policy_combinator(target, operation_name, target_interface_name, adm, comb, rcombinator);
  
    //	MICODebug::instance()->debugger()
    //	<< "Access Decision:  Required Rights are  ";
    //	for (int i = 0; i < resultRights.length(); i++) {
    //		MICODebug::instance()->debugger() << resultRights[i].right;
    //		}
    //	MICODebug::instance()->debugger() << endl;
    // finished up combinig resultRights
    // get required_rights object
    // for new mapping
    //SecurityLevel2::RequiredRights_ptr rr = secman->required_rights();
    if (result_rights_.length() == 0) { // no rights required for this object!
	if (!map_flag_ && MICOSL2::paranoid)
	    return FALSE;
	else
	    return TRUE;
    }
    if (result_rights_.length() == 1
	&& strcmp(result_rights_[0].rights_list, "-") == 0) {
	return FALSE; // not accessable object
    }
    SecurityLevel2::AccessRights_var rr = secman->access_rights();
    // for new mapping
    //Security::RightsList * objrights = NULL;
    //Security::RightsList_var crights = new Security::RightsList; // access rights
    Security::RightsList* crights = new Security::RightsList; // access rights
    // for new mapping
    // rr->get_required_rights(target, operation_name, target_interface_name, objrights, rcombinator);
    //////////////////////////////////////////////
    Security::AttributeTypeList atl;
    atl.length(4);
    Security::ExtensibleFamily fam;
    fam.family_definer = 0;
    fam.family = 1;
    Security::AttributeType at;
    at.attribute_family = fam;
    at.attribute_type = Security::Public;
    atl[3] = atl[2] = atl[1] = atl[0] = at;
    atl[1].attribute_type = Security::AccessId;
    atl[2].attribute_type = Security::PrimaryGroupId;
    atl[3].attribute_type = Security::GroupId;
    Security::AttributeList_var attrib;
    for (CORBA::ULong i = 0; i < cred_list.length(); i++) {
	attrib = cred_list[i]->get_attributes(atl);
	Security::RightsList_var irights = rr->get_all_effective_rights(*attrib);
	int len = crights->length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    len++;
	    crights->length(len);
	    //crights[len - 1] = irights[j];
	    (*crights)[len - 1] = irights[j];
	}
    }
    /*
      MICODebug::instance()->debugger()
      << "Access Decision:  Attributes are" << endl;
      for (int i = 0; i < attrib->length(); i++) {
      MICODebug::instance()->debugger() << "family " << (*attrib)[i].attribute_type.attribute_family.family << endl;
      MICODebug::instance()->debugger() << "type " << (*attrib)[i].attribute_type.attribute_type << endl;
      MICODebug::instance()->debugger() << "value " << &(*attrib)[i].value[0] << endl;
      }
      MICODebug::instance()->debugger()
      << "Access Decision:  Effective Rights are  ";
      for (int i = 0; i < crights->length(); i++) {
      MICODebug::instance()->debugger() << (*crights)[i].right;
      }
      MICODebug::instance()->debugger() << endl;
    */
    // for new mapping
    /*
      if (objrights->length() == 0 || objrights == NULL)
      return TRUE; // no rights required! But in case "NULL" rights for object not specified at all
      // let's it be accessable by default
      
      // now we have resultRights & objectrights
      if (rcombinator == Security::SecAllRights)
      return check_all_rights(objrights);
      else if (rcombinator == Security::SecAnyRight)
      return check_any_rights(objrights);
    */	

    if (rcombinator == Security::SecAllRights)
	return this->check_all_rights(crights);
    else if (rcombinator == Security::SecAnyRight)
	return this->check_any_rights(crights);
  
    // impossible
    return FALSE;
}


void
MICOSL2::AccessDecision_impl::policy_combinator
(CORBA::Object_ptr obj,
 const char* operation_name,
 const char* interface_name,
 SecurityDomain::DomainManagerAdmin_ptr adm,
 SecurityDomain::PolicyCombinator comb,
 Security::RightsCombinator& rcomb)
{
    SecurityDomain::DomainAuthority_var dm = SecurityDomain::DomainAuthority::_narrow(adm);
    SecurityDomain::PolicyCombinator _comb;
    try {
	_comb = dm->get_policy_combinator(Security::SecTargetInvocationAccess);
    } catch (...) {
	return;
    }

    CORBA::Policy_var policy;
    try {
	policy = dm->get_domain_policy(Security::SecTargetInvocationAccess);
    } catch (...) {
	return;
    }

    SecurityAdmin::ObjectAccessPolicy_var dapolicy = SecurityAdmin::ObjectAccessPolicy::_narrow(policy);
    if (CORBA::is_nil (dapolicy))
	return;
  
    Security::RightsList* irights;
    dapolicy->get_required_rights(obj, operation_name, interface_name, irights, rcomb);
    if (irights == NULL) {
	irights = new Security::RightsList;
	map_flag_ = FALSE;
    }
    else
	map_flag_ = TRUE;
    if (fl_) { // we're at the bottom, first attempt
	int len = result_rights_.length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    len++;
	    result_rights_.length(len);
	    result_rights_[len - 1] = (*irights)[j];
	}
	if (_comb == SecurityDomain::FirstFit)
	    return;
    }
    else if (comb == SecurityDomain::Union)
	this->combin_union(irights);
    else if (comb == SecurityDomain::Intersection)
	this->combin_intersection(irights);
    else if (comb == SecurityDomain::Negation)
	this->combin_negation(irights);
    delete irights;
    fl_ = FALSE;
    SecurityDomain::DomainManagerAdminList_var parents = dm->get_parent_domain_managers();
    if (parents->length() == 0)
	return; // finish
	
    // one level up (we don't have overlapped domains)
    this->policy_combinator(obj, operation_name, interface_name, parents[(CORBA::ULong)0], _comb, rcomb);
}

// for new mapping
/*
void
MICOSL2::AccessDecision_impl::policy_combinator(const SecurityLevel2::CredentialsList& cred_list, SecurityDomain::DomainManagerAdmin_ptr adm, SecurityDomain::PolicyCombinator comb) {	

SecurityDomain::DomainAuthority_var dm = SecurityDomain::DomainAuthority::_narrow(adm);
//	if (dm->is_root())
//		return;
	
SecurityDomain::PolicyCombinator _comb;
try {
	_comb = dm->get_policy_combinator(Security::SecTargetInvocationAccess);
	} catch (...) {
		return;
	}

	CORBA::Policy_var policy;
	try {
	policy = dm->get_domain_policy(Security::SecTargetInvocationAccess);
	} catch (...) {
		return;
	}
	
	SecurityAdmin::DomainAccessPolicy_var dapolicy = SecurityAdmin::DomainAccessPolicy::_narrow(policy);
	if (CORBA::is_nil (dapolicy))
		return;
	
	
	Security::AttributeTypeList atl;
	atl.length(4);
	Security::ExtensibleFamily fam;
	fam.family_definer = 0;
	fam.family = 1;
	Security::AttributeType at;
	at.attribute_family = fam;
	at.attribute_type = Security::Public;
	atl[3] = atl[2] = atl[1] = atl[0] = at;
	atl[1].attribute_type = Security::AccessId;
	atl[2].attribute_type = Security::PrimaryGroupId;
	atl[3].attribute_type = Security::GroupId;
	
		
	for (int i = 0; i < cred_list.length(); i++) {
		Security::AttributeList_out attrib = cred_list[i]->get_attributes(atl);
		Security::RightsList * irights = dapolicy->get_all_effective_rights(*attrib);
		if (fl) {// we're at the bottom, first attempt
			int len = resultRights.length();
			for (int j = 0; j < irights->length(); j++) {
				len++;
				resultRights.length(len);
				resultRights[len - 1] = (*irights)[j];
			}
		}
		else if (comb == SecurityDomain::Union)
			combin_union(irights);
		else if (comb == SecurityDomain::Intersection)
			combin_intersection(irights);
		else if (comb == SecurityDomain::Negation)
			combin_negation(irights);
	}
	fl = FALSE;
	
	SecurityDomain::DomainManagerAdminList * parents = dm->get_parent_domain_managers();
	if (parents->length() == 0)
		return; // finish
	
	
	// one level up (we don't have overlapped domains)
	policy_combinator(cred_list, (*parents)[0], _comb);
	
}
*/

void
MICOSL2::AccessDecision_impl::combin_union(Security::RightsList* irights)
{
    for (CORBA::ULong j = 0; j < irights->length(); j++) {
	CORBA::Boolean f = TRUE;
	for (CORBA::ULong k = 0; k < result_rights_.length(); k++) {
	    if (result_rights_[k].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		&& result_rights_[k].rights_family.family == (*irights)[j].rights_family.family) {
		if (strcmp(result_rights_[k].rights_list, (*irights)[j].rights_list) == 0) { // has it already
		    f = FALSE;
		    break;
		}
	    }
	}
	if (f) { // needs to be added
	    int len = result_rights_.length();
	    result_rights_.length(len + 1);
	    result_rights_[len] = (*irights)[j];
	}
    }
}


void
MICOSL2::AccessDecision_impl::combin_intersection(Security::RightsList* irights)
{
    for (CORBA::ULong j = 0; j < irights->length(); j++) {
	CORBA::Boolean f = TRUE;
	for (CORBA::ULong k = 0; k < result_rights_.length(); k++) {
	    if (result_rights_[k].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		&& result_rights_[k].rights_family.family == (*irights)[j].rights_family.family) {
		if (strcmp(result_rights_[k].rights_list, (*irights)[j].rights_list) == 0) { // has it already
		    f = FALSE;
		    break;
		}
	    }
	}
	if (f) { // needs to be deleted
	    CORBA::ULong len = result_rights_.length();
	    for (CORBA::ULong i = j; i < len -1; i++) {
		result_rights_[i] = result_rights_[i + 1];
	    }
	    result_rights_.length(len -1); 
	}
    }
}


void
MICOSL2::AccessDecision_impl::combin_negation(Security::RightsList* irights)
{
    for (CORBA::ULong j = 0; j < irights->length(); j++) {
	CORBA::Boolean f = FALSE;
	for (CORBA::ULong k = 0; k < result_rights_.length(); k++) {
	    if (result_rights_[k].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		&& result_rights_[k].rights_family.family == (*irights)[j].rights_family.family) {
		if (strcmp(result_rights_[k].rights_list, (*irights)[j].rights_list) == 0) { // has it already
		    f = TRUE;
		    break;
		}
	    }
	}
	if (f) { // needs to be deleted
	    CORBA::ULong len = result_rights_.length();
	    for (CORBA::ULong i = j; i < len -1; i++) {
		result_rights_[i] = result_rights_[i + 1];
	    }
	    result_rights_.length(len -1); 
	}
    }
}


CORBA::Boolean
MICOSL2::AccessDecision_impl::check_all_rights(Security::RightsList* irights)
{
    if (irights->length() == 0) {
  	delete irights;
  	return FALSE;
    }

    for (CORBA::ULong j = 0; j < irights->length(); j++) {
	CORBA::Boolean match = FALSE;
	for (CORBA::ULong k = 0; k < result_rights_.length(); k++) {
	    if (result_rights_[k].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		&& result_rights_[k].rights_family.family == (*irights)[j].rights_family.family) {
		if (strcmp(result_rights_[k].rights_list, (*irights)[j].rights_list) == 0) { // match
		    match = TRUE;
		    break;
		}
	    }
	}
	if (!match) {
	    delete irights;
	    return FALSE;
	}
    }
    delete irights;
    return TRUE;
}


CORBA::Boolean
MICOSL2::AccessDecision_impl::check_any_rights(Security::RightsList* irights)
{
    for (CORBA::ULong j = 0; j < irights->length(); j++) {
	for (CORBA::ULong k = 0; k < result_rights_.length(); k++) {
	    if (result_rights_[k].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		&& result_rights_[k].rights_family.family == (*irights)[j].rights_family.family) {
		if (strcmp(result_rights_[k].rights_list, (*irights)[j].rights_list) == 0) { // match
		    delete irights;
		    return TRUE;
		}
	    }
	}
    }
    delete irights;
    return FALSE;
}


static class AuditRightsInit
    : public Interceptor::InitInterceptor
{
public:
    AuditRightsInit ()
        : Interceptor::InitInterceptor(0)
    {}
    Interceptor::Status
    initialize (CORBA::ORB_ptr orb, const char *orbid, int &argc, char *argv[])
    {
	MICOGetOpt::OptMap opts; // Audit, Required Rights
	// Audit options
	//    opts["-AuditType"] = "arg-expected";
	//    opts["-AuditArchName"] = "arg-expected";
	// Required Rights options
	opts["-RightsConfig"] = "arg-expected";
	// "Paranoid" option
	opts["-Paranoid"] = "arg-expected";
	MICOGetOpt opt_parser(opts);
	CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
	assert (r);
	r = opt_parser.parse (argc, argv, TRUE);
	assert (r);
	MICOSL2::acad_options = opt_parser.opts ();
    
	//    const MICOGetOpt::OptVec &o = acad_options;
	//    string type_str;
	//    string name_str;
	//    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	//      const string &arg = (*i).first;
	//      if (arg == "-AuditType")
	//	type_str = (*i).second;	 else
	//	  if (arg == "-AuditArchName")
	//	    name_str = (*i).second;
	//    }
	//    if (type_str == "") {
	//      AuditServerInterceptor::_exec_deactivate();
	//      AuditClientInterceptor::_exec_deactivate();
	//    }
	//    
	return Interceptor::INVOKE_CONTINUE;
    }
} _audit_rights_init;

void
MICOSL2::_init () {
  _audit_rights_init;
}
