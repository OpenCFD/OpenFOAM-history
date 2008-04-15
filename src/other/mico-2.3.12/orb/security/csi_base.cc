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
CORBA::TypeCodeConst _tc_OID;
}

namespace CSI
{
CORBA::TypeCodeConst _tc_OIDList;
}

class _Marshaller__seq__seq_octet : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CSI::OID,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq__seq_octet();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq__seq_octet::~_Marshaller__seq__seq_octet()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq__seq_octet::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq__seq_octet::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq__seq_octet::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq__seq_octet::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !CORBA::_stcseq_octet->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq__seq_octet::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    CORBA::_stcseq_octet->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq__seq_octet::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000130000004c00000001000000150000003c000000010000001800"
    "000049444c3a6f6d672e6f72672f4353492f4f49443a312e300004000000"
    "4f494400130000000c000000010000000a0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq__seq_octet::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq__seq_octet;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSI::OID,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq__seq_octet, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CSI::OID,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSI::OID,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq__seq_octet, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSI::OID,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq__seq_octet, (void *&)_s);
}


struct __tc_init_CSI_BASE {
  __tc_init_CSI_BASE()
  {
    CSI::_tc_OID = 
    "01000000150000003c000000010000001800000049444c3a6f6d672e6f72"
    "672f4353492f4f49443a312e3000040000004f494400130000000c000000"
    "010000000a00000000000000";
    CSI::_tc_OIDList = 
    "010000001500000084000000010000001c00000049444c3a6f6d672e6f72"
    "672f4353492f4f49444c6973743a312e3000080000004f49444c69737400"
    "130000004c00000001000000150000003c00000001000000180000004944"
    "4c3a6f6d672e6f72672f4353492f4f49443a312e3000040000004f494400"
    "130000000c000000010000000a0000000000000000000000";
    _marshaller__seq__seq_octet = new _Marshaller__seq__seq_octet;
  }

  ~__tc_init_CSI_BASE()
  {
    delete static_cast<_Marshaller__seq__seq_octet*>(_marshaller__seq__seq_octet);
  }
};

static __tc_init_CSI_BASE __init_CSI_BASE;

