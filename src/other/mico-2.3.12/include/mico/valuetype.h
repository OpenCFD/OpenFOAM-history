/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __VALUETYPE_H__
#define __VALUETYPE_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace CORBA
{


// Common definitions for valuebox StringValue
class StringValue : 
  virtual public ::CORBA::ValueBase,
  virtual public ::CORBA::DefaultValueRefCountBase
{
  private:
    ::CORBA::String_var value;

  public:
    StringValue ();
    StringValue (char *);
    StringValue (const char *);
    StringValue (const ::CORBA::String_var &);
    StringValue (const StringValue &);

    StringValue & operator= (char *);
    StringValue & operator= (const char *);
    StringValue & operator= (const ::CORBA::String_var &);

    const char * _value () const;
    void _value (char *);
    void _value (const char *);
    void _value (const ::CORBA::String_var &);

    char & operator[] (::CORBA::ULong);
    char operator[] (::CORBA::ULong) const;

    const char* _boxed_in () const;
    char*& _boxed_inout ();
    CORBA::String_out _boxed_out ();

    static StringValue* _downcast (::CORBA::ValueBase *);
    static StringValue* _downcast (::CORBA::AbstractBase *);

    ::CORBA::ValueDef_ptr get_value_def ();
    void * _narrow_helper (const char *);
    ::CORBA::ValueBase * _copy_value ();
    void _get_marshal_info (std::vector<std::string> &, ::CORBA::Boolean &);
    void _marshal_members (::CORBA::DataEncoder &);
    ::CORBA::Boolean _demarshal_members (::CORBA::DataDecoder &);

  protected:
    ~StringValue ();

  private:
    void operator= (const StringValue &);
};

typedef ValueVar< StringValue > StringValue_var;
typedef ValueOut< StringValue > StringValue_out;

extern MICO_EXPORT ::CORBA::TypeCodeConst _tc_StringValue;


// Common definitions for valuebox WStringValue
class WStringValue : 
  virtual public ::CORBA::ValueBase,
  virtual public ::CORBA::DefaultValueRefCountBase
{
  private:
    ::CORBA::WString_var value;

  public:
    WStringValue ();
    WStringValue (::CORBA::WChar *);
    WStringValue (const ::CORBA::WChar *);
    WStringValue (const ::CORBA::WString_var &);
    WStringValue (const WStringValue &);

    WStringValue & operator= (::CORBA::WChar *);
    WStringValue & operator= (const ::CORBA::WChar *);
    WStringValue & operator= (const ::CORBA::WString_var &);

    const ::CORBA::WChar * _value () const;
    void _value (::CORBA::WChar *);
    void _value (const ::CORBA::WChar *);
    void _value (const ::CORBA::WString_var &);

    ::CORBA::WChar & operator[] (::CORBA::ULong);
    ::CORBA::WChar operator[] (::CORBA::ULong) const;

    const CORBA::WChar* _boxed_in () const;
    CORBA::WChar*& _boxed_inout ();
    CORBA::WString_out _boxed_out ();

    static WStringValue* _downcast (::CORBA::ValueBase *);
    static WStringValue* _downcast (::CORBA::AbstractBase *);

    ::CORBA::ValueDef_ptr get_value_def ();
    void * _narrow_helper (const char *);
    ::CORBA::ValueBase * _copy_value ();
    void _get_marshal_info (std::vector<std::string> &, ::CORBA::Boolean &);
    void _marshal_members (::CORBA::DataEncoder &);
    ::CORBA::Boolean _demarshal_members (::CORBA::DataDecoder &);

  protected:
    ~WStringValue ();

  private:
    void operator= (const WStringValue &);
};

typedef ValueVar< WStringValue > WStringValue_var;
typedef ValueOut< WStringValue > WStringValue_out;

extern MICO_EXPORT ::CORBA::TypeCodeConst _tc_WStringValue;

class DataInputStream;
typedef DataInputStream *DataInputStream_ptr;
typedef DataInputStream_ptr DataInputStreamRef;
typedef ValueVar< DataInputStream > DataInputStream_var;
typedef ValueOut< DataInputStream > DataInputStream_out;


// Common definitions for valuetype DataInputStream
class DataInputStream : 
  virtual public CORBA::ValueBase
{
  public:
    static DataInputStream* _downcast (CORBA::ValueBase *);
    static DataInputStream* _downcast (CORBA::AbstractBase *);

    virtual CORBA::Any* read_any() = 0;
    virtual CORBA::Boolean read_boolean() = 0;
    virtual CORBA::Char read_char() = 0;
    virtual CORBA::WChar read_wchar() = 0;
    virtual CORBA::Octet read_octet() = 0;
    virtual CORBA::Short read_short() = 0;
    virtual CORBA::UShort read_ushort() = 0;
    virtual CORBA::Long read_long() = 0;
    virtual CORBA::ULong read_ulong() = 0;
    virtual CORBA::LongLong read_longlong() = 0;
    virtual CORBA::ULongLong read_ulonglong() = 0;
    virtual CORBA::Float read_float() = 0;
    virtual CORBA::Double read_double() = 0;
    virtual CORBA::LongDouble read_longdouble() = 0;
    virtual char* read_string() = 0;
    virtual CORBA::WChar* read_wstring() = 0;
    virtual CORBA::Object_ptr read_Object() = 0;
    virtual CORBA::AbstractBase_ptr read_Abstract() = 0;
    virtual CORBA::ValueBase* read_Value() = 0;
    virtual CORBA::TypeCode_ptr read_TypeCode() = 0;
    virtual void read_any_array( AnySeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_boolean_array( BooleanSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_char_array( CharSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_wchar_array( WCharSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_octet_array( OctetSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_short_array( ShortSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_ushort_array( UShortSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_long_array( LongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_ulong_array( ULongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_longlong_array( LongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_ulonglong_array( ULongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_float_array( FloatSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void read_double_array( DoubleSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;

  public:
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    DataInputStream ();
    virtual ~DataInputStream ();

  private:
    DataInputStream (const DataInputStream &);
    void operator= (const DataInputStream &);
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DataInputStream;

class DataOutputStream;
typedef DataOutputStream *DataOutputStream_ptr;
typedef DataOutputStream_ptr DataOutputStreamRef;
typedef ValueVar< DataOutputStream > DataOutputStream_var;
typedef ValueOut< DataOutputStream > DataOutputStream_out;


// Common definitions for valuetype DataOutputStream
class DataOutputStream : 
  virtual public CORBA::ValueBase
{
  public:
    static DataOutputStream* _downcast (CORBA::ValueBase *);
    static DataOutputStream* _downcast (CORBA::AbstractBase *);

    virtual void write_any( const CORBA::Any& value ) = 0;
    virtual void write_boolean( CORBA::Boolean value ) = 0;
    virtual void write_char( CORBA::Char value ) = 0;
    virtual void write_wchar( CORBA::WChar value ) = 0;
    virtual void write_octet( CORBA::Octet value ) = 0;
    virtual void write_short( CORBA::Short value ) = 0;
    virtual void write_ushort( CORBA::UShort value ) = 0;
    virtual void write_long( CORBA::Long value ) = 0;
    virtual void write_ulong( CORBA::ULong value ) = 0;
    virtual void write_longlong( CORBA::LongLong value ) = 0;
    virtual void write_ulonglong( CORBA::ULongLong value ) = 0;
    virtual void write_float( CORBA::Float value ) = 0;
    virtual void write_double( CORBA::Double value ) = 0;
    virtual void write_longdouble( CORBA::LongDouble value ) = 0;
    virtual void write_string( const char* value ) = 0;
    virtual void write_wstring( const CORBA::WChar* value ) = 0;
    virtual void write_Object( CORBA::Object_ptr value ) = 0;
    virtual void write_Abstract( CORBA::AbstractBase_ptr value ) = 0;
    virtual void write_Value( CORBA::ValueBase* value ) = 0;
    virtual void write_TypeCode( CORBA::TypeCode_ptr value ) = 0;
    virtual void write_any_array( const AnySeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_boolean_array( const BooleanSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_char_array( const CharSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_wchar_array( const WCharSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_octet_array( const OctetSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_short_array( const ShortSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_ushort_array( const UShortSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_long_array( const LongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_ulong_array( const ULongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_longlong_array( const LongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_ulonglong_array( const ULongLongSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_float_array( const FloatSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;
    virtual void write_double_array( const DoubleSeq& seq, CORBA::ULong offset, CORBA::ULong length ) = 0;

  public:
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    DataOutputStream ();
    virtual ~DataOutputStream ();

  private:
    DataOutputStream (const DataOutputStream &);
    void operator= (const DataOutputStream &);
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DataOutputStream;

class CustomMarshal;
typedef CustomMarshal *CustomMarshal_ptr;
typedef CustomMarshal_ptr CustomMarshalRef;
typedef ValueVar< CustomMarshal > CustomMarshal_var;
typedef ValueOut< CustomMarshal > CustomMarshal_out;


// Common definitions for valuetype CustomMarshal
class CustomMarshal : 
  virtual public CORBA::ValueBase
{
  public:
    static CustomMarshal* _downcast (CORBA::ValueBase *);
    static CustomMarshal* _downcast (CORBA::AbstractBase *);

    virtual void marshal( DataOutputStream* os ) = 0;
    virtual void unmarshal( DataInputStream* is ) = 0;

  public:
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    CustomMarshal ();
    virtual ~CustomMarshal ();

  private:
    CustomMarshal (const CustomMarshal &);
    void operator= (const CustomMarshal &);
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CustomMarshal;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA



namespace OBV_CORBA
{

}


void operator<<=( ::CORBA::Any &a, const ::CORBA::StringValue* vb );
void operator<<=( ::CORBA::Any &a, ::CORBA::StringValue** vb_ptr );
::CORBA::Boolean operator>>=( const ::CORBA::Any &a, ::CORBA::StringValue* & vb_ptr );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_StringValue;

void operator<<=( ::CORBA::Any &a, const ::CORBA::WStringValue* vb );
void operator<<=( ::CORBA::Any &a, ::CORBA::WStringValue** vb_ptr );
::CORBA::Boolean operator>>=( const ::CORBA::Any &a, ::CORBA::WStringValue* & vb_ptr );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_WStringValue;

void operator<<=( CORBA::Any &a, const CORBA::DataInputStream* val );
void operator<<=( CORBA::Any &a, CORBA::DataInputStream** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::DataInputStream* & val_ptr );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_DataInputStream;

void operator<<=( CORBA::Any &a, const CORBA::DataOutputStream* val );
void operator<<=( CORBA::Any &a, CORBA::DataOutputStream** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::DataOutputStream* & val_ptr );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_DataOutputStream;

void operator<<=( CORBA::Any &a, const CORBA::CustomMarshal* val );
void operator<<=( CORBA::Any &a, CORBA::CustomMarshal** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::CustomMarshal* & val_ptr );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_CustomMarshal;

#endif
