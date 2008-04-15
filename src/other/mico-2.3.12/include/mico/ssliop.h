/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>

#ifndef __SSLIOP_H__
#define __SSLIOP_H__


#ifdef _WIN32
// If you compile this file and would like to link
// it into the DLL, then please define ``BUILD_MICO_DLL'' symbol
// on your C++ compiler command-line.
// Depending on your compiler, you can use either
// ``-DBUILD_MICO_DLL'' or ``/DBUILD_MICO_DLL'' parameter
#ifdef BUILD_MICO_DLL
// Please do not forget to use gendef tool to correctly
// export all symbols from the resulting object file.
// If you would like to use more common __declspec(dllexport) way,
// then please use --windows-dll-with-export <name>
// IDL compiler parameter
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
// all other platforms
#define MICO_EXPORT /**/
#endif // _WIN32



#include <mico/current.h>
#include <mico/service_info.h>
#include <mico/ir_base.h>
#include <mico/ir.h>
#include <mico/policy.h>
#include <mico/timebase.h>
#include <mico/security/csi_base.h>
#include <mico/security/security.h>



namespace SSLIOP
{

struct SSL;
typedef TFixVar< SSL > SSL_var;
typedef SSL& SSL_out;


struct SSL {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SSL_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SSL();
  ~SSL();
  SSL( const SSL& s );
  SSL& operator=( const SSL& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ::Security::AssociationOptions target_supports;
  ::Security::AssociationOptions target_requires;
  CORBA::UShort port;
};

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
