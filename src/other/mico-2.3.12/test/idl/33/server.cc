
#include "val_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

class Test_impl
    : virtual public POA_Test
{
    Val*
    op(Val* p1, Val*& p2, Val_out p3)
    {
	Val_impl* res = new Val_impl;
	res->num(10);
	assert(p1->num() == 1);
	assert(p2->num() == 2);
	p2->num(3);
	p3 = new Val_impl;
	p3->num(5);
	return res;
    }
};

using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  ValFactory_impl* fact = new ValFactory_impl;
  orb->register_value_factory("IDL:Val:1.0", fact);

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  Test_impl* impl = new Test_impl;
  PortableServer::ObjectId_var oid = poa->activate_object (impl);

  ofstream of ("test.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  poa->destroy (TRUE, TRUE);
  delete impl;

  return 0;
}
