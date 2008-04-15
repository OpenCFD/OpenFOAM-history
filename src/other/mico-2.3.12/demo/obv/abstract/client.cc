#include "abstract.h"
#include "abstract_impl.h"

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Register factories.
   */

  CORBA::ValueFactoryBase_var vf = new Currency_Factory;
  orb->register_value_factory ("IDL:Currency:1.0", vf);

  /*
   * Acquire Currency value
   */

  Currency_var cur = new Currency_impl;

  /*
   * Acquire Account reference
   */

  CORBA::Object_var obj = orb->bind ("IDL:Account:1.0");
  Account_var acc = Account::_narrow (obj);
  assert (!CORBA::is_nil (acc));

  /*
   * Display both of them
   */

  obj = orb->bind ("IDL:Example:1.0");
  Example_var ex = Example::_narrow (obj);
  assert (!CORBA::is_nil (ex));

  ex->display (cur);
  ex->display (acc);

  return 0;
}
