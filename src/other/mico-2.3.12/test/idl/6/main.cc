#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "struct.h"
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
  void bar( const S& s1, S &s2 )
  {
    cout << s1.a << endl;
    cout << s1.b << endl;
    cout << s1.c << endl << endl;
    s2 = s1;
  };
  void bar2( const S2& s )
  {
    cout << s.a << endl;
    cout << s.x.a << endl;
    cout << s.x.b << endl;
    cout << s.x.c << endl << endl;
  };
  S2 bar3()
  {
    S2 s;
    s.x.a = 42;
    s.x.b = 13;
    s.x.c = 'P';
    s.a = 42;
    return s;
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

  S st1, st2;

  st1.a = 47;
  st1.b = 11;
  st1.c = 'S';
  client->bar( st1, st2 );
  cout << st2.a << endl;
  cout << st2.b << endl;
  cout << st2.c << endl << endl;

  S2 s;

  s.a = 4711;
  s.x = st1;
  client->bar2( s );

  S2 s2 = client->bar3();
  cout << s2.a << endl;
  cout << s2.x.c << endl;
  CORBA::release( fooref );
}
