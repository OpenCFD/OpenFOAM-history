/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __DYNANY_H__
#define __DYNANY_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace DynamicAny
{

class DynAny;
typedef DynAny *DynAny_ptr;
typedef DynAny_ptr DynAnyRef;
typedef ObjVar< DynAny > DynAny_var;
typedef ObjOut< DynAny > DynAny_out;

class DynBasic;
typedef DynBasic *DynBasic_ptr;
typedef DynBasic_ptr DynBasicRef;
typedef ObjVar< DynBasic > DynBasic_var;
typedef ObjOut< DynBasic > DynBasic_out;

class DynFixed;
typedef DynFixed *DynFixed_ptr;
typedef DynFixed_ptr DynFixedRef;
typedef ObjVar< DynFixed > DynFixed_var;
typedef ObjOut< DynFixed > DynFixed_out;

class DynEnum;
typedef DynEnum *DynEnum_ptr;
typedef DynEnum_ptr DynEnumRef;
typedef ObjVar< DynEnum > DynEnum_var;
typedef ObjOut< DynEnum > DynEnum_out;

class DynStruct;
typedef DynStruct *DynStruct_ptr;
typedef DynStruct_ptr DynStructRef;
typedef ObjVar< DynStruct > DynStruct_var;
typedef ObjOut< DynStruct > DynStruct_out;

class DynUnion;
typedef DynUnion *DynUnion_ptr;
typedef DynUnion_ptr DynUnionRef;
typedef ObjVar< DynUnion > DynUnion_var;
typedef ObjOut< DynUnion > DynUnion_out;

class DynSequence;
typedef DynSequence *DynSequence_ptr;
typedef DynSequence_ptr DynSequenceRef;
typedef ObjVar< DynSequence > DynSequence_var;
typedef ObjOut< DynSequence > DynSequence_out;

class DynArray;
typedef DynArray *DynArray_ptr;
typedef DynArray_ptr DynArrayRef;
typedef ObjVar< DynArray > DynArray_var;
typedef ObjOut< DynArray > DynArray_out;

class DynValueCommon;
typedef DynValueCommon *DynValueCommon_ptr;
typedef DynValueCommon_ptr DynValueCommonRef;
typedef ObjVar< DynValueCommon > DynValueCommon_var;
typedef ObjOut< DynValueCommon > DynValueCommon_out;

class DynValue;
typedef DynValue *DynValue_ptr;
typedef DynValue_ptr DynValueRef;
typedef ObjVar< DynValue > DynValue_var;
typedef ObjOut< DynValue > DynValue_out;

class DynValueBox;
typedef DynValueBox *DynValueBox_ptr;
typedef DynValueBox_ptr DynValueBoxRef;
typedef ObjVar< DynValueBox > DynValueBox_var;
typedef ObjOut< DynValueBox > DynValueBox_out;

class DynAnyFactory;
typedef DynAnyFactory *DynAnyFactory_ptr;
typedef DynAnyFactory_ptr DynAnyFactoryRef;
typedef ObjVar< DynAnyFactory > DynAnyFactory_var;
typedef ObjOut< DynAnyFactory > DynAnyFactory_out;

}






namespace DynamicAny
{


/*
 * Base class and common definitions for local interface DynAny
 */

class DynAny : 
  virtual public CORBA::Object
{
  public:
    virtual ~DynAny();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynAny_ptr _ptr_type;
    typedef DynAny_var _var_type;
    #endif

    static DynAny_ptr _narrow( CORBA::Object_ptr obj );
    static DynAny_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynAny_ptr _duplicate( DynAny_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynAny_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct InvalidValue : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      InvalidValue();
      ~InvalidValue();
      InvalidValue( const InvalidValue& s );
      InvalidValue& operator=( const InvalidValue& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      InvalidValue *operator->() { return this; }
      InvalidValue& operator*() { return *this; }
      operator InvalidValue*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static InvalidValue *_downcast( CORBA::Exception *ex );
      static const InvalidValue *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef InvalidValue InvalidValue_catch;
    #else
    typedef ExceptVar< InvalidValue > InvalidValue_var;
    typedef TVarOut< InvalidValue > InvalidValue_out;
    typedef InvalidValue_var InvalidValue_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidValue;

    struct TypeMismatch : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      TypeMismatch();
      ~TypeMismatch();
      TypeMismatch( const TypeMismatch& s );
      TypeMismatch& operator=( const TypeMismatch& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      TypeMismatch *operator->() { return this; }
      TypeMismatch& operator*() { return *this; }
      operator TypeMismatch*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static TypeMismatch *_downcast( CORBA::Exception *ex );
      static const TypeMismatch *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef TypeMismatch TypeMismatch_catch;
    #else
    typedef ExceptVar< TypeMismatch > TypeMismatch_var;
    typedef TVarOut< TypeMismatch > TypeMismatch_out;
    typedef TypeMismatch_var TypeMismatch_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_TypeMismatch;

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual void assign( DynAny_ptr dyn_any ) = 0;
    virtual void from_any( const CORBA::Any& value ) = 0;
    virtual CORBA::Any* to_any() = 0;
    virtual CORBA::Boolean equal( DynAny_ptr dyn_any ) = 0;
    virtual void destroy() = 0;
    virtual DynAny_ptr copy() = 0;
    virtual void insert_boolean( CORBA::Boolean value ) = 0;
    virtual void insert_octet( CORBA::Octet value ) = 0;
    virtual void insert_char( CORBA::Char value ) = 0;
    virtual void insert_short( CORBA::Short value ) = 0;
    virtual void insert_ushort( CORBA::UShort value ) = 0;
    virtual void insert_long( CORBA::Long value ) = 0;
    virtual void insert_ulong( CORBA::ULong value ) = 0;
    virtual void insert_float( CORBA::Float value ) = 0;
    virtual void insert_double( CORBA::Double value ) = 0;
    virtual void insert_string( const char* value ) = 0;
    virtual void insert_reference( CORBA::Object_ptr value ) = 0;
    virtual void insert_typecode( CORBA::TypeCode_ptr value ) = 0;
    virtual void insert_longlong( CORBA::LongLong value ) = 0;
    virtual void insert_ulonglong( CORBA::ULongLong value ) = 0;
    virtual void insert_longdouble( CORBA::LongDouble value ) = 0;
    virtual void insert_wchar( CORBA::WChar value ) = 0;
    virtual void insert_wstring( const CORBA::WChar* value ) = 0;
    virtual void insert_any( const CORBA::Any& value ) = 0;
    virtual void insert_dyn_any( DynAny_ptr value ) = 0;
    virtual void insert_val( CORBA::ValueBase* value ) = 0;
    virtual CORBA::Boolean get_boolean() = 0;
    virtual CORBA::Octet get_octet() = 0;
    virtual CORBA::Char get_char() = 0;
    virtual CORBA::Short get_short() = 0;
    virtual CORBA::UShort get_ushort() = 0;
    virtual CORBA::Long get_long() = 0;
    virtual CORBA::ULong get_ulong() = 0;
    virtual CORBA::Float get_float() = 0;
    virtual CORBA::Double get_double() = 0;
    virtual char* get_string() = 0;
    virtual CORBA::Object_ptr get_reference() = 0;
    virtual CORBA::TypeCode_ptr get_typecode() = 0;
    virtual CORBA::LongLong get_longlong() = 0;
    virtual CORBA::ULongLong get_ulonglong() = 0;
    virtual CORBA::LongDouble get_longdouble() = 0;
    virtual CORBA::WChar get_wchar() = 0;
    virtual CORBA::WChar* get_wstring() = 0;
    virtual CORBA::Any* get_any() = 0;
    virtual DynAny_ptr get_dyn_any() = 0;
    virtual CORBA::ValueBase* get_val() = 0;
    virtual CORBA::Boolean seek( CORBA::Long index ) = 0;
    virtual void rewind() = 0;
    virtual CORBA::Boolean next() = 0;
    virtual CORBA::ULong component_count() = 0;
    virtual DynAny_ptr current_component() = 0;
    virtual void insert_abstract( CORBA::AbstractBase_ptr value ) = 0;
    virtual CORBA::AbstractBase_ptr get_abstract() = 0;

  protected:
    DynAny() {};
  private:
    DynAny( const DynAny& );
    void operator=( const DynAny& );
};



/*
 * Base class and common definitions for local interface DynBasic
 */

class DynBasic : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynBasic();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynBasic_ptr _ptr_type;
    typedef DynBasic_var _var_type;
    #endif

    static DynBasic_ptr _narrow( CORBA::Object_ptr obj );
    static DynBasic_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynBasic_ptr _duplicate( DynBasic_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynBasic_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    DynBasic() {};
  private:
    DynBasic( const DynBasic& );
    void operator=( const DynBasic& );
};



/*
 * Base class and common definitions for local interface DynFixed
 */

class DynFixed : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynFixed();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynFixed_ptr _ptr_type;
    typedef DynFixed_var _var_type;
    #endif

    static DynFixed_ptr _narrow( CORBA::Object_ptr obj );
    static DynFixed_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynFixed_ptr _duplicate( DynFixed_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynFixed_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* get_value() = 0;
    virtual void set_value( const char* val ) = 0;

  protected:
    DynFixed() {};
  private:
    DynFixed( const DynFixed& );
    void operator=( const DynFixed& );
};



/*
 * Base class and common definitions for local interface DynEnum
 */

class DynEnum : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynEnum();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynEnum_ptr _ptr_type;
    typedef DynEnum_var _var_type;
    #endif

    static DynEnum_ptr _narrow( CORBA::Object_ptr obj );
    static DynEnum_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynEnum_ptr _duplicate( DynEnum_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynEnum_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* get_as_string() = 0;
    virtual void set_as_string( const char* value ) = 0;
    virtual CORBA::ULong get_as_ulong() = 0;
    virtual void set_as_ulong( CORBA::ULong value ) = 0;

  protected:
    DynEnum() {};
  private:
    DynEnum( const DynEnum& );
    void operator=( const DynEnum& );
};


typedef char* FieldName;
typedef CORBA::String_var FieldName_var;
typedef CORBA::String_out FieldName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_FieldName;

struct NameValuePair;
typedef TVarVar< NameValuePair > NameValuePair_var;
typedef TVarOut< NameValuePair > NameValuePair_out;


struct NameValuePair {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef NameValuePair_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  NameValuePair();
  ~NameValuePair();
  NameValuePair( const NameValuePair& s );
  NameValuePair& operator=( const NameValuePair& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  FieldName_var id;
  CORBA::Any value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_NameValuePair;

typedef SequenceTmpl< NameValuePair,MICO_TID_DEF> NameValuePairSeq;
typedef TSeqVar< SequenceTmpl< NameValuePair,MICO_TID_DEF> > NameValuePairSeq_var;
typedef TSeqOut< SequenceTmpl< NameValuePair,MICO_TID_DEF> > NameValuePairSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_NameValuePairSeq;

struct NameDynAnyPair;
typedef TVarVar< NameDynAnyPair > NameDynAnyPair_var;
typedef TVarOut< NameDynAnyPair > NameDynAnyPair_out;


struct NameDynAnyPair {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef NameDynAnyPair_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  NameDynAnyPair();
  ~NameDynAnyPair();
  NameDynAnyPair( const NameDynAnyPair& s );
  NameDynAnyPair& operator=( const NameDynAnyPair& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  FieldName_var id;
  DynAny_var value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_NameDynAnyPair;

typedef SequenceTmpl< NameDynAnyPair,MICO_TID_DEF> NameDynAnyPairSeq;
typedef TSeqVar< SequenceTmpl< NameDynAnyPair,MICO_TID_DEF> > NameDynAnyPairSeq_var;
typedef TSeqOut< SequenceTmpl< NameDynAnyPair,MICO_TID_DEF> > NameDynAnyPairSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_NameDynAnyPairSeq;


/*
 * Base class and common definitions for local interface DynStruct
 */

class DynStruct : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynStruct();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynStruct_ptr _ptr_type;
    typedef DynStruct_var _var_type;
    #endif

    static DynStruct_ptr _narrow( CORBA::Object_ptr obj );
    static DynStruct_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynStruct_ptr _duplicate( DynStruct_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynStruct_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* current_member_name() = 0;
    virtual ::CORBA::TCKind current_member_kind() = 0;
    virtual NameValuePairSeq* get_members() = 0;
    virtual void set_members( const NameValuePairSeq& value ) = 0;
    virtual NameDynAnyPairSeq* get_members_as_dyn_any() = 0;
    virtual void set_members_as_dyn_any( const NameDynAnyPairSeq& value ) = 0;

  protected:
    DynStruct() {};
  private:
    DynStruct( const DynStruct& );
    void operator=( const DynStruct& );
};



/*
 * Base class and common definitions for local interface DynUnion
 */

class DynUnion : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynUnion();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynUnion_ptr _ptr_type;
    typedef DynUnion_var _var_type;
    #endif

    static DynUnion_ptr _narrow( CORBA::Object_ptr obj );
    static DynUnion_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynUnion_ptr _duplicate( DynUnion_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynUnion_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual DynAny_ptr get_discriminator() = 0;
    virtual void set_discriminator( DynAny_ptr d ) = 0;
    virtual void set_to_default_member() = 0;
    virtual void set_to_no_active_member() = 0;
    virtual CORBA::Boolean has_no_active_member() = 0;
    virtual ::CORBA::TCKind discriminator_kind() = 0;
    virtual DynAny_ptr member() = 0;
    virtual char* member_name() = 0;
    virtual ::CORBA::TCKind member_kind() = 0;

  protected:
    DynUnion() {};
  private:
    DynUnion( const DynUnion& );
    void operator=( const DynUnion& );
};


typedef SequenceTmpl< CORBA::Any,MICO_TID_DEF> AnySeq;
typedef TSeqVar< SequenceTmpl< CORBA::Any,MICO_TID_DEF> > AnySeq_var;
typedef TSeqOut< SequenceTmpl< CORBA::Any,MICO_TID_DEF> > AnySeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AnySeq;

typedef IfaceSequenceTmpl< DynAny_var,DynAny_ptr> DynAnySeq;
typedef TSeqVar< IfaceSequenceTmpl< DynAny_var,DynAny_ptr> > DynAnySeq_var;
typedef TSeqOut< IfaceSequenceTmpl< DynAny_var,DynAny_ptr> > DynAnySeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DynAnySeq;


/*
 * Base class and common definitions for local interface DynSequence
 */

class DynSequence : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynSequence();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynSequence_ptr _ptr_type;
    typedef DynSequence_var _var_type;
    #endif

    static DynSequence_ptr _narrow( CORBA::Object_ptr obj );
    static DynSequence_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynSequence_ptr _duplicate( DynSequence_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynSequence_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong get_length() = 0;
    virtual void set_length( CORBA::ULong len ) = 0;
    virtual AnySeq* get_elements() = 0;
    virtual void set_elements( const AnySeq& value ) = 0;
    virtual DynAnySeq* get_elements_as_dyn_any() = 0;
    virtual void set_elements_as_dyn_any( const DynAnySeq& value ) = 0;

  protected:
    DynSequence() {};
  private:
    DynSequence( const DynSequence& );
    void operator=( const DynSequence& );
};



/*
 * Base class and common definitions for local interface DynArray
 */

class DynArray : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynArray();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynArray_ptr _ptr_type;
    typedef DynArray_var _var_type;
    #endif

    static DynArray_ptr _narrow( CORBA::Object_ptr obj );
    static DynArray_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynArray_ptr _duplicate( DynArray_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynArray_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual AnySeq* get_elements() = 0;
    virtual void set_elements( const AnySeq& value ) = 0;
    virtual DynAnySeq* get_elements_as_dyn_any() = 0;
    virtual void set_elements_as_dyn_any( const DynAnySeq& value ) = 0;

  protected:
    DynArray() {};
  private:
    DynArray( const DynArray& );
    void operator=( const DynArray& );
};



/*
 * Base class and common definitions for local interface DynValueCommon
 */

class DynValueCommon : 
  virtual public ::DynamicAny::DynAny
{
  public:
    virtual ~DynValueCommon();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynValueCommon_ptr _ptr_type;
    typedef DynValueCommon_var _var_type;
    #endif

    static DynValueCommon_ptr _narrow( CORBA::Object_ptr obj );
    static DynValueCommon_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynValueCommon_ptr _duplicate( DynValueCommon_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynValueCommon_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Boolean is_null() = 0;
    virtual void set_to_null() = 0;
    virtual void set_to_value() = 0;

  protected:
    DynValueCommon() {};
  private:
    DynValueCommon( const DynValueCommon& );
    void operator=( const DynValueCommon& );
};



/*
 * Base class and common definitions for local interface DynValue
 */

class DynValue : 
  virtual public ::DynamicAny::DynValueCommon
{
  public:
    virtual ~DynValue();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynValue_ptr _ptr_type;
    typedef DynValue_var _var_type;
    #endif

    static DynValue_ptr _narrow( CORBA::Object_ptr obj );
    static DynValue_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynValue_ptr _duplicate( DynValue_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynValue_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* current_member_name() = 0;
    virtual ::CORBA::TCKind current_member_kind() = 0;
    virtual NameValuePairSeq* get_members() = 0;
    virtual void set_members( const NameValuePairSeq& values ) = 0;
    virtual NameDynAnyPairSeq* get_members_as_dyn_any() = 0;
    virtual void set_members_as_dyn_any( const NameDynAnyPairSeq& values ) = 0;

  protected:
    DynValue() {};
  private:
    DynValue( const DynValue& );
    void operator=( const DynValue& );
};



/*
 * Base class and common definitions for local interface DynValueBox
 */

class DynValueBox : 
  virtual public ::DynamicAny::DynValueCommon
{
  public:
    virtual ~DynValueBox();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynValueBox_ptr _ptr_type;
    typedef DynValueBox_var _var_type;
    #endif

    static DynValueBox_ptr _narrow( CORBA::Object_ptr obj );
    static DynValueBox_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynValueBox_ptr _duplicate( DynValueBox_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynValueBox_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Any* get_boxed_value() = 0;
    virtual void set_boxed_value( const CORBA::Any& boxed ) = 0;
    virtual DynAny_ptr get_boxed_value_as_dyn_any() = 0;
    virtual void set_boxed_value_as_dyn_any( DynAny_ptr boxed ) = 0;

  protected:
    DynValueBox() {};
  private:
    DynValueBox( const DynValueBox& );
    void operator=( const DynValueBox& );
};



/*
 * Base class and common definitions for local interface DynAnyFactory
 */

class DynAnyFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~DynAnyFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DynAnyFactory_ptr _ptr_type;
    typedef DynAnyFactory_var _var_type;
    #endif

    static DynAnyFactory_ptr _narrow( CORBA::Object_ptr obj );
    static DynAnyFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DynAnyFactory_ptr _duplicate( DynAnyFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DynAnyFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct InconsistentTypeCode : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      InconsistentTypeCode();
      ~InconsistentTypeCode();
      InconsistentTypeCode( const InconsistentTypeCode& s );
      InconsistentTypeCode& operator=( const InconsistentTypeCode& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      InconsistentTypeCode *operator->() { return this; }
      InconsistentTypeCode& operator*() { return *this; }
      operator InconsistentTypeCode*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static InconsistentTypeCode *_downcast( CORBA::Exception *ex );
      static const InconsistentTypeCode *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef InconsistentTypeCode InconsistentTypeCode_catch;
    #else
    typedef ExceptVar< InconsistentTypeCode > InconsistentTypeCode_var;
    typedef TVarOut< InconsistentTypeCode > InconsistentTypeCode_out;
    typedef InconsistentTypeCode_var InconsistentTypeCode_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_InconsistentTypeCode;

    virtual DynAny_ptr create_dyn_any( const CORBA::Any& value ) = 0;
    virtual DynAny_ptr create_dyn_any_from_type_code( CORBA::TypeCode_ptr type ) = 0;

  protected:
    DynAnyFactory() {};
  private:
    DynAnyFactory( const DynAnyFactory& );
    void operator=( const DynAnyFactory& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::DynamicAny::DynAny::InvalidValue &e );
void operator<<=( CORBA::Any &a, ::DynamicAny::DynAny::InvalidValue *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::DynamicAny::DynAny::InvalidValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::DynamicAny::DynAny::InvalidValue *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAny_InvalidValue;

void operator<<=( CORBA::Any &a, const ::DynamicAny::DynAny::TypeMismatch &e );
void operator<<=( CORBA::Any &a, ::DynamicAny::DynAny::TypeMismatch *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::DynamicAny::DynAny::TypeMismatch &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::DynamicAny::DynAny::TypeMismatch *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAny_TypeMismatch;

void operator<<=( CORBA::Any &_a, const ::DynamicAny::NameValuePair &_s );
void operator<<=( CORBA::Any &_a, ::DynamicAny::NameValuePair *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::DynamicAny::NameValuePair &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::DynamicAny::NameValuePair *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_DynamicAny_NameValuePair;

void operator<<=( CORBA::Any &a, const ::DynamicAny::DynAnyFactory::InconsistentTypeCode &e );
void operator<<=( CORBA::Any &a, ::DynamicAny::DynAnyFactory::InconsistentTypeCode *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::DynamicAny::DynAnyFactory::InconsistentTypeCode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::DynamicAny::DynAnyFactory::InconsistentTypeCode *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_DynamicAny_DynAnyFactory_InconsistentTypeCode;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< DynamicAny::NameValuePair,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_DynamicAny_NameValuePair;

#endif
