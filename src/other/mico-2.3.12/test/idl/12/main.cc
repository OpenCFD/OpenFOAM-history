#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "objref.h"
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
  void hello( const char * s )
  {
    cout << "Hello " << s << endl;
  };
};


class bar_impl : virtual public POA_bar
{
public:
  bar_impl()
  {
  };
  foo_ptr run( foo_ptr obj )
  {
    obj->hello( "Sima" );
    return foo::_duplicate( obj );
  };
  void run1( ObjectAlias_ptr obj )
  {
    foo_var f = foo::_narrow( obj );
    assert( !CORBA::is_nil( f ) );
    f->hello( "Sima 2" );
  };
  void run2( const S& s )
  {
    cout << s.x << endl;
    s.obj->hello( "World" );
  };
  void run3( const fooSeq& seq )
  {
    for( int i = 0; i < seq.length(); i++ ) {
      cout << "[" << i << "]" << endl;
      seq[ i ]->hello( "Berkeley" );
    }
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
  bar_impl* bar = new bar_impl;
  bar_ptr bar_obj = bar->_this();
  
  CORBA::String_var foo_ref = orb->object_to_string( foo_obj );
  CORBA::String_var bar_ref = orb->object_to_string( bar_obj );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( foo_ref ) );
#else
  obj = orb->string_to_object( foo_ref );
#endif
  foo_var foo_client = foo::_narrow( obj );

#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( bar_ref ) );
#else
  obj = orb->string_to_object( bar_ref );
#endif
  bar_var bar_client = bar::_narrow( obj );

  foo_var res = bar_client->run( foo_client );
  res->hello( "Test" );

  bar_client->run1( foo_client );
  
  S s;
  s.x = 42;
  s.obj = foo_client;
  bar_client->run2( s );

  fooSeq seq;
  seq.length( 2 );
  seq[ 0 ] = foo_client;
  seq[ 1 ] = foo_client;
  bar_client->run3( seq );
  CORBA::release( foo_obj );
  CORBA::release( bar_obj );
}
