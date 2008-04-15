#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "attribute.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class foo_impl : virtual public POA_foo
{
private:
  CORBA::Long a1;
  CORBA::Char a2;
  
public:
  foo_impl()
  {
    a1 = 4711;
  };
  CORBA::Long attr1()
  {
    return a1;
  };
  CORBA::Char attr2()
  {
    return a2;
  }
  void attr2( CORBA::Char c )
  {
    a2 = c;
  }
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
  foo_ptr foo_obj = foo->_this();
  
  CORBA::String_var foo_ref = orb->object_to_string( foo_obj );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( foo_ref ) );
#else
  obj = orb->string_to_object( foo_ref );
#endif
  foo_var foo_client = foo::_narrow( obj );

  cout << foo_client->attr1() << endl;
  foo_client->attr2( 'C' );
  cout << foo_client->attr2() << endl;
  CORBA::release( foo_obj );
}
