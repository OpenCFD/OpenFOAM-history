#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "oneway.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class foo_impl : virtual public POA_foo
{
public:
  foo_impl()
  {
  };
  void bar( CORBA::Short x )
  {
    cout << "In bar(): x = " << x << endl;
  };
};



int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();
  mgr->activate();

  // server side
  foo_impl* foo = new foo_impl;
  foo_ptr server = foo->_this();
  CORBA::String_var ref = orb->object_to_string( server );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( ref ) );
#else
  obj = orb->string_to_object( ref );
#endif
  foo_var client = foo::_narrow( obj );

  client->bar( 42 );
  CORBA::release( server );
}
