#include <CORBA.h>
#include "simple_impl.h"

int main (int argc, char **argv);

int main (int argc, char **argv) {

   CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

   CORBA::Object_var o;

   o = orb->resolve_initial_references ("RootPOA");
   PortableServer::POA_var rootpoa = PortableServer::POA::_narrow (o);
   if (CORBA::is_nil (rootpoa))
      {
	cerr << "server: resolve_initial_references (): couldn't resolve RootPOA"
	     << endl;
	exit (-1);
      }
   
   PortableServer::POAManager_var mgr = rootpoa->the_POAManager ();

   simple_impl1 *s1 = new simple_impl1;
   
   rootpoa->activate_object (s1);
   
   simple_impl2 *s2 = new simple_impl2;
   
   rootpoa->activate_object (s2);
   
   cout << "server: activating POA manager" << endl;
   mgr->activate ();
   
   cout << "server: running" << endl;
   
   orb->run ();

   return 0;
}
