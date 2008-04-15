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
namespace CSIIOP
{
CORBA::TypeCodeConst _tc_AssociationOptions;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_ServiceConfigurationSyntax;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_ServiceSpecificName;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_ServiceConfiguration;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::ServiceConfiguration::ServiceConfiguration()
{
}

CSIIOP::ServiceConfiguration::ServiceConfiguration( const ServiceConfiguration& _s )
{
  syntax = ((ServiceConfiguration&)_s).syntax;
  name = ((ServiceConfiguration&)_s).name;
}

CSIIOP::ServiceConfiguration::~ServiceConfiguration()
{
}

CSIIOP::ServiceConfiguration&
CSIIOP::ServiceConfiguration::operator=( const ServiceConfiguration& _s )
{
  syntax = ((ServiceConfiguration&)_s).syntax;
  name = ((ServiceConfiguration&)_s).name;
  return *this;
}
#endif

class _Marshaller_CSIIOP_ServiceConfiguration : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::ServiceConfiguration _MICO_T;
  public:
    ~_Marshaller_CSIIOP_ServiceConfiguration();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_ServiceConfiguration::~_Marshaller_CSIIOP_ServiceConfiguration()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_ServiceConfiguration::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_ServiceConfiguration::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_ServiceConfiguration::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_ServiceConfiguration::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->syntax ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->name ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_ServiceConfiguration::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->syntax );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->name );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_ServiceConfiguration::typecode()
{
  return CSIIOP::_tc_ServiceConfiguration;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_ServiceConfiguration;

void operator<<=( CORBA::Any &_a, const CSIIOP::ServiceConfiguration &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_ServiceConfiguration, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::ServiceConfiguration *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::ServiceConfiguration &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_ServiceConfiguration, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::ServiceConfiguration *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_ServiceConfiguration, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_ServiceConfigurationList;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_AS_ContextSec;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::AS_ContextSec::AS_ContextSec()
{
}

CSIIOP::AS_ContextSec::AS_ContextSec( const AS_ContextSec& _s )
{
  target_supports = ((AS_ContextSec&)_s).target_supports;
  target_requires = ((AS_ContextSec&)_s).target_requires;
  client_authentication_mech = ((AS_ContextSec&)_s).client_authentication_mech;
  target_name = ((AS_ContextSec&)_s).target_name;
}

CSIIOP::AS_ContextSec::~AS_ContextSec()
{
}

CSIIOP::AS_ContextSec&
CSIIOP::AS_ContextSec::operator=( const AS_ContextSec& _s )
{
  target_supports = ((AS_ContextSec&)_s).target_supports;
  target_requires = ((AS_ContextSec&)_s).target_requires;
  client_authentication_mech = ((AS_ContextSec&)_s).client_authentication_mech;
  target_name = ((AS_ContextSec&)_s).target_name;
  return *this;
}
#endif

class _Marshaller_CSIIOP_AS_ContextSec : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::AS_ContextSec _MICO_T;
  public:
    ~_Marshaller_CSIIOP_AS_ContextSec();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_AS_ContextSec::~_Marshaller_CSIIOP_AS_ContextSec()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_AS_ContextSec::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_AS_ContextSec::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_AS_ContextSec::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_AS_ContextSec::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_supports ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_requires ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->client_authentication_mech ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->target_name ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_AS_ContextSec::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_supports );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_requires );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->client_authentication_mech );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->target_name );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_AS_ContextSec::typecode()
{
  return CSIIOP::_tc_AS_ContextSec;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_AS_ContextSec;

void operator<<=( CORBA::Any &_a, const CSIIOP::AS_ContextSec &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_AS_ContextSec, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::AS_ContextSec *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::AS_ContextSec &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_AS_ContextSec, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::AS_ContextSec *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_AS_ContextSec, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_SAS_ContextSec;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::SAS_ContextSec::SAS_ContextSec()
{
}

CSIIOP::SAS_ContextSec::SAS_ContextSec( const SAS_ContextSec& _s )
{
  target_supports = ((SAS_ContextSec&)_s).target_supports;
  target_requires = ((SAS_ContextSec&)_s).target_requires;
  privilege_authorities = ((SAS_ContextSec&)_s).privilege_authorities;
  supported_naming_mechanisms = ((SAS_ContextSec&)_s).supported_naming_mechanisms;
  supported_identity_types = ((SAS_ContextSec&)_s).supported_identity_types;
}

CSIIOP::SAS_ContextSec::~SAS_ContextSec()
{
}

CSIIOP::SAS_ContextSec&
CSIIOP::SAS_ContextSec::operator=( const SAS_ContextSec& _s )
{
  target_supports = ((SAS_ContextSec&)_s).target_supports;
  target_requires = ((SAS_ContextSec&)_s).target_requires;
  privilege_authorities = ((SAS_ContextSec&)_s).privilege_authorities;
  supported_naming_mechanisms = ((SAS_ContextSec&)_s).supported_naming_mechanisms;
  supported_identity_types = ((SAS_ContextSec&)_s).supported_identity_types;
  return *this;
}
#endif

class _Marshaller_CSIIOP_SAS_ContextSec : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::SAS_ContextSec _MICO_T;
  public:
    ~_Marshaller_CSIIOP_SAS_ContextSec();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_SAS_ContextSec::~_Marshaller_CSIIOP_SAS_ContextSec()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_SAS_ContextSec::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_SAS_ContextSec::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_SAS_ContextSec::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_SAS_ContextSec::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_supports ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_requires ) &&
    _marshaller__seq_CSIIOP_ServiceConfiguration->demarshal( dc, &((_MICO_T*)v)->privilege_authorities ) &&
    _marshaller__seq__seq_octet->demarshal( dc, &((_MICO_T*)v)->supported_naming_mechanisms ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->supported_identity_types ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_SAS_ContextSec::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_supports );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_requires );
  _marshaller__seq_CSIIOP_ServiceConfiguration->marshal( ec, &((_MICO_T*)v)->privilege_authorities );
  _marshaller__seq__seq_octet->marshal( ec, &((_MICO_T*)v)->supported_naming_mechanisms );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->supported_identity_types );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_SAS_ContextSec::typecode()
{
  return CSIIOP::_tc_SAS_ContextSec;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_SAS_ContextSec;

void operator<<=( CORBA::Any &_a, const CSIIOP::SAS_ContextSec &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_SAS_ContextSec, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::SAS_ContextSec *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::SAS_ContextSec &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_SAS_ContextSec, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::SAS_ContextSec *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_SAS_ContextSec, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_CompoundSecMech;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::CompoundSecMech::CompoundSecMech()
{
}

CSIIOP::CompoundSecMech::CompoundSecMech( const CompoundSecMech& _s )
{
  target_requires = ((CompoundSecMech&)_s).target_requires;
  transport_mech = ((CompoundSecMech&)_s).transport_mech;
  as_context_mech = ((CompoundSecMech&)_s).as_context_mech;
  sas_context_mech = ((CompoundSecMech&)_s).sas_context_mech;
}

CSIIOP::CompoundSecMech::~CompoundSecMech()
{
}

CSIIOP::CompoundSecMech&
CSIIOP::CompoundSecMech::operator=( const CompoundSecMech& _s )
{
  target_requires = ((CompoundSecMech&)_s).target_requires;
  transport_mech = ((CompoundSecMech&)_s).transport_mech;
  as_context_mech = ((CompoundSecMech&)_s).as_context_mech;
  sas_context_mech = ((CompoundSecMech&)_s).sas_context_mech;
  return *this;
}
#endif

class _Marshaller_CSIIOP_CompoundSecMech : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::CompoundSecMech _MICO_T;
  public:
    ~_Marshaller_CSIIOP_CompoundSecMech();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_CompoundSecMech::~_Marshaller_CSIIOP_CompoundSecMech()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_CompoundSecMech::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_CompoundSecMech::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_CompoundSecMech::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_CompoundSecMech::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_requires ) &&
    _marshaller_IOP_TaggedComponent->demarshal( dc, &((_MICO_T*)v)->transport_mech ) &&
    _marshaller_CSIIOP_AS_ContextSec->demarshal( dc, &((_MICO_T*)v)->as_context_mech ) &&
    _marshaller_CSIIOP_SAS_ContextSec->demarshal( dc, &((_MICO_T*)v)->sas_context_mech ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_CompoundSecMech::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_requires );
  _marshaller_IOP_TaggedComponent->marshal( ec, &((_MICO_T*)v)->transport_mech );
  _marshaller_CSIIOP_AS_ContextSec->marshal( ec, &((_MICO_T*)v)->as_context_mech );
  _marshaller_CSIIOP_SAS_ContextSec->marshal( ec, &((_MICO_T*)v)->sas_context_mech );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_CompoundSecMech::typecode()
{
  return CSIIOP::_tc_CompoundSecMech;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_CompoundSecMech;

void operator<<=( CORBA::Any &_a, const CSIIOP::CompoundSecMech &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_CompoundSecMech, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::CompoundSecMech *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::CompoundSecMech &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_CompoundSecMech, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::CompoundSecMech *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_CompoundSecMech, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_CompoundSecMechanisms;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_CompoundSecMechList;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::CompoundSecMechList::CompoundSecMechList()
{
}

CSIIOP::CompoundSecMechList::CompoundSecMechList( const CompoundSecMechList& _s )
{
  stateful = ((CompoundSecMechList&)_s).stateful;
  mechanism_list = ((CompoundSecMechList&)_s).mechanism_list;
}

CSIIOP::CompoundSecMechList::~CompoundSecMechList()
{
}

CSIIOP::CompoundSecMechList&
CSIIOP::CompoundSecMechList::operator=( const CompoundSecMechList& _s )
{
  stateful = ((CompoundSecMechList&)_s).stateful;
  mechanism_list = ((CompoundSecMechList&)_s).mechanism_list;
  return *this;
}
#endif

class _Marshaller_CSIIOP_CompoundSecMechList : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::CompoundSecMechList _MICO_T;
  public:
    ~_Marshaller_CSIIOP_CompoundSecMechList();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_CompoundSecMechList::~_Marshaller_CSIIOP_CompoundSecMechList()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_CompoundSecMechList::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_CompoundSecMechList::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_CompoundSecMechList::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_CompoundSecMechList::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->stateful ) &&
    _marshaller__seq_CSIIOP_CompoundSecMech->demarshal( dc, &((_MICO_T*)v)->mechanism_list ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_CompoundSecMechList::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->stateful );
  _marshaller__seq_CSIIOP_CompoundSecMech->marshal( ec, &((_MICO_T*)v)->mechanism_list );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_CompoundSecMechList::typecode()
{
  return CSIIOP::_tc_CompoundSecMechList;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_CompoundSecMechList;

void operator<<=( CORBA::Any &_a, const CSIIOP::CompoundSecMechList &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_CompoundSecMechList, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::CompoundSecMechList *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::CompoundSecMechList &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_CompoundSecMechList, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::CompoundSecMechList *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_CompoundSecMechList, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_TransportAddress;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::TransportAddress::TransportAddress()
{
}

CSIIOP::TransportAddress::TransportAddress( const TransportAddress& _s )
{
  host_name = ((TransportAddress&)_s).host_name;
  port = ((TransportAddress&)_s).port;
}

CSIIOP::TransportAddress::~TransportAddress()
{
}

CSIIOP::TransportAddress&
CSIIOP::TransportAddress::operator=( const TransportAddress& _s )
{
  host_name = ((TransportAddress&)_s).host_name;
  port = ((TransportAddress&)_s).port;
  return *this;
}
#endif

class _Marshaller_CSIIOP_TransportAddress : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::TransportAddress _MICO_T;
  public:
    ~_Marshaller_CSIIOP_TransportAddress();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_TransportAddress::~_Marshaller_CSIIOP_TransportAddress()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_TransportAddress::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_TransportAddress::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_TransportAddress::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_TransportAddress::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->host_name._for_demarshal() ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->port ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_TransportAddress::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->host_name.inout() );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->port );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_TransportAddress::typecode()
{
  return CSIIOP::_tc_TransportAddress;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_TransportAddress;

void operator<<=( CORBA::Any &_a, const CSIIOP::TransportAddress &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_TransportAddress, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::TransportAddress *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::TransportAddress &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_TransportAddress, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::TransportAddress *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_TransportAddress, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_TransportAddressList;
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_SECIOP_SEC_TRANS;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::SECIOP_SEC_TRANS::SECIOP_SEC_TRANS()
{
}

CSIIOP::SECIOP_SEC_TRANS::SECIOP_SEC_TRANS( const SECIOP_SEC_TRANS& _s )
{
  target_supports = ((SECIOP_SEC_TRANS&)_s).target_supports;
  target_requires = ((SECIOP_SEC_TRANS&)_s).target_requires;
  mech_oid = ((SECIOP_SEC_TRANS&)_s).mech_oid;
  target_name = ((SECIOP_SEC_TRANS&)_s).target_name;
  addresses = ((SECIOP_SEC_TRANS&)_s).addresses;
}

CSIIOP::SECIOP_SEC_TRANS::~SECIOP_SEC_TRANS()
{
}

CSIIOP::SECIOP_SEC_TRANS&
CSIIOP::SECIOP_SEC_TRANS::operator=( const SECIOP_SEC_TRANS& _s )
{
  target_supports = ((SECIOP_SEC_TRANS&)_s).target_supports;
  target_requires = ((SECIOP_SEC_TRANS&)_s).target_requires;
  mech_oid = ((SECIOP_SEC_TRANS&)_s).mech_oid;
  target_name = ((SECIOP_SEC_TRANS&)_s).target_name;
  addresses = ((SECIOP_SEC_TRANS&)_s).addresses;
  return *this;
}
#endif

class _Marshaller_CSIIOP_SECIOP_SEC_TRANS : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::SECIOP_SEC_TRANS _MICO_T;
  public:
    ~_Marshaller_CSIIOP_SECIOP_SEC_TRANS();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_SECIOP_SEC_TRANS::~_Marshaller_CSIIOP_SECIOP_SEC_TRANS()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_SECIOP_SEC_TRANS::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_SECIOP_SEC_TRANS::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_SECIOP_SEC_TRANS::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_SECIOP_SEC_TRANS::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_supports ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_requires ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->mech_oid ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->target_name ) &&
    _marshaller__seq_CSIIOP_TransportAddress->demarshal( dc, &((_MICO_T*)v)->addresses ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_SECIOP_SEC_TRANS::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_supports );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_requires );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->mech_oid );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->target_name );
  _marshaller__seq_CSIIOP_TransportAddress->marshal( ec, &((_MICO_T*)v)->addresses );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_SECIOP_SEC_TRANS::typecode()
{
  return CSIIOP::_tc_SECIOP_SEC_TRANS;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_SECIOP_SEC_TRANS;

void operator<<=( CORBA::Any &_a, const CSIIOP::SECIOP_SEC_TRANS &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_SECIOP_SEC_TRANS, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::SECIOP_SEC_TRANS *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::SECIOP_SEC_TRANS &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_SECIOP_SEC_TRANS, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::SECIOP_SEC_TRANS *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_SECIOP_SEC_TRANS, (void *&)_s);
}

namespace CSIIOP
{
CORBA::TypeCodeConst _tc_TLS_SEC_TRANS;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSIIOP::TLS_SEC_TRANS::TLS_SEC_TRANS()
{
}

CSIIOP::TLS_SEC_TRANS::TLS_SEC_TRANS( const TLS_SEC_TRANS& _s )
{
  target_supports = ((TLS_SEC_TRANS&)_s).target_supports;
  target_requires = ((TLS_SEC_TRANS&)_s).target_requires;
  addresses = ((TLS_SEC_TRANS&)_s).addresses;
}

CSIIOP::TLS_SEC_TRANS::~TLS_SEC_TRANS()
{
}

CSIIOP::TLS_SEC_TRANS&
CSIIOP::TLS_SEC_TRANS::operator=( const TLS_SEC_TRANS& _s )
{
  target_supports = ((TLS_SEC_TRANS&)_s).target_supports;
  target_requires = ((TLS_SEC_TRANS&)_s).target_requires;
  addresses = ((TLS_SEC_TRANS&)_s).addresses;
  return *this;
}
#endif

class _Marshaller_CSIIOP_TLS_SEC_TRANS : public ::CORBA::StaticTypeInfo {
    typedef CSIIOP::TLS_SEC_TRANS _MICO_T;
  public:
    ~_Marshaller_CSIIOP_TLS_SEC_TRANS();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSIIOP_TLS_SEC_TRANS::~_Marshaller_CSIIOP_TLS_SEC_TRANS()
{
}

::CORBA::StaticValueType _Marshaller_CSIIOP_TLS_SEC_TRANS::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSIIOP_TLS_SEC_TRANS::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSIIOP_TLS_SEC_TRANS::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSIIOP_TLS_SEC_TRANS::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_supports ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->target_requires ) &&
    _marshaller__seq_CSIIOP_TransportAddress->demarshal( dc, &((_MICO_T*)v)->addresses ) &&
    dc.struct_end();
}

void _Marshaller_CSIIOP_TLS_SEC_TRANS::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_supports );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->target_requires );
  _marshaller__seq_CSIIOP_TransportAddress->marshal( ec, &((_MICO_T*)v)->addresses );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSIIOP_TLS_SEC_TRANS::typecode()
{
  return CSIIOP::_tc_TLS_SEC_TRANS;
}

::CORBA::StaticTypeInfo *_marshaller_CSIIOP_TLS_SEC_TRANS;

void operator<<=( CORBA::Any &_a, const CSIIOP::TLS_SEC_TRANS &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_TLS_SEC_TRANS, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSIIOP::TLS_SEC_TRANS *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSIIOP::TLS_SEC_TRANS &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSIIOP_TLS_SEC_TRANS, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSIIOP::TLS_SEC_TRANS *&_s )
{
  return _a.to_static_any (_marshaller_CSIIOP_TLS_SEC_TRANS, (void *&)_s);
}

class _Marshaller__seq_CSIIOP_ServiceConfiguration : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CSIIOP_ServiceConfiguration();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CSIIOP_ServiceConfiguration::~_Marshaller__seq_CSIIOP_ServiceConfiguration()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CSIIOP_ServiceConfiguration::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CSIIOP_ServiceConfiguration::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CSIIOP_ServiceConfiguration::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CSIIOP_ServiceConfiguration::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CSIIOP_ServiceConfiguration->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CSIIOP_ServiceConfiguration::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CSIIOP_ServiceConfiguration->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_ServiceConfiguration::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000130000004c010000010000000f0000003c010000010000002c00"
    "000049444c3a6f6d672e6f72672f435349494f502f53657276696365436f"
    "6e66696775726174696f6e3a312e30001500000053657276696365436f6e"
    "66696775726174696f6e00000000020000000700000073796e7461780000"
    "1500000060000000010000003200000049444c3a6f6d672e6f72672f4353"
    "49494f502f53657276696365436f6e66696775726174696f6e53796e7461"
    "783a312e300000001b00000053657276696365436f6e6669677572617469"
    "6f6e53796e746178000005000000050000006e616d650000000015000000"
    "60000000010000002b00000049444c3a6f6d672e6f72672f435349494f50"
    "2f5365727669636553706563696669634e616d653a312e30000014000000"
    "5365727669636553706563696669634e616d6500130000000c0000000100"
    "00000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_ServiceConfiguration::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_ServiceConfiguration;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_ServiceConfiguration, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_ServiceConfiguration, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CSIIOP_ServiceConfiguration, (void *&)_s);
}


class _Marshaller__seq_CSIIOP_CompoundSecMech : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CSIIOP_CompoundSecMech();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CSIIOP_CompoundSecMech::~_Marshaller__seq_CSIIOP_CompoundSecMech()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CSIIOP_CompoundSecMech::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CSIIOP_CompoundSecMech::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CSIIOP_CompoundSecMech::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CSIIOP_CompoundSecMech::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CSIIOP_CompoundSecMech->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CSIIOP_CompoundSecMech::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CSIIOP_CompoundSecMech->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_CompoundSecMech::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000cc070000010000000f000000bc070000010000002700"
    "000049444c3a6f6d672e6f72672f435349494f502f436f6d706f756e6453"
    "65634d6563683a312e30000010000000436f6d706f756e645365634d6563"
    "680004000000100000007461726765745f72657175697265730015000000"
    "50000000010000002a00000049444c3a6f6d672e6f72672f435349494f50"
    "2f4173736f63696174696f6e4f7074696f6e733a312e3000000013000000"
    "4173736f63696174696f6e4f7074696f6e730000040000000f0000007472"
    "616e73706f72745f6d65636800000f000000a8000000010000001c000000"
    "49444c3a494f502f546167676564436f6d706f6e656e743a312e30001000"
    "0000546167676564436f6d706f6e656e7400020000000400000074616700"
    "1500000034000000010000001800000049444c3a494f502f436f6d706f6e"
    "656e7449643a312e30000c000000436f6d706f6e656e7449640005000000"
    "0f000000636f6d706f6e656e745f646174610000130000000c0000000100"
    "00000a000000000000001000000061735f636f6e746578745f6d65636800"
    "0f000000f8010000010000002500000049444c3a6f6d672e6f72672f4353"
    "49494f502f41535f436f6e746578745365633a312e30000000000e000000"
    "41535f436f6e746578745365630000000400000010000000746172676574"
    "5f737570706f727473001500000050000000010000002a00000049444c3a"
    "6f6d672e6f72672f435349494f502f4173736f63696174696f6e4f707469"
    "6f6e733a312e30000000130000004173736f63696174696f6e4f7074696f"
    "6e73000004000000100000007461726765745f7265717569726573001500"
    "000050000000010000002a00000049444c3a6f6d672e6f72672f43534949"
    "4f502f4173736f63696174696f6e4f7074696f6e733a312e300000001300"
    "00004173736f63696174696f6e4f7074696f6e730000040000001b000000"
    "636c69656e745f61757468656e7469636174696f6e5f6d65636800001500"
    "00003c000000010000001800000049444c3a6f6d672e6f72672f4353492f"
    "4f49443a312e3000040000004f494400130000000c000000010000000a00"
    "0000000000000c0000007461726765745f6e616d6500150000005c000000"
    "010000002800000049444c3a6f6d672e6f72672f4353492f4753535f4e54"
    "5f4578706f727465644e616d653a312e3000140000004753535f4e545f45"
    "78706f727465644e616d6500130000000c000000010000000a0000000000"
    "0000110000007361735f636f6e746578745f6d656368000000000f000000"
    "10040000010000002600000049444c3a6f6d672e6f72672f435349494f50"
    "2f5341535f436f6e746578745365633a312e300000000f0000005341535f"
    "436f6e74657874536563000005000000100000007461726765745f737570"
    "706f727473001500000050000000010000002a00000049444c3a6f6d672e"
    "6f72672f435349494f502f4173736f63696174696f6e4f7074696f6e733a"
    "312e30000000130000004173736f63696174696f6e4f7074696f6e730000"
    "04000000100000007461726765745f726571756972657300150000005000"
    "0000010000002a00000049444c3a6f6d672e6f72672f435349494f502f41"
    "73736f63696174696f6e4f7074696f6e733a312e30000000130000004173"
    "736f63696174696f6e4f7074696f6e730000040000001600000070726976"
    "696c6567655f617574686f72697469657300000015000000ac0100000100"
    "00003000000049444c3a6f6d672e6f72672f435349494f502f5365727669"
    "6365436f6e66696775726174696f6e4c6973743a312e3000190000005365"
    "7276696365436f6e66696775726174696f6e4c6973740000000013000000"
    "4c010000010000000f0000003c010000010000002c00000049444c3a6f6d"
    "672e6f72672f435349494f502f53657276696365436f6e66696775726174"
    "696f6e3a312e30001500000053657276696365436f6e6669677572617469"
    "6f6e00000000020000000700000073796e74617800001500000060000000"
    "010000003200000049444c3a6f6d672e6f72672f435349494f502f536572"
    "76696365436f6e66696775726174696f6e53796e7461783a312e30000000"
    "1b00000053657276696365436f6e66696775726174696f6e53796e746178"
    "000005000000050000006e616d6500000000150000006000000001000000"
    "2b00000049444c3a6f6d672e6f72672f435349494f502f53657276696365"
    "53706563696669634e616d653a312e300000140000005365727669636553"
    "706563696669634e616d6500130000000c000000010000000a0000000000"
    "0000000000001c000000737570706f727465645f6e616d696e675f6d6563"
    "68616e69736d73001500000084000000010000001c00000049444c3a6f6d"
    "672e6f72672f4353492f4f49444c6973743a312e3000080000004f49444c"
    "69737400130000004c00000001000000150000003c000000010000001800"
    "000049444c3a6f6d672e6f72672f4353492f4f49443a312e300004000000"
    "4f494400130000000c000000010000000a00000000000000000000001900"
    "0000737570706f727465645f6964656e746974795f747970657300000000"
    "150000004c000000010000002600000049444c3a6f6d672e6f72672f4353"
    "492f4964656e74697479546f6b656e547970653a312e3000000012000000"
    "4964656e74697479546f6b656e547970650000000500000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_CompoundSecMech::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_CompoundSecMech;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_CompoundSecMech, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_CompoundSecMech, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CSIIOP_CompoundSecMech, (void *&)_s);
}


class _Marshaller__seq_CSIIOP_TransportAddress : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CSIIOP_TransportAddress();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CSIIOP_TransportAddress::~_Marshaller__seq_CSIIOP_TransportAddress()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CSIIOP_TransportAddress::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CSIIOP_TransportAddress::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CSIIOP_TransportAddress::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CSIIOP_TransportAddress::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CSIIOP_TransportAddress->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CSIIOP_TransportAddress::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CSIIOP_TransportAddress->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_TransportAddress::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000084000000010000000f00000074000000010000002800"
    "000049444c3a6f6d672e6f72672f435349494f502f5472616e73706f7274"
    "416464726573733a312e3000110000005472616e73706f72744164647265"
    "737300000000020000000a000000686f73745f6e616d6500000012000000"
    "0000000005000000706f7274000000000400000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSIIOP_TransportAddress::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_TransportAddress;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_TransportAddress, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSIIOP_TransportAddress, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CSIIOP_TransportAddress, (void *&)_s);
}


struct __tc_init_CSIIOP {
  __tc_init_CSIIOP()
  {
    CSIIOP::_tc_AssociationOptions = 
    "010000001500000050000000010000002a00000049444c3a6f6d672e6f72"
    "672f435349494f502f4173736f63696174696f6e4f7074696f6e733a312e"
    "30000000130000004173736f63696174696f6e4f7074696f6e7300000400"
    "0000";
    CSIIOP::_tc_ServiceConfigurationSyntax = 
    "010000001500000060000000010000003200000049444c3a6f6d672e6f72"
    "672f435349494f502f53657276696365436f6e66696775726174696f6e53"
    "796e7461783a312e300000001b00000053657276696365436f6e66696775"
    "726174696f6e53796e746178000005000000";
    CSIIOP::_tc_ServiceSpecificName = 
    "010000001500000060000000010000002b00000049444c3a6f6d672e6f72"
    "672f435349494f502f5365727669636553706563696669634e616d653a31"
    "2e300000140000005365727669636553706563696669634e616d65001300"
    "00000c000000010000000a00000000000000";
    CSIIOP::_tc_ServiceConfiguration = 
    "010000000f0000003c010000010000002c00000049444c3a6f6d672e6f72"
    "672f435349494f502f53657276696365436f6e66696775726174696f6e3a"
    "312e30001500000053657276696365436f6e66696775726174696f6e0000"
    "0000020000000700000073796e7461780000150000006000000001000000"
    "3200000049444c3a6f6d672e6f72672f435349494f502f53657276696365"
    "436f6e66696775726174696f6e53796e7461783a312e300000001b000000"
    "53657276696365436f6e66696775726174696f6e53796e74617800000500"
    "0000050000006e616d65000000001500000060000000010000002b000000"
    "49444c3a6f6d672e6f72672f435349494f502f5365727669636553706563"
    "696669634e616d653a312e30000014000000536572766963655370656369"
    "6669634e616d6500130000000c000000010000000a00000000000000";
    _marshaller_CSIIOP_ServiceConfiguration = new _Marshaller_CSIIOP_ServiceConfiguration;
    CSIIOP::_tc_ServiceConfigurationList = 
    "0100000015000000ac010000010000003000000049444c3a6f6d672e6f72"
    "672f435349494f502f53657276696365436f6e66696775726174696f6e4c"
    "6973743a312e30001900000053657276696365436f6e6669677572617469"
    "6f6e4c69737400000000130000004c010000010000000f0000003c010000"
    "010000002c00000049444c3a6f6d672e6f72672f435349494f502f536572"
    "76696365436f6e66696775726174696f6e3a312e30001500000053657276"
    "696365436f6e66696775726174696f6e0000000002000000070000007379"
    "6e74617800001500000060000000010000003200000049444c3a6f6d672e"
    "6f72672f435349494f502f53657276696365436f6e66696775726174696f"
    "6e53796e7461783a312e300000001b00000053657276696365436f6e6669"
    "6775726174696f6e53796e746178000005000000050000006e616d650000"
    "00001500000060000000010000002b00000049444c3a6f6d672e6f72672f"
    "435349494f502f5365727669636553706563696669634e616d653a312e30"
    "0000140000005365727669636553706563696669634e616d650013000000"
    "0c000000010000000a0000000000000000000000";
    CSIIOP::_tc_AS_ContextSec = 
    "010000000f000000f8010000010000002500000049444c3a6f6d672e6f72"
    "672f435349494f502f41535f436f6e746578745365633a312e3000000000"
    "0e00000041535f436f6e7465787453656300000004000000100000007461"
    "726765745f737570706f727473001500000050000000010000002a000000"
    "49444c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e"
    "4f7074696f6e733a312e30000000130000004173736f63696174696f6e4f"
    "7074696f6e73000004000000100000007461726765745f72657175697265"
    "73001500000050000000010000002a00000049444c3a6f6d672e6f72672f"
    "435349494f502f4173736f63696174696f6e4f7074696f6e733a312e3000"
    "0000130000004173736f63696174696f6e4f7074696f6e73000004000000"
    "1b000000636c69656e745f61757468656e7469636174696f6e5f6d656368"
    "0000150000003c000000010000001800000049444c3a6f6d672e6f72672f"
    "4353492f4f49443a312e3000040000004f494400130000000c0000000100"
    "00000a000000000000000c0000007461726765745f6e616d650015000000"
    "5c000000010000002800000049444c3a6f6d672e6f72672f4353492f4753"
    "535f4e545f4578706f727465644e616d653a312e3000140000004753535f"
    "4e545f4578706f727465644e616d6500130000000c000000010000000a00"
    "000000000000";
    _marshaller_CSIIOP_AS_ContextSec = new _Marshaller_CSIIOP_AS_ContextSec;
    CSIIOP::_tc_SAS_ContextSec = 
    "010000000f00000010040000010000002600000049444c3a6f6d672e6f72"
    "672f435349494f502f5341535f436f6e746578745365633a312e30000000"
    "0f0000005341535f436f6e74657874536563000005000000100000007461"
    "726765745f737570706f727473001500000050000000010000002a000000"
    "49444c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e"
    "4f7074696f6e733a312e30000000130000004173736f63696174696f6e4f"
    "7074696f6e73000004000000100000007461726765745f72657175697265"
    "73001500000050000000010000002a00000049444c3a6f6d672e6f72672f"
    "435349494f502f4173736f63696174696f6e4f7074696f6e733a312e3000"
    "0000130000004173736f63696174696f6e4f7074696f6e73000004000000"
    "1600000070726976696c6567655f617574686f7269746965730000001500"
    "0000ac010000010000003000000049444c3a6f6d672e6f72672f43534949"
    "4f502f53657276696365436f6e66696775726174696f6e4c6973743a312e"
    "30001900000053657276696365436f6e66696775726174696f6e4c697374"
    "00000000130000004c010000010000000f0000003c010000010000002c00"
    "000049444c3a6f6d672e6f72672f435349494f502f53657276696365436f"
    "6e66696775726174696f6e3a312e30001500000053657276696365436f6e"
    "66696775726174696f6e00000000020000000700000073796e7461780000"
    "1500000060000000010000003200000049444c3a6f6d672e6f72672f4353"
    "49494f502f53657276696365436f6e66696775726174696f6e53796e7461"
    "783a312e300000001b00000053657276696365436f6e6669677572617469"
    "6f6e53796e746178000005000000050000006e616d650000000015000000"
    "60000000010000002b00000049444c3a6f6d672e6f72672f435349494f50"
    "2f5365727669636553706563696669634e616d653a312e30000014000000"
    "5365727669636553706563696669634e616d6500130000000c0000000100"
    "00000a00000000000000000000001c000000737570706f727465645f6e61"
    "6d696e675f6d656368616e69736d73001500000084000000010000001c00"
    "000049444c3a6f6d672e6f72672f4353492f4f49444c6973743a312e3000"
    "080000004f49444c69737400130000004c00000001000000150000003c00"
    "0000010000001800000049444c3a6f6d672e6f72672f4353492f4f49443a"
    "312e3000040000004f494400130000000c000000010000000a0000000000"
    "00000000000019000000737570706f727465645f6964656e746974795f74"
    "7970657300000000150000004c000000010000002600000049444c3a6f6d"
    "672e6f72672f4353492f4964656e74697479546f6b656e547970653a312e"
    "30000000120000004964656e74697479546f6b656e547970650000000500"
    "0000";
    _marshaller_CSIIOP_SAS_ContextSec = new _Marshaller_CSIIOP_SAS_ContextSec;
    CSIIOP::_tc_CompoundSecMech = 
    "010000000f000000bc070000010000002700000049444c3a6f6d672e6f72"
    "672f435349494f502f436f6d706f756e645365634d6563683a312e300000"
    "10000000436f6d706f756e645365634d6563680004000000100000007461"
    "726765745f7265717569726573001500000050000000010000002a000000"
    "49444c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e"
    "4f7074696f6e733a312e30000000130000004173736f63696174696f6e4f"
    "7074696f6e730000040000000f0000007472616e73706f72745f6d656368"
    "00000f000000a8000000010000001c00000049444c3a494f502f54616767"
    "6564436f6d706f6e656e743a312e300010000000546167676564436f6d70"
    "6f6e656e7400020000000400000074616700150000003400000001000000"
    "1800000049444c3a494f502f436f6d706f6e656e7449643a312e30000c00"
    "0000436f6d706f6e656e74496400050000000f000000636f6d706f6e656e"
    "745f646174610000130000000c000000010000000a000000000000001000"
    "000061735f636f6e746578745f6d656368000f000000f801000001000000"
    "2500000049444c3a6f6d672e6f72672f435349494f502f41535f436f6e74"
    "6578745365633a312e30000000000e00000041535f436f6e746578745365"
    "6300000004000000100000007461726765745f737570706f727473001500"
    "000050000000010000002a00000049444c3a6f6d672e6f72672f43534949"
    "4f502f4173736f63696174696f6e4f7074696f6e733a312e300000001300"
    "00004173736f63696174696f6e4f7074696f6e7300000400000010000000"
    "7461726765745f7265717569726573001500000050000000010000002a00"
    "000049444c3a6f6d672e6f72672f435349494f502f4173736f6369617469"
    "6f6e4f7074696f6e733a312e30000000130000004173736f63696174696f"
    "6e4f7074696f6e730000040000001b000000636c69656e745f6175746865"
    "6e7469636174696f6e5f6d6563680000150000003c000000010000001800"
    "000049444c3a6f6d672e6f72672f4353492f4f49443a312e300004000000"
    "4f494400130000000c000000010000000a000000000000000c0000007461"
    "726765745f6e616d6500150000005c000000010000002800000049444c3a"
    "6f6d672e6f72672f4353492f4753535f4e545f4578706f727465644e616d"
    "653a312e3000140000004753535f4e545f4578706f727465644e616d6500"
    "130000000c000000010000000a00000000000000110000007361735f636f"
    "6e746578745f6d656368000000000f000000100400000100000026000000"
    "49444c3a6f6d672e6f72672f435349494f502f5341535f436f6e74657874"
    "5365633a312e300000000f0000005341535f436f6e746578745365630000"
    "05000000100000007461726765745f737570706f72747300150000005000"
    "0000010000002a00000049444c3a6f6d672e6f72672f435349494f502f41"
    "73736f63696174696f6e4f7074696f6e733a312e30000000130000004173"
    "736f63696174696f6e4f7074696f6e730000040000001000000074617267"
    "65745f7265717569726573001500000050000000010000002a0000004944"
    "4c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e4f70"
    "74696f6e733a312e30000000130000004173736f63696174696f6e4f7074"
    "696f6e730000040000001600000070726976696c6567655f617574686f72"
    "697469657300000015000000ac010000010000003000000049444c3a6f6d"
    "672e6f72672f435349494f502f53657276696365436f6e66696775726174"
    "696f6e4c6973743a312e30001900000053657276696365436f6e66696775"
    "726174696f6e4c69737400000000130000004c010000010000000f000000"
    "3c010000010000002c00000049444c3a6f6d672e6f72672f435349494f50"
    "2f53657276696365436f6e66696775726174696f6e3a312e300015000000"
    "53657276696365436f6e66696775726174696f6e00000000020000000700"
    "000073796e74617800001500000060000000010000003200000049444c3a"
    "6f6d672e6f72672f435349494f502f53657276696365436f6e6669677572"
    "6174696f6e53796e7461783a312e300000001b0000005365727669636543"
    "6f6e66696775726174696f6e53796e746178000005000000050000006e61"
    "6d65000000001500000060000000010000002b00000049444c3a6f6d672e"
    "6f72672f435349494f502f5365727669636553706563696669634e616d65"
    "3a312e300000140000005365727669636553706563696669634e616d6500"
    "130000000c000000010000000a00000000000000000000001c0000007375"
    "70706f727465645f6e616d696e675f6d656368616e69736d730015000000"
    "84000000010000001c00000049444c3a6f6d672e6f72672f4353492f4f49"
    "444c6973743a312e3000080000004f49444c69737400130000004c000000"
    "01000000150000003c000000010000001800000049444c3a6f6d672e6f72"
    "672f4353492f4f49443a312e3000040000004f494400130000000c000000"
    "010000000a000000000000000000000019000000737570706f727465645f"
    "6964656e746974795f747970657300000000150000004c00000001000000"
    "2600000049444c3a6f6d672e6f72672f4353492f4964656e74697479546f"
    "6b656e547970653a312e30000000120000004964656e74697479546f6b65"
    "6e5479706500000005000000";
    _marshaller_CSIIOP_CompoundSecMech = new _Marshaller_CSIIOP_CompoundSecMech;
    CSIIOP::_tc_CompoundSecMechanisms = 
    "010000001500000028080000010000002d00000049444c3a6f6d672e6f72"
    "672f435349494f502f436f6d706f756e645365634d656368616e69736d73"
    "3a312e300000000016000000436f6d706f756e645365634d656368616e69"
    "736d7300000013000000cc070000010000000f000000bc07000001000000"
    "2700000049444c3a6f6d672e6f72672f435349494f502f436f6d706f756e"
    "645365634d6563683a312e30000010000000436f6d706f756e645365634d"
    "6563680004000000100000007461726765745f7265717569726573001500"
    "000050000000010000002a00000049444c3a6f6d672e6f72672f43534949"
    "4f502f4173736f63696174696f6e4f7074696f6e733a312e300000001300"
    "00004173736f63696174696f6e4f7074696f6e730000040000000f000000"
    "7472616e73706f72745f6d65636800000f000000a8000000010000001c00"
    "000049444c3a494f502f546167676564436f6d706f6e656e743a312e3000"
    "10000000546167676564436f6d706f6e656e740002000000040000007461"
    "67001500000034000000010000001800000049444c3a494f502f436f6d70"
    "6f6e656e7449643a312e30000c000000436f6d706f6e656e744964000500"
    "00000f000000636f6d706f6e656e745f646174610000130000000c000000"
    "010000000a000000000000001000000061735f636f6e746578745f6d6563"
    "68000f000000f8010000010000002500000049444c3a6f6d672e6f72672f"
    "435349494f502f41535f436f6e746578745365633a312e30000000000e00"
    "000041535f436f6e74657874536563000000040000001000000074617267"
    "65745f737570706f727473001500000050000000010000002a0000004944"
    "4c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e4f70"
    "74696f6e733a312e30000000130000004173736f63696174696f6e4f7074"
    "696f6e73000004000000100000007461726765745f726571756972657300"
    "1500000050000000010000002a00000049444c3a6f6d672e6f72672f4353"
    "49494f502f4173736f63696174696f6e4f7074696f6e733a312e30000000"
    "130000004173736f63696174696f6e4f7074696f6e730000040000001b00"
    "0000636c69656e745f61757468656e7469636174696f6e5f6d6563680000"
    "150000003c000000010000001800000049444c3a6f6d672e6f72672f4353"
    "492f4f49443a312e3000040000004f494400130000000c00000001000000"
    "0a000000000000000c0000007461726765745f6e616d6500150000005c00"
    "0000010000002800000049444c3a6f6d672e6f72672f4353492f4753535f"
    "4e545f4578706f727465644e616d653a312e3000140000004753535f4e54"
    "5f4578706f727465644e616d6500130000000c000000010000000a000000"
    "00000000110000007361735f636f6e746578745f6d656368000000000f00"
    "000010040000010000002600000049444c3a6f6d672e6f72672f43534949"
    "4f502f5341535f436f6e746578745365633a312e300000000f0000005341"
    "535f436f6e74657874536563000005000000100000007461726765745f73"
    "7570706f727473001500000050000000010000002a00000049444c3a6f6d"
    "672e6f72672f435349494f502f4173736f63696174696f6e4f7074696f6e"
    "733a312e30000000130000004173736f63696174696f6e4f7074696f6e73"
    "000004000000100000007461726765745f72657175697265730015000000"
    "50000000010000002a00000049444c3a6f6d672e6f72672f435349494f50"
    "2f4173736f63696174696f6e4f7074696f6e733a312e3000000013000000"
    "4173736f63696174696f6e4f7074696f6e73000004000000160000007072"
    "6976696c6567655f617574686f72697469657300000015000000ac010000"
    "010000003000000049444c3a6f6d672e6f72672f435349494f502f536572"
    "76696365436f6e66696775726174696f6e4c6973743a312e300019000000"
    "53657276696365436f6e66696775726174696f6e4c697374000000001300"
    "00004c010000010000000f0000003c010000010000002c00000049444c3a"
    "6f6d672e6f72672f435349494f502f53657276696365436f6e6669677572"
    "6174696f6e3a312e30001500000053657276696365436f6e666967757261"
    "74696f6e00000000020000000700000073796e7461780000150000006000"
    "0000010000003200000049444c3a6f6d672e6f72672f435349494f502f53"
    "657276696365436f6e66696775726174696f6e53796e7461783a312e3000"
    "00001b00000053657276696365436f6e66696775726174696f6e53796e74"
    "6178000005000000050000006e616d650000000015000000600000000100"
    "00002b00000049444c3a6f6d672e6f72672f435349494f502f5365727669"
    "636553706563696669634e616d653a312e30000014000000536572766963"
    "6553706563696669634e616d6500130000000c000000010000000a000000"
    "00000000000000001c000000737570706f727465645f6e616d696e675f6d"
    "656368616e69736d73001500000084000000010000001c00000049444c3a"
    "6f6d672e6f72672f4353492f4f49444c6973743a312e3000080000004f49"
    "444c69737400130000004c00000001000000150000003c00000001000000"
    "1800000049444c3a6f6d672e6f72672f4353492f4f49443a312e30000400"
    "00004f494400130000000c000000010000000a0000000000000000000000"
    "19000000737570706f727465645f6964656e746974795f74797065730000"
    "0000150000004c000000010000002600000049444c3a6f6d672e6f72672f"
    "4353492f4964656e74697479546f6b656e547970653a312e300000001200"
    "00004964656e74697479546f6b656e547970650000000500000000000000"
    ;
    CSIIOP::_tc_CompoundSecMechList = 
    "010000000f000000a8080000010000002b00000049444c3a6f6d672e6f72"
    "672f435349494f502f436f6d706f756e645365634d6563684c6973743a31"
    "2e30000014000000436f6d706f756e645365634d6563684c697374000200"
    "000009000000737461746566756c00000000080000000f0000006d656368"
    "616e69736d5f6c69737400001500000028080000010000002d0000004944"
    "4c3a6f6d672e6f72672f435349494f502f436f6d706f756e645365634d65"
    "6368616e69736d733a312e300000000016000000436f6d706f756e645365"
    "634d656368616e69736d7300000013000000cc070000010000000f000000"
    "bc070000010000002700000049444c3a6f6d672e6f72672f435349494f50"
    "2f436f6d706f756e645365634d6563683a312e30000010000000436f6d70"
    "6f756e645365634d6563680004000000100000007461726765745f726571"
    "7569726573001500000050000000010000002a00000049444c3a6f6d672e"
    "6f72672f435349494f502f4173736f63696174696f6e4f7074696f6e733a"
    "312e30000000130000004173736f63696174696f6e4f7074696f6e730000"
    "040000000f0000007472616e73706f72745f6d65636800000f000000a800"
    "0000010000001c00000049444c3a494f502f546167676564436f6d706f6e"
    "656e743a312e300010000000546167676564436f6d706f6e656e74000200"
    "000004000000746167001500000034000000010000001800000049444c3a"
    "494f502f436f6d706f6e656e7449643a312e30000c000000436f6d706f6e"
    "656e74496400050000000f000000636f6d706f6e656e745f646174610000"
    "130000000c000000010000000a000000000000001000000061735f636f6e"
    "746578745f6d656368000f000000f8010000010000002500000049444c3a"
    "6f6d672e6f72672f435349494f502f41535f436f6e746578745365633a31"
    "2e30000000000e00000041535f436f6e7465787453656300000004000000"
    "100000007461726765745f737570706f7274730015000000500000000100"
    "00002a00000049444c3a6f6d672e6f72672f435349494f502f4173736f63"
    "696174696f6e4f7074696f6e733a312e30000000130000004173736f6369"
    "6174696f6e4f7074696f6e73000004000000100000007461726765745f72"
    "65717569726573001500000050000000010000002a00000049444c3a6f6d"
    "672e6f72672f435349494f502f4173736f63696174696f6e4f7074696f6e"
    "733a312e30000000130000004173736f63696174696f6e4f7074696f6e73"
    "0000040000001b000000636c69656e745f61757468656e7469636174696f"
    "6e5f6d6563680000150000003c000000010000001800000049444c3a6f6d"
    "672e6f72672f4353492f4f49443a312e3000040000004f49440013000000"
    "0c000000010000000a000000000000000c0000007461726765745f6e616d"
    "6500150000005c000000010000002800000049444c3a6f6d672e6f72672f"
    "4353492f4753535f4e545f4578706f727465644e616d653a312e30001400"
    "00004753535f4e545f4578706f727465644e616d6500130000000c000000"
    "010000000a00000000000000110000007361735f636f6e746578745f6d65"
    "6368000000000f00000010040000010000002600000049444c3a6f6d672e"
    "6f72672f435349494f502f5341535f436f6e746578745365633a312e3000"
    "00000f0000005341535f436f6e7465787453656300000500000010000000"
    "7461726765745f737570706f727473001500000050000000010000002a00"
    "000049444c3a6f6d672e6f72672f435349494f502f4173736f6369617469"
    "6f6e4f7074696f6e733a312e30000000130000004173736f63696174696f"
    "6e4f7074696f6e73000004000000100000007461726765745f7265717569"
    "726573001500000050000000010000002a00000049444c3a6f6d672e6f72"
    "672f435349494f502f4173736f63696174696f6e4f7074696f6e733a312e"
    "30000000130000004173736f63696174696f6e4f7074696f6e7300000400"
    "00001600000070726976696c6567655f617574686f726974696573000000"
    "15000000ac010000010000003000000049444c3a6f6d672e6f72672f4353"
    "49494f502f53657276696365436f6e66696775726174696f6e4c6973743a"
    "312e30001900000053657276696365436f6e66696775726174696f6e4c69"
    "737400000000130000004c010000010000000f0000003c01000001000000"
    "2c00000049444c3a6f6d672e6f72672f435349494f502f53657276696365"
    "436f6e66696775726174696f6e3a312e3000150000005365727669636543"
    "6f6e66696775726174696f6e00000000020000000700000073796e746178"
    "00001500000060000000010000003200000049444c3a6f6d672e6f72672f"
    "435349494f502f53657276696365436f6e66696775726174696f6e53796e"
    "7461783a312e300000001b00000053657276696365436f6e666967757261"
    "74696f6e53796e746178000005000000050000006e616d65000000001500"
    "000060000000010000002b00000049444c3a6f6d672e6f72672f43534949"
    "4f502f5365727669636553706563696669634e616d653a312e3000001400"
    "00005365727669636553706563696669634e616d6500130000000c000000"
    "010000000a00000000000000000000001c000000737570706f727465645f"
    "6e616d696e675f6d656368616e69736d7300150000008400000001000000"
    "1c00000049444c3a6f6d672e6f72672f4353492f4f49444c6973743a312e"
    "3000080000004f49444c69737400130000004c0000000100000015000000"
    "3c000000010000001800000049444c3a6f6d672e6f72672f4353492f4f49"
    "443a312e3000040000004f494400130000000c000000010000000a000000"
    "000000000000000019000000737570706f727465645f6964656e74697479"
    "5f747970657300000000150000004c000000010000002600000049444c3a"
    "6f6d672e6f72672f4353492f4964656e74697479546f6b656e547970653a"
    "312e30000000120000004964656e74697479546f6b656e54797065000000"
    "0500000000000000";
    _marshaller_CSIIOP_CompoundSecMechList = new _Marshaller_CSIIOP_CompoundSecMechList;
    CSIIOP::_tc_TransportAddress = 
    "010000000f00000074000000010000002800000049444c3a6f6d672e6f72"
    "672f435349494f502f5472616e73706f7274416464726573733a312e3000"
    "110000005472616e73706f72744164647265737300000000020000000a00"
    "0000686f73745f6e616d65000000120000000000000005000000706f7274"
    "0000000004000000";
    _marshaller_CSIIOP_TransportAddress = new _Marshaller_CSIIOP_TransportAddress;
    CSIIOP::_tc_TransportAddressList = 
    "0100000015000000dc000000010000002c00000049444c3a6f6d672e6f72"
    "672f435349494f502f5472616e73706f7274416464726573734c6973743a"
    "312e3000150000005472616e73706f7274416464726573734c6973740000"
    "00001300000084000000010000000f000000740000000100000028000000"
    "49444c3a6f6d672e6f72672f435349494f502f5472616e73706f72744164"
    "64726573733a312e3000110000005472616e73706f727441646472657373"
    "00000000020000000a000000686f73745f6e616d65000000120000000000"
    "000005000000706f7274000000000400000000000000";
    CSIIOP::_tc_SECIOP_SEC_TRANS = 
    "010000000f000000e0020000010000002800000049444c3a6f6d672e6f72"
    "672f435349494f502f534543494f505f5345435f5452414e533a312e3000"
    "11000000534543494f505f5345435f5452414e5300000000050000001000"
    "00007461726765745f737570706f72747300150000005000000001000000"
    "2a00000049444c3a6f6d672e6f72672f435349494f502f4173736f636961"
    "74696f6e4f7074696f6e733a312e30000000130000004173736f63696174"
    "696f6e4f7074696f6e73000004000000100000007461726765745f726571"
    "7569726573001500000050000000010000002a00000049444c3a6f6d672e"
    "6f72672f435349494f502f4173736f63696174696f6e4f7074696f6e733a"
    "312e30000000130000004173736f63696174696f6e4f7074696f6e730000"
    "04000000090000006d6563685f6f696400000000150000003c0000000100"
    "00001800000049444c3a6f6d672e6f72672f4353492f4f49443a312e3000"
    "040000004f494400130000000c000000010000000a000000000000000c00"
    "00007461726765745f6e616d6500150000005c0000000100000028000000"
    "49444c3a6f6d672e6f72672f4353492f4753535f4e545f4578706f727465"
    "644e616d653a312e3000140000004753535f4e545f4578706f727465644e"
    "616d6500130000000c000000010000000a000000000000000a0000006164"
    "6472657373657300000015000000dc000000010000002c00000049444c3a"
    "6f6d672e6f72672f435349494f502f5472616e73706f7274416464726573"
    "734c6973743a312e3000150000005472616e73706f727441646472657373"
    "4c697374000000001300000084000000010000000f000000740000000100"
    "00002800000049444c3a6f6d672e6f72672f435349494f502f5472616e73"
    "706f7274416464726573733a312e3000110000005472616e73706f727441"
    "64647265737300000000020000000a000000686f73745f6e616d65000000"
    "120000000000000005000000706f7274000000000400000000000000";
    _marshaller_CSIIOP_SECIOP_SEC_TRANS = new _Marshaller_CSIIOP_SECIOP_SEC_TRANS;
    CSIIOP::_tc_TLS_SEC_TRANS = 
    "010000000f00000014020000010000002500000049444c3a6f6d672e6f72"
    "672f435349494f502f544c535f5345435f5452414e533a312e3000000000"
    "0e000000544c535f5345435f5452414e5300000003000000100000007461"
    "726765745f737570706f727473001500000050000000010000002a000000"
    "49444c3a6f6d672e6f72672f435349494f502f4173736f63696174696f6e"
    "4f7074696f6e733a312e30000000130000004173736f63696174696f6e4f"
    "7074696f6e73000004000000100000007461726765745f72657175697265"
    "73001500000050000000010000002a00000049444c3a6f6d672e6f72672f"
    "435349494f502f4173736f63696174696f6e4f7074696f6e733a312e3000"
    "0000130000004173736f63696174696f6e4f7074696f6e73000004000000"
    "0a00000061646472657373657300000015000000dc000000010000002c00"
    "000049444c3a6f6d672e6f72672f435349494f502f5472616e73706f7274"
    "416464726573734c6973743a312e3000150000005472616e73706f727441"
    "6464726573734c697374000000001300000084000000010000000f000000"
    "74000000010000002800000049444c3a6f6d672e6f72672f435349494f50"
    "2f5472616e73706f7274416464726573733a312e3000110000005472616e"
    "73706f72744164647265737300000000020000000a000000686f73745f6e"
    "616d65000000120000000000000005000000706f72740000000004000000"
    "00000000";
    _marshaller_CSIIOP_TLS_SEC_TRANS = new _Marshaller_CSIIOP_TLS_SEC_TRANS;
    _marshaller__seq_CSIIOP_ServiceConfiguration = new _Marshaller__seq_CSIIOP_ServiceConfiguration;
    _marshaller__seq_CSIIOP_CompoundSecMech = new _Marshaller__seq_CSIIOP_CompoundSecMech;
    _marshaller__seq_CSIIOP_TransportAddress = new _Marshaller__seq_CSIIOP_TransportAddress;
  }

  ~__tc_init_CSIIOP()
  {
    delete static_cast<_Marshaller_CSIIOP_ServiceConfiguration*>(_marshaller_CSIIOP_ServiceConfiguration);
    delete static_cast<_Marshaller_CSIIOP_AS_ContextSec*>(_marshaller_CSIIOP_AS_ContextSec);
    delete static_cast<_Marshaller_CSIIOP_SAS_ContextSec*>(_marshaller_CSIIOP_SAS_ContextSec);
    delete static_cast<_Marshaller_CSIIOP_CompoundSecMech*>(_marshaller_CSIIOP_CompoundSecMech);
    delete static_cast<_Marshaller_CSIIOP_CompoundSecMechList*>(_marshaller_CSIIOP_CompoundSecMechList);
    delete static_cast<_Marshaller_CSIIOP_TransportAddress*>(_marshaller_CSIIOP_TransportAddress);
    delete static_cast<_Marshaller_CSIIOP_SECIOP_SEC_TRANS*>(_marshaller_CSIIOP_SECIOP_SEC_TRANS);
    delete static_cast<_Marshaller_CSIIOP_TLS_SEC_TRANS*>(_marshaller_CSIIOP_TLS_SEC_TRANS);
    delete static_cast<_Marshaller__seq_CSIIOP_ServiceConfiguration*>(_marshaller__seq_CSIIOP_ServiceConfiguration);
    delete static_cast<_Marshaller__seq_CSIIOP_CompoundSecMech*>(_marshaller__seq_CSIIOP_CompoundSecMech);
    delete static_cast<_Marshaller__seq_CSIIOP_TransportAddress*>(_marshaller__seq_CSIIOP_TransportAddress);
  }
};

static __tc_init_CSIIOP __init_CSIIOP;

