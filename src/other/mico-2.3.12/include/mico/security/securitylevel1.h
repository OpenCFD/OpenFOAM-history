/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __SECURITYLEVEL1_H__
#define __SECURITYLEVEL1_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace SecurityLevel1
{

class Current;
typedef Current *Current_ptr;
typedef Current_ptr CurrentRef;
typedef ObjVar< Current > Current_var;
typedef ObjOut< Current > Current_out;

}






namespace SecurityLevel1
{


/*
 * Base class and common definitions for interface Current
 */

class Current : 
  virtual public ::CORBA::Current
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

    virtual ::Security::AttributeList* get_attributes( const ::Security::AttributeTypeList& attributes ) = 0;
    virtual ::Security::AttributeList* get_target_attributes( const ::Security::AttributeTypeList& attributes, CORBA::Object_ptr obj ) = 0;

  protected:
    Current() {};
  private:
    Current( const Current& );
    void operator=( const Current& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
