/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <mico/CCM.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------
namespace Components
{
CORBA::TypeCodeConst _tc_FeatureName;
}

namespace Components
{
CORBA::TypeCodeConst _tc_NameList;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidName;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidName::InvalidName()
{
}

Components::InvalidName::InvalidName( const InvalidName& _s )
{
}

Components::InvalidName::~InvalidName()
{
}

Components::InvalidName&
Components::InvalidName::operator=( const InvalidName& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidName : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidName _MICO_T;
  public:
    ~_Marshaller_Components_InvalidName();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidName::~_Marshaller_Components_InvalidName()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidName::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidName::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidName::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidName::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidName::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidName:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidName::typecode()
{
  return Components::_tc_InvalidName;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidName;

void operator<<=( CORBA::Any &_a, const Components::InvalidName &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidName, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidName *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidName &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidName, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidName *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidName, (void *&)_e);
}

void Components::InvalidName::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidName_var( (Components::InvalidName*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidName::_repoid() const
{
  return "IDL:omg.org/Components/InvalidName:1.0";
}

void Components::InvalidName::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidName->marshal( _en, (void*) this );
}

void Components::InvalidName::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidName::_clone() const
{
  return new InvalidName( *this );
}

Components::InvalidName *Components::InvalidName::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidName:1.0" ) )
    return (InvalidName *) _ex;
  return NULL;
}

const Components::InvalidName *Components::InvalidName::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidName:1.0" ) )
    return (InvalidName *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidConfiguration;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidConfiguration::InvalidConfiguration()
{
}

Components::InvalidConfiguration::InvalidConfiguration( const InvalidConfiguration& _s )
{
}

Components::InvalidConfiguration::~InvalidConfiguration()
{
}

Components::InvalidConfiguration&
Components::InvalidConfiguration::operator=( const InvalidConfiguration& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidConfiguration : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidConfiguration _MICO_T;
  public:
    ~_Marshaller_Components_InvalidConfiguration();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidConfiguration::~_Marshaller_Components_InvalidConfiguration()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidConfiguration::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidConfiguration::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidConfiguration::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidConfiguration::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidConfiguration::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidConfiguration:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidConfiguration::typecode()
{
  return Components::_tc_InvalidConfiguration;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidConfiguration;

void operator<<=( CORBA::Any &_a, const Components::InvalidConfiguration &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidConfiguration, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidConfiguration *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidConfiguration &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidConfiguration, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidConfiguration *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidConfiguration, (void *&)_e);
}

void Components::InvalidConfiguration::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidConfiguration_var( (Components::InvalidConfiguration*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidConfiguration::_repoid() const
{
  return "IDL:omg.org/Components/InvalidConfiguration:1.0";
}

void Components::InvalidConfiguration::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidConfiguration->marshal( _en, (void*) this );
}

void Components::InvalidConfiguration::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidConfiguration::_clone() const
{
  return new InvalidConfiguration( *this );
}

Components::InvalidConfiguration *Components::InvalidConfiguration::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidConfiguration:1.0" ) )
    return (InvalidConfiguration *) _ex;
  return NULL;
}

const Components::InvalidConfiguration *Components::InvalidConfiguration::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidConfiguration:1.0" ) )
    return (InvalidConfiguration *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_AlreadyConnected;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::AlreadyConnected::AlreadyConnected()
{
}

Components::AlreadyConnected::AlreadyConnected( const AlreadyConnected& _s )
{
}

Components::AlreadyConnected::~AlreadyConnected()
{
}

Components::AlreadyConnected&
Components::AlreadyConnected::operator=( const AlreadyConnected& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_AlreadyConnected : public ::CORBA::StaticTypeInfo {
    typedef ::Components::AlreadyConnected _MICO_T;
  public:
    ~_Marshaller_Components_AlreadyConnected();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_AlreadyConnected::~_Marshaller_Components_AlreadyConnected()
{
}

::CORBA::StaticValueType _Marshaller_Components_AlreadyConnected::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_AlreadyConnected::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_AlreadyConnected::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_AlreadyConnected::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_AlreadyConnected::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/AlreadyConnected:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_AlreadyConnected::typecode()
{
  return Components::_tc_AlreadyConnected;
}

::CORBA::StaticTypeInfo *_marshaller_Components_AlreadyConnected;

void operator<<=( CORBA::Any &_a, const Components::AlreadyConnected &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_AlreadyConnected, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::AlreadyConnected *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::AlreadyConnected &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_AlreadyConnected, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::AlreadyConnected *&_e )
{
  return _a.to_static_any (_marshaller_Components_AlreadyConnected, (void *&)_e);
}

void Components::AlreadyConnected::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw AlreadyConnected_var( (Components::AlreadyConnected*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::AlreadyConnected::_repoid() const
{
  return "IDL:omg.org/Components/AlreadyConnected:1.0";
}

void Components::AlreadyConnected::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_AlreadyConnected->marshal( _en, (void*) this );
}

void Components::AlreadyConnected::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::AlreadyConnected::_clone() const
{
  return new AlreadyConnected( *this );
}

Components::AlreadyConnected *Components::AlreadyConnected::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/AlreadyConnected:1.0" ) )
    return (AlreadyConnected *) _ex;
  return NULL;
}

const Components::AlreadyConnected *Components::AlreadyConnected::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/AlreadyConnected:1.0" ) )
    return (AlreadyConnected *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidConnection;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidConnection::InvalidConnection()
{
}

Components::InvalidConnection::InvalidConnection( const InvalidConnection& _s )
{
}

Components::InvalidConnection::~InvalidConnection()
{
}

Components::InvalidConnection&
Components::InvalidConnection::operator=( const InvalidConnection& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidConnection : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidConnection _MICO_T;
  public:
    ~_Marshaller_Components_InvalidConnection();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidConnection::~_Marshaller_Components_InvalidConnection()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidConnection::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidConnection::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidConnection::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidConnection::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidConnection::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidConnection:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidConnection::typecode()
{
  return Components::_tc_InvalidConnection;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidConnection;

void operator<<=( CORBA::Any &_a, const Components::InvalidConnection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidConnection, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidConnection *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidConnection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidConnection, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidConnection *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidConnection, (void *&)_e);
}

void Components::InvalidConnection::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidConnection_var( (Components::InvalidConnection*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidConnection::_repoid() const
{
  return "IDL:omg.org/Components/InvalidConnection:1.0";
}

void Components::InvalidConnection::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidConnection->marshal( _en, (void*) this );
}

void Components::InvalidConnection::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidConnection::_clone() const
{
  return new InvalidConnection( *this );
}

Components::InvalidConnection *Components::InvalidConnection::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidConnection:1.0" ) )
    return (InvalidConnection *) _ex;
  return NULL;
}

const Components::InvalidConnection *Components::InvalidConnection::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidConnection:1.0" ) )
    return (InvalidConnection *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_NoConnection;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::NoConnection::NoConnection()
{
}

Components::NoConnection::NoConnection( const NoConnection& _s )
{
}

Components::NoConnection::~NoConnection()
{
}

Components::NoConnection&
Components::NoConnection::operator=( const NoConnection& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_NoConnection : public ::CORBA::StaticTypeInfo {
    typedef ::Components::NoConnection _MICO_T;
  public:
    ~_Marshaller_Components_NoConnection();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_NoConnection::~_Marshaller_Components_NoConnection()
{
}

::CORBA::StaticValueType _Marshaller_Components_NoConnection::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_NoConnection::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_NoConnection::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_NoConnection::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_NoConnection::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/NoConnection:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_NoConnection::typecode()
{
  return Components::_tc_NoConnection;
}

::CORBA::StaticTypeInfo *_marshaller_Components_NoConnection;

void operator<<=( CORBA::Any &_a, const Components::NoConnection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_NoConnection, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::NoConnection *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::NoConnection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_NoConnection, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::NoConnection *&_e )
{
  return _a.to_static_any (_marshaller_Components_NoConnection, (void *&)_e);
}

void Components::NoConnection::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw NoConnection_var( (Components::NoConnection*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::NoConnection::_repoid() const
{
  return "IDL:omg.org/Components/NoConnection:1.0";
}

void Components::NoConnection::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_NoConnection->marshal( _en, (void*) this );
}

void Components::NoConnection::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::NoConnection::_clone() const
{
  return new NoConnection( *this );
}

Components::NoConnection *Components::NoConnection::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/NoConnection:1.0" ) )
    return (NoConnection *) _ex;
  return NULL;
}

const Components::NoConnection *Components::NoConnection::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/NoConnection:1.0" ) )
    return (NoConnection *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ExceededConnectionLimit;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::ExceededConnectionLimit::ExceededConnectionLimit()
{
}

Components::ExceededConnectionLimit::ExceededConnectionLimit( const ExceededConnectionLimit& _s )
{
}

Components::ExceededConnectionLimit::~ExceededConnectionLimit()
{
}

Components::ExceededConnectionLimit&
Components::ExceededConnectionLimit::operator=( const ExceededConnectionLimit& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_ExceededConnectionLimit : public ::CORBA::StaticTypeInfo {
    typedef ::Components::ExceededConnectionLimit _MICO_T;
  public:
    ~_Marshaller_Components_ExceededConnectionLimit();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ExceededConnectionLimit::~_Marshaller_Components_ExceededConnectionLimit()
{
}

::CORBA::StaticValueType _Marshaller_Components_ExceededConnectionLimit::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_ExceededConnectionLimit::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ExceededConnectionLimit::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ExceededConnectionLimit::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_ExceededConnectionLimit::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/ExceededConnectionLimit:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_ExceededConnectionLimit::typecode()
{
  return Components::_tc_ExceededConnectionLimit;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ExceededConnectionLimit;

void operator<<=( CORBA::Any &_a, const Components::ExceededConnectionLimit &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_ExceededConnectionLimit, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::ExceededConnectionLimit *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::ExceededConnectionLimit &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_ExceededConnectionLimit, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::ExceededConnectionLimit *&_e )
{
  return _a.to_static_any (_marshaller_Components_ExceededConnectionLimit, (void *&)_e);
}

void Components::ExceededConnectionLimit::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw ExceededConnectionLimit_var( (Components::ExceededConnectionLimit*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::ExceededConnectionLimit::_repoid() const
{
  return "IDL:omg.org/Components/ExceededConnectionLimit:1.0";
}

void Components::ExceededConnectionLimit::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_ExceededConnectionLimit->marshal( _en, (void*) this );
}

void Components::ExceededConnectionLimit::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::ExceededConnectionLimit::_clone() const
{
  return new ExceededConnectionLimit( *this );
}

Components::ExceededConnectionLimit *Components::ExceededConnectionLimit::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/ExceededConnectionLimit:1.0" ) )
    return (ExceededConnectionLimit *) _ex;
  return NULL;
}

const Components::ExceededConnectionLimit *Components::ExceededConnectionLimit::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/ExceededConnectionLimit:1.0" ) )
    return (ExceededConnectionLimit *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_CookieRequired;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::CookieRequired::CookieRequired()
{
}

Components::CookieRequired::CookieRequired( const CookieRequired& _s )
{
}

Components::CookieRequired::~CookieRequired()
{
}

Components::CookieRequired&
Components::CookieRequired::operator=( const CookieRequired& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_CookieRequired : public ::CORBA::StaticTypeInfo {
    typedef ::Components::CookieRequired _MICO_T;
  public:
    ~_Marshaller_Components_CookieRequired();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CookieRequired::~_Marshaller_Components_CookieRequired()
{
}

::CORBA::StaticValueType _Marshaller_Components_CookieRequired::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_CookieRequired::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_CookieRequired::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_CookieRequired::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_CookieRequired::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/CookieRequired:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_CookieRequired::typecode()
{
  return Components::_tc_CookieRequired;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CookieRequired;

void operator<<=( CORBA::Any &_a, const Components::CookieRequired &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CookieRequired, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::CookieRequired *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::CookieRequired &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CookieRequired, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::CookieRequired *&_e )
{
  return _a.to_static_any (_marshaller_Components_CookieRequired, (void *&)_e);
}

void Components::CookieRequired::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw CookieRequired_var( (Components::CookieRequired*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::CookieRequired::_repoid() const
{
  return "IDL:omg.org/Components/CookieRequired:1.0";
}

void Components::CookieRequired::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_CookieRequired->marshal( _en, (void*) this );
}

void Components::CookieRequired::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::CookieRequired::_clone() const
{
  return new CookieRequired( *this );
}

Components::CookieRequired *Components::CookieRequired::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CookieRequired:1.0" ) )
    return (CookieRequired *) _ex;
  return NULL;
}

const Components::CookieRequired *Components::CookieRequired::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CookieRequired:1.0" ) )
    return (CookieRequired *) _ex;
  return NULL;
}


// valuetype PortDescription
Components::PortDescription::PortDescription ()
{
}

Components::PortDescription::~PortDescription ()
{
}

void *
Components::PortDescription::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/PortDescription:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::PortDescription *
Components::PortDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/PortDescription:1.0")))) {
    return (Components::PortDescription *) p;
  }
  return 0;
}

Components::PortDescription *
Components::PortDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::PortDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/PortDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::PortDescription::_copy_members (const PortDescription& other)
{
  name (other.name());
  type_id (other.type_id());
}

CORBA::ValueBase *
Components::PortDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/PortDescription:1.0";
  PortDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::PortDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/PortDescription:1.0");
  chunked = FALSE;
}

void
Components::PortDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  const char * _name = name ();
  CORBA::_stc_string->marshal (ec, &_name);
  const char * _type_id = type_id ();
  CORBA::_stc_string->marshal (ec, &_type_id);
}

CORBA::Boolean
Components::PortDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  Components::FeatureName_var _name;
  if (!CORBA::_stc_string->demarshal (dc, &_name._for_demarshal())) {
      return FALSE;
  }
  name (_name);
  CORBA::String_var _type_id;
  if (!CORBA::_stc_string->demarshal (dc, &_type_id._for_demarshal())) {
      return FALSE;
  }
  type_id (_type_id);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_PortDescription;
}

class _Marshaller_Components_PortDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::PortDescription* _MICO_T;
  public:
    ~_Marshaller_Components_PortDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_PortDescription::~_Marshaller_Components_PortDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_PortDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_PortDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_PortDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_PortDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/PortDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::PortDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_PortDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_PortDescription::typecode()
{
  return Components::_tc_PortDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_PortDescription;

void
operator<<=( CORBA::Any &_a, const Components::PortDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_PortDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::PortDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_PortDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::PortDescription* &_val_ptr )
{
  Components::PortDescription* *p;
  if (_a.to_static_any (_marshaller_Components_PortDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype PortDescription
OBV_Components::PortDescription::PortDescription ()
{
}

OBV_Components::PortDescription::PortDescription (const char* _name, const char* _type_id)
{
  this->name(_name);
  this->type_id(_type_id);
}

OBV_Components::PortDescription::~PortDescription ()
{
}

void OBV_Components::PortDescription::name( char *_p )
{
  _m.name = _p;
}

void OBV_Components::PortDescription::name( const char *_p )
{
  _m.name = _p;
}

void OBV_Components::PortDescription::name( const CORBA::String_var &_p )
{
  _m.name = _p;
}

const char *OBV_Components::PortDescription::name() const
{
  return _m.name;
}

void OBV_Components::PortDescription::type_id( char *_p )
{
  _m.type_id = _p;
}

void OBV_Components::PortDescription::type_id( const char *_p )
{
  _m.type_id = _p;
}

void OBV_Components::PortDescription::type_id( const CORBA::String_var &_p )
{
  _m.type_id = _p;
}

const char *OBV_Components::PortDescription::type_id() const
{
  return _m.type_id;
}


// valuetype Cookie
Components::Cookie::Cookie ()
{
}

Components::Cookie::~Cookie ()
{
}

void *
Components::Cookie::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Cookie:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::Cookie *
Components::Cookie::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/Cookie:1.0")))) {
    return (Components::Cookie *) p;
  }
  return 0;
}

Components::Cookie *
Components::Cookie::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::Cookie::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/Cookie:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::Cookie::_copy_members (const Cookie& other)
{
  CookieValue (other.CookieValue());
}

CORBA::ValueBase *
Components::Cookie::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/Cookie:1.0";
  Cookie * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::Cookie::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/Cookie:1.0");
  chunked = FALSE;
}

void
Components::Cookie::_marshal_members (CORBA::DataEncoder &ec)
{
  _CookieValue_seq & _CookieValue = CookieValue ();
  CORBA::_stcseq_octet->marshal (ec, &_CookieValue);
}

CORBA::Boolean
Components::Cookie::_demarshal_members (CORBA::DataDecoder &dc)
{
  SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _CookieValue;
  if (!CORBA::_stcseq_octet->demarshal (dc, &_CookieValue)) {
      return FALSE;
  }
  CookieValue (_CookieValue);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_Cookie;
}

class _Marshaller_Components_Cookie : public ::CORBA::StaticTypeInfo {
    typedef Components::Cookie* _MICO_T;
  public:
    ~_Marshaller_Components_Cookie();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Cookie::~_Marshaller_Components_Cookie()
{
}

::CORBA::StaticValueType _Marshaller_Components_Cookie::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Cookie::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_Cookie::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_Cookie::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/Cookie:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::Cookie::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_Cookie::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_Cookie::typecode()
{
  return Components::_tc_Cookie;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Cookie;

void
operator<<=( CORBA::Any &_a, const Components::Cookie* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_Cookie, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Cookie** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_Cookie, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Cookie* &_val_ptr )
{
  Components::Cookie* *p;
  if (_a.to_static_any (_marshaller_Components_Cookie, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype Cookie
OBV_Components::Cookie::Cookie ()
{
}

OBV_Components::Cookie::Cookie (const _CookieValue_seq& _CookieValue)
{
  this->CookieValue(_CookieValue);
}

OBV_Components::Cookie::~Cookie ()
{
}

void OBV_Components::Cookie::CookieValue( const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET>& _p )
{
  _m.CookieValue = _p;
}

const OBV_Components::Cookie::_CookieValue_seq& OBV_Components::Cookie::CookieValue() const
{
  return _m.CookieValue;
}

OBV_Components::Cookie::_CookieValue_seq& OBV_Components::Cookie::CookieValue()
{
  return _m.CookieValue;
}


// valuetype EventBase
Components::EventBase::EventBase ()
{
}

Components::EventBase::~EventBase ()
{
}

void *
Components::EventBase::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/EventBase:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::EventBase *
Components::EventBase::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/EventBase:1.0")))) {
    return (Components::EventBase *) p;
  }
  return 0;
}

Components::EventBase *
Components::EventBase::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::EventBase::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/EventBase:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::EventBase::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/EventBase:1.0");
  chunked = FALSE;
}

void
Components::EventBase::_marshal_members (CORBA::DataEncoder &ec)
{
}

CORBA::Boolean
Components::EventBase::_demarshal_members (CORBA::DataDecoder &dc)
{
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_EventBase;
}

class _Marshaller_Components_EventBase : public ::CORBA::StaticTypeInfo {
    typedef Components::EventBase* _MICO_T;
  public:
    ~_Marshaller_Components_EventBase();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_EventBase::~_Marshaller_Components_EventBase()
{
}

::CORBA::StaticValueType _Marshaller_Components_EventBase::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_EventBase::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_EventBase::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_EventBase::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/EventBase:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::EventBase::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_EventBase::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_EventBase::typecode()
{
  return Components::_tc_EventBase;
}

::CORBA::StaticTypeInfo *_marshaller_Components_EventBase;

void
operator<<=( CORBA::Any &_a, const Components::EventBase* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_EventBase, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::EventBase** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_EventBase, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::EventBase* &_val_ptr )
{
  Components::EventBase* *p;
  if (_a.to_static_any (_marshaller_Components_EventBase, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// valuetype FacetDescription
Components::FacetDescription::FacetDescription ()
{
}

Components::FacetDescription::~FacetDescription ()
{
}

void *
Components::FacetDescription::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/FacetDescription:1.0") == 0) {
    return (void *) this;
  }
  if ((p = Components::PortDescription::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

Components::FacetDescription *
Components::FacetDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/FacetDescription:1.0")))) {
    return (Components::FacetDescription *) p;
  }
  return 0;
}

Components::FacetDescription *
Components::FacetDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::FacetDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/FacetDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::FacetDescription::_copy_members (const FacetDescription& other)
{
  facet_ref (other.facet_ref());
}

CORBA::ValueBase *
Components::FacetDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/FacetDescription:1.0";
  FacetDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->Components::PortDescription::_copy_members (*this);
  _res->_copy_members (*this);
  return _res;
}

void
Components::FacetDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/FacetDescription:1.0");
  chunked = FALSE;
}

void
Components::FacetDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::PortDescription::_marshal_members (ec);
  CORBA::Object_ptr _facet_ref = facet_ref ();
  CORBA::_stc_Object->marshal (ec, &_facet_ref);
}

CORBA::Boolean
Components::FacetDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  if (!Components::PortDescription::_demarshal_members (dc)) {
      return FALSE;
  }
  CORBA::Object_var _facet_ref;
  if (!CORBA::_stc_Object->demarshal (dc, &_facet_ref._for_demarshal())) {
      return FALSE;
  }
  facet_ref (_facet_ref);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_FacetDescription;
}

class _Marshaller_Components_FacetDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::FacetDescription* _MICO_T;
  public:
    ~_Marshaller_Components_FacetDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_FacetDescription::~_Marshaller_Components_FacetDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_FacetDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_FacetDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_FacetDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_FacetDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/FacetDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::FacetDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_FacetDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_FacetDescription::typecode()
{
  return Components::_tc_FacetDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_FacetDescription;

void
operator<<=( CORBA::Any &_a, const Components::FacetDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_FacetDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::FacetDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_FacetDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::FacetDescription* &_val_ptr )
{
  Components::FacetDescription* *p;
  if (_a.to_static_any (_marshaller_Components_FacetDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype FacetDescription
OBV_Components::FacetDescription::FacetDescription ()
{
}

OBV_Components::FacetDescription::FacetDescription (const char* _name, const char* _type_id, CORBA::Object_ptr _facet_ref)
{
  this->name(_name);
  this->type_id(_type_id);
  this->facet_ref(_facet_ref);
}

OBV_Components::FacetDescription::~FacetDescription ()
{
}

void OBV_Components::FacetDescription::facet_ref( CORBA::Object_ptr _p )
{
  _m.facet_ref = CORBA::Object::_duplicate( _p );
}

CORBA::Object_ptr OBV_Components::FacetDescription::facet_ref() const
{
  return _m.facet_ref.in();
}

namespace Components
{
CORBA::TypeCodeConst _tc_FacetDescriptions;
}


/*
 * Base interface for class Navigation
 */

Components::Navigation::~Navigation()
{
}

void *
Components::Navigation::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Navigation:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Navigation_ptr
Components::Navigation::_narrow( CORBA::Object_ptr _obj )
{
  Components::Navigation_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Navigation:1.0" )))
      return _duplicate( (Components::Navigation_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Navigation:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Navigation:1.0")) {
      _o = new Components::Navigation_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Navigation_ptr
Components::Navigation::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Navigation;
}
class _Marshaller_Components_Navigation : public ::CORBA::StaticTypeInfo {
    typedef Components::Navigation_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Navigation();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Navigation::~_Marshaller_Components_Navigation()
{
}

::CORBA::StaticValueType _Marshaller_Components_Navigation::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Navigation::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Navigation::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Navigation::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Navigation::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Navigation::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Navigation::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Navigation::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Navigation::typecode()
{
  return Components::_tc_Navigation;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Navigation;

void
operator<<=( CORBA::Any &_a, const Components::Navigation_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Navigation, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Navigation_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Navigation, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Navigation_ptr &_obj )
{
  Components::Navigation_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Navigation, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Navigation
 */

Components::Navigation_stub::~Navigation_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Navigation::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Navigation:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Navigation *
POA_Components::Navigation::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Navigation:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Navigation *) p;
  }
  return NULL;
}

Components::Navigation_stub_clp::Navigation_stub_clp ()
{
}

Components::Navigation_stub_clp::Navigation_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Navigation_stub_clp::~Navigation_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Object_ptr Components::Navigation_stub::provide_facet( const char* _par_name )
{
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::Object_ptr _res = CORBA::Object::_nil();
  CORBA::StaticAny __res( CORBA::_stc_Object, &_res );

  CORBA::StaticRequest __req( this, "provide_facet" );
  __req.add_in_arg( &_sa_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Object_ptr
Components::Navigation_stub_clp::provide_facet( const char* _par_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Navigation * _myserv = POA_Components::Navigation::_narrow (_serv);
    if (_myserv) {
      CORBA::Object_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->provide_facet(_par_name);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Navigation_stub::provide_facet(_par_name);
}

#endif // MICO_CONF_NO_POA

Components::FacetDescriptions* Components::Navigation_stub::get_all_facets()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_FacetDescription );

  CORBA::StaticRequest __req( this, "get_all_facets" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::FacetDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::FacetDescriptions*
Components::Navigation_stub_clp::get_all_facets()
{
  return Components::Navigation_stub::get_all_facets();
}

#endif // MICO_CONF_NO_POA

Components::FacetDescriptions* Components::Navigation_stub::get_named_facets( const Components::NameList& _par_names )
{
  CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );
  CORBA::StaticAny __res( _marshaller__seq_Components_FacetDescription );

  CORBA::StaticRequest __req( this, "get_named_facets" );
  __req.add_in_arg( &_sa_names );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return (Components::FacetDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::FacetDescriptions*
Components::Navigation_stub_clp::get_named_facets( const Components::NameList& _par_names )
{
  return Components::Navigation_stub::get_named_facets(_par_names);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean Components::Navigation_stub::same_component( CORBA::Object_ptr _par_ref )
{
  CORBA::StaticAny _sa_ref( CORBA::_stc_Object, &_par_ref );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "same_component" );
  __req.add_in_arg( &_sa_ref );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
Components::Navigation_stub_clp::same_component( CORBA::Object_ptr _par_ref )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Navigation * _myserv = POA_Components::Navigation::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->same_component(_par_ref);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Navigation_stub::same_component(_par_ref);
}

#endif // MICO_CONF_NO_POA


// valuetype ConnectionDescription
Components::ConnectionDescription::ConnectionDescription ()
{
}

Components::ConnectionDescription::~ConnectionDescription ()
{
}

void *
Components::ConnectionDescription::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ConnectionDescription:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::ConnectionDescription *
Components::ConnectionDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/ConnectionDescription:1.0")))) {
    return (Components::ConnectionDescription *) p;
  }
  return 0;
}

Components::ConnectionDescription *
Components::ConnectionDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::ConnectionDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/ConnectionDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::ConnectionDescription::_copy_members (const ConnectionDescription& other)
{
  Components::Cookie_var _ck = Components::Cookie::_downcast (other.ck()->_copy_value());
  ck (_ck);
  objref (other.objref());
}

CORBA::ValueBase *
Components::ConnectionDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/ConnectionDescription:1.0";
  ConnectionDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::ConnectionDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/ConnectionDescription:1.0");
  chunked = FALSE;
}

void
Components::ConnectionDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::Cookie* _ck = ck ();
  _marshaller_Components_Cookie->marshal (ec, &_ck);
  CORBA::Object_ptr _objref = objref ();
  CORBA::_stc_Object->marshal (ec, &_objref);
}

CORBA::Boolean
Components::ConnectionDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  Components::Cookie_var _ck;
  if (!_marshaller_Components_Cookie->demarshal (dc, &_ck._for_demarshal())) {
      return FALSE;
  }
  ck (_ck);
  CORBA::Object_var _objref;
  if (!CORBA::_stc_Object->demarshal (dc, &_objref._for_demarshal())) {
      return FALSE;
  }
  objref (_objref);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConnectionDescription;
}

class _Marshaller_Components_ConnectionDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::ConnectionDescription* _MICO_T;
  public:
    ~_Marshaller_Components_ConnectionDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ConnectionDescription::~_Marshaller_Components_ConnectionDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_ConnectionDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ConnectionDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ConnectionDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ConnectionDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/ConnectionDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::ConnectionDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_ConnectionDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_ConnectionDescription::typecode()
{
  return Components::_tc_ConnectionDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ConnectionDescription;

void
operator<<=( CORBA::Any &_a, const Components::ConnectionDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_ConnectionDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ConnectionDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_ConnectionDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ConnectionDescription* &_val_ptr )
{
  Components::ConnectionDescription* *p;
  if (_a.to_static_any (_marshaller_Components_ConnectionDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype ConnectionDescription
OBV_Components::ConnectionDescription::ConnectionDescription ()
{
}

OBV_Components::ConnectionDescription::ConnectionDescription (::Components::Cookie* _ck, CORBA::Object_ptr _objref)
{
  this->ck(_ck);
  this->objref(_objref);
}

OBV_Components::ConnectionDescription::~ConnectionDescription ()
{
}

void OBV_Components::ConnectionDescription::ck( ::Components::Cookie* _p )
{
  CORBA::add_ref ( _p );
  _m.ck = _p;
}

::Components::Cookie* OBV_Components::ConnectionDescription::ck() const
{
  return _m.ck.in();
}

void OBV_Components::ConnectionDescription::objref( CORBA::Object_ptr _p )
{
  _m.objref = CORBA::Object::_duplicate( _p );
}

CORBA::Object_ptr OBV_Components::ConnectionDescription::objref() const
{
  return _m.objref.in();
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConnectionDescriptions;
}


// valuetype ReceptacleDescription
Components::ReceptacleDescription::ReceptacleDescription ()
{
}

Components::ReceptacleDescription::~ReceptacleDescription ()
{
}

void *
Components::ReceptacleDescription::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/ReceptacleDescription:1.0") == 0) {
    return (void *) this;
  }
  if ((p = Components::PortDescription::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

Components::ReceptacleDescription *
Components::ReceptacleDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/ReceptacleDescription:1.0")))) {
    return (Components::ReceptacleDescription *) p;
  }
  return 0;
}

Components::ReceptacleDescription *
Components::ReceptacleDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::ReceptacleDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/ReceptacleDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::ReceptacleDescription::_copy_members (const ReceptacleDescription& other)
{
  is_multiplex (other.is_multiplex());
  connections (other.connections());
}

CORBA::ValueBase *
Components::ReceptacleDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/ReceptacleDescription:1.0";
  ReceptacleDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->Components::PortDescription::_copy_members (*this);
  _res->_copy_members (*this);
  return _res;
}

void
Components::ReceptacleDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/ReceptacleDescription:1.0");
  chunked = FALSE;
}

void
Components::ReceptacleDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::PortDescription::_marshal_members (ec);
  CORBA::Boolean _is_multiplex = is_multiplex ();
  CORBA::_stc_boolean->marshal (ec, &_is_multiplex);
  Components::ConnectionDescriptions& _connections = connections ();
  _marshaller__seq_Components_ConnectionDescription->marshal (ec, &_connections);
}

CORBA::Boolean
Components::ReceptacleDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  if (!Components::PortDescription::_demarshal_members (dc)) {
      return FALSE;
  }
  CORBA::Boolean _is_multiplex;
  if (!CORBA::_stc_boolean->demarshal (dc, &_is_multiplex)) {
      return FALSE;
  }
  is_multiplex (_is_multiplex);
  Components::ConnectionDescriptions _connections;
  if (!_marshaller__seq_Components_ConnectionDescription->demarshal (dc, &_connections)) {
      return FALSE;
  }
  connections (_connections);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ReceptacleDescription;
}

class _Marshaller_Components_ReceptacleDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::ReceptacleDescription* _MICO_T;
  public:
    ~_Marshaller_Components_ReceptacleDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ReceptacleDescription::~_Marshaller_Components_ReceptacleDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_ReceptacleDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ReceptacleDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ReceptacleDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ReceptacleDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/ReceptacleDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::ReceptacleDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_ReceptacleDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_ReceptacleDescription::typecode()
{
  return Components::_tc_ReceptacleDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ReceptacleDescription;

void
operator<<=( CORBA::Any &_a, const Components::ReceptacleDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_ReceptacleDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ReceptacleDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_ReceptacleDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ReceptacleDescription* &_val_ptr )
{
  Components::ReceptacleDescription* *p;
  if (_a.to_static_any (_marshaller_Components_ReceptacleDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype ReceptacleDescription
OBV_Components::ReceptacleDescription::ReceptacleDescription ()
{
}

OBV_Components::ReceptacleDescription::ReceptacleDescription (const char* _name, const char* _type_id, CORBA::Boolean _is_multiplex, const ::Components::ConnectionDescriptions& _connections)
{
  this->name(_name);
  this->type_id(_type_id);
  this->is_multiplex(_is_multiplex);
  this->connections(_connections);
}

OBV_Components::ReceptacleDescription::~ReceptacleDescription ()
{
}

void OBV_Components::ReceptacleDescription::is_multiplex( CORBA::Boolean _p )
{
  _m.is_multiplex = _p;
}

CORBA::Boolean OBV_Components::ReceptacleDescription::is_multiplex() const
{
  return (CORBA::Boolean)_m.is_multiplex;
}

void OBV_Components::ReceptacleDescription::connections( const ::Components::ConnectionDescriptions& _p )
{
  _m.connections = _p;
}

const ::Components::ConnectionDescriptions& OBV_Components::ReceptacleDescription::connections() const
{
  return (::Components::ConnectionDescriptions&) _m.connections;
}

::Components::ConnectionDescriptions& OBV_Components::ReceptacleDescription::connections()
{
  return _m.connections;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ReceptacleDescriptions;
}


/*
 * Base interface for class Receptacles
 */

Components::Receptacles::~Receptacles()
{
}

void *
Components::Receptacles::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Receptacles:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Receptacles_ptr
Components::Receptacles::_narrow( CORBA::Object_ptr _obj )
{
  Components::Receptacles_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Receptacles:1.0" )))
      return _duplicate( (Components::Receptacles_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Receptacles:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Receptacles:1.0")) {
      _o = new Components::Receptacles_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Receptacles_ptr
Components::Receptacles::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Receptacles;
}
class _Marshaller_Components_Receptacles : public ::CORBA::StaticTypeInfo {
    typedef Components::Receptacles_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Receptacles();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Receptacles::~_Marshaller_Components_Receptacles()
{
}

::CORBA::StaticValueType _Marshaller_Components_Receptacles::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Receptacles::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Receptacles::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Receptacles::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Receptacles::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Receptacles::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Receptacles::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Receptacles::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Receptacles::typecode()
{
  return Components::_tc_Receptacles;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Receptacles;

void
operator<<=( CORBA::Any &_a, const Components::Receptacles_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Receptacles, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Receptacles_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Receptacles, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Receptacles_ptr &_obj )
{
  Components::Receptacles_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Receptacles, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Receptacles
 */

Components::Receptacles_stub::~Receptacles_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Receptacles::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Receptacles:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Receptacles *
POA_Components::Receptacles::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Receptacles:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Receptacles *) p;
  }
  return NULL;
}

Components::Receptacles_stub_clp::Receptacles_stub_clp ()
{
}

Components::Receptacles_stub_clp::Receptacles_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Receptacles_stub_clp::~Receptacles_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::Cookie* Components::Receptacles_stub::connect( const char* _par_name, CORBA::Object_ptr _par_connection )
{
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_connection( CORBA::_stc_Object, &_par_connection );
  Components::Cookie* _res = NULL;
  CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );

  CORBA::StaticRequest __req( this, "connect" );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_connection );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    _marshaller_Components_InvalidConnection, "IDL:omg.org/Components/InvalidConnection:1.0",
    _marshaller_Components_AlreadyConnected, "IDL:omg.org/Components/AlreadyConnected:1.0",
    _marshaller_Components_ExceededConnectionLimit, "IDL:omg.org/Components/ExceededConnectionLimit:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::Cookie*
Components::Receptacles_stub_clp::connect( const char* _par_name, CORBA::Object_ptr _par_connection )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Receptacles * _myserv = POA_Components::Receptacles::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->connect(_par_name, _par_connection);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      Components::Cookie* __res2 = Components::Cookie::_downcast (__res->_copy_value ());
      CORBA::remove_ref (__res);
      return __res2;
    }
    _postinvoke ();
  }

  return Components::Receptacles_stub::connect(_par_name, _par_connection);
}

#endif // MICO_CONF_NO_POA

void Components::Receptacles_stub::disconnect( const char* _par_name, Components::Cookie* _par_ck )
{
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_ck( _marshaller_Components_Cookie, &_par_ck );
  CORBA::StaticRequest __req( this, "disconnect" );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_ck );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    _marshaller_Components_InvalidConnection, "IDL:omg.org/Components/InvalidConnection:1.0",
    _marshaller_Components_CookieRequired, "IDL:omg.org/Components/CookieRequired:1.0",
    _marshaller_Components_NoConnection, "IDL:omg.org/Components/NoConnection:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Receptacles_stub_clp::disconnect( const char* _par_name, Components::Cookie* _par_ck )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Receptacles * _myserv = POA_Components::Receptacles::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* _copy_of_par_ck;
      _copy_of_par_ck = Components::Cookie::_downcast (_par_ck->_copy_value());
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->disconnect(_par_name, _copy_of_par_ck);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      CORBA::remove_ref (_copy_of_par_ck);
      return;
    }
    _postinvoke ();
  }

  Components::Receptacles_stub::disconnect(_par_name, _par_ck);
}

#endif // MICO_CONF_NO_POA

Components::ConnectionDescriptions* Components::Receptacles_stub::get_connections( const char* _par_name )
{
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny __res( _marshaller__seq_Components_ConnectionDescription );

  CORBA::StaticRequest __req( this, "get_connections" );
  __req.add_in_arg( &_sa_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return (Components::ConnectionDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ConnectionDescriptions*
Components::Receptacles_stub_clp::get_connections( const char* _par_name )
{
  return Components::Receptacles_stub::get_connections(_par_name);
}

#endif // MICO_CONF_NO_POA

Components::ReceptacleDescriptions* Components::Receptacles_stub::get_all_receptacles()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_ReceptacleDescription );

  CORBA::StaticRequest __req( this, "get_all_receptacles" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ReceptacleDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ReceptacleDescriptions*
Components::Receptacles_stub_clp::get_all_receptacles()
{
  return Components::Receptacles_stub::get_all_receptacles();
}

#endif // MICO_CONF_NO_POA

Components::ReceptacleDescriptions* Components::Receptacles_stub::get_named_receptacles( const Components::NameList& _par_names )
{
  CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );
  CORBA::StaticAny __res( _marshaller__seq_Components_ReceptacleDescription );

  CORBA::StaticRequest __req( this, "get_named_receptacles" );
  __req.add_in_arg( &_sa_names );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return (Components::ReceptacleDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ReceptacleDescriptions*
Components::Receptacles_stub_clp::get_named_receptacles( const Components::NameList& _par_names )
{
  return Components::Receptacles_stub::get_named_receptacles(_par_names);
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_BadEventType;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::BadEventType::BadEventType()
{
}

Components::BadEventType::BadEventType( const BadEventType& _s )
{
  expected_event_type = ((BadEventType&)_s).expected_event_type;
}

Components::BadEventType::~BadEventType()
{
}

Components::BadEventType&
Components::BadEventType::operator=( const BadEventType& _s )
{
  expected_event_type = ((BadEventType&)_s).expected_event_type;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
Components::BadEventType::BadEventType()
{
}

#endif

Components::BadEventType::BadEventType( const char* _m0 )
{
  expected_event_type = _m0;
}

class _Marshaller_Components_BadEventType : public ::CORBA::StaticTypeInfo {
    typedef ::Components::BadEventType _MICO_T;
  public:
    ~_Marshaller_Components_BadEventType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_BadEventType::~_Marshaller_Components_BadEventType()
{
}

::CORBA::StaticValueType _Marshaller_Components_BadEventType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_BadEventType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_BadEventType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_BadEventType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->expected_event_type._for_demarshal() ) &&
    dc.except_end();
}

void _Marshaller_Components_BadEventType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/BadEventType:1.0" );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->expected_event_type.inout() );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_BadEventType::typecode()
{
  return Components::_tc_BadEventType;
}

::CORBA::StaticTypeInfo *_marshaller_Components_BadEventType;

void operator<<=( CORBA::Any &_a, const Components::BadEventType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_BadEventType, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::BadEventType *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::BadEventType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_BadEventType, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::BadEventType *&_e )
{
  return _a.to_static_any (_marshaller_Components_BadEventType, (void *&)_e);
}

void Components::BadEventType::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw BadEventType_var( (Components::BadEventType*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::BadEventType::_repoid() const
{
  return "IDL:omg.org/Components/BadEventType:1.0";
}

void Components::BadEventType::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_BadEventType->marshal( _en, (void*) this );
}

void Components::BadEventType::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::BadEventType::_clone() const
{
  return new BadEventType( *this );
}

Components::BadEventType *Components::BadEventType::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/BadEventType:1.0" ) )
    return (BadEventType *) _ex;
  return NULL;
}

const Components::BadEventType *Components::BadEventType::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/BadEventType:1.0" ) )
    return (BadEventType *) _ex;
  return NULL;
}


/*
 * Base interface for class EventConsumerBase
 */

Components::EventConsumerBase::~EventConsumerBase()
{
}

void *
Components::EventConsumerBase::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/EventConsumerBase:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::EventConsumerBase_ptr
Components::EventConsumerBase::_narrow( CORBA::Object_ptr _obj )
{
  Components::EventConsumerBase_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/EventConsumerBase:1.0" )))
      return _duplicate( (Components::EventConsumerBase_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/EventConsumerBase:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/EventConsumerBase:1.0")) {
      _o = new Components::EventConsumerBase_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::EventConsumerBase_ptr
Components::EventConsumerBase::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_EventConsumerBase;
}
class _Marshaller_Components_EventConsumerBase : public ::CORBA::StaticTypeInfo {
    typedef Components::EventConsumerBase_ptr _MICO_T;
  public:
    ~_Marshaller_Components_EventConsumerBase();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_EventConsumerBase::~_Marshaller_Components_EventConsumerBase()
{
}

::CORBA::StaticValueType _Marshaller_Components_EventConsumerBase::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_EventConsumerBase::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::EventConsumerBase::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_EventConsumerBase::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_EventConsumerBase::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_EventConsumerBase::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::EventConsumerBase::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_EventConsumerBase::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_EventConsumerBase::typecode()
{
  return Components::_tc_EventConsumerBase;
}

::CORBA::StaticTypeInfo *_marshaller_Components_EventConsumerBase;

void
operator<<=( CORBA::Any &_a, const Components::EventConsumerBase_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_EventConsumerBase, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::EventConsumerBase_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_EventConsumerBase, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::EventConsumerBase_ptr &_obj )
{
  Components::EventConsumerBase_ptr *p;
  if (_a.to_static_any (_marshaller_Components_EventConsumerBase, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class EventConsumerBase
 */

Components::EventConsumerBase_stub::~EventConsumerBase_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::EventConsumerBase::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/EventConsumerBase:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::EventConsumerBase *
POA_Components::EventConsumerBase::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/EventConsumerBase:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::EventConsumerBase *) p;
  }
  return NULL;
}

Components::EventConsumerBase_stub_clp::EventConsumerBase_stub_clp ()
{
}

Components::EventConsumerBase_stub_clp::EventConsumerBase_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::EventConsumerBase_stub_clp::~EventConsumerBase_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void Components::EventConsumerBase_stub::push_event( Components::EventBase* _par_evt )
{
  CORBA::StaticAny _sa_evt( _marshaller_Components_EventBase, &_par_evt );
  CORBA::StaticRequest __req( this, "push_event" );
  __req.add_in_arg( &_sa_evt );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_BadEventType, "IDL:omg.org/Components/BadEventType:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::EventConsumerBase_stub_clp::push_event( Components::EventBase* _par_evt )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::EventConsumerBase * _myserv = POA_Components::EventConsumerBase::_narrow (_serv);
    if (_myserv) {
      Components::EventBase* _copy_of_par_evt;
      _copy_of_par_evt = Components::EventBase::_downcast (_par_evt->_copy_value());
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->push_event(_copy_of_par_evt);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      CORBA::remove_ref (_copy_of_par_evt);
      return;
    }
    _postinvoke ();
  }

  Components::EventConsumerBase_stub::push_event(_par_evt);
}

#endif // MICO_CONF_NO_POA


// valuetype ConsumerDescription
Components::ConsumerDescription::ConsumerDescription ()
{
}

Components::ConsumerDescription::~ConsumerDescription ()
{
}

void *
Components::ConsumerDescription::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/ConsumerDescription:1.0") == 0) {
    return (void *) this;
  }
  if ((p = Components::PortDescription::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

Components::ConsumerDescription *
Components::ConsumerDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/ConsumerDescription:1.0")))) {
    return (Components::ConsumerDescription *) p;
  }
  return 0;
}

Components::ConsumerDescription *
Components::ConsumerDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::ConsumerDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/ConsumerDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::ConsumerDescription::_copy_members (const ConsumerDescription& other)
{
  consumer (other.consumer());
}

CORBA::ValueBase *
Components::ConsumerDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/ConsumerDescription:1.0";
  ConsumerDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->Components::PortDescription::_copy_members (*this);
  _res->_copy_members (*this);
  return _res;
}

void
Components::ConsumerDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/ConsumerDescription:1.0");
  chunked = FALSE;
}

void
Components::ConsumerDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::PortDescription::_marshal_members (ec);
  Components::EventConsumerBase_ptr _consumer = consumer ();
  _marshaller_Components_EventConsumerBase->marshal (ec, &_consumer);
}

CORBA::Boolean
Components::ConsumerDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  if (!Components::PortDescription::_demarshal_members (dc)) {
      return FALSE;
  }
  Components::EventConsumerBase_var _consumer;
  if (!_marshaller_Components_EventConsumerBase->demarshal (dc, &_consumer._for_demarshal())) {
      return FALSE;
  }
  consumer (_consumer);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConsumerDescription;
}

class _Marshaller_Components_ConsumerDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::ConsumerDescription* _MICO_T;
  public:
    ~_Marshaller_Components_ConsumerDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ConsumerDescription::~_Marshaller_Components_ConsumerDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_ConsumerDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ConsumerDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ConsumerDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ConsumerDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/ConsumerDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::ConsumerDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_ConsumerDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_ConsumerDescription::typecode()
{
  return Components::_tc_ConsumerDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ConsumerDescription;

void
operator<<=( CORBA::Any &_a, const Components::ConsumerDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_ConsumerDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ConsumerDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_ConsumerDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ConsumerDescription* &_val_ptr )
{
  Components::ConsumerDescription* *p;
  if (_a.to_static_any (_marshaller_Components_ConsumerDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype ConsumerDescription
OBV_Components::ConsumerDescription::ConsumerDescription ()
{
}

OBV_Components::ConsumerDescription::ConsumerDescription (const char* _name, const char* _type_id, ::Components::EventConsumerBase_ptr _consumer)
{
  this->name(_name);
  this->type_id(_type_id);
  this->consumer(_consumer);
}

OBV_Components::ConsumerDescription::~ConsumerDescription ()
{
}

void OBV_Components::ConsumerDescription::consumer( ::Components::EventConsumerBase_ptr _p )
{
  _m.consumer = ::Components::EventConsumerBase::_duplicate( _p );
}

::Components::EventConsumerBase_ptr OBV_Components::ConsumerDescription::consumer() const
{
  return _m.consumer.in();
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConsumerDescriptions;
}


// valuetype EmitterDescription
Components::EmitterDescription::EmitterDescription ()
{
}

Components::EmitterDescription::~EmitterDescription ()
{
}

void *
Components::EmitterDescription::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/EmitterDescription:1.0") == 0) {
    return (void *) this;
  }
  if ((p = Components::PortDescription::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

Components::EmitterDescription *
Components::EmitterDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/EmitterDescription:1.0")))) {
    return (Components::EmitterDescription *) p;
  }
  return 0;
}

Components::EmitterDescription *
Components::EmitterDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::EmitterDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/EmitterDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::EmitterDescription::_copy_members (const EmitterDescription& other)
{
  consumer (other.consumer());
}

CORBA::ValueBase *
Components::EmitterDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/EmitterDescription:1.0";
  EmitterDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->Components::PortDescription::_copy_members (*this);
  _res->_copy_members (*this);
  return _res;
}

void
Components::EmitterDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/EmitterDescription:1.0");
  chunked = FALSE;
}

void
Components::EmitterDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::PortDescription::_marshal_members (ec);
  Components::EventConsumerBase_ptr _consumer = consumer ();
  _marshaller_Components_EventConsumerBase->marshal (ec, &_consumer);
}

CORBA::Boolean
Components::EmitterDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  if (!Components::PortDescription::_demarshal_members (dc)) {
      return FALSE;
  }
  Components::EventConsumerBase_var _consumer;
  if (!_marshaller_Components_EventConsumerBase->demarshal (dc, &_consumer._for_demarshal())) {
      return FALSE;
  }
  consumer (_consumer);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_EmitterDescription;
}

class _Marshaller_Components_EmitterDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::EmitterDescription* _MICO_T;
  public:
    ~_Marshaller_Components_EmitterDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_EmitterDescription::~_Marshaller_Components_EmitterDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_EmitterDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_EmitterDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_EmitterDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_EmitterDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/EmitterDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::EmitterDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_EmitterDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_EmitterDescription::typecode()
{
  return Components::_tc_EmitterDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_EmitterDescription;

void
operator<<=( CORBA::Any &_a, const Components::EmitterDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_EmitterDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::EmitterDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_EmitterDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::EmitterDescription* &_val_ptr )
{
  Components::EmitterDescription* *p;
  if (_a.to_static_any (_marshaller_Components_EmitterDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype EmitterDescription
OBV_Components::EmitterDescription::EmitterDescription ()
{
}

OBV_Components::EmitterDescription::EmitterDescription (const char* _name, const char* _type_id, ::Components::EventConsumerBase_ptr _consumer)
{
  this->name(_name);
  this->type_id(_type_id);
  this->consumer(_consumer);
}

OBV_Components::EmitterDescription::~EmitterDescription ()
{
}

void OBV_Components::EmitterDescription::consumer( ::Components::EventConsumerBase_ptr _p )
{
  _m.consumer = ::Components::EventConsumerBase::_duplicate( _p );
}

::Components::EventConsumerBase_ptr OBV_Components::EmitterDescription::consumer() const
{
  return _m.consumer.in();
}

namespace Components
{
CORBA::TypeCodeConst _tc_EmitterDescriptions;
}


// valuetype SubscriberDescription
Components::SubscriberDescription::SubscriberDescription ()
{
}

Components::SubscriberDescription::~SubscriberDescription ()
{
}

void *
Components::SubscriberDescription::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/SubscriberDescription:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::SubscriberDescription *
Components::SubscriberDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/SubscriberDescription:1.0")))) {
    return (Components::SubscriberDescription *) p;
  }
  return 0;
}

Components::SubscriberDescription *
Components::SubscriberDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::SubscriberDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/SubscriberDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::SubscriberDescription::_copy_members (const SubscriberDescription& other)
{
  Components::Cookie_var _ck = Components::Cookie::_downcast (other.ck()->_copy_value());
  ck (_ck);
  consumer (other.consumer());
}

CORBA::ValueBase *
Components::SubscriberDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/SubscriberDescription:1.0";
  SubscriberDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::SubscriberDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/SubscriberDescription:1.0");
  chunked = FALSE;
}

void
Components::SubscriberDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::Cookie* _ck = ck ();
  _marshaller_Components_Cookie->marshal (ec, &_ck);
  Components::EventConsumerBase_ptr _consumer = consumer ();
  _marshaller_Components_EventConsumerBase->marshal (ec, &_consumer);
}

CORBA::Boolean
Components::SubscriberDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  Components::Cookie_var _ck;
  if (!_marshaller_Components_Cookie->demarshal (dc, &_ck._for_demarshal())) {
      return FALSE;
  }
  ck (_ck);
  Components::EventConsumerBase_var _consumer;
  if (!_marshaller_Components_EventConsumerBase->demarshal (dc, &_consumer._for_demarshal())) {
      return FALSE;
  }
  consumer (_consumer);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_SubscriberDescription;
}

class _Marshaller_Components_SubscriberDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::SubscriberDescription* _MICO_T;
  public:
    ~_Marshaller_Components_SubscriberDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_SubscriberDescription::~_Marshaller_Components_SubscriberDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_SubscriberDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_SubscriberDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_SubscriberDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_SubscriberDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/SubscriberDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::SubscriberDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_SubscriberDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_SubscriberDescription::typecode()
{
  return Components::_tc_SubscriberDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_SubscriberDescription;

void
operator<<=( CORBA::Any &_a, const Components::SubscriberDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_SubscriberDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::SubscriberDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_SubscriberDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::SubscriberDescription* &_val_ptr )
{
  Components::SubscriberDescription* *p;
  if (_a.to_static_any (_marshaller_Components_SubscriberDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype SubscriberDescription
OBV_Components::SubscriberDescription::SubscriberDescription ()
{
}

OBV_Components::SubscriberDescription::SubscriberDescription (::Components::Cookie* _ck, ::Components::EventConsumerBase_ptr _consumer)
{
  this->ck(_ck);
  this->consumer(_consumer);
}

OBV_Components::SubscriberDescription::~SubscriberDescription ()
{
}

void OBV_Components::SubscriberDescription::ck( ::Components::Cookie* _p )
{
  CORBA::add_ref ( _p );
  _m.ck = _p;
}

::Components::Cookie* OBV_Components::SubscriberDescription::ck() const
{
  return _m.ck.in();
}

void OBV_Components::SubscriberDescription::consumer( ::Components::EventConsumerBase_ptr _p )
{
  _m.consumer = ::Components::EventConsumerBase::_duplicate( _p );
}

::Components::EventConsumerBase_ptr OBV_Components::SubscriberDescription::consumer() const
{
  return _m.consumer.in();
}

namespace Components
{
CORBA::TypeCodeConst _tc_SubscriberDescriptions;
}


// valuetype PublisherDescription
Components::PublisherDescription::PublisherDescription ()
{
}

Components::PublisherDescription::~PublisherDescription ()
{
}

void *
Components::PublisherDescription::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/PublisherDescription:1.0") == 0) {
    return (void *) this;
  }
  if ((p = Components::PortDescription::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

Components::PublisherDescription *
Components::PublisherDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/PublisherDescription:1.0")))) {
    return (Components::PublisherDescription *) p;
  }
  return 0;
}

Components::PublisherDescription *
Components::PublisherDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::PublisherDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/PublisherDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::PublisherDescription::_copy_members (const PublisherDescription& other)
{
  consumers (other.consumers());
}

CORBA::ValueBase *
Components::PublisherDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/PublisherDescription:1.0";
  PublisherDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->Components::PortDescription::_copy_members (*this);
  _res->_copy_members (*this);
  return _res;
}

void
Components::PublisherDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/PublisherDescription:1.0");
  chunked = FALSE;
}

void
Components::PublisherDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::PortDescription::_marshal_members (ec);
  Components::SubscriberDescriptions& _consumers = consumers ();
  _marshaller__seq_Components_SubscriberDescription->marshal (ec, &_consumers);
}

CORBA::Boolean
Components::PublisherDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  if (!Components::PortDescription::_demarshal_members (dc)) {
      return FALSE;
  }
  Components::SubscriberDescriptions _consumers;
  if (!_marshaller__seq_Components_SubscriberDescription->demarshal (dc, &_consumers)) {
      return FALSE;
  }
  consumers (_consumers);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_PublisherDescription;
}

class _Marshaller_Components_PublisherDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::PublisherDescription* _MICO_T;
  public:
    ~_Marshaller_Components_PublisherDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_PublisherDescription::~_Marshaller_Components_PublisherDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_PublisherDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_PublisherDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_PublisherDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_PublisherDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/PublisherDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::PublisherDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_PublisherDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_PublisherDescription::typecode()
{
  return Components::_tc_PublisherDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_PublisherDescription;

void
operator<<=( CORBA::Any &_a, const Components::PublisherDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_PublisherDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::PublisherDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_PublisherDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::PublisherDescription* &_val_ptr )
{
  Components::PublisherDescription* *p;
  if (_a.to_static_any (_marshaller_Components_PublisherDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype PublisherDescription
OBV_Components::PublisherDescription::PublisherDescription ()
{
}

OBV_Components::PublisherDescription::PublisherDescription (const char* _name, const char* _type_id, const ::Components::SubscriberDescriptions& _consumers)
{
  this->name(_name);
  this->type_id(_type_id);
  this->consumers(_consumers);
}

OBV_Components::PublisherDescription::~PublisherDescription ()
{
}

void OBV_Components::PublisherDescription::consumers( const ::Components::SubscriberDescriptions& _p )
{
  _m.consumers = _p;
}

const ::Components::SubscriberDescriptions& OBV_Components::PublisherDescription::consumers() const
{
  return (::Components::SubscriberDescriptions&) _m.consumers;
}

::Components::SubscriberDescriptions& OBV_Components::PublisherDescription::consumers()
{
  return _m.consumers;
}

namespace Components
{
CORBA::TypeCodeConst _tc_PublisherDescriptions;
}


/*
 * Base interface for class Events
 */

Components::Events::~Events()
{
}

void *
Components::Events::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Events:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Events_ptr
Components::Events::_narrow( CORBA::Object_ptr _obj )
{
  Components::Events_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Events:1.0" )))
      return _duplicate( (Components::Events_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Events:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Events:1.0")) {
      _o = new Components::Events_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Events_ptr
Components::Events::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Events;
}
class _Marshaller_Components_Events : public ::CORBA::StaticTypeInfo {
    typedef Components::Events_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Events();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Events::~_Marshaller_Components_Events()
{
}

::CORBA::StaticValueType _Marshaller_Components_Events::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Events::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Events::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Events::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Events::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Events::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Events::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Events::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Events::typecode()
{
  return Components::_tc_Events;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Events;

void
operator<<=( CORBA::Any &_a, const Components::Events_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Events, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Events_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Events, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Events_ptr &_obj )
{
  Components::Events_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Events, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Events
 */

Components::Events_stub::~Events_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Events::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Events:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Events *
POA_Components::Events::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Events:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Events *) p;
  }
  return NULL;
}

Components::Events_stub_clp::Events_stub_clp ()
{
}

Components::Events_stub_clp::Events_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Events_stub_clp::~Events_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::EventConsumerBase_ptr Components::Events_stub::get_consumer( const char* _par_sink_name )
{
  CORBA::StaticAny _sa_sink_name( CORBA::_stc_string, &_par_sink_name );
  Components::EventConsumerBase_ptr _res = Components::EventConsumerBase::_nil();
  CORBA::StaticAny __res( _marshaller_Components_EventConsumerBase, &_res );

  CORBA::StaticRequest __req( this, "get_consumer" );
  __req.add_in_arg( &_sa_sink_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::EventConsumerBase_ptr
Components::Events_stub_clp::get_consumer( const char* _par_sink_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Events * _myserv = POA_Components::Events::_narrow (_serv);
    if (_myserv) {
      Components::EventConsumerBase_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_consumer(_par_sink_name);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Events_stub::get_consumer(_par_sink_name);
}

#endif // MICO_CONF_NO_POA

Components::ConsumerDescriptions* Components::Events_stub::get_all_consumers()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_ConsumerDescription );

  CORBA::StaticRequest __req( this, "get_all_consumers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ConsumerDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ConsumerDescriptions*
Components::Events_stub_clp::get_all_consumers()
{
  return Components::Events_stub::get_all_consumers();
}

#endif // MICO_CONF_NO_POA

Components::ConsumerDescriptions* Components::Events_stub::get_named_consumers( const Components::NameList& _par_names )
{
  CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );
  CORBA::StaticAny __res( _marshaller__seq_Components_ConsumerDescription );

  CORBA::StaticRequest __req( this, "get_named_consumers" );
  __req.add_in_arg( &_sa_names );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ConsumerDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ConsumerDescriptions*
Components::Events_stub_clp::get_named_consumers( const Components::NameList& _par_names )
{
  return Components::Events_stub::get_named_consumers(_par_names);
}

#endif // MICO_CONF_NO_POA

Components::Cookie* Components::Events_stub::subscribe( const char* _par_publisher_name, Components::EventConsumerBase_ptr _par_subscriber )
{
  CORBA::StaticAny _sa_publisher_name( CORBA::_stc_string, &_par_publisher_name );
  CORBA::StaticAny _sa_subscriber( _marshaller_Components_EventConsumerBase, &_par_subscriber );
  Components::Cookie* _res = NULL;
  CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );

  CORBA::StaticRequest __req( this, "subscribe" );
  __req.add_in_arg( &_sa_publisher_name );
  __req.add_in_arg( &_sa_subscriber );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::Cookie*
Components::Events_stub_clp::subscribe( const char* _par_publisher_name, Components::EventConsumerBase_ptr _par_subscriber )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Events * _myserv = POA_Components::Events::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->subscribe(_par_publisher_name, _par_subscriber);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      Components::Cookie* __res2 = Components::Cookie::_downcast (__res->_copy_value ());
      CORBA::remove_ref (__res);
      return __res2;
    }
    _postinvoke ();
  }

  return Components::Events_stub::subscribe(_par_publisher_name, _par_subscriber);
}

#endif // MICO_CONF_NO_POA

void Components::Events_stub::unsubscribe( const char* _par_publisher_name, Components::Cookie* _par_ck )
{
  CORBA::StaticAny _sa_publisher_name( CORBA::_stc_string, &_par_publisher_name );
  CORBA::StaticAny _sa_ck( _marshaller_Components_Cookie, &_par_ck );
  CORBA::StaticRequest __req( this, "unsubscribe" );
  __req.add_in_arg( &_sa_publisher_name );
  __req.add_in_arg( &_sa_ck );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    _marshaller_Components_InvalidConnection, "IDL:omg.org/Components/InvalidConnection:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Events_stub_clp::unsubscribe( const char* _par_publisher_name, Components::Cookie* _par_ck )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Events * _myserv = POA_Components::Events::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* _copy_of_par_ck;
      _copy_of_par_ck = Components::Cookie::_downcast (_par_ck->_copy_value());
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->unsubscribe(_par_publisher_name, _copy_of_par_ck);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      CORBA::remove_ref (_copy_of_par_ck);
      return;
    }
    _postinvoke ();
  }

  Components::Events_stub::unsubscribe(_par_publisher_name, _par_ck);
}

#endif // MICO_CONF_NO_POA

Components::PublisherDescriptions* Components::Events_stub::get_all_publishers()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_PublisherDescription );

  CORBA::StaticRequest __req( this, "get_all_publishers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::PublisherDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::PublisherDescriptions*
Components::Events_stub_clp::get_all_publishers()
{
  return Components::Events_stub::get_all_publishers();
}

#endif // MICO_CONF_NO_POA

Components::PublisherDescriptions* Components::Events_stub::get_named_publishers( const Components::NameList& _par_names )
{
  CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );
  CORBA::StaticAny __res( _marshaller__seq_Components_PublisherDescription );

  CORBA::StaticRequest __req( this, "get_named_publishers" );
  __req.add_in_arg( &_sa_names );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::PublisherDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::PublisherDescriptions*
Components::Events_stub_clp::get_named_publishers( const Components::NameList& _par_names )
{
  return Components::Events_stub::get_named_publishers(_par_names);
}

#endif // MICO_CONF_NO_POA

void Components::Events_stub::connect_consumer( const char* _par_emitter_name, Components::EventConsumerBase_ptr _par_consumer )
{
  CORBA::StaticAny _sa_emitter_name( CORBA::_stc_string, &_par_emitter_name );
  CORBA::StaticAny _sa_consumer( _marshaller_Components_EventConsumerBase, &_par_consumer );
  CORBA::StaticRequest __req( this, "connect_consumer" );
  __req.add_in_arg( &_sa_emitter_name );
  __req.add_in_arg( &_sa_consumer );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    _marshaller_Components_AlreadyConnected, "IDL:omg.org/Components/AlreadyConnected:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Events_stub_clp::connect_consumer( const char* _par_emitter_name, Components::EventConsumerBase_ptr _par_consumer )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Events * _myserv = POA_Components::Events::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->connect_consumer(_par_emitter_name, _par_consumer);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Events_stub::connect_consumer(_par_emitter_name, _par_consumer);
}

#endif // MICO_CONF_NO_POA

Components::EventConsumerBase_ptr Components::Events_stub::disconnect_consumer( const char* _par_source_name )
{
  CORBA::StaticAny _sa_source_name( CORBA::_stc_string, &_par_source_name );
  Components::EventConsumerBase_ptr _res = Components::EventConsumerBase::_nil();
  CORBA::StaticAny __res( _marshaller_Components_EventConsumerBase, &_res );

  CORBA::StaticRequest __req( this, "disconnect_consumer" );
  __req.add_in_arg( &_sa_source_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidName, "IDL:omg.org/Components/InvalidName:1.0",
    _marshaller_Components_NoConnection, "IDL:omg.org/Components/NoConnection:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::EventConsumerBase_ptr
Components::Events_stub_clp::disconnect_consumer( const char* _par_source_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Events * _myserv = POA_Components::Events::_narrow (_serv);
    if (_myserv) {
      Components::EventConsumerBase_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->disconnect_consumer(_par_source_name);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Events_stub::disconnect_consumer(_par_source_name);
}

#endif // MICO_CONF_NO_POA

Components::EmitterDescriptions* Components::Events_stub::get_all_emitters()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_EmitterDescription );

  CORBA::StaticRequest __req( this, "get_all_emitters" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::EmitterDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::EmitterDescriptions*
Components::Events_stub_clp::get_all_emitters()
{
  return Components::Events_stub::get_all_emitters();
}

#endif // MICO_CONF_NO_POA

Components::EmitterDescriptions* Components::Events_stub::get_named_emitters( const Components::NameList& _par_names )
{
  CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );
  CORBA::StaticAny __res( _marshaller__seq_Components_EmitterDescription );

  CORBA::StaticRequest __req( this, "get_named_emitters" );
  __req.add_in_arg( &_sa_names );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::EmitterDescriptions*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::EmitterDescriptions*
Components::Events_stub_clp::get_named_emitters( const Components::NameList& _par_names )
{
  return Components::Events_stub::get_named_emitters(_par_names);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class CCMHome
 */

Components::CCMHome::~CCMHome()
{
}

void *
Components::CCMHome::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/CCMHome:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::CCMHome_ptr
Components::CCMHome::_narrow( CORBA::Object_ptr _obj )
{
  Components::CCMHome_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/CCMHome:1.0" )))
      return _duplicate( (Components::CCMHome_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/CCMHome:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/CCMHome:1.0")) {
      _o = new Components::CCMHome_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::CCMHome_ptr
Components::CCMHome::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_CCMHome;
}
class _Marshaller_Components_CCMHome : public ::CORBA::StaticTypeInfo {
    typedef Components::CCMHome_ptr _MICO_T;
  public:
    ~_Marshaller_Components_CCMHome();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CCMHome::~_Marshaller_Components_CCMHome()
{
}

::CORBA::StaticValueType _Marshaller_Components_CCMHome::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_CCMHome::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::CCMHome::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_CCMHome::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_CCMHome::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_CCMHome::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::CCMHome::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_CCMHome::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_CCMHome::typecode()
{
  return Components::_tc_CCMHome;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CCMHome;

void
operator<<=( CORBA::Any &_a, const Components::CCMHome_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMHome, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::CCMHome_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMHome, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::CCMHome_ptr &_obj )
{
  Components::CCMHome_ptr *p;
  if (_a.to_static_any (_marshaller_Components_CCMHome, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class CCMHome
 */

Components::CCMHome_stub::~CCMHome_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::CCMHome::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/CCMHome:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::CCMHome *
POA_Components::CCMHome::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/CCMHome:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::CCMHome *) p;
  }
  return NULL;
}

Components::CCMHome_stub_clp::CCMHome_stub_clp ()
{
}

Components::CCMHome_stub_clp::CCMHome_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::CCMHome_stub_clp::~CCMHome_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Object_ptr Components::CCMHome_stub::get_component_def()
{
  CORBA::Object_ptr _res = CORBA::Object::_nil();
  CORBA::StaticAny __res( CORBA::_stc_Object, &_res );

  CORBA::StaticRequest __req( this, "get_component_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Object_ptr
Components::CCMHome_stub_clp::get_component_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMHome * _myserv = POA_Components::CCMHome::_narrow (_serv);
    if (_myserv) {
      CORBA::Object_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_component_def();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::CCMHome_stub::get_component_def();
}

#endif // MICO_CONF_NO_POA

CORBA::Object_ptr Components::CCMHome_stub::get_home_def()
{
  CORBA::Object_ptr _res = CORBA::Object::_nil();
  CORBA::StaticAny __res( CORBA::_stc_Object, &_res );

  CORBA::StaticRequest __req( this, "get_home_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Object_ptr
Components::CCMHome_stub_clp::get_home_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMHome * _myserv = POA_Components::CCMHome::_narrow (_serv);
    if (_myserv) {
      CORBA::Object_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_home_def();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::CCMHome_stub::get_home_def();
}

#endif // MICO_CONF_NO_POA

void Components::CCMHome_stub::remove_component( Components::CCMObject_ptr _par_comp )
{
  CORBA::StaticAny _sa_comp( _marshaller_Components_CCMObject, &_par_comp );
  CORBA::StaticRequest __req( this, "remove_component" );
  __req.add_in_arg( &_sa_comp );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::CCMHome_stub_clp::remove_component( Components::CCMObject_ptr _par_comp )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMHome * _myserv = POA_Components::CCMHome::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove_component(_par_comp);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::CCMHome_stub::remove_component(_par_comp);
}

#endif // MICO_CONF_NO_POA


// valuetype ComponentPortDescription
Components::ComponentPortDescription::ComponentPortDescription ()
{
}

Components::ComponentPortDescription::~ComponentPortDescription ()
{
}

void *
Components::ComponentPortDescription::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ComponentPortDescription:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::ComponentPortDescription *
Components::ComponentPortDescription::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/ComponentPortDescription:1.0")))) {
    return (Components::ComponentPortDescription *) p;
  }
  return 0;
}

Components::ComponentPortDescription *
Components::ComponentPortDescription::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::ComponentPortDescription::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/ComponentPortDescription:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::ComponentPortDescription::_copy_members (const ComponentPortDescription& other)
{
  facets (other.facets());
  receptacles (other.receptacles());
  consumers (other.consumers());
  emitters (other.emitters());
  publishers (other.publishers());
}

CORBA::ValueBase *
Components::ComponentPortDescription::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/ComponentPortDescription:1.0";
  ComponentPortDescription * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::ComponentPortDescription::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/ComponentPortDescription:1.0");
  chunked = FALSE;
}

void
Components::ComponentPortDescription::_marshal_members (CORBA::DataEncoder &ec)
{
  Components::FacetDescriptions& _facets = facets ();
  _marshaller__seq_Components_FacetDescription->marshal (ec, &_facets);
  Components::ReceptacleDescriptions& _receptacles = receptacles ();
  _marshaller__seq_Components_ReceptacleDescription->marshal (ec, &_receptacles);
  Components::ConsumerDescriptions& _consumers = consumers ();
  _marshaller__seq_Components_ConsumerDescription->marshal (ec, &_consumers);
  Components::EmitterDescriptions& _emitters = emitters ();
  _marshaller__seq_Components_EmitterDescription->marshal (ec, &_emitters);
  Components::PublisherDescriptions& _publishers = publishers ();
  _marshaller__seq_Components_PublisherDescription->marshal (ec, &_publishers);
}

CORBA::Boolean
Components::ComponentPortDescription::_demarshal_members (CORBA::DataDecoder &dc)
{
  Components::FacetDescriptions _facets;
  if (!_marshaller__seq_Components_FacetDescription->demarshal (dc, &_facets)) {
      return FALSE;
  }
  facets (_facets);
  Components::ReceptacleDescriptions _receptacles;
  if (!_marshaller__seq_Components_ReceptacleDescription->demarshal (dc, &_receptacles)) {
      return FALSE;
  }
  receptacles (_receptacles);
  Components::ConsumerDescriptions _consumers;
  if (!_marshaller__seq_Components_ConsumerDescription->demarshal (dc, &_consumers)) {
      return FALSE;
  }
  consumers (_consumers);
  Components::EmitterDescriptions _emitters;
  if (!_marshaller__seq_Components_EmitterDescription->demarshal (dc, &_emitters)) {
      return FALSE;
  }
  emitters (_emitters);
  Components::PublisherDescriptions _publishers;
  if (!_marshaller__seq_Components_PublisherDescription->demarshal (dc, &_publishers)) {
      return FALSE;
  }
  publishers (_publishers);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ComponentPortDescription;
}

class _Marshaller_Components_ComponentPortDescription : public ::CORBA::StaticTypeInfo {
    typedef Components::ComponentPortDescription* _MICO_T;
  public:
    ~_Marshaller_Components_ComponentPortDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ComponentPortDescription::~_Marshaller_Components_ComponentPortDescription()
{
}

::CORBA::StaticValueType _Marshaller_Components_ComponentPortDescription::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ComponentPortDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ComponentPortDescription::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ComponentPortDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/ComponentPortDescription:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::ComponentPortDescription::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_ComponentPortDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_ComponentPortDescription::typecode()
{
  return Components::_tc_ComponentPortDescription;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ComponentPortDescription;

void
operator<<=( CORBA::Any &_a, const Components::ComponentPortDescription* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_ComponentPortDescription, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ComponentPortDescription** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_ComponentPortDescription, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ComponentPortDescription* &_val_ptr )
{
  Components::ComponentPortDescription* *p;
  if (_a.to_static_any (_marshaller_Components_ComponentPortDescription, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype ComponentPortDescription
OBV_Components::ComponentPortDescription::ComponentPortDescription ()
{
}

OBV_Components::ComponentPortDescription::ComponentPortDescription (const ::Components::FacetDescriptions& _facets, const ::Components::ReceptacleDescriptions& _receptacles, const ::Components::ConsumerDescriptions& _consumers, const ::Components::EmitterDescriptions& _emitters, const ::Components::PublisherDescriptions& _publishers)
{
  this->facets(_facets);
  this->receptacles(_receptacles);
  this->consumers(_consumers);
  this->emitters(_emitters);
  this->publishers(_publishers);
}

OBV_Components::ComponentPortDescription::~ComponentPortDescription ()
{
}

void OBV_Components::ComponentPortDescription::facets( const ::Components::FacetDescriptions& _p )
{
  _m.facets = _p;
}

const ::Components::FacetDescriptions& OBV_Components::ComponentPortDescription::facets() const
{
  return (::Components::FacetDescriptions&) _m.facets;
}

::Components::FacetDescriptions& OBV_Components::ComponentPortDescription::facets()
{
  return _m.facets;
}

void OBV_Components::ComponentPortDescription::receptacles( const ::Components::ReceptacleDescriptions& _p )
{
  _m.receptacles = _p;
}

const ::Components::ReceptacleDescriptions& OBV_Components::ComponentPortDescription::receptacles() const
{
  return (::Components::ReceptacleDescriptions&) _m.receptacles;
}

::Components::ReceptacleDescriptions& OBV_Components::ComponentPortDescription::receptacles()
{
  return _m.receptacles;
}

void OBV_Components::ComponentPortDescription::consumers( const ::Components::ConsumerDescriptions& _p )
{
  _m.consumers = _p;
}

const ::Components::ConsumerDescriptions& OBV_Components::ComponentPortDescription::consumers() const
{
  return (::Components::ConsumerDescriptions&) _m.consumers;
}

::Components::ConsumerDescriptions& OBV_Components::ComponentPortDescription::consumers()
{
  return _m.consumers;
}

void OBV_Components::ComponentPortDescription::emitters( const ::Components::EmitterDescriptions& _p )
{
  _m.emitters = _p;
}

const ::Components::EmitterDescriptions& OBV_Components::ComponentPortDescription::emitters() const
{
  return (::Components::EmitterDescriptions&) _m.emitters;
}

::Components::EmitterDescriptions& OBV_Components::ComponentPortDescription::emitters()
{
  return _m.emitters;
}

void OBV_Components::ComponentPortDescription::publishers( const ::Components::PublisherDescriptions& _p )
{
  _m.publishers = _p;
}

const ::Components::PublisherDescriptions& OBV_Components::ComponentPortDescription::publishers() const
{
  return (::Components::PublisherDescriptions&) _m.publishers;
}

::Components::PublisherDescriptions& OBV_Components::ComponentPortDescription::publishers()
{
  return _m.publishers;
}


/*
 * Base interface for class CCMObject
 */

Components::CCMObject::~CCMObject()
{
}

void *
Components::CCMObject::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/CCMObject:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::Navigation::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = Components::Receptacles::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = Components::Events::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

Components::CCMObject_ptr
Components::CCMObject::_narrow( CORBA::Object_ptr _obj )
{
  Components::CCMObject_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/CCMObject:1.0" )))
      return _duplicate( (Components::CCMObject_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/CCMObject:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/CCMObject:1.0")) {
      _o = new Components::CCMObject_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::CCMObject_ptr
Components::CCMObject::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_CCMObject;
}
class _Marshaller_Components_CCMObject : public ::CORBA::StaticTypeInfo {
    typedef Components::CCMObject_ptr _MICO_T;
  public:
    ~_Marshaller_Components_CCMObject();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CCMObject::~_Marshaller_Components_CCMObject()
{
}

::CORBA::StaticValueType _Marshaller_Components_CCMObject::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_CCMObject::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::CCMObject::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_CCMObject::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_CCMObject::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_CCMObject::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::CCMObject::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_CCMObject::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_CCMObject::typecode()
{
  return Components::_tc_CCMObject;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CCMObject;

void
operator<<=( CORBA::Any &_a, const Components::CCMObject_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMObject, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::CCMObject_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMObject, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::CCMObject_ptr &_obj )
{
  Components::CCMObject_ptr *p;
  if (_a.to_static_any (_marshaller_Components_CCMObject, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class CCMObject
 */

Components::CCMObject_stub::~CCMObject_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::CCMObject::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/CCMObject:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_Components::Navigation::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_Components::Receptacles::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_Components::Events::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_Components::CCMObject *
POA_Components::CCMObject::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/CCMObject:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::CCMObject *) p;
  }
  return NULL;
}

Components::CCMObject_stub_clp::CCMObject_stub_clp ()
{
}

Components::CCMObject_stub_clp::CCMObject_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::CCMObject_stub_clp::~CCMObject_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Object_ptr Components::CCMObject_stub::get_component_def()
{
  CORBA::Object_ptr _res = CORBA::Object::_nil();
  CORBA::StaticAny __res( CORBA::_stc_Object, &_res );

  CORBA::StaticRequest __req( this, "get_component_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Object_ptr
Components::CCMObject_stub_clp::get_component_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMObject * _myserv = POA_Components::CCMObject::_narrow (_serv);
    if (_myserv) {
      CORBA::Object_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_component_def();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::CCMObject_stub::get_component_def();
}

#endif // MICO_CONF_NO_POA

Components::CCMHome_ptr Components::CCMObject_stub::get_ccm_home()
{
  Components::CCMHome_ptr _res = Components::CCMHome::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );

  CORBA::StaticRequest __req( this, "get_ccm_home" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMHome_ptr
Components::CCMObject_stub_clp::get_ccm_home()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMObject * _myserv = POA_Components::CCMObject::_narrow (_serv);
    if (_myserv) {
      Components::CCMHome_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_ccm_home();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::CCMObject_stub::get_ccm_home();
}

#endif // MICO_CONF_NO_POA

void Components::CCMObject_stub::configuration_complete()
{
  CORBA::StaticRequest __req( this, "configuration_complete" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::CCMObject_stub_clp::configuration_complete()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMObject * _myserv = POA_Components::CCMObject::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->configuration_complete();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::CCMObject_stub::configuration_complete();
}

#endif // MICO_CONF_NO_POA

void Components::CCMObject_stub::remove()
{
  CORBA::StaticRequest __req( this, "remove" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::CCMObject_stub_clp::remove()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMObject * _myserv = POA_Components::CCMObject::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::CCMObject_stub::remove();
}

#endif // MICO_CONF_NO_POA

Components::ComponentPortDescription* Components::CCMObject_stub::get_all_ports()
{
  Components::ComponentPortDescription* _res = NULL;
  CORBA::StaticAny __res( _marshaller_Components_ComponentPortDescription, &_res );

  CORBA::StaticRequest __req( this, "get_all_ports" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::ComponentPortDescription*
Components::CCMObject_stub_clp::get_all_ports()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::CCMObject * _myserv = POA_Components::CCMObject::_narrow (_serv);
    if (_myserv) {
      Components::ComponentPortDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_all_ports();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      Components::ComponentPortDescription* __res2 = Components::ComponentPortDescription::_downcast (__res->_copy_value ());
      CORBA::remove_ref (__res);
      return __res2;
    }
    _postinvoke ();
  }

  return Components::CCMObject_stub::get_all_ports();
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_DuplicateKeyValue;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::DuplicateKeyValue::DuplicateKeyValue()
{
}

Components::DuplicateKeyValue::DuplicateKeyValue( const DuplicateKeyValue& _s )
{
}

Components::DuplicateKeyValue::~DuplicateKeyValue()
{
}

Components::DuplicateKeyValue&
Components::DuplicateKeyValue::operator=( const DuplicateKeyValue& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_DuplicateKeyValue : public ::CORBA::StaticTypeInfo {
    typedef ::Components::DuplicateKeyValue _MICO_T;
  public:
    ~_Marshaller_Components_DuplicateKeyValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_DuplicateKeyValue::~_Marshaller_Components_DuplicateKeyValue()
{
}

::CORBA::StaticValueType _Marshaller_Components_DuplicateKeyValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_DuplicateKeyValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_DuplicateKeyValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_DuplicateKeyValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_DuplicateKeyValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/DuplicateKeyValue:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_DuplicateKeyValue::typecode()
{
  return Components::_tc_DuplicateKeyValue;
}

::CORBA::StaticTypeInfo *_marshaller_Components_DuplicateKeyValue;

void operator<<=( CORBA::Any &_a, const Components::DuplicateKeyValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_DuplicateKeyValue, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::DuplicateKeyValue *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::DuplicateKeyValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_DuplicateKeyValue, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::DuplicateKeyValue *&_e )
{
  return _a.to_static_any (_marshaller_Components_DuplicateKeyValue, (void *&)_e);
}

void Components::DuplicateKeyValue::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw DuplicateKeyValue_var( (Components::DuplicateKeyValue*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::DuplicateKeyValue::_repoid() const
{
  return "IDL:omg.org/Components/DuplicateKeyValue:1.0";
}

void Components::DuplicateKeyValue::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_DuplicateKeyValue->marshal( _en, (void*) this );
}

void Components::DuplicateKeyValue::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::DuplicateKeyValue::_clone() const
{
  return new DuplicateKeyValue( *this );
}

Components::DuplicateKeyValue *Components::DuplicateKeyValue::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/DuplicateKeyValue:1.0" ) )
    return (DuplicateKeyValue *) _ex;
  return NULL;
}

const Components::DuplicateKeyValue *Components::DuplicateKeyValue::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/DuplicateKeyValue:1.0" ) )
    return (DuplicateKeyValue *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_UnknownKeyValue;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::UnknownKeyValue::UnknownKeyValue()
{
}

Components::UnknownKeyValue::UnknownKeyValue( const UnknownKeyValue& _s )
{
}

Components::UnknownKeyValue::~UnknownKeyValue()
{
}

Components::UnknownKeyValue&
Components::UnknownKeyValue::operator=( const UnknownKeyValue& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_UnknownKeyValue : public ::CORBA::StaticTypeInfo {
    typedef ::Components::UnknownKeyValue _MICO_T;
  public:
    ~_Marshaller_Components_UnknownKeyValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_UnknownKeyValue::~_Marshaller_Components_UnknownKeyValue()
{
}

::CORBA::StaticValueType _Marshaller_Components_UnknownKeyValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_UnknownKeyValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_UnknownKeyValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_UnknownKeyValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_UnknownKeyValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/UnknownKeyValue:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_UnknownKeyValue::typecode()
{
  return Components::_tc_UnknownKeyValue;
}

::CORBA::StaticTypeInfo *_marshaller_Components_UnknownKeyValue;

void operator<<=( CORBA::Any &_a, const Components::UnknownKeyValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_UnknownKeyValue, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::UnknownKeyValue *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::UnknownKeyValue &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_UnknownKeyValue, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::UnknownKeyValue *&_e )
{
  return _a.to_static_any (_marshaller_Components_UnknownKeyValue, (void *&)_e);
}

void Components::UnknownKeyValue::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw UnknownKeyValue_var( (Components::UnknownKeyValue*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::UnknownKeyValue::_repoid() const
{
  return "IDL:omg.org/Components/UnknownKeyValue:1.0";
}

void Components::UnknownKeyValue::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_UnknownKeyValue->marshal( _en, (void*) this );
}

void Components::UnknownKeyValue::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::UnknownKeyValue::_clone() const
{
  return new UnknownKeyValue( *this );
}

Components::UnknownKeyValue *Components::UnknownKeyValue::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/UnknownKeyValue:1.0" ) )
    return (UnknownKeyValue *) _ex;
  return NULL;
}

const Components::UnknownKeyValue *Components::UnknownKeyValue::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/UnknownKeyValue:1.0" ) )
    return (UnknownKeyValue *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidKey;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidKey::InvalidKey()
{
}

Components::InvalidKey::InvalidKey( const InvalidKey& _s )
{
}

Components::InvalidKey::~InvalidKey()
{
}

Components::InvalidKey&
Components::InvalidKey::operator=( const InvalidKey& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidKey : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidKey _MICO_T;
  public:
    ~_Marshaller_Components_InvalidKey();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidKey::~_Marshaller_Components_InvalidKey()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidKey::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidKey::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidKey::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidKey::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidKey::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidKey:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidKey::typecode()
{
  return Components::_tc_InvalidKey;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidKey;

void operator<<=( CORBA::Any &_a, const Components::InvalidKey &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidKey, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidKey *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidKey &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidKey, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidKey *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidKey, (void *&)_e);
}

void Components::InvalidKey::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidKey_var( (Components::InvalidKey*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidKey::_repoid() const
{
  return "IDL:omg.org/Components/InvalidKey:1.0";
}

void Components::InvalidKey::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidKey->marshal( _en, (void*) this );
}

void Components::InvalidKey::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidKey::_clone() const
{
  return new InvalidKey( *this );
}

Components::InvalidKey *Components::InvalidKey::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidKey:1.0" ) )
    return (InvalidKey *) _ex;
  return NULL;
}

const Components::InvalidKey *Components::InvalidKey::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidKey:1.0" ) )
    return (InvalidKey *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_CCMHomes;
}


/*
 * Base interface for class KeylessCCMHome
 */

Components::KeylessCCMHome::~KeylessCCMHome()
{
}

void *
Components::KeylessCCMHome::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/KeylessCCMHome:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::KeylessCCMHome_ptr
Components::KeylessCCMHome::_narrow( CORBA::Object_ptr _obj )
{
  Components::KeylessCCMHome_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/KeylessCCMHome:1.0" )))
      return _duplicate( (Components::KeylessCCMHome_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/KeylessCCMHome:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/KeylessCCMHome:1.0")) {
      _o = new Components::KeylessCCMHome_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::KeylessCCMHome_ptr
Components::KeylessCCMHome::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_KeylessCCMHome;
}
class _Marshaller_Components_KeylessCCMHome : public ::CORBA::StaticTypeInfo {
    typedef Components::KeylessCCMHome_ptr _MICO_T;
  public:
    ~_Marshaller_Components_KeylessCCMHome();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_KeylessCCMHome::~_Marshaller_Components_KeylessCCMHome()
{
}

::CORBA::StaticValueType _Marshaller_Components_KeylessCCMHome::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_KeylessCCMHome::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::KeylessCCMHome::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_KeylessCCMHome::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_KeylessCCMHome::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_KeylessCCMHome::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::KeylessCCMHome::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_KeylessCCMHome::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_KeylessCCMHome::typecode()
{
  return Components::_tc_KeylessCCMHome;
}

::CORBA::StaticTypeInfo *_marshaller_Components_KeylessCCMHome;

void
operator<<=( CORBA::Any &_a, const Components::KeylessCCMHome_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_KeylessCCMHome, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::KeylessCCMHome_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_KeylessCCMHome, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::KeylessCCMHome_ptr &_obj )
{
  Components::KeylessCCMHome_ptr *p;
  if (_a.to_static_any (_marshaller_Components_KeylessCCMHome, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class KeylessCCMHome
 */

Components::KeylessCCMHome_stub::~KeylessCCMHome_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::KeylessCCMHome::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/KeylessCCMHome:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::KeylessCCMHome *
POA_Components::KeylessCCMHome::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/KeylessCCMHome:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::KeylessCCMHome *) p;
  }
  return NULL;
}

Components::KeylessCCMHome_stub_clp::KeylessCCMHome_stub_clp ()
{
}

Components::KeylessCCMHome_stub_clp::KeylessCCMHome_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::KeylessCCMHome_stub_clp::~KeylessCCMHome_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::CCMObject_ptr Components::KeylessCCMHome_stub::create_component()
{
  Components::CCMObject_ptr _res = Components::CCMObject::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMObject, &_res );

  CORBA::StaticRequest __req( this, "create_component" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMObject_ptr
Components::KeylessCCMHome_stub_clp::create_component()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::KeylessCCMHome * _myserv = POA_Components::KeylessCCMHome::_narrow (_serv);
    if (_myserv) {
      Components::CCMObject_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_component();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::KeylessCCMHome_stub::create_component();
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_HomeNotFound;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::HomeNotFound::HomeNotFound()
{
}

Components::HomeNotFound::HomeNotFound( const HomeNotFound& _s )
{
}

Components::HomeNotFound::~HomeNotFound()
{
}

Components::HomeNotFound&
Components::HomeNotFound::operator=( const HomeNotFound& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_HomeNotFound : public ::CORBA::StaticTypeInfo {
    typedef ::Components::HomeNotFound _MICO_T;
  public:
    ~_Marshaller_Components_HomeNotFound();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_HomeNotFound::~_Marshaller_Components_HomeNotFound()
{
}

::CORBA::StaticValueType _Marshaller_Components_HomeNotFound::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_HomeNotFound::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_HomeNotFound::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_HomeNotFound::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_HomeNotFound::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/HomeNotFound:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_HomeNotFound::typecode()
{
  return Components::_tc_HomeNotFound;
}

::CORBA::StaticTypeInfo *_marshaller_Components_HomeNotFound;

void operator<<=( CORBA::Any &_a, const Components::HomeNotFound &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_HomeNotFound, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::HomeNotFound *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::HomeNotFound &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_HomeNotFound, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::HomeNotFound *&_e )
{
  return _a.to_static_any (_marshaller_Components_HomeNotFound, (void *&)_e);
}

void Components::HomeNotFound::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw HomeNotFound_var( (Components::HomeNotFound*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::HomeNotFound::_repoid() const
{
  return "IDL:omg.org/Components/HomeNotFound:1.0";
}

void Components::HomeNotFound::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_HomeNotFound->marshal( _en, (void*) this );
}

void Components::HomeNotFound::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::HomeNotFound::_clone() const
{
  return new HomeNotFound( *this );
}

Components::HomeNotFound *Components::HomeNotFound::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/HomeNotFound:1.0" ) )
    return (HomeNotFound *) _ex;
  return NULL;
}

const Components::HomeNotFound *Components::HomeNotFound::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/HomeNotFound:1.0" ) )
    return (HomeNotFound *) _ex;
  return NULL;
}


/*
 * Base interface for class HomeFinder
 */

Components::HomeFinder::~HomeFinder()
{
}

void *
Components::HomeFinder::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/HomeFinder:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::HomeFinder_ptr
Components::HomeFinder::_narrow( CORBA::Object_ptr _obj )
{
  Components::HomeFinder_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/HomeFinder:1.0" )))
      return _duplicate( (Components::HomeFinder_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/HomeFinder:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/HomeFinder:1.0")) {
      _o = new Components::HomeFinder_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::HomeFinder_ptr
Components::HomeFinder::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_HomeFinder;
}
class _Marshaller_Components_HomeFinder : public ::CORBA::StaticTypeInfo {
    typedef Components::HomeFinder_ptr _MICO_T;
  public:
    ~_Marshaller_Components_HomeFinder();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_HomeFinder::~_Marshaller_Components_HomeFinder()
{
}

::CORBA::StaticValueType _Marshaller_Components_HomeFinder::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_HomeFinder::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::HomeFinder::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_HomeFinder::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_HomeFinder::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_HomeFinder::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::HomeFinder::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_HomeFinder::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_HomeFinder::typecode()
{
  return Components::_tc_HomeFinder;
}

::CORBA::StaticTypeInfo *_marshaller_Components_HomeFinder;

void
operator<<=( CORBA::Any &_a, const Components::HomeFinder_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_HomeFinder, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::HomeFinder_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_HomeFinder, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::HomeFinder_ptr &_obj )
{
  Components::HomeFinder_ptr *p;
  if (_a.to_static_any (_marshaller_Components_HomeFinder, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class HomeFinder
 */

Components::HomeFinder_stub::~HomeFinder_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::HomeFinder::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/HomeFinder:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::HomeFinder *
POA_Components::HomeFinder::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/HomeFinder:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::HomeFinder *) p;
  }
  return NULL;
}

Components::HomeFinder_stub_clp::HomeFinder_stub_clp ()
{
}

Components::HomeFinder_stub_clp::HomeFinder_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::HomeFinder_stub_clp::~HomeFinder_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::CCMHome_ptr Components::HomeFinder_stub::find_home_by_component_type( const char* _par_comp_repid )
{
  CORBA::StaticAny _sa_comp_repid( CORBA::_stc_string, &_par_comp_repid );
  Components::CCMHome_ptr _res = Components::CCMHome::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );

  CORBA::StaticRequest __req( this, "find_home_by_component_type" );
  __req.add_in_arg( &_sa_comp_repid );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_HomeNotFound, "IDL:omg.org/Components/HomeNotFound:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMHome_ptr
Components::HomeFinder_stub_clp::find_home_by_component_type( const char* _par_comp_repid )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::HomeFinder * _myserv = POA_Components::HomeFinder::_narrow (_serv);
    if (_myserv) {
      Components::CCMHome_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_home_by_component_type(_par_comp_repid);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::HomeFinder_stub::find_home_by_component_type(_par_comp_repid);
}

#endif // MICO_CONF_NO_POA

Components::CCMHome_ptr Components::HomeFinder_stub::find_home_by_home_type( const char* _par_home_repid )
{
  CORBA::StaticAny _sa_home_repid( CORBA::_stc_string, &_par_home_repid );
  Components::CCMHome_ptr _res = Components::CCMHome::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );

  CORBA::StaticRequest __req( this, "find_home_by_home_type" );
  __req.add_in_arg( &_sa_home_repid );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_HomeNotFound, "IDL:omg.org/Components/HomeNotFound:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMHome_ptr
Components::HomeFinder_stub_clp::find_home_by_home_type( const char* _par_home_repid )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::HomeFinder * _myserv = POA_Components::HomeFinder::_narrow (_serv);
    if (_myserv) {
      Components::CCMHome_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_home_by_home_type(_par_home_repid);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::HomeFinder_stub::find_home_by_home_type(_par_home_repid);
}

#endif // MICO_CONF_NO_POA

Components::CCMHome_ptr Components::HomeFinder_stub::find_home_by_name( const char* _par_home_name )
{
  CORBA::StaticAny _sa_home_name( CORBA::_stc_string, &_par_home_name );
  Components::CCMHome_ptr _res = Components::CCMHome::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );

  CORBA::StaticRequest __req( this, "find_home_by_name" );
  __req.add_in_arg( &_sa_home_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMHome_ptr
Components::HomeFinder_stub_clp::find_home_by_name( const char* _par_home_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::HomeFinder * _myserv = POA_Components::HomeFinder::_narrow (_serv);
    if (_myserv) {
      Components::CCMHome_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_home_by_name(_par_home_name);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::HomeFinder_stub::find_home_by_name(_par_home_name);
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_WrongComponentType;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::WrongComponentType::WrongComponentType()
{
}

Components::WrongComponentType::WrongComponentType( const WrongComponentType& _s )
{
}

Components::WrongComponentType::~WrongComponentType()
{
}

Components::WrongComponentType&
Components::WrongComponentType::operator=( const WrongComponentType& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_WrongComponentType : public ::CORBA::StaticTypeInfo {
    typedef ::Components::WrongComponentType _MICO_T;
  public:
    ~_Marshaller_Components_WrongComponentType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_WrongComponentType::~_Marshaller_Components_WrongComponentType()
{
}

::CORBA::StaticValueType _Marshaller_Components_WrongComponentType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_WrongComponentType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_WrongComponentType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_WrongComponentType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_WrongComponentType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/WrongComponentType:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_WrongComponentType::typecode()
{
  return Components::_tc_WrongComponentType;
}

::CORBA::StaticTypeInfo *_marshaller_Components_WrongComponentType;

void operator<<=( CORBA::Any &_a, const Components::WrongComponentType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_WrongComponentType, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::WrongComponentType *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::WrongComponentType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_WrongComponentType, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::WrongComponentType *&_e )
{
  return _a.to_static_any (_marshaller_Components_WrongComponentType, (void *&)_e);
}

void Components::WrongComponentType::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw WrongComponentType_var( (Components::WrongComponentType*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::WrongComponentType::_repoid() const
{
  return "IDL:omg.org/Components/WrongComponentType:1.0";
}

void Components::WrongComponentType::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_WrongComponentType->marshal( _en, (void*) this );
}

void Components::WrongComponentType::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::WrongComponentType::_clone() const
{
  return new WrongComponentType( *this );
}

Components::WrongComponentType *Components::WrongComponentType::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/WrongComponentType:1.0" ) )
    return (WrongComponentType *) _ex;
  return NULL;
}

const Components::WrongComponentType *Components::WrongComponentType::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/WrongComponentType:1.0" ) )
    return (WrongComponentType *) _ex;
  return NULL;
}


/*
 * Base interface for class Configurator
 */

Components::Configurator::~Configurator()
{
}

void *
Components::Configurator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Configurator:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Configurator_ptr
Components::Configurator::_narrow( CORBA::Object_ptr _obj )
{
  Components::Configurator_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Configurator:1.0" )))
      return _duplicate( (Components::Configurator_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Configurator:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Configurator:1.0")) {
      _o = new Components::Configurator_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Configurator_ptr
Components::Configurator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Configurator;
}
class _Marshaller_Components_Configurator : public ::CORBA::StaticTypeInfo {
    typedef Components::Configurator_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Configurator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Configurator::~_Marshaller_Components_Configurator()
{
}

::CORBA::StaticValueType _Marshaller_Components_Configurator::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Configurator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Configurator::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Configurator::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Configurator::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Configurator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Configurator::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Configurator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Configurator::typecode()
{
  return Components::_tc_Configurator;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Configurator;

void
operator<<=( CORBA::Any &_a, const Components::Configurator_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Configurator, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Configurator_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Configurator, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Configurator_ptr &_obj )
{
  Components::Configurator_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Configurator, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Configurator
 */

Components::Configurator_stub::~Configurator_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Configurator::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Configurator:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Configurator *
POA_Components::Configurator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Configurator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Configurator *) p;
  }
  return NULL;
}

Components::Configurator_stub_clp::Configurator_stub_clp ()
{
}

Components::Configurator_stub_clp::Configurator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Configurator_stub_clp::~Configurator_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void Components::Configurator_stub::configure( Components::CCMObject_ptr _par_comp )
{
  CORBA::StaticAny _sa_comp( _marshaller_Components_CCMObject, &_par_comp );
  CORBA::StaticRequest __req( this, "configure" );
  __req.add_in_arg( &_sa_comp );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_WrongComponentType, "IDL:omg.org/Components/WrongComponentType:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Configurator_stub_clp::configure( Components::CCMObject_ptr _par_comp )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Configurator * _myserv = POA_Components::Configurator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->configure(_par_comp);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Configurator_stub::configure(_par_comp);
}

#endif // MICO_CONF_NO_POA


// valuetype ConfigValue
Components::ConfigValue::ConfigValue ()
{
}

Components::ConfigValue::~ConfigValue ()
{
}

void *
Components::ConfigValue::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ConfigValue:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

Components::ConfigValue *
Components::ConfigValue::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/Components/ConfigValue:1.0")))) {
    return (Components::ConfigValue *) p;
  }
  return 0;
}

Components::ConfigValue *
Components::ConfigValue::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
Components::ConfigValue::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/Components/ConfigValue:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
Components::ConfigValue::_copy_members (const ConfigValue& other)
{
  name (other.name());
  value (other.value());
}

CORBA::ValueBase *
Components::ConfigValue::_copy_value ()
{
  vector<string> _dummy;
  string _repo_id = "IDL:omg.org/Components/ConfigValue:1.0";
  ConfigValue * _res = _downcast (_create (_dummy, _repo_id));
  assert (_res != 0);
  _res->_copy_members (*this);
  return _res;
}

void
Components::ConfigValue::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/Components/ConfigValue:1.0");
  chunked = FALSE;
}

void
Components::ConfigValue::_marshal_members (CORBA::DataEncoder &ec)
{
  const char * _name = name ();
  CORBA::_stc_string->marshal (ec, &_name);
  CORBA::Any& _value = value ();
  CORBA::_stc_any->marshal (ec, &_value);
}

CORBA::Boolean
Components::ConfigValue::_demarshal_members (CORBA::DataDecoder &dc)
{
  Components::FeatureName_var _name;
  if (!CORBA::_stc_string->demarshal (dc, &_name._for_demarshal())) {
      return FALSE;
  }
  name (_name);
  CORBA::Any _value;
  if (!CORBA::_stc_any->demarshal (dc, &_value)) {
      return FALSE;
  }
  value (_value);
  return TRUE;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConfigValue;
}

class _Marshaller_Components_ConfigValue : public ::CORBA::StaticTypeInfo {
    typedef Components::ConfigValue* _MICO_T;
  public:
    ~_Marshaller_Components_ConfigValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ConfigValue::~_Marshaller_Components_ConfigValue()
{
}

::CORBA::StaticValueType _Marshaller_Components_ConfigValue::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ConfigValue::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_ConfigValue::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_ConfigValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/Components/ConfigValue:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::Components::ConfigValue::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_Components_ConfigValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_Components_ConfigValue::typecode()
{
  return Components::_tc_ConfigValue;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ConfigValue;

void
operator<<=( CORBA::Any &_a, const Components::ConfigValue* _val )
{
  CORBA::StaticAny _sa (_marshaller_Components_ConfigValue, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ConfigValue** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_Components_ConfigValue, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ConfigValue* &_val_ptr )
{
  Components::ConfigValue* *p;
  if (_a.to_static_any (_marshaller_Components_ConfigValue, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// OBV class for valuetype ConfigValue
OBV_Components::ConfigValue::ConfigValue ()
{
}

OBV_Components::ConfigValue::ConfigValue (const char* _name, const CORBA::Any& _value)
{
  this->name(_name);
  this->value(_value);
}

OBV_Components::ConfigValue::~ConfigValue ()
{
}

void OBV_Components::ConfigValue::name( char *_p )
{
  _m.name = _p;
}

void OBV_Components::ConfigValue::name( const char *_p )
{
  _m.name = _p;
}

void OBV_Components::ConfigValue::name( const CORBA::String_var &_p )
{
  _m.name = _p;
}

const char *OBV_Components::ConfigValue::name() const
{
  return _m.name;
}

void OBV_Components::ConfigValue::value( const CORBA::Any& _p )
{
  _m.value = _p;
}

const CORBA::Any& OBV_Components::ConfigValue::value() const
{
  return (CORBA::Any&) _m.value;
}

CORBA::Any& OBV_Components::ConfigValue::value()
{
  return _m.value;
}

namespace Components
{
CORBA::TypeCodeConst _tc_ConfigValues;
}


/*
 * Base interface for class StandardConfigurator
 */

Components::StandardConfigurator::~StandardConfigurator()
{
}

void *
Components::StandardConfigurator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/StandardConfigurator:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::Configurator::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

Components::StandardConfigurator_ptr
Components::StandardConfigurator::_narrow( CORBA::Object_ptr _obj )
{
  Components::StandardConfigurator_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/StandardConfigurator:1.0" )))
      return _duplicate( (Components::StandardConfigurator_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/StandardConfigurator:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/StandardConfigurator:1.0")) {
      _o = new Components::StandardConfigurator_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::StandardConfigurator_ptr
Components::StandardConfigurator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_StandardConfigurator;
}
class _Marshaller_Components_StandardConfigurator : public ::CORBA::StaticTypeInfo {
    typedef Components::StandardConfigurator_ptr _MICO_T;
  public:
    ~_Marshaller_Components_StandardConfigurator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_StandardConfigurator::~_Marshaller_Components_StandardConfigurator()
{
}

::CORBA::StaticValueType _Marshaller_Components_StandardConfigurator::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_StandardConfigurator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::StandardConfigurator::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_StandardConfigurator::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_StandardConfigurator::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_StandardConfigurator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::StandardConfigurator::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_StandardConfigurator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_StandardConfigurator::typecode()
{
  return Components::_tc_StandardConfigurator;
}

::CORBA::StaticTypeInfo *_marshaller_Components_StandardConfigurator;

void
operator<<=( CORBA::Any &_a, const Components::StandardConfigurator_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_StandardConfigurator, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::StandardConfigurator_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_StandardConfigurator, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::StandardConfigurator_ptr &_obj )
{
  Components::StandardConfigurator_ptr *p;
  if (_a.to_static_any (_marshaller_Components_StandardConfigurator, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class StandardConfigurator
 */

Components::StandardConfigurator_stub::~StandardConfigurator_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::StandardConfigurator::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/Components/StandardConfigurator:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_Components::Configurator::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_Components::StandardConfigurator *
POA_Components::StandardConfigurator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/StandardConfigurator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::StandardConfigurator *) p;
  }
  return NULL;
}

Components::StandardConfigurator_stub_clp::StandardConfigurator_stub_clp ()
{
}

Components::StandardConfigurator_stub_clp::StandardConfigurator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::StandardConfigurator_stub_clp::~StandardConfigurator_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void Components::StandardConfigurator_stub::set_configuration( const Components::ConfigValues& _par_descr )
{
  CORBA::StaticAny _sa_descr( _marshaller__seq_Components_ConfigValue, &_par_descr );
  CORBA::StaticRequest __req( this, "set_configuration" );
  __req.add_in_arg( &_sa_descr );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::StandardConfigurator_stub_clp::set_configuration( const Components::ConfigValues& _par_descr )
{
  Components::StandardConfigurator_stub::set_configuration(_par_descr);
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_IllegalState;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::IllegalState::IllegalState()
{
}

Components::IllegalState::IllegalState( const IllegalState& _s )
{
}

Components::IllegalState::~IllegalState()
{
}

Components::IllegalState&
Components::IllegalState::operator=( const IllegalState& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_IllegalState : public ::CORBA::StaticTypeInfo {
    typedef ::Components::IllegalState _MICO_T;
  public:
    ~_Marshaller_Components_IllegalState();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_IllegalState::~_Marshaller_Components_IllegalState()
{
}

::CORBA::StaticValueType _Marshaller_Components_IllegalState::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_IllegalState::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_IllegalState::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_IllegalState::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_IllegalState::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/IllegalState:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_IllegalState::typecode()
{
  return Components::_tc_IllegalState;
}

::CORBA::StaticTypeInfo *_marshaller_Components_IllegalState;

void operator<<=( CORBA::Any &_a, const Components::IllegalState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_IllegalState, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::IllegalState *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::IllegalState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_IllegalState, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::IllegalState *&_e )
{
  return _a.to_static_any (_marshaller_Components_IllegalState, (void *&)_e);
}

void Components::IllegalState::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw IllegalState_var( (Components::IllegalState*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::IllegalState::_repoid() const
{
  return "IDL:omg.org/Components/IllegalState:1.0";
}

void Components::IllegalState::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_IllegalState->marshal( _en, (void*) this );
}

void Components::IllegalState::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::IllegalState::_clone() const
{
  return new IllegalState( *this );
}

Components::IllegalState *Components::IllegalState::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/IllegalState:1.0" ) )
    return (IllegalState *) _ex;
  return NULL;
}

const Components::IllegalState *Components::IllegalState::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/IllegalState:1.0" ) )
    return (IllegalState *) _ex;
  return NULL;
}


/*
 * Base interface for class CCMContext
 */

Components::CCMContext::~CCMContext()
{
}

void *
Components::CCMContext::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/CCMContext:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::CCMContext_ptr
Components::CCMContext::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/CCMContext:1.0" )))
      return _duplicate( (Components::CCMContext_ptr) _p );
  }
  return _nil();
}

Components::CCMContext_ptr
Components::CCMContext::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_CCMContext;
}


/*
 * Base interface for class SessionContext
 */

Components::SessionContext::~SessionContext()
{
}

void *
Components::SessionContext::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/SessionContext:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::CCMContext::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

Components::SessionContext_ptr
Components::SessionContext::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/SessionContext:1.0" )))
      return _duplicate( (Components::SessionContext_ptr) _p );
  }
  return _nil();
}

Components::SessionContext_ptr
Components::SessionContext::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_SessionContext;
}

namespace Components
{
CORBA::TypeCodeConst _tc_CCMExceptionReason;
}

void operator<<=( CORBA::Any &_a, const Components::CCMExceptionReason &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMExceptionReason, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::CCMExceptionReason &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMExceptionReason, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Components_CCMExceptionReason : public ::CORBA::StaticTypeInfo {
    typedef Components::CCMExceptionReason _MICO_T;
  public:
    ~_Marshaller_Components_CCMExceptionReason();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CCMExceptionReason::~_Marshaller_Components_CCMExceptionReason()
{
}

::CORBA::StaticValueType _Marshaller_Components_CCMExceptionReason::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_CCMExceptionReason::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_CCMExceptionReason::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_CCMExceptionReason::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Components_CCMExceptionReason::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Components_CCMExceptionReason::typecode()
{
  return Components::_tc_CCMExceptionReason;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CCMExceptionReason;

namespace Components
{
CORBA::TypeCodeConst _tc_CCMException;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::CCMException::CCMException()
{
}

Components::CCMException::CCMException( const CCMException& _s )
{
  reason = ((CCMException&)_s).reason;
}

Components::CCMException::~CCMException()
{
}

Components::CCMException&
Components::CCMException::operator=( const CCMException& _s )
{
  reason = ((CCMException&)_s).reason;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
Components::CCMException::CCMException()
{
}

#endif

Components::CCMException::CCMException( Components::CCMExceptionReason _m0 )
{
  reason = _m0;
}

class _Marshaller_Components_CCMException : public ::CORBA::StaticTypeInfo {
    typedef ::Components::CCMException _MICO_T;
  public:
    ~_Marshaller_Components_CCMException();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CCMException::~_Marshaller_Components_CCMException()
{
}

::CORBA::StaticValueType _Marshaller_Components_CCMException::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_CCMException::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_CCMException::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_CCMException::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    _marshaller_Components_CCMExceptionReason->demarshal( dc, &((_MICO_T*)v)->reason ) &&
    dc.except_end();
}

void _Marshaller_Components_CCMException::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/CCMException:1.0" );
  _marshaller_Components_CCMExceptionReason->marshal( ec, &((_MICO_T*)v)->reason );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_CCMException::typecode()
{
  return Components::_tc_CCMException;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CCMException;

void operator<<=( CORBA::Any &_a, const Components::CCMException &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMException, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::CCMException *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::CCMException &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CCMException, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::CCMException *&_e )
{
  return _a.to_static_any (_marshaller_Components_CCMException, (void *&)_e);
}

void Components::CCMException::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw CCMException_var( (Components::CCMException*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::CCMException::_repoid() const
{
  return "IDL:omg.org/Components/CCMException:1.0";
}

void Components::CCMException::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_CCMException->marshal( _en, (void*) this );
}

void Components::CCMException::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::CCMException::_clone() const
{
  return new CCMException( *this );
}

Components::CCMException *Components::CCMException::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CCMException:1.0" ) )
    return (CCMException *) _ex;
  return NULL;
}

const Components::CCMException *Components::CCMException::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CCMException:1.0" ) )
    return (CCMException *) _ex;
  return NULL;
}


/*
 * Base interface for class EnterpriseComponent
 */

Components::EnterpriseComponent::~EnterpriseComponent()
{
}

void *
Components::EnterpriseComponent::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/EnterpriseComponent:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::EnterpriseComponent_ptr
Components::EnterpriseComponent::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/EnterpriseComponent:1.0" )))
      return _duplicate( (Components::EnterpriseComponent_ptr) _p );
  }
  return _nil();
}

Components::EnterpriseComponent_ptr
Components::EnterpriseComponent::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_EnterpriseComponent;
}


/*
 * Base interface for class SessionComponent
 */

Components::SessionComponent::~SessionComponent()
{
}

void *
Components::SessionComponent::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/SessionComponent:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::EnterpriseComponent::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

Components::SessionComponent_ptr
Components::SessionComponent::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/SessionComponent:1.0" )))
      return _duplicate( (Components::SessionComponent_ptr) _p );
  }
  return _nil();
}

Components::SessionComponent_ptr
Components::SessionComponent::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_SessionComponent;
}


/*
 * Base interface for class HomeExecutorBase
 */

Components::HomeExecutorBase::~HomeExecutorBase()
{
}

void *
Components::HomeExecutorBase::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/HomeExecutorBase:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::HomeExecutorBase_ptr
Components::HomeExecutorBase::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/HomeExecutorBase:1.0" )))
      return _duplicate( (Components::HomeExecutorBase_ptr) _p );
  }
  return _nil();
}

Components::HomeExecutorBase_ptr
Components::HomeExecutorBase::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_HomeExecutorBase;
}


/*
 * Base interface for class ExecutorLocator
 */

Components::ExecutorLocator::~ExecutorLocator()
{
}

void *
Components::ExecutorLocator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/ExecutorLocator:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::EnterpriseComponent::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

Components::ExecutorLocator_ptr
Components::ExecutorLocator::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/ExecutorLocator:1.0" )))
      return _duplicate( (Components::ExecutorLocator_ptr) _p );
  }
  return _nil();
}

Components::ExecutorLocator_ptr
Components::ExecutorLocator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_ExecutorLocator;
}

namespace Components
{
CORBA::TypeCodeConst _tc_UnknownImplId;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::UnknownImplId::UnknownImplId()
{
}

Components::UnknownImplId::UnknownImplId( const UnknownImplId& _s )
{
}

Components::UnknownImplId::~UnknownImplId()
{
}

Components::UnknownImplId&
Components::UnknownImplId::operator=( const UnknownImplId& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_UnknownImplId : public ::CORBA::StaticTypeInfo {
    typedef ::Components::UnknownImplId _MICO_T;
  public:
    ~_Marshaller_Components_UnknownImplId();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_UnknownImplId::~_Marshaller_Components_UnknownImplId()
{
}

::CORBA::StaticValueType _Marshaller_Components_UnknownImplId::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_UnknownImplId::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_UnknownImplId::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_UnknownImplId::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_UnknownImplId::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/UnknownImplId:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_UnknownImplId::typecode()
{
  return Components::_tc_UnknownImplId;
}

::CORBA::StaticTypeInfo *_marshaller_Components_UnknownImplId;

void operator<<=( CORBA::Any &_a, const Components::UnknownImplId &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_UnknownImplId, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::UnknownImplId *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::UnknownImplId &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_UnknownImplId, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::UnknownImplId *&_e )
{
  return _a.to_static_any (_marshaller_Components_UnknownImplId, (void *&)_e);
}

void Components::UnknownImplId::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw UnknownImplId_var( (Components::UnknownImplId*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::UnknownImplId::_repoid() const
{
  return "IDL:omg.org/Components/UnknownImplId:1.0";
}

void Components::UnknownImplId::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_UnknownImplId->marshal( _en, (void*) this );
}

void Components::UnknownImplId::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::UnknownImplId::_clone() const
{
  return new UnknownImplId( *this );
}

Components::UnknownImplId *Components::UnknownImplId::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/UnknownImplId:1.0" ) )
    return (UnknownImplId *) _ex;
  return NULL;
}

const Components::UnknownImplId *Components::UnknownImplId::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/UnknownImplId:1.0" ) )
    return (UnknownImplId *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidLocation;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidLocation::InvalidLocation()
{
}

Components::InvalidLocation::InvalidLocation( const InvalidLocation& _s )
{
}

Components::InvalidLocation::~InvalidLocation()
{
}

Components::InvalidLocation&
Components::InvalidLocation::operator=( const InvalidLocation& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidLocation : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidLocation _MICO_T;
  public:
    ~_Marshaller_Components_InvalidLocation();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidLocation::~_Marshaller_Components_InvalidLocation()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidLocation::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidLocation::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidLocation::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidLocation::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidLocation::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidLocation:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidLocation::typecode()
{
  return Components::_tc_InvalidLocation;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidLocation;

void operator<<=( CORBA::Any &_a, const Components::InvalidLocation &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidLocation, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidLocation *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidLocation &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidLocation, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidLocation *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidLocation, (void *&)_e);
}

void Components::InvalidLocation::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidLocation_var( (Components::InvalidLocation*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidLocation::_repoid() const
{
  return "IDL:omg.org/Components/InvalidLocation:1.0";
}

void Components::InvalidLocation::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidLocation->marshal( _en, (void*) this );
}

void Components::InvalidLocation::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidLocation::_clone() const
{
  return new InvalidLocation( *this );
}

Components::InvalidLocation *Components::InvalidLocation::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidLocation:1.0" ) )
    return (InvalidLocation *) _ex;
  return NULL;
}

const Components::InvalidLocation *Components::InvalidLocation::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidLocation:1.0" ) )
    return (InvalidLocation *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_CreateFailure;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::CreateFailure::CreateFailure()
{
}

Components::CreateFailure::CreateFailure( const CreateFailure& _s )
{
}

Components::CreateFailure::~CreateFailure()
{
}

Components::CreateFailure&
Components::CreateFailure::operator=( const CreateFailure& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_CreateFailure : public ::CORBA::StaticTypeInfo {
    typedef ::Components::CreateFailure _MICO_T;
  public:
    ~_Marshaller_Components_CreateFailure();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_CreateFailure::~_Marshaller_Components_CreateFailure()
{
}

::CORBA::StaticValueType _Marshaller_Components_CreateFailure::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_CreateFailure::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_CreateFailure::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_CreateFailure::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_CreateFailure::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/CreateFailure:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_CreateFailure::typecode()
{
  return Components::_tc_CreateFailure;
}

::CORBA::StaticTypeInfo *_marshaller_Components_CreateFailure;

void operator<<=( CORBA::Any &_a, const Components::CreateFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CreateFailure, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::CreateFailure *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::CreateFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_CreateFailure, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::CreateFailure *&_e )
{
  return _a.to_static_any (_marshaller_Components_CreateFailure, (void *&)_e);
}

void Components::CreateFailure::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw CreateFailure_var( (Components::CreateFailure*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::CreateFailure::_repoid() const
{
  return "IDL:omg.org/Components/CreateFailure:1.0";
}

void Components::CreateFailure::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_CreateFailure->marshal( _en, (void*) this );
}

void Components::CreateFailure::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::CreateFailure::_clone() const
{
  return new CreateFailure( *this );
}

Components::CreateFailure *Components::CreateFailure::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CreateFailure:1.0" ) )
    return (CreateFailure *) _ex;
  return NULL;
}

const Components::CreateFailure *Components::CreateFailure::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/CreateFailure:1.0" ) )
    return (CreateFailure *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_RemoveFailure;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::RemoveFailure::RemoveFailure()
{
}

Components::RemoveFailure::RemoveFailure( const RemoveFailure& _s )
{
}

Components::RemoveFailure::~RemoveFailure()
{
}

Components::RemoveFailure&
Components::RemoveFailure::operator=( const RemoveFailure& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_RemoveFailure : public ::CORBA::StaticTypeInfo {
    typedef ::Components::RemoveFailure _MICO_T;
  public:
    ~_Marshaller_Components_RemoveFailure();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_RemoveFailure::~_Marshaller_Components_RemoveFailure()
{
}

::CORBA::StaticValueType _Marshaller_Components_RemoveFailure::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_RemoveFailure::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_RemoveFailure::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_RemoveFailure::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_RemoveFailure::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/RemoveFailure:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_RemoveFailure::typecode()
{
  return Components::_tc_RemoveFailure;
}

::CORBA::StaticTypeInfo *_marshaller_Components_RemoveFailure;

void operator<<=( CORBA::Any &_a, const Components::RemoveFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_RemoveFailure, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::RemoveFailure *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::RemoveFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_RemoveFailure, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::RemoveFailure *&_e )
{
  return _a.to_static_any (_marshaller_Components_RemoveFailure, (void *&)_e);
}

void Components::RemoveFailure::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw RemoveFailure_var( (Components::RemoveFailure*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::RemoveFailure::_repoid() const
{
  return "IDL:omg.org/Components/RemoveFailure:1.0";
}

void Components::RemoveFailure::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_RemoveFailure->marshal( _en, (void*) this );
}

void Components::RemoveFailure::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::RemoveFailure::_clone() const
{
  return new RemoveFailure( *this );
}

Components::RemoveFailure *Components::RemoveFailure::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/RemoveFailure:1.0" ) )
    return (RemoveFailure *) _ex;
  return NULL;
}

const Components::RemoveFailure *Components::RemoveFailure::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/RemoveFailure:1.0" ) )
    return (RemoveFailure *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InstallationFailure;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InstallationFailure::InstallationFailure()
{
}

Components::InstallationFailure::InstallationFailure( const InstallationFailure& _s )
{
}

Components::InstallationFailure::~InstallationFailure()
{
}

Components::InstallationFailure&
Components::InstallationFailure::operator=( const InstallationFailure& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InstallationFailure : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InstallationFailure _MICO_T;
  public:
    ~_Marshaller_Components_InstallationFailure();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InstallationFailure::~_Marshaller_Components_InstallationFailure()
{
}

::CORBA::StaticValueType _Marshaller_Components_InstallationFailure::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InstallationFailure::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InstallationFailure::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InstallationFailure::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InstallationFailure::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InstallationFailure:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InstallationFailure::typecode()
{
  return Components::_tc_InstallationFailure;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InstallationFailure;

void operator<<=( CORBA::Any &_a, const Components::InstallationFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InstallationFailure, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InstallationFailure *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InstallationFailure &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InstallationFailure, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InstallationFailure *&_e )
{
  return _a.to_static_any (_marshaller_Components_InstallationFailure, (void *&)_e);
}

void Components::InstallationFailure::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InstallationFailure_var( (Components::InstallationFailure*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InstallationFailure::_repoid() const
{
  return "IDL:omg.org/Components/InstallationFailure:1.0";
}

void Components::InstallationFailure::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InstallationFailure->marshal( _en, (void*) this );
}

void Components::InstallationFailure::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InstallationFailure::_clone() const
{
  return new InstallationFailure( *this );
}

Components::InstallationFailure *Components::InstallationFailure::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InstallationFailure:1.0" ) )
    return (InstallationFailure *) _ex;
  return NULL;
}

const Components::InstallationFailure *Components::InstallationFailure::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InstallationFailure:1.0" ) )
    return (InstallationFailure *) _ex;
  return NULL;
}

namespace Components
{
CORBA::TypeCodeConst _tc_InvalidAssembly;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Components::InvalidAssembly::InvalidAssembly()
{
}

Components::InvalidAssembly::InvalidAssembly( const InvalidAssembly& _s )
{
}

Components::InvalidAssembly::~InvalidAssembly()
{
}

Components::InvalidAssembly&
Components::InvalidAssembly::operator=( const InvalidAssembly& _s )
{
  return *this;
}
#endif

class _Marshaller_Components_InvalidAssembly : public ::CORBA::StaticTypeInfo {
    typedef ::Components::InvalidAssembly _MICO_T;
  public:
    ~_Marshaller_Components_InvalidAssembly();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_InvalidAssembly::~_Marshaller_Components_InvalidAssembly()
{
}

::CORBA::StaticValueType _Marshaller_Components_InvalidAssembly::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_InvalidAssembly::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_InvalidAssembly::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_InvalidAssembly::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Components_InvalidAssembly::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Components/InvalidAssembly:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Components_InvalidAssembly::typecode()
{
  return Components::_tc_InvalidAssembly;
}

::CORBA::StaticTypeInfo *_marshaller_Components_InvalidAssembly;

void operator<<=( CORBA::Any &_a, const Components::InvalidAssembly &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidAssembly, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Components::InvalidAssembly *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::InvalidAssembly &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_InvalidAssembly, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Components::InvalidAssembly *&_e )
{
  return _a.to_static_any (_marshaller_Components_InvalidAssembly, (void *&)_e);
}

void Components::InvalidAssembly::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw InvalidAssembly_var( (Components::InvalidAssembly*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Components::InvalidAssembly::_repoid() const
{
  return "IDL:omg.org/Components/InvalidAssembly:1.0";
}

void Components::InvalidAssembly::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Components_InvalidAssembly->marshal( _en, (void*) this );
}

void Components::InvalidAssembly::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Components::InvalidAssembly::_clone() const
{
  return new InvalidAssembly( *this );
}

Components::InvalidAssembly *Components::InvalidAssembly::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidAssembly:1.0" ) )
    return (InvalidAssembly *) _ex;
  return NULL;
}

const Components::InvalidAssembly *Components::InvalidAssembly::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Components/InvalidAssembly:1.0" ) )
    return (InvalidAssembly *) _ex;
  return NULL;
}


/*
 * Base interface for class Container
 */

Components::Container::~Container()
{
}

void *
Components::Container::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Container:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Container_ptr
Components::Container::_narrow( CORBA::Object_ptr _obj )
{
  Components::Container_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Container:1.0" )))
      return _duplicate( (Components::Container_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Container:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Container:1.0")) {
      _o = new Components::Container_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Container_ptr
Components::Container::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Container;
}
class _Marshaller_Components_Container : public ::CORBA::StaticTypeInfo {
    typedef Components::Container_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Container();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Container::~_Marshaller_Components_Container()
{
}

::CORBA::StaticValueType _Marshaller_Components_Container::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Container::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Container::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Container::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Container::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Container::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Container::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Container::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Container::typecode()
{
  return Components::_tc_Container;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Container;

void
operator<<=( CORBA::Any &_a, const Components::Container_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Container, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Container_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Container, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Container_ptr &_obj )
{
  Components::Container_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Container, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Container
 */

Components::Container_stub::~Container_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Container::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Container:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Container *
POA_Components::Container::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Container:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Container *) p;
  }
  return NULL;
}

Components::Container_stub_clp::Container_stub_clp ()
{
}

Components::Container_stub_clp::Container_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Container_stub_clp::~Container_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::ConfigValues* Components::Container_stub::configuration()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_ConfigValue );

  CORBA::StaticRequest __req( this, "_get_configuration" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ConfigValues*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ConfigValues*
Components::Container_stub_clp::configuration()
{
  return Components::Container_stub::configuration();
}

#endif // MICO_CONF_NO_POA

Components::ComponentServer_ptr Components::Container_stub::get_component_server()
{
  Components::ComponentServer_ptr _res = Components::ComponentServer::_nil();
  CORBA::StaticAny __res( _marshaller_Components_ComponentServer, &_res );

  CORBA::StaticRequest __req( this, "get_component_server" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::ComponentServer_ptr
Components::Container_stub_clp::get_component_server()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Container * _myserv = POA_Components::Container::_narrow (_serv);
    if (_myserv) {
      Components::ComponentServer_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_component_server();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Container_stub::get_component_server();
}

#endif // MICO_CONF_NO_POA

Components::CCMHome_ptr Components::Container_stub::install_home( const char* _par_id, const char* _par_entrypt, const Components::ConfigValues& _par_config )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_entrypt( CORBA::_stc_string, &_par_entrypt );
  CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );
  Components::CCMHome_ptr _res = Components::CCMHome::_nil();
  CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );

  CORBA::StaticRequest __req( this, "install_home" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_entrypt );
  __req.add_in_arg( &_sa_config );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidConfiguration, "IDL:omg.org/Components/InvalidConfiguration:1.0",
    _marshaller_Components_InstallationFailure, "IDL:omg.org/Components/InstallationFailure:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::CCMHome_ptr
Components::Container_stub_clp::install_home( const char* _par_id, const char* _par_entrypt, const Components::ConfigValues& _par_config )
{
  return Components::Container_stub::install_home(_par_id, _par_entrypt, _par_config);
}

#endif // MICO_CONF_NO_POA

void Components::Container_stub::remove_home( Components::CCMHome_ptr _par_href )
{
  CORBA::StaticAny _sa_href( _marshaller_Components_CCMHome, &_par_href );
  CORBA::StaticRequest __req( this, "remove_home" );
  __req.add_in_arg( &_sa_href );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Container_stub_clp::remove_home( Components::CCMHome_ptr _par_href )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Container * _myserv = POA_Components::Container::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove_home(_par_href);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Container_stub::remove_home(_par_href);
}

#endif // MICO_CONF_NO_POA

Components::CCMHomes* Components::Container_stub::get_homes()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_CCMHome );

  CORBA::StaticRequest __req( this, "get_homes" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::CCMHomes*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::CCMHomes*
Components::Container_stub_clp::get_homes()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Container * _myserv = POA_Components::Container::_narrow (_serv);
    if (_myserv) {
      Components::CCMHomes* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_homes();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Container_stub::get_homes();
}

#endif // MICO_CONF_NO_POA

void Components::Container_stub::remove()
{
  CORBA::StaticRequest __req( this, "remove" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Container_stub_clp::remove()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Container * _myserv = POA_Components::Container::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Container_stub::remove();
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_Containers;
}


/*
 * Base interface for class ComponentServer
 */

Components::ComponentServer::~ComponentServer()
{
}

void *
Components::ComponentServer::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/ComponentServer:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::ComponentServer_ptr
Components::ComponentServer::_narrow( CORBA::Object_ptr _obj )
{
  Components::ComponentServer_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/ComponentServer:1.0" )))
      return _duplicate( (Components::ComponentServer_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/ComponentServer:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/ComponentServer:1.0")) {
      _o = new Components::ComponentServer_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::ComponentServer_ptr
Components::ComponentServer::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_ComponentServer;
}
class _Marshaller_Components_ComponentServer : public ::CORBA::StaticTypeInfo {
    typedef Components::ComponentServer_ptr _MICO_T;
  public:
    ~_Marshaller_Components_ComponentServer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ComponentServer::~_Marshaller_Components_ComponentServer()
{
}

::CORBA::StaticValueType _Marshaller_Components_ComponentServer::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ComponentServer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::ComponentServer::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_ComponentServer::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_ComponentServer::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_ComponentServer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::ComponentServer::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_ComponentServer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_ComponentServer::typecode()
{
  return Components::_tc_ComponentServer;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ComponentServer;

void
operator<<=( CORBA::Any &_a, const Components::ComponentServer_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_ComponentServer, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ComponentServer_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_ComponentServer, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ComponentServer_ptr &_obj )
{
  Components::ComponentServer_ptr *p;
  if (_a.to_static_any (_marshaller_Components_ComponentServer, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ComponentServer
 */

Components::ComponentServer_stub::~ComponentServer_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::ComponentServer::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ComponentServer:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::ComponentServer *
POA_Components::ComponentServer::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/ComponentServer:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::ComponentServer *) p;
  }
  return NULL;
}

Components::ComponentServer_stub_clp::ComponentServer_stub_clp ()
{
}

Components::ComponentServer_stub_clp::ComponentServer_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::ComponentServer_stub_clp::~ComponentServer_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::ConfigValues* Components::ComponentServer_stub::configuration()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_ConfigValue );

  CORBA::StaticRequest __req( this, "_get_configuration" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ConfigValues*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ConfigValues*
Components::ComponentServer_stub_clp::configuration()
{
  return Components::ComponentServer_stub::configuration();
}

#endif // MICO_CONF_NO_POA

Components::ServerActivator_ptr Components::ComponentServer_stub::get_server_activator()
{
  Components::ServerActivator_ptr _res = Components::ServerActivator::_nil();
  CORBA::StaticAny __res( _marshaller_Components_ServerActivator, &_res );

  CORBA::StaticRequest __req( this, "get_server_activator" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::ServerActivator_ptr
Components::ComponentServer_stub_clp::get_server_activator()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentServer * _myserv = POA_Components::ComponentServer::_narrow (_serv);
    if (_myserv) {
      Components::ServerActivator_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_server_activator();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::ComponentServer_stub::get_server_activator();
}

#endif // MICO_CONF_NO_POA

Components::Container_ptr Components::ComponentServer_stub::create_container( const Components::ConfigValues& _par_config )
{
  CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );
  Components::Container_ptr _res = Components::Container::_nil();
  CORBA::StaticAny __res( _marshaller_Components_Container, &_res );

  CORBA::StaticRequest __req( this, "create_container" );
  __req.add_in_arg( &_sa_config );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_CreateFailure, "IDL:omg.org/Components/CreateFailure:1.0",
    _marshaller_Components_InvalidConfiguration, "IDL:omg.org/Components/InvalidConfiguration:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::Container_ptr
Components::ComponentServer_stub_clp::create_container( const Components::ConfigValues& _par_config )
{
  return Components::ComponentServer_stub::create_container(_par_config);
}

#endif // MICO_CONF_NO_POA

void Components::ComponentServer_stub::remove_container( Components::Container_ptr _par_cref )
{
  CORBA::StaticAny _sa_cref( _marshaller_Components_Container, &_par_cref );
  CORBA::StaticRequest __req( this, "remove_container" );
  __req.add_in_arg( &_sa_cref );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ComponentServer_stub_clp::remove_container( Components::Container_ptr _par_cref )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentServer * _myserv = POA_Components::ComponentServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove_container(_par_cref);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ComponentServer_stub::remove_container(_par_cref);
}

#endif // MICO_CONF_NO_POA

Components::Containers* Components::ComponentServer_stub::get_containers()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_Container );

  CORBA::StaticRequest __req( this, "get_containers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::Containers*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::Containers*
Components::ComponentServer_stub_clp::get_containers()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentServer * _myserv = POA_Components::ComponentServer::_narrow (_serv);
    if (_myserv) {
      Components::Containers* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_containers();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::ComponentServer_stub::get_containers();
}

#endif // MICO_CONF_NO_POA

void Components::ComponentServer_stub::remove()
{
  CORBA::StaticRequest __req( this, "remove" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ComponentServer_stub_clp::remove()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentServer * _myserv = POA_Components::ComponentServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ComponentServer_stub::remove();
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_ComponentServers;
}

namespace Components
{
CORBA::TypeCodeConst _tc_UUID;
}

namespace Components
{
CORBA::TypeCodeConst _tc_Location;
}


/*
 * Base interface for class ComponentInstallation
 */

Components::ComponentInstallation::~ComponentInstallation()
{
}

void *
Components::ComponentInstallation::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/ComponentInstallation:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::ComponentInstallation_ptr
Components::ComponentInstallation::_narrow( CORBA::Object_ptr _obj )
{
  Components::ComponentInstallation_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/ComponentInstallation:1.0" )))
      return _duplicate( (Components::ComponentInstallation_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/ComponentInstallation:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/ComponentInstallation:1.0")) {
      _o = new Components::ComponentInstallation_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::ComponentInstallation_ptr
Components::ComponentInstallation::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_ComponentInstallation;
}
class _Marshaller_Components_ComponentInstallation : public ::CORBA::StaticTypeInfo {
    typedef Components::ComponentInstallation_ptr _MICO_T;
  public:
    ~_Marshaller_Components_ComponentInstallation();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ComponentInstallation::~_Marshaller_Components_ComponentInstallation()
{
}

::CORBA::StaticValueType _Marshaller_Components_ComponentInstallation::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ComponentInstallation::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::ComponentInstallation::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_ComponentInstallation::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_ComponentInstallation::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_ComponentInstallation::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::ComponentInstallation::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_ComponentInstallation::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_ComponentInstallation::typecode()
{
  return Components::_tc_ComponentInstallation;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ComponentInstallation;

void
operator<<=( CORBA::Any &_a, const Components::ComponentInstallation_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_ComponentInstallation, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ComponentInstallation_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_ComponentInstallation, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ComponentInstallation_ptr &_obj )
{
  Components::ComponentInstallation_ptr *p;
  if (_a.to_static_any (_marshaller_Components_ComponentInstallation, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ComponentInstallation
 */

Components::ComponentInstallation_stub::~ComponentInstallation_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::ComponentInstallation::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ComponentInstallation:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::ComponentInstallation *
POA_Components::ComponentInstallation::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/ComponentInstallation:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::ComponentInstallation *) p;
  }
  return NULL;
}

Components::ComponentInstallation_stub_clp::ComponentInstallation_stub_clp ()
{
}

Components::ComponentInstallation_stub_clp::ComponentInstallation_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::ComponentInstallation_stub_clp::~ComponentInstallation_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void Components::ComponentInstallation_stub::install( const char* _par_implUUID, const char* _par_component_loc )
{
  CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID );
  CORBA::StaticAny _sa_component_loc( CORBA::_stc_string, &_par_component_loc );
  CORBA::StaticRequest __req( this, "install" );
  __req.add_in_arg( &_sa_implUUID );
  __req.add_in_arg( &_sa_component_loc );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidLocation, "IDL:omg.org/Components/InvalidLocation:1.0",
    _marshaller_Components_InstallationFailure, "IDL:omg.org/Components/InstallationFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ComponentInstallation_stub_clp::install( const char* _par_implUUID, const char* _par_component_loc )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentInstallation * _myserv = POA_Components::ComponentInstallation::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->install(_par_implUUID, _par_component_loc);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ComponentInstallation_stub::install(_par_implUUID, _par_component_loc);
}

#endif // MICO_CONF_NO_POA

void Components::ComponentInstallation_stub::replace( const char* _par_implUUID, const char* _par_component_loc )
{
  CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID );
  CORBA::StaticAny _sa_component_loc( CORBA::_stc_string, &_par_component_loc );
  CORBA::StaticRequest __req( this, "replace" );
  __req.add_in_arg( &_sa_implUUID );
  __req.add_in_arg( &_sa_component_loc );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidLocation, "IDL:omg.org/Components/InvalidLocation:1.0",
    _marshaller_Components_InstallationFailure, "IDL:omg.org/Components/InstallationFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ComponentInstallation_stub_clp::replace( const char* _par_implUUID, const char* _par_component_loc )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentInstallation * _myserv = POA_Components::ComponentInstallation::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->replace(_par_implUUID, _par_component_loc);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ComponentInstallation_stub::replace(_par_implUUID, _par_component_loc);
}

#endif // MICO_CONF_NO_POA

void Components::ComponentInstallation_stub::remove( const char* _par_implUUID )
{
  CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID );
  CORBA::StaticRequest __req( this, "remove" );
  __req.add_in_arg( &_sa_implUUID );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_UnknownImplId, "IDL:omg.org/Components/UnknownImplId:1.0",
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ComponentInstallation_stub_clp::remove( const char* _par_implUUID )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentInstallation * _myserv = POA_Components::ComponentInstallation::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove(_par_implUUID);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ComponentInstallation_stub::remove(_par_implUUID);
}

#endif // MICO_CONF_NO_POA

char* Components::ComponentInstallation_stub::get_implementation( const char* _par_implUUID )
{
  CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID );
  Components::Location _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "get_implementation" );
  __req.add_in_arg( &_sa_implUUID );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_UnknownImplId, "IDL:omg.org/Components/UnknownImplId:1.0",
    _marshaller_Components_InstallationFailure, "IDL:omg.org/Components/InstallationFailure:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
Components::ComponentInstallation_stub_clp::get_implementation( const char* _par_implUUID )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ComponentInstallation * _myserv = POA_Components::ComponentInstallation::_narrow (_serv);
    if (_myserv) {
      Components::Location __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_implementation(_par_implUUID);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::ComponentInstallation_stub::get_implementation(_par_implUUID);
}

#endif // MICO_CONF_NO_POA

namespace Components
{
CORBA::TypeCodeConst _tc_AssemblyState;
}

void operator<<=( CORBA::Any &_a, const Components::AssemblyState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_AssemblyState, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Components::AssemblyState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Components_AssemblyState, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Components_AssemblyState : public ::CORBA::StaticTypeInfo {
    typedef Components::AssemblyState _MICO_T;
  public:
    ~_Marshaller_Components_AssemblyState();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_AssemblyState::~_Marshaller_Components_AssemblyState()
{
}

::CORBA::StaticValueType _Marshaller_Components_AssemblyState::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Components_AssemblyState::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Components_AssemblyState::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Components_AssemblyState::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Components_AssemblyState::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Components_AssemblyState::typecode()
{
  return Components::_tc_AssemblyState;
}

::CORBA::StaticTypeInfo *_marshaller_Components_AssemblyState;


/*
 * Base interface for class Assembly
 */

Components::Assembly::~Assembly()
{
}

void *
Components::Assembly::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/Assembly:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::Assembly_ptr
Components::Assembly::_narrow( CORBA::Object_ptr _obj )
{
  Components::Assembly_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/Assembly:1.0" )))
      return _duplicate( (Components::Assembly_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/Assembly:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/Assembly:1.0")) {
      _o = new Components::Assembly_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::Assembly_ptr
Components::Assembly::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_Assembly;
}
class _Marshaller_Components_Assembly : public ::CORBA::StaticTypeInfo {
    typedef Components::Assembly_ptr _MICO_T;
  public:
    ~_Marshaller_Components_Assembly();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_Assembly::~_Marshaller_Components_Assembly()
{
}

::CORBA::StaticValueType _Marshaller_Components_Assembly::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_Assembly::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::Assembly::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_Assembly::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_Assembly::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_Assembly::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::Assembly::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_Assembly::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_Assembly::typecode()
{
  return Components::_tc_Assembly;
}

::CORBA::StaticTypeInfo *_marshaller_Components_Assembly;

void
operator<<=( CORBA::Any &_a, const Components::Assembly_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_Assembly, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::Assembly_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_Assembly, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::Assembly_ptr &_obj )
{
  Components::Assembly_ptr *p;
  if (_a.to_static_any (_marshaller_Components_Assembly, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Assembly
 */

Components::Assembly_stub::~Assembly_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::Assembly::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Assembly:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::Assembly *
POA_Components::Assembly::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/Assembly:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::Assembly *) p;
  }
  return NULL;
}

Components::Assembly_stub_clp::Assembly_stub_clp ()
{
}

Components::Assembly_stub_clp::Assembly_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::Assembly_stub_clp::~Assembly_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void Components::Assembly_stub::build()
{
  CORBA::StaticRequest __req( this, "build" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_CreateFailure, "IDL:omg.org/Components/CreateFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Assembly_stub_clp::build()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Assembly * _myserv = POA_Components::Assembly::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->build();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Assembly_stub::build();
}

#endif // MICO_CONF_NO_POA

void Components::Assembly_stub::tear_down()
{
  CORBA::StaticRequest __req( this, "tear_down" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::Assembly_stub_clp::tear_down()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Assembly * _myserv = POA_Components::Assembly::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->tear_down();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::Assembly_stub::tear_down();
}

#endif // MICO_CONF_NO_POA

Components::AssemblyState Components::Assembly_stub::get_state()
{
  Components::AssemblyState _res;
  CORBA::StaticAny __res( _marshaller_Components_AssemblyState, &_res );

  CORBA::StaticRequest __req( this, "get_state" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::AssemblyState
Components::Assembly_stub_clp::get_state()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::Assembly * _myserv = POA_Components::Assembly::_narrow (_serv);
    if (_myserv) {
      Components::AssemblyState __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_state();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::Assembly_stub::get_state();
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class AssemblyFactory
 */

Components::AssemblyFactory::~AssemblyFactory()
{
}

void *
Components::AssemblyFactory::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/AssemblyFactory:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::AssemblyFactory_ptr
Components::AssemblyFactory::_narrow( CORBA::Object_ptr _obj )
{
  Components::AssemblyFactory_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/AssemblyFactory:1.0" )))
      return _duplicate( (Components::AssemblyFactory_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/AssemblyFactory:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/AssemblyFactory:1.0")) {
      _o = new Components::AssemblyFactory_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::AssemblyFactory_ptr
Components::AssemblyFactory::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_AssemblyFactory;
}
class _Marshaller_Components_AssemblyFactory : public ::CORBA::StaticTypeInfo {
    typedef Components::AssemblyFactory_ptr _MICO_T;
  public:
    ~_Marshaller_Components_AssemblyFactory();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_AssemblyFactory::~_Marshaller_Components_AssemblyFactory()
{
}

::CORBA::StaticValueType _Marshaller_Components_AssemblyFactory::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_AssemblyFactory::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::AssemblyFactory::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_AssemblyFactory::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_AssemblyFactory::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_AssemblyFactory::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::AssemblyFactory::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_AssemblyFactory::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_AssemblyFactory::typecode()
{
  return Components::_tc_AssemblyFactory;
}

::CORBA::StaticTypeInfo *_marshaller_Components_AssemblyFactory;

void
operator<<=( CORBA::Any &_a, const Components::AssemblyFactory_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_AssemblyFactory, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::AssemblyFactory_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_AssemblyFactory, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::AssemblyFactory_ptr &_obj )
{
  Components::AssemblyFactory_ptr *p;
  if (_a.to_static_any (_marshaller_Components_AssemblyFactory, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class AssemblyFactory
 */

Components::AssemblyFactory_stub::~AssemblyFactory_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::AssemblyFactory::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/AssemblyFactory:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::AssemblyFactory *
POA_Components::AssemblyFactory::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/AssemblyFactory:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::AssemblyFactory *) p;
  }
  return NULL;
}

Components::AssemblyFactory_stub_clp::AssemblyFactory_stub_clp ()
{
}

Components::AssemblyFactory_stub_clp::AssemblyFactory_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::AssemblyFactory_stub_clp::~AssemblyFactory_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::Cookie* Components::AssemblyFactory_stub::create( const char* _par_assembly_loc )
{
  CORBA::StaticAny _sa_assembly_loc( CORBA::_stc_string, &_par_assembly_loc );
  Components::Cookie* _res = NULL;
  CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );

  CORBA::StaticRequest __req( this, "create" );
  __req.add_in_arg( &_sa_assembly_loc );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidLocation, "IDL:omg.org/Components/InvalidLocation:1.0",
    _marshaller_Components_CreateFailure, "IDL:omg.org/Components/CreateFailure:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::Cookie*
Components::AssemblyFactory_stub_clp::create( const char* _par_assembly_loc )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::AssemblyFactory * _myserv = POA_Components::AssemblyFactory::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create(_par_assembly_loc);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      Components::Cookie* __res2 = Components::Cookie::_downcast (__res->_copy_value ());
      CORBA::remove_ref (__res);
      return __res2;
    }
    _postinvoke ();
  }

  return Components::AssemblyFactory_stub::create(_par_assembly_loc);
}

#endif // MICO_CONF_NO_POA

Components::Assembly_ptr Components::AssemblyFactory_stub::lookup( Components::Cookie* _par_c )
{
  CORBA::StaticAny _sa_c( _marshaller_Components_Cookie, &_par_c );
  Components::Assembly_ptr _res = Components::Assembly::_nil();
  CORBA::StaticAny __res( _marshaller_Components_Assembly, &_res );

  CORBA::StaticRequest __req( this, "lookup" );
  __req.add_in_arg( &_sa_c );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidAssembly, "IDL:omg.org/Components/InvalidAssembly:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::Assembly_ptr
Components::AssemblyFactory_stub_clp::lookup( Components::Cookie* _par_c )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::AssemblyFactory * _myserv = POA_Components::AssemblyFactory::_narrow (_serv);
    if (_myserv) {
      Components::Assembly_ptr __res;

      Components::Cookie* _copy_of_par_c;
      _copy_of_par_c = Components::Cookie::_downcast (_par_c->_copy_value());
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->lookup(_copy_of_par_c);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      CORBA::remove_ref (_copy_of_par_c);
      return __res;
    }
    _postinvoke ();
  }

  return Components::AssemblyFactory_stub::lookup(_par_c);
}

#endif // MICO_CONF_NO_POA

void Components::AssemblyFactory_stub::destroy( Components::Cookie* _par_c )
{
  CORBA::StaticAny _sa_c( _marshaller_Components_Cookie, &_par_c );
  CORBA::StaticRequest __req( this, "destroy" );
  __req.add_in_arg( &_sa_c );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_InvalidAssembly, "IDL:omg.org/Components/InvalidAssembly:1.0",
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::AssemblyFactory_stub_clp::destroy( Components::Cookie* _par_c )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::AssemblyFactory * _myserv = POA_Components::AssemblyFactory::_narrow (_serv);
    if (_myserv) {
      Components::Cookie* _copy_of_par_c;
      _copy_of_par_c = Components::Cookie::_downcast (_par_c->_copy_value());
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->destroy(_copy_of_par_c);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      CORBA::remove_ref (_copy_of_par_c);
      return;
    }
    _postinvoke ();
  }

  Components::AssemblyFactory_stub::destroy(_par_c);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ServerActivator
 */

Components::ServerActivator::~ServerActivator()
{
}

void *
Components::ServerActivator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Components/ServerActivator:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Components::ServerActivator_ptr
Components::ServerActivator::_narrow( CORBA::Object_ptr _obj )
{
  Components::ServerActivator_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Components/ServerActivator:1.0" )))
      return _duplicate( (Components::ServerActivator_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Components/ServerActivator:1.0") || _obj->_is_a_remote ("IDL:omg.org/Components/ServerActivator:1.0")) {
      _o = new Components::ServerActivator_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Components::ServerActivator_ptr
Components::ServerActivator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Components
{
CORBA::TypeCodeConst _tc_ServerActivator;
}
class _Marshaller_Components_ServerActivator : public ::CORBA::StaticTypeInfo {
    typedef Components::ServerActivator_ptr _MICO_T;
  public:
    ~_Marshaller_Components_ServerActivator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Components_ServerActivator::~_Marshaller_Components_ServerActivator()
{
}

::CORBA::StaticValueType _Marshaller_Components_ServerActivator::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Components_ServerActivator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Components::ServerActivator::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Components_ServerActivator::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Components_ServerActivator::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Components_ServerActivator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Components::ServerActivator::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Components_ServerActivator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Components_ServerActivator::typecode()
{
  return Components::_tc_ServerActivator;
}

::CORBA::StaticTypeInfo *_marshaller_Components_ServerActivator;

void
operator<<=( CORBA::Any &_a, const Components::ServerActivator_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Components_ServerActivator, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Components::ServerActivator_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Components_ServerActivator, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Components::ServerActivator_ptr &_obj )
{
  Components::ServerActivator_ptr *p;
  if (_a.to_static_any (_marshaller_Components_ServerActivator, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ServerActivator
 */

Components::ServerActivator_stub::~ServerActivator_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Components::ServerActivator::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ServerActivator:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Components::ServerActivator *
POA_Components::ServerActivator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Components/ServerActivator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Components::ServerActivator *) p;
  }
  return NULL;
}

Components::ServerActivator_stub_clp::ServerActivator_stub_clp ()
{
}

Components::ServerActivator_stub_clp::ServerActivator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Components::ServerActivator_stub_clp::~ServerActivator_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

Components::ComponentServer_ptr Components::ServerActivator_stub::create_component_server( const Components::ConfigValues& _par_config )
{
  CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );
  Components::ComponentServer_ptr _res = Components::ComponentServer::_nil();
  CORBA::StaticAny __res( _marshaller_Components_ComponentServer, &_res );

  CORBA::StaticRequest __req( this, "create_component_server" );
  __req.add_in_arg( &_sa_config );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_CreateFailure, "IDL:omg.org/Components/CreateFailure:1.0",
    _marshaller_Components_InvalidConfiguration, "IDL:omg.org/Components/InvalidConfiguration:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

Components::ComponentServer_ptr
Components::ServerActivator_stub_clp::create_component_server( const Components::ConfigValues& _par_config )
{
  return Components::ServerActivator_stub::create_component_server(_par_config);
}

#endif // MICO_CONF_NO_POA

void Components::ServerActivator_stub::remove_component_server( Components::ComponentServer_ptr _par_server )
{
  CORBA::StaticAny _sa_server( _marshaller_Components_ComponentServer, &_par_server );
  CORBA::StaticRequest __req( this, "remove_component_server" );
  __req.add_in_arg( &_sa_server );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_RemoveFailure, "IDL:omg.org/Components/RemoveFailure:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
Components::ServerActivator_stub_clp::remove_component_server( Components::ComponentServer_ptr _par_server )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ServerActivator * _myserv = POA_Components::ServerActivator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->remove_component_server(_par_server);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  Components::ServerActivator_stub::remove_component_server(_par_server);
}

#endif // MICO_CONF_NO_POA

Components::ComponentServers* Components::ServerActivator_stub::get_component_servers()
{
  CORBA::StaticAny __res( _marshaller__seq_Components_ComponentServer );

  CORBA::StaticRequest __req( this, "get_component_servers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (Components::ComponentServers*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

Components::ComponentServers*
Components::ServerActivator_stub_clp::get_component_servers()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Components::ServerActivator * _myserv = POA_Components::ServerActivator::_narrow (_serv);
    if (_myserv) {
      Components::ComponentServers* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_component_servers();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return Components::ServerActivator_stub::get_component_servers();
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class MicoCCMD
 */

MICOCCM::MicoCCMD::~MicoCCMD()
{
}

void *
MICOCCM::MicoCCMD::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:mico.org/MICOCCM/MicoCCMD:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::ServerActivator::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = Components::ComponentInstallation::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = Components::AssemblyFactory::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

MICOCCM::MicoCCMD_ptr
MICOCCM::MicoCCMD::_narrow( CORBA::Object_ptr _obj )
{
  MICOCCM::MicoCCMD_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:mico.org/MICOCCM/MicoCCMD:1.0" )))
      return _duplicate( (MICOCCM::MicoCCMD_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:mico.org/MICOCCM/MicoCCMD:1.0") || _obj->_is_a_remote ("IDL:mico.org/MICOCCM/MicoCCMD:1.0")) {
      _o = new MICOCCM::MicoCCMD_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

MICOCCM::MicoCCMD_ptr
MICOCCM::MicoCCMD::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace MICOCCM
{
CORBA::TypeCodeConst _tc_MicoCCMD;
}
class _Marshaller_MICOCCM_MicoCCMD : public ::CORBA::StaticTypeInfo {
    typedef MICOCCM::MicoCCMD_ptr _MICO_T;
  public:
    ~_Marshaller_MICOCCM_MicoCCMD();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_MICOCCM_MicoCCMD::~_Marshaller_MICOCCM_MicoCCMD()
{
}

::CORBA::StaticValueType _Marshaller_MICOCCM_MicoCCMD::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_MICOCCM_MicoCCMD::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::MICOCCM::MicoCCMD::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_MICOCCM_MicoCCMD::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_MICOCCM_MicoCCMD::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_MICOCCM_MicoCCMD::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::MICOCCM::MicoCCMD::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_MICOCCM_MicoCCMD::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_MICOCCM_MicoCCMD::typecode()
{
  return MICOCCM::_tc_MicoCCMD;
}

::CORBA::StaticTypeInfo *_marshaller_MICOCCM_MicoCCMD;

void
operator<<=( CORBA::Any &_a, const MICOCCM::MicoCCMD_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_MICOCCM_MicoCCMD, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, MICOCCM::MicoCCMD_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_MICOCCM_MicoCCMD, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, MICOCCM::MicoCCMD_ptr &_obj )
{
  MICOCCM::MicoCCMD_ptr *p;
  if (_a.to_static_any (_marshaller_MICOCCM_MicoCCMD, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class MicoCCMD
 */

MICOCCM::MicoCCMD_stub::~MicoCCMD_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_MICOCCM::MicoCCMD::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:mico.org/MICOCCM/MicoCCMD:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_Components::ServerActivator::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_Components::ComponentInstallation::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_Components::AssemblyFactory::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_MICOCCM::MicoCCMD *
POA_MICOCCM::MicoCCMD::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:mico.org/MICOCCM/MicoCCMD:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_MICOCCM::MicoCCMD *) p;
  }
  return NULL;
}

MICOCCM::MicoCCMD_stub_clp::MicoCCMD_stub_clp ()
{
}

MICOCCM::MicoCCMD_stub_clp::MicoCCMD_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

MICOCCM::MicoCCMD_stub_clp::~MicoCCMD_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Object_ptr MICOCCM::MicoCCMD_stub::exec( const char* _par_prog, const Components::NameList& _par_args, const char* _par_iorfile )
{
  CORBA::StaticAny _sa_prog( CORBA::_stc_string, &_par_prog );
  CORBA::StaticAny _sa_args( CORBA::_stcseq_string, &_par_args );
  CORBA::StaticAny _sa_iorfile( CORBA::_stc_string, &_par_iorfile );
  CORBA::Object_ptr _res = CORBA::Object::_nil();
  CORBA::StaticAny __res( CORBA::_stc_Object, &_res );

  CORBA::StaticRequest __req( this, "exec" );
  __req.add_in_arg( &_sa_prog );
  __req.add_in_arg( &_sa_args );
  __req.add_in_arg( &_sa_iorfile );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Components_CreateFailure, "IDL:omg.org/Components/CreateFailure:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Object_ptr
MICOCCM::MicoCCMD_stub_clp::exec( const char* _par_prog, const Components::NameList& _par_args, const char* _par_iorfile )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_MICOCCM::MicoCCMD * _myserv = POA_MICOCCM::MicoCCMD::_narrow (_serv);
    if (_myserv) {
      CORBA::Object_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->exec(_par_prog, _par_args, _par_iorfile);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return MICOCCM::MicoCCMD_stub::exec(_par_prog, _par_args, _par_iorfile);
}

#endif // MICO_CONF_NO_POA

void MICOCCM::MicoCCMD_stub::callback( const char* _par_token, CORBA::Object_ptr _par_csref )
{
  CORBA::StaticAny _sa_token( CORBA::_stc_string, &_par_token );
  CORBA::StaticAny _sa_csref( CORBA::_stc_Object, &_par_csref );
  CORBA::StaticRequest __req( this, "callback" );
  __req.add_in_arg( &_sa_token );
  __req.add_in_arg( &_sa_csref );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
MICOCCM::MicoCCMD_stub_clp::callback( const char* _par_token, CORBA::Object_ptr _par_csref )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_MICOCCM::MicoCCMD * _myserv = POA_MICOCCM::MicoCCMD::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->callback(_par_token, _par_csref);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  MICOCCM::MicoCCMD_stub::callback(_par_token, _par_csref);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ComponentServer
 */

MICOCCM::ComponentServer::~ComponentServer()
{
}

void *
MICOCCM::ComponentServer::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:mico.org/MICOCCM/ComponentServer:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = Components::ComponentServer::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

MICOCCM::ComponentServer_ptr
MICOCCM::ComponentServer::_narrow( CORBA::Object_ptr _obj )
{
  MICOCCM::ComponentServer_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:mico.org/MICOCCM/ComponentServer:1.0" )))
      return _duplicate( (MICOCCM::ComponentServer_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:mico.org/MICOCCM/ComponentServer:1.0") || _obj->_is_a_remote ("IDL:mico.org/MICOCCM/ComponentServer:1.0")) {
      _o = new MICOCCM::ComponentServer_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

MICOCCM::ComponentServer_ptr
MICOCCM::ComponentServer::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace MICOCCM
{
CORBA::TypeCodeConst _tc_ComponentServer;
}
class _Marshaller_MICOCCM_ComponentServer : public ::CORBA::StaticTypeInfo {
    typedef MICOCCM::ComponentServer_ptr _MICO_T;
  public:
    ~_Marshaller_MICOCCM_ComponentServer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_MICOCCM_ComponentServer::~_Marshaller_MICOCCM_ComponentServer()
{
}

::CORBA::StaticValueType _Marshaller_MICOCCM_ComponentServer::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_MICOCCM_ComponentServer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::MICOCCM::ComponentServer::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_MICOCCM_ComponentServer::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_MICOCCM_ComponentServer::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_MICOCCM_ComponentServer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::MICOCCM::ComponentServer::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_MICOCCM_ComponentServer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_MICOCCM_ComponentServer::typecode()
{
  return MICOCCM::_tc_ComponentServer;
}

::CORBA::StaticTypeInfo *_marshaller_MICOCCM_ComponentServer;

void
operator<<=( CORBA::Any &_a, const MICOCCM::ComponentServer_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_MICOCCM_ComponentServer, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, MICOCCM::ComponentServer_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_MICOCCM_ComponentServer, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, MICOCCM::ComponentServer_ptr &_obj )
{
  MICOCCM::ComponentServer_ptr *p;
  if (_a.to_static_any (_marshaller_MICOCCM_ComponentServer, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ComponentServer
 */

MICOCCM::ComponentServer_stub::~ComponentServer_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_MICOCCM::ComponentServer::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:mico.org/MICOCCM/ComponentServer:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_Components::ComponentServer::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_MICOCCM::ComponentServer *
POA_MICOCCM::ComponentServer::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:mico.org/MICOCCM/ComponentServer:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_MICOCCM::ComponentServer *) p;
  }
  return NULL;
}

MICOCCM::ComponentServer_stub_clp::ComponentServer_stub_clp ()
{
}

MICOCCM::ComponentServer_stub_clp::ComponentServer_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

MICOCCM::ComponentServer_stub_clp::~ComponentServer_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void MICOCCM::ComponentServer_stub::set_config_values( const Components::ConfigValues& _par_config )
{
  CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );
  CORBA::StaticRequest __req( this, "set_config_values" );
  __req.add_in_arg( &_sa_config );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
MICOCCM::ComponentServer_stub_clp::set_config_values( const Components::ConfigValues& _par_config )
{
  MICOCCM::ComponentServer_stub::set_config_values(_par_config);
}

#endif // MICO_CONF_NO_POA

class _Marshaller__seq_Components_FacetDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_FacetDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_FacetDescription::~_Marshaller__seq_Components_FacetDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_FacetDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_FacetDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_FacetDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_FacetDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_FacetDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_FacetDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_FacetDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_FacetDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000084010000010000001d00000072010000010000002c00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f4661636574"
    "4465736372697074696f6e3a312e30001100000046616365744465736372"
    "697074696f6e000000001d000000ca000000010000002b00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f506f727444657363726970"
    "74696f6e3a312e30000010000000506f72744465736372697074696f6e00"
    "000000000000000002000000050000006e616d6500000000150000004800"
    "0000010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f466561747572654e616d653a312e3000000c000000466561747572"
    "654e616d650012000000000000000100000008000000747970655f696400"
    "120000000000000001000000010000000a00000066616365745f72656600"
    "00000e00000033000000010000001d00000049444c3a6f6d672e6f72672f"
    "434f5242412f4f626a6563743a312e3000000000070000004f626a656374"
    "00000100000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_FacetDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_FacetDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_FacetDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_FacetDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_FacetDescription, (void *&)_s);
}


class _Marshaller__seq_Components_ConnectionDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_ConnectionDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_ConnectionDescription::~_Marshaller__seq_Components_ConnectionDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_ConnectionDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_ConnectionDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_ConnectionDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_ConnectionDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_ConnectionDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_ConnectionDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_ConnectionDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConnectionDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000038010000010000001d00000026010000010000003100"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e6e65"
    "6374696f6e4465736372697074696f6e3a312e300000000016000000436f"
    "6e6e656374696f6e4465736372697074696f6e0000000000000002000000"
    "03000000636b00001d0000006a000000010000002200000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f436f6f6b69653a312e30000000"
    "07000000436f6f6b696500000000000000000000010000000c000000436f"
    "6f6b696556616c756500130000000c000000010000000a00000000000000"
    "01000100070000006f626a72656600000e00000033000000010000001d00"
    "000049444c3a6f6d672e6f72672f434f5242412f4f626a6563743a312e30"
    "00000000070000004f626a65637400000100000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConnectionDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_ConnectionDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConnectionDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConnectionDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_ConnectionDescription, (void *&)_s);
}


class _Marshaller__seq_Components_ReceptacleDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_ReceptacleDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_ReceptacleDescription::~_Marshaller__seq_Components_ReceptacleDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_ReceptacleDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_ReceptacleDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_ReceptacleDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_ReceptacleDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_ReceptacleDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_ReceptacleDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_ReceptacleDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ReceptacleDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000010030000010000001d000000fe020000010000003100"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5265636570"
    "7461636c654465736372697074696f6e3a312e3000000000160000005265"
    "6365707461636c654465736372697074696f6e0000001d000000ca000000"
    "010000002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f506f72744465736372697074696f6e3a312e30000010000000506f7274"
    "4465736372697074696f6e00000000000000000002000000050000006e61"
    "6d65000000001500000048000000010000002700000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f466561747572654e616d653a312e30"
    "00000c000000466561747572654e616d6500120000000000000001000000"
    "08000000747970655f696400120000000000000001000000020000000d00"
    "000069735f6d756c7469706c65780000000008000000010000000c000000"
    "636f6e6e656374696f6e7300150000009801000001000000320000004944"
    "4c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e6e656374696f"
    "6e4465736372697074696f6e733a312e3000000017000000436f6e6e6563"
    "74696f6e4465736372697074696f6e730000130000003801000001000000"
    "1d00000026010000010000003100000049444c3a6f6d672e6f72672f436f"
    "6d706f6e656e74732f436f6e6e656374696f6e4465736372697074696f6e"
    "3a312e300000000016000000436f6e6e656374696f6e4465736372697074"
    "696f6e000000000000000200000003000000636b00001d0000006a000000"
    "010000002200000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f436f6f6b69653a312e3000000007000000436f6f6b6965000000000000"
    "00000000010000000c000000436f6f6b696556616c756500130000000c00"
    "0000010000000a0000000000000001000100070000006f626a7265660000"
    "0e00000033000000010000001d00000049444c3a6f6d672e6f72672f434f"
    "5242412f4f626a6563743a312e3000000000070000004f626a6563740000"
    "01000000000000000100000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ReceptacleDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_ReceptacleDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ReceptacleDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ReceptacleDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_ReceptacleDescription, (void *&)_s);
}


class _Marshaller__seq_Components_ConsumerDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_ConsumerDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_ConsumerDescription::~_Marshaller__seq_Components_ConsumerDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_ConsumerDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_ConsumerDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_ConsumerDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_ConsumerDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_ConsumerDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_ConsumerDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_ConsumerDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConsumerDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000a4010000010000001d00000094010000010000002f00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e7375"
    "6d65724465736372697074696f6e3a312e30000014000000436f6e73756d"
    "65724465736372697074696f6e00000000001d000000ca00000001000000"
    "2b00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f506f72"
    "744465736372697074696f6e3a312e30000010000000506f727444657363"
    "72697074696f6e00000000000000000002000000050000006e616d650000"
    "00001500000048000000010000002700000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f466561747572654e616d653a312e3000000c00"
    "0000466561747572654e616d650012000000000000000100000008000000"
    "747970655f6964001200000000000000010000000100000009000000636f"
    "6e73756d6572000000000e0000004e000000010000002d00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d"
    "6572426173653a312e3000000000120000004576656e74436f6e73756d65"
    "724261736500010000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConsumerDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_ConsumerDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConsumerDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConsumerDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_ConsumerDescription, (void *&)_s);
}


class _Marshaller__seq_Components_EmitterDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_EmitterDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_EmitterDescription::~_Marshaller__seq_Components_EmitterDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_EmitterDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_EmitterDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_EmitterDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_EmitterDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_EmitterDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_EmitterDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_EmitterDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_EmitterDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000a4010000010000001d00000094010000010000002e00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f456d697474"
    "65724465736372697074696f6e3a312e3000000013000000456d69747465"
    "724465736372697074696f6e0000000000001d000000ca00000001000000"
    "2b00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f506f72"
    "744465736372697074696f6e3a312e30000010000000506f727444657363"
    "72697074696f6e00000000000000000002000000050000006e616d650000"
    "00001500000048000000010000002700000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f466561747572654e616d653a312e3000000c00"
    "0000466561747572654e616d650012000000000000000100000008000000"
    "747970655f6964001200000000000000010000000100000009000000636f"
    "6e73756d6572000000000e0000004e000000010000002d00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d"
    "6572426173653a312e3000000000120000004576656e74436f6e73756d65"
    "724261736500010000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_EmitterDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_EmitterDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_EmitterDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_EmitterDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_EmitterDescription, (void *&)_s);
}


class _Marshaller__seq_Components_SubscriberDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_SubscriberDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_SubscriberDescription::~_Marshaller__seq_Components_SubscriberDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_SubscriberDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_SubscriberDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_SubscriberDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_SubscriberDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_SubscriberDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_SubscriberDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_SubscriberDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_SubscriberDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000054010000010000001d00000044010000010000003100"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5375627363"
    "72696265724465736372697074696f6e3a312e3000000000160000005375"
    "62736372696265724465736372697074696f6e0000000000000002000000"
    "03000000636b00001d0000006a000000010000002200000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f436f6f6b69653a312e30000000"
    "07000000436f6f6b696500000000000000000000010000000c000000436f"
    "6f6b696556616c756500130000000c000000010000000a00000000000000"
    "0100010009000000636f6e73756d6572000000000e0000004e0000000100"
    "00002d00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f45"
    "76656e74436f6e73756d6572426173653a312e3000000000120000004576"
    "656e74436f6e73756d65724261736500010000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_SubscriberDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_SubscriberDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_SubscriberDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_SubscriberDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_SubscriberDescription, (void *&)_s);
}


class _Marshaller__seq_Components_PublisherDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_PublisherDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_PublisherDescription::~_Marshaller__seq_Components_PublisherDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_PublisherDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_PublisherDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_PublisherDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_PublisherDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_PublisherDescription->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_PublisherDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_PublisherDescription->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_PublisherDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000130000000c030000010000001d000000fa020000010000003000"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5075626c69"
    "736865724465736372697074696f6e3a312e3000150000005075626c6973"
    "6865724465736372697074696f6e000000001d000000ca00000001000000"
    "2b00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f506f72"
    "744465736372697074696f6e3a312e30000010000000506f727444657363"
    "72697074696f6e00000000000000000002000000050000006e616d650000"
    "00001500000048000000010000002700000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f466561747572654e616d653a312e3000000c00"
    "0000466561747572654e616d650012000000000000000100000008000000"
    "747970655f696400120000000000000001000000010000000a000000636f"
    "6e73756d65727300000015000000b4010000010000003200000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f5375627363726962657244"
    "65736372697074696f6e733a312e30000000170000005375627363726962"
    "65724465736372697074696f6e7300001300000054010000010000001d00"
    "000044010000010000003100000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f537562736372696265724465736372697074696f6e3a31"
    "2e300000000016000000537562736372696265724465736372697074696f"
    "6e000000000000000200000003000000636b00001d0000006a0000000100"
    "00002200000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f43"
    "6f6f6b69653a312e3000000007000000436f6f6b69650000000000000000"
    "0000010000000c000000436f6f6b696556616c756500130000000c000000"
    "010000000a000000000000000100010009000000636f6e73756d65720000"
    "00000e0000004e000000010000002d00000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f4576656e74436f6e73756d6572426173653a31"
    "2e3000000000120000004576656e74436f6e73756d657242617365000100"
    "000000000100000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_PublisherDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_PublisherDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_PublisherDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_PublisherDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_PublisherDescription, (void *&)_s);
}


class _Marshaller__seq_Components_CCMHome : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_CCMHome();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_CCMHome::~_Marshaller__seq_Components_CCMHome()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_CCMHome::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_CCMHome::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_CCMHome::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_CCMHome::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_CCMHome->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_CCMHome::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_CCMHome->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_CCMHome::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000048000000010000000e00000038000000010000002300"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f43434d486f"
    "6d653a312e3000000800000043434d486f6d650000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_CCMHome::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_CCMHome;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_CCMHome, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_CCMHome, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_CCMHome, (void *&)_s);
}


class _Marshaller__seq_Components_ConfigValue : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_ConfigValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_ConfigValue::~_Marshaller__seq_Components_ConfigValue()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_ConfigValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_ConfigValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_ConfigValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_ConfigValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_ConfigValue->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_ConfigValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_ConfigValue->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConfigValue::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000d0000000010000001d000000be000000010000002700"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e6669"
    "6756616c75653a312e3000000c000000436f6e66696756616c7565000000"
    "00000000000002000000050000006e616d65000000001500000048000000"
    "010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f466561747572654e616d653a312e3000000c000000466561747572654e"
    "616d65001200000000000000010000000600000076616c75650000000b00"
    "00000100000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ConfigValue::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_ConfigValue;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConfigValue, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ConfigValue, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_ConfigValue, (void *&)_s);
}


class _Marshaller__seq_Components_Container : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_Container();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_Container::~_Marshaller__seq_Components_Container()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_Container::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_Container::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_Container::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_Container::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_Container->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_Container::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_Container->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_Container::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000050000000010000000e0000003e000000010000002500"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e7461"
    "696e65723a312e30000000000a000000436f6e7461696e65720000000000"
    "0000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_Container::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_Container;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_Container, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_Container, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_Container, (void *&)_s);
}


class _Marshaller__seq_Components_ComponentServer : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Components_ComponentServer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Components_ComponentServer::~_Marshaller__seq_Components_ComponentServer()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Components_ComponentServer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Components_ComponentServer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Components_ComponentServer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Components_ComponentServer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Components_ComponentServer->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Components_ComponentServer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Components_ComponentServer->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ComponentServer::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000058000000010000000e00000048000000010000002b00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6d706f"
    "6e656e745365727665723a312e30000010000000436f6d706f6e656e7453"
    "65727665720000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Components_ComponentServer::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Components_ComponentServer;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ComponentServer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Components_ComponentServer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Components_ComponentServer, (void *&)_s);
}


struct __tc_init_CCM {
  __tc_init_CCM()
  {
    Components::_tc_FeatureName = 
    "010000001500000048000000010000002700000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f466561747572654e616d653a312e300000"
    "0c000000466561747572654e616d65001200000000000000";
    Components::_tc_NameList = 
    "01000000150000009c000000010000002400000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4e616d654c6973743a312e300009000000"
    "4e616d654c69737400000000130000005800000001000000150000004800"
    "0000010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f466561747572654e616d653a312e3000000c000000466561747572"
    "654e616d6500120000000000000000000000";
    Components::_tc_InvalidName = 
    "010000001600000044000000010000002700000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c69644e616d653a312e300000"
    "0c000000496e76616c69644e616d650000000000";
    _marshaller_Components_InvalidName = new _Marshaller_Components_InvalidName;
    Components::_tc_InvalidConfiguration = 
    "010000001600000058000000010000003000000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c6964436f6e66696775726174"
    "696f6e3a312e300015000000496e76616c6964436f6e6669677572617469"
    "6f6e0000000000000000";
    _marshaller_Components_InvalidConfiguration = new _Marshaller_Components_InvalidConfiguration;
    Components::_tc_AlreadyConnected = 
    "010000001600000050000000010000002c00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f416c7265616479436f6e6e65637465643a"
    "312e300011000000416c7265616479436f6e6e6563746564000000000000"
    "0000";
    _marshaller_Components_AlreadyConnected = new _Marshaller_Components_AlreadyConnected;
    Components::_tc_InvalidConnection = 
    "010000001600000054000000010000002d00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c6964436f6e6e656374696f6e"
    "3a312e300000000012000000496e76616c6964436f6e6e656374696f6e00"
    "000000000000";
    _marshaller_Components_InvalidConnection = new _Marshaller_Components_InvalidConnection;
    Components::_tc_NoConnection = 
    "010000001600000048000000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4e6f436f6e6e656374696f6e3a312e3000"
    "0d0000004e6f436f6e6e656374696f6e0000000000000000";
    _marshaller_Components_NoConnection = new _Marshaller_Components_NoConnection;
    Components::_tc_ExceededConnectionLimit = 
    "01000000160000005c000000010000003300000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4578636565646564436f6e6e656374696f"
    "6e4c696d69743a312e300000180000004578636565646564436f6e6e6563"
    "74696f6e4c696d69740000000000";
    _marshaller_Components_ExceededConnectionLimit = new _Marshaller_Components_ExceededConnectionLimit;
    Components::_tc_CookieRequired = 
    "01000000160000004c000000010000002a00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6f6b696552657175697265643a312e"
    "300000000f000000436f6f6b69655265717569726564000000000000";
    _marshaller_Components_CookieRequired = new _Marshaller_Components_CookieRequired;
    Components::_tc_PortDescription = 
    "010000001d000000ca000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f506f72744465736372697074696f6e3a31"
    "2e30000010000000506f72744465736372697074696f6e00000000000000"
    "000002000000050000006e616d6500000000150000004800000001000000"
    "2700000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f466561"
    "747572654e616d653a312e3000000c000000466561747572654e616d6500"
    "12000000000000000100000008000000747970655f696400120000000000"
    "00000100";
    _marshaller_Components_PortDescription = new _Marshaller_Components_PortDescription;
    Components::_tc_Cookie = 
    "010000001d0000006a000000010000002200000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6f6b69653a312e3000000007000000"
    "436f6f6b696500000000000000000000010000000c000000436f6f6b6965"
    "56616c756500130000000c000000010000000a000000000000000100";
    _marshaller_Components_Cookie = new _Marshaller_Components_Cookie;
    Components::_tc_EventBase = 
    "010000001d00000048000000010000002500000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4576656e74426173653a312e3000000000"
    "0a0000004576656e74426173650002000000000000000000";
    _marshaller_Components_EventBase = new _Marshaller_Components_EventBase;
    Components::_tc_FacetDescription = 
    "010000001d00000072010000010000002c00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f46616365744465736372697074696f6e3a"
    "312e30001100000046616365744465736372697074696f6e000000001d00"
    "0000ca000000010000002b00000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f506f72744465736372697074696f6e3a312e3000001000"
    "0000506f72744465736372697074696f6e00000000000000000002000000"
    "050000006e616d6500000000150000004800000001000000270000004944"
    "4c3a6f6d672e6f72672f436f6d706f6e656e74732f466561747572654e61"
    "6d653a312e3000000c000000466561747572654e616d6500120000000000"
    "00000100000008000000747970655f696400120000000000000001000000"
    "010000000a00000066616365745f7265660000000e000000330000000100"
    "00001d00000049444c3a6f6d672e6f72672f434f5242412f4f626a656374"
    "3a312e3000000000070000004f626a65637400000100";
    _marshaller_Components_FacetDescription = new _Marshaller_Components_FacetDescription;
    Components::_tc_FacetDescriptions = 
    "0100000015000000dc010000010000002d00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f46616365744465736372697074696f6e73"
    "3a312e30000000001200000046616365744465736372697074696f6e7300"
    "00001300000084010000010000001d00000072010000010000002c000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f46616365744465"
    "736372697074696f6e3a312e300011000000466163657444657363726970"
    "74696f6e000000001d000000ca000000010000002b00000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f506f7274446573637269707469"
    "6f6e3a312e30000010000000506f72744465736372697074696f6e000000"
    "00000000000002000000050000006e616d65000000001500000048000000"
    "010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f466561747572654e616d653a312e3000000c000000466561747572654e"
    "616d650012000000000000000100000008000000747970655f6964001200"
    "00000000000001000000010000000a00000066616365745f726566000000"
    "0e00000033000000010000001d00000049444c3a6f6d672e6f72672f434f"
    "5242412f4f626a6563743a312e3000000000070000004f626a6563740000"
    "0100000000000000";
    Components::_tc_Navigation = 
    "010000000e0000003f000000010000002600000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4e617669676174696f6e3a312e30000000"
    "0b0000004e617669676174696f6e00";
    _marshaller_Components_Navigation = new _Marshaller_Components_Navigation;
    Components::_tc_ConnectionDescription = 
    "010000001d00000026010000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e6e656374696f6e44657363726970"
    "74696f6e3a312e300000000016000000436f6e6e656374696f6e44657363"
    "72697074696f6e000000000000000200000003000000636b00001d000000"
    "6a000000010000002200000049444c3a6f6d672e6f72672f436f6d706f6e"
    "656e74732f436f6f6b69653a312e3000000007000000436f6f6b69650000"
    "0000000000000000010000000c000000436f6f6b696556616c7565001300"
    "00000c000000010000000a0000000000000001000100070000006f626a72"
    "656600000e00000033000000010000001d00000049444c3a6f6d672e6f72"
    "672f434f5242412f4f626a6563743a312e3000000000070000004f626a65"
    "637400000100";
    _marshaller_Components_ConnectionDescription = new _Marshaller_Components_ConnectionDescription;
    Components::_tc_ConnectionDescriptions = 
    "010000001500000098010000010000003200000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e6e656374696f6e44657363726970"
    "74696f6e733a312e3000000017000000436f6e6e656374696f6e44657363"
    "72697074696f6e7300001300000038010000010000001d00000026010000"
    "010000003100000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f436f6e6e656374696f6e4465736372697074696f6e3a312e3000000000"
    "16000000436f6e6e656374696f6e4465736372697074696f6e0000000000"
    "00000200000003000000636b00001d0000006a0000000100000022000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6f6b69653a"
    "312e3000000007000000436f6f6b69650000000000000000000001000000"
    "0c000000436f6f6b696556616c756500130000000c000000010000000a00"
    "00000000000001000100070000006f626a72656600000e00000033000000"
    "010000001d00000049444c3a6f6d672e6f72672f434f5242412f4f626a65"
    "63743a312e3000000000070000004f626a65637400000100000000000000"
    ;
    Components::_tc_ReceptacleDescription = 
    "010000001d000000fe020000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f52656365707461636c6544657363726970"
    "74696f6e3a312e30000000001600000052656365707461636c6544657363"
    "72697074696f6e0000001d000000ca000000010000002b00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f506f727444657363726970"
    "74696f6e3a312e30000010000000506f72744465736372697074696f6e00"
    "000000000000000002000000050000006e616d6500000000150000004800"
    "0000010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f466561747572654e616d653a312e3000000c000000466561747572"
    "654e616d650012000000000000000100000008000000747970655f696400"
    "120000000000000001000000020000000d00000069735f6d756c7469706c"
    "65780000000008000000010000000c000000636f6e6e656374696f6e7300"
    "1500000098010000010000003200000049444c3a6f6d672e6f72672f436f"
    "6d706f6e656e74732f436f6e6e656374696f6e4465736372697074696f6e"
    "733a312e3000000017000000436f6e6e656374696f6e4465736372697074"
    "696f6e7300001300000038010000010000001d0000002601000001000000"
    "3100000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e"
    "6e656374696f6e4465736372697074696f6e3a312e300000000016000000"
    "436f6e6e656374696f6e4465736372697074696f6e000000000000000200"
    "000003000000636b00001d0000006a000000010000002200000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f436f6f6b69653a312e3000"
    "000007000000436f6f6b696500000000000000000000010000000c000000"
    "436f6f6b696556616c756500130000000c000000010000000a0000000000"
    "000001000100070000006f626a72656600000e0000003300000001000000"
    "1d00000049444c3a6f6d672e6f72672f434f5242412f4f626a6563743a31"
    "2e3000000000070000004f626a656374000001000000000000000100";
    _marshaller_Components_ReceptacleDescription = new _Marshaller_Components_ReceptacleDescription;
    Components::_tc_ReceptacleDescriptions = 
    "010000001500000070030000010000003200000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f52656365707461636c6544657363726970"
    "74696f6e733a312e300000001700000052656365707461636c6544657363"
    "72697074696f6e7300001300000010030000010000001d000000fe020000"
    "010000003100000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f52656365707461636c654465736372697074696f6e3a312e3000000000"
    "1600000052656365707461636c654465736372697074696f6e0000001d00"
    "0000ca000000010000002b00000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f506f72744465736372697074696f6e3a312e3000001000"
    "0000506f72744465736372697074696f6e00000000000000000002000000"
    "050000006e616d6500000000150000004800000001000000270000004944"
    "4c3a6f6d672e6f72672f436f6d706f6e656e74732f466561747572654e61"
    "6d653a312e3000000c000000466561747572654e616d6500120000000000"
    "00000100000008000000747970655f696400120000000000000001000000"
    "020000000d00000069735f6d756c7469706c657800000000080000000100"
    "00000c000000636f6e6e656374696f6e7300150000009801000001000000"
    "3200000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e"
    "6e656374696f6e4465736372697074696f6e733a312e3000000017000000"
    "436f6e6e656374696f6e4465736372697074696f6e730000130000003801"
    "0000010000001d00000026010000010000003100000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f436f6e6e656374696f6e4465736372"
    "697074696f6e3a312e300000000016000000436f6e6e656374696f6e4465"
    "736372697074696f6e000000000000000200000003000000636b00001d00"
    "00006a000000010000002200000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f436f6f6b69653a312e3000000007000000436f6f6b6965"
    "00000000000000000000010000000c000000436f6f6b696556616c756500"
    "130000000c000000010000000a0000000000000001000100070000006f62"
    "6a72656600000e00000033000000010000001d00000049444c3a6f6d672e"
    "6f72672f434f5242412f4f626a6563743a312e3000000000070000004f62"
    "6a656374000001000000000000000100000000000000";
    Components::_tc_Receptacles = 
    "010000000e00000040000000010000002700000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f52656365707461636c65733a312e300000"
    "0c00000052656365707461636c657300";
    _marshaller_Components_Receptacles = new _Marshaller_Components_Receptacles;
    Components::_tc_BadEventType = 
    "010000001600000068000000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4261644576656e74547970653a312e3000"
    "0d0000004261644576656e74547970650000000001000000140000006578"
    "7065637465645f6576656e745f74797065001200000000000000";
    _marshaller_Components_BadEventType = new _Marshaller_Components_BadEventType;
    Components::_tc_EventConsumerBase = 
    "010000000e0000004e000000010000002d00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4576656e74436f6e73756d657242617365"
    "3a312e3000000000120000004576656e74436f6e73756d65724261736500"
    ;
    _marshaller_Components_EventConsumerBase = new _Marshaller_Components_EventConsumerBase;
    Components::_tc_ConsumerDescription = 
    "010000001d00000094010000010000002f00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e73756d6572446573637269707469"
    "6f6e3a312e30000014000000436f6e73756d65724465736372697074696f"
    "6e00000000001d000000ca000000010000002b00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f506f72744465736372697074696f6e"
    "3a312e30000010000000506f72744465736372697074696f6e0000000000"
    "0000000002000000050000006e616d650000000015000000480000000100"
    "00002700000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f46"
    "6561747572654e616d653a312e3000000c000000466561747572654e616d"
    "650012000000000000000100000008000000747970655f69640012000000"
    "00000000010000000100000009000000636f6e73756d6572000000000e00"
    "00004e000000010000002d00000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f4576656e74436f6e73756d6572426173653a312e300000"
    "0000120000004576656e74436f6e73756d657242617365000100";
    _marshaller_Components_ConsumerDescription = new _Marshaller_Components_ConsumerDescription;
    Components::_tc_ConsumerDescriptions = 
    "010000001500000000020000010000003000000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e73756d6572446573637269707469"
    "6f6e733a312e300015000000436f6e73756d65724465736372697074696f"
    "6e730000000013000000a4010000010000001d0000009401000001000000"
    "2f00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e"
    "73756d65724465736372697074696f6e3a312e30000014000000436f6e73"
    "756d65724465736372697074696f6e00000000001d000000ca0000000100"
    "00002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f50"
    "6f72744465736372697074696f6e3a312e30000010000000506f72744465"
    "736372697074696f6e00000000000000000002000000050000006e616d65"
    "000000001500000048000000010000002700000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f466561747572654e616d653a312e300000"
    "0c000000466561747572654e616d65001200000000000000010000000800"
    "0000747970655f6964001200000000000000010000000100000009000000"
    "636f6e73756d6572000000000e0000004e000000010000002d0000004944"
    "4c3a6f6d672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73"
    "756d6572426173653a312e3000000000120000004576656e74436f6e7375"
    "6d65724261736500010000000000";
    Components::_tc_EmitterDescription = 
    "010000001d00000094010000010000002e00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f456d69747465724465736372697074696f"
    "6e3a312e3000000013000000456d69747465724465736372697074696f6e"
    "0000000000001d000000ca000000010000002b00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f506f72744465736372697074696f6e"
    "3a312e30000010000000506f72744465736372697074696f6e0000000000"
    "0000000002000000050000006e616d650000000015000000480000000100"
    "00002700000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f46"
    "6561747572654e616d653a312e3000000c000000466561747572654e616d"
    "650012000000000000000100000008000000747970655f69640012000000"
    "00000000010000000100000009000000636f6e73756d6572000000000e00"
    "00004e000000010000002d00000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f4576656e74436f6e73756d6572426173653a312e300000"
    "0000120000004576656e74436f6e73756d657242617365000100";
    _marshaller_Components_EmitterDescription = new _Marshaller_Components_EmitterDescription;
    Components::_tc_EmitterDescriptions = 
    "0100000015000000fc010000010000002f00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f456d69747465724465736372697074696f"
    "6e733a312e30000014000000456d69747465724465736372697074696f6e"
    "730013000000a4010000010000001d00000094010000010000002e000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f456d6974746572"
    "4465736372697074696f6e3a312e3000000013000000456d697474657244"
    "65736372697074696f6e0000000000001d000000ca000000010000002b00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f506f727444"
    "65736372697074696f6e3a312e30000010000000506f7274446573637269"
    "7074696f6e00000000000000000002000000050000006e616d6500000000"
    "1500000048000000010000002700000049444c3a6f6d672e6f72672f436f"
    "6d706f6e656e74732f466561747572654e616d653a312e3000000c000000"
    "466561747572654e616d6500120000000000000001000000080000007479"
    "70655f6964001200000000000000010000000100000009000000636f6e73"
    "756d6572000000000e0000004e000000010000002d00000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d6572"
    "426173653a312e3000000000120000004576656e74436f6e73756d657242"
    "61736500010000000000";
    Components::_tc_SubscriberDescription = 
    "010000001d00000044010000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5375627363726962657244657363726970"
    "74696f6e3a312e3000000000160000005375627363726962657244657363"
    "72697074696f6e000000000000000200000003000000636b00001d000000"
    "6a000000010000002200000049444c3a6f6d672e6f72672f436f6d706f6e"
    "656e74732f436f6f6b69653a312e3000000007000000436f6f6b69650000"
    "0000000000000000010000000c000000436f6f6b696556616c7565001300"
    "00000c000000010000000a000000000000000100010009000000636f6e73"
    "756d6572000000000e0000004e000000010000002d00000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d6572"
    "426173653a312e3000000000120000004576656e74436f6e73756d657242"
    "617365000100";
    _marshaller_Components_SubscriberDescription = new _Marshaller_Components_SubscriberDescription;
    Components::_tc_SubscriberDescriptions = 
    "0100000015000000b4010000010000003200000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5375627363726962657244657363726970"
    "74696f6e733a312e30000000170000005375627363726962657244657363"
    "72697074696f6e7300001300000054010000010000001d00000044010000"
    "010000003100000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f537562736372696265724465736372697074696f6e3a312e3000000000"
    "16000000537562736372696265724465736372697074696f6e0000000000"
    "00000200000003000000636b00001d0000006a0000000100000022000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6f6b69653a"
    "312e3000000007000000436f6f6b69650000000000000000000001000000"
    "0c000000436f6f6b696556616c756500130000000c000000010000000a00"
    "0000000000000100010009000000636f6e73756d6572000000000e000000"
    "4e000000010000002d00000049444c3a6f6d672e6f72672f436f6d706f6e"
    "656e74732f4576656e74436f6e73756d6572426173653a312e3000000000"
    "120000004576656e74436f6e73756d65724261736500010000000000";
    Components::_tc_PublisherDescription = 
    "010000001d000000fa020000010000003000000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5075626c69736865724465736372697074"
    "696f6e3a312e3000150000005075626c6973686572446573637269707469"
    "6f6e000000001d000000ca000000010000002b00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f506f72744465736372697074696f6e"
    "3a312e30000010000000506f72744465736372697074696f6e0000000000"
    "0000000002000000050000006e616d650000000015000000480000000100"
    "00002700000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f46"
    "6561747572654e616d653a312e3000000c000000466561747572654e616d"
    "650012000000000000000100000008000000747970655f69640012000000"
    "0000000001000000010000000a000000636f6e73756d6572730000001500"
    "0000b4010000010000003200000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f537562736372696265724465736372697074696f6e733a"
    "312e3000000017000000537562736372696265724465736372697074696f"
    "6e7300001300000054010000010000001d00000044010000010000003100"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5375627363"
    "72696265724465736372697074696f6e3a312e3000000000160000005375"
    "62736372696265724465736372697074696f6e0000000000000002000000"
    "03000000636b00001d0000006a000000010000002200000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f436f6f6b69653a312e30000000"
    "07000000436f6f6b696500000000000000000000010000000c000000436f"
    "6f6b696556616c756500130000000c000000010000000a00000000000000"
    "0100010009000000636f6e73756d6572000000000e0000004e0000000100"
    "00002d00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f45"
    "76656e74436f6e73756d6572426173653a312e3000000000120000004576"
    "656e74436f6e73756d657242617365000100000000000100";
    _marshaller_Components_PublisherDescription = new _Marshaller_Components_PublisherDescription;
    Components::_tc_PublisherDescriptions = 
    "01000000150000006c030000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5075626c69736865724465736372697074"
    "696f6e733a312e3000000000160000005075626c69736865724465736372"
    "697074696f6e73000000130000000c030000010000001d000000fa020000"
    "010000003000000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f5075626c69736865724465736372697074696f6e3a312e300015000000"
    "5075626c69736865724465736372697074696f6e000000001d000000ca00"
    "0000010000002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f506f72744465736372697074696f6e3a312e30000010000000506f"
    "72744465736372697074696f6e0000000000000000000200000005000000"
    "6e616d65000000001500000048000000010000002700000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f466561747572654e616d653a31"
    "2e3000000c000000466561747572654e616d650012000000000000000100"
    "000008000000747970655f69640012000000000000000100000001000000"
    "0a000000636f6e73756d65727300000015000000b4010000010000003200"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5375627363"
    "72696265724465736372697074696f6e733a312e30000000170000005375"
    "62736372696265724465736372697074696f6e7300001300000054010000"
    "010000001d00000044010000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5375627363726962657244657363726970"
    "74696f6e3a312e3000000000160000005375627363726962657244657363"
    "72697074696f6e000000000000000200000003000000636b00001d000000"
    "6a000000010000002200000049444c3a6f6d672e6f72672f436f6d706f6e"
    "656e74732f436f6f6b69653a312e3000000007000000436f6f6b69650000"
    "0000000000000000010000000c000000436f6f6b696556616c7565001300"
    "00000c000000010000000a000000000000000100010009000000636f6e73"
    "756d6572000000000e0000004e000000010000002d00000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d6572"
    "426173653a312e3000000000120000004576656e74436f6e73756d657242"
    "617365000100000000000100000000000000";
    Components::_tc_Events = 
    "010000000e00000037000000010000002200000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4576656e74733a312e3000000007000000"
    "4576656e747300";
    _marshaller_Components_Events = new _Marshaller_Components_Events;
    Components::_tc_CCMHome = 
    "010000000e00000038000000010000002300000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d486f6d653a312e30000008000000"
    "43434d486f6d6500";
    _marshaller_Components_CCMHome = new _Marshaller_Components_CCMHome;
    Components::_tc_ComponentPortDescription = 
    "010000001d0000009e0d0000010000003400000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6d706f6e656e74506f727444657363"
    "72697074696f6e3a312e300019000000436f6d706f6e656e74506f727444"
    "65736372697074696f6e0000000000000000050000000700000066616365"
    "7473000015000000dc010000010000002d00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f46616365744465736372697074696f6e73"
    "3a312e30000000001200000046616365744465736372697074696f6e7300"
    "00001300000084010000010000001d00000072010000010000002c000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f46616365744465"
    "736372697074696f6e3a312e300011000000466163657444657363726970"
    "74696f6e000000001d000000ca000000010000002b00000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f506f7274446573637269707469"
    "6f6e3a312e30000010000000506f72744465736372697074696f6e000000"
    "00000000000002000000050000006e616d65000000001500000048000000"
    "010000002700000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f466561747572654e616d653a312e3000000c000000466561747572654e"
    "616d650012000000000000000100000008000000747970655f6964001200"
    "00000000000001000000010000000a00000066616365745f726566000000"
    "0e00000033000000010000001d00000049444c3a6f6d672e6f72672f434f"
    "5242412f4f626a6563743a312e3000000000070000004f626a6563740000"
    "0100000000000000010000000c00000072656365707461636c6573001500"
    "000070030000010000003200000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f52656365707461636c654465736372697074696f6e733a"
    "312e300000001700000052656365707461636c654465736372697074696f"
    "6e7300001300000010030000010000001d000000fe020000010000003100"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f5265636570"
    "7461636c654465736372697074696f6e3a312e3000000000160000005265"
    "6365707461636c654465736372697074696f6e0000001d000000ca000000"
    "010000002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f506f72744465736372697074696f6e3a312e30000010000000506f7274"
    "4465736372697074696f6e00000000000000000002000000050000006e61"
    "6d65000000001500000048000000010000002700000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f466561747572654e616d653a312e30"
    "00000c000000466561747572654e616d6500120000000000000001000000"
    "08000000747970655f696400120000000000000001000000020000000d00"
    "000069735f6d756c7469706c65780000000008000000010000000c000000"
    "636f6e6e656374696f6e7300150000009801000001000000320000004944"
    "4c3a6f6d672e6f72672f436f6d706f6e656e74732f436f6e6e656374696f"
    "6e4465736372697074696f6e733a312e3000000017000000436f6e6e6563"
    "74696f6e4465736372697074696f6e730000130000003801000001000000"
    "1d00000026010000010000003100000049444c3a6f6d672e6f72672f436f"
    "6d706f6e656e74732f436f6e6e656374696f6e4465736372697074696f6e"
    "3a312e300000000016000000436f6e6e656374696f6e4465736372697074"
    "696f6e000000000000000200000003000000636b00001d0000006a000000"
    "010000002200000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f436f6f6b69653a312e3000000007000000436f6f6b6965000000000000"
    "00000000010000000c000000436f6f6b696556616c756500130000000c00"
    "0000010000000a0000000000000001000100070000006f626a7265660000"
    "0e00000033000000010000001d00000049444c3a6f6d672e6f72672f434f"
    "5242412f4f626a6563743a312e3000000000070000004f626a6563740000"
    "01000000000000000100000000000000010000000a000000636f6e73756d"
    "6572730000001500000000020000010000003000000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f436f6e73756d657244657363726970"
    "74696f6e733a312e300015000000436f6e73756d65724465736372697074"
    "696f6e730000000013000000a4010000010000001d000000940100000100"
    "00002f00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f43"
    "6f6e73756d65724465736372697074696f6e3a312e30000014000000436f"
    "6e73756d65724465736372697074696f6e00000000001d000000ca000000"
    "010000002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f506f72744465736372697074696f6e3a312e30000010000000506f7274"
    "4465736372697074696f6e00000000000000000002000000050000006e61"
    "6d65000000001500000048000000010000002700000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f466561747572654e616d653a312e30"
    "00000c000000466561747572654e616d6500120000000000000001000000"
    "08000000747970655f696400120000000000000001000000010000000900"
    "0000636f6e73756d6572000000000e0000004e000000010000002d000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f4576656e74436f"
    "6e73756d6572426173653a312e3000000000120000004576656e74436f6e"
    "73756d657242617365000100000000000100000009000000656d69747465"
    "72730000000015000000fc010000010000002f00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f456d69747465724465736372697074"
    "696f6e733a312e30000014000000456d6974746572446573637269707469"
    "6f6e730013000000a4010000010000001d00000094010000010000002e00"
    "000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f456d697474"
    "65724465736372697074696f6e3a312e3000000013000000456d69747465"
    "724465736372697074696f6e0000000000001d000000ca00000001000000"
    "2b00000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f506f72"
    "744465736372697074696f6e3a312e30000010000000506f727444657363"
    "72697074696f6e00000000000000000002000000050000006e616d650000"
    "00001500000048000000010000002700000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f466561747572654e616d653a312e3000000c00"
    "0000466561747572654e616d650012000000000000000100000008000000"
    "747970655f6964001200000000000000010000000100000009000000636f"
    "6e73756d6572000000000e0000004e000000010000002d00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d"
    "6572426173653a312e3000000000120000004576656e74436f6e73756d65"
    "724261736500010000000000010000000b0000007075626c697368657273"
    "0000150000006c030000010000003100000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f5075626c69736865724465736372697074696f"
    "6e733a312e3000000000160000005075626c697368657244657363726970"
    "74696f6e73000000130000000c030000010000001d000000fa0200000100"
    "00003000000049444c3a6f6d672e6f72672f436f6d706f6e656e74732f50"
    "75626c69736865724465736372697074696f6e3a312e3000150000005075"
    "626c69736865724465736372697074696f6e000000001d000000ca000000"
    "010000002b00000049444c3a6f6d672e6f72672f436f6d706f6e656e7473"
    "2f506f72744465736372697074696f6e3a312e30000010000000506f7274"
    "4465736372697074696f6e00000000000000000002000000050000006e61"
    "6d65000000001500000048000000010000002700000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f466561747572654e616d653a312e30"
    "00000c000000466561747572654e616d6500120000000000000001000000"
    "08000000747970655f696400120000000000000001000000010000000a00"
    "0000636f6e73756d65727300000015000000b40100000100000032000000"
    "49444c3a6f6d672e6f72672f436f6d706f6e656e74732f53756273637269"
    "6265724465736372697074696f6e733a312e300000001700000053756273"
    "6372696265724465736372697074696f6e73000013000000540100000100"
    "00001d00000044010000010000003100000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f53756273637269626572446573637269707469"
    "6f6e3a312e30000000001600000053756273637269626572446573637269"
    "7074696f6e000000000000000200000003000000636b00001d0000006a00"
    "0000010000002200000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f436f6f6b69653a312e3000000007000000436f6f6b696500000000"
    "000000000000010000000c000000436f6f6b696556616c75650013000000"
    "0c000000010000000a000000000000000100010009000000636f6e73756d"
    "6572000000000e0000004e000000010000002d00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f4576656e74436f6e73756d65724261"
    "73653a312e3000000000120000004576656e74436f6e73756d6572426173"
    "650001000000000001000000000000000100";
    _marshaller_Components_ComponentPortDescription = new _Marshaller_Components_ComponentPortDescription;
    Components::_tc_CCMObject = 
    "010000000e0000003e000000010000002500000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d4f626a6563743a312e3000000000"
    "0a00000043434d4f626a65637400";
    _marshaller_Components_CCMObject = new _Marshaller_Components_CCMObject;
    Components::_tc_DuplicateKeyValue = 
    "010000001600000054000000010000002d00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4475706c69636174654b657956616c7565"
    "3a312e3000000000120000004475706c69636174654b657956616c756500"
    "000000000000";
    _marshaller_Components_DuplicateKeyValue = new _Marshaller_Components_DuplicateKeyValue;
    Components::_tc_UnknownKeyValue = 
    "01000000160000004c000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f556e6b6e6f776e4b657956616c75653a31"
    "2e30000010000000556e6b6e6f776e4b657956616c75650000000000";
    _marshaller_Components_UnknownKeyValue = new _Marshaller_Components_UnknownKeyValue;
    Components::_tc_InvalidKey = 
    "010000001600000044000000010000002600000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c69644b65793a312e30000000"
    "0b000000496e76616c69644b6579000000000000";
    _marshaller_Components_InvalidKey = new _Marshaller_Components_InvalidKey;
    Components::_tc_CCMHomes = 
    "01000000150000008c000000010000002400000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d486f6d65733a312e300009000000"
    "43434d486f6d6573000000001300000048000000010000000e0000003800"
    "0000010000002300000049444c3a6f6d672e6f72672f436f6d706f6e656e"
    "74732f43434d486f6d653a312e3000000800000043434d486f6d65000000"
    "0000";
    Components::_tc_KeylessCCMHome = 
    "010000000e00000047000000010000002a00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4b65796c65737343434d486f6d653a312e"
    "300000000f0000004b65796c65737343434d486f6d6500";
    _marshaller_Components_KeylessCCMHome = new _Marshaller_Components_KeylessCCMHome;
    Components::_tc_HomeNotFound = 
    "010000001600000048000000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f486f6d654e6f74466f756e643a312e3000"
    "0d000000486f6d654e6f74466f756e640000000000000000";
    _marshaller_Components_HomeNotFound = new _Marshaller_Components_HomeNotFound;
    Components::_tc_HomeFinder = 
    "010000000e0000003f000000010000002600000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f486f6d6546696e6465723a312e30000000"
    "0b000000486f6d6546696e64657200";
    _marshaller_Components_HomeFinder = new _Marshaller_Components_HomeFinder;
    Components::_tc_WrongComponentType = 
    "010000001600000054000000010000002e00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f57726f6e67436f6d706f6e656e74547970"
    "653a312e300000001300000057726f6e67436f6d706f6e656e7454797065"
    "000000000000";
    _marshaller_Components_WrongComponentType = new _Marshaller_Components_WrongComponentType;
    Components::_tc_Configurator = 
    "010000000e00000041000000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e666967757261746f723a312e3000"
    "0d000000436f6e666967757261746f7200";
    _marshaller_Components_Configurator = new _Marshaller_Components_Configurator;
    Components::_tc_ConfigValue = 
    "010000001d000000be000000010000002700000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e66696756616c75653a312e300000"
    "0c000000436f6e66696756616c7565000000000000000000020000000500"
    "00006e616d65000000001500000048000000010000002700000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f466561747572654e616d65"
    "3a312e3000000c000000466561747572654e616d65001200000000000000"
    "010000000600000076616c75650000000b0000000100";
    _marshaller_Components_ConfigValue = new _Marshaller_Components_ConfigValue;
    Components::_tc_ConfigValues = 
    "01000000150000001c010000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e66696756616c7565733a312e3000"
    "0d000000436f6e66696756616c7565730000000013000000d00000000100"
    "00001d000000be000000010000002700000049444c3a6f6d672e6f72672f"
    "436f6d706f6e656e74732f436f6e66696756616c75653a312e3000000c00"
    "0000436f6e66696756616c75650000000000000000000200000005000000"
    "6e616d65000000001500000048000000010000002700000049444c3a6f6d"
    "672e6f72672f436f6d706f6e656e74732f466561747572654e616d653a31"
    "2e3000000c000000466561747572654e616d650012000000000000000100"
    "00000600000076616c75650000000b0000000100000000000000";
    Components::_tc_StandardConfigurator = 
    "010000000e00000051000000010000003000000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f5374616e64617264436f6e666967757261"
    "746f723a312e3000150000005374616e64617264436f6e66696775726174"
    "6f7200";
    _marshaller_Components_StandardConfigurator = new _Marshaller_Components_StandardConfigurator;
    Components::_tc_IllegalState = 
    "010000001600000048000000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496c6c6567616c53746174653a312e3000"
    "0d000000496c6c6567616c53746174650000000000000000";
    _marshaller_Components_IllegalState = new _Marshaller_Components_IllegalState;
    Components::_tc_CCMContext = 
    "010000000e0000003f000000010000002600000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d436f6e746578743a312e30000000"
    "0b00000043434d436f6e7465787400";
    Components::_tc_SessionContext = 
    "010000000e00000047000000010000002a00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f53657373696f6e436f6e746578743a312e"
    "300000000f00000053657373696f6e436f6e7465787400";
    Components::_tc_CCMExceptionReason = 
    "0100000011000000df000000010000002e00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d457863657074696f6e526561736f"
    "6e3a312e300000001300000043434d457863657074696f6e526561736f6e"
    "0000070000000d00000053595354454d5f4552524f52000000000d000000"
    "4352454154455f4552524f52000000000d00000052454d4f56455f455252"
    "4f52000000000e0000004455504c49434154455f4b45590000000b000000"
    "46494e445f4552524f520000110000004f424a4543545f4e4f545f464f55"
    "4e44000000000f0000004e4f5f535543485f454e5449545900";
    _marshaller_Components_CCMExceptionReason = new _Marshaller_Components_CCMExceptionReason;
    Components::_tc_CCMException = 
    "01000000160000003b010000010000002800000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f43434d457863657074696f6e3a312e3000"
    "0d00000043434d457863657074696f6e0000000001000000070000007265"
    "61736f6e000011000000df000000010000002e00000049444c3a6f6d672e"
    "6f72672f436f6d706f6e656e74732f43434d457863657074696f6e526561"
    "736f6e3a312e300000001300000043434d457863657074696f6e52656173"
    "6f6e0000070000000d00000053595354454d5f4552524f52000000000d00"
    "00004352454154455f4552524f52000000000d00000052454d4f56455f45"
    "52524f52000000000e0000004455504c49434154455f4b45590000000b00"
    "000046494e445f4552524f520000110000004f424a4543545f4e4f545f46"
    "4f554e44000000000f0000004e4f5f535543485f454e5449545900";
    _marshaller_Components_CCMException = new _Marshaller_Components_CCMException;
    Components::_tc_EnterpriseComponent = 
    "010000000e00000050000000010000002f00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f456e7465727072697365436f6d706f6e65"
    "6e743a312e30000014000000456e7465727072697365436f6d706f6e656e"
    "7400";
    Components::_tc_SessionComponent = 
    "010000000e00000049000000010000002c00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f53657373696f6e436f6d706f6e656e743a"
    "312e30001100000053657373696f6e436f6d706f6e656e7400";
    Components::_tc_HomeExecutorBase = 
    "010000000e00000049000000010000002c00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f486f6d654578656375746f72426173653a"
    "312e300011000000486f6d654578656375746f724261736500";
    Components::_tc_ExecutorLocator = 
    "010000000e00000048000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4578656375746f724c6f6361746f723a31"
    "2e300000100000004578656375746f724c6f6361746f7200";
    Components::_tc_UnknownImplId = 
    "01000000160000004c000000010000002900000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f556e6b6e6f776e496d706c49643a312e30"
    "000000000e000000556e6b6e6f776e496d706c496400000000000000";
    _marshaller_Components_UnknownImplId = new _Marshaller_Components_UnknownImplId;
    Components::_tc_InvalidLocation = 
    "01000000160000004c000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c69644c6f636174696f6e3a31"
    "2e30000010000000496e76616c69644c6f636174696f6e0000000000";
    _marshaller_Components_InvalidLocation = new _Marshaller_Components_InvalidLocation;
    Components::_tc_CreateFailure = 
    "01000000160000004c000000010000002900000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4372656174654661696c7572653a312e30"
    "000000000e0000004372656174654661696c75726500000000000000";
    _marshaller_Components_CreateFailure = new _Marshaller_Components_CreateFailure;
    Components::_tc_RemoveFailure = 
    "01000000160000004c000000010000002900000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f52656d6f76654661696c7572653a312e30"
    "000000000e00000052656d6f76654661696c75726500000000000000";
    _marshaller_Components_RemoveFailure = new _Marshaller_Components_RemoveFailure;
    Components::_tc_InstallationFailure = 
    "010000001600000054000000010000002f00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e7374616c6c6174696f6e4661696c75"
    "72653a312e30000014000000496e7374616c6c6174696f6e4661696c7572"
    "650000000000";
    _marshaller_Components_InstallationFailure = new _Marshaller_Components_InstallationFailure;
    Components::_tc_InvalidAssembly = 
    "01000000160000004c000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f496e76616c6964417373656d626c793a31"
    "2e30000010000000496e76616c6964417373656d626c790000000000";
    _marshaller_Components_InvalidAssembly = new _Marshaller_Components_InvalidAssembly;
    Components::_tc_Container = 
    "010000000e0000003e000000010000002500000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e7461696e65723a312e3000000000"
    "0a000000436f6e7461696e657200";
    _marshaller_Components_Container = new _Marshaller_Components_Container;
    Components::_tc_Containers = 
    "010000001500000098000000010000002600000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6e7461696e6572733a312e30000000"
    "0b000000436f6e7461696e65727300001300000050000000010000000e00"
    "00003e000000010000002500000049444c3a6f6d672e6f72672f436f6d70"
    "6f6e656e74732f436f6e7461696e65723a312e30000000000a000000436f"
    "6e7461696e657200000000000000";
    Components::_tc_ComponentServer = 
    "010000000e00000048000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6d706f6e656e745365727665723a31"
    "2e30000010000000436f6d706f6e656e7453657276657200";
    _marshaller_Components_ComponentServer = new _Marshaller_Components_ComponentServer;
    Components::_tc_ComponentServers = 
    "0100000015000000ac000000010000002c00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6d706f6e656e74536572766572733a"
    "312e300011000000436f6d706f6e656e7453657276657273000000001300"
    "000058000000010000000e00000048000000010000002b00000049444c3a"
    "6f6d672e6f72672f436f6d706f6e656e74732f436f6d706f6e656e745365"
    "727665723a312e30000010000000436f6d706f6e656e7453657276657200"
    "00000000";
    Components::_tc_UUID = 
    "01000000150000003c000000010000002000000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f555549443a312e30000500000055554944"
    "000000001200000000000000";
    Components::_tc_Location = 
    "010000001500000044000000010000002400000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f4c6f636174696f6e3a312e300009000000"
    "4c6f636174696f6e000000001200000000000000";
    Components::_tc_ComponentInstallation = 
    "010000000e00000056000000010000003100000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f436f6d706f6e656e74496e7374616c6c61"
    "74696f6e3a312e300000000016000000436f6d706f6e656e74496e737461"
    "6c6c6174696f6e00";
    _marshaller_Components_ComponentInstallation = new _Marshaller_Components_ComponentInstallation;
    Components::_tc_AssemblyState = 
    "01000000110000006a000000010000002900000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f417373656d626c7953746174653a312e30"
    "000000000e000000417373656d626c795374617465000000020000000900"
    "0000494e414354495645000000000a000000494e5345525649434500";
    _marshaller_Components_AssemblyState = new _Marshaller_Components_AssemblyState;
    Components::_tc_Assembly = 
    "010000000e00000039000000010000002400000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f417373656d626c793a312e300009000000"
    "417373656d626c7900";
    _marshaller_Components_Assembly = new _Marshaller_Components_Assembly;
    Components::_tc_AssemblyFactory = 
    "010000000e00000048000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f417373656d626c79466163746f72793a31"
    "2e30000010000000417373656d626c79466163746f727900";
    _marshaller_Components_AssemblyFactory = new _Marshaller_Components_AssemblyFactory;
    Components::_tc_ServerActivator = 
    "010000000e00000048000000010000002b00000049444c3a6f6d672e6f72"
    "672f436f6d706f6e656e74732f536572766572416374697661746f723a31"
    "2e30000010000000536572766572416374697661746f7200";
    _marshaller_Components_ServerActivator = new _Marshaller_Components_ServerActivator;
    MICOCCM::_tc_MicoCCMD = 
    "010000000e00000039000000010000002200000049444c3a6d69636f2e6f"
    "72672f4d49434f43434d2f4d69636f43434d443a312e3000000009000000"
    "4d69636f43434d4400";
    _marshaller_MICOCCM_MicoCCMD = new _Marshaller_MICOCCM_MicoCCMD;
    MICOCCM::_tc_ComponentServer = 
    "010000000e00000048000000010000002900000049444c3a6d69636f2e6f"
    "72672f4d49434f43434d2f436f6d706f6e656e745365727665723a312e30"
    "0000000010000000436f6d706f6e656e7453657276657200";
    _marshaller_MICOCCM_ComponentServer = new _Marshaller_MICOCCM_ComponentServer;
    _marshaller__seq_Components_FacetDescription = new _Marshaller__seq_Components_FacetDescription;
    _marshaller__seq_Components_ConnectionDescription = new _Marshaller__seq_Components_ConnectionDescription;
    _marshaller__seq_Components_ReceptacleDescription = new _Marshaller__seq_Components_ReceptacleDescription;
    _marshaller__seq_Components_ConsumerDescription = new _Marshaller__seq_Components_ConsumerDescription;
    _marshaller__seq_Components_EmitterDescription = new _Marshaller__seq_Components_EmitterDescription;
    _marshaller__seq_Components_SubscriberDescription = new _Marshaller__seq_Components_SubscriberDescription;
    _marshaller__seq_Components_PublisherDescription = new _Marshaller__seq_Components_PublisherDescription;
    _marshaller__seq_Components_CCMHome = new _Marshaller__seq_Components_CCMHome;
    _marshaller__seq_Components_ConfigValue = new _Marshaller__seq_Components_ConfigValue;
    _marshaller__seq_Components_Container = new _Marshaller__seq_Components_Container;
    _marshaller__seq_Components_ComponentServer = new _Marshaller__seq_Components_ComponentServer;
  }

  ~__tc_init_CCM()
  {
    delete static_cast<_Marshaller_Components_InvalidName*>(_marshaller_Components_InvalidName);
    delete static_cast<_Marshaller_Components_InvalidConfiguration*>(_marshaller_Components_InvalidConfiguration);
    delete static_cast<_Marshaller_Components_AlreadyConnected*>(_marshaller_Components_AlreadyConnected);
    delete static_cast<_Marshaller_Components_InvalidConnection*>(_marshaller_Components_InvalidConnection);
    delete static_cast<_Marshaller_Components_NoConnection*>(_marshaller_Components_NoConnection);
    delete static_cast<_Marshaller_Components_ExceededConnectionLimit*>(_marshaller_Components_ExceededConnectionLimit);
    delete static_cast<_Marshaller_Components_CookieRequired*>(_marshaller_Components_CookieRequired);
    delete static_cast<_Marshaller_Components_PortDescription*>(_marshaller_Components_PortDescription);
    delete static_cast<_Marshaller_Components_Cookie*>(_marshaller_Components_Cookie);
    delete static_cast<_Marshaller_Components_EventBase*>(_marshaller_Components_EventBase);
    delete static_cast<_Marshaller_Components_FacetDescription*>(_marshaller_Components_FacetDescription);
    delete static_cast<_Marshaller_Components_Navigation*>(_marshaller_Components_Navigation);
    delete static_cast<_Marshaller_Components_ConnectionDescription*>(_marshaller_Components_ConnectionDescription);
    delete static_cast<_Marshaller_Components_ReceptacleDescription*>(_marshaller_Components_ReceptacleDescription);
    delete static_cast<_Marshaller_Components_Receptacles*>(_marshaller_Components_Receptacles);
    delete static_cast<_Marshaller_Components_BadEventType*>(_marshaller_Components_BadEventType);
    delete static_cast<_Marshaller_Components_EventConsumerBase*>(_marshaller_Components_EventConsumerBase);
    delete static_cast<_Marshaller_Components_ConsumerDescription*>(_marshaller_Components_ConsumerDescription);
    delete static_cast<_Marshaller_Components_EmitterDescription*>(_marshaller_Components_EmitterDescription);
    delete static_cast<_Marshaller_Components_SubscriberDescription*>(_marshaller_Components_SubscriberDescription);
    delete static_cast<_Marshaller_Components_PublisherDescription*>(_marshaller_Components_PublisherDescription);
    delete static_cast<_Marshaller_Components_Events*>(_marshaller_Components_Events);
    delete static_cast<_Marshaller_Components_CCMHome*>(_marshaller_Components_CCMHome);
    delete static_cast<_Marshaller_Components_ComponentPortDescription*>(_marshaller_Components_ComponentPortDescription);
    delete static_cast<_Marshaller_Components_CCMObject*>(_marshaller_Components_CCMObject);
    delete static_cast<_Marshaller_Components_DuplicateKeyValue*>(_marshaller_Components_DuplicateKeyValue);
    delete static_cast<_Marshaller_Components_UnknownKeyValue*>(_marshaller_Components_UnknownKeyValue);
    delete static_cast<_Marshaller_Components_InvalidKey*>(_marshaller_Components_InvalidKey);
    delete static_cast<_Marshaller_Components_KeylessCCMHome*>(_marshaller_Components_KeylessCCMHome);
    delete static_cast<_Marshaller_Components_HomeNotFound*>(_marshaller_Components_HomeNotFound);
    delete static_cast<_Marshaller_Components_HomeFinder*>(_marshaller_Components_HomeFinder);
    delete static_cast<_Marshaller_Components_WrongComponentType*>(_marshaller_Components_WrongComponentType);
    delete static_cast<_Marshaller_Components_Configurator*>(_marshaller_Components_Configurator);
    delete static_cast<_Marshaller_Components_ConfigValue*>(_marshaller_Components_ConfigValue);
    delete static_cast<_Marshaller_Components_StandardConfigurator*>(_marshaller_Components_StandardConfigurator);
    delete static_cast<_Marshaller_Components_IllegalState*>(_marshaller_Components_IllegalState);
    delete static_cast<_Marshaller_Components_CCMExceptionReason*>(_marshaller_Components_CCMExceptionReason);
    delete static_cast<_Marshaller_Components_CCMException*>(_marshaller_Components_CCMException);
    delete static_cast<_Marshaller_Components_UnknownImplId*>(_marshaller_Components_UnknownImplId);
    delete static_cast<_Marshaller_Components_InvalidLocation*>(_marshaller_Components_InvalidLocation);
    delete static_cast<_Marshaller_Components_CreateFailure*>(_marshaller_Components_CreateFailure);
    delete static_cast<_Marshaller_Components_RemoveFailure*>(_marshaller_Components_RemoveFailure);
    delete static_cast<_Marshaller_Components_InstallationFailure*>(_marshaller_Components_InstallationFailure);
    delete static_cast<_Marshaller_Components_InvalidAssembly*>(_marshaller_Components_InvalidAssembly);
    delete static_cast<_Marshaller_Components_Container*>(_marshaller_Components_Container);
    delete static_cast<_Marshaller_Components_ComponentServer*>(_marshaller_Components_ComponentServer);
    delete static_cast<_Marshaller_Components_ComponentInstallation*>(_marshaller_Components_ComponentInstallation);
    delete static_cast<_Marshaller_Components_AssemblyState*>(_marshaller_Components_AssemblyState);
    delete static_cast<_Marshaller_Components_Assembly*>(_marshaller_Components_Assembly);
    delete static_cast<_Marshaller_Components_AssemblyFactory*>(_marshaller_Components_AssemblyFactory);
    delete static_cast<_Marshaller_Components_ServerActivator*>(_marshaller_Components_ServerActivator);
    delete static_cast<_Marshaller_MICOCCM_MicoCCMD*>(_marshaller_MICOCCM_MicoCCMD);
    delete static_cast<_Marshaller_MICOCCM_ComponentServer*>(_marshaller_MICOCCM_ComponentServer);
    delete static_cast<_Marshaller__seq_Components_FacetDescription*>(_marshaller__seq_Components_FacetDescription);
    delete static_cast<_Marshaller__seq_Components_ConnectionDescription*>(_marshaller__seq_Components_ConnectionDescription);
    delete static_cast<_Marshaller__seq_Components_ReceptacleDescription*>(_marshaller__seq_Components_ReceptacleDescription);
    delete static_cast<_Marshaller__seq_Components_ConsumerDescription*>(_marshaller__seq_Components_ConsumerDescription);
    delete static_cast<_Marshaller__seq_Components_EmitterDescription*>(_marshaller__seq_Components_EmitterDescription);
    delete static_cast<_Marshaller__seq_Components_SubscriberDescription*>(_marshaller__seq_Components_SubscriberDescription);
    delete static_cast<_Marshaller__seq_Components_PublisherDescription*>(_marshaller__seq_Components_PublisherDescription);
    delete static_cast<_Marshaller__seq_Components_CCMHome*>(_marshaller__seq_Components_CCMHome);
    delete static_cast<_Marshaller__seq_Components_ConfigValue*>(_marshaller__seq_Components_ConfigValue);
    delete static_cast<_Marshaller__seq_Components_Container*>(_marshaller__seq_Components_Container);
    delete static_cast<_Marshaller__seq_Components_ComponentServer*>(_marshaller__seq_Components_ComponentServer);
  }
};

static __tc_init_CCM __init_CCM;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface Components::Navigation
POA_Components::Navigation::~Navigation()
{
}

::Components::Navigation_ptr
POA_Components::Navigation::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Navigation::_narrow (obj);
}

CORBA::Boolean
POA_Components::Navigation::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Navigation:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Navigation::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Navigation:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Navigation::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Navigation:1.0");
}

CORBA::Object_ptr
POA_Components::Navigation::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Navigation_stub_clp (poa, obj);
}

bool
POA_Components::Navigation::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 2:
      if( strcmp( __req->op_name(), "same_component" ) == 0 ) {
        CORBA::Object_var _par_ref;
        CORBA::StaticAny _sa_ref( CORBA::_stc_Object, &_par_ref._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_ref );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = same_component( _par_ref.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "get_named_facets" ) == 0 ) {
        ::Components::NameList _par_names;
        CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );

        ::Components::FacetDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_FacetDescription );
        __req->add_in_arg( &_sa_names );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = get_named_facets( _par_names );
          __res.value( _marshaller__seq_Components_FacetDescription, _res );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "provide_facet" ) == 0 ) {
        ::Components::FeatureName_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );

        CORBA::Object_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_Object, &_res );
        __req->add_in_arg( &_sa_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = provide_facet( _par_name.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "get_all_facets" ) == 0 ) {
        ::Components::FacetDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_FacetDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_facets();
        __res.value( _marshaller__seq_Components_FacetDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Navigation::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::Receptacles
POA_Components::Receptacles::~Receptacles()
{
}

::Components::Receptacles_ptr
POA_Components::Receptacles::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Receptacles::_narrow (obj);
}

CORBA::Boolean
POA_Components::Receptacles::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Receptacles:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Receptacles::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Receptacles:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Receptacles::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Receptacles:1.0");
}

CORBA::Object_ptr
POA_Components::Receptacles::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Receptacles_stub_clp (poa, obj);
}

bool
POA_Components::Receptacles::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 1:
      if( strcmp( __req->op_name(), "connect" ) == 0 ) {
        ::Components::FeatureName_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        CORBA::Object_var _par_connection;
        CORBA::StaticAny _sa_connection( CORBA::_stc_Object, &_par_connection._for_demarshal() );

        ::Components::Cookie* _res;
        CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_connection );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = connect( _par_name.inout(), _par_connection.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InvalidConnection_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::AlreadyConnected_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::ExceededConnectionLimit_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::remove_ref( _res );
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "disconnect" ) == 0 ) {
        ::Components::FeatureName_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::Components::Cookie_var _par_ck;
        CORBA::StaticAny _sa_ck( _marshaller_Components_Cookie, &_par_ck._for_demarshal() );

        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_ck );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          disconnect( _par_name.inout(), _par_ck.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InvalidConnection_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::CookieRequired_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::NoConnection_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "get_all_receptacles" ) == 0 ) {
        ::Components::ReceptacleDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ReceptacleDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_receptacles();
        __res.value( _marshaller__seq_Components_ReceptacleDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "get_connections" ) == 0 ) {
        ::Components::FeatureName_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );

        ::Components::ConnectionDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ConnectionDescription );
        __req->add_in_arg( &_sa_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = get_connections( _par_name.inout() );
          __res.value( _marshaller__seq_Components_ConnectionDescription, _res );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "get_named_receptacles" ) == 0 ) {
        ::Components::NameList _par_names;
        CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );

        ::Components::ReceptacleDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ReceptacleDescription );
        __req->add_in_arg( &_sa_names );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = get_named_receptacles( _par_names );
          __res.value( _marshaller__seq_Components_ReceptacleDescription, _res );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Receptacles::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::EventConsumerBase
POA_Components::EventConsumerBase::~EventConsumerBase()
{
}

::Components::EventConsumerBase_ptr
POA_Components::EventConsumerBase::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::EventConsumerBase::_narrow (obj);
}

CORBA::Boolean
POA_Components::EventConsumerBase::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/EventConsumerBase:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::EventConsumerBase::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/EventConsumerBase:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::EventConsumerBase::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/EventConsumerBase:1.0");
}

CORBA::Object_ptr
POA_Components::EventConsumerBase::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::EventConsumerBase_stub_clp (poa, obj);
}

bool
POA_Components::EventConsumerBase::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "push_event" ) == 0 ) {
      ::Components::EventBase_var _par_evt;
      CORBA::StaticAny _sa_evt( _marshaller_Components_EventBase, &_par_evt._for_demarshal() );

      __req->add_in_arg( &_sa_evt );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        push_event( _par_evt.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::BadEventType_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::EventConsumerBase::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::Events
POA_Components::Events::~Events()
{
}

::Components::Events_ptr
POA_Components::Events::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Events::_narrow (obj);
}

CORBA::Boolean
POA_Components::Events::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Events:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Events::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Events:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Events::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Events:1.0");
}

CORBA::Object_ptr
POA_Components::Events::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Events_stub_clp (poa, obj);
}

bool
POA_Components::Events::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 17)) {
    case 1:
      if( strcmp( __req->op_name(), "get_named_consumers" ) == 0 ) {
        ::Components::NameList _par_names;
        CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );

        ::Components::ConsumerDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ConsumerDescription );
        __req->add_in_arg( &_sa_names );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_named_consumers( _par_names );
        __res.value( _marshaller__seq_Components_ConsumerDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "get_consumer" ) == 0 ) {
        ::Components::FeatureName_var _par_sink_name;
        CORBA::StaticAny _sa_sink_name( CORBA::_stc_string, &_par_sink_name._for_demarshal() );

        ::Components::EventConsumerBase_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_EventConsumerBase, &_res );
        __req->add_in_arg( &_sa_sink_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = get_consumer( _par_sink_name.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "subscribe" ) == 0 ) {
        ::Components::FeatureName_var _par_publisher_name;
        CORBA::StaticAny _sa_publisher_name( CORBA::_stc_string, &_par_publisher_name._for_demarshal() );
        ::Components::EventConsumerBase_var _par_subscriber;
        CORBA::StaticAny _sa_subscriber( _marshaller_Components_EventConsumerBase, &_par_subscriber._for_demarshal() );

        ::Components::Cookie* _res;
        CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );
        __req->add_in_arg( &_sa_publisher_name );
        __req->add_in_arg( &_sa_subscriber );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = subscribe( _par_publisher_name.inout(), _par_subscriber.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::remove_ref( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "disconnect_consumer" ) == 0 ) {
        ::Components::FeatureName_var _par_source_name;
        CORBA::StaticAny _sa_source_name( CORBA::_stc_string, &_par_source_name._for_demarshal() );

        ::Components::EventConsumerBase_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_EventConsumerBase, &_res );
        __req->add_in_arg( &_sa_source_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = disconnect_consumer( _par_source_name.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::NoConnection_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "get_all_emitters" ) == 0 ) {
        ::Components::EmitterDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_EmitterDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_emitters();
        __res.value( _marshaller__seq_Components_EmitterDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "get_named_emitters" ) == 0 ) {
        ::Components::NameList _par_names;
        CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );

        ::Components::EmitterDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_EmitterDescription );
        __req->add_in_arg( &_sa_names );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_named_emitters( _par_names );
        __res.value( _marshaller__seq_Components_EmitterDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "get_all_publishers" ) == 0 ) {
        ::Components::PublisherDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_PublisherDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_publishers();
        __res.value( _marshaller__seq_Components_PublisherDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "get_named_publishers" ) == 0 ) {
        ::Components::NameList _par_names;
        CORBA::StaticAny _sa_names( CORBA::_stcseq_string, &_par_names );

        ::Components::PublisherDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_PublisherDescription );
        __req->add_in_arg( &_sa_names );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_named_publishers( _par_names );
        __res.value( _marshaller__seq_Components_PublisherDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 12:
      if( strcmp( __req->op_name(), "unsubscribe" ) == 0 ) {
        ::Components::FeatureName_var _par_publisher_name;
        CORBA::StaticAny _sa_publisher_name( CORBA::_stc_string, &_par_publisher_name._for_demarshal() );
        ::Components::Cookie_var _par_ck;
        CORBA::StaticAny _sa_ck( _marshaller_Components_Cookie, &_par_ck._for_demarshal() );

        __req->add_in_arg( &_sa_publisher_name );
        __req->add_in_arg( &_sa_ck );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          unsubscribe( _par_publisher_name.inout(), _par_ck.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InvalidConnection_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 13:
      if( strcmp( __req->op_name(), "connect_consumer" ) == 0 ) {
        ::Components::FeatureName_var _par_emitter_name;
        CORBA::StaticAny _sa_emitter_name( CORBA::_stc_string, &_par_emitter_name._for_demarshal() );
        ::Components::EventConsumerBase_var _par_consumer;
        CORBA::StaticAny _sa_consumer( _marshaller_Components_EventConsumerBase, &_par_consumer._for_demarshal() );

        __req->add_in_arg( &_sa_emitter_name );
        __req->add_in_arg( &_sa_consumer );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          connect_consumer( _par_emitter_name.inout(), _par_consumer.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidName_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::AlreadyConnected_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 15:
      if( strcmp( __req->op_name(), "get_all_consumers" ) == 0 ) {
        ::Components::ConsumerDescriptions* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ConsumerDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_consumers();
        __res.value( _marshaller__seq_Components_ConsumerDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Events::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::CCMHome
POA_Components::CCMHome::~CCMHome()
{
}

::Components::CCMHome_ptr
POA_Components::CCMHome::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::CCMHome::_narrow (obj);
}

CORBA::Boolean
POA_Components::CCMHome::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/CCMHome:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::CCMHome::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/CCMHome:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::CCMHome::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/CCMHome:1.0");
}

CORBA::Object_ptr
POA_Components::CCMHome::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::CCMHome_stub_clp (poa, obj);
}

bool
POA_Components::CCMHome::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "get_component_def" ) == 0 ) {
      CORBA::Object_ptr _res;
      CORBA::StaticAny __res( CORBA::_stc_Object, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = get_component_def();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "get_home_def" ) == 0 ) {
      CORBA::Object_ptr _res;
      CORBA::StaticAny __res( CORBA::_stc_Object, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = get_home_def();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "remove_component" ) == 0 ) {
      ::Components::CCMObject_var _par_comp;
      CORBA::StaticAny _sa_comp( _marshaller_Components_CCMObject, &_par_comp._for_demarshal() );

      __req->add_in_arg( &_sa_comp );

      if( !__req->read_args() )
        return true;

      remove_component( _par_comp.inout() );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::CCMHome::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::CCMObject
POA_Components::CCMObject::~CCMObject()
{
}

::Components::CCMObject_ptr
POA_Components::CCMObject::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::CCMObject::_narrow (obj);
}

CORBA::Boolean
POA_Components::CCMObject::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/CCMObject:1.0") == 0) {
    return TRUE;
  }
  if (POA_Components::Navigation::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_Components::Receptacles::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_Components::Events::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::CCMObject::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/CCMObject:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::CCMObject::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/CCMObject:1.0");
}

CORBA::Object_ptr
POA_Components::CCMObject::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::CCMObject_stub_clp (poa, obj);
}

bool
POA_Components::CCMObject::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 1:
      if( strcmp( __req->op_name(), "remove" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        remove();
        __req->write_results();
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "get_ccm_home" ) == 0 ) {
        ::Components::CCMHome_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_ccm_home();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "configuration_complete" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        configuration_complete();
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "get_all_ports" ) == 0 ) {
        ::Components::ComponentPortDescription* _res;
        CORBA::StaticAny __res( _marshaller_Components_ComponentPortDescription, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_all_ports();
        __req->write_results();
        CORBA::remove_ref( _res );
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "get_component_def" ) == 0 ) {
        CORBA::Object_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_Object, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_component_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_Components::Navigation::dispatch (__req)) {
    return true;
  }

  if (POA_Components::Receptacles::dispatch (__req)) {
    return true;
  }

  if (POA_Components::Events::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_Components::CCMObject::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::KeylessCCMHome
POA_Components::KeylessCCMHome::~KeylessCCMHome()
{
}

::Components::KeylessCCMHome_ptr
POA_Components::KeylessCCMHome::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::KeylessCCMHome::_narrow (obj);
}

CORBA::Boolean
POA_Components::KeylessCCMHome::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/KeylessCCMHome:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::KeylessCCMHome::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/KeylessCCMHome:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::KeylessCCMHome::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/KeylessCCMHome:1.0");
}

CORBA::Object_ptr
POA_Components::KeylessCCMHome::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::KeylessCCMHome_stub_clp (poa, obj);
}

bool
POA_Components::KeylessCCMHome::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "create_component" ) == 0 ) {
      ::Components::CCMObject_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_CCMObject, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = create_component();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::KeylessCCMHome::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::HomeFinder
POA_Components::HomeFinder::~HomeFinder()
{
}

::Components::HomeFinder_ptr
POA_Components::HomeFinder::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::HomeFinder::_narrow (obj);
}

CORBA::Boolean
POA_Components::HomeFinder::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/HomeFinder:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::HomeFinder::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/HomeFinder:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::HomeFinder::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/HomeFinder:1.0");
}

CORBA::Object_ptr
POA_Components::HomeFinder::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::HomeFinder_stub_clp (poa, obj);
}

bool
POA_Components::HomeFinder::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "find_home_by_component_type" ) == 0 ) {
      CORBA::String_var _par_comp_repid;
      CORBA::StaticAny _sa_comp_repid( CORBA::_stc_string, &_par_comp_repid._for_demarshal() );

      ::Components::CCMHome_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );
      __req->add_in_arg( &_sa_comp_repid );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = find_home_by_component_type( _par_comp_repid.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::HomeNotFound_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "find_home_by_home_type" ) == 0 ) {
      CORBA::String_var _par_home_repid;
      CORBA::StaticAny _sa_home_repid( CORBA::_stc_string, &_par_home_repid._for_demarshal() );

      ::Components::CCMHome_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );
      __req->add_in_arg( &_sa_home_repid );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = find_home_by_home_type( _par_home_repid.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::HomeNotFound_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "find_home_by_name" ) == 0 ) {
      CORBA::String_var _par_home_name;
      CORBA::StaticAny _sa_home_name( CORBA::_stc_string, &_par_home_name._for_demarshal() );

      ::Components::CCMHome_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );
      __req->add_in_arg( &_sa_home_name );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = find_home_by_name( _par_home_name.inout() );
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::HomeFinder::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::Configurator
POA_Components::Configurator::~Configurator()
{
}

::Components::Configurator_ptr
POA_Components::Configurator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Configurator::_narrow (obj);
}

CORBA::Boolean
POA_Components::Configurator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Configurator:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Configurator::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Configurator:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Configurator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Configurator:1.0");
}

CORBA::Object_ptr
POA_Components::Configurator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Configurator_stub_clp (poa, obj);
}

bool
POA_Components::Configurator::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "configure" ) == 0 ) {
      ::Components::CCMObject_var _par_comp;
      CORBA::StaticAny _sa_comp( _marshaller_Components_CCMObject, &_par_comp._for_demarshal() );

      __req->add_in_arg( &_sa_comp );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        configure( _par_comp.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::WrongComponentType_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Configurator::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::StandardConfigurator
POA_Components::StandardConfigurator::~StandardConfigurator()
{
}

::Components::StandardConfigurator_ptr
POA_Components::StandardConfigurator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::StandardConfigurator::_narrow (obj);
}

CORBA::Boolean
POA_Components::StandardConfigurator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/StandardConfigurator:1.0") == 0) {
    return TRUE;
  }
  if (POA_Components::Configurator::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::StandardConfigurator::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/StandardConfigurator:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::StandardConfigurator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/StandardConfigurator:1.0");
}

CORBA::Object_ptr
POA_Components::StandardConfigurator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::StandardConfigurator_stub_clp (poa, obj);
}

bool
POA_Components::StandardConfigurator::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "set_configuration" ) == 0 ) {
      ::Components::ConfigValues _par_descr;
      CORBA::StaticAny _sa_descr( _marshaller__seq_Components_ConfigValue, &_par_descr );

      __req->add_in_arg( &_sa_descr );

      if( !__req->read_args() )
        return true;

      set_configuration( _par_descr );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_Components::Configurator::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_Components::StandardConfigurator::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::Container
POA_Components::Container::~Container()
{
}

::Components::Container_ptr
POA_Components::Container::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Container::_narrow (obj);
}

CORBA::Boolean
POA_Components::Container::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Container:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Container::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Container:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Container::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Container:1.0");
}

CORBA::Object_ptr
POA_Components::Container::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Container_stub_clp (poa, obj);
}

bool
POA_Components::Container::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 11)) {
    case 6:
      if( strcmp( __req->op_name(), "install_home" ) == 0 ) {
        CORBA::String_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        CORBA::String_var _par_entrypt;
        CORBA::StaticAny _sa_entrypt( CORBA::_stc_string, &_par_entrypt._for_demarshal() );
        ::Components::ConfigValues _par_config;
        CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );

        ::Components::CCMHome_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_CCMHome, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_entrypt );
        __req->add_in_arg( &_sa_config );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = install_home( _par_id.inout(), _par_entrypt.inout(), _par_config );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidConfiguration_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InstallationFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "get_component_server" ) == 0 ) {
        ::Components::ComponentServer_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_ComponentServer, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_component_server();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "remove_home" ) == 0 ) {
        ::Components::CCMHome_var _par_href;
        CORBA::StaticAny _sa_href( _marshaller_Components_CCMHome, &_par_href._for_demarshal() );

        __req->add_in_arg( &_sa_href );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          remove_home( _par_href.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::RemoveFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "remove" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          remove();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::RemoveFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "get_homes" ) == 0 ) {
        ::Components::CCMHomes* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_CCMHome );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_homes();
        __res.value( _marshaller__seq_Components_CCMHome, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "_get_configuration" ) == 0 ) {
        ::Components::ConfigValues* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ConfigValue );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = configuration();
        __res.value( _marshaller__seq_Components_ConfigValue, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Container::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::ComponentServer
POA_Components::ComponentServer::~ComponentServer()
{
}

::Components::ComponentServer_ptr
POA_Components::ComponentServer::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::ComponentServer::_narrow (obj);
}

CORBA::Boolean
POA_Components::ComponentServer::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ComponentServer:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::ComponentServer::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/ComponentServer:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::ComponentServer::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/ComponentServer:1.0");
}

CORBA::Object_ptr
POA_Components::ComponentServer::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::ComponentServer_stub_clp (poa, obj);
}

bool
POA_Components::ComponentServer::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 11)) {
    case 5:
      if( strcmp( __req->op_name(), "get_server_activator" ) == 0 ) {
        ::Components::ServerActivator_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_ServerActivator, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_server_activator();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "remove_container" ) == 0 ) {
        ::Components::Container_var _par_cref;
        CORBA::StaticAny _sa_cref( _marshaller_Components_Container, &_par_cref._for_demarshal() );

        __req->add_in_arg( &_sa_cref );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          remove_container( _par_cref.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::RemoveFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "create_container" ) == 0 ) {
        ::Components::ConfigValues _par_config;
        CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );

        ::Components::Container_ptr _res;
        CORBA::StaticAny __res( _marshaller_Components_Container, &_res );
        __req->add_in_arg( &_sa_config );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = create_container( _par_config );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::CreateFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InvalidConfiguration_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "remove" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          remove();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::RemoveFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "get_containers" ) == 0 ) {
        ::Components::Containers* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_Container );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_containers();
        __res.value( _marshaller__seq_Components_Container, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "_get_configuration" ) == 0 ) {
        ::Components::ConfigValues* _res;
        CORBA::StaticAny __res( _marshaller__seq_Components_ConfigValue );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = configuration();
        __res.value( _marshaller__seq_Components_ConfigValue, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::ComponentServer::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::ComponentInstallation
POA_Components::ComponentInstallation::~ComponentInstallation()
{
}

::Components::ComponentInstallation_ptr
POA_Components::ComponentInstallation::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::ComponentInstallation::_narrow (obj);
}

CORBA::Boolean
POA_Components::ComponentInstallation::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ComponentInstallation:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::ComponentInstallation::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/ComponentInstallation:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::ComponentInstallation::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/ComponentInstallation:1.0");
}

CORBA::Object_ptr
POA_Components::ComponentInstallation::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::ComponentInstallation_stub_clp (poa, obj);
}

bool
POA_Components::ComponentInstallation::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 0:
      if( strcmp( __req->op_name(), "replace" ) == 0 ) {
        ::Components::UUID_var _par_implUUID;
        CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID._for_demarshal() );
        ::Components::Location_var _par_component_loc;
        CORBA::StaticAny _sa_component_loc( CORBA::_stc_string, &_par_component_loc._for_demarshal() );

        __req->add_in_arg( &_sa_implUUID );
        __req->add_in_arg( &_sa_component_loc );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          replace( _par_implUUID.inout(), _par_component_loc.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidLocation_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InstallationFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "remove" ) == 0 ) {
        ::Components::UUID_var _par_implUUID;
        CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID._for_demarshal() );

        __req->add_in_arg( &_sa_implUUID );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          remove( _par_implUUID.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::UnknownImplId_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::RemoveFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "get_implementation" ) == 0 ) {
        ::Components::UUID_var _par_implUUID;
        CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID._for_demarshal() );

        ::Components::Location _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->add_in_arg( &_sa_implUUID );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = get_implementation( _par_implUUID.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::UnknownImplId_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InstallationFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "install" ) == 0 ) {
        ::Components::UUID_var _par_implUUID;
        CORBA::StaticAny _sa_implUUID( CORBA::_stc_string, &_par_implUUID._for_demarshal() );
        ::Components::Location_var _par_component_loc;
        CORBA::StaticAny _sa_component_loc( CORBA::_stc_string, &_par_component_loc._for_demarshal() );

        __req->add_in_arg( &_sa_implUUID );
        __req->add_in_arg( &_sa_component_loc );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          install( _par_implUUID.inout(), _par_component_loc.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::Components::InvalidLocation_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::Components::InstallationFailure_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::ComponentInstallation::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::Assembly
POA_Components::Assembly::~Assembly()
{
}

::Components::Assembly_ptr
POA_Components::Assembly::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::Assembly::_narrow (obj);
}

CORBA::Boolean
POA_Components::Assembly::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/Assembly:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::Assembly::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/Assembly:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::Assembly::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/Assembly:1.0");
}

CORBA::Object_ptr
POA_Components::Assembly::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::Assembly_stub_clp (poa, obj);
}

bool
POA_Components::Assembly::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "build" ) == 0 ) {

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        build();
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::CreateFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "tear_down" ) == 0 ) {

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        tear_down();
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::RemoveFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "get_state" ) == 0 ) {
      ::Components::AssemblyState _res;
      CORBA::StaticAny __res( _marshaller_Components_AssemblyState, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = get_state();
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::Assembly::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::AssemblyFactory
POA_Components::AssemblyFactory::~AssemblyFactory()
{
}

::Components::AssemblyFactory_ptr
POA_Components::AssemblyFactory::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::AssemblyFactory::_narrow (obj);
}

CORBA::Boolean
POA_Components::AssemblyFactory::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/AssemblyFactory:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::AssemblyFactory::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/AssemblyFactory:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::AssemblyFactory::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/AssemblyFactory:1.0");
}

CORBA::Object_ptr
POA_Components::AssemblyFactory::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::AssemblyFactory_stub_clp (poa, obj);
}

bool
POA_Components::AssemblyFactory::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "create" ) == 0 ) {
      ::Components::Location_var _par_assembly_loc;
      CORBA::StaticAny _sa_assembly_loc( CORBA::_stc_string, &_par_assembly_loc._for_demarshal() );

      ::Components::Cookie* _res;
      CORBA::StaticAny __res( _marshaller_Components_Cookie, &_res );
      __req->add_in_arg( &_sa_assembly_loc );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = create( _par_assembly_loc.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::InvalidLocation_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      } catch( ::Components::CreateFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::remove_ref( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "lookup" ) == 0 ) {
      ::Components::Cookie_var _par_c;
      CORBA::StaticAny _sa_c( _marshaller_Components_Cookie, &_par_c._for_demarshal() );

      ::Components::Assembly_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_Assembly, &_res );
      __req->add_in_arg( &_sa_c );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = lookup( _par_c.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::InvalidAssembly_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "destroy" ) == 0 ) {
      ::Components::Cookie_var _par_c;
      CORBA::StaticAny _sa_c( _marshaller_Components_Cookie, &_par_c._for_demarshal() );

      __req->add_in_arg( &_sa_c );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        destroy( _par_c.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::InvalidAssembly_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      } catch( ::Components::RemoveFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::AssemblyFactory::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface Components::ServerActivator
POA_Components::ServerActivator::~ServerActivator()
{
}

::Components::ServerActivator_ptr
POA_Components::ServerActivator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Components::ServerActivator::_narrow (obj);
}

CORBA::Boolean
POA_Components::ServerActivator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Components/ServerActivator:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Components::ServerActivator::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Components/ServerActivator:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Components::ServerActivator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Components/ServerActivator:1.0");
}

CORBA::Object_ptr
POA_Components::ServerActivator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Components::ServerActivator_stub_clp (poa, obj);
}

bool
POA_Components::ServerActivator::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "create_component_server" ) == 0 ) {
      ::Components::ConfigValues _par_config;
      CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );

      ::Components::ComponentServer_ptr _res;
      CORBA::StaticAny __res( _marshaller_Components_ComponentServer, &_res );
      __req->add_in_arg( &_sa_config );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = create_component_server( _par_config );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::CreateFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      } catch( ::Components::InvalidConfiguration_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "remove_component_server" ) == 0 ) {
      ::Components::ComponentServer_var _par_server;
      CORBA::StaticAny _sa_server( _marshaller_Components_ComponentServer, &_par_server._for_demarshal() );

      __req->add_in_arg( &_sa_server );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        remove_component_server( _par_server.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::RemoveFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "get_component_servers" ) == 0 ) {
      ::Components::ComponentServers* _res;
      CORBA::StaticAny __res( _marshaller__seq_Components_ComponentServer );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = get_component_servers();
      __res.value( _marshaller__seq_Components_ComponentServer, _res );
      __req->write_results();
      delete _res;
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_Components::ServerActivator::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface MICOCCM::MicoCCMD
POA_MICOCCM::MicoCCMD::~MicoCCMD()
{
}

::MICOCCM::MicoCCMD_ptr
POA_MICOCCM::MicoCCMD::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::MICOCCM::MicoCCMD::_narrow (obj);
}

CORBA::Boolean
POA_MICOCCM::MicoCCMD::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:mico.org/MICOCCM/MicoCCMD:1.0") == 0) {
    return TRUE;
  }
  if (POA_Components::ServerActivator::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_Components::ComponentInstallation::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_Components::AssemblyFactory::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_MICOCCM::MicoCCMD::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:mico.org/MICOCCM/MicoCCMD:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_MICOCCM::MicoCCMD::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:mico.org/MICOCCM/MicoCCMD:1.0");
}

CORBA::Object_ptr
POA_MICOCCM::MicoCCMD::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::MICOCCM::MicoCCMD_stub_clp (poa, obj);
}

bool
POA_MICOCCM::MicoCCMD::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "exec" ) == 0 ) {
      CORBA::String_var _par_prog;
      CORBA::StaticAny _sa_prog( CORBA::_stc_string, &_par_prog._for_demarshal() );
      ::Components::NameList _par_args;
      CORBA::StaticAny _sa_args( CORBA::_stcseq_string, &_par_args );
      CORBA::String_var _par_iorfile;
      CORBA::StaticAny _sa_iorfile( CORBA::_stc_string, &_par_iorfile._for_demarshal() );

      CORBA::Object_ptr _res;
      CORBA::StaticAny __res( CORBA::_stc_Object, &_res );
      __req->add_in_arg( &_sa_prog );
      __req->add_in_arg( &_sa_args );
      __req->add_in_arg( &_sa_iorfile );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = exec( _par_prog.inout(), _par_args, _par_iorfile.inout() );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Components::CreateFailure_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "callback" ) == 0 ) {
      CORBA::String_var _par_token;
      CORBA::StaticAny _sa_token( CORBA::_stc_string, &_par_token._for_demarshal() );
      CORBA::Object_var _par_csref;
      CORBA::StaticAny _sa_csref( CORBA::_stc_Object, &_par_csref._for_demarshal() );

      __req->add_in_arg( &_sa_token );
      __req->add_in_arg( &_sa_csref );

      if( !__req->read_args() )
        return true;

      callback( _par_token.inout(), _par_csref.inout() );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_Components::ServerActivator::dispatch (__req)) {
    return true;
  }

  if (POA_Components::ComponentInstallation::dispatch (__req)) {
    return true;
  }

  if (POA_Components::AssemblyFactory::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_MICOCCM::MicoCCMD::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface MICOCCM::ComponentServer
POA_MICOCCM::ComponentServer::~ComponentServer()
{
}

::MICOCCM::ComponentServer_ptr
POA_MICOCCM::ComponentServer::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::MICOCCM::ComponentServer::_narrow (obj);
}

CORBA::Boolean
POA_MICOCCM::ComponentServer::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:mico.org/MICOCCM/ComponentServer:1.0") == 0) {
    return TRUE;
  }
  if (POA_Components::ComponentServer::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_MICOCCM::ComponentServer::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:mico.org/MICOCCM/ComponentServer:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_MICOCCM::ComponentServer::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:mico.org/MICOCCM/ComponentServer:1.0");
}

CORBA::Object_ptr
POA_MICOCCM::ComponentServer::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::MICOCCM::ComponentServer_stub_clp (poa, obj);
}

bool
POA_MICOCCM::ComponentServer::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "set_config_values" ) == 0 ) {
      ::Components::ConfigValues _par_config;
      CORBA::StaticAny _sa_config( _marshaller__seq_Components_ConfigValue, &_par_config );

      __req->add_in_arg( &_sa_config );

      if( !__req->read_args() )
        return true;

      set_config_values( _par_config );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_Components::ComponentServer::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_MICOCCM::ComponentServer::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}

