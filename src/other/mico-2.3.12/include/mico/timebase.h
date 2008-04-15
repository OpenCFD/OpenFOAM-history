/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace TimeBase
{

typedef CORBA::ULongLong TimeT;
typedef TimeT& TimeT_out;
typedef TimeT InaccuracyT;
typedef InaccuracyT& InaccuracyT_out;
typedef CORBA::Short TdfT;
typedef TdfT& TdfT_out;
struct UtcT;
typedef TFixVar< UtcT > UtcT_var;
typedef UtcT& UtcT_out;


struct UtcT {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef UtcT_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UtcT();
  ~UtcT();
  UtcT( const UtcT& s );
  UtcT& operator=( const UtcT& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  TimeT time;
  CORBA::ULong inacclo;
  CORBA::UShort inacchi;
  TdfT tdf;
};

struct IntervalT;
typedef TFixVar< IntervalT > IntervalT_var;
typedef IntervalT& IntervalT_out;


struct IntervalT {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef IntervalT_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  IntervalT();
  ~IntervalT();
  IntervalT( const IntervalT& s );
  IntervalT& operator=( const IntervalT& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  TimeT lower_bound;
  TimeT upper_bound;
};

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_TimeBase_UtcT;

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_TimeBase_IntervalT;

#endif
