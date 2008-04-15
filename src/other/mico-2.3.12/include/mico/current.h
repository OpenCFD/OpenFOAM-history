/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __CURRENT_H__
#define __CURRENT_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace CORBA
{

class Current;
typedef Current *Current_ptr;
typedef Current_ptr CurrentRef;
typedef ObjVar< Current > Current_var;
typedef ObjOut< Current > Current_out;

class PrincipalCurrent;
typedef PrincipalCurrent *PrincipalCurrent_ptr;
typedef PrincipalCurrent_ptr PrincipalCurrentRef;
typedef ObjVar< PrincipalCurrent > PrincipalCurrent_var;
typedef ObjOut< PrincipalCurrent > PrincipalCurrent_out;

}






namespace CORBA
{


/*
 * Base class and common definitions for interface Current
 */

class Current : 
  virtual public CORBA::Object
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

  protected:
    Current() {};
  private:
    Current( const Current& );
    void operator=( const Current& );
};



/*
 * Base class and common definitions for interface PrincipalCurrent
 */

class PrincipalCurrent : 
  virtual public ::CORBA::Current
{
  public:
    virtual ~PrincipalCurrent();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PrincipalCurrent_ptr _ptr_type;
    typedef PrincipalCurrent_var _var_type;
    #endif

    static PrincipalCurrent_ptr _narrow( CORBA::Object_ptr obj );
    static PrincipalCurrent_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PrincipalCurrent_ptr _duplicate( PrincipalCurrent_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PrincipalCurrent_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Principal_ptr get_principal() = 0;

  protected:
    PrincipalCurrent() {};
  private:
    PrincipalCurrent( const PrincipalCurrent& );
    void operator=( const PrincipalCurrent& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
