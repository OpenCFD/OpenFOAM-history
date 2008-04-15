/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <mico/reflection.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------
namespace Reflection
{
CORBA::TypeCodeConst _tc_FormatNotSupported;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Reflection::FormatNotSupported::FormatNotSupported()
{
}

Reflection::FormatNotSupported::FormatNotSupported( const FormatNotSupported& _s )
{
}

Reflection::FormatNotSupported::~FormatNotSupported()
{
}

Reflection::FormatNotSupported&
Reflection::FormatNotSupported::operator=( const FormatNotSupported& _s )
{
  return *this;
}
#endif

class _Marshaller_Reflection_FormatNotSupported : public ::CORBA::StaticTypeInfo {
    typedef ::Reflection::FormatNotSupported _MICO_T;
  public:
    ~_Marshaller_Reflection_FormatNotSupported();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Reflection_FormatNotSupported::~_Marshaller_Reflection_FormatNotSupported()
{
}

::CORBA::StaticValueType _Marshaller_Reflection_FormatNotSupported::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Reflection_FormatNotSupported::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Reflection_FormatNotSupported::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Reflection_FormatNotSupported::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    dc.except_end();
}

void _Marshaller_Reflection_FormatNotSupported::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:omg.org/Reflection/FormatNotSupported:1.0" );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_Reflection_FormatNotSupported::typecode()
{
  return Reflection::_tc_FormatNotSupported;
}

::CORBA::StaticTypeInfo *_marshaller_Reflection_FormatNotSupported;

void operator<<=( CORBA::Any &_a, const Reflection::FormatNotSupported &_e )
{
  CORBA::StaticAny _sa (_marshaller_Reflection_FormatNotSupported, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Reflection::FormatNotSupported *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Reflection::FormatNotSupported &_e )
{
  CORBA::StaticAny _sa (_marshaller_Reflection_FormatNotSupported, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Reflection::FormatNotSupported *&_e )
{
  return _a.to_static_any (_marshaller_Reflection_FormatNotSupported, (void *&)_e);
}

void Reflection::FormatNotSupported::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw FormatNotSupported_var( (Reflection::FormatNotSupported*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *Reflection::FormatNotSupported::_repoid() const
{
  return "IDL:omg.org/Reflection/FormatNotSupported:1.0";
}

void Reflection::FormatNotSupported::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_Reflection_FormatNotSupported->marshal( _en, (void*) this );
}

void Reflection::FormatNotSupported::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *Reflection::FormatNotSupported::_clone() const
{
  return new FormatNotSupported( *this );
}

Reflection::FormatNotSupported *Reflection::FormatNotSupported::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Reflection/FormatNotSupported:1.0" ) )
    return (FormatNotSupported *) _ex;
  return NULL;
}

const Reflection::FormatNotSupported *Reflection::FormatNotSupported::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:omg.org/Reflection/FormatNotSupported:1.0" ) )
    return (FormatNotSupported *) _ex;
  return NULL;
}


/*
 * Base interface for class IFRProvider
 */

Reflection::IFRProvider::~IFRProvider()
{
}

void *
Reflection::IFRProvider::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/Reflection/IFRProvider:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

Reflection::IFRProvider_ptr
Reflection::IFRProvider::_narrow( CORBA::Object_ptr _obj )
{
  Reflection::IFRProvider_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/Reflection/IFRProvider:1.0" )))
      return _duplicate( (Reflection::IFRProvider_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/Reflection/IFRProvider:1.0") || _obj->_is_a_remote ("IDL:omg.org/Reflection/IFRProvider:1.0")) {
      _o = new Reflection::IFRProvider_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

Reflection::IFRProvider_ptr
Reflection::IFRProvider::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace Reflection
{
CORBA::TypeCodeConst _tc_IFRProvider;
}
class _Marshaller_Reflection_IFRProvider : public ::CORBA::StaticTypeInfo {
    typedef Reflection::IFRProvider_ptr _MICO_T;
  public:
    ~_Marshaller_Reflection_IFRProvider();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Reflection_IFRProvider::~_Marshaller_Reflection_IFRProvider()
{
}

::CORBA::StaticValueType _Marshaller_Reflection_IFRProvider::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_Reflection_IFRProvider::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::Reflection::IFRProvider::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_Reflection_IFRProvider::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_Reflection_IFRProvider::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_Reflection_IFRProvider::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::Reflection::IFRProvider::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_Reflection_IFRProvider::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_Reflection_IFRProvider::typecode()
{
  return Reflection::_tc_IFRProvider;
}

::CORBA::StaticTypeInfo *_marshaller_Reflection_IFRProvider;

void
operator<<=( CORBA::Any &_a, const Reflection::IFRProvider_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_Reflection_IFRProvider, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, Reflection::IFRProvider_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_Reflection_IFRProvider, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, Reflection::IFRProvider_ptr &_obj )
{
  Reflection::IFRProvider_ptr *p;
  if (_a.to_static_any (_marshaller_Reflection_IFRProvider, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class IFRProvider
 */

Reflection::IFRProvider_stub::~IFRProvider_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_Reflection::IFRProvider::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Reflection/IFRProvider:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_Reflection::IFRProvider *
POA_Reflection::IFRProvider::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/Reflection/IFRProvider:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_Reflection::IFRProvider *) p;
  }
  return NULL;
}

Reflection::IFRProvider_stub_clp::IFRProvider_stub_clp ()
{
}

Reflection::IFRProvider_stub_clp::IFRProvider_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

Reflection::IFRProvider_stub_clp::~IFRProvider_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Any* Reflection::IFRProvider_stub::omg_get_ifr_metadata()
{
  CORBA::StaticAny __res( CORBA::_stc_any );

  CORBA::StaticRequest __req( this, "omg_get_ifr_metadata" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Reflection_FormatNotSupported, "IDL:omg.org/Reflection/FormatNotSupported:1.0",
    0);
  return (CORBA::Any*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::Any*
Reflection::IFRProvider_stub_clp::omg_get_ifr_metadata()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Reflection::IFRProvider * _myserv = POA_Reflection::IFRProvider::_narrow (_serv);
    if (_myserv) {
      CORBA::Any* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->omg_get_ifr_metadata();
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

  return Reflection::IFRProvider_stub::omg_get_ifr_metadata();
}

#endif // MICO_CONF_NO_POA

char* Reflection::IFRProvider_stub::omg_get_xml_metadata()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "omg_get_xml_metadata" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_Reflection_FormatNotSupported, "IDL:omg.org/Reflection/FormatNotSupported:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
Reflection::IFRProvider_stub_clp::omg_get_xml_metadata()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_Reflection::IFRProvider * _myserv = POA_Reflection::IFRProvider::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->omg_get_xml_metadata();
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

  return Reflection::IFRProvider_stub::omg_get_xml_metadata();
}

#endif // MICO_CONF_NO_POA

struct __tc_init_REFLECTION {
  __tc_init_REFLECTION()
  {
    Reflection::_tc_FormatNotSupported = 
    "010000001600000054000000010000002e00000049444c3a6f6d672e6f72"
    "672f5265666c656374696f6e2f466f726d61744e6f74537570706f727465"
    "643a312e3000000013000000466f726d61744e6f74537570706f72746564"
    "000000000000";
    _marshaller_Reflection_FormatNotSupported = new _Marshaller_Reflection_FormatNotSupported;
    Reflection::_tc_IFRProvider = 
    "010000000e00000040000000010000002700000049444c3a6f6d672e6f72"
    "672f5265666c656374696f6e2f49465250726f76696465723a312e300000"
    "0c00000049465250726f766964657200";
    _marshaller_Reflection_IFRProvider = new _Marshaller_Reflection_IFRProvider;
  }

  ~__tc_init_REFLECTION()
  {
    delete static_cast<_Marshaller_Reflection_FormatNotSupported*>(_marshaller_Reflection_FormatNotSupported);
    delete static_cast<_Marshaller_Reflection_IFRProvider*>(_marshaller_Reflection_IFRProvider);
  }
};

static __tc_init_REFLECTION __init_REFLECTION;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface Reflection::IFRProvider
POA_Reflection::IFRProvider::~IFRProvider()
{
}

::Reflection::IFRProvider_ptr
POA_Reflection::IFRProvider::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::Reflection::IFRProvider::_narrow (obj);
}

CORBA::Boolean
POA_Reflection::IFRProvider::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/Reflection/IFRProvider:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_Reflection::IFRProvider::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/Reflection/IFRProvider:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_Reflection::IFRProvider::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/Reflection/IFRProvider:1.0");
}

CORBA::Object_ptr
POA_Reflection::IFRProvider::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::Reflection::IFRProvider_stub_clp (poa, obj);
}

bool
POA_Reflection::IFRProvider::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "omg_get_ifr_metadata" ) == 0 ) {
      CORBA::Any* _res;
      CORBA::StaticAny __res( CORBA::_stc_any );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = omg_get_ifr_metadata();
        __res.value( CORBA::_stc_any, _res );
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Reflection::FormatNotSupported_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      delete _res;
      return true;
    }
    if( strcmp( __req->op_name(), "omg_get_xml_metadata" ) == 0 ) {
      char* _res;
      CORBA::StaticAny __res( CORBA::_stc_string, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _res = omg_get_xml_metadata();
      #ifdef HAVE_EXCEPTIONS
      } catch( ::Reflection::FormatNotSupported_catch &_ex ) {
        __req->set_exception( _ex->_clone() );
        __req->write_results();
        return true;
      }
      #endif
      __req->write_results();
      CORBA::string_free( _res );
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
POA_Reflection::IFRProvider::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}

