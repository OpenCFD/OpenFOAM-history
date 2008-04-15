// Mico client that receives some true random numbers from random.org's corba-server
// (see http://random.org/).

// To install the demo copy client.cc, random.idl and Makefile to mico/demo/random
// and compile (make).

// Then copy the current IOR of the server from the webpage http://random.org/corba.html
// to a file "called random.ior" (the ior in the file random.ior you got may be
// out-of-date) and start the client by entering "client".

// Sample output:

// /home/private/staff/frank/corba/source/random >client
// Trying to make an object from:
// IOR:000000000000000f49444c3a52616e646f6d3a312e3000000000000100000000000000500001000000000016706c616e7874792e6473672e63732e7463642e69650006220000002c3a5c706c616e7874792e6473672e63732e7463642e69653a52616e646f6d3a303a3a49523a52616e646f6d00
// Here are some true random numbers: 
// 1745298653
// 210788698
// 1325523403
// 1791383982
// 1554167097
// 913108180
// 222588291
// 10202268
// 1746275756
// 347304634
// /home/private/staff/frank/corba/source/random >

// See mico/demo/interop/orbix for a similar demo.

#include <CORBA-SMALL.h>
#include "random.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif


using namespace std;

int main( int argc, char *argv[] )
{
    unsigned long r;

    // ORB initialization
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

    CORBA::String_var ref;

    // random.org's current IOR must be in the file random.ior
    ifstream istr( "random.ior" );
    if( !istr ) 
    {
        cerr << "error: file random.ior not readable" << endl;
        exit( 1 );
    }
    char buf[ 1024 ];
    istr >> buf;
    ref = (const char *) buf;
  
    cout << "Trying to make an object from:\n" << buf << endl;

    CORBA::Object_var obj = orb->string_to_object( ref );
    assert( !CORBA::is_nil( obj ) );
  
    Random_var client = Random::_narrow( obj );
    assert( !CORBA::is_nil( client ) );

    // perform some remote methodinvocations
    cout << "Here are some true random numbers: " << endl;

    for( short i=0; i<10; i++ )
    {
        r = client->lrand48();
        cout << r << endl;
    }

    return 0;
}
