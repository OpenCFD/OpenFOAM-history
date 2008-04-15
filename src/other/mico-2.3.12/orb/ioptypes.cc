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
CORBA::TypeCodeConst _tc_ServiceID;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_ServiceContext;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::ServiceContext::ServiceContext()
{
}

IOP::ServiceContext::ServiceContext( const ServiceContext& _s )
{
  context_id = ((ServiceContext&)_s).context_id;
  context_data = ((ServiceContext&)_s).context_data;
}

IOP::ServiceContext::~ServiceContext()
{
}

IOP::ServiceContext&
IOP::ServiceContext::operator=( const ServiceContext& _s )
{
  context_id = ((ServiceContext&)_s).context_id;
  context_data = ((ServiceContext&)_s).context_data;
  return *this;
}
#endif

class _Marshaller_IOP_ServiceContext : public ::CORBA::StaticTypeInfo {
    typedef IOP::ServiceContext _MICO_T;
  public:
    ~_Marshaller_IOP_ServiceContext();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_ServiceContext::~_Marshaller_IOP_ServiceContext()
{
}

::CORBA::StaticValueType _Marshaller_IOP_ServiceContext::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_ServiceContext::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_ServiceContext::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_ServiceContext::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->context_id ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->context_data ) &&
    dc.struct_end();
}

void _Marshaller_IOP_ServiceContext::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->context_id );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->context_data );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_ServiceContext::typecode()
{
  return IOP::_tc_ServiceContext;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_ServiceContext;

void operator<<=( CORBA::Any &_a, const IOP::ServiceContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_ServiceContext, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::ServiceContext *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::ServiceContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_ServiceContext, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::ServiceContext *&_s )
{
  return _a.to_static_any (_marshaller_IOP_ServiceContext, (void *&)_s);
}

namespace IOP
{
CORBA::TypeCodeConst _tc_ServiceContextList;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_ProfileId;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_TaggedProfile;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::TaggedProfile::TaggedProfile()
{
}

IOP::TaggedProfile::TaggedProfile( const TaggedProfile& _s )
{
  tag = ((TaggedProfile&)_s).tag;
  profile_data = ((TaggedProfile&)_s).profile_data;
}

IOP::TaggedProfile::~TaggedProfile()
{
}

IOP::TaggedProfile&
IOP::TaggedProfile::operator=( const TaggedProfile& _s )
{
  tag = ((TaggedProfile&)_s).tag;
  profile_data = ((TaggedProfile&)_s).profile_data;
  return *this;
}
#endif

class _Marshaller_IOP_TaggedProfile : public ::CORBA::StaticTypeInfo {
    typedef IOP::TaggedProfile _MICO_T;
  public:
    ~_Marshaller_IOP_TaggedProfile();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_TaggedProfile::~_Marshaller_IOP_TaggedProfile()
{
}

::CORBA::StaticValueType _Marshaller_IOP_TaggedProfile::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_TaggedProfile::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_TaggedProfile::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_TaggedProfile::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->tag ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->profile_data ) &&
    dc.struct_end();
}

void _Marshaller_IOP_TaggedProfile::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->tag );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->profile_data );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_TaggedProfile::typecode()
{
  return IOP::_tc_TaggedProfile;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_TaggedProfile;

void operator<<=( CORBA::Any &_a, const IOP::TaggedProfile &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_TaggedProfile, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::TaggedProfile *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::TaggedProfile &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_TaggedProfile, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::TaggedProfile *&_s )
{
  return _a.to_static_any (_marshaller_IOP_TaggedProfile, (void *&)_s);
}

namespace IOP
{
CORBA::TypeCodeConst _tc_IOR;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::IOR::IOR()
{
}

IOP::IOR::IOR( const IOR& _s )
{
  type_id = ((IOR&)_s).type_id;
  profiles = ((IOR&)_s).profiles;
}

IOP::IOR::~IOR()
{
}

IOP::IOR&
IOP::IOR::operator=( const IOR& _s )
{
  type_id = ((IOR&)_s).type_id;
  profiles = ((IOR&)_s).profiles;
  return *this;
}
#endif

class _Marshaller_IOP_IOR : public ::CORBA::StaticTypeInfo {
    typedef IOP::IOR _MICO_T;
  public:
    ~_Marshaller_IOP_IOR();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_IOR::~_Marshaller_IOP_IOR()
{
}

::CORBA::StaticValueType _Marshaller_IOP_IOR::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_IOR::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_IOR::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_IOR::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->type_id._for_demarshal() ) &&
    _marshaller__seq_IOP_TaggedProfile->demarshal( dc, &((_MICO_T*)v)->profiles ) &&
    dc.struct_end();
}

void _Marshaller_IOP_IOR::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->type_id.inout() );
  _marshaller__seq_IOP_TaggedProfile->marshal( ec, &((_MICO_T*)v)->profiles );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_IOR::typecode()
{
  return IOP::_tc_IOR;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_IOR;

void operator<<=( CORBA::Any &_a, const IOP::IOR &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_IOR, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::IOR *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::IOR &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_IOR, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::IOR *&_s )
{
  return _a.to_static_any (_marshaller_IOP_IOR, (void *&)_s);
}

namespace IOP
{
CORBA::TypeCodeConst _tc_ComponentId;
}

namespace IOP
{
CORBA::TypeCodeConst _tc_TaggedComponent;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IOP::TaggedComponent::TaggedComponent()
{
}

IOP::TaggedComponent::TaggedComponent( const TaggedComponent& _s )
{
  tag = ((TaggedComponent&)_s).tag;
  component_data = ((TaggedComponent&)_s).component_data;
}

IOP::TaggedComponent::~TaggedComponent()
{
}

IOP::TaggedComponent&
IOP::TaggedComponent::operator=( const TaggedComponent& _s )
{
  tag = ((TaggedComponent&)_s).tag;
  component_data = ((TaggedComponent&)_s).component_data;
  return *this;
}
#endif

class _Marshaller_IOP_TaggedComponent : public ::CORBA::StaticTypeInfo {
    typedef IOP::TaggedComponent _MICO_T;
  public:
    ~_Marshaller_IOP_TaggedComponent();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IOP_TaggedComponent::~_Marshaller_IOP_TaggedComponent()
{
}

::CORBA::StaticValueType _Marshaller_IOP_TaggedComponent::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IOP_TaggedComponent::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IOP_TaggedComponent::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IOP_TaggedComponent::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->tag ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->component_data ) &&
    dc.struct_end();
}

void _Marshaller_IOP_TaggedComponent::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->tag );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->component_data );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IOP_TaggedComponent::typecode()
{
  return IOP::_tc_TaggedComponent;
}

::CORBA::StaticTypeInfo *_marshaller_IOP_TaggedComponent;

void operator<<=( CORBA::Any &_a, const IOP::TaggedComponent &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_TaggedComponent, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IOP::TaggedComponent *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IOP::TaggedComponent &_s )
{
  CORBA::StaticAny _sa (_marshaller_IOP_TaggedComponent, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IOP::TaggedComponent *&_s )
{
  return _a.to_static_any (_marshaller_IOP_TaggedComponent, (void *&)_s);
}

namespace IOP
{
CORBA::TypeCodeConst _tc_MultipleComponentProfile;
}

namespace GIOP
{
CORBA::TypeCodeConst _tc_MsgType_1_1;
}

void operator<<=( CORBA::Any &_a, const GIOP::MsgType_1_1 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_MsgType_1_1, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, GIOP::MsgType_1_1 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_MsgType_1_1, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_GIOP_MsgType_1_1 : public ::CORBA::StaticTypeInfo {
    typedef GIOP::MsgType_1_1 _MICO_T;
  public:
    ~_Marshaller_GIOP_MsgType_1_1();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_GIOP_MsgType_1_1::~_Marshaller_GIOP_MsgType_1_1()
{
}

::CORBA::StaticValueType _Marshaller_GIOP_MsgType_1_1::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_GIOP_MsgType_1_1::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_GIOP_MsgType_1_1::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_GIOP_MsgType_1_1::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_GIOP_MsgType_1_1::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_GIOP_MsgType_1_1::typecode()
{
  return GIOP::_tc_MsgType_1_1;
}

::CORBA::StaticTypeInfo *_marshaller_GIOP_MsgType_1_1;

namespace GIOP
{
CORBA::TypeCodeConst _tc_MsgType;
}

namespace GIOP
{
CORBA::TypeCodeConst _tc_ReplyStatusType_1_2;
}

void operator<<=( CORBA::Any &_a, const GIOP::ReplyStatusType_1_2 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_ReplyStatusType_1_2, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, GIOP::ReplyStatusType_1_2 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_ReplyStatusType_1_2, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_GIOP_ReplyStatusType_1_2 : public ::CORBA::StaticTypeInfo {
    typedef GIOP::ReplyStatusType_1_2 _MICO_T;
  public:
    ~_Marshaller_GIOP_ReplyStatusType_1_2();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_GIOP_ReplyStatusType_1_2::~_Marshaller_GIOP_ReplyStatusType_1_2()
{
}

::CORBA::StaticValueType _Marshaller_GIOP_ReplyStatusType_1_2::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_GIOP_ReplyStatusType_1_2::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_GIOP_ReplyStatusType_1_2::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_GIOP_ReplyStatusType_1_2::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_GIOP_ReplyStatusType_1_2::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_GIOP_ReplyStatusType_1_2::typecode()
{
  return GIOP::_tc_ReplyStatusType_1_2;
}

::CORBA::StaticTypeInfo *_marshaller_GIOP_ReplyStatusType_1_2;

namespace GIOP
{
CORBA::TypeCodeConst _tc_ReplyStatusType;
}

namespace GIOP
{
CORBA::TypeCodeConst _tc_LocateStatusType_1_2;
}

void operator<<=( CORBA::Any &_a, const GIOP::LocateStatusType_1_2 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_LocateStatusType_1_2, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, GIOP::LocateStatusType_1_2 &_e )
{
  CORBA::StaticAny _sa (_marshaller_GIOP_LocateStatusType_1_2, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_GIOP_LocateStatusType_1_2 : public ::CORBA::StaticTypeInfo {
    typedef GIOP::LocateStatusType_1_2 _MICO_T;
  public:
    ~_Marshaller_GIOP_LocateStatusType_1_2();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_GIOP_LocateStatusType_1_2::~_Marshaller_GIOP_LocateStatusType_1_2()
{
}

::CORBA::StaticValueType _Marshaller_GIOP_LocateStatusType_1_2::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_GIOP_LocateStatusType_1_2::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_GIOP_LocateStatusType_1_2::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_GIOP_LocateStatusType_1_2::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_GIOP_LocateStatusType_1_2::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_GIOP_LocateStatusType_1_2::typecode()
{
  return GIOP::_tc_LocateStatusType_1_2;
}

::CORBA::StaticTypeInfo *_marshaller_GIOP_LocateStatusType_1_2;

namespace GIOP
{
CORBA::TypeCodeConst _tc_LocateStatusType;
}

namespace GIOP
{
CORBA::TypeCodeConst _tc_AddressingDisposition;
}

namespace IIOP
{
CORBA::TypeCodeConst _tc_Version;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
IIOP::Version::Version()
{
}

IIOP::Version::Version( const Version& _s )
{
  major = ((Version&)_s).major;
  minor = ((Version&)_s).minor;
}

IIOP::Version::~Version()
{
}

IIOP::Version&
IIOP::Version::operator=( const Version& _s )
{
  major = ((Version&)_s).major;
  minor = ((Version&)_s).minor;
  return *this;
}
#endif

class _Marshaller_IIOP_Version : public ::CORBA::StaticTypeInfo {
    typedef IIOP::Version _MICO_T;
  public:
    ~_Marshaller_IIOP_Version();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_IIOP_Version::~_Marshaller_IIOP_Version()
{
}

::CORBA::StaticValueType _Marshaller_IIOP_Version::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_IIOP_Version::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_IIOP_Version::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_IIOP_Version::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_char->demarshal( dc, &((_MICO_T*)v)->major ) &&
    CORBA::_stc_char->demarshal( dc, &((_MICO_T*)v)->minor ) &&
    dc.struct_end();
}

void _Marshaller_IIOP_Version::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_char->marshal( ec, &((_MICO_T*)v)->major );
  CORBA::_stc_char->marshal( ec, &((_MICO_T*)v)->minor );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_IIOP_Version::typecode()
{
  return IIOP::_tc_Version;
}

::CORBA::StaticTypeInfo *_marshaller_IIOP_Version;

void operator<<=( CORBA::Any &_a, const IIOP::Version &_s )
{
  CORBA::StaticAny _sa (_marshaller_IIOP_Version, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IIOP::Version *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IIOP::Version &_s )
{
  CORBA::StaticAny _sa (_marshaller_IIOP_Version, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IIOP::Version *&_s )
{
  return _a.to_static_any (_marshaller_IIOP_Version, (void *&)_s);
}

class _Marshaller__seq_IOP_ServiceContext : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_IOP_ServiceContext();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_IOP_ServiceContext::~_Marshaller__seq_IOP_ServiceContext()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_IOP_ServiceContext::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_IOP_ServiceContext::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_IOP_ServiceContext::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_IOP_ServiceContext::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_IOP_ServiceContext->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_IOP_ServiceContext::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_IOP_ServiceContext->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_ServiceContext::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000c0000000010000000f000000b0000000010000001b00"
    "000049444c3a494f502f53657276696365436f6e746578743a312e300000"
    "0f00000053657276696365436f6e746578740000020000000b000000636f"
    "6e746578745f696400001500000034000000010000001600000049444c3a"
    "494f502f5365727669636549443a312e300000000a000000536572766963"
    "654944000000050000000d000000636f6e746578745f6461746100000000"
    "130000000c000000010000000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_ServiceContext::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_IOP_ServiceContext;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_ServiceContext, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_ServiceContext, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_IOP_ServiceContext, (void *&)_s);
}


class _Marshaller__seq_IOP_TaggedProfile : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_IOP_TaggedProfile();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_IOP_TaggedProfile::~_Marshaller__seq_IOP_TaggedProfile()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_IOP_TaggedProfile::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_IOP_TaggedProfile::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_IOP_TaggedProfile::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_IOP_TaggedProfile::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_IOP_TaggedProfile->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_IOP_TaggedProfile::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_IOP_TaggedProfile->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_TaggedProfile::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000b8000000010000000f000000a8000000010000001a00"
    "000049444c3a494f502f54616767656450726f66696c653a312e30000000"
    "0e00000054616767656450726f66696c6500000002000000040000007461"
    "67001500000034000000010000001600000049444c3a494f502f50726f66"
    "696c6549643a312e300000000a00000050726f66696c6549640000000500"
    "00000d00000070726f66696c655f6461746100000000130000000c000000"
    "010000000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_TaggedProfile::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_IOP_TaggedProfile;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_TaggedProfile, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_TaggedProfile, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_IOP_TaggedProfile, (void *&)_s);
}


class _Marshaller__seq_IOP_TaggedComponent : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_IOP_TaggedComponent();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_IOP_TaggedComponent::~_Marshaller__seq_IOP_TaggedComponent()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_IOP_TaggedComponent::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_IOP_TaggedComponent::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_IOP_TaggedComponent::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_IOP_TaggedComponent::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_IOP_TaggedComponent->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_IOP_TaggedComponent::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_IOP_TaggedComponent->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_TaggedComponent::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000b8000000010000000f000000a8000000010000001c00"
    "000049444c3a494f502f546167676564436f6d706f6e656e743a312e3000"
    "10000000546167676564436f6d706f6e656e740002000000040000007461"
    "67001500000034000000010000001800000049444c3a494f502f436f6d70"
    "6f6e656e7449643a312e30000c000000436f6d706f6e656e744964000500"
    "00000f000000636f6d706f6e656e745f646174610000130000000c000000"
    "010000000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_IOP_TaggedComponent::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_IOP_TaggedComponent;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_TaggedComponent, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_IOP_TaggedComponent, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_IOP_TaggedComponent, (void *&)_s);
}


struct __tc_init_IOPTYPES {
  __tc_init_IOPTYPES()
  {
    IOP::_tc_ServiceID = 
    "010000001500000034000000010000001600000049444c3a494f502f5365"
    "727669636549443a312e300000000a000000536572766963654944000000"
    "05000000";
    IOP::_tc_ServiceContext = 
    "010000000f000000b0000000010000001b00000049444c3a494f502f5365"
    "7276696365436f6e746578743a312e3000000f0000005365727669636543"
    "6f6e746578740000020000000b000000636f6e746578745f696400001500"
    "000034000000010000001600000049444c3a494f502f5365727669636549"
    "443a312e300000000a000000536572766963654944000000050000000d00"
    "0000636f6e746578745f6461746100000000130000000c00000001000000"
    "0a00000000000000";
    _marshaller_IOP_ServiceContext = new _Marshaller_IOP_ServiceContext;
    IOP::_tc_ServiceContextList = 
    "010000001500000008010000010000001f00000049444c3a494f502f5365"
    "7276696365436f6e746578744c6973743a312e3000001300000053657276"
    "696365436f6e746578744c697374000013000000c0000000010000000f00"
    "0000b0000000010000001b00000049444c3a494f502f5365727669636543"
    "6f6e746578743a312e3000000f00000053657276696365436f6e74657874"
    "0000020000000b000000636f6e746578745f696400001500000034000000"
    "010000001600000049444c3a494f502f5365727669636549443a312e3000"
    "00000a000000536572766963654944000000050000000d000000636f6e74"
    "6578745f6461746100000000130000000c000000010000000a0000000000"
    "000000000000";
    IOP::_tc_ProfileId = 
    "010000001500000034000000010000001600000049444c3a494f502f5072"
    "6f66696c6549643a312e300000000a00000050726f66696c654964000000"
    "05000000";
    IOP::_tc_TaggedProfile = 
    "010000000f000000a8000000010000001a00000049444c3a494f502f5461"
    "6767656450726f66696c653a312e300000000e0000005461676765645072"
    "6f66696c6500000002000000040000007461670015000000340000000100"
    "00001600000049444c3a494f502f50726f66696c6549643a312e30000000"
    "0a00000050726f66696c654964000000050000000d00000070726f66696c"
    "655f6461746100000000130000000c000000010000000a00000000000000"
    ;
    _marshaller_IOP_TaggedProfile = new _Marshaller_IOP_TaggedProfile;
    IOP::_tc_IOR = 
    "010000000f00000008010000010000001000000049444c3a494f502f494f"
    "523a312e300004000000494f52000200000008000000747970655f696400"
    "12000000000000000900000070726f66696c65730000000013000000b800"
    "0000010000000f000000a8000000010000001a00000049444c3a494f502f"
    "54616767656450726f66696c653a312e300000000e000000546167676564"
    "50726f66696c650000000200000004000000746167001500000034000000"
    "010000001600000049444c3a494f502f50726f66696c6549643a312e3000"
    "00000a00000050726f66696c654964000000050000000d00000070726f66"
    "696c655f6461746100000000130000000c000000010000000a0000000000"
    "000000000000";
    _marshaller_IOP_IOR = new _Marshaller_IOP_IOR;
    IOP::_tc_ComponentId = 
    "010000001500000034000000010000001800000049444c3a494f502f436f"
    "6d706f6e656e7449643a312e30000c000000436f6d706f6e656e74496400"
    "05000000";
    IOP::_tc_TaggedComponent = 
    "010000000f000000a8000000010000001c00000049444c3a494f502f5461"
    "67676564436f6d706f6e656e743a312e300010000000546167676564436f"
    "6d706f6e656e740002000000040000007461670015000000340000000100"
    "00001800000049444c3a494f502f436f6d706f6e656e7449643a312e3000"
    "0c000000436f6d706f6e656e74496400050000000f000000636f6d706f6e"
    "656e745f646174610000130000000c000000010000000a00000000000000"
    ;
    _marshaller_IOP_TaggedComponent = new _Marshaller_IOP_TaggedComponent;
    IOP::_tc_MultipleComponentProfile = 
    "010000001500000010010000010000002500000049444c3a494f502f4d75"
    "6c7469706c65436f6d706f6e656e7450726f66696c653a312e3000000000"
    "190000004d756c7469706c65436f6d706f6e656e7450726f66696c650000"
    "000013000000b8000000010000000f000000a8000000010000001c000000"
    "49444c3a494f502f546167676564436f6d706f6e656e743a312e30001000"
    "0000546167676564436f6d706f6e656e7400020000000400000074616700"
    "1500000034000000010000001800000049444c3a494f502f436f6d706f6e"
    "656e7449643a312e30000c000000436f6d706f6e656e7449640005000000"
    "0f000000636f6d706f6e656e745f646174610000130000000c0000000100"
    "00000a0000000000000000000000";
    GIOP::_tc_MsgType_1_1 = 
    "0100000011000000bd000000010000001900000049444c3a47494f502f4d"
    "7367547970655f315f313a312e30000000000c0000004d7367547970655f"
    "315f310008000000080000005265717565737400060000005265706c7900"
    "00000e00000043616e63656c526571756573740000000e0000004c6f6361"
    "7465526571756573740000000c0000004c6f636174655265706c79001000"
    "0000436c6f7365436f6e6e656374696f6e000d0000004d65737361676545"
    "72726f720000000009000000467261676d656e7400";
    _marshaller_GIOP_MsgType_1_1 = new _Marshaller_GIOP_MsgType_1_1;
    GIOP::_tc_MsgType = 
    "0100000015000000f1000000010000001500000049444c3a47494f502f4d"
    "7367547970653a312e3000000000080000004d7367547970650011000000"
    "bd000000010000001900000049444c3a47494f502f4d7367547970655f31"
    "5f313a312e30000000000c0000004d7367547970655f315f310008000000"
    "080000005265717565737400060000005265706c790000000e0000004361"
    "6e63656c526571756573740000000e0000004c6f63617465526571756573"
    "740000000c0000004c6f636174655265706c790010000000436c6f736543"
    "6f6e6e656374696f6e000d0000004d6573736167654572726f7200000000"
    "09000000467261676d656e7400";
    GIOP::_tc_ReplyStatusType_1_2 = 
    "0100000011000000d6000000010000002100000049444c3a47494f502f52"
    "65706c79537461747573547970655f315f323a312e300000000014000000"
    "5265706c79537461747573547970655f315f3200060000000d0000004e4f"
    "5f455843455054494f4e000000000f000000555345525f45584345505449"
    "4f4e00001100000053595354454d5f455843455054494f4e000000001100"
    "00004c4f434154494f4e5f464f525741524400000000160000004c4f4341"
    "54494f4e5f464f52574152445f5045524d000000160000004e454544535f"
    "41444452455353494e475f4d4f444500";
    _marshaller_GIOP_ReplyStatusType_1_2 = new _Marshaller_GIOP_ReplyStatusType_1_2;
    GIOP::_tc_ReplyStatusType = 
    "01000000150000001a010000010000001d00000049444c3a47494f502f52"
    "65706c79537461747573547970653a312e3000000000100000005265706c"
    "79537461747573547970650011000000d600000001000000210000004944"
    "4c3a47494f502f5265706c79537461747573547970655f315f323a312e30"
    "00000000140000005265706c79537461747573547970655f315f32000600"
    "00000d0000004e4f5f455843455054494f4e000000000f00000055534552"
    "5f455843455054494f4e00001100000053595354454d5f45584345505449"
    "4f4e00000000110000004c4f434154494f4e5f464f525741524400000000"
    "160000004c4f434154494f4e5f464f52574152445f5045524d0000001600"
    "00004e454544535f41444452455353494e475f4d4f444500";
    GIOP::_tc_LocateStatusType_1_2 = 
    "0100000011000000d6000000010000002200000049444c3a47494f502f4c"
    "6f63617465537461747573547970655f315f323a312e3000000015000000"
    "4c6f63617465537461747573547970655f315f3200000000060000000f00"
    "0000554e4b4e4f574e5f4f424a45435400000c0000004f424a4543545f48"
    "455245000f0000004f424a4543545f464f52574152440000140000004f42"
    "4a4543545f464f52574152445f5045524d00150000004c4f435f53595354"
    "454d5f455843455054494f4e000000001a0000004c4f435f4e454544535f"
    "41444452455353494e475f4d4f444500";
    _marshaller_GIOP_LocateStatusType_1_2 = new _Marshaller_GIOP_LocateStatusType_1_2;
    GIOP::_tc_LocateStatusType = 
    "01000000150000001e010000010000001e00000049444c3a47494f502f4c"
    "6f63617465537461747573547970653a312e30000000110000004c6f6361"
    "7465537461747573547970650000000011000000d6000000010000002200"
    "000049444c3a47494f502f4c6f63617465537461747573547970655f315f"
    "323a312e30000000150000004c6f63617465537461747573547970655f31"
    "5f3200000000060000000f000000554e4b4e4f574e5f4f424a4543540000"
    "0c0000004f424a4543545f48455245000f0000004f424a4543545f464f52"
    "574152440000140000004f424a4543545f464f52574152445f5045524d00"
    "150000004c4f435f53595354454d5f455843455054494f4e000000001a00"
    "00004c4f435f4e454544535f41444452455353494e475f4d4f444500";
    GIOP::_tc_AddressingDisposition = 
    "01000000150000004c000000010000002300000049444c3a47494f502f41"
    "646472657373696e67446973706f736974696f6e3a312e30000016000000"
    "41646472657373696e67446973706f736974696f6e00000002000000";
    IIOP::_tc_Version = 
    "010000000f00000050000000010000001500000049444c3a49494f502f56"
    "657273696f6e3a312e30000000000800000056657273696f6e0002000000"
    "060000006d616a6f7200000009000000060000006d696e6f720000000900"
    "0000";
    _marshaller_IIOP_Version = new _Marshaller_IIOP_Version;
    _marshaller__seq_IOP_ServiceContext = new _Marshaller__seq_IOP_ServiceContext;
    _marshaller__seq_IOP_TaggedProfile = new _Marshaller__seq_IOP_TaggedProfile;
    _marshaller__seq_IOP_TaggedComponent = new _Marshaller__seq_IOP_TaggedComponent;
  }

  ~__tc_init_IOPTYPES()
  {
    delete static_cast<_Marshaller_IOP_ServiceContext*>(_marshaller_IOP_ServiceContext);
    delete static_cast<_Marshaller_IOP_TaggedProfile*>(_marshaller_IOP_TaggedProfile);
    delete static_cast<_Marshaller_IOP_IOR*>(_marshaller_IOP_IOR);
    delete static_cast<_Marshaller_IOP_TaggedComponent*>(_marshaller_IOP_TaggedComponent);
    delete static_cast<_Marshaller_GIOP_MsgType_1_1*>(_marshaller_GIOP_MsgType_1_1);
    delete static_cast<_Marshaller_GIOP_ReplyStatusType_1_2*>(_marshaller_GIOP_ReplyStatusType_1_2);
    delete static_cast<_Marshaller_GIOP_LocateStatusType_1_2*>(_marshaller_GIOP_LocateStatusType_1_2);
    delete static_cast<_Marshaller_IIOP_Version*>(_marshaller_IIOP_Version);
    delete static_cast<_Marshaller__seq_IOP_ServiceContext*>(_marshaller__seq_IOP_ServiceContext);
    delete static_cast<_Marshaller__seq_IOP_TaggedProfile*>(_marshaller__seq_IOP_TaggedProfile);
    delete static_cast<_Marshaller__seq_IOP_TaggedComponent*>(_marshaller__seq_IOP_TaggedComponent);
  }
};

static __tc_init_IOPTYPES __init_IOPTYPES;

