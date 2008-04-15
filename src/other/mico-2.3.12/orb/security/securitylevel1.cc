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

SecurityLevel1::Current::~Current()
{
}

void *
SecurityLevel1::Current::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityLevel1/Current:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Current::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityLevel1::Current_ptr
SecurityLevel1::Current::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityLevel1/Current:1.0" )))
      return _duplicate( (SecurityLevel1::Current_ptr) _p );
  }
  return _nil();
}

SecurityLevel1::Current_ptr
SecurityLevel1::Current::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}


struct __tc_init_SECURITYLEVEL1 {
  __tc_init_SECURITYLEVEL1()
  {
  }

  ~__tc_init_SECURITYLEVEL1()
  {
  }
};

static __tc_init_SECURITYLEVEL1 __init_SECURITYLEVEL1;

