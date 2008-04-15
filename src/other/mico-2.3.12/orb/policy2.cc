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
namespace MICOPolicy
{
CORBA::TypeCodeConst TransportPrefPolicy::_tc_ProfileTag;
}

namespace MICOPolicy
{
CORBA::TypeCodeConst TransportPrefPolicy::_tc_ProfileTagSeq;
}


/*
 * Base interface for class TransportPrefPolicy
 */

MICOPolicy::TransportPrefPolicy::~TransportPrefPolicy()
{
}

void *
MICOPolicy::TransportPrefPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/MICOPolicy/TransportPrefPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

MICOPolicy::TransportPrefPolicy_ptr
MICOPolicy::TransportPrefPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/MICOPolicy/TransportPrefPolicy:1.0" )))
      return _duplicate( (MICOPolicy::TransportPrefPolicy_ptr) _p );
  }
  return _nil();
}

MICOPolicy::TransportPrefPolicy_ptr
MICOPolicy::TransportPrefPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace MICOPolicy
{
CORBA::TypeCodeConst _tc_TransportPrefPolicy;
}
class _Marshaller_MICOPolicy_TransportPrefPolicy : public ::CORBA::StaticTypeInfo {
    typedef MICOPolicy::TransportPrefPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_MICOPolicy_TransportPrefPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_MICOPolicy_TransportPrefPolicy::~_Marshaller_MICOPolicy_TransportPrefPolicy()
{
}

::CORBA::StaticValueType _Marshaller_MICOPolicy_TransportPrefPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_MICOPolicy_TransportPrefPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::MICOPolicy::TransportPrefPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_MICOPolicy_TransportPrefPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_MICOPolicy_TransportPrefPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_MICOPolicy_TransportPrefPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::MICOPolicy::TransportPrefPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_MICOPolicy_TransportPrefPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_MICOPolicy_TransportPrefPolicy::typecode()
{
  return MICOPolicy::_tc_TransportPrefPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_MICOPolicy_TransportPrefPolicy;

void
operator<<=( CORBA::Any &_a, const MICOPolicy::TransportPrefPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_MICOPolicy_TransportPrefPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, MICOPolicy::TransportPrefPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_MICOPolicy_TransportPrefPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, MICOPolicy::TransportPrefPolicy_ptr &_obj )
{
  MICOPolicy::TransportPrefPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_MICOPolicy_TransportPrefPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


namespace BiDirPolicy
{
CORBA::TypeCodeConst _tc_BidirectionalPolicyValue;
}


/*
 * Base interface for class BidirectionalPolicy
 */

BiDirPolicy::BidirectionalPolicy::~BidirectionalPolicy()
{
}

void *
BiDirPolicy::BidirectionalPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/BiDirPolicy/BidirectionalPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

BiDirPolicy::BidirectionalPolicy_ptr
BiDirPolicy::BidirectionalPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/BiDirPolicy/BidirectionalPolicy:1.0" )))
      return _duplicate( (BiDirPolicy::BidirectionalPolicy_ptr) _p );
  }
  return _nil();
}

BiDirPolicy::BidirectionalPolicy_ptr
BiDirPolicy::BidirectionalPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace BiDirPolicy
{
CORBA::TypeCodeConst _tc_BidirectionalPolicy;
}
class _Marshaller_BiDirPolicy_BidirectionalPolicy : public ::CORBA::StaticTypeInfo {
    typedef BiDirPolicy::BidirectionalPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_BiDirPolicy_BidirectionalPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_BiDirPolicy_BidirectionalPolicy::~_Marshaller_BiDirPolicy_BidirectionalPolicy()
{
}

::CORBA::StaticValueType _Marshaller_BiDirPolicy_BidirectionalPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_BiDirPolicy_BidirectionalPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::BiDirPolicy::BidirectionalPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_BiDirPolicy_BidirectionalPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_BiDirPolicy_BidirectionalPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_BiDirPolicy_BidirectionalPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::BiDirPolicy::BidirectionalPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_BiDirPolicy_BidirectionalPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_BiDirPolicy_BidirectionalPolicy::typecode()
{
  return BiDirPolicy::_tc_BidirectionalPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_BiDirPolicy_BidirectionalPolicy;

void
operator<<=( CORBA::Any &_a, const BiDirPolicy::BidirectionalPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_BiDirPolicy_BidirectionalPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, BiDirPolicy::BidirectionalPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_BiDirPolicy_BidirectionalPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, BiDirPolicy::BidirectionalPolicy_ptr &_obj )
{
  BiDirPolicy::BidirectionalPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_BiDirPolicy_BidirectionalPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


struct __tc_init_POLICY2 {
  __tc_init_POLICY2()
  {
    MICOPolicy::TransportPrefPolicy::_tc_ProfileTag = 
    "010000001500000058000000010000003a00000049444c3a6f6d672e6f72"
    "672f4d49434f506f6c6963792f5472616e73706f727450726566506f6c69"
    "63792f50726f66696c655461673a312e300000000b00000050726f66696c"
    "65546167000005000000";
    MICOPolicy::TransportPrefPolicy::_tc_ProfileTagSeq = 
    "0100000015000000cc000000010000003d00000049444c3a6f6d672e6f72"
    "672f4d49434f506f6c6963792f5472616e73706f727450726566506f6c69"
    "63792f50726f66696c655461675365713a312e30000000000e0000005072"
    "6f66696c6554616753657100000013000000680000000100000015000000"
    "58000000010000003a00000049444c3a6f6d672e6f72672f4d49434f506f"
    "6c6963792f5472616e73706f727450726566506f6c6963792f50726f6669"
    "6c655461673a312e300000000b00000050726f66696c6554616700000500"
    "000000000000";
    MICOPolicy::_tc_TransportPrefPolicy = 
    "010000000e00000050000000010000002f00000049444c3a6f6d672e6f72"
    "672f4d49434f506f6c6963792f5472616e73706f727450726566506f6c69"
    "63793a312e300000140000005472616e73706f727450726566506f6c6963"
    "7900";
    _marshaller_MICOPolicy_TransportPrefPolicy = new _Marshaller_MICOPolicy_TransportPrefPolicy;
    BiDirPolicy::_tc_BidirectionalPolicyValue = 
    "010000001500000064000000010000003500000049444c3a6f6d672e6f72"
    "672f4269446972506f6c6963792f4269646972656374696f6e616c506f6c"
    "69637956616c75653a312e3000000000190000004269646972656374696f"
    "6e616c506f6c69637956616c75650000000004000000";
    BiDirPolicy::_tc_BidirectionalPolicy = 
    "010000000e00000050000000010000003000000049444c3a6f6d672e6f72"
    "672f4269446972506f6c6963792f4269646972656374696f6e616c506f6c"
    "6963793a312e3000140000004269646972656374696f6e616c506f6c6963"
    "7900";
    _marshaller_BiDirPolicy_BidirectionalPolicy = new _Marshaller_BiDirPolicy_BidirectionalPolicy;
  }

  ~__tc_init_POLICY2()
  {
    delete static_cast<_Marshaller_MICOPolicy_TransportPrefPolicy*>(_marshaller_MICOPolicy_TransportPrefPolicy);
    delete static_cast<_Marshaller_BiDirPolicy_BidirectionalPolicy*>(_marshaller_BiDirPolicy_BidirectionalPolicy);
  }
};

static __tc_init_POLICY2 __init_POLICY2;

