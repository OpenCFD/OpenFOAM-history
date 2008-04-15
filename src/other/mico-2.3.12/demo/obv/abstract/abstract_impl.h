#include "abstract.h"

#ifndef __ABSTRACT_IMPL_H__
#define __ABSTRACT_IMPL_H__

class Currency_impl :
  virtual public OBV_Currency,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  Currency_impl ()
  {
  }

  ~Currency_impl ()
  {
  }

  char * get_description ()
  {
    return CORBA::string_dup ("Currency");
  }
};

class Currency_Factory : virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new Currency_impl;
  }
};

#endif

