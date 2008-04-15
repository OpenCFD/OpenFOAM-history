#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "pseudo.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

void show_any( const CORBA::Any* a )
{
  S1 s1;
  S2 s2;
  
  if( *a >>= s1 ) {
    cout << "Found struct S1" << endl;
    cout << s1.x << endl;
    cout << s1.c << endl;
  }
  if( *a >>= s2 ) {
    cout << "Found struct S2" << endl;
    cout << s2.str.in() << endl;
  }
  cout << "--------------------" << endl;
}


class foo_impl : virtual public POA_foo
{
public:
  foo_impl()
  {
  };
  CORBA::Any* bar( const CORBA::Any& a1, CORBA::Any_out a2, CORBA::Any& a3 )
  {
    show_any( &a1 );
    show_any( &a3 );
    a2 = new CORBA::Any;
    *a2 = a1;
    S1 s1;
    s1.x = 54321;
    s1.c = 'F';
    a3 <<= s1;
    CORBA::Any* result = new CORBA::Any;
    *result = a1;
    return result;
  };
  CORBA::TypeCode_ptr bar1( CORBA::TypeCode_ptr tc1, CORBA::TypeCode_out tc2,
                            CORBA::TypeCode_ptr& tc3 )
  {
    cout << "TCKind(0)=" << tc1->kind() << endl;
    cout << "TCKind(1)=" << tc3->kind() << endl;
    tc2 = new CORBA::TypeCode;
    *tc2 = *tc1;
    CORBA::release( tc3 );
    tc3 = CORBA::_tc_short;
    return CORBA::_tc_long;
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

  CORBA::Any a, a3;
  CORBA::Any_var a2;
  CORBA::Any_var result;
  
  S2 s2;
  s2.str = (const char *) "Hello";
  a <<= s2;
  a3 <<= s2;
  result = foo_client->bar( a, a2, a3 );
  show_any( a2 );
  show_any( &a3 );
  show_any( result );
  
  S1 s1;
  s1.x = 42;
  s1.c = 'C';
  a <<= s1;
  a3 <<= a3;
  result = foo_client->bar( a, a2, a3 );
  show_any( a2 );
  show_any( &a3 );
  show_any( result );

  CORBA::TypeCode_var tc1, tc2, tc3;
  tc3 = CORBA::_tc_octet;
  tc1 = foo_client->bar1( CORBA::_tc_char, tc2, tc3.inout() );
  cout << "TCKind(2)=" << tc1->kind() << endl;
  cout << "TCKind(3)=" << tc2->kind() << endl;
  cout << "TCKind(4)=" << tc3->kind() << endl;
  
  CORBA::release( foo_obj );
}
