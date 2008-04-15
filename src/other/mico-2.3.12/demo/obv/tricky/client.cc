/*
 * Client
 */

#include "tricky.h"
#include "tricky_impl.h"


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  CORBA::Object_var obj = orb->bind ("IDL:iface:1.0");

  if (CORBA::is_nil (obj)) {
    cout << "oops: bind to iface failed" << endl;
    exit (1);
  }

  iface_var iface = iface::_narrow (obj);
  assert (!CORBA::is_nil (iface));

  vt_var v = new vt_impl;
  v->x (1);

  derived_var d = new derived_impl;
  d->y (2);
  d->z (v);

  node_var n = new node_impl;
  n->next (n);

  iface->skipped_value (d, v);
  iface->shared_parameter (v, v);
  iface->cyclic (n);

  return 0;
}
