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
namespace CORBA
{
CORBA::TypeCodeConst ImplementationDef::_tc_ActivationMode;
}

void operator<<=( CORBA::Any &_a, const CORBA::ImplementationDef::ActivationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef_ActivationMode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ImplementationDef::ActivationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef_ActivationMode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_CORBA_ImplementationDef_ActivationMode : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ImplementationDef::ActivationMode _MICO_T;
  public:
    ~_Marshaller_CORBA_ImplementationDef_ActivationMode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ImplementationDef_ActivationMode::~_Marshaller_CORBA_ImplementationDef_ActivationMode()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ImplementationDef_ActivationMode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ImplementationDef_ActivationMode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ImplementationDef_ActivationMode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ImplementationDef_ActivationMode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_CORBA_ImplementationDef_ActivationMode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ImplementationDef_ActivationMode::typecode()
{
  return CORBA::ImplementationDef::_tc_ActivationMode;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef_ActivationMode;

namespace CORBA
{
CORBA::TypeCodeConst ImplementationDef::_tc_ObjectTag;
}

namespace CORBA
{
CORBA::TypeCodeConst ImplementationDef::_tc_ObjectInfo;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ImplementationDef::ObjectInfo::ObjectInfo()
{
}

CORBA::ImplementationDef::ObjectInfo::ObjectInfo( const ObjectInfo& _s )
{
  repoid = ((ObjectInfo&)_s).repoid;
  tag = ((ObjectInfo&)_s).tag;
}

CORBA::ImplementationDef::ObjectInfo::~ObjectInfo()
{
}

CORBA::ImplementationDef::ObjectInfo&
CORBA::ImplementationDef::ObjectInfo::operator=( const ObjectInfo& _s )
{
  repoid = ((ObjectInfo&)_s).repoid;
  tag = ((ObjectInfo&)_s).tag;
  return *this;
}
#endif

class _Marshaller_CORBA_ImplementationDef_ObjectInfo : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ImplementationDef::ObjectInfo _MICO_T;
  public:
    ~_Marshaller_CORBA_ImplementationDef_ObjectInfo();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ImplementationDef_ObjectInfo::~_Marshaller_CORBA_ImplementationDef_ObjectInfo()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ImplementationDef_ObjectInfo::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ImplementationDef_ObjectInfo::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ImplementationDef_ObjectInfo::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ImplementationDef_ObjectInfo::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->repoid._for_demarshal() ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->tag ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ImplementationDef_ObjectInfo::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->repoid.inout() );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->tag );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ImplementationDef_ObjectInfo::typecode()
{
  return CORBA::ImplementationDef::_tc_ObjectInfo;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef_ObjectInfo;

void operator<<=( CORBA::Any &_a, const CORBA::ImplementationDef::ObjectInfo &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef_ObjectInfo, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ImplementationDef::ObjectInfo *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ImplementationDef::ObjectInfo &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef_ObjectInfo, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ImplementationDef::ObjectInfo *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ImplementationDef_ObjectInfo, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst ImplementationDef::_tc_ObjectInfoList;
}


/*
 * Base interface for class ImplementationDef
 */

CORBA::ImplementationDef::~ImplementationDef()
{
}

void *
CORBA::ImplementationDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ImplementationDef:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::ImplementationDef_ptr
CORBA::ImplementationDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ImplementationDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ImplementationDef:1.0" )))
      return _duplicate( (CORBA::ImplementationDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ImplementationDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ImplementationDef:1.0")) {
      _o = new CORBA::ImplementationDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ImplementationDef_ptr
CORBA::ImplementationDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ImplementationDef;
}
class _Marshaller_CORBA_ImplementationDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ImplementationDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ImplementationDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ImplementationDef::~_Marshaller_CORBA_ImplementationDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ImplementationDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ImplementationDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ImplementationDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ImplementationDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ImplementationDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ImplementationDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ImplementationDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ImplementationDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ImplementationDef::typecode()
{
  return CORBA::_tc_ImplementationDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ImplementationDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ImplementationDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplementationDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ImplementationDef_ptr &_obj )
{
  CORBA::ImplementationDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ImplementationDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ImplementationDef
 */

CORBA::ImplementationDef_stub::~ImplementationDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ImplementationDef::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ImplementationDef:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_CORBA::ImplementationDef *
POA_CORBA::ImplementationDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ImplementationDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ImplementationDef *) p;
  }
  return NULL;
}

CORBA::ImplementationDef_stub_clp::ImplementationDef_stub_clp ()
{
}

CORBA::ImplementationDef_stub_clp::ImplementationDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ImplementationDef_stub_clp::~ImplementationDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ImplementationDef::ActivationMode CORBA::ImplementationDef_stub::mode()
{
  CORBA::ImplementationDef::ActivationMode _res;
  CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef_ActivationMode, &_res );

  CORBA::StaticRequest __req( this, "_get_mode" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplementationDef::ActivationMode
CORBA::ImplementationDef_stub_clp::mode()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplementationDef::ActivationMode __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->mode();
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

  return CORBA::ImplementationDef_stub::mode();
}

#endif // MICO_CONF_NO_POA

void CORBA::ImplementationDef_stub::mode( CORBA::ImplementationDef::ActivationMode _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ImplementationDef_ActivationMode, &_par__value );
  CORBA::StaticRequest __req( this, "_set_mode" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ImplementationDef_stub_clp::mode( CORBA::ImplementationDef::ActivationMode _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->mode(_par__value);
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

  CORBA::ImplementationDef_stub::mode(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplementationDef::ObjectInfoList* CORBA::ImplementationDef_stub::objs()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo );

  CORBA::StaticRequest __req( this, "_get_objs" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ImplementationDef::ObjectInfoList*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplementationDef::ObjectInfoList*
CORBA::ImplementationDef_stub_clp::objs()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplementationDef::ObjectInfoList* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->objs();
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

  return CORBA::ImplementationDef_stub::objs();
}

#endif // MICO_CONF_NO_POA

void CORBA::ImplementationDef_stub::objs( const CORBA::ImplementationDef::ObjectInfoList& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_par__value );
  CORBA::StaticRequest __req( this, "_set_objs" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ImplementationDef_stub_clp::objs( const CORBA::ImplementationDef::ObjectInfoList& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->objs(_par__value);
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

  CORBA::ImplementationDef_stub::objs(_par__value);
}

#endif // MICO_CONF_NO_POA

char* CORBA::ImplementationDef_stub::name()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_name" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::ImplementationDef_stub_clp::name()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->name();
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

  return CORBA::ImplementationDef_stub::name();
}

#endif // MICO_CONF_NO_POA

char* CORBA::ImplementationDef_stub::command()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_command" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::ImplementationDef_stub_clp::command()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->command();
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

  return CORBA::ImplementationDef_stub::command();
}

#endif // MICO_CONF_NO_POA

void CORBA::ImplementationDef_stub::command( const char* _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_command" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ImplementationDef_stub_clp::command( const char* _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->command(_par__value);
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

  CORBA::ImplementationDef_stub::command(_par__value);
}

#endif // MICO_CONF_NO_POA

char* CORBA::ImplementationDef_stub::tostring()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_tostring" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::ImplementationDef_stub_clp::tostring()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplementationDef * _myserv = POA_CORBA::ImplementationDef::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->tostring();
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

  return CORBA::ImplementationDef_stub::tostring();
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst ImplRepository::_tc_ImplDefSeq;
}


/*
 * Base interface for class ImplRepository
 */

CORBA::ImplRepository::~ImplRepository()
{
}

void *
CORBA::ImplRepository::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ImplRepository:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::ImplRepository_ptr
CORBA::ImplRepository::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ImplRepository_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ImplRepository:1.0" )))
      return _duplicate( (CORBA::ImplRepository_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ImplRepository:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ImplRepository:1.0")) {
      _o = new CORBA::ImplRepository_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ImplRepository_ptr
CORBA::ImplRepository::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ImplRepository;
}
class _Marshaller_CORBA_ImplRepository : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ImplRepository_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ImplRepository();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ImplRepository::~_Marshaller_CORBA_ImplRepository()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ImplRepository::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ImplRepository::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ImplRepository::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ImplRepository::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ImplRepository::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ImplRepository::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ImplRepository::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ImplRepository::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ImplRepository::typecode()
{
  return CORBA::_tc_ImplRepository;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ImplRepository;

void
operator<<=( CORBA::Any &_a, const CORBA::ImplRepository_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplRepository, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ImplRepository_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ImplRepository, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ImplRepository_ptr &_obj )
{
  CORBA::ImplRepository_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ImplRepository, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ImplRepository
 */

CORBA::ImplRepository_stub::~ImplRepository_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ImplRepository::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ImplRepository:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_CORBA::ImplRepository *
POA_CORBA::ImplRepository::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ImplRepository:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ImplRepository *) p;
  }
  return NULL;
}

CORBA::ImplRepository_stub_clp::ImplRepository_stub_clp ()
{
}

CORBA::ImplRepository_stub_clp::ImplRepository_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ImplRepository_stub_clp::~ImplRepository_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ImplementationDef_ptr CORBA::ImplRepository_stub::restore( const char* _par_asstring )
{
  CORBA::StaticAny _sa_asstring( CORBA::_stc_string, &_par_asstring );
  CORBA::ImplementationDef_ptr _res = CORBA::ImplementationDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef, &_res );

  CORBA::StaticRequest __req( this, "restore" );
  __req.add_in_arg( &_sa_asstring );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplementationDef_ptr
CORBA::ImplRepository_stub_clp::restore( const char* _par_asstring )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplementationDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->restore(_par_asstring);
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

  return CORBA::ImplRepository_stub::restore(_par_asstring);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplementationDef_ptr CORBA::ImplRepository_stub::create( CORBA::ImplementationDef::ActivationMode _par_mode, const CORBA::ImplementationDef::ObjectInfoList& _par_objs, const char* _par_name, const char* _par_command )
{
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_ImplementationDef_ActivationMode, &_par_mode );
  CORBA::StaticAny _sa_objs( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_par_objs );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_command( CORBA::_stc_string, &_par_command );
  CORBA::ImplementationDef_ptr _res = CORBA::ImplementationDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef, &_res );

  CORBA::StaticRequest __req( this, "create" );
  __req.add_in_arg( &_sa_mode );
  __req.add_in_arg( &_sa_objs );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_command );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplementationDef_ptr
CORBA::ImplRepository_stub_clp::create( CORBA::ImplementationDef::ActivationMode _par_mode, const CORBA::ImplementationDef::ObjectInfoList& _par_objs, const char* _par_name, const char* _par_command )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplementationDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create(_par_mode, _par_objs, _par_name, _par_command);
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

  return CORBA::ImplRepository_stub::create(_par_mode, _par_objs, _par_name, _par_command);
}

#endif // MICO_CONF_NO_POA

void CORBA::ImplRepository_stub::destroy( CORBA::ImplementationDef_ptr _par_impl_def )
{
  CORBA::StaticAny _sa_impl_def( _marshaller_CORBA_ImplementationDef, &_par_impl_def );
  CORBA::StaticRequest __req( this, "destroy" );
  __req.add_in_arg( &_sa_impl_def );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ImplRepository_stub_clp::destroy( CORBA::ImplementationDef_ptr _par_impl_def )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->destroy(_par_impl_def);
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

  CORBA::ImplRepository_stub::destroy(_par_impl_def);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq* CORBA::ImplRepository_stub::find_by_name( const char* _par_name )
{
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );

  CORBA::StaticRequest __req( this, "find_by_name" );
  __req.add_in_arg( &_sa_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ImplRepository::ImplDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq*
CORBA::ImplRepository_stub_clp::find_by_name( const char* _par_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplRepository::ImplDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_by_name(_par_name);
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

  return CORBA::ImplRepository_stub::find_by_name(_par_name);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq* CORBA::ImplRepository_stub::find_by_repoid( const char* _par_repoid )
{
  CORBA::StaticAny _sa_repoid( CORBA::_stc_string, &_par_repoid );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );

  CORBA::StaticRequest __req( this, "find_by_repoid" );
  __req.add_in_arg( &_sa_repoid );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ImplRepository::ImplDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq*
CORBA::ImplRepository_stub_clp::find_by_repoid( const char* _par_repoid )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplRepository::ImplDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_by_repoid(_par_repoid);
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

  return CORBA::ImplRepository_stub::find_by_repoid(_par_repoid);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq* CORBA::ImplRepository_stub::find_by_repoid_tag( const char* _par_repoid, const CORBA::ImplementationDef::ObjectTag& _par_tag )
{
  CORBA::StaticAny _sa_repoid( CORBA::_stc_string, &_par_repoid );
  CORBA::StaticAny _sa_tag( CORBA::_stcseq_octet, &_par_tag );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );

  CORBA::StaticRequest __req( this, "find_by_repoid_tag" );
  __req.add_in_arg( &_sa_repoid );
  __req.add_in_arg( &_sa_tag );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ImplRepository::ImplDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq*
CORBA::ImplRepository_stub_clp::find_by_repoid_tag( const char* _par_repoid, const CORBA::ImplementationDef::ObjectTag& _par_tag )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplRepository::ImplDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_by_repoid_tag(_par_repoid, _par_tag);
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

  return CORBA::ImplRepository_stub::find_by_repoid_tag(_par_repoid, _par_tag);
}

#endif // MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq* CORBA::ImplRepository_stub::find_all()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );

  CORBA::StaticRequest __req( this, "find_all" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ImplRepository::ImplDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ImplRepository::ImplDefSeq*
CORBA::ImplRepository_stub_clp::find_all()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ImplRepository * _myserv = POA_CORBA::ImplRepository::_narrow (_serv);
    if (_myserv) {
      CORBA::ImplRepository::ImplDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->find_all();
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

  return CORBA::ImplRepository_stub::find_all();
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class POAMediator
 */

CORBA::POAMediator::~POAMediator()
{
}

void *
CORBA::POAMediator::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/POAMediator:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::POAMediator_ptr
CORBA::POAMediator::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::POAMediator_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/POAMediator:1.0" )))
      return _duplicate( (CORBA::POAMediator_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/POAMediator:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/POAMediator:1.0")) {
      _o = new CORBA::POAMediator_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::POAMediator_ptr
CORBA::POAMediator::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_POAMediator;
}
class _Marshaller_CORBA_POAMediator : public ::CORBA::StaticTypeInfo {
    typedef CORBA::POAMediator_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_POAMediator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_POAMediator::~_Marshaller_CORBA_POAMediator()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_POAMediator::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_POAMediator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::POAMediator::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_POAMediator::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_POAMediator::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_POAMediator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::POAMediator::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_POAMediator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_POAMediator::typecode()
{
  return CORBA::_tc_POAMediator;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_POAMediator;

void
operator<<=( CORBA::Any &_a, const CORBA::POAMediator_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_POAMediator, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::POAMediator_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_POAMediator, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::POAMediator_ptr &_obj )
{
  CORBA::POAMediator_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_POAMediator, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class POAMediator
 */

CORBA::POAMediator_stub::~POAMediator_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::POAMediator::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/POAMediator:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_CORBA::POAMediator *
POA_CORBA::POAMediator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/POAMediator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::POAMediator *) p;
  }
  return NULL;
}

CORBA::POAMediator_stub_clp::POAMediator_stub_clp ()
{
}

CORBA::POAMediator_stub_clp::POAMediator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::POAMediator_stub_clp::~POAMediator_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

char* CORBA::POAMediator_stub::create_impl( const char* _par_svid, const char* _par_ior )
{
  CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid );
  CORBA::StaticAny _sa_ior( CORBA::_stc_string, &_par_ior );
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "create_impl" );
  __req.add_in_arg( &_sa_svid );
  __req.add_in_arg( &_sa_ior );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::POAMediator_stub_clp::create_impl( const char* _par_svid, const char* _par_ior )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_impl(_par_svid, _par_ior);
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

  return CORBA::POAMediator_stub::create_impl(_par_svid, _par_ior);
}

#endif // MICO_CONF_NO_POA

void CORBA::POAMediator_stub::activate_impl( const char* _par_svid )
{
  CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid );
  CORBA::StaticRequest __req( this, "activate_impl" );
  __req.add_in_arg( &_sa_svid );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::POAMediator_stub_clp::activate_impl( const char* _par_svid )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->activate_impl(_par_svid);
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

  CORBA::POAMediator_stub::activate_impl(_par_svid);
}

#endif // MICO_CONF_NO_POA

void CORBA::POAMediator_stub::deactivate_impl( const char* _par_svid )
{
  CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid );
  CORBA::StaticRequest __req( this, "deactivate_impl" );
  __req.add_in_arg( &_sa_svid );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::POAMediator_stub_clp::deactivate_impl( const char* _par_svid )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->deactivate_impl(_par_svid);
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

  CORBA::POAMediator_stub::deactivate_impl(_par_svid);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::POAMediator_stub::force_activation( CORBA::ImplementationDef_ptr _par_impl )
{
  CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "force_activation" );
  __req.add_in_arg( &_sa_impl );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::POAMediator_stub_clp::force_activation( CORBA::ImplementationDef_ptr _par_impl )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->force_activation(_par_impl);
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

  return CORBA::POAMediator_stub::force_activation(_par_impl);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::POAMediator_stub::hold( CORBA::ImplementationDef_ptr _par_impl )
{
  CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "hold" );
  __req.add_in_arg( &_sa_impl );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::POAMediator_stub_clp::hold( CORBA::ImplementationDef_ptr _par_impl )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->hold(_par_impl);
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

  return CORBA::POAMediator_stub::hold(_par_impl);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::POAMediator_stub::stop( CORBA::ImplementationDef_ptr _par_impl )
{
  CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "stop" );
  __req.add_in_arg( &_sa_impl );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::POAMediator_stub_clp::stop( CORBA::ImplementationDef_ptr _par_impl )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->stop(_par_impl);
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

  return CORBA::POAMediator_stub::stop(_par_impl);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::POAMediator_stub::_cxx_continue( CORBA::ImplementationDef_ptr _par_impl )
{
  CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "continue" );
  __req.add_in_arg( &_sa_impl );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::POAMediator_stub_clp::_cxx_continue( CORBA::ImplementationDef_ptr _par_impl )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->_cxx_continue(_par_impl);
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

  return CORBA::POAMediator_stub::_cxx_continue(_par_impl);
}

#endif // MICO_CONF_NO_POA

void CORBA::POAMediator_stub::shutdown_server()
{
  CORBA::StaticRequest __req( this, "shutdown_server" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::POAMediator_stub_clp::shutdown_server()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::POAMediator * _myserv = POA_CORBA::POAMediator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->shutdown_server();
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

  CORBA::POAMediator_stub::shutdown_server();
}

#endif // MICO_CONF_NO_POA

class _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ImplementationDef_ObjectInfo();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::~_Marshaller__seq_CORBA_ImplementationDef_ObjectInfo()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ImplementationDef_ObjectInfo->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ImplementationDef_ObjectInfo->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000e4000000010000000f000000d4000000010000003300"
    "000049444c3a6f6d672e6f72672f434f5242412f496d706c656d656e7461"
    "74696f6e4465662f4f626a656374496e666f3a312e3000000b0000004f62"
    "6a656374496e666f000002000000070000007265706f6964000012000000"
    "000000000400000074616700150000006000000001000000320000004944"
    "4c3a6f6d672e6f72672f434f5242412f496d706c656d656e746174696f6e"
    "4465662f4f626a6563745461673a312e300000000a0000004f626a656374"
    "546167000000130000000c000000010000000a0000000000000000000000"
    ))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ImplementationDef_ObjectInfo;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ImplementationDef_ObjectInfo, (void *&)_s);
}


class _Marshaller__seq_CORBA_ImplementationDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ImplementationDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ImplementationDef::~_Marshaller__seq_CORBA_ImplementationDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ImplementationDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ImplementationDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ImplementationDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ImplementationDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ImplementationDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ImplementationDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ImplementationDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ImplementationDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000058000000010000000e00000046000000010000002800"
    "000049444c3a6f6d672e6f72672f434f5242412f496d706c656d656e7461"
    "74696f6e4465663a312e300012000000496d706c656d656e746174696f6e"
    "44656600000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ImplementationDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ImplementationDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ImplementationDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ImplementationDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ImplementationDef, (void *&)_s);
}


struct __tc_init_IMR {
  __tc_init_IMR()
  {
    CORBA::ImplementationDef::_tc_ActivationMode = 
    "010000001100000068000000010000003700000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c656d656e746174696f6e4465662f41637469"
    "766174696f6e4d6f64653a312e3000000f00000041637469766174696f6e"
    "4d6f64650000010000000c0000004163746976617465504f4100";
    _marshaller_CORBA_ImplementationDef_ActivationMode = new _Marshaller_CORBA_ImplementationDef_ActivationMode;
    CORBA::ImplementationDef::_tc_ObjectTag = 
    "010000001500000060000000010000003200000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c656d656e746174696f6e4465662f4f626a65"
    "63745461673a312e300000000a0000004f626a6563745461670000001300"
    "00000c000000010000000a00000000000000";
    CORBA::ImplementationDef::_tc_ObjectInfo = 
    "010000000f000000d4000000010000003300000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c656d656e746174696f6e4465662f4f626a65"
    "6374496e666f3a312e3000000b0000004f626a656374496e666f00000200"
    "0000070000007265706f6964000012000000000000000400000074616700"
    "1500000060000000010000003200000049444c3a6f6d672e6f72672f434f"
    "5242412f496d706c656d656e746174696f6e4465662f4f626a6563745461"
    "673a312e300000000a0000004f626a656374546167000000130000000c00"
    "0000010000000a00000000000000";
    _marshaller_CORBA_ImplementationDef_ObjectInfo = new _Marshaller_CORBA_ImplementationDef_ObjectInfo;
    CORBA::ImplementationDef::_tc_ObjectInfoList = 
    "010000001500000040010000010000003700000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c656d656e746174696f6e4465662f4f626a65"
    "6374496e666f4c6973743a312e3000000f0000004f626a656374496e666f"
    "4c697374000013000000e4000000010000000f000000d400000001000000"
    "3300000049444c3a6f6d672e6f72672f434f5242412f496d706c656d656e"
    "746174696f6e4465662f4f626a656374496e666f3a312e3000000b000000"
    "4f626a656374496e666f000002000000070000007265706f696400001200"
    "000000000000040000007461670015000000600000000100000032000000"
    "49444c3a6f6d672e6f72672f434f5242412f496d706c656d656e74617469"
    "6f6e4465662f4f626a6563745461673a312e300000000a0000004f626a65"
    "6374546167000000130000000c000000010000000a000000000000000000"
    "0000";
    CORBA::_tc_ImplementationDef = 
    "010000000e00000046000000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c656d656e746174696f6e4465663a312e3000"
    "12000000496d706c656d656e746174696f6e44656600";
    _marshaller_CORBA_ImplementationDef = new _Marshaller_CORBA_ImplementationDef;
    CORBA::ImplRepository::_tc_ImplDefSeq = 
    "0100000015000000a8000000010000003000000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c5265706f7369746f72792f496d706c446566"
    "5365713a312e30000b000000496d706c4465665365710000130000005800"
    "0000010000000e00000046000000010000002800000049444c3a6f6d672e"
    "6f72672f434f5242412f496d706c656d656e746174696f6e4465663a312e"
    "300012000000496d706c656d656e746174696f6e44656600000000000000"
    ;
    CORBA::_tc_ImplRepository = 
    "010000000e00000043000000010000002500000049444c3a6f6d672e6f72"
    "672f434f5242412f496d706c5265706f7369746f72793a312e3000000000"
    "0f000000496d706c5265706f7369746f727900";
    _marshaller_CORBA_ImplRepository = new _Marshaller_CORBA_ImplRepository;
    CORBA::_tc_POAMediator = 
    "010000000e0000003c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f504f414d65646961746f723a312e300000000c000000"
    "504f414d65646961746f7200";
    _marshaller_CORBA_POAMediator = new _Marshaller_CORBA_POAMediator;
    _marshaller__seq_CORBA_ImplementationDef_ObjectInfo = new _Marshaller__seq_CORBA_ImplementationDef_ObjectInfo;
    _marshaller__seq_CORBA_ImplementationDef = new _Marshaller__seq_CORBA_ImplementationDef;
  }

  ~__tc_init_IMR()
  {
    delete static_cast<_Marshaller_CORBA_ImplementationDef_ActivationMode*>(_marshaller_CORBA_ImplementationDef_ActivationMode);
    delete static_cast<_Marshaller_CORBA_ImplementationDef_ObjectInfo*>(_marshaller_CORBA_ImplementationDef_ObjectInfo);
    delete static_cast<_Marshaller_CORBA_ImplementationDef*>(_marshaller_CORBA_ImplementationDef);
    delete static_cast<_Marshaller_CORBA_ImplRepository*>(_marshaller_CORBA_ImplRepository);
    delete static_cast<_Marshaller_CORBA_POAMediator*>(_marshaller_CORBA_POAMediator);
    delete static_cast<_Marshaller__seq_CORBA_ImplementationDef_ObjectInfo*>(_marshaller__seq_CORBA_ImplementationDef_ObjectInfo);
    delete static_cast<_Marshaller__seq_CORBA_ImplementationDef*>(_marshaller__seq_CORBA_ImplementationDef);
  }
};

static __tc_init_IMR __init_IMR;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface CORBA::ImplementationDef
POA_CORBA::ImplementationDef::~ImplementationDef()
{
}

::CORBA::ImplementationDef_ptr
POA_CORBA::ImplementationDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ImplementationDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ImplementationDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ImplementationDef:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ImplementationDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ImplementationDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ImplementationDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ImplementationDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ImplementationDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ImplementationDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ImplementationDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 13)) {
    case 1:
      if( strcmp( __req->op_name(), "_get_name" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = name();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_get_objs" ) == 0 ) {
        ::CORBA::ImplementationDef::ObjectInfoList* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = objs();
        __res.value( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "_get_tostring" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = tostring();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "_get_mode" ) == 0 ) {
        ::CORBA::ImplementationDef::ActivationMode _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef_ActivationMode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = mode();
        __req->write_results();
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "_get_command" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = command();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "_set_mode" ) == 0 ) {
        ::CORBA::ImplementationDef::ActivationMode _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ImplementationDef_ActivationMode, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        mode( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 12:
      if( strcmp( __req->op_name(), "_set_objs" ) == 0 ) {
        ::CORBA::ImplementationDef::ObjectInfoList _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        objs( _par__value );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_command" ) == 0 ) {
        CORBA::String_var _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        command( _par__value.inout() );
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
POA_CORBA::ImplementationDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ImplRepository
POA_CORBA::ImplRepository::~ImplRepository()
{
}

::CORBA::ImplRepository_ptr
POA_CORBA::ImplRepository::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ImplRepository::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ImplRepository::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ImplRepository:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ImplRepository::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ImplRepository:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ImplRepository::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ImplRepository:1.0");
}

CORBA::Object_ptr
POA_CORBA::ImplRepository::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ImplRepository_stub_clp (poa, obj);
}

bool
POA_CORBA::ImplRepository::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 11)) {
    case 1:
      if( strcmp( __req->op_name(), "destroy" ) == 0 ) {
        ::CORBA::ImplementationDef_var _par_impl_def;
        CORBA::StaticAny _sa_impl_def( _marshaller_CORBA_ImplementationDef, &_par_impl_def._for_demarshal() );

        __req->add_in_arg( &_sa_impl_def );

        if( !__req->read_args() )
          return true;

        destroy( _par_impl_def.inout() );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "find_by_name" ) == 0 ) {
        CORBA::String_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );

        ::CORBA::ImplRepository::ImplDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );
        __req->add_in_arg( &_sa_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = find_by_name( _par_name.inout() );
        __res.value( _marshaller__seq_CORBA_ImplementationDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "find_by_repoid" ) == 0 ) {
        CORBA::String_var _par_repoid;
        CORBA::StaticAny _sa_repoid( CORBA::_stc_string, &_par_repoid._for_demarshal() );

        ::CORBA::ImplRepository::ImplDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );
        __req->add_in_arg( &_sa_repoid );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = find_by_repoid( _par_repoid.inout() );
        __res.value( _marshaller__seq_CORBA_ImplementationDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "create" ) == 0 ) {
        ::CORBA::ImplementationDef::ActivationMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_ImplementationDef_ActivationMode, &_par_mode );
        ::CORBA::ImplementationDef::ObjectInfoList _par_objs;
        CORBA::StaticAny _sa_objs( _marshaller__seq_CORBA_ImplementationDef_ObjectInfo, &_par_objs );
        CORBA::String_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        CORBA::String_var _par_command;
        CORBA::StaticAny _sa_command( CORBA::_stc_string, &_par_command._for_demarshal() );

        ::CORBA::ImplementationDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef, &_res );
        __req->add_in_arg( &_sa_mode );
        __req->add_in_arg( &_sa_objs );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_command );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create( _par_mode, _par_objs, _par_name.inout(), _par_command.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "find_by_repoid_tag" ) == 0 ) {
        CORBA::String_var _par_repoid;
        CORBA::StaticAny _sa_repoid( CORBA::_stc_string, &_par_repoid._for_demarshal() );
        ::CORBA::ImplementationDef::ObjectTag _par_tag;
        CORBA::StaticAny _sa_tag( CORBA::_stcseq_octet, &_par_tag );

        ::CORBA::ImplRepository::ImplDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );
        __req->add_in_arg( &_sa_repoid );
        __req->add_in_arg( &_sa_tag );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = find_by_repoid_tag( _par_repoid.inout(), _par_tag );
        __res.value( _marshaller__seq_CORBA_ImplementationDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "restore" ) == 0 ) {
        CORBA::String_var _par_asstring;
        CORBA::StaticAny _sa_asstring( CORBA::_stc_string, &_par_asstring._for_demarshal() );

        ::CORBA::ImplementationDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ImplementationDef, &_res );
        __req->add_in_arg( &_sa_asstring );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = restore( _par_asstring.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "find_all" ) == 0 ) {
        ::CORBA::ImplRepository::ImplDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ImplementationDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = find_all();
        __res.value( _marshaller__seq_CORBA_ImplementationDef, _res );
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
POA_CORBA::ImplRepository::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::POAMediator
POA_CORBA::POAMediator::~POAMediator()
{
}

::CORBA::POAMediator_ptr
POA_CORBA::POAMediator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::POAMediator::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::POAMediator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/POAMediator:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::POAMediator::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/POAMediator:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::POAMediator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/POAMediator:1.0");
}

CORBA::Object_ptr
POA_CORBA::POAMediator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::POAMediator_stub_clp (poa, obj);
}

bool
POA_CORBA::POAMediator::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 13)) {
    case 2:
      if( strcmp( __req->op_name(), "deactivate_impl" ) == 0 ) {
        CORBA::String_var _par_svid;
        CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid._for_demarshal() );

        __req->add_in_arg( &_sa_svid );

        if( !__req->read_args() )
          return true;

        deactivate_impl( _par_svid.inout() );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "continue" ) == 0 ) {
        ::CORBA::ImplementationDef_var _par_impl;
        CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_impl );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = _cxx_continue( _par_impl.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "force_activation" ) == 0 ) {
        ::CORBA::ImplementationDef_var _par_impl;
        CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_impl );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = force_activation( _par_impl.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "create_impl" ) == 0 ) {
        CORBA::String_var _par_svid;
        CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid._for_demarshal() );
        CORBA::String_var _par_ior;
        CORBA::StaticAny _sa_ior( CORBA::_stc_string, &_par_ior._for_demarshal() );

        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->add_in_arg( &_sa_svid );
        __req->add_in_arg( &_sa_ior );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_impl( _par_svid.inout(), _par_ior.inout() );
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "stop" ) == 0 ) {
        ::CORBA::ImplementationDef_var _par_impl;
        CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_impl );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = stop( _par_impl.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "activate_impl" ) == 0 ) {
        CORBA::String_var _par_svid;
        CORBA::StaticAny _sa_svid( CORBA::_stc_string, &_par_svid._for_demarshal() );

        __req->add_in_arg( &_sa_svid );

        if( !__req->read_args() )
          return true;

        activate_impl( _par_svid.inout() );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "hold" ) == 0 ) {
        ::CORBA::ImplementationDef_var _par_impl;
        CORBA::StaticAny _sa_impl( _marshaller_CORBA_ImplementationDef, &_par_impl._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_impl );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = hold( _par_impl.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "shutdown_server" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        shutdown_server();
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
POA_CORBA::POAMediator::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}

