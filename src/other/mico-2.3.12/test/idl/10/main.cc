#define FORCE_MARSHALLING
#define MICO_CONF_POA
// These #includes need to be done manually if
// MICO_NO_TOPLEVEL_MODULES is defined
#include <CORBA-SMALL.h>
#include <mico/template_impl.h>

#include "module.h"

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class foo_impl : virtual public POA_M::foo
{
public:
  foo_impl()
  {
  };
  void bar( M::foo::E e )
  {
    cout << e << endl << endl;
  };
};


class foo2_impl : virtual public POA_M2::foo
{
public:
  foo2_impl()
  {
  };
  void bar( M::foo::E e )
  {
    cout << e << endl << endl;
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
  foo_impl* server = new foo_impl;
  foo2_impl* server2 = new foo2_impl;
  M::foo_ptr fooref = server->_this();
  M2::foo_ptr foo2ref = server2->_this();
  
  CORBA::String_var ref = orb->object_to_string( fooref );
  CORBA::String_var ref2 = orb->object_to_string( foo2ref );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( ref ) );
#else
  obj = orb->string_to_object( ref );
#endif
  M::foo_var client = M::foo::_narrow( obj );

#ifdef FORCE_MARSHALLING
  CORBA::Object_var obj2 = new CORBA::Object( new CORBA::IOR( ref2 ) );
#else
  CORBA::Object_var obj2 = orb->string_to_object( ref2 );
#endif
  M2::foo_var client2 = M2::foo::_narrow( obj2 );

  client->bar( M::foo::B );
  client2->bar( M::foo::C );
  CORBA::release( fooref );
  CORBA::release( foo2ref );
}
