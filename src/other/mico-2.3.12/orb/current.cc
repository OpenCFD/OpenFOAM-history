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
 * Base interface for class Current
 */

CORBA::Current::~Current()
{
}

void *
CORBA::Current::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/Current:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::Current_ptr
CORBA::Current::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/Current:1.0" )))
      return _duplicate( (CORBA::Current_ptr) _p );
  }
  return _nil();
}

CORBA::Current_ptr
CORBA::Current::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}



/*
 * Base interface for class PrincipalCurrent
 */

CORBA::PrincipalCurrent::~PrincipalCurrent()
{
}

void *
CORBA::PrincipalCurrent::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/PrincipalCurrent:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Current::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::PrincipalCurrent_ptr
CORBA::PrincipalCurrent::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/PrincipalCurrent:1.0" )))
      return _duplicate( (CORBA::PrincipalCurrent_ptr) _p );
  }
  return _nil();
}

CORBA::PrincipalCurrent_ptr
CORBA::PrincipalCurrent::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}


struct __tc_init_CURRENT {
  __tc_init_CURRENT()
  {
  }

  ~__tc_init_CURRENT()
  {
  }
};

static __tc_init_CURRENT __init_CURRENT;

