/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __POLICY2_H__
#define __POLICY2_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace MICOPolicy
{

class TransportPrefPolicy;
typedef TransportPrefPolicy *TransportPrefPolicy_ptr;
typedef TransportPrefPolicy_ptr TransportPrefPolicyRef;
typedef ObjVar< TransportPrefPolicy > TransportPrefPolicy_var;
typedef ObjOut< TransportPrefPolicy > TransportPrefPolicy_out;

}


namespace BiDirPolicy
{

class BidirectionalPolicy;
typedef BidirectionalPolicy *BidirectionalPolicy_ptr;
typedef BidirectionalPolicy_ptr BidirectionalPolicyRef;
typedef ObjVar< BidirectionalPolicy > BidirectionalPolicy_var;
typedef ObjOut< BidirectionalPolicy > BidirectionalPolicy_out;

}






namespace MICOPolicy
{

const ::CORBA::PolicyType TRANSPORTPREF_POLICY_TYPE = 1000;

/*
 * Base class and common definitions for interface TransportPrefPolicy
 */

class TransportPrefPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~TransportPrefPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef TransportPrefPolicy_ptr _ptr_type;
    typedef TransportPrefPolicy_var _var_type;
    #endif

    static TransportPrefPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static TransportPrefPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static TransportPrefPolicy_ptr _duplicate( TransportPrefPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static TransportPrefPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    typedef CORBA::ULong ProfileTag;
    typedef ProfileTag& ProfileTag_out;
    static MICO_EXPORT CORBA::TypeCodeConst _tc_ProfileTag;

    typedef SequenceTmpl< ProfileTag,MICO_TID_DEF> ProfileTagSeq;
    typedef TSeqVar< SequenceTmpl< ProfileTag,MICO_TID_DEF> > ProfileTagSeq_var;
    typedef TSeqOut< SequenceTmpl< ProfileTag,MICO_TID_DEF> > ProfileTagSeq_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ProfileTagSeq;

    virtual ::MICOPolicy::TransportPrefPolicy::ProfileTagSeq* preferences() = 0;
    virtual void preferences( const ::MICOPolicy::TransportPrefPolicy::ProfileTagSeq& value ) = 0;
    virtual ::MICOPolicy::TransportPrefPolicy::ProfileTagSeq* preferences_nocopy() = 0;

  protected:
    TransportPrefPolicy() {};
  private:
    TransportPrefPolicy( const TransportPrefPolicy& );
    void operator=( const TransportPrefPolicy& );
};


}


namespace BiDirPolicy
{

typedef CORBA::UShort BidirectionalPolicyValue;
typedef BidirectionalPolicyValue& BidirectionalPolicyValue_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_BidirectionalPolicyValue;

const BidirectionalPolicyValue NORMAL = 0;
const BidirectionalPolicyValue BOTH = 1;
const ::CORBA::PolicyType BIDIRECTIONAL_POLICY_TYPE = 37;

/*
 * Base class and common definitions for interface BidirectionalPolicy
 */

class BidirectionalPolicy : 
  virtual public ::CORBA::Policy
{
  public:
    virtual ~BidirectionalPolicy();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef BidirectionalPolicy_ptr _ptr_type;
    typedef BidirectionalPolicy_var _var_type;
    #endif

    static BidirectionalPolicy_ptr _narrow( CORBA::Object_ptr obj );
    static BidirectionalPolicy_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static BidirectionalPolicy_ptr _duplicate( BidirectionalPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static BidirectionalPolicy_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::BiDirPolicy::BidirectionalPolicyValue value() = 0;

  protected:
    BidirectionalPolicy() {};
  private:
    BidirectionalPolicy( const BidirectionalPolicy& );
    void operator=( const BidirectionalPolicy& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
