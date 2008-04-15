
/*
 * Implementation for the date valuetype and its factory
 */

#ifndef __DATE_IMPL_H__
#define __DATE_IMPL_H__

#include "tricky.h"

class vt_impl :
  virtual public OBV_vt,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
};

class vt_factory : virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

class base_impl :
  virtual public OBV_base,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
};

class base_factory : virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

class derived_impl :
  virtual public OBV_derived,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
};

class derived_factory :  virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

class node_impl :
  virtual public OBV_node,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
};

class node_factory :  virtual public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

#endif
