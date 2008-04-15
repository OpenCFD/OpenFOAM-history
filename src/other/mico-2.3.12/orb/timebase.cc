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



#ifdef HAVE_EXPLICIT_STRUCT_OPS
TimeBase::UtcT::UtcT()
{
}

TimeBase::UtcT::UtcT( const UtcT& _s )
{
  time = ((UtcT&)_s).time;
  inacclo = ((UtcT&)_s).inacclo;
  inacchi = ((UtcT&)_s).inacchi;
  tdf = ((UtcT&)_s).tdf;
}

TimeBase::UtcT::~UtcT()
{
}

TimeBase::UtcT&
TimeBase::UtcT::operator=( const UtcT& _s )
{
  time = ((UtcT&)_s).time;
  inacclo = ((UtcT&)_s).inacclo;
  inacchi = ((UtcT&)_s).inacchi;
  tdf = ((UtcT&)_s).tdf;
  return *this;
}
#endif

class _Marshaller_TimeBase_UtcT : public ::CORBA::StaticTypeInfo {
    typedef TimeBase::UtcT _MICO_T;
  public:
    ~_Marshaller_TimeBase_UtcT();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
};


_Marshaller_TimeBase_UtcT::~_Marshaller_TimeBase_UtcT()
{
}

::CORBA::StaticValueType _Marshaller_TimeBase_UtcT::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_TimeBase_UtcT::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_TimeBase_UtcT::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_TimeBase_UtcT::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->time ) &&
    CORBA::_stc_ulong->demarshal( dc, &((_MICO_T*)v)->inacclo ) &&
    CORBA::_stc_ushort->demarshal( dc, &((_MICO_T*)v)->inacchi ) &&
    CORBA::_stc_short->demarshal( dc, &((_MICO_T*)v)->tdf ) &&
    dc.struct_end();
}

void _Marshaller_TimeBase_UtcT::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->time );
  CORBA::_stc_ulong->marshal( ec, &((_MICO_T*)v)->inacclo );
  CORBA::_stc_ushort->marshal( ec, &((_MICO_T*)v)->inacchi );
  CORBA::_stc_short->marshal( ec, &((_MICO_T*)v)->tdf );
  ec.struct_end();
}

::CORBA::StaticTypeInfo *_marshaller_TimeBase_UtcT;

#ifdef HAVE_EXPLICIT_STRUCT_OPS
TimeBase::IntervalT::IntervalT()
{
}

TimeBase::IntervalT::IntervalT( const IntervalT& _s )
{
  lower_bound = ((IntervalT&)_s).lower_bound;
  upper_bound = ((IntervalT&)_s).upper_bound;
}

TimeBase::IntervalT::~IntervalT()
{
}

TimeBase::IntervalT&
TimeBase::IntervalT::operator=( const IntervalT& _s )
{
  lower_bound = ((IntervalT&)_s).lower_bound;
  upper_bound = ((IntervalT&)_s).upper_bound;
  return *this;
}
#endif

class _Marshaller_TimeBase_IntervalT : public ::CORBA::StaticTypeInfo {
    typedef TimeBase::IntervalT _MICO_T;
  public:
    ~_Marshaller_TimeBase_IntervalT();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
};


_Marshaller_TimeBase_IntervalT::~_Marshaller_TimeBase_IntervalT()
{
}

::CORBA::StaticValueType _Marshaller_TimeBase_IntervalT::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_TimeBase_IntervalT::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_TimeBase_IntervalT::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_TimeBase_IntervalT::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->lower_bound ) &&
    CORBA::_stc_ulonglong->demarshal( dc, &((_MICO_T*)v)->upper_bound ) &&
    dc.struct_end();
}

void _Marshaller_TimeBase_IntervalT::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->lower_bound );
  CORBA::_stc_ulonglong->marshal( ec, &((_MICO_T*)v)->upper_bound );
  ec.struct_end();
}

::CORBA::StaticTypeInfo *_marshaller_TimeBase_IntervalT;

struct __tc_init_TIMEBASE {
  __tc_init_TIMEBASE()
  {
    _marshaller_TimeBase_UtcT = new _Marshaller_TimeBase_UtcT;
    _marshaller_TimeBase_IntervalT = new _Marshaller_TimeBase_IntervalT;
  }

  ~__tc_init_TIMEBASE()
  {
    delete static_cast<_Marshaller_TimeBase_UtcT*>(_marshaller_TimeBase_UtcT);
    delete static_cast<_Marshaller_TimeBase_IntervalT*>(_marshaller_TimeBase_IntervalT);
  }
};

static __tc_init_TIMEBASE __init_TIMEBASE;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------
