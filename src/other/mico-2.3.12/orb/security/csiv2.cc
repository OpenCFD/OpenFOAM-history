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
namespace CSIv2
{
CORBA::TypeCodeConst _tc_DistinguishedName;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_DistinguishedNameList;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_UserIdentityList;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_GSSAuthError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIv2::GSSAuthError::GSSAuthError()
{
}

CSIv2::GSSAuthError::GSSAuthError( const GSSAuthError& _s )
{
  reason = ((GSSAuthError&)_s).reason;
}

CSIv2::GSSAuthError::~GSSAuthError()
{
}

CSIv2::GSSAuthError&
CSIv2::GSSAuthError::operator=( const GSSAuthError& _s )
{
  reason = ((GSSAuthError&)_s).reason;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
CSIv2::GSSAuthError::GSSAuthError()
{
}

#endif

CSIv2::GSSAuthError::GSSAuthError( GSSUP::ErrorCode _m0 )
{
  reason = _m0;
}

class _Marshaller_CSIv2_GSSAuthError : public ::CORBA::StaticTypeInfo {
    typedef ::CSIv2::GSSAuthError _MICO_T;
  public:
    ~_Marshaller_CSIv2_GSSAuthError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIv2_GSSAuthError::~_Marshaller_CSIv2_GSSAuthError()
{
}

::CORBA::StaticValueType _Marshaller_CSIv2_GSSAuthError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIv2_GSSAuthError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIv2_GSSAuthError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIv2_GSSAuthError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->reason ) &&
    dc.except_end();
}

void _Marshaller_CSIv2_GSSAuthError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:CSIv2/GSSAuthError:1.0" );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->reason );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIv2_GSSAuthError::typecode()
{
  return CSIv2::_tc_GSSAuthError;
}

::CORBA::StaticTypeInfo *_marshaller_CSIv2_GSSAuthError;

void operator<<=( CORBA::Any &_a, const CSIv2::GSSAuthError &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_GSSAuthError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIv2::GSSAuthError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIv2::GSSAuthError &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_GSSAuthError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIv2::GSSAuthError *&_e )
{
  return _a.to_static_any (_marshaller_CSIv2_GSSAuthError, (void *&)_e);
}

void CSIv2::GSSAuthError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw GSSAuthError_var( (CSIv2::GSSAuthError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *CSIv2::GSSAuthError::_repoid() const
{
  return "IDL:CSIv2/GSSAuthError:1.0";
}

void CSIv2::GSSAuthError::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void CSIv2::GSSAuthError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *CSIv2::GSSAuthError::_clone() const
{
  return new GSSAuthError( *this );
}

CSIv2::GSSAuthError *CSIv2::GSSAuthError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/GSSAuthError:1.0" ) )
    return (GSSAuthError *) _ex;
  return NULL;
}

const CSIv2::GSSAuthError *CSIv2::GSSAuthError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/GSSAuthError:1.0" ) )
    return (GSSAuthError *) _ex;
  return NULL;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_TrustIdentityError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIv2::TrustIdentityError::TrustIdentityError()
{
}

CSIv2::TrustIdentityError::TrustIdentityError( const TrustIdentityError& _s )
{
}

CSIv2::TrustIdentityError::~TrustIdentityError()
{
}

CSIv2::TrustIdentityError&
CSIv2::TrustIdentityError::operator=( const TrustIdentityError& _s )
{
  return *this;
}
#endif

class _Marshaller_CSIv2_TrustIdentityError : public ::CORBA::StaticTypeInfo {
    typedef ::CSIv2::TrustIdentityError _MICO_T;
  public:
    ~_Marshaller_CSIv2_TrustIdentityError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIv2_TrustIdentityError::~_Marshaller_CSIv2_TrustIdentityError()
{
}

::CORBA::StaticValueType _Marshaller_CSIv2_TrustIdentityError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIv2_TrustIdentityError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIv2_TrustIdentityError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIv2_TrustIdentityError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_CSIv2_TrustIdentityError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:CSIv2/TrustIdentityError:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIv2_TrustIdentityError::typecode()
{
  return CSIv2::_tc_TrustIdentityError;
}

::CORBA::StaticTypeInfo *_marshaller_CSIv2_TrustIdentityError;

void operator<<=( CORBA::Any &_a, const CSIv2::TrustIdentityError &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_TrustIdentityError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIv2::TrustIdentityError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIv2::TrustIdentityError &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_TrustIdentityError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIv2::TrustIdentityError *&_e )
{
  return _a.to_static_any (_marshaller_CSIv2_TrustIdentityError, (void *&)_e);
}

void CSIv2::TrustIdentityError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw TrustIdentityError_var( (CSIv2::TrustIdentityError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *CSIv2::TrustIdentityError::_repoid() const
{
  return "IDL:CSIv2/TrustIdentityError:1.0";
}

void CSIv2::TrustIdentityError::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void CSIv2::TrustIdentityError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *CSIv2::TrustIdentityError::_clone() const
{
  return new TrustIdentityError( *this );
}

CSIv2::TrustIdentityError *CSIv2::TrustIdentityError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/TrustIdentityError:1.0" ) )
    return (TrustIdentityError *) _ex;
  return NULL;
}

const CSIv2::TrustIdentityError *CSIv2::TrustIdentityError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/TrustIdentityError:1.0" ) )
    return (TrustIdentityError *) _ex;
  return NULL;
}


/*
 * Base interface for class SecurityManager
 */

CSIv2::SecurityManager::~SecurityManager()
{
}

void *
CSIv2::SecurityManager::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/SecurityManager:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CSIv2::SecurityManager_ptr
CSIv2::SecurityManager::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/SecurityManager:1.0" )))
      return _duplicate( (CSIv2::SecurityManager_ptr) _p );
  }
  return _nil();
}

CSIv2::SecurityManager_ptr
CSIv2::SecurityManager::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_SecurityManager;
}


/*
 * Base interface for class CMSetup
 */

CSIv2::CMSetup::~CMSetup()
{
}

void *
CSIv2::CMSetup::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/CMSetup:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CSIv2::CMSetup_ptr
CSIv2::CMSetup::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/CMSetup:1.0" )))
      return _duplicate( (CSIv2::CMSetup_ptr) _p );
  }
  return _nil();
}

CSIv2::CMSetup_ptr
CSIv2::CMSetup::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_CMSetup;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_InvalidMechanism;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIv2::InvalidMechanism::InvalidMechanism()
{
}

CSIv2::InvalidMechanism::InvalidMechanism( const InvalidMechanism& _s )
{
}

CSIv2::InvalidMechanism::~InvalidMechanism()
{
}

CSIv2::InvalidMechanism&
CSIv2::InvalidMechanism::operator=( const InvalidMechanism& _s )
{
  return *this;
}
#endif

class _Marshaller_CSIv2_InvalidMechanism : public ::CORBA::StaticTypeInfo {
    typedef ::CSIv2::InvalidMechanism _MICO_T;
  public:
    ~_Marshaller_CSIv2_InvalidMechanism();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIv2_InvalidMechanism::~_Marshaller_CSIv2_InvalidMechanism()
{
}

::CORBA::StaticValueType _Marshaller_CSIv2_InvalidMechanism::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIv2_InvalidMechanism::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIv2_InvalidMechanism::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIv2_InvalidMechanism::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_CSIv2_InvalidMechanism::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:CSIv2/InvalidMechanism:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIv2_InvalidMechanism::typecode()
{
  return CSIv2::_tc_InvalidMechanism;
}

::CORBA::StaticTypeInfo *_marshaller_CSIv2_InvalidMechanism;

void operator<<=( CORBA::Any &_a, const CSIv2::InvalidMechanism &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_InvalidMechanism, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIv2::InvalidMechanism *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIv2::InvalidMechanism &_e )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_InvalidMechanism, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIv2::InvalidMechanism *&_e )
{
  return _a.to_static_any (_marshaller_CSIv2_InvalidMechanism, (void *&)_e);
}

void CSIv2::InvalidMechanism::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidMechanism_var( (CSIv2::InvalidMechanism*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *CSIv2::InvalidMechanism::_repoid() const
{
  return "IDL:CSIv2/InvalidMechanism:1.0";
}

void CSIv2::InvalidMechanism::_encode( CORBA::DataEncoder & ) const
{
  assert(0);
}

void CSIv2::InvalidMechanism::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *CSIv2::InvalidMechanism::_clone() const
{
  return new InvalidMechanism( *this );
}

CSIv2::InvalidMechanism *CSIv2::InvalidMechanism::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/InvalidMechanism:1.0" ) )
    return (InvalidMechanism *) _ex;
  return NULL;
}

const CSIv2::InvalidMechanism *CSIv2::InvalidMechanism::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:CSIv2/InvalidMechanism:1.0" ) )
    return (InvalidMechanism *) _ex;
  return NULL;
}


/*
 * Base interface for class TSS
 */

CSIv2::TSS::~TSS()
{
}

void *
CSIv2::TSS::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/TSS:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CSIv2::CMSetup::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CSIv2::TSS_ptr
CSIv2::TSS::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/TSS:1.0" )))
      return _duplicate( (CSIv2::TSS_ptr) _p );
  }
  return _nil();
}

CSIv2::TSS_ptr
CSIv2::TSS::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_TSS;
}


/*
 * Base interface for class CSS
 */

CSIv2::CSS::~CSS()
{
}

void *
CSIv2::CSS::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/CSS:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CSIv2::CMSetup::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CSIv2::CSS_ptr
CSIv2::CSS::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/CSS:1.0" )))
      return _duplicate( (CSIv2::CSS_ptr) _p );
  }
  return _nil();
}

CSIv2::CSS_ptr
CSIv2::CSS::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_CSS;
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_UserIdentity;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIv2::UserIdentity::UserIdentity()
{
}

CSIv2::UserIdentity::UserIdentity( const UserIdentity& _s )
{
  user_name = ((UserIdentity&)_s).user_name;
  identity_name = ((UserIdentity&)_s).identity_name;
}

CSIv2::UserIdentity::~UserIdentity()
{
}

CSIv2::UserIdentity&
CSIv2::UserIdentity::operator=( const UserIdentity& _s )
{
  user_name = ((UserIdentity&)_s).user_name;
  identity_name = ((UserIdentity&)_s).identity_name;
  return *this;
}
#endif

class _Marshaller_CSIv2_UserIdentity : public ::CORBA::StaticTypeInfo {
    typedef CSIv2::UserIdentity _MICO_T;
  public:
    ~_Marshaller_CSIv2_UserIdentity();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIv2_UserIdentity::~_Marshaller_CSIv2_UserIdentity()
{
}

::CORBA::StaticValueType _Marshaller_CSIv2_UserIdentity::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIv2_UserIdentity::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIv2_UserIdentity::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIv2_UserIdentity::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->user_name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->identity_name._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CSIv2_UserIdentity::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->user_name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->identity_name.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIv2_UserIdentity::typecode()
{
  return CSIv2::_tc_UserIdentity;
}

::CORBA::StaticTypeInfo *_marshaller_CSIv2_UserIdentity;

void operator<<=( CORBA::Any &_a, const CSIv2::UserIdentity &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_UserIdentity, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIv2::UserIdentity *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIv2::UserIdentity &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIv2_UserIdentity, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIv2::UserIdentity *&_s )
{
  return _a.to_static_any (_marshaller_CSIv2_UserIdentity, (void *&)_s);
}


/*
 * Base interface for class ServerRequestInterceptor
 */

CSIv2::ServerRequestInterceptor::~ServerRequestInterceptor()
{
}

void *
CSIv2::ServerRequestInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/ServerRequestInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::ServerRequestInterceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CSIv2::ServerRequestInterceptor_ptr
CSIv2::ServerRequestInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/ServerRequestInterceptor:1.0" )))
      return _duplicate( (CSIv2::ServerRequestInterceptor_ptr) _p );
  }
  return _nil();
}

CSIv2::ServerRequestInterceptor_ptr
CSIv2::ServerRequestInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_ServerRequestInterceptor;
}


/*
 * Base interface for class ClientRequestInterceptor
 */

CSIv2::ClientRequestInterceptor::~ClientRequestInterceptor()
{
}

void *
CSIv2::ClientRequestInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/ClientRequestInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::ClientRequestInterceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CSIv2::ClientRequestInterceptor_ptr
CSIv2::ClientRequestInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/ClientRequestInterceptor:1.0" )))
      return _duplicate( (CSIv2::ClientRequestInterceptor_ptr) _p );
  }
  return _nil();
}

CSIv2::ClientRequestInterceptor_ptr
CSIv2::ClientRequestInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_ClientRequestInterceptor;
}


/*
 * Base interface for class IORInterceptor
 */

CSIv2::IORInterceptor::~IORInterceptor()
{
}

void *
CSIv2::IORInterceptor::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:CSIv2/IORInterceptor:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = PortableInterceptor::IORInterceptor::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CSIv2::IORInterceptor_ptr
CSIv2::IORInterceptor::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:CSIv2/IORInterceptor:1.0" )))
      return _duplicate( (CSIv2::IORInterceptor_ptr) _p );
  }
  return _nil();
}

CSIv2::IORInterceptor_ptr
CSIv2::IORInterceptor::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CSIv2
{
CORBA::TypeCodeConst _tc_IORInterceptor;
}

class _Marshaller__seq_CSIv2_UserIdentity : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CSIv2_UserIdentity();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CSIv2_UserIdentity::~_Marshaller__seq_CSIv2_UserIdentity()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CSIv2_UserIdentity::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CSIv2_UserIdentity::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CSIv2_UserIdentity::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CSIv2_UserIdentity::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CSIv2_UserIdentity->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CSIv2_UserIdentity::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CSIv2_UserIdentity->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIv2_UserIdentity::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000080000000010000000f00000070000000010000001b00"
    "000049444c3a43534976322f557365724964656e746974793a312e300000"
    "0d000000557365724964656e7469747900000000020000000a0000007573"
    "65725f6e616d6500000012000000000000000e0000006964656e74697479"
    "5f6e616d65000000120000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIv2_UserIdentity::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CSIv2_UserIdentity;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIv2_UserIdentity, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIv2_UserIdentity, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CSIv2_UserIdentity, (void *&)_s);
}


struct __tc_init_CSIV2 {
  __tc_init_CSIV2()
  {
    CSIv2::_tc_DistinguishedName = 
    "010000001500000048000000010000002000000049444c3a43534976322f"
    "44697374696e677569736865644e616d653a312e30001200000044697374"
    "696e677569736865644e616d650000001200000000000000";
    CSIv2::_tc_DistinguishedNameList = 
    "0100000015000000a8000000010000002400000049444c3a43534976322f"
    "44697374696e677569736865644e616d654c6973743a312e300016000000"
    "44697374696e677569736865644e616d654c697374000000130000005800"
    "0000010000001500000048000000010000002000000049444c3a43534976"
    "322f44697374696e677569736865644e616d653a312e3000120000004469"
    "7374696e677569736865644e616d65000000120000000000000000000000"
    ;
    CSIv2::_tc_UserIdentityList = 
    "0100000015000000c8000000010000001f00000049444c3a43534976322f"
    "557365724964656e746974794c6973743a312e3000001100000055736572"
    "4964656e746974794c697374000000001300000080000000010000000f00"
    "000070000000010000001b00000049444c3a43534976322f557365724964"
    "656e746974793a312e3000000d000000557365724964656e746974790000"
    "0000020000000a000000757365725f6e616d650000001200000000000000"
    "0e0000006964656e746974795f6e616d6500000012000000000000000000"
    "0000";
    CSIv2::_tc_GSSAuthError = 
    "01000000160000008c000000010000001b00000049444c3a43534976322f"
    "475353417574684572726f723a312e3000000d0000004753534175746845"
    "72726f72000000000100000007000000726561736f6e0000150000003c00"
    "0000010000002000000049444c3a6f6d672e6f72672f47535355502f4572"
    "726f72436f64653a312e30000a0000004572726f72436f64650000000500"
    "0000";
    _marshaller_CSIv2_GSSAuthError = new _Marshaller_CSIv2_GSSAuthError;
    CSIv2::_tc_TrustIdentityError = 
    "010000001600000048000000010000002100000049444c3a43534976322f"
    "54727573744964656e746974794572726f723a312e300000000013000000"
    "54727573744964656e746974794572726f72000000000000";
    _marshaller_CSIv2_TrustIdentityError = new _Marshaller_CSIv2_TrustIdentityError;
    CSIv2::_tc_SecurityManager = 
    "010000000e0000003c000000010000001e00000049444c3a43534976322f"
    "53656375726974794d616e616765723a312e300000001000000053656375"
    "726974794d616e6167657200";
    CSIv2::_tc_CMSetup = 
    "010000000e0000002c000000010000001600000049444c3a43534976322f"
    "434d53657475703a312e3000000008000000434d536574757000";
    CSIv2::_tc_InvalidMechanism = 
    "010000001600000044000000010000001f00000049444c3a43534976322f"
    "496e76616c69644d656368616e69736d3a312e30000011000000496e7661"
    "6c69644d656368616e69736d0000000000000000";
    _marshaller_CSIv2_InvalidMechanism = new _Marshaller_CSIv2_InvalidMechanism;
    CSIv2::_tc_TSS = 
    "010000000e00000024000000010000001200000049444c3a43534976322f"
    "5453533a312e300000000400000054535300";
    CSIv2::_tc_CSS = 
    "010000000e00000024000000010000001200000049444c3a43534976322f"
    "4353533a312e300000000400000043535300";
    CSIv2::_tc_UserIdentity = 
    "010000000f00000070000000010000001b00000049444c3a43534976322f"
    "557365724964656e746974793a312e3000000d000000557365724964656e"
    "7469747900000000020000000a000000757365725f6e616d650000001200"
    "0000000000000e0000006964656e746974795f6e616d6500000012000000"
    "00000000";
    _marshaller_CSIv2_UserIdentity = new _Marshaller_CSIv2_UserIdentity;
    CSIv2::_tc_ServerRequestInterceptor = 
    "010000000e0000004d000000010000002700000049444c3a43534976322f"
    "53657276657252657175657374496e746572636570746f723a312e300000"
    "1900000053657276657252657175657374496e746572636570746f7200";
    CSIv2::_tc_ClientRequestInterceptor = 
    "010000000e0000004d000000010000002700000049444c3a43534976322f"
    "436c69656e7452657175657374496e746572636570746f723a312e300000"
    "19000000436c69656e7452657175657374496e746572636570746f7200";
    CSIv2::_tc_IORInterceptor = 
    "010000000e0000003b000000010000001d00000049444c3a43534976322f"
    "494f52496e746572636570746f723a312e30000000000f000000494f5249"
    "6e746572636570746f7200";
    _marshaller__seq_CSIv2_UserIdentity = new _Marshaller__seq_CSIv2_UserIdentity;
  }

  ~__tc_init_CSIV2()
  {
    delete static_cast<_Marshaller_CSIv2_GSSAuthError*>(_marshaller_CSIv2_GSSAuthError);
    delete static_cast<_Marshaller_CSIv2_TrustIdentityError*>(_marshaller_CSIv2_TrustIdentityError);
    delete static_cast<_Marshaller_CSIv2_InvalidMechanism*>(_marshaller_CSIv2_InvalidMechanism);
    delete static_cast<_Marshaller_CSIv2_UserIdentity*>(_marshaller_CSIv2_UserIdentity);
    delete static_cast<_Marshaller__seq_CSIv2_UserIdentity*>(_marshaller__seq_CSIv2_UserIdentity);
  }
};

static __tc_init_CSIV2 __init_CSIV2;

