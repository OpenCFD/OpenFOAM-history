
/*
 * Implementations
 */

#include "tricky.h"
#include "tricky_impl.h"

CORBA::ValueBase *
vt_factory::create_for_unmarshal ()
{
  return new vt_impl;
}

CORBA::ValueBase *
base_factory::create_for_unmarshal ()
{
  return new base_impl;
}

CORBA::ValueBase *
derived_factory::create_for_unmarshal ()
{
  return new derived_impl;
}

CORBA::ValueBase *
node_factory::create_for_unmarshal ()
{
  return new node_impl;
}
