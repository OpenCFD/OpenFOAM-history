// -*- c++ -*-
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

#ifndef __SECURITYLEVEL2_IMPL_H__
#define __SECURITYLEVEL2_IMPL_H__

#include <CORBA.h>
#include <mico/ssl.h>
#include <mico/intercept.h>
#include <mico/security/securitylevel1_impl.h>
#include <mico/security/audit_impl.h>

namespace MICOSL2
{
    extern void _init ();

    extern MICOGetOpt::OptVec acad_options;

    class Current_impl;
    class Credentials_impl;
    class ReceivedCredentials_impl;
    class TargetCredentials_impl;
    class SecurityManager_impl;
  
    class SecurityFeatures
    {
	CORBA::Boolean* features_;
	int len_;
    public:
	SecurityFeatures();
	~SecurityFeatures();

	void
	set_from_options(Security::AssociationOptions);

	CORBA::Boolean
	get_security_feature(Security::SecurityFeature feature);

	int
	length()
	{ return len_; }
    };

    class Credentials_impl
	: virtual public SecurityLevel2::Credentials
    {
    protected:
	CORBA::Principal_ptr target_principal_;
	CORBA::Object_ptr target_;
    
	Security::CredentialsType cred_type_;
	Security::AuthenticationStatus auth_status_;
	Security::MechanismType mech_;
    
	Security::AssociationOptions accept_options_supported_;
	Security::AssociationOptions accept_options_required_;
	Security::AssociationOptions invoc_options_supported_;
	Security::AssociationOptions invoc_options_required_;
    
	SecurityFeatures features_;
	Security::AssociationOptions opt_supported_;
	std::string public_;

	void
	check_set_options(Security::AssociationOptions opts);

	virtual Security::SecAttribute
	get_SSL_attribute(const Security::AttributeType attrtype);
    public:
	Credentials_impl();
	~Credentials_impl();
    
	virtual Security::CredentialsType
	credentials_type();

	virtual void
	set_credentials_type(Security::CredentialsType type);

	virtual Security::AuthenticationStatus
	authentication_state();

	virtual void
	set_authentication_state(Security::AuthenticationStatus value);

	virtual char*
	mechanism();

	virtual void
	set_mechanism(const char *);
    
	virtual Security::AssociationOptions
	accepting_options_supported();

	virtual void
	accepting_options_supported(Security::AssociationOptions value);

	virtual Security::AssociationOptions
	accepting_options_required();

	virtual void
	accepting_options_required(Security::AssociationOptions value);

	virtual Security::AssociationOptions
	invocation_options_supported();

	virtual void
	invocation_options_supported(Security::AssociationOptions value);

	virtual Security::AssociationOptions
	invocation_options_required();

	virtual void
	invocation_options_required(Security::AssociationOptions value);

	void
	options_supported(Security::AssociationOptions value); //to load options from mechanism

	virtual SecurityLevel2::Credentials_ptr
	copy();

	virtual void
	destroy();

	virtual CORBA::Boolean
	get_security_feature
	(Security::CommunicationDirection direction,
	 Security::SecurityFeature feature);

	virtual Security::AttributeList*
	get_attributes(const Security::AttributeTypeList& attributes);

	virtual CORBA::Boolean
	set_attributes
	(const Security::AttributeList& requested_attributes,
	 Security::AttributeList_out actual_attributes);

	virtual CORBA::Boolean
	refresh(const Security::Opaque& refresh_data);

	virtual CORBA::Boolean
	is_valid(Security::UtcT& expiry_time);

	CORBA::Principal_ptr
	get_principal();

	void
	set_principal(CORBA::Principal_ptr);
    };


    class ReceivedCredentials_impl
	: public SecurityLevel2::ReceivedCredentials,
	  public virtual MICOSL2::Credentials_impl
    {
    public:
	ReceivedCredentials_impl(CORBA::ORB_ptr _orb);
	~ReceivedCredentials_impl();

	virtual Security::AssociationOptions
	association_options_used();

	virtual Security::DelegationState
	delegation_state();

	virtual Security::DelegationMode
	delegation_mode();

	virtual SecurityLevel2::CredentialsList*
	accepting_credentials();
    private:
	SecurityLevel2::CredentialsList accept_cred_list_;
	Security::AssociationOptions  assoc_options_used_;
	Security::DelegationState del_state_;
	Security::DelegationMode del_mode_;
    };

    class TargetCredentials_impl
	: public SecurityLevel2::TargetCredentials,
	  public virtual MICOSL2::Credentials_impl
    {
    public:
	TargetCredentials_impl(CORBA::ORB_ptr _orb,CORBA::Object_ptr target);
	~TargetCredentials_impl();

	virtual Security::AssociationOptions
	association_options_used();

	virtual SecurityLevel2::CredentialsList*
	initiating_credentials();
    private:
	SecurityLevel2::CredentialsList init_cred_list_;
	Security::AssociationOptions assoc_options_used_;
	Security::DelegationState del_state_;
	Security::DelegationMode del_mode_;
    };

    class PrincipalAuthenticator_impl
	: public SecurityLevel2::PrincipalAuthenticator
    {
    public:
	PrincipalAuthenticator_impl();
	PrincipalAuthenticator_impl(SecurityLevel2::SecurityManager* secman);
	virtual ~PrincipalAuthenticator_impl();

	void
	set_manager(SecurityLevel2::SecurityManager* secman);
    
	virtual Security::AuthenticationStatus
	authenticate
	(Security::AuthenticationMethod method,
	 const char* mechanism,
	 const char* security_name,
	 const CORBA::Any& auth_data,
	 const Security::AttributeList& privileges,
	 SecurityLevel2::Credentials_out creds,
	 CORBA::Any_out continuation_data,
	 CORBA::Any_out auth_specific_data);

	virtual Security::AuthenticationStatus
	continue_authentication
	(const CORBA::Any& response_data,
	 SecurityLevel2::Credentials_ptr creds,
	 CORBA::Any_out continuation_data,
	 CORBA::Any_out auth_specific_data);
    
	virtual Security::AuthenticationMethodList*
	get_supported_authen_methods(const char* mechanism);
    
    private:
	SecurityLevel2::SecurityManager_var secman_;
	Security::AuthenticationMethodList method_list_;
    };

    typedef SequenceTmpl<Security::AttributeTypeList*,MICO_TID_DEF> AttributeTypeListList;

    class AttributeManager
    {
    public:
	AttributeManager();
	virtual ~AttributeManager();
    
	virtual void
	init();

	virtual void
	add_attr_types(const Security::AttributeTypeList& attr_type_list);

	void
	add_attr_types(unsigned int family, int len, unsigned int* attributes);

	virtual Security::AttributeTypeList*
	get_family_attr_types(const Security::ExtensibleFamily& family);

	Security::AttributeTypeList*
	get_all_attr_types();

	Security::AttributeTypeList*
	filter(const Security::AttributeTypeList& attr_type_list);
    
    private:
	MICOSL2::AttributeTypeListList* p_main_list_;

	CORBA::Long
	find_attr_type(const Security::AttributeType& attr_type);

	CORBA::Long
	find_family_index(const Security::ExtensibleFamily& family);

	CORBA::Long add_empty_family(const Security::ExtensibleFamily& family);
    };

    class MechanismPolicy_impl
	: public ::SecurityLevel2::MechanismPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	MechanismPolicy_impl();
	MechanismPolicy_impl(Security::MechanismTypeList* );
	virtual ~MechanismPolicy_impl();

	virtual Security::MechanismTypeList*
	mechanisms();
    
	CORBA::PolicyType
	policy_type()
	{ return Security::SecMechanismPolicy; }

	CORBA::Policy_ptr
	copy ();

    private:
	Security::MechanismTypeList mechanisms_list_;
    };


    class InvocationCredentialsPolicy_impl
	: public ::SecurityLevel2::InvocationCredentialsPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	InvocationCredentialsPolicy_impl();
	InvocationCredentialsPolicy_impl(SecurityLevel2::CredentialsList*);

	SecurityLevel2::CredentialsList*
	creds();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecInvocationCredentialsPolicy; }

	CORBA::Policy_ptr
	copy ();
    private:
	SecurityLevel2::CredentialsList cred_list_;
    };
  

    class QOPPolicy_impl
	: public ::SecurityLevel2::QOPPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	QOPPolicy_impl(Security::QOP qop);
	QOPPolicy_impl();
	virtual ~QOPPolicy_impl();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecQOPPolicy; }

	CORBA::Policy_ptr
	copy();

	Security::QOP
	qop();
    
    private:
	Security::QOP qop_;
    };

  
    class DelegationDirectivePolicy_impl
	: public ::SecurityLevel2::DelegationDirectivePolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	DelegationDirectivePolicy_impl();
	DelegationDirectivePolicy_impl(Security::DelegationDirective);
	virtual ~DelegationDirectivePolicy_impl();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecDelegationDirectivePolicy; }

	virtual Security::DelegationDirective
	delegation_mode();

	CORBA::Policy_ptr
	copy();

    private:
	Security::DelegationDirective deleg_mode_;
    };
  

    class EstablishTrustPolicy_impl
	: virtual public ::SecurityLevel2::EstablishTrustPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	EstablishTrustPolicy_impl();
	EstablishTrustPolicy_impl(Security::EstablishTrust);
	virtual ~EstablishTrustPolicy_impl();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecEstablishTrustPolicy; }

	Security::EstablishTrust
	trust();
    
	//CORBA::Boolean constr_policy (CORBA::Object_ptr object);
	CORBA::Policy_ptr
	copy ();
    
    private:
	Security::EstablishTrust trust_;
    };
  
    class PolicyCurrent_impl
	: public SecurityLevel2::PolicyCurrent
    {
	CORBA::ORB_ptr orb_;
    public:
	PolicyCurrent_impl();
	PolicyCurrent_impl(CORBA::ORB_ptr _orb);
	virtual ~PolicyCurrent_impl();

	virtual void
	set_policy_overrides
	(const CORBA::PolicyList& policies,
	 CORBA::SetOverrideType override_type);

	virtual CORBA::PolicyList*
	get_policy_overrides(const CORBA::PolicyTypeSeq& policy_types);
    private:
	CORBA::PolicyList policies_;

	CORBA::Boolean
	policy_type_allowed(const CORBA::Policy_var policy);

	void
	create_default_policy_list();
  };
  
  
    /////////////////////
    // Required Rights //
    /////////////////////
    // for new mapping
    /*
      struct RightsRecord {
      RightsRecord() {}
      ~RightsRecord() {}
      RightsRecord( const RightsRecord& s );
      RightsRecord& operator=( const RightsRecord& s );
      
      Security::RightsList rightslist;        // List of required rights
      string opname;                          // operation name
      Security::RightsCombinator combinator;  // Rights combinator
      };

      typedef map<string, RightsRecord *, less<string> > RightsMap;

      class RequiredRights_impl : virtual public SecurityLevel2::RequiredRights
      {
      public:
      RequiredRights_impl();
      ~RequiredRights_impl();
    
      virtual void get_required_rights( CORBA::Object_ptr obj, const char* operation_name, const char* interface_name, Security::RightsList*& rights, Security::RightsCombinator& rights_combinator );
      virtual void set_required_rights( const char* operation_name, const char* interface_name, const Security::RightsList& rights, Security::RightsCombinator rights_combinator );
      virtual CORBA::Boolean loadConfigFile(const char* filename);
      
      private:
      RightsMap rightsmap;
      };
    */
    // for new mapping

    typedef std::map<std::string, Security::RightsList *, std::less<std::string> > SecAttrMap;
  
    class AccessRights_impl
	: virtual public SecurityLevel2::AccessRights
    {
    public:
	AccessRights_impl();
	virtual ~AccessRights_impl();
    
	virtual void
	grant_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	virtual void
	revoke_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	virtual void
	replace_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	virtual Security::RightsList*
	get_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::ExtensibleFamily& rights_family);

	virtual Security::RightsList*
	get_all_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state);

	virtual Security::RightsList*
	get_effective_rights
	(const Security::AttributeList& attribute_list,
	 const Security::ExtensibleFamily& rights_family);

	virtual Security::RightsList*
	get_all_effective_rights
	(const Security::AttributeList& attrib_list);
    
	virtual CORBA::Boolean
	load_config_file(const char* filename);
    
    protected:
	std::string key_;
	SecAttrMap atrmap_;

	void
	attribute_to_key(const Security::SecAttribute& attr);
    };
  
  
    //
    // Access Decision
    //
    class AccessDecision_impl
	: virtual public SecurityLevel2::AccessDecision {
    public:
	AccessDecision_impl();
	virtual ~AccessDecision_impl();
    
	virtual CORBA::Boolean
	access_allowed
	(const SecurityLevel2::CredentialsList& cred_list,
	 CORBA::Object_ptr target,
	 const char* operation_name,
	 const char* target_interface_name);
    protected:
	// for new mapping
	void
	policy_combinator
	(CORBA::Object_ptr obj,
	 const char* operation_name,
	 const char* interface_name,
	 SecurityDomain::DomainManagerAdmin_ptr adm,
	 SecurityDomain::PolicyCombinator comb,
	 Security::RightsCombinator& rcomb);

	//  void policy_combinator(const SecurityLevel2::CredentialsList& cred_list,
	//	  					   SecurityDomain::DomainManagerAdmin_ptr dm,
	//	  					   SecurityDomain::PolicyCombinator comb);
	
	void
	combin_union(Security::RightsList* irights);

	void
	combin_intersection(Security::RightsList* irights);

	void
	combin_negation(Security::RightsList* irights);

	CORBA::Boolean
	check_all_rights(Security::RightsList* irights);

	CORBA::Boolean
	check_any_rights(Security::RightsList* irights);
  
    private:
	Security::RightsList result_rights_;
	CORBA::Boolean fl_;
	CORBA::Boolean map_flag_;
    };

    //
    // Security Manager
    //
    class SecurityManager_impl
	: public SecurityLevel2::SecurityManager
    {
    private:
	CORBA::ORB_ptr orb_;
	PrincipalAuthenticator_impl prin_auth_;
	CORBA::Any any_;
	SecurityLevel2::CredentialsList cred_list_;
	Security::MechandOptionsList mech_list_;
	MICOSL2::AttributeManager* attr_man_;
	MICOSL2::AuditDecision_impl* audit_des_;
	// for new mapping
	//MICOSL2::RequiredRights_impl * rrights;
	MICOSL2::AccessRights_impl* rrights_;
	MICOSL2::AccessDecision_impl* adecision_;
    public:
	SecurityManager_impl(CORBA::ORB_ptr _orb);
	SecurityManager_impl();
	virtual ~SecurityManager_impl();
  
	virtual Security::MechandOptionsList*
	supported_mechanisms();

	virtual SecurityLevel2::CredentialsList*
	own_credentials();

	virtual SecurityLevel2::AuditDecision_ptr
	audit_decision();

	virtual void
	remove_own_credentials(SecurityLevel2::Credentials_ptr creds);

	virtual void
	set_own_credentials(const SecurityLevel2::CredentialsList& creds);

	virtual SecurityLevel2::TargetCredentials_ptr
	get_target_credentials(CORBA::Object_ptr target);

	virtual CORBA::Policy_ptr
	get_security_policy(CORBA::PolicyType policy_type);

	virtual SecurityLevel2::PrincipalAuthenticator_ptr
	principal_authenticator();

	virtual CORBA::Any*
	get_method_data(Security::AuthenticationMethod method);

	MICOSL2::AttributeManager*
	attribute_manager();

	// for new mapping
	// SecurityLevel2::RequiredRights_ptr required_rights();

	virtual SecurityLevel2::AccessRights_ptr
	access_rights();

	virtual SecurityLevel2::AccessDecision_ptr
	access_decision();
    };


    class Current_impl
	: public SecurityLevel2::Current,
	  public virtual MICOSL1::Current_impl
    {
	CORBA::ORB_ptr orb_;
    public:
	Current_impl(CORBA::ORB_ptr _orb);
	Current_impl();  // Needed ??
	virtual ~Current_impl();

	SecurityLevel2::ReceivedCredentials_ptr
	received_credentials();

	//    Security::MechandOptionsList* supported_mechanisms();
	//    SecurityLevel2::CredentialsList* own_credentials();
	
	//    CORBA::Policy_ptr get_policy( CORBA::PolicyType policy_type );
	//    void remove_own_credentials(SecurityLevel2:: Credentials_ptr creds );

	SecurityLevel2::TargetCredentials_ptr
	get_target_credentials(CORBA::Object_ptr target);
    };
 
    extern char MICO_defining_authority[];
    extern MICOSL2::AttributeManager* S_attr_man;
    extern CORBA::Boolean paranoid;
 
} // MICOSL2

#endif // __SECURITYLEVEL2_IMPL_H__
