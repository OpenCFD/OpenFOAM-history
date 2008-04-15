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
namespace CSI
{
CORBA::TypeCodeConst _tc_X509CertificateChain;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_X501DistinguishedName;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_UTF8String;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_GSSToken;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_GSS_NT_ExportedName;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_GSS_NT_ExportedNameList;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_MsgType;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_ContextId;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_AuthorizationElementType;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_AuthorizationElementContents;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_AuthorizationElement;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::AuthorizationElement::AuthorizationElement()
{
}

CSI::AuthorizationElement::AuthorizationElement( const AuthorizationElement& _s )
{
  the_type = ((AuthorizationElement&)_s).the_type;
  the_element = ((AuthorizationElement&)_s).the_element;
}

CSI::AuthorizationElement::~AuthorizationElement()
{
}

CSI::AuthorizationElement&
CSI::AuthorizationElement::operator=( const AuthorizationElement& _s )
{
  the_type = ((AuthorizationElement&)_s).the_type;
  the_element = ((AuthorizationElement&)_s).the_element;
  return *this;
}
#endif

class _Marshaller_CSI_AuthorizationElement : public ::CORBA::StaticTypeInfo {
    typedef CSI::AuthorizationElement _MICO_T;
  public:
    ~_Marshaller_CSI_AuthorizationElement();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_AuthorizationElement::~_Marshaller_CSI_AuthorizationElement()
{
}

::CORBA::StaticValueType _Marshaller_CSI_AuthorizationElement::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_AuthorizationElement::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_AuthorizationElement::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_AuthorizationElement::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->the_type ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->the_element ) &&
    dc.struct_end();
}

void _Marshaller_CSI_AuthorizationElement::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->the_type );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->the_element );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_AuthorizationElement::typecode()
{
  return CSI::_tc_AuthorizationElement;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_AuthorizationElement;

void operator<<=( CORBA::Any &_a, const CSI::AuthorizationElement &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_AuthorizationElement, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::AuthorizationElement *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::AuthorizationElement &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_AuthorizationElement, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::AuthorizationElement *&_s )
{
  return _a.to_static_any (_marshaller_CSI_AuthorizationElement, (void *&)_s);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_AuthorizationToken;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_IdentityTokenType;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_IdentityExtension;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_IdentityToken;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::IdentityToken::IdentityToken()
{
}

CSI::IdentityToken::IdentityToken( const IdentityToken&_u )
{
  _discriminator = _u._discriminator;
  _m.absent = ((IdentityToken&)_u)._m.absent;
  _m.anonymous = ((IdentityToken&)_u)._m.anonymous;
  _m.principal_name = ((IdentityToken&)_u)._m.principal_name;
  _m.certificate_chain = ((IdentityToken&)_u)._m.certificate_chain;
  _m.dn = ((IdentityToken&)_u)._m.dn;
  _m.id = ((IdentityToken&)_u)._m.id;
}

CSI::IdentityToken::~IdentityToken()
{
}

CSI::IdentityToken&
CSI::IdentityToken::operator=( const IdentityToken&_u )
{
  _discriminator = _u._discriminator;
  _m.absent = ((IdentityToken&)_u)._m.absent;
  _m.anonymous = ((IdentityToken&)_u)._m.anonymous;
  _m.principal_name = ((IdentityToken&)_u)._m.principal_name;
  _m.certificate_chain = ((IdentityToken&)_u)._m.certificate_chain;
  _m.dn = ((IdentityToken&)_u)._m.dn;
  _m.id = ((IdentityToken&)_u)._m.id;
  return *this;
}
#endif

void CSI::IdentityToken::_d( CSI::IdentityTokenType _p )
{
  _discriminator = _p;
}

CSI::IdentityTokenType CSI::IdentityToken::_d() const
{
  return _discriminator;
}

void CSI::IdentityToken::absent( CORBA::Boolean _p )
{
  _discriminator = 0;
  _m.absent = _p;
}

CORBA::Boolean CSI::IdentityToken::absent() const
{
  return (CORBA::Boolean)_m.absent;
}

void CSI::IdentityToken::anonymous( CORBA::Boolean _p )
{
  _discriminator = 1;
  _m.anonymous = _p;
}

CORBA::Boolean CSI::IdentityToken::anonymous() const
{
  return (CORBA::Boolean)_m.anonymous;
}

void CSI::IdentityToken::principal_name( const CSI::GSS_NT_ExportedName& _p )
{
  _discriminator = 2;
  _m.principal_name = _p;
}

const CSI::GSS_NT_ExportedName& CSI::IdentityToken::principal_name() const
{
  return (CSI::GSS_NT_ExportedName&) _m.principal_name;
}

CSI::GSS_NT_ExportedName& CSI::IdentityToken::principal_name()
{
  return _m.principal_name;
}

void CSI::IdentityToken::certificate_chain( const CSI::X509CertificateChain& _p )
{
  _discriminator = 4;
  _m.certificate_chain = _p;
}

const CSI::X509CertificateChain& CSI::IdentityToken::certificate_chain() const
{
  return (CSI::X509CertificateChain&) _m.certificate_chain;
}

CSI::X509CertificateChain& CSI::IdentityToken::certificate_chain()
{
  return _m.certificate_chain;
}

void CSI::IdentityToken::dn( const CSI::X501DistinguishedName& _p )
{
  _discriminator = 8;
  _m.dn = _p;
}

const CSI::X501DistinguishedName& CSI::IdentityToken::dn() const
{
  return (CSI::X501DistinguishedName&) _m.dn;
}

CSI::X501DistinguishedName& CSI::IdentityToken::dn()
{
  return _m.dn;
}

void CSI::IdentityToken::id( const CSI::IdentityExtension& _p )
{
  _discriminator = 3;
  _m.id = _p;
}

const CSI::IdentityExtension& CSI::IdentityToken::id() const
{
  return (CSI::IdentityExtension&) _m.id;
}

CSI::IdentityExtension& CSI::IdentityToken::id()
{
  return _m.id;
}

class _Marshaller_CSI_IdentityToken : public ::CORBA::StaticTypeInfo {
    typedef CSI::IdentityToken _MICO_T;
  public:
    ~_Marshaller_CSI_IdentityToken();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_IdentityToken::~_Marshaller_CSI_IdentityToken()
{
}

::CORBA::StaticValueType _Marshaller_CSI_IdentityToken::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_IdentityToken::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_IdentityToken::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_IdentityToken::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  if( !dc.union_begin() )
    return FALSE;
  if( !CORBA::_stc_ulong->demarshal( dc, &((_MICO_T *) v)->_discriminator ) )
    return FALSE;
  switch( ((_MICO_T *) v)->_discriminator ) {
    case 0:
      if( !CORBA::_stc_boolean->demarshal( dc, &((_MICO_T *) v)->_m.absent ) )
        return FALSE;
      break;
    case 1:
      if( !CORBA::_stc_boolean->demarshal( dc, &((_MICO_T *) v)->_m.anonymous ) )
        return FALSE;
      break;
    case 2:
      if( !CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T *) v)->_m.principal_name ) )
        return FALSE;
      break;
    case 4:
      if( !CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T *) v)->_m.certificate_chain ) )
        return FALSE;
      break;
    case 8:
      if( !CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T *) v)->_m.dn ) )
        return FALSE;
      break;
    default:
      if( !CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T *) v)->_m.id ) )
        return FALSE;
      break;
  }
  return dc.union_end();
}

void _Marshaller_CSI_IdentityToken::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.union_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T *) v)->_discriminator );
  switch( ((_MICO_T *) v)->_discriminator ) {
    case 0:
      CORBA::_stc_boolean->marshal( ec, &((_MICO_T *) v)->_m.absent );
      break;
    case 1:
      CORBA::_stc_boolean->marshal( ec, &((_MICO_T *) v)->_m.anonymous );
      break;
    case 2:
      CORBA::_stcseq_octet->marshal( ec, &((_MICO_T *) v)->_m.principal_name );
      break;
    case 4:
      CORBA::_stcseq_octet->marshal( ec, &((_MICO_T *) v)->_m.certificate_chain );
      break;
    case 8:
      CORBA::_stcseq_octet->marshal( ec, &((_MICO_T *) v)->_m.dn );
      break;
    default:
      CORBA::_stcseq_octet->marshal( ec, &((_MICO_T *) v)->_m.id );
      break;
  }
  ec.union_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_IdentityToken::typecode()
{
  return CSI::_tc_IdentityToken;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_IdentityToken;

void operator<<=( CORBA::Any &_a, const CSI::IdentityToken &_u )
{
  CORBA::StaticAny _sa (_marshaller_CSI_IdentityToken, &_u);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::IdentityToken *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::IdentityToken &_u )
{
  CORBA::StaticAny _sa (_marshaller_CSI_IdentityToken, &_u);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::IdentityToken *&_u )
{
  return _a.to_static_any (_marshaller_CSI_IdentityToken, (void *&)_u);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_EstablishContext;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::EstablishContext::EstablishContext()
{
}

CSI::EstablishContext::EstablishContext( const EstablishContext& _s )
{
  client_context_id = ((EstablishContext&)_s).client_context_id;
  authorization_token = ((EstablishContext&)_s).authorization_token;
  identity_token = ((EstablishContext&)_s).identity_token;
  client_authentication_token = ((EstablishContext&)_s).client_authentication_token;
}

CSI::EstablishContext::~EstablishContext()
{
}

CSI::EstablishContext&
CSI::EstablishContext::operator=( const EstablishContext& _s )
{
  client_context_id = ((EstablishContext&)_s).client_context_id;
  authorization_token = ((EstablishContext&)_s).authorization_token;
  identity_token = ((EstablishContext&)_s).identity_token;
  client_authentication_token = ((EstablishContext&)_s).client_authentication_token;
  return *this;
}
#endif

class _Marshaller_CSI_EstablishContext : public ::CORBA::StaticTypeInfo {
    typedef CSI::EstablishContext _MICO_T;
  public:
    ~_Marshaller_CSI_EstablishContext();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_EstablishContext::~_Marshaller_CSI_EstablishContext()
{
}

::CORBA::StaticValueType _Marshaller_CSI_EstablishContext::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_EstablishContext::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_EstablishContext::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_EstablishContext::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->client_context_id ) &&
    _marshaller__seq_CSI_AuthorizationElement->demarshal( dc, &((_MICO_T*)v)->authorization_token ) &&
    _marshaller_CSI_IdentityToken->demarshal( dc, &((_MICO_T*)v)->identity_token ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->client_authentication_token ) &&
    dc.struct_end();
}

void _Marshaller_CSI_EstablishContext::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->client_context_id );
  _marshaller__seq_CSI_AuthorizationElement->marshal( ec, &((_MICO_T*)v)->authorization_token );
  _marshaller_CSI_IdentityToken->marshal( ec, &((_MICO_T*)v)->identity_token );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->client_authentication_token );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_EstablishContext::typecode()
{
  return CSI::_tc_EstablishContext;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_EstablishContext;

void operator<<=( CORBA::Any &_a, const CSI::EstablishContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_EstablishContext, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::EstablishContext *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::EstablishContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_EstablishContext, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::EstablishContext *&_s )
{
  return _a.to_static_any (_marshaller_CSI_EstablishContext, (void *&)_s);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_CompleteEstablishContext;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::CompleteEstablishContext::CompleteEstablishContext()
{
}

CSI::CompleteEstablishContext::CompleteEstablishContext( const CompleteEstablishContext& _s )
{
  client_context_id = ((CompleteEstablishContext&)_s).client_context_id;
  context_stateful = ((CompleteEstablishContext&)_s).context_stateful;
  final_context_token = ((CompleteEstablishContext&)_s).final_context_token;
}

CSI::CompleteEstablishContext::~CompleteEstablishContext()
{
}

CSI::CompleteEstablishContext&
CSI::CompleteEstablishContext::operator=( const CompleteEstablishContext& _s )
{
  client_context_id = ((CompleteEstablishContext&)_s).client_context_id;
  context_stateful = ((CompleteEstablishContext&)_s).context_stateful;
  final_context_token = ((CompleteEstablishContext&)_s).final_context_token;
  return *this;
}
#endif

class _Marshaller_CSI_CompleteEstablishContext : public ::CORBA::StaticTypeInfo {
    typedef CSI::CompleteEstablishContext _MICO_T;
  public:
    ~_Marshaller_CSI_CompleteEstablishContext();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_CompleteEstablishContext::~_Marshaller_CSI_CompleteEstablishContext()
{
}

::CORBA::StaticValueType _Marshaller_CSI_CompleteEstablishContext::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_CompleteEstablishContext::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_CompleteEstablishContext::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_CompleteEstablishContext::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->client_context_id ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->context_stateful ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->final_context_token ) &&
    dc.struct_end();
}

void _Marshaller_CSI_CompleteEstablishContext::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->client_context_id );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->context_stateful );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->final_context_token );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_CompleteEstablishContext::typecode()
{
  return CSI::_tc_CompleteEstablishContext;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_CompleteEstablishContext;

void operator<<=( CORBA::Any &_a, const CSI::CompleteEstablishContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_CompleteEstablishContext, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::CompleteEstablishContext *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::CompleteEstablishContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_CompleteEstablishContext, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::CompleteEstablishContext *&_s )
{
  return _a.to_static_any (_marshaller_CSI_CompleteEstablishContext, (void *&)_s);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_ContextError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::ContextError::ContextError()
{
}

CSI::ContextError::ContextError( const ContextError& _s )
{
  client_context_id = ((ContextError&)_s).client_context_id;
  major_status = ((ContextError&)_s).major_status;
  minor_status = ((ContextError&)_s).minor_status;
  error_token = ((ContextError&)_s).error_token;
}

CSI::ContextError::~ContextError()
{
}

CSI::ContextError&
CSI::ContextError::operator=( const ContextError& _s )
{
  client_context_id = ((ContextError&)_s).client_context_id;
  major_status = ((ContextError&)_s).major_status;
  minor_status = ((ContextError&)_s).minor_status;
  error_token = ((ContextError&)_s).error_token;
  return *this;
}
#endif

class _Marshaller_CSI_ContextError : public ::CORBA::StaticTypeInfo {
    typedef CSI::ContextError _MICO_T;
  public:
    ~_Marshaller_CSI_ContextError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_ContextError::~_Marshaller_CSI_ContextError()
{
}

::CORBA::StaticValueType _Marshaller_CSI_ContextError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_ContextError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_ContextError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_ContextError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->client_context_id ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->major_status ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->minor_status ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->error_token ) &&
    dc.struct_end();
}

void _Marshaller_CSI_ContextError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->client_context_id );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->major_status );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->minor_status );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->error_token );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_ContextError::typecode()
{
  return CSI::_tc_ContextError;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_ContextError;

void operator<<=( CORBA::Any &_a, const CSI::ContextError &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_ContextError, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::ContextError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::ContextError &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_ContextError, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::ContextError *&_s )
{
  return _a.to_static_any (_marshaller_CSI_ContextError, (void *&)_s);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_MessageInContext;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::MessageInContext::MessageInContext()
{
}

CSI::MessageInContext::MessageInContext( const MessageInContext& _s )
{
  client_context_id = ((MessageInContext&)_s).client_context_id;
  discard_context = ((MessageInContext&)_s).discard_context;
}

CSI::MessageInContext::~MessageInContext()
{
}

CSI::MessageInContext&
CSI::MessageInContext::operator=( const MessageInContext& _s )
{
  client_context_id = ((MessageInContext&)_s).client_context_id;
  discard_context = ((MessageInContext&)_s).discard_context;
  return *this;
}
#endif

class _Marshaller_CSI_MessageInContext : public ::CORBA::StaticTypeInfo {
    typedef CSI::MessageInContext _MICO_T;
  public:
    ~_Marshaller_CSI_MessageInContext();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_MessageInContext::~_Marshaller_CSI_MessageInContext()
{
}

::CORBA::StaticValueType _Marshaller_CSI_MessageInContext::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_MessageInContext::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_MessageInContext::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_MessageInContext::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->client_context_id ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->discard_context ) &&
    dc.struct_end();
}

void _Marshaller_CSI_MessageInContext::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->client_context_id );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->discard_context );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_MessageInContext::typecode()
{
  return CSI::_tc_MessageInContext;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_MessageInContext;

void operator<<=( CORBA::Any &_a, const CSI::MessageInContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_MessageInContext, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::MessageInContext *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::MessageInContext &_s )
{
  CORBA::StaticAny _sa (_marshaller_CSI_MessageInContext, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::MessageInContext *&_s )
{
  return _a.to_static_any (_marshaller_CSI_MessageInContext, (void *&)_s);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_SASContextBody;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CSI::SASContextBody::SASContextBody()
{
}

CSI::SASContextBody::SASContextBody( const SASContextBody&_u )
{
  _discriminator = _u._discriminator;
  _m.establish_msg = ((SASContextBody&)_u)._m.establish_msg;
  _m.complete_msg = ((SASContextBody&)_u)._m.complete_msg;
  _m.error_msg = ((SASContextBody&)_u)._m.error_msg;
  _m.in_context_msg = ((SASContextBody&)_u)._m.in_context_msg;
}

CSI::SASContextBody::~SASContextBody()
{
}

CSI::SASContextBody&
CSI::SASContextBody::operator=( const SASContextBody&_u )
{
  _discriminator = _u._discriminator;
  _m.establish_msg = ((SASContextBody&)_u)._m.establish_msg;
  _m.complete_msg = ((SASContextBody&)_u)._m.complete_msg;
  _m.error_msg = ((SASContextBody&)_u)._m.error_msg;
  _m.in_context_msg = ((SASContextBody&)_u)._m.in_context_msg;
  return *this;
}
#endif

void CSI::SASContextBody::_d( CSI::MsgType _p )
{
  _discriminator = _p;
}

CSI::MsgType CSI::SASContextBody::_d() const
{
  return _discriminator;
}

void CSI::SASContextBody::establish_msg( const CSI::EstablishContext& _p )
{
  _discriminator = 0;
  _m.establish_msg = _p;
}

const CSI::EstablishContext& CSI::SASContextBody::establish_msg() const
{
  return (CSI::EstablishContext&) _m.establish_msg;
}

CSI::EstablishContext& CSI::SASContextBody::establish_msg()
{
  return _m.establish_msg;
}

void CSI::SASContextBody::complete_msg( const CSI::CompleteEstablishContext& _p )
{
  _discriminator = 1;
  _m.complete_msg = _p;
}

const CSI::CompleteEstablishContext& CSI::SASContextBody::complete_msg() const
{
  return (CSI::CompleteEstablishContext&) _m.complete_msg;
}

CSI::CompleteEstablishContext& CSI::SASContextBody::complete_msg()
{
  return _m.complete_msg;
}

void CSI::SASContextBody::error_msg( const CSI::ContextError& _p )
{
  _discriminator = 4;
  _m.error_msg = _p;
}

const CSI::ContextError& CSI::SASContextBody::error_msg() const
{
  return (CSI::ContextError&) _m.error_msg;
}

CSI::ContextError& CSI::SASContextBody::error_msg()
{
  return _m.error_msg;
}

void CSI::SASContextBody::in_context_msg( const CSI::MessageInContext& _p )
{
  _discriminator = 5;
  _m.in_context_msg = _p;
}

const CSI::MessageInContext& CSI::SASContextBody::in_context_msg() const
{
  return (CSI::MessageInContext&) _m.in_context_msg;
}

CSI::MessageInContext& CSI::SASContextBody::in_context_msg()
{
  return _m.in_context_msg;
}

void CSI::SASContextBody::_default()
{
  _discriminator = 2;
}

class _Marshaller_CSI_SASContextBody : public ::CORBA::StaticTypeInfo {
    typedef CSI::SASContextBody _MICO_T;
  public:
    ~_Marshaller_CSI_SASContextBody();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CSI_SASContextBody::~_Marshaller_CSI_SASContextBody()
{
}

::CORBA::StaticValueType _Marshaller_CSI_SASContextBody::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CSI_SASContextBody::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CSI_SASContextBody::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CSI_SASContextBody::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  if( !dc.union_begin() )
    return FALSE;
  if( !CORBA::_stc_short->demarshal( dc, &((_MICO_T *) v)->_discriminator ) )
    return FALSE;
  switch( ((_MICO_T *) v)->_discriminator ) {
    case 0:
      if( !_marshaller_CSI_EstablishContext->demarshal( dc, &((_MICO_T *) v)->_m.establish_msg ) )
        return FALSE;
      break;
    case 1:
      if( !_marshaller_CSI_CompleteEstablishContext->demarshal( dc, &((_MICO_T *) v)->_m.complete_msg ) )
        return FALSE;
      break;
    case 4:
      if( !_marshaller_CSI_ContextError->demarshal( dc, &((_MICO_T *) v)->_m.error_msg ) )
        return FALSE;
      break;
    case 5:
      if( !_marshaller_CSI_MessageInContext->demarshal( dc, &((_MICO_T *) v)->_m.in_context_msg ) )
        return FALSE;
      break;
  }
  return dc.union_end();
}

void _Marshaller_CSI_SASContextBody::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.union_begin();
  CORBA::_stc_short->marshal( ec, &((_MICO_T *) v)->_discriminator );
  switch( ((_MICO_T *) v)->_discriminator ) {
    case 0:
      _marshaller_CSI_EstablishContext->marshal( ec, &((_MICO_T *) v)->_m.establish_msg );
      break;
    case 1:
      _marshaller_CSI_CompleteEstablishContext->marshal( ec, &((_MICO_T *) v)->_m.complete_msg );
      break;
    case 4:
      _marshaller_CSI_ContextError->marshal( ec, &((_MICO_T *) v)->_m.error_msg );
      break;
    case 5:
      _marshaller_CSI_MessageInContext->marshal( ec, &((_MICO_T *) v)->_m.in_context_msg );
      break;
  }
  ec.union_end();
}

::CORBA::TypeCode_ptr _Marshaller_CSI_SASContextBody::typecode()
{
  return CSI::_tc_SASContextBody;
}

::CORBA::StaticTypeInfo *_marshaller_CSI_SASContextBody;

void operator<<=( CORBA::Any &_a, const CSI::SASContextBody &_u )
{
  CORBA::StaticAny _sa (_marshaller_CSI_SASContextBody, &_u);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CSI::SASContextBody *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CSI::SASContextBody &_u )
{
  CORBA::StaticAny _sa (_marshaller_CSI_SASContextBody, &_u);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CSI::SASContextBody *&_u )
{
  return _a.to_static_any (_marshaller_CSI_SASContextBody, (void *&)_u);
}

namespace CSI
{
CORBA::TypeCodeConst _tc_StringOID;
}

class _Marshaller__seq_CSI_AuthorizationElement : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CSI_AuthorizationElement();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CSI_AuthorizationElement::~_Marshaller__seq_CSI_AuthorizationElement()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CSI_AuthorizationElement::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CSI_AuthorizationElement::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CSI_AuthorizationElement::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CSI_AuthorizationElement::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CSI_AuthorizationElement->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CSI_AuthorizationElement::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CSI_AuthorizationElement->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSI_AuthorizationElement::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000064010000010000000f00000054010000010000002900"
    "000049444c3a6f6d672e6f72672f4353492f417574686f72697a6174696f"
    "6e456c656d656e743a312e300000000015000000417574686f72697a6174"
    "696f6e456c656d656e740000000002000000090000007468655f74797065"
    "00000000150000005c000000010000002d00000049444c3a6f6d672e6f72"
    "672f4353492f417574686f72697a6174696f6e456c656d656e7454797065"
    "3a312e300000000019000000417574686f72697a6174696f6e456c656d65"
    "6e745479706500000000050000000c0000007468655f656c656d656e7400"
    "1500000074000000010000003100000049444c3a6f6d672e6f72672f4353"
    "492f417574686f72697a6174696f6e456c656d656e74436f6e74656e7473"
    "3a312e30000000001d000000417574686f72697a6174696f6e456c656d65"
    "6e74436f6e74656e747300000000130000000c000000010000000a000000"
    "0000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CSI_AuthorizationElement::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CSI_AuthorizationElement;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSI_AuthorizationElement, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CSI_AuthorizationElement, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CSI_AuthorizationElement, (void *&)_s);
}


struct __tc_init_CSI {
  __tc_init_CSI()
  {
    CSI::_tc_X509CertificateChain = 
    "010000001500000064000000010000002900000049444c3a6f6d672e6f72"
    "672f4353492f583530394365727469666963617465436861696e3a312e30"
    "0000000015000000583530394365727469666963617465436861696e0000"
    "0000130000000c000000010000000a00000000000000";
    CSI::_tc_X501DistinguishedName = 
    "010000001500000064000000010000002a00000049444c3a6f6d672e6f72"
    "672f4353492f5835303144697374696e677569736865644e616d653a312e"
    "30000000160000005835303144697374696e677569736865644e616d6500"
    "0000130000000c000000010000000a00000000000000";
    CSI::_tc_UTF8String = 
    "01000000150000004c000000010000001f00000049444c3a6f6d672e6f72"
    "672f4353492f55544638537472696e673a312e3000000b00000055544638"
    "537472696e670000130000000c000000010000000a00000000000000";
    CSI::_tc_GSSToken = 
    "01000000150000004c000000010000001d00000049444c3a6f6d672e6f72"
    "672f4353492f475353546f6b656e3a312e30000000000900000047535354"
    "6f6b656e00000000130000000c000000010000000a00000000000000";
    CSI::_tc_GSS_NT_ExportedName = 
    "01000000150000005c000000010000002800000049444c3a6f6d672e6f72"
    "672f4353492f4753535f4e545f4578706f727465644e616d653a312e3000"
    "140000004753535f4e545f4578706f727465644e616d6500130000000c00"
    "0000010000000a00000000000000";
    CSI::_tc_GSS_NT_ExportedNameList = 
    "0100000015000000c4000000010000002c00000049444c3a6f6d672e6f72"
    "672f4353492f4753535f4e545f4578706f727465644e616d654c6973743a"
    "312e3000180000004753535f4e545f4578706f727465644e616d654c6973"
    "7400130000006c00000001000000150000005c0000000100000028000000"
    "49444c3a6f6d672e6f72672f4353492f4753535f4e545f4578706f727465"
    "644e616d653a312e3000140000004753535f4e545f4578706f727465644e"
    "616d6500130000000c000000010000000a0000000000000000000000";
    CSI::_tc_MsgType = 
    "010000001500000034000000010000001c00000049444c3a6f6d672e6f72"
    "672f4353492f4d7367547970653a312e3000080000004d73675479706500"
    "02000000";
    CSI::_tc_ContextId = 
    "01000000150000003c000000010000001e00000049444c3a6f6d672e6f72"
    "672f4353492f436f6e7465787449643a312e300000000a000000436f6e74"
    "657874496400000018000000";
    CSI::_tc_AuthorizationElementType = 
    "01000000150000005c000000010000002d00000049444c3a6f6d672e6f72"
    "672f4353492f417574686f72697a6174696f6e456c656d656e7454797065"
    "3a312e300000000019000000417574686f72697a6174696f6e456c656d65"
    "6e74547970650000000005000000";
    CSI::_tc_AuthorizationElementContents = 
    "010000001500000074000000010000003100000049444c3a6f6d672e6f72"
    "672f4353492f417574686f72697a6174696f6e456c656d656e74436f6e74"
    "656e74733a312e30000000001d000000417574686f72697a6174696f6e45"
    "6c656d656e74436f6e74656e747300000000130000000c00000001000000"
    "0a00000000000000";
    CSI::_tc_AuthorizationElement = 
    "010000000f00000054010000010000002900000049444c3a6f6d672e6f72"
    "672f4353492f417574686f72697a6174696f6e456c656d656e743a312e30"
    "0000000015000000417574686f72697a6174696f6e456c656d656e740000"
    "000002000000090000007468655f7479706500000000150000005c000000"
    "010000002d00000049444c3a6f6d672e6f72672f4353492f417574686f72"
    "697a6174696f6e456c656d656e74547970653a312e300000000019000000"
    "417574686f72697a6174696f6e456c656d656e7454797065000000000500"
    "00000c0000007468655f656c656d656e7400150000007400000001000000"
    "3100000049444c3a6f6d672e6f72672f4353492f417574686f72697a6174"
    "696f6e456c656d656e74436f6e74656e74733a312e30000000001d000000"
    "417574686f72697a6174696f6e456c656d656e74436f6e74656e74730000"
    "0000130000000c000000010000000a00000000000000";
    _marshaller_CSI_AuthorizationElement = new _Marshaller_CSI_AuthorizationElement;
    CSI::_tc_AuthorizationToken = 
    "0100000015000000b4010000010000002700000049444c3a6f6d672e6f72"
    "672f4353492f417574686f72697a6174696f6e546f6b656e3a312e300000"
    "13000000417574686f72697a6174696f6e546f6b656e0000130000006401"
    "0000010000000f00000054010000010000002900000049444c3a6f6d672e"
    "6f72672f4353492f417574686f72697a6174696f6e456c656d656e743a31"
    "2e300000000015000000417574686f72697a6174696f6e456c656d656e74"
    "0000000002000000090000007468655f7479706500000000150000005c00"
    "0000010000002d00000049444c3a6f6d672e6f72672f4353492f41757468"
    "6f72697a6174696f6e456c656d656e74547970653a312e30000000001900"
    "0000417574686f72697a6174696f6e456c656d656e745479706500000000"
    "050000000c0000007468655f656c656d656e740015000000740000000100"
    "00003100000049444c3a6f6d672e6f72672f4353492f417574686f72697a"
    "6174696f6e456c656d656e74436f6e74656e74733a312e30000000001d00"
    "0000417574686f72697a6174696f6e456c656d656e74436f6e74656e7473"
    "00000000130000000c000000010000000a0000000000000000000000";
    CSI::_tc_IdentityTokenType = 
    "01000000150000004c000000010000002600000049444c3a6f6d672e6f72"
    "672f4353492f4964656e74697479546f6b656e547970653a312e30000000"
    "120000004964656e74697479546f6b656e5479706500000005000000";
    CSI::_tc_IdentityExtension = 
    "01000000150000005c000000010000002600000049444c3a6f6d672e6f72"
    "672f4353492f4964656e74697479457874656e73696f6e3a312e30000000"
    "120000004964656e74697479457874656e73696f6e000000130000000c00"
    "0000010000000a00000000000000";
    CSI::_tc_IdentityToken = 
    "0100000010000000b4020000010000002200000049444c3a6f6d672e6f72"
    "672f4353492f4964656e74697479546f6b656e3a312e300000000e000000"
    "4964656e74697479546f6b656e000000150000004c000000010000002600"
    "000049444c3a6f6d672e6f72672f4353492f4964656e74697479546f6b65"
    "6e547970653a312e30000000120000004964656e74697479546f6b656e54"
    "797065000000050000000500000006000000000000000700000061627365"
    "6e74000008000000010000000a000000616e6f6e796d6f75730000000800"
    "0000020000000f0000007072696e636970616c5f6e616d65000015000000"
    "5c000000010000002800000049444c3a6f6d672e6f72672f4353492f4753"
    "535f4e545f4578706f727465644e616d653a312e3000140000004753535f"
    "4e545f4578706f727465644e616d6500130000000c000000010000000a00"
    "000000000000040000001200000063657274696669636174655f63686169"
    "6e0000001500000064000000010000002900000049444c3a6f6d672e6f72"
    "672f4353492f583530394365727469666963617465436861696e3a312e30"
    "0000000015000000583530394365727469666963617465436861696e0000"
    "0000130000000c000000010000000a000000000000000800000003000000"
    "646e00001500000064000000010000002a00000049444c3a6f6d672e6f72"
    "672f4353492f5835303144697374696e677569736865644e616d653a312e"
    "30000000160000005835303144697374696e677569736865644e616d6500"
    "0000130000000c000000010000000a000000000000000000000003000000"
    "69640000150000005c000000010000002600000049444c3a6f6d672e6f72"
    "672f4353492f4964656e74697479457874656e73696f6e3a312e30000000"
    "120000004964656e74697479457874656e73696f6e000000130000000c00"
    "0000010000000a00000000000000";
    _marshaller_CSI_IdentityToken = new _Marshaller_CSI_IdentityToken;
    CSI::_tc_EstablishContext = 
    "010000000f000000c0050000010000002500000049444c3a6f6d672e6f72"
    "672f4353492f45737461626c697368436f6e746578743a312e3000000000"
    "1100000045737461626c697368436f6e7465787400000000040000001200"
    "0000636c69656e745f636f6e746578745f6964000000150000003c000000"
    "010000001e00000049444c3a6f6d672e6f72672f4353492f436f6e746578"
    "7449643a312e300000000a000000436f6e74657874496400000018000000"
    "14000000617574686f72697a6174696f6e5f746f6b656e0015000000b401"
    "0000010000002700000049444c3a6f6d672e6f72672f4353492f41757468"
    "6f72697a6174696f6e546f6b656e3a312e30000013000000417574686f72"
    "697a6174696f6e546f6b656e00001300000064010000010000000f000000"
    "54010000010000002900000049444c3a6f6d672e6f72672f4353492f4175"
    "74686f72697a6174696f6e456c656d656e743a312e300000000015000000"
    "417574686f72697a6174696f6e456c656d656e7400000000020000000900"
    "00007468655f7479706500000000150000005c000000010000002d000000"
    "49444c3a6f6d672e6f72672f4353492f417574686f72697a6174696f6e45"
    "6c656d656e74547970653a312e300000000019000000417574686f72697a"
    "6174696f6e456c656d656e745479706500000000050000000c0000007468"
    "655f656c656d656e74001500000074000000010000003100000049444c3a"
    "6f6d672e6f72672f4353492f417574686f72697a6174696f6e456c656d65"
    "6e74436f6e74656e74733a312e30000000001d000000417574686f72697a"
    "6174696f6e456c656d656e74436f6e74656e747300000000130000000c00"
    "0000010000000a00000000000000000000000f0000006964656e74697479"
    "5f746f6b656e000010000000b4020000010000002200000049444c3a6f6d"
    "672e6f72672f4353492f4964656e74697479546f6b656e3a312e30000000"
    "0e0000004964656e74697479546f6b656e000000150000004c0000000100"
    "00002600000049444c3a6f6d672e6f72672f4353492f4964656e74697479"
    "546f6b656e547970653a312e30000000120000004964656e74697479546f"
    "6b656e547970650000000500000005000000060000000000000007000000"
    "616273656e74000008000000010000000a000000616e6f6e796d6f757300"
    "000008000000020000000f0000007072696e636970616c5f6e616d650000"
    "150000005c000000010000002800000049444c3a6f6d672e6f72672f4353"
    "492f4753535f4e545f4578706f727465644e616d653a312e300014000000"
    "4753535f4e545f4578706f727465644e616d6500130000000c0000000100"
    "00000a00000000000000040000001200000063657274696669636174655f"
    "636861696e0000001500000064000000010000002900000049444c3a6f6d"
    "672e6f72672f4353492f583530394365727469666963617465436861696e"
    "3a312e300000000015000000583530394365727469666963617465436861"
    "696e00000000130000000c000000010000000a0000000000000008000000"
    "03000000646e00001500000064000000010000002a00000049444c3a6f6d"
    "672e6f72672f4353492f5835303144697374696e677569736865644e616d"
    "653a312e30000000160000005835303144697374696e677569736865644e"
    "616d65000000130000000c000000010000000a0000000000000000000000"
    "0300000069640000150000005c000000010000002600000049444c3a6f6d"
    "672e6f72672f4353492f4964656e74697479457874656e73696f6e3a312e"
    "30000000120000004964656e74697479457874656e73696f6e0000001300"
    "00000c000000010000000a000000000000001c000000636c69656e745f61"
    "757468656e7469636174696f6e5f746f6b656e00150000004c0000000100"
    "00001d00000049444c3a6f6d672e6f72672f4353492f475353546f6b656e"
    "3a312e300000000009000000475353546f6b656e00000000130000000c00"
    "0000010000000a00000000000000";
    _marshaller_CSI_EstablishContext = new _Marshaller_CSI_EstablishContext;
    CSI::_tc_CompleteEstablishContext = 
    "010000000f00000040010000010000002d00000049444c3a6f6d672e6f72"
    "672f4353492f436f6d706c65746545737461626c697368436f6e74657874"
    "3a312e300000000019000000436f6d706c65746545737461626c69736843"
    "6f6e74657874000000000300000012000000636c69656e745f636f6e7465"
    "78745f6964000000150000003c000000010000001e00000049444c3a6f6d"
    "672e6f72672f4353492f436f6e7465787449643a312e300000000a000000"
    "436f6e7465787449640000001800000011000000636f6e746578745f7374"
    "61746566756c00000000080000001400000066696e616c5f636f6e746578"
    "745f746f6b656e00150000004c000000010000001d00000049444c3a6f6d"
    "672e6f72672f4353492f475353546f6b656e3a312e300000000009000000"
    "475353546f6b656e00000000130000000c000000010000000a0000000000"
    "0000";
    _marshaller_CSI_CompleteEstablishContext = new _Marshaller_CSI_CompleteEstablishContext;
    CSI::_tc_ContextError = 
    "010000000f00000034010000010000002100000049444c3a6f6d672e6f72"
    "672f4353492f436f6e746578744572726f723a312e30000000000d000000"
    "436f6e746578744572726f72000000000400000012000000636c69656e74"
    "5f636f6e746578745f6964000000150000003c000000010000001e000000"
    "49444c3a6f6d672e6f72672f4353492f436f6e7465787449643a312e3000"
    "00000a000000436f6e746578744964000000180000000d0000006d616a6f"
    "725f73746174757300000000030000000d0000006d696e6f725f73746174"
    "757300000000030000000c0000006572726f725f746f6b656e0015000000"
    "4c000000010000001d00000049444c3a6f6d672e6f72672f4353492f4753"
    "53546f6b656e3a312e300000000009000000475353546f6b656e00000000"
    "130000000c000000010000000a00000000000000";
    _marshaller_CSI_ContextError = new _Marshaller_CSI_ContextError;
    CSI::_tc_MessageInContext = 
    "010000000f000000c0000000010000002500000049444c3a6f6d672e6f72"
    "672f4353492f4d657373616765496e436f6e746578743a312e3000000000"
    "110000004d657373616765496e436f6e7465787400000000020000001200"
    "0000636c69656e745f636f6e746578745f6964000000150000003c000000"
    "010000001e00000049444c3a6f6d672e6f72672f4353492f436f6e746578"
    "7449643a312e300000000a000000436f6e74657874496400000018000000"
    "10000000646973636172645f636f6e746578740008000000";
    _marshaller_CSI_MessageInContext = new _Marshaller_CSI_MessageInContext;
    CSI::_tc_SASContextBody = 
    "0100000010000000f4090000010000002300000049444c3a6f6d672e6f72"
    "672f4353492f534153436f6e74657874426f64793a312e3000000f000000"
    "534153436f6e74657874426f647900001500000034000000010000001c00"
    "000049444c3a6f6d672e6f72672f4353492f4d7367547970653a312e3000"
    "080000004d7367547970650002000000ffffffff04000000000000000e00"
    "000065737461626c6973685f6d73670000000f000000c005000001000000"
    "2500000049444c3a6f6d672e6f72672f4353492f45737461626c69736843"
    "6f6e746578743a312e30000000001100000045737461626c697368436f6e"
    "74657874000000000400000012000000636c69656e745f636f6e74657874"
    "5f6964000000150000003c000000010000001e00000049444c3a6f6d672e"
    "6f72672f4353492f436f6e7465787449643a312e300000000a000000436f"
    "6e7465787449640000001800000014000000617574686f72697a6174696f"
    "6e5f746f6b656e0015000000b4010000010000002700000049444c3a6f6d"
    "672e6f72672f4353492f417574686f72697a6174696f6e546f6b656e3a31"
    "2e30000013000000417574686f72697a6174696f6e546f6b656e00001300"
    "000064010000010000000f00000054010000010000002900000049444c3a"
    "6f6d672e6f72672f4353492f417574686f72697a6174696f6e456c656d65"
    "6e743a312e300000000015000000417574686f72697a6174696f6e456c65"
    "6d656e740000000002000000090000007468655f74797065000000001500"
    "00005c000000010000002d00000049444c3a6f6d672e6f72672f4353492f"
    "417574686f72697a6174696f6e456c656d656e74547970653a312e300000"
    "000019000000417574686f72697a6174696f6e456c656d656e7454797065"
    "00000000050000000c0000007468655f656c656d656e7400150000007400"
    "0000010000003100000049444c3a6f6d672e6f72672f4353492f41757468"
    "6f72697a6174696f6e456c656d656e74436f6e74656e74733a312e300000"
    "00001d000000417574686f72697a6174696f6e456c656d656e74436f6e74"
    "656e747300000000130000000c000000010000000a000000000000000000"
    "00000f0000006964656e746974795f746f6b656e000010000000b4020000"
    "010000002200000049444c3a6f6d672e6f72672f4353492f4964656e7469"
    "7479546f6b656e3a312e300000000e0000004964656e74697479546f6b65"
    "6e000000150000004c000000010000002600000049444c3a6f6d672e6f72"
    "672f4353492f4964656e74697479546f6b656e547970653a312e30000000"
    "120000004964656e74697479546f6b656e54797065000000050000000500"
    "0000060000000000000007000000616273656e7400000800000001000000"
    "0a000000616e6f6e796d6f757300000008000000020000000f0000007072"
    "696e636970616c5f6e616d650000150000005c0000000100000028000000"
    "49444c3a6f6d672e6f72672f4353492f4753535f4e545f4578706f727465"
    "644e616d653a312e3000140000004753535f4e545f4578706f727465644e"
    "616d6500130000000c000000010000000a00000000000000040000001200"
    "000063657274696669636174655f636861696e0000001500000064000000"
    "010000002900000049444c3a6f6d672e6f72672f4353492f583530394365"
    "727469666963617465436861696e3a312e30000000001500000058353039"
    "4365727469666963617465436861696e00000000130000000c0000000100"
    "00000a000000000000000800000003000000646e00001500000064000000"
    "010000002a00000049444c3a6f6d672e6f72672f4353492f583530314469"
    "7374696e677569736865644e616d653a312e300000001600000058353031"
    "44697374696e677569736865644e616d65000000130000000c0000000100"
    "00000a00000000000000000000000300000069640000150000005c000000"
    "010000002600000049444c3a6f6d672e6f72672f4353492f4964656e7469"
    "7479457874656e73696f6e3a312e30000000120000004964656e74697479"
    "457874656e73696f6e000000130000000c000000010000000a0000000000"
    "00001c000000636c69656e745f61757468656e7469636174696f6e5f746f"
    "6b656e00150000004c000000010000001d00000049444c3a6f6d672e6f72"
    "672f4353492f475353546f6b656e3a312e30000000000900000047535354"
    "6f6b656e00000000130000000c000000010000000a000000000000000100"
    "00000d000000636f6d706c6574655f6d7367000000000f00000040010000"
    "010000002d00000049444c3a6f6d672e6f72672f4353492f436f6d706c65"
    "746545737461626c697368436f6e746578743a312e300000000019000000"
    "436f6d706c65746545737461626c697368436f6e74657874000000000300"
    "000012000000636c69656e745f636f6e746578745f696400000015000000"
    "3c000000010000001e00000049444c3a6f6d672e6f72672f4353492f436f"
    "6e7465787449643a312e300000000a000000436f6e746578744964000000"
    "1800000011000000636f6e746578745f737461746566756c000000000800"
    "00001400000066696e616c5f636f6e746578745f746f6b656e0015000000"
    "4c000000010000001d00000049444c3a6f6d672e6f72672f4353492f4753"
    "53546f6b656e3a312e300000000009000000475353546f6b656e00000000"
    "130000000c000000010000000a00000000000000040000000a0000006572"
    "726f725f6d73670000000f00000034010000010000002100000049444c3a"
    "6f6d672e6f72672f4353492f436f6e746578744572726f723a312e300000"
    "00000d000000436f6e746578744572726f72000000000400000012000000"
    "636c69656e745f636f6e746578745f6964000000150000003c0000000100"
    "00001e00000049444c3a6f6d672e6f72672f4353492f436f6e7465787449"
    "643a312e300000000a000000436f6e746578744964000000180000000d00"
    "00006d616a6f725f73746174757300000000030000000d0000006d696e6f"
    "725f73746174757300000000030000000c0000006572726f725f746f6b65"
    "6e00150000004c000000010000001d00000049444c3a6f6d672e6f72672f"
    "4353492f475353546f6b656e3a312e300000000009000000475353546f6b"
    "656e00000000130000000c000000010000000a0000000000000005000000"
    "0f000000696e5f636f6e746578745f6d736700000f000000c00000000100"
    "00002500000049444c3a6f6d672e6f72672f4353492f4d65737361676549"
    "6e436f6e746578743a312e3000000000110000004d657373616765496e43"
    "6f6e74657874000000000200000012000000636c69656e745f636f6e7465"
    "78745f6964000000150000003c000000010000001e00000049444c3a6f6d"
    "672e6f72672f4353492f436f6e7465787449643a312e300000000a000000"
    "436f6e7465787449640000001800000010000000646973636172645f636f"
    "6e746578740008000000";
    _marshaller_CSI_SASContextBody = new _Marshaller_CSI_SASContextBody;
    CSI::_tc_StringOID = 
    "010000001500000040000000010000001e00000049444c3a6f6d672e6f72"
    "672f4353492f537472696e674f49443a312e300000000a00000053747269"
    "6e674f49440000001200000000000000";
    _marshaller__seq_CSI_AuthorizationElement = new _Marshaller__seq_CSI_AuthorizationElement;
  }

  ~__tc_init_CSI()
  {
    delete static_cast<_Marshaller_CSI_AuthorizationElement*>(_marshaller_CSI_AuthorizationElement);
    delete static_cast<_Marshaller_CSI_IdentityToken*>(_marshaller_CSI_IdentityToken);
    delete static_cast<_Marshaller_CSI_EstablishContext*>(_marshaller_CSI_EstablishContext);
    delete static_cast<_Marshaller_CSI_CompleteEstablishContext*>(_marshaller_CSI_CompleteEstablishContext);
    delete static_cast<_Marshaller_CSI_ContextError*>(_marshaller_CSI_ContextError);
    delete static_cast<_Marshaller_CSI_MessageInContext*>(_marshaller_CSI_MessageInContext);
    delete static_cast<_Marshaller_CSI_SASContextBody*>(_marshaller_CSI_SASContextBody);
    delete static_cast<_Marshaller__seq_CSI_AuthorizationElement*>(_marshaller__seq_CSI_AuthorizationElement);
  }
};

static __tc_init_CSI __init_CSI;

