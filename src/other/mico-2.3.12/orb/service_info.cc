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



#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ServiceDetail::ServiceDetail()
{
}

CORBA::ServiceDetail::ServiceDetail( const ServiceDetail& _s )
{
  service_detail_type = ((ServiceDetail&)_s).service_detail_type;
  service_detail = ((ServiceDetail&)_s).service_detail;
}

CORBA::ServiceDetail::~ServiceDetail()
{
}

CORBA::ServiceDetail&
CORBA::ServiceDetail::operator=( const ServiceDetail& _s )
{
  service_detail_type = ((ServiceDetail&)_s).service_detail_type;
  service_detail = ((ServiceDetail&)_s).service_detail;
  return *this;
}
#endif

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ServiceInformation::ServiceInformation()
{
}

CORBA::ServiceInformation::ServiceInformation( const ServiceInformation& _s )
{
  service_options = ((ServiceInformation&)_s).service_options;
  service_details = ((ServiceInformation&)_s).service_details;
}

CORBA::ServiceInformation::~ServiceInformation()
{
}

CORBA::ServiceInformation&
CORBA::ServiceInformation::operator=( const ServiceInformation& _s )
{
  service_options = ((ServiceInformation&)_s).service_options;
  service_details = ((ServiceInformation&)_s).service_details;
  return *this;
}
#endif

struct __tc_init_SERVICE_INFO {
  __tc_init_SERVICE_INFO()
  {
  }

  ~__tc_init_SERVICE_INFO()
  {
  }
};

static __tc_init_SERVICE_INFO __init_SERVICE_INFO;

