#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "typedef.h"
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
  void bar1( const char* s )
  {
    cout << s << endl << endl;
  };
  void bar2( foo::L l )
  {
    cout << l << endl << endl;
  };
  void bar3( const foo::S& s )
  {
    cout << s.a << endl;
    cout << s.b << endl;
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
  foo_ptr fooref = server->_this();
  CORBA::String_var ref = orb->object_to_string( fooref );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( ref ) );
#else
  obj = orb->string_to_object( ref );
#endif
  foo_var client = foo::_narrow( obj );

  client->bar1( "Hallo" );
  client->bar2( 100 );

  foo::S fooS;
  fooS.a = 100;
  fooS.b = 'A';
  client->bar3( fooS );
  CORBA::release( fooref );
}
