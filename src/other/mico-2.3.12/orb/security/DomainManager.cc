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

#ifdef HAVE_EXPLICIT_STRUCT_OPS
SecurityDomain::NameComponent::NameComponent()
{
}

SecurityDomain::NameComponent::NameComponent( const NameComponent& _s )
{
  id = ((NameComponent&)_s).id;
  kind = ((NameComponent&)_s).kind;
}

SecurityDomain::NameComponent::~NameComponent()
{
}

SecurityDomain::NameComponent&
SecurityDomain::NameComponent::operator=( const NameComponent& _s )
{
  id = ((NameComponent&)_s).id;
  kind = ((NameComponent&)_s).kind;
  return *this;
}
#endif





#ifdef HAVE_EXPLICIT_STRUCT_OPS
SecurityDomain::CombinatorInfo::CombinatorInfo()
{
}

SecurityDomain::CombinatorInfo::CombinatorInfo( const CombinatorInfo& _s )
{
  policy_type = ((CombinatorInfo&)_s).policy_type;
  combinator = ((CombinatorInfo&)_s).combinator;
}

SecurityDomain::CombinatorInfo::~CombinatorInfo()
{
}

SecurityDomain::CombinatorInfo&
SecurityDomain::CombinatorInfo::operator=( const CombinatorInfo& _s )
{
  policy_type = ((CombinatorInfo&)_s).policy_type;
  combinator = ((CombinatorInfo&)_s).combinator;
  return *this;
}
#endif



/*
 * Base interface for class NameIterator
 */

SecurityDomain::NameIterator::~NameIterator()
{
}

void *
SecurityDomain::NameIterator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/NameIterator:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityDomain::NameIterator_ptr
SecurityDomain::NameIterator::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/NameIterator:1.0" )))
      return _duplicate( (SecurityDomain::NameIterator_ptr) _p );
  }
  return _nil();
}

SecurityDomain::NameIterator_ptr
SecurityDomain::NameIterator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}




/*
 * Base interface for class DomainManagerAdmin
 */

SecurityDomain::DomainManagerAdmin::~DomainManagerAdmin()
{
}

void *
SecurityDomain::DomainManagerAdmin::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/DomainManagerAdmin:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityDomain::DomainManagerAdmin_ptr
SecurityDomain::DomainManagerAdmin::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/DomainManagerAdmin:1.0" )))
      return _duplicate( (SecurityDomain::DomainManagerAdmin_ptr) _p );
  }
  return _nil();
}

SecurityDomain::DomainManagerAdmin_ptr
SecurityDomain::DomainManagerAdmin::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class DomainManagerFactory
 */

SecurityDomain::DomainManagerFactory::~DomainManagerFactory()
{
}

void *
SecurityDomain::DomainManagerFactory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/DomainManagerFactory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityDomain::DomainManagerFactory_ptr
SecurityDomain::DomainManagerFactory::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/DomainManagerFactory:1.0" )))
      return _duplicate( (SecurityDomain::DomainManagerFactory_ptr) _p );
  }
  return _nil();
}

SecurityDomain::DomainManagerFactory_ptr
SecurityDomain::DomainManagerFactory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class DomainAuthority
 */

SecurityDomain::DomainAuthority::~DomainAuthority()
{
}

void *
SecurityDomain::DomainAuthority::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/DomainAuthority:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityDomain::DomainManagerAdmin::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityDomain::DomainAuthority_ptr
SecurityDomain::DomainAuthority::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/DomainAuthority:1.0" )))
      return _duplicate( (SecurityDomain::DomainAuthority_ptr) _p );
  }
  return _nil();
}

SecurityDomain::DomainAuthority_ptr
SecurityDomain::DomainAuthority::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class DomainAuthorityAdmin
 */

SecurityDomain::DomainAuthorityAdmin::~DomainAuthorityAdmin()
{
}

void *
SecurityDomain::DomainAuthorityAdmin::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/DomainAuthorityAdmin:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityDomain::DomainAuthority::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityDomain::DomainAuthorityAdmin_ptr
SecurityDomain::DomainAuthorityAdmin::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/DomainAuthorityAdmin:1.0" )))
      return _duplicate( (SecurityDomain::DomainAuthorityAdmin_ptr) _p );
  }
  return _nil();
}

SecurityDomain::DomainAuthorityAdmin_ptr
SecurityDomain::DomainAuthorityAdmin::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class NameExt
 */

SecurityDomain::NameExt::~NameExt()
{
}

void *
SecurityDomain::NameExt::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:SecurityDomain/NameExt:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

SecurityDomain::NameExt_ptr
SecurityDomain::NameExt::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:SecurityDomain/NameExt:1.0" )))
      return _duplicate( (SecurityDomain::NameExt_ptr) _p );
  }
  return _nil();
}

SecurityDomain::NameExt_ptr
SecurityDomain::NameExt::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}


struct __tc_init_DOMAINMANAGER {
  __tc_init_DOMAINMANAGER()
  {
  }

  ~__tc_init_DOMAINMANAGER()
  {
  }
};

static __tc_init_DOMAINMANAGER __init_DOMAINMANAGER;

