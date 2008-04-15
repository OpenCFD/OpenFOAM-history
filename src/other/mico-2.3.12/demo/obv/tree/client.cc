#include "tree.h"
#include "tree_impl.h"


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Register factories.
   *
   * Note: The factory for NameValue is not registered. So when receiving
   * a NameValue from the server, it must be truncated to Name.
   */

  CORBA::ValueFactoryBase_var vf = new BinaryTree_Factory;
  orb->register_value_factory ("IDL:BinaryTree:1.0", vf);

  vf = new StringKey_Factory;
  orb->register_value_factory ("IDL:StringKey:1.0", vf);

  vf = new Name_Factory;
  orb->register_value_factory ("IDL:Name:1.0", vf);

  CORBA::Object_var obj = orb->bind ("IDL:Map:1.0");
  Map_ptr map = Map::_narrow (obj);
  assert (!CORBA::is_nil (map));

  StringKey_var s1 = new StringKey_impl ("key42");
  Name_var n1 = new Name_impl ("name42");
  map->insert (s1.in(), n1.in());

  s1 = new StringKey_impl ("key2");
  Node_var n = map->find (s1.in());
  Name * n2 = Name::_downcast (n);
  assert (n2 != 0);
  cout << "found " << n2->name() << endl;

  // narrowing to NameValue should fail: we don't have a factory for them
  NameValue * nv = NameValue::_downcast (n);
  assert (nv == 0);

  return 0;
}
