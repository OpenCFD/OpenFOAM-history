
#include "val_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

#define FORCE_MARSHALING
#define WAY1

using namespace std;

class Test_impl
    : virtual public POA_Test
{
    Val*
    op(Val* p1)
    {
	assert(p1->num() == 1);
	p1->num(2);
#ifdef WAY1
	// 1.
	Val* res = dynamic_cast<Val*>(p1->_copy_value());
#endif // WAY1
#ifdef WAY2
	// 2.
	Val* res = new Val_impl(p1->num());
#endif // WAY2
#ifdef WAY3
	// 3.
	CORBA::add_ref(p1);
	Val* res = p1; 
#endif // WAY3
	return res;
    }
};

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

  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  cout << "Running." << endl;
  mgr->activate ();

#ifndef FORCE_MARSHALING
  CORBA::Object_var obj = orb->string_to_object (str);
#else // FORCE_MARSHALING
  CORBA::Object_var obj = new CORBA::Object(new CORBA::IOR(str));
#endif // FORCE_MARSHALING

  Test_var tt = Test::_narrow (obj);

  if (CORBA::is_nil (tt)) {
    cout << "oops: could not locate test server" << endl;
    exit (1);
  }
  Val* p1 = new Val_impl;
#ifndef FORCE_MARSHALING
  const long long max = 10000000;
#else // FORCE_MARSHALING
  const long long max = 100000;
#endif // FORCE_MARSHALING
  for (long long i = 0; i < max; i++) {
      p1->num(1);
      Val_var res = tt->op(p1);
      assert(res->num() == 2);
//        CORBA::remove_ref(res);
  }
  return 0;
}
