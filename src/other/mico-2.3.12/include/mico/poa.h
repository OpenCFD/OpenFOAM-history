/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __POA_H__
#define __POA_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace PortableServer
{

class POA;
typedef POA *POA_ptr;
typedef POA_ptr POARef;
typedef ObjVar< POA > POA_var;
typedef ObjOut< POA > POA_out;

class POAManager;
typedef POAManager *POAManager_ptr;
typedef POAManager_ptr POAManagerRef;
typedef ObjVar< POAManager > POAManager_var;
typedef ObjOut< POAManager > POAManager_out;

class ThreadPolicy;
typedef ThreadPolicy *ThreadPolicy_ptr;
typedef ThreadPolicy_ptr ThreadPolicyRef;
typedef ObjVar< ThreadPolicy > ThreadPolicy_var;
typedef ObjOut< ThreadPolicy > ThreadPolicy_out;

class LifespanPolicy;
typedef LifespanPolicy *LifespanPolicy_ptr;
typedef LifespanPolicy_ptr LifespanPolicyRef;
typedef ObjVar< LifespanPolicy > LifespanPolicy_var;
typedef ObjOut< LifespanPolicy > LifespanPolicy_out;

class IdUniquenessPolicy;
typedef IdUniquenessPolicy *IdUniquenessPolicy_ptr;
typedef IdUniquenessPolicy_ptr IdUniquenessPolicyRef;
typedef ObjVar< IdUniquenessPolicy > IdUniquenessPolicy_var;
typedef ObjOut< IdUniquenessPolicy > IdUniquenessPolicy_out;

class IdAssignmentPolicy;
typedef IdAssignmentPolicy *IdAssignmentPolicy_ptr;
typedef IdAssignmentPolicy_ptr IdAssignmentPolicyRef;
typedef ObjVar< IdAssignmentPolicy > IdAssignmentPolicy_var;
typedef ObjOut< IdAssignmentPolicy > IdAssignmentPolicy_out;

class ImplicitActivationPolicy;
typedef ImplicitActivationPolicy *ImplicitActivationPolicy_ptr;
typedef ImplicitActivationPolicy_ptr ImplicitActivationPolicyRef;
typedef ObjVar< ImplicitActivationPolicy > ImplicitActivationPolicy_var;
typedef ObjOut< ImplicitActivationPolicy > ImplicitActivationPolicy_out;

class ServantRetentionPolicy;
typedef ServantRetentionPolicy *ServantRetentionPolicy_ptr;
typedef ServantRetentionPolicy_ptr ServantRetentionPolicyRef;
typedef ObjVar< ServantRetentionPolicy > ServantRetentionPolicy_var;
typedef ObjOut< ServantRetentionPolicy > ServantRetentionPolicy_out;

class RequestProcessingPolicy;
typedef RequestProcessingPolicy *RequestProcessingPolicy_ptr;
typedef RequestProcessingPolicy_ptr RequestProcessingPolicyRef;
typedef ObjVar< RequestProcessingPolicy > RequestProcessingPolicy_var;
typedef ObjOut< RequestProcessingPolicy > RequestProcessingPolicy_out;

class AdapterActivator;
typedef AdapterActivator *AdapterActivator_ptr;
typedef AdapterActivator_ptr AdapterActivatorRef;
typedef ObjVar< AdapterActivator > AdapterActivator_var;
typedef ObjOut< AdapterActivator > AdapterActivator_out;

class ServantManager;
typedef ServantManager *ServantManager_ptr;
typedef ServantManager_ptr ServantManagerRef;
typedef ObjVar< ServantManager > ServantManager_var;
typedef ObjOut< ServantManager > ServantManager_out;

class ServantActivator;
typedef ServantActivator *ServantActivator_ptr;
typedef ServantActivator_ptr ServantActivatorRef;
typedef ObjVar< ServantActivator > ServantActivator_var;
typedef ObjOut< ServantActivator > ServantActivator_out;

class ServantLocator;
typedef ServantLocator *ServantLocator_ptr;
typedef ServantLocator_ptr ServantLocatorRef;
typedef ObjVar< ServantLocator > ServantLocator_var;
typedef ObjOut< ServantLocator > ServantLocator_out;

class Current;
typedef Current *Current_ptr;
typedef Current_ptr CurrentRef;
typedef ObjVar< Current > Current_var;
typedef ObjOut< Current > Current_out;

}






namespace PortableServer
{

enum ThreadPolicyValue {
  ORB_CTRL_MODEL = 0,
  SINGLE_THREAD_MODEL,
  MAIN_THREAD_MODEL
};

typedef ThreadPolicyValue& ThreadPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ThreadPolicyValue;

enum LifespanPolicyValue {
  TRANSIENT = 0,
  PERSISTENT
};

typedef LifespanPolicyValue& LifespanPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_LifespanPolicyValue;

enum IdUniquenessPolicyValue {
  UNIQUE_ID = 0,
  MULTIPLE_ID
};

typedef IdUniquenessPolicyValue& IdUniquenessPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IdUniquenessPolicyValue;

enum IdAssignmentPolicyValue {
  USER_ID = 0,
  SYSTEM_ID
};

typedef IdAssignmentPolicyValue& IdAssignmentPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IdAssignmentPolicyValue;

enum ImplicitActivationPolicyValue {
  IMPLICIT_ACTIVATION = 0,
  NO_IMPLICIT_ACTIVATION
};

typedef ImplicitActivationPolicyValue& ImplicitActivationPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ImplicitActivationPolicyValue;

enum ServantRetentionPolicyValue {
  RETAIN = 0,
  NON_RETAIN
};

typedef ServantRetentionPolicyValue& ServantRetentionPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServantRetentionPolicyValue;

enum RequestProcessingPolicyValue {
  USE_ACTIVE_OBJECT_MAP_ONLY = 0,
  USE_DEFAULT_SERVANT,
  USE_SERVANT_MANAGER
};

typedef RequestProcessingPolicyValue& RequestProcessingPolicyValue_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RequestProcessingPolicyValue;

typedef IfaceSequenceTmpl< POA_var,POA_ptr> POAList;
typedef TSeqVar< IfaceSequenceTmpl< POA_var,POA_ptr> > POAList_var;
typedef TSeqOut< IfaceSequenceTmpl< POA_var,POA_ptr> > POAList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_POAList;

typedef MICO_Native_PortableServer_Servant Servant;
typedef MICO_Native_PortableServer_Servant& Servant_out;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> ObjectId;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ObjectId_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ObjectId_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectId;


/*
 * Base class and common definitions for interface POAManager
 */

class POAManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~POAManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef POAManager_ptr _ptr_type;
    typedef POAManager_var _var_type;
    #endif

    static POAManager_ptr _narrow( CORBA::Object_ptr obj );
    static POAManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static POAManager_ptr _duplicate( POAManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static POAManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct AdapterInactive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      AdapterInactive();
      ~AdapterInactive();
      AdapterInactive( const AdapterInactive& s );
      AdapterInactive& operator=( const AdapterInactive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      AdapterInactive *operator->() { return this; }
      AdapterInactive& operator*() { return *this; }
      operator AdapterInactive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static AdapterInactive *_downcast( CORBA::Exception *ex );
      static const AdapterInactive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef AdapterInactive AdapterInactive_catch;
    #else
    typedef ExceptVar< AdapterInactive > AdapterInactive_var;
    typedef TVarOut< AdapterInactive > AdapterInactive_out;
    typedef AdapterInactive_var AdapterInactive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_AdapterInactive;

    enum State {
      HOLDING = 0,
      ACTIVE,
      DISCARDING,
      INACTIVE
    };

    typedef State& State_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_State;

    virtual void activate() = 0;
    virtual void hold_requests( CORBA::Boolean wait_for_completion ) = 0;
    virtual void discard_requests( CORBA::Boolean wait_for_completion ) = 0;
    virtual void deactivate( CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion ) = 0;
    virtual State get_state() = 0;
    virtual void add_managed_poa( POA_ptr managed ) = 0;
    virtual void del_managed_poa( POA_ptr managed ) = 0;

  protected:
    POAManager() {};
  private:
    POAManager( const POAManager& );
    void operator=( const POAManager& );
};



/*
 * Base class and common definitions for interface POA
 */

class POA : 
  virtual public CORBA::Object
{
  public:
    virtual ~POA();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef POA_ptr _ptr_type;
    typedef POA_var _var_type;
    #endif

    static POA_ptr _narrow( CORBA::Object_ptr obj );
    static POA_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static POA_ptr _duplicate( POA_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static POA_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct AdapterAlreadyExists : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      AdapterAlreadyExists();
      ~AdapterAlreadyExists();
      AdapterAlreadyExists( const AdapterAlreadyExists& s );
      AdapterAlreadyExists& operator=( const AdapterAlreadyExists& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      AdapterAlreadyExists *operator->() { return this; }
      AdapterAlreadyExists& operator*() { return *this; }
      operator AdapterAlreadyExists*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static AdapterAlreadyExists *_downcast( CORBA::Exception *ex );
      static const AdapterAlreadyExists *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef AdapterAlreadyExists AdapterAlreadyExists_catch;
    #else
    typedef ExceptVar< AdapterAlreadyExists > AdapterAlreadyExists_var;
    typedef TVarOut< AdapterAlreadyExists > AdapterAlreadyExists_out;
    typedef AdapterAlreadyExists_var AdapterAlreadyExists_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_AdapterAlreadyExists;

    struct AdapterInactive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      AdapterInactive();
      ~AdapterInactive();
      AdapterInactive( const AdapterInactive& s );
      AdapterInactive& operator=( const AdapterInactive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      AdapterInactive *operator->() { return this; }
      AdapterInactive& operator*() { return *this; }
      operator AdapterInactive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static AdapterInactive *_downcast( CORBA::Exception *ex );
      static const AdapterInactive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef AdapterInactive AdapterInactive_catch;
    #else
    typedef ExceptVar< AdapterInactive > AdapterInactive_var;
    typedef TVarOut< AdapterInactive > AdapterInactive_out;
    typedef AdapterInactive_var AdapterInactive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_AdapterInactive;

    struct AdapterNonExistent : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      AdapterNonExistent();
      ~AdapterNonExistent();
      AdapterNonExistent( const AdapterNonExistent& s );
      AdapterNonExistent& operator=( const AdapterNonExistent& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      AdapterNonExistent *operator->() { return this; }
      AdapterNonExistent& operator*() { return *this; }
      operator AdapterNonExistent*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static AdapterNonExistent *_downcast( CORBA::Exception *ex );
      static const AdapterNonExistent *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef AdapterNonExistent AdapterNonExistent_catch;
    #else
    typedef ExceptVar< AdapterNonExistent > AdapterNonExistent_var;
    typedef TVarOut< AdapterNonExistent > AdapterNonExistent_out;
    typedef AdapterNonExistent_var AdapterNonExistent_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_AdapterNonExistent;

    struct InvalidPolicy : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      InvalidPolicy();
      ~InvalidPolicy();
      InvalidPolicy( const InvalidPolicy& s );
      InvalidPolicy& operator=( const InvalidPolicy& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      #ifndef HAVE_EXPLICIT_STRUCT_OPS
      InvalidPolicy();
      #endif //HAVE_EXPLICIT_STRUCT_OPS
      InvalidPolicy( CORBA::UShort _m0 );

      #ifdef HAVE_STD_EH
      InvalidPolicy *operator->() { return this; }
      InvalidPolicy& operator*() { return *this; }
      operator InvalidPolicy*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static InvalidPolicy *_downcast( CORBA::Exception *ex );
      static const InvalidPolicy *_downcast( const CORBA::Exception *ex );
      CORBA::UShort index;
    };

    #ifdef HAVE_STD_EH
    typedef InvalidPolicy InvalidPolicy_catch;
    #else
    typedef ExceptVar< InvalidPolicy > InvalidPolicy_var;
    typedef TVarOut< InvalidPolicy > InvalidPolicy_out;
    typedef InvalidPolicy_var InvalidPolicy_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidPolicy;

    struct NoServant : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      NoServant();
      ~NoServant();
      NoServant( const NoServant& s );
      NoServant& operator=( const NoServant& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      NoServant *operator->() { return this; }
      NoServant& operator*() { return *this; }
      operator NoServant*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static NoServant *_downcast( CORBA::Exception *ex );
      static const NoServant *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef NoServant NoServant_catch;
    #else
    typedef ExceptVar< NoServant > NoServant_var;
    typedef TVarOut< NoServant > NoServant_out;
    typedef NoServant_var NoServant_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_NoServant;

    struct ObjectAlreadyActive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ObjectAlreadyActive();
      ~ObjectAlreadyActive();
      ObjectAlreadyActive( const ObjectAlreadyActive& s );
      ObjectAlreadyActive& operator=( const ObjectAlreadyActive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      ObjectAlreadyActive *operator->() { return this; }
      ObjectAlreadyActive& operator*() { return *this; }
      operator ObjectAlreadyActive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static ObjectAlreadyActive *_downcast( CORBA::Exception *ex );
      static const ObjectAlreadyActive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef ObjectAlreadyActive ObjectAlreadyActive_catch;
    #else
    typedef ExceptVar< ObjectAlreadyActive > ObjectAlreadyActive_var;
    typedef TVarOut< ObjectAlreadyActive > ObjectAlreadyActive_out;
    typedef ObjectAlreadyActive_var ObjectAlreadyActive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectAlreadyActive;

    struct ObjectNotActive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ObjectNotActive();
      ~ObjectNotActive();
      ObjectNotActive( const ObjectNotActive& s );
      ObjectNotActive& operator=( const ObjectNotActive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      ObjectNotActive *operator->() { return this; }
      ObjectNotActive& operator*() { return *this; }
      operator ObjectNotActive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static ObjectNotActive *_downcast( CORBA::Exception *ex );
      static const ObjectNotActive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef ObjectNotActive ObjectNotActive_catch;
    #else
    typedef ExceptVar< ObjectNotActive > ObjectNotActive_var;
    typedef TVarOut< ObjectNotActive > ObjectNotActive_out;
    typedef ObjectNotActive_var ObjectNotActive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectNotActive;

    struct ServantAlreadyActive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ServantAlreadyActive();
      ~ServantAlreadyActive();
      ServantAlreadyActive( const ServantAlreadyActive& s );
      ServantAlreadyActive& operator=( const ServantAlreadyActive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      ServantAlreadyActive *operator->() { return this; }
      ServantAlreadyActive& operator*() { return *this; }
      operator ServantAlreadyActive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static ServantAlreadyActive *_downcast( CORBA::Exception *ex );
      static const ServantAlreadyActive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef ServantAlreadyActive ServantAlreadyActive_catch;
    #else
    typedef ExceptVar< ServantAlreadyActive > ServantAlreadyActive_var;
    typedef TVarOut< ServantAlreadyActive > ServantAlreadyActive_out;
    typedef ServantAlreadyActive_var ServantAlreadyActive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ServantAlreadyActive;

    struct ServantNotActive : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ServantNotActive();
      ~ServantNotActive();
      ServantNotActive( const ServantNotActive& s );
      ServantNotActive& operator=( const ServantNotActive& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      ServantNotActive *operator->() { return this; }
      ServantNotActive& operator*() { return *this; }
      operator ServantNotActive*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static ServantNotActive *_downcast( CORBA::Exception *ex );
      static const ServantNotActive *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef ServantNotActive ServantNotActive_catch;
    #else
    typedef ExceptVar< ServantNotActive > ServantNotActive_var;
    typedef TVarOut< ServantNotActive > ServantNotActive_out;
    typedef ServantNotActive_var ServantNotActive_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ServantNotActive;

    struct WrongAdapter : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      WrongAdapter();
      ~WrongAdapter();
      WrongAdapter( const WrongAdapter& s );
      WrongAdapter& operator=( const WrongAdapter& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      WrongAdapter *operator->() { return this; }
      WrongAdapter& operator*() { return *this; }
      operator WrongAdapter*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static WrongAdapter *_downcast( CORBA::Exception *ex );
      static const WrongAdapter *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef WrongAdapter WrongAdapter_catch;
    #else
    typedef ExceptVar< WrongAdapter > WrongAdapter_var;
    typedef TVarOut< WrongAdapter > WrongAdapter_out;
    typedef WrongAdapter_var WrongAdapter_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_WrongAdapter;

    struct WrongPolicy : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      WrongPolicy();
      ~WrongPolicy();
      WrongPolicy( const WrongPolicy& s );
      WrongPolicy& operator=( const WrongPolicy& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      WrongPolicy *operator->() { return this; }
      WrongPolicy& operator*() { return *this; }
      operator WrongPolicy*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static WrongPolicy *_downcast( CORBA::Exception *ex );
      static const WrongPolicy *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef WrongPolicy WrongPolicy_catch;
    #else
    typedef ExceptVar< WrongPolicy > WrongPolicy_var;
    typedef TVarOut< WrongPolicy > WrongPolicy_out;
    typedef WrongPolicy_var WrongPolicy_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_WrongPolicy;

    virtual char* the_name() = 0;
    virtual ::PortableServer::POA_ptr the_parent() = 0;
    virtual ::PortableServer::POAManager_ptr the_POAManager() = 0;
    virtual ::PortableServer::POAList* the_children() = 0;
    virtual ::PortableServer::AdapterActivator_ptr the_activator() = 0;
    virtual void the_activator( ::PortableServer::AdapterActivator_ptr value ) = 0;
    virtual ::CORBA::OctetSeq* id() = 0;

    virtual POA_ptr create_POA( const char* adapter_name, POAManager_ptr a_POAManager, const ::CORBA::PolicyList& policies ) = 0;
    virtual POA_ptr find_POA( const char* adapter_name, CORBA::Boolean activate_it ) = 0;
    virtual void destroy( CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion ) = 0;
    virtual ThreadPolicy_ptr create_thread_policy( ThreadPolicyValue value ) = 0;
    virtual LifespanPolicy_ptr create_lifespan_policy( LifespanPolicyValue value ) = 0;
    virtual IdUniquenessPolicy_ptr create_id_uniqueness_policy( IdUniquenessPolicyValue value ) = 0;
    virtual IdAssignmentPolicy_ptr create_id_assignment_policy( IdAssignmentPolicyValue value ) = 0;
    virtual ImplicitActivationPolicy_ptr create_implicit_activation_policy( ImplicitActivationPolicyValue value ) = 0;
    virtual ServantRetentionPolicy_ptr create_servant_retention_policy( ServantRetentionPolicyValue value ) = 0;
    virtual RequestProcessingPolicy_ptr create_request_processing_policy( RequestProcessingPolicyValue value ) = 0;
    virtual ServantManager_ptr get_servant_manager() = 0;
    virtual void set_servant_manager( ServantManager_ptr imgr ) = 0;
    virtual Servant get_servant() = 0;
    virtual void set_servant( Servant p_servant ) = 0;
    virtual ObjectId* activate_object( Servant p_servant ) = 0;
    virtual void activate_object_with_id( const ObjectId& id, Servant p_servant ) = 0;
    virtual void deactivate_object( const ObjectId& oid ) = 0;
    virtual CORBA::Object_ptr create_reference( const char* intf ) = 0;
    virtual CORBA::Object_ptr create_reference_with_id( const ObjectId& oid, const char* intf ) = 0;
    virtual ObjectId* servant_to_id( Servant p_servant ) = 0;
    virtual CORBA::Object_ptr servant_to_reference( Servant p_servant ) = 0;
    virtual Servant reference_to_servant( CORBA::Object_ptr reference ) = 0;
    virtual ObjectId* reference_to_id( CORBA::Object_ptr reference ) = 0;
    virtual Servant id_to_servant( const ObjectId& oid ) = 0;
    virtual CORBA::Object_ptr id_to_reference( const ObjectId& oid ) = 0;
    virtual CORBA::Object_ptr activate_for_this( Servant serv ) = 0;
    virtual void poa_manager_callback( POAManager::State newstate, CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion ) = 0;
    virtual Servant preinvoke( CORBA::Object_ptr for_obj ) = 0;
    virtual void postinvoke() = 0;
#ifdef USE_CSL2
    virtual void register_ODM_factory( ::ObjectDomainMapping::Factory_ptr fry ) = 0;
    virtual ::ObjectDomainMapping::Manager_ptr get_ODM() = 0;
#endif // USE_CSL2
  protected:
    POA() {};
  private:
    POA( const POA& );
    void operator=( const POA& );
};


struct ForwardRequest : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest();
  ~ForwardRequest();
  ForwardRequest( const ForwardRequest& s );
  ForwardRequest& operator=( const ForwardRequest& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest( CORBA::Object_ptr _m0 );

  #ifdef HAVE_STD_EH
  ForwardRequest *operator->() { return this; }
  ForwardRequest& operator*() { return *this; }
  operator ForwardRequest*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static ForwardRequest *_downcast( CORBA::Exception *ex );
  static const ForwardRequest *_downcast( const CORBA::Exception *ex );
  CORBA::Object_var forward_reference;
};

#ifdef HAVE_STD_EH
typedef ForwardRequest ForwardRequest_catch;
#else
typedef ExceptVar< ForwardRequest > ForwardRequest_var;
typedef TVarOut< ForwardRequest > ForwardRequest_out;
typedef ForwardRequest_var ForwardRequest_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ForwardRequest;

const ::CORBA::PolicyType THREAD_POLICY_ID = 16;
const ::CORBA::PolicyType LIFESPAN_POLICY_ID = 17;
const ::CORBA::PolicyType ID_UNIQUENESS_POLICY_ID = 18;
const ::CORBA::PolicyType ID_ASSIGNMENT_POLICY_ID = 19;
const ::CORBA::PolicyType IMPLICIT_ACTIVATION_POLICY_ID = 20;
const ::CORBA::PolicyType SERVANT_RETENTION_POLICY_ID = 21;
const ::CORBA::PolicyType REQUEST_PROCESSING_POLICY_ID = 22;

/*
 * Base class and common definitions for interface ThreadPolicy
 */

class ThreadPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~ThreadPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ThreadPolicy_ptr _ptr_type;
    typedef ThreadPolicy_var _var_type;
    #endif

    static ThreadPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static ThreadPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ThreadPolicy_ptr _duplicate( ThreadPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ThreadPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::ThreadPolicyValue value() = 0;

  protected:
    ThreadPolicy() {};
  private:
    ThreadPolicy( const ThreadPolicy& );
    void operator=( const ThreadPolicy& );
};



/*
 * Base class and common definitions for interface LifespanPolicy
 */

class LifespanPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~LifespanPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef LifespanPolicy_ptr _ptr_type;
    typedef LifespanPolicy_var _var_type;
    #endif

    static LifespanPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static LifespanPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static LifespanPolicy_ptr _duplicate( LifespanPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static LifespanPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::LifespanPolicyValue value() = 0;

  protected:
    LifespanPolicy() {};
  private:
    LifespanPolicy( const LifespanPolicy& );
    void operator=( const LifespanPolicy& );
};



/*
 * Base class and common definitions for interface IdUniquenessPolicy
 */

class IdUniquenessPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~IdUniquenessPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IdUniquenessPolicy_ptr _ptr_type;
    typedef IdUniquenessPolicy_var _var_type;
    #endif

    static IdUniquenessPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static IdUniquenessPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IdUniquenessPolicy_ptr _duplicate( IdUniquenessPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IdUniquenessPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::IdUniquenessPolicyValue value() = 0;

  protected:
    IdUniquenessPolicy() {};
  private:
    IdUniquenessPolicy( const IdUniquenessPolicy& );
    void operator=( const IdUniquenessPolicy& );
};



/*
 * Base class and common definitions for interface IdAssignmentPolicy
 */

class IdAssignmentPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~IdAssignmentPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IdAssignmentPolicy_ptr _ptr_type;
    typedef IdAssignmentPolicy_var _var_type;
    #endif

    static IdAssignmentPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static IdAssignmentPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IdAssignmentPolicy_ptr _duplicate( IdAssignmentPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IdAssignmentPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::IdAssignmentPolicyValue value() = 0;

  protected:
    IdAssignmentPolicy() {};
  private:
    IdAssignmentPolicy( const IdAssignmentPolicy& );
    void operator=( const IdAssignmentPolicy& );
};



/*
 * Base class and common definitions for interface ImplicitActivationPolicy
 */

class ImplicitActivationPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~ImplicitActivationPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ImplicitActivationPolicy_ptr _ptr_type;
    typedef ImplicitActivationPolicy_var _var_type;
    #endif

    static ImplicitActivationPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static ImplicitActivationPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ImplicitActivationPolicy_ptr _duplicate( ImplicitActivationPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ImplicitActivationPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::ImplicitActivationPolicyValue value() = 0;

  protected:
    ImplicitActivationPolicy() {};
  private:
    ImplicitActivationPolicy( const ImplicitActivationPolicy& );
    void operator=( const ImplicitActivationPolicy& );
};



/*
 * Base class and common definitions for interface ServantRetentionPolicy
 */

class ServantRetentionPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~ServantRetentionPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServantRetentionPolicy_ptr _ptr_type;
    typedef ServantRetentionPolicy_var _var_type;
    #endif

    static ServantRetentionPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static ServantRetentionPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServantRetentionPolicy_ptr _duplicate( ServantRetentionPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServantRetentionPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::ServantRetentionPolicyValue value() = 0;

  protected:
    ServantRetentionPolicy() {};
  private:
    ServantRetentionPolicy( const ServantRetentionPolicy& );
    void operator=( const ServantRetentionPolicy& );
};



/*
 * Base class and common definitions for interface RequestProcessingPolicy
 */

class RequestProcessingPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~RequestProcessingPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef RequestProcessingPolicy_ptr _ptr_type;
    typedef RequestProcessingPolicy_var _var_type;
    #endif

    static RequestProcessingPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static RequestProcessingPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static RequestProcessingPolicy_ptr _duplicate( RequestProcessingPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static RequestProcessingPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::PortableServer::RequestProcessingPolicyValue value() = 0;

  protected:
    RequestProcessingPolicy() {};
  private:
    RequestProcessingPolicy( const RequestProcessingPolicy& );
    void operator=( const RequestProcessingPolicy& );
};



/*
 * Base class and common definitions for interface AdapterActivator
 */

class AdapterActivator : 
  virtual public CORBA::Object
{
  public:
    virtual ~AdapterActivator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AdapterActivator_ptr _ptr_type;
    typedef AdapterActivator_var _var_type;
    #endif

    static AdapterActivator_ptr _narrow( CORBA::Object_ptr obj );
    static AdapterActivator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AdapterActivator_ptr _duplicate( AdapterActivator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AdapterActivator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Boolean unknown_adapter( POA_ptr parent, const char* name ) = 0;

  protected:
    AdapterActivator() {};
  private:
    AdapterActivator( const AdapterActivator& );
    void operator=( const AdapterActivator& );
};



/*
 * Base class and common definitions for interface ServantManager
 */

class ServantManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~ServantManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServantManager_ptr _ptr_type;
    typedef ServantManager_var _var_type;
    #endif

    static ServantManager_ptr _narrow( CORBA::Object_ptr obj );
    static ServantManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServantManager_ptr _duplicate( ServantManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServantManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ServantManager() {};
  private:
    ServantManager( const ServantManager& );
    void operator=( const ServantManager& );
};



/*
 * Base class and common definitions for interface ServantActivator
 */

class ServantActivator : 
  virtual public ::PortableServer::ServantManager
{
  public:
    virtual ~ServantActivator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServantActivator_ptr _ptr_type;
    typedef ServantActivator_var _var_type;
    #endif

    static ServantActivator_ptr _narrow( CORBA::Object_ptr obj );
    static ServantActivator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServantActivator_ptr _duplicate( ServantActivator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServantActivator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Servant incarnate( const ObjectId& oid, POA_ptr adapter ) = 0;
    virtual void etherealize( const ObjectId& oid, POA_ptr adapter, Servant serv, CORBA::Boolean cleanup_in_progress, CORBA::Boolean remaining_activations ) = 0;

  protected:
    ServantActivator() {};
  private:
    ServantActivator( const ServantActivator& );
    void operator=( const ServantActivator& );
};



/*
 * Base class and common definitions for interface ServantLocator
 */

class ServantLocator : 
  virtual public ::PortableServer::ServantManager
{
  public:
    virtual ~ServantLocator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServantLocator_ptr _ptr_type;
    typedef ServantLocator_var _var_type;
    #endif

    static ServantLocator_ptr _narrow( CORBA::Object_ptr obj );
    static ServantLocator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServantLocator_ptr _duplicate( ServantLocator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServantLocator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    typedef MICO_Native_PortableServer_ServantLocator_Cookie Cookie;
    typedef MICO_Native_PortableServer_ServantLocator_Cookie& Cookie_out;

    virtual Servant preinvoke( const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie_out the_cookie ) = 0;
    virtual void postinvoke( const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie the_cookie, Servant the_servant ) = 0;

  protected:
    ServantLocator() {};
  private:
    ServantLocator( const ServantLocator& );
    void operator=( const ServantLocator& );
};



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

    struct NoContext : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      NoContext();
      ~NoContext();
      NoContext( const NoContext& s );
      NoContext& operator=( const NoContext& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      NoContext *operator->() { return this; }
      NoContext& operator*() { return *this; }
      operator NoContext*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static NoContext *_downcast( CORBA::Exception *ex );
      static const NoContext *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef NoContext NoContext_catch;
    #else
    typedef ExceptVar< NoContext > NoContext_var;
    typedef TVarOut< NoContext > NoContext_out;
    typedef NoContext_var NoContext_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_NoContext;

    virtual POA_ptr get_POA() = 0;
    virtual ObjectId* get_object_id() = 0;
    virtual CORBA::Object_ptr get_reference() = 0;
    virtual Servant get_servant() = 0;
    virtual CORBA::Boolean iscurrent() = 0;

  protected:
    Current() {};
  private:
    Current( const Current& );
    void operator=( const Current& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::PortableServer::ThreadPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::ThreadPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_ThreadPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::LifespanPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::LifespanPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_LifespanPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::IdUniquenessPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::IdUniquenessPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_IdUniquenessPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::IdAssignmentPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::IdAssignmentPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_IdAssignmentPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::ImplicitActivationPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::ImplicitActivationPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_ImplicitActivationPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::ServantRetentionPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::ServantRetentionPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_ServantRetentionPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::RequestProcessingPolicyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::RequestProcessingPolicyValue &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_RequestProcessingPolicyValue;

void operator<<=( CORBA::Any &a, const ::PortableServer::POAManager::AdapterInactive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POAManager::AdapterInactive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POAManager::AdapterInactive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POAManager::AdapterInactive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POAManager_AdapterInactive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POAManager::State &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POAManager::State &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POAManager_State;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::AdapterAlreadyExists &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::AdapterAlreadyExists *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::AdapterAlreadyExists &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::AdapterAlreadyExists *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_AdapterAlreadyExists;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::AdapterInactive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::AdapterInactive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::AdapterInactive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::AdapterInactive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_AdapterInactive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::AdapterNonExistent &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::AdapterNonExistent *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::AdapterNonExistent &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::AdapterNonExistent *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_AdapterNonExistent;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::InvalidPolicy &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::InvalidPolicy *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::InvalidPolicy &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::InvalidPolicy *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_InvalidPolicy;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::NoServant &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::NoServant *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::NoServant &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::NoServant *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_NoServant;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::ObjectAlreadyActive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::ObjectAlreadyActive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::ObjectAlreadyActive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::ObjectAlreadyActive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_ObjectAlreadyActive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::ObjectNotActive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::ObjectNotActive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::ObjectNotActive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::ObjectNotActive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_ObjectNotActive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::ServantAlreadyActive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::ServantAlreadyActive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::ServantAlreadyActive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::ServantAlreadyActive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_ServantAlreadyActive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::ServantNotActive &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::ServantNotActive *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::ServantNotActive &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::ServantNotActive *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_ServantNotActive;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::WrongAdapter &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::WrongAdapter *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::WrongAdapter &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::WrongAdapter *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_WrongAdapter;

void operator<<=( CORBA::Any &a, const ::PortableServer::POA::WrongPolicy &e );
void operator<<=( CORBA::Any &a, ::PortableServer::POA::WrongPolicy *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::POA::WrongPolicy &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::POA::WrongPolicy *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_POA_WrongPolicy;

void operator<<=( CORBA::Any &a, const ::PortableServer::ForwardRequest &e );
void operator<<=( CORBA::Any &a, ::PortableServer::ForwardRequest *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::ForwardRequest &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::ForwardRequest *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_ForwardRequest;

void operator<<=( CORBA::Any &a, const ::PortableServer::Current::NoContext &e );
void operator<<=( CORBA::Any &a, ::PortableServer::Current::NoContext *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableServer::Current::NoContext &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableServer::Current::NoContext *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableServer_Current_NoContext;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< PortableServer::POA_var,PortableServer::POA_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< PortableServer::POA_var,PortableServer::POA_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< PortableServer::POA_var,PortableServer::POA_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< PortableServer::POA_var,PortableServer::POA_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_PortableServer_POA;

#endif
