/*
 * A simple "Hello World" example that uses the POA
 */

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

/*
 * Hello World implementation. This implementation does not inherit
 * any skeleton class, but is tied to the POA skeleton template.
 */

class HelloWorld_impl
{
public:
  void hello ();
};

void
HelloWorld_impl::hello ()
{
  cout << "Hello World" << endl;
}

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * Create a Hello World object and tie it to the skeleton template
   */

  HelloWorld_impl hello;
  POA_HelloWorld_tie<HelloWorld_impl> tie_hello (hello);

  /*
   * Activate the Servant
   */

  PortableServer::ObjectId_var oid = poa->activate_object (&tie_hello);

  /*
   * Write reference to file
   */

  ofstream of ("hello.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  /*
   * Activate the POA and start serving requests
   */

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);

  return 0;
}
