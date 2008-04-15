/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __ORB_EXCEPTS_H__
#define __ORB_EXCEPTS_H__


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

struct Bounds : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Bounds();
  ~Bounds();
  Bounds( const Bounds& s );
  Bounds& operator=( const Bounds& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  Bounds *operator->() { return this; }
  Bounds& operator*() { return *this; }
  operator Bounds*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static Bounds *_downcast( CORBA::Exception *ex );
  static const Bounds *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef Bounds Bounds_catch;
#else
typedef ExceptVar< Bounds > Bounds_var;
typedef TVarOut< Bounds > Bounds_out;
typedef Bounds_var Bounds_catch;
#endif // HAVE_STD_EH

struct WrongTransaction : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  WrongTransaction();
  ~WrongTransaction();
  WrongTransaction( const WrongTransaction& s );
  WrongTransaction& operator=( const WrongTransaction& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  WrongTransaction *operator->() { return this; }
  WrongTransaction& operator*() { return *this; }
  operator WrongTransaction*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static WrongTransaction *_downcast( CORBA::Exception *ex );
  static const WrongTransaction *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef WrongTransaction WrongTransaction_catch;
#else
typedef ExceptVar< WrongTransaction > WrongTransaction_var;
typedef TVarOut< WrongTransaction > WrongTransaction_out;
typedef WrongTransaction_var WrongTransaction_catch;
#endif // HAVE_STD_EH

struct ORB_InvalidName : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ORB_InvalidName();
  ~ORB_InvalidName();
  ORB_InvalidName( const ORB_InvalidName& s );
  ORB_InvalidName& operator=( const ORB_InvalidName& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  ORB_InvalidName *operator->() { return this; }
  ORB_InvalidName& operator*() { return *this; }
  operator ORB_InvalidName*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static ORB_InvalidName *_downcast( CORBA::Exception *ex );
  static const ORB_InvalidName *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef ORB_InvalidName ORB_InvalidName_catch;
#else
typedef ExceptVar< ORB_InvalidName > ORB_InvalidName_var;
typedef TVarOut< ORB_InvalidName > ORB_InvalidName_out;
typedef ORB_InvalidName_var ORB_InvalidName_catch;
#endif // HAVE_STD_EH

struct TypeCode_Bounds : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TypeCode_Bounds();
  ~TypeCode_Bounds();
  TypeCode_Bounds( const TypeCode_Bounds& s );
  TypeCode_Bounds& operator=( const TypeCode_Bounds& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  TypeCode_Bounds *operator->() { return this; }
  TypeCode_Bounds& operator*() { return *this; }
  operator TypeCode_Bounds*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static TypeCode_Bounds *_downcast( CORBA::Exception *ex );
  static const TypeCode_Bounds *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef TypeCode_Bounds TypeCode_Bounds_catch;
#else
typedef ExceptVar< TypeCode_Bounds > TypeCode_Bounds_var;
typedef TVarOut< TypeCode_Bounds > TypeCode_Bounds_out;
typedef TypeCode_Bounds_var TypeCode_Bounds_catch;
#endif // HAVE_STD_EH

struct TypeCode_BadKind : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TypeCode_BadKind();
  ~TypeCode_BadKind();
  TypeCode_BadKind( const TypeCode_BadKind& s );
  TypeCode_BadKind& operator=( const TypeCode_BadKind& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  TypeCode_BadKind *operator->() { return this; }
  TypeCode_BadKind& operator*() { return *this; }
  operator TypeCode_BadKind*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static TypeCode_BadKind *_downcast( CORBA::Exception *ex );
  static const TypeCode_BadKind *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef TypeCode_BadKind TypeCode_BadKind_catch;
#else
typedef ExceptVar< TypeCode_BadKind > TypeCode_BadKind_var;
typedef TVarOut< TypeCode_BadKind > TypeCode_BadKind_out;
typedef TypeCode_BadKind_var TypeCode_BadKind_catch;
#endif // HAVE_STD_EH

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
