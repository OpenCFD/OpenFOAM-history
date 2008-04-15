#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "array.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

void fill_SS( SS ss )
{
  char buf[ 2 ];
  buf[ 1 ] = '\0';
  
  for( int i = 0; i < 5; i++ ) {
    for( int j = 0; j < 4; j++ ) {
      buf[ 0 ] = 'A' + i * j;
      ss[ i ][ j ] = CORBA::string_dup( buf );
    }
  }
}

void print_SS( const SS ss )
{
  for( int i = 0; i < 5; i++ )
    for( int j = 0; j < 4; j++ )
      cout << "ss[" << i << "][ " << j << " ] = " << ss[ i ][ j ].in() << endl;
  cout << "---------------------" << endl;
}


void fill_long_arr( long_arr a )
{
  for( int i = 0; i < 10; i++ )
    a[ i ] = i;
}

void print_long_arr( long_arr a )
{
  for( int i = 0; i < 10; i++ )
    cout << "long_arr[ " << i << " ] = " << a[ i ] << endl;
  cout << "---------------------" << endl;
}


class foo_impl : virtual public POA_foo
{
public:
  foo_impl()
  {
  };
  SS_slice* bar( const SS ss1, SS ss2, SS_out ss3, long_arr_out arr )
  {
    print_SS( ss1 );
    print_SS( ss2 );
    ss2[ 0 ][ 0 ] = (const char*) "Hello";
    ss3 = SS_alloc();
    fill_SS( ss3 );
    fill_long_arr( arr );
    SS_slice *res = SS_alloc();
    fill_SS( res );
    return res;
  };
  // fixed size arrays as return values are handled as if they were var length!
  long_arr_slice* baz ()
  {
    long_arr_slice *ret = long_arr_alloc();
    fill_long_arr (ret);
    return ret;
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

  SS ss1;
  fill_SS( ss1 );
  SS ss2;
  fill_SS( ss2 );
  SS_slice* ss3;
  long_arr arr;
  SS_slice *res = foo_client->bar( ss1, ss2, ss3, arr );
  cout << "Result: " << ss2[0][0].in() << endl;
  print_SS( ss3 );
  SS_free( ss3 );
  print_SS( res );
  SS_free( res );
  print_long_arr( arr );

  long_arr_slice *la = foo_client->baz();
  print_long_arr (la);
  long_arr_free (la);

  CORBA::Any a;
  a <<= SS_forany( ss1 );

  SS_forany ss_forany;
  CORBA::Boolean r = ( a >>= ss_forany );
  assert (r);

  print_SS( ss_forany );

  CORBA::release( foo_obj );

  // Little test for SequenceArrayTmpl<>
  arrseq x, y;
  x.length( 2 );
  
  x[ 0 ][ 0 ][ 0 ] = 100;
  x[ 0 ][ 0 ][ 1 ] = 200;
  x[ 1 ][ 0 ][ 0 ] = 300;
  x[ 1 ][ 0 ][ 1 ] = 400;
  assert( x.length() == 2 );
  cout << x[ 0 ][ 0 ][ 0 ] << endl;
  cout << x[ 0 ][ 0 ][ 1 ] << endl;
  cout << x[ 1 ][ 0 ][ 0 ] << endl;
  cout << x[ 1 ][ 0 ][ 1 ] << endl;

  y = x;
  assert( y.length() == 2 );
  cout << y[ 0 ][ 0 ][ 0 ] << endl;
  cout << y[ 0 ][ 0 ][ 1 ] << endl;
  cout << y[ 1 ][ 0 ][ 0 ] << endl;
  cout << y[ 1 ][ 0 ][ 1 ] << endl;

  assert( x == y );
  y[ 1 ][ 0 ][ 1 ] = 11;
  assert( !(x == y) );
}

