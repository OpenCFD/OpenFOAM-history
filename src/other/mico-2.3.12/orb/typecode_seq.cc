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
CORBA::TypeCodeConst _tc_TypeCodeSeq;
}

class _Marshaller__seq_TypeCode : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_TypeCode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_TypeCode::~_Marshaller__seq_TypeCode()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_TypeCode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_TypeCode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_TypeCode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_TypeCode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !CORBA::_stc_TypeCode->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_TypeCode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    CORBA::_stc_TypeCode->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_TypeCode::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000130000000c000000010000000c00000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_TypeCode::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_TypeCode;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_TypeCode, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_TypeCode, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::TypeCode_var,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_TypeCode, (void *&)_s);
}


struct __tc_init_TYPECODE_SEQ {
  __tc_init_TYPECODE_SEQ()
  {
    CORBA::_tc_TypeCodeSeq = 
    "010000001500000050000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f54797065436f64655365713a312e300000000c000000"
    "54797065436f646553657100130000000c000000010000000c0000000000"
    "0000";
    _marshaller__seq_TypeCode = new _Marshaller__seq_TypeCode;
  }

  ~__tc_init_TYPECODE_SEQ()
  {
    delete static_cast<_Marshaller__seq_TypeCode*>(_marshaller__seq_TypeCode);
  }
};

static __tc_init_TYPECODE_SEQ __init_TYPECODE_SEQ;

