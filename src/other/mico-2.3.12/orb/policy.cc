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
 * Base interface for class Policy
 */

CORBA::Policy::~Policy()
{
}

void *
CORBA::Policy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/Policy:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::Policy_ptr
CORBA::Policy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/Policy:1.0" )))
      return _duplicate( (CORBA::Policy_ptr) _p );
  }
  return _nil();
}

CORBA::Policy_ptr
CORBA::Policy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}





/*
 * Base interface for class DomainManager
 */

CORBA::DomainManager::~DomainManager()
{
}

void *
CORBA::DomainManager::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/DomainManager:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::DomainManager_ptr
CORBA::DomainManager::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/DomainManager:1.0" )))
      return _duplicate( (CORBA::DomainManager_ptr) _p );
  }
  return _nil();
}

CORBA::DomainManager_ptr
CORBA::DomainManager::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class ConstructionPolicy
 */

CORBA::ConstructionPolicy::~ConstructionPolicy()
{
}

void *
CORBA::ConstructionPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ConstructionPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ConstructionPolicy_ptr
CORBA::ConstructionPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ConstructionPolicy:1.0" )))
      return _duplicate( (CORBA::ConstructionPolicy_ptr) _p );
  }
  return _nil();
}

CORBA::ConstructionPolicy_ptr
CORBA::ConstructionPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}






#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::PolicyError::PolicyError()
{
}

CORBA::PolicyError::PolicyError( const PolicyError& _s )
{
  reason = ((PolicyError&)_s).reason;
}

CORBA::PolicyError::~PolicyError()
{
}

CORBA::PolicyError&
CORBA::PolicyError::operator=( const PolicyError& _s )
{
  reason = ((PolicyError&)_s).reason;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
CORBA::PolicyError::PolicyError()
{
}

#endif

CORBA::PolicyError::PolicyError( CORBA::PolicyErrorCode _m0 )
{
  reason = _m0;
}

void CORBA::PolicyError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw PolicyError_var( (CORBA::PolicyError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *CORBA::PolicyError::_repoid() const
{
  return "IDL:omg.org/CORBA/PolicyError:1.0";
}

void CORBA::PolicyError::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void CORBA::PolicyError::_encode_any( CORBA::Any & ) const
{
  // use --any to make this work!
  assert(0);
}

CORBA::Exception *CORBA::PolicyError::_clone() const
{
  return new PolicyError( *this );
}

CORBA::PolicyError *CORBA::PolicyError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/CORBA/PolicyError:1.0" ) )
    return (PolicyError *) _ex;
  return NULL;
}

const CORBA::PolicyError *CORBA::PolicyError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/CORBA/PolicyError:1.0" ) )
    return (PolicyError *) _ex;
  return NULL;
}

struct __tc_init_POLICY {
  __tc_init_POLICY()
  {
  }

  ~__tc_init_POLICY()
  {
  }
};

static __tc_init_POLICY __init_POLICY;

