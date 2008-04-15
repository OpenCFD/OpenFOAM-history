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
namespace DynamicAny
{
CORBA::TypeCodeConst DynAny::_tc_InvalidValue;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
DynamicAny::DynAny::InvalidValue::InvalidValue()
{
}

DynamicAny::DynAny::InvalidValue::InvalidValue( const InvalidValue& _s )
{
}

DynamicAny::DynAny::InvalidValue::~InvalidValue()
{
}

DynamicAny::DynAny::InvalidValue&
DynamicAny::DynAny::InvalidValue::operator=( const InvalidValue& _s )
{
  return *this;
}
#endif

class _Marshaller_DynamicAny_DynAny_InvalidValue : public ::CORBA::StaticTypeInfo {
    typedef ::DynamicAny::DynAny::InvalidValue _MICO_T;
  public:
    ~_Marshaller_DynamicAny_DynAny_InvalidValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_DynamicAny_DynAny_InvalidValue::~_Marshaller_DynamicAny_DynAny_InvalidValue()
{
}

::CORBA::StaticValueType _Marshaller_DynamicAny_DynAny_InvalidValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_DynamicAny_DynAny_InvalidValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_DynamicAny_DynAny_InvalidValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_DynamicAny_DynAny_InvalidValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_DynamicAny_DynAny_InvalidValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/DynamicAny/DynAny/InvalidValue:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_DynamicAny_DynAny_InvalidValue::typecode()
{
  return DynamicAny::DynAny::_tc_InvalidValue;
}

::CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAny_InvalidValue;

void operator<<=( CORBA::Any &_a, const DynamicAny::DynAny::InvalidValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAny_InvalidValue, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, DynamicAny::DynAny::InvalidValue *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, DynamicAny::DynAny::InvalidValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAny_InvalidValue, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const DynamicAny::DynAny::InvalidValue *&_e )
{
  return _a.to_static_any (_marshaller_DynamicAny_DynAny_InvalidValue, (void *&)_e);
}

void DynamicAny::DynAny::InvalidValue::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidValue_var( (DynamicAny::DynAny::InvalidValue*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *DynamicAny::DynAny::InvalidValue::_repoid() const
{
  return "IDL:omg.org/DynamicAny/DynAny/InvalidValue:1.0";
}

void DynamicAny::DynAny::InvalidValue::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void DynamicAny::DynAny::InvalidValue::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *DynamicAny::DynAny::InvalidValue::_clone() const
{
  return new InvalidValue( *this );
}

DynamicAny::DynAny::InvalidValue *DynamicAny::DynAny::InvalidValue::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAny/InvalidValue:1.0" ) )
    return (InvalidValue *) _ex;
  return NULL;
}

const DynamicAny::DynAny::InvalidValue *DynamicAny::DynAny::InvalidValue::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAny/InvalidValue:1.0" ) )
    return (InvalidValue *) _ex;
  return NULL;
}

namespace DynamicAny
{
CORBA::TypeCodeConst DynAny::_tc_TypeMismatch;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
DynamicAny::DynAny::TypeMismatch::TypeMismatch()
{
}

DynamicAny::DynAny::TypeMismatch::TypeMismatch( const TypeMismatch& _s )
{
}

DynamicAny::DynAny::TypeMismatch::~TypeMismatch()
{
}

DynamicAny::DynAny::TypeMismatch&
DynamicAny::DynAny::TypeMismatch::operator=( const TypeMismatch& _s )
{
  return *this;
}
#endif

class _Marshaller_DynamicAny_DynAny_TypeMismatch : public ::CORBA::StaticTypeInfo {
    typedef ::DynamicAny::DynAny::TypeMismatch _MICO_T;
  public:
    ~_Marshaller_DynamicAny_DynAny_TypeMismatch();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_DynamicAny_DynAny_TypeMismatch::~_Marshaller_DynamicAny_DynAny_TypeMismatch()
{
}

::CORBA::StaticValueType _Marshaller_DynamicAny_DynAny_TypeMismatch::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_DynamicAny_DynAny_TypeMismatch::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_DynamicAny_DynAny_TypeMismatch::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_DynamicAny_DynAny_TypeMismatch::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_DynamicAny_DynAny_TypeMismatch::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/DynamicAny/DynAny/TypeMismatch:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_DynamicAny_DynAny_TypeMismatch::typecode()
{
  return DynamicAny::DynAny::_tc_TypeMismatch;
}

::CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAny_TypeMismatch;

void operator<<=( CORBA::Any &_a, const DynamicAny::DynAny::TypeMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAny_TypeMismatch, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, DynamicAny::DynAny::TypeMismatch *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, DynamicAny::DynAny::TypeMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAny_TypeMismatch, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const DynamicAny::DynAny::TypeMismatch *&_e )
{
  return _a.to_static_any (_marshaller_DynamicAny_DynAny_TypeMismatch, (void *&)_e);
}

void DynamicAny::DynAny::TypeMismatch::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw TypeMismatch_var( (DynamicAny::DynAny::TypeMismatch*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *DynamicAny::DynAny::TypeMismatch::_repoid() const
{
  return "IDL:omg.org/DynamicAny/DynAny/TypeMismatch:1.0";
}

void DynamicAny::DynAny::TypeMismatch::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void DynamicAny::DynAny::TypeMismatch::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *DynamicAny::DynAny::TypeMismatch::_clone() const
{
  return new TypeMismatch( *this );
}

DynamicAny::DynAny::TypeMismatch *DynamicAny::DynAny::TypeMismatch::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAny/TypeMismatch:1.0" ) )
    return (TypeMismatch *) _ex;
  return NULL;
}

const DynamicAny::DynAny::TypeMismatch *DynamicAny::DynAny::TypeMismatch::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAny/TypeMismatch:1.0" ) )
    return (TypeMismatch *) _ex;
  return NULL;
}


/*
 * Base interface for class DynAny
 */

DynamicAny::DynAny::~DynAny()
{
}

void *
DynamicAny::DynAny::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynAny:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

DynamicAny::DynAny_ptr
DynamicAny::DynAny::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynAny:1.0" )))
      return _duplicate( (DynamicAny::DynAny_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynAny_ptr
DynamicAny::DynAny::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynAny;
}


/*
 * Base interface for class DynBasic
 */

DynamicAny::DynBasic::~DynBasic()
{
}

void *
DynamicAny::DynBasic::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynBasic:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynBasic_ptr
DynamicAny::DynBasic::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynBasic:1.0" )))
      return _duplicate( (DynamicAny::DynBasic_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynBasic_ptr
DynamicAny::DynBasic::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynBasic;
}


/*
 * Base interface for class DynFixed
 */

DynamicAny::DynFixed::~DynFixed()
{
}

void *
DynamicAny::DynFixed::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynFixed:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynFixed_ptr
DynamicAny::DynFixed::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynFixed:1.0" )))
      return _duplicate( (DynamicAny::DynFixed_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynFixed_ptr
DynamicAny::DynFixed::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynFixed;
}


/*
 * Base interface for class DynEnum
 */

DynamicAny::DynEnum::~DynEnum()
{
}

void *
DynamicAny::DynEnum::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynEnum:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynEnum_ptr
DynamicAny::DynEnum::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynEnum:1.0" )))
      return _duplicate( (DynamicAny::DynEnum_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynEnum_ptr
DynamicAny::DynEnum::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynEnum;
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_FieldName;
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_NameValuePair;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
DynamicAny::NameValuePair::NameValuePair()
{
}

DynamicAny::NameValuePair::NameValuePair( const NameValuePair& _s )
{
  id = ((NameValuePair&)_s).id;
  value = ((NameValuePair&)_s).value;
}

DynamicAny::NameValuePair::~NameValuePair()
{
}

DynamicAny::NameValuePair&
DynamicAny::NameValuePair::operator=( const NameValuePair& _s )
{
  id = ((NameValuePair&)_s).id;
  value = ((NameValuePair&)_s).value;
  return *this;
}
#endif

class _Marshaller_DynamicAny_NameValuePair : public ::CORBA::StaticTypeInfo {
    typedef DynamicAny::NameValuePair _MICO_T;
  public:
    ~_Marshaller_DynamicAny_NameValuePair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_DynamicAny_NameValuePair::~_Marshaller_DynamicAny_NameValuePair()
{
}

::CORBA::StaticValueType _Marshaller_DynamicAny_NameValuePair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_DynamicAny_NameValuePair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_DynamicAny_NameValuePair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_DynamicAny_NameValuePair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_DynamicAny_NameValuePair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_DynamicAny_NameValuePair::typecode()
{
  return DynamicAny::_tc_NameValuePair;
}

::CORBA::StaticTypeInfo *_marshaller_DynamicAny_NameValuePair;

void operator<<=( CORBA::Any &_a, const DynamicAny::NameValuePair &_s )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_NameValuePair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, DynamicAny::NameValuePair *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, DynamicAny::NameValuePair &_s )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_NameValuePair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const DynamicAny::NameValuePair *&_s )
{
  return _a.to_static_any (_marshaller_DynamicAny_NameValuePair, (void *&)_s);
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_NameValuePairSeq;
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_NameDynAnyPair;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
DynamicAny::NameDynAnyPair::NameDynAnyPair()
{
}

DynamicAny::NameDynAnyPair::NameDynAnyPair( const NameDynAnyPair& _s )
{
  id = ((NameDynAnyPair&)_s).id;
  value = ((NameDynAnyPair&)_s).value;
}

DynamicAny::NameDynAnyPair::~NameDynAnyPair()
{
}

DynamicAny::NameDynAnyPair&
DynamicAny::NameDynAnyPair::operator=( const NameDynAnyPair& _s )
{
  id = ((NameDynAnyPair&)_s).id;
  value = ((NameDynAnyPair&)_s).value;
  return *this;
}
#endif

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_NameDynAnyPairSeq;
}


/*
 * Base interface for class DynStruct
 */

DynamicAny::DynStruct::~DynStruct()
{
}

void *
DynamicAny::DynStruct::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynStruct:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynStruct_ptr
DynamicAny::DynStruct::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynStruct:1.0" )))
      return _duplicate( (DynamicAny::DynStruct_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynStruct_ptr
DynamicAny::DynStruct::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynStruct;
}


/*
 * Base interface for class DynUnion
 */

DynamicAny::DynUnion::~DynUnion()
{
}

void *
DynamicAny::DynUnion::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynUnion:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynUnion_ptr
DynamicAny::DynUnion::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynUnion:1.0" )))
      return _duplicate( (DynamicAny::DynUnion_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynUnion_ptr
DynamicAny::DynUnion::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynUnion;
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_AnySeq;
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynAnySeq;
}


/*
 * Base interface for class DynSequence
 */

DynamicAny::DynSequence::~DynSequence()
{
}

void *
DynamicAny::DynSequence::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynSequence:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynSequence_ptr
DynamicAny::DynSequence::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynSequence:1.0" )))
      return _duplicate( (DynamicAny::DynSequence_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynSequence_ptr
DynamicAny::DynSequence::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynSequence;
}


/*
 * Base interface for class DynArray
 */

DynamicAny::DynArray::~DynArray()
{
}

void *
DynamicAny::DynArray::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynArray:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynArray_ptr
DynamicAny::DynArray::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynArray:1.0" )))
      return _duplicate( (DynamicAny::DynArray_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynArray_ptr
DynamicAny::DynArray::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynArray;
}


/*
 * Base interface for class DynValueCommon
 */

DynamicAny::DynValueCommon::~DynValueCommon()
{
}

void *
DynamicAny::DynValueCommon::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynValueCommon:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynAny::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynValueCommon_ptr
DynamicAny::DynValueCommon::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynValueCommon:1.0" )))
      return _duplicate( (DynamicAny::DynValueCommon_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynValueCommon_ptr
DynamicAny::DynValueCommon::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynValueCommon;
}


/*
 * Base interface for class DynValue
 */

DynamicAny::DynValue::~DynValue()
{
}

void *
DynamicAny::DynValue::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynValue:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynValueCommon::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynValue_ptr
DynamicAny::DynValue::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynValue:1.0" )))
      return _duplicate( (DynamicAny::DynValue_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynValue_ptr
DynamicAny::DynValue::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynValue;
}


/*
 * Base interface for class DynValueBox
 */

DynamicAny::DynValueBox::~DynValueBox()
{
}

void *
DynamicAny::DynValueBox::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynValueBox:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = DynamicAny::DynValueCommon::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

DynamicAny::DynValueBox_ptr
DynamicAny::DynValueBox::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynValueBox:1.0" )))
      return _duplicate( (DynamicAny::DynValueBox_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynValueBox_ptr
DynamicAny::DynValueBox::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynValueBox;
}

namespace DynamicAny
{
CORBA::TypeCodeConst DynAnyFactory::_tc_InconsistentTypeCode;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
DynamicAny::DynAnyFactory::InconsistentTypeCode::InconsistentTypeCode()
{
}

DynamicAny::DynAnyFactory::InconsistentTypeCode::InconsistentTypeCode( const InconsistentTypeCode& _s )
{
}

DynamicAny::DynAnyFactory::InconsistentTypeCode::~InconsistentTypeCode()
{
}

DynamicAny::DynAnyFactory::InconsistentTypeCode&
DynamicAny::DynAnyFactory::InconsistentTypeCode::operator=( const InconsistentTypeCode& _s )
{
  return *this;
}
#endif

class _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode : public ::CORBA::StaticTypeInfo {
    typedef ::DynamicAny::DynAnyFactory::InconsistentTypeCode _MICO_T;
  public:
    ~_Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::~_Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode()
{
}

::CORBA::StaticValueType _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/DynamicAny/DynAnyFactory/InconsistentTypeCode:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode::typecode()
{
  return DynamicAny::DynAnyFactory::_tc_InconsistentTypeCode;
}

::CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode;

void operator<<=( CORBA::Any &_a, const DynamicAny::DynAnyFactory::InconsistentTypeCode &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, DynamicAny::DynAnyFactory::InconsistentTypeCode *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, DynamicAny::DynAnyFactory::InconsistentTypeCode &_e )
{
  CORBA::StaticAny _sa (_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const DynamicAny::DynAnyFactory::InconsistentTypeCode *&_e )
{
  return _a.to_static_any (_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode, (void *&)_e);
}

void DynamicAny::DynAnyFactory::InconsistentTypeCode::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InconsistentTypeCode_var( (DynamicAny::DynAnyFactory::InconsistentTypeCode*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *DynamicAny::DynAnyFactory::InconsistentTypeCode::_repoid() const
{
  return "IDL:omg.org/DynamicAny/DynAnyFactory/InconsistentTypeCode:1.0";
}

void DynamicAny::DynAnyFactory::InconsistentTypeCode::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void DynamicAny::DynAnyFactory::InconsistentTypeCode::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *DynamicAny::DynAnyFactory::InconsistentTypeCode::_clone() const
{
  return new InconsistentTypeCode( *this );
}

DynamicAny::DynAnyFactory::InconsistentTypeCode *DynamicAny::DynAnyFactory::InconsistentTypeCode::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAnyFactory/InconsistentTypeCode:1.0" ) )
    return (InconsistentTypeCode *) _ex;
  return NULL;
}

const DynamicAny::DynAnyFactory::InconsistentTypeCode *DynamicAny::DynAnyFactory::InconsistentTypeCode::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/DynamicAny/DynAnyFactory/InconsistentTypeCode:1.0" ) )
    return (InconsistentTypeCode *) _ex;
  return NULL;
}


/*
 * Base interface for class DynAnyFactory
 */

DynamicAny::DynAnyFactory::~DynAnyFactory()
{
}

void *
DynamicAny::DynAnyFactory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/DynamicAny/DynAnyFactory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

DynamicAny::DynAnyFactory_ptr
DynamicAny::DynAnyFactory::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/DynamicAny/DynAnyFactory:1.0" )))
      return _duplicate( (DynamicAny::DynAnyFactory_ptr) _p );
  }
  return _nil();
}

DynamicAny::DynAnyFactory_ptr
DynamicAny::DynAnyFactory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace DynamicAny
{
CORBA::TypeCodeConst _tc_DynAnyFactory;
}

class _Marshaller__seq_DynamicAny_NameValuePair : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_DynamicAny_NameValuePair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_DynamicAny_NameValuePair::~_Marshaller__seq_DynamicAny_NameValuePair()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_DynamicAny_NameValuePair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_DynamicAny_NameValuePair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_DynamicAny_NameValuePair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_DynamicAny_NameValuePair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_DynamicAny_NameValuePair->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_DynamicAny_NameValuePair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_DynamicAny_NameValuePair->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_DynamicAny_NameValuePair::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000c4000000010000000f000000b4000000010000002900"
    "000049444c3a6f6d672e6f72672f44796e616d6963416e792f4e616d6556"
    "616c7565506169723a312e30000000000e0000004e616d6556616c756550"
    "616972000000020000000300000069640000150000004800000001000000"
    "2500000049444c3a6f6d672e6f72672f44796e616d6963416e792f466965"
    "6c644e616d653a312e30000000000a0000004669656c644e616d65000000"
    "12000000000000000600000076616c75650000000b00000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_DynamicAny_NameValuePair::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_DynamicAny_NameValuePair;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_DynamicAny_NameValuePair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_DynamicAny_NameValuePair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_DynamicAny_NameValuePair, (void *&)_s);
}


struct __tc_init_DYNANY {
  __tc_init_DYNANY()
  {
    DynamicAny::DynAny::_tc_InvalidValue = 
    "010000001600000050000000010000002f00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e792f496e76616c696456616c"
    "75653a312e3000000d000000496e76616c696456616c7565000000000000"
    "0000";
    _marshaller_DynamicAny_DynAny_InvalidValue = new _Marshaller_DynamicAny_DynAny_InvalidValue;
    DynamicAny::DynAny::_tc_TypeMismatch = 
    "010000001600000050000000010000002f00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e792f547970654d69736d6174"
    "63683a312e3000000d000000547970654d69736d61746368000000000000"
    "0000";
    _marshaller_DynamicAny_DynAny_TypeMismatch = new _Marshaller_DynamicAny_DynAny_TypeMismatch;
    DynamicAny::_tc_DynAny = 
    "010000000e00000037000000010000002200000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e793a312e3000000007000000"
    "44796e416e7900";
    DynamicAny::_tc_DynBasic = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e42617369633a312e300009000000"
    "44796e426173696300";
    DynamicAny::_tc_DynFixed = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e46697865643a312e300009000000"
    "44796e466978656400";
    DynamicAny::_tc_DynEnum = 
    "010000000e00000038000000010000002300000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e456e756d3a312e30000008000000"
    "44796e456e756d00";
    DynamicAny::_tc_FieldName = 
    "010000001500000048000000010000002500000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f4669656c644e616d653a312e3000000000"
    "0a0000004669656c644e616d650000001200000000000000";
    DynamicAny::_tc_NameValuePair = 
    "010000000f000000b4000000010000002900000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f4e616d6556616c7565506169723a312e30"
    "000000000e0000004e616d6556616c756550616972000000020000000300"
    "0000696400001500000048000000010000002500000049444c3a6f6d672e"
    "6f72672f44796e616d6963416e792f4669656c644e616d653a312e300000"
    "00000a0000004669656c644e616d65000000120000000000000006000000"
    "76616c75650000000b000000";
    _marshaller_DynamicAny_NameValuePair = new _Marshaller_DynamicAny_NameValuePair;
    DynamicAny::_tc_NameValuePairSeq = 
    "010000001500000018010000010000002c00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f4e616d6556616c7565506169725365713a"
    "312e3000110000004e616d6556616c756550616972536571000000001300"
    "0000c4000000010000000f000000b4000000010000002900000049444c3a"
    "6f6d672e6f72672f44796e616d6963416e792f4e616d6556616c75655061"
    "69723a312e30000000000e0000004e616d6556616c756550616972000000"
    "020000000300000069640000150000004800000001000000250000004944"
    "4c3a6f6d672e6f72672f44796e616d6963416e792f4669656c644e616d65"
    "3a312e30000000000a0000004669656c644e616d65000000120000000000"
    "00000600000076616c75650000000b00000000000000";
    DynamicAny::_tc_NameDynAnyPair = 
    "010000000f000000ef000000010000002a00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f4e616d6544796e416e79506169723a312e"
    "300000000f0000004e616d6544796e416e79506169720000020000000300"
    "0000696400001500000048000000010000002500000049444c3a6f6d672e"
    "6f72672f44796e616d6963416e792f4669656c644e616d653a312e300000"
    "00000a0000004669656c644e616d65000000120000000000000006000000"
    "76616c75650000000e00000037000000010000002200000049444c3a6f6d"
    "672e6f72672f44796e616d6963416e792f44796e416e793a312e30000000"
    "0700000044796e416e7900";
    DynamicAny::_tc_NameDynAnyPairSeq = 
    "010000001500000058010000010000002d00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f4e616d6544796e416e7950616972536571"
    "3a312e3000000000120000004e616d6544796e416e795061697253657100"
    "00001300000000010000010000000f000000ef000000010000002a000000"
    "49444c3a6f6d672e6f72672f44796e616d6963416e792f4e616d6544796e"
    "416e79506169723a312e300000000f0000004e616d6544796e416e795061"
    "697200000200000003000000696400001500000048000000010000002500"
    "000049444c3a6f6d672e6f72672f44796e616d6963416e792f4669656c64"
    "4e616d653a312e30000000000a0000004669656c644e616d650000001200"
    "0000000000000600000076616c75650000000e0000003700000001000000"
    "2200000049444c3a6f6d672e6f72672f44796e616d6963416e792f44796e"
    "416e793a312e300000000700000044796e416e79000000000000";
    DynamicAny::_tc_DynStruct = 
    "010000000e0000003e000000010000002500000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e5374727563743a312e3000000000"
    "0a00000044796e53747275637400";
    DynamicAny::_tc_DynUnion = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e556e696f6e3a312e300009000000"
    "44796e556e696f6e00";
    DynamicAny::_tc_AnySeq = 
    "01000000150000004c000000010000002200000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f416e795365713a312e3000000007000000"
    "416e795365710000130000000c000000010000000b00000000000000";
    DynamicAny::_tc_DynAnySeq = 
    "010000001500000090000000010000002500000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e795365713a312e3000000000"
    "0a00000044796e416e795365710000001300000048000000010000000e00"
    "000037000000010000002200000049444c3a6f6d672e6f72672f44796e61"
    "6d6963416e792f44796e416e793a312e300000000700000044796e416e79"
    "000000000000";
    DynamicAny::_tc_DynSequence = 
    "010000000e00000040000000010000002700000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e53657175656e63653a312e300000"
    "0c00000044796e53657175656e636500";
    DynamicAny::_tc_DynArray = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e41727261793a312e300009000000"
    "44796e417272617900";
    DynamicAny::_tc_DynValueCommon = 
    "010000000e00000047000000010000002a00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e56616c7565436f6d6d6f6e3a312e"
    "300000000f00000044796e56616c7565436f6d6d6f6e00";
    DynamicAny::_tc_DynValue = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e56616c75653a312e300009000000"
    "44796e56616c756500";
    DynamicAny::_tc_DynValueBox = 
    "010000000e00000040000000010000002700000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e56616c7565426f783a312e300000"
    "0c00000044796e56616c7565426f7800";
    DynamicAny::DynAnyFactory::_tc_InconsistentTypeCode = 
    "010000001600000068000000010000003e00000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e79466163746f72792f496e63"
    "6f6e73697374656e7454797065436f64653a312e3000000015000000496e"
    "636f6e73697374656e7454797065436f64650000000000000000";
    _marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode = new _Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode;
    DynamicAny::_tc_DynAnyFactory = 
    "010000000e00000046000000010000002900000049444c3a6f6d672e6f72"
    "672f44796e616d6963416e792f44796e416e79466163746f72793a312e30"
    "000000000e00000044796e416e79466163746f727900";
    _marshaller__seq_DynamicAny_NameValuePair = new _Marshaller__seq_DynamicAny_NameValuePair;
  }

  ~__tc_init_DYNANY()
  {
    delete static_cast<_Marshaller_DynamicAny_DynAny_InvalidValue*>(_marshaller_DynamicAny_DynAny_InvalidValue);
    delete static_cast<_Marshaller_DynamicAny_DynAny_TypeMismatch*>(_marshaller_DynamicAny_DynAny_TypeMismatch);
    delete static_cast<_Marshaller_DynamicAny_NameValuePair*>(_marshaller_DynamicAny_NameValuePair);
    delete static_cast<_Marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode*>(_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode);
    delete static_cast<_Marshaller__seq_DynamicAny_NameValuePair*>(_marshaller__seq_DynamicAny_NameValuePair);
  }
};

static __tc_init_DYNANY __init_DYNANY;

