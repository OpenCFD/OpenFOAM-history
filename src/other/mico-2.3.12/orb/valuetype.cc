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

// valuebox StringValue
CORBA::StringValue::StringValue ()
{
}

CORBA::StringValue::StringValue (char * _p)
{
  value = _p;
}

CORBA::StringValue::StringValue (const char * _p)
{
  value = _p;
}

CORBA::StringValue::StringValue (const ::CORBA::String_var & _p)
{
  value = _p;
}

CORBA::StringValue::StringValue (const StringValue& _p)
{
  value = _p.value;
}

CORBA::StringValue::~StringValue ()
{
}

CORBA::StringValue &
CORBA::StringValue::operator= (char * _p)
{
  value = _p;
  return *this;
}

CORBA::StringValue &
CORBA::StringValue::operator= (const char * _p)
{
  value = _p;
  return *this;
}

CORBA::StringValue &
CORBA::StringValue::operator= (const ::CORBA::String_var & _p)
{
  value = _p;
  return *this;
}

const char *
CORBA::StringValue::_value () const
{
  return value;
}

void
CORBA::StringValue::_value (char * _p)
{
  value = _p;
}

void
CORBA::StringValue::_value (const char * _p)
{
  value = _p;
}

void
CORBA::StringValue::_value (const ::CORBA::String_var & _p)
{
  value = _p;
}

char &
CORBA::StringValue::operator[] (::CORBA::ULong idx)
{
  return value[idx];
}

char
CORBA::StringValue::operator[] (::CORBA::ULong idx) const
{
  return value[idx];
}

const char*
CORBA::StringValue::_boxed_in() const
{
  return value.in();
}

char*&
CORBA::StringValue::_boxed_inout()
{
  return value.inout();
}

CORBA::String_out
CORBA::StringValue::_boxed_out()
{
  return value.out();
}

void *
CORBA::StringValue::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/StringValue:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

CORBA::StringValue *
CORBA::StringValue::_downcast (::CORBA::ValueBase *vb)
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/CORBA/StringValue:1.0")))) {
    return (::CORBA::StringValue *) p;
  }
  return 0;
}

CORBA::StringValue *
CORBA::StringValue::_downcast (::CORBA::AbstractBase *)
{
  return 0;
}

::CORBA::ValueDef_ptr
CORBA::StringValue::get_value_def () 
{
  return ::CORBA::ValueDef::_nil ();
}

::CORBA::ValueBase *
CORBA::StringValue::_copy_value ()
{
  return new StringValue (*this);
}

void
CORBA::StringValue::_get_marshal_info (vector<string> & repoids, ::CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/CORBA/StringValue:1.0");
  chunked = FALSE;
}

void
CORBA::StringValue::_marshal_members (::CORBA::DataEncoder &ec)
{
  CORBA::_stc_string->marshal (ec, &value.inout());
}

::CORBA::Boolean
CORBA::StringValue::_demarshal_members (::CORBA::DataDecoder &ec)
{
  return CORBA::_stc_string->demarshal (ec, &value._for_demarshal());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_StringValue;
}

class _Marshaller_CORBA_StringValue : public ::CORBA::StaticTypeInfo {
    typedef ::CORBA::StringValue* _MICO_T;
  public:
    ~_Marshaller_CORBA_StringValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_StringValue::~_Marshaller_CORBA_StringValue()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_StringValue::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_StringValue::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_StringValue::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_StringValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = new ::CORBA::StringValue;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/CORBA/StringValue:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::CORBA::StringValue::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_CORBA_StringValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_StringValue::typecode()
{
  return CORBA::_tc_StringValue;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_StringValue;

void
operator<<=( ::CORBA::Any &_a, const ::CORBA::StringValue* _val )
{
  ::CORBA::StaticAny _sa (_marshaller_CORBA_StringValue, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( ::CORBA::Any &_a, ::CORBA::StringValue** _val_ptr )
{
  ::CORBA::ValueBase_var _val = *_val_ptr;
  ::CORBA::StaticAny _sa (_marshaller_CORBA_StringValue, _val_ptr);
  _a.from_static_any (_sa);
}

::CORBA::Boolean
operator>>=( const ::CORBA::Any &_a, ::CORBA::StringValue* &_val_ptr )
{
  ::CORBA::StringValue* *p;
  if (_a.to_static_any (_marshaller_CORBA_StringValue, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// valuebox WStringValue
CORBA::WStringValue::WStringValue ()
{
}

CORBA::WStringValue::WStringValue (::CORBA::WChar * _p)
{
  value = _p;
}

CORBA::WStringValue::WStringValue (const ::CORBA::WChar * _p)
{
  value = _p;
}

CORBA::WStringValue::WStringValue (const ::CORBA::WString_var & _p)
{
  value = _p;
}

CORBA::WStringValue::WStringValue (const WStringValue& _p)
{
  value = _p.value;
}

CORBA::WStringValue::~WStringValue ()
{
}

CORBA::WStringValue &
CORBA::WStringValue::operator= (::CORBA::WChar * _p)
{
  value = _p;
  return *this;
}

CORBA::WStringValue &
CORBA::WStringValue::operator= (const ::CORBA::WChar * _p)
{
  value = _p;
  return *this;
}

CORBA::WStringValue &
CORBA::WStringValue::operator= (const ::CORBA::WString_var & _p)
{
  value = _p;
  return *this;
}

const ::CORBA::WChar *
CORBA::WStringValue::_value () const
{
  return value;
}

void
CORBA::WStringValue::_value (::CORBA::WChar * _p)
{
  value = _p;
}

void
CORBA::WStringValue::_value (const ::CORBA::WChar * _p)
{
  value = _p;
}

void
CORBA::WStringValue::_value (const ::CORBA::WString_var & _p)
{
  value = _p;
}

::CORBA::WChar &
CORBA::WStringValue::operator[] (::CORBA::ULong idx)
{
  return value[idx];
}

::CORBA::WChar
CORBA::WStringValue::operator[] (::CORBA::ULong idx) const
{
  return value[idx];
}

const CORBA::WChar*
CORBA::WStringValue::_boxed_in() const
{
  return value.in();
}

CORBA::WChar*&
CORBA::WStringValue::_boxed_inout()
{
  return value.inout();
}

CORBA::WString_out
CORBA::WStringValue::_boxed_out()
{
  return value.out();
}

void *
CORBA::WStringValue::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/WStringValue:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

CORBA::WStringValue *
CORBA::WStringValue::_downcast (::CORBA::ValueBase *vb)
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/CORBA/WStringValue:1.0")))) {
    return (::CORBA::WStringValue *) p;
  }
  return 0;
}

CORBA::WStringValue *
CORBA::WStringValue::_downcast (::CORBA::AbstractBase *)
{
  return 0;
}

::CORBA::ValueDef_ptr
CORBA::WStringValue::get_value_def () 
{
  return ::CORBA::ValueDef::_nil ();
}

::CORBA::ValueBase *
CORBA::WStringValue::_copy_value ()
{
  return new WStringValue (*this);
}

void
CORBA::WStringValue::_get_marshal_info (vector<string> & repoids, ::CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/CORBA/WStringValue:1.0");
  chunked = FALSE;
}

void
CORBA::WStringValue::_marshal_members (::CORBA::DataEncoder &ec)
{
  CORBA::_stc_wstring->marshal (ec, &value.inout());
}

::CORBA::Boolean
CORBA::WStringValue::_demarshal_members (::CORBA::DataDecoder &ec)
{
  return CORBA::_stc_wstring->demarshal (ec, &value._for_demarshal());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_WStringValue;
}

class _Marshaller_CORBA_WStringValue : public ::CORBA::StaticTypeInfo {
    typedef ::CORBA::WStringValue* _MICO_T;
  public:
    ~_Marshaller_CORBA_WStringValue();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_WStringValue::~_Marshaller_CORBA_WStringValue()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_WStringValue::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_WStringValue::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_WStringValue::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_WStringValue::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = new ::CORBA::WStringValue;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/CORBA/WStringValue:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::CORBA::WStringValue::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_CORBA_WStringValue::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_WStringValue::typecode()
{
  return CORBA::_tc_WStringValue;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_WStringValue;

void
operator<<=( ::CORBA::Any &_a, const ::CORBA::WStringValue* _val )
{
  ::CORBA::StaticAny _sa (_marshaller_CORBA_WStringValue, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( ::CORBA::Any &_a, ::CORBA::WStringValue** _val_ptr )
{
  ::CORBA::ValueBase_var _val = *_val_ptr;
  ::CORBA::StaticAny _sa (_marshaller_CORBA_WStringValue, _val_ptr);
  _a.from_static_any (_sa);
}

::CORBA::Boolean
operator>>=( const ::CORBA::Any &_a, ::CORBA::WStringValue* &_val_ptr )
{
  ::CORBA::WStringValue* *p;
  if (_a.to_static_any (_marshaller_CORBA_WStringValue, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// valuetype DataInputStream
CORBA::DataInputStream::DataInputStream ()
{
}

CORBA::DataInputStream::~DataInputStream ()
{
}

void *
CORBA::DataInputStream::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/DataInputStream:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

CORBA::DataInputStream *
CORBA::DataInputStream::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/CORBA/DataInputStream:1.0")))) {
    return (CORBA::DataInputStream *) p;
  }
  return 0;
}

CORBA::DataInputStream *
CORBA::DataInputStream::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
CORBA::DataInputStream::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/CORBA/DataInputStream:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
CORBA::DataInputStream::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/CORBA/DataInputStream:1.0");
  chunked = FALSE;
}

void
CORBA::DataInputStream::_marshal_members (CORBA::DataEncoder &ec)
{
}

CORBA::Boolean
CORBA::DataInputStream::_demarshal_members (CORBA::DataDecoder &dc)
{
  return TRUE;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_DataInputStream;
}

class _Marshaller_CORBA_DataInputStream : public ::CORBA::StaticTypeInfo {
    typedef CORBA::DataInputStream* _MICO_T;
  public:
    ~_Marshaller_CORBA_DataInputStream();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_DataInputStream::~_Marshaller_CORBA_DataInputStream()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_DataInputStream::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_DataInputStream::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_DataInputStream::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_DataInputStream::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/CORBA/DataInputStream:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::CORBA::DataInputStream::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_CORBA_DataInputStream::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_DataInputStream::typecode()
{
  return CORBA::_tc_DataInputStream;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_DataInputStream;

void
operator<<=( CORBA::Any &_a, const CORBA::DataInputStream* _val )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_DataInputStream, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::DataInputStream** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_CORBA_DataInputStream, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::DataInputStream* &_val_ptr )
{
  CORBA::DataInputStream* *p;
  if (_a.to_static_any (_marshaller_CORBA_DataInputStream, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// valuetype DataOutputStream
CORBA::DataOutputStream::DataOutputStream ()
{
}

CORBA::DataOutputStream::~DataOutputStream ()
{
}

void *
CORBA::DataOutputStream::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/DataOutputStream:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

CORBA::DataOutputStream *
CORBA::DataOutputStream::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/CORBA/DataOutputStream:1.0")))) {
    return (CORBA::DataOutputStream *) p;
  }
  return 0;
}

CORBA::DataOutputStream *
CORBA::DataOutputStream::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
CORBA::DataOutputStream::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/CORBA/DataOutputStream:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
CORBA::DataOutputStream::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/CORBA/DataOutputStream:1.0");
  chunked = FALSE;
}

void
CORBA::DataOutputStream::_marshal_members (CORBA::DataEncoder &ec)
{
}

CORBA::Boolean
CORBA::DataOutputStream::_demarshal_members (CORBA::DataDecoder &dc)
{
  return TRUE;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_DataOutputStream;
}

class _Marshaller_CORBA_DataOutputStream : public ::CORBA::StaticTypeInfo {
    typedef CORBA::DataOutputStream* _MICO_T;
  public:
    ~_Marshaller_CORBA_DataOutputStream();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_DataOutputStream::~_Marshaller_CORBA_DataOutputStream()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_DataOutputStream::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_DataOutputStream::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_DataOutputStream::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_DataOutputStream::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/CORBA/DataOutputStream:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::CORBA::DataOutputStream::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_CORBA_DataOutputStream::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_DataOutputStream::typecode()
{
  return CORBA::_tc_DataOutputStream;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_DataOutputStream;

void
operator<<=( CORBA::Any &_a, const CORBA::DataOutputStream* _val )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_DataOutputStream, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::DataOutputStream** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_CORBA_DataOutputStream, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::DataOutputStream* &_val_ptr )
{
  CORBA::DataOutputStream* *p;
  if (_a.to_static_any (_marshaller_CORBA_DataOutputStream, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}


// valuetype CustomMarshal
CORBA::CustomMarshal::CustomMarshal ()
{
}

CORBA::CustomMarshal::~CustomMarshal ()
{
}

void *
CORBA::CustomMarshal::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/CustomMarshal:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

CORBA::CustomMarshal *
CORBA::CustomMarshal::_downcast (CORBA::ValueBase * vb) 
{
  void * p;
  if (vb && ((p = vb->_narrow_helper ("IDL:omg.org/CORBA/CustomMarshal:1.0")))) {
    return (CORBA::CustomMarshal *) p;
  }
  return 0;
}

CORBA::CustomMarshal *
CORBA::CustomMarshal::_downcast (CORBA::AbstractBase * vb) 
{
  return _downcast (vb->_to_value());
}

CORBA::ValueDef_ptr
CORBA::CustomMarshal::get_value_def () 
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/CORBA/CustomMarshal:1.0");
  return CORBA::ValueDef::_narrow (cv);
}

void
CORBA::CustomMarshal::_get_marshal_info (vector<string> & repoids, CORBA::Boolean & chunked)
{
  repoids.push_back ("IDL:omg.org/CORBA/CustomMarshal:1.0");
  chunked = FALSE;
}

void
CORBA::CustomMarshal::_marshal_members (CORBA::DataEncoder &ec)
{
}

CORBA::Boolean
CORBA::CustomMarshal::_demarshal_members (CORBA::DataDecoder &dc)
{
  return TRUE;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_CustomMarshal;
}

class _Marshaller_CORBA_CustomMarshal : public ::CORBA::StaticTypeInfo {
    typedef CORBA::CustomMarshal* _MICO_T;
  public:
    ~_Marshaller_CORBA_CustomMarshal();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_CustomMarshal::~_Marshaller_CORBA_CustomMarshal()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_CustomMarshal::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_CustomMarshal::assign( StaticValueType d, const StaticValueType s ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)d);
  ::CORBA::add_ref (*(_MICO_T*)s);
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_CustomMarshal::free( StaticValueType v ) const
{
  ::CORBA::remove_ref (*(_MICO_T*)v);
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_CustomMarshal::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ValueBase* vb = NULL;
  if (!::CORBA::ValueBase::_demarshal (dc, vb, "IDL:omg.org/CORBA/CustomMarshal:1.0")) {
    return FALSE;
  }
  ::CORBA::remove_ref (*(_MICO_T *)v);
  *(_MICO_T *)v = ::CORBA::CustomMarshal::_downcast (vb);
  if (vb && !*(_MICO_T *)v) {
    ::CORBA::remove_ref (vb);
    return FALSE;
  }
  return TRUE;
}

void _Marshaller_CORBA_CustomMarshal::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_CustomMarshal::typecode()
{
  return CORBA::_tc_CustomMarshal;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_CustomMarshal;

void
operator<<=( CORBA::Any &_a, const CORBA::CustomMarshal* _val )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_CustomMarshal, &_val);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::CustomMarshal** _val_ptr )
{
  CORBA::ValueBase_var _val = *_val_ptr;
  CORBA::StaticAny _sa (_marshaller_CORBA_CustomMarshal, _val_ptr);
  _a.from_static_any (_sa);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::CustomMarshal* &_val_ptr )
{
  CORBA::CustomMarshal* *p;
  if (_a.to_static_any (_marshaller_CORBA_CustomMarshal, (void *&)p)) {
    _val_ptr = *p;
    return TRUE;
  }
  return FALSE;
}

struct __tc_init_VALUETYPE {
  __tc_init_VALUETYPE()
  {
    CORBA::_tc_StringValue = 
    "010000001e00000044000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f537472696e6756616c75653a312e300000000c000000"
    "537472696e6756616c7565001200000000000000";
    _marshaller_CORBA_StringValue = new _Marshaller_CORBA_StringValue;
    CORBA::_tc_WStringValue = 
    "010000001e00000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f57537472696e6756616c75653a312e3000000d000000"
    "57537472696e6756616c7565000000001b00000000000000";
    _marshaller_CORBA_WStringValue = new _Marshaller_CORBA_WStringValue;
    CORBA::_tc_DataInputStream = 
    "010000001d00000050000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f44617461496e70757453747265616d3a312e30000000"
    "1000000044617461496e70757453747265616d0002000000000000000000"
    "0000";
    _marshaller_CORBA_DataInputStream = new _Marshaller_CORBA_DataInputStream;
    CORBA::_tc_DataOutputStream = 
    "010000001d00000050000000010000002700000049444c3a6f6d672e6f72"
    "672f434f5242412f446174614f757470757453747265616d3a312e300000"
    "11000000446174614f757470757453747265616d00000200000000000000"
    "0000";
    _marshaller_CORBA_DataOutputStream = new _Marshaller_CORBA_DataOutputStream;
    CORBA::_tc_CustomMarshal = 
    "010000001d00000048000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f437573746f6d4d61727368616c3a312e30000e000000"
    "437573746f6d4d61727368616c0002000000000000000000";
    _marshaller_CORBA_CustomMarshal = new _Marshaller_CORBA_CustomMarshal;
  }

  ~__tc_init_VALUETYPE()
  {
    delete static_cast<_Marshaller_CORBA_StringValue*>(_marshaller_CORBA_StringValue);
    delete static_cast<_Marshaller_CORBA_WStringValue*>(_marshaller_CORBA_WStringValue);
    delete static_cast<_Marshaller_CORBA_DataInputStream*>(_marshaller_CORBA_DataInputStream);
    delete static_cast<_Marshaller_CORBA_DataOutputStream*>(_marshaller_CORBA_DataOutputStream);
    delete static_cast<_Marshaller_CORBA_CustomMarshal*>(_marshaller_CORBA_CustomMarshal);
  }
};

static __tc_init_VALUETYPE __init_VALUETYPE;

