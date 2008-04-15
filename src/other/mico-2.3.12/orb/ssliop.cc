/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <mico/ssliop.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------
#ifdef HAVE_EXPLICIT_STRUCT_OPS
SSLIOP::SSL::SSL()
{
}

SSLIOP::SSL::SSL( const SSL& _s )
{
  target_supports = ((SSL&)_s).target_supports;
  target_requires = ((SSL&)_s).target_requires;
  port = ((SSL&)_s).port;
}

SSLIOP::SSL::~SSL()
{
}

SSLIOP::SSL&
SSLIOP::SSL::operator=( const SSL& _s )
{
  target_supports = ((SSL&)_s).target_supports;
  target_requires = ((SSL&)_s).target_requires;
  port = ((SSL&)_s).port;
  return *this;
}
#endif

struct __tc_init_SSLIOP {
  __tc_init_SSLIOP()
  {
  }

  ~__tc_init_SSLIOP()
  {
  }
};

static __tc_init_SSLIOP __init_SSLIOP;

