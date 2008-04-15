/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __TCKIND_H__
#define __TCKIND_H__


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

enum TCKind {
  tk_null = 0,
  tk_void,
  tk_short,
  tk_long,
  tk_ushort,
  tk_ulong,
  tk_float,
  tk_double,
  tk_boolean,
  tk_char,
  tk_octet,
  tk_any,
  tk_TypeCode,
  tk_Principal,
  tk_objref,
  tk_struct,
  tk_union,
  tk_enum,
  tk_string,
  tk_sequence,
  tk_array,
  tk_alias,
  tk_except,
  tk_longlong,
  tk_ulonglong,
  tk_longdouble,
  tk_wchar,
  tk_wstring,
  tk_fixed,
  tk_value,
  tk_value_box,
  tk_native,
  tk_abstract_interface,
  tk_local_interface
};

typedef TCKind& TCKind_out;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
