#include "inherit.h"

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Tags to find the appropriate server
   */

  CORBA::ORB::ObjectTag_var t1 = CORBA::ORB::string_to_tag ("Base_impl");
  CORBA::ORB::ObjectTag_var t2 = CORBA::ORB::string_to_tag ("Derived_impl_1");
  CORBA::ORB::ObjectTag_var t3 = CORBA::ORB::string_to_tag ("Derived_impl_2");
  CORBA::ORB::ObjectTag_var t4 = CORBA::ORB::string_to_tag ("Base_Tie_impl");
  CORBA::ORB::ObjectTag_var t5 = CORBA::ORB::string_to_tag ("Derived_Tie_impl_1");
  CORBA::ORB::ObjectTag_var t6 = CORBA::ORB::string_to_tag ("Derived_Tie_impl_2");

  /*
   * Bind to the all of the objects
   */

  CORBA::Object_var o1, o2, o3, o4, o5, o6;
  o1 = orb->bind ("IDL:Base:1.0", t1);
  o2 = orb->bind ("IDL:Derived:1.0", t2);
  o3 = orb->bind ("IDL:Derived:1.0", t3);
  o4 = orb->bind ("IDL:Base:1.0", t4);
  o5 = orb->bind ("IDL:Derived:1.0", t5);
  o6 = orb->bind ("IDL:Derived:1.0", t6);

  if (CORBA::is_nil (o1)) {
    fprintf (stderr, "couldn't bind to server\n");
    exit (1);
  }

  assert (!CORBA::is_nil (o2) && !CORBA::is_nil (o3) && !CORBA::is_nil (o4));

  Base_var b1, b2;
  Derived_var d1, d2, d3, d4;

  b1 = Base::_narrow (o1);
  b2 = Base::_narrow (o4);
  d1 = Derived::_narrow (o2);
  d2 = Derived::_narrow (o3);
  d3 = Derived::_narrow (o5);
  d4 = Derived::_narrow (o6);

  assert (!CORBA::is_nil (b1) && !CORBA::is_nil (b2) &&
	  !CORBA::is_nil (d1) && !CORBA::is_nil (d2) &&
	  !CORBA::is_nil (d3) && !CORBA::is_nil (d4));

  /*
   * Invoke BaseOps
   */

  b1->BaseOp (); // BaseOp (inheritance based)
  b2->BaseOp (); // BaseOp (tie based)
  d1->BaseOp (); // BaseOp (inheritance based)
  d2->BaseOp (); // BaseOp in derived interface (inheritance based)
  d3->BaseOp (); // BaseOp (tie based)
  d4->BaseOp (); // BaseOp in derived interface (tie based)

  /*
   * Invoke DerivedOps
   */

  d1->DerivedOp (); // DerivedOp (inheritance based, impl inheritance)
  d2->DerivedOp (); // DerivedOp (inheritance based)
  d3->DerivedOp (); // DerivedOp (tie based, impl inheritance)
  d4->DerivedOp (); // DerivedOp (tie based)

  return 0;
}
