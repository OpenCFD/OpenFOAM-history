#include <CORBA.h>
#include <mico/template_impl.h>

#ifdef _WIN32


namespace CORBA {
  CORBA::TypeCode_ptr _tc_null;
  CORBA::TypeCode_ptr _tc_void;
  CORBA::TypeCode_ptr _tc_short;
  CORBA::TypeCode_ptr _tc_long;
  CORBA::TypeCode_ptr _tc_longlong;
  CORBA::TypeCode_ptr _tc_ushort;
  CORBA::TypeCode_ptr _tc_ulong;
  CORBA::TypeCode_ptr _tc_ulonglong;
  CORBA::TypeCode_ptr _tc_float;
  CORBA::TypeCode_ptr _tc_double;
  CORBA::TypeCode_ptr _tc_longdouble;
  CORBA::TypeCode_ptr _tc_boolean;
  CORBA::TypeCode_ptr _tc_char;
  CORBA::TypeCode_ptr _tc_wchar;
  CORBA::TypeCode_ptr _tc_octet;
  CORBA::TypeCode_ptr _tc_any;
  CORBA::TypeCode_ptr _tc_TypeCode;
  CORBA::TypeCode_ptr _tc_Principal;
  CORBA::TypeCode_ptr _tc_Object;
  CORBA::TypeCode_ptr _tc_string;
  CORBA::TypeCode_ptr _tc_wstring;
  CORBA::TypeCode_ptr _tc_Context;

  StaticTypeInfo *_stc_void;
  StaticTypeInfo *_stc_short;
  StaticTypeInfo *_stc_long;
  StaticTypeInfo *_stc_longlong;
  StaticTypeInfo *_stc_ushort;
  StaticTypeInfo *_stc_ulong;
  StaticTypeInfo *_stc_ulonglong;
  StaticTypeInfo *_stc_float;
  StaticTypeInfo *_stc_double;
  StaticTypeInfo *_stc_longdouble;
  StaticTypeInfo *_stc_char;
  StaticTypeInfo *_stc_wchar;
  StaticTypeInfo *_stc_octet;
  StaticTypeInfo *_stc_boolean;
  StaticTypeInfo *_stc_any;
  StaticTypeInfo *_stc_TypeCode;
  StaticTypeInfo *_stc_Principal;
  StaticTypeInfo *_stc_Context;
  StaticTypeInfo *_stc_Object;
  StaticTypeInfo *_stc_string;
  StaticTypeInfo *_stc_wstring;
  StaticTypeInfo *_stcseq_short;
  StaticTypeInfo *_stcseq_long;
  StaticTypeInfo *_stcseq_longlong;
  StaticTypeInfo *_stcseq_ushort;
  StaticTypeInfo *_stcseq_ulong;
  StaticTypeInfo *_stcseq_ulonglong;
  StaticTypeInfo *_stcseq_float;
  StaticTypeInfo *_stcseq_double;
  StaticTypeInfo *_stcseq_longdouble;
  StaticTypeInfo *_stcseq_char;
  StaticTypeInfo *_stcseq_wchar;
  StaticTypeInfo *_stcseq_octet;
  StaticTypeInfo *_stcseq_boolean;
  StaticTypeInfo *_stcseq_string;
  StaticTypeInfo *_stcseq_wstring;
  StaticTypeInfo *_stcseq_Object;

  CORBA::TypeCodeConst _tc_AliasDef;
  CORBA::TypeCodeConst _tc_ArrayDef;
  CORBA::TypeCodeConst _tc_AttrDescriptionSeq;
  CORBA::TypeCodeConst _tc_AttributeDef;
  CORBA::TypeCodeConst _tc_AttributeDescription;
  CORBA::TypeCodeConst _tc_AttributeMode;
  CORBA::TypeCodeConst _tc_ConstantDef;
  CORBA::TypeCodeConst _tc_ConstantDescription;
  CORBA::TypeCodeConst _tc_Contained;
  CORBA::TypeCodeConst _tc_ContainedSeq;
  CORBA::TypeCodeConst _tc_Container;
  CORBA::TypeCodeConst _tc_ContextIdSeq;
  CORBA::TypeCodeConst _tc_ContextIdentifier;
  CORBA::TypeCodeConst _tc_DefinitionKind;
  CORBA::TypeCodeConst _tc_Description;
  CORBA::TypeCodeConst _tc_EnumDef;
  CORBA::TypeCodeConst _tc_EnumMemberSeq;
  CORBA::TypeCodeConst _tc_ExcDescriptionSeq;
  CORBA::TypeCodeConst _tc_ExceptionDef;
  CORBA::TypeCodeConst _tc_ExceptionDefSeq;
  CORBA::TypeCodeConst _tc_ExceptionDescription;
  CORBA::TypeCodeConst _tc_FixedDef;
  CORBA::TypeCodeConst _tc_IDLType;
  CORBA::TypeCodeConst _tc_IDLTypeSeq;
  CORBA::TypeCodeConst _tc_IRObject;
  CORBA::TypeCodeConst _tc_Identifier;
  CORBA::TypeCodeConst _tc_ImplementationDef;
  CORBA::TypeCodeConst _tc_ImplRepository;
  CORBA::TypeCodeConst _tc_InterfaceDef;
  CORBA::TypeCodeConst _tc_InterfaceDefSeq;
  CORBA::TypeCodeConst _tc_InterfaceDescription;
  CORBA::TypeCodeConst _tc_ModuleDef;
  CORBA::TypeCodeConst _tc_ModuleDescription;
  CORBA::TypeCodeConst _tc_OAMediator;
  CORBA::TypeCodeConst _tc_OAServer;
  CORBA::TypeCodeConst _tc_OpDescriptionSeq;
  CORBA::TypeCodeConst _tc_OperationDef;
  CORBA::TypeCodeConst _tc_OperationDescription;
  CORBA::TypeCodeConst _tc_OperationMode;
  CORBA::TypeCodeConst _tc_ParDescriptionSeq;
  CORBA::TypeCodeConst _tc_ParameterDescription;
  CORBA::TypeCodeConst _tc_ParameterMode;
  CORBA::TypeCodeConst _tc_PrimitiveDef;
  CORBA::TypeCodeConst _tc_PrimitiveKind;
  CORBA::TypeCodeConst _tc_Repository;
  CORBA::TypeCodeConst _tc_RepositoryId;
  CORBA::TypeCodeConst _tc_RepositoryIdSeq;
  CORBA::TypeCodeConst _tc_ScopedName;
  CORBA::TypeCodeConst _tc_SequenceDef;
  CORBA::TypeCodeConst _tc_StringDef;
  CORBA::TypeCodeConst _tc_StructDef;
  CORBA::TypeCodeConst _tc_StructMember;
  CORBA::TypeCodeConst _tc_StructMemberSeq;
  CORBA::TypeCodeConst _tc_TypeDescription;
  CORBA::TypeCodeConst _tc_TypedefDef;
  CORBA::TypeCodeConst _tc_UnionDef;
  CORBA::TypeCodeConst _tc_UnionMember;
  CORBA::TypeCodeConst _tc_UnionMemberSeq;
  CORBA::TypeCodeConst _tc_VersionSpec;
  CORBA::TypeCodeConst _tc_WstringDef;

  CORBA::TypeCodeConst Container::_tc_Description;
}






#endif // _WIN32
