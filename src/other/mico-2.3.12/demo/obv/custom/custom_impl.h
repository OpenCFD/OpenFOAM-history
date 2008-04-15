#ifndef __CUSTOM_IMPL_H__
#define __CUSTOM_IMPL_H__

#include "custom.h"

class CustomValue_impl :
  virtual public CustomValue,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  CORBA::Long value ();

  void marshal (CORBA::DataOutputStream *os);
  void unmarshal (CORBA::DataInputStream *is);
};

class CustomValue_Factory : virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new CustomValue_impl;
  }
};

#endif
