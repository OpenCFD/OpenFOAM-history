#include <CORBA.h>
#include "simple.h"

PortableServer::Current_var current;

// Implementation for interface simple
class simple_impl : virtual public POA_simple
{
  public:

    void func( CORBA::Char id ) throw( ::CORBA::SystemException);
    void flush() throw( ::CORBA::SystemException);
};

// Implementation for interface simple

void
simple_impl::func( CORBA::Char id ) throw( ::CORBA::SystemException) {
  //  cout << "(implementation) is in method: simple_impl::func( " << id << " )" << endl;
  cout << "Current=" << current->get_object_id() << id << endl;
}

void
simple_impl::flush() throw( ::CORBA::SystemException) {

  cout << endl;
}

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
   
   o = orb->resolve_initial_references ("POACurrent");
   current = PortableServer::Current::_narrow (o);

   PortableServer::POAManager_var mgr = rootpoa->the_POAManager ();

   simple_impl *s = new simple_impl;
   
   rootpoa->activate_object (s);
   
   cout << "server: activating POA manager" << endl;
   mgr->activate ();
   
   cout << "server: running" << endl;
   
   orb->run ();

   return 0;
}
