
#include <CORBA.h>
#include <coss/CosNaming.h>
#include "hello.h"

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
  CosNaming::NamingContextExt_var nc =
    CosNaming::NamingContextExt::_narrow (obj);

  assert (!CORBA::is_nil (nc));

  obj = nc->resolve_str ("HelloHome");

  assert (!CORBA::is_nil (obj));

  HelloHome_var hh = HelloHome::_narrow (obj);
  HelloWorld_var hw = hh->create ();

  hw->message ("Hello World");
  hw->sayHello ();
  hw->remove ();

  return 0;
}
