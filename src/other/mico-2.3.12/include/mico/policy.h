/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __POLICY_H__
#define __POLICY_H__


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

class Policy;
//typedef Policy *Policy_ptr;
typedef Policy_ptr PolicyRef;
//typedef ObjVar< Policy > Policy_var;
//typedef ObjOut< Policy > Policy_out;

class DomainManager;
//typedef DomainManager *DomainManager_ptr;
typedef DomainManager_ptr DomainManagerRef;
//typedef ObjVar< DomainManager > DomainManager_var;
//typedef ObjOut< DomainManager > DomainManager_out;

class ConstructionPolicy;
typedef ConstructionPolicy *ConstructionPolicy_ptr;
typedef ConstructionPolicy_ptr ConstructionPolicyRef;
typedef ObjVar< ConstructionPolicy > ConstructionPolicy_var;
typedef ObjOut< ConstructionPolicy > ConstructionPolicy_out;

}






namespace CORBA
{

//typedef CORBA::ULong PolicyType;
typedef PolicyType& PolicyType_out;

/*
 * Base class and common definitions for interface Policy
 */

class Policy : 
  virtual public CORBA::Object
{
  public:
    virtual ~Policy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Policy_ptr _ptr_type;
    typedef Policy_var _var_type;
    #endif

    static Policy_ptr _narrow( CORBA::Object_ptr obj );
    static Policy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Policy_ptr _duplicate( Policy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Policy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::PolicyType policy_type() = 0;

    virtual Policy_ptr copy() = 0;
    virtual void destroy() = 0;

  protected:
    Policy() {};
  private:
    Policy( const Policy& );
    void operator=( const Policy& );
};


//typedef IfaceSequenceTmpl< Policy_var,Policy_ptr> PolicyList;
typedef TSeqVar< IfaceSequenceTmpl< Policy_var,Policy_ptr> > PolicyList_var;
typedef TSeqOut< IfaceSequenceTmpl< Policy_var,Policy_ptr> > PolicyList_out;

//enum SetOverrideType {
//  SET_OVERRIDE = 0,
//  ADD_OVERIDE
//};

typedef SetOverrideType& SetOverrideType_out;


/*
 * Base class and common definitions for interface DomainManager
 */

class DomainManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~DomainManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainManager_ptr _ptr_type;
    typedef DomainManager_var _var_type;
    #endif

    static DomainManager_ptr _narrow( CORBA::Object_ptr obj );
    static DomainManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainManager_ptr _duplicate( DomainManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Policy_ptr get_domain_policy( PolicyType policy_type ) = 0;
    virtual void set_domain_policy( Policy_ptr policy ) = 0;
    virtual DomainManager_ptr copy() = 0;

  protected:
    DomainManager() {};
  private:
    DomainManager( const DomainManager& );
    void operator=( const DomainManager& );
};


const PolicyType SecConstruction = 11;

/*
 * Base class and common definitions for interface ConstructionPolicy
 */

class ConstructionPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~ConstructionPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ConstructionPolicy_ptr _ptr_type;
    typedef ConstructionPolicy_var _var_type;
    #endif

    static ConstructionPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static ConstructionPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ConstructionPolicy_ptr _duplicate( ConstructionPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ConstructionPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void make_domain_manager( InterfaceDef_ptr object_type, CORBA::Boolean constr_policy ) = 0;
    virtual CORBA::Boolean constr_policy( CORBA::Object_ptr object ) = 0;

  protected:
    ConstructionPolicy() {};
  private:
    ConstructionPolicy( const ConstructionPolicy& );
    void operator=( const ConstructionPolicy& );
};


//typedef IfaceSequenceTmpl< DomainManager_var,DomainManager_ptr> DomainManagerList;
typedef TSeqVar< IfaceSequenceTmpl< DomainManager_var,DomainManager_ptr> > DomainManagerList_var;
typedef TSeqOut< IfaceSequenceTmpl< DomainManager_var,DomainManager_ptr> > DomainManagerList_out;

typedef SequenceTmpl< PolicyType,MICO_TID_DEF> PolicyTypeSeq;
typedef TSeqVar< SequenceTmpl< PolicyType,MICO_TID_DEF> > PolicyTypeSeq_var;
typedef TSeqOut< SequenceTmpl< PolicyType,MICO_TID_DEF> > PolicyTypeSeq_out;

typedef CORBA::Short PolicyErrorCode;
typedef PolicyErrorCode& PolicyErrorCode_out;
const PolicyErrorCode BAD_POLICY = 0;
const PolicyErrorCode UNSUPPORTED_POLICY = 1;
const PolicyErrorCode BAD_POLICY_TYPE = 2;
const PolicyErrorCode BAD_POLICY_VALUE = 3;
const PolicyErrorCode UNSUPPORTED_POLICY_VALUE = 4;
struct PolicyError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  PolicyError();
  ~PolicyError();
  PolicyError( const PolicyError& s );
  PolicyError& operator=( const PolicyError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  PolicyError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  PolicyError( ::CORBA::PolicyErrorCode _m0 );

  #ifdef HAVE_STD_EH
  PolicyError *operator->() { return this; }
  PolicyError& operator*() { return *this; }
  operator PolicyError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static PolicyError *_downcast( CORBA::Exception *ex );
  static const PolicyError *_downcast( const CORBA::Exception *ex );
  PolicyErrorCode reason;
};

#ifdef HAVE_STD_EH
typedef PolicyError PolicyError_catch;
#else
typedef ExceptVar< PolicyError > PolicyError_var;
typedef TVarOut< PolicyError > PolicyError_out;
typedef PolicyError_var PolicyError_catch;
#endif // HAVE_STD_EH

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
