/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>
#include <mico/template_impl.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------

/*
 * Base interface for class Credentials
 */

SecurityLevel2::Credentials::~Credentials()
{
}

void *
SecurityLevel2::Credentials::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/Credentials:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::Credentials_ptr
SecurityLevel2::Credentials::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/Credentials:1.0" )))
      return _duplicate( (SecurityLevel2::Credentials_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::Credentials_ptr
SecurityLevel2::Credentials::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class Current
 */

SecurityLevel2::Current::~Current()
{
}

void *
SecurityLevel2::Current::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/Current:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityLevel1::Current::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::Current_ptr
SecurityLevel2::Current::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/Current:1.0" )))
      return _duplicate( (SecurityLevel2::Current_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::Current_ptr
SecurityLevel2::Current::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class AuditDecision
 */

SecurityLevel2::AuditDecision::~AuditDecision()
{
}

void *
SecurityLevel2::AuditDecision::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/AuditDecision:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::AuditDecision_ptr
SecurityLevel2::AuditDecision::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/AuditDecision:1.0" )))
      return _duplicate( (SecurityLevel2::AuditDecision_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::AuditDecision_ptr
SecurityLevel2::AuditDecision::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}




/*
 * Base interface for class AccessDecision
 */

SecurityLevel2::AccessDecision::~AccessDecision()
{
}

void *
SecurityLevel2::AccessDecision::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/AccessDecision:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::AccessDecision_ptr
SecurityLevel2::AccessDecision::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/AccessDecision:1.0" )))
      return _duplicate( (SecurityLevel2::AccessDecision_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::AccessDecision_ptr
SecurityLevel2::AccessDecision::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class RequiredRights
 */

SecurityLevel2::RequiredRights::~RequiredRights()
{
}

void *
SecurityLevel2::RequiredRights::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/RequiredRights:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::RequiredRights_ptr
SecurityLevel2::RequiredRights::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/RequiredRights:1.0" )))
      return _duplicate( (SecurityLevel2::RequiredRights_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::RequiredRights_ptr
SecurityLevel2::RequiredRights::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class AccessRights
 */

SecurityLevel2::AccessRights::~AccessRights()
{
}

void *
SecurityLevel2::AccessRights::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/AccessRights:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::AccessRights_ptr
SecurityLevel2::AccessRights::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/AccessRights:1.0" )))
      return _duplicate( (SecurityLevel2::AccessRights_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::AccessRights_ptr
SecurityLevel2::AccessRights::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class ReceivedCredentials
 */

SecurityLevel2::ReceivedCredentials::~ReceivedCredentials()
{
}

void *
SecurityLevel2::ReceivedCredentials::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/ReceivedCredentials:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityLevel2::Credentials::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::ReceivedCredentials_ptr
SecurityLevel2::ReceivedCredentials::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/ReceivedCredentials:1.0" )))
      return _duplicate( (SecurityLevel2::ReceivedCredentials_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::ReceivedCredentials_ptr
SecurityLevel2::ReceivedCredentials::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class TargetCredentials
 */

SecurityLevel2::TargetCredentials::~TargetCredentials()
{
}

void *
SecurityLevel2::TargetCredentials::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/TargetCredentials:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityLevel2::Credentials::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::TargetCredentials_ptr
SecurityLevel2::TargetCredentials::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/TargetCredentials:1.0" )))
      return _duplicate( (SecurityLevel2::TargetCredentials_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::TargetCredentials_ptr
SecurityLevel2::TargetCredentials::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class PrincipalAuthenticator
 */

SecurityLevel2::PrincipalAuthenticator::~PrincipalAuthenticator()
{
}

void *
SecurityLevel2::PrincipalAuthenticator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/PrincipalAuthenticator:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::PrincipalAuthenticator_ptr
SecurityLevel2::PrincipalAuthenticator::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/PrincipalAuthenticator:1.0" )))
      return _duplicate( (SecurityLevel2::PrincipalAuthenticator_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::PrincipalAuthenticator_ptr
SecurityLevel2::PrincipalAuthenticator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class PolicyCurrent
 */

SecurityLevel2::PolicyCurrent::~PolicyCurrent()
{
}

void *
SecurityLevel2::PolicyCurrent::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/PolicyCurrent:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Current::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::PolicyCurrent_ptr
SecurityLevel2::PolicyCurrent::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/PolicyCurrent:1.0" )))
      return _duplicate( (SecurityLevel2::PolicyCurrent_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::PolicyCurrent_ptr
SecurityLevel2::PolicyCurrent::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class MechanismPolicy
 */

SecurityLevel2::MechanismPolicy::~MechanismPolicy()
{
}

void *
SecurityLevel2::MechanismPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/MechanismPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::MechanismPolicy_ptr
SecurityLevel2::MechanismPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/MechanismPolicy:1.0" )))
      return _duplicate( (SecurityLevel2::MechanismPolicy_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::MechanismPolicy_ptr
SecurityLevel2::MechanismPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class InvocationCredentialsPolicy
 */

SecurityLevel2::InvocationCredentialsPolicy::~InvocationCredentialsPolicy()
{
}

void *
SecurityLevel2::InvocationCredentialsPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/InvocationCredentialsPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::InvocationCredentialsPolicy_ptr
SecurityLevel2::InvocationCredentialsPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/InvocationCredentialsPolicy:1.0" )))
      return _duplicate( (SecurityLevel2::InvocationCredentialsPolicy_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::InvocationCredentialsPolicy_ptr
SecurityLevel2::InvocationCredentialsPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class QOPPolicy
 */

SecurityLevel2::QOPPolicy::~QOPPolicy()
{
}

void *
SecurityLevel2::QOPPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/QOPPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::QOPPolicy_ptr
SecurityLevel2::QOPPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/QOPPolicy:1.0" )))
      return _duplicate( (SecurityLevel2::QOPPolicy_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::QOPPolicy_ptr
SecurityLevel2::QOPPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class DelegationDirectivePolicy
 */

SecurityLevel2::DelegationDirectivePolicy::~DelegationDirectivePolicy()
{
}

void *
SecurityLevel2::DelegationDirectivePolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/DelegationDirectivePolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::DelegationDirectivePolicy_ptr
SecurityLevel2::DelegationDirectivePolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/DelegationDirectivePolicy:1.0" )))
      return _duplicate( (SecurityLevel2::DelegationDirectivePolicy_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::DelegationDirectivePolicy_ptr
SecurityLevel2::DelegationDirectivePolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class EstablishTrustPolicy
 */

SecurityLevel2::EstablishTrustPolicy::~EstablishTrustPolicy()
{
}

void *
SecurityLevel2::EstablishTrustPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/EstablishTrustPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel2::EstablishTrustPolicy_ptr
SecurityLevel2::EstablishTrustPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/EstablishTrustPolicy:1.0" )))
      return _duplicate( (SecurityLevel2::EstablishTrustPolicy_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::EstablishTrustPolicy_ptr
SecurityLevel2::EstablishTrustPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class SecurityManager
 */

SecurityLevel2::SecurityManager::~SecurityManager()
{
}

void *
SecurityLevel2::SecurityManager::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/SecurityManager:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::SecurityManager_ptr
SecurityLevel2::SecurityManager::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/SecurityManager:1.0" )))
      return _duplicate( (SecurityLevel2::SecurityManager_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::SecurityManager_ptr
SecurityLevel2::SecurityManager::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class AuditChannel
 */

SecurityLevel2::AuditChannel::~AuditChannel()
{
}

void *
SecurityLevel2::AuditChannel::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel2/AuditChannel:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityLevel2::AuditChannel_ptr
SecurityLevel2::AuditChannel::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel2/AuditChannel:1.0" )))
      return _duplicate( (SecurityLevel2::AuditChannel_ptr) _p );
  }
  return _nil();
}

SecurityLevel2::AuditChannel_ptr
SecurityLevel2::AuditChannel::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}


struct __tc_init_SECURITYLEVEL2 {
  __tc_init_SECURITYLEVEL2()
  {
  }

  ~__tc_init_SECURITYLEVEL2()
  {
  }
};

static __tc_init_SECURITYLEVEL2 __init_SECURITYLEVEL2;

