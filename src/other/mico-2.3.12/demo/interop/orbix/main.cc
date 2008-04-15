#include <iostream.h>
#include <fstream.h>
#include "grid.h"


int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::String_var ref;

  ifstream istr( "/tmp/gridiiop.ref" );
  if( !istr ) {
    cerr << "error: file /tmp/gridiiop.ref not readable" << endl;
    exit( 1 );
  }
  char buf[ 1024 ];
  istr >> buf;
  ref = (const char *) buf;
  
  CORBA::Object_var obj = orb->string_to_object( ref );
  assert( !CORBA::is_nil( obj ) );
  
  grid_var client = grid::_narrow( obj );
  assert( !CORBA::is_nil( client ) );
  
  for( int i = 0; i < client->height(); i++ )
    for( int j = 0; j < client->width(); j++ )
      client->set( i, j, i * j );
  
  for( int i = 0; i < client->height(); i++ )
    for( int j = 0; j < client->width(); j++ )
      cout << "[" << i << "," << j << "] = " << client->get( i, j ) << endl;
}
