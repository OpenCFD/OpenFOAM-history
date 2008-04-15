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
namespace Security
{
CORBA::TypeCodeConst _tc_SecurityName;
}

namespace Security
{
CORBA::TypeCodeConst _tc_Opaque;
}

namespace Security
{
CORBA::TypeCodeConst _tc_ExtensibleFamily;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::ExtensibleFamily::ExtensibleFamily()
{
}

Security::ExtensibleFamily::ExtensibleFamily( const ExtensibleFamily& _s )
{
  family_definer = ((ExtensibleFamily&)_s).family_definer;
  family = ((ExtensibleFamily&)_s).family;
}

Security::ExtensibleFamily::~ExtensibleFamily()
{
}

Security::ExtensibleFamily&
Security::ExtensibleFamily::operator=( const ExtensibleFamily& _s )
{
  family_definer = ((ExtensibleFamily&)_s).family_definer;
  family = ((ExtensibleFamily&)_s).family;
  return *this;
}
#endif

class _Marshaller_Security_ExtensibleFamily : public ::CORBA::StaticTypeInfo {
    typedef Security::ExtensibleFamily _MICO_T;
  public:
    ~_Marshaller_Security_ExtensibleFamily();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_ExtensibleFamily::~_Marshaller_Security_ExtensibleFamily()
{
}

::CORBA::StaticValueType _Marshaller_Security_ExtensibleFamily::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_ExtensibleFamily::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_ExtensibleFamily::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_ExtensibleFamily::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->family_definer ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->family ) &&
    dc.struct_end();
}

void _Marshaller_Security_ExtensibleFamily::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->family_definer );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->family );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_ExtensibleFamily::typecode()
{
  return Security::_tc_ExtensibleFamily;
}

::CORBA::StaticTypeInfo *_marshaller_Security_ExtensibleFamily;

void operator<<=( CORBA::Any &_a, const Security::ExtensibleFamily &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_ExtensibleFamily, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::ExtensibleFamily *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::ExtensibleFamily &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_ExtensibleFamily, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::ExtensibleFamily *&_s )
{
  return _a.to_static_any (_marshaller_Security_ExtensibleFamily, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityAttributeType;
}

namespace Security
{
CORBA::TypeCodeConst _tc_AttributeType;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::AttributeType::AttributeType()
{
}

Security::AttributeType::AttributeType( const AttributeType& _s )
{
  attribute_family = ((AttributeType&)_s).attribute_family;
  attribute_type = ((AttributeType&)_s).attribute_type;
}

Security::AttributeType::~AttributeType()
{
}

Security::AttributeType&
Security::AttributeType::operator=( const AttributeType& _s )
{
  attribute_family = ((AttributeType&)_s).attribute_family;
  attribute_type = ((AttributeType&)_s).attribute_type;
  return *this;
}
#endif

class _Marshaller_Security_AttributeType : public ::CORBA::StaticTypeInfo {
    typedef Security::AttributeType _MICO_T;
  public:
    ~_Marshaller_Security_AttributeType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_AttributeType::~_Marshaller_Security_AttributeType()
{
}

::CORBA::StaticValueType _Marshaller_Security_AttributeType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_AttributeType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_AttributeType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_AttributeType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_Security_ExtensibleFamily->demarshal( dc, &((_MICO_T*)v)->attribute_family ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->attribute_type ) &&
    dc.struct_end();
}

void _Marshaller_Security_AttributeType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_Security_ExtensibleFamily->marshal( ec, &((_MICO_T*)v)->attribute_family );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->attribute_type );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_AttributeType::typecode()
{
  return Security::_tc_AttributeType;
}

::CORBA::StaticTypeInfo *_marshaller_Security_AttributeType;

void operator<<=( CORBA::Any &_a, const Security::AttributeType &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_AttributeType, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::AttributeType *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::AttributeType &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_AttributeType, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::AttributeType *&_s )
{
  return _a.to_static_any (_marshaller_Security_AttributeType, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_AttributeTypeList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SecAttribute;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::SecAttribute::SecAttribute()
{
}

Security::SecAttribute::SecAttribute( const SecAttribute& _s )
{
  attribute_type = ((SecAttribute&)_s).attribute_type;
  defining_authority = ((SecAttribute&)_s).defining_authority;
  value = ((SecAttribute&)_s).value;
}

Security::SecAttribute::~SecAttribute()
{
}

Security::SecAttribute&
Security::SecAttribute::operator=( const SecAttribute& _s )
{
  attribute_type = ((SecAttribute&)_s).attribute_type;
  defining_authority = ((SecAttribute&)_s).defining_authority;
  value = ((SecAttribute&)_s).value;
  return *this;
}
#endif

class _Marshaller_Security_SecAttribute : public ::CORBA::StaticTypeInfo {
    typedef Security::SecAttribute _MICO_T;
  public:
    ~_Marshaller_Security_SecAttribute();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SecAttribute::~_Marshaller_Security_SecAttribute()
{
}

::CORBA::StaticValueType _Marshaller_Security_SecAttribute::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SecAttribute::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SecAttribute::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SecAttribute::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_Security_AttributeType->demarshal( dc, &((_MICO_T*)v)->attribute_type ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->defining_authority ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_Security_SecAttribute::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_Security_AttributeType->marshal( ec, &((_MICO_T*)v)->attribute_type );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->defining_authority );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_SecAttribute::typecode()
{
  return Security::_tc_SecAttribute;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SecAttribute;

void operator<<=( CORBA::Any &_a, const Security::SecAttribute &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecAttribute, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::SecAttribute *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SecAttribute &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecAttribute, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::SecAttribute *&_s )
{
  return _a.to_static_any (_marshaller_Security_SecAttribute, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_AttributeList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_AuthenticationStatus;
}

void operator<<=( CORBA::Any &_a, const Security::AuthenticationStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuthenticationStatus, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::AuthenticationStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuthenticationStatus, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_AuthenticationStatus : public ::CORBA::StaticTypeInfo {
    typedef Security::AuthenticationStatus _MICO_T;
  public:
    ~_Marshaller_Security_AuthenticationStatus();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_AuthenticationStatus::~_Marshaller_Security_AuthenticationStatus()
{
}

::CORBA::StaticValueType _Marshaller_Security_AuthenticationStatus::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_AuthenticationStatus::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_AuthenticationStatus::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_AuthenticationStatus::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_AuthenticationStatus::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_AuthenticationStatus::typecode()
{
  return Security::_tc_AuthenticationStatus;
}

::CORBA::StaticTypeInfo *_marshaller_Security_AuthenticationStatus;

namespace Security
{
CORBA::TypeCodeConst _tc_AssociationStatus;
}

void operator<<=( CORBA::Any &_a, const Security::AssociationStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AssociationStatus, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::AssociationStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AssociationStatus, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_AssociationStatus : public ::CORBA::StaticTypeInfo {
    typedef Security::AssociationStatus _MICO_T;
  public:
    ~_Marshaller_Security_AssociationStatus();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_AssociationStatus::~_Marshaller_Security_AssociationStatus()
{
}

::CORBA::StaticValueType _Marshaller_Security_AssociationStatus::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_AssociationStatus::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_AssociationStatus::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_AssociationStatus::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_AssociationStatus::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_AssociationStatus::typecode()
{
  return Security::_tc_AssociationStatus;
}

::CORBA::StaticTypeInfo *_marshaller_Security_AssociationStatus;

namespace Security
{
CORBA::TypeCodeConst _tc_AuthenticationMethod;
}

namespace Security
{
CORBA::TypeCodeConst _tc_AuthenticationMethodList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_CredentialsType;
}

void operator<<=( CORBA::Any &_a, const Security::CredentialsType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_CredentialsType, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::CredentialsType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_CredentialsType, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_CredentialsType : public ::CORBA::StaticTypeInfo {
    typedef Security::CredentialsType _MICO_T;
  public:
    ~_Marshaller_Security_CredentialsType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_CredentialsType::~_Marshaller_Security_CredentialsType()
{
}

::CORBA::StaticValueType _Marshaller_Security_CredentialsType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_CredentialsType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_CredentialsType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_CredentialsType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_CredentialsType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_CredentialsType::typecode()
{
  return Security::_tc_CredentialsType;
}

::CORBA::StaticTypeInfo *_marshaller_Security_CredentialsType;

namespace Security
{
CORBA::TypeCodeConst _tc_ChannelBindings;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::ChannelBindings::ChannelBindings()
{
}

Security::ChannelBindings::ChannelBindings( const ChannelBindings& _s )
{
  initiator_addrtype = ((ChannelBindings&)_s).initiator_addrtype;
  initiator_address = ((ChannelBindings&)_s).initiator_address;
  acceptor_addrtype = ((ChannelBindings&)_s).acceptor_addrtype;
  acceptor_address = ((ChannelBindings&)_s).acceptor_address;
  application_data = ((ChannelBindings&)_s).application_data;
}

Security::ChannelBindings::~ChannelBindings()
{
}

Security::ChannelBindings&
Security::ChannelBindings::operator=( const ChannelBindings& _s )
{
  initiator_addrtype = ((ChannelBindings&)_s).initiator_addrtype;
  initiator_address = ((ChannelBindings&)_s).initiator_address;
  acceptor_addrtype = ((ChannelBindings&)_s).acceptor_addrtype;
  acceptor_address = ((ChannelBindings&)_s).acceptor_address;
  application_data = ((ChannelBindings&)_s).application_data;
  return *this;
}
#endif

class _Marshaller_Security_ChannelBindings : public ::CORBA::StaticTypeInfo {
    typedef Security::ChannelBindings _MICO_T;
  public:
    ~_Marshaller_Security_ChannelBindings();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_ChannelBindings::~_Marshaller_Security_ChannelBindings()
{
}

::CORBA::StaticValueType _Marshaller_Security_ChannelBindings::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_ChannelBindings::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_ChannelBindings::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_ChannelBindings::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->initiator_addrtype ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->initiator_address ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->acceptor_addrtype ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->acceptor_address ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->application_data ) &&
    dc.struct_end();
}

void _Marshaller_Security_ChannelBindings::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->initiator_addrtype );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->initiator_address );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->acceptor_addrtype );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->acceptor_address );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->application_data );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_ChannelBindings::typecode()
{
  return Security::_tc_ChannelBindings;
}

::CORBA::StaticTypeInfo *_marshaller_Security_ChannelBindings;

void operator<<=( CORBA::Any &_a, const Security::ChannelBindings &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_ChannelBindings, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::ChannelBindings *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::ChannelBindings &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_ChannelBindings, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::ChannelBindings *&_s )
{
  return _a.to_static_any (_marshaller_Security_ChannelBindings, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_Right;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::Right::Right()
{
}

Security::Right::Right( const Right& _s )
{
  rights_family = ((Right&)_s).rights_family;
  rights_list = ((Right&)_s).rights_list;
}

Security::Right::~Right()
{
}

Security::Right&
Security::Right::operator=( const Right& _s )
{
  rights_family = ((Right&)_s).rights_family;
  rights_list = ((Right&)_s).rights_list;
  return *this;
}
#endif

class _Marshaller_Security_Right : public ::CORBA::StaticTypeInfo {
    typedef Security::Right _MICO_T;
  public:
    ~_Marshaller_Security_Right();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_Right::~_Marshaller_Security_Right()
{
}

::CORBA::StaticValueType _Marshaller_Security_Right::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_Right::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_Right::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_Right::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_Security_ExtensibleFamily->demarshal( dc, &((_MICO_T*)v)->rights_family ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->rights_list._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_Security_Right::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_Security_ExtensibleFamily->marshal( ec, &((_MICO_T*)v)->rights_family );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->rights_list.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_Right::typecode()
{
  return Security::_tc_Right;
}

::CORBA::StaticTypeInfo *_marshaller_Security_Right;

void operator<<=( CORBA::Any &_a, const Security::Right &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_Right, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::Right *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::Right &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_Right, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::Right *&_s )
{
  return _a.to_static_any (_marshaller_Security_Right, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_RightsList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_RightsCombinator;
}

void operator<<=( CORBA::Any &_a, const Security::RightsCombinator &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_RightsCombinator, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::RightsCombinator &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_RightsCombinator, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_RightsCombinator : public ::CORBA::StaticTypeInfo {
    typedef Security::RightsCombinator _MICO_T;
  public:
    ~_Marshaller_Security_RightsCombinator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_RightsCombinator::~_Marshaller_Security_RightsCombinator()
{
}

::CORBA::StaticValueType _Marshaller_Security_RightsCombinator::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_RightsCombinator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_RightsCombinator::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_RightsCombinator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_RightsCombinator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_RightsCombinator::typecode()
{
  return Security::_tc_RightsCombinator;
}

::CORBA::StaticTypeInfo *_marshaller_Security_RightsCombinator;

namespace Security
{
CORBA::TypeCodeConst _tc_DelegationState;
}

void operator<<=( CORBA::Any &_a, const Security::DelegationState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationState, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::DelegationState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationState, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_DelegationState : public ::CORBA::StaticTypeInfo {
    typedef Security::DelegationState _MICO_T;
  public:
    ~_Marshaller_Security_DelegationState();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_DelegationState::~_Marshaller_Security_DelegationState()
{
}

::CORBA::StaticValueType _Marshaller_Security_DelegationState::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_DelegationState::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_DelegationState::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_DelegationState::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_DelegationState::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_DelegationState::typecode()
{
  return Security::_tc_DelegationState;
}

::CORBA::StaticTypeInfo *_marshaller_Security_DelegationState;

namespace Security
{
CORBA::TypeCodeConst _tc_DelegationDirective;
}

void operator<<=( CORBA::Any &_a, const Security::DelegationDirective &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationDirective, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::DelegationDirective &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationDirective, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_DelegationDirective : public ::CORBA::StaticTypeInfo {
    typedef Security::DelegationDirective _MICO_T;
  public:
    ~_Marshaller_Security_DelegationDirective();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_DelegationDirective::~_Marshaller_Security_DelegationDirective()
{
}

::CORBA::StaticValueType _Marshaller_Security_DelegationDirective::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_DelegationDirective::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_DelegationDirective::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_DelegationDirective::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_DelegationDirective::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_DelegationDirective::typecode()
{
  return Security::_tc_DelegationDirective;
}

::CORBA::StaticTypeInfo *_marshaller_Security_DelegationDirective;

namespace Security
{
CORBA::TypeCodeConst _tc_UtcT;
}

namespace Security
{
CORBA::TypeCodeConst _tc_IntervalT;
}

namespace Security
{
CORBA::TypeCodeConst _tc_TimeT;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityFeature;
}

void operator<<=( CORBA::Any &_a, const Security::SecurityFeature &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityFeature, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SecurityFeature &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityFeature, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_SecurityFeature : public ::CORBA::StaticTypeInfo {
    typedef Security::SecurityFeature _MICO_T;
  public:
    ~_Marshaller_Security_SecurityFeature();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SecurityFeature::~_Marshaller_Security_SecurityFeature()
{
}

::CORBA::StaticValueType _Marshaller_Security_SecurityFeature::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SecurityFeature::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SecurityFeature::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SecurityFeature::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_SecurityFeature::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_SecurityFeature::typecode()
{
  return Security::_tc_SecurityFeature;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SecurityFeature;

namespace Security
{
CORBA::TypeCodeConst _tc_QOP;
}

void operator<<=( CORBA::Any &_a, const Security::QOP &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_QOP, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::QOP &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_QOP, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_QOP : public ::CORBA::StaticTypeInfo {
    typedef Security::QOP _MICO_T;
  public:
    ~_Marshaller_Security_QOP();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_QOP::~_Marshaller_Security_QOP()
{
}

::CORBA::StaticValueType _Marshaller_Security_QOP::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_QOP::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_QOP::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_QOP::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_QOP::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_QOP::typecode()
{
  return Security::_tc_QOP;
}

::CORBA::StaticTypeInfo *_marshaller_Security_QOP;

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityContextType;
}

void operator<<=( CORBA::Any &_a, const Security::SecurityContextType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityContextType, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SecurityContextType &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityContextType, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_SecurityContextType : public ::CORBA::StaticTypeInfo {
    typedef Security::SecurityContextType _MICO_T;
  public:
    ~_Marshaller_Security_SecurityContextType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SecurityContextType::~_Marshaller_Security_SecurityContextType()
{
}

::CORBA::StaticValueType _Marshaller_Security_SecurityContextType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SecurityContextType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SecurityContextType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SecurityContextType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_SecurityContextType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_SecurityContextType::typecode()
{
  return Security::_tc_SecurityContextType;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SecurityContextType;

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityContextState;
}

void operator<<=( CORBA::Any &_a, const Security::SecurityContextState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityContextState, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SecurityContextState &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityContextState, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_SecurityContextState : public ::CORBA::StaticTypeInfo {
    typedef Security::SecurityContextState _MICO_T;
  public:
    ~_Marshaller_Security_SecurityContextState();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SecurityContextState::~_Marshaller_Security_SecurityContextState()
{
}

::CORBA::StaticValueType _Marshaller_Security_SecurityContextState::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SecurityContextState::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SecurityContextState::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SecurityContextState::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_SecurityContextState::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_SecurityContextState::typecode()
{
  return Security::_tc_SecurityContextState;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SecurityContextState;

namespace Security
{
CORBA::TypeCodeConst _tc_OpaqueBuffer;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::OpaqueBuffer::OpaqueBuffer()
{
}

Security::OpaqueBuffer::OpaqueBuffer( const OpaqueBuffer& _s )
{
  buffer = ((OpaqueBuffer&)_s).buffer;
  startpos = ((OpaqueBuffer&)_s).startpos;
  endpos = ((OpaqueBuffer&)_s).endpos;
}

Security::OpaqueBuffer::~OpaqueBuffer()
{
}

Security::OpaqueBuffer&
Security::OpaqueBuffer::operator=( const OpaqueBuffer& _s )
{
  buffer = ((OpaqueBuffer&)_s).buffer;
  startpos = ((OpaqueBuffer&)_s).startpos;
  endpos = ((OpaqueBuffer&)_s).endpos;
  return *this;
}
#endif

class _Marshaller_Security_OpaqueBuffer : public ::CORBA::StaticTypeInfo {
    typedef Security::OpaqueBuffer _MICO_T;
  public:
    ~_Marshaller_Security_OpaqueBuffer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_OpaqueBuffer::~_Marshaller_Security_OpaqueBuffer()
{
}

::CORBA::StaticValueType _Marshaller_Security_OpaqueBuffer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_OpaqueBuffer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_OpaqueBuffer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_OpaqueBuffer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->buffer ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->startpos ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->endpos ) &&
    dc.struct_end();
}

void _Marshaller_Security_OpaqueBuffer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->buffer );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->startpos );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->endpos );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_OpaqueBuffer::typecode()
{
  return Security::_tc_OpaqueBuffer;
}

::CORBA::StaticTypeInfo *_marshaller_Security_OpaqueBuffer;

void operator<<=( CORBA::Any &_a, const Security::OpaqueBuffer &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_OpaqueBuffer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::OpaqueBuffer *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::OpaqueBuffer &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_OpaqueBuffer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::OpaqueBuffer *&_s )
{
  return _a.to_static_any (_marshaller_Security_OpaqueBuffer, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_AssociationOptions;
}

namespace Security
{
CORBA::TypeCodeConst _tc_RequiresSupports;
}

void operator<<=( CORBA::Any &_a, const Security::RequiresSupports &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_RequiresSupports, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::RequiresSupports &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_RequiresSupports, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_RequiresSupports : public ::CORBA::StaticTypeInfo {
    typedef Security::RequiresSupports _MICO_T;
  public:
    ~_Marshaller_Security_RequiresSupports();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_RequiresSupports::~_Marshaller_Security_RequiresSupports()
{
}

::CORBA::StaticValueType _Marshaller_Security_RequiresSupports::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_RequiresSupports::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_RequiresSupports::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_RequiresSupports::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_RequiresSupports::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_RequiresSupports::typecode()
{
  return Security::_tc_RequiresSupports;
}

::CORBA::StaticTypeInfo *_marshaller_Security_RequiresSupports;

namespace Security
{
CORBA::TypeCodeConst _tc_CommunicationDirection;
}

void operator<<=( CORBA::Any &_a, const Security::CommunicationDirection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_CommunicationDirection, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::CommunicationDirection &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_CommunicationDirection, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_CommunicationDirection : public ::CORBA::StaticTypeInfo {
    typedef Security::CommunicationDirection _MICO_T;
  public:
    ~_Marshaller_Security_CommunicationDirection();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_CommunicationDirection::~_Marshaller_Security_CommunicationDirection()
{
}

::CORBA::StaticValueType _Marshaller_Security_CommunicationDirection::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_CommunicationDirection::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_CommunicationDirection::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_CommunicationDirection::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_CommunicationDirection::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_CommunicationDirection::typecode()
{
  return Security::_tc_CommunicationDirection;
}

::CORBA::StaticTypeInfo *_marshaller_Security_CommunicationDirection;

namespace Security
{
CORBA::TypeCodeConst _tc_OptionsDirectionPair;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::OptionsDirectionPair::OptionsDirectionPair()
{
}

Security::OptionsDirectionPair::OptionsDirectionPair( const OptionsDirectionPair& _s )
{
  options = ((OptionsDirectionPair&)_s).options;
  direction = ((OptionsDirectionPair&)_s).direction;
}

Security::OptionsDirectionPair::~OptionsDirectionPair()
{
}

Security::OptionsDirectionPair&
Security::OptionsDirectionPair::operator=( const OptionsDirectionPair& _s )
{
  options = ((OptionsDirectionPair&)_s).options;
  direction = ((OptionsDirectionPair&)_s).direction;
  return *this;
}
#endif

class _Marshaller_Security_OptionsDirectionPair : public ::CORBA::StaticTypeInfo {
    typedef Security::OptionsDirectionPair _MICO_T;
  public:
    ~_Marshaller_Security_OptionsDirectionPair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_OptionsDirectionPair::~_Marshaller_Security_OptionsDirectionPair()
{
}

::CORBA::StaticValueType _Marshaller_Security_OptionsDirectionPair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_OptionsDirectionPair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_OptionsDirectionPair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_OptionsDirectionPair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->options ) &&
    _marshaller_Security_CommunicationDirection->demarshal( dc, &((_MICO_T*)v)->direction ) &&
    dc.struct_end();
}

void _Marshaller_Security_OptionsDirectionPair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->options );
  _marshaller_Security_CommunicationDirection->marshal( ec, &((_MICO_T*)v)->direction );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_OptionsDirectionPair::typecode()
{
  return Security::_tc_OptionsDirectionPair;
}

::CORBA::StaticTypeInfo *_marshaller_Security_OptionsDirectionPair;

void operator<<=( CORBA::Any &_a, const Security::OptionsDirectionPair &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_OptionsDirectionPair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::OptionsDirectionPair *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::OptionsDirectionPair &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_OptionsDirectionPair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::OptionsDirectionPair *&_s )
{
  return _a.to_static_any (_marshaller_Security_OptionsDirectionPair, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_OptionsDirectionPairList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_DelegationMode;
}

void operator<<=( CORBA::Any &_a, const Security::DelegationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationMode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::DelegationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DelegationMode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_DelegationMode : public ::CORBA::StaticTypeInfo {
    typedef Security::DelegationMode _MICO_T;
  public:
    ~_Marshaller_Security_DelegationMode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_DelegationMode::~_Marshaller_Security_DelegationMode()
{
}

::CORBA::StaticValueType _Marshaller_Security_DelegationMode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_DelegationMode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_DelegationMode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_DelegationMode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_DelegationMode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_DelegationMode::typecode()
{
  return Security::_tc_DelegationMode;
}

::CORBA::StaticTypeInfo *_marshaller_Security_DelegationMode;

namespace Security
{
CORBA::TypeCodeConst _tc_MechanismType;
}

namespace Security
{
CORBA::TypeCodeConst _tc_MechanismTypeList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityMechanismData;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::SecurityMechanismData::SecurityMechanismData()
{
}

Security::SecurityMechanismData::SecurityMechanismData( const SecurityMechanismData& _s )
{
  mechanism = ((SecurityMechanismData&)_s).mechanism;
  security_name = ((SecurityMechanismData&)_s).security_name;
  options_supported = ((SecurityMechanismData&)_s).options_supported;
  options_required = ((SecurityMechanismData&)_s).options_required;
}

Security::SecurityMechanismData::~SecurityMechanismData()
{
}

Security::SecurityMechanismData&
Security::SecurityMechanismData::operator=( const SecurityMechanismData& _s )
{
  mechanism = ((SecurityMechanismData&)_s).mechanism;
  security_name = ((SecurityMechanismData&)_s).security_name;
  options_supported = ((SecurityMechanismData&)_s).options_supported;
  options_required = ((SecurityMechanismData&)_s).options_required;
  return *this;
}
#endif

class _Marshaller_Security_SecurityMechanismData : public ::CORBA::StaticTypeInfo {
    typedef Security::SecurityMechanismData _MICO_T;
  public:
    ~_Marshaller_Security_SecurityMechanismData();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SecurityMechanismData::~_Marshaller_Security_SecurityMechanismData()
{
}

::CORBA::StaticValueType _Marshaller_Security_SecurityMechanismData::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SecurityMechanismData::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SecurityMechanismData::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SecurityMechanismData::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->mechanism._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->security_name._for_demarshal() ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->options_supported ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->options_required ) &&
    dc.struct_end();
}

void _Marshaller_Security_SecurityMechanismData::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->mechanism.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->security_name.inout() );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->options_supported );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->options_required );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_SecurityMechanismData::typecode()
{
  return Security::_tc_SecurityMechanismData;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SecurityMechanismData;

void operator<<=( CORBA::Any &_a, const Security::SecurityMechanismData &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityMechanismData, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::SecurityMechanismData *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SecurityMechanismData &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SecurityMechanismData, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::SecurityMechanismData *&_s )
{
  return _a.to_static_any (_marshaller_Security_SecurityMechanismData, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_SecurityMechanismDataList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_MechandOptions;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::MechandOptions::MechandOptions()
{
}

Security::MechandOptions::MechandOptions( const MechandOptions& _s )
{
  mechanism_type = ((MechandOptions&)_s).mechanism_type;
  options_supported = ((MechandOptions&)_s).options_supported;
}

Security::MechandOptions::~MechandOptions()
{
}

Security::MechandOptions&
Security::MechandOptions::operator=( const MechandOptions& _s )
{
  mechanism_type = ((MechandOptions&)_s).mechanism_type;
  options_supported = ((MechandOptions&)_s).options_supported;
  return *this;
}
#endif

class _Marshaller_Security_MechandOptions : public ::CORBA::StaticTypeInfo {
    typedef Security::MechandOptions _MICO_T;
  public:
    ~_Marshaller_Security_MechandOptions();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_MechandOptions::~_Marshaller_Security_MechandOptions()
{
}

::CORBA::StaticValueType _Marshaller_Security_MechandOptions::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_MechandOptions::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_MechandOptions::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_MechandOptions::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->mechanism_type._for_demarshal() ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->options_supported ) &&
    dc.struct_end();
}

void _Marshaller_Security_MechandOptions::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->mechanism_type.inout() );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->options_supported );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_MechandOptions::typecode()
{
  return Security::_tc_MechandOptions;
}

::CORBA::StaticTypeInfo *_marshaller_Security_MechandOptions;

void operator<<=( CORBA::Any &_a, const Security::MechandOptions &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_MechandOptions, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::MechandOptions *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::MechandOptions &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_MechandOptions, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::MechandOptions *&_s )
{
  return _a.to_static_any (_marshaller_Security_MechandOptions, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_MechandOptionsList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_EstablishTrust;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::EstablishTrust::EstablishTrust()
{
}

Security::EstablishTrust::EstablishTrust( const EstablishTrust& _s )
{
  trust_in_client = ((EstablishTrust&)_s).trust_in_client;
  trust_in_target = ((EstablishTrust&)_s).trust_in_target;
}

Security::EstablishTrust::~EstablishTrust()
{
}

Security::EstablishTrust&
Security::EstablishTrust::operator=( const EstablishTrust& _s )
{
  trust_in_client = ((EstablishTrust&)_s).trust_in_client;
  trust_in_target = ((EstablishTrust&)_s).trust_in_target;
  return *this;
}
#endif

class _Marshaller_Security_EstablishTrust : public ::CORBA::StaticTypeInfo {
    typedef Security::EstablishTrust _MICO_T;
  public:
    ~_Marshaller_Security_EstablishTrust();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_EstablishTrust::~_Marshaller_Security_EstablishTrust()
{
}

::CORBA::StaticValueType _Marshaller_Security_EstablishTrust::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_EstablishTrust::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_EstablishTrust::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_EstablishTrust::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->trust_in_client ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->trust_in_target ) &&
    dc.struct_end();
}

void _Marshaller_Security_EstablishTrust::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->trust_in_client );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->trust_in_target );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_EstablishTrust::typecode()
{
  return Security::_tc_EstablishTrust;
}

::CORBA::StaticTypeInfo *_marshaller_Security_EstablishTrust;

void operator<<=( CORBA::Any &_a, const Security::EstablishTrust &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_EstablishTrust, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::EstablishTrust *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::EstablishTrust &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_EstablishTrust, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::EstablishTrust *&_s )
{
  return _a.to_static_any (_marshaller_Security_EstablishTrust, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_AuditChannelId;
}

namespace Security
{
CORBA::TypeCodeConst _tc_EventType;
}

namespace Security
{
CORBA::TypeCodeConst _tc_DayOfTheWeek;
}

void operator<<=( CORBA::Any &_a, const Security::DayOfTheWeek &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DayOfTheWeek, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::DayOfTheWeek &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_DayOfTheWeek, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_DayOfTheWeek : public ::CORBA::StaticTypeInfo {
    typedef Security::DayOfTheWeek _MICO_T;
  public:
    ~_Marshaller_Security_DayOfTheWeek();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_DayOfTheWeek::~_Marshaller_Security_DayOfTheWeek()
{
}

::CORBA::StaticValueType _Marshaller_Security_DayOfTheWeek::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_DayOfTheWeek::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_DayOfTheWeek::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_DayOfTheWeek::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_DayOfTheWeek::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_DayOfTheWeek::typecode()
{
  return Security::_tc_DayOfTheWeek;
}

::CORBA::StaticTypeInfo *_marshaller_Security_DayOfTheWeek;

namespace Security
{
CORBA::TypeCodeConst _tc_AuditCombinator;
}

void operator<<=( CORBA::Any &_a, const Security::AuditCombinator &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuditCombinator, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::AuditCombinator &_e )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuditCombinator, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_Security_AuditCombinator : public ::CORBA::StaticTypeInfo {
    typedef Security::AuditCombinator _MICO_T;
  public:
    ~_Marshaller_Security_AuditCombinator();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_AuditCombinator::~_Marshaller_Security_AuditCombinator()
{
}

::CORBA::StaticValueType _Marshaller_Security_AuditCombinator::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_AuditCombinator::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_AuditCombinator::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_AuditCombinator::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_Security_AuditCombinator::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_Security_AuditCombinator::typecode()
{
  return Security::_tc_AuditCombinator;
}

::CORBA::StaticTypeInfo *_marshaller_Security_AuditCombinator;

namespace Security
{
CORBA::TypeCodeConst _tc_AuditEventType;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::AuditEventType::AuditEventType()
{
}

Security::AuditEventType::AuditEventType( const AuditEventType& _s )
{
  event_family = ((AuditEventType&)_s).event_family;
  event_type = ((AuditEventType&)_s).event_type;
}

Security::AuditEventType::~AuditEventType()
{
}

Security::AuditEventType&
Security::AuditEventType::operator=( const AuditEventType& _s )
{
  event_family = ((AuditEventType&)_s).event_family;
  event_type = ((AuditEventType&)_s).event_type;
  return *this;
}
#endif

class _Marshaller_Security_AuditEventType : public ::CORBA::StaticTypeInfo {
    typedef Security::AuditEventType _MICO_T;
  public:
    ~_Marshaller_Security_AuditEventType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_AuditEventType::~_Marshaller_Security_AuditEventType()
{
}

::CORBA::StaticValueType _Marshaller_Security_AuditEventType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_AuditEventType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_AuditEventType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_AuditEventType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_Security_ExtensibleFamily->demarshal( dc, &((_MICO_T*)v)->event_family ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->event_type ) &&
    dc.struct_end();
}

void _Marshaller_Security_AuditEventType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_Security_ExtensibleFamily->marshal( ec, &((_MICO_T*)v)->event_family );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->event_type );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_AuditEventType::typecode()
{
  return Security::_tc_AuditEventType;
}

::CORBA::StaticTypeInfo *_marshaller_Security_AuditEventType;

void operator<<=( CORBA::Any &_a, const Security::AuditEventType &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuditEventType, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::AuditEventType *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::AuditEventType &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_AuditEventType, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::AuditEventType *&_s )
{
  return _a.to_static_any (_marshaller_Security_AuditEventType, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_AuditEventTypeList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SelectorType;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SelectorValue;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::SelectorValue::SelectorValue()
{
}

Security::SelectorValue::SelectorValue( const SelectorValue& _s )
{
  selector = ((SelectorValue&)_s).selector;
  value = ((SelectorValue&)_s).value;
}

Security::SelectorValue::~SelectorValue()
{
}

Security::SelectorValue&
Security::SelectorValue::operator=( const SelectorValue& _s )
{
  selector = ((SelectorValue&)_s).selector;
  value = ((SelectorValue&)_s).value;
  return *this;
}
#endif

class _Marshaller_Security_SelectorValue : public ::CORBA::StaticTypeInfo {
    typedef Security::SelectorValue _MICO_T;
  public:
    ~_Marshaller_Security_SelectorValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SelectorValue::~_Marshaller_Security_SelectorValue()
{
}

::CORBA::StaticValueType _Marshaller_Security_SelectorValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SelectorValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SelectorValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SelectorValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->selector ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_Security_SelectorValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->selector );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_SelectorValue::typecode()
{
  return Security::_tc_SelectorValue;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SelectorValue;

void operator<<=( CORBA::Any &_a, const Security::SelectorValue &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SelectorValue, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::SelectorValue *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SelectorValue &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SelectorValue, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::SelectorValue *&_s )
{
  return _a.to_static_any (_marshaller_Security_SelectorValue, (void *&)_s);
}

namespace Security
{
CORBA::TypeCodeConst _tc_SelectorValueList;
}

namespace Security
{
CORBA::TypeCodeConst _tc_SSLKeyCertCAPass;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
Security::SSLKeyCertCAPass::SSLKeyCertCAPass()
{
}

Security::SSLKeyCertCAPass::SSLKeyCertCAPass( const SSLKeyCertCAPass& _s )
{
  key = ((SSLKeyCertCAPass&)_s).key;
  cert = ((SSLKeyCertCAPass&)_s).cert;
  CAfile = ((SSLKeyCertCAPass&)_s).CAfile;
  CAdir = ((SSLKeyCertCAPass&)_s).CAdir;
  pass = ((SSLKeyCertCAPass&)_s).pass;
}

Security::SSLKeyCertCAPass::~SSLKeyCertCAPass()
{
}

Security::SSLKeyCertCAPass&
Security::SSLKeyCertCAPass::operator=( const SSLKeyCertCAPass& _s )
{
  key = ((SSLKeyCertCAPass&)_s).key;
  cert = ((SSLKeyCertCAPass&)_s).cert;
  CAfile = ((SSLKeyCertCAPass&)_s).CAfile;
  CAdir = ((SSLKeyCertCAPass&)_s).CAdir;
  pass = ((SSLKeyCertCAPass&)_s).pass;
  return *this;
}
#endif

class _Marshaller_Security_SSLKeyCertCAPass : public ::CORBA::StaticTypeInfo {
    typedef Security::SSLKeyCertCAPass _MICO_T;
  public:
    ~_Marshaller_Security_SSLKeyCertCAPass();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_Security_SSLKeyCertCAPass::~_Marshaller_Security_SSLKeyCertCAPass()
{
}

::CORBA::StaticValueType _Marshaller_Security_SSLKeyCertCAPass::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_Security_SSLKeyCertCAPass::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_Security_SSLKeyCertCAPass::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_Security_SSLKeyCertCAPass::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->key._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->cert._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->CAfile._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->CAdir._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->pass._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_Security_SSLKeyCertCAPass::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->key.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->cert.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->CAfile.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->CAdir.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->pass.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_Security_SSLKeyCertCAPass::typecode()
{
  return Security::_tc_SSLKeyCertCAPass;
}

::CORBA::StaticTypeInfo *_marshaller_Security_SSLKeyCertCAPass;

void operator<<=( CORBA::Any &_a, const Security::SSLKeyCertCAPass &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SSLKeyCertCAPass, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, Security::SSLKeyCertCAPass *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, Security::SSLKeyCertCAPass &_s )
{
  CORBA::StaticAny _sa (_marshaller_Security_SSLKeyCertCAPass, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const Security::SSLKeyCertCAPass *&_s )
{
  return _a.to_static_any (_marshaller_Security_SSLKeyCertCAPass, (void *&)_s);
}

class _Marshaller__seq_Security_AttributeType : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::AttributeType,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_AttributeType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_AttributeType::~_Marshaller__seq_Security_AttributeType()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_AttributeType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_AttributeType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_AttributeType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_AttributeType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_AttributeType->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_AttributeType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_AttributeType->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_AttributeType::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000064010000010000000f00000054010000010000002700"
    "000049444c3a6f6d672e6f72672f53656375726974792f41747472696275"
    "7465547970653a312e3000000e0000004174747269627574655479706500"
    "000002000000110000006174747269627574655f66616d696c7900000000"
    "0f00000078000000010000002a00000049444c3a6f6d672e6f72672f5365"
    "6375726974792f457874656e7369626c6546616d696c793a312e30000000"
    "11000000457874656e7369626c6546616d696c7900000000020000000f00"
    "000066616d696c795f646566696e65720000040000000700000066616d69"
    "6c790000040000000f0000006174747269627574655f7479706500001500"
    "000058000000010000002f00000049444c3a6f6d672e6f72672f53656375"
    "726974792f5365637572697479417474726962757465547970653a312e30"
    "000016000000536563757269747941747472696275746554797065000000"
    "0500000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_AttributeType::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_AttributeType;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::AttributeType,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_AttributeType, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::AttributeType,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::AttributeType,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_AttributeType, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::AttributeType,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_AttributeType, (void *&)_s);
}


class _Marshaller__seq_Security_SecAttribute : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_SecAttribute();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_SecAttribute::~_Marshaller__seq_Security_SecAttribute()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_SecAttribute::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_SecAttribute::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_SecAttribute::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_SecAttribute::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_SecAttribute->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_SecAttribute::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_SecAttribute->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SecAttribute::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000080020000010000000f00000070020000010000002600"
    "000049444c3a6f6d672e6f72672f53656375726974792f53656341747472"
    "69627574653a312e300000000d0000005365634174747269627574650000"
    "0000030000000f0000006174747269627574655f7479706500000f000000"
    "54010000010000002700000049444c3a6f6d672e6f72672f536563757269"
    "74792f417474726962757465547970653a312e3000000e00000041747472"
    "69627574655479706500000002000000110000006174747269627574655f"
    "66616d696c79000000000f00000078000000010000002a00000049444c3a"
    "6f6d672e6f72672f53656375726974792f457874656e7369626c6546616d"
    "696c793a312e3000000011000000457874656e7369626c6546616d696c79"
    "00000000020000000f00000066616d696c795f646566696e657200000400"
    "00000700000066616d696c790000040000000f0000006174747269627574"
    "655f7479706500001500000058000000010000002f00000049444c3a6f6d"
    "672e6f72672f53656375726974792f536563757269747941747472696275"
    "7465547970653a312e300000160000005365637572697479417474726962"
    "757465547970650000000500000013000000646566696e696e675f617574"
    "686f726974790000150000003c000000010000001800000049444c3a6f6d"
    "672e6f72672f4353492f4f49443a312e3000040000004f49440013000000"
    "0c000000010000000a000000000000000600000076616c75650000001500"
    "000048000000010000002000000049444c3a6f6d672e6f72672f53656375"
    "726974792f4f70617175653a312e3000070000004f706171756500001300"
    "00000c000000010000000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SecAttribute::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_SecAttribute;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SecAttribute, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SecAttribute, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_SecAttribute, (void *&)_s);
}


class _Marshaller__seq_Security_Right : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::Right,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_Right();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_Right::~_Marshaller__seq_Security_Right()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_Right::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_Right::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_Right::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_Right::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_Right->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_Right::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_Right->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_Right::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000f4000000010000000f000000e4000000010000001f00"
    "000049444c3a6f6d672e6f72672f53656375726974792f52696768743a31"
    "2e300000060000005269676874000000020000000e000000726967687473"
    "5f66616d696c790000000f00000078000000010000002a00000049444c3a"
    "6f6d672e6f72672f53656375726974792f457874656e7369626c6546616d"
    "696c793a312e3000000011000000457874656e7369626c6546616d696c79"
    "00000000020000000f00000066616d696c795f646566696e657200000400"
    "00000700000066616d696c790000040000000c0000007269676874735f6c"
    "69737400120000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_Right::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_Right;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::Right,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_Right, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::Right,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::Right,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_Right, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::Right,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_Right, (void *&)_s);
}


class _Marshaller__seq_Security_OptionsDirectionPair : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_OptionsDirectionPair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_OptionsDirectionPair::~_Marshaller__seq_Security_OptionsDirectionPair()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_OptionsDirectionPair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_OptionsDirectionPair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_OptionsDirectionPair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_OptionsDirectionPair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_OptionsDirectionPair->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_OptionsDirectionPair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_OptionsDirectionPair->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_OptionsDirectionPair::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000084010000010000000f00000072010000010000002e00"
    "000049444c3a6f6d672e6f72672f53656375726974792f4f7074696f6e73"
    "446972656374696f6e506169723a312e30000000150000004f7074696f6e"
    "73446972656374696f6e506169720000000002000000080000006f707469"
    "6f6e73001500000050000000010000002c00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4173736f63696174696f6e4f7074696f6e733a"
    "312e3000130000004173736f63696174696f6e4f7074696f6e7300000400"
    "00000a000000646972656374696f6e000000110000009e00000001000000"
    "3000000049444c3a6f6d672e6f72672f53656375726974792f436f6d6d75"
    "6e69636174696f6e446972656374696f6e3a312e300017000000436f6d6d"
    "756e69636174696f6e446972656374696f6e000003000000110000005365"
    "63446972656374696f6e426f746800000000140000005365634469726563"
    "74696f6e526571756573740012000000536563446972656374696f6e5265"
    "706c7900000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_OptionsDirectionPair::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_OptionsDirectionPair;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_OptionsDirectionPair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_OptionsDirectionPair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_OptionsDirectionPair, (void *&)_s);
}


class _Marshaller__seq_Security_SecurityMechanismData : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_SecurityMechanismData();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_SecurityMechanismData::~_Marshaller__seq_Security_SecurityMechanismData()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_SecurityMechanismData::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_SecurityMechanismData::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_SecurityMechanismData::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_SecurityMechanismData::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_SecurityMechanismData->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_SecurityMechanismData::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_SecurityMechanismData->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SecurityMechanismData::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000014020000010000000f00000004020000010000002f00"
    "000049444c3a6f6d672e6f72672f53656375726974792f53656375726974"
    "794d656368616e69736d446174613a312e30000016000000536563757269"
    "74794d656368616e69736d44617461000000040000000a0000006d656368"
    "616e69736d000000150000004c000000010000002700000049444c3a6f6d"
    "672e6f72672f53656375726974792f4d656368616e69736d547970653a31"
    "2e3000000e0000004d656368616e69736d54797065000000120000000000"
    "00000e00000073656375726974795f6e616d65000000150000004c000000"
    "010000002600000049444c3a6f6d672e6f72672f53656375726974792f53"
    "656375726974794e616d653a312e300000000d0000005365637572697479"
    "4e616d65000000001200000000000000120000006f7074696f6e735f7375"
    "70706f727465640000001500000050000000010000002c00000049444c3a"
    "6f6d672e6f72672f53656375726974792f4173736f63696174696f6e4f70"
    "74696f6e733a312e3000130000004173736f63696174696f6e4f7074696f"
    "6e73000004000000110000006f7074696f6e735f72657175697265640000"
    "00001500000050000000010000002c00000049444c3a6f6d672e6f72672f"
    "53656375726974792f4173736f63696174696f6e4f7074696f6e733a312e"
    "3000130000004173736f63696174696f6e4f7074696f6e73000004000000"
    "00000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SecurityMechanismData::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_SecurityMechanismData;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SecurityMechanismData, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SecurityMechanismData, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_SecurityMechanismData, (void *&)_s);
}


class _Marshaller__seq_Security_MechandOptions : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_MechandOptions();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_MechandOptions::~_Marshaller__seq_Security_MechandOptions()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_MechandOptions::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_MechandOptions::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_MechandOptions::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_MechandOptions::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_MechandOptions->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_MechandOptions::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_MechandOptions->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_MechandOptions::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000030010000010000000f00000020010000010000002800"
    "000049444c3a6f6d672e6f72672f53656375726974792f4d656368616e64"
    "4f7074696f6e733a312e30000f0000004d656368616e644f7074696f6e73"
    "0000020000000f0000006d656368616e69736d5f74797065000015000000"
    "4c000000010000002700000049444c3a6f6d672e6f72672f536563757269"
    "74792f4d656368616e69736d547970653a312e3000000e0000004d656368"
    "616e69736d547970650000001200000000000000120000006f7074696f6e"
    "735f737570706f727465640000001500000050000000010000002c000000"
    "49444c3a6f6d672e6f72672f53656375726974792f4173736f6369617469"
    "6f6e4f7074696f6e733a312e3000130000004173736f63696174696f6e4f"
    "7074696f6e7300000400000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_MechandOptions::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_MechandOptions;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_MechandOptions, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_MechandOptions, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_MechandOptions, (void *&)_s);
}


class _Marshaller__seq_Security_AuditEventType : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_AuditEventType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_AuditEventType::~_Marshaller__seq_Security_AuditEventType()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_AuditEventType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_AuditEventType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_AuditEventType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_AuditEventType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_AuditEventType->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_AuditEventType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_AuditEventType->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_AuditEventType::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000000010000010000000f000000f0000000010000002800"
    "000049444c3a6f6d672e6f72672f53656375726974792f41756469744576"
    "656e74547970653a312e30000f00000041756469744576656e7454797065"
    "0000020000000d0000006576656e745f66616d696c79000000000f000000"
    "78000000010000002a00000049444c3a6f6d672e6f72672f536563757269"
    "74792f457874656e7369626c6546616d696c793a312e3000000011000000"
    "457874656e7369626c6546616d696c7900000000020000000f0000006661"
    "6d696c795f646566696e65720000040000000700000066616d696c790000"
    "040000000b0000006576656e745f7479706500000400000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_AuditEventType::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_AuditEventType;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_AuditEventType, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_AuditEventType, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_AuditEventType, (void *&)_s);
}


class _Marshaller__seq_Security_SelectorValue : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_Security_SelectorValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_Security_SelectorValue::~_Marshaller__seq_Security_SelectorValue()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_Security_SelectorValue::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_Security_SelectorValue::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_Security_SelectorValue::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_Security_SelectorValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_Security_SelectorValue->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_Security_SelectorValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_Security_SelectorValue->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SelectorValue::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000c8000000010000000f000000b8000000010000002700"
    "000049444c3a6f6d672e6f72672f53656375726974792f53656c6563746f"
    "7256616c75653a312e3000000e00000053656c6563746f7256616c756500"
    "0000020000000900000073656c6563746f72000000001500000048000000"
    "010000002600000049444c3a6f6d672e6f72672f53656375726974792f53"
    "656c6563746f72547970653a312e300000000d00000053656c6563746f72"
    "5479706500000000050000000600000076616c75650000000b0000000000"
    "0000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_Security_SelectorValue::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_Security_SelectorValue;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SelectorValue, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_Security_SelectorValue, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_Security_SelectorValue, (void *&)_s);
}


struct __tc_init_SECURITY {
  __tc_init_SECURITY()
  {
    Security::_tc_SecurityName = 
    "01000000150000004c000000010000002600000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656375726974794e616d653a312e30000000"
    "0d00000053656375726974794e616d65000000001200000000000000";
    Security::_tc_Opaque = 
    "010000001500000048000000010000002000000049444c3a6f6d672e6f72"
    "672f53656375726974792f4f70617175653a312e3000070000004f706171"
    "75650000130000000c000000010000000a00000000000000";
    Security::_tc_ExtensibleFamily = 
    "010000000f00000078000000010000002a00000049444c3a6f6d672e6f72"
    "672f53656375726974792f457874656e7369626c6546616d696c793a312e"
    "3000000011000000457874656e7369626c6546616d696c79000000000200"
    "00000f00000066616d696c795f646566696e657200000400000007000000"
    "66616d696c79000004000000";
    _marshaller_Security_ExtensibleFamily = new _Marshaller_Security_ExtensibleFamily;
    Security::_tc_SecurityAttributeType = 
    "010000001500000058000000010000002f00000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656375726974794174747269627574655479"
    "70653a312e30000016000000536563757269747941747472696275746554"
    "79706500000005000000";
    Security::_tc_AttributeType = 
    "010000000f00000054010000010000002700000049444c3a6f6d672e6f72"
    "672f53656375726974792f417474726962757465547970653a312e300000"
    "0e0000004174747269627574655479706500000002000000110000006174"
    "747269627574655f66616d696c79000000000f0000007800000001000000"
    "2a00000049444c3a6f6d672e6f72672f53656375726974792f457874656e"
    "7369626c6546616d696c793a312e3000000011000000457874656e736962"
    "6c6546616d696c7900000000020000000f00000066616d696c795f646566"
    "696e65720000040000000700000066616d696c790000040000000f000000"
    "6174747269627574655f7479706500001500000058000000010000002f00"
    "000049444c3a6f6d672e6f72672f53656375726974792f53656375726974"
    "79417474726962757465547970653a312e30000016000000536563757269"
    "74794174747269627574655479706500000005000000";
    _marshaller_Security_AttributeType = new _Marshaller_Security_AttributeType;
    Security::_tc_AttributeTypeList = 
    "0100000015000000b8010000010000002b00000049444c3a6f6d672e6f72"
    "672f53656375726974792f417474726962757465547970654c6973743a31"
    "2e30000012000000417474726962757465547970654c6973740000001300"
    "000064010000010000000f00000054010000010000002700000049444c3a"
    "6f6d672e6f72672f53656375726974792f41747472696275746554797065"
    "3a312e3000000e0000004174747269627574655479706500000002000000"
    "110000006174747269627574655f66616d696c79000000000f0000007800"
    "0000010000002a00000049444c3a6f6d672e6f72672f5365637572697479"
    "2f457874656e7369626c6546616d696c793a312e30000000110000004578"
    "74656e7369626c6546616d696c7900000000020000000f00000066616d69"
    "6c795f646566696e65720000040000000700000066616d696c7900000400"
    "00000f0000006174747269627574655f7479706500001500000058000000"
    "010000002f00000049444c3a6f6d672e6f72672f53656375726974792f53"
    "65637572697479417474726962757465547970653a312e30000016000000"
    "536563757269747941747472696275746554797065000000050000000000"
    "0000";
    Security::_tc_SecAttribute = 
    "010000000f00000070020000010000002600000049444c3a6f6d672e6f72"
    "672f53656375726974792f5365634174747269627574653a312e30000000"
    "0d00000053656341747472696275746500000000030000000f0000006174"
    "747269627574655f7479706500000f000000540100000100000027000000"
    "49444c3a6f6d672e6f72672f53656375726974792f417474726962757465"
    "547970653a312e3000000e00000041747472696275746554797065000000"
    "02000000110000006174747269627574655f66616d696c79000000000f00"
    "000078000000010000002a00000049444c3a6f6d672e6f72672f53656375"
    "726974792f457874656e7369626c6546616d696c793a312e300000001100"
    "0000457874656e7369626c6546616d696c7900000000020000000f000000"
    "66616d696c795f646566696e65720000040000000700000066616d696c79"
    "0000040000000f0000006174747269627574655f74797065000015000000"
    "58000000010000002f00000049444c3a6f6d672e6f72672f536563757269"
    "74792f5365637572697479417474726962757465547970653a312e300000"
    "160000005365637572697479417474726962757465547970650000000500"
    "000013000000646566696e696e675f617574686f72697479000015000000"
    "3c000000010000001800000049444c3a6f6d672e6f72672f4353492f4f49"
    "443a312e3000040000004f494400130000000c000000010000000a000000"
    "000000000600000076616c75650000001500000048000000010000002000"
    "000049444c3a6f6d672e6f72672f53656375726974792f4f70617175653a"
    "312e3000070000004f70617175650000130000000c000000010000000a00"
    "000000000000";
    _marshaller_Security_SecAttribute = new _Marshaller_Security_SecAttribute;
    Security::_tc_AttributeList = 
    "0100000015000000cc020000010000002700000049444c3a6f6d672e6f72"
    "672f53656375726974792f4174747269627574654c6973743a312e300000"
    "0e0000004174747269627574654c69737400000013000000800200000100"
    "00000f00000070020000010000002600000049444c3a6f6d672e6f72672f"
    "53656375726974792f5365634174747269627574653a312e300000000d00"
    "000053656341747472696275746500000000030000000f00000061747472"
    "69627574655f7479706500000f0000005401000001000000270000004944"
    "4c3a6f6d672e6f72672f53656375726974792f4174747269627574655479"
    "70653a312e3000000e000000417474726962757465547970650000000200"
    "0000110000006174747269627574655f66616d696c79000000000f000000"
    "78000000010000002a00000049444c3a6f6d672e6f72672f536563757269"
    "74792f457874656e7369626c6546616d696c793a312e3000000011000000"
    "457874656e7369626c6546616d696c7900000000020000000f0000006661"
    "6d696c795f646566696e65720000040000000700000066616d696c790000"
    "040000000f0000006174747269627574655f747970650000150000005800"
    "0000010000002f00000049444c3a6f6d672e6f72672f5365637572697479"
    "2f5365637572697479417474726962757465547970653a312e3000001600"
    "000053656375726974794174747269627574655479706500000005000000"
    "13000000646566696e696e675f617574686f726974790000150000003c00"
    "0000010000001800000049444c3a6f6d672e6f72672f4353492f4f49443a"
    "312e3000040000004f494400130000000c000000010000000a0000000000"
    "00000600000076616c756500000015000000480000000100000020000000"
    "49444c3a6f6d672e6f72672f53656375726974792f4f70617175653a312e"
    "3000070000004f70617175650000130000000c000000010000000a000000"
    "0000000000000000";
    Security::_tc_AuthenticationStatus = 
    "0100000011000000a7000000010000002e00000049444c3a6f6d672e6f72"
    "672f53656375726974792f41757468656e7469636174696f6e5374617475"
    "733a312e300000001500000041757468656e7469636174696f6e53746174"
    "757300000000040000000f00000053656341757468537563636573730000"
    "0f000000536563417574684661696c757265000010000000536563417574"
    "68436f6e74696e7565000f000000536563417574684578706972656400";
    _marshaller_Security_AuthenticationStatus = new _Marshaller_Security_AuthenticationStatus;
    Security::_tc_AssociationStatus = 
    "01000000110000008d000000010000002b00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4173736f63696174696f6e5374617475733a31"
    "2e300000120000004173736f63696174696f6e5374617475730000000300"
    "0000100000005365634173736f6353756363657373001000000053656341"
    "73736f634661696c75726500110000005365634173736f63436f6e74696e"
    "756500";
    _marshaller_Security_AssociationStatus = new _Marshaller_Security_AssociationStatus;
    Security::_tc_AuthenticationMethod = 
    "010000001500000058000000010000002e00000049444c3a6f6d672e6f72"
    "672f53656375726974792f41757468656e7469636174696f6e4d6574686f"
    "643a312e300000001500000041757468656e7469636174696f6e4d657468"
    "6f640000000005000000";
    Security::_tc_AuthenticationMethodList = 
    "0100000015000000cc000000010000003200000049444c3a6f6d672e6f72"
    "672f53656375726974792f41757468656e7469636174696f6e4d6574686f"
    "644c6973743a312e300000001900000041757468656e7469636174696f6e"
    "4d6574686f644c6973740000000013000000680000000100000015000000"
    "58000000010000002e00000049444c3a6f6d672e6f72672f536563757269"
    "74792f41757468656e7469636174696f6e4d6574686f643a312e30000000"
    "1500000041757468656e7469636174696f6e4d6574686f64000000000500"
    "000000000000";
    Security::_tc_CredentialsType = 
    "010000001100000099000000010000002900000049444c3a6f6d672e6f72"
    "672f53656375726974792f43726564656e7469616c73547970653a312e30"
    "000000001000000043726564656e7469616c735479706500030000001200"
    "00005365634f776e43726564656e7469616c730000001700000053656352"
    "6563656976656443726564656e7469616c73000015000000536563546172"
    "67657443726564656e7469616c7300";
    _marshaller_Security_CredentialsType = new _Marshaller_Security_CredentialsType;
    Security::_tc_ChannelBindings = 
    "010000000f00000008010000010000002900000049444c3a6f6d672e6f72"
    "672f53656375726974792f4368616e6e656c42696e64696e67733a312e30"
    "00000000100000004368616e6e656c42696e64696e677300050000001300"
    "0000696e69746961746f725f616464727479706500000500000012000000"
    "696e69746961746f725f61646472657373000000130000000c0000000100"
    "00000a00000000000000120000006163636570746f725f61646472747970"
    "6500000005000000110000006163636570746f725f616464726573730000"
    "0000130000000c000000010000000a00000000000000110000006170706c"
    "69636174696f6e5f6461746100000000130000000c000000010000000a00"
    "000000000000";
    _marshaller_Security_ChannelBindings = new _Marshaller_Security_ChannelBindings;
    Security::_tc_Right = 
    "010000000f000000e4000000010000001f00000049444c3a6f6d672e6f72"
    "672f53656375726974792f52696768743a312e3000000600000052696768"
    "74000000020000000e0000007269676874735f66616d696c790000000f00"
    "000078000000010000002a00000049444c3a6f6d672e6f72672f53656375"
    "726974792f457874656e7369626c6546616d696c793a312e300000001100"
    "0000457874656e7369626c6546616d696c7900000000020000000f000000"
    "66616d696c795f646566696e65720000040000000700000066616d696c79"
    "0000040000000c0000007269676874735f6c697374001200000000000000"
    ;
    _marshaller_Security_Right = new _Marshaller_Security_Right;
    Security::_tc_RightsList = 
    "010000001500000038010000010000002400000049444c3a6f6d672e6f72"
    "672f53656375726974792f5269676874734c6973743a312e30000b000000"
    "5269676874734c697374000013000000f4000000010000000f000000e400"
    "0000010000001f00000049444c3a6f6d672e6f72672f5365637572697479"
    "2f52696768743a312e300000060000005269676874000000020000000e00"
    "00007269676874735f66616d696c790000000f0000007800000001000000"
    "2a00000049444c3a6f6d672e6f72672f53656375726974792f457874656e"
    "7369626c6546616d696c793a312e3000000011000000457874656e736962"
    "6c6546616d696c7900000000020000000f00000066616d696c795f646566"
    "696e65720000040000000700000066616d696c790000040000000c000000"
    "7269676874735f6c69737400120000000000000000000000";
    Security::_tc_RightsCombinator = 
    "010000001100000074000000010000002a00000049444c3a6f6d672e6f72"
    "672f53656375726974792f526967687473436f6d62696e61746f723a312e"
    "3000000011000000526967687473436f6d62696e61746f72000000000200"
    "00000d000000536563416c6c526967687473000000000c00000053656341"
    "6e79526967687400";
    _marshaller_Security_RightsCombinator = new _Marshaller_Security_RightsCombinator;
    Security::_tc_DelegationState = 
    "010000001100000070000000010000002900000049444c3a6f6d672e6f72"
    "672f53656375726974792f44656c65676174696f6e53746174653a312e30"
    "000000001000000044656c65676174696f6e537461746500020000000d00"
    "0000536563496e69746961746f72000000000c00000053656344656c6567"
    "61746500";
    _marshaller_Security_DelegationState = new _Marshaller_Security_DelegationState;
    Security::_tc_DelegationDirective = 
    "010000001100000073000000010000002d00000049444c3a6f6d672e6f72"
    "672f53656375726974792f44656c65676174696f6e446972656374697665"
    "3a312e30000000001400000044656c65676174696f6e4469726563746976"
    "6500020000000900000044656c6567617465000000000b0000004e6f4465"
    "6c656761746500";
    _marshaller_Security_DelegationDirective = new _Marshaller_Security_DelegationDirective;
    Security::_tc_UtcT = 
    "010000001500000028010000010000001e00000049444c3a6f6d672e6f72"
    "672f53656375726974792f557463543a312e300000000500000055746354"
    "000000000f000000ec000000010000001e00000049444c3a6f6d672e6f72"
    "672f54696d65426173652f557463543a312e300000000500000055746354"
    "00000000040000000500000074696d650000000015000000380000000100"
    "00001f00000049444c3a6f6d672e6f72672f54696d65426173652f54696d"
    "65543a312e3000000600000054696d65540000001800000008000000696e"
    "6163636c6f000500000008000000696e6163636869000400000004000000"
    "746466001500000038000000010000001e00000049444c3a6f6d672e6f72"
    "672f54696d65426173652f546466543a312e300000000500000054646654"
    "0000000002000000";
    Security::_tc_IntervalT = 
    "010000001500000024010000010000002300000049444c3a6f6d672e6f72"
    "672f53656375726974792f496e74657276616c543a312e3000000a000000"
    "496e74657276616c540000000f000000e000000001000000230000004944"
    "4c3a6f6d672e6f72672f54696d65426173652f496e74657276616c543a31"
    "2e3000000a000000496e74657276616c54000000020000000c0000006c6f"
    "7765725f626f756e64001500000038000000010000001f00000049444c3a"
    "6f6d672e6f72672f54696d65426173652f54696d65543a312e3000000600"
    "000054696d6554000000180000000c00000075707065725f626f756e6400"
    "1500000038000000010000001f00000049444c3a6f6d672e6f72672f5469"
    "6d65426173652f54696d65543a312e3000000600000054696d6554000000"
    "18000000";
    Security::_tc_TimeT = 
    "010000001500000074000000010000001f00000049444c3a6f6d672e6f72"
    "672f53656375726974792f54696d65543a312e3000000600000054696d65"
    "540000001500000038000000010000001f00000049444c3a6f6d672e6f72"
    "672f54696d65426173652f54696d65543a312e3000000600000054696d65"
    "5400000018000000";
    Security::_tc_SecurityFeature = 
    "010000001100000066010000010000002900000049444c3a6f6d672e6f72"
    "672f53656375726974792f5365637572697479466561747572653a312e30"
    "0000000010000000536563757269747946656174757265000b0000001000"
    "00005365634e6f44656c65676174696f6e001400000053656353696d706c"
    "6544656c65676174696f6e0017000000536563436f6d706f736974654465"
    "6c65676174696f6e0000100000005365634e6f50726f74656374696f6e00"
    "0d000000536563496e746567726974790000000013000000536563436f6e"
    "666964656e7469616c69747900001f000000536563496e74656772697479"
    "416e64436f6e666964656e7469616c697479000010000000536563446574"
    "6563745265706c617900150000005365634465746563744d69736f726465"
    "72696e67000000001a00000053656345737461626c697368547275737449"
    "6e5461726765740000001a00000053656345737461626c69736854727573"
    "74496e436c69656e7400";
    _marshaller_Security_SecurityFeature = new _Marshaller_Security_SecurityFeature;
    Security::_tc_QOP = 
    "0100000011000000a2000000010000001d00000049444c3a6f6d672e6f72"
    "672f53656375726974792f514f503a312e300000000004000000514f5000"
    "0400000013000000536563514f504e6f50726f74656374696f6e00001000"
    "0000536563514f50496e746567726974790016000000536563514f50436f"
    "6e666964656e7469616c69747900000022000000536563514f50496e7465"
    "6772697479416e64436f6e666964656e7469616c69747900";
    _marshaller_Security_QOP = new _Marshaller_Security_QOP;
    Security::_tc_SecurityContextType = 
    "01000000110000008a000000010000002d00000049444c3a6f6d672e6f72"
    "672f53656375726974792f5365637572697479436f6e7465787454797065"
    "3a312e3000000000140000005365637572697479436f6e74657874547970"
    "65000200000016000000436c69656e745365637572697479436f6e746578"
    "74000000160000005365727665725365637572697479436f6e7465787400"
    ;
    _marshaller_Security_SecurityContextType = new _Marshaller_Security_SecurityContextType;
    Security::_tc_SecurityContextState = 
    "010000001100000016010000010000002e00000049444c3a6f6d672e6f72"
    "672f53656375726974792f5365637572697479436f6e7465787453746174"
    "653a312e30000000150000005365637572697479436f6e74657874537461"
    "7465000000000700000016000000536563436f6e74657874496e69746961"
    "6c697a656400000014000000536563436f6e74657874436f6e74696e7565"
    "64001c000000536563436f6e74657874436c69656e7445737461626c6973"
    "6865640016000000536563436f6e7465787445737461626c697368656400"
    "00001b000000536563436f6e7465787445737461626c6973684578706972"
    "6564000012000000536563436f6e74657874457870697265640000001200"
    "0000536563436f6e74657874496e76616c696400";
    _marshaller_Security_SecurityContextState = new _Marshaller_Security_SecurityContextState;
    Security::_tc_OpaqueBuffer = 
    "010000000f000000c8000000010000002600000049444c3a6f6d672e6f72"
    "672f53656375726974792f4f70617175654275666665723a312e30000000"
    "0d0000004f70617175654275666665720000000003000000070000006275"
    "6666657200001500000048000000010000002000000049444c3a6f6d672e"
    "6f72672f53656375726974792f4f70617175653a312e3000070000004f70"
    "617175650000130000000c000000010000000a0000000000000009000000"
    "7374617274706f73000000000500000007000000656e64706f7300000500"
    "0000";
    _marshaller_Security_OpaqueBuffer = new _Marshaller_Security_OpaqueBuffer;
    Security::_tc_AssociationOptions = 
    "010000001500000050000000010000002c00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4173736f63696174696f6e4f7074696f6e733a"
    "312e3000130000004173736f63696174696f6e4f7074696f6e7300000400"
    "0000";
    Security::_tc_RequiresSupports = 
    "010000001100000070000000010000002a00000049444c3a6f6d672e6f72"
    "672f53656375726974792f5265717569726573537570706f7274733a312e"
    "30000000110000005265717569726573537570706f727473000000000200"
    "00000c0000005365635265717569726573000c000000536563537570706f"
    "72747300";
    _marshaller_Security_RequiresSupports = new _Marshaller_Security_RequiresSupports;
    Security::_tc_CommunicationDirection = 
    "01000000110000009e000000010000003000000049444c3a6f6d672e6f72"
    "672f53656375726974792f436f6d6d756e69636174696f6e446972656374"
    "696f6e3a312e300017000000436f6d6d756e69636174696f6e4469726563"
    "74696f6e00000300000011000000536563446972656374696f6e426f7468"
    "0000000014000000536563446972656374696f6e52657175657374001200"
    "0000536563446972656374696f6e5265706c7900";
    _marshaller_Security_CommunicationDirection = new _Marshaller_Security_CommunicationDirection;
    Security::_tc_OptionsDirectionPair = 
    "010000000f00000072010000010000002e00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4f7074696f6e73446972656374696f6e506169"
    "723a312e30000000150000004f7074696f6e73446972656374696f6e5061"
    "69720000000002000000080000006f7074696f6e73001500000050000000"
    "010000002c00000049444c3a6f6d672e6f72672f53656375726974792f41"
    "73736f63696174696f6e4f7074696f6e733a312e3000130000004173736f"
    "63696174696f6e4f7074696f6e730000040000000a000000646972656374"
    "696f6e000000110000009e000000010000003000000049444c3a6f6d672e"
    "6f72672f53656375726974792f436f6d6d756e69636174696f6e44697265"
    "6374696f6e3a312e300017000000436f6d6d756e69636174696f6e446972"
    "656374696f6e00000300000011000000536563446972656374696f6e426f"
    "74680000000014000000536563446972656374696f6e5265717565737400"
    "12000000536563446972656374696f6e5265706c7900";
    _marshaller_Security_OptionsDirectionPair = new _Marshaller_Security_OptionsDirectionPair;
    Security::_tc_OptionsDirectionPairList = 
    "0100000015000000e8010000010000003200000049444c3a6f6d672e6f72"
    "672f53656375726974792f4f7074696f6e73446972656374696f6e506169"
    "724c6973743a312e30000000190000004f7074696f6e7344697265637469"
    "6f6e506169724c697374000000001300000084010000010000000f000000"
    "72010000010000002e00000049444c3a6f6d672e6f72672f536563757269"
    "74792f4f7074696f6e73446972656374696f6e506169723a312e30000000"
    "150000004f7074696f6e73446972656374696f6e50616972000000000200"
    "0000080000006f7074696f6e73001500000050000000010000002c000000"
    "49444c3a6f6d672e6f72672f53656375726974792f4173736f6369617469"
    "6f6e4f7074696f6e733a312e3000130000004173736f63696174696f6e4f"
    "7074696f6e730000040000000a000000646972656374696f6e0000001100"
    "00009e000000010000003000000049444c3a6f6d672e6f72672f53656375"
    "726974792f436f6d6d756e69636174696f6e446972656374696f6e3a312e"
    "300017000000436f6d6d756e69636174696f6e446972656374696f6e0000"
    "0300000011000000536563446972656374696f6e426f7468000000001400"
    "0000536563446972656374696f6e52657175657374001200000053656344"
    "6972656374696f6e5265706c7900000000000000";
    Security::_tc_DelegationMode = 
    "0100000011000000a6000000010000002800000049444c3a6f6d672e6f72"
    "672f53656375726974792f44656c65676174696f6e4d6f64653a312e3000"
    "0f00000044656c65676174696f6e4d6f6465000003000000170000005365"
    "6344656c4d6f64654e6f44656c65676174696f6e00001b00000053656344"
    "656c4d6f646553696d706c6544656c65676174696f6e00001e0000005365"
    "6344656c4d6f6465436f6d706f7369746544656c65676174696f6e00";
    _marshaller_Security_DelegationMode = new _Marshaller_Security_DelegationMode;
    Security::_tc_MechanismType = 
    "01000000150000004c000000010000002700000049444c3a6f6d672e6f72"
    "672f53656375726974792f4d656368616e69736d547970653a312e300000"
    "0e0000004d656368616e69736d547970650000001200000000000000";
    Security::_tc_MechanismTypeList = 
    "0100000015000000b0000000010000002b00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4d656368616e69736d547970654c6973743a31"
    "2e300000120000004d656368616e69736d547970654c6973740000001300"
    "00005c00000001000000150000004c000000010000002700000049444c3a"
    "6f6d672e6f72672f53656375726974792f4d656368616e69736d54797065"
    "3a312e3000000e0000004d656368616e69736d5479706500000012000000"
    "0000000000000000";
    Security::_tc_SecurityMechanismData = 
    "010000000f00000004020000010000002f00000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656375726974794d656368616e69736d4461"
    "74613a312e3000001600000053656375726974794d656368616e69736d44"
    "617461000000040000000a0000006d656368616e69736d00000015000000"
    "4c000000010000002700000049444c3a6f6d672e6f72672f536563757269"
    "74792f4d656368616e69736d547970653a312e3000000e0000004d656368"
    "616e69736d5479706500000012000000000000000e000000736563757269"
    "74795f6e616d65000000150000004c000000010000002600000049444c3a"
    "6f6d672e6f72672f53656375726974792f53656375726974794e616d653a"
    "312e300000000d00000053656375726974794e616d650000000012000000"
    "00000000120000006f7074696f6e735f737570706f727465640000001500"
    "000050000000010000002c00000049444c3a6f6d672e6f72672f53656375"
    "726974792f4173736f63696174696f6e4f7074696f6e733a312e30001300"
    "00004173736f63696174696f6e4f7074696f6e7300000400000011000000"
    "6f7074696f6e735f72657175697265640000000015000000500000000100"
    "00002c00000049444c3a6f6d672e6f72672f53656375726974792f417373"
    "6f63696174696f6e4f7074696f6e733a312e3000130000004173736f6369"
    "6174696f6e4f7074696f6e73000004000000";
    _marshaller_Security_SecurityMechanismData = new _Marshaller_Security_SecurityMechanismData;
    Security::_tc_SecurityMechanismDataList = 
    "010000001500000078020000010000003300000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656375726974794d656368616e69736d4461"
    "74614c6973743a312e3000001a00000053656375726974794d656368616e"
    "69736d446174614c6973740000001300000014020000010000000f000000"
    "04020000010000002f00000049444c3a6f6d672e6f72672f536563757269"
    "74792f53656375726974794d656368616e69736d446174613a312e300000"
    "1600000053656375726974794d656368616e69736d446174610000000400"
    "00000a0000006d656368616e69736d000000150000004c00000001000000"
    "2700000049444c3a6f6d672e6f72672f53656375726974792f4d65636861"
    "6e69736d547970653a312e3000000e0000004d656368616e69736d547970"
    "6500000012000000000000000e00000073656375726974795f6e616d6500"
    "0000150000004c000000010000002600000049444c3a6f6d672e6f72672f"
    "53656375726974792f53656375726974794e616d653a312e300000000d00"
    "000053656375726974794e616d6500000000120000000000000012000000"
    "6f7074696f6e735f737570706f7274656400000015000000500000000100"
    "00002c00000049444c3a6f6d672e6f72672f53656375726974792f417373"
    "6f63696174696f6e4f7074696f6e733a312e3000130000004173736f6369"
    "6174696f6e4f7074696f6e73000004000000110000006f7074696f6e735f"
    "7265717569726564000000001500000050000000010000002c0000004944"
    "4c3a6f6d672e6f72672f53656375726974792f4173736f63696174696f6e"
    "4f7074696f6e733a312e3000130000004173736f63696174696f6e4f7074"
    "696f6e7300000400000000000000";
    Security::_tc_MechandOptions = 
    "010000000f00000020010000010000002800000049444c3a6f6d672e6f72"
    "672f53656375726974792f4d656368616e644f7074696f6e733a312e3000"
    "0f0000004d656368616e644f7074696f6e730000020000000f0000006d65"
    "6368616e69736d5f747970650000150000004c0000000100000027000000"
    "49444c3a6f6d672e6f72672f53656375726974792f4d656368616e69736d"
    "547970653a312e3000000e0000004d656368616e69736d54797065000000"
    "1200000000000000120000006f7074696f6e735f737570706f7274656400"
    "00001500000050000000010000002c00000049444c3a6f6d672e6f72672f"
    "53656375726974792f4173736f63696174696f6e4f7074696f6e733a312e"
    "3000130000004173736f63696174696f6e4f7074696f6e73000004000000"
    ;
    _marshaller_Security_MechandOptions = new _Marshaller_Security_MechandOptions;
    Security::_tc_MechandOptionsList = 
    "010000001500000084010000010000002c00000049444c3a6f6d672e6f72"
    "672f53656375726974792f4d656368616e644f7074696f6e734c6973743a"
    "312e3000130000004d656368616e644f7074696f6e734c69737400001300"
    "000030010000010000000f00000020010000010000002800000049444c3a"
    "6f6d672e6f72672f53656375726974792f4d656368616e644f7074696f6e"
    "733a312e30000f0000004d656368616e644f7074696f6e73000002000000"
    "0f0000006d656368616e69736d5f747970650000150000004c0000000100"
    "00002700000049444c3a6f6d672e6f72672f53656375726974792f4d6563"
    "68616e69736d547970653a312e3000000e0000004d656368616e69736d54"
    "7970650000001200000000000000120000006f7074696f6e735f73757070"
    "6f727465640000001500000050000000010000002c00000049444c3a6f6d"
    "672e6f72672f53656375726974792f4173736f63696174696f6e4f707469"
    "6f6e733a312e3000130000004173736f63696174696f6e4f7074696f6e73"
    "00000400000000000000";
    Security::_tc_EstablishTrust = 
    "010000000f00000078000000010000002800000049444c3a6f6d672e6f72"
    "672f53656375726974792f45737461626c69736854727573743a312e3000"
    "0f00000045737461626c6973685472757374000002000000100000007472"
    "7573745f696e5f636c69656e7400080000001000000074727573745f696e"
    "5f7461726765740008000000";
    _marshaller_Security_EstablishTrust = new _Marshaller_Security_EstablishTrust;
    Security::_tc_AuditChannelId = 
    "010000001500000048000000010000002800000049444c3a6f6d672e6f72"
    "672f53656375726974792f41756469744368616e6e656c49643a312e3000"
    "0f00000041756469744368616e6e656c4964000005000000";
    Security::_tc_EventType = 
    "010000001500000040000000010000002300000049444c3a6f6d672e6f72"
    "672f53656375726974792f4576656e74547970653a312e3000000a000000"
    "4576656e745479706500000004000000";
    Security::_tc_DayOfTheWeek = 
    "0100000011000000a7000000010000002600000049444c3a6f6d672e6f72"
    "672f53656375726974792f4461794f665468655765656b3a312e30000000"
    "0d0000004461794f665468655765656b0000000007000000070000004d6f"
    "6e64617900000800000054756573646179000a0000005765646e65736461"
    "790000000900000054687572736461790000000007000000467269646179"
    "0000090000005361747572646179000000000700000053756e64617900";
    _marshaller_Security_DayOfTheWeek = new _Marshaller_Security_DayOfTheWeek;
    Security::_tc_AuditCombinator = 
    "010000001100000073000000010000002900000049444c3a6f6d672e6f72"
    "672f53656375726974792f4175646974436f6d62696e61746f723a312e30"
    "00000000100000004175646974436f6d62696e61746f7200020000001000"
    "0000536563416c6c53656c6563746f7273000f000000536563416e795365"
    "6c6563746f7200";
    _marshaller_Security_AuditCombinator = new _Marshaller_Security_AuditCombinator;
    Security::_tc_AuditEventType = 
    "010000000f000000f0000000010000002800000049444c3a6f6d672e6f72"
    "672f53656375726974792f41756469744576656e74547970653a312e3000"
    "0f00000041756469744576656e74547970650000020000000d0000006576"
    "656e745f66616d696c79000000000f00000078000000010000002a000000"
    "49444c3a6f6d672e6f72672f53656375726974792f457874656e7369626c"
    "6546616d696c793a312e3000000011000000457874656e7369626c654661"
    "6d696c7900000000020000000f00000066616d696c795f646566696e6572"
    "0000040000000700000066616d696c790000040000000b0000006576656e"
    "745f74797065000004000000";
    _marshaller_Security_AuditEventType = new _Marshaller_Security_AuditEventType;
    Security::_tc_AuditEventTypeList = 
    "010000001500000054010000010000002c00000049444c3a6f6d672e6f72"
    "672f53656375726974792f41756469744576656e74547970654c6973743a"
    "312e30001300000041756469744576656e74547970654c69737400001300"
    "000000010000010000000f000000f0000000010000002800000049444c3a"
    "6f6d672e6f72672f53656375726974792f41756469744576656e74547970"
    "653a312e30000f00000041756469744576656e7454797065000002000000"
    "0d0000006576656e745f66616d696c79000000000f000000780000000100"
    "00002a00000049444c3a6f6d672e6f72672f53656375726974792f457874"
    "656e7369626c6546616d696c793a312e3000000011000000457874656e73"
    "69626c6546616d696c7900000000020000000f00000066616d696c795f64"
    "6566696e65720000040000000700000066616d696c790000040000000b00"
    "00006576656e745f7479706500000400000000000000";
    Security::_tc_SelectorType = 
    "010000001500000048000000010000002600000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656c6563746f72547970653a312e30000000"
    "0d00000053656c6563746f72547970650000000005000000";
    Security::_tc_SelectorValue = 
    "010000000f000000b8000000010000002700000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656c6563746f7256616c75653a312e300000"
    "0e00000053656c6563746f7256616c756500000002000000090000007365"
    "6c6563746f72000000001500000048000000010000002600000049444c3a"
    "6f6d672e6f72672f53656375726974792f53656c6563746f72547970653a"
    "312e300000000d00000053656c6563746f72547970650000000005000000"
    "0600000076616c75650000000b000000";
    _marshaller_Security_SelectorValue = new _Marshaller_Security_SelectorValue;
    Security::_tc_SelectorValueList = 
    "01000000150000001c010000010000002b00000049444c3a6f6d672e6f72"
    "672f53656375726974792f53656c6563746f7256616c75654c6973743a31"
    "2e3000001200000053656c6563746f7256616c75654c6973740000001300"
    "0000c8000000010000000f000000b8000000010000002700000049444c3a"
    "6f6d672e6f72672f53656375726974792f53656c6563746f7256616c7565"
    "3a312e3000000e00000053656c6563746f7256616c756500000002000000"
    "0900000073656c6563746f72000000001500000048000000010000002600"
    "000049444c3a6f6d672e6f72672f53656375726974792f53656c6563746f"
    "72547970653a312e300000000d00000053656c6563746f72547970650000"
    "0000050000000600000076616c75650000000b00000000000000";
    Security::_tc_SSLKeyCertCAPass = 
    "010000000f000000b0000000010000002a00000049444c3a6f6d672e6f72"
    "672f53656375726974792f53534c4b6579436572744341506173733a312e"
    "300000001100000053534c4b657943657274434150617373000000000500"
    "0000040000006b6579001200000000000000050000006365727400000000"
    "120000000000000007000000434166696c65000012000000000000000600"
    "000043416469720000001200000000000000050000007061737300000000"
    "1200000000000000";
    _marshaller_Security_SSLKeyCertCAPass = new _Marshaller_Security_SSLKeyCertCAPass;
    _marshaller__seq_Security_AttributeType = new _Marshaller__seq_Security_AttributeType;
    _marshaller__seq_Security_SecAttribute = new _Marshaller__seq_Security_SecAttribute;
    _marshaller__seq_Security_Right = new _Marshaller__seq_Security_Right;
    _marshaller__seq_Security_OptionsDirectionPair = new _Marshaller__seq_Security_OptionsDirectionPair;
    _marshaller__seq_Security_SecurityMechanismData = new _Marshaller__seq_Security_SecurityMechanismData;
    _marshaller__seq_Security_MechandOptions = new _Marshaller__seq_Security_MechandOptions;
    _marshaller__seq_Security_AuditEventType = new _Marshaller__seq_Security_AuditEventType;
    _marshaller__seq_Security_SelectorValue = new _Marshaller__seq_Security_SelectorValue;
  }

  ~__tc_init_SECURITY()
  {
    delete static_cast<_Marshaller_Security_ExtensibleFamily*>(_marshaller_Security_ExtensibleFamily);
    delete static_cast<_Marshaller_Security_AttributeType*>(_marshaller_Security_AttributeType);
    delete static_cast<_Marshaller_Security_SecAttribute*>(_marshaller_Security_SecAttribute);
    delete static_cast<_Marshaller_Security_AuthenticationStatus*>(_marshaller_Security_AuthenticationStatus);
    delete static_cast<_Marshaller_Security_AssociationStatus*>(_marshaller_Security_AssociationStatus);
    delete static_cast<_Marshaller_Security_CredentialsType*>(_marshaller_Security_CredentialsType);
    delete static_cast<_Marshaller_Security_ChannelBindings*>(_marshaller_Security_ChannelBindings);
    delete static_cast<_Marshaller_Security_Right*>(_marshaller_Security_Right);
    delete static_cast<_Marshaller_Security_RightsCombinator*>(_marshaller_Security_RightsCombinator);
    delete static_cast<_Marshaller_Security_DelegationState*>(_marshaller_Security_DelegationState);
    delete static_cast<_Marshaller_Security_DelegationDirective*>(_marshaller_Security_DelegationDirective);
    delete static_cast<_Marshaller_Security_SecurityFeature*>(_marshaller_Security_SecurityFeature);
    delete static_cast<_Marshaller_Security_QOP*>(_marshaller_Security_QOP);
    delete static_cast<_Marshaller_Security_SecurityContextType*>(_marshaller_Security_SecurityContextType);
    delete static_cast<_Marshaller_Security_SecurityContextState*>(_marshaller_Security_SecurityContextState);
    delete static_cast<_Marshaller_Security_OpaqueBuffer*>(_marshaller_Security_OpaqueBuffer);
    delete static_cast<_Marshaller_Security_RequiresSupports*>(_marshaller_Security_RequiresSupports);
    delete static_cast<_Marshaller_Security_CommunicationDirection*>(_marshaller_Security_CommunicationDirection);
    delete static_cast<_Marshaller_Security_OptionsDirectionPair*>(_marshaller_Security_OptionsDirectionPair);
    delete static_cast<_Marshaller_Security_DelegationMode*>(_marshaller_Security_DelegationMode);
    delete static_cast<_Marshaller_Security_SecurityMechanismData*>(_marshaller_Security_SecurityMechanismData);
    delete static_cast<_Marshaller_Security_MechandOptions*>(_marshaller_Security_MechandOptions);
    delete static_cast<_Marshaller_Security_EstablishTrust*>(_marshaller_Security_EstablishTrust);
    delete static_cast<_Marshaller_Security_DayOfTheWeek*>(_marshaller_Security_DayOfTheWeek);
    delete static_cast<_Marshaller_Security_AuditCombinator*>(_marshaller_Security_AuditCombinator);
    delete static_cast<_Marshaller_Security_AuditEventType*>(_marshaller_Security_AuditEventType);
    delete static_cast<_Marshaller_Security_SelectorValue*>(_marshaller_Security_SelectorValue);
    delete static_cast<_Marshaller_Security_SSLKeyCertCAPass*>(_marshaller_Security_SSLKeyCertCAPass);
    delete static_cast<_Marshaller__seq_Security_AttributeType*>(_marshaller__seq_Security_AttributeType);
    delete static_cast<_Marshaller__seq_Security_SecAttribute*>(_marshaller__seq_Security_SecAttribute);
    delete static_cast<_Marshaller__seq_Security_Right*>(_marshaller__seq_Security_Right);
    delete static_cast<_Marshaller__seq_Security_OptionsDirectionPair*>(_marshaller__seq_Security_OptionsDirectionPair);
    delete static_cast<_Marshaller__seq_Security_SecurityMechanismData*>(_marshaller__seq_Security_SecurityMechanismData);
    delete static_cast<_Marshaller__seq_Security_MechandOptions*>(_marshaller__seq_Security_MechandOptions);
    delete static_cast<_Marshaller__seq_Security_AuditEventType*>(_marshaller__seq_Security_AuditEventType);
    delete static_cast<_Marshaller__seq_Security_SelectorValue*>(_marshaller__seq_Security_SelectorValue);
  }
};

static __tc_init_SECURITY __init_SECURITY;

