/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __ODM_H__
#define __ODM_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace ObjectDomainMapping
{

class Manager;
typedef Manager *Manager_ptr;
typedef Manager_ptr ManagerRef;
typedef ObjVar< Manager > Manager_var;
typedef ObjOut< Manager > Manager_out;

class Factory;
typedef Factory *Factory_ptr;
typedef Factory_ptr FactoryRef;
typedef ObjVar< Factory > Factory_var;
typedef ObjOut< Factory > Factory_out;

class ODM;
typedef ODM *ODM_ptr;
typedef ODM_ptr ODMRef;
typedef ObjVar< ODM > ODM_var;
typedef ObjOut< ODM > ODM_out;

}






namespace ObjectDomainMapping
{


/*
 * Base class and common definitions for interface Manager
 */

class Manager : 
  virtual public CORBA::Object
{
  public:
    virtual ~Manager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Manager_ptr _ptr_type;
    typedef Manager_var _var_type;
    #endif

    static Manager_ptr _narrow( CORBA::Object_ptr obj );
    static Manager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Manager_ptr _duplicate( Manager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Manager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    typedef CORBA::UShort ODMGranularity;
    typedef ODMGranularity& ODMGranularity_out;
    virtual void set_domain_name_key( ODMGranularity granularity, const ::Security::Opaque& key, const ::SecurityDomain::NameList& domainNameList ) = 0;
    virtual ::SecurityDomain::NameList* get_domain_names( const ::Security::Opaque& key ) = 0;
    virtual void remove_domain_names( const ::Security::Opaque& key ) = 0;
    virtual void set_parent_odm_key( const ::Security::Opaque& key, Manager_ptr odm ) = 0;
    virtual void set_default_parent_odm( Manager_ptr odm ) = 0;
    virtual void set_default_name_key( const ::SecurityDomain::NameList& domainNameList ) = 0;

  protected:
    Manager() {};
  private:
    Manager( const Manager& );
    void operator=( const Manager& );
};



/*
 * Base class and common definitions for interface Factory
 */

class Factory : 
  virtual public CORBA::Object
{
  public:
    virtual ~Factory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Factory_ptr _ptr_type;
    typedef Factory_var _var_type;
    #endif

    static Factory_ptr _narrow( CORBA::Object_ptr obj );
    static Factory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Factory_ptr _duplicate( Factory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Factory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Manager_ptr create() = 0;
    virtual CORBA::Boolean load_config_file( const char* filename ) = 0;
    virtual CORBA::Boolean save_config_file( const char* filename ) = 0;

  protected:
    Factory() {};
  private:
    Factory( const Factory& );
    void operator=( const Factory& );
};



/*
 * Base class and common definitions for interface ODM
 */

class ODM : 
  virtual public CORBA::Object
{
  public:
    virtual ~ODM();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ODM_ptr _ptr_type;
    typedef ODM_var _var_type;
    #endif

    static ODM_ptr _narrow( CORBA::Object_ptr obj );
    static ODM_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ODM_ptr _duplicate( ODM_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ODM_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Factory_ptr create() = 0;
    virtual Factory_ptr current() = 0;

  protected:
    ODM() {};
  private:
    ODM( const ODM& );
    void operator=( const ODM& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
