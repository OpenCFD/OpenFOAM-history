/*
 * Server
 */

#include "tricky.h"
#include "tricky_impl.h"
#include <string.h>


using namespace std;

class iface_impl : virtual public POA_iface
{
public:
  void skipped_value (base *b, vt *v)
  {
    assert (v && v->x() == 1);
  }
  void shared_parameter (vt *v1, vt *v2)
  {
    assert (v1 == v2);
  }
  void cyclic (node *n)
  {
    assert (n && n->next() == n);
  }
};

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Register factories
   */

  vt_factory * vf = new vt_factory;
  orb->register_value_factory ("IDL:vt:1.0", vf);

  base_factory * bf = new base_factory;
  orb->register_value_factory ("IDL:base:1.0", bf);

  node_factory * nf = new node_factory;
  orb->register_value_factory ("IDL:node:1.0", nf);

  /*
   * Initialize POA, and activate our calendar server
   */

  CORBA::Object_var po = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (po);
  PortableServer::POAManager_var mgr = poa->the_POAManager ();

  PortableServer::ServantBase_var iface = new iface_impl;
  CORBA::Object_var obj = iface->_this ();

  cout << "Running." << endl;
  mgr->activate ();
  orb->run ();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  vf->_remove_ref ();
  bf->_remove_ref ();
  return 0;
}


