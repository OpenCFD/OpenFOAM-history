/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __SECURITYADMIN_H__
#define __SECURITYADMIN_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace SecurityAdmin
{

class AccessPolicy;
typedef AccessPolicy *AccessPolicy_ptr;
typedef AccessPolicy_ptr AccessPolicyRef;
typedef ObjVar< AccessPolicy > AccessPolicy_var;
typedef ObjOut< AccessPolicy > AccessPolicy_out;

class ObjectAccessPolicy;
typedef ObjectAccessPolicy *ObjectAccessPolicy_ptr;
typedef ObjectAccessPolicy_ptr ObjectAccessPolicyRef;
typedef ObjVar< ObjectAccessPolicy > ObjectAccessPolicy_var;
typedef ObjOut< ObjectAccessPolicy > ObjectAccessPolicy_out;

class DomainAccessPolicy;
typedef DomainAccessPolicy *DomainAccessPolicy_ptr;
typedef DomainAccessPolicy_ptr DomainAccessPolicyRef;
typedef ObjVar< DomainAccessPolicy > DomainAccessPolicy_var;
typedef ObjOut< DomainAccessPolicy > DomainAccessPolicy_out;

class AuditPolicy;
typedef AuditPolicy *AuditPolicy_ptr;
typedef AuditPolicy_ptr AuditPolicyRef;
typedef ObjVar< AuditPolicy > AuditPolicy_var;
typedef ObjOut< AuditPolicy > AuditPolicy_out;

class AuditTargetPolicy;
typedef AuditTargetPolicy *AuditTargetPolicy_ptr;
typedef AuditTargetPolicy_ptr AuditTargetPolicyRef;
typedef ObjVar< AuditTargetPolicy > AuditTargetPolicy_var;
typedef ObjOut< AuditTargetPolicy > AuditTargetPolicy_out;

class AuditClientPolicy;
typedef AuditClientPolicy *AuditClientPolicy_ptr;
typedef AuditClientPolicy_ptr AuditClientPolicyRef;
typedef ObjVar< AuditClientPolicy > AuditClientPolicy_var;
typedef ObjOut< AuditClientPolicy > AuditClientPolicy_out;

class SecureInvocationPolicy;
typedef SecureInvocationPolicy *SecureInvocationPolicy_ptr;
typedef SecureInvocationPolicy_ptr SecureInvocationPolicyRef;
typedef ObjVar< SecureInvocationPolicy > SecureInvocationPolicy_var;
typedef ObjOut< SecureInvocationPolicy > SecureInvocationPolicy_out;

class DelegationPolicy;
typedef DelegationPolicy *DelegationPolicy_ptr;
typedef DelegationPolicy_ptr DelegationPolicyRef;
typedef ObjVar< DelegationPolicy > DelegationPolicy_var;
typedef ObjOut< DelegationPolicy > DelegationPolicy_out;

}






namespace SecurityAdmin
{


/*
 * Base class and common definitions for interface AccessPolicy
 */

class AccessPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~AccessPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AccessPolicy_ptr _ptr_type;
    typedef AccessPolicy_var _var_type;
    #endif

    static AccessPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static AccessPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AccessPolicy_ptr _duplicate( AccessPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AccessPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Security::RightsList* get_effective_rights( const ::Security::AttributeList& attribute_list, const ::Security::ExtensibleFamily& rights_family ) = 0;
    virtual ::Security::RightsList* get_all_effective_rights( const ::Security::AttributeList& attrib_list ) = 0;

  protected:
    AccessPolicy() {};
  private:
    AccessPolicy( const AccessPolicy& );
    void operator=( const AccessPolicy& );
};



/*
 * Base class and common definitions for interface ObjectAccessPolicy
 */

class ObjectAccessPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~ObjectAccessPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ObjectAccessPolicy_ptr _ptr_type;
    typedef ObjectAccessPolicy_var _var_type;
    #endif

    static ObjectAccessPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static ObjectAccessPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ObjectAccessPolicy_ptr _duplicate( ObjectAccessPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ObjectAccessPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void get_required_rights( CORBA::Object_ptr obj, const char* operation_name, const char* interface_name, ::Security::RightsList_out rights, ::Security::RightsCombinator_out rights_combinator ) = 0;
    virtual void set_required_rights( const char* operation_name, const char* interface_name, const ::Security::RightsList& rights, ::Security::RightsCombinator rights_combinator ) = 0;

  protected:
    ObjectAccessPolicy() {};
  private:
    ObjectAccessPolicy( const ObjectAccessPolicy& );
    void operator=( const ObjectAccessPolicy& );
};



/*
 * Base class and common definitions for interface DomainAccessPolicy
 */

class DomainAccessPolicy : 
  virtual public ::SecurityAdmin::AccessPolicy
{
  public:
    virtual ~DomainAccessPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainAccessPolicy_ptr _ptr_type;
    typedef DomainAccessPolicy_var _var_type;
    #endif

    static DomainAccessPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static DomainAccessPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainAccessPolicy_ptr _duplicate( DomainAccessPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainAccessPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void grant_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual void revoke_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual void replace_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::RightsList& rights ) = 0;
    virtual ::Security::RightsList* get_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state, const ::Security::ExtensibleFamily& rights_family ) = 0;
    virtual ::Security::RightsList* get_all_rights( const ::Security::SecAttribute& priv_attr, ::Security::DelegationState del_state ) = 0;

  protected:
    DomainAccessPolicy() {};
  private:
    DomainAccessPolicy( const DomainAccessPolicy& );
    void operator=( const DomainAccessPolicy& );
};



/*
 * Base class and common definitions for interface AuditPolicy
 */

class AuditPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~AuditPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AuditPolicy_ptr _ptr_type;
    typedef AuditPolicy_var _var_type;
    #endif

    static AuditPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static AuditPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AuditPolicy_ptr _duplicate( AuditPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AuditPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_audit_selectors( const char* object_type, const ::Security::AuditEventTypeList& events, const ::Security::SelectorValueList& selectors, ::Security::AuditCombinator audit_combinator ) = 0;
    virtual void clear_audit_selectors( const char* object_type, const ::Security::AuditEventTypeList& events ) = 0;
    virtual void replace_audit_selectors( const char* object_type, const ::Security::AuditEventTypeList& events, const ::Security::SelectorValueList& selectors, ::Security::AuditCombinator audit_combinator ) = 0;
    virtual void get_audit_selectors( const char* object_type, const ::Security::AuditEventType& event_type, ::Security::SelectorValueList_out selectors, ::Security::AuditCombinator_out audit_combinator ) = 0;
    virtual void set_audit_channel( ::Security::AuditChannelId audit_channel_id ) = 0;

  protected:
    AuditPolicy() {};
  private:
    AuditPolicy( const AuditPolicy& );
    void operator=( const AuditPolicy& );
};



/*
 * Base class and common definitions for interface AuditTargetPolicy
 */

class AuditTargetPolicy : 
  virtual public ::SecurityAdmin::AuditPolicy
{
  public:
    virtual ~AuditTargetPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AuditTargetPolicy_ptr _ptr_type;
    typedef AuditTargetPolicy_var _var_type;
    #endif

    static AuditTargetPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static AuditTargetPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AuditTargetPolicy_ptr _duplicate( AuditTargetPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AuditTargetPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    AuditTargetPolicy() {};
  private:
    AuditTargetPolicy( const AuditTargetPolicy& );
    void operator=( const AuditTargetPolicy& );
};



/*
 * Base class and common definitions for interface AuditClientPolicy
 */

class AuditClientPolicy : 
  virtual public ::SecurityAdmin::AuditPolicy
{
  public:
    virtual ~AuditClientPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AuditClientPolicy_ptr _ptr_type;
    typedef AuditClientPolicy_var _var_type;
    #endif

    static AuditClientPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static AuditClientPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AuditClientPolicy_ptr _duplicate( AuditClientPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AuditClientPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    AuditClientPolicy() {};
  private:
    AuditClientPolicy( const AuditClientPolicy& );
    void operator=( const AuditClientPolicy& );
};



/*
 * Base class and common definitions for interface SecureInvocationPolicy
 */

class SecureInvocationPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~SecureInvocationPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SecureInvocationPolicy_ptr _ptr_type;
    typedef SecureInvocationPolicy_var _var_type;
    #endif

    static SecureInvocationPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static SecureInvocationPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SecureInvocationPolicy_ptr _duplicate( SecureInvocationPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SecureInvocationPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_association_options( const char* object_type, ::Security::RequiresSupports requires_supports, ::Security::CommunicationDirection direction, ::Security::AssociationOptions options ) = 0;
    virtual ::Security::AssociationOptions get_association_options( const char* object_type, ::Security::RequiresSupports requires_supports, ::Security::CommunicationDirection direction ) = 0;

  protected:
    SecureInvocationPolicy() {};
  private:
    SecureInvocationPolicy( const SecureInvocationPolicy& );
    void operator=( const SecureInvocationPolicy& );
};



/*
 * Base class and common definitions for interface DelegationPolicy
 */

class DelegationPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~DelegationPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DelegationPolicy_ptr _ptr_type;
    typedef DelegationPolicy_var _var_type;
    #endif

    static DelegationPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static DelegationPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DelegationPolicy_ptr _duplicate( DelegationPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DelegationPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_delegation_mode( const char* object_type, ::Security::DelegationMode mode ) = 0;
    virtual ::Security::DelegationMode get_delegation_mode( const char* object_type ) = 0;

  protected:
    DelegationPolicy() {};
  private:
    DelegationPolicy( const DelegationPolicy& );
    void operator=( const DelegationPolicy& );
};


struct AuditTimeInterval;
typedef TFixVar< AuditTimeInterval > AuditTimeInterval_var;
typedef AuditTimeInterval& AuditTimeInterval_out;


struct AuditTimeInterval {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AuditTimeInterval_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AuditTimeInterval();
  ~AuditTimeInterval();
  AuditTimeInterval( const AuditTimeInterval& s );
  AuditTimeInterval& operator=( const AuditTimeInterval& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Long begin;
  CORBA::Long end;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuditTimeInterval;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::SecurityAdmin::AuditTimeInterval &_s );
void operator<<=( CORBA::Any &_a, ::SecurityAdmin::AuditTimeInterval *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::SecurityAdmin::AuditTimeInterval &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::SecurityAdmin::AuditTimeInterval *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AuditTimeInterval;

#endif
