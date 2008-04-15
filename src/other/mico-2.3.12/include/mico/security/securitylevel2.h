/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __SECURITYLEVEL2_H__
#define __SECURITYLEVEL2_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace SecurityLevel2
{

class Credentials;
typedef Credentials *Credentials_ptr;
typedef Credentials_ptr CredentialsRef;
typedef ObjVar< Credentials > Credentials_var;
typedef ObjOut< Credentials > Credentials_out;

class ReceivedCredentials;
typedef ReceivedCredentials *ReceivedCredentials_ptr;
typedef ReceivedCredentials_ptr ReceivedCredentialsRef;
typedef ObjVar< ReceivedCredentials > ReceivedCredentials_var;
typedef ObjOut< ReceivedCredentials > ReceivedCredentials_out;

class TargetCredentials;
typedef TargetCredentials *TargetCredentials_ptr;
typedef TargetCredentials_ptr TargetCredentialsRef;
typedef ObjVar< TargetCredentials > TargetCredentials_var;
typedef ObjOut< TargetCredentials > TargetCredentials_out;

class Current;
typedef Current *Current_ptr;
typedef Current_ptr CurrentRef;
typedef ObjVar< Current > Current_var;
typedef ObjOut< Current > Current_out;

class AuditChannel;
typedef AuditChannel *AuditChannel_ptr;
typedef AuditChannel_ptr AuditChannelRef;
typedef ObjVar< AuditChannel > AuditChannel_var;
typedef ObjOut< AuditChannel > AuditChannel_out;

class AuditDecision;
typedef AuditDecision *AuditDecision_ptr;
typedef AuditDecision_ptr AuditDecisionRef;
typedef ObjVar< AuditDecision > AuditDecision_var;
typedef ObjOut< AuditDecision > AuditDecision_out;

class AccessDecision;
typedef AccessDecision *AccessDecision_ptr;
typedef AccessDecision_ptr AccessDecisionRef;
typedef ObjVar< AccessDecision > AccessDecision_var;
typedef ObjOut< AccessDecision > AccessDecision_out;

class RequiredRights;
typedef RequiredRights *RequiredRights_ptr;
typedef RequiredRights_ptr RequiredRightsRef;
typedef ObjVar< RequiredRights > RequiredRights_var;
typedef ObjOut< RequiredRights > RequiredRights_out;

class AccessRights;
typedef AccessRights *AccessRights_ptr;
typedef AccessRights_ptr AccessRightsRef;
typedef ObjVar< AccessRights > AccessRights_var;
typedef ObjOut< AccessRights > AccessRights_out;

class PrincipalAuthenticator;
typedef PrincipalAuthenticator *PrincipalAuthenticator_ptr;
typedef PrincipalAuthenticator_ptr PrincipalAuthenticatorRef;
typedef ObjVar< PrincipalAuthenticator > PrincipalAuthenticator_var;
typedef ObjOut< PrincipalAuthenticator > PrincipalAuthenticator_out;

class PolicyCurrent;
typedef PolicyCurrent *PolicyCurrent_ptr;
typedef PolicyCurrent_ptr PolicyCurrentRef;
typedef ObjVar< PolicyCurrent > PolicyCurrent_var;
typedef ObjOut< PolicyCurrent > PolicyCurrent_out;

class MechanismPolicy;
typedef MechanismPolicy *MechanismPolicy_ptr;
typedef MechanismPolicy_ptr MechanismPolicyRef;
typedef ObjVar< MechanismPolicy > MechanismPolicy_var;
typedef ObjOut< MechanismPolicy > MechanismPolicy_out;

class InvocationCredentialsPolicy;
typedef InvocationCredentialsPolicy *InvocationCredentialsPolicy_ptr;
typedef InvocationCredentialsPolicy_ptr InvocationCredentialsPolicyRef;
typedef ObjVar< InvocationCredentialsPolicy > InvocationCredentialsPolicy_var;
typedef ObjOut< InvocationCredentialsPolicy > InvocationCredentialsPolicy_out;

class QOPPolicy;
typedef QOPPolicy *QOPPolicy_ptr;
typedef QOPPolicy_ptr QOPPolicyRef;
typedef ObjVar< QOPPolicy > QOPPolicy_var;
typedef ObjOut< QOPPolicy > QOPPolicy_out;

class DelegationDirectivePolicy;
typedef DelegationDirectivePolicy *DelegationDirectivePolicy_ptr;
typedef DelegationDirectivePolicy_ptr DelegationDirectivePolicyRef;
typedef ObjVar< DelegationDirectivePolicy > DelegationDirectivePolicy_var;
typedef ObjOut< DelegationDirectivePolicy > DelegationDirectivePolicy_out;

class EstablishTrustPolicy;
typedef EstablishTrustPolicy *EstablishTrustPolicy_ptr;
typedef EstablishTrustPolicy_ptr EstablishTrustPolicyRef;
typedef ObjVar< EstablishTrustPolicy > EstablishTrustPolicy_var;
typedef ObjOut< EstablishTrustPolicy > EstablishTrustPolicy_out;

class SecurityManager;
typedef SecurityManager *SecurityManager_ptr;
typedef SecurityManager_ptr SecurityManagerRef;
typedef ObjVar< SecurityManager > SecurityManager_var;
typedef ObjOut< SecurityManager > SecurityManager_out;

}






namespace SecurityLevel2
{


/*
 * Base class and common definitions for interface Credentials
 */

class Credentials : 
  virtual public CORBA::Object
{
  public:
    virtual ~Credentials();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Credentials_ptr _ptr_type;
    typedef Credentials_var _var_type;
    #endif

    static Credentials_ptr _narrow( CORBA::Object_ptr obj );
    static Credentials_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Credentials_ptr _duplicate( Credentials_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Credentials_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::CredentialsType credentials_type() = 0;
    virtual ::Security::AuthenticationStatus authentication_state() = 0;
    virtual char* mechanism() = 0;
    virtual ::Security::AssociationOptions accepting_options_supported() = 0;
    virtual void accepting_options_supported( ::Security::AssociationOptions value ) = 0;
    virtual ::Security::AssociationOptions accepting_options_required() = 0;
    virtual void accepting_options_required( ::Security::AssociationOptions value ) = 0;
    virtual ::Security::AssociationOptions invocation_options_supported() = 0;
    virtual void invocation_options_supported( ::Security::AssociationOptions value ) = 0;
    virtual ::Security::AssociationOptions invocation_options_required() = 0;
    virtual void invocation_options_required( ::Security::AssociationOptions value ) = 0;

    virtual Credentials_ptr copy() = 0;
    virtual void destroy() = 0;
    virtual CORBA::Boolean get_security_feature( ::Security::CommunicationDirection direction, ::Security::SecurityFeature feature ) = 0;
    virtual ::Security::AttributeList* get_attributes( const ::Security::AttributeTypeList& attributes ) = 0;
    virtual CORBA::Boolean set_attributes( const ::Security::AttributeList& requested_attributes, ::Security::AttributeList_out actual_attributes ) = 0;
    virtual CORBA::Boolean is_valid( ::Security::UtcT_out expiry_time ) = 0;
    virtual CORBA::Boolean refresh( const ::Security::Opaque& refresh_data ) = 0;

  protected:
    Credentials() {};
  private:
    Credentials( const Credentials& );
    void operator=( const Credentials& );
};



/*
 * Base class and common definitions for interface Current
 */

class Current : 
  virtual public ::SecurityLevel1::Current
{
  public:
    virtual ~Current();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Current_ptr _ptr_type;
    typedef Current_var _var_type;
    #endif

    static Current_ptr _narrow( CORBA::Object_ptr obj );
    static Current_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Current_ptr _duplicate( Current_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Current_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::SecurityLevel2::ReceivedCredentials_ptr received_credentials() = 0;

    virtual TargetCredentials_ptr get_target_credentials( CORBA::Object_ptr target ) = 0;

  protected:
    Current() {};
  private:
    Current( const Current& );
    void operator=( const Current& );
};



/*
 * Base class and common definitions for interface AuditDecision
 */

class AuditDecision : 
  virtual public CORBA::Object
{
  public:
    virtual ~AuditDecision();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AuditDecision_ptr _ptr_type;
    typedef AuditDecision_var _var_type;
    #endif

    static AuditDecision_ptr _narrow( CORBA::Object_ptr obj );
    static AuditDecision_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AuditDecision_ptr _duplicate( AuditDecision_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AuditDecision_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::SecurityLevel2::AuditChannel_ptr audit_channel() = 0;

    virtual CORBA::Boolean audit_needed( const ::Security::AuditEventType& event_type, const ::Security::SelectorValueList& value_list ) = 0;
    virtual CORBA::Boolean create( const char* arch_type, const char* arch_name ) = 0;

  protected:
    AuditDecision() {};
  private:
    AuditDecision( const AuditDecision& );
    void operator=( const AuditDecision& );
};


typedef IfaceSequenceTmpl< Credentials_var,Credentials_ptr> CredentialsList;
typedef TSeqVar< IfaceSequenceTmpl< Credentials_var,Credentials_ptr> > CredentialsList_var;
typedef TSeqOut< IfaceSequenceTmpl< Credentials_var,Credentials_ptr> > CredentialsList_out;


/*
 * Base class and common definitions for interface AccessDecision
 */

class AccessDecision : 
  virtual public CORBA::Object
{
  public:
    virtual ~AccessDecision();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AccessDecision_ptr _ptr_type;
    typedef AccessDecision_var _var_type;
    #endif

    static AccessDecision_ptr _narrow( CORBA::Object_ptr obj );
    static AccessDecision_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AccessDecision_ptr _duplicate( AccessDecision_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AccessDecision_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Boolean access_allowed( const CredentialsList& cred_list, CORBA::Object_ptr target, const char* operation_name, const char* target_interface_name ) = 0;

  protected:
    AccessDecision() {};
  private:
    AccessDecision( const AccessDecision& );
    void operator=( const AccessDecision& );
};



/*
 * Base class and common definitions for interface RequiredRights
 */

class RequiredRights : 
  virtual public CORBA::Object
{
  public:
    virtual ~RequiredRights();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef RequiredRights_ptr _ptr_type;
    typedef RequiredRights_var _var_type;
    #endif

    static RequiredRights_ptr _narrow( CORBA::Object_ptr obj );
    static RequiredRights_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static RequiredRights_ptr _duplicate( RequiredRights_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static RequiredRights_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void get_required_rights( CORBA::Object_ptr obj, const char* operation_name, const char* interface_name, ::Security::RightsList_out rights, ::Security::RightsCombinator_out rights_combinator ) = 0;
    virtual void set_required_rights( const char* operation_name, const char* interface_name, const ::Security::RightsList& rights, ::Security::RightsCombinator rights_combinator ) = 0;
    virtual CORBA::Boolean load_config_file( const char* filename ) = 0;

  protected:
    RequiredRights() {};
  private:
    RequiredRights( const RequiredRights& );
    void operator=( const RequiredRights& );
};



/*
 * Base class and common definitions for interface AccessRights
 */

class AccessRights : 
  virtual public CORBA::Object
{
  public:
    virtual ~AccessRights();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AccessRights_ptr _ptr_type;
    typedef AccessRights_var _var_type;
    #endif

    static AccessRights_ptr _narrow( CORBA::Object_ptr obj );
    static AccessRights_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AccessRights_ptr _duplicate( AccessRights_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AccessRights_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::RightsList* get_effective_rights( const ::Security::AttributeList& attribute_list, const ::Security::ExtensibleFamily& rights_family ) = 0;
    virtual ::Security::RightsList* get_all_effective_rights( const ::Security::AttributeList& attrib_list ) = 0;
    virtual void grant_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual void revoke_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual void replace_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual ::Security::RightsList* get_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::ExtensibleFamily& rights_family ) = 0;
    virtual ::Security::RightsList* get_all_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state ) = 0;
    virtual CORBA::Boolean load_config_file( const char* filename ) = 0;

  protected:
    AccessRights() {};
  private:
    AccessRights( const AccessRights& );
    void operator=( const AccessRights& );
};


const ::Security::AuthenticationMethod KeyCertCAPass = 1;

/*
 * Base class and common definitions for interface ReceivedCredentials
 */

class ReceivedCredentials : 
  virtual public ::SecurityLevel2::Credentials
{
  public:
    virtual ~ReceivedCredentials();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ReceivedCredentials_ptr _ptr_type;
    typedef ReceivedCredentials_var _var_type;
    #endif

    static ReceivedCredentials_ptr _narrow( CORBA::Object_ptr obj );
    static ReceivedCredentials_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ReceivedCredentials_ptr _duplicate( ReceivedCredentials_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ReceivedCredentials_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::SecurityLevel2::CredentialsList* accepting_credentials() = 0;
    virtual ::Security::AssociationOptions association_options_used() = 0;
    virtual ::Security::DelegationState delegation_state() = 0;
    virtual ::Security::DelegationMode delegation_mode() = 0;

  protected:
    ReceivedCredentials() {};
  private:
    ReceivedCredentials( const ReceivedCredentials& );
    void operator=( const ReceivedCredentials& );
};



/*
 * Base class and common definitions for interface TargetCredentials
 */

class TargetCredentials : 
  virtual public ::SecurityLevel2::Credentials
{
  public:
    virtual ~TargetCredentials();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef TargetCredentials_ptr _ptr_type;
    typedef TargetCredentials_var _var_type;
    #endif

    static TargetCredentials_ptr _narrow( CORBA::Object_ptr obj );
    static TargetCredentials_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static TargetCredentials_ptr _duplicate( TargetCredentials_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static TargetCredentials_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::SecurityLevel2::CredentialsList* initiating_credentials() = 0;
    virtual ::Security::AssociationOptions association_options_used() = 0;

  protected:
    TargetCredentials() {};
  private:
    TargetCredentials( const TargetCredentials& );
    void operator=( const TargetCredentials& );
};



/*
 * Base class and common definitions for interface PrincipalAuthenticator
 */

class PrincipalAuthenticator : 
  virtual public CORBA::Object
{
  public:
    virtual ~PrincipalAuthenticator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PrincipalAuthenticator_ptr _ptr_type;
    typedef PrincipalAuthenticator_var _var_type;
    #endif

    static PrincipalAuthenticator_ptr _narrow( CORBA::Object_ptr obj );
    static PrincipalAuthenticator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PrincipalAuthenticator_ptr _duplicate( PrincipalAuthenticator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PrincipalAuthenticator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::AuthenticationStatus authenticate( ::Security::AuthenticationMethod method, const char* mechanism, const char* security_name, const CORBA::Any& auth_data, const ::Security::AttributeList& privileges, Credentials_out creds, CORBA::Any_out continuation_data, CORBA::Any_out auth_specific_data ) = 0;
    virtual ::Security::AuthenticationMethodList* get_supported_authen_methods( const char* mechanism ) = 0;
    virtual ::Security::AuthenticationStatus continue_authentication( const CORBA::Any& response_data, Credentials_ptr creds, CORBA::Any_out continuation_data, CORBA::Any_out auth_specific_data ) = 0;

  protected:
    PrincipalAuthenticator() {};
  private:
    PrincipalAuthenticator( const PrincipalAuthenticator& );
    void operator=( const PrincipalAuthenticator& );
};



/*
 * Base class and common definitions for interface PolicyCurrent
 */

class PolicyCurrent : 
  virtual public ::CORBA::Current
{
  public:
    virtual ~PolicyCurrent();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PolicyCurrent_ptr _ptr_type;
    typedef PolicyCurrent_var _var_type;
    #endif

    static PolicyCurrent_ptr _narrow( CORBA::Object_ptr obj );
    static PolicyCurrent_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PolicyCurrent_ptr _duplicate( PolicyCurrent_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PolicyCurrent_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_policy_overrides( const ::CORBA::PolicyList& policies, ::CORBA::SetOverrideType override_type ) = 0;
    virtual ::CORBA::PolicyList* get_policy_overrides( const ::CORBA::PolicyTypeSeq& policy_types ) = 0;

  protected:
    PolicyCurrent() {};
  private:
    PolicyCurrent( const PolicyCurrent& );
    void operator=( const PolicyCurrent& );
};



/*
 * Base class and common definitions for interface MechanismPolicy
 */

class MechanismPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~MechanismPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef MechanismPolicy_ptr _ptr_type;
    typedef MechanismPolicy_var _var_type;
    #endif

    static MechanismPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static MechanismPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static MechanismPolicy_ptr _duplicate( MechanismPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static MechanismPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::MechanismTypeList* mechanisms() = 0;

  protected:
    MechanismPolicy() {};
  private:
    MechanismPolicy( const MechanismPolicy& );
    void operator=( const MechanismPolicy& );
};



/*
 * Base class and common definitions for interface InvocationCredentialsPolicy
 */

class InvocationCredentialsPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~InvocationCredentialsPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef InvocationCredentialsPolicy_ptr _ptr_type;
    typedef InvocationCredentialsPolicy_var _var_type;
    #endif

    static InvocationCredentialsPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static InvocationCredentialsPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static InvocationCredentialsPolicy_ptr _duplicate( InvocationCredentialsPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static InvocationCredentialsPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::SecurityLevel2::CredentialsList* creds() = 0;

  protected:
    InvocationCredentialsPolicy() {};
  private:
    InvocationCredentialsPolicy( const InvocationCredentialsPolicy& );
    void operator=( const InvocationCredentialsPolicy& );
};



/*
 * Base class and common definitions for interface QOPPolicy
 */

class QOPPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~QOPPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef QOPPolicy_ptr _ptr_type;
    typedef QOPPolicy_var _var_type;
    #endif

    static QOPPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static QOPPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static QOPPolicy_ptr _duplicate( QOPPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static QOPPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::QOP qop() = 0;

  protected:
    QOPPolicy() {};
  private:
    QOPPolicy( const QOPPolicy& );
    void operator=( const QOPPolicy& );
};



/*
 * Base class and common definitions for interface DelegationDirectivePolicy
 */

class DelegationDirectivePolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~DelegationDirectivePolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DelegationDirectivePolicy_ptr _ptr_type;
    typedef DelegationDirectivePolicy_var _var_type;
    #endif

    static DelegationDirectivePolicy_ptr _narrow( CORBA::Object_ptr obj );
    static DelegationDirectivePolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DelegationDirectivePolicy_ptr _duplicate( DelegationDirectivePolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DelegationDirectivePolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::DelegationDirective delegation_mode() = 0;

  protected:
    DelegationDirectivePolicy() {};
  private:
    DelegationDirectivePolicy( const DelegationDirectivePolicy& );
    void operator=( const DelegationDirectivePolicy& );
};



/*
 * Base class and common definitions for interface EstablishTrustPolicy
 */

class EstablishTrustPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~EstablishTrustPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EstablishTrustPolicy_ptr _ptr_type;
    typedef EstablishTrustPolicy_var _var_type;
    #endif

    static EstablishTrustPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static EstablishTrustPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EstablishTrustPolicy_ptr _duplicate( EstablishTrustPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EstablishTrustPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::EstablishTrust trust() = 0;

  protected:
    EstablishTrustPolicy() {};
  private:
    EstablishTrustPolicy( const EstablishTrustPolicy& );
    void operator=( const EstablishTrustPolicy& );
};



/*
 * Base class and common definitions for interface SecurityManager
 */

class SecurityManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~SecurityManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SecurityManager_ptr _ptr_type;
    typedef SecurityManager_var _var_type;
    #endif

    static SecurityManager_ptr _narrow( CORBA::Object_ptr obj );
    static SecurityManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SecurityManager_ptr _duplicate( SecurityManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SecurityManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::MechandOptionsList* supported_mechanisms() = 0;
    virtual ::SecurityLevel2::PrincipalAuthenticator_ptr principal_authenticator() = 0;
    virtual ::SecurityLevel2::AuditDecision_ptr audit_decision() = 0;
    virtual ::SecurityLevel2::CredentialsList* own_credentials() = 0;
    virtual ::SecurityLevel2::AccessRights_ptr access_rights() = 0;
    virtual ::SecurityLevel2::AccessDecision_ptr access_decision() = 0;

    virtual void remove_own_credentials( Credentials_ptr creds ) = 0;
    virtual void set_own_credentials( const CredentialsList& creds ) = 0;
    virtual TargetCredentials_ptr get_target_credentials( CORBA::Object_ptr target ) = 0;
    virtual ::CORBA::Policy_ptr get_security_policy( ::CORBA::PolicyType policy_type ) = 0;
    virtual CORBA::Any* get_method_data( ::Security::AuthenticationMethod method ) = 0;

  protected:
    SecurityManager() {};
  private:
    SecurityManager( const SecurityManager& );
    void operator=( const SecurityManager& );
};



/*
 * Base class and common definitions for interface AuditChannel
 */

class AuditChannel : 
  virtual public CORBA::Object
{
  public:
    virtual ~AuditChannel();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AuditChannel_ptr _ptr_type;
    typedef AuditChannel_var _var_type;
    #endif

    static AuditChannel_ptr _narrow( CORBA::Object_ptr obj );
    static AuditChannel_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AuditChannel_ptr _duplicate( AuditChannel_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AuditChannel_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::AuditChannelId audit_channel_id() = 0;

    virtual void audit_write( const ::Security::AuditEventType& event_type, const CredentialsList& creds, const ::Security::UtcT& time, const ::Security::SelectorValueList& descriptors, const ::Security::Opaque& event_specific_data ) = 0;

  protected:
    AuditChannel() {};
  private:
    AuditChannel( const AuditChannel& );
    void operator=( const AuditChannel& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
