/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __GSSUP_H__
#define __GSSUP_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace GSSUP
{

const ::CSI::StringOID GSSUPMechOID = "oid:2.23.130.1.1.1";
struct InitialContextToken;
typedef TVarVar< InitialContextToken > InitialContextToken_var;
typedef TVarOut< InitialContextToken > InitialContextToken_out;


struct InitialContextToken {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef InitialContextToken_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InitialContextToken();
  ~InitialContextToken();
  InitialContextToken( const InitialContextToken& s );
  InitialContextToken& operator=( const InitialContextToken& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ::CSI::UTF8String username;
  ::CSI::UTF8String password;
  ::CSI::GSS_NT_ExportedName target_name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InitialContextToken;

typedef CORBA::ULong ErrorCode;
typedef ErrorCode& ErrorCode_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ErrorCode;

struct ErrorToken;
typedef TFixVar< ErrorToken > ErrorToken_var;
typedef ErrorToken& ErrorToken_out;


struct ErrorToken {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ErrorToken_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ErrorToken();
  ~ErrorToken();
  ErrorToken( const ErrorToken& s );
  ErrorToken& operator=( const ErrorToken& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ErrorCode error_code;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ErrorToken;

const ErrorCode GSS_UP_S_G_UNSPECIFIED = 1;
const ErrorCode GSS_UP_S_G_NOUSER = 2;
const ErrorCode GSS_UP_S_G_BAD_PASSWORD = 3;
const ErrorCode GSS_UP_S_G_BAD_TARGET = 4;
}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::GSSUP::InitialContextToken &_s );
void operator<<=( CORBA::Any &_a, ::GSSUP::InitialContextToken *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::GSSUP::InitialContextToken &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::GSSUP::InitialContextToken *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_GSSUP_InitialContextToken;

void operator<<=( CORBA::Any &_a, const ::GSSUP::ErrorToken &_s );
void operator<<=( CORBA::Any &_a, ::GSSUP::ErrorToken *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::GSSUP::ErrorToken &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::GSSUP::ErrorToken *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_GSSUP_ErrorToken;

#endif
