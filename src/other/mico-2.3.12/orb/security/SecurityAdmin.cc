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

/*
 * Base interface for class AccessPolicy
 */

SecurityAdmin::AccessPolicy::~AccessPolicy()
{
}

void *
SecurityAdmin::AccessPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/AccessPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::AccessPolicy_ptr
SecurityAdmin::AccessPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/AccessPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::AccessPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::AccessPolicy_ptr
SecurityAdmin::AccessPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_AccessPolicy;
}
class _Marshaller_SecurityAdmin_AccessPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::AccessPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_AccessPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_AccessPolicy::~_Marshaller_SecurityAdmin_AccessPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_AccessPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_AccessPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::AccessPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_AccessPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_AccessPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_AccessPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::AccessPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_AccessPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_AccessPolicy::typecode()
{
  return SecurityAdmin::_tc_AccessPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AccessPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::AccessPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AccessPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::AccessPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AccessPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::AccessPolicy_ptr &_obj )
{
  SecurityAdmin::AccessPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_AccessPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class ObjectAccessPolicy
 */

SecurityAdmin::ObjectAccessPolicy::~ObjectAccessPolicy()
{
}

void *
SecurityAdmin::ObjectAccessPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/ObjectAccessPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::ObjectAccessPolicy_ptr
SecurityAdmin::ObjectAccessPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/ObjectAccessPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::ObjectAccessPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::ObjectAccessPolicy_ptr
SecurityAdmin::ObjectAccessPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_ObjectAccessPolicy;
}
class _Marshaller_SecurityAdmin_ObjectAccessPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::ObjectAccessPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_ObjectAccessPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_ObjectAccessPolicy::~_Marshaller_SecurityAdmin_ObjectAccessPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_ObjectAccessPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_ObjectAccessPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::ObjectAccessPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_ObjectAccessPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_ObjectAccessPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_ObjectAccessPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::ObjectAccessPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_ObjectAccessPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_ObjectAccessPolicy::typecode()
{
  return SecurityAdmin::_tc_ObjectAccessPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_ObjectAccessPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::ObjectAccessPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_ObjectAccessPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::ObjectAccessPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_ObjectAccessPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::ObjectAccessPolicy_ptr &_obj )
{
  SecurityAdmin::ObjectAccessPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_ObjectAccessPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class DomainAccessPolicy
 */

SecurityAdmin::DomainAccessPolicy::~DomainAccessPolicy()
{
}

void *
SecurityAdmin::DomainAccessPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/DomainAccessPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityAdmin::AccessPolicy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::DomainAccessPolicy_ptr
SecurityAdmin::DomainAccessPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/DomainAccessPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::DomainAccessPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::DomainAccessPolicy_ptr
SecurityAdmin::DomainAccessPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_DomainAccessPolicy;
}
class _Marshaller_SecurityAdmin_DomainAccessPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::DomainAccessPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_DomainAccessPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_DomainAccessPolicy::~_Marshaller_SecurityAdmin_DomainAccessPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_DomainAccessPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_DomainAccessPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::DomainAccessPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_DomainAccessPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_DomainAccessPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_DomainAccessPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::DomainAccessPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_DomainAccessPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_DomainAccessPolicy::typecode()
{
  return SecurityAdmin::_tc_DomainAccessPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_DomainAccessPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::DomainAccessPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_DomainAccessPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::DomainAccessPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_DomainAccessPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::DomainAccessPolicy_ptr &_obj )
{
  SecurityAdmin::DomainAccessPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_DomainAccessPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class AuditPolicy
 */

SecurityAdmin::AuditPolicy::~AuditPolicy()
{
}

void *
SecurityAdmin::AuditPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/AuditPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::AuditPolicy_ptr
SecurityAdmin::AuditPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/AuditPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::AuditPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::AuditPolicy_ptr
SecurityAdmin::AuditPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_AuditPolicy;
}
class _Marshaller_SecurityAdmin_AuditPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::AuditPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_AuditPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_AuditPolicy::~_Marshaller_SecurityAdmin_AuditPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_AuditPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_AuditPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::AuditPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_AuditPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_AuditPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_AuditPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::AuditPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_AuditPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_AuditPolicy::typecode()
{
  return SecurityAdmin::_tc_AuditPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AuditPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::AuditPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::AuditPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::AuditPolicy_ptr &_obj )
{
  SecurityAdmin::AuditPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_AuditPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class AuditTargetPolicy
 */

SecurityAdmin::AuditTargetPolicy::~AuditTargetPolicy()
{
}

void *
SecurityAdmin::AuditTargetPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/AuditTargetPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityAdmin::AuditPolicy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::AuditTargetPolicy_ptr
SecurityAdmin::AuditTargetPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/AuditTargetPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::AuditTargetPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::AuditTargetPolicy_ptr
SecurityAdmin::AuditTargetPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_AuditTargetPolicy;
}
class _Marshaller_SecurityAdmin_AuditTargetPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::AuditTargetPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_AuditTargetPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_AuditTargetPolicy::~_Marshaller_SecurityAdmin_AuditTargetPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_AuditTargetPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_AuditTargetPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::AuditTargetPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_AuditTargetPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_AuditTargetPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_AuditTargetPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::AuditTargetPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_AuditTargetPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_AuditTargetPolicy::typecode()
{
  return SecurityAdmin::_tc_AuditTargetPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AuditTargetPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::AuditTargetPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditTargetPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::AuditTargetPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditTargetPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::AuditTargetPolicy_ptr &_obj )
{
  SecurityAdmin::AuditTargetPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_AuditTargetPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class AuditClientPolicy
 */

SecurityAdmin::AuditClientPolicy::~AuditClientPolicy()
{
}

void *
SecurityAdmin::AuditClientPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/AuditClientPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = SecurityAdmin::AuditPolicy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::AuditClientPolicy_ptr
SecurityAdmin::AuditClientPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/AuditClientPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::AuditClientPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::AuditClientPolicy_ptr
SecurityAdmin::AuditClientPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_AuditClientPolicy;
}
class _Marshaller_SecurityAdmin_AuditClientPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::AuditClientPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_AuditClientPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_AuditClientPolicy::~_Marshaller_SecurityAdmin_AuditClientPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_AuditClientPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_AuditClientPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::AuditClientPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_AuditClientPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_AuditClientPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_AuditClientPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::AuditClientPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_AuditClientPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_AuditClientPolicy::typecode()
{
  return SecurityAdmin::_tc_AuditClientPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AuditClientPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::AuditClientPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditClientPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::AuditClientPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditClientPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::AuditClientPolicy_ptr &_obj )
{
  SecurityAdmin::AuditClientPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_AuditClientPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class SecureInvocationPolicy
 */

SecurityAdmin::SecureInvocationPolicy::~SecureInvocationPolicy()
{
}

void *
SecurityAdmin::SecureInvocationPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/SecureInvocationPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::SecureInvocationPolicy_ptr
SecurityAdmin::SecureInvocationPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/SecureInvocationPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::SecureInvocationPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::SecureInvocationPolicy_ptr
SecurityAdmin::SecureInvocationPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_SecureInvocationPolicy;
}
class _Marshaller_SecurityAdmin_SecureInvocationPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::SecureInvocationPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_SecureInvocationPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_SecureInvocationPolicy::~_Marshaller_SecurityAdmin_SecureInvocationPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_SecureInvocationPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_SecureInvocationPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::SecureInvocationPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_SecureInvocationPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_SecureInvocationPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_SecureInvocationPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::SecureInvocationPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_SecureInvocationPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_SecureInvocationPolicy::typecode()
{
  return SecurityAdmin::_tc_SecureInvocationPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_SecureInvocationPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::SecureInvocationPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_SecureInvocationPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::SecureInvocationPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_SecureInvocationPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::SecureInvocationPolicy_ptr &_obj )
{
  SecurityAdmin::SecureInvocationPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_SecureInvocationPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}



/*
 * Base interface for class DelegationPolicy
 */

SecurityAdmin::DelegationPolicy::~DelegationPolicy()
{
}

void *
SecurityAdmin::DelegationPolicy::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/SecurityAdmin/DelegationPolicy:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Policy::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

SecurityAdmin::DelegationPolicy_ptr
SecurityAdmin::DelegationPolicy::_narrow( CORBA::Object_ptr _obj )
{
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/SecurityAdmin/DelegationPolicy:1.0" )))
      return _duplicate( (SecurityAdmin::DelegationPolicy_ptr) _p );
  }
  return _nil();
}

SecurityAdmin::DelegationPolicy_ptr
SecurityAdmin::DelegationPolicy::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_DelegationPolicy;
}
class _Marshaller_SecurityAdmin_DelegationPolicy : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::DelegationPolicy_ptr _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_DelegationPolicy();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_DelegationPolicy::~_Marshaller_SecurityAdmin_DelegationPolicy()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_DelegationPolicy::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_SecurityAdmin_DelegationPolicy::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::SecurityAdmin::DelegationPolicy::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_SecurityAdmin_DelegationPolicy::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_SecurityAdmin_DelegationPolicy::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_SecurityAdmin_DelegationPolicy::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::SecurityAdmin::DelegationPolicy::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_SecurityAdmin_DelegationPolicy::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_DelegationPolicy::typecode()
{
  return SecurityAdmin::_tc_DelegationPolicy;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_DelegationPolicy;

void
operator<<=( CORBA::Any &_a, const SecurityAdmin::DelegationPolicy_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_DelegationPolicy, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, SecurityAdmin::DelegationPolicy_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_DelegationPolicy, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, SecurityAdmin::DelegationPolicy_ptr &_obj )
{
  SecurityAdmin::DelegationPolicy_ptr *p;
  if (_a.to_static_any (_marshaller_SecurityAdmin_DelegationPolicy, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


namespace SecurityAdmin
{
CORBA::TypeCodeConst _tc_AuditTimeInterval;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
SecurityAdmin::AuditTimeInterval::AuditTimeInterval()
{
}

SecurityAdmin::AuditTimeInterval::AuditTimeInterval( const AuditTimeInterval& _s )
{
  begin = ((AuditTimeInterval&)_s).begin;
  end = ((AuditTimeInterval&)_s).end;
}

SecurityAdmin::AuditTimeInterval::~AuditTimeInterval()
{
}

SecurityAdmin::AuditTimeInterval&
SecurityAdmin::AuditTimeInterval::operator=( const AuditTimeInterval& _s )
{
  begin = ((AuditTimeInterval&)_s).begin;
  end = ((AuditTimeInterval&)_s).end;
  return *this;
}
#endif

class _Marshaller_SecurityAdmin_AuditTimeInterval : public ::CORBA::StaticTypeInfo {
    typedef SecurityAdmin::AuditTimeInterval _MICO_T;
  public:
    ~_Marshaller_SecurityAdmin_AuditTimeInterval();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_SecurityAdmin_AuditTimeInterval::~_Marshaller_SecurityAdmin_AuditTimeInterval()
{
}

::CORBA::StaticValueType _Marshaller_SecurityAdmin_AuditTimeInterval::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_SecurityAdmin_AuditTimeInterval::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_SecurityAdmin_AuditTimeInterval::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_SecurityAdmin_AuditTimeInterval::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->begin ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->end ) &&
    dc.struct_end();
}

void _Marshaller_SecurityAdmin_AuditTimeInterval::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->begin );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->end );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_SecurityAdmin_AuditTimeInterval::typecode()
{
  return SecurityAdmin::_tc_AuditTimeInterval;
}

::CORBA::StaticTypeInfo *_marshaller_SecurityAdmin_AuditTimeInterval;

void operator<<=( CORBA::Any &_a, const SecurityAdmin::AuditTimeInterval &_s )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditTimeInterval, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SecurityAdmin::AuditTimeInterval *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SecurityAdmin::AuditTimeInterval &_s )
{
  CORBA::StaticAny _sa (_marshaller_SecurityAdmin_AuditTimeInterval, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SecurityAdmin::AuditTimeInterval *&_s )
{
  return _a.to_static_any (_marshaller_SecurityAdmin_AuditTimeInterval, (void *&)_s);
}

struct __tc_init_SECURITYADMIN {
  __tc_init_SECURITYADMIN()
  {
    SecurityAdmin::_tc_AccessPolicy = 
    "010000000e00000045000000010000002b00000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f416363657373506f6c6963793a31"
    "2e3000000d000000416363657373506f6c69637900";
    _marshaller_SecurityAdmin_AccessPolicy = new _Marshaller_SecurityAdmin_AccessPolicy;
    SecurityAdmin::_tc_ObjectAccessPolicy = 
    "010000000e00000053000000010000003100000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f4f626a656374416363657373506f"
    "6c6963793a312e3000000000130000004f626a656374416363657373506f"
    "6c69637900";
    _marshaller_SecurityAdmin_ObjectAccessPolicy = new _Marshaller_SecurityAdmin_ObjectAccessPolicy;
    SecurityAdmin::_tc_DomainAccessPolicy = 
    "010000000e00000053000000010000003100000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f446f6d61696e416363657373506f"
    "6c6963793a312e300000000013000000446f6d61696e416363657373506f"
    "6c69637900";
    _marshaller_SecurityAdmin_DomainAccessPolicy = new _Marshaller_SecurityAdmin_DomainAccessPolicy;
    SecurityAdmin::_tc_AuditPolicy = 
    "010000000e00000044000000010000002a00000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f4175646974506f6c6963793a312e"
    "300000000c0000004175646974506f6c69637900";
    _marshaller_SecurityAdmin_AuditPolicy = new _Marshaller_SecurityAdmin_AuditPolicy;
    SecurityAdmin::_tc_AuditTargetPolicy = 
    "010000000e0000004e000000010000003000000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f4175646974546172676574506f6c"
    "6963793a312e3000120000004175646974546172676574506f6c69637900"
    ;
    _marshaller_SecurityAdmin_AuditTargetPolicy = new _Marshaller_SecurityAdmin_AuditTargetPolicy;
    SecurityAdmin::_tc_AuditClientPolicy = 
    "010000000e0000004e000000010000003000000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f4175646974436c69656e74506f6c"
    "6963793a312e3000120000004175646974436c69656e74506f6c69637900"
    ;
    _marshaller_SecurityAdmin_AuditClientPolicy = new _Marshaller_SecurityAdmin_AuditClientPolicy;
    SecurityAdmin::_tc_SecureInvocationPolicy = 
    "010000000e0000005b000000010000003500000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f536563757265496e766f63617469"
    "6f6e506f6c6963793a312e300000000017000000536563757265496e766f"
    "636174696f6e506f6c69637900";
    _marshaller_SecurityAdmin_SecureInvocationPolicy = new _Marshaller_SecurityAdmin_SecureInvocationPolicy;
    SecurityAdmin::_tc_DelegationPolicy = 
    "010000000e0000004d000000010000002f00000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f44656c65676174696f6e506f6c69"
    "63793a312e3000001100000044656c65676174696f6e506f6c69637900";
    _marshaller_SecurityAdmin_DelegationPolicy = new _Marshaller_SecurityAdmin_DelegationPolicy;
    SecurityAdmin::_tc_AuditTimeInterval = 
    "010000000f00000070000000010000003000000049444c3a6f6d672e6f72"
    "672f536563757269747941646d696e2f417564697454696d65496e746572"
    "76616c3a312e300012000000417564697454696d65496e74657276616c00"
    "00000200000006000000626567696e0000000300000004000000656e6400"
    "03000000";
    _marshaller_SecurityAdmin_AuditTimeInterval = new _Marshaller_SecurityAdmin_AuditTimeInterval;
  }

  ~__tc_init_SECURITYADMIN()
  {
    delete static_cast<_Marshaller_SecurityAdmin_AccessPolicy*>(_marshaller_SecurityAdmin_AccessPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_ObjectAccessPolicy*>(_marshaller_SecurityAdmin_ObjectAccessPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_DomainAccessPolicy*>(_marshaller_SecurityAdmin_DomainAccessPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_AuditPolicy*>(_marshaller_SecurityAdmin_AuditPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_AuditTargetPolicy*>(_marshaller_SecurityAdmin_AuditTargetPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_AuditClientPolicy*>(_marshaller_SecurityAdmin_AuditClientPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_SecureInvocationPolicy*>(_marshaller_SecurityAdmin_SecureInvocationPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_DelegationPolicy*>(_marshaller_SecurityAdmin_DelegationPolicy);
    delete static_cast<_Marshaller_SecurityAdmin_AuditTimeInterval*>(_marshaller_SecurityAdmin_AuditTimeInterval);
  }
};

static __tc_init_SECURITYADMIN __init_SECURITYADMIN;

