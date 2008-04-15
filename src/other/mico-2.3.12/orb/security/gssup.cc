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
namespace GSSUP
{
CORBA::TypeCodeConst _tc_InitialContextToken;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
GSSUP::InitialContextToken::InitialContextToken()
{
}

GSSUP::InitialContextToken::InitialContextToken( const InitialContextToken& _s )
{
  username = ((InitialContextToken&)_s).username;
  password = ((InitialContextToken&)_s).password;
  target_name = ((InitialContextToken&)_s).target_name;
}

GSSUP::InitialContextToken::~InitialContextToken()
{
}

GSSUP::InitialContextToken&
GSSUP::InitialContextToken::operator=( const InitialContextToken& _s )
{
  username = ((InitialContextToken&)_s).username;
  password = ((InitialContextToken&)_s).password;
  target_name = ((InitialContextToken&)_s).target_name;
  return *this;
}
#endif

class _Marshaller_GSSUP_InitialContextToken : public ::CORBA::StaticTypeInfo {
    typedef GSSUP::InitialContextToken _MICO_T;
  public:
    ~_Marshaller_GSSUP_InitialContextToken();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_GSSUP_InitialContextToken::~_Marshaller_GSSUP_InitialContextToken()
{
}

::CORBA::StaticValueType _Marshaller_GSSUP_InitialContextToken::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_GSSUP_InitialContextToken::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_GSSUP_InitialContextToken::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_GSSUP_InitialContextToken::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->username ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->password ) &&
    CORBA::_stcseq_octet->demarshal( dc, &((_MICO_T*)v)->target_name ) &&
    dc.struct_end();
}

void _Marshaller_GSSUP_InitialContextToken::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->username );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->password );
  CORBA::_stcseq_octet->marshal( ec, &((_MICO_T*)v)->target_name );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_GSSUP_InitialContextToken::typecode()
{
  return GSSUP::_tc_InitialContextToken;
}

::CORBA::StaticTypeInfo *_marshaller_GSSUP_InitialContextToken;

void operator<<=( CORBA::Any &_a, const GSSUP::InitialContextToken &_s )
{
  CORBA::StaticAny _sa (_marshaller_GSSUP_InitialContextToken, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, GSSUP::InitialContextToken *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, GSSUP::InitialContextToken &_s )
{
  CORBA::StaticAny _sa (_marshaller_GSSUP_InitialContextToken, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const GSSUP::InitialContextToken *&_s )
{
  return _a.to_static_any (_marshaller_GSSUP_InitialContextToken, (void *&)_s);
}

namespace GSSUP
{
CORBA::TypeCodeConst _tc_ErrorCode;
}

namespace GSSUP
{
CORBA::TypeCodeConst _tc_ErrorToken;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
GSSUP::ErrorToken::ErrorToken()
{
}

GSSUP::ErrorToken::ErrorToken( const ErrorToken& _s )
{
  error_code = ((ErrorToken&)_s).error_code;
}

GSSUP::ErrorToken::~ErrorToken()
{
}

GSSUP::ErrorToken&
GSSUP::ErrorToken::operator=( const ErrorToken& _s )
{
  error_code = ((ErrorToken&)_s).error_code;
  return *this;
}
#endif

class _Marshaller_GSSUP_ErrorToken : public ::CORBA::StaticTypeInfo {
    typedef GSSUP::ErrorToken _MICO_T;
  public:
    ~_Marshaller_GSSUP_ErrorToken();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_GSSUP_ErrorToken::~_Marshaller_GSSUP_ErrorToken()
{
}

::CORBA::StaticValueType _Marshaller_GSSUP_ErrorToken::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_GSSUP_ErrorToken::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_GSSUP_ErrorToken::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_GSSUP_ErrorToken::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->error_code ) &&
    dc.struct_end();
}

void _Marshaller_GSSUP_ErrorToken::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->error_code );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_GSSUP_ErrorToken::typecode()
{
  return GSSUP::_tc_ErrorToken;
}

::CORBA::StaticTypeInfo *_marshaller_GSSUP_ErrorToken;

void operator<<=( CORBA::Any &_a, const GSSUP::ErrorToken &_s )
{
  CORBA::StaticAny _sa (_marshaller_GSSUP_ErrorToken, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, GSSUP::ErrorToken *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, GSSUP::ErrorToken &_s )
{
  CORBA::StaticAny _sa (_marshaller_GSSUP_ErrorToken, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const GSSUP::ErrorToken *&_s )
{
  return _a.to_static_any (_marshaller_GSSUP_ErrorToken, (void *&)_s);
}

struct __tc_init_GSSUP {
  __tc_init_GSSUP()
  {
    GSSUP::_tc_InitialContextToken = 
    "010000000f0000008c010000010000002a00000049444c3a6f6d672e6f72"
    "672f47535355502f496e697469616c436f6e74657874546f6b656e3a312e"
    "3000000014000000496e697469616c436f6e74657874546f6b656e000300"
    "000009000000757365726e616d6500000000150000004c00000001000000"
    "1f00000049444c3a6f6d672e6f72672f4353492f55544638537472696e67"
    "3a312e3000000b00000055544638537472696e670000130000000c000000"
    "010000000a000000000000000900000070617373776f7264000000001500"
    "00004c000000010000001f00000049444c3a6f6d672e6f72672f4353492f"
    "55544638537472696e673a312e3000000b00000055544638537472696e67"
    "0000130000000c000000010000000a000000000000000c00000074617267"
    "65745f6e616d6500150000005c000000010000002800000049444c3a6f6d"
    "672e6f72672f4353492f4753535f4e545f4578706f727465644e616d653a"
    "312e3000140000004753535f4e545f4578706f727465644e616d65001300"
    "00000c000000010000000a00000000000000";
    _marshaller_GSSUP_InitialContextToken = new _Marshaller_GSSUP_InitialContextToken;
    GSSUP::_tc_ErrorCode = 
    "01000000150000003c000000010000002000000049444c3a6f6d672e6f72"
    "672f47535355502f4572726f72436f64653a312e30000a0000004572726f"
    "72436f646500000005000000";
    GSSUP::_tc_ErrorToken = 
    "010000000f00000094000000010000002100000049444c3a6f6d672e6f72"
    "672f47535355502f4572726f72546f6b656e3a312e30000000000b000000"
    "4572726f72546f6b656e0000010000000b0000006572726f725f636f6465"
    "0000150000003c000000010000002000000049444c3a6f6d672e6f72672f"
    "47535355502f4572726f72436f64653a312e30000a0000004572726f7243"
    "6f646500000005000000";
    _marshaller_GSSUP_ErrorToken = new _Marshaller_GSSUP_ErrorToken;
  }

  ~__tc_init_GSSUP()
  {
    delete static_cast<_Marshaller_GSSUP_InitialContextToken*>(_marshaller_GSSUP_InitialContextToken);
    delete static_cast<_Marshaller_GSSUP_ErrorToken*>(_marshaller_GSSUP_ErrorToken);
  }
};

static __tc_init_GSSUP __init_GSSUP;

