#include "tree.h"
#include "tree_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Register factories.
   */

  CORBA::ValueFactoryBase_var vf = new BinaryTree_Factory;
  orb->register_value_factory ("IDL:BinaryTree:1.0", vf);

  vf = new StringKey_Factory;
  orb->register_value_factory ("IDL:StringKey:1.0", vf);

  vf = new Name_Factory;
  orb->register_value_factory ("IDL:Name:1.0", vf);

  vf = new NameValue_Factory;
  orb->register_value_factory ("IDL:NameValue:1.0", vf);

  /*
   * Initialize POA
   */

  CORBA::Object_var po = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (po);
  PortableServer::POAManager_var mgr = poa->the_POAManager ();

  /*
   * Some local testing
   */

  BinaryTree_impl * t1 = new BinaryTree_impl;

  {
    /*
     * Insert some data
     */
    {
      {
	StringKey_var s1 = new StringKey_impl ("key1");
	NameValue_var nv1 = new NameValue_impl ("name1", "value1");
	t1->insert (s1.in(), nv1.in());
      }
      {
	StringKey_var s1 = new StringKey_impl ("key2");
	NameValue_var nv1 = new NameValue_impl ("name2", "value2");
	t1->insert (s1.in(), nv1.in());
      }
      {
	StringKey_var s1 = new StringKey_impl ("key0");
	NameValue_var nv1 = new NameValue_impl ("name0", "value0");
	t1->insert (s1.in(), nv1.in());
      }
    }

    /*
     * Try to find some data
     */

    {
      {
	StringKey_var s1 = new StringKey_impl ("key0");
	Node_var n = t1->find (s1.in());
	NameValue * nv = NameValue::_downcast (n);
	assert (nv != 0);
	cout << "found " << nv->name() << " " << nv->value() << endl;
      }
      {
	StringKey_var s1 = new StringKey_impl ("key42");
	Node_var n = t1->find (s1.in());
	assert (n.in() == 0);
      }
    }
  }

  /*
   * Register the Tree with the POA and activate it
   */

  Map_var map = t1->_this ();

  /*
   * Write reference to file
   */

  ofstream of ("Map.ref");
  CORBA::String_var str = orb->object_to_string (map.in());
  of << str.in() << endl;
  of.close ();

  mgr->activate ();
  orb->run ();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  CORBA::remove_ref (t1);
  return 0;
}
