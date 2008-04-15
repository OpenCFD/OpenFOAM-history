/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __SERVICE_INFO_H__
#define __SERVICE_INFO_H__


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

typedef CORBA::UShort ServiceType;
typedef ServiceType& ServiceType_out;
typedef CORBA::ULong ServiceOption;
typedef ServiceOption& ServiceOption_out;
typedef CORBA::ULong ServiceDetailType;
typedef ServiceDetailType& ServiceDetailType_out;
const ServiceType Security = 1;
struct ServiceDetail;
typedef TVarVar< ServiceDetail > ServiceDetail_var;
typedef TVarOut< ServiceDetail > ServiceDetail_out;


struct ServiceDetail {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ServiceDetail_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ServiceDetail();
  ~ServiceDetail();
  ServiceDetail( const ServiceDetail& s );
  ServiceDetail& operator=( const ServiceDetail& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ServiceDetailType service_detail_type;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _service_detail_seq;
  _service_detail_seq service_detail;
};

struct ServiceInformation;
typedef TVarVar< ServiceInformation > ServiceInformation_var;
typedef TVarOut< ServiceInformation > ServiceInformation_out;


struct ServiceInformation {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ServiceInformation_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ServiceInformation();
  ~ServiceInformation();
  ServiceInformation( const ServiceInformation& s );
  ServiceInformation& operator=( const ServiceInformation& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  typedef SequenceTmpl< ServiceOption,MICO_TID_DEF> _service_options_seq;
  _service_options_seq service_options;
  typedef SequenceTmpl< ServiceDetail,MICO_TID_DEF> _service_details_seq;
  _service_details_seq service_details;
};

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
