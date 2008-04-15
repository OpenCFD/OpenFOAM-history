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
 * Base interface for class Manager
 */

ObjectDomainMapping::Manager::~Manager()
{
}

void *
ObjectDomainMapping::Manager::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/ObjectDomainMapping/Manager:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

ObjectDomainMapping::Manager_ptr
ObjectDomainMapping::Manager::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/ObjectDomainMapping/Manager:1.0" )))
      return _duplicate( (ObjectDomainMapping::Manager_ptr) _p );
  }
  return _nil();
}

ObjectDomainMapping::Manager_ptr
ObjectDomainMapping::Manager::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class Factory
 */

ObjectDomainMapping::Factory::~Factory()
{
}

void *
ObjectDomainMapping::Factory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/ObjectDomainMapping/Factory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

ObjectDomainMapping::Factory_ptr
ObjectDomainMapping::Factory::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/ObjectDomainMapping/Factory:1.0" )))
      return _duplicate( (ObjectDomainMapping::Factory_ptr) _p );
  }
  return _nil();
}

ObjectDomainMapping::Factory_ptr
ObjectDomainMapping::Factory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class ODM
 */

ObjectDomainMapping::ODM::~ODM()
{
}

void *
ObjectDomainMapping::ODM::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/ObjectDomainMapping/ODM:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

ObjectDomainMapping::ODM_ptr
ObjectDomainMapping::ODM::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/ObjectDomainMapping/ODM:1.0" )))
      return _duplicate( (ObjectDomainMapping::ODM_ptr) _p );
  }
  return _nil();
}

ObjectDomainMapping::ODM_ptr
ObjectDomainMapping::ODM::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}


struct __tc_init_ODM {
  __tc_init_ODM()
  {
  }

  ~__tc_init_ODM()
  {
  }
};

static __tc_init_ODM __init_ODM;

