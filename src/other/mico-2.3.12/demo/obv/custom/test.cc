#include "custom.h"
#include "custom_impl.h"

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  CORBA::ValueFactoryBase_var vf = new CustomValue_Factory;
  orb->register_value_factory ("IDL:CustomValue:1.0", vf);

  CustomValue_var c1 = new CustomValue_impl;
  // should use custom marshalling
  CORBA::ValueBase_var c2 = c1->_copy_value ();

  return 0;
}
