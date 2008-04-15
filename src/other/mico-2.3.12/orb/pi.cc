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
namespace IOP
{
CORBA::TypeCodeConst _tc_TaggedComponentSeq;
}

namespace IOP
{
CORBA::TypeCodeConst Codec::_tc_InvalidTypeForEncoding;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::Codec::InvalidTypeForEncoding::InvalidTypeForEncoding()
{
}

IOP::Codec::InvalidTypeForEncoding::InvalidTypeForEncoding( const InvalidTypeForEncoding& _s )
{
}

IOP::Codec::InvalidTypeForEncoding::~InvalidTypeForEncoding()
{
}

IOP::Codec::InvalidTypeForEncoding&
IOP::Codec::InvalidTypeForEncoding::operator=( const InvalidTypeForEncoding& _s )
{
  return *this;
}
#endif

class _Marshaller_IOP_Codec_InvalidTypeForEncoding : public ::CORBA::StaticTypeInfo {
    typedef ::IOP::Codec::InvalidTypeForEncoding _MICO_T;
  public:
    ~_Marshaller_IOP_Codec_InvalidTypeForEncoding();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_Codec_InvalidTypeForEncoding::~_Marshaller_IOP_Codec_InvalidTypeForEncoding()
{
}

::CORBA::StaticValueType _Marshaller_IOP_Codec_InvalidTypeForEncoding::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_Codec_InvalidTypeForEncoding::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_Codec_InvalidTypeForEncoding::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_Codec_InvalidTypeForEncoding::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_IOP_Codec_InvalidTypeForEncoding::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/IOP/Codec/InvalidTypeForEncoding:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_Codec_InvalidTypeForEncoding::typecode()
{
  return IOP::Codec::_tc_InvalidTypeForEncoding;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_Codec_InvalidTypeForEncoding;

void operator<<=( CORBA::Any &_a, const IOP::Codec::InvalidTypeForEncoding &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_InvalidTypeForEncoding, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::Codec::InvalidTypeForEncoding *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::Codec::InvalidTypeForEncoding &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_InvalidTypeForEncoding, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::Codec::InvalidTypeForEncoding *&_e )
{
  return _a.to_static_any (_marshaller_IOP_Codec_InvalidTypeForEncoding, (void *&)_e);
}

void IOP::Codec::InvalidTypeForEncoding::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidTypeForEncoding_var( (IOP::Codec::InvalidTypeForEncoding*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *IOP::Codec::InvalidTypeForEncoding::_repoid() const
{
  return "IDL:omg.org/IOP/Codec/InvalidTypeForEncoding:1.0";
}

void IOP::Codec::InvalidTypeForEncoding::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void IOP::Codec::InvalidTypeForEncoding::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *IOP::Codec::InvalidTypeForEncoding::_clone() const
{
  return new InvalidTypeForEncoding( *this );
}

IOP::Codec::InvalidTypeForEncoding *IOP::Codec::InvalidTypeForEncoding::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/InvalidTypeForEncoding:1.0" ) )
    return (InvalidTypeForEncoding *) _ex;
  return NULL;
}

const IOP::Codec::InvalidTypeForEncoding *IOP::Codec::InvalidTypeForEncoding::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/InvalidTypeForEncoding:1.0" ) )
    return (InvalidTypeForEncoding *) _ex;
  return NULL;
}

namespace IOP
{
CORBA::TypeCodeConst Codec::_tc_FormatMismatch;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::Codec::FormatMismatch::FormatMismatch()
{
}

IOP::Codec::FormatMismatch::FormatMismatch( const FormatMismatch& _s )
{
}

IOP::Codec::FormatMismatch::~FormatMismatch()
{
}

IOP::Codec::FormatMismatch&
IOP::Codec::FormatMismatch::operator=( const FormatMismatch& _s )
{
  return *this;
}
#endif

class _Marshaller_IOP_Codec_FormatMismatch : public ::CORBA::StaticTypeInfo {
    typedef ::IOP::Codec::FormatMismatch _MICO_T;
  public:
    ~_Marshaller_IOP_Codec_FormatMismatch();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_Codec_FormatMismatch::~_Marshaller_IOP_Codec_FormatMismatch()
{
}

::CORBA::StaticValueType _Marshaller_IOP_Codec_FormatMismatch::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_Codec_FormatMismatch::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_Codec_FormatMismatch::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_Codec_FormatMismatch::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_IOP_Codec_FormatMismatch::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/IOP/Codec/FormatMismatch:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_Codec_FormatMismatch::typecode()
{
  return IOP::Codec::_tc_FormatMismatch;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_Codec_FormatMismatch;

void operator<<=( CORBA::Any &_a, const IOP::Codec::FormatMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_FormatMismatch, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::Codec::FormatMismatch *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::Codec::FormatMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_FormatMismatch, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::Codec::FormatMismatch *&_e )
{
  return _a.to_static_any (_marshaller_IOP_Codec_FormatMismatch, (void *&)_e);
}

void IOP::Codec::FormatMismatch::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw FormatMismatch_var( (IOP::Codec::FormatMismatch*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *IOP::Codec::FormatMismatch::_repoid() const
{
  return "IDL:omg.org/IOP/Codec/FormatMismatch:1.0";
}

void IOP::Codec::FormatMismatch::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void IOP::Codec::FormatMismatch::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *IOP::Codec::FormatMismatch::_clone() const
{
  return new FormatMismatch( *this );
}

IOP::Codec::FormatMismatch *IOP::Codec::FormatMismatch::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/FormatMismatch:1.0" ) )
    return (FormatMismatch *) _ex;
  return NULL;
}

const IOP::Codec::FormatMismatch *IOP::Codec::FormatMismatch::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/FormatMismatch:1.0" ) )
    return (FormatMismatch *) _ex;
  return NULL;
}

namespace IOP
{
CORBA::TypeCodeConst Codec::_tc_TypeMismatch;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::Codec::TypeMismatch::TypeMismatch()
{
}

IOP::Codec::TypeMismatch::TypeMismatch( const TypeMismatch& _s )
{
}

IOP::Codec::TypeMismatch::~TypeMismatch()
{
}

IOP::Codec::TypeMismatch&
IOP::Codec::TypeMismatch::operator=( const TypeMismatch& _s )
{
  return *this;
}
#endif

class _Marshaller_IOP_Codec_TypeMismatch : public ::CORBA::StaticTypeInfo {
    typedef ::IOP::Codec::TypeMismatch _MICO_T;
  public:
    ~_Marshaller_IOP_Codec_TypeMismatch();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_Codec_TypeMismatch::~_Marshaller_IOP_Codec_TypeMismatch()
{
}

::CORBA::StaticValueType _Marshaller_IOP_Codec_TypeMismatch::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_Codec_TypeMismatch::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_Codec_TypeMismatch::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_Codec_TypeMismatch::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_IOP_Codec_TypeMismatch::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/IOP/Codec/TypeMismatch:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_Codec_TypeMismatch::typecode()
{
  return IOP::Codec::_tc_TypeMismatch;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_Codec_TypeMismatch;

void operator<<=( CORBA::Any &_a, const IOP::Codec::TypeMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_TypeMismatch, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::Codec::TypeMismatch *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::Codec::TypeMismatch &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Codec_TypeMismatch, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::Codec::TypeMismatch *&_e )
{
  return _a.to_static_any (_marshaller_IOP_Codec_TypeMismatch, (void *&)_e);
}

void IOP::Codec::TypeMismatch::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw TypeMismatch_var( (IOP::Codec::TypeMismatch*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *IOP::Codec::TypeMismatch::_repoid() const
{
  return "IDL:omg.org/IOP/Codec/TypeMismatch:1.0";
}

void IOP::Codec::TypeMismatch::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void IOP::Codec::TypeMismatch::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *IOP::Codec::TypeMismatch::_clone() const
{
  return new TypeMismatch( *this );
}

IOP::Codec::TypeMismatch *IOP::Codec::TypeMismatch::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/TypeMismatch:1.0" ) )
    return (TypeMismatch *) _ex;
  return NULL;
}

const IOP::Codec::TypeMismatch *IOP::Codec::TypeMismatch::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/Codec/TypeMismatch:1.0" ) )
    return (TypeMismatch *) _ex;
  return NULL;
}


/*
 * Base interface for class Codec
 */

IOP::Codec::~Codec()
{
}

void *
IOP::Codec::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/IOP/Codec:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

IOP::Codec_ptr
IOP::Codec::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/IOP/Codec:1.0" )))
      return _duplicate( (IOP::Codec_ptr) _p );
  }
  return _nil();
}

IOP::Codec_ptr
IOP::Codec::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace IOP
{
CORBA::TypeCodeConst _tc_Codec;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_EncodingFormat;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_Encoding;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::Encoding::Encoding()
{
}

IOP::Encoding::Encoding( const Encoding& _s )
{
  format = ((Encoding&)_s).format;
  major_version = ((Encoding&)_s).major_version;
  minor_version = ((Encoding&)_s).minor_version;
}

IOP::Encoding::~Encoding()
{
}

IOP::Encoding&
IOP::Encoding::operator=( const Encoding& _s )
{
  format = ((Encoding&)_s).format;
  major_version = ((Encoding&)_s).major_version;
  minor_version = ((Encoding&)_s).minor_version;
  return *this;
}
#endif

class _Marshaller_IOP_Encoding : public ::CORBA::StaticTypeInfo {
    typedef IOP::Encoding _MICO_T;
  public:
    ~_Marshaller_IOP_Encoding();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_Encoding::~_Marshaller_IOP_Encoding()
{
}

::CORBA::StaticValueType _Marshaller_IOP_Encoding::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_Encoding::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_Encoding::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_Encoding::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_short->demarshal( dc, &((_MICO_T*)v)->format ) &&
    CORBA::_stc_octet->demarshal( dc, &((_MICO_T*)v)->major_version ) &&
    CORBA::_stc_octet->demarshal( dc, &((_MICO_T*)v)->minor_version ) &&
    dc.struct_end();
}

void _Marshaller_IOP_Encoding::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_short->marshal( ec, &((_MICO_T*)v)->format );
  CORBA::_stc_octet->marshal( ec, &((_MICO_T*)v)->major_version );
  CORBA::_stc_octet->marshal( ec, &((_MICO_T*)v)->minor_version );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_Encoding::typecode()
{
  return IOP::_tc_Encoding;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_Encoding;

void operator<<=( CORBA::Any &_a, const IOP::Encoding &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Encoding, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::Encoding *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::Encoding &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_Encoding, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::Encoding *&_s )
{
  return _a.to_static_any (_marshaller_IOP_Encoding, (void *&)_s);
}

namespace IOP
{
CORBA::TypeCodeConst CodecFactory::_tc_UnknownEncoding;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::CodecFactory::UnknownEncoding::UnknownEncoding()
{
}

IOP::CodecFactory::UnknownEncoding::UnknownEncoding( const UnknownEncoding& _s )
{
}

IOP::CodecFactory::UnknownEncoding::~UnknownEncoding()
{
}

IOP::CodecFactory::UnknownEncoding&
IOP::CodecFactory::UnknownEncoding::operator=( const UnknownEncoding& _s )
{
  return *this;
}
#endif

class _Marshaller_IOP_CodecFactory_UnknownEncoding : public ::CORBA::StaticTypeInfo {
    typedef ::IOP::CodecFactory::UnknownEncoding _MICO_T;
  public:
    ~_Marshaller_IOP_CodecFactory_UnknownEncoding();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_CodecFactory_UnknownEncoding::~_Marshaller_IOP_CodecFactory_UnknownEncoding()
{
}

::CORBA::StaticValueType _Marshaller_IOP_CodecFactory_UnknownEncoding::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_CodecFactory_UnknownEncoding::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_CodecFactory_UnknownEncoding::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_CodecFactory_UnknownEncoding::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_IOP_CodecFactory_UnknownEncoding::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/IOP/CodecFactory/UnknownEncoding:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_CodecFactory_UnknownEncoding::typecode()
{
  return IOP::CodecFactory::_tc_UnknownEncoding;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_CodecFactory_UnknownEncoding;

void operator<<=( CORBA::Any &_a, const IOP::CodecFactory::UnknownEncoding &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_CodecFactory_UnknownEncoding, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::CodecFactory::UnknownEncoding *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::CodecFactory::UnknownEncoding &_e )
{
  CORBA::StaticAny _sa (_marshaller_IOP_CodecFactory_UnknownEncoding, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::CodecFactory::UnknownEncoding *&_e )
{
  return _a.to_static_any (_marshaller_IOP_CodecFactory_UnknownEncoding, (void *&)_e);
}

void IOP::CodecFactory::UnknownEncoding::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw UnknownEncoding_var( (IOP::CodecFactory::UnknownEncoding*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *IOP::CodecFactory::UnknownEncoding::_repoid() const
{
  return "IDL:omg.org/IOP/CodecFactory/UnknownEncoding:1.0";
}

void IOP::CodecFactory::UnknownEncoding::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void IOP::CodecFactory::UnknownEncoding::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *IOP::CodecFactory::UnknownEncoding::_clone() const
{
  return new UnknownEncoding( *this );
}

IOP::CodecFactory::UnknownEncoding *IOP::CodecFactory::UnknownEncoding::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/CodecFactory/UnknownEncoding:1.0" ) )
    return (UnknownEncoding *) _ex;
  return NULL;
}

const IOP::CodecFactory::UnknownEncoding *IOP::CodecFactory::UnknownEncoding::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/IOP/CodecFactory/UnknownEncoding:1.0" ) )
    return (UnknownEncoding *) _ex;
  return NULL;
}


/*
 * Base interface for class CodecFactory
 */

IOP::CodecFactory::~CodecFactory()
{
}

void *
IOP::CodecFactory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/IOP/CodecFactory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

IOP::CodecFactory_ptr
IOP::CodecFactory::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/IOP/CodecFactory:1.0" )))
      return _duplicate( (IOP::CodecFactory_ptr) _p );
  }
  return _nil();
}

IOP::CodecFactory_ptr
IOP::CodecFactory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace IOP
{
CORBA::TypeCodeConst _tc_CodecFactory;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_StringSeq;
}

namespace Messaging
{
CORBA::TypeCodeConst _tc_SyncScope;
}

void operator<<=( CORBA::Any &_a, const Messaging::SyncScope &_e )
{
  CORBA::StaticAny _sa (_marshaller_Messaging_SyncScope, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Messaging::SyncScope &_e )
{
  CORBA::StaticAny _sa (_marshaller_Messaging_SyncScope, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Messaging_SyncScope : public ::CORBA::StaticTypeInfo {
    typedef Messaging::SyncScope _MICO_T;
  public:
    ~_Marshaller_Messaging_SyncScope();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Messaging_SyncScope::~_Marshaller_Messaging_SyncScope()
{
}

::CORBA::StaticValueType _Marshaller_Messaging_SyncScope::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Messaging_SyncScope::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Messaging_SyncScope::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Messaging_SyncScope::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Messaging_SyncScope::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Messaging_SyncScope::typecode()
{
  return Messaging::_tc_SyncScope;
}

::CORBA::StaticTypeInfo *_marshaller_Messaging_SyncScope;

namespace Dynamic
{
CORBA::TypeCodeConst _tc_Parameter;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Dynamic::Parameter::Parameter()
{
}

Dynamic::Parameter::Parameter( const Parameter& _s )
{
  argument = ((Parameter&)_s).argument;
  mode = ((Parameter&)_s).mode;
}

Dynamic::Parameter::~Parameter()
{
}

Dynamic::Parameter&
Dynamic::Parameter::operator=( const Parameter& _s )
{
  argument = ((Parameter&)_s).argument;
  mode = ((Parameter&)_s).mode;
  return *this;
}
#endif

class _Marshaller_Dynamic_Parameter : public ::CORBA::StaticTypeInfo {
    typedef Dynamic::Parameter _MICO_T;
  public:
    ~_Marshaller_Dynamic_Parameter();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Dynamic_Parameter::~_Marshaller_Dynamic_Parameter()
{
}

::CORBA::StaticValueType _Marshaller_Dynamic_Parameter::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Dynamic_Parameter::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Dynamic_Parameter::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Dynamic_Parameter::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->argument ) &&
    _marshaller_CORBA_ParameterMode->demarshal( dc, &((_MICO_T*)v)->mode ) &&
    dc.struct_end();
}

void _Marshaller_Dynamic_Parameter::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->argument );
  _marshaller_CORBA_ParameterMode->marshal( ec, &((_MICO_T*)v)->mode );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Dynamic_Parameter::typecode()
{
  return Dynamic::_tc_Parameter;
}

::CORBA::StaticTypeInfo *_marshaller_Dynamic_Parameter;

void operator<<=( CORBA::Any &_a, const Dynamic::Parameter &_s )
{
  CORBA::StaticAny _sa (_marshaller_Dynamic_Parameter, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Dynamic::Parameter *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Dynamic::Parameter &_s )
{
  CORBA::StaticAny _sa (_marshaller_Dynamic_Parameter, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Dynamic::Parameter *&_s )
{
  return _a.to_static_any (_marshaller_Dynamic_Parameter, (void *&)_s);
}

namespace Dynamic
{
CORBA::TypeCodeConst _tc_ParameterList;
}

namespace Dynamic
{
CORBA::TypeCodeConst _tc_ContextList;
}

namespace Dynamic
{
CORBA::TypeCodeConst _tc_ExceptionList;
}

namespace Dynamic
{
CORBA::TypeCodeConst _tc_RequestContext;
}


/*
 * Base interface for class Interceptor
 */

PortableInterceptor::Interceptor::~Interceptor()
{
}

void *
PortableInterceptor::Interceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/Interceptor:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::Interceptor_ptr
PortableInterceptor::Interceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/Interceptor:1.0" )))
      return _duplicate( (PortableInterceptor::Interceptor_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::Interceptor_ptr
PortableInterceptor::Interceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_Interceptor;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ForwardRequest;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::ForwardRequest::ForwardRequest()
{
}

PortableInterceptor::ForwardRequest::ForwardRequest( const ForwardRequest& _s )
{
  forward = ((ForwardRequest&)_s).forward;
  permanent = ((ForwardRequest&)_s).permanent;
}

PortableInterceptor::ForwardRequest::~ForwardRequest()
{
}

PortableInterceptor::ForwardRequest&
PortableInterceptor::ForwardRequest::operator=( const ForwardRequest& _s )
{
  forward = ((ForwardRequest&)_s).forward;
  permanent = ((ForwardRequest&)_s).permanent;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::ForwardRequest::ForwardRequest()
{
}

#endif

PortableInterceptor::ForwardRequest::ForwardRequest( CORBA::Object_ptr _m0, CORBA::Boolean _m1 )
{
  forward = CORBA::Object::_duplicate( _m0 );
  permanent = _m1;
}

class _Marshaller_PortableInterceptor_ForwardRequest : public ::CORBA::StaticTypeInfo {
    typedef ::PortableInterceptor::ForwardRequest _MICO_T;
  public:
    ~_Marshaller_PortableInterceptor_ForwardRequest();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_PortableInterceptor_ForwardRequest::~_Marshaller_PortableInterceptor_ForwardRequest()
{
}

::CORBA::StaticValueType _Marshaller_PortableInterceptor_ForwardRequest::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_PortableInterceptor_ForwardRequest::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_PortableInterceptor_ForwardRequest::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_PortableInterceptor_ForwardRequest::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    CORBA::_stc_Object->demarshal( dc, &((_MICO_T*)v)->forward._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->permanent ) &&
    dc.except_end();
}

void _Marshaller_PortableInterceptor_ForwardRequest::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/PortableInterceptor/ForwardRequest:1.0" );
  CORBA::_stc_Object->marshal( ec, &((_MICO_T*)v)->forward.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->permanent );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_PortableInterceptor_ForwardRequest::typecode()
{
  return PortableInterceptor::_tc_ForwardRequest;
}

::CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ForwardRequest;

void operator<<=( CORBA::Any &_a, const PortableInterceptor::ForwardRequest &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ForwardRequest, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, PortableInterceptor::ForwardRequest *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, PortableInterceptor::ForwardRequest &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ForwardRequest, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const PortableInterceptor::ForwardRequest *&_e )
{
  return _a.to_static_any (_marshaller_PortableInterceptor_ForwardRequest, (void *&)_e);
}

void PortableInterceptor::ForwardRequest::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw ForwardRequest_var( (PortableInterceptor::ForwardRequest*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *PortableInterceptor::ForwardRequest::_repoid() const
{
  return "IDL:omg.org/PortableInterceptor/ForwardRequest:1.0";
}

void PortableInterceptor::ForwardRequest::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void PortableInterceptor::ForwardRequest::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *PortableInterceptor::ForwardRequest::_clone() const
{
  return new ForwardRequest( *this );
}

PortableInterceptor::ForwardRequest *PortableInterceptor::ForwardRequest::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ForwardRequest:1.0" ) )
    return (ForwardRequest *) _ex;
  return NULL;
}

const PortableInterceptor::ForwardRequest *PortableInterceptor::ForwardRequest::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ForwardRequest:1.0" ) )
    return (ForwardRequest *) _ex;
  return NULL;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ReplyStatus;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_SlotId;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_InvalidSlot;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::InvalidSlot::InvalidSlot()
{
}

PortableInterceptor::InvalidSlot::InvalidSlot( const InvalidSlot& _s )
{
}

PortableInterceptor::InvalidSlot::~InvalidSlot()
{
}

PortableInterceptor::InvalidSlot&
PortableInterceptor::InvalidSlot::operator=( const InvalidSlot& _s )
{
  return *this;
}
#endif

class _Marshaller_PortableInterceptor_InvalidSlot : public ::CORBA::StaticTypeInfo {
    typedef ::PortableInterceptor::InvalidSlot _MICO_T;
  public:
    ~_Marshaller_PortableInterceptor_InvalidSlot();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_PortableInterceptor_InvalidSlot::~_Marshaller_PortableInterceptor_InvalidSlot()
{
}

::CORBA::StaticValueType _Marshaller_PortableInterceptor_InvalidSlot::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_PortableInterceptor_InvalidSlot::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_PortableInterceptor_InvalidSlot::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_PortableInterceptor_InvalidSlot::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_PortableInterceptor_InvalidSlot::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/PortableInterceptor/InvalidSlot:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_PortableInterceptor_InvalidSlot::typecode()
{
  return PortableInterceptor::_tc_InvalidSlot;
}

::CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_InvalidSlot;

void operator<<=( CORBA::Any &_a, const PortableInterceptor::InvalidSlot &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_InvalidSlot, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, PortableInterceptor::InvalidSlot *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, PortableInterceptor::InvalidSlot &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_InvalidSlot, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const PortableInterceptor::InvalidSlot *&_e )
{
  return _a.to_static_any (_marshaller_PortableInterceptor_InvalidSlot, (void *&)_e);
}

void PortableInterceptor::InvalidSlot::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidSlot_var( (PortableInterceptor::InvalidSlot*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *PortableInterceptor::InvalidSlot::_repoid() const
{
  return "IDL:omg.org/PortableInterceptor/InvalidSlot:1.0";
}

void PortableInterceptor::InvalidSlot::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void PortableInterceptor::InvalidSlot::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *PortableInterceptor::InvalidSlot::_clone() const
{
  return new InvalidSlot( *this );
}

PortableInterceptor::InvalidSlot *PortableInterceptor::InvalidSlot::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/InvalidSlot:1.0" ) )
    return (InvalidSlot *) _ex;
  return NULL;
}

const PortableInterceptor::InvalidSlot *PortableInterceptor::InvalidSlot::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/InvalidSlot:1.0" ) )
    return (InvalidSlot *) _ex;
  return NULL;
}


/*
 * Base interface for class Current
 */

PortableInterceptor::Current::~Current()
{
}

void *
PortableInterceptor::Current::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/Current:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Current::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::Current_ptr
PortableInterceptor::Current::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/Current:1.0" )))
      return _duplicate( (PortableInterceptor::Current_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::Current_ptr
PortableInterceptor::Current::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_Current;
}


/*
 * Base interface for class RequestInfo
 */

PortableInterceptor::RequestInfo::~RequestInfo()
{
}

void *
PortableInterceptor::RequestInfo::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/RequestInfo:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::RequestInfo_ptr
PortableInterceptor::RequestInfo::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/RequestInfo:1.0" )))
      return _duplicate( (PortableInterceptor::RequestInfo_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::RequestInfo_ptr
PortableInterceptor::RequestInfo::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_RequestInfo;
}


/*
 * Base interface for class ClientRequestInfo
 */

PortableInterceptor::ClientRequestInfo::~ClientRequestInfo()
{
}

void *
PortableInterceptor::ClientRequestInfo::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ClientRequestInfo:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::RequestInfo::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::ClientRequestInfo_ptr
PortableInterceptor::ClientRequestInfo::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ClientRequestInfo:1.0" )))
      return _duplicate( (PortableInterceptor::ClientRequestInfo_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ClientRequestInfo_ptr
PortableInterceptor::ClientRequestInfo::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ClientRequestInfo;
}


/*
 * Base interface for class ServerRequestInfo
 */

PortableInterceptor::ServerRequestInfo::~ServerRequestInfo()
{
}

void *
PortableInterceptor::ServerRequestInfo::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ServerRequestInfo:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::RequestInfo::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::ServerRequestInfo_ptr
PortableInterceptor::ServerRequestInfo::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ServerRequestInfo:1.0" )))
      return _duplicate( (PortableInterceptor::ServerRequestInfo_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ServerRequestInfo_ptr
PortableInterceptor::ServerRequestInfo::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ServerRequestInfo;
}


/*
 * Base interface for class ClientRequestInterceptor
 */

PortableInterceptor::ClientRequestInterceptor::~ClientRequestInterceptor()
{
}

void *
PortableInterceptor::ClientRequestInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ClientRequestInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::Interceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::ClientRequestInterceptor_ptr
PortableInterceptor::ClientRequestInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ClientRequestInterceptor:1.0" )))
      return _duplicate( (PortableInterceptor::ClientRequestInterceptor_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ClientRequestInterceptor_ptr
PortableInterceptor::ClientRequestInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ClientRequestInterceptor;
}


/*
 * Base interface for class ServerRequestInterceptor
 */

PortableInterceptor::ServerRequestInterceptor::~ServerRequestInterceptor()
{
}

void *
PortableInterceptor::ServerRequestInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ServerRequestInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::Interceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::ServerRequestInterceptor_ptr
PortableInterceptor::ServerRequestInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ServerRequestInterceptor:1.0" )))
      return _duplicate( (PortableInterceptor::ServerRequestInterceptor_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ServerRequestInterceptor_ptr
PortableInterceptor::ServerRequestInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ServerRequestInterceptor;
}


/*
 * Base interface for class IORInfo
 */

PortableInterceptor::IORInfo::~IORInfo()
{
}

void *
PortableInterceptor::IORInfo::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/IORInfo:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::IORInfo_ptr
PortableInterceptor::IORInfo::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/IORInfo:1.0" )))
      return _duplicate( (PortableInterceptor::IORInfo_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::IORInfo_ptr
PortableInterceptor::IORInfo::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_IORInfo;
}


/*
 * Base interface for class IORInterceptor
 */

PortableInterceptor::IORInterceptor::~IORInterceptor()
{
}

void *
PortableInterceptor::IORInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/IORInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::Interceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

PortableInterceptor::IORInterceptor_ptr
PortableInterceptor::IORInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/IORInterceptor:1.0" )))
      return _duplicate( (PortableInterceptor::IORInterceptor_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::IORInterceptor_ptr
PortableInterceptor::IORInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_IORInterceptor;
}


/*
 * Base interface for class PolicyFactory
 */

PortableInterceptor::PolicyFactory::~PolicyFactory()
{
}

void *
PortableInterceptor::PolicyFactory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/PolicyFactory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::PolicyFactory_ptr
PortableInterceptor::PolicyFactory::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/PolicyFactory:1.0" )))
      return _duplicate( (PortableInterceptor::PolicyFactory_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::PolicyFactory_ptr
PortableInterceptor::PolicyFactory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_PolicyFactory;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst ORBInitInfo::_tc_ObjectId;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst ORBInitInfo::_tc_DuplicateName;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::ORBInitInfo::DuplicateName::DuplicateName()
{
}

PortableInterceptor::ORBInitInfo::DuplicateName::DuplicateName( const DuplicateName& _s )
{
  name = ((DuplicateName&)_s).name;
}

PortableInterceptor::ORBInitInfo::DuplicateName::~DuplicateName()
{
}

PortableInterceptor::ORBInitInfo::DuplicateName&
PortableInterceptor::ORBInitInfo::DuplicateName::operator=( const DuplicateName& _s )
{
  name = ((DuplicateName&)_s).name;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::ORBInitInfo::DuplicateName::DuplicateName()
{
}

#endif

PortableInterceptor::ORBInitInfo::DuplicateName::DuplicateName( const char* _m0 )
{
  name = _m0;
}

class _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName : public ::CORBA::StaticTypeInfo {
    typedef ::PortableInterceptor::ORBInitInfo::DuplicateName _MICO_T;
  public:
    ~_Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::~_Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName()
{
}

::CORBA::StaticValueType _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    dc.except_end();
}

void _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/PortableInterceptor/ORBInitInfo/DuplicateName:1.0" );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName::typecode()
{
  return PortableInterceptor::ORBInitInfo::_tc_DuplicateName;
}

::CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName;

void operator<<=( CORBA::Any &_a, const PortableInterceptor::ORBInitInfo::DuplicateName &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, PortableInterceptor::ORBInitInfo::DuplicateName *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, PortableInterceptor::ORBInitInfo::DuplicateName &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const PortableInterceptor::ORBInitInfo::DuplicateName *&_e )
{
  return _a.to_static_any (_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName, (void *&)_e);
}

void PortableInterceptor::ORBInitInfo::DuplicateName::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw DuplicateName_var( (PortableInterceptor::ORBInitInfo::DuplicateName*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *PortableInterceptor::ORBInitInfo::DuplicateName::_repoid() const
{
  return "IDL:omg.org/PortableInterceptor/ORBInitInfo/DuplicateName:1.0";
}

void PortableInterceptor::ORBInitInfo::DuplicateName::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void PortableInterceptor::ORBInitInfo::DuplicateName::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *PortableInterceptor::ORBInitInfo::DuplicateName::_clone() const
{
  return new DuplicateName( *this );
}

PortableInterceptor::ORBInitInfo::DuplicateName *PortableInterceptor::ORBInitInfo::DuplicateName::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ORBInitInfo/DuplicateName:1.0" ) )
    return (DuplicateName *) _ex;
  return NULL;
}

const PortableInterceptor::ORBInitInfo::DuplicateName *PortableInterceptor::ORBInitInfo::DuplicateName::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ORBInitInfo/DuplicateName:1.0" ) )
    return (DuplicateName *) _ex;
  return NULL;
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst ORBInitInfo::_tc_InvalidName;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
PortableInterceptor::ORBInitInfo::InvalidName::InvalidName()
{
}

PortableInterceptor::ORBInitInfo::InvalidName::InvalidName( const InvalidName& _s )
{
}

PortableInterceptor::ORBInitInfo::InvalidName::~InvalidName()
{
}

PortableInterceptor::ORBInitInfo::InvalidName&
PortableInterceptor::ORBInitInfo::InvalidName::operator=( const InvalidName& _s )
{
  return *this;
}
#endif

class _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName : public ::CORBA::StaticTypeInfo {
    typedef ::PortableInterceptor::ORBInitInfo::InvalidName _MICO_T;
  public:
    ~_Marshaller_PortableInterceptor_ORBInitInfo_InvalidName();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::~_Marshaller_PortableInterceptor_ORBInitInfo_InvalidName()
{
}

::CORBA::StaticValueType _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/PortableInterceptor/ORBInitInfo/InvalidName:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName::typecode()
{
  return PortableInterceptor::ORBInitInfo::_tc_InvalidName;
}

::CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ORBInitInfo_InvalidName;

void operator<<=( CORBA::Any &_a, const PortableInterceptor::ORBInitInfo::InvalidName &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ORBInitInfo_InvalidName, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, PortableInterceptor::ORBInitInfo::InvalidName *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, PortableInterceptor::ORBInitInfo::InvalidName &_e )
{
  CORBA::StaticAny _sa (_marshaller_PortableInterceptor_ORBInitInfo_InvalidName, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const PortableInterceptor::ORBInitInfo::InvalidName *&_e )
{
  return _a.to_static_any (_marshaller_PortableInterceptor_ORBInitInfo_InvalidName, (void *&)_e);
}

void PortableInterceptor::ORBInitInfo::InvalidName::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidName_var( (PortableInterceptor::ORBInitInfo::InvalidName*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *PortableInterceptor::ORBInitInfo::InvalidName::_repoid() const
{
  return "IDL:omg.org/PortableInterceptor/ORBInitInfo/InvalidName:1.0";
}

void PortableInterceptor::ORBInitInfo::InvalidName::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void PortableInterceptor::ORBInitInfo::InvalidName::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *PortableInterceptor::ORBInitInfo::InvalidName::_clone() const
{
  return new InvalidName( *this );
}

PortableInterceptor::ORBInitInfo::InvalidName *PortableInterceptor::ORBInitInfo::InvalidName::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ORBInitInfo/InvalidName:1.0" ) )
    return (InvalidName *) _ex;
  return NULL;
}

const PortableInterceptor::ORBInitInfo::InvalidName *PortableInterceptor::ORBInitInfo::InvalidName::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/PortableInterceptor/ORBInitInfo/InvalidName:1.0" ) )
    return (InvalidName *) _ex;
  return NULL;
}


/*
 * Base interface for class ORBInitInfo
 */

PortableInterceptor::ORBInitInfo::~ORBInitInfo()
{
}

void *
PortableInterceptor::ORBInitInfo::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ORBInitInfo:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::ORBInitInfo_ptr
PortableInterceptor::ORBInitInfo::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ORBInitInfo:1.0" )))
      return _duplicate( (PortableInterceptor::ORBInitInfo_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ORBInitInfo_ptr
PortableInterceptor::ORBInitInfo::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ORBInitInfo;
}


/*
 * Base interface for class ORBInitializer
 */

PortableInterceptor::ORBInitializer::~ORBInitializer()
{
}

void *
PortableInterceptor::ORBInitializer::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/PortableInterceptor/ORBInitializer:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

PortableInterceptor::ORBInitializer_ptr
PortableInterceptor::ORBInitializer::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/PortableInterceptor/ORBInitializer:1.0" )))
      return _duplicate( (PortableInterceptor::ORBInitializer_ptr) _p );
  }
  return _nil();
}

PortableInterceptor::ORBInitializer_ptr
PortableInterceptor::ORBInitializer::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace PortableInterceptor
{
CORBA::TypeCodeConst _tc_ORBInitializer;
}

class _Marshaller__seq_Dynamic_Parameter : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Dynamic_Parameter();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Dynamic_Parameter::~_Marshaller__seq_Dynamic_Parameter()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Dynamic_Parameter::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Dynamic_Parameter::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Dynamic_Parameter::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Dynamic_Parameter::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Dynamic_Parameter->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Dynamic_Parameter::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Dynamic_Parameter->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Dynamic_Parameter::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000ec000000010000000f000000dc000000010000002200"
    "000049444c3a6f6d672e6f72672f44796e616d69632f506172616d657465"
    "723a312e300000000a000000506172616d65746572000000020000000900"
    "0000617267756d656e74000000000b000000050000006d6f646500000000"
    "1100000074000000010000002400000049444c3a6f6d672e6f72672f434f"
    "5242412f506172616d657465724d6f64653a312e30000e00000050617261"
    "6d657465724d6f64650000000300000009000000504152414d5f494e0000"
    "00000a000000504152414d5f4f55540000000c000000504152414d5f494e"
    "4f55540000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Dynamic_Parameter::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Dynamic_Parameter;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Dynamic_Parameter, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Dynamic_Parameter, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Dynamic_Parameter, (void *&)_s);
}


struct __tc_init_PI {
  __tc_init_PI()
  {
    IOP::_tc_TaggedComponentSeq = 
    "010000001500000008010000010000002700000049444c3a6f6d672e6f72"
    "672f494f502f546167676564436f6d706f6e656e745365713a312e300000"
    "13000000546167676564436f6d706f6e656e74536571000013000000b800"
    "0000010000000f000000a8000000010000001c00000049444c3a494f502f"
    "546167676564436f6d706f6e656e743a312e300010000000546167676564"
    "436f6d706f6e656e74000200000004000000746167001500000034000000"
    "010000001800000049444c3a494f502f436f6d706f6e656e7449643a312e"
    "30000c000000436f6d706f6e656e74496400050000000f000000636f6d70"
    "6f6e656e745f646174610000130000000c000000010000000a0000000000"
    "000000000000";
    IOP::Codec::_tc_InvalidTypeForEncoding = 
    "01000000160000005c000000010000003100000049444c3a6f6d672e6f72"
    "672f494f502f436f6465632f496e76616c696454797065466f72456e636f"
    "64696e673a312e300000000017000000496e76616c696454797065466f72"
    "456e636f64696e67000000000000";
    _marshaller_IOP_Codec_InvalidTypeForEncoding = new _Marshaller_IOP_Codec_InvalidTypeForEncoding;
    IOP::Codec::_tc_FormatMismatch = 
    "01000000160000004c000000010000002900000049444c3a6f6d672e6f72"
    "672f494f502f436f6465632f466f726d61744d69736d617463683a312e30"
    "000000000f000000466f726d61744d69736d61746368000000000000";
    _marshaller_IOP_Codec_FormatMismatch = new _Marshaller_IOP_Codec_FormatMismatch;
    IOP::Codec::_tc_TypeMismatch = 
    "010000001600000048000000010000002700000049444c3a6f6d672e6f72"
    "672f494f502f436f6465632f547970654d69736d617463683a312e300000"
    "0d000000547970654d69736d617463680000000000000000";
    _marshaller_IOP_Codec_TypeMismatch = new _Marshaller_IOP_Codec_TypeMismatch;
    IOP::_tc_Codec = 
    "010000000e0000002e000000010000001a00000049444c3a6f6d672e6f72"
    "672f494f502f436f6465633a312e3000000006000000436f64656300";
    IOP::_tc_EncodingFormat = 
    "010000001500000044000000010000002300000049444c3a6f6d672e6f72"
    "672f494f502f456e636f64696e67466f726d61743a312e3000000f000000"
    "456e636f64696e67466f726d6174000002000000";
    IOP::_tc_Encoding = 
    "010000000f000000c4000000010000001d00000049444c3a6f6d672e6f72"
    "672f494f502f456e636f64696e673a312e300000000009000000456e636f"
    "64696e67000000000300000007000000666f726d61740000150000004400"
    "0000010000002300000049444c3a6f6d672e6f72672f494f502f456e636f"
    "64696e67466f726d61743a312e3000000f000000456e636f64696e67466f"
    "726d61740000020000000e0000006d616a6f725f76657273696f6e000000"
    "0a0000000e0000006d696e6f725f76657273696f6e0000000a000000";
    _marshaller_IOP_Encoding = new _Marshaller_IOP_Encoding;
    IOP::CodecFactory::_tc_UnknownEncoding = 
    "010000001600000054000000010000003100000049444c3a6f6d672e6f72"
    "672f494f502f436f646563466163746f72792f556e6b6e6f776e456e636f"
    "64696e673a312e300000000010000000556e6b6e6f776e456e636f64696e"
    "670000000000";
    _marshaller_IOP_CodecFactory_UnknownEncoding = new _Marshaller_IOP_CodecFactory_UnknownEncoding;
    IOP::_tc_CodecFactory = 
    "010000000e0000003d000000010000002100000049444c3a6f6d672e6f72"
    "672f494f502f436f646563466163746f72793a312e30000000000d000000"
    "436f646563466163746f727900";
    CORBA::_tc_StringSeq = 
    "010000001500000050000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f537472696e675365713a312e30000a00000053747269"
    "6e6753657100000013000000100000000100000012000000000000000000"
    "0000";
    Messaging::_tc_SyncScope = 
    "010000001100000095000000010000002400000049444c3a6f6d672e6f72"
    "672f4d6573736167696e672f53796e6353636f70653a312e30000a000000"
    "53796e6353636f7065000000040000000a00000053594e435f4e4f4e4500"
    "00001400000053594e435f574954485f5452414e53504f52540011000000"
    "53594e435f574954485f534552564552000000001100000053594e435f57"
    "4954485f54415247455400";
    _marshaller_Messaging_SyncScope = new _Marshaller_Messaging_SyncScope;
    Dynamic::_tc_Parameter = 
    "010000000f000000dc000000010000002200000049444c3a6f6d672e6f72"
    "672f44796e616d69632f506172616d657465723a312e300000000a000000"
    "506172616d657465720000000200000009000000617267756d656e740000"
    "00000b000000050000006d6f646500000000110000007400000001000000"
    "2400000049444c3a6f6d672e6f72672f434f5242412f506172616d657465"
    "724d6f64653a312e30000e000000506172616d657465724d6f6465000000"
    "0300000009000000504152414d5f494e000000000a000000504152414d5f"
    "4f55540000000c000000504152414d5f494e4f555400";
    _marshaller_Dynamic_Parameter = new _Marshaller_Dynamic_Parameter;
    Dynamic::_tc_ParameterList = 
    "010000001500000038010000010000002600000049444c3a6f6d672e6f72"
    "672f44796e616d69632f506172616d657465724c6973743a312e30000000"
    "0e000000506172616d657465724c69737400000013000000ec0000000100"
    "00000f000000dc000000010000002200000049444c3a6f6d672e6f72672f"
    "44796e616d69632f506172616d657465723a312e300000000a0000005061"
    "72616d657465720000000200000009000000617267756d656e7400000000"
    "0b000000050000006d6f6465000000001100000074000000010000002400"
    "000049444c3a6f6d672e6f72672f434f5242412f506172616d657465724d"
    "6f64653a312e30000e000000506172616d657465724d6f64650000000300"
    "000009000000504152414d5f494e000000000a000000504152414d5f4f55"
    "540000000c000000504152414d5f494e4f55540000000000";
    Dynamic::_tc_ContextList = 
    "010000001500000094000000010000002400000049444c3a6f6d672e6f72"
    "672f44796e616d69632f436f6e746578744c6973743a312e30000c000000"
    "436f6e746578744c69737400150000005000000001000000200000004944"
    "4c3a6f6d672e6f72672f434f5242412f537472696e675365713a312e3000"
    "0a000000537472696e675365710000001300000010000000010000001200"
    "00000000000000000000";
    Dynamic::_tc_ExceptionList = 
    "01000000150000009c000000010000002600000049444c3a6f6d672e6f72"
    "672f44796e616d69632f457863657074696f6e4c6973743a312e30000000"
    "0e000000457863657074696f6e4c69737400000015000000500000000100"
    "00002200000049444c3a6f6d672e6f72672f434f5242412f54797065436f"
    "64655365713a312e300000000c00000054797065436f6465536571001300"
    "00000c000000010000000c00000000000000";
    Dynamic::_tc_RequestContext = 
    "01000000150000009c000000010000002700000049444c3a6f6d672e6f72"
    "672f44796e616d69632f52657175657374436f6e746578743a312e300000"
    "0f00000052657175657374436f6e74657874000015000000500000000100"
    "00002000000049444c3a6f6d672e6f72672f434f5242412f537472696e67"
    "5365713a312e30000a000000537472696e67536571000000130000001000"
    "000001000000120000000000000000000000";
    PortableInterceptor::_tc_Interceptor = 
    "010000000e00000048000000010000003000000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f496e746572636570"
    "746f723a312e30000c000000496e746572636570746f7200";
    PortableInterceptor::_tc_ForwardRequest = 
    "0100000016000000b0000000010000003300000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f466f727761726452"
    "6571756573743a312e3000000f000000466f727761726452657175657374"
    "00000200000008000000666f7277617264000e0000003300000001000000"
    "1d00000049444c3a6f6d672e6f72672f434f5242412f4f626a6563743a31"
    "2e3000000000070000004f626a65637400000a0000007065726d616e656e"
    "7400000008000000";
    _marshaller_PortableInterceptor_ForwardRequest = new _Marshaller_PortableInterceptor_ForwardRequest;
    PortableInterceptor::_tc_ReplyStatus = 
    "01000000150000004c000000010000003000000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f5265706c79537461"
    "7475733a312e30000c0000005265706c795374617475730002000000";
    PortableInterceptor::_tc_SlotId = 
    "010000001500000044000000010000002b00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f536c6f7449643a31"
    "2e30000007000000536c6f744964000005000000";
    PortableInterceptor::_tc_InvalidSlot = 
    "01000000160000004c000000010000003000000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f496e76616c696453"
    "6c6f743a312e30000c000000496e76616c6964536c6f740000000000";
    _marshaller_PortableInterceptor_InvalidSlot = new _Marshaller_PortableInterceptor_InvalidSlot;
    PortableInterceptor::_tc_Current = 
    "010000000e00000040000000010000002c00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f43757272656e743a"
    "312e30000800000043757272656e7400";
    PortableInterceptor::_tc_RequestInfo = 
    "010000000e00000048000000010000003000000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f5265717565737449"
    "6e666f3a312e30000c00000052657175657374496e666f00";
    PortableInterceptor::_tc_ClientRequestInfo = 
    "010000000e00000056000000010000003600000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f436c69656e745265"
    "7175657374496e666f3a312e3000000012000000436c69656e7452657175"
    "657374496e666f00";
    PortableInterceptor::_tc_ServerRequestInfo = 
    "010000000e00000056000000010000003600000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f5365727665725265"
    "7175657374496e666f3a312e300000001200000053657276657252657175"
    "657374496e666f00";
    PortableInterceptor::_tc_ClientRequestInterceptor = 
    "010000000e00000065000000010000003d00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f436c69656e745265"
    "7175657374496e746572636570746f723a312e300000000019000000436c"
    "69656e7452657175657374496e746572636570746f7200";
    PortableInterceptor::_tc_ServerRequestInterceptor = 
    "010000000e00000065000000010000003d00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f5365727665725265"
    "7175657374496e746572636570746f723a312e3000000000190000005365"
    "7276657252657175657374496e746572636570746f7200";
    PortableInterceptor::_tc_IORInfo = 
    "010000000e00000040000000010000002c00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f494f52496e666f3a"
    "312e300008000000494f52496e666f00";
    PortableInterceptor::_tc_IORInterceptor = 
    "010000000e0000004f000000010000003300000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f494f52496e746572"
    "636570746f723a312e3000000f000000494f52496e746572636570746f72"
    "00";
    PortableInterceptor::_tc_PolicyFactory = 
    "010000000e0000004e000000010000003200000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f506f6c6963794661"
    "63746f72793a312e300000000e000000506f6c696379466163746f727900"
    ;
    PortableInterceptor::ORBInitInfo::_tc_ObjectId = 
    "01000000150000005c000000010000003900000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f4f5242496e697449"
    "6e666f2f4f626a65637449643a312e3000000000090000004f626a656374"
    "4964000000001200000000000000";
    PortableInterceptor::ORBInitInfo::_tc_DuplicateName = 
    "010000001600000074000000010000003e00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f4f5242496e697449"
    "6e666f2f4475706c69636174654e616d653a312e300000000e0000004475"
    "706c69636174654e616d6500000001000000050000006e616d6500000000"
    "1200000000000000";
    _marshaller_PortableInterceptor_ORBInitInfo_DuplicateName = new _Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName;
    PortableInterceptor::ORBInitInfo::_tc_InvalidName = 
    "010000001600000058000000010000003c00000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f4f5242496e697449"
    "6e666f2f496e76616c69644e616d653a312e30000c000000496e76616c69"
    "644e616d650000000000";
    _marshaller_PortableInterceptor_ORBInitInfo_InvalidName = new _Marshaller_PortableInterceptor_ORBInitInfo_InvalidName;
    PortableInterceptor::_tc_ORBInitInfo = 
    "010000000e00000048000000010000003000000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f4f5242496e697449"
    "6e666f3a312e30000c0000004f5242496e6974496e666f00";
    PortableInterceptor::_tc_ORBInitializer = 
    "010000000e0000004f000000010000003300000049444c3a6f6d672e6f72"
    "672f506f727461626c65496e746572636570746f722f4f5242496e697469"
    "616c697a65723a312e3000000f0000004f5242496e697469616c697a6572"
    "00";
    _marshaller__seq_Dynamic_Parameter = new _Marshaller__seq_Dynamic_Parameter;
  }

  ~__tc_init_PI()
  {
    delete static_cast<_Marshaller_IOP_Codec_InvalidTypeForEncoding*>(_marshaller_IOP_Codec_InvalidTypeForEncoding);
    delete static_cast<_Marshaller_IOP_Codec_FormatMismatch*>(_marshaller_IOP_Codec_FormatMismatch);
    delete static_cast<_Marshaller_IOP_Codec_TypeMismatch*>(_marshaller_IOP_Codec_TypeMismatch);
    delete static_cast<_Marshaller_IOP_Encoding*>(_marshaller_IOP_Encoding);
    delete static_cast<_Marshaller_IOP_CodecFactory_UnknownEncoding*>(_marshaller_IOP_CodecFactory_UnknownEncoding);
    delete static_cast<_Marshaller_Messaging_SyncScope*>(_marshaller_Messaging_SyncScope);
    delete static_cast<_Marshaller_Dynamic_Parameter*>(_marshaller_Dynamic_Parameter);
    delete static_cast<_Marshaller_PortableInterceptor_ForwardRequest*>(_marshaller_PortableInterceptor_ForwardRequest);
    delete static_cast<_Marshaller_PortableInterceptor_InvalidSlot*>(_marshaller_PortableInterceptor_InvalidSlot);
    delete static_cast<_Marshaller_PortableInterceptor_ORBInitInfo_DuplicateName*>(_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName);
    delete static_cast<_Marshaller_PortableInterceptor_ORBInitInfo_InvalidName*>(_marshaller_PortableInterceptor_ORBInitInfo_InvalidName);
    delete static_cast<_Marshaller__seq_Dynamic_Parameter*>(_marshaller__seq_Dynamic_Parameter);
  }
};

static __tc_init_PI __init_PI;

